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
 * Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * File Name    : code_flash.c
 * Version      : 1.00
 * Device(s)    : R5F51138AxFP
 * Tool-Chain   : IAR Systems ICCRX
 * Description  : This file implements Flash self-programming/erase functions
 * Creation Date: 17/02/2014
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 Includes
 ***********************************************************************************************************************/
#include <string.h>
#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#include "r_rsk_utility.h"
#include "code_flash.h"
#include "crc.h"
#include "r_flash_rx_if.h"   /* Includes flash definitions and function prototypes */

/***********************************************************************************************************************
 Global functions
 ***********************************************************************************************************************/
static uint8_t erase_block (const uint16_t block);
static uint8_t write_flash (const uint16_t block, uint8_t * const data, const uint16_t num_bytes);
static uint8_t write_flash_block (const uint16_t block, uint8_t * data);

/***********************************************************************************************************************
 Local data declarations
 ***********************************************************************************************************************/

/* block_buf holds entire flash block or single-block data to be programmed to FLASH */
/* It is assumed that the Flash block lies on a 4-byte boundary as this is required */
/* by the Flash self-programming library. */
static uint8_t block_buf[FLASH_BLOCK_SIZE];
static uint32_t block_start;
static uint8_t new_block = TRUE;

/* API Functions */

/******************************************************************************
 * Function Name : flash_write_init
 * Description   : Initialise flash API
 * Arguments     : none
 * Return value  : none
 ******************************************************************************/
void flash_write_init (void)
{
    flash_err_t open_result;

    open_result = R_FLASH_Open();

    if (FLASH_SUCCESS != open_result)
    {
        while (1)
        {
            /* Endless loop - flash library failed to open correctly */
        }
    }
}

/******************************************************************************
 * End of function flash_write_init
 ******************************************************************************/

/******************************************************************************
 * Function Name : flash_block_write
 * Description   : Writes data to an address in code flash memory
 *
 *                 No checking of parameters is performed.
 *
 * Arguments     : uint32_t address        : address to be written in flash
 *                 char * data             : buffer containing data to write.
 *                 const uint16_t no_bytes : Number of bytes to be written
 * Return value  : TRUE if written ok, otherwise FALSE
 ******************************************************************************/
uint8_t flash_block_write (const uint32_t address, char * const data, const uint16_t no_bytes)
{
    char * pdata_ptr = data;
    char * pdest_ptr = (char *) &block_buf;
    uint16_t bytes_remaining = no_bytes;
    uint16_t block;
    uint8_t block_write_ok;
    uint32_t addr = address;

    /* loop until all outstanding bytes have been dealt with */
    while (bytes_remaining)
    {
        if (TRUE == new_block)
        {

            /* get start of new block */

            /* Calculate the start address of the block being written to */
            block = (((uint32_t) FLASH_CF_BLOCK_END - addr) / FLASH_BLOCK_SIZE); /* Calculate the block number first */
            block_start = ((uint32_t) FLASH_CF_BLOCK_0 - (block * FLASH_BLOCK_SIZE)); /* Now calculate the address of that block */

            /* Copy the entire flash block into RAM buffer */
            memcpy((char *) block_buf, (char*) block_start, (uint16_t) FLASH_BLOCK_SIZE);

            /* new block is created, so de-assert flag */
            new_block = FALSE;
        }

        /* prepare to write valid bytes from the source to the
         * buffer */
        pdest_ptr = (char *) block_buf;
        pdest_ptr += (addr - block_start);

        /* write as many bytes to buffer as possible */
        /* until block boundary is exceeded or no bytes left */
        while ((((addr >= block_start) && (addr <= ((block_start + FLASH_BLOCK_SIZE) - 1))) && (bytes_remaining > 0)))
        {

            /* copy to location in buffer */
            (*pdest_ptr) = (*pdata_ptr);

            /* increment source and destination pointers */
            pdest_ptr++;
            pdata_ptr++;
            addr++;
            bytes_remaining--;
        }

        /* If bytes_remaining is not zero then there are bytes
         * left over, or the data was not in the current block,
         * so it is now time to program the current block
         * before returning to deal with the remainder
         */
        if (0 == bytes_remaining)
        {

            /* New data completely added to buffer */
            /* No flash programming required at this stage */
            return (TRUE);
        }

        /* There are bytes left over, so they would not fit into
         * current block. Therefore program current block back to
         * Flash at the appropriate location and start new block */
        block_write_ok = write_flash_block((((uint32_t) FLASH_CF_BLOCK_END - block_start) / FLASH_BLOCK_SIZE),
                block_buf);

        /* check for successful programming operation */
        if (TRUE != block_write_ok)
        {
            return (FALSE);
        }

        /* prepare remaining bytes to go into a new buffer */
        new_block = TRUE;
    }
    return (TRUE);
}

/******************************************************************************
 * End of function flash_block_write
 ******************************************************************************/

/******************************************************************************
 * Function Name : flash_flush_buffer
 * Description   : The bytes written to flash via the flash_block_write function may not
 *                 all be written to flash, depending upon whether or not a new
 *                 flash block needs to be written to. If not, they remain in
 *                 the buffer, ready to be written when the current active block
 *                 needs to change. flash_flush_buffer forces the buffer contents
 *                 to be written to flash, at the location held in block_start
 * Arguments     : none
 * Return value  : TRUE if written ok, otherwise FALSE
 ******************************************************************************/
uint8_t flash_flush_buffer (void)
{
    if (TRUE == new_block)
    {
        return (TRUE);
    }
    new_block = TRUE;
    return (write_flash_block((((uint32_t) FLASH_CF_BLOCK_END - block_start) / FLASH_BLOCK_SIZE), block_buf));
}

/******************************************************************************
 * End of function flash_flush_buffer
 ******************************************************************************/

/******************************************************************************
 * Function Name : write_flash_block
 * Description   : Writes to data flash to a complete FLASH block.
 *                 Performs a block erase first.
 *                 Assumes that the block size (on RX113 = 1024Bytes).
 *                 No checking of parameters is performed.
 *
 * Arguments     : uint16_t block    : Block to be written in flash
 *                 uint8_t * data    : buffer containing data to write. Should
 *                                   : be block size in length
 * Return value  : TRUE if written OK, otherwise FALSE
 ******************************************************************************/
static uint8_t write_flash_block (const uint16_t block, uint8_t * data)
{

    uint8_t result = UNDECIDED;

    /* erase block */
    result = erase_block(block);

    if (TRUE == result)
    {
        /* write block to flash area */
        result = write_flash(block, data, FLASH_PROG_BLOCK_SIZE);
    }
    return (result);
}

/******************************************************************************
 * End of function write_flash_block
 ******************************************************************************/

#pragma location = "CODE_IN_RAM"
/******************************************************************************
 * Function Name : write_flash
 * Description   : Writes to data flash at the specified address. Address must be
 *                 a multiple of 4, and a multiple of 4 bytes up to 256 bytes
 *                 maximum can be written.
 *                 Also data must not overlap the flash block boundary.
 *                 Verifies that flash has been written correctly.
 *
 *                 No checking of parameters is performed.
 *
 * Arguments     : uint16_t block    : block to be written in flash
 *                 uint8_t *data   : buffer containing data to write
 *                 uint16_t num_bytes: number of bytes to write )
 * Return value  : TRUE if written successful, otherwise FALSE
 ******************************************************************************/
static uint8_t write_flash (const uint16_t block, uint8_t * const data, const uint16_t num_bytes)
{

    /* return value */
    uint8_t write_ok = TRUE;

    flash_err_t flash_block_write_ok;

    uint32_t flash_address;

    flash_address = ((uint32_t) FLASH_CF_BLOCK_0 - (block * FLASH_BLOCK_SIZE));

    flash_block_write_ok = R_FLASH_Write((uint32_t) data, flash_address, (uint32_t) num_bytes);

    if (FLASH_SUCCESS != flash_block_write_ok)
    {
        /* If there was a problem writing to the flash, set the return flag */
        write_ok = FALSE;
    }

    return (write_ok);
}

/******************************************************************************
 * End of function write_flash
 ******************************************************************************/

#pragma location = "CODE_IN_RAM"
/******************************************************************************
 * Function Name : erase_application_code
 * Description   : Erase the flash area dedicated to the user's application
 * Argument      : none
 * Return value  : TRUE      - erase process successful
 *                 FALSE     - erase process failed
 *                 UNDECIDED - erase process ongoing
 ******************************************************************************/
uint8_t erase_application_code (void)
{
    uint8_t ret_value = TRUE;
    uint16_t block;
    uint16_t num_blocks;

    /* This erase loop requires that APP_START is defined as the first address
     * of a flash block and APP_END is the final address of a flash block
     */

    /* Calculate the number of blocks that need erasing */
    num_blocks = ((APP_END - APP_START) / FLASH_BLOCK_SIZE);

    /* Calculate the start block */
    block = (((uint32_t) FLASH_CF_BLOCK_END - APP_START) / FLASH_BLOCK_SIZE);

    /* Loop while there is blocks to be erased and the last erase was successful */
    for (; (num_blocks > 0) && (TRUE == ret_value); num_blocks--)
    {
        ret_value = erase_block(block);
        block--;
    }

    return (ret_value);
}

/******************************************************************************
 * End of function erase_application_code
 ******************************************************************************/

#pragma location = "CODE_IN_RAM"
/******************************************************************************
 * Function Name : erase_block
 * Description   : Erase 1k block of the Flash using the Flash API.
 * Argument      : 0 based block number
 * Return value  : TRUE - Successful Erase, FALSE - Erase Failure
 ******************************************************************************/
static uint8_t erase_block (const uint16_t block)
{

    uint8_t ret_val = TRUE; /* return value */
    flash_err_t erase_ok; /* Return result from the flash API */
    uint32_t flash_address; /* Address calculated from block */

    flash_address = ((uint32_t) FLASH_CF_BLOCK_0 - (block * FLASH_BLOCK_SIZE));

    erase_ok = R_FLASH_Erase((flash_block_address_t) flash_address, 1);

    /* Check the return and return a false if there was an issue */
    if (FLASH_SUCCESS != erase_ok)
    {
        ret_val = FALSE;
    }

    return (ret_val);
}

/******************************************************************************
 * End of function erase_block
 ******************************************************************************/

