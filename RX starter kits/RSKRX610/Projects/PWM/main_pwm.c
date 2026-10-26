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
* File Name    : main_pwm.c
* Version     : 1.00
* Device     : R5F56107VNFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX610
* Description   : Defines the main program code
* Instructions  : CAUTION: Ensure R83 is fitted and R84 is removed before
*               proceeding
*
*          1. Compile and download sample code. Click 'Go' to
*           start the software.
*
*          2. Connect an oscillioscope to pin 20, on JA2. The output
*             should be a 1KHz squarewave, with a varying duty cycle.
*           The duty cycle should increment to 90%, and then flip back
*           to 10% and being to increment again.
*
*          3. Press switch 3 to stop the duty cycle changing, and keep
*             the squarewave output constant.
******************************************************************************/
/******************************************************************************
* History     : 22.01.2010 Ver. 1.00 First Release
*””FILE COMMENT END””*********************************************************/

/******************************************************************************
User Includes (Project level includes)
******************************************************************************/
/* LCD function definitions */
#include "lcd.h"
/* PWM function defintions */
#include "pwm.h"


/*””FUNC COMMENT””*************************************************************
* Outline     : main
* Description   : The main program function. The function displays the splash
*          screen to the LCD display, initialises the PWM output then
*          waits in an infinite loop.
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
  DisplayLCD(LCD_LINE1, "  PWM   ");
  DisplayLCD(LCD_LINE2, " JA2-20 ");

  /* Initialise PWM output */
  Init_PWM();

  /* Infinite While Loop */
  while (1);
}
/******************************************************************************
End of function main
******************************************************************************/
