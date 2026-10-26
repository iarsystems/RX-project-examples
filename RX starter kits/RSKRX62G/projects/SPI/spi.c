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
/*******************************************************************************
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.    */
/*******************************************************************************
* File Name     : spi.c
* Version       : 1.00
* Device        : R5F562GAADFP
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX62G
* Description   : Defines SPI functions
******************************************************************************/
/******************************************************************************
* History       : 06.09.2012 Ver. 1.00 First Release
******************************************************************************/

/******************************************************************************
* Project Includes
******************************************************************************/
/* Defines standard string functions used in this file */
#include <string.h>
/* IO Port RPDL function defintions */
#include "r_pdl_io_port.h"
/* CMT RPDL function definitions */
#include "r_pdl_cmt.h"
/* CGC RPDL function definitions */
#include "r_pdl_cgc.h"
/* ADC RPDL function definitions */
#include "r_pdl_adc_10.h"
/* LPC RPDL function definitions */
#include "r_pdl_spi.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* Defines switch functions and variables used in this file */
#include "switch.h"
/* Following header file provides function prototypes for LCD controlling
   functions & macro defines */
#include "lcd.h"
/* Provides declarations of functions defined in this file */
#include "spi.h"

/*******************************************************************************
* Global variables
*******************************************************************************/
/* SPI status register */
uint16_t status_flags=0x0;
/* Declare transfer monitoring flag */
uint8_t slave_transfer_complete;
/* Global variable which stores the value of the last AD conversion */
uint32_t usADC_Result = 0x0;
/* Declare storage variable for received data */
uint32_t master_rx_data=0;
/* Switch flag which indicates which switches have been pressed */
extern volatile uint8_t gSwitchFlag;


/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
/* AD conversion complete callback function prototype declaration */
void CB_ADConversion(void);
/* CB_Switch callback function prototype declaration */
void CB_Switch(void);
/* 16 bit integer to character string function prototype declaration */
void uint16_ToString(uint8_t *, uint8_t, uint16_t);


/******************************************************************************
* Outline       : Init_SPI
* Description   : This function configures the SPI interface as a master
*                 channel and sets the transfer to a loopback mode. The transfer
*                 data is set to 16 bits. The ADC is also configured for oneshot
*                 operations
* Argument      : none
* Return value  : none
******************************************************************************/
void Init_SPI(void)
{
    /* Declare error flag */
    bool err = true;

    /* Configure the master SPI channel using -A pins */
    err &= R_SPI_Create
                    (
                        MASTER_CHANNEL,
                        PDL_SPI_MODE_SPI_MASTER | PDL_SPI_PIN_A,
                        PDL_SPI_FRAME_1_1,
                        PDL_NO_DATA,
                        2E6
                    );

    /* Enable direct loopback mode */
    err &= R_SPI_Control
                    (
                        MASTER_CHANNEL,
                        PDL_SPI_LOOPBACK_DIRECT,
                        PDL_NO_DATA
                    );

    /* Configure the transfer commands */
    err &= R_SPI_Command
                    (
                        MASTER_CHANNEL,
                        0,
                        PDL_SPI_CLOCK_MODE_0 | PDL_SPI_LENGTH_16 | \
                        PDL_SPI_LSB_FIRST | PDL_SPI_ASSERT_SSL0,
                        PDL_NO_DATA
                    );

    /* Configure the ADC unit */
    err &=  R_ADC_10_Create
                    (
                        0,
                        PDL_ADC_10_MODE_SINGLE | PDL_ADC_10_CHANNELS_OPTION_0,
                        PDL_ADC_10_TRIGGER_SOFTWARE,
                        50E6,
                        4E-6,
                        PDL_NO_FUNC,
                        0
                    );

    /* Configure switch callback function */
    SetSwitchReleaseCallback(CB_Switch);
}
/*******************************************************************************
* End of function Init_SPI
*******************************************************************************/

/*******************************************************************************
* Outline      : CB_Switch
* Description  : If switch SW3 is pressed an A/D conversion is started and the
*                result is stored in usADC_Result. The result is shifted out of
*                the shift register before being shifted back into the shift
*                register.
* Argument     : none
* Return value : none
*******************************************************************************/
void CB_Switch(void)
{
    /* Declare error flag */
    bool err = true;

    /* Declare temporary character string */
    uint8_t lcd_buffer[] = "=H\'WXYZ \0";

    /* Check if switch pressed was SW3 */
    if(gSwitchFlag & SWITCHPRESS_3)
    {
        /* Trigger AD conversion */
        err &=  R_ADC_10_Control
                (
                    PDL_ADC_10_0_ON
                );

        /* Poll ADC unit until conversion is complete and store result in the
           global variable usADC_Result */
        uint16_t res;
        err &=  R_ADC_10_Read
                (
                    0,
                    &res
                );
        usADC_Result = res;

        /* Transfer all the data once */
        err &= R_SPI_Transfer
        (
            MASTER_CHANNEL,
            PDL_NO_DATA,
            &usADC_Result,
            &master_rx_data,
            1,
            PDL_NO_FUNC,
            0
        );

        /* If the transmitted and received data match, display on debug LCD */
        if(usADC_Result == master_rx_data)
        {
            /* Convert ADC result into a character string, and store in the local
            string lcd_buffer */
            uint16_ToString(lcd_buffer, 3, (uint16_t)master_rx_data);

            /* Display the contents of the local string lcd_buffer */
            DisplayLCD(LCD_LINE2, lcd_buffer);
        }
        else
        {
            /* Display a 'transfer fail' if data do not match */
            DisplayLCD(LCD_LINE2, "SPI Fail");
        }
    }

    /* Clear switch press flags */
    gSwitchFlag &= 0x0F;

    /* Halt in while loop when RPDL errors detected */
    while(!err);
}
/*******************************************************************************
* End of function CB_Switch
*******************************************************************************/

/*******************************************************************************
* Outline      : uint16_ToString
* Description  : Function converts a 16 bit integer into a character string,
*                inserts it into the array via the pointer passed at execution.
* Argument     : * output_string : Pointer to uint8_t array that will hold
*                                  character string.
*                            pos : uint8_t number, element number to begin
*                                  inserting the character string from (offset).
*                   input_number : 16 bit integer to convert into a string.
* Return value : none
* Note         : No input validation is used, so output data can overflow the
*                array passed.
*******************************************************************************/
void uint16_ToString(uint8_t *output_string, uint8_t pos, uint16_t input_number)
{
    /* Declare temporary character storage variable, and bit_shift variable */
    uint8_t a = 0x00, bit_shift = 12u;

    /* Declare 16bit mask variable */
    uint16_t mask = 0xF000;

    /* Loop through until each hex digit is converted to an ASCII character */
    while(bit_shift < 30u)
    {
        /* Mask and shift the hex digit, and store in temporary variable, a */
        a = (uint8_t)((input_number & mask) >> bit_shift);

        /* Convert the hex digit into an ASCII character, and store in output
           string */
        output_string[pos] = (uint8_t)((a < 0x0A) ? (a + 0x30) : (a + 0x37));

        /* Shift the bit mask 4 bits to the right, to convert the next digit */
        mask = (uint16_t)(mask >> 4u);

        /* Decrement the bit_shift counter by 4 (bits in a each digit) */
        bit_shift -= 4u;

        /* Increment the output string location */
        pos++;
    }
}
/*******************************************************************************
* End of function uint16_ToString
*******************************************************************************/
