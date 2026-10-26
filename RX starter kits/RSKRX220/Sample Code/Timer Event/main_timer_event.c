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
* File Name     : main_timer_event.c
* Version       : 1.00
* Device        : R5F52206
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX220
* Description   : This sample demonstrates usage of the timer unit, with an
*                 external clock, by counting switch SW2 presses.
*
* Operation     : 1. Connect J1-17 (SW2) and JA3-7 (A6/TMCI3**).
*
*                 2. Build and download the sample code to the RSK.
*
*                 3. Click 'Reset Go' to start the software. Instructions will
*                    be displayed on the debug LCD.
*
*                 4. Press switch SW2, and observe the count value displayed on
*                    debug LCD. The value will increment each time the switch
*                    is pressed.
*
*                 5. Keeping pressing the switch, and the value on the LCD
*                    will increment up to 99, then loop back around to 0. The
*                    value is also available in the variable g_event_count.
*
* Note          : **A6 and TMCI3 are multiplexed on pin 6 of PortA.
*                 Since the clock source for the timer is a non-debounced
*                 switch, count may increment by more than one with each switch
*                 press. This does not represent the final accuracy of the
*                 timer. Do not press any other switches other than switch SW2.
******************************************************************************/
/******************************************************************************
* History       : 14.12.2012  Ver. 1.00 First Release
******************************************************************************/

/*******************************************************************************
* System Includes
*******************************************************************************/
#include <intrinsics.h>

/******************************************************************************
* Project Includes
******************************************************************************/

/* Defines debug LCD functions used in this file */
#include "lcd.h"

/* Defines timer functions used in this file */
#include "timer_event.h"

/******************************************************************************
* Function Name: main
* Description  : Main program function. The function displays instructions
*                on the debug LCD, then initialises a timer channel to be
*                clocked from SW2 switch presses.
* Arguments    : none
* Return Value : none
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
    init_timer_event();

    /* Infinite while loop */
    while (1)
    {
        /* Sample operation is performed by interrupt routine */
    }
}
/******************************************************************************
* End of function main
******************************************************************************/

