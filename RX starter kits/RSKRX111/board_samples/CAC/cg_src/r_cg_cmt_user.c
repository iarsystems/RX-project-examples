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
* Creation Date: 2013-08-16
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
#include <string.h>
#include "r_cg_cac.h"
#include "lcd.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */

/* The functions in this file are generated using "Application Leading Tool (Applilet)" for e2 studio.
 * Warnings exist for interrupt handlers. */

static void uint32_tobcdstring (char * const output_string, const uint8_t pos, uint32_t number);
volatile uint8_t Frequency_Measured_Flag = 0U;
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
    Frequency_Measured_Flag = 1;
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */

/*******************************************************************************
* Function Name: Update_LCD
* Description  : Updates LCD with measurements for IWDT frequency and limits
* Arguments    : none
* Return Value : none
* Note         :
*******************************************************************************/
void Update_LCD(void)
{
    /* Declare temporary character string */
    char lcd_buffer[22] = "Actual Freq:       Hz";

        /* Convert frequency result into a character string, and store in the
           local string lcd_buffer */
        uint32_tobcdstring(lcd_buffer, 12, Frequency_Value());

        /* Display the contents of the local string lcd_buffer */
        Display_LCD(5, (uint8_t *)lcd_buffer);


    strcpy(lcd_buffer, "Upper Limit:       Hz");

    /* Convert frequency result into a character string, and store in the
       local string lcd_buffer */
    uint32_tobcdstring(lcd_buffer, 12, Upper_Frequency_Limit());

    /* Display the contents of the local string lcd_buffer */
    Display_LCD(2, (uint8_t *)lcd_buffer);

    strcpy(lcd_buffer, "Lower Limit:       Hz");

    /* Convert frequency result into a character string, and store in the
       local string lcd_buffer */
    uint32_tobcdstring(lcd_buffer, 12, Lower_Frequency_Limit());

    /* Display the contents of the local string lcd_buffer */
    Display_LCD(3, (uint8_t *)lcd_buffer);

    /* reset flag */
    Frequency_Measured_Flag = 0U;
}
/*******************************************************************************
* End of function Update_LCD
*******************************************************************************/

/*******************************************************************************
* Function Name: Frequency_Measured
* Description  : Indicates if a scheduled frequency measurement is due.
* Arguments    : none
* Return Value : 0 : Not due
*                1 : due
* Note         :
*******************************************************************************/
uint8_t Frequency_Measured(void)
{
    return (Frequency_Measured_Flag);
}
/*******************************************************************************
* End of function Frequency_Measured
*******************************************************************************/

/*******************************************************************************
* Function Name: uint32_tobcdstring
* Description  : This function converts an 32bit integer into a BCD string,
*                which is inserted into the character array passed as an
*                argument.
* Arguments    : (char *)string   - Pointer to array to store BCD string
*                (uint8_t)pos     - Array element to start BCD string from.
*                (uint32_t)number - 32bit integer to convert.
* Return Value : none
* Note         : No input validation is used, so output data can overflow the
*                array passed.
*******************************************************************************/
static void uint32_tobcdstring (char * const output_string, const uint8_t pos,
                                                                uint32_t number)
{
    /* Local digit position variable */
    uint8_t digit = 6u;

    /* Digit is a leading zero indicator variable */
    uint8_t leading_zero = 1U;

    /* Local string buffer variable */
    char buffer_string[6] = "00000";

    /* Decrement digit each iteration, until digit is zero */
    while (digit--)
    {
        /* Write BCD value to string element */
        buffer_string[digit] = (char) (0x30u + (number % 10u));

        /* Move to the next decimal digit */
        number /= 10u;
    }

    /* Set digit position to zero */
    digit = 0u;

    /* Loop through each digit, and replace leading zeros with a space
       character */
    while (digit < 6u)
    {
        /* Check if digit is not a zero character, or if leading_zero indicator
           is false */
        if ((0x30 != buffer_string[digit]) || (!leading_zero))
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
* End of function uint32_tobcdstring
*******************************************************************************/
/* End user code. Do not edit comment generated here */
