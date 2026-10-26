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
* File Name     : main_iwdt.c
* Version       : 1.00
* Device        : R5F563TE
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX63T144
* Description   : This sample demonstrates usage of the independent watchdog
*                 timer, by configuring the timer to require periodic resets to
*                 prevent a timer underflow. The sample configures a periodic
*                 timer reset which has a period that varies with the position
*                 of the potentiometer, RV1. The LEDs flash to indicate each
*                 period.
* Operation     : 1. Build and download the sample code to the RSK.
*
*                 2. Ensure the potentiometer, RV1, is turned to the fully anti-
*                    clockwise position before starting the sample. To start the
*                    sample, click 'Resume'.
*
*                 3. The debug LCD will display the sample name, and the user
*                    LEDs will begin to flash.
*
*                 4. Slowly turn the potentiometer RV1 clockwise, and observe
*                    the gradual decrease of the LED flash rate. Each time the
*                    LEDs toggle output state, the independent watchdog timer is
*                    refreshed, preventing it from underflowing which would
*                    cause a system reset.
*
*                 5. Continue to turn the potentiometer clockwise - the LED
*                    flashing will slow down to a point where the independent
*                    watchdog timer is allowed to underflow.  At this point a
*                    system reset will occur, all the LEDs will be turned on,
*                    and the program will wait in an infinite while loop.
*
*                 6. The debug LCD will display:
*                    "  IWDT  "
*                    "Underflo".
*
*                 7. To repeat the sample, ensure RV1 is fully anti-clockwise,
*                    click 'Suspend' and click 'Restart'. Click 'Resume' to run
*                    the sample again.
*******************************************************************************/
/*******************************************************************************
* History       : 05.12.2013  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* Project Includes
******************************************************************************/

/* Defines debug LCD functions used in this file */
#include "lcd.h"

/* Defines the independent watchdog timer functions used in this file */
#include "iwdt.h"

/* Defines the independent watchdog timer main function in this file */
#include "main_iwdt.h"

/* Intrinsics */
#include <intrinsics.h>

/******************************************************************************
* Function Name: main
* Description  : Main program function. This function first initialises the
*                debug LCD, and displays the name of the sample. The function
*                then calls the Init_IWDT function, which configures the
*                independent watchdog timer, and the LED flash reset interrupts.
*                After the initialisation function has been called, main waits
*                in an infinite while loop.
* Arguments    : none
* Return Value : none
******************************************************************************/
void main (void)
{
    /* Enable interrupts */
    __enable_interrupt();

    /* Initialise the debug LCD display */
    Init_LCD();

    /* Display the sample name on the debug LCD */
    Display_LCD(LCD_LINE1, "  IWDT  ");

    /* Configure the watchdog timer and timer reset interrupts */
    Init_IWDT();

    /* Infinite while loop */
    while (1)
    {
        /* Do nothing - see interrupt functions in iwdt.c */
    }
}

/******************************************************************************
* End of function main
******************************************************************************/

