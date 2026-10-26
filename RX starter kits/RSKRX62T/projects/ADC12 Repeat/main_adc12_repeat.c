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
* File Name    : main_adc12_repeat.c
* Version     : 2.00
* Device     : R5F562TAADFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX62T
* Description   : Demonstration of the ADC module, in repeat mode. The 
*          program configures the ADC to perform repeat AD conversions
*          and display the results on the LCD.
* Instructions  :   1. Connect J4 pin2 (ADPOT) and J4 pin16 (AN000)
*
*          2. Compile the sample code, and download to the RSK. Click
*             the 'Reset Go' button to start the software. Instructions
*             will be displayed on the LCD.
*           
*            3. Observe the LCD display - the RX62T will repeatedly make
*               ADC readings of the potential divider formed by the 
*             potentiometer, RV1. 
*           
*            4. Adjust the setting of the potentiometer, and observe the
*               change in the value being displayed on the LCD.
*
*            5. The user may examine the ADC conversion result in the 
*             global variable usADC_Result.  
*
*      NOTE: If the power supply in use is not filtered enough, you may
*                 notice some variations in the displayed ADC result when the
*          application is executed more than once.
******************************************************************************/  
/******************************************************************************
* History     : 15.10.2010 Ver. 1.00 First Release
              : 21.05.2011 Ver. 2.00 Second Release (Updated to RPDL CS3)
******************************************************************************/

/******************************************************************************
* Project Includes
******************************************************************************/
/* LCD function definitions */
#include "lcd.h"
/* ADC repeat function definitions */
#include "ADC12repeat.h"

#include "hwsetup.h"
#include <intrinsics.h>

/******************************************************************************
* Outline     : main
* Description   : The main program function. This function calls the ADC 
*          initialisation function, then waits in an infinite while loop
*          whilst the ADC readings take place in interrupts.
* Argument    : none
* Return value  : none
******************************************************************************/
void main(void)
{
  /* Set up hardware */
  HardwareSetup();
  
  /* Enable interrupt */
  __enable_interrupt();
  
  /* Initialise the debug LCD */
  InitialiseLCD();
  
  /* Display instructions on the debug LCD */  
  DisplayLCD(LCD_LINE1, "AD12 RPT");  

  /* Initalise the 10-bit ADC Periperhal */
  Init_ADC12Repeat();

  /* Infinite while loop */  
  while (1);
}
/******************************************************************************
* End of function main
******************************************************************************/
