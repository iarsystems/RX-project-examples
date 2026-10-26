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
* File Name     : main_wdt.c
* Version       : 1.00
* Device        : R5F5210B
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX210B
* Description   : This sample demonstrates usage of the watchdog timer. It
*                 configures the compare match timer interrupt to refresh the
*                 watchdog timer to prevent it from overflowing. The period of
*                 the compare match timer is set using the potentiometer RV1
*                 available on the RSK. At the fully clockwise position of RV1
*                 the compare match timer period exceeds the watchdog timer
*                 period and triggers the watchdog overflow interrupt.
*
* Operation     : 1. Build and download the sample code to the RSK.
*
*                 2. Ensure the potentiometer, RV1, is turned to the fully
*                    anti-clockwise position before starting the sample. To
*                    start the sample, click the 'Reset Go' icon on the
*                    'Debug Run' toolbar.
*
*                 3. The debug LCD will display the sample name, and the user
*                    LEDs will begin to flash.
*
*                 4. Slowly turn the potentiometer RV1 clockwise, and observe
*                    the gradual decrease of the LED flash rate. Each time the
*                    LEDs toggle output state, the watchdog timer is reset,
*                    preventing it from triggering an overflow interrupt.
*
*                 5. Continue to turn the potentiometer clockwise. The LED
*                    flashing will slow down to a point where the watchdog
*                    timer is allowed to overflow.
*
*                 6. When the timer has overflowed, the LEDs will stay
*                    constantly lit, and the program will wait in an infinite
*                    while loop. The debug LCD will display "Watchdog Overflow".
*
*                 7. To repeat the sample, ensure RV1 is fully anti-clockwise
*                    and click the 'Stop' icon on the 'Debug Run' toolbar,
*                    followed by the 'Reset Go' icon.
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
/* LCD controlling function prototypes & macro defines */
#include "lcd.h"
/* Watchdog timer function prototypes */
#include "wdt.h"
/* MCU and RSK hardware initialization */
#include "hwsetup.h"

/*******************************************************************************
* Outline      : main
* Description  : Main program function. This function first initialises the
*                debug LCD, and displays the name of the sample. The function
*                then calls the Init_WDT function, which configures the watchdog
*                timer, and the LED flash reset interrupts. After the
*                initialisation function has been called, main waits in an
*                infinite while loop.
* Argument     : none
* Return value : none
*******************************************************************************/
void main(void)
{
    /* Configure the MCU and RSK hardware */
    HardwareSetup();

    /* Enable interrupts */
    __enable_interrupt();

    /* Initialise the debug LCD display */
    Init_LCD();

    /* Display the application name on the debug LCD */
    Display_LCD(LCD_LINE1, "  WDT   ");

    /* Configure the watchdog timer and timer reset interrupts */
    Init_WDT();

    /* Infinite while loop */
    while (1);
}
/******************************************************************************
* End of function main
******************************************************************************/
