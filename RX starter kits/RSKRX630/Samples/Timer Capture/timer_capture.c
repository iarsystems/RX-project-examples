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
* File Name    : timer_capture.c
* Version     : 1.00
* Device     : R5F5630E
* Tool-Chain   : Renesas RX Toolchain 1.2.0.0
* H/W Platform  : RSKRX630
* Description   : Defines MTU3 configuration functions
*******************************************************************************/
/*******************************************************************************
* History     : 23 Jan. 2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* System Includes
*******************************************************************************/
/* Following header file provides standard integer type definitions. */
#include <stdint.h>
/* Following header file provides string type definitions. */
#include <string.h>
/* Following header file provides Boolen type definitions */
#include <stdbool.h>

/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/
/* Defines RX630 LEDs and switches */
#include "rskrx630def.h"
/* Defines RX630 port registers */
#include "iorx630.h"
/* LCD controlling function prototypes & macro defines */
#include "lcd.h"
/* Switch handler function definitions */
#include "switch.h"
/* Declares function prototypes defined in this file */
#include "timer_capture.h"

/******************************************************************************
* Local Function Prototypes
******************************************************************************/
/* 16 bit integer to character string function prototype declaration */
static void uint16_ToBCDString(uint8_t *, uint8_t, uint16_t);
/* Start timer switch press callback function prototype */
void StartTimer_TimerCapture(void);
/* Stop timer switch release callback function prototype */
void StopTimer_TimerCapture(void);

/******************************************************************************
Global Variables
******************************************************************************/
/* Variable for storing the captured interval value */
uint16_t gInputCaptureValue = 0;
/* Switch status flag variable (imported) */
extern volatile uint8_t gSwitchFlag;

/*******************************************************************************
* Outline     : Init_TimerCapture
* Description   : Function configures TPU3 for input capture used to measure the 
*          interval between the falling and rising edge detected on the 
*          TIOCB3 pin. The TIOCB3 is configured as an input pin and set 
*          for peripheral use. The timer clock source is configured as 
*          PCLK/4096 (11.718KHz).
* Argument    : none
* Return value  : none
*******************************************************************************/
void Init_TimerCapture(void)
{    
  /* Protection off */
  SYSTEM.PRCR.WORD = 0xA503;

  /* Cancel the MTU3 module clock stop mode */
  MSTP_TPU3 = 0;
    
  /* Protection on */
  SYSTEM.PRCR.WORD = 0xA500;
  
  /* Select PCLK/4096=11.718KHz, clock counted at 
     falling edge, no clearing of TCNT counter. */
  TPU3.TCR.BYTE = 0x07;
  
  /* Select normal operation and TIOCB3 pin 
     as the TGRC input capture input source */
  TPU3.TMDR.BYTE = 0x00;
  
  /* Configure switch press down callback function */
  SetSwitchPressCallback(StartTimer_TimerCapture);
  
  /* Configure switch press release callback function */
  SetSwitchReleaseCallback(StopTimer_TimerCapture);
}
/*******************************************************************************
* End of function Init_TimerCapture
*******************************************************************************/

/*******************************************************************************
* Outline     : StartTimer_TimerCapture
* Description   : This is a callback function, which executes when any of the
*          user switches is pressed down. The function checks if the
*          switch pressed was SW1, and if so starts the TPU3 timer.
* Argument    : none
* Return value  : none
*******************************************************************************/
void StartTimer_TimerCapture(void)
{
  /* Check if SW1 is held down */
  if(gSwitchFlag & SWITCHHOLD_1)
  {
    /* Clear the counter */
    TPU3.TCNT = 0x0;
  
    /* Start the timer */
    TPUA.TSTR.BIT.CST3 = 0x1;  
  }
}
/*******************************************************************************
* End of function StartTimer_TimerCapture
*******************************************************************************/

/*******************************************************************************
* Outline     : Init_TimerCapture
* Description   : This is a callback function, which executes when any of the
*          user switches are released. The function checks if the switch
*          released was SW1, and if so stops the TPU3 time, and updates
*          the debug LCD with timer value converted to milliseconds.
*          (Values above 5 seconds will simlpy display " >5 sec")
* Argument    : none
* Return value  : none
*******************************************************************************/
void StopTimer_TimerCapture(void)
{
  /* Check if SW1 was released */
  if(gSwitchFlag & SWITCHPRESS_1)
  {
    /* Stop the counter */
    TPUA.TSTR.BIT.CST3 = 0x0;
    
    /* Read the captured timer count */
    gInputCaptureValue = TPU3.TCNT;
    
    /* Check if the count value is greater than 5500ms */
    if((gInputCaptureValue > 64453u) || (TPU3.TSR.BIT.TCFV == 1u))
    {
      /* Display that the count value is greater than 5 seconds on the debug
         LCD screen */
      Display_LCD(LCD_LINE2, " >5 sec ");
    
      /* Clear the TCNT overflow flag */
      TPU3.TSR.BIT.TCFV = 0;
    }
    /* Count value is less than 5500ms */
    else
    {
      /* Declare temporary local lcd buffer */
      uint8_t lcd_buffer[] = "      ms";
    
      /* Convert the value to time */
      gInputCaptureValue *= 0.0853;
    
      /* Convert the timer value into a BCD character string, and insert into 
         the temporary string lcd_buffer */
      uint16_ToBCDString(lcd_buffer, 1u, gInputCaptureValue);
    
      /* Display the contents of lcd_buffer onto the debug LCD */
      Display_LCD(LCD_LINE2, lcd_buffer);
    }
  }
  
  /* Clear switch release bits in switch flag */
  gSwitchFlag &= 0x00;
}
/*******************************************************************************
* End of function StopTimer_TimerCapture
*******************************************************************************/

/*******************************************************************************
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
*******************************************************************************/
static void uint16_ToBCDString(uint8_t * output_string, uint8_t pos, 
      uint16_t number)
{
  /* Local digit position variable */
  uint8_t digit = 5u;
  
  /* Digit is a leading zero indicator variable */
  bool leading_zero = true;
  
  /* Local string buffer variable */
  uint8_t buffer_string[5] = "00000";
  
  /* Decrement digit each iteration, until digit is zero */
  while(digit--)
  {
    /* Write BCD value to string element */
    buffer_string[digit] =(uint8_t) (0x30 + (number % 10));
    
    /* Move to the next decimal digit */
    number /= 10u;
  }
  
  /* Set digit position to zero */
  digit = 0u;
  
  /* Loop through each digit, and replace leading zeros with a space
     character */
  while(digit < 5u)
  {
    /* Check if digit is not a zero character, or if leading_zero indicator is
       false */
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
