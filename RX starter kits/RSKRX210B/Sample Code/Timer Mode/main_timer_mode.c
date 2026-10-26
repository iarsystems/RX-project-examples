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
* File Name     : main_timer_mode.c
* Version       : 1.00
* Device        : R5F5210B
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX210B
* Description   : This sample uses channel 4 of the MTU2 to output a 1KHz square
*                 waveform on the MTIOC4A output pin.
* Operation     : 1. Connect an oscilloscope probe to the header pin J1-24.
*
*                 2. Compile and download the sample code to the target. Click
*                    the 'Reset Go' icon on the 'Debug Run' toolbar to start the
*                    software.
*
*                 3. A 1 KHz square wave output will be displayed on the
*                    oscilloscope.
*******************************************************************************/
/*******************************************************************************
* History       : 14.05.2013  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* System Includes
*******************************************************************************/
#include <intrinsics.h>

/******************************************************************************
User Includes (Project Level Includes)
******************************************************************************/
/* LCD controlling function prototypes & macro defines */
#include "lcd.h"
/* Prototypes for Timer control functions */
#include "timer_mode.h"
/* MCU and RSK hardware initialization */
#include "hwsetup.h"

/******************************************************************************
* Outline       : main
* Description   : The main program function. The function displays the splash
*                 screen on the LCD display, initialises the timer output, then
*                 waits in an infinite while loop.
* Argument      : none
* Return value  : none
******************************************************************************/
void main(void)
{
    /* Configure the MCU and RSK hardware */
    HardwareSetup();

    /* Enable interrupts */
    __enable_interrupt();

    /* Initialise the LCD Display */
    Init_LCD();

    /* Display Splash Screen */
    Display_LCD(LCD_LINE1, " 1 KHz  ");
    Display_LCD(LCD_LINE2, " J1 P24 ");

    /* Initialise timer */
    Init_TimerMode();

    /* Infinite while loop */
    while(1);
}
/******************************************************************************
* End of function main
******************************************************************************/
