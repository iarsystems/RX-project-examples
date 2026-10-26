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

/* The functions in this file are generated using "Application Leading Tool (Applilet)" for e2 studio.
 * Warnings exist for interrupt handlers. */

#include "switch.h"
#include "lcd.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */


/* The functions in this file are generated using "Application Leading Tool (Applilet)" for e2 studio.
 * Warnings exist for interrupt handlers. */

/* Define for number of 16MHz PCLK timer ticks in a ms */
#define PCLKB_TICKS_PER_MS 4000

static void uint16_to_bcd_string(uint8_t * const output_string, const uint8_t pos,
                                 uint16_t number);

/* Variable for storing the TGRB event count */
static volatile uint16_t tgrb_current_value;

/* Variable for halting updates and displaying the duty cycle */
static volatile uint8_t vary_duty_cycle = 1U;
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_mtu2_u0_tgia3_interrupt
* Description  : This function MTU2 Unit0 TGIA3 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = VECT_MTU3_TGIA3
#if FAST_INTERRUPT_VECTOR == VECT_MTU3_TGIA3
__fast_interrupt static void r_mtu2_u0_tgia3_interrupt(void)
#else
__interrupt static void r_mtu2_u0_tgia3_interrupt(void)
#endif
{
    /* Start user code. Do not edit comment generated here */

    /* Stop the TCNT counter */
    MTU.TSTR.BIT.CST3 = 0x0;

    /* Update the duty cycle if SW1 hasn't been pressed. */
    if (1 == vary_duty_cycle)
    {
        /* Decrease the duty cycle by 0.025 % */
        /* Cast required to remove compiler warning  -
           Implicit conversion: int to unsigned short. */
        MTU3.TGRB += (uint16_t)(PCLKB_TICKS_PER_MS / 4000);

        /* Read duty cycle */
        tgrb_current_value = MTU3.TGRB;

        /* Check if the duty cycle equals 10% */
        if (tgrb_current_value >
                (PCLKB_TICKS_PER_MS - (PCLKB_TICKS_PER_MS / 10)))
        {
            /* Reset the duty cycle to 90% */
            MTU3.TGRB = PCLKB_TICKS_PER_MS / 10;
        }
    }

    /* Start the TCNT counter */
    MTU.TSTR.BIT.CST3 = 0x1;

    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */

/*******************************************************************************
* Function Name : cb_switch_press
* Description   : Switch Press function. This function checks which switch was
*                 pressed and controls actions accordingly.
*
*                 If switch SW1 is pressed the PWM duty cycle will be fixed at
*                 the current value and will be displayed the third line of the
*                 debug LCD.
*                 If either of switches SW2 or SW3 are pressed then the duty
*                 cycle will be allowed to vary.
*
*                 Note that this must not be called from an interrupt as
*                 Display_LCD uses the SCI5 interrupt which would then block.
* Argument      : none
* Return value  : none
*******************************************************************************/
void cb_switch_press (void)
{
    /* Variable for starting clearance of LCD's line
    two only after the first SW1 press */
    static uint8_t  clear_lcd = 0U;

    /* Local LCD buffer string */
    uint8_t         lcd_buffer[] = "Duty Cycle:   %";
    uint32_t        calc_duty_percent = 0;

    /* Set the vary_duty_cycle to false and display the duty
       cycle as a percentage if SW1 has been pressed */
    if (g_switch_flag & SWITCHHOLD_1)
    {
        /* Set flag to indicate stoppage of duty cycle updates. */
        vary_duty_cycle  = 0U;

        /* Set to true to allow clearance of LCD's line 2 */
        clear_lcd = 1U;

        /* Calculate the percentage */
        calc_duty_percent =
            (uint32_t)((tgrb_current_value * 100) / PCLKB_TICKS_PER_MS);

        /* Convert the calculated percentage into a BCD character string */
        uint16_to_bcd_string(lcd_buffer, 9u, (uint16_t)calc_duty_percent);

        /* Display contents of lcd_buffer onto the debug LCD */
        Display_LCD(3, lcd_buffer);
    }

    /* Either SW2 or SW3 has been pressed - is LCD clearance allowed? */
    else if (1 == clear_lcd)
    {
        /* Allow duty cycle to be updated */
        vary_duty_cycle  = 1U;

        /* Clear the third line of the debug LCD */
        Display_LCD(3, (uint8_t *)"               ");
    }
    else
    {
        /* Do nothing */
    }

    /* Clear the switch press flags */
    g_switch_flag &= ~SWITCHPRESS_ALL;
}
/*******************************************************************************
* End of function cb_switch_press
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
static void uint16_to_bcd_string (uint8_t * const output_string,
                                  const uint8_t pos,
                                  uint16_t number)
{
    /* Local digit position variable */
    uint8_t digit = 5u;

    /* Digit is a leading zero indicator variable */
    uint8_t leading_zero = 1u;

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
            leading_zero = 0u;
        }

        /* Increment the loop counter */
        digit++;
    }
}
/*******************************************************************************
* End of function uint16_to_bcd_string
*******************************************************************************/

/* End user code. Do not edit comment generated here */
