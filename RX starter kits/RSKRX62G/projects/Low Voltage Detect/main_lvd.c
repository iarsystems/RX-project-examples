/* Adapted for use with IAR Embedded Workbench */
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
* File Name     : main_lvd.c
* Version       : 1.00
* Device        : R5F562GAADFP
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX62G
* Description   : This sample demonstrates usage of the low voltage detection
*                 circuit.
* Operation     : 1. Connect a 5.0V regulated variable power supply to the RSK.
*
*                 2. Set the voltage level to 5.0V.
*
*                 3. Build and download the sample code to the RSK.
*
*                 4. Observe all LEDs flashing.
*
*                 5. When the power supply equals or falls below the Vdet1's
*                    threshold (approximately 4.15V), all LEDs are turned off.
******************************************************************************/
/******************************************************************************
* History       : 06.09.2012 Ver. 1.00 First Release
******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
/* Defines debug LCD functions used in this file */
#include "lcd.h"
/* Defines functions and variables used in lvd.c file */
#include "lvd.h"

#include "hwsetup.h"
#include "intrinsics.h"

/*******************************************************************************
* Outline      : main
* Description  : Main program function. This function first initialises the
*                debug LCD, and displays the name of the sample. The function
*                then calls the Init_RSPI function, which configures the
*                serial peripheral interface. The program then waits in an
*                infinite while loop
* Argument     : none
* Return value : none
******************************************************************************/
void main(void)
{
    /* Do the hardware setup */
    HardwareSetup();

    /* Enable interrupts */
    __enable_interrupt();

    /* Initialise the debug LCD display */
    InitialiseLCD();

    /* Display the sample name on the debug LCD */
    DisplayLCD(LCD_LINE1, "  LVD   ");

    /* Configure the watchdog timer and timer reset interrupts */
    Init_LVD();

    /* Infinite while loop */
    while(1);
}
/******************************************************************************
* End of function main
******************************************************************************/
