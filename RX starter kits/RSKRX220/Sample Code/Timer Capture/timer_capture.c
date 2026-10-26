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
* File Name     : timer_capture.c
* Version       : 1.00
* Device        : R5F52206
* Tool-Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX220
* Description   : Defines MTU3 configuration functions
*******************************************************************************/
/*******************************************************************************
* History       : 14.12.2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* System Includes
*******************************************************************************/

/* Following header file provides standard integer type definitions. */
#include <stdint.h>

/* Following header file provides string type definitions. */
#include <string.h>

/* Following header file provides boolean type definitions */
#include <stdbool.h>

/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/

/* Defines RX220 LEDs and switches */
#include "rskrx220def.h"

/* Defines RX220 port registers */
#include <iorx220.h>

/* LCD controlling function prototypes & macro defines */
#include "lcd.h"

/* Switch handler function definitions */
#include "switch.h"

/* Declares function prototypes defined in this file */
#include "timer_capture.h"

/******************************************************************************
* Local Function Prototypes
******************************************************************************/

/* 16 bit integer to character string function prototype declaration */
static void uint16_tobcdstring (char * const output_string, const uint8_t pos,
                                                              uint16_t number);

/******************************************************************************
Global Variables
******************************************************************************/

/* Variable for storing the captured interval value */
extern uint16_t g_input_capture_value;

/* Static global MTU4 overflow counter variable */
static volatile uint16_t g_overflow_count = 0;

/*******************************************************************************
* Function Name: init_timer_capture
* Description  : Function configures MTU4 for input capture used to measure the
*                interval between the falling and rising edge detected on the
*                MTIOC4D/P31/SW1 pin. The timer clock source is configured as
*                PCLK/1024 (19.53kHz) and P31 is set for peripheral use to
*                capture  and generate an interrupt on rising and falling edges.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void init_timer_capture (void)
{
    /* Protection off */
    SYSTEM.PRCR.WORD = PROTECTOFF;

    /* Cancel the MTU4 module clock stop mode */
    MSTP_MTU4 = 0x0;

    /* Protection on */
    SYSTEM.PRCR.WORD = PROTECTON;

    /* Select timer scaling as PCLK/1024=19.53KHz */
    MTU4.TCR.BIT.TPSC = 0x5u;

    /* Timer counts on falling edge */
    MTU4.TCR.BIT.CKEG = 0x1u;

    /* Select normal timer mode */
    MTU4.TMDR.BIT.MD = 0x0;

    /* Enable noise filter */
    MTU4.NFCR.BIT.NFBEN = 0x1u;

    /* Reconfigure SW1 as MTIOC4D capture pin */
    /* Set pin 1 of port 3 for peripheral function */
    PORT3.PMR.BIT.B1 = 0x1u;

    /* Enable write to PFSWE bit */
    MPC.PWPR.BYTE = 0x00;

    /* Disable write protection to PFS registers */
    MPC.PWPR.BYTE = PFSPROTECTOFF;

    /* Disable switch IRQ on pin 1 of port 3 */
    MPC.P31PFS.BIT.ISEL = 0x0;

    /* Configure pin 1 of port 3 for MTIOC4D inputs */
    MPC.P31PFS.BIT.PSEL = 0x1u;

    /* Enable write protection to PFS registers */
    MPC.PWPR.BYTE = PFSPROTECTON;

    /* Set up the input capture pin on both edges */
    MTU4.TIORL.BIT.IOD = 0xFu;

    /* Enable up the input capture interrupt */
    MTU4.TIER.BIT.TGIED = 0x1u;

    /* Set TGID4 interrupt priority level */
    IPR(MTU4, TGID4) = TIMER_IPL;

    /* Enable TGID4 interrupts */
    IEN(MTU4, TGID4) = 0x1u;

    /* Clear TGID4 interrupt flag */
    IR(MTU4, TGID4) = 0x0;

    /* Enable the timer overflow interrupt */
    MTU4.TIER.BIT.TCIEV = 0x1u;

    /* Set TCIV4 interrupt priority level */
    IPR(MTU4, TCIV4) = TIMER_IPL;

    /* Enable TCIV4 interrupts */
    IEN(MTU4, TCIV4) = 0x1u;

    /* Clear TCIV4 interrupt flag */
    IR(MTU4, TCIV4) = 0x0;

    /* Start the timer */
    MTU.TSTR.BIT.CST4 = 0x1u;
}
/*******************************************************************************
* End of function init_timer_capture
*******************************************************************************/

/*******************************************************************************
* Function Name: Excep_MTU24_TGID4_isr
* Description  : MTU4 input capture interrupt service routine. This function
*                checks if P31 (SW1) is high or low. If it is low (switch
*                just pressed) the counter is reset, if it is high (switch
*                released), the end count value is fetched and the press
*                duration is calculated and displayed.
* Arguments    : none
* Return Value : none
*******************************************************************************/
#pragma vector=VECT_MTU4_TGID4
__interrupt  void Excep_MTU24_TGID4_isr(void)
{
    /* Check if SW1 is pressed */
    if (0x0 == PORT3.PIDR.BIT.B1)
    {
        /* Clear the counter */
        MTU4.TCNT = 0x0;

        /* Reset the overflow count to zero */
        g_overflow_count = 0u;
    }
    else
    {
        /* Fetch the timer value */
        const uint16_t end_count = MTU4.TGRD;

        /* Check if the switch hold time is greater than 60 seconds */
        if ((TIMEOUT_OVERFLOWS < g_overflow_count) ||
            ((TIMEOUT_OVERFLOWS == g_overflow_count) &&
            (TIMEOUT_TICKS <= end_count)))
        {
            /* Display that the count value is greater than 30 seconds on
               the debug LCD screen */
            Display_LCD(LCD_LINE2, ">60 sec ");
        }
        else
        {
            /* Calculate the switch hold time in milliseconds */
            g_input_capture_value =
                              (uint16_t)((g_overflow_count * OVERFLOW_TIME) +
                                                (end_count / TIMER_FREQ_KHZ));

            /* Declare temporary local lcd buffer */
            char lcd_buffer[] = "     0ms";

            /* Convert the timer value into a BCD character string, and insert
               into the temporary string lcd_buffer */
            uint16_tobcdstring(lcd_buffer, 1u, g_input_capture_value);

            /* Display the contents of lcd_buffer onto the debug LCD */
            Display_LCD(LCD_LINE2, lcd_buffer);
        }
    }
}
/*******************************************************************************
* End of function Excep_MTU24_TGID4_isr
*******************************************************************************/

/*******************************************************************************
* Function Name: Excep_MTU24_TGIV4_isr
* Description  : MTU4 timer overflow interrupt service routine. The function
*                incrementes the g_overflow_count variable every time MTU4
*                overflows.
* Argument     : none
* Return value : none
*******************************************************************************/
#pragma vector=VECT_MTU4_TCIV4
__interrupt  void Excep_MTU24_TGIV4_isr(void)
{
    /* Increment the g_overflow_count variable */
    g_overflow_count++;
}
/*******************************************************************************
* End of function Excep_MTU24_TGIV4_isr
*******************************************************************************/

/*******************************************************************************
* Function Name: uint16_tobcdstring
* Description  : This function converts an 16bit integer into a BCD string,
*                which is inserted into the character array passed as an
*                argument.
* Arguments    : (char *)output_string        - Pointer to char array that will
*                                               hold character string.
*                (uint8_t)pos                 - uint8_t number, element number
*                                               to begin inserting the character
*                                               string from (offset).
*                (const uint16_t)input_number - 16 bit integer to convert into
*                                               a string.
* Return Value : none
* Note         : No input validation is used, so output data can overflow the
*                array passed.
*******************************************************************************/
static void uint16_tobcdstring (char * const output_string, const uint8_t pos,
                                                                uint16_t number)
{
    /* Local digit position variable */
    uint8_t digit = 5u;

    /* Digit is a leading zero indicator variable */
    bool    leading_zero = true;

    /* Local string buffer variable */
    char buffer_string[6] = "00000";

    /* Decrement digit each iteration, until digit is zero */
    while (digit--)
    {
        /* Write BCD value to string element */
        buffer_string[digit] = (char) (0x30u + (number % 10));

        /* Move to the next decimal digit */
        number /= 10u;
    }

    /* Set digit position to zero */
    digit = 0u;

    /* Loop through each digit, and replace leading zeros with a space
       character */
    while (5u > digit)
    {
        /* Check if digit is not a zero character, or if leading_zero indicator
           is false */
        if ((0x30 != buffer_string[digit]) || (!leading_zero))
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
* End of function uint16_tobcdstring
*******************************************************************************/

