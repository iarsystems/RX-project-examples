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
/* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.   */
/*******************************************************************************
* File Name     : main_pwm.c
* Version       : 1.00
* Device        : R5F52206
* Tool-Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX220
* Description   : This sample uses channel 3 of the MTU0 to output a 1KHz PWM
*                 square waveform on the MTIOC0A output pin.
* Operation     : 1. Connect an oscilloscope probe to the header pin JA3-12.
*
*                 2. Compile and download the sample code to the target. Click
*                    'Reset Go' icon on the 'Debug Run' toolbar to start the
*                    software.
*
*                 3. A 1 KHz PWM square wave output with varying duty cycle
*                    will be displayed on the oscilloscope. The duty cycle
*                    will increase from 10% to 90% before returning to 10%
*                    and repeating. The signal can also be seen on LED3.
*
*                 4. Press SW1 to stop the duty cycle modulation and display
*                    the duty as a percentage. Observe the static waveform on
*                    the oscilloscope.
*
*                 5. Press SW2 or SW3 to continue duty cycle modulation and
*                    clear the debug LCD's second line. Observe the waveform's
*                    duty cycle varying.
*******************************************************************************/
/*******************************************************************************
* History       : 14.12.2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* System Includes
*******************************************************************************/
#include <intrinsics.h>

/******************************************************************************
User Includes (Project Level Includes)
******************************************************************************/

/* LCD controlling function prototypes & macro defines */
#include "lcd.h"

/* Prototypes for Timer control functions */
#include "pwm.h"

/******************************************************************************
* Function Name: main
* Description  : The main program function. The function displays the splash
*                screen on the LCD display, initialises the timer for PWM
*                output, then waits in an infinite while loop.
* Arguments    : none
* Return Value : none
******************************************************************************/
void main (void)
{
    /* Enable interrupts */
    __enable_interrupt();

    /* Initialise the LCD Display */
    Init_LCD();

    /* Display Splash Screen */
    Display_LCD(LCD_LINE1, " 1 KHz  ");
    Display_LCD(LCD_LINE2, "JA3 P12 ");

    /* Initialise PWM timers */
    init_pwm();

    /* Infinite while loop */
    while (1)
    {
        /* Sample operation is performed by interrupt routine */
    }
}
/******************************************************************************
* End of function main
******************************************************************************/

