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
* File Name     : timer_capture.c
* Version       : 1.00
* Device        : R5F563TE
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX63T144
* Description   : Defines MTU3 configuration functions and switch callback
*                 functions used to perform timer capture.
*******************************************************************************/
/*******************************************************************************
* History       : 05.12.2013 Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* System Includes
*******************************************************************************/
/* Following header file provides standard integer type definitions. */
#include <stdint.h>

/* Following header file provides string type definitions. */
#include <string.h>

/* Following header file provides Boolen type definitions */
#include <stdbool.h>

/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/
/* Defines R5F563TE port registers */
#include <iorx63t.h>

/* LCD controlling function prototypes & macro defines */
#include "lcd.h"

/* Switch handler function definitions */
#include "switch.h"

/* Declares function prototypes defined in this file */
#include "timer_capture.h"

/*******************************************************************************
* Macro Definitions
*******************************************************************************/
/* The MTU clock frequency is set up as PCLKA/1024, where PCLKA is set to 6MHz*/
#define TIMER_CLOCK_FREQ_HZ (5859u)

/******************************************************************************
* Local Function Prototypes
******************************************************************************/
/* 16 bit integer to character string function prototype declaration */
static void uint16_to_bcd_string(char * const output_string, const uint8_t pos,
                                    uint16_t number);

/* Start timer switch press callback function prototype */
static void start_timer_capture(void);

/* Stop timer switch release callback function prototype */
static void stop_timer_capture(void);

/******************************************************************************
Global Variables
******************************************************************************/
/* Variable for storing the captured interval value */
uint16_t g_input_capture_value = 0;

/*******************************************************************************
* Function Name : Init_TimerCapture
* Description   : Function configures MTU3 for input capture used to measure the
*                 interval. The timer clock source is configured as
*                 PCLK/1024 (5.859KHz).
* Argument      : none
* Return value  : none
*******************************************************************************/
void Init_TimerCapture (void)
{
    /* Protection off */
    SYSTEM.PRCR.WORD = 0xA503;

    /* Set PCKA clk to 6 MHz - (i92 /32) */
    SYSTEM.SCKCR.BIT.PCKA = 5;

    /* Cancel the MTU3 module clock stop mode */
    MSTP_MTU3 = 0;

    /* Protection on */
    SYSTEM.PRCR.WORD = 0xA500;

    /* Select PCLKA/1024=5.859 kHz, clock counted at
       falling edge, no clearing of TCNT counter. */
    MTU3.TCR.BIT.TPSC = 0x5;
    MTU3.TCR.BIT.CKEG = 0x1;
    MTU3.TCR.BIT.CCLR = 0x0;

    /* Select normal operation  */
    MTU3.TMDR1.BIT.MD = 0x0;

    /* Configure switch press down callback function */
    SetSwitchPressCallback(start_timer_capture);

    /* Configure switch press release callback function */
    SetSwitchReleaseCallback(stop_timer_capture);
}
/*******************************************************************************
* End of function Init_TimerCapture
*******************************************************************************/

/*******************************************************************************
* Function Name : start_timer_capture
* Description   : This is a callback function, which executes when any of the
*                 user switches is pressed down. The function checks if the
*                 switch pressed was SW1, and if so starts the MTU3 timer.
* Argument      : none
* Return value  : none
*******************************************************************************/
static void start_timer_capture (void)
{
    /* Check if SW1 is held down */
    if (g_switch_flag & SWITCHHOLD_1)
    {
        /* Clear the counter */
        MTU3.TCNT = 0x0;

        /* Start the timer */
        MTU.TSTRA.BIT.CST3 = 0x1;
    }
}
/*******************************************************************************
* End of function start_timer_capture
*******************************************************************************/

/*******************************************************************************
* Function Name : stop_timer_capture
* Description   : This is a callback function, which executes when any of the
*                 user switches are released. The function checks if the switch
*                 released was SW1, and if so stops the MTU3 timer, and updates
*                 the debug LCD with timer value converted to milliseconds.
*                 (Values above 10 seconds will simply display " >10 sec")
* Argument      : none
* Return value  : none
*******************************************************************************/
static void stop_timer_capture (void)
{
    /* Check if SW1 was released */
    if (g_switch_flag & SWITCHPRESS_1)
    {
        /* Stop the counter */
        MTU.TSTRA.BIT.CST3 = 0x0;

        /* Read the captured timer count */
        g_input_capture_value = MTU3.TCNT;

        /* Check if the count value is greater than 10,000 ms */
        if ((g_input_capture_value > (10u * TIMER_CLOCK_FREQ_HZ)) ||
                    (0x1 == MTU3.TSR.BIT.TCFV))
        {
            /* Display that the count value is greater than 5 seconds on the
               debug LCD screen */
            Display_LCD(LCD_LINE2, " >10 sec ");

            /* Clear the TCNT overflow flag */
            MTU3.TSR.BIT.TCFV = 0;
        }
        /* Count value is less than 10,000 ms*/
        else
        {
            /* Declare temporary local lcd buffer */
            char lcd_buffer[] = "      ms";

            /* Convert the value to time in ms */
            /* Cast required to prevent "Precision lost" compiler warning */
            const uint16_t time_ms = (uint16_t)((1000 * g_input_capture_value) /
                                        TIMER_CLOCK_FREQ_HZ);

            /* Convert the timer value into a BCD character string, and insert
               into the temporary string lcd_buffer */
            uint16_to_bcd_string(lcd_buffer, 1u, time_ms);

            /* Display the contents of lcd_buffer onto the debug LCD */
            Display_LCD(LCD_LINE2, lcd_buffer);
        }
    }

    /* Clear switch release bits in switch flag */
    g_switch_flag &= ~SWITCHPRESS_ALL;
}
/*******************************************************************************
* End of function stop_timer_capture
*******************************************************************************/

/*******************************************************************************
* Function Name: uint16_to_bcd_string
* Description  : This function converts an 16bit integer into a BCD string,
*                which is inserted into the character array passed as an
*                argument.
* Argument     : char * string      - Pointer to array to store BCD string
*                uint8_t pos        - Array element to start BCD string from.
*                uint16_t number    - 16bit integer to convert.
* Return value : none
* Note         : No input validation is used, so output data can overflow the
*                array passed.
*******************************************************************************/
static void uint16_to_bcd_string (char * const output_string, const uint8_t pos,
                                    uint16_t number)
{
    /* Local digit position variable */
    uint8_t digit = 5u;

    /* Digit is a leading zero indicator variable */
    bool    leading_zero = true;

    /* Local string buffer variable */
    char    buffer_string[6] = "00000";

    /* Decrement digit each iteration, until digit is zero */
    while (digit--)
    {
        /* Write BCD value to string element */
        buffer_string[digit] = (char) (0x30 + (number % 10));

        /* Move to the next decimal digit */
        number /= 10u;
    }

    /* Set digit position to zero */
    digit = 0u;

    /* Loop through each digit, and replace leading zeros with a space
       character */
    while (digit < 5u)
    {
        /* Check if digit is not a zero character, or if leading_zero indicator
           is false */
        if ((buffer_string[digit] != 0x30) || (!leading_zero))
        {
            /* Copy digit from buffer string to output string */
            output_string[digit + pos] = buffer_string[digit];

            /* Set the leading zero indicator variable to false */
            leading_zero = false;
        }

        /* Increment the loop counter */
        digit++;
    }
}
/*******************************************************************************
* End of function uint16_to_bcd_string
*******************************************************************************/


