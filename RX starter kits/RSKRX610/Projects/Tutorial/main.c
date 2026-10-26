/* Adapted for use with IAR Embedded Workbench */
/******************************************************************************
* DISCLAIMER:
* The software supplied by Renesas Technology Europe Ltd is
* intended and supplied for use on Renesas Technology products.
* This software is owned by Renesas Technology Europe, Ltd. Or
* Renesas Technology Corporation and is protected under applicable
* copyright laws. All rights are reserved.
*
* THIS SOFTWARE IS PROVIDED "AS IS". NO WARRANTIES, WHETHER EXPRESS,
* IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* APPLY TO THIS SOFTWARE. RENESAS TECHNOLOGY AMERICA, INC. AND
* AND RENESAS TECHNOLOGY CORPORATION RESERVE THE RIGHT, WITHOUT
* NOTICE, TO MAKE CHANGES TO THIS SOFTWARE. NEITHER RENESAS
* TECHNOLOGY AMERICA, INC. NOR RENESAS TECHNOLOGY CORPORATION SHALL,
* IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
* CONSEQUENTIAL DAMAGES FOR ANY REASON WHATSOEVER ARISING OUT OF THE
* USE OR APPLICATION OF THIS SOFTWARE.
******************************************************************************/
/* Copyright (C) 2009. Renesas Technology Europe, All Rights Reserved */
/*””FILE COMMENT””******************************* Technical reference data ****
* File Name    : main.c
* Version     : 1.00
* Device     : RX610
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX610
* Description   : This is the main tutorial code. This code will call three
*                 functions to demonstrate Port pin control (FlashLEDs),
*          Interrupt usage (TimerADC) and C variable initialization
*          (Statics_Test). Code is also included to drive the optional
*          LCD module
* Instructions  : 1. Compile and download and tutorial code. Click 'Go'
           to start the software.
*
*          2. The LEDs will begin to flash, press any button to proceed.
*             (The program proceed automatically after 200 flashes).
*
*          3. The string TESTTEST will slowly replace the word STATIC on
*           the second LCD line then be replaced with RX610 again. The
*           LEDs will also blink at a rate varied by the pot RV1.
******************************************************************************/
/******************************************************************************
* History     : 22.01.2010 Ver. 1.00 First Release
*””FILE COMMENT END””*********************************************************/

/******************************************************************************
User Includes (Project level includes)
******************************************************************************/
/* LCD function definitions */
#include "lcd.h"
/* Timer & ADC function definitions */
#include "timeradc.h"
/* LED flash function definitions */
#include "flashLED.h"


/******************************************************************************
System Definitions & Global Variables
******************************************************************************/
/* Global initialized variable*/
char ucStr[] = " STATIC \0";
/* Static variable declaration, used in static variable test */
const char ucReplace[] = "TESTTEST\0";
/* Device nickname definition */
#define NICKNAME "RX610   "
/* Statics Test function declaration */
void Statics_Test(void);


/*””FUNC COMMENT””*************************************************************
* Outline     : main
* Description   : The main program function. Displays the Renesas splash screen
          onto the LCD display, then calls the flashLED and TimerADC
          functions. The function then calls the statics test routine,
          before waiting in an infinite while loop.
* Argument    : none
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
#include "intrinsics.h"
#include "hwsetup.h"

void main(void)
{
  HardwareSetup();

  /* Enable Interrupt */
  __enable_interrupt();

  /* Initialise the LCD Display */
  InitialiseLCD();

  /* Displays the Renesas splash screen */
  DisplayLCD(LCD_LINE1, "Renesas");
    DisplayLCD(LCD_LINE2, NICKNAME);

  /* Begins the initial LED flash sequence */
  FlashLED();

  /* Begins the ADC-varying flash Sequence */
  TimerADC();

  /* Begins the static variable test */
  Statics_Test();

  /* Defines an infinite loop to keep the MCU running */
  while(1);
}
/***********************************************************************************
End main function
***********************************************************************************/


/***********************************************************************************
Function Name : Statics_Test
Description    : Static variable test sequence

Parameters    : none
Return value  : none
***********************************************************************************/
void Statics_Test(void)
{
   /* At this point please right click on the 'ucSTR' variable and select
      'Add to Watch'. A dialog will be displayed showing the current value of the
    variable.
        Step through the following code to see that the initialised data is being
    overwritten with the different data. */

  /* Initialise delay and count variables */
  unsigned short uiCount, delayA, delayB;

  /* Write ucStr varaible, "STATIC" to LCD */
  DisplayLCD(LCD_LINE2,ucStr);

  /* Begin for loop which writes one letter of ucReplace to the LCD at a time
     The nested while loops generate the delay bewteen each letter change */
  for (uiCount=0; uiCount<8; uiCount++)
  {

    /* Replace letter number uiCount of ucStr from ucReplace */
    ucStr[uiCount] = ucReplace[uiCount];
    DisplayLCD(LCD_LINE2,ucStr);

    /* Set delay variables */
    delayA = 0xAA;
    delayB = 0xFFFF;

    /* Nested while delay loop */
    while(delayA)
    {
      while(delayB)
      {
        delayB--;
      }
      delayB = 0xFFFF;
      delayA--;
    }
  }

  /* Clear LCD Display */
  ucStr[uiCount] = '\0';

  /* Write MCU nickname to LCD again */
  DisplayLCD(LCD_LINE2,NICKNAME);
}
/***********************************************************************************
End Statics_Test function
***********************************************************************************/
