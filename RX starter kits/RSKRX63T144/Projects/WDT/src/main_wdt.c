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
* File Name     : main_wdt.c
* Version       : 1.00
* Device        : R5F563TE
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX63T144
* Description   : This sample demonstrates use of the WDT unit, by creating a
*                 periodic CMT interrupt that resets the WDT count. The CMT
*                 interrupt period is set by the potentiometer, which can be
*                 adjusted so that it is too slow to reset the WDT count and
*                 allows a WDT interrupt.
* Operation     : Before proceeding, ensure that the potentiometer RV1 is
*                 to rotated fully anti-clockwise.
*                 1. Compile and download the sample code to the target. Click
*                    the 'Resume' icon on the 'Debug' toolbar to start
*                    the software.
*
*                 2. The debug LCD will display the sample name, and the user
*                    LEDs will flash.
*
*                 3. Gradually rotate the pot clockwise, and observe the user
*                    LED flash rate slowing down.
*
*                 4. At approximately 2/3 of the full rotation the WDT interrupt
*                    will occur. The debug LCD will indicate the WDT interrupt
*                    has occured and the user LEDs will all remain lit.
*
*                 5. Turn the pot fully anti-clockwise and reset the MCU to
*                    start the sample again.
*******************************************************************************/
/*******************************************************************************
* History       : 05.12.2013  Ver. 1.00 First Release
*******************************************************************************/
/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/
/* Defines WDT functions used in this file */
#include "wdt.h"

/* Defines LCD functions used in this file */
#include "lcd.h"

/* This module's header file */
#include "main_wdt.h"

/* Intrinsics */
#include <intrinsics.h>

/******************************************************************************
* Function Name : main
* Description   : The main program function. This first displays the smaple name
*                 on the debug LCD, calls the WDT initialisation function then
*                 waits in an infinite while loop. The rest of the sample's
*                 functionality occurs in interrupts.
* Argument      : none
* Return value  : none
******************************************************************************/
void main (void)
{
    /* Enable interrupts */
    __enable_interrupt();

    /* Initialise the LCD Display */
    Init_LCD();

    /* Initialise the WDT sample */
    Init_WDT_Sample();

    /* Display the sample name on the debug LCD */
    Display_LCD(LCD_LINE1, "Watchdog");
    Display_LCD(LCD_LINE2, "        ");

    /* Infinite while loop */
    while (1)
    {
        /* Main program functionality is in interrupts */
    }
}
/******************************************************************************
* End of function main
******************************************************************************/

