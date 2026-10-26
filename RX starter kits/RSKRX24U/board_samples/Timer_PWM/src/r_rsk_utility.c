/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No 
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all 
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM 
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES 
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS 
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of 
* this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer 
*
* Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.    
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_rsk_utility.c
* Version      : 1.00
* Device(s)    : R5F524UEAxFB
* Tool-Chain   : CCRX
* H/W Platform : RSKRX24U
* Description  : Contains functions for converting an integer to decimal or hexadecimal string.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 30.11.2016 1.00     First Release
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Includes integer conversion function prototypes */
#include "r_rsk_utility.h"


/***********************************************************************************************************************
* Function Name: uint16_to_bcd_string
* Description  : This function converts an 16bit integer into a BCD string,
*                which is inserted into the character array passed as an argument.
* Arguments    : char * string      - Pointer to array to store BCD string
*                uint8_t pos        - Array element to start BCD string from.
*                uint16_t number    - 16bit integer to convert.
* Return value : none
* Note         : No input validation is used, so output data can overflow the array passed.
***********************************************************************************************************************/
void uint16_to_bcd_string (char * const output_string, uint8_t pos, uint16_t number)
{
    /* Local digit position variable */
    uint8_t digit = 5u;

    /* Digit is a leading zero indicator variable */
    uint8_t leading_zero = 1;

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
        if ((0x30 != buffer_string[digit]) || (!leading_zero))
        {
            /* Copy digit from buffer string to output string */
            output_string[pos++] = buffer_string[digit];

            /* Set the leading zero indicator variable to false */
            leading_zero = 0;
        }

        /* Increment the loop counter */
        digit++;
    }
}

/***********************************************************************************************************************
* End of function uint16_to_bcd_string
***********************************************************************************************************************/


/***********************************************************************************************************************
* Function Name: uint16_to_string
* Description  : Function converts a 16 bit integer into a character string,
*                inserts it into the array via the pointer passed at execution.
* Arguments    : * output_string : Pointer to char array that will hold character string.
*                            pos : uint8_t number, element number to begin inserting the character string from (offset).
*                   input_number : 16 bit integer to convert into a string.
* Return value : none
* Note         : No input validation is used, so output data can overflow the array passed.
***********************************************************************************************************************/
void uint16_to_string (char * const output_string, uint8_t pos, const uint16_t input_number)
{
    /* Declare temporary character storage variable, and bit_shift variable */
    uint8_t  a         = 0x00;
    uint8_t  bit_shift = 12u;

    /* Declare 16bit mask variable */
    uint16_t mask      = 0xF000;

    /* Loop through until each hex digit is converted to an ASCII character */
    while (bit_shift < 30u)
    {

        /* Mask and shift the hex digit, and store in temporary variable, a */
        a = (uint8_t)((input_number & mask) >> bit_shift);

        /* Convert the hex digit into an ASCII character, and store in output string */
        output_string[pos] = (char)((a < 0x0A) ? (a + 0x30) : (a + 0x37));

        /* Shift the bit mask 4 bits to the right, to convert the next digit */
        mask = (uint16_t) (mask >> 4u);

        /* Decrement the bit_shift counter by 4 (bits in a each digit) */
        bit_shift = (uint8_t)(bit_shift - 4u);

        /* Increment the output string location */
        pos++;
    }
}

/***********************************************************************************************************************
* End of function uint16_to_string
***********************************************************************************************************************/
