/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIESREGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_main.c
* Version      : Applilet4 for RX113 V1.01.00.02 [31 Jul 2014]
* Device(s)    : R5F51138AxFP
* Tool-Chain   : IAR Systems ICCRX
* Description  : This file implements main function.
* Creation Date: 2015-01-16
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
#include "r_cg_port.h"
#include "r_cg_sci.h"
#include "r_cg_s12ad.h"
/* Start user code for include. Do not edit comment generated here */
#include "r_cg_s12ad.h"
#include "r_lcd_appv2.h"
#include "r_rsk_utility.h"
#include "rskrx113def.h"

/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */

/* Welcome banner - displayed on serial port at startup*/
static uint8_t welcome_banner[] = "RSK RX113 - Tutorial - Press 'c' or SW3 for ADC Conversion\r\n\0";

/* Prototype declaration for get_adc */
static uint16_t get_adc (void);

/* Prototype declaration for lcd_display_adc */
static void lcd_display_adc (const uint16_t adc_result);

/* Prototype declaration for uart_display_adc */
static void uart_display_adc (const uint8_t adc_count, const uint16_t adc_result);

/* Variable to store the ADC conversion count for user display */
static uint8_t adc_count = 0;

/* Prototype declaration for led_display_count */
static void led_display_count (const uint8_t count);

/* Variable for flagging user requested ADC conversion */
volatile uint8_t g_adc_trigger = FALSE;

/* End user code. Do not edit comment generated here */


/* Set option bytes */
#pragma location = "OPTBYTE"
__root const uint32_t OFS0_location = 0xFFFFFFFFUL;
#pragma location = "OPTBYTE"
__root const uint32_t OFS1_location = 0xFFFFFF7FUL;


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

    /* Display Project Title on LCD*/
    R_LCD_DisplayPanelString( PANEL_LCD_LINE1, (uint8_t*) "TUTOR");

    /* Set up SCI1 receive buffer and callback function */
    R_SCI1_Serial_Receive((uint8_t *) &g_rx_char, 1);

    /* Enable SCI1 operations */
    R_SCI1_Start();

    /* Display Welcome Banner on Serial Port */
    R_SCI1_AsyncTransmit(welcome_banner, sizeof(welcome_banner));

    while (1U)
    {
        uint16_t adc_result;

        /* If the user has requested ADC sample via the serial port */
        if (TRUE == g_adc_trigger)
        {
            /* Call the function to perform an ADC conversion */
            adc_result = get_adc();

            /* Display the result on the LCD */
            lcd_display_adc(adc_result);

            /* Display count on LEDs */
            led_display_count(adc_count);

            /* Send the result to SCI1 UART */
            uart_display_adc(adc_count, adc_result);

            /* Increment the adc_count and check roll over */
            if (16 == (++adc_count))
            {
                adc_count = 0;
            }

            /* Reset the flag */
            g_adc_trigger = FALSE;
        }

        /* SW3 is directly wired into the ADTRG0n pin so will 
         cause the conversion and interrupt */
        else if (TRUE == g_adc_complete)
        {
            /* Get the result of the ADC conversion */
            R_S12AD_Get_ValueResult(ADCHANNEL0, &adc_result);

            /* Display the result on the LCD */
            lcd_display_adc(adc_result);

            /* Display count on LEDs */
            led_display_count(adc_count);

            /* Send the result to the UART */
            uart_display_adc(adc_count, adc_result);

            /* Increment the adc_count and check roll over */
            if (16 == (++adc_count))
            {
            	adc_count = 0;
            }

            /* Reset the flag */
            g_adc_complete = FALSE;
        }
        else
        {
            /* do nothing */
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

    /* Initialise the LCD for the RSK LCD APP V2 display board */
    R_LCD_Create();
    R_LCD_Start();

    /* Start the ADC */
    R_S12AD_Start();

    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */

/******************************************************************************
 * Function Name : get_adc
 * Description   : Creates a ADC12 Software trigger and returns the ADC result,
 *  				  once the ADC conversion is complete.
 * Argument      : none
 * Return value  : uint16_t ADC sample value
 ******************************************************************************/
static uint16_t get_adc (void)
{
    /* A variable to retrieve the ADC result */
    uint16_t adc_result;

    /* Start a conversion */
    R_S12AD_SWTriggerStart();

    /* Wait for the ADC conversion to complete */
    while (FALSE == g_adc_complete)
    {
        /* Wait */
    }

    /* Stop conversion */
    R_S12AD_SWTriggerStop();

    /* Clear ADC flag */
    g_adc_complete = FALSE;

    R_S12AD_Get_ValueResult(ADCHANNEL0, &adc_result);

    /* Set AD conversion start trigger source back to ADTRG0n pin */
    R_S12AD_Start();

    return adc_result;
}

/******************************************************************************
 * End of function get_adc
 ******************************************************************************/

/******************************************************************************
 * Function Name : lcd_display_adc
 * Description   : Converts ADC result to a string and displays
 *                 it on the LCD panel.
 * Argument      : uint16_t adc result
 * Return value  : none
 ******************************************************************************/
static void lcd_display_adc (const uint16_t adc_result)
{
    /* Declare temporary character string */
    char lcd_buf[4];

    /* Convert ADC result into a character string, and store in the
     local string lcd_buffer */
    uint16_to_string(lcd_buf, 0u, adc_result);

    /* Display the ADC value - Line 3 provides three
     * characters, so skip the unused leading zero
     */
    R_LCD_DisplayPanelString( PANEL_LCD_LINE3, (uint8_t *) lcd_buf + 1);
}

/******************************************************************************
 * End of function lcd_display_adc
 ******************************************************************************/

/******************************************************************************
 * Function Name : uart_display_adc
 * Description   : Converts ADC result to a string and sends it to the UART1.
 * Argument      : uint8_t : adc_count
 *                 uint16_t: ADC result
 * Return value  : none
 ******************************************************************************/
static void uart_display_adc (const uint8_t adc_count, const uint16_t adc_result)
{
    /* Declare a temporary variable */
    char a;

    /* Declare temporary character string */
    static uint8_t uart_buffer[] = "ADC xH Value: xxxH\r\n";

    /* Convert ADC result into a character string, and store in the local.
     Casting to ensure use of correct data type. */
    a = (char) (adc_count & 0x000F);
    uart_buffer[4] = (char) ((a < 0x0A) ? (a + 0x30) : (a + 0x37));
    a = (char) ((adc_result & 0x0F00) >> 8);
    uart_buffer[14] = (char) ((a < 0x0A) ? (a + 0x30) : (a + 0x37));
    a = (char) ((adc_result & 0x00F0) >> 4);
    uart_buffer[15] = (char) ((a < 0x0A) ? (a + 0x30) : (a + 0x37));
    a = (char) (adc_result & 0x000F);
    uart_buffer[16] = (char) ((a < 0x0A) ? (a + 0x30) : (a + 0x37));

    /* Send the string to the UART */
    R_SCI1_AsyncTransmit(uart_buffer, sizeof(uart_buffer));
}

/******************************************************************************
 * End of function uart_display_adc
 ******************************************************************************/

/******************************************************************************
 * Function Name : led_display_count
 * Description   : Converts count to binary and displays on 4 LEDS0-3
 * Argument      : uint8_t count
 * Return value  : none
 ******************************************************************************/
static void led_display_count (const uint8_t count)
{
    /* Set LEDs according to lower nibble of count parameter */
    LED0 = (uint8_t) ((count & 0x01) ? LED_ON : LED_OFF);
    LED1 = (uint8_t) ((count & 0x02) ? LED_ON : LED_OFF);
    LED2 = (uint8_t) ((count & 0x04) ? LED_ON : LED_OFF);
    LED3 = (uint8_t) ((count & 0x08) ? LED_ON : LED_OFF);
}

/******************************************************************************
 * End of function led_display_count
 ******************************************************************************/

/* End user code. Do not edit comment generated here */
