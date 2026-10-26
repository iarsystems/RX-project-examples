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
* File Name    : main_wdt.c
* Version     : 2.00
* Device     : R5F562TAADFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX62T
* Description   : This sample demonstrates usage of the watchdog timer, by 
*          configuring the timer to require periodic resets to prevent
*          a timer overflow. The sample configures a periodic timer reset
*          which has a period that varies with the position of the
*          potentiometer, RV1. The LEDs flash to indicate each period.
* Operation      : 1. Build and download the sample code to the RSK.
*
*          2. Ensure the potentiometer, RV1, is turned to the fully anti-
*             clockwise position before starting the sample. To start the
*           sample, click 'Reset Go'.
*          
*          3. The debug LCD will display the sample name, and the user
*             LEDs will begin to flash.
*          
*          4. Slowly turn the potentiometer RV1 clockwise, and observe the
*             gradual decrease of the LED flash rate. Each time the LEDs
*           toggle output state, the watchdog timer is reset, preventing
*           it from triggering an overflow interrupt.
*          
*          5. Continue to turn the potentiometer clockwise - the LED
*             flashing will slow down to a point where the watchdog timer
*           is allowed to overflow.
*          
*          6. When the timer has overflowed, the LEDs will stay constantly
*             lit, and the program will wait in an infinite while loop. 
*           The debug LCD will display "Watchdog Overflow".
*          
*          7. To repeat the sample, ensure RV1 is fully anti-clockwise
*             and click 'Reset Go'.
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
/* Defines watchdog timer functions used in this file */
#include "wdt.h"
#include "hwsetup.h"
#include <intrinsics.h>

/******************************************************************************
* Outline      : main
* Description  : Main program function. This function first initialises the 
*         debug LCD, and displays the name of the sample. The function
*         then calls the Init_WDT function, which configures the watchdog
*         timer, and the LED flash reset interrupts. After the
*         initialisation function has been called, main waits in an
*         infinite while loop.
* Argument     : none
* Return value : none
******************************************************************************/
void main(void)
{
  /* Set up hardware */
  HardwareSetup();
  
  /* Enable interrupt */
  __enable_interrupt();
  
  /* Initialise the debug LCD display */
  InitialiseLCD();
  
  /* Display the sample name on the debug LCD */
  DisplayLCD(LCD_LINE1, "  WDT   ");
  
  /* Configure the watchdog timer and timer reset interrupts */
  Init_WDT();
    
  /* Infinite while loop */
  while (1);
}
/******************************************************************************
* End of function main
******************************************************************************/
