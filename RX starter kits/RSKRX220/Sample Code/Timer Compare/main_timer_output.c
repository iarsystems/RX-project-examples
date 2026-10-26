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
* File Name     : main_timer_output.c
* Version       : 1.00
* Device        : R5F52206
* Tool-Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX220
* Description   : This sample triggers an interrupt function to toggle the user
*                 LEDs everytime a compare match occurs.
* Operation     : 1. Build and download the sample code to the RSK.
*
*                 2. Click 'Reset Go' to start the software.
*
*                 3. Observe the user LEDs, which will flash everytime a
*                    compare match occurs at 100ms intervals.
*******************************************************************************/
/*******************************************************************************
* History       : 14.12.2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* System Includes
*******************************************************************************/

/* Following header file provides standard integer type definitions. */
#include <stdint.h>

#include <intrinsics.h>

/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/

/* LCD controlling function prototypes & macro definitions */
#include "lcd.h"

/* Defines comapre match timer functions used in this file */
#include "timer_output.h"


/*******************************************************************************
* Function Name: main
* Description  : Main program function. The function first displays instructions
*                on the debug LCD, then initialises a CMT channel to execute a
*                callback function on every compare match.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void main (void)
{
    /* Enable interrupts */
    __enable_interrupt();

    /* Initialise the debug LCD */
    Init_LCD();

    /* Display the sample name */
    Display_LCD(LCD_LINE1, "Timer   ");
    Display_LCD(LCD_LINE2, "Compare ");

    /* Configure the CMT timer for compare match operation */
    init_compare_match_timer();

    /* Infinite while loop */
    while (1)
    {
        /* Sample operation is performed by interrupt routine */
    }
}
/*******************************************************************************
* End of function main
*******************************************************************************/

