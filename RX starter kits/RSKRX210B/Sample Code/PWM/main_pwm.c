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
* File Name     : main_pwm.c
* Version       : 1.00
* Device        : R5F5210B
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX210B
* Description   : This sample demonstrates usage of the timers to generate a
*                 PWM output. The MCU will generate a square wave output, with
*                 a duty cycle that varies from 10% to 90%.
*
* Operation     : 1. Compile and download the sample code to the target. Click
*                    the 'Reset Go' icon on the 'Debug Run' toolbar to start the
*                    software.
*
*                 2. The program will create the PWM output at the TMO2 pin,
*                    which is accessible at the header pin J2-5.
*
*                 3. The waveform will be a square wave, with a duty cycle that
*                    increments from 10% to 90%, then returns to 10% and
*                    repeats.
*
*                 4. Press switch SW1 to stop the duty cycle from varying. The
*                    value of the duty cycle will be displayed on the debug LCD
*                    as a percentage.
*
*                 5. To repeat the sample, stop and restart the software by
*                    clicking the 'Stop' icon followed by the 'Reset Go' icon
*                    on the 'Debug Run' toolbar.
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
/* LCD controlling function prototypes & macro definitions */
#include "lcd.h"
/* Defines PWM functions used in this file */
#include "pwm.h"
/* MCU and RSK hardware initialization */
#include "hwsetup.h"

/*******************************************************************************
* Outline       : main
* Description   : The main program function. The function displays the splash
*                 screen to the LCD display, initialises the PWM output then
*                 waits in an infinite loop.
* Argument      : none
* Return value  : none
*******************************************************************************/
void main(void)
{
    /* Configure the MCU and RSK hardware */
    HardwareSetup();

    /* Enable interrupts */
    __enable_interrupt();

    /* Initialise the LCD Display */
    Init_LCD();

    /* Display application name on the LCD */
    Display_LCD(LCD_LINE1, "  PWM   ");
    Display_LCD(LCD_LINE2, "Push SW1");

    /* Initialise the PWM output */
    Init_PWM();

    /* Infinite while loop */
    while (1);
}
/******************************************************************************
* End of function main
******************************************************************************/
