/* Adapted for use with IAR Embedded Workbench */
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
/* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.   */
/*******************************************************************************
* File Name     : r_spi_flash.c
* Version       : 1.00
* Device        : R5F563NB
* Tool-Chain    : Renesas RX Standard 1.2.0.0
* H/W Platform  : RSK+RX63N
* Description   : Master file for SPI flash driver. Contains all the user
*                 functions to access the SPI flash memory device. 
* Note          : 1. This SPI Flash Driver requires uncontested control of the
*                    SPI channel for correct operation.
*                 2. The read, write and erase functions are all blocking
*                    functions. They will not return until the flash device
*                    operation is complete. This means there is no requirement
*                    to add a wait between successive function calls.
*                 3. If a function returns false, it indicates that at least one
*                    part of the operation was not successful. In the case of
*                    read or write operations, it is possible that partial or
*                    incorrect data is written or read.
*                 4. The write and erase functions require memory protection to
*                    be disabled.
*                 5. All SPI functions require the SPI unit to be initialised
*                    first, otherwise they will return false.
*******************************************************************************/
/*******************************************************************************
* History       : 13 Aug. 2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* System Includes
*******************************************************************************/
/* Following header file provides standard integer type definitions. */
#include <stdint.h>
/* Following header file provides standard boolean type definitions. */
#include <stdbool.h>

/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/
/* Provides delcarations for user driver functions defined in this file */
#include "r_spiflash_driver.h"
/* Defines SPI flash device specific functions used in this file */
#include "r_spiflash_sst25v.h"

/*******************************************************************************
* Outline     : R_SPIFLASH_Write
* Module        : SPI Flash, Driver User Functions
* Description   : This driver function writes the specified data to the SPI
*                 flash device.
* Argument      : uint8_t * tx_ptr  - Pointer to location to store write data
*                 uint32_t address   - Flash memory address to start read from
*                 uint32_t num_bytes - Number of bytes to read
* Return value  : Function returns true if successful, false if unsuccessful.
*                 Function will fail in the following conditions:
*                   1. SPI has not been initialised
*                   2. SPI data transfer fails
*                   3. The address passed is not in range
*                   4. The last requested byte is outside address range
*                   5. Number of bytes passed is zero
*******************************************************************************/
bool R_SPIFLASH_Write( uint8_t * tx_ptr,
                       uint32_t address,
                       uint32_t num_bytes
                      )
{
    /* Call the write function */
    return write_sst225v_spiflash(tx_ptr, address, num_bytes);
}
/*******************************************************************************
* End of function R_SPIFLASH_Write
*******************************************************************************/

/*******************************************************************************
* Outline     : R_SPIFLASH_Read
* Module        : SPI Flash, Driver User Functions
* Description   : This driver function reads from the SPI flash device, the
*                 number of bytes passed at the location specified.
* Argument      : uint8_t * rx_ptr - Pointer to array to store read data
*                 uint32_t address      - Flash memory address to read from
*                 uint32_t num_bytes    - Number of bytes to read
* Return value  : Function returns true if successful, false if unsuccessful.
*                 Function will fail in the following conditions:
*                   1. SPI has not been initialised
*                   2. SPI data transfer fails
*                   3. The address passed is not in range
*                   4. The last requested byte is outside address range
*******************************************************************************/
bool R_SPIFLASH_Read( uint8_t * rx_ptr,
                      uint32_t address,
                      uint32_t num_bytes
                     )
{
    /* Call the read function */
    return read_sst25v_spiflash(rx_ptr, address, num_bytes);
}
/*******************************************************************************
* End of function R_SPIFLASH_Read
*******************************************************************************/

/*******************************************************************************
* Outline     : R_SPIFLASH_Initialise
* Module        : SPI Flash, Driver User Functions
* Description   : This driver function initialises the SPI channel and the flash
*                 device.
* Argument      : None
* Return value  : Function returns true if successful, false if unsuccessful.
*                 Function will fail in the following conditions:
*                   1. SPI unit fails to initialise
*                   2. JEDEC data request fails
*                   3. The manufactuers ID or device ID reported by the flash
*                      device does not match those defined in header file
*                      r_spiflash_sst25v.h
*******************************************************************************/
bool R_SPIFLASH_Initialise(void)
{
    /* Call the initialisation function */
    return initialise_sst25v_spiflash();
}
/*******************************************************************************
* End of function R_SPIFLASH_Initialise
*******************************************************************************/

/*******************************************************************************
* Outline     : R_SPIFLASH_EraseBlock
* Module        : SPI Flash, Driver User Functions
* Description   : This driver function erases a 32KByte block, at the address 
*                 specified. The address passed must be on a block boundary. The
*                 R_SPIFLASH_GetBlockAddress function can be used to fetch the
*                 addresses of the blocks.
* Argument      : uint32_t address -  Address of block to erase
* Return value  : Function returns true if successful, false if unsuccessful.
*                 Function will fail in the following conditions:
*                   1. SPI has not been initialised
*                   2. SPI data transfer fails
*                   3. The address passed is not in range, or is not aligned to
*                      a block boundary.
*******************************************************************************/
bool R_SPIFLASH_EraseBlock(uint32_t address)
{
    /* Call the erase block function */
    return erase_block_sst25v_spiflash(address);
}
/*******************************************************************************
* End of function R_SPIFLASH_EraseBlock
*******************************************************************************/

/*******************************************************************************
* Outline     : R_SPIFLASH_EraseChip
* Module        : SPI Flash, Driver User Functions
* Description   : This driver function erases the entire contents of the flash
*                 memory device.
* Argument      : None
* Return value  : Function returns true if successful, false if unsuccessful.
*                 Function will fail in the following conditions:
*                   1. SPI has not been initialised
*                   2. SPI data transfer fails
*                   3. Memory protection is enabled
*******************************************************************************/
bool R_SPIFLASH_EraseChip(void)
{
    /* Call the erase chip function */
    return erase_chip_sst25v_spiflash();
}
/*******************************************************************************
* End of function R_SPIFLASH_EraseChip
*******************************************************************************/

/*******************************************************************************
* Outline     : R_SPIFLASH_MemoryProtection
* Module        : SPI Flash, Driver User Functions
* Description   : This driver function enables or disables the flash device's
*                 memory protection. Memory protection prevents write and erase
*                 operations.
* Argument      : bool control - true:  Enable memory protection
*                              - false  Disable memory protection
* Return value  : Function returns true if successful, false if unsuccessful.
*                 Function will fail in the following conditions:
*                   1. SPI has not been initialised
*                   2. SPI data transfer fails
*                   3. Unable to verify memory protection change was succssful
*******************************************************************************/
bool R_SPIFLASH_MemoryProtection(bool control) 
{
    /* Call the memory protection control function */
    return enable_mem_protect_sst25v_spiflash(control);
}
/*******************************************************************************
* End of function R_SPIFLASH_MemoryProtection
*******************************************************************************/

/*******************************************************************************
* Outline     : R_SPIFLASH_GetJEDEC
* Module        : SPI Flash, Driver User Functions
* Description   : This driver function requests the JEDEC data from the flash
*                 memory device. If successful, the function stores the 3 data
*                 bytes into the pointer location passed (*id_container).
*                 Data format:
*                 Byte 0: [Manufacturers ID]
*                 Byte 1: [Memory Type]
*                 Byte 2: [Memory Capacity]
* Argument      : uint8_t * rx_ptr  - Pointer to 3 byte array to store the
*                                     JEDEC data.
* Return value  : Function returns true if successful, false if unsuccessful.
*                 Function will fail in the following conditions:
*                   1. SPI has not been initialised
*                   2. SPI data transfer fails
*******************************************************************************/
bool R_SPIFLASH_GetJEDEC(uint8_t * rx_ptr)
{
    /* Call the get JEDEC data function */
    return get_jedec_sst25v_spiflash(rx_ptr);
}
/*******************************************************************************
* End of function R_SPIFLASH_GetJEDEC
*******************************************************************************/

/*******************************************************************************
* Outline     : R_SPIFLASH_GetSize
* Module        : SPI Flash, Driver User Functions
* Description   : This driver function retrieves the size of the flash memory
*                 device in bytes. And the number of 32KB blocks. This function
*                 does not require the SPI unit to be initialised.
* Argument      : uint32_t * size_bytes - Pointer to location to store size
*                 uint16_t   num_blocks - Pointer to location to store number of
*                                         blocks
* Return value  : Function will always return true
*******************************************************************************/
bool R_SPIFLASH_GetSize(uint32_t * size_bytes, uint16_t * num_blocks)
{
    /* Calculate total size of flash memory by subtracting the bottom
       address from the top address. Store value in pointer location */
    *size_bytes = (TOP_ADDRESS_SST25V - BOTTOM_ADDRESS_SST25V);
    
    /* Store number of blocks in pointer location */
    *num_blocks = NUM_BLOCKS_SST25V;
    
    /* Return true */
    return true;
}
/*******************************************************************************
* End of function R_SPIFLASH_GetSize
*******************************************************************************/

/*******************************************************************************
* Outline     : R_SPIFLASH_GetBlockAddress
* Module        : SPI Flash, Driver User Functions
* Description   : This driver function fetches the address of each block
*                 (indexed as block 0 starting at bottom address, and last block
*                  ending at top address).
* Argument      : uint16_t  block_num - Block number
*                 uint32_t * address  - Pointer to location to store address
* Return value  : Function returns true if successful, false if unsuccessful.
*                 Function will fail in the following conditions:
*                   1. Block number is greater than total number of blocks
*******************************************************************************/
bool R_SPIFLASH_GetBlockAddress(uint16_t block_num, uint32_t * address)
{
    /* Call the get block address function */
    return get_block_addr_sst25v_spiflash(block_num, address);
}
/*******************************************************************************
* End of function R_SPIFLASH_GetBlockAddress
*******************************************************************************/
