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
* File Name    : main_adc_one.c
* Version     : 1.00
* Device     : R5F56107VNFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX610
* Description   : Demonstration of the ADC module, in one-shot mode. The
*          program configures the ADC to perform a conversion when
*          switch 3 is press.
* Instructions  : 1. Compile the code, and press 'Go' to run the sample.
*           Instructions will be displayed on the LCD.
*
*          2. Press SW3 to perform a one-shot AD conversion, on the
*           voltage divider formed by the potentiometer RV1.
*
*          3. The conversion result will display on the LCD screen.
*
*          4. Adjust the pot, and repress SW3 to repeat conversion.
*
*          5. The user may examine the ADC conversion result in the
*           global variable usADC_Result.
*
*      NOTE: If the power supply in use is not filtered enough, you may
*                 notice some variations in the displayed ADC result when the
*          application is executed more than once.
******************************************************************************/
/******************************************************************************
* History     : 22.01.2010 Ver. 1.00 First Release
*””FILE COMMENT END””*********************************************************/

/******************************************************************************
User Includes (Project level includes)
******************************************************************************/
/* LCD function definitions */
#include "lcd.h"
/* Defines standard typedefs used in this file */
#include <stdint.h>
/* ADC oneshot function definitions */
#include "adc_oneshot.h"


/******************************************************************************
Global Variables
******************************************************************************/
/* Global variable, ADC result container */
uint16_t usADC_Result;


/*””FUNC COMMENT””*************************************************************
* Outline     : main
* Description   : The main program function. This function calls the ADC
*          initialisation function, then waits in an infinite while loop
*          whilst the ADC readings take place in interrupts.
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

  /* Displays the program start screen */
  DisplayLCD(LCD_LINE1, "ADC ONE");
  DisplayLCD(LCD_LINE2, "Push SW3");

  /* Initalise the ADC Periperhal */
  ADC_Init();

  /* Infinite While Loop */
  while (1);
}
/******************************************************************************
End of function main
******************************************************************************/
