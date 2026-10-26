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
/*******************************************************************************
* Copyright (C) 2010 Renesas Electronics Corporation. All rights reserved.    */ 
/*******************************************************************************
* File Name     : main_tmr_1_shot.c
* Version       : 1.10
* Device        : R5F562N8
* Tool Chain    : RX Family C Compiler
* H/W Platform  : YRDKRX62N
* Description   : This sample demonstrates use of the TMR unit, by creating one
*                 timer to restore the debug LCD.
*      
* Operation     : 1.  Build and download the sample code to the YRDK.
*
*                 2.  Click 'Reset Go' to start the software.
*         
*                 3.  The debug LCD will show 
*                       "TMR 1 Shot "
*                       "Press SW3  "
*          
*                 4.  Press switch 3 and the debug LCD will display a new message
*                     and the one shot timer is started.
*
*                  5. When the one shot timer fires, the debut LCD is restored to 
*                     the original display.
*******************************************************************************/
/*******************************************************************************
* History    : 08.Oct.2010  Ver. 1.00   First Release
*            : 30.Nov.2010  Ver. 1.10   Updated to use new GlyphLib_v2.
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
/* Defines debug LCD functions used in this file */
#include "lcd.h"
/* Defines TMR functions used in this file */
#include "tmr_1_shot.h"
/* RSPI API library support */
#include "YRDKRX62N_RSPI_API.h"
/* Board specific definitions */
#include "YRDKRX62N.h"

#include "machine.h"
#include "intrinsics.h"
#include "hwsetup.h"

uint8_t lcd_buffer1[] = "TMR 1 Shot  ";
uint8_t lcd_buffer2[] = "Press SW3   ";

/*******************************************************************************
* Outline       : main
* Description   : The main program function. The function first initialises the 
*                 debug LCD, then displays the name of the sample on the display.
*                 The function then executes the initialisation function, Init_
*                 tmr_1_shot, which configures the TMR unit. The MCU then returns
*                 to the main function, where it enters an infinite while loop.
* Argument      : none
* Return value  : none
*******************************************************************************/
void main(void)
{
  HardwareSetup();

  /* Enable Interrupt */
  __enable_interrupt();  

  /* Initialise the LCD on the RSPI bus */
  YRDKRX62N_RSPI_Init(RSPI_CHANNEL_0);
  InitialiseLCD();

  /* Display instructions onto the LCD */
  DisplayLCD(LCD_LINE1, lcd_buffer1);
  DisplayLCD(LCD_LINE2, lcd_buffer2);

  /* Initialise TMR unit for soperation */
  Init_TMR_1_Shot();

  /* Infinite while loop */
  while (1);

}
/*******************************************************************************
* End of function main
*******************************************************************************/

