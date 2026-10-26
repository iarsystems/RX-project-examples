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
* File Name     : main_voltage_detect.c
* Version       : 1.00
* Device        : R5F563TE
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX63T144
* Description   : This sample demonstrates usage of the voltage detect
*                 peripheral, by indicating when the power supply voltage
*                 falls below a certain threshold.
* Operation     : Before starting this sample, power the RSK board from a
*                 variable power supply instead of by the debugger. Set
*                 variable power supply to 5V initially. You will need to make
*                 the following jumper modifications:
*                           J8 - short 2 & 3.
*
*                 1. Compile and download the sample code to the target.
*                    Click 'Resume' icon on the 'Debug' toolbar to
*                    start the software.
*
*                 2. Observe all the user LEDs flashing, and that the debug LCD
*                    indicates that VCC > VDET (detection voltage).
*
*                 3. Gradually reduce the power supply voltage, until the user
*                    LEDs stop flashing and LED3 is lit constantly (at approx.
*                    4.5V). The debug LCD will display VCC < VDET.
*
*                 4. Raise the power supply voltage to above 4.7V and the user
*                    LEDs and debug LCD will return to their original state.
*
* Note           : DO NOT EXCEED 5.5V FOR THE POWER SUPPLY
*                  The debug LCD may appear dim as the power supply voltage is
*                  reduced, which is to be expected.
*******************************************************************************/
/*******************************************************************************
* History       : 05.12.2013  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/
/* Defines voltage detect functions used in this file */
#include "voltage_detect.h"

/* Defines LCD functions used in this file */
#include "lcd.h"

/* This module's header file */
#include "main_voltage_detect.h"

/* Intrinsics */
#include <intrinsics.h>

/******************************************************************************
* Function Name : main
* Description   : Main function first displays the sample name, and indicates
*                 that VCC (the supply voltage) is greater than Vdet (detection
*                 voltage). The function then calls the voltage detect initial-
*                 ise function. Finally the function then waits in an infinite
*                 while loop, repeatedly calling the LED flash function.
* Argument      : none
* Return value  : none
******************************************************************************/
void main (void)
{
    /* Enable interrupts */
    __enable_interrupt();

    /* Initialise the LCD Display */
    Init_LCD();

    /* Displays sample name on debug LCD, and indicates VCC > Vdet */
    Display_LCD(LCD_LINE1, "V.Detect");
    Display_LCD(LCD_LINE2, "VCC>VDET");

    /* Initialise the voltage detect registers */
    Init_VoltageDetect();

    /* Infinite while loop */
    while (1)
    {
        /* Toggle the user LEDs */
        LedFlash_VoltageDetect();
    }
}
/******************************************************************************
* End of function main
******************************************************************************/

