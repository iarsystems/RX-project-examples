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
* File Name     : adc12oneshot.c
* Version       : 1.00
* Device        : R5F563TE
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX63T144
* Description   : Defines ADC functions used for 12bit S12AD unit.
*******************************************************************************/
/*******************************************************************************
* History       : 05.12.2013  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/
/* Defines R5F563TE port registers */
#include <iorx63t.h>

/* LCD controlling function prototypes & macro definitions */
#include "lcd.h"

/* Defines switch functions and variables used in this file */
#include "switch.h"

/* LCD controlling function prototypes & macro definitions */
#include "lcd.h"

/* Provides declarations of functions defined in this file */
#include "adc12oneshot.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* The variable g_adc_result is used to contain the value of the 12 bit ADC */
volatile uint16_t g_adc_result;

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
/* 16 bit integer to character string function prototype declaration */
static void uint16_to_string (char * const output_string, uint8_t pos,
                                const uint16_t input_number);

/*******************************************************************************
* Function Name : Init_ADC12Oneshot
* Description   : ADC initialisation function. This function configures the
*                 S12AD unit for single scan operation.
* Argument      : none
* Return value  : none
*******************************************************************************/
void Init_ADC12Oneshot (void)
{
    /* Protection off */
    SYSTEM.PRCR.WORD = 0xA503;

    /* Cancel the S12AD module clock stop mode */
    MSTP_S12AD = 0x0;

    /* Protection on */
    SYSTEM.PRCR.WORD = 0xA500;

    /* Use the AN000 (Potentiometer) pin
       as an I/O for peripheral functions */

    /* Enable Group B scan end Interrupt */
    S12AD.ADCSR.WORD |= 0x40;

    /* Selects AN000 */
    S12AD.ADANSA.WORD = 0x0001;

    /* Set ADCSR to "One Shot", single cycle mode */
    S12AD.ADCSR.BIT.ADCS = 0x0;
}
/*******************************************************************************
* End of function Init_ADC12Oneshot
*******************************************************************************/

/*******************************************************************************
* Function Name: uint16_to_string
* Description  : Function converts a 16 bit integer into a character string,
*                inserts it into the array via the pointer passed at execution.
* Argument     : * output_string : Pointer to char array that will hold
*                                  character string.
*                            pos : uint8_t number, element number to begin
*                                  inserting the character string from (offset).
*                   input_number : 16 bit integer to convert into a string.
* Return value : none
* Note         : No input validation is used, so output data can overflow the
*                array passed.
*******************************************************************************/
static void uint16_to_string (char * const output_string, uint8_t pos,
                                const uint16_t input_number)
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

        /* Convert the hex digit into an ASCII character, and store in output
           string */
        output_string[pos] = (char)((a < 0x0A) ? (a + 0x30) : (a + 0x37));

        /* Shift the bit mask 4 bits to the right, to convert the next digit */
        mask = (uint16_t) (mask >> 4u);

        /* Decrement the bit_shift counter by 4 (bits in a each digit) */
        bit_shift -= 4u;

        /* Increment the output string location */
        pos++;
    }
}
/*******************************************************************************
* End of function uint16_to_string
*******************************************************************************/

/******************************************************************************
* Function Name : CB_Switch_Press
* Description   : Switch press callback funtion. Reads the S12AD result,
*                 converts it to a string and displays it on the debug LCD.
* Argument      : none
* Return value  : none
******************************************************************************/
void CB_Switch_Press (void)
{
    /* Check if switch 3 was pressed */
    if ((g_switch_flag & 0x0F) & SWITCHHOLD_3)
    {

        /* Declare temporary character string */
        char lcd_buffer[9] = "=H\'WXYZ\0";

        /* Start a conversion */
        S12AD.ADCSR.BIT.ADST = 1;

        /* Wait for the conversion to end */
        while (1 == S12AD.ADCSR.BIT.ADST)
        {
        }

        /* Fetch the S12AD value */
        g_adc_result = S12AD.ADDR0;

        /* Convert ADC result into a character string, and store in the local
           string lcd_buffer */
        uint16_to_string(lcd_buffer, 3, g_adc_result);

        /* Display the contents of the local string lcd_buffer */
        Display_LCD(LCD_LINE2, lcd_buffer);

        /* Clear flag */
        g_switch_flag = 0x0;
    }
}
/******************************************************************************
* End of function CB_Switch_Press
******************************************************************************/

