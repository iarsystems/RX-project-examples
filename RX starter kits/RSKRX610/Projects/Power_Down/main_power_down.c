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
* File Name    : main_power_down.c
* Version     : 1.00
* Device     : R5F56107VNFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX610
* Description   : Defines the main program code
* Instructions  : 1. Compile and download the sample code to the RSK. Press
*           'Go' to start the software.
*
*          2. Instructions will be displayed on the LCD, and the user
*             LEDs will begin to flash.
*
*          3. Press switch 1 to put the MCU into sleep mode. The user
*             LEDs will stop flashing, and the MCU mode will be shown
*           as 'Sleep'.
*
*          4. Press any of the switches to wake the MCU up again. The
*             LEDs will resume flashing, and the MCU mode will be
*           displayed as 'Active'.
******************************************************************************/
/******************************************************************************
* History     : 22.01.2010 Ver. 1.00 First Release
*””FILE COMMENT END””*********************************************************/

/******************************************************************************
User Includes (Project level includes)
******************************************************************************/
/* LCD function definitions */
#include "lcd.h"
/* Power down function definitions */
#include "power_down.h"


/******************************************************************************
Global Variables
******************************************************************************/
/* Sleep flag variable, used to indicate when to transition to sleep mode */
extern unsigned short SleepFlag;


/*””FUNC COMMENT””*************************************************************
* Outline     : main
* Description   : The function first shows the splash screen on the LCD display,
*          then enters an infinite while loop; where if checks the
*          SleepFlag and then checks the ToggleLEDs function. If the
*          SleepFlag is true, the PowerDown_Sleep function is called.
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

  /* Initialise the LCD Display */
  InitialiseLCD();

  /* Displays the program start screen */
  DisplayLCD(LCD_LINE1, "  LPC   ");
  DisplayLCD(LCD_LINE2, "PressSW1");

  /* Initialise the MCU sleep mode registers */
  PowerDown_Init();

  /* Infinite While Loop */
  while (1)
  {
    /* Is the SleepFlag true? */
    if(SleepFlag)
    {
      /* Put the MCU into sleep mode */
      PowerDown_Sleep();
    }

    /* Toggle the LEDs, to make them flash */
    ToggleLEDs();
  }
}
/******************************************************************************
End of function main
******************************************************************************/
