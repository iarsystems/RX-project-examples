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
/* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.   */
/*******************************************************************************
* File Name    : main_adc12_repeat.c
* Version     : 1.00
* Device     : R5F52108DFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX210
* Description   : Demonstration of the ADC module, in continuous scan mode. The 
*          program configures the ADC to perform repeat AD conversions
*          and display the results on the LCD.
* Operation    :   1. Connect an external power supply to the analog pins AN001 
*             (JA1-10 or J4-18), AN002 (JA1-11 or J4-17), AN003
*             (JA1-12 or J4-16) keeping the initial voltages at 0.0 V.
*            Please ensure that the power supply ground is connected  
*            to the ground signal on RSK.
*
*            2. Compile and download the sample code to the target. Click 
*             'Reset Go' icon on the 'Debug Run' toolbar to start the 
*             software.
*           
*            3. Observe the LCD display - the RX210 CPU will repeatedly 
*               display ADC readings of the ADC channels AN000 to AN003.
*           
*            4. Adjust the setting of the potentiometer, and observe the
*               change in the value being displayed on the LCD for
*             channel AN000.
*
*          5. Vary the analog voltages applied to the AN001 to AN003 
*             pins & observe corresponding ADC readings on the LCD 
*             display.   
*
*            6. The user may also examine the ADC conversion result in  
*             the global variable usADC_Result[0] to usADC_Result[3].  
*
*       NOTE: 1. If the power supply in use is not filtered enough, you may
*                   notice some variations in the displayed ADC result when the
*            application is executed more than once.
*
*         2. Please ensure that the external input voltage applied to  
*          AN001-AN003 pins do not exceed the MCU supply voltage
*          (which is by default kept at 3.3V) to avoid damage to
*          the MCU pins.
*******************************************************************************/
/*******************************************************************************
* History     : 05.08.2011  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/
/* LCD controlling function prototypes & macro defines */
#include "lcd.h"
/* ADC repeat function definitions */
#include "ADC12repeat.h"

/*******************************************************************************
* Outline     : main
* Description   : The main program function. This function calls the ADC 
*          initialisation function, then waits in an infinite while loop
*          whilst the ADC readings take place in the timer interrupt.
* Argument    : none
* Return value  : none
*******************************************************************************/
#include "intrinsics.h"
#include "hwsetup.h"

void main(void)
{
  /* Hardware setup */
  HardwareSetup();

  /* Enable Interrupt */
  __enable_interrupt();

  /* Initialise the debug LCD */
  Init_LCD();
  
  /* Display instructions on the debug LCD */  
  Display_LCD(LCD_LINE1, "AD12 RPT");  

  /* Initialise the 12-bit ADC module */
  Init_ADC12Repeat();

  /* Infinite while loop */  
  while (1);
}
/******************************************************************************
* End of function main
******************************************************************************/
