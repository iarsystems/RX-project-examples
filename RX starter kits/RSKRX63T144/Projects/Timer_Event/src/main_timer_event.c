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
* File Name     : main_timer_event.c
* Version       : 1.00
* Device        : R5F563TE
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX63T144
* Description   : This sample demonstrates usage of the timer unit with an
*                 external clock, by counting switch SW2 presses.
*
* Operation     : 1. Connect JA2 pin 9 (SW2) and J3 pin 22 (MTCLKA).
*
*                 2. Build and download the sample code to the RSK.
*
*                 3. Click 'Resume' to start the software. Instructions will
*                    be displayed on the debug LCD.
*
*                 4. Press switch SW2, and observe the count value displayed on
*                    debug LCD. The value will increment each time the switch is
*                    pressed.
*
*                 5. Keep pressing the switch, the value will increment
*                    up to 99, then loop back around to 0.
*
* Note          : Since the clock source for the timer is a non-debounced
*                 switch, count may increment by more than one with each switch
*                 press. This does not represent the final accuracy of the
*                 timer. Do not press any other switches other than switch SW2.
******************************************************************************/
/******************************************************************************
* History       : 05.12.2013 Ver. 1.00 First Release
******************************************************************************/

/******************************************************************************
* Project Includes
******************************************************************************/
/* Defines debug LCD functions used in this file */
#include "lcd.h"

/* Defines timer functions used in this file */
#include "timer_event.h"

/* This module's header file */
#include "main_timer_event.h"

/* Intrinsics */
#include <intrinsics.h>

/******************************************************************************
* Function Name : main
* Description   : Main program function. The function displays instructions
*                 on the debug LCD, then initialises a timer channel to be
*                 clocked from SW2 switch presses.
* Argument      : none
* Return value  : none
******************************************************************************/
void main (void)
{
    /* Enable interrupts */
    __enable_interrupt();

    /* Initialise the debug LCD */
    Init_LCD();

    /* Display instructions on the debug LCD */
    Display_LCD(LCD_LINE1, "TR Event");
    Display_LCD(LCD_LINE2, "Push SW2");

    /* Initialise the timer unit */
    Init_TimerEvent();

    /* Infinite while loop */
    while (1)
    {
        /* Functionality is in interrupts */
    }
}
/******************************************************************************
* End of function main
******************************************************************************/

