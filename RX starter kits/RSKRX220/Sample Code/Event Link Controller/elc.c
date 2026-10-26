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
* File Name     : elc.c
* Version       : 1.00
* Device        : R5F52206
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX220
* Description   : Defines the ELC & ADC configuration functions.
*******************************************************************************/
/*******************************************************************************
* History       : 14.12.2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/

/* Defines RX220 port registers */
#include <iorx220.h>

/* Defines macros relating to the RX220 user LEDs and switches */
#include "rskrx220def.h"

/* LCD controlling function prototypes & macro definitions */
#include "lcd.h"

/* Defines switch functions and variables used in this file */
#include "switch.h"

/* Defines ADC conversion functions used in this file */
#include "adc.h"

/* Provides declarations of functions defined in this file */
#include "elc.h"

/*******************************************************************************
* Function Prototypes
*******************************************************************************/

/* 16 bit integer to character string function prototype declaration */
static void uint16_to_string (char * const output_string, uint8_t pos, 
                                                   const uint16_t input_number);

/*******************************************************************************
* Global Variables
*******************************************************************************/

/* Global ADC result container variable */
volatile uint16_t usADC_Result[16];

/*******************************************************************************
* Function Name: init_elc
* Description  : This function configures the ADC & ELC unit. The ADC
*                conversion end interrupt will trigger the event of toggling 
*                the port pin PB0.  
* Arguments    : none
* Return Value : none
*******************************************************************************/
void init_elc (void)
{
    /* Reconfigure the port pin P14 (connected to LED0) as input */
    LED0_PORT_DIR = 0x0;

    /* Configure the port pin PB0 as output */
    PORTB.PDR.BIT.B0 = 0x1u;
            
    /* Configure the port pin PB0 as 1  */  
    PORTB.PODR.BIT.B0 = 0x1u;   
    
    /* Protection off */
    SYSTEM.PRCR.WORD = PROTECTOFF;
    
    /* Cancel the ELC module clock stop mode */
    MSTP_ELC = 0x0;
    
    /* Protection on */
    SYSTEM.PRCR.WORD = PROTECTON;   

    /* Create a link between ADC conversion end and toggling the port pin
       PB0 */
    ELC.ELSR[24].BYTE = 0x58u;

    /* Use the port pin PB0 as single port trigger module   */
    ELC.PEL0.BIT.PSB = 0x0;
    ELC.PEL0.BIT.PSP = 0x1u;
    ELC.PEL0.BIT.PSM = 0x2u;

    /* Enable the ELC   */
    ELC.ELCR.BIT.ELCON = 0x1u;
            
    /* Configure switch callback function */
    set_switch_release_callback(cb_switch_elc);     
}
/******************************************************************************
* End of function init_elc
******************************************************************************/

/******************************************************************************
* Function Name: cb_switch_elc 
* Description  : Callback function for switch. If the SW1 press is detected,
*                this function starts the ADC conversion. When the ADC 
*                conversion completes, the ELC triggers the event of toggling 
*                the port pin PB0. 
* Arguments    : none
* Return Value : none
******************************************************************************/
void cb_switch_elc (void)
{
    /* Declare temporary character string */
    char lcd_buffer[] = "=H\'WXYZ \0";

    /* Check if switch pressed was SW1 */
    if (g_switch_flag & SWITCHPRESS_1)
    {
        /* Declare a variable to store the ADC result */
        uint16_t adc_result;
        
        /*  Run the ADC conversion */
        adc_result = run_adc_single();   
            
        /* Convert ADC result into a character string, and store in the local
           string lcd_buffer */     
        uint16_to_string(lcd_buffer, 3u, adc_result);

        /* Display the contents of the local string lcd_buffer */
        Display_LCD(LCD_LINE2, lcd_buffer);     
    }

    /* Clear the switch flag    */
    g_switch_flag = 0x00;

}
/*******************************************************************************
* End of function cb_switch_elc 
*******************************************************************************/

/*******************************************************************************
* Function Name: uint16_to_string
* Description  : Function converts a 16 bit integer into a character string, 
*                inserts it into the array via the pointer passed at execution.
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
static void uint16_to_string (char * const output_string, uint8_t pos, 
                                                    const uint16_t input_number)
{
    /* Declare temporary character storage variable, and bit_shift variable */  
    char     a = 0x00;
    
    /* Number of bits in 3 hex characters */
    uint8_t  bit_shift = 12u;
    
    /* Declare 16bit mask variable */
    uint16_t mask = 0xF000u;
    
    /* Loop through until each hex digit is converted to an ASCII character */
    while (bit_shift < 30u)
    {
        /* Mask and shift the hex digit, and store in temporary variable, a */ 
        a = (char)((input_number & mask) >> bit_shift);
        
        /* Convert the hex digit into an ASCII character, and store in output
           string */
        output_string[pos] = (char)((a < 0x0A) ? (a + 0x30) : (a + 0x37));
        
        /* Shift the bit mask 4 bits to the right, to convert the next digit */
        mask = (uint16_t) (mask >> 4u);
        
        /* Decrement the bit_shift counter by 4 (bits in each digit) */
        bit_shift -= 4u;
        
        /* Increment the output string location */
        pos++;
    }
}
/*******************************************************************************
* End of function uint16_to_string
*******************************************************************************/

