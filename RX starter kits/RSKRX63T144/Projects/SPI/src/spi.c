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
* File Name     : spi.c
* Version       : 1.00
* Device        : R5F563TE
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX63T144
* Description   : Defines controlling functions for SPI module
*******************************************************************************/
/*******************************************************************************
* History       : 05.12.2013  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/
/* Defines  R5F563TE port registers */
#include <iorx63t.h>

/* Defines switch functions and variables used in this file */
#include "switch.h"

/* LCD controlling function prototypes & macro definitions */
#include "lcd.h"

/* Defines ADC conversion functions used in this file */
#include "adc.h"

/* Provides declarations of functions defined in this file */
#include "spi.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Declare storage variable for received data */
volatile uint32_t g_rx_data = 0;

/* Stores the data to be transmitted */
volatile uint32_t g_tx_data;

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
/* 16 bit integer to character string function prototype declaration */
static void uint16_to_string (char * const output_string, uint8_t pos,
                                                   const uint16_t input_number);

/* Switch callback function prototype declaration */
static void cb_switch_spi (void);

/*******************************************************************************
* Function Name: Init_SPI
* Description  : This function configures the SPI interface as a master
*                channel and sets the transfer to a loop back mode. The
*                transfer data is set to 16 bits. The ADC data will be looped
*                back over the SPI bus.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void Init_SPI (void)
{
    /* Disable protection */
    SYSTEM.PRCR.WORD = 0xA503u;

    /* Cancel the RSPI module clock stop mode */
    MSTP_RSPI1 = 0x0;

    /* Enable protection */
    SYSTEM.PRCR.WORD = 0xA500u;

    /* Use SPI 3-wire mode */
    RSPI0.SPCR.BIT.SPMS = 0x1u;

    /* Set to use master SPI mode */
    RSPI1.SPCR.BIT.MSTR = 0x1u;

    /* Enable loopback mode */
    RSPI1.SPPCR.BIT.SPLP2 = 0x1u;

    /* Set bit rate to 240 kbps */
    /* Bit rate = (PCLKB) / (2 * (n + 1)* 2^N) */
    /* Where N = BRDV = 0 below, n is contents of SPBR register - See
       Hardware Manual section 29.2.8 */
    RSPI1.SPBR = 99u;
    RSPI1.SPCMD0.BIT.BRDV = 0x0;

    /* Assert SSL0 */
    RSPI1.SPCMD0.BIT.SSLA = 0x0;

    /* Use 16 bit data length */
    RSPI1.SPCMD0.BIT.SPB = 0xFu;

    /* Access SPDR with long words */
    RSPI1.SPDCR.BIT.SPLW = 0x1u;

    /* Send/receive LSB first */
    RSPI1.SPCMD0.BIT.LSBF = 0x1u;

    /* Empty the transmit buffer */
    RSPI0.SPDR.LONG = 0x00000000;

    /* Enable SPI */
    RSPI1.SPCR.BIT.SPE = 0x1u;

    /* Configure switch callback function */
    SetSwitchReleaseCallback(cb_switch_spi);
}
/*******************************************************************************
* End of function Init_SPI
*******************************************************************************/

/*******************************************************************************
* Function Name: cb_switch_spi
* Description  : If switch SW3 is pressed an A/D conversion is started and the
*                result is stored in 'gAdcResult' variable. The result is
*                shifted out of the transmit shift register before being shifted
*                into the receive shift register.
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void cb_switch_spi (void)
{
    /* Declare temporary character string */
    char lcd_buffer[9] = "=H\'WXYZ ";

    /* Check if switch pressed was SW3 */
    if (g_switch_flag & SWITCHPRESS_3)
    {
        /* Declare a variable to store the ADC result */
        uint16_t adc_result;

        /*  Run the ADC conversion */
        adc_result = Run_ADC_Single();

        /* Copy the ADC reading to a transmit buffer */
        g_tx_data = adc_result;

        /* Write data to SPI unit */
        RSPI1.SPDR.LONG = g_tx_data;

        /* Brief delay to allow transfer to take place */
        uint16_t delay = 0x0FFF;
        while (delay--)
        {
            /* Do nothing until transfer has taken place */
        }

        /* Read data back from the receive buffer
           Note that SPDR provides access to the tx/rx buffers.
           See Hardware Manual Section 29.2.5 */
        g_rx_data = RSPI1.SPDR.LONG;

        /* Transmitted and received data match */
        if (g_tx_data == g_rx_data)
        {
            /* Convert the ADC result into a character string, and store in
               the local string buffer 'lcd_buffer' */
            uint16_to_string(lcd_buffer, 3u, (uint16_t)g_rx_data);

            /* Display the contents of the local string 'lcd_buffer' */
            Display_LCD(LCD_LINE2, lcd_buffer);
        }

        /* Received data does not match with the transmitted data */
        else
        {
            /* Display a 'transfer fail' if data do not match */
            Display_LCD(LCD_LINE2, "SPI Fail");
        }
    }

    /* Clear switch press flags */
    g_switch_flag &= ~SWITCHPRESS_ALL;

}
/*******************************************************************************
* End of function cb_switch_spi
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
    uint8_t  a = 0x00;

    /* Number of bits in 3 hex characters */
    uint8_t  bit_shift = 12u;

    /* Declare 16bit mask variable */
    uint16_t mask = 0xF000u;

    /* Loop through until each hex digit is converted to an ASCII character */
    while (30u > bit_shift)
    {
        /* Mask and shift the hex digit, and store in temporary variable, a */
        a = (uint8_t)((input_number & mask) >> bit_shift);

        /* Convert the hex digit into an ASCII character, and store in output
           string */
        output_string[pos] = (char)((a < 0x0Au) ? (a + 0x30u) : (a + 0x37u));

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

