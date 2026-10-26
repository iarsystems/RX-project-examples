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
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.    */
/*******************************************************************************
* File Name     : main_timer_capture.c
* Version       : 1.00
* Device        : R5F562GAADFP
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX62G
* Description   : Defines the main program code
* Operation     : 1. Build and download the sample code to the RSK.
*
*                 2. Click 'Reset Go' to start the software. Instructions will
*                    be displayed on the debug LCD.
*
*                 3. Press switch SW1, and observe the result on the debug LCD.
*                    The time duration, in milliseconds (ms), will be displayed.
*                    If SW1 is held for more than 10 seconds, ">10 sec" is
*                    displayed.
*
*                 4. Repress switch SW1 to repeat the test.
*
* Note          : The sample is intended as a demonstration, and does not repre-
*                 sent the final accuracy of the MCU.
******************************************************************************/
/******************************************************************************
* History       : 06.09.2012 Ver. 1.00 First Release
******************************************************************************/

/******************************************************************************
* Project Includes
******************************************************************************/
/* Defines debug LCD functions used in this file */
#include "lcd.h"
/* Defines timer functions used in this file */
#include "timer_capture.h"

#include "hwsetup.h"
#include "intrinsics.h"

/******************************************************************************
* Outline       : main
* Description   : Main program function. This function first displays instruct-
*                 ions the debug LCD, then initialises the timer unit. The
*                 function then waits in an infinite while loop, where the rest
*                 of the sample's functionality is achieved through interrupts.
* Argument      : none
* Return value  : none
******************************************************************************/
void main(void)
{
    /* Do the hardware setup */
    HardwareSetup();

    /* Enable interrupts */
    __enable_interrupt();

    /* Initialise the debug LCD display */
    InitialiseLCD();

    /* Display instructions on the debug LCD */
    DisplayLCD(LCD_LINE1, "Capture ");
    DisplayLCD(LCD_LINE2, "Push SW1");

    /* Initialise the timer unit */
    Init_TimerCapture();

    /* Infinite while loop */
    while(1);
}
/******************************************************************************
* End of function main
******************************************************************************/
