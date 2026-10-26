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
* File Name     : main_analog_compare.c
* Version       : 1.00
* Device        : R5F5210B
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX210B
* Description   : Demonstration of the analog comparator module. The program
*                 configures the comparator A1 to compare the voltage at the pin
*                 CMPA1(port pin PE3) with the reference voltage set at the pin
*                 CVREFA (port pin PA1)
*
* Operation     :   1. Connect a wire between JA3-2 (CVREFA) and J4-20 (AD_POT)
*                      to set the reference using the pot RV1 available on the
*                      RSK. Adjust the pot to keep at the middle so as to set
*                      approx. half as a reference voltage.
*
*                   2. Connect an external regulated power supply at JA3-32
*                      (CMPA1 input). Set the initial voltage to 0.0V.
*
*                   3. Compile and download the sample code to the target. Click
*                      the 'Reset Go' icon on the 'Debug Run' toolbar to start
*                      the software.
*
*                   4. Increase the input voltage at JA3-32 slowly to cross
*                      the reference voltage set at JA3-2.
*
*                   5. The COMPA1 interrupt will occur which will turn on LED0
*                      & LED1 & turn off LED2 & LED3. The LCD will display
*                      "VI>=VREF".
*
*                   6. Now reduce the voltage at JA3-32 below the reference
*                      set at JA3-2. Another COMPA1 interrupt will occur
*                      which will turn on LED2 & LED3 & turn off LED0 & LED1.
*                      The LCD will display "VI<VREF ".
*
*                   7. Repeat the steps 4 to 6 to observe the results on the
*                      LCD.
*
*           NOTE:   1. Please ensure that the external input voltage applied to
*                      JA3-2 (CMPA1 pin) do not exceed the MCU supply voltage
*                      (which is by default kept at 3.3V) to avoid damage to
*                      the MCU pins.
*                   2. If the power supply connected at CMPA1 input is not
*                      stable and the CMPA1 voltage level matches to the voltage
*                      at CVREFA pin, the comparator state may oscillate causing
*                      the display to switch between the messages "VI<VREF"
*                      and "VI>=VREF".
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
/* Analog comparator function definitions */
#include "analog_compare.h"
/* MCU and RSK hardware initialization */
#include "hwsetup.h"

#include <intrinsics.h>

/******************************************************************************
* Outline       : main
* Description   : The main program function. This function calls the comparator
*                 initialisation function, then waits in an infinite while loop
* Argument      : none
* Return value  : none
******************************************************************************/
void main(void)
{
    /* Configure the MCU and RSK hardware */
    HardwareSetup();

    /* Enable interrupts */
    __enable_interrupt();

    /* Initialise the debug LCD */
    Init_LCD();

    /* Display application name on the debug LCD */
    Display_LCD(LCD_LINE1, "ANALOG  ");
    Display_LCD(LCD_LINE2, "COMPARE ");

    /* Initialise the comparator A */
    Init_AnalogCompare();

    /* Infinite while loop */
    while (1);
}
/******************************************************************************
* End of function main
******************************************************************************/
