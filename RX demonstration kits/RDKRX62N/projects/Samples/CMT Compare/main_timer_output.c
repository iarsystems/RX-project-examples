/* Adapted for use with IAR Embedded Workbench */
/****************************************************************************** 
* DISCLAIMER

* This software is supplied by Renesas Electronics Corp. and is
* only intended for use with Renesas products.  

* No other uses are authorized.

* This software is owned by Renesas Electronics Corp. and is 
* protected under the applicable laws, including copyright laws.

* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY,
* INCLUDING BUT NOT LIMITED TO WAWRRANTIES OF MERCHANTABILITY, 
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
* File Name     : main_timer_mode.c
* Version       : 1.1
* Device(s)     : R5F562N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.1
* OS            : None
* H/W Platform  : YRDKRX62N
* Description   : This sample triggers a callback function to toggle the user
*                 LEDs everytime a compare match occurs. 
* Operation     : 1.  Build and download the sample code to the YRDK.
*
*                 2.  Click 'Reset Go' to start the software.
*
*                 3.  Observe the user LEDs, which will flash everytime a compare
*                     match occurs in the CMT channel.
* Limitations   : None
*******************************************************************************
* History : DD.MMM.YYYY     Description
*         : 08.Oct.2010     First release
*         : 02.Dec.2010     Second YRDK release
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
#include <stdbool.h>

/* Defines debug LCD functions used in this file */
#include "lcd.h"
/* Defines comapre match timer functions used in this file */
#include "timer_compare.h"

/* RSPI API library support */
#include "YRDKRX62N_RSPI_API.h"

/* Board specific definitions */
#include "YRDKRX62N.h"

#include "machine.h"
#include "intrinsics.h"
#include "hwsetup.h"

/*******************************************************************************
* Outline       : main
* Description   : Main program function. The function first displays instructions
*                 on the debug LCD, then initialises a CMT channel to execute a
*                 callback function on every compare match. The Callback function
*                 toggles the state of the user LEDs.
* Argument      : none
* Return value  : none
*******************************************************************************/
void main(void)
{
  HardwareSetup();

  /* Enable Interrupt */
  __enable_interrupt();

  /* Initialize RSPI peripheral */
  YRDKRX62N_RSPI_Init(RSPI_CHANNEL_0);

  /* Initialise the debug LCD */
  InitialiseLCD();

  /* Display instructions on the debug LCD */
  DisplayLCD(LCD_LINE1, "CMT Compare ");

  DisplayLCD(LCD_LINE3, "CMT match");
  DisplayLCD(LCD_LINE4, "every 100 uS");
  DisplayLCD(LCD_LINE5, "toggles LEDs");

  /* Configure the CMT timer for compare match operation */
  Init_CompareMatchTimer();

  /* Infinite while loop */
  while (1);  
}
/*******************************************************************************
* End of function main
*******************************************************************************/
