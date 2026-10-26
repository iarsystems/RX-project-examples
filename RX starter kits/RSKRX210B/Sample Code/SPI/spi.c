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
* Device        : R5F5210B
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX210B
* Description   : Defines controlling functions for SPI module       
*******************************************************************************/
/*******************************************************************************
* History       : 14.05.2013  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/
/* IO Port RPDL function definitions */
#include "r_pdl_io_port.h"
/* ADC RPDL function definitions */
#include "r_pdl_adc_12.h"
/* SPI RPDL function definitions */
#include "r_pdl_spi.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* Defines switch functions and variables used in this file */
#include "switch.h"
/* LCD controlling function prototypes & macro definitions */
#include "lcd.h"
/* Provides declarations of functions defined in this file */
#include "spi.h"

/*******************************************************************************
* Function Prototypes
*******************************************************************************/
/* 16 bit integer to character string function prototype declaration */
static void uint16_ToString(uint8_t *, uint8_t, uint16_t);
/* Switch callback function prototype declaration */
void CB_Switch_SPI(void);

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Global variable which stores the value of the AD conversion */
static uint16_t gAdcResult[16];
/* Declare storage variable for received data */
extern volatile uint32_t gMasterRxData;
/* Stores the data to be transmitted */
extern volatile uint32_t gTxData;
/* Switch flag which indicates which switches have been pressed */
extern volatile uint8_t gSwitchFlag;

/*******************************************************************************
* Outline       : Init_SPI
* Description   : This function configures the SPI interface as a master 
*                 channel and sets the transfer to a loop back mode. The 
*                 transfer data is set to 16 bits. The ADC is configured in
*                 single scan mode. The ADC data will be looped back over the 
*                 SPI bus.
* Argument      : none
* Return value  : none
*******************************************************************************/
void Init_SPI(void)
{
    /* Declare error flag */
    bool err = true;
            
    /* Configure the port pin P40 as analog channel input AN000 */
    err &=  R_ADC_12_Set
            (
                PDL_ADC_12_PIN_AN000_P40
            );
    
    /* Configure the 12-bit ADC unit */
    err &=  R_ADC_12_CreateUnit
            (
                0,
                PDL_ADC_12_SCAN_SINGLE,
                PDL_NO_DATA,
                PDL_NO_DATA,
                PDL_NO_DATA,
                PDL_NO_DATA,
                PDL_NO_DATA,
                PDL_NO_FUNC,
                PDL_NO_DATA,
                PDL_NO_FUNC,
                PDL_NO_DATA
            );          

    /* Configure the ADC channel AN000
       Use the sampling time as 5 uS  */
    err &=  R_ADC_12_CreateChannel
            (
                0,
                0,
                PDL_ADC_12_CH_GROUP_A,
                5E-6
            );
    
    /* Configure the master SPI channel  */
    err &=  R_SPI_Create
            (
                0,
                PDL_SPI_MODE_SPI_MASTER,
                PDL_SPI_FRAME_1_1,
                PDL_NO_DATA,
                2E6
            );
        
    /* Enable direct loopback mode */
    err &=  R_SPI_Control
            (
                0,
                PDL_SPI_LOOPBACK_DIRECT,
                PDL_NO_DATA
            );
                
    /* Configure the transfer commands */
    err &=  R_SPI_Command
            (
                0,
                0,
                PDL_SPI_CLOCK_MODE_0 | PDL_SPI_LENGTH_16 | \
                PDL_SPI_LSB_FIRST | PDL_SPI_ASSERT_SSL0,
                PDL_NO_DATA
            );
            
    /* Configure switch callback function */
    SetSwitchReleaseCallback(CB_Switch_SPI);
}
/*******************************************************************************
* End of function Init_SPI
*******************************************************************************/

/*******************************************************************************
* Outline      : CB_Switch_SPI
* Description  : If switch SW3 is pressed an A/D conversion is started and the 
*                result is stored in 'gAdcResult' variable. The result is  
*                shifted out of the transmit shift register before being shifted   
*                into the receive shift register.
* Argument     : none
* Return value : none
*******************************************************************************/
void CB_Switch_SPI(void)
{
    /* Declare error flag */
    bool err = true;

    /* Declare temporary character string */
    uint8_t lcd_buffer[9] = "=H\'WXYZ \0";
                
    /* Check if switch pressed was SW3 */
    if(gSwitchFlag & SWITCHPRESS_3)
    {               
        /* Start the AD conversion  */
        err &= R_ADC_12_Control
            (
                PDL_ADC_12_0_ON
            );

        /* Poll ADC unit until conversion is complete and store result in the
           global variable 'gAdcResult' */  
        err &=  R_ADC_12_Read
            (
                0,
                gAdcResult,
                0
            );  
            
        /* Copy the ADC reading to a transmit buffer */
        gTxData = gAdcResult[0];
        
        /* Transfer all the data once */
        err &=  R_SPI_Transfer
                (
                    0,
                    PDL_NO_DATA,
                    &gTxData,
                    &gMasterRxData, 
                    1,
                    PDL_NO_FUNC,
                    0
                );
        
        /* Transmitted and received data match */
        if(gTxData == gMasterRxData)
        {
            /* Convert the ADC result into a character string, and store in 
               the local string buffer 'lcd_buffer' */
            uint16_ToString(lcd_buffer, 3, (uint16_t)gMasterRxData);

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
    gSwitchFlag &= 0x0F;
    
    /* Halt in while loop when RPDL errors detected */  
    while(!err);        
}
/*******************************************************************************
* End of function CB_Switch_SPI
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
    uint8_t a, bit_shift = 12u;
    
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
