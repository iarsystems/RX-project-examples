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
* File Name    : r_cg_rspi_user.c
* Version      : Applilet4 for RX111 V1.00.00.05 [06 Jun 2013]
* Device(s)    : R5F51115AxFM
* Tool-Chain   : IAR Systems icc78k0r
* Description  : This file implements device driver for RSPI module.
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
#include "r_cg_rspi.h"
/* Start user code for include. Do not edit comment generated here */
#include "r_cg_s12adb.h"
#include "lcd.h"
#include "switch.h"
#include <stddef.h>
#include "r_cg_icu.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
extern uint32_t * gp_rspi0_tx_address;         /* RSPI0 transmit buffer address */
extern uint16_t   g_rspi0_tx_count;            /* RSPI0 transmit data number */
extern uint32_t * gp_rspi0_rx_address;         /* RSPI0 receive buffer address */
extern uint16_t   g_rspi0_rx_count;            /* RSPI0 receive data number */
extern uint16_t   g_rspi0_rx_length;           /* RSPI0 receive data length */
/* Start user code for global. Do not edit comment generated here */

/* The functions in this file are generated using "Application Leading Tool (Applilet)" for e2 studio.
 * Warnings exist for interrupt handlers and callback functions. */

static void uint16_to_string (uint8_t * const output_string, uint8_t pos,
                              const uint16_t input_number);

/* Declare storage variable for received data */
volatile uint32_t g_rx_data = 0;

/* Stores the data to be transmitted */
volatile uint32_t g_tx_data;

/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_rspi0_transmit_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = VECT_RSPI0_SPTI0
#if FAST_INTERRUPT_VECTOR == VECT_RSPI0_SPTI0
__fast_interrupt static void r_rspi0_transmit_interrupt(void)
#else
__interrupt static void r_rspi0_transmit_interrupt(void)
#endif
{
    if (g_rspi0_tx_count > 0U)
    {
        /* Write data for transmission */
        RSPI0.SPDR.WORD.H = (*(uint16_t*)gp_rspi0_tx_address);
        gp_rspi0_tx_address++;
        g_rspi0_tx_count--;
    }
    else
    {
        /* Disable transmit interrupt */
        RSPI0.SPCR.BIT.SPTIE = 0U;

        /* Enable idle interrupt */
        RSPI0.SPCR2.BIT.SPIIE = 1U;
    }
}
/***********************************************************************************************************************
* Function Name: r_rspi0_receive_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = VECT_RSPI0_SPRI0
#if FAST_INTERRUPT_VECTOR == VECT_RSPI0_SPRI0
__fast_interrupt static void r_rspi0_receive_interrupt(void)
#else
__interrupt static void r_rspi0_receive_interrupt(void)
#endif
{
    if (g_rspi0_rx_length > g_rspi0_rx_count)
    {
        *(uint16_t *)gp_rspi0_rx_address = RSPI0.SPDR.WORD.H;
        gp_rspi0_rx_address++;
        g_rspi0_rx_count++;

        if (g_rspi0_rx_length == g_rspi0_rx_count)
        {
            /* Disable receive interrupt */
            RSPI0.SPCR.BIT.SPRIE = 0U;
            r_rspi0_callback_receiveend();
        }
    }
}
/***********************************************************************************************************************
* Function Name: r_rspi0_error_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = VECT_RSPI0_SPEI0
#if FAST_INTERRUPT_VECTOR == VECT_RSPI0_SPEI0
__fast_interrupt static void r_rspi0_error_interrupt(void)
#else
__interrupt static void r_rspi0_error_interrupt(void)
#endif
{
    uint8_t err_type;

    if (1U == RSPI0.SPSR.BIT.MODF)
    {
    }
    /* Disable RSPI function */
    RSPI0.SPCR.BIT.SPE = 0U;

    /* Disable transmit interrupt */
    RSPI0.SPCR.BIT.SPTIE = 0U;

    /* Disable receive interrupt */
    RSPI0.SPCR.BIT.SPRIE = 0U;

    /* Disable error interrupt */
    RSPI0.SPCR.BIT.SPEIE = 0U;

    /* Disable idle interrupt */
    RSPI0.SPCR2.BIT.SPIIE = 0U;

    /* Clear error sources */
    err_type = RSPI0.SPSR.BYTE;
    RSPI0.SPSR.BYTE = 0xA0U;

    if (err_type != 0U)
    {
        r_rspi0_callback_error(err_type);
    }
}
/***********************************************************************************************************************
* Function Name: r_rspi0_idle_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = VECT_RSPI0_SPII0
#if FAST_INTERRUPT_VECTOR == VECT_RSPI0_SPII0
__fast_interrupt static void r_rspi0_idle_interrupt(void)
#else
__interrupt static void r_rspi0_idle_interrupt(void)
#endif
{
    /* Disable RSPI function */
    RSPI0.SPCR.BIT.SPE = 0U;

    /* Disable idle interrupt */
    RSPI0.SPCR2.BIT.SPIIE = 0U;

    r_rspi0_callback_transmitend();
}
/***********************************************************************************************************************
* Function Name: r_rspi0_callback_transmitend
* Description  : This function is a callback function when RSPI0 finishes transmission.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_rspi0_callback_transmitend(void)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_rspi0_callback_receiveend
* Description  : This function is a callback function when RSPI0 finishes reception.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_rspi0_callback_receiveend(void)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_rspi0_callback_error
* Description  : This function is a callback function when RSPI0 error occurs.
* Arguments    : err_type -
*                    error type value
* Return Value : None
***********************************************************************************************************************/
static void r_rspi0_callback_error(uint8_t err_type)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/******************************************************************************
* Function Name : Init_SPI
* Description   : Initialises the SPI functions for this sample
* Argument      : none
* Return value  : none
******************************************************************************/
void Init_SPI (void)
{
    /* enable loopback mode */
    R_RSPI0_LoopBackDirect();

    /* start rSPI */
    R_RSPI0_Start();
}
/******************************************************************************
* End of function Init_SPI
******************************************************************************/

/******************************************************************************
* Function Name : Switch_Pressed
* Description   : Returns value depending on whether switch has been pressed
* Argument      : none
* Return value  : 1 if switch flag has been pressed, 0 otherwise
******************************************************************************/
uint8_t Switch_Pressed (void)
{
	uint8_t sw_pressed = 0;
    if (g_switch_flag)
    {
    	sw_pressed = 1;
    }
    return sw_pressed;
}
/******************************************************************************
* End of function Switch_Pressed
******************************************************************************/

/******************************************************************************
* Function Name : cb_switch_spi
* Description   : Perfoms SPI writes/reads on switch presses
* Argument      : none
* Return value  : none
******************************************************************************/
void cb_switch_spi (void)
{
    uint16_t delay = 0x0FFF;

    /* Declare temporary character string */
    uint8_t  lcd_buffer[9] = "0xxxxx ";

    /* Check if switch pressed was SW3 */
    if (g_switch_flag & SWITCHHOLD_3)
    {
        /* Declare a variable to store the ADC result */
        uint16_t adc_result;

        /*  Run the ADC conversion */
        adc_result = Run_ADC_Single();

        /* Copy the ADC reading to a transmit buffer */
        g_tx_data = adc_result;

        /* Write data to SPI unit */
        R_RSPI0_Send_Receive((uint32_t *)(&g_tx_data), 1, (uint32_t *)(&g_rx_data));

        /* Brief delay to allow transfer to take place */
        while (delay--)
        {
            /* Do nothing until transfer has taken place */
        }

        /* Transmitted and received data match */
        if (g_tx_data == g_rx_data)
        {
            /* Convert the ADC result into a character string, and store in
               the local string buffer 'lcd_buffer' */
            uint16_to_string(lcd_buffer, 2, (uint16_t)g_rx_data);

            /* Display the contents of the local string 'lcd_buffer' */
            Display_LCD(3, lcd_buffer);
        }

        /* Received data does not match with the transmitted data */
        else
        {
            /* Display a 'transfer fail' if data do not match */
            Display_LCD(3, (uint8_t *)"SPI Fail");
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
static void uint16_to_string (uint8_t * const output_string, uint8_t pos,
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
        output_string[pos] = (uint8_t)((a < 0x0Au) ? (a + 0x30u) : (a + 0x37u));

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

/* End user code. Do not edit comment generated here */
