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
* File Name     : main_doc.c
* Version       : 1.00
* Device        : R5F52206
* Tool-Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX220
* Description   : This sample demonstrates the DOC module. The application
*                 checks the input data against the reference constant data set
*                 in 'g_compare_reference_doc'.
* Operation     : 1. Connect the RSK to a PC with a 9-way, straight through
*                    serial cable.
*
*                 2. Open a terminal program (e.g HyperTerminal) and ensure
*                    the following settings are set:
*
*                    Baud Rate.............19200
*                    Data Bits.................8
*                    Parity.................None
*                    Stop Bits.................1
*                    Flow Control...........None
*
*                 3. Compile and download the sample code to the target. click
*                    'Reset Go' icon on the 'Debug Run' toolbar to start the
*                    software.
*
*                 4. Instructions will be displayed on the terminal window.
*                    Enter a number into the terminal, and it will display
*                    the comparison result using DOC unit.
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

/* LCD controlling function prototypes & macro defines */
#include "lcd.h"

/* Contains DOC function prototypes */
#include "doc.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Set input reference value */
const uint16_t g_compare_reference_doc = 1000;

/*******************************************************************************
* Function Name: main
* Description  : Main program function. This function first initialises the
*                debug LCD, and displays the name of the sample on the LCD.
*                The function then calls the init_doc initialisation function,
*                which configures the SCI and DOC units.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void main (void)
{
    /* Enable interrupts */
    __enable_interrupt();

    /* Initialise the LCD Display */
    Init_LCD();

    /* Display application name on the LCD */
    Display_LCD(LCD_LINE1, "REF=1000");
    Display_LCD(LCD_LINE2, "  DOC   ");

    /* Initialise DOC */
    init_doc();

    /* Infinite while loop */
    while (1)
    {
        /* Sample operation is performed by interrupt routine */
    }
}
/*******************************************************************************
* End of function main
*******************************************************************************/

