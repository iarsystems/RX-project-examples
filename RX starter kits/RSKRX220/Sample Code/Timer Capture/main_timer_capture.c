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
* File Name     : main_timer_capture.c
* Version       : 1.00
* Device        : R5F52206
* Tool-Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX220
* Description   : This sample uses channel D of MTU4 to measure the duration
*                 between SW1 switch press and release.
*
* Operation     : 1. Compile and download the sample code to the target. Click
*                    'Reset Go' icon on the 'Debug Run' toolbar to start the
*                     software.
*
*                 2. Press SW1.
*
*                 3. The debug LCD will display the captured interval between
*                    the switch press and release. To observe the interval
*                    value in the HEW watch window, right-click on the variable
*                    g_input_capture_value and select 'Add Watch'.
*
*                 2. Go to Step 3) to measure another interval.
*
* Note          : Since the trigger source for the timer is a non-debounced
*                 switch, occasionally a switch bounce will cause 0ms to be
*                 displayed. This does not represent the final accuracy of the
*                 timer. Do not press any switches other than switch SW1.
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
#include "timer_capture.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/

/* Variable for storing the captured interval value */
uint16_t g_input_capture_value = 0;

/******************************************************************************
* Function Name: main
* Description  : The main program function. The function displays the splash
*                screen on the LCD display, initialises the TPU3 channel, then
*                waits in an infinite while loop.
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
    Display_LCD(LCD_LINE1, "Capture ");
    Display_LCD(LCD_LINE2, "Push SW1");

    /* Initialise TPU timer registers */
    init_timer_capture();

    /* Infinite while loop */
    while (1)
    {
        /* Sample operation is performed by interrupt routine */
    }
}
/******************************************************************************
* End of function main
******************************************************************************/

