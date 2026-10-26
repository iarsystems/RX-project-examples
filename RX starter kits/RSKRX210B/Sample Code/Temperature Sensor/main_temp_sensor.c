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
* File Name     : main_temp_sensor.c
* Version       : 1.00
* Device        : R5F5210B
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX210B
* Description   : Demonstration of the temperature sensor module. The
*                 application measures the ambient temperature around the MCU &
*                 displays on the LCD.
*
* Operation     : 1. By default the MCU is set be supplied power with 5.0V from
*                    the power (PWR) connector or the E1, using J17 (pins 1-2
*                    shorted). Use of the PWR connector is recommended here.
*                    When supplying 3.3V via the voltage regulator (U11) to the
*                    MCU, J17 (pins 2-3 shorted), comment out the following macro
*                    from the temp_sensor.h file:
*                            #define SUPPLY_5V0
*
*                 2. Compile and download the sample code to the target. Click
*                    the 'Reset Go' icon on the 'Debug Run' toolbar to start the
*                    software.
*
*                 3. Observe the LCD display - the MCU will repeatedly measure
*                    the temperature of the chip & will display the readings
*                    on the debug LCD.
*
* Note :          1. Do not expose the RSK to temperatures over +70 or below
*                    +5 degrees C.
*                 2. Accuracy of the reading may be improved by calibration of
*                    the sensor. See section 35.3.1 of the Hardware Manual for
*                    sensor characteristic details, and temp_sensor.h for the
*                    calibration constants.
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
/* Provides access to temperature sensor function prototypes */
#include "temp_sensor.h"
/* MCU and RSK hardware initialization */
#include "hwsetup.h"

/*******************************************************************************
* Outline       : main
* Description   : The main program function. This function calls the temperature
*                 sensor initialisation function, then waits in an infinite
*                 while loop whilst the temperature measurement re-starts every
*                 500 ms.
* Argument      : none
* Return value  : none
*******************************************************************************/
void main(void)
{
    /* Configure the MCU and RSK hardware */
    HardwareSetup();

    /* Enable interrupts */
    __enable_interrupt();

    /* Initialise the debug LCD */
    Init_LCD();

    /* Display application name on the debug LCD */
    Display_LCD(LCD_LINE1, "Reading ");
    Display_LCD(LCD_LINE2, "Temp... ");

    /* Initialise the temperature sensor & ADC */
    Init_TempSensor();

    /* Infinite while loop */
    while (1);
}
/*******************************************************************************
* End of function main
*******************************************************************************/
