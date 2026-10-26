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
* File Name     : main_cac.c
* Version       : 1.00
* Device        : R5F563TE
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX63T144
* Description   : Demonstration of the Clock Accuracy Circuit (CAC).
*                 The program configures the CAC to measure the frequency of
*                 the IWDTCLK signal and display it on the Debug LCD.
* Operation     :   1. Compile and download the sample code to the target.
*                      Click 'Resume' icon on the 'Debug' toolbar to
*                      start the software.
*
*                   2. Observe the LCD display - the program will display the
*                      frequency of the IWDTCLK in kHz, using the 3MHz PCLKB as
*                      a reference.
*
*                   3. If the IWDTCLK frequency moves out of specification the
*                      frequency error interrupt handler will run and "Freq Err"
*                      will be displayed on the debug LCD.
*                      From the hardware manual Revision 1.00 p1750 the LOCO
*                      (IWDTCLK) oscillator is specified as follows:
*                      fLOCO 106.25 KHz (min.) 125 KHz (typ.) 143.75 KHz (max.)
*
* Note          : By connecting an external 1kHz clock to the CACREF pin(JA2-20)
*                 and setting USE_INTERNAL_CLOCK to 0 in cac.h, the IWDTCLK may
*                 be measured against an external source.
*                 Varying the external clock source will also cause the
*                 frequency error interrupt to occur.
*
*******************************************************************************/
/*******************************************************************************
* History       : 05.12.2013  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/
/* CAC oneshot function definitions */
#include "cac.h"

/* LCD controlling function prototypes & macro definitions */
#include "lcd.h"

/* This module's header file */
#include "main_cac.h"

/* Intrinsics */
#include <intrinsics.h>

/******************************************************************************
* Function Name : main
* Description   : Main function first displays the sample name, then calls the
*                 Init_CAC function. The function then waits in an infinite
*                 while loop.
* Argument      : none
* Return value  : none
******************************************************************************/
void main (void)
{
    /* Enable interrupts */
    __enable_interrupt();

    /* Initialise the debug LCD */
    Init_LCD();

    /* Display sample name on the debug LCD */
    Display_LCD(LCD_LINE1, "  CAC   ");

    /* Initialise the CAC module */
    Init_CAC();

    /* Infinite while loop */
    while (1)
    {
        /* Sample operation is performed by interrupt routine */
    }
}
/******************************************************************************
* End of function main
******************************************************************************/

