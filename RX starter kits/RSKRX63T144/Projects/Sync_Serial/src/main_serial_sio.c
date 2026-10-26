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
* File Name     : main_serial_sio.c
* Version       : 1.00
* Device        : R5F563TE
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX63T144
* Description   : This sample demonstrates usage of the SCI unit in synchronous
*                 mode by performing a loop back test between two SCI channels.
* Operation     : Before starting, ensure the RSK is configured as follows:
*
*                 Wire Connections
*                 ----------------
*                 J2, pin 11 (SCK0) to J1, pin 13 (SCK2)
*                 J2, pin 13 (RXD0) to J1, pin 5  (TXD2)
*                 J2, pin 12 (TXD0) to J1, pin 4  (RXD2)
*
*
*                 1. Build and download the sample code to the RSK
*
*                 2. Double click the variable g_sci0_rx_buffer and drag it into
*                    the Expressions window, and do the same for the variables
*                    g_sci2_tx_buffer, g_sci0_tx_buffer and g_sci2_rx_buffer.
*                    From the Expressions window, right-click on the buffers
*                    and select 'Real-time Refresh'.
*
*                 3. Click 'Resume' to start the software. The name of the
*                    sample will be displayed on the debug LCD.
*
*                 4. The sample will then begin the data transfer. When the SCI
*                    activity is complete, the second line of the debug LCD will
*                    display either "Success" or "Failure".
*
*                 5. You can observe that the contents of g_sci0_rx_buffer have
*                    filled with the contents of g_sci2_tx_buffer. Likewise the
*                    contents of g_sci2_rx_buffer have been filled with that of
*                    g_sci0_tx_buffer.
*
*                 6. If the debug LCD reports "Failure", or shows " RX/TX "
*                    without changing; check the configuration specified is set
*                    and click "Resume" to start again.
******************************************************************************/
/******************************************************************************
* History       : 05.12.2013  Ver. 1.00 First Release
******************************************************************************/

/******************************************************************************
* Project Includes
******************************************************************************/
#include <stdint.h>

/* Following header file provides string type definitions. */
#include <string.h>

/* Following header file provides Boolean type definitions */
#include <stdbool.h>

/* Defines debug LCD functions used in this file */
#include "lcd.h"

/* Defines synchronous serial functions used in this file */
#include "sync.h"

/* This module's header file */
#include "main_serial_sio.h"

/* Intrinsics */
#include <intrinsics.h>

/******************************************************************************
* Global Variables
******************************************************************************/

/******************************************************************************
* Function Name: main
* Description  : Main program function. This function first intialises the debug
*                LCD, and displays the name of the sample. Main then intialises
*                the SCI unit, and performs a data transfer from channel 0 to
*                channel 12, then another transfer in the other direction. After
*                the second transfer, the debug LCD displays either "Success" or
*                "Failure". The main function finally waits in an infinite while
*                loop.
* Argument     : none
* Return value : none
*******************************************************************************/
void main (void)
{
    /* Enable interrupts */
    __enable_interrupt();

    /* Initialise the debug LCD */
    Init_LCD();

    /* Display the sample name on the debug LCD, and RX/TX to indicate the SCI
       data transfers are in progress */
    Display_LCD(LCD_LINE1, "Sync SCI");
    Display_LCD(LCD_LINE2, " RX/TX  ");

    /* Initialise the SCI unit */
    Init_Sync();

    /* Configure the SCI channels, and start a transfer from channel 0 to 2 */
    SCI0toSCI2Transfer_Sync();

    /* Configure the SCI channels, and start a transfer from channel 2 to 0 */
    SCI2toSCI0Transfer_Sync();

    /* Check if the transfers were successful, report the results to the LCD */
    Transfer_End();

    /* Infinite while loop */
    while (1)
    {
        /* Do nothing */
    }
}
/******************************************************************************
* End of function main
******************************************************************************/

