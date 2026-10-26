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
* File Name    : r_cg_cmt_user.c
* Version      : Applilet4 for RX111 V1.00.00.05 [06 Jun 2013]
* Device(s)    : R5F51115AxFM
* Tool-Chain   : IAR Systems icc78k0r
* Description  : This file implements device driver for CMT module.
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
#include "r_cg_cmt.h"
/* Start user code for include. Do not edit comment generated here */
#include "r_cg_mtu2.h"
#include "lcd.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */

/* The functions in this file are generated using "Application Leading Tool (Applilet)" for e2 studio.
 * Warnings exist for interrupt handlers. */

static void uint16_to_bcd_string (uint8_t * const output_string, const uint8_t pos,
                                    uint16_t number);

/* Declare a variable used to store the switch press count */
uint16_t g_event_count;
volatile uint8_t update_LCD_Flag = 0U;
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_cmt_cmi1_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = VECT_CMT1_CMI1
#if FAST_INTERRUPT_VECTOR == VECT_CMT1_CMI1
__fast_interrupt static void r_cmt_cmi1_interrupt(void)
#else
__interrupt static void r_cmt_cmi1_interrupt(void)
#endif
{
    /* Start user code. Do not edit comment generated here */
    update_LCD_Flag = 1U;
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/*******************************************************************************
* Function Name: Init_TimerEvent
* Description  : The function disables switch 2 interrupts then initialises
*                timer 3 of the MTU3 unit, to be clocked from the falling edge
*                of MTCLKA. It also configures the CMT2 timer used to create
*                a 300ms periodic interrupt, whose handler is used to update the
*                LCD with the switch press count.
* Argument     : none
* Return value : none
*******************************************************************************/
void Init_TimerEvent (void)
{
    /* Disable SW2 interrupt requests */
    IEN(ICU, IRQ1) = 0x0;

    /* reset the counter */
    MTU3.TCNT = 0;

    /* Start the timer */
    R_MTU2_U0_C3_Start();

    /*start the CMT1 timer to schedule the LCD updates */
    R_CMT1_Start();
}
/*******************************************************************************
* End of function Init_TimerEvent
*******************************************************************************/

/*******************************************************************************
* Function Name: displayEventCount
* Description  : Display the number of events counted on the LCD.
* Argument     : none
* Return value : none
*******************************************************************************/
void displayEventCount (void)
{
    /* Declare temporary character string */
    uint8_t lcd_buffer[] = "Count:  0";

    /* Fetch switch count value */
    g_event_count = MTU3.TCNT;


    /* Check if SW2 was pressed */
    if (g_event_count)
    {
        /* Convert switch press count into a character string, and store in the
           local string lcd_buffer */
        uint16_to_bcd_string(lcd_buffer, 4, g_event_count);

        /* Display the contents of the local string lcd_buffer */
        Display_LCD(3, lcd_buffer);
    }
}
/*******************************************************************************
* End of function displayEventCount
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
    uint8_t    digit = 5u;

    /* Digit is a leading zero indicator variable */
    uint8_t    leading_zero = 1U;

    /* Local string buffer variable */
    uint8_t    buffer_string[6] = "00000";

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
            leading_zero = 0U;
        }

        /* Increment the loop counter */
        digit++;
    }
}
/*******************************************************************************
* End of function uint16_to_bcd_string
*******************************************************************************/
/* End user code. Do not edit comment generated here */
