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
* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_rsk_flash_data.c
* Version      : 1.0
* Device(s)    : R5F571MLCxFC
* Tool-Chain   : IAR Embedded Workbench
* H/W Platform : RSK+RX71M
* Description  : This file contains the flash data functions.
* Creation Date: 08/10/2013
***********************************************************************************************************************/
/***********************************************************************************************************************
Includes <System Includes> , Project Includes
***********************************************************************************************************************/

/* For memcpy() */
#include <string.h>

/* Contains string functions used in this file */
#include "platform.h"
#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"

#include "r_rsk_flash_data.h"

/* Includes Flash Data Library definitions and function prototypes */
#include "r_flash_api_rx_if.h"

/* local low level functions used to erase/read and write flash */
static void flash_error(const flash_err_t status_enu);
static uint8_t erase_block(const uint16_t block);
static uint8_t read_flash(const uint16_t addr, uint8_t * const data, const uint16_t no_bytes);
static uint8_t write_flash(const uint16_t addr, uint8_t * const data, const uint16_t no_bytes);

/***********************************************************************************************************************
Local data declarations
***********************************************************************************************************************/

/* variable tracks the current flash block - erased and ready for writing */
static uint16_t current_block = 0xffff;

/* Flag indicates API open status */
static flash_err_t api_opened = FLASH_ERR_FAILURE;

/* API Functions */
/******************************************************************************
* Function Name : R_FLASH_DataOpen
* Description   : Initialises the Flash system
* Arguments     : none
* Return value  : TRUE if opened ok, otherwise FALSE
******************************************************************************/
uint8_t R_FLASH_DataOpen ()
{
    uint8_t open_ok = FALSE;
    api_opened = R_FLASH_Open();

    if (FLASH_SUCCESS == api_opened)
    {
        open_ok = TRUE;
    }

    return open_ok;
}

/******************************************************************************
* End of function R_FLASH_DataOpen
******************************************************************************/

/******************************************************************************
* Function Name : R_FLASH_DataRead
* Description   : Read data from the FLASH
* Arguments     : uint16_t addr    : address of flash data (0-based)
*                 uint8_t * data   : destination buffer to place retrieved data
*                 uint16_t no_bytes: number of bytes to read
* Return value  : TRUE if READ ok, otherwise FALSE
******************************************************************************/
uint8_t R_FLASH_DataRead (const uint16_t addr, uint8_t * const data, const uint16_t no_bytes)
{
    /* Just a wrapper for read_flash */
    return read_flash(addr, data, no_bytes);
}

/******************************************************************************
* End of function R_FLASH_DataRead
******************************************************************************/

/******************************************************************************
* Function Name : R_FLASH_DataWrite
* Description   : Write data to the FLASH.  This function should not be used in
*                 a random access manner.  Successive writes should increase,
*                 thereby allowing the function to perform the necessary erase
*                 operation before writing.
* Arguments     : uint16_t addr    : address of flash data (0-based)
*                 uint8_t * data   : buffer containing data to write
*                 uint16_t no_bytes: number of bytes to write
* Return value  : TRUE if READ ok, otherwise FALSE
******************************************************************************/
uint8_t R_FLASH_DataWrite (const uint16_t addr, uint8_t * const data, const uint16_t no_bytes)
{
    /* Check to see if this is the first write attempted in this session */
    if (0xffff == current_block)
    {
        /* block needs erasing before write */
        current_block = (uint16_t)(addr / FLASH_DF_BLOCK_SIZE);

        /* Check the address is zero - to see if it the first write to this block */
        if (0u == (addr % FLASH_DF_BLOCK_SIZE))
        {
            if (FALSE == erase_block(current_block))
            {
                return FALSE;
            }
        }

        /* else we assume that the current block was erased in a previous session */

    }
    else if (current_block != (addr / FLASH_DF_BLOCK_SIZE))
    {
       /* we have changed block since last time , need to erase */
        current_block = (uint16_t)(addr / FLASH_DF_BLOCK_SIZE);

        if (FALSE == erase_block(current_block))
        {
            return FALSE;
        }
    }
    else
    {
       /* Nothing to do */
    }

    /* proceed with the write */

    return write_flash(addr, data, no_bytes);
}

/******************************************************************************
* End of function Flash_Write
******************************************************************************/

/***********************************************************************************************************************
* Function Name : R_FLASH_DataErase
* Description   : Erases the whole FLASH
* Arguments     : None
* Return Value  : FALSE if failed, TRUE if ok
***********************************************************************************************************************/
uint8_t R_FLASH_DataErase (void)
{
    uint8_t erase_ok = TRUE;
    uint16_t block;

    for (block = 0; block < FLASH_NO_BLOCKS; block++)
    {
        erase_ok = erase_block(block);
        if (FALSE == erase_ok)
        {
            return erase_ok;
        }
        else if (0 == block)
        {
            current_block = 0;
        }
        else
        {
            /* Nothing to do */
        }
    }

    return erase_ok;
}

/******************************************************************************
* End of function R_FLASH_DataErase
******************************************************************************/


/******************************************************************************
* Function Name : erase_block
* Description   : Erase 1k block of the Data Flash using the FDL.
* Argument      : 0 based block number
* Return value  : none
******************************************************************************/
static uint8_t erase_block (const uint16_t block)
{
    /* Flash Status */
    flash_err_t status;

    /* return code */
    uint8_t     erase_ok = FALSE;

    /* Check for open API */
    if (FLASH_SUCCESS != api_opened)
    {
        flash_error(api_opened);
    }
    else
    {
        status =  R_FLASH_Erase((flash_block_address_t)(FLASH_DF_BLOCK_0 + (block * FLASH_DF_BLOCK_SIZE)), 1);

         /* Check status */
        if (FLASH_SUCCESS != status)
        {
            /* Call the error handler function and stop */
            flash_error(status);
        }
        else
        {
            erase_ok = TRUE;
        }
    }

    return erase_ok;
}

/******************************************************************************
* End of function erase_block
******************************************************************************/

/******************************************************************************
* Function Name : read_flash
* Description   : Read data from the FLASH
* Arguments     : uint16_t addr    : address of flash data (0-based)
*                 uint8_t * data   : destination buffer to place retrieved data
*                 uint16_t no_bytes: number of bytes to read
* Return value  : TRUE if READ ok, otherwise FALSE
******************************************************************************/
static uint8_t read_flash (const uint16_t addr, uint8_t * const data, const uint16_t no_bytes)
{
    /* Since we are just reading from memory mapped area, do a copy */
    memcpy(data, (void *)(FLASH_DF_BLOCK_0 + addr), (uint32_t)no_bytes);

    return TRUE;
}

/******************************************************************************
* End of function read_flash
******************************************************************************/

/******************************************************************************
* Function Name : write_flash
* Description   : Writes to data flash at the specified address.
* Arguments     : uint16_t addr    : address of flash data (0-based)
*                 uint8_t * data   : buffer containing data to write
*                 uint16_t no_bytes: number of bytes to write
* Return value  : TRUE if written ok, otherwise FALSE
******************************************************************************/
static uint8_t write_flash (const uint16_t addr, uint8_t * const data, const uint16_t no_bytes)
{
    /* Flash Status */
    flash_err_t status;

    /* return value */
    uint8_t     write_ok = FALSE;

    /* Check for open API */
    if (FLASH_SUCCESS != api_opened)
    {
        flash_error(api_opened);
    }
    else
    {
        status = R_FLASH_Write((uint32_t)data, (uint32_t)(FLASH_DF_BLOCK_0 + addr), (uint32_t)no_bytes);

        /* Check status */
        if (FLASH_SUCCESS == status)
        {
            write_ok = TRUE;;
        }
        else
        {
            /* Call the error handler function and stop */
            flash_error(status);
        }
    }

    return write_ok;
}

/******************************************************************************
* End of function write_flash
******************************************************************************/

/******************************************************************************
* Function Name : flash_error
* Description   : stub function for error trapping
* Argument      : status_enu error code
* Return value  : none
******************************************************************************/
static void flash_error (const flash_err_t status_enu)
{
    /* trap error here */
    if (FLASH_SUCCESS != status_enu)
    {
        while (1)
        {
            /* trap */
        }
    }
}

/******************************************************************************
* End of function flash_error
******************************************************************************/


