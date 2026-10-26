#include "platform.h"
#if defined(BSP_MCU_RX63_ALL)
#include "r_rspi_rx_if.h"
#include "r_glyph_config.h"
#endif
#include "lcd.h"
#include "r_sci_async_rx_if.h"
#include "r_byteq_if.h"
#include "r_sci_async_rx_config.h"
#include <stdio.h>
#include <string.h>
#include "cmd_parser.h"
#include "r_cgc_rx_if.h"
#include "r_lpc_rx_if.h"
#include "r_irq_rx_if.h"

/******************************************************************************
Macro definitions, local data
******************************************************************************/
#define CR      13
#define LF      10

#define BUFSIZE 80
char rxBuffer[BUFSIZE];

#define _000001B8_RTC_RTCSCLK_STABLE_CYCLE          (0x000001B8U)
#define _00_RTC_ALARM_INT_DISABLE                   (0x00U)
#define _01_RTC_ALARM_INT_ENABLE                    (0x01U)
/* Carry Interrupt Enable (CIE) */
#define _00_RTC_CARRY_INT_DISABLE                   (0x00U)
#define _02_RTC_CARRY_INT_ENABLE                    (0x02U)
/* Periodic Interrupt Enable (PIE) */
#define _00_RTC_PERIOD_INT_DISABLE                  (0x00U)
#define _04_RTC_PERIOD_INT_ENABLE                   (0x04U)


#define _00_RTC_HOUR_MODE_12                        (0x00U)
#define _40_RTC_HOUR_MODE_24                        (0x40U)

/*
RTC Control Register (RCR3)
*/
/* Sub-clock Control (RTCEN) */
#define _00_RTC_SUBCLOCK_DISABLE                    (0x00U)
#define _01_RTC_SUBCLOCK_ENABLE                     (0x01U)
/* Sub-clock Oscillator Drive Capacity Control (RTCDV[1:0]) */
#define _00_RTC_SUBCLOCK_DRIVE_MID                  (0x00U)
#define _02_RTC_SUBCLOCK_DRIVE_HIGH                 (0x02U)
#define _04_RTC_SUBCLOCK_DRIVE_LOW                  (0x04U)
#define _00_RTC_PERIODIC_INT_PERIOD_DISABLE         (0x00U)

#define LED0_ON		PORTB.PODR.BIT.B7 = 0;
#define LED0_OFF	PORTB.PODR.BIT.B7 = 1;
#define	LED0_TOGGLE	PORTB.PODR.BIT.B7 = ~PORTB.PODR.BIT.B7;

#define LED1_ON		PORTA.PODR.BIT.B0 = 0;
#define LED1_OFF	PORTA.PODR.BIT.B0 = 1;
#define	LED1_TOGGLE	PORTA.PODR.BIT.B0 = ~PORTA.PODR.BIT.B0;

#define LED2_ON		PORT5.PODR.BIT.B4 = 0;
#define LED2_OFF	PORT5.PODR.BIT.B4 = 1;
#define	LED2_TOGGLE	PORT5.PODR.BIT.B4 = ~PORT5.PODR.BIT.B4;

#define LED3_ON		PORT1.PODR.BIT.B7 = 0;
#define LED3_OFF	PORT1.PODR.BIT.B7 = 1;
#define	LED3_TOGGLE	PORT1.PODR.BIT.B7 = ~PORT1.PODR.BIT.B7;


sci_hdl_t   console;
sci_uart_t  config;
cgc_system_clock_config_t clock;
cgc_clock_config_t clock_config;
cgc_clocks_t clock_source;
cgc_err_t clock_request_status = CGC_SUCCESS;		// A NAK'd command will check this byte.
lpc_err_t powmode_request_status = LPC_SUCCESS;		// A NAK'd command will check this byte.;


volatile uint32_t got_command;
unsigned long mstpaMaster, mstpbMaster ;
bool awake = false;
bool sw1_pressed = false;

/******************************************************************************
Prototypes for local functions
******************************************************************************/
void sci_111_init_ports(void);
void sci_callback(void *pArgs);
void charput (char theChar) ;
char charget (void);
static void rtc_111_init(void);
static void checkErrorCode(void);

// Callback functions for switch pushes
void sw1_callback(void * pdata);
void sw2_callback(void * pdata);
void sw3_callback(void * pdata);
static void ConfigureUnusePort(void);
static void ConfigureIRQandClkPins(void);

const char *clk_err_strings[] = {
  {""},
  {"inactive clock specified as system clock"},
  {"active clock source cannot be modified without stopping first"},
  {"Clock has stabilized after its been turned on/off"},
  {"clock has not stabilized after its been turned on/off"},
  {"PLL initialization attempted when main OSC is turned off"},
  {"Null pointer passed as argument"},
  {"Illegal attempt to stop LOCO when Oscillation stop is enabled"},
  {"The Oscillation stop detection status flag is set"},
  {"Attempt to clear Oscillation Stop Detect Status with PLL/MAIN_OSC active"},
  {"Invalid argument supplied, or unsupported operation using current settings"}
};


const char *lpc_err_strings[] = {
  {""},
  {"Inactive clock specified as system clock"},
  {"Null pointer passed as argument"},
  {"Software Standby Mode cannot be entered if Oscillation Stop is enabled"},
  {"Clock speed exceeds the limit of current operating mode"},
  {"Main Oscillator is more than 8 MHz and is chosen as the Sleep return clock"},
  {"Return Clock Switch is only supported if clock on entering Sleep is SubOSC"},
  {"Illegal argument not specified in enum"}
};


const char *irq_err_strings[] = {
  {""},
  {"Invalid IRQ number"},
  {"IRQ not yet opened"},
  {"IRQ still open from previous open"},
  {"Control command is not recognized"},
  {"Argument is not valid for parameter"},
  {"Received null pointer; missing required argument"},
  {"The calling procedure does not own the lock"},
  {"The lock procedure failed"}
};
/*****************************************************************************
* Function Name: main()
* Description  : Main loop for command processing application.
*
*
* Arguments    : none
* Return Value : none
******************************************************************************/
void main(void)
{
  uint32_t    i;
  sci_err_t   err;
  irq_err_t   result;
  irq_handle_t sw1_handle;
  irq_handle_t sw2_handle;
  irq_handle_t sw3_handle;
  sci_baud_t  baudInfo;

#ifdef USE_LCD
  lcd_initialize();
  lcd_display(LCD_LINE1,"RSKRX111");
  lcd_display(LCD_LINE2,"PressSW1");
#endif

  //Configures unused port
  ConfigureUnusePort();

  // Turn off all clocks except the HOCO (reduce power usage)
  clock_request_status = R_CGC_ClockStop(CGC_LOCO);
  clock_request_status = R_CGC_ClockStop(CGC_MAIN_OSC);
  clock_request_status = R_CGC_ClockStop(CGC_SUBCLOCK);
  clock_request_status = R_CGC_ClockStop(CGC_PLL);

  sci_111_init_ports();       // Init port for appropriate device - FOR RSKRX111 ONLY!

  /* Open channel for Async Mode operation */
  config.baud_rate = 2400;		//Fastest rate for which all PCLKB divisors will work with HOCO
  config.clk_src = SCI_CLK_INT;
  config.data_size = SCI_DATA_8BIT;
  config.parity_en = SCI_PARITY_OFF;
  config.parity_type = SCI_EVEN_PARITY;
  config.stop_bits = SCI_STOPBITS_1;
  config.int_priority = 2;            // 1=lowest, 15=highest

  err = R_SCI_Open(SCI_CH1, SCI_MODE_ASYNC, (void *)&config, sci_callback, &console);

  // Setup the divisors for the various clocks. These need to initially reflect what the bsp is using at this time.
  clock.pclkb= ClockDivToBits(BSP_CFG_PCKB_DIV);
  clock.pclkd= ClockDivToBits(BSP_CFG_PCKD_DIV);
  clock.iclk = ClockDivToBits(BSP_CFG_ICK_DIV);
  clock.fclk = ClockDivToBits(BSP_CFG_FCK_DIV);

  clock_config.divider = BSP_CFG_PLL_DIV >> 1;

  if (BSP_CFG_PLL_MUL == 6)
    clock_config.multiplier = PLL_MUL_6;	// Multiply by 6  16MHz main clock ==> (clk/2)*6 = 48MHz
  else
    clock_config.multiplier = PLL_MUL_8;

  clock_source = (cgc_clocks_t)BSP_CFG_CLOCK_SOURCE;				// We're initially setup to run off the HOCO

  // Setup so that a simple clkout::e will start the clkout
  clock_request_status = R_CGC_ClockOutputConfig(CGC_HOCO, CLOCKOUT_DIV_1, false);

  rtc_111_init();             // Init rtc - FOR RSKRX111 ONLY!

  // Save the initial state of the module stop registers after everything
  // we need has been turned on
  mstpaMaster = SYSTEM.MSTPCRA.LONG ;
  mstpbMaster = SYSTEM.MSTPCRB.LONG ;
  got_command = false;

  SYSTEM.PRCR.WORD = 0xA502;         // Remove protection for modifying low power registers
  DTC.DTCST.BIT.DTCST = 0;           // Clear the DTCST bit before disabling the DTC module
  SYSTEM.MSTPCRA.LONG = 0xFFFFFFFF;  // Turn off all peripheral modules except SCI1
  SYSTEM.MSTPCRB.LONG = 0xFFFFFFFF;
  SYSTEM.MSTPCRB.BIT.MSTPB30 = 0;
  SYSTEM.MSTPCRC.LONG = 0xFFFFFFFE;  // Leave RAM0 on
  SYSTEM.MSTPCRC.BIT.DSLPE = 0;	   // Disable deep sleep mode

  SYSTEM.PRCR.WORD = 0xA500;         // Set protection for modifying low power registers

  memset(&rxBuffer[0],0,sizeof(rxBuffer));  // insure we don't parse leftovers from prior command

  //LED0_OFF;		// Current usage is slightly higher with LEDs off
  //LED1_OFF;
  //LED2_OFF;
  //LED3_OFF;
  // Configure the port pins for clkout and the push switches
  ConfigureIRQandClkPins();

  // sw1: IRQ0, priority 3
  result = R_IRQ_Open(0, IRQ_TRIG_FALLING, IRQ_PRI_3, &sw1_handle, &sw1_callback);
  if ((result > IRQ_SUCCESS) && (result <= IRQ_ERR_LOCK))
    printf("sw1 IRQ setup failed - %s\r\n",irq_err_strings[result]);

  // sw2: IRQ1, priority 3
  result = R_IRQ_Open(1, IRQ_TRIG_FALLING, IRQ_PRI_3, &sw2_handle, &sw2_callback);
  if ((result > IRQ_SUCCESS) && (result <= IRQ_ERR_LOCK))
    printf("sw2 IRQ setup failed - %s\r\n",irq_err_strings[result]);

  // sw3: IRQ4, priority 3
  result = R_IRQ_Open(4, IRQ_TRIG_FALLING, IRQ_PRI_3, &sw3_handle, &sw3_callback);
  if ((result > IRQ_SUCCESS) && (result <= IRQ_ERR_LOCK))
    printf("sw3 IRQ setup failed - %s\r\n",irq_err_strings[result]);

  __enable_interrupt();

  printf("\r\nRSK RX111 test program - Type ? for help\r\n");

  while(1)
  {
    if (got_command == true)
    {
      i=0;
      do
      {
        err = R_SCI_Receive(console, (uint8_t *)&rxBuffer[i], 1);
        if (rxBuffer[i] == '\r')
        {
          err = SCI_SUCCESS;
          break;
        }
        i++;
      } while (err == SCI_SUCCESS);

      // If we got a complete command, then attempt to process it
      if (err == SCI_SUCCESS)
      {
        if (processCommand(&rxBuffer[0]) == false)
        {
          checkErrorCode();		// Will printf a 'NAK', possibly add'l info
        }
        else
          printf("\r\n");
        memset(&rxBuffer[0],0,sizeof(rxBuffer));  // insure we don't parse leftovers from prior command

      }
      got_command = false;
    }
    else
      if (awake)
      {
        if (console != NULL && sw1_pressed)
        {
          printf("\r\n");				// Clean new line
          processCommand("core");		// Show the menu
        }
        // We've been awoken vis SW2. The serial port may need to be re-opened, or the queue flushed.
        else if (console == NULL)
        {
          // Are we using the R_LPC_ReturnClockSwitch() feature?
          if (SYSTEM.RSTCKCR.BIT.RSTCKEN == 0)
          {
            // Manually set a clock source that will allow the serial port to function again.
            // It is imperative that when switching to a higher clock frequency, the mode
            // is changed first to support that frequency.
            setPowermode('h');	// first switch the power mode to high w/o touching the clock src

            // Now that we're in high power mode, start the HOCO.
            clock_request_status = R_CGC_ClockStart(CGC_HOCO, &clock_config);	// start HOCO

            // HOCO should be up and running. Make it the iclk source.
            clock_request_status = R_CGC_SystemClockSet(CGC_HOCO, &clock);	// Make the clksrc = HOCO
          }
          // re-open the serial port with HOCO as the clock
          baudInfo.pclk = R_CGC_ClockHzGet(CGC_PCLKB);
          baudInfo.rate = 2400;

          err = R_SCI_Open(SCI_CH1, SCI_MODE_ASYNC, (void *)&config, sci_callback, &console);
          err = R_SCI_Control(console, SCI_CMD_CHANGE_BAUD, (void *)&baudInfo);	// Set the baud rate using the new clock
        }
        else
        {
          // Flush anything that might be in the Tx Queue
          R_SCI_Control(console, SCI_CMD_FLUSH_TX_QUEUE, NULL);
        }
        awake = false;
        sw1_pressed = false;
        printf("\r\n");				// Clean new line
        processCommand("?");		// Show the menu
      }
  }
}


/*****************************************************************************
* Function Name: sci_callback
* Description  : This is a template for an SCI Async Mode callback function.
* Arguments    : pArgs -
*                    ptr to sci_cb_args_t structure casted to a void. Structure
*                    contains event and associated data.
* Return Value : none
******************************************************************************/
void sci_callback(void *pArgs)
{
  sci_cb_args_t    *args;


  args = (sci_cb_args_t *)pArgs;

  if (args->event == SCI_EVT_RX_CHAR)
  {
    // from RXI interrupt; received character
    // char is in args->byte
    /* For now echo the received input back to the console */

    charput((char)args->byte);
    if (args->byte == CR)
    {
      got_command = true;
      charput('\n');
    }

    nop();
  }

}


/*****************************************************************************
* Function Name: sci_111_init_ports
* Description  : This function initializes the port pins associated with
*                channel 1 for the RSKRX111. Pins are configured for TX/RX
*                operation.
* Arguments    : none
* Return Value : none
******************************************************************************/
void sci_111_init_ports(void)
{

  /* init channel 1 */
  MPC.PWPR.BYTE = 0x00;       // unlock this reg
  MPC.PWPR.BYTE = 0x40;       // unlock PFS regs

  PORT1.PODR.BIT.B6 = 0;      // set line low (dflt)
  MPC.P16PFS.BYTE = 0x0A;     // Pin Func Select P16 TXD1
  MPC.P15PFS.BYTE = 0x0A;     // Pin Func Select P15 RXD1
  PORT1.PDR.BIT.B6 = 1;       // set TX pin direction to output
  PORT1.PDR.BIT.B5 = 0;       // set RX pin direction to input (dflt)
  PORT1.PMR.BIT.B6 = 1;       // set TX pin mode to peripheral
  PORT1.PMR.BIT.B5 = 1;       // set RX pin mode to peripheral

  MPC.PWPR.BYTE = 0;          // lock PFS regs
  MPC.PWPR.BYTE = 0x80;       // lock this reg

  return;
}


/*****************************************************************************
* Function Name: charput
* Description  : Maps stream I/O stdout to the serial port console
* Arguments    : char theChar - byte to send
* Return Value : none
******************************************************************************/
void charput (char theChar)
{
  sci_err_t err;

  do
  {
    /* insure that a full tx buffer doesn't prevent us from completing our send */
    err = R_SCI_Send(console, (uint8_t *)&theChar, 1);
  } while (err == SCI_ERR_INSUFFICIENT_SPACE);
}

/*****************************************************************************
* Function Name: charget
* Description  : Maps stream I/O stdin to the serial port console
* Arguments    : none
* Return Value : char - character read from serial port. If none available,
*                       returns a zero (does NOT block)
******************************************************************************/
char charget ()
{
  sci_err_t err;
  char theChar;

  err = R_SCI_Receive(console, (uint8_t *)&theChar, 1u);
  if (err == SCI_SUCCESS)
  {
    return(theChar);
  }
  else
  {
    return(0);
  }
}

/*****************************************************************************
* Function Name: rtc_111_init
* Description  : Setup the RTC to run in calendar mode
* Arguments    :
* Return Value : None
******************************************************************************/
static void rtc_111_init(void)
{
  uint32_t    w_count;

  /* Disable ALM, PRD and CUP interrupts */
  IEN(RTC,ALM) = 0U;
  IEN(RTC,PRD) = 0U;
  IEN(RTC,CUP) = 0U;

  /* Set sub-clock oscillator */
  RTC.RCR3.BYTE = _00_RTC_SUBCLOCK_DRIVE_MID | _01_RTC_SUBCLOCK_ENABLE;

  for (w_count = 0U; w_count <= _000001B8_RTC_RTCSCLK_STABLE_CYCLE; w_count++)
  {
    nop();
  }

  /* Stop all counters */
  RTC.RCR2.BIT.START = 0U;

  while (RTC.RCR2.BIT.START != 0U);

  /* Select count mode */
  RTC.RCR2.BIT.CNTMD = 0U;

  while (RTC.RCR2.BIT.CNTMD != 0U);

  /* Execute RTC software reset */
  RTC.RCR2.BIT.RESET = 1U;

  while (RTC.RCR2.BIT.RESET != 0U);

  /* Set control registers */
  RTC.RCR1.BYTE = _00_RTC_ALARM_INT_DISABLE | _00_RTC_CARRY_INT_DISABLE | _00_RTC_PERIOD_INT_DISABLE |
    _00_RTC_PERIODIC_INT_PERIOD_DISABLE;
  RTC.RCR2.BYTE = _00_RTC_HOUR_MODE_12;
}

/*****************************************************************************
* Function Name: sw1_callback(void * pdata)
* Description  : Callback function for the SW1 switch push.
* Arguments    :
* Return Value : None
******************************************************************************/
void sw1_callback(void * pdata)
{
  awake = true;
  sw1_pressed = true;
}

/*****************************************************************************
* Function Name: sw2_callback(void * pdata)
* Description  : Callback function for the SW2 switch push.
* Arguments    :
* Return Value : None
******************************************************************************/
void sw2_callback(void * pdata)
{
  awake = true;
}


/*****************************************************************************
* Function Name: sw3_callback(void * pdata)
* Description  : Callback function for the SW3 switch push.
* Arguments    :
* Return Value : None
******************************************************************************/
void sw3_callback(void * pdata)
{
  awake = true;

}


/******************************************************************************
* Function Name: ConfigureUnusePort
* Description  : Configures unused port.
* Arguments    : None
* Return Value : None
* Note         : P15 is set as an input port, in order to avoid the collision
*                with the R1OUT output of MAX3222.
******************************************************************************/
static void ConfigureUnusePort(void)
{
  //PORT0 : P03, P05
  PORT0.PMR.BYTE				= 0x00;
  PORT0.PODR.BYTE				= 0x00;
  PORT0.PDR.BYTE			   |= 0x28;


  //PORT1 : P14, P15(MAX3222CPWR-R1OUT), P16, P17(LED3)
  PORT1.PMR.BYTE				= 0x00;
  PORT1.PODR.BYTE				= 0x00;
  PORT1.PDR.BYTE			   |= 0xD0;


  //PORT2 : P26, P27
  PORT2.PMR.BYTE				= 0x00;
  PORT2.PODR.BYTE				= 0x00;
  PORT2.PDR.BYTE			   |= 0xC0;

  // This is the one that we need in order to get proper low current readings when in low power mode.
  //PORT4 : P40(LCD-D0), P41(LCD-D1), P42(LCD-D2), P43(LCD-D3), P44, P46
  PORT4.PMR.BYTE				= 0x00;
  PORT4.PODR.BYTE				= 0x00;
  PORT4.PDR.BYTE			   |= 0x5F;


  //PORT5 : P54(LED2), P55
  PORT5.PMR.BYTE				= 0x00;
  PORT5.PODR.BYTE				= 0x00;
  PORT5.PDR.BYTE			   |= 0x30;

  //PORTA : PA0(LED1), PA1, PA3, PA4, PA6
  PORTA.PMR.BYTE				= 0x00;
  PORTA.PODR.BYTE				= 0x00;
  PORTA.PDR.BYTE			   |= 0x5B;

  //PORTB : PB0, PB1(LCD-E), PB3, PB5, PB6, PB7(LED0)
  PORTB.PMR.BYTE				= 0x00;
  PORTB.PODR.BYTE				= 0x00;
  PORTB.PDR.BYTE			   |= 0xEB;

  //PORTC : PC2, PC3, PC4, PC5(LCD-RS), PC6, PC7
  PORTC.PMR.BYTE				= 0x00;
  PORTC.PODR.BYTE				= 0x00;
  PORTC.PDR.BYTE			   |= 0xFC;

  //PORTE : PE0, PE1, PE2, PE3, PE4(IRQ4), PE5, PE6, PE7
  PORTE.PMR.BYTE				= 0x00;
  PORTE.PODR.BYTE				= 0x00;
  PORTE.PDR.BYTE			   |= 0xFF;

}

/******************************************************************************
* Function Name: checkErrorCode
* Description  : Indicate the command has been Nak'd. If there is additional
*                error info available, include it.
* Arguments    : None
* Return Value : None
* Note         : None
*
******************************************************************************/
static void checkErrorCode(void)
{
  printf("NAK");		// At least this
  if ((clock_request_status > CGC_SUCCESS) && (clock_request_status  <= CGC_ERR_ILL_PARAM))
  {
    printf(" - ");
    printf(clk_err_strings[clock_request_status]);
  }
  else
    if ((powmode_request_status > LPC_SUCCESS) && (powmode_request_status  <= LPC_ERR_ILL_PARAM))
    {
      printf(" - ");
      printf(lpc_err_strings[powmode_request_status]);
    }

    printf("\r\n");
}

/******************************************************************************
* Function Name: ConfigureIRQandClkPins
* Description  : Configure the port pins to use clkout and IRQs
*                for SW1, SW2 and SW3.
* Arguments    : None
* Return Value : None
******************************************************************************/
static void ConfigureIRQandClkPins(void)
{
  MPC.PWPR.BYTE = 0x00;       // unlock this reg
  MPC.PWPR.BYTE = 0x40;       // unlock PFS regs

  PORTC.PODR.BIT.B4 = 0;      // set line low (dflt)
  MPC.PC4PFS.BIT.PSEL = 0x09;	// CLKOUT for PC4
  PORTC.PDR.BIT.B4 = 1;		// Set pin direction to output
  PORTC.PMR.BIT.B4 = 1;       // set pin mode to peripheral

  // Setup the I/O for the SW1, SW2 and SW3 switches
  PORT3.PDR.BIT.B0   = 0;     /* Make the pin an input */
  PORT3.PMR.BIT.B0   = 0;     /* Set port mode register for GPIO mode. */
  MPC.P30PFS.BYTE  = 0x40;    /* P30 is used as IRQ pin */

  PORT3.PDR.BIT.B1   = 0;     /* Make the pin an input */
  PORT3.PMR.BIT.B1   = 0;     /* Set port mode register for GPIO mode. */
  MPC.P31PFS.BYTE  = 0x40;    /* P31 is used as IRQ pin */

  PORTE.PDR.BIT.B4   = 0;     /* Make the pin an input */
  PORTE.PMR.BIT.B4   = 0;     /* Set port mode register for GPIO mode. */
  MPC.PE4PFS.BYTE  = 0x40;    /* PE4 is used as IRQ pin */

  MPC.PWPR.BYTE = 0;          // lock PFS regs
  MPC.PWPR.BYTE = 0x80;       // lock this reg

}
