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
* File Name    : main_vdet.c
* Version     : 1.00
* Device     : R5F52108DFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX210
* Description    : This sample demonstrates usage of the low voltage detection 
*          circuit.
* Operation       : 1. Fit a jumper to short the pins 1 and 2 of J17. 
*
*          2. Connect a regulated variable DC power supply to the RSK.
*             Set the initial voltage level to 5.0V.
*          
*          3. Compile and download the sample code to the target. Click 
*           'Reset Go' icon on the 'Debug Run' toolbar to start the 
*           software.
*          
*          4. Observe all LEDs flashing. Reduce the power supply below
*           4.0V.
*          
*          5. When the power supply equals or falls below the Vdet2's  
*             threshold (approximately 4.0V), only the LED3 will be ON
*           LED0-LED2 will be turned off.
*
*          6. When the power supply raises above VDET2 level the  
*           LED0-LED2 will re-start flashing. Now when LED3 in on   
*           LED2-LED0 will be off and when LED3 is off LED0-LED2 will 
*           be on. This indicates that the VDET2 interrupt had
*           occurred.
*
*          7. When the power supply equals or falls below the Vdet1's  
*             threshold (approx. 1.9V) all the LEDs (LED0-LED3) will be 
*           turned off. When the power supply raises above VDET1's 
*           threshold the LED3 will turn on again.
*  
*      Note : 1. While testing this sample code, please ensure that the  
*              VCC is always more than 1.62V in order to keep the MCU 
*            & thus the sample application running.
*           2. Please note that, the debug LCD operates on 5V, so when
*            the VCC is reduced, the LCD will be blank at approx. 4V.
*            The display will appear again when the voltage rises above 
*            4V.
*******************************************************************************/
/*******************************************************************************
* History     : 05.08.2011  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/
/* LCD controlling function prototypes & macro definitions */
#include "lcd.h"
/* Defines functions and variables used in vdet.c file */
#include "vdet.h"

/*******************************************************************************
* Outline      : main
* Description  : Main program function. This function first initialises the 
*         debug LCD, and displays the name of the sample. The function
*         then calls the 'Init_LVD' function, which configures the 
*         voltage detect circuitry. The program then waits in an 
*         infinite while loop.
* Argument     : none
* Return value : none
******************************************************************************/
#include "intrinsics.h"
#include "hwsetup.h"

void main(void)
{
  /* Hardware setup */
  HardwareSetup();

  /* Enable Interrupt */
  __enable_interrupt();

  /* Initialise the debug LCD display */
  Init_LCD();
  
  /* Display the sample name on the debug LCD */
  Display_LCD(LCD_LINE1, " V DET  ");
  
  /* Configure the voltage detect circuit & related interrupts */
  Init_VDET();  
    
  /* Infinite while loop */
  while(1);
}
/******************************************************************************
* End of function main
******************************************************************************/
