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
* File Name    : r_cg_main.c
* Version      : Applilet4 for RX111 V1.00.00.05 [06 Jun 2013]
* Device(s)    : R5F51115AxFM
* Tool-Chain   : IAR Systems icc78k0r
* Description  : This file implements main function.
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
#include "r_cg_cgc.h"
#include "r_cg_sci.h"
#include "r_cg_s12adb.h"
#include "r_cg_dac.h"
/* Start user code for include. Do not edit comment generated here */
#include "lcd.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
uint16_t g_adc_result;

static void reportA2DValue(void);
static void uint16_to_string (uint8_t * const output_string, uint8_t pos,
                                const uint16_t input_number);
/* End user code. Do not edit comment generated here */


/* Set option bytes */
#pragma location = "OPTBYTE"
__root const uint32_t OFS0_location = 0xFFFFFFFFUL;
#pragma location = "OPTBYTE"
__root const uint32_t OFS1_location = 0xFFFFFFFFUL;


void R_MAIN_UserInit(void);
/***********************************************************************************************************************
* Function Name: main
* Description  : This function implements main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void main(void)
{
    R_MAIN_UserInit();
    /* Start user code. Do not edit comment generated here */
    /* Display the sample name on the debug LCD */
    DisplayCenter (0, (uint8_t *)"DAC Output");
    DisplayCenter (2, (uint8_t *)"= Hex 0xxxxx");

    /* Initialise the ADC module and start conversions */
    R_S12ADb_Start();
    R_DAC0_Start();
    
    /* clear GBADI Interrupts (set in Applilet) which prevent continuous interrupts from running */
    //IEN(S12AD,GBADI) = 0U;      /* Disable GBADI interrupt */
    //IR(S12AD,GBADI) = 0U;       /* Clear GBADI interrupt flag */

    while (1U)
    {
        /* Interrupt reports A2D complete and LCD updated here */
        /* Done this way because LCD uses SPI and interrupts cannot be nested on RX*/
        if (A2DConversionComplete)
        {
            A2DConversionComplete = 0U;
            
            /* Read the A/D result (12 bits)*/
            g_adc_result = (uint16_t)(S12AD.ADDR0); 

            /* Display the value */
            reportA2DValue();

            /* The DAC is 10 bits, so divide the value by 4 */
            g_adc_result = (uint16_t)(g_adc_result >> 2);

            /* Write the value to the DAC */
            R_DAC0_Set_ConversionValue(g_adc_result);
            
        }
    }
     /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: R_MAIN_UserInit
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_MAIN_UserInit(void)
{
    /* Start user code. Do not edit comment generated here */
    /* Initialise the Debug LCD */
    Init_LCD();
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/***********************************************************************************************************************
* Function Name: reportA2DValue
* Description  : Display the A/D value
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void reportA2DValue(void)
{
    /* Declare temporary character string */
    uint8_t lcd_buffer[13] = "= Hex 0xxxxx";

    /* Convert ADC result into a character string, and store in the local
       string lcd_buffer */
    uint16_to_string(lcd_buffer, 8, g_adc_result);

    /* Display the contents of the local string lcd_buffer */
    DisplayCenter (2, lcd_buffer);

    /* Delay briefly for digit to be read */
    DisplayDelay(5000);

}

/***********************************************************************************************************************
* Function Name : uint16_to_string
* Description   : Function converts a 16 bit integer into a character string, 
*                  inserts it into the array via the pointer passed at execution.
* Argument      : * output_string : Pointer to uint8_t array that will hold 
*                                   character string.
*                             pos : uint8_t number, element number to begin 
*                                   inserting the character string from (offset).
*                    input_number : 16 bit integer to convert into a string.
* Return value  : none
* Note            : No input validation is used, so output data can overflow the
*                  array passed.
***********************************************************************************************************************/
static void uint16_to_string (uint8_t * const output_string, uint8_t pos, uint16_t const input_number)
{
    /* Declare temporary character storage variable, and bit_shift variable */    
    uint8_t     a;
    uint8_t     bit_shift = 12u;
    
    /* Declare 16bit mask variable */
    uint16_t    mask = 0xF000;
    
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
/* End user code. Do not edit comment generated here */
