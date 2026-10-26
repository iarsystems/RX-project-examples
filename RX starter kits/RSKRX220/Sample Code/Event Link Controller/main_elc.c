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
* File Name     : main_elc.c
* Version       : 1.00
* Device        : R5F52206
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX220
* Description   : Demonstration of the event link control module. The program
*                 configures the ADC & ELC modules. When the ADC conversion
*                 completes the ELC unit triggers the event of toggling the
*                 port pin PB0.
* Operation     :   1. Connect a link between JA3-9 (PB0) and J2-7 (P14, LED0).
*
*                   2. Compile and download the sample code to the target. Click
*                      'Reset Go' on the 'Debug Run' toolbar to start the
*                      software.
*
*                   3. Press SW1 to start the ADC conversion of the value set
*                      at AN000 pin by the pot RV1.
*
*                   4. Once the ADC conversion is finished the ELC will trigger
*                      the event of toggling the port pin PB0.
*
*                   5. The user can observe that every time the SW1 is pressed
*                      the LED0 is toggled.
*
*                   6. The debug LCD will display the ADC result.
*******************************************************************************/
/*******************************************************************************
* History       : 14.12.2012  Ver. 1.00 First Release
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

/* ELC configuration function definition */
#include "elc.h"

/* ADC configuration function definition */
#include "adc.h"

/******************************************************************************
* Function Name: main
* Description  : The main program function. This function calls the ELC
*                initialisation function, then waits in an infinite while loop
* Arguments    : none
* Return Value : none
******************************************************************************/
void main (void)
{
    /* Enable interrupts */
    __enable_interrupt();

    /* Initialise the debug LCD */
    Init_LCD();

    /* Display application name on the debug LCD */
    Display_LCD(LCD_LINE1, "ELC     ");
    Display_LCD(LCD_LINE2, "PUSH SW1");

    /* Initialise the ADC for single-scan conversion */
    init_adc();

    /* Initialise the ELC module */
    init_elc();

    /* Infinite while loop */
    while (1)
    {
        /* Sample operation is performed by interrupt routine */
    }
}
/******************************************************************************
* End of function main
******************************************************************************/

