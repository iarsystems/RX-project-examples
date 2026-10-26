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
* File Name     : flash_data.c
* Version       : 1.00
* Device        : R5F562GAADFP
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX62G
* Description   : Defines flash and ADC functions used in this sample.
******************************************************************************/
/******************************************************************************
* History       : 06.09.2012 Ver. 1.00 First Release
******************************************************************************/

/******************************************************************************
* Project Includes
******************************************************************************/
/* Provides standard string function definitions used in this file */
#include <string.h>
/* Defines flash API functions used in this file */
#include "r_Flash_API_RX600.h"

#include "r_flash_api_rx600_private.h"

/* Defines options for customization of the flash API */
#include "r_Flash_API_RX600_UserConfig.h"
/* ADC RPDL function definitions */
#include "r_pdl_adc_10.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* Defines switch functions and variables used in this file */
#include "switch.h"
/* Defines LCD functions used in this file */
#include "lcd.h"
/* Declares prototypes for functions defined in this file */
#include "flash_data.h"

/******************************************************************************
* Global Variables
******************************************************************************/
/* Imported global variable, list of block addresses array */
/* Flash write buffer array */
uint8_t gFlashWriteBuffer[16];
/* Flash write address location global variable */
uint32_t gFlashWriteAddr;

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
/* ADC initialisation function prototype declaration */
void InitADC_FlashData(void);
/* Switch callback function prototype delcaration */
void CB_Switch(void);
/* 16bit integer to character string function prototype delcaration */
void uint16_ToString(uint8_t *, uint8_t, uint16_t);
/* 32bit integer to character string function prototype declaration */
void uint32_ToString(uint8_t *, uint8_t, uint32_t);

/*******************************************************************************
* Outline      : Init_FlashData
* Description  : This function initialises the MCU flash area, allowing it to be
*                read and written to by user code. The function then calls the
*                InitADC_FlashData function to configure the ADC unit used in
*                the project. Finally the function erases the contents of the
*                data flash.
* Argument     : none
* Return value : none
*******************************************************************************/
void Init_FlashData(void)
{
    /* Enable MCU access to the data flash area */
    R_FlashDataAreaAccess(0xFFFF, 0xFFFF);

    /* Initialise the ADC unit */
    InitADC_FlashData();

    /* Erase flash memory before writing to it */
    Erase_FlashData();
}
/******************************************************************************
* End of function Init_FlashData
******************************************************************************/

/*******************************************************************************
* Outline      : Erase_FlashData
* Description  : This function enters a for loop, and erases a block of data
*                flash memory each iteration until all blocks have been erased.
* Argument     : none
* Return value : none
*******************************************************************************/
void Erase_FlashData(void)
{
    /* Declare flash API error flag */
    uint8_t ret = 0;

    /* Declare loop count and block count variables */
    uint8_t loop_counter = 0, block_count = BLOCK_DB0;

    uint32_t address = 0x00000000;

    /* Initialise a for loop to erase each of the data flash blocks */
    for(loop_counter = 0; loop_counter < DF_NUM_BLOCKS; loop_counter++)
    {
        /* Fetch beginning address of DF block */
        address = g_flash_BlockAddresses[BLOCK_DB0 + loop_counter];

        /* Erase data flash block */
        ret |=  R_FlashErase
                (
                    block_count
                );

        /* Check if data flash area is blank */
        ret |=  R_FlashDataAreaBlankCheck
                (
                    address,
                    BLANK_CHECK_ENTIRE_BLOCK
                );

        /* Increment block counter */
        block_count++;
    }

    /* Check Blank Checking */
    ret |= R_FlashDataAreaBlankCheck
            (
            address,
            BLANK_CHECK_ENTIRE_BLOCK
        );

    /* Halt in while loop when flash API errors detected */
    while(ret);
}
/*******************************************************************************
* End of function Erase_FlashData
*******************************************************************************/

/*******************************************************************************
* Outline      : Write_FlashData
* Description  : This function writes the contents of gFlashWriteBuffer to the
*                dataflash, at the location pointed by gFlashWriteAddr. If the
*                number of bytes to write to flash is not a multiple of 8, the
*                data is padded with null bytes (0x00) to make up to the
*                nearest multiple of 8.
* Argument     : none
* Return value : none
*******************************************************************************/
void Write_FlashData(void)
{
    /* Declare flash API error flag */
    uint8_t ret = 0;

    /* Declare data padding array and loop counter */
    uint8_t pad_buffer[256];

    /* Declare pointer to flash write location */
    uint32_t * flash_ptr = (uint32_t *)gFlashWriteAddr;

    /* Declare the number of bytes variable, and initialise with the number of
       bytes the variable gFlashWriteBuffer contains */
    uint16_t num_bytes = sizeof(gFlashWriteBuffer);

    /* Clear the contents of the flash write buffer array */
    memset(pad_buffer, 0x00, 256);

    /* Copy contents of the write buffer to the padding buffer */
    strncpy((char*)pad_buffer, (char*)gFlashWriteBuffer, num_bytes);

    /* Check if number of bytes is greater than 256 */
    if(num_bytes > 256)
    {
        /* Number of bytes to write too high, set error flag to 1 */
        ret |= 1;
    }
    /* Check if number of bytes to write is a multiple of 8 */
    else if(num_bytes % 8u)
    {
        /* Pad the data to write so it makes up to the nearest multiple of 8 */
        num_bytes += 8u - (num_bytes % 8u);
    }

    /* Halt in while loop when flash API errors detected */
    while(ret);

    /* Write contents of write buffer to data flash */
    ret |=  R_FlashWrite
            (
                gFlashWriteAddr,
                (uint32_t)pad_buffer,
                (uint16_t)num_bytes
            );

    /* Compare memory locations to verify written data */
    ret |= memcmp(gFlashWriteBuffer, flash_ptr, (size_t)num_bytes);

    /* Halt in while loop when flash API errors detected */
    while(ret);
}
/*******************************************************************************
* End of function Write_FlashData
*******************************************************************************/

/*******************************************************************************
* Outline      : CB_Switch
* Description  : Switch interrupt callback function. This function first checks
*                which switch has been pressed. If SW1 was pressed, the function
*                triggers an ADC conversion and fetches the result, and writes
*                the value into an incrementing data flash address. The function
*                then displays both the ADC value and the address location it
*                was written to on the debug LCD. If SW3 was pressed, the
*                function erases the flash contents, and resets the write flash
*                write address.
* Argument     : none
* Return value : none
*******************************************************************************/
void CB_Switch(void)
{
    /* Declare error flag */
    bool err = true;

    /* Declare temporary character string */
    uint8_t lcd_buffer[] = "=H\'WXYZ \0";

    /* Declare ADC result container variable */
    uint16_t adc_result = 0x0000;

    /* Check if switch pressed was SW1 */
    if(gSwitchFlag & SWITCHPRESS_1)
    {
        /* Trigger AD conversion */
        err &=  R_ADC_10_Control
                (
                    PDL_ADC_10_0_ON
                );

        /* Poll ADC unit until conversion is complete and store result in the
           global variable usADC_Result */
        err &=  R_ADC_10_Read
                (
                    0,
                    &adc_result
                );

        /* Convert ADC result into a character string, and store in the local
           string lcd_buffer */
        uint16_ToString(lcd_buffer, 3, adc_result);

        /* Clear the contents of the flash write buffer array */
        memset(gFlashWriteBuffer, 0x00, 16);

        /* Copy the contents of the lcd buffer into the flash write buffer
           array */
        strncpy ((char*)gFlashWriteBuffer, (char*)lcd_buffer, 8);

        /* Write the contents of gFlashDataBuffer to flash memory, at the
           location specified in the address variable gFlashWriteAddr */
        Write_FlashData();

        /* Display the contents of the local string lcd_buffer */
        DisplayLCD(LCD_LINE2, lcd_buffer);

        /* Convert the 32 bit address location into a 8bit character string */
        uint32_ToString(lcd_buffer, 0, gFlashWriteAddr);

        /* Display the flash write address on the LCD */
        DisplayLCD(LCD_LINE1, lcd_buffer);

        /* Increment the flash write address by 16 bytes */
        gFlashWriteAddr += 0x10;

    }
    /* Check if switch SW3 was pressed */
    else if(gSwitchFlag & SWITCHPRESS_3)
    {
        /* Erase the data flash memory */
        Erase_FlashData();

        /* Reset the flash write address to the start of block DB0 */
        gFlashWriteAddr = g_flash_BlockAddresses[BLOCK_DB0];

        /* Display 'Flash Erased' message on the debug LCD */
        DisplayLCD(LCD_LINE1, "Flash   ");
        DisplayLCD(LCD_LINE2, "Erased  ");
    }

    /* Clear switch press flags */
    gSwitchFlag &= 0x0F;

    /* Halt in while loop when RPDL errors detected */
    while(!err);
}
/******************************************************************************
* End of function CB_Switch
******************************************************************************/

/*******************************************************************************
* Outline      : InitADC_FlashData
* Description  : This function configures the ADC unit for single shot operation
*                and sets the switch callback function to CB_Switch.
* Argument     : none
* Return value : none
*******************************************************************************/
void InitADC_FlashData(void)
{
    /* Declare error flag */
    bool err = true;

    /* Configure the ADC unit */
    err &=  R_ADC_10_Create
            (
                0,
                PDL_ADC_10_MODE_SINGLE|PDL_ADC_10_CHANNELS_OPTION_0,
                PDL_ADC_10_TRIGGER_SOFTWARE,
                50E6,
                4E-6,
                PDL_NO_FUNC,
                0
            );

    /* Set the initial flash write address to the start of block DB0 */
    gFlashWriteAddr = g_flash_BlockAddresses[BLOCK_DB0];

    /* Configure switch callback function */
    SetSwitchReleaseCallback(CB_Switch);

    /* Halt in while loop when RPDL errors detected */
    while(!err);
}
/******************************************************************************
* End of function InitADC_FlashData
******************************************************************************/

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
void uint16_ToString(uint8_t * output_string, uint8_t pos,
                     uint16_t input_number)
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
        bit_shift -= (uint8_t)(4u);

        /* Increment the output string location */
        pos++;
    }
}
/******************************************************************************
* End of function uint16_ToString
******************************************************************************/

/******************************************************************************
* Outline      : uint32_ToString
* Description  : Function converts a 32 bit integer into a character string,
*                inserts it into the array via the pointer passed at execution.
* Argument     : * output_string : Pointer to uint8_t array that will hold
*                                  character string.
*                            pos : uint8_t number, element number to begin
*                                  inserting the character string from (offset).
*                   input_number : 32 bit integer to convert into a string.
* Return value : none
* Note         : No input validation is used, so output data can overflow the
*                array passed.
******************************************************************************/
void uint32_ToString(uint8_t * output_string, uint8_t pos,
                     uint32_t input_number)
{
    /* Declare temporary charcter storage variable, and bit_shift variable */
    uint8_t a = 0x00, bit_shift = 28u;

    /* Declare 32bit mask variable */
    uint32_t mask = 0xF0000000;

    /* Loop through until each hex digit is converted to an ASCII character */
    while(bit_shift < 30u)
    {
        /* Mask and shift the hex digit, and store in temporary variable, a */
        a = (uint8_t)((input_number & mask) >> bit_shift);

        /* Convert the hex digit into an ASCII character, and store in output
           string */
        output_string[pos] = (uint8_t)((a < 0x0A) ? (a + 0x30) : (a + 0x37));

        /* Shift the bit mask 4 bits to the right, to convert the next digit */
        mask =(uint32_t)( mask >> 4u);

        /* Decrement the bit_shift counter by 4 (bits in a each digit) */
        bit_shift -= (uint8_t)(4u);

        /* Increment the output string location */
        pos++;
    }
}
/******************************************************************************
* End of function uint32_ToString
******************************************************************************/
