/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
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
* Copyright (C) 2022 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : main.c
* Version      : 1.00
* Device(s)    : R5F56609HxFB
* Tool-Chain   : CCRX
* H/W Platform : RSKRX660
* Description  : This file implements main function.
* Creation Date: 2022-01-18
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/

#include "r_smc_entry.h"
#include "r_okaya_lcd.h"

#include "r_rsk_switch.h"
#include "r_rsk_debug.h"
#include "rskrx660def.h"

#include "Config_S12AD0.h"
#include "Config_SCI10.h"

#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables (to be accessed by other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/

/* Variable for flagging user requested ADC conversion */
volatile uint8_t g_adc_trigger = FALSE;

/* Prototype declaration for cb_switch_press */
static void cb_switch_press (void);

/* Prototype declaration for get_adc */
static uint16_t get_adc (void);

/* Prototype declaration for lcd_display_adc */
static void lcd_display_adc (const uint16_t adc_result);

/* Prototype declaration for uart_display_adc */
static void uart_display_adc (const uint8_t adc_count, const uint16_t adc_result);

/* Variable to store the A/D conversion count for user display */
static uint8_t s_adc_count = 0;

/* Prototype declaration for led_display_count */
static void led_display_count (const uint8_t count);

void main (void);

/***********************************************************************************************************************
* Function Name: main
* Description  : This function implements main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void main(void)
{
    /* Initialize the switch module */
    R_SWITCH_Init();

    /* Set the call back function when SW1 or SW2 is pressed */
    R_SWITCH_SetPressCallback(cb_switch_press);

    /* Initialize the debug LCD */
    R_LCD_Init();

    /* Displays the application name on the debug LCD.
       Casting for use as characters. */
    R_LCD_Display(0, (uint8_t *)" RSKRX660 ");

    /* Casting for use as characters. */
    R_LCD_Display(1, (uint8_t *)" Tutorial ");

    /* Casting for use as characters. */
    R_LCD_Display(2, (uint8_t *)" Press Any Switch ");

    /* Start the A/D converter */
    R_Config_S12AD0_Start();

    /* Set up SCI10 receive buffer and callback function */
    R_Config_SCI10_Serial_Receive((uint8_t *)&g_rx_char, 1);

    /* Enable SCI10 operations */
    R_Config_SCI10_Start();

    while (1U)
    {
        uint16_t adc_result;

        /* Wait for user requested A/D conversion flag to be set (SW1 or SW2) */
        if (TRUE == g_adc_trigger)
        {
            /* Call the function to perform an A/D conversion */
            adc_result = get_adc();

            /* Display the result on the LCD */
            lcd_display_adc(adc_result);

            /* Increment the adc count and display using the LEDs */
            if (16 == ( ++s_adc_count))
            {
                s_adc_count = 0;
            }
            led_display_count(s_adc_count);

            /* Send the result to the UART */
            uart_display_adc(s_adc_count, adc_result);

            /* Reset the flag */
            g_adc_trigger = FALSE;
        }

        /* SW3 is directly wired into the ADTRG0n pin so will
           cause the interrupt to fire */
        else if (TRUE == g_adc_complete)
        {
            /* Get the result of the A/D conversion */
            R_Config_S12AD0_Get_ValueResult(ADCHANNEL0, &adc_result);

            /* Display the result on the LCD */
            lcd_display_adc(adc_result);

            /* Increment the adc count and display using the LEDs */
            if (16 == ( ++s_adc_count))
            {
                s_adc_count = 0;
            }
            led_display_count(s_adc_count);

            /* Send the result to the UART */
            uart_display_adc(s_adc_count, adc_result);

            /* Reset the flag */
            g_adc_complete = FALSE;
        }
        else
        {
            /* do nothing */
        }
    }
}

/***********************************************************************************************************************
* End of function main
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : cb_switch_press
* Description   : Switch press callback function. Sets g_adc_trigger flag.
* Argument      : none
* Return value  : none
***********************************************************************************************************************/
static void cb_switch_press(void)
{
    /* Check if switch 1 or 2 was pressed */
    if (g_switch_flag & (SWITCHPRESS_1 | SWITCHPRESS_2))
    {
        /* Set the flag indicating a user requested A/D conversion is required */
        g_adc_trigger = TRUE;

        /* Clear flag */
        g_switch_flag = 0x0;
    }
}

/***********************************************************************************************************************
* End of function cb_switch_press
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : get_adc
* Description   : Reads the ADC result, converts it to a string and displays it on the LCD panel.
* Argument      : none
* Return value  : uint16_t adc value
***********************************************************************************************************************/
static uint16_t get_adc(void)
{
    /* A variable to retrieve the adc result */
    uint16_t adc_result;

    /* Stop the A/D converter being triggered from the pin ADTRG0n */
    R_Config_S12AD0_Stop();

    /* Start a conversion */
    R_S12AD0_SWTriggerStart();

    /* Wait for the A/D conversion to complete */
    while (FALSE == g_adc_complete)
    {
        /* Wait */
        nop();
    }

    /* Stop conversion */
    R_S12AD0_SWTriggerStop();

    /* Clear ADC flag */
    g_adc_complete = FALSE;

    R_Config_S12AD0_Get_ValueResult(ADCHANNEL0, &adc_result);

    /* Set AD conversion start trigger source back to ADTRG0n pin */
    R_Config_S12AD0_Start();

    return (adc_result);
}

/***********************************************************************************************************************
* End of function get_adc
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : lcd_display_adc
* Description   : Converts adc result to a string and displays it on the LCD panel.
* Argument      : uint16_t adc result
* Return value  : none
***********************************************************************************************************************/
static void lcd_display_adc(const uint16_t adc_result)
{
    /* Declare a temporary variable */
    char_t tmp;

    /* Declare temporary character string */
    char_t lcd_buffer[11] = " ADC: XXXH";

    /* Convert ADC result into a character string, and store in the local.
       Casting to ensure use of correct data type. */
    tmp           = (char_t)((adc_result & 0x0F00) >> 8);
    lcd_buffer[6] = (tmp < 0x0A) ? (tmp + 0x30) : (tmp + 0x37);

    /* Convert ADC result into a character string, and store in the local.
       Casting to ensure use of correct data type. */
    tmp           = (char_t)((adc_result & 0x00F0) >> 4);
    lcd_buffer[7] = (tmp < 0x0A) ? (tmp + 0x30) : (tmp + 0x37);

    /* Convert ADC result into a character string, and store in the local.
       Casting to ensure use of correct data type. */
    tmp           = (char_t)(adc_result & 0x000F);
    lcd_buffer[8] = (tmp < 0x0A) ? (tmp + 0x30) : (tmp + 0x37);

    /* Display the contents of the local string lcd_buffer */
    R_LCD_Display(3, (uint8_t *)lcd_buffer);
}

/***********************************************************************************************************************
* End of function lcd_display_adc
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : uart_display_adc
* Description   : Converts adc result to a string and sends it to the UART1.
* Argument      : uint8_t : adc_count
*                 uint16_t: adc_result
* Return value  : none
***********************************************************************************************************************/
static void uart_display_adc(const uint8_t adc_count, const uint16_t adc_result)
{
    /* Declare a temporary variable */
    char_t tmp;

    /* Declare temporary character string */
    char_t uart_buffer[] = "ADC xH Value: xxxH\r\n";

    /* Convert ADC result into a character string, and store in the local.
       Casting to ensure use of correct data type. */
    tmp            = (char_t)(adc_count & 0x000F);
    uart_buffer[4] = (tmp < 0x0A) ? (tmp + 0x30) : (tmp + 0x37);

    /* Convert ADC result into a character string, and store in the local.
       Casting to ensure use of correct data type. */
    tmp             = (char_t)((adc_result & 0x0F00) >> 8);
    uart_buffer[14] = (tmp < 0x0A) ? (tmp + 0x30) : (tmp + 0x37);

    /* Convert ADC result into a character string, and store in the local.
       Casting to ensure use of correct data type. */
    tmp             = (char_t)((adc_result & 0x00F0) >> 4);
    uart_buffer[15] = (tmp < 0x0A) ? (tmp + 0x30) : (tmp + 0x37);

    /* Convert ADC result into a character string, and store in the local.
       Casting to ensure use of correct data type. */
    tmp             = (char_t)(adc_result & 0x000F);
    uart_buffer[16] = (tmp < 0x0A) ? (tmp + 0x30) : (tmp + 0x37);

    /* Send the string to the UART */
    r_debug_print(uart_buffer);
}

/***********************************************************************************************************************
* End of function uart_display_adc
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : led_display_count
* Description   : Converts count to binary and displays on 4 LEDS0-3
* Argument      : uint8_t count
* Return value  : none
***********************************************************************************************************************/
static void led_display_count(const uint8_t count)
{
    /* Set LED0 according to bit0 of count parameter */
    LED0 = ((count & 0x01) ? LED_ON : LED_OFF);

    /* Set LED1 according to bit1 of count parameter */
    LED1 = ((count & 0x02) ? LED_ON : LED_OFF);

    /* Set LED2 according to bit2 of count parameter */
    LED2 = ((count & 0x04) ? LED_ON : LED_OFF);

    /* Set LED3 according to bit3 of count parameter */
    LED3 = ((count & 0x08) ? LED_ON : LED_OFF);
}

/***********************************************************************************************************************
* End of function led_display_count
***********************************************************************************************************************/
