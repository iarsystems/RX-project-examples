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
* File Name     : main_MTU_Normal_Mode.c 
* Version       : 1.1
* Device(s)     : R5F562N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.1
* OS            : None
* H/W Platform  : YRDKRX62N
* Description   : This sample generates a 1KHz square waveform using the MTU.
*
* Operation     : 1. Build and download the sample code to the RDK+.
*                
*                 2. Click 'Reset Go' to start the program.
*
*                 3. The 1Khz waveform can be observed with an oscillscope
*                    connected to the header JN2, pin 8. 
*                   (JN2 pin 4 can be used for a ground.)
*
*******************************************************************************
* History : DD.MMM.YYYY     Version     Description
*         : 12.May.2010     1.00        First release
*         : 02.Dec.2010     1.10        Update to use new GlyphLib_v2
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
#include <stdbool.h>

/* Defines debug LCD functions used in this file */
#include "lcd.h"
/* Defines timer functions used in this file */
#include "MTU_Normal_Mode.h"
/* RSPI API library support */
#include "YRDKRX62N_RSPI_API.h"
/* Board specific definitions */
#include "YRDKRX62N.h"

#include "machine.h"
#include "intrinsics.h"
#include "hwsetup.h"

/*******************************************************************************
* Outline      : main
* Description  : Main program function. This function first displays on the 
*                debug LCD; then calls the timer initialisation function, which
*                starts the 1KHz output waveform.
* Argument     : none
* Return value : none
*******************************************************************************/
void main(void)
{
  HardwareSetup();

  /* Enable Interrupt */
  __enable_interrupt();

  /* Initialise the LCD display on RSPI bus */
  YRDKRX62N_RSPI_Init(RSPI_CHANNEL_0);
  InitialiseLCD();

  /* Display instructions on the debug LCD */
  DisplayLCD(LCD_LINE1, "MTU   ");
  DisplayLCD(LCD_LINE2, "Square Wave ");
  DisplayLCD(LCD_LINE4, "JN2 pins -- ");
  DisplayLCD(LCD_LINE5, " 4: Ground ");    
  DisplayLCD(LCD_LINE6, " 8: 1 KHz ");        

  /* Initialise the timer to generate the output waveform */
  Init_MTU_Normal_Mode();

  /* Infinite while loop */
  while (1);    
}
/*******************************************************************************
* End of function main
*******************************************************************************/
