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
/*******************************************************************************
* File Name     : main.c
* Version       : 1.1
* Device(s)     : R5F562N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.1
* OS            : None
* H/W Platform  : YRDKRX62N
* Description   : This tutorial sample will demonstrate basic use of the YRDK
*                 hardware and the J-Link debugger. 
* Operation     : 1.  Build and download the tutorial project to the YRDK.
*
*                 2.  Click 'Reset Go' to start the software.
*         
*                 3.  "Renesas RX62N" will be displayed on the debug LCD,
*                     and the user LEDs will flash.
*          
*                 4.  The user LEDs will flash at a fixed rate until either a
*                     switch is pressed, or the LEDs have flashed 200 times.
*          
*                 5.  The software will then vary the rate in which the LEDs flash
*                     by the position of the potentiometer, VR1. Turn the pot in
*                     both directions, and observe the change in flash rate.
*          
*                 6.  While the LEDs flash at a varying rate, the second line of
*                     the debug LCD will show " STATIC ". The second LCD line will
*                     slowly be replaced, letter by letter, with the string 
*                     "TESTTEST".
*          
*                 7.  Once the second line of the debug LCD shows "TESTTEST"
*                     fully, it will return back to showing "RX62N". The LEDs will
*                     continue to flash at a varying rate until the tutorial is
*                     stopped.
*          
*                 8.  In order to repeat the tutorial, click 'Reset Go'.
*******************************************************************************/
/*******************************************************************************
* History : DD.MMM.YYYY     Version     Description
*         : 08.Oct.2010     1.00        First release
*         : 02.Dec.2010     1.10        Second YRDK release
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
#include <stdbool.h>

/* CMT RPDL definitions */
#include "r_pdl_cmt.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* Defines debug LCD functions used in this file */
#include "lcd.h"
/* Defines LED flash functions used in this file */
#include "flashled.h"
/* Defines timer and ADC functions used in this file */
#include "timeradc.h"
/* RSPI API library support */
#include "YRDKRX62N_RSPI_API.h"
/* Board specific definitions */
#include "YRDKRX62N.h"

#include "machine.h"
#include "intrinsics.h"
#include "hwsetup.h"

/*******************************************************************************
* Macro Definitions
*******************************************************************************/
/* Nickname string definition */
#define NICKNAME "RX62N   \0"

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Statics test replacement variable */
uint8_t ucStr[] = " STATIC \0";
/* Statics test const variable */
const uint8_t ucReplace[] = "TEST TEST\0";

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
/* Delcaration of statics test function prototype */
void Statics_Test(void);

/*******************************************************************************
* Outline       : main
* Description   : Main program function. The function first initialises the debug
*                 LCD, and displays the splash screen on the screen. The function
*                 then calls the FlashLED, TimerADC and Statics_Test functions. 
*                 These start by flashing the LEDs at a fixed rate until a switch is
*                 pressed, then the flash rate varies with the position of the   
*                 pot, RV1. Finally, Statics_Test displays " STATICS " on the    
*                 debug LCD, which is replaced with "TESTTEST" a letter at a 
*                 time.
* Argument      : none
* Return value  : none
*******************************************************************************/
void main(void)
{
  HardwareSetup();

  /* Enable Interrupt */
  __enable_interrupt();  

  /* Initialise the debug LCD on the RSPI bus */
  YRDKRX62N_RSPI_Init(RSPI_CHANNEL_0);
  InitialiseLCD();

  /* Display instructions on the debug LCD */
  DisplayLCD(LCD_LINE1, "Renesas ");
  DisplayLCD(LCD_LINE2, NICKNAME);
  DisplayLCD(LCD_LINE4, "CMT->LED");
  DisplayLCD(LCD_LINE5, "Press SW"); 
  DisplayLCD(LCD_LINE6, "to");
  DisplayLCD(LCD_LINE7, "Change Test");

  /* Executes the initial LED flashing sequence */
  FlashLED();  

  DisplayLCD(LCD_LINE4, "Static Test");
  DisplayLCD(LCD_LINE5, "                ");
  DisplayLCD(LCD_LINE6, "Pot -> AD0");
  DisplayLCD(LCD_LINE7, "AD0 -> LEDs");

  /* Executes the ADC-varying LED flash sequence */ 
  TimerADC();

  /* Executes the static variable test */
  Statics_Test();

  /* Infinite while loop */
  while (1);
}
/*******************************************************************************
* End of function main
*******************************************************************************/

/*******************************************************************************
* Outline       : Statics_Test
* Description   : Static variable test routine. The function replaces the
*                 contents of the string ucStr with that of ucReplace, one
*                 element at a time. Right-click the variable ucStr, and select
*                 instant watch - click add in the subsequent dialog. If you step
*                 through the function, you can watch the string elements being
*                 overwritten with the new data.
* Argument      : none
* Return value  : none
*******************************************************************************/
void Statics_Test(void)
{
  /* Declare error flag */
  bool err = true;

  /* Declare loop count variable */
  uint8_t loop_counter;

  /* Write the initial contents of ucStr to the debug LCD */
  DisplayLCD(LCD_LINE2, ucStr);

  /* Initialise a for loop, and replace each element of ucStr in each loop
  iteration with the contents of ucReplace */
  for (loop_counter = 0; loop_counter < 10; loop_counter++)
  {
    /* Replace character from ucStr with characer from ucReplace */
    ucStr[loop_counter] = ucReplace[loop_counter];

    /* Write current contents of ucStr to the debug LCD */
    DisplayLCD(LCD_LINE2, ucStr);

    /* Start a oneshot timer to create a delay between each loop 
    iteration */
    err &=  R_CMT_CreateOneShot
      (
      2, 
      PDL_NO_DATA,
      600E-3,
      PDL_NO_FUNC,
      0
      );  
  }

  /* Revert the debug LCD back to displaying the nickname */    
  DisplayLCD(LCD_LINE2, NICKNAME);
}
/*******************************************************************************
* End of function Statics_Test
*******************************************************************************/
