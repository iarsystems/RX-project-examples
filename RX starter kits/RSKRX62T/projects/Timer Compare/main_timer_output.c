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
* File Name     : main_timer_mode.c
* Version     : 2.00
* Device     : R5F562TAADFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX62T
* Description   : This sample triggers a callback function to toggle the user
*          LEDs everytime a compare match occurs. 
* Operation      : 1. Build and download the sample code to the RSK.
*
*          2. Click 'Reset Go' to start the software.
*
*          3. Observe the user LEDs, which will flash everytime a compare
*          match occurs in the CMT channel.
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
/* Defines comapre match timer functions used in this file */
#include "timer_output.h"
#include "hwsetup.h"
#include <intrinsics.h>

/******************************************************************************
* Outline     : main
* Description    : Main program function. The function first displays instructions
*          on the debug LCD, then initialises a CMT channel to execute a
*          callback function on every compare match. The Callback function
*          toggles the state of the user LEDs.
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
  
  /* Display the sample name */
  DisplayLCD(LCD_LINE1, "Timer   ");
  DisplayLCD(LCD_LINE2, "Compare ");
  
  /* Configure the CMT timer for compare match operation */
  Init_CompareMatchTimer();
  
  /* Infinite while loop */
  while(1);
}
/******************************************************************************
* End of function main
******************************************************************************/
