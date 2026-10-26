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
/* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.   */
/*******************************************************************************
* File Name     : main_adc12_repeat.c
* Version       : 1.00
* Device        : R5F5210B
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX210B
* Description   : Demonstration of the ADC module, in continuous scan mode. The
*                 program configures the ADC to perform repeat AD conversions
*                 and display the results on the LCD.
* Operation     :   1. Compile and download the sample code to the target. Click
*                      the 'Reset Go' icon on the 'Debug Run' toolbar to start
*                      the software.
*
*                   2. Observe the LCD display - the RX210B CPU will repeatedly
*                      display ADC readings of the ADC channel AN000.
*
*                   3. Adjust the setting of the potentiometer, and observe the
*                      change in the value being displayed on the LCD.
*
*                   6. The user may also examine the ADC conversion result in
*                      the global variable gADC_Result.
*
*          NOTE: 1. If the power supply in use is not filtered enough, you may
*                   notice some variations in the displayed ADC result when the
*                   application is executed more than once.
*
*******************************************************************************/
/*******************************************************************************
* History       : 14.05.2013  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* System Includes
*******************************************************************************/
#include <intrinsics.h>

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/
/* LCD controlling function prototypes & macro defines */
#include "lcd.h"
/* ADC repeat function definitions */
#include "adc12repeat.h"
/* MCU and RSK hardware initialization */
#include "hwsetup.h"

/*******************************************************************************
* Outline       : main
* Description   : The main program function. This function calls the ADC
*                 initialisation function, then waits in an infinite while loop
*                 whilst the ADC readings take place in the timer interrupt.
* Argument      : none
* Return value  : none
*******************************************************************************/
void main(void)
{
    /* Configure the MCU and RSK hardware */
    HardwareSetup();

    /* Enable interrupts */
    __enable_interrupt();

    /* Initialise the debug LCD */
    Init_LCD();

    /* Display sample name on the debug LCD */
    Display_LCD(LCD_LINE1, "AD12 RPT");

    /* Initialise the 12-bit ADC module */
    Init_ADC12Repeat();

    /* Infinite while loop */
    while (1);
}
/******************************************************************************
* End of function main
******************************************************************************/
