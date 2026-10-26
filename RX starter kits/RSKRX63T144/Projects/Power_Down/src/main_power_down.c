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
* File Name     : main_power_down.c
* Version       : 1.00
* Device        : R5F563TE
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX63T144
* Description   : This sample demonstrates usage of power down modes, using
*                 RSK switches to send the system into software standby mode.
* Operation     : 1. Connect to target
*
*                 2. Compile and download the sample code to the target.
*                    Click 'Resume' icon on the 'Debug' toolbar to
*                    start the software.
*
*                 3. The LCD will display "Pwr Down" and the LEDs will begin to
*                    flash. Press SW1 to enter Deep Software Standby mode and
*                    the LCD 2nd line will show "DPSWSTBY".
*
*                 4. It is also possible to enter Software Standby mode by
*                    pressing SW2 - in this case the LCD 2nd line will show
*                    "SWSTBY".
*
*                 5. Pressing SW3 will wake the MCU.  Once awake the LCD 2nd
*                    line will show "Awake" if  woken from Software Standby
*                    mode, or "DeepWake" if woken from Deep Software Standby
*                    mode.
*
* Note          : By removing R59, and fitting an ammeter across J7; it is
*                 possible to observe the current consumption as the MCU
*                 switches between normal and standby mode.
*******************************************************************************/
/*******************************************************************************
* History       : 05.12.2013  Ver. 1.00 First Release
*******************************************************************************/

/******************************************************************************
* Project Includes
******************************************************************************/
/* Defines debug LCD functions used in this file */
#include "lcd.h"

/* Defines switch functions used in this file */
#include "switch.h"

/* Defines power down functions used in this file */
#include "power_down.h"

/* Defines the main function in this file */
#include "main_power_down.h"

/* Intrinsics */
#include <intrinsics.h>

/*******************************************************************************
* Function Name: main
* Description  : The main function initialises the debug LCD and displays the
*                sample name then calls the power down initialisation
*                function and sets up a callback function for switch press.
*                The function then enters an infinite while loop,
*                which polls both the g_enter_standby flag and the
*                g_enter_deep_standby flag. If the g_enter_standby flag is true
*                the EnterSwStandby_PowerDown() function is entered. If the
*                g_enter_deep_standby flag is true the
*                EnterDeepSwStandby_PowerDown() function is entered. If neither
*                flags are true the LEDs are flashed in the ToggleLEDs function.
* Argument     : none
* Return value : none
*******************************************************************************/
void main (void)
{
    /* Enable interrupts */
    __enable_interrupt();

    /* Initialise LCD */
    Init_LCD();

    /* Display sample name on debug LCD */
    Display_LCD(LCD_LINE1, "Pwr Down");

    /* Display the reset source - we may have reset due to wake from Deep
       Software Standby mode */
    DisplayResetSource();

    /* Initialise the power down registers */
    Init_PowerDown();

    /* Configure a switch press callback function */
    SetSwitchPressCallback(CB_Switch_Press);

    /* Infinite while loop */
    while (1)
    {
        /* Check if the g_enter_standby flag is true */
        if (g_enter_standby)
        {
            /* Enter standby mode */
            EnterSwStandby_PowerDown();

            /* Reset the flag */
            g_enter_standby = false;

        }
        else if (g_enter_deep_standby)
        {
            /* Enter standby mode */
            EnterDeepSwStandby_PowerDown();

            /* We will never get here, as a wake from Deep Software Standby
                causes a reset - see function DisplayResetSource() above, which
                displays the DSWSTBY reset source on the LCD if applicable */
        }
        else
        {
            /* Toggle the LEDs */
            ToggleLEDs();
        }
    }
}
/*******************************************************************************
* End of function main
******************************************************************************/

