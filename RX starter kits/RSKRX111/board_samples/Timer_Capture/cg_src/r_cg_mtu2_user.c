/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. This 
* software is owned by Renesas Electronics Corporation and is protected under 
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING 
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT 
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR 
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE 
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software 
* and to discontinue the availability of this software.  By using this software, 
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_mtu2_user.c
* Version      : Applilet4 for RX111 V1.00.00.05 [06 Jun 2013]
* Device(s)    : R5F51115AxFM
* Tool-Chain   : IAR Systems icc78k0r
* Description  : This file implements device driver for MTU2 module.
* Creation Date: 2013-08-26
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_mtu2.h"
/* Start user code for include. Do not edit comment generated here */
#include "switch.h"
#include "lcd.h"

#define TIMER_CLOCK_FREQ_HZ (16000000 / 1024)

/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */

/* The functions in this file are generated using "Application Leading Tool (Applilet)" for e2 studio.
 * Warnings exist for interrupt handlers and callback functions. */


/* initialise timer capture */
void Init_TimerCapture(void);

/* 16 bit integer to character string function prototype declaration */
static void uint16_to_bcd_string(uint8_t * const output_string, const uint8_t pos,
                                 uint16_t number);

/* Start timer switch press callback function prototype */
static void start_timer_capture(void);

/* Stop timer switch release callback function prototype */
static void stop_timer_capture(void);

/* Variable for storing the captured interval value */
uint16_t g_input_capture_value = 0;

/* flag indicating timer overflow */
volatile uint8_t timerOverFlowFlag = 0U;

/* flag indicating counter has stopped */
volatile uint8_t stopTimerFlag = 0U;

/* flag indicating counter has started */
volatile uint8_t startTimerFlag = 0U;

volatile uint8_t timerOverflowCounter = 0U;

/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_mtu2_u0_tciv3_interrupt
* Description  : This function MTU2 Unit0 TCIV3 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = VECT_MTU3_TCIV3
#if FAST_INTERRUPT_VECTOR == VECT_MTU3_TCIV3
__fast_interrupt static void r_mtu2_u0_tciv3_interrupt(void)
#else
__interrupt static void r_mtu2_u0_tciv3_interrupt(void)
#endif
{
    /* Start user code. Do not edit comment generated here */

    /* Clear timer overflow flag */
    //timerOverFlowFlag = 1U;
    timerOverflowCounter++;

    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */

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
        R_MTU2_U0_C3_Start();

        /* Clear timer overflow flag */
        timerOverFlowFlag = 0U;

        timerOverflowCounter = 0;

        startTimerFlag = 1U;
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
        R_MTU2_U0_C3_Stop();

        /* Read the captured timer count */
        g_input_capture_value = MTU3.TCNT;

        stopTimerFlag = 1U;
    }
}
/*******************************************************************************
* End of function stop_timer_capture
*******************************************************************************/


/*******************************************************************************
* Function Name : updateLCDWithCounterValue
* Description   : Updates the debug LCD with timer value converted to milliseconds.
*                 (Values above 65 seconds will simply display " >65 sec")
* Argument      : none
* Return value  : none
*******************************************************************************/
void updateLCDWithCounterValue (void)
{

    /* Declare temporary local lcd buffer */
    uint8_t lcd_buffer[] = "      ms       ";

    /* check to see if the number of counter overflows would overflow the
       timer variable */
    if (timerOverflowCounter >
           (uint16_t)((65535 * TIMER_CLOCK_FREQ_HZ) / (65535 * 1000)))
    {
        /* Display that the count value is greater than 65 seconds on the
           debug LCD screen */
        Display_LCD(2, (uint8_t *)" >65 sec       ");
    }
    else
    {
        /* Convert the value to time in ms */
        /* Cast required to prevent "Precision lost" compiler warning */
        uint16_t time_ms = (uint16_t)(timerOverflowCounter * ((65535 * 1000) /
                                                           TIMER_CLOCK_FREQ_HZ));

        /* calculate time on counter in ms*/
        uint16_t time_ctr_ms = (uint16_t)((1000 * g_input_capture_value) /
                                    TIMER_CLOCK_FREQ_HZ);

        /* add switch debounce time
           based on 16MHz clock and CMT0 divide by 512 */
        time_ctr_ms += (((uint16_t)512 * DEBOUNCE_SHORT) / 16000);

        /* make sure that combine counter and overflow times still won't overflow
           the timer variable */
        if ((65535 - time_ms) >= time_ctr_ms)
        {
            /* Combine the 2 times to get an overall figure */
            time_ms += time_ctr_ms;

            /* Convert the timer value into a BCD character string, and insert
               into the temporary string lcd_buffer */
            uint16_to_bcd_string(lcd_buffer, 1u, time_ms);

            /* Display the contents of lcd_buffer onto the debug LCD */
            Display_LCD(2, lcd_buffer);
        }
        else
        {
            /* Display that the count value is greater than 5 seconds on the
               debug LCD screen */
            Display_LCD(2, (uint8_t *)" >65 sec       ");
        }
    }
    /* Clear switch release bits in switch flag */
    g_switch_flag &= ~SWITCHPRESS_ALL;
}
/*******************************************************************************
* End of function updateLCDWithCounterValue
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
static void uint16_to_bcd_string (uint8_t * const output_string, const uint8_t pos,
                                    uint16_t number)
{
    /* Local digit position variable */
    uint8_t digit = 5u;

    /* Digit is a leading zero indicator variable */
    bool    leading_zero = true;

    /* Local string buffer variable */
    uint8_t buffer_string[6] = "00000";

    /* Decrement digit each iteration, until digit is zero */
    while (digit--)
    {
        /* Write BCD value to string element */
        buffer_string[digit] = (uint8_t) (0x30 + (number % 10));

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

/* End user code. Do not edit comment generated here */
