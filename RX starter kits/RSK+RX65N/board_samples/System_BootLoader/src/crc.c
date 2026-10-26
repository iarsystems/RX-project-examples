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
* Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : crc.c
* Version      : 1.00
* Device(s)    : R5F565N9AxFB
* Tool-Chain   : CCRX
* H/W Platform : RSK+RX65N
* Description  : CRC checksum calculations on application flash block
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 30.06.2016 1.00     First Release
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/

#include <string.h>
#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#include "r_cg_crc.h"
#include "code_flash.h"
#include "crc.h"

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
static uint16_t get_crc_from_block (uint8_t* const start, uint8_t* const end, const uint8_t reset);

/***********************************************************************************************************************
* Function Name: is_application_crc_ok
* Description  : This function compares the stored CRC checksum calculation for the application block against a
*                directly calculated one.
* Arguments    : None
* Return Value : TRUE  - CRC checksum for application area is valid
*                FALSE - CRC checksum for application area is not valid
***********************************************************************************************************************/
uint8_t is_application_crc_ok (const uint16_t calculatedChecksum)
{
    uint16_t read_crc;

    /* Calculate the CRC value for the application area */
    calc_app_crc_value();

    read_crc = read_app_crc_value();

    /* Compare the calculated CRC value to the value already stored for the application area */
    if (read_crc == calculatedChecksum)
    {
        return (TRUE);
    }

    return (FALSE);
}

/***********************************************************************************************************************
* End of function is_application_crc_ok
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: read_app_crc_value
* Description  : This function reads the CRC checksum value for the application code
* Arguments    : None
* Return Value : uint16_t stored CRC checksum value
***********************************************************************************************************************/
uint16_t read_app_crc_value (void)
{
    volatile uint32_t* pret_value;

    /* Need to add a second volatile uint16_t variable to return result to
     * prevent a compiler segmentation fault
     */
    volatile uint16_t ret2 = 0;

    pret_value = APP_CRC_CHECKSUM_PTR;
    ret2 = (uint16_t) *pret_value;

    return (ret2);
}

/***********************************************************************************************************************
* End of function read_app_crc_value
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: write_app_crc_value
* Description  : This function writes the CRC checksum value for the application code
* Arguments    : uint16_t value - CRC value to be written
* Return Value : void
***********************************************************************************************************************/
void write_app_crc_value (uint16_t value)
{
    uint8_t val_holder[5];
    memset(val_holder, '\0', 5);
    memcpy(val_holder, &value, 2);

    /* Put 2-byte checksum into 4-byte variable because flash self-prog library */
    /* programs in multiples of 4 bytes */
    flash_block_write((uint32_t) APP_CRC_CHECKSUM_PTR, (char *) val_holder, 4);

    /* Ensure that checksum is written, not just stored in the Flash write buffer */
    flash_flush_buffer();

}

/***********************************************************************************************************************
* End of function write_app_crc_value
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: calc_app_crc_value
* Description  : This function calculates the CRC checksum value for the application code using the hardware CRC
*                functionality.
* Arguments    : None
* Return Value : uint16_t calculated CRC checksum value
***********************************************************************************************************************/
uint16_t calc_app_crc_value (void)
{

    /* Main Application Flash code space */
    return (get_crc_from_block((uint8_t *) (APP_START), (uint8_t *) (APP_END), TRUE));
}

/***********************************************************************************************************************
* End of function calc_app_crc_value
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: get_crc_from_block
* Description  : This function calculates the CRC checksum value for a block of code
* Arguments    : Start - start address for the CRC
*                end - final address for the CRC
*                reset - flag to indicate if the CRC block should be reset (IE: New CRC Calculation)
* Return Value : uint16_t calculated CRC checksum value
***********************************************************************************************************************/
static uint16_t get_crc_from_block (uint8_t * const start, uint8_t * const end, const uint8_t reset)
{
    volatile uint8_t* ptr = start;
    uint32_t crc_value;

    if (TRUE == reset)
    {
        /* Clear output register */
        CRC.CRCCR.BIT.DORCLR = 1U;

        /* Set initial value for data output register */
        CRC.CRCDOR.WORD = 0x0000U;

        R_CRC_Input_Data(0x00);
    }

    while (ptr < end)
    {
         R_CRC_InputDataByte(*ptr++);
    }

    R_CRC_Get_Result(&crc_value);

    return ((uint16_t)crc_value);
}

/***********************************************************************************************************************
* End of function get_crc_from_block
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: is_valid_address
* Description  : checks if address is within application code boundaries
* Arguments    : uint32_t checkAddress - address to be checked
* Return Value : uint8_t TRUE or FALSE
***********************************************************************************************************************/
uint8_t is_valid_address (const uint32_t checkAddress)
{

    /* Test Application Flash Area */
    if ((checkAddress >= APP_START) && (checkAddress <= APP_END))
    {

        /* Valid location */
        return (TRUE);
    }

    /* Not in this area, so in a bad area */
    return (FALSE);
}

/***********************************************************************************************************************
* End of function is_valid_address
***********************************************************************************************************************/
