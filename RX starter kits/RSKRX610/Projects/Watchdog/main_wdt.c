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
* File Name    : main_wdt.c
* Version     : 1.00
* Device     : R5F56107VNFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX610
* Description   : Demonstration of the watchdog timer. Program sets a WDT
*          timeout interrupt that is called when the reset freqency,
*          set by RV1, is too low.
* Instructions  : 1. Ensure that the pot, RV1, is turned fully anti-clockwise.
*
*          2. Compile and download the sample code. Click 'Go'
*             to start the software.
*
*          3. The timer will run, and an interrupt will blink the LEDs
*             at a frequency determined by the pot, RV1. The  interrupt
*           will also periodically reset the Watchdog timer.
*
*          4. By turning the pot RV1 clockwise, the WDT reset rate is
*             reduced. When the time period between timer interrupts is
*           too great, the WDT will timeout and the LEDs will stay
*           static. Reset the MCU to repeat the example.
******************************************************************************/
/******************************************************************************
* History     : 22.01.2010 Ver. 1.00 First Release
*””FILE COMMENT END””*********************************************************/

/******************************************************************************
User Includes (Project level includes)
******************************************************************************/
/* LCD function definitions */
#include "lcd.h"
/* WDT function definitions */
#include "WDT.h"


/*””FUNC COMMENT””*************************************************************
* Outline     : main
* Description   : The main program function. Function displays the splash
          screen on the LCD display, calls the watchdog intialisation
          routine, and then waits in an infinite while loop.
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
  DisplayLCD(LCD_LINE1, "  WDT   ");

  /* Initialise the Watch Dog Timer */
  WDT_Init();

  /* Infinite While Loop */
  while (1);
}
/******************************************************************************
End of function main
******************************************************************************/
