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
* File Name    : timer_event.c
* Version     : 1.00
* Device     : R5F5630E
* Tool-Chain   : Renesas RX Toolchain 1.2.0.0
* H/W Platform  : RSKRX630
* Description   : Provides timer event catching functions used in this sample.
*******************************************************************************/
/*******************************************************************************
* History     : 23 Jan. 2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
/* Defines RX630 port registers */
#include "iorx630.h"
/* Defines macros relating to the RX630 user LEDs and switches */
#include "rskrx630def.h"
/* Defines LCD functions used in this file */
#include "lcd.h"
/* Defines switch functions and variables used in this file */
#include "switch.h"
/* Provides declarations of functions defined in this file */
#include "timer_event.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Switch press flag global variable */
extern volatile uint8_t gSwitchFlag;
/* Defines count value buffer variable */
uint8_t usCount = 0;
/* Declare a variable to hold the periodic delay specified */
volatile uint32_t gPeriodic_Delay;
/* Declare a variable to store the global delay count value */
volatile uint32_t gDelay_Counter = 0;
/* Declare a variable to indicate displaying of count value */
volatile bool gDisplayCount = 0;

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
/* CMT2 initialisation function prototype */
static void Init_Timer(void);
/* 16 bit integer to character string function prototype declaration */
static void uint16_ToBCDString(uint8_t*, uint8_t, uint16_t);
/* Timer_Delay function prototype */
static void Timer_Delay(uint32_t, uint8_t, uint8_t);

/*******************************************************************************
* Outline      : Init_TimerEvent
* Description  : The function disables swicth 2 interrupts then initialises timer 
*         2 of the MTU3 unit, to be clocked from the falling edge from 
*         switch SW2. It also conigures the CMT timer used to create 
*         a 100ms periodic interrupt which calls the CB_CMTUpdateLCD
*         callback function.
* Argument     : none
* Return value : none
*******************************************************************************/
void Init_TimerEvent(void)
{
  /* Initialise CMT2 */
  Init_Timer();
  
  /* Configure PortA_6 as an input (used as clock input) */
  PORTA.PDR.BIT.B6 = 0x0;
  
  /* Configure PortA_6 for peripheral function */
  PORTA.PMR.BIT.B6 = 0x1;

  /* Enable write to PFSWE bit */
  MPC.PWPR.BYTE = 0x00;
  
  /* Disable write protection to PFS registers */
  MPC.PWPR.BYTE = 0x40;
  
  /* Configure PortA_6 for TMCI3 inputs */
  MPC.PA6PFS.BIT.PSEL = 0x5;
  
  /* Enable write protection to PFS registers */
  MPC.PWPR.BYTE = 0x80;
  
  /* Protection off */
  SYSTEM.PRCR.WORD = 0xA503;

  /* Cancel the TMR3 module clock stop mode */
  MSTP_TMR3 = 0;
  
  /* Cancel the CMT2 module clock stop mode */
  MSTP_CMT2 = 0;  
  
  /* Enable digital filtering for SW2 pin (IRQ12)*/
  ICU.IRQFLTE1.BIT.FLTEN12 = 0x1;
  
  /* Set the sampling clock frequency to PCLK/64=750KHz */
  ICU.IRQFLTC1.BIT.FCLKSEL12 = 0x3;
  
  /* Protection on */
  SYSTEM.PRCR.WORD = 0xA500;
  
  /* Disable SW2 interrupt requests */
  IEN(ICU, IRQ12) = 0x0;
  
  /* TCNT Ccunter clearing by compare match A only. No interrupts used */
  TMR3.TCR.BYTE = 0x08;
  
  /* Set TCNT clock source to falling edge of external clock */
  TMR3.TCCR.BYTE = 0x02;
  
  /* No output after compare match of A or B */
  TMR3.TCSR.BYTE = 0x00;
  
  /* Set compare match A value  */
  TMR3.TCORA = 0x63;
  
  /* Configure a 300 ms periodic delay used 
     to update the ADC result on to the LCD */
  Timer_Delay(300, 'm', PERIODIC_MODE);
}
/*******************************************************************************
* End of function Init_TimerEvent
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

  /* Set CMT2 interrupt priority level to 5*/
  IPR(CMT2,CMI2) = 0x5;
  /* Enable CMT2 interrupts */
  IEN(CMT2,CMI2) = 0x1;  
  /* Clear CMT2 interrupt flag */
  IR(CMT2,CMI2) = 0x0;
  
}
/*******************************************************************************
* End of function Init_Timer
*******************************************************************************/

/******************************************************************************
* Outline    : Timer_Delay
* Description  : Function used to create delays in milliseconds or 
*          microseconds depending on the user selection for controlling 
*          the debug LCD function's calls. 
* Argument      : uint32_t   -   _Delay_Period
*          int8     -   Unit  
* Return value  : none
******************************************************************************/
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
  if((timer_mode != PERIODIC_MODE))
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
/******************************************************************************
* End of function Timer_Delay
******************************************************************************/

/******************************************************************************
* Outline      : uint16_ToBCDString
* Description  : This function converts an 16bit integer into a BCD string,
*         which is inserted into the character array passed as an 
*         argument.
* Argument     : uint8_t * string  - Pointer to array to store BCD string 
*         uint8_t pos    - Array element to start BCD string from.
*         uint16_t number  - 16bit integer to convert.
* Return value : none
* Note       : No input validation is used, so output data can overflow the
*         array passed.
******************************************************************************/
static void uint16_ToBCDString(uint8_t * output_string,
                        uint8_t pos, uint16_t number)
{
  /* Local digit position variable */
  uint8_t digit = 5u;
  
  /* Digit is a leading zero indicator variable */
  bool leading_zero = true;
  
  /* Local string buffer variable */
  uint8_t buffer_string[] = "00000";
  
  /* Decrement digit each iteration, until digit is zero */
  while(digit--)
  {
    /* Write BCD value to string element */
    buffer_string[digit] = (uint8_t)(0x30 + (number % 10));
    
    /* Move to the next decimal digit */
    number /= 10u;
  }
  
  /* Set digit position to zero */
  digit = 0u;
  
  /* Loop through each digit, and replace leading zeros with a space
     character */
  while(digit < 5u)
  {
    /* Check if digit is not a zero character, 
       or if leading_zero indicator is false */
    if((buffer_string[digit] != 0x30) | (!leading_zero))
    {
      /* Copy digit from buffer string to output string */
      output_string[digit + pos] = buffer_string[digit];
      
      /* Set the leading zero indicator variable to false */
      leading_zero = false;  
    }
    
    /* Increment the loop counter */
    digit++;
  }
}
/*******************************************************************************
* End of function uint16_ToBCDString
*******************************************************************************/

/*******************************************************************************
* Outline      : Excep_CMTU1_CMT2
* Description  : CMT2 interrupt handler is called by the CMT period interrupt,
*         every 300ms. The function reads the count value of CMT's
*         channel 2, converts it into a BCD character string and 
*         displays it on the debug LCD.
* Argument      : none  
* Return value  : none
********************************************************************************/
#pragma vector=VECT_CMT2_CMI2
__interrupt void Excep_CMTU1_CMT2(void)
{
    /* Declare temporary character string */
  uint8_t lcd_buffer[] = "Count: 0";
  
  /* Fetch ADC value */
  usCount = TMR3.TCNT;
  
  /* Check if SW2 press was pressed */
  if(usCount == 0x1)
  {
    /* Set flag to true to allow TCNT updates on the LCD */
    gDisplayCount = true;
  }
    
  /* Convert ADC result into a character string, and store in the local
     string lcd_buffer */  
  uint16_ToBCDString(lcd_buffer, 3, (uint16_t)usCount);
  
  /* Check if SW2 press was pressed */
  if(gDisplayCount == true)
  {
    /* Display the contents of the local string lcd_buffer */
    Display_LCD(LCD_LINE2, lcd_buffer);  
  }  
}
/********************************************************************************
* End of function Excep_CMTU1_CMT2
********************************************************************************/
