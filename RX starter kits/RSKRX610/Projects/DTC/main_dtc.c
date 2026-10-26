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
* File Name    : main_dtc.c
* Version     : 1.00
* Device     : R5F56107VNFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX610
* Description   : Main file for the DTC sample.
* Instructions  : 1. Build and download the sample code. Click 'Go' to
*           start the software.
*
*          2. Right click the global variable 'gDTC_DataBuff', and
*           select 'Watch'.
*
*          3. Press switch 1. This will trigger the ADC to take a
*           reading from the potential divider formed by the
*           potentiometer RV1.
*
*          4. When the AD conversion completes, the DTC will trigger
*             and transfer the ADC value to fill the destination array
*           'gDTC_DataBuff'.
*
*          5. Adjust the potentiometer and press switch 1 again to
*           repeat the test.
******************************************************************************/
/******************************************************************************
* History     : 22.01.2010 Ver. 1.00 First Release
*””FILE COMMENT END””*********************************************************/

/******************************************************************************
User Includes (Project level includes)
******************************************************************************/
/* Declares DTC functions used in this file */
#include "dtc.h"
/* Declares LCD functions used in this file */
#include "lcd.h"


/******************************************************************************
System Definitions & Global Variables
******************************************************************************/
/* Global variable - DTC transfer destination */
short gDTC_DataBuff[256];


/*””FUNC COMMENT””*************************************************************
* Outline     : main
* Description   : The main program function. The function displays the splash
*          screen to the LCD, then intialises the ADC and DTC routines.
*          The function then waits in an infinite while loop, whilst
*          the rest of the program functionality is performed through
*          interrupts.
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
  DisplayLCD(LCD_LINE1, "   DTC  ");
  DisplayLCD(LCD_LINE2, "PressSW1");

  /* Initialise PWM output */
  Init_DTC();

  /* Infinite While Loop */
  while (1);
}
/******************************************************************************
End of function main
******************************************************************************/
