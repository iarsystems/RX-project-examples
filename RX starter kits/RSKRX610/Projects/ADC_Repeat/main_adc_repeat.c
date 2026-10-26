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
* File Name    : main_adc_repeat.c
* Version     : 1.00
* Device     : R5F56107VNFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX610
* Description   : Demonstration of the ADC module, in repeat mode. The
*          program configures the ADC to perform repeat AD conversions
*          and display the results on the LCD.
* Instructions  : 1. Compile the sample code, and download to the RSK. Click
*           the 'Go' button to start the software. Instructions
*           will be displayed on the LCD.
*
*          2. Observe the LCD display - the RX610 will repeatedly make
*             ADC readings of the potential divider formed by the
*           potentiometer, RV1.
*
*          3. Adjust the setting of the potentiometer, and observe the
*             change in the value being displayed on the LCD.
*
*          4. The user may examine the ADC conversion result in the
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
/* ADC repeat function definitions */
#include "adc_repeat.h"


/******************************************************************************
System Definitions & Global Variables
******************************************************************************/
/* Global variable, ADC result container */
uint16_t usADC_Result;

/*””FUNC COMMENT””*************************************************************
* Outline     : main
* Description   : The main program function. This function calls the ADC
*          initialisation function, then waits in an infinite while loop
*          whilst the ADC readings take placed in interrupts.
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
  DisplayLCD(LCD_LINE1, "ADC RPT");

  /* Initalise the ADC Periperhal */
  ADC_Init();

  /* Infinite While Loop */
  while (1);
}
/******************************************************************************
End of function main
******************************************************************************/
