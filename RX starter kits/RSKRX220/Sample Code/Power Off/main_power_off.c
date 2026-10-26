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
* File Name     : main_power_off.c
* Version       : 1.00
* Device        : R5F52206
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX220
* Description   : Demonstrates entering the MCU into low power sleep/standby
*                 mode.
* Operation     : 1. Compile and download the sample code to the target.
*                    Click 'Reset Go' icon on the 'Debug Run' toolbar to
*                    start the software.
*
*                 2. Observe the flashing user LEDs. This indicates that the
*                    MCU is in normal (active) mode, this is also displayed on
*                    debug LCD.
*
*                 3. Press SW1 to enter the MCU into standby mode. The user LEDs
*                    will all turn off, and 'Standby' will be displayed on the
*                    debug LCD.
*
*                 4. Press any switch to wake the MCU up, which will update the
*                    debug LCD accordingly and repeat flashing the user LEDs.
* Note          : By removing R68, and fitting an ammeter across J14; it is
*                 possible to observe the current consumption as the MCU
*                 switches between normal and standby mode.
*                 To achieve the supply current characteristics in the RX220
*                 manual it is neccessary to disconnect the emulator and use an
*                 external 5V supply.
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

/* Defines power down functions used in this file */
#include "power_off.h"

/* Defines LCD functions used in this file */
#include "lcd.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/

/* Prepare to enter standby flag variable (imported) */
extern volatile bool gEnterStandby;

/******************************************************************************
* Function Name: main
* Description  : The main function intialises the debug LCD and displays the
*                sample name, then calls the power off initialisation function.
*                The function then enters an infinite while loop, which flashes
*                the LEDs and polls the gEnterStandby flag, calling the enter
*                standby function if its true.
* Arguments    : none
* Return Value : none
******************************************************************************/
void main (void)
{
    /* Enable interrupts */
    __enable_interrupt();

    /* Initialise the debug LCD */
    Init_LCD();

    /* Display current MCU mode on the debug LCD */
    Display_LCD(LCD_LINE1, "PowerOff");
    Display_LCD(LCD_LINE2, " Active ");

    /* Initialise the standby power registers */
    init_power_off();

    /* Infinite while loop */
    while (1)
    {
        /* Toggle the user LEDs */
        flash_leds_power_off();

        /* Check if the gEnterStandby flag has been set true */
        if (gEnterStandby)
        {
            /* Enter standby mode */
            enter_standby_power_off();

            /* MCU returns from standby and executes SwitchCB_PowerOff to clear
               gEnterStandby flag */
        }
    }
}
/******************************************************************************
* End of function main
******************************************************************************/

