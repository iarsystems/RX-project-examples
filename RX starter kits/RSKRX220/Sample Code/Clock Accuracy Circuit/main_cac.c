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
* File Name     : main_cac.c
* Version       : 1.00
* Device        : R5F52206
* Tool-Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX220
* Description   : Demonstration of the Clock Accuracy Circuit (CAC).
*                 The program configures the CAC to measure the frequency of
*                 the High-Frequency On-Chip Oscillator (HOCO) and display it
*                 on the Debug LCD.
* Operation     :   1. Compile and download the sample code to the target.
*                      Click 'Reset Go' icon on the 'Debug Run' toolbar to
*                      start the software.
*
*                   2. Observe the LCD display - the RX220 CPU will display the
*                      frequency of the HOCO in kHz, using the main clock as a
*                      reference.
*
*                   3. If the HOCO frequency becomes outside 32MHz +/- 10% the
*                      frequency error interrupt handler will run and "Freq Err"
*                      will be displayed on the debug LCD.
*
* Note          : By connecting an external 1kHz clock to the CACREF pin (JA3-1)
*                 and setting USE_INTERNAL_CLOCK to 0 in cac.h, the HOCO may be
*                 measured against an external source.
*                 Varying the external clock source will also cause the
*                 frequency error interrupt to take place.
*
*******************************************************************************/
/*******************************************************************************
* History       : 14.12.2012  Ver. 1.00 First Release
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

/* CAC oneshot function definitions */
#include "cac.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/

/*******************************************************************************
* Function Name: main
* Description  : The main program function. This function initialises the
*                debug LCD and displays the sample name. It then calls the CAC
*                initialisation function, then waits in an infinite while
*                loop whilst the CAC results are written to the debug LCD by a
*                timer interrupt.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void main (void)
{
    /* Enable interrupts */
    __enable_interrupt();

    /* Initialise the debug LCD */
    Init_LCD();

    /* Display sample name on the debug LCD */
    Display_LCD(LCD_LINE1, "  CAC   ");

    /* Initialise the CAC module */
    init_cac();

    /* Infinite while loop */
    while (1)
    {
        /* Sample operation is performed by interrupt routine */
    }
}
/******************************************************************************
* End of function main
******************************************************************************/

