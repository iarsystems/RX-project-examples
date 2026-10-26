/******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Electronics Corp. and is
* only intended for use with Renesas products.  

* No other uses are authorized.

* This software is owned by Renesas Electronics Corp. and is 
* protected under the applicable laws, including copyright laws.

* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY,
* INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH
* WARRANTIES ARE EXPRESSLY DISCLAIMED.

* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER 
* RENESAS ELECTRONICS CORP. NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR AND DIRECT, INDIRECT, SPECIAL, INCIDENTAL
* OR COSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE,
* EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE
* POSSIBILITIES OF SUCH DAMAGES.

* Renesas reserves the right, without notice, to make changes to this
* software and to discontinue availability of this software.
* By using this software, you agree to the additional terms and 
* conditions found by accessing the following link:
* http://www.renesas.com/disclaimer
*******************************************************************************/
/* Copyright (C) 2010. Renesas Electronics Corp., All Rights Reserved         */
/******************************************************************************
* File Name     : main_timer_capture.c
* Version       : 1.1
* Device(s)     : R5F562N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.1
* OS            : None
* H/W Platform  : YRDKRX62N
* Description   : Demonstrates capture a timer value from the Compare Match Timer (CMT)
* Operation     : 1.  Build and download the sample code to the YRDK.
*
*                 2.  Click 'Reset Go' to start the software. Instructions will
*                     be displayed on the debug LCD.
*          
*                 3.  Press switch SW1, and observe the result on the debug LCD. 
*                     The time duration, in milliseconds (ms), will be displayed.
*        
*                 4.  Press switch SW1 again to repeat the test.
*
* Note          : The sample is intended as a demonstration, and does not repre-
*                 sent the final accuracy of the MCU.            
*******************************************************************************
* History : DD.MMM.YYYY     Version     Description
*         : 08.Oct.2010     1.00        First release
*         : 02.Dec.2010     1.10        Second YRDK release
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
#include <stdbool.h>

/* Defines debug LCD functions used in this file */
#include "lcd.h"
/* Defines timer functions used in this file */
#include "timer_capture.h"
/* RSPI API library support */
#include "YRDKRX62N_RSPI_API.h"
/* Board specific definitions */
#include "YRDKRX62N.h"

#include "machine.h"
#include "intrinsics.h"
#include "hwsetup.h"

/*******************************************************************************
* Outline       : main
* Description   : Main program function. This function first displays instruct-
*                 ions the debug LCD, then initialises the timer unit. The 
*                 function then waits in an infinite while loop, where the rest 
*                 of the sample's functionality is achieved through interrupts.
* Argument      : none
* Return value  : none
*******************************************************************************/
void main(void)
{

  HardwareSetup();

  /* Enable Interrupt */
  __enable_interrupt();  

  /* Initialise the LCD display on the RSPI bus */
  YRDKRX62N_RSPI_Init(RSPI_CHANNEL_0);
  InitialiseLCD();

  /* Display instructions on the debug LCD */
  DisplayLCD(LCD_LINE1, "Capture ");
  DisplayLCD(LCD_LINE2, "Push SW1");

  /* Initialise the timer unit */
  Init_TimerCapture();

  /* Infinite while loop */
  while (1);
}
/*******************************************************************************
* End of function main
*******************************************************************************/
