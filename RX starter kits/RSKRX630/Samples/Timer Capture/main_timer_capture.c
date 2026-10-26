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
* File Name    : main_timer_capture.c
* Version     : 1.00
* Device     : R5F5630E
* Tool-Chain   : Renesas RX Toolchain 1.2.0.0
* H/W Platform  : RSKRX630
* Description   : This sample uses channel 4 of TPU3 to measure the duration 
*          between SW1 switch press and release.   
*
* Operation    : 1. Compile and download the sample code to the target. Click 
*           'Reset Go' icon on the 'Debug Run' toolbar to start the 
*            software.
*
*          2. Press SW1.   
*        
*          3. The debug LCD will display the captured interval between 
*             the switch press and release. To observe the interval 
*           value in the HEW watch window, right-click on the variable 
*           gInputCaptureValue and select 'Add Watch'.
*
*          2. Go to Step 3) to measure another interval.
*******************************************************************************/
/*******************************************************************************
* History     : 23 Jan. 2012  Ver. 1.00 First Release
*******************************************************************************/

/******************************************************************************
User Includes (Project Level Includes)
******************************************************************************/
/* LCD controlling function prototypes & macro defines */
#include "lcd.h"
/* Prototypes for Timer control functions */
#include "timer_capture.h"

#include "intrinsics.h"

#include "hwsetup.h"

/******************************************************************************
* Outline     : main
* Description   : The main program function. The function displays the splash
*          screen on the LCD display, initialises the TPU3 channel, then
*          waits in an infinite while loop.
* Argument    : none
* Return value  : none
******************************************************************************/
void main(void)
{
  
  HardwareSetup();
  
  __enable_interrupt();
  
  /* Initialise the LCD Display */
  Init_LCD();
  
  /* Display Splash Screen */
  Display_LCD(LCD_LINE1, "Capture ");
  Display_LCD(LCD_LINE2, "Push SW1");
  
  /* Initialise TPU timer registers */
  Init_TimerCapture();
  
  /* Infinite while loop */
  while(1);
}
/******************************************************************************
* End of function main
******************************************************************************/
