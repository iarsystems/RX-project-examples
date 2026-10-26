#include "platform.h"
#include "r_sci_async_rx_if.h"
#include "r_byteq_if.h"
#include "r_sci_async_rx_config.h"
#include "r_cgc_rx_if.h"
#include "r_lpc_rx_if.h"
#include "r_irq_rx_if.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cmd_parser.h"
#include "coremark.h"

/******************************************************************************
Macro definitions, local data
******************************************************************************/
#define PERIPH_INITIAL_STATE  0
#define PERIPH_ALL_ON         1
#define PERIPH_ALL_OFF        2
#define PERIPH_RESTORE        4
#define MAX_DIVISORS		  7

enum {HIGH_POWER, LOW_POWER, MEDIUM_POWER};
enum {FCLK, PCLKB, PCLKD};
uint16_t divisors[] = {1,2,4,8,16,32,64};


/* Command handlers */
static bool cmdVersion (void);
static bool cmdPowMode (void);
static bool cmdCpuMode (void);
static bool cmdClkSrc (void);
static bool cmdHelp (void);
static bool cmdMstp (void);
static bool cmdRTCDate (void);
static bool cmdRTCTime (void);
static bool cmdRTC (void);
static bool cmdClkDiv(void);
static bool cmdPLLDiv(void);
static bool cmdPLLMul(void);
static bool cmdgetFreqs(void);
static bool cmdClkRun(void);
static bool cmdAll(void);
static bool cmdClkOut (void);
static bool cmdrunCoremark(void);

const char *clock_names[] = {
   {"LOCO"},
   {"HOCO"},
   {"Main"},
   {"Sub-Clock"},
   {"PLL"},
   {"Unspecified"}
};


const HOST_COMMAND commands[] = {
   {"ver", cmdVersion,""},
   {"powmode", cmdPowMode,"::<hi | med | low>"},
   {"iclksrc", cmdClkSrc,"::<main | pll | hoco | loco | sub>"},
   {"clkout", cmdClkOut,"::<enabled | disabled>::<main | hoco | loco | sub>::<1 | 2 | 4 | 8 | 16>"},
   {"clkdiv", cmdClkDiv,"::<i | f | b | d>::<1 | 2 | 4 | 8 | 16 | 32 | 64>"},
   {"plldiv", cmdPLLDiv,"::<1 | 2 | 4>"},
   {"pllmul", cmdPLLMul,"::<6 | 8>"},
   {"clkrun", cmdClkRun,"<main | pll | hoco | loco | sub>::<run | stop>"},
   {"cpumode", cmdCpuMode,"::<sleep | dslp | stby>::<h | m | l>"},
   {"mstp", cmdMstp, "::<a | b | c>::<value in hex>"},
   {"rtcdate", cmdRTCDate, "::mm::dd::yy"},
   {"rtctime", cmdRTCTime, "::hh:mm:ss"},
   {"rtc", cmdRTC, "::<enabled | disabled>"},
   {"freqs", cmdgetFreqs, ""},
   {"core", cmdrunCoremark, ""},
   {"all", cmdAll, ""},
    {"?", cmdHelp,""}

};

char *cmdBuffer=NULL;
char *get_token (char *toParse, char *token, size_t tokenLen, char *breakChars) ;
char *cmdPtr=NULL;          // Pointer into command buffer
char cmd[10];
char arg1[10];
char arg2[10];
char arg3[10];
char arg4[10];
char *pArg1;
char *pArg2;
char *pArg3;
char *pArg4;


extern unsigned long mstpaMaster, mstpbMaster ;
extern sci_hdl_t   console;
extern sci_uart_t  config;
extern cgc_system_clock_config_t clock;
extern cgc_clock_config_t clock_config;
extern cgc_err_t clock_request_status;		                // A NAK'd command will check this byte.
extern cgc_clocks_t clock_source;
extern lpc_err_t powmode_request_status;		            // A NAK'd command will check this byte.

sci_baud_t  baudInfo;
unsigned long mstpaCopy, mstpbCopy;
static lpc_operating_mode_t current_powerMode = OP_HIGH_SPEED;
static uint32_t systemClkRate = BSP_SELECTED_CLOCK_HZ;
static bool clkOutputEnabled = false;
static cgc_clocks_t clockOutputSource = CGC_HOCO;
static cgc_clockout_dividers_t clockout_divider = CLOCKOUT_DIV_1;

void ModuleStopSet (int state);
void SetAlarm (int seconds);
static bool getSCKCR(uint16_t *clkvalue, char reqClk);
static bool EnterSleepSTMode (char slpType);
static uint8_t bcdToDec(uint8_t val);


/******************************************************************************
* Function Name: processCommand(char *rxbuffer)
* Description  : Parses a token from a string based on an array of delimiters
* Arguments    : char *rxbuffer - Pointer to buffer containing received command string
* Return Value : bool - true if command successfully parsed, false otherwise
******************************************************************************/
bool processCommand(char *rxbuffer)
{
uint32_t i;
bool retVal;

	cmdBuffer = rxbuffer;
    baudInfo.rate = config.baud_rate;		// Many commands need this
    clock_request_status = CGC_SUCCESS;		// Until we know otherwise

    cmd[0] = 0;
    pArg1 = &arg1[0];
    pArg2 = &arg2[0];
    pArg3 = &arg3[0];
    pArg4 = &arg4[0];
    cmdPtr = cmdBuffer;

	for (i = 0 ; i < (sizeof(commands) / sizeof(HOST_COMMAND)); i++)
	 {
	   if (strncmp(commands[i].command, (void *) cmdBuffer, strlen(commands[i].command)) == 0)
	   {
	     // Prepare our common pointers
		 cmdPtr = get_token(cmdPtr, cmd, 10, "::");
		 if (cmdPtr != NULL)
			 cmdPtr = get_token(cmdPtr, arg1, 10, "::");
		 else
			 pArg1 = NULL;

		 if (cmdPtr != NULL)
			 cmdPtr = get_token(cmdPtr, arg2, 10, "::");
		 else
			 pArg2 = NULL;

		 if (cmdPtr != NULL)
			 cmdPtr = get_token(cmdPtr, arg3, 10, "::");
		 else
			 pArg3 = NULL;

		 if (cmdPtr != NULL)
			 cmdPtr = get_token(cmdPtr, arg4, 10, "::");
		 else
			 pArg4 = NULL;

		  // If there is only a single ':' NAK the command
		  if ((strstr(cmdBuffer,"::") == NULL) && (strstr(cmdBuffer,":") != NULL))
			return(false);

		 retVal = (*commands[i].commandHandler)() ;           // Received data is a command, execute the command
		 return(retVal) ;
	   }
	 } // for (i...)
	return(false);
}

/******************************************************************************
* Function Name: get_token
* Description  : Returns the next (if any) token from the provided command
*                string. Returns a NULL pointer if the '\r' is detected.
* Arguments    : char *toParse - Pointer to string to parse
*              : char *token - Pointer to buffer for parsed token result
*              : size_t tokenLen - Max. length of token
*              : char *breakChars - Characters to look for to delineate a token
* Return Value : char * - Pointer to character after token in the source string,
*                         or NULL if end of command string reached without seeing
*                         a new token.
******************************************************************************/
char *get_token(char *toParse, char *token, size_t tokenLen, char *breakChars)
{
  char *tokenLim, *breakPtr;
  int breakcnt = 0;

  if (!*toParse)                            // Nothing to parse, return
    return NULL;

  tokenLim = token + tokenLen - 1;          // Maximum length of scanned token

  while (*toParse && (token < tokenLim) && (*toParse != '\r'))    // Go until no more chars to check or we have exceeded the token length
  {
    for (breakPtr=breakChars; *breakPtr; breakPtr++)  // Check each of the break characters
    {
      if (*toParse == *breakPtr)            // Found a break character in the source string
      {
        *token = 0;                         // Null terminate the token
        breakcnt++;
                                            // Bump the target string pointer past any other break chars
        for (++toParse, breakPtr=breakChars; *toParse && *breakPtr; ++breakPtr)
        {
          if (*toParse == *breakPtr)        // If this character is one of the break characters
          {
            ++toParse;                      // Skip to next character in target string
            breakPtr = breakChars;          // Start scanning of break character array from beginning
          }
        }
        return (char *)toParse;
      }
    }
    if (*toParse != '\r')
    		*token++ = *toParse++;                 // Copy character from source to token
  }
  *token = 0;
  if (breakcnt == 0)		// No delimitters at all, return NULL
	  return(NULL);
  return (char *)toParse;
}


/*****************************************************************************
* Function Name: cmdAll
* Description  : Dump all of the relevant settings
* Format       : all
* Arguments    : none
* Return Value : none
******************************************************************************/
static bool cmdAll (void)
{
	processCommand("ver\r\n");
	printf("\r\nFreqs:\r\n");
	processCommand("freqs\r\n");
	printf("\r\n");
	processCommand("powmode\r\n");
	processCommand("iclksrc\r\n");
	processCommand("clkrun\r\n");
	processCommand("pllmul\r\n");
	processCommand("plldiv\r\n");
	processCommand("clkdiv::i\r\n");
	processCommand("clkdiv::f\r\n");
	processCommand("clkdiv::b\r\n");
	processCommand("clkdiv::d\r\n");
	processCommand("clkout\r\n");

    return(true);
}
/*****************************************************************************
* Function Name: cmdVersion
* Description  : Retrieves the current version of this firmware and the
*                included FIT modules.
* Format       : ver
* Arguments    : none
* Return Value : none
******************************************************************************/
static bool cmdVersion (void)
{
    printf("Version 1.2 FIT\r\n");
    printf("SCI:    %x.%x\r\n", (R_SCI_GetVersion() >> 16),(R_SCI_GetVersion() & 0xff));
    printf("BYTEQ:  %x.%x\r\n", (R_BYTEQ_GetVersion() >> 16),(R_BYTEQ_GetVersion() & 0xff));
    printf("Clock:  %x.%x\r\n", (R_CGC_GetVersion() >> 16),(R_CGC_GetVersion() & 0xff));
    printf("LPC:    %x.%x\r\n", (R_LPC_GetVersion() >> 16),(R_LPC_GetVersion() & 0xff));
    printf("IRQ:    %x.%x\r\n", (R_IRQ_GetVersion() >> 16),(R_IRQ_GetVersion() & 0xff));
    return(true);
}

/*****************************************************************************
* Function Name: cmdgetFreqs
* Description  : Retrieves the frequencies of the clocks
* Format       : freqs
* Arguments    : none
* Return Value : none
******************************************************************************/
static bool cmdgetFreqs (void)
{
    printf("PClkB: %d\r\n",R_CGC_ClockHzGet(CGC_PCLKB));
    printf("PClkD: %d\r\n",R_CGC_ClockHzGet(CGC_PCLKD));
    printf("FCLK:  %d\r\n",R_CGC_ClockHzGet(CGC_FCLK));
    printf("ICLK:  %d\r\n",R_CGC_ClockHzGet(CGC_ICLK));
	if (R_CGC_ClockCheck(CGC_PLL) == CGC_ERR_CLOCK_INACTIVE)
	    printf("PLL:   Stopped\r\n");
	else
	{
		if (SYSTEM.PLLCR.BIT.STC == PLL_MUL_6)
		{
			printf("PLL:   %d\r\n",(BSP_CFG_XTAL_HZ>>SYSTEM.PLLCR.BIT.PLIDIV)*6);
		}
		else  if (SYSTEM.PLLCR.BIT.STC == PLL_MUL_8)
		{
			printf("PLL:   %d\r\n",(BSP_CFG_XTAL_HZ>>SYSTEM.PLLCR.BIT.PLIDIV)*8);
		}
	}

   return(true);
}

/*****************************************************************************
* Function Name: cmdPowMode
* Description  : Sets/retrieves the RX100 power mode based on the supplied parameter
* Format       : powmode::<hi|med|low>
* Return Value : true if command successfully parsed/implemented, false otherwise
******************************************************************************/
static bool cmdPowMode (void)
{
bool retVal = true;

	if (pArg1 != NULL)		// Did we find a "::" delimiter ?
	{
   	   retVal = setPowermode(*pArg1); 	// Execute the request
	}

	if (retVal == true)
	{
        if (current_powerMode == OP_HIGH_SPEED)
        {
    		printf("Power mode: high\r\n");
  	    }
        else
        if (current_powerMode == OP_MIDDLE_SPEED)
         {
     		printf("Power mode: medium\r\n");
   	    }
        else
         {
      		printf("Power mode: low\r\n");
    	 }
	}
	return(retVal);
}


/*****************************************************************************
* Function Name: cmdCpuMode
* Description  : Sets/retrieves the RX100 cpu mode based on the supplied parameter
* Format       : cpumode::<sleep | dslp | sstby>::<h | m | l>
* Arguments    : none
* Return Value : true if command successfully parsed/implemented, false otherwise
******************************************************************************/
static bool cmdCpuMode (void)
{
    bool retVal = false;
    char sltype;

	  // We don't implement a query, we can only respond if we're awake.
	  if (pArg1 != NULL)
	  {
		sltype = *pArg1;
		if (pArg1[1] == 't' && pArg1[0] == 's')
			sltype = 't';

	    // is there a powmode flag?
		if (pArg2 != NULL)
		{
			// Switch power mode before sleep
			setPowermode(*pArg2);		// Do the requested power mode
		}
	    switch(sltype)
	    {
	    	case 'd':
				printf("deep sleep...\r\n") ;
			break;

		    case 's':
				printf("sleep...\r\n") ;
		    break;

		    case 't':
				printf("standby...\r\n") ;
		    break;

		    default:
			  return(false);
	    }

      retVal = EnterSleepSTMode(sltype);
	  }
    return(retVal);
}

/*****************************************************************************
* Function Name: cmdClkSrc
* Description  : Gets/Sets the clock source
* Arguments    : none
* Format       : iclksrc::<main | pll | hoco | loco | sub>
* Return Value : true if command successfully parsed/implemented, false otherwise
******************************************************************************/
static bool cmdClkSrc (void)
{
	  bool retVal = true;
      volatile signed int i;
      cgc_clocks_t clkSrc;
	  uint16_t  clkdivisor;


	  if (pArg1 != NULL)
	    {
            baudInfo.rate = config.baud_rate;
 		  switch(*pArg1)
		  {
			  case 'm':
				    clkSrc = CGC_MAIN_OSC;
	                systemClkRate = BSP_CFG_XTAL_HZ;
			  break;

			  case 'p':
				    clkSrc = CGC_PLL;
	        		if (SYSTEM.PLLCR.BIT.STC == PLL_MUL_6)
	        		{
	        			systemClkRate = (uint32_t)((BSP_CFG_XTAL_HZ>>SYSTEM.PLLCR.BIT.PLIDIV)*6);
	        		}
	        		else  if (SYSTEM.PLLCR.BIT.STC == PLL_MUL_8)
	        		{
	        			systemClkRate = (uint32_t)((BSP_CFG_XTAL_HZ>>SYSTEM.PLLCR.BIT.PLIDIV)*8);
	        		}
			  break;

			  case 'h':
				    clkSrc = CGC_HOCO;
	                systemClkRate = BSP_HOCO_HZ;
			  break;

			  case 'l':
				    clkSrc = CGC_LOCO;
	                systemClkRate = BSP_LOCO_HZ;
			  break;

			  case 's':
				    clkSrc = CGC_SUBCLOCK;
	                systemClkRate = BSP_SUB_CLOCK_HZ;
			  break;

			  default:
				    return(false);
		  }

		  clock_request_status = R_CGC_SystemClockSet(clkSrc, &clock);
		  if (clock_request_status != CGC_SUCCESS)
			  return(false);
		  else
		  {
			  clock_source = clkSrc;		// update our global copy.
			  // Try and get the serial port up and running with the new clock.
	          if (getSCKCR(&clkdivisor, 'b') == true)
	           {
	        	baudInfo.pclk =  (systemClkRate / clkdivisor);
	        	R_SCI_Control(console, SCI_CMD_CHANGE_BAUD, (void *)&baudInfo);	// Set the baud rate using the new clock
	        	printf("\r\n");	// Insure baud rate change gets a new line
	           }
		  }
    }

  	  printf("Clock source: %s\r\n",clock_names[clock_source]);
  	  return(retVal);
}


/*****************************************************************************
* Function Name: cmdClkRun
* Description  : Starts/Stops the specified clock
* Arguments    : none
* Format       : clkrun::<main | pll | hoco | loco | sub>::<run | stop>
* Return Value : true if command successfully parsed/implemented, false otherwise
******************************************************************************/
static bool cmdClkRun (void)
{
    bool retVal = true;
    cgc_clocks_t count;
    cgc_clocks_t clock_source=CGC_MAIN_OSC;

	  if (pArg1 != NULL)
	  {
		switch(*pArg1)
		  {
			  case 'm':
				  clock_source = CGC_MAIN_OSC;
			  break;

			  case 'p':
				  clock_source = CGC_PLL;
			  break;

			  case 'h':
				  clock_source = CGC_HOCO;
			  break;

			  case 'l':
				  clock_source = CGC_LOCO;
			  break;

			  case 's':
				  clock_source = CGC_SUBCLOCK;
			  break;

			  default:
				    retVal = false;
			  break;
		  }

	    // is there a run/stop argument?
		if (pArg2 != NULL)
		{
			if (*pArg2 == 'r')
			{
				clock_request_status = R_CGC_ClockStart(clock_source, &clock_config);
			}
			else
			if (*pArg2 == 's')
			{
				clock_request_status = R_CGC_ClockStop(clock_source);
			}
			else
				return(false);

			// Show the state of the specified clock.
			if ((retVal == true) && (clock_request_status == CGC_SUCCESS))
			{
				printf("%s: ",clock_names[clock_source]);
				if (R_CGC_ClockCheck(clock_source) == CGC_ERR_CLOCK_INACTIVE)
					printf("Stopped\r\n");
				else
					printf("Running\r\n");
			}
			else
				retVal = false;
		 }
		else
			retVal = false;
	  }
	  else
	  {
		  // Show the run state of all the clocks
		  for (count=CGC_LOCO; count<=CGC_PLL; count++)
		  {
			// Show the state of the specified clock.
				printf("%s: ",clock_names[count]);
				if (R_CGC_ClockCheck(count) == CGC_ERR_CLOCK_INACTIVE)
					printf("Stopped\r\n");
				else
					printf("Running\r\n");
		  }
		  return(true);
	  }
    return(retVal);
}

/*****************************************************************************
* Function Name: cmdClkOut
* Description  : Enables/Disables the specified clock on the clkout pin
* Arguments    : none
* Format       : clkout::<enable | disable>::<main | hoco | loco | sub>::<1 | 2 | 4 | 8 | 16>
* Return Value : true if command successfully parsed/implemented, false otherwise
******************************************************************************/
static bool cmdClkOut (void)
{
    bool retVal = true;
    bool clkEnable=false;
    bool enableOnly = true;
    cgc_clocks_t clock_source=CGC_MAIN_OSC;
    uint8_t clockDiv = clockout_divider;


  if (pArg1 != NULL)
  {
	  if (*pArg1 == 'e')
		  clkEnable = true;
	  else
		  if (*pArg1 != 'd')
			  return(false);


	  if (pArg2 != NULL)	// is there a clock name argument?
		{
			enableOnly = false;
			switch(*pArg2)
			  {
				  case 'm':
					  clock_source = CGC_MAIN_OSC;
				  break;

				  case 'h':
					  clock_source = CGC_HOCO;
				  break;

				  case 'l':
					  clock_source = CGC_LOCO;
				  break;

				  case 's':
					  clock_source = CGC_SUBCLOCK;
				  break;

				  default:
					return(false);
			  }

			if (pArg3 != NULL)
			{
				clockDiv = (uint16_t)strtol(pArg3,NULL,10);
				clockout_divider = (cgc_clockout_dividers_t)ClockDivToBits(clockDiv);
			}

			clock_request_status = R_CGC_ClockOutputConfig(clock_source, clockout_divider, clkEnable);
			if (clock_request_status != CGC_SUCCESS)
			  return(false);
			clkOutputEnabled = clkEnable;
			clockOutputSource = clock_source;
		}
      if (enableOnly)
		{
			R_CGC_ClockOutputEnable(clkEnable);
			clkOutputEnabled = clkEnable;
		}
  }
  printf("ClockOutput: %s, divisor: %d ", clock_names[clockOutputSource],divisors[clockout_divider]);
  if (clkOutputEnabled)
  {
	  printf("(enabled)\r\n");
  }
  else
  {
	  printf("(disabled)\r\n");
  }
 return(retVal);
}


/*****************************************************************************
* Function Name: cmdMstp
* Description  : Reads/sets the MSTP registers
* Arguments    : none
* Format       : mstp::<a | b | c>::<value in hex>
* Return Value : true if command successfully parsed/implemented, false otherwise
******************************************************************************/
bool cmdMstp (void)
{
  char regName = 0;
  uint32_t regValue = 0;
  bool reg_write = false;
  bool retVal = true;


  if (pArg1 == NULL)		// Did we find a "::" delimiter ?
  {
	  // We did not find a "::"
	  regName = '*';	// This is just a query, return current val.
  }
  else
  {
	regName = *pArg1;

	// read the value, there better be one
	if (pArg2 == NULL)
		return(false);

	regValue = (uint32_t)strtoul (&pArg2[0],NULL,16);
	reg_write = true;
  }

  SYSTEM.PRCR.WORD = 0xA502;         // Remove protection for modifying system registers
  switch(regName)
  {
	  case 'a':
		  if (reg_write == true)
			  SYSTEM.MSTPCRA.LONG = regValue;
		  printf("MSTPA = %lx\r\n", SYSTEM.MSTPCRA.LONG) ;
	  break;

	  case 'b':
		  if (reg_write == true)
			  SYSTEM.MSTPCRB.LONG = regValue;
		  printf("MSTPB = %lx\r\n", SYSTEM.MSTPCRB.LONG) ;
	  break;

	  case 'c':
		  if (reg_write == true)
			  SYSTEM.MSTPCRC.LONG = regValue;
		  printf("MSTPC = %lx\r\n", SYSTEM.MSTPCRC.LONG) ;
	  break;

	  case '*':
		  printf("MSTPA = %lx\r\n", SYSTEM.MSTPCRA.LONG) ;
		  printf("MSTPB = %lx\r\n", SYSTEM.MSTPCRB.LONG) ;
		  printf("MSTPC = %lx\r\n", SYSTEM.MSTPCRC.LONG) ;
	  break;

	  default:
		    retVal = false;;
	  break;

  }
  SYSTEM.PRCR.WORD = 0xA500;         // Set protection for modifying system registers
  return(retVal);
}

/*****************************************************************************
* Function Name: cmdClkDiv
* Description  : Reads/sets the PCKB, PCKD, or FCK divisor portion of SCKCR.
*                Note: The actual update of the clock's divisor doesn't
*                happen until a call is made to R_CGC_SystemClockSet().
* Arguments    : none
* Format       : pclkdiv::::<i | f | b | d>::<1 | 2 | 4 | 8 | 16 | 32 | 64>
* Return Value : true if command successfully parsed/implemented, false otherwise
******************************************************************************/
bool cmdClkDiv (void)
{
  volatile signed int i;
  uint8_t clockDiv = 0;
  uint8_t clockBits;
  uint16_t configuredDivisor;
  bool retVal = false;
  char clkName = 0;
  char dirty = ' ';

  if (pArg1 != NULL)		// Did we find a "::" delimiter ?
  {
	clkName = pArg1[0];
    // Get the configured divisor for this clock.
	switch(clkName)
	{
		case 'b':
			configuredDivisor = clock.pclkb;
		break;

		case 'd':
			configuredDivisor = clock.pclkd;
		break;

		case 'i':
			configuredDivisor = clock.iclk;
		break;

		case 'f':
			configuredDivisor = clock.fclk;
		break;

		default:
			return(false);
	}

	if (pArg2 != NULL)
	{
		clockDiv = (uint16_t)strtol(&pArg2[0],NULL,10);
		clockBits = ClockDivToBits(clockDiv);
		if (clockBits == 0xFF)
			return(false);
		retVal = true;

		// The actual update of the clock's divisor doesn't happen until a call is made to R_CGC_SystemClockSet()
		switch(clkName)
		{
			case 'b':
				clock.pclkb = clockBits;
			break;

			case 'd':
				clock.pclkd = clockBits;
			break;

			case 'i':
				clock.iclk = clockBits;
			break;

			case 'f':
				clock.fclk = clockBits;
			break;

			default:
				return(false);
		}

	}
	else
	{
		// just a query.

#if defined(__IAR_SYSTEMS_ICC__)
    uint16_t tmp_clockDiv = (uint16_t)clockDiv;
		retVal = getSCKCR(&tmp_clockDiv, clkName);
    clockDiv = tmp_clockDiv;
#else
    retVal = getSCKCR(&(uint16_t)clockDiv, clkName);
#endif
	}
	if (retVal == true)
	{
		// Show an '*' if the currently configured divisor has not yet been written to the register.
		if (clockDiv != divisors[configuredDivisor])
		{
			dirty = '*';
		}
		printf("Clk %c divisor=%d%c\r\n",clkName,clockDiv,dirty);
	}

  }
  return(retVal);
}

/*****************************************************************************
* Function Name: cmdPLLDiv
* Description  : Query - Reads the actual PLL divisor in the PLLCR
*                Write - Updates the clock_config structure supplied to actual
*                        FIT clock module functions. Until a R_CGC_ClockStart()
*                        is successfully executed, the PLLCR register contents
*                        may not reflect the clock_config settings.
* Arguments    : none
* Format       : plldiv::<1 | 2 | 4>
* Return Value : true if command successfully parsed/implemented, false otherwise
******************************************************************************/
bool cmdPLLDiv (void)
{
  volatile signed int i;
  bool retVal = true;
  char dirty = ' ';

  if (pArg1 != NULL)		// Did we find a "::" delimiter ?
  {
	switch(*pArg1)
	{
		case '1':
			clock_config.divider = PLL_DIV_1;
		break;

		case '2':
			clock_config.divider = PLL_DIV_2;
		break;

		case '4':
			clock_config.divider = PLL_DIV_4;
		break;

		default:
			return(false);

	}
  }
  if (retVal == true)
	{
	  // Show the setting.
	  if (clock_config.divider != SYSTEM.PLLCR.BIT.PLIDIV)
		  dirty = '*';
	  printf("PLL divider: %d%c\r\n",1<<clock_config.divider,dirty);
	}

  return(retVal);
}

/*****************************************************************************
* Function Name: cmdPLLMul
* Description  : Query - Reads the actual PLL multiplier in the PLLCR
*                Write - Updates the clock_config structure supplied to actual
*                        FIT clock module functions. Until a R_CGC_ClockStart()
*                        is successfully executed, the PLLCR register contents
*                        may not reflect the clock_config settings.
* Arguments    : none
* Format       : pllmul::<6 | 8>
* Return Value : true if command successfully parsed/implemented, false otherwise
******************************************************************************/
bool cmdPLLMul (void)
{
	  volatile signed int i;
	  bool retVal = true;
	  char dirty = ' ';

	  if (pArg1 != NULL)		// Did we find a "::" delimiter ?
	  {
		switch(*pArg1)
		{
			case '6':
				clock_config.multiplier = PLL_MUL_6;

			break;

			case '8':
			    clock_config.multiplier = PLL_MUL_8;

			break;

			default:
				return(false);

		}
	  }
	  if (retVal == true)
		{
		  // Show the setting.
		  if (clock_config.multiplier != SYSTEM.PLLCR.BIT.STC)
			  dirty = '*';
		  if (clock_config.multiplier == PLL_MUL_8)
			  printf("PLL multiplier: 8%c\r\n",dirty);
		  else
			  printf("PLL multiplier: 6%c\r\n",dirty);

		}

	  return(retVal);
}

/*****************************************************************************
* Function Name: cmdRTCDate
* Description  : Reads/sets the RTC date
* Arguments    : none
* Format       : rtcDate::mm:dd:yy
* Return Value : true if command successfully parsed/implemented, false otherwise
******************************************************************************/
bool cmdRTCDate (void)
{
  int month, day, year;
  unsigned char bcdByte, upperNibble, lowerNibble ;


  // Did we find a "::" delimiter ?
  if (pArg1 != NULL)
  {
	   month = atoi(pArg1) ;
	   if ((month < 1) || (month > 12))
	   {
	     return(false);
	   }

	   if (pArg2 == 0)
	   {
	     return(false);
	   }
	   else
	   {
	     day = atoi(pArg2) ;
	     if ((day < 1) || (day > 31))
	     {
	        return(false);
	     }
	   }

	   if (pArg3 == 0)
	   {
	        return(false);
	   }
	   else
	   {
	     year = atoi(pArg3) ;
	     if ((year < 0) || (year > 99))
	     {
	        return(false);
	     }
	   }

	   // Turn off the clock to set it
	   RTC.RCR2.BIT.START = 0 ;
	   while (RTC.RCR2.BIT.START != 0) ;         // Wait for it to stop

	   RTC.RCR2.BIT.RESET = 1 ;                  // Reset the 64 Hz counter

	   // Month
	   upperNibble = (unsigned char) ((month / 10) & 0xFF);
	   lowerNibble = (unsigned char) ((month % 10) & 0xFF) ;
	   bcdByte = (unsigned char)((upperNibble << 4) | lowerNibble);
	   RTC.RMONCNT.BYTE = bcdByte ;

	   // Day
	   upperNibble = (unsigned char) ((day / 10) & 0xFF);
	   lowerNibble = (unsigned char) ((day % 10) & 0xFF) ;
	   bcdByte = (unsigned char)((upperNibble << 4) | lowerNibble);
	   RTC.RDAYCNT.BYTE = bcdByte ;

	   // Year
	   upperNibble = (unsigned char) ((year / 10) & 0xFF);       // 10's digit
	   RTC.RYRCNT.BIT.YR10 = upperNibble ;

	   upperNibble = (unsigned char) ((year % 10) & 0xFF);       // 1's digit
	   RTC.RYRCNT.BIT.YR1 = upperNibble ;

	   RTC.RCR2.BIT.START = 1 ;                    // Restart the clock


  }
  printf("Current date: %02x/%02x/%02x\r\n", RTC.RMONCNT.BYTE, RTC.RDAYCNT.BYTE, RTC.RYRCNT.WORD) ;
  return(true);

}

/*****************************************************************************
* Function Name: cmdRTCTime
* Description  : Reads/sets the RTC time
* Arguments    : none
* Format       : rtcTime::hh::mm::ss
* Return Value : true if command successfully parsed/implemented, false otherwise
******************************************************************************/
bool cmdRTCTime (void)
{
  int hours, minutes, seconds ;
  unsigned char bcdByte, upperNibble, lowerNibble ;


  // Did we find a "::" delimiter ?
  if (pArg1 != NULL)
  {
      hours = atoi(pArg1);
      if ((hours < 0) || (hours > 23))
      {
        return(false);
      }

      if (pArg2 == 0)
      {
        minutes = 0;
      }
      else
      {
       minutes = atoi(pArg2);
        if ((minutes < 0) || (minutes > 59))
        {
          return(false);
        }
      }

      if (pArg3 == 0)
      {
        seconds = 0;
      }
      else
      {
        seconds = atoi(pArg3);
        if ((seconds < 0) || (seconds > 59))
        {
          return(false);
        }
      }

      // Turn off the clock to set it
      RTC.RCR2.BIT.START = 0 ;
      while (RTC.RCR2.BIT.START != 0);         // Wait for it to stop

      RTC.RCR2.BIT.RESET = 1;                  // Reset the 64 Hz counter

      upperNibble = (unsigned char) ((hours / 10) & 0xFF);
      lowerNibble = (unsigned char) ((hours % 10) & 0xFF);
      bcdByte = (unsigned char)((upperNibble << 4) | lowerNibble);

      RTC.RHRCNT.BYTE = bcdByte;

      upperNibble = (unsigned char) ((minutes / 10) & 0xFF);
      lowerNibble = (unsigned char) ((minutes % 10) & 0xFF) ;
      bcdByte = (unsigned char)((upperNibble << 4) | lowerNibble);
      RTC.RMINCNT.BYTE = bcdByte;

      upperNibble = (unsigned char) ((seconds / 10) & 0xFF);
      lowerNibble = (unsigned char) ((seconds % 10) & 0xFF) ;
      bcdByte = (unsigned char)((upperNibble << 4) | lowerNibble);
      RTC.RSECCNT.BYTE = bcdByte;

      RTC.RCR2.BIT.START = 1;                    // Restart the clock
  }
  printf("Current time: %02x:%02x:%02x\r\n", RTC.RHRCNT.BYTE, RTC.RMINCNT.BYTE, RTC.RSECCNT.BYTE);
  return(true);

}
/*****************************************************************************
* Function Name: cmdHelp
* Description  : Display the available commands and their syntax
* Arguments    : none
* Format       : ?
* Return Value : true if command successfully parsed/implemented, false otherwise
******************************************************************************/
static bool cmdHelp (void)
{
    uint32_t    i;
    uint32_t	max_cmds = (sizeof(commands) / sizeof(HOST_COMMAND));

    printf("An '*' indicates this requested value has not yet been written.\r\n");
    printf("Most arguments can be supplied using a single character. ie (s)leep.\r\n");
    printf("Most commands used without an argument will act as a query.\r\n\r\n");

   printf("Available commands:\r\n");
   for (i = 0 ; i < max_cmds; i++)
     {
       printf("%s%s\r\n", commands[i].command,commands[i].format);
     } // for (i...)

   return(true);
}

/*****************************************************************************
* Function Name: cmdRTC
* Description  : Reads/enables/disables the rtc.
* Arguments    : none
* Format       : rtc::<enabled | disabled>. rtc::e or rtc::d are acceptable.
* Return Value : true if command successfully parsed/implemented, false otherwise
******************************************************************************/
bool cmdRTC (void)
{
  bool bStarting = false;


  // Did we find a "::" delimiter ?
  if (pArg1 != NULL)
  {
     if (*pArg1 == 'e')
     {
         bStarting = true;
         RTC.RCR2.BIT.START = 1;
     }
     else
         if (*pArg1 == 'd')
         {
             RTC.RCR2.BIT.START = 0;
         }
         else
         {
             return(false);
         }
     while( RTC.RCR2.BIT.START != bStarting);
  }
  // Show what the rtc is now set for.
  if (RTC.RCR2.BIT.START)
      printf("RTC is enabled\r\n");
  else
      printf("RTC is disabled\r\n");

  return(true);
}


/*****************************************************************************
* Function Name: cmdrunCoremark
* Description  : Execute the Cormark performance test until complete
* Format       : core
* Return Value : true if command successfully parsed/implemented, false otherwise
******************************************************************************/
static bool cmdrunCoremark(void)
{

	processCommand("rtc::e\r\n");				// We need the RTC running
	return(runCoreMark());
}


//-----------------------------------------------------------------------------
// ModuleStopSet
//
// Configures the MSTP bits.  Call with PERIPH_ALL_ON to power everything up,
// or with PERIPH_INITIAL_STATE to restore module power states to their
// original state after board initialization (everything off except for SCI2
// and CMT0).
//
// Expects: Nothing
// Returns: Nothing
//-----------------------------------------------------------------------------
void ModuleStopSet (int state)
{

  if (state == PERIPH_ALL_ON)
  {
    mstpaCopy = SYSTEM.MSTPCRA.LONG ;
    mstpbCopy = SYSTEM.MSTPCRB.LONG ;
    SYSTEM.MSTPCRA.LONG = 0xEFF57DFF;
    SYSTEM.MSTPCRB.LONG = 0xBB55FDAF;
  }
  else if (state == PERIPH_INITIAL_STATE)
  {
    SYSTEM.MSTPCRA.LONG  = mstpaMaster ;
    SYSTEM.MSTPCRB.LONG  = mstpbMaster ;
  }
  else if (state == PERIPH_ALL_OFF)
  {
    mstpaCopy = SYSTEM.MSTPCRA.LONG ;
    mstpbCopy = SYSTEM.MSTPCRB.LONG ;
    SYSTEM.MSTPCRA.LONG  = 0xF7FFFFFF ;
    SYSTEM.MSTPCRB.LONG  = 0xFFFFFFFF ;
  }
  else if (state == PERIPH_RESTORE)
  {
    SYSTEM.MSTPCRA.LONG  = mstpaCopy ;
    SYSTEM.MSTPCRB.LONG  = mstpbCopy ;
  }
}

//-----------------------------------------------------------------------------
// SetAlarm
//
// Sets the RTC alarm to go off.
//
// Expects:
//  int seconds - BCD representation of seconds (i.e. 0x30 to wake up in 30 seconds)
//
// Returns:
//  Nothing
//-----------------------------------------------------------------------------
void SetAlarm (int seconds)
{
  unsigned char secs ;

  RTC.RCR2.BIT.START = 0 ;                  // Turn off the clock
  while (RTC.RCR2.BIT.START != 0) ;         // Wait for it to stop

  RTC.RCR2.BIT.RESET = 1 ;                  // Reset the 64 Hz counter
  IR(RTC, ALM) = 0 ;                        // Clear any pending interrupts

  secs = (char)(RTC.RSECCNT.BYTE + seconds);       // Wake up in 10 seconds
  if (secs > 0x59)
	  secs = (unsigned char)(secs - 0x60);     // Handle rollover (in BCD)

  RTC.RSECAR.BYTE = (unsigned char)((secs | 0x80));         // Set the alarm enable bit
  RTC.RCR1.BIT.AIE = 1 ;                    // Enable alarm interrupt

  IEN(RTC, ALM) = 1 ;                       // Enable interrupt in ICU
  IPR(RTC, ALM) = 0x05 ;                    // Set IRQ priority > 0

  RTC.RCR2.BIT.START = 1 ;                  // Turn on the clock
}

/*****************************************************************************
* Function Name: EnterSleepSTMode
* Description  : Enter the requested sleep/deepsleep/standby mode. Exits
*                on SW2 push.
* Arguments    : char slpType - 's', 'd', or 't'
* Format       : ?
* Return Value : true if command successfully parsed/implemented, false otherwise
******************************************************************************/
static bool EnterSleepSTMode (char slpType)
{

	if (slpType == 's')
	{
		powmode_request_status =  R_LPC_ConfigureLowPowerMode(LP_SLEEP);
	    if (powmode_request_status != LPC_SUCCESS)
		  return(false);

		if (current_powerMode == OP_LOW_SPEED)
		{
			// For low power sleep mode we will let the RSTCKCR register control the mode
			// when we exit sleep
			powmode_request_status = R_LPC_ReturnClockSwitch(LPC_HOCO, true);
		}
	}
	else
	if (slpType == 'd')
	{
		powmode_request_status =  R_LPC_ConfigureLowPowerMode(LP_DEEP_SLEEP);
	    if (powmode_request_status != LPC_SUCCESS)
		  return(false);
	}
	else
	if (slpType == 't')
	{
		powmode_request_status =  R_LPC_ConfigureLowPowerMode(LP_SW_STANDBY);
	    if (powmode_request_status != LPC_SUCCESS)
		  return(false);
	}

    if (powmode_request_status != LPC_SUCCESS)
    	return(false);

    // Do this last so we can still show an error (above) on the console if there is one.
    R_SCI_Close(console);		// Reduce power usage while sleeping. We'll re-open when we wake up
    console = NULL;

	powmode_request_status =  R_LPC_LowPowerActivate(NULL);

    return(true);
}





/******************************************************************************
* Function Name: setPowermode
* Description  : Execute the requested power mode setting
* Arguments    : char mode - requested 'h', 'm', or 'l' power mode
*
* Return Value : none
* Note         : true if valid/successful, false otherwise
******************************************************************************/
bool setPowermode(char mode)
{
	lpc_operating_mode_t requested_powerMode;

	  switch(mode)
	    {
	    	case 'h':
	    		requested_powerMode = OP_HIGH_SPEED;
	    	break;

	    	case 'm':
	    		requested_powerMode = OP_MIDDLE_SPEED;
	    	break;

	    	case 'l':
	    		requested_powerMode = OP_LOW_SPEED;

      		    // Low power mode must have ICLK src = sub-clock
	    		clock_request_status = R_CGC_ClockStart(CGC_SUBCLOCK, &clock_config); // Make sure the sub-clock is running
	    		if ((clock_request_status != CGC_SUCCESS) && (clock_request_status != CGC_ERR_CLOCK_ACTIVE))
	    		  return(false);
	    		clock_request_status = R_CGC_SystemClockSet(CGC_SUBCLOCK, &clock);
	    		if (clock_request_status != CGC_SUCCESS)
	    		  return(false);
	    	  	requested_powerMode = OP_LOW_SPEED;
	    	break;

		    default:
			  return(false);
	    }

	if (requested_powerMode == OP_LOW_SPEED)
	{
        R_SCI_Close(console);		// Close the serial port to reduce power usage. It's not usable in low power mode anyway
        console = NULL;
		clock_request_status = R_CGC_ClockStop(CGC_HOCO); // HOCO and LOCO must be stopped to enter low power mode
		clock_request_status = R_CGC_ClockStop(CGC_LOCO);
	}

    // Enter low power mode
    powmode_request_status = R_LPC_ConfigureOperatingMode(requested_powerMode);	// Set the appropriate power mode (regulators)
    if (powmode_request_status != LPC_SUCCESS)
    	return(false);

	current_powerMode = requested_powerMode;		// If we made it here, we've successfully changed modes

return(true);
}



/******************************************************************************
* Function Name: getSCKCR
* Description  : Return the clock divisor for the specified clock
* Arguments    : uint16_t *clkvalue - pointer to where to store the divisor in use
* Return Value : char reqClk - 'f', 'b' or 'd'
* Note         : true if successful/valid, false otherwise
******************************************************************************/
static bool getSCKCR(uint16_t *clkvalue, char reqClk)
{
	  uint32_t temp_clock;
	switch(reqClk)
	{
		case 'f':	//FCLK
			temp_clock = SYSTEM.SCKCR.BIT.FCK;
		break;

		case 'b':	//PCLKB
			temp_clock = SYSTEM.SCKCR.BIT.PCKB;
		break;

		case 'd':	//PCLKD
			temp_clock = SYSTEM.SCKCR.BIT.PCKD;
		break;

		case 'i':	//ICLK
			temp_clock = SYSTEM.SCKCR.BIT.ICK;
		break;

		default:
			return(false);
	}
	if (temp_clock >= MAX_DIVISORS)		// just to be safe..
		return(false);
	*clkvalue = divisors[temp_clock];
return(true);
}

/******************************************************************************
* Function Name: ClockDivToBits
* Description  : Return the clock bit sequence for the specified divisor
* Arguments    : uint8_t clockDiv - 1,2,4,8,16,32,64
* Return Value : uint8_t - 4 bit sequence corresponding to the divisor provided.
*                           ie. 0x06 is returned for a divisor of 64.
* Note         : 0xFF is returned if an invalid divisor is provided
******************************************************************************/
uint8_t ClockDivToBits(uint8_t clockDiv)
{
	uint8_t retVal = 0xFF;

	switch(clockDiv)
	{
		case 1:
			retVal = 0x00;
		break;

		case 2:
			retVal = 0x01;
		break;

		case 4:
			retVal = 0x02;
		break;

		case 8:
			retVal = 0x03;
		break;

		case 16:
			retVal = 0x04;
		break;

		case 32:
			retVal = 0x05;
		break;

		case 64:
			retVal = 0x06;
		break;
	}
return(retVal);
}

uint32_t getTimeinSecs()
{
	return (uint32_t)((bcdToDec(RTC.RHRCNT.BYTE)*3600) + (bcdToDec(RTC.RMINCNT.BYTE)*60) + bcdToDec(RTC.RSECCNT.BYTE));
}

static uint8_t bcdToDec(uint8_t val)
{
  return ((uint8_t)((val/16*10) + (val%16)));
}

