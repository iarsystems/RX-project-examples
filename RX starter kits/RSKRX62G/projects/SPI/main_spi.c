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
* File Name     : main_spi.c
* Version       : 1.00
* Device        : R5F562GAADFP
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX62G
* Description   : This sample demonstrates usage of the serial peripheral
*                 interface through loopback mode whereby the transmittter and
*                 receiver pins internally connected. The transmitted data
*                 becomes the received data in this mode.
*
* Operation     : 1. Build and download the sample code to the RSK.
*
*                 2. Adjust the potentiometer.
*
*                 3. Press switch SW3 to display the 10-bit A/D value  to
*                    corresponding the potentiometer position onto the debug
*                    LCD. A message will be displayed on the debug LCD if the
*                    transfer fails.
*                    The data can be watched in the variable
*                    master_rx_data. Right click the variable, and select
*                    'Instant Watch' and click Add on the following
*                    dialog.  Right click in the watch window and select
*                    Enable Auto-Update.
*
*                 4. Go to Step 2 to repeat the test.
******************************************************************************/
/******************************************************************************
* History       : 06.09.2012 Ver. 1.00 First Release
******************************************************************************/

/******************************************************************************
* Project Includes
******************************************************************************/
/* Defines debug LCD functions used in this file */
#include "lcd.h"
/* defines the serial peripheral interface's functions used in this file */
#include "spi.h"

#include "hwsetup.h"
#include "intrinsics.h"

/******************************************************************************
* Outline      : main
* Description  : Main program function. This function first initialises the
*                debug LCD, and displays the name of the sample. The function
*                then calls the Init_SPI function, which configures the
*                Serial Peripheral Interface detect module. The program then
*                waits in an infinite loop.
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
    DisplayLCD(LCD_LINE1, "  SPI   ");
    DisplayLCD(LCD_LINE2, "Push SW3");

    /* Configure the watchdog timer and timer reset interrupts */
    Init_SPI();

    /* Infinite while loop */
    while (1);
}
/******************************************************************************
* End of function main
******************************************************************************/
