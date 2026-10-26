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
* File Name     : main_crc.c
* Version       : 1.00
* Device        : R5F562GAADFP
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX62G
* Description   : Defines the main program code
* Instructions  : 1. Connect the RSK to a PC with a 9-way, straight through
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
*                 3. Compile and download the sample code. Click 'Reset Go'
*                    to start the software.
*
*                 4. Instuctions will be displayed on the terminal window.
*                    Enter a character into the terminal, and it will display
*                    the corresponding 8bit CRC checksum.
******************************************************************************/
/******************************************************************************
* History       : 06.09.2012 Ver. 1.00 First Release
******************************************************************************/

/******************************************************************************
* Project level
******************************************************************************/
/* LCD function definitions */
#include "lcd.h"
/* CRC function definitions */
#include "crc.h"

#include "hwsetup.h"
#include "intrinsics.h"

/******************************************************************************
* Outline       : main
* Description   : Main program function. This function first initialises the
*                 debug LCD, and displays the name of the sample on the screen.
*                 The function then calls the Init_CRC initialisation function,
*                 which configures the SCI and CRC units. When the MCU returns
*                 to main, it enters an infinite while loop.
* Argument      : none
* Return value  : none
******************************************************************************/
void main(void)
{
    /* Do the hardware setup */
    HardwareSetup();

    /* Enable interrupts */
    __enable_interrupt();

    /* Initialise the LCD Display */
    InitialiseLCD();

    /* Display Splash Screen */
    DisplayLCD(LCD_LINE1, "  CRC   ");
    DisplayLCD(LCD_LINE2, "  Calc  ");

    /* Initialise CRC calculator */
    Init_CRC();

    /* Infinite while loop */
    while (1);
}
/******************************************************************************
* End of function main
******************************************************************************/
