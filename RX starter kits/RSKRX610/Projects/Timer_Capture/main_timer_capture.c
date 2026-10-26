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
* File Name    : main_timer_capture.c
* Version     : 1.00
* Device     : R5F56107VNFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX610
* Description   : Defines the main program code
* Instructions  : 1. Compile and download the sample code. Click 'Go' to
*           start the software.
*
*          2. Press SW3 to toggle the timer on and off. When toggling
*             the timer off, the value of the counter is displayed on
*           the LCD. LED0 lights on to indicate timer activity.
******************************************************************************/
/******************************************************************************
* History     : 22.01.2010 Ver. 1.00 First Release
*””FILE COMMENT END””*********************************************************/

/******************************************************************************
User Includes (Project level includes)
******************************************************************************/
/* LCD function definitions */
#include "lcd.h"
/* Timer function definitions */
#include "timer_capture.h"


/*****************************************************************************
System Definitions & Global Variables
******************************************************************************/
/* The global variable KeyPress is used as a switch status flag */
extern unsigned short KeyPress;


/*””FUNC COMMENT””*************************************************************
* Outline     : main
* Description   : The main program function. The function displays the splash
*          screen on the LCD display, initialises the timer module and
*          then waits in an infinite while loop.
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

  /* Display Splash Screen */
  DisplayLCD(LCD_LINE1, " Timer  ");
  DisplayLCD(LCD_LINE2, "PressSW3");

  /* Initialise timer */
  Timer_Init();

  /* Infinite While Loop */
  while (1)
  {
    /* Check if switch 3 has been pressed */
    if(KeyPress == 0x3)
    {
      /* Run switch 3 callback function */
      SW3_callback();

      /* Reset keypress flag */
      KeyPress = 0x0;
    }
  }
}
/******************************************************************************
End of function main
******************************************************************************/
