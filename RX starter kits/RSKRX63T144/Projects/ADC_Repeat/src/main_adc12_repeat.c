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
* Device        : R5F563TE
* Tool-Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX63T144
* Description   : Demonstration of the ADC module, in continuous scan mode. The
*                 program configures the ADC to perform repeat AD conversions
*                 and display the results on the LCD.
* Operation     : 1. Compile and download the sample code to the target. Click
*                    'Resume' icon on the 'Debug' toolbar to start the
*                    software.
*
*                 2. Observe the LCD display - the R5F563TE CPU will repeatedly
*                    display ADC readings of the ADC channel AN000.
*
*                 3. Adjust the setting of the potentiometer, and observe the
*                    change in the value being displayed on the LCD for
*                    channel AN000.
*
*                 4. The user may also examine the ADC conversion result in
*                    the global variable g_adc_result.
*******************************************************************************/
/*******************************************************************************
* History       : 05.12.2013  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/
/* LCD controlling function prototypes & macro definitions */
#include "lcd.h"

/* ADC repeat function definitions */
#include "adc12repeat.h"

/* this module's header file */
#include "main_adc12_repeat.h"

/* Intrinsics */
#include <intrinsics.h>

/*******************************************************************************
* Function Name : main
* Description   : The main program function. This function calls the ADC
*                 initialisation function, then waits in an infinite while loop
*                 whilst the ADC readings take place in the timer interrupt.
* Argument      : none
* Return value  : none
*******************************************************************************/
void main (void)
{
    /* Enable interrupts */
    __enable_interrupt();

    /* Initialise the debug LCD */
    Init_LCD();

    /* Display instructions on the debug LCD */
    Display_LCD(LCD_LINE1, "AD12 RPT");
    Display_LCD(LCD_LINE2, "=H'0000 ");

    /* Initialise the 12-bit ADC module */
    Init_ADC12Repeat();

    /* Infinite while loop */
    while (1)
    {
        /* Do nothing */
    }
}
/******************************************************************************
* End of function main
******************************************************************************/

