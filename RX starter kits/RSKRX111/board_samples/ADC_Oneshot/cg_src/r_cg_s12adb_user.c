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
* File Name    : r_cg_s12adb_user.c
* Version      : Applilet4 for RX111 V1.00.00.05 [06 Jun 2013]
* Device(s)    : R5F51115AxFM
* Tool-Chain   : IAR Systems icc78k0r
* Description  : This file implements device driver for S12ADb module.
* Creation Date: 2013-08-15
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
#include "r_cg_s12adb.h"
/* Start user code for include. Do not edit comment generated here */
#include "switch.h"
#include "lcd.h"
#include <stddef.h>
#include "r_cg_icu.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* Switch flag status variable */
extern volatile uint8_t g_switch_flag;


/* The variable g_adc_result is used to contain the value of the 12 bit ADC */
volatile uint16_t g_adc_result;

/* 16 bit integer to character string function prototype declaration */
static void uint16_to_string(uint8_t * const output_string, uint8_t pos,
                                const uint16_t input_number);

static uint16_t Run_ADC_Single(void);
/* End user code. Do not edit comment generated here */





/* Start user code for adding. Do not edit comment generated here */
/*******************************************************************************
* Function Name: Init_ADC
* Description  : Initialises the ADC
* Arguments    : none
* Return Value : none
* Note         :
*******************************************************************************/
void Init_ADC(void)
{
    /* set switch press callback function */
    SetSwitchPressCallback((void (*)(void))Switch_Pressed);

    /* set switch press callback function */
    SetSwitchReleaseCallback(NULL);

    /* set the result to zero */
    g_adc_result = 0;
}
/*******************************************************************************
* End of function Init_ADC
*******************************************************************************/

/******************************************************************************
* Function Name : CB_Switch_Press
* Description   : Switch press callback function. Reads the S12AD result,
*                 converts it to a string and displays it on the debug LCD.
* Argument      : none
* Return value  : none
******************************************************************************/
void Display_ADC_Reading(void)
{
    /* Declare temporary character string */
    uint8_t lcd_buffer[9] = "0xxxxx ";

    /* Check if switch pressed was SW3 */
    if (g_switch_flag & SWITCHHOLD_3)
    {
        /*  Run the ADC conversion */
        g_adc_result = Run_ADC_Single();

        uint16_to_string(lcd_buffer, 2, g_adc_result);

        /* Display the contents of the local string 'lcd_buffer' */
        Display_LCD(3, lcd_buffer);

    }

    /* Clear switch press flags */
    g_switch_flag &= ~SWITCHPRESS_ALL;
    g_switch_flag &= ~SWITCHHOLD_ALL;

}
/******************************************************************************
* End of function Display_ADC_Reading
******************************************************************************/

/*******************************************************************************
* Function Name: Switch_Pressed
* Description  : indicates if a switch has been pressed
* Arguments    : none
* Return Value : 1 if switch pressed. 0 if not
* Note         :
*******************************************************************************/
uint8_t Switch_Pressed(void)
{
    if (g_switch_flag)
    {
       return (1);
    }
    return (0);
}
/*******************************************************************************
* End of function Switch_Pressed
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
static void uint16_to_string (uint8_t * const output_string, uint8_t pos,
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
        output_string[pos] = (uint8_t)((a < 0x0A) ? (a + 0x30) : (a + 0x37));

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
* Function Name: Run_ADC_Single
* Description  : Runs an A-D conversion and returns the result.
* Arguments    : none
* Return Value : uint16_t result
******************************************************************************/
static uint16_t Run_ADC_Single(void)
{
	uint16_t result;

    /* Start a conversion */
	R_S12ADb_Start();

    /* Wait for the conversion to end */
    while (0x1 == S12AD.ADCSR.BIT.ADST)
    {
    }

    /* get result of conversion */
    R_S12ADb_Get_ValueResult(ADCHANNEL0, &result);

    /* Return the S12AD value */
    return (result);
}
/******************************************************************************
* End of function Run_ADC_Single
******************************************************************************/
/* End user code. Do not edit comment generated here */
