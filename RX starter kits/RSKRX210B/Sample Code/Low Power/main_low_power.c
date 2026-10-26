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
* File Name     : main_low_power.c
* Version       : 1.00
* Device        : R5F5210B
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX210B
* Description   : This sample demonstrates the MCU operating modes. The CPU
*                 changes the modes with the key presses. To check the current
*                 drawn by the CPU, please remove the resistor R68 & connect
*                 an ammeter across J14. Please refer to sec. 3.3 (component
*                 placement) in the User's Manual to locate the resistor
*                 R68.
*
* Operation    : 1. Compile and flash the sample code to the target.
*
*                2. Disconnect the debugger and connect a 3.3V supply to the
*                   RSK.
*
*                3. At startup the RX210B CPU will operate in high speed
*                   operating mode. Observe the current drawn by the CPU.
*
*                4. Pressing the user switches SW1 to SW3 will change the
*                   operating and power mode of the MCU as specified below.
*                           __________________________________________________
*                           | Power Mode        | Operating Mode              |
*                    _______|___________________|_____________________________|
*                   | SW1   | Normal Power Mode | Low Speed Operating Mode 2  |
*                   | SW2   | Software Standby  | X*                          |
*                   | SW3   | Normal Power Mode | High Speed Operating Mode   |
*                   |_______|___________________|_____________________________|
*                       * Operating mode is preserved from previous setting
*
*       NOTE    : 1. As the MCU supply routes through the ammeter (connected
*                    across J14), changing the ammeter settings while the
*                    application is running will interrupt the MCU supply and
*                    will give erroneous results.
*
*                 2. The debug LCD has not been used in this application as the
*                    supplied 3.3V supply is not sufficient to drive the debug
*                    LCD.
*******************************************************************************/
/*******************************************************************************
* History       : 14.05.2013  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* System Includes
*******************************************************************************/
#include <intrinsics.h>

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/
/* Contains prototypes of the functions called in this file */
#include "low_power.h"
/* MCU and RSK hardware initialization */
#include "hwsetup.h"

/******************************************************************************
* Outline      : main
* Description  : Main program function.
* Argument     : none
* Return value : none
******************************************************************************/
void main(void)
{
    /* Configure the MCU and RSK hardware */
    HardwareSetup();

    /* Enable interrupts */
    __enable_interrupt();

    /* Initialise the low power modes */
    Init_LowPower();

    /* Infinite while loop */
    while (1);
}
/******************************************************************************
* End of function main
******************************************************************************/
