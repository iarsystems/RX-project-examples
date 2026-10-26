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
* File Name    : main_adc_oneshot.c
* Version      : 2.00
* Device       : R5F562N8
* Tool Chain   : IAR Embedded Workbench
* H/W Platform : RSK+RX62N
* Description  : This sample demonstrates usage of the ADC unit, in oneshot
*                mode. The program takes a sample of the voltage created by the
*                potentiometer RV1.
* Operation    :
*
*         1. Build and download the sample code to the RSK+.
*         
*         2. Instructions will be displayed on the debug LCD. Adjust the
*            potentiometer RV1, and press switch SW3 to trigger an AD
*            conversion.
*          
*         3. The ADC result will be displayed on the second line of the
*            debug LCD in the format =H'0000.
*        
*         4. Re-adjust RV1, and press SW3 again to perform another 
*            conversion.
*          
*         5. The value of the AD conversion can be watched in the
*            variable 'usADC_Result'. Open the 'Live Watch' window
*            ('View' menu) and drag the variable to it.
*******************************************************************************/
/*******************************************************************************
* History    : 29.07.2010 Ver. 1.00 First Release
*           18.04.2011 Ver. 2.00 Second Release (Updated to RPDL CS4)
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
/* Defines debug LCD functions used in this file */
#include "lcd.h"
/* Defines oneshot ADC functions used in this file */
#include "ADCone.h"
#include "hwsetup.h"
#include "intrinsics.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* ADC result global storage variable */
extern uint16_t usADC_Result;

/*******************************************************************************
* Outline      : main
* Description  : The main program function. The function first initialises the
*         debug LCD, then displays name of the sample and operation 
*         instructions. Main then calls the ADC initialisation function,
*         Init_ADCOneshot, which configures the ADC. After this, the
*         MCU returns to main, where it enters an infinite while loop.
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
  DisplayLCD(LCD_LINE1, "ADC One ");
  DisplayLCD(LCD_LINE2, "Push SW3");
  
  /* Initialise the ADC unit */
  Init_ADCOneshot();
  
  /* Infinite while loop */
  while(1);
  
}
/*******************************************************************************
* End of function main
*******************************************************************************/

