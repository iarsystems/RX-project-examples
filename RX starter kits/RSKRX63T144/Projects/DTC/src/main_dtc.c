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
* File Name     : main_dtc.c
* Version       : 1.00
* Device        : R5F563TE
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX63T144
* Description   : This sample demonstrates the DTC unit by performing a DTC
*                 transfer from S12AD unit into a 16 element array.
* Operation     : 1. Compile and download the sample code to the target.
*
*                 2. Double-click the variable 'g_dtc_destination' and drag
*                    it into the Expressions window. Right-click the variable
*                    'g_dtc_destination' in the Expressions window and select
*                    'Real-time Refresh'.
*
*                 3. The contents of the variable 'g_dtc_destination' can be
*                    viewed in the watch window.
*
*                 4. Click the 'Resume' icon on the 'Debug' toolbar to
*                    start the software. Instructions will be displayed on the
*                    debug LCD.
*
*                 5. Adjust the potentiometer, RV1, and press the switch SW1.
*                    The result from the ADC conversion will be transferred
*                    into the first element of the transfer destination array.
*
*                 6. Re-press the switch SW1, and subsequent results will be
*                    transferred into the the next array element.
*
*                 7. Once the last array has been filled, the next transfer
*                    will clear the array contents and start from the first
*                    element.
*
*                 8. Whenever the DTC completes transfer of 2 bytes of data,
*                    the LED0 toggles its state.
*
* Note          : 1. In order to watch the contents of the g_dtc_destination
*                    array during execution, the sample must be built in
*                    'debug' mode.
*                 2. Place a breakpoint inside the callback function
*                    cb_switch_dtc at the call of 'memset' function.
*                    This enables the viewing of the variable
*                    'g_dtc_destination' with every array element filled up
*                    before they are cleared.
*******************************************************************************/
/*******************************************************************************
* History       : 05.12.2013  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/
/* LCD controlling function prototypes & macro definitions */
#include "lcd.h"

/* Defines DTC functions used in this file */
#include "dtc.h"

/* This module's header file */
#include "main_dtc.h"

/* Intrinsics */
#include <intrinsics.h>

/*******************************************************************************
* Function Name : main
* Description   : Main program function. This function first initialises the
*                 debug LCD and displays instructions on the screen. Main then
*                 calls the DTC initialisation function, which configures the
*                 ADC & DTC module. The function then returns to main, where it
*                 waits in an infinite while loop. When the user presses SW1,
*                 the ADC data is transferred to the 'g_dtc_destination' by the
*                 DTC. The LED0 toggles every time the DTC transfers 16 bytes
*                 of data.
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
    Display_LCD(LCD_LINE1, "  DTC   ");
    Display_LCD(LCD_LINE2, "Push SW1");

    /* Initialise the DTC module */
    Init_DTC();

    /* Infinite while loop */
    while (1)
    {
        /* Functionality is performed by DTC on each ADC conversion interrupt */
    }
}
/******************************************************************************
* End of function main
******************************************************************************/

