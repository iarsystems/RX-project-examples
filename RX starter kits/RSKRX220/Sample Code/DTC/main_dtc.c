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
* File Name     : main_dtc.c
* Version       : 1.00
* Device        : R5F52206
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX220
* Description   : This sample demonstrates the DTC unit by performing a DTC
*                 transfer from S12AD unit into a 16 element array.
* Operation     : 1. Compile and download the sample code to the target.
*
*                 2. Right-click the variable 'g_dtc_destination' and select
*                    'Add Watch'. Right-click the variable 'g_dtc_destination'
*                    in the Watch window and select 'Enable Auto Update'.
*
*                 3. The contents of the variable 'g_dtc_destination' can be
*                    viewed in the watch window.
*
*                 4. Click 'Reset Go' icon on the 'Debug Run' toolbar to start
*                    the software. Instructions will be displayed on the debug
*                    LCD.
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
*                 2. Place a breakpoint inside the callback function CB_Switch
*                    at the call of 'memset' function.
*                    This enables the viewing of the variable 'g_dtc_destination'
*                    with every array element filled up before they are cleared.
*******************************************************************************/
/*******************************************************************************
* History       : 14.12.2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* System Includes
*******************************************************************************/
#include <intrinsics.h>

/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/

/* LCD controlling function prototypes & macro definitions */
#include "lcd.h"

/* Defines DTC functions used in this file */
#include "dtc.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/

/* Declare the DTC transfer destination array */
uint16_t g_dtc_destination[16];

/*******************************************************************************
* Function Name: main
* Description  : Main program function. This function first initialises the
*                debug LCD and displays instructions on the screen. Main then
*                calls the DTC initialisation function, which configures the
*                ADC & DTC module. The function then returns to main, where it
*                waits in an infinite while loop. When the user presses SW1,
*                the ADC data is transferred to the 'g_dtc_destination' by the
*                DTC. The LED0 toggles every time the DTC transfers 16 bytes
*                of data.
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
    Display_LCD(LCD_LINE1, "  DTC   ");
    Display_LCD(LCD_LINE2, "Push SW1");

    /* Initialise the DTC module */
    init_dtc();

    /* Infinite while loop */
    while (1)
    {
        /* Sample operation is performed by interrupt routine */
    }
}
/******************************************************************************
* End of function main
******************************************************************************/

