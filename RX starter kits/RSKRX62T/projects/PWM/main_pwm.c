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
/*******************************************************************************
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.    */ 
/*******************************************************************************
* File Name    : main_pwm.c
* Version     : 2.00
* Device     : R5F562TAADFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX62T
* Description   : This sample demonstrates usage of the timers to generate a PWM
*          output. The MCU will generate a squarewave output, with a duty
*          cycle that varies from 10% to 90%.
* Operation     1. Build and download the sample code to the RSK
*
*         2. Click 'Reset Go' to start the software. Instructions will be
*           displayed on the debug LCD.
*
*         3. The program will create the PWM output on the TMO0 pin,
*          which is accessible through JA2, pin 13.
*
*         4. The waveform will be a squarewave, with a duty cycle that
*          increments from 10% to 90%, and returns to 10% and repeats.
*
*         5. Press switch SW1 to stop the duty cycle from varying. The
*          value of the duty cycle will be displayed on the debug LCD 
*          as a percentage. 
*
*         6. To repeat the sample, restart the software by clicking
*            'Reset Go'
******************************************************************************/  
/******************************************************************************
* History     : 15.10.2010 Ver. 1.00 First Release
              : 21.05.2011 Ver. 2.00 Second Release (Updated to RPDL CS3)
******************************************************************************/

/******************************************************************************
* Project Includes
******************************************************************************/
/* Defines debug LCD functions used in this file */
#include "lcd.h"
/* Defines PWM functions used in this file */
#include "pwm.h"
#include "hwsetup.h"
#include <intrinsics.h>

/******************************************************************************
* Outline     : main
* Description   : The main program function. The function displays the splash
*          screen to the LCD display, initialises the PWM output then
*          waits in an infinite loop.
* Argument    : none
* Return value  : none
******************************************************************************/
void main(void)
{
  /* Set up hardware */
  HardwareSetup();
  
  /* Enable interrupt */
  __enable_interrupt();
  
  /* Initialise the LCD Display */
  InitialiseLCD();
  
  /* Display Splash Screen */
  DisplayLCD(LCD_LINE1, "  PWM   ");
  DisplayLCD(LCD_LINE2, "Push SW1");
  
  /* Initialise the PWM output */
  Init_PWM();
  
  /* Infinite while loop */
  while (1);
}
/******************************************************************************
* End of function main
******************************************************************************/
