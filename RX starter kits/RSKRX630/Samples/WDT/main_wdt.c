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
/* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.   */
/*******************************************************************************
* File Name    : main_wdt.c
* Version     : 1.00
* Device     : R5F5630E
* Tool-Chain   : Renesas RX Toolchain 1.2.0.0
* H/W Platform  : RSKRX630
* Description   : This sample demonstrates use of the WDT unit, by creating a 
*          periodic CMT interrupt that resets the WDT count. The CMT
*          interrupt period is set by the potentiometer, which can be
*          adjusted so that it is too slow to reset the WDT count and 
*          allows a WDT interrupt.
* Operation    : Before proceeding, ensure that the potentiometer RV1 is
*          to rotated fully anti-clockwise.
*          1. Compile and download the sample code to the target. Click 
*           'Reset Go' icon on the 'Debug Run' toolbar to start the 
*           software.
*
*          2. The debug LCD will display the sample name, and the user
*           LEDs will flash.
*
*          3. Gradually rotate the pot clockwise, and observe the user
*           LED flash rate slowing down.
*
*          4. At approximately 2/3 of the full rotation the WDT interrupt
*           will occur. The debug LCD will indicate the WDT interrupt
*             has occured and the user LEDs will all remain on staticly.
*
*          5. Turn the pot fully anti-clockwise and reset the MCU to
*           start the sample again.
*******************************************************************************/
/*******************************************************************************
* History     : 23 Jan. 2012 Ver. 1.00 First Release
*******************************************************************************/
/* Defines WDT functions used in this file */
#include "wdt.h"
/* Defines LCD functions used in this file */
#include "lcd.h"

#include "intrinsics.h"

#include "hwsetup.h"


/*******************************************************************************
* Outline     : main
* Description   : The main program function. This first displays the sample name
*          on the debug LCD, calls the WDT initialisation function then
*          waits in an infinite while loop. The rest of the sample's
*            functionality occurs in interrupts.
* Argument    : none
* Return value  : none
*******************************************************************************/
void main(void)
{  
  
  HardwareSetup();
  
  __enable_interrupt();

  /* Initialise the debug LCD */
  Init_LCD();
  
  /* Display the sample name on the debug LCD */
  Display_LCD(LCD_LINE1, "WatchDog");

  /* Initialise the WDT unit */
  Init_WDT();
  
  /* Infinite while loop */
  while(1);
}
/*******************************************************************************
* End of function main
*******************************************************************************/
