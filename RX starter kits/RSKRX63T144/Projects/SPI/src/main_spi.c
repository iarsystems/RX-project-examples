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
* File Name     : main_spi.c
* Version       : 1.00
* Device        : R5F563TE
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX63T144
* Description   : This sample demonstrates the SPI unit, by transferring data
*                 from the SPI channel in loopback mode.
* Operation     : 1. Compile and download the sample code to the target. Click
*                    the 'Resume' icon on the 'Debug' toolbar to start the
*                    software.
*
*                 2. Adjust the potentiometer 'RV1'.
*
*                 3. Press switch SW3 to display the 12-bit A/D value
*                    corresponding to the potentiometer position on the debug
*                    LCD. Note that this data is sent through the SPI
*                    interface loopback before being displayed.

*                 4. A message will be displayed on the debug LCD if the
*                    transfer fails.
*
*                 5. The data can be watched in the variable 'g_rx_data'
*                    Double click the variable, and add it to the Expressions
*                    window.  Do the same with the variable 'g_tx_data'.
*                    The contents of the 'g_tx_data'  (transmitted data)
*                    & 'g_rx_data' (received data) should match.
*
*                 6. Go to Step 2 to repeat the test.
*******************************************************************************/
/*******************************************************************************
* History       : 05.12.2013  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/

/* LCD controlling function prototypes & macro definitions */
#include "lcd.h"

/* Defines the serial peripheral interface's functions used in this file */
#include "spi.h"

/* Defines ADC configuration functions used in this file */
#include "adc.h"

/* This module's header file */
#include "main_spi.h"

/* Intrinsics */
#include <intrinsics.h>

/******************************************************************************
* Function Name: main
* Description  : Main program function. This function first initialises the
*                debug LCD, and displays the name of the sample. The function
*                then calls the Init_SPI function, which configures the
*                Serial Peripheral Interface module. The program then waits in
*                an infinite loop.
* Arguments    : none
* Return Value : none
******************************************************************************/
void main (void)
{
    /* Enable interrupts */
    __enable_interrupt();

    /* Initialise the debug LCD display */
    Init_LCD();

    /* Display the application name on the debug LCD */
    Display_LCD(LCD_LINE1, "  SPI   ");
    Display_LCD(LCD_LINE2, "Push SW3");

    /* Set up the ADC */
    Init_ADC();

    /* Configure the SPI module */
    Init_SPI();

    /* Infinite while loop */
    while (1)
    {
        /* Sample operation is performed by interrupt routine */
    }
}
/*******************************************************************************
* End of function main
******************************************************************************/

