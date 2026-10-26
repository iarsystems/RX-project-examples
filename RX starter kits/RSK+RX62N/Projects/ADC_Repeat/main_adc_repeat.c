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
* File Name    : main_adc_repeat.c
* Version      : 2.00
* Device       : R5F562N8
* Tool Chain   : IAR Embedded Workbench
* H/W Platform : RSK+RX62N
* Description  : This sample demonstrates use of the ADC unit, by periodically
*                sampling the voltage from the potentiometer RV1, and displaying
*                the result on the debug LCD.
* Operation    :
*
*         1. Build and download the sample code to the RSK+.
*         
*         2. The debug LCD will show the name of the sample, and the
*            current ADC value.
*          
*         3. Adjust the potentiometer, RV1, and observe the value
*            displayed on the LCD change.
*
*         4. The user may examine the AD conversion result in the global
*            variable usADC_Result.
*******************************************************************************/
/*******************************************************************************
* History    : 15.09.2010 Ver. 1.00 First Release
*           18.04.2011 Ver. 2.00 Second Release (Updated to RPDL CS4)
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
/* Defines debug LCD functions used in this file */
#include "lcd.h"
/* Defines ADC repeat functions used in this file */
#include "ADCrepeat.h"
#include "hwsetup.h"
#include "intrinsics.h"

/*******************************************************************************
* Outline      : main
* Description  : The main program function. The function first initialises the 
*         debug LCD, then displays the name of the sample on the display.
*         The function then executes the initialisation function, Init_
*         ADCRepeat, which configures the ADC unit. The MCU then returns
*         to the main function, where it enters an infinite while loop.
* Argument     : none
* Return value : none
*******************************************************************************/
void main(void)
{
  HardwareSetup();
  __enable_interrupt();

  /* Initialise the debug LCD */
  InitialiseLCD();
  
  /* Display instructions onto the LCD */
  DisplayLCD(LCD_LINE1, "ADC RPT ");
  
  /* Initialise ADC unit for repeat operation */
  Init_ADCRepeat();
  
  /* Infinite while loop */
  while(1);
  
}
/*******************************************************************************
* End of function main
*******************************************************************************/

