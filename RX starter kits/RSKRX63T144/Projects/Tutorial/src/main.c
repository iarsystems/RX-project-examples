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
* File Name     : main.c
* Version       : 1.00
* Device        : R5F563TE
* Tool-Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX63T144
* Description   : This is the main tutorial code. This code will call three
*                 functions to demonstrate Port pin control (FlashLEDs),
*                 Interrupt usage (TimerADC) and C variable initialization
*                 (Statics_Test). Code is also included to drive the optional
*                 LCD module
* Operation     : 1. The tutorial code initialises the LCD module, and displays
*                    'Renesas' on the first line of the LCD, and the name of the
*                    MCU on the second line.
*
*                 2. The tutorial code calls the Flash_LED function which
*                    toggles the user LEDs repeatedly and waits in a loop until
*                    either a switch is pressed or the LEDs flash 200 times.
*
*                 3. The tutorial then calls the Timer_ADC function which
*                    configures the ADC unit, and a timer unit to generate
*                    periodic interrupts. The timer's period is varied by the
*                    ADC conversion results. The ADC unit is configured to
*                    generate an interrupt after each AD conversion completes
*                    and store the result.
*
*                 4. The ADC interrupt fetches the ADC result, which is used to
*                    set a new timer period. The timer interrupt handler toggles
*                    the user LEDs.
*
*                 5. After calling Timer_ADC and setting up the timer & ADC
*                    interrupts, the tutorial calls the static_test function
*                    which runs in parallel to the callback functions in step 4.
*                    This function displays the string 'STATIC' which is fetched
*                    from a static variable and is replaced, letter by letter,
*                    by the string 'TESTTEST'. The LCD then reverts to the
*                    original display on completion.
*
*                 6. After the static_test function completes, the MCU waits in
*                    an infinite while loop; and is periodically interrupted
*                    with the callback function in step 4.
*******************************************************************************/
/*******************************************************************************
* History       : 05.12.2013  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
System Includes
*******************************************************************************/
/* Following header file provides C99 type definitions. */
#include <stdint.h>

/* Intrinsics */
#include <intrinsics.h>

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/
/* Defines macros relating to the R5F563TE user LEDs and switches */
#include "rskrx63tdef.h"

/* LCD controlling function prototypes & macro definitions */
#include "lcd.h"

/* Timer & ADC function definitions */
#include "timeradc.h"

/* LED flash function definitions */
#include "flashled.h"

/* main definitions */
#include "main.h"

/******************************************************************************
* Macro Definitions
******************************************************************************/
/* Device nickname definition */
#define NICKNAME "RX63T144"

/******************************************************************************
* Function Prototypes
******************************************************************************/
/* Static Test function declaration */
static void static_test(void);

/******************************************************************************
* Global Variables
******************************************************************************/
/* Global initialized variable*/
char cStr[] = " STATIC \0";

/* Static variable declaration, used in static variable test */
const char cReplace[] = "TESTTEST\0";


/******************************************************************************
* Function Name : main
* Description   : The main program function. Displays the Renesas splash screen
*                 onto the LCD display, then calls the 'flashLED' and 'TimerADC'
*                 functions. The function then calls the statics test routine,
*                 before waiting in an infinite while loop.
* Argument      : none
* Return value  : none
******************************************************************************/
void main (void)
{
    /* Enable interrupts */
    __enable_interrupt();

    /* Initialise the debug LCD */
    Init_LCD();

    /* Displays the Renesas splash screen */
    Display_LCD(LCD_LINE1, "Renesas");
    Display_LCD(LCD_LINE2, NICKNAME);

    /* Begins the initial LED flash sequence */
    Flash_LED();

    /* Begins the ADC-varying flash Sequence */
    Timer_ADC();

    /* Begins the static variable test */
    static_test();

    /* Infinite while loop */
    while (1)
    {
        /* Do nothing */
    }

}
/******************************************************************************
* End of function main
******************************************************************************/

/******************************************************************************
* Function Name : static_test
* Description   : Static variable test routine. The function replaces the
*                 contents of the string 'cStr' with that of 'ucReplace', one
*                 element at a time. Right-click the variable 'cStr', and
*                 select 'instant watch' - click add in the subsequent dialog.
*                 If you step through the function, you can watch the string
*                 elements being overwritten with the new data.
* Argument      : none
* Return value  : none
******************************************************************************/
static void static_test (void)
{
    /* Declare loop count variable */
    uint8_t uicount = 0;

    /* Write cStr variable, "STATIC" to LCD */
    Display_LCD(LCD_LINE2, cStr);

    /* Begin for loop which writes one letter of ucReplace to the LCD at a time
       The nested while loops generate the delay between each letter change */
    for (uicount = 0; uicount < 8; uicount++)
    {
        /* Start a one-shot timer to create a delay between each loop
           iteration */
        Timer_Delay(40, MILLISECS);

        /* Replace letter number 'uiCount' of 'ucStr' from 'ucReplace' */
        cStr[uicount] = cReplace[uicount];
        Display_LCD(LCD_LINE2, cStr);
    }

    /* Clear LCD Display */
    cStr[uicount] = '\0';

    /* Add a delay before overwritting the string */
    Timer_Delay(40, MILLISECS);

    /* Write MCU nickname to LCD again */
    Display_LCD(LCD_LINE2, NICKNAME);
}
/******************************************************************************
* End of function static_test
******************************************************************************/

