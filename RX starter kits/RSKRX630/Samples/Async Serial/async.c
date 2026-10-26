/* Adapted for use with IAR Embedded Workbench */
/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*******************************************************************************/
/* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.   */
/*******************************************************************************
* File Name    : async.c
* Version     : 1.00
* Device     : R5F5630E
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX630
* Description   : Defines SCI functions used in this sample.
*******************************************************************************/
/*******************************************************************************
* History     : 23 Jan. 2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
System Includes 
*******************************************************************************/
/* Defines standard boolean variable types used in this file */
#include <stdbool.h>
/* Following header file provides standard integer type definitions. */
#include <stdint.h>
/* Following header file provides string type definitions. */
#include <string.h>

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/
/* Defines RX630 port registers */
#include "iorx630.h"
/* Defines RX630 LEDs and switches */
#include "rskrx630def.h"
/* Declares functions defined in this file */
#include "async.h"

/*******************************************************************************
User Defines
*******************************************************************************/
/* Define a macro to indicate selection of a periodic timer mode */
#define periodic_mode 0x1

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Declaration of the SCI status flag global variable */
volatile bool gSCI_Flag = true;
/* Declare transmit and receive buffer variables used in this file */
uint8_t gSCI_TXBuffer[2]={0x2F, 0x0};
/* Declare a receive buffer variable */
volatile uint8_t gSCI_RXBuffer;
/* Declaration of the command string to clear the terminal screen */
const uint8_t cmd_clr_scr[] = {27, 91, 50, 74, 0, 27, 91, 72, 0};

/* Declare a variable used to select a oneshot or priodic timer */
uint8_t gTimer_Mode_Flag = 0;
/* Declare a variable used to count the number of elapsed timer periods */
volatile uint32_t gPeriodic_Delay;
/* Declare a variable to store the global count value */
volatile uint32_t gDelay_Counter = 0;


/*******************************************************************************
* Function Prototypes
*******************************************************************************/
/* Init_Timer function prototype */
static void Init_Timer(void);
/* Transmit_Async function prototype */
static void Transmit_Async(void);

/* Timer_Delay function prototype */
static void Timer_Delay(uint32_t User_Delay, uint8_t Unit, uint8_t timer_mode);
/* Text_Write function prptotype */
static void Text_Write(uint8_t* msg_string);

/*******************************************************************************
* Outline      : Init_Async
* Description  : This function initialises the SCI channel connected to the 
*         RS232 connector on the RSK. The channel is configured for 
*         transmission and reception, and instructions are sent to the
*         terminal.
* Argument     : none
* Return value : none
*******************************************************************************/
void Init_Async(void)
{  
  /* Initialise CMT2 */
  Init_Timer();
  
  /* Protection off */
  SYSTEM.PRCR.WORD = 0xA503;
  
  /* Cancel the SCI0 module clock stop mode */
  MSTP_SCI0 = 0;
  
  /* Protection on */
  SYSTEM.PRCR.WORD = 0xA500;
  
  /* Configure SCI0's TXD0, RXD0 and SCK0 pins for peripheral functions */
  PORT2.PMR.BYTE = 0x07;
  
  /* PWPR.PFSWE write protect off */
  MPC.PWPR.BYTE = 0x00;  
  
  /* PFS register write protect off */
  MPC.PWPR.BYTE = 0x40;  
  
  /* Configure SCI0's communication pins */
  /* Set Port2's pin 0 function to TXD0 */
  MPC.P20PFS.BYTE = 0x0A; 
  
  /* Set Port2's pin 1 function to RXD0 */
  MPC.P21PFS.BYTE = 0x0A; 
  
  /* Set Port2's pin 2 function to SCK0 */
  MPC.P22PFS.BYTE = 0x0A; 
  
  /* Turn on PFS register write protect */
  MPC.PWPR.BYTE = 0x80;  
  
  /* Cancel SCI0 module stop mode */
  SYSTEM.MSTPCRB.LONG = 0x7FFFFFFF;
  
  /* Disable All bits and CKE0,1 = 00 */
    SCI0.SCR.BYTE = 0x00;                                  

  /* Set output port TXD0(P20) */
  PORT2.PDR.BIT.B0 = 1;
  
  /* Set Receive port RXD0(P21)*/
  PORT2.PDR.BIT.B1 = 1;

  /* disable All bits and CKE0,1 = 00 once more */
    SCI0.SCR.BYTE = 0x00;                                  

  /* CKS[0-1] = 00: PCLK 0 div        
       1Sop 8bits non-parity     */
    SCI0.SMR.BYTE = 0x00;   
                                                            
    /* SMIF = 0: Serial  */
    SCI0.SCMR.BYTE = 0xf2;                                   

    /* PCLK = 48MHz, Set baudrate to 19200 bps 
       77 = 48MHz / (64*0.5*19200)-1 */
    SCI0.BRR = 77;                                     
                              
  /* Declare a loop count variable */
  uint16_t i=0;

  /* Allow a small delay */
    for ( i = 0; i < 20; i++ )
  {
  }                               

  /* Clear bits ORER, PER and FER */
    SCI0.SSR.BYTE &= 0xC7;                                    
    
  /* Set SCI0 RXI0 interrupt priority level to 7 */  
  IPR(SCI0,RXI0) = 0x7;
  /* Enable SCI0 RXI0 interrupts */
  IEN(SCI0,RXI0) = 0x1;
  /* Clear SCI0 RXI0 interrupt flag */
  IR(SCI0,RXI0) = 0x0;

  /* Set bits TIE, RIE, RE and TE. Enable receive interrupts */
    SCI0.SCR.BYTE = 0xF0;  
  
  /* Clear the text on terminal window */
  Text_Write((uint8_t*)cmd_clr_scr);                                  

  /* Display splash screen on terminal window */
  Text_Write("Renesas RX630 Async Serial \n\r");
        
  /* Inform user on how to stop transmission */            
  Text_Write("Press 'z' to stop and any key to resume\r\n\n");
  
  /* Initialise the transmit buffer array to end in a NULL character */
  gSCI_TXBuffer[1] = 0x00;
  
  /* Start periodic CMT2 count */
  Timer_Delay(300, 'm', periodic_mode);
}
/*******************************************************************************
* End of function Init_Async
*******************************************************************************/

/*******************************************************************************
* Outline      : Init_Timer
* Description  : This function initialises the CMT channel 2.
* Argument     : none
* Return value : none
*******************************************************************************/
static void Init_Timer(void)
{
  /* Protection off */
  SYSTEM.PRCR.WORD = 0xA503;
  
  /* Cancel the CMT2 module clock stop mode */
  MSTP_CMT2 = 0;
  
  /* Protection on */
  SYSTEM.PRCR.WORD = 0xA500;

  /* Set the CMT2 interrupt level to 4 */
  IPR(CMT2,CMI2) = 0x4;
  
  /* Enable CMT2 interrupts */
  IEN(CMT2,CMI2) = 0x1;
  
  /* clear CMT2 interrupt request flag */
  IR(CMT2,CMI2) = 0x0;  
}
/*******************************************************************************
* End of function Init_Timer
*******************************************************************************/

/*******************************************************************************
* Outline     : Transmit_Async 
* Description   : Sends incrementing numbers across the UART link to the 
*          terminal window.
* Argument    : none
* Return value  : none
*******************************************************************************/
static void Transmit_Async(void)
{  
  /* Check SCI status flag */
  if(gSCI_Flag)
  {
    /* Increment number in transmit buffer */
    gSCI_TXBuffer[0]++;
    
    /* Check if hex value of transmit buffer is greater than 0x39 (9) */
    if(gSCI_TXBuffer[0] > 0x39)
    {
      /* Insert a new line and return the cursor left */
      Text_Write("\r\n");
          
      /* Reset the ASCII character in the transmit buffer back to 0 */
      gSCI_TXBuffer[0] = 0x30;
    }
    
    /* Send the transmit buffer to the terminal */
    Text_Write(gSCI_TXBuffer);
        
    /* Turn on LED0 */
    LED0 = LED_ON;  
  
    /* Turn off LED1 */
    LED1 = LED_OFF;    
  }
}
/********************************************************************************
* End of function Transmit_Async
********************************************************************************/

/********************************************************************************
* Outline    : Timer_Delay
* Description  : Function used to create delays in milliseconds or 
*          microseconds depending on the user selection for controlling 
*          the debug LCD function's calls. 
* Argument      : uint32_t   -   _Delay_Period
*          int8     -   Unit  
*          int8     -   timer_mode  
* Return value  : none
********************************************************************************/
static void Timer_Delay(uint32_t User_Delay, uint8_t Unit, uint8_t timer_mode)
{
  /* Clear the timer's count */
  gDelay_Counter = 0;
    
  /* Check if microseconds delay is required */
  if(Unit == 'u')
  {  
    /* Select the PCLK clock division as PCLK/8 = 6MHz */ 
    CMT2.CMCR.BIT.CKS = 0x0;
    
    /* Store a copy of the user delay value to gPeriodic_Delay */
    gPeriodic_Delay = User_Delay * 6;
  
    /* Specify the timer period */
    CMT2.CMCOR = gPeriodic_Delay;
  }
  
  /* Check if milliseconds delay is required */
  if(Unit == 'm')
  {
    /* Select the PCLK clock division as PCLK/128 = 375KHz */ 
    CMT2.CMCR.BIT.CKS = 0x2;
    
    /* Stor a copy of the user delay value to gPeriodic_Delay */
    gPeriodic_Delay = User_Delay * 375;
  
    /* Specify the timer period */
    CMT2.CMCOR = gPeriodic_Delay;
  }
  
  /* Enable the compare match interrupt */
  CMT2.CMCR.BIT.CMIE = 1;

  /* Start CMT2 count */
  CMT.CMSTR1.BIT.STR2 = 1;

  /* Skip the following instructions if a periodic timer is required */
  if((timer_mode != periodic_mode))
  {
    /* Wait for the timer to timeout */
    while((gDelay_Counter != gPeriodic_Delay))
    {
      /* Wait */
    }  

    /* Stop CMT2 count */
    CMT.CMSTR1.BIT.STR2 = 0;
  }      
}
/*******************************************************************************
* End of function Timer_Delay
*******************************************************************************/

/*******************************************************************************
* Outline    : Text_Write
* Description  : Transmits null-terminated string.
* Argument    : (uint8_t*) msg_string - null terminated string
* Return value  : None
*******************************************************************************/
static void Text_Write(uint8_t* msg_string)
{
  /* Declare a count variable */
  uint8_t i = 0;
  
  /* Declare a count variable */
  uint16_t j = 0;

  /* This loop reads the text string and puts it into the UART's 
     transmit buffer */
  for (i=0; msg_string[i]; i++)
  {    
    /* Check that the last transmission was completed */
    while(!(SCI0.SSR.BYTE & 0x80));
    
    /* Transmit the next character */
    SCI0.TDR = msg_string[i];
    
    /* Count up to 1000 */
    for (j = 0; j < 1000; j++)
    {
      /* delay */
    }
  }
}
/*******************************************************************************
* End of Function Text_Write
*******************************************************************************/

/*******************************************************************************
* Outline     : Excep_SCI0_RXI0
* Description   : SCI0 receive interrupt handler, called when a character is 
*          received from the terminal program. Sets rx_flag if character 
*          is 'z' entered.
* Argument    : none
* Return value  : none
*******************************************************************************/
#pragma vector=VECT_SCI0_RXI0
__interrupt void Excep_SCI0_RXI0(void)
{  
  /* Store the received character */
  gSCI_RXBuffer = SCI0.RDR;
  
  /* Check if last received character was 'z' */
  if(gSCI_RXBuffer == 'z')
  {
    /* Set SCI status flag to 0 */
    gSCI_Flag = false;
    
    /* Turn off LED0 */
    LED0 = LED_OFF;
      
    /* Turn on LED1 */
    LED1 = LED_ON;
  }
  
  /* Check if the SCI status flag is 0, and the last received character
     was not 'z' */
  else if(!gSCI_Flag)
  {
    /* Set SCI status flag to 1 */
    gSCI_Flag = true;
  }
}
/*******************************************************************************
* End of function Excep_SCI0_RXI0
*******************************************************************************/

/*******************************************************************************
* Outline    : Excep_CMTU1_CMT2
* Description  : CMT0 interrupt handler. This function calls the Transmit_Async 
*          function and increments the gDelay_Counter variable before 
*          exiting.
* Argument      : none  
* Return value  : none
*******************************************************************************/
#pragma vector=VECT_CMT2_CMI2
__interrupt void Excep_CMTU1_CMT2(void)
{  
  /* Transmit a byte */
  Transmit_Async();
  
  /* Increment counter */
  gDelay_Counter++;
}
/*******************************************************************************
* End of function Excep_CMTU1_CMT2
*******************************************************************************/
