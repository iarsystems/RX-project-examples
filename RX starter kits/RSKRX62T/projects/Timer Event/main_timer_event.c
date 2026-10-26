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
* File Name    : main_timer_event.c
* Version     : 2.00
* Device     : R5F562TAADFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX62T
* Description    : This sample demonstrates usage of the timer unit, with an 
*          external clock, by counting switch SW2 presses.
* Operation       : 1. Build and download the sample code to the RSK.
*
*          2. Click 'Reset Go' to start the software. Instructions will be
*           displayed on the debug LCD.
*
*          3. Press switch SW2, and observe the count value displayed on
*           debug LCD. The value will increment each time the switch is
*           pressed.
*
*          4. Keeping pressing the switch, and the value will increment
*           up to 99, then loop back around to 1.
* Note        : Since the clock source for the timer is a non-debounced 
*          switch, count may increment by more than one with each switch
*          press. This does not represent the final accuracy of the 
*          timer. Do not press any other switches other than switch SW2.
******************************************************************************/  
/******************************************************************************
* History     : 15.10.2010 Ver. 1.00 First Releas
              : 21.05.2011 Ver. 2.00 Second Release (Updated to RPDL CS3)
******************************************************************************/

/******************************************************************************
* Project Includes
******************************************************************************/
/* Defines debug LCD functions used in this file */
#include "lcd.h"
/* Defines timer functions used in this file */
#include "timer_event.h"
#include "hwsetup.h"
#include <intrinsics.h>

/******************************************************************************
* Outline     : main
* Description    : Main program function. The function displays instructions
*          on the debug LCD, then initialises a timer channel to count
*          switch presses from SW2. 
* Argument    : none
* Return value  : none
******************************************************************************/
void main(void)
{
  /* Set up hardware */
  HardwareSetup();
  
  /* Enable interrupt */
  __enable_interrupt();
  
  /* Initialise the debug LCD */
  InitialiseLCD();
  
  /* Display instructions on the debug LCD */
  DisplayLCD(LCD_LINE1, "TR Event");
  DisplayLCD(LCD_LINE2, "Push SW2");

  /* Initialise the timer unit */
  Init_TimerEvent();

  /* Infinite while loop */
  while(1);
}
/******************************************************************************
* End of function main
******************************************************************************/
