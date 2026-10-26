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
/*******************************************************************************
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.    */
/*******************************************************************************
* File Name     : main_serial_sio.c
* Version       : 2.00
* Device        : R5F562GAADFP
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX62G
* Description   : This sample demonstrates usage of the SCI unit in synchronous
*                 mode by performing a loop back test between two SCI channels.
* Operation     : Before starting, configuration is set:
*
*                 Resistor Modifications
*                 ----------------------
*                 Fit: R110, R107 - Remove: R111, R108
*                    Note: This configuration will disconnect CRX-A & CTX-A.
*
*                 Wire Connections
*                 ----------------
*                 JA6, pin 11 (SCK2-A) to JA2, pin 10 (SCK0)
*                 JA6, pin 12 (RXD2-A) to JA2, pin 6 (TXD0)
*                 JA6, pin 9 (TXD2-A) to JA2, pin 8 (RXD0)
*
*
*                 1. Build and download the sample code to the RSK
*
*                 2. Right click the variable gSCI0_RXBuffer and select "Add
*                    Watch", and do the same for the variable gSCI2_TXBuffer.
*
*                 3. Click 'Reset Go' to start the software. The name of the
*                    sample will be displayed on the debug LCD.
*
*                 4. The sample will then being the data transfer. When the SCI
*                    activity is complete, the second line of the debug LCD will
*                    display either "Success" or "Failure".
*
*                 5. You can observe the contents of gSCI0_RXBuffer being filled
*                    with the contents of gSCI2_TXBuffer. Click 'Reset Go' to
*                    start the sample again.
*
*                 6. If the debug LCD reports "Failure", or shows " RX/TX "
*                    without changing; check the configuration specified is set
*                    and click "Reset Go" to start again.
******************************************************************************/
/******************************************************************************
* History       : 15.10.2010 Ver. 1.00 First Release
                : 21.05.2011 Ver. 2.00 Second Release (Updated to RPDL CS3)
                : 06.09.2012 Ver. 3.00 Migrated to RX62G (RPDL CS1)
******************************************************************************/

/******************************************************************************
* Project Includes
******************************************************************************/
/* Defines debug LCD functions used in this file */
#include "lcd.h"
/* Defines synchronous serial functions used in this file */
#include "sync.h"

#include "hwsetup.h"
#include "intrinsics.h"

/******************************************************************************
* Global Variables
******************************************************************************/
/* Declaration of the SCI0 transmit buffer */
extern uint8_t gSCI0_TXBuffer[];
/* Declaration of the SCI2 transmit buffer */
extern uint8_t gSCI2_TXBuffer[];
/* Declaration of SCI0 and SCI2 receive buffers */
extern uint8_t gSCI0_RXBuffer[26], gSCI2_RXBuffer[26];

/******************************************************************************
* Outline      : main
* Description  : Main program function. This function first intialises the debug
*                LCD, and displays the name of the sample. Main then intialises
*                the SCI unit, and performs a data transfer from channel 6 to
*                channel 2, then another transfer in the other direction. After
*                the second transfer, the debug LCD displays either "Success" or
*                "Failure". The main function finally waits in an infinite while
*                loop.
* Argument     : none
* Return value : none
*******************************************************************************/
void main(void)
{
    /* Do the hardware setup */
    HardwareSetup();

    /* Enable interrupts */
    __enable_interrupt();

    /* Initialise the debug LCD */
    InitialiseLCD();

    /* Display the sample name on the debug LCD, and RX/TX to indicate the SCI
       data transfers are in progress */
    DisplayLCD(LCD_LINE1, "Sync SCI");
    DisplayLCD(LCD_LINE2, " RX/TX  ");

    /* Initialise the SCI unit */
    Init_Sync();

    /* Configure the SCI channels, and start a transfer from channel 2 to 0 */
    SCI2toSCI0Transfer_Sync();

    /* Configure the SCI channels, and start a transfer from channel 0 to 2,
       then report the results onto the debug LCD */
    SCI0toSCI2Transfer_Sync();

    /* Infinite while loop */
    while(1);
}
/******************************************************************************
* End of function main
******************************************************************************/
