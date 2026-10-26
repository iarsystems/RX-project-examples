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
* File Name     : r_spiflash_sst25v.c
* Version       : 1.00
* Device        : R5F563NB
* Tool-Chain    : Renesas RX Standard 1.2.0.0
* H/W Platform  : RSK+RX63N
* Description   : Hardware abstraction layer (HAL) for the SPI flash driver.
*                 This file supports the SST25VF016B, 16Mbit SPI Serial Flash.
*                 This file defines flash device specific functions to access
*                 flash device, and are used by the r_spiflash_driver.c file.
*                 These functions should not be used by application code. 
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
#include "r_spiflash_sst25v.h"
/* Defines low level, MCU dependent SPI protocol functions */
#include "r_spiflash_io.h"

/*******************************************************************************
* Private Global Variables
*******************************************************************************/
/* Private global boolean flag to indicate SPI unit initialisation status */
static volatile bool gIsFlashInit_SST25V   = false;
/* Private global boolean flag to indicate memory protection status */
static volatile bool gMemoryProtect_SST25V = MEM_PROTECT_STARTUP_SST25V;

/*******************************************************************************
* Private Local Function Prototypes
*******************************************************************************/
/* Enable write access local static function prototype */
static bool enable_write_sst25v_spiflash(void);
/* Single byte write local static function prototype */
static bool write_byte_sst25v_spiflash(uint8_t, uint32_t);
/* Page write local static function prototype */
static bool write_page_sst25v_spiflash(uint8_t *, uint32_t, uint32_t);

/*******************************************************************************
* Outline     : initialise_sst25v_spiflash
* Module        : SPI Flash, SST25V Device HAL
* Description   : This function calls the low level SPI unit initialisation 
*                 function, and then fetches the JEDEC data to verify the correct
*                 flash device is connected and operational. This function must
*                 be called prior to calling any other spi flash functions.
* Argument      : None
* Return value  : Function returns true if successful, false if unsuccessful.
*                 Function will fail in the following conditions:
*                   1. SPI unit fails to initialise
*                   2. JEDEC data request fails
*                   3. The manufactuers ID or device ID reported by the flash
*                      device does not match those defined in header file
*                      r_spiflash_sst25v.h
*******************************************************************************/
bool initialise_sst25v_spiflash(void)
{
    /* Declare return value container variable */
    bool ret = true;
    
    /* Intialise the SPI unit (HAL layer) */
    if(!init_io_spiflash())
    {
        /* Initialisation failed, set return value false */
        ret = false;
    }
    /* SPI unit intialised successfully */
    else
    {
        /* Set SPI initialised flag true */
        gIsFlashInit_SST25V = true;
    }
    
    /* Declare local variable to hold flash id */
    uint8_t flash_id[3u] = {0};
    
    /* Retrieve ID from memory device, check if operation failed */
    if(!get_jedec_sst25v_spiflash(flash_id))
    {
        /* Get Flash ID failed, set return value false */
        ret = false;
    }
    
    /* Check to see if id from flash EEPROM differs the driver's
       expected id values */
    if( (MANUFACTURER_ID_SST25V != flash_id[0u]) || \
        (DEVICE_ID_SST25V       != flash_id[2u] ) )
    {
        /* Unexpected device or manufacturer ID, set return value false */
        ret = false;
    }
    
    /* Return from function */
    return ret;
}
/*******************************************************************************
* End of function initialise_sst25v_spiflash
*******************************************************************************/

/*******************************************************************************
* Outline     : get_jedec_sst25v_spiflash
* Module        : SPI Flash, SST25V Device HAL
* Description   : This function requests the JEDEC data from the flash memory
*                 device. If successful, the function stores the 3 data bytes
*                 into the pointer location passed (*id_container). Data format:
*                 Byte 0: [Manufacturers ID]
*                 Byte 1: [Memory Type]
*                 Byte 2: [Memory Capacity]
* Argument      : uint8_t * id_container  - Pointer to 3 byte array to store the
*                                           JEDEC data.
* Return value  : Function returns true if successful, false if unsuccessful.
*                 Function will fail in the following conditions:
*                   1. SPI has not been initialised
*                   2. SPI data transfer fails
*******************************************************************************/
bool get_jedec_sst25v_spiflash(uint8_t * id_container)
{
    /* Check if SPI unit has been initialised */
    if(!gIsFlashInit_SST25V)
    {
        /* SPI unit has not been initialised yet, return false */
        return false;
    }
    
    /* Declare local transmit buffer (command byte, followed by 3
       dummy bytes, then nop byte) */
    uint8_t tx_buffer[5u] = {
                                CMD_READ_JEDEC_SST25V,
                                DUMMY_BYTE_SST25V,
                                DUMMY_BYTE_SST25V,
                                DUMMY_BYTE_SST25V,
                                NOP_BYTE_SST25V
                            };                              
    
    /* Declare local receive buffer to store rx data */
    uint8_t rx_buffer[5] = {0};
    
    /* Declare return value variable */
    bool ret = true;
    
    /* Start SPI transfer, check if it fails */
    if(!rxtx_io_spiflash(tx_buffer, rx_buffer, 4u))
    {
        /* SPI transfer failed, set return value to false */
        ret = false;
    }
    /* If no error was reported, continue to fetch rx data */
    else
    {
        /* Create modifable pointer from the passed pointer id_container */
        uint8_t * id_ptr = id_container;
    
        /* Extract JEDEC data values from rx_data, and store into output
           pointer */
        *id_ptr = rx_buffer[1u];
        id_ptr++;
        *id_ptr = rx_buffer[2u];
        id_ptr++;
        *id_ptr = rx_buffer[3u];
    }

    /* Return from function */
    return ret;
}
/*******************************************************************************
* End of function get_jedec_sst25v_spiflash
*******************************************************************************/

/*******************************************************************************
* Outline     : read_status_sst25v_spiflash
* Module        : SPI Flash, SST25V Device HAL
* Description   : This function attempts to read the flash device's status
*                 register. If successful, it stores it into the location passed
*                 by the pointer (*container). Specific status register bits are
*                 defined in r_spiflash_sst25v.h.
* Argument      : uint8_t * container  - Pointer to byte to store the status
*                                        register value.
* Return value  : Function returns true if successful, false if unsuccessful.
*                 Function will fail in the following conditions:
*                   1. SPI has not been initialised
*                   2. SPI data transfer fails
*******************************************************************************/
bool read_status_sst25v_spiflash(uint8_t * container)
{
    /* Check if SPI unit has been initialised */
    if(!gIsFlashInit_SST25V)
    {
        /* SPI unit has not been initialised yet, return false */
        return false;
    }
    
    /* Declare local transmit buffer. First byte is read status command, second
       is dummy byte to read back status value */
    uint8_t tx_buffer[2u] = { 
                                CMD_READ_STATUS_SST25V,
                                DUMMY_BYTE_SST25V
                            };                              
    
    /* Declare local receive buffer to store rx data */
    uint8_t rx_buffer[2u] = {0};
    
    /* Declare return value variable */
    bool ret = true;
    
    /* Start SPI transfer, check if it fails */
    if(!rxtx_io_spiflash(tx_buffer, rx_buffer, 2u))
    {
        /* SPI transfer failed, set return value to false */
        ret = false;
    }
    /* If no error was reported, continue to fetch rx data */
    else
    {    
        /* Extract flash status register from from rx_data, and store into output pointer */
        *container = rx_buffer[1u];
    }

    /* Return from function */
    return ret;    
}
/*******************************************************************************
* End of function read_status_sst25v_spiflash
*******************************************************************************/

/*******************************************************************************
* Outline     : enable_write_sst25v_spiflash
* Module        : SPI Flash, SST25V Device HAL
* Description   : This function sends the command the flash device to enable 
*                 write access. The function must be called immediately any write
*                 or erase operation.
* Argument      : None
* Return value  : Function returns true if successful, false if unsuccessful.
*                 Function will fail in the following conditions:
*                   1. SPI has not been initialised
*                   2. SPI data transfer fails
*******************************************************************************/
bool enable_write_sst25v_spiflash(void)
{
    /* Check if SPI unit has been initialised */
    if(!gIsFlashInit_SST25V)
    {
        /* SPI unit has not been initialised yet, return false */
        return false;
    }   
    
    /* Declare return value */
    bool ret = true;
    
    /* Declare temporary transmit data buffer. Send command to
       enable write access to the flash memory */
    uint8_t tx_buffer[1u] = {
                               CMD_WRITE_ENABLE_SST25V,
                            };
     
    /* Declare temporary receive data buffer (to be discarded) */
    uint8_t rx_buffer[1u] = {0};
    
    /* Start SPI transfer, check if it fails */
    if(!rxtx_io_spiflash(tx_buffer, rx_buffer, 1u))
    {
        /* SPI transfer failed, set return value to false */
        ret = false;
    }  
        
    /* Return from function */
    return ret;
}
/*******************************************************************************
* End of function enable_write_sst25v_spiflash
*******************************************************************************/

/*******************************************************************************
* Outline     : enable_mem_protect_sst25v_spiflash
* Module        : SPI Flash, SST25V Device HAL
* Description   : This function sends the command enable or disable flash memory
*                 protection, based on the boolean input passed (control).
*                 The function also checks the flash device status afterwards to
*                 verify the setting has been made correctly.
* Argument      : bool control - true:  Enable memory protection
*                              - false  Disable memory protection
* Return value  : Function returns true if successful, false if unsuccessful.
*                 Function will fail in the following conditions:
*                   1. SPI has not been initialised
*                   2. SPI data transfer fails
*                   3. Unable to verify memory protection change was succssful
*******************************************************************************/
bool enable_mem_protect_sst25v_spiflash(bool control)
{
    /* Check if SPI unit has been initialised */
    if(!gIsFlashInit_SST25V)
    {
        /* SPI unit has not been initialised yet, return false */
        return false;
    }    
    
    /* Declare return value variable */
    bool ret = true;
    
    /* Declare temporary transmit data buffer. Send command to
       enable write access to the flash device's status register */
    uint8_t tx_buffer[2u] = {
                                CMD_ENABLE_WSR_SST25V,
                                NOP_BYTE_SST25V
                            };
     
    /* Declare temporary receive data buffer (to be discarded) */
    uint8_t rx_buffer[2u] = {0};
    
    /* Start SPI transfer, check if it fails */
    if(!rxtx_io_spiflash(tx_buffer, rx_buffer, 1u))
    {
        /* SPI transfer failed, set return value to false */
        ret = false;
    }
        
    /* Set tx_buffer to send command to write to flash device's status
       register */
    tx_buffer[0u] = CMD_WRITE_STATUS_SST25V;
    
    /* Check if control input is true (enable protection) */
    if(control)
    {
        /* Enable memory protection */
        tx_buffer[1u] = ENABLE_PROTECT_WSR_SST25V;
    }
    /* Control input is false (disable protection) */
    else
    {    
        /* Disable memory protection */
        tx_buffer[1u] = DISABLE_PROTECT_WSR_SST25V;
    }
                
    /* Start SPI transfer, check if it fails */
    if(!rxtx_io_spiflash(tx_buffer, rx_buffer, 2u))
    {
        /* SPI transfer failed, set return value to false */
        ret = false;
    }
    
    /* Declare temporary variable to hold flash device status */
    uint8_t device_status = 0u;
    
    /* Fetch the flash device status register, to check operation was
       successful. Also check if the read status function failed. */
    if(!read_status_sst25v_spiflash(&device_status))
    {
        /* Failed to retreive flash device status register, set return 
           value false (failure). */
        ret = false;
    }
    
    /* Check if the write enable bit differs from the value of the control
       input (true: enable, false: disable) */
    if((bool)(device_status & ENABLE_PROTECT_WSR_SST25V) != control)
    {
        /* Requested access does not match flash device's status register */
        ret = false;
    }
    /* Enable/disable operation successful, update gWriteEnable_SST25V flag */
    else
    {
        /* Update the write enable flag */
        gMemoryProtect_SST25V = control;
    }
    
    /* Return from function */
    return ret;
}
/*******************************************************************************
* End of function enable_write_sst25v_spiflash
*******************************************************************************/

/*******************************************************************************
* Outline     : erase_chip_sst25v_spiflash
* Module        : SPI Flash, SST25V Device HAL
* Description   : This function sends the command to erase the entire contents 
*                 of the flash memory device. This a blocking function which 
*                 waits the specfied chip erase time before returning. This 
*                 function requires that memory protection is disabled.
* Argument      : none
* Return value  : Function returns true if successful, false if unsuccessful.
*                 Function will fail in the following conditions:
*                   1. SPI has not been initialised
*                   2. SPI data transfer fails
*                   3. Memory protection is enabled
*******************************************************************************/
bool erase_chip_sst25v_spiflash(void)
{
    /* Check if SPI unit has been initialised */
    if(!gIsFlashInit_SST25V)
    {
        /* SPI unit has not been initialised yet, return false */
        return false;
    }  
    
    /* Check if write access if enabled */
    if(gMemoryProtect_SST25V)
    {
        /* Write access is not enabled, return false */
        return false;
    }
    
    /* Enable write access, and check if it fails */
    if(!enable_write_sst25v_spiflash())
    {
        /* Failed enabling write access, return false */
        return false;
    }
    
    /* Declare return value variable */
    bool ret = true;
    
    /* Declare temporary transmit data buffer. Send the command to
       disable write access */
    uint8_t tx_buffer[1u] = {
                                 CMD_CHIP_ERASE_SST25V
                             };
        
    /* Declare temporary receive data buffer (to be discarded) */
    uint8_t rx_buffer[1u] = {0};
        
    /* Start SPI transfer, check if it fails */
    if(!rxtx_io_spiflash(tx_buffer, rx_buffer, 1u))
    {
        /* SPI transfer failed, set return value to false */
        ret = false;
    }  
    /* Erase command was sent successfully */
    else
    {
        /* Wait the for the erase to complete */
        delay_msec_io_spiflash(CHIP_ERASE_TIMEMS_SST25V);
    }
        
    /* Return from function */
    return ret;
}
/*******************************************************************************
* End of function erase_chip_sst25v_spiflash
*******************************************************************************/

/*******************************************************************************
* Outline     : write_byte_sst25v_spiflash
* Module        : SPI Flash, SST25V Device HAL
* Description   : This function attempts to write a single byte to the flash 
*                 memory device. The location must be in the erased state before
*                 writing, otherwise write will report succesful but will be 
*                 ignored by the flash device. This is a blocking function, and
*                 so will not return until the specified flash program time has
*                 elasped.
* Argument      : uint8_t  data    - Data byte to write to flash device
*                 uint32_t address - Flash memory address to write to
* Return value  : Function returns true if successful, false if unsuccessful.
*                 Function will fail in the following conditions:
*                   1. SPI has not been initialised
*                   2. SPI data transfer fails
*                   3. Memory protection is enabled
*                   4. The address passed is not in range
*******************************************************************************/
bool write_byte_sst25v_spiflash( uint8_t data,
                                 uint32_t address
                                )
{
    /* Check if SPI unit has not been initialised */
    if(!gIsFlashInit_SST25V)
    {
        /* SPI unit has not been initialised yet, return false */
        return false;
    }  
    
    /* Check if write access if disabled */
    if(gMemoryProtect_SST25V)
    {
        /* Write access is not enabled, return false */
        return false;
    }
    
    /* Check if address input is outside device address range
       Compiler warning may occur if BOTTOM_ADDRESS_SST25V is zero */
    if( (address > TOP_ADDRESS_SST25V) || (address < BOTTOM_ADDRESS_SST25V) )
    {
        /* Memory address outside range of device, return false (failure) */
        return false;
    }
    
    /* Enable write access, and check if it fails */
    if(!enable_write_sst25v_spiflash())
    {
        /* Failed enabling write access, return false */
        return false;
    }
        
    /* Declare temporary array to hold address bytes */
    uint8_t addr_bytes[3u] = {0};
    
    /* Extract 24bit address from 32bit integer, and split into 3 bytes */
    addr_bytes[2] = (uint8_t)((address & 0x00FF0000u) >> 16u);
    addr_bytes[1] = (uint8_t)((address & 0x0000FF00u) >> 8u);
    addr_bytes[0] = (uint8_t)( address & 0x000000FFu);
        
    /* Declare return value variable */
    bool ret = true;
    
    /* Declare temporary transmit data buffer. Send the byte write command,
       followed by the 24bit memory address (MSB first) as 3 bytes, then the
       data byte.  */
    uint8_t tx_buffer[5u] = {
                                 CMD_BYTE_WRITE_SST25V,
                                 addr_bytes[2u],
                                 addr_bytes[1u],
                                 addr_bytes[0u],
                                 data
                             };
        
    /* Declare temporary receive data buffer (to be discarded) */
    uint8_t rx_buffer[5u] = {0};
        
    /* Start SPI transfer, check if it fails */
    if(!rxtx_io_spiflash(tx_buffer, rx_buffer, 5u))
    {
        /* SPI transfer failed, set return value to false */
        ret = false;
    }  
    
    /* Wait for byte program to complete */
    delay_usec_io_spiflash(BYTE_PROGRAM_TIMEUS_SST25V);
    
    /* Return from function */
    return ret;
}
/*******************************************************************************
* End of function write_byte_sst25v_spiflash
*******************************************************************************/

/*******************************************************************************
* Outline     : read_sst25v_spiflash
* Module        : SPI Flash, SST25V Device HAL
* Description   : This function attempts to read data from flash memory device.
*                 The read will start from the address passed, and read 
*                 subsequent bytes until num_bytes have been read. This is a
*                 blocking function that will no return until the number of
*                 requested bytes has been read.
* Argument      : uint8_t  rx_container - Pointer to array to store read data
*                 uint32_t address      - Flash memory address to read from
*                 uint32_t num_bytes    - Number of bytes to read
* Return value  : Function returns true if successful, false if unsuccessful.
*                 Function will fail in the following conditions:
*                   1. SPI has not been initialised
*                   2. SPI data transfer fails
*                   3. The address passed is not in range
*                   4. The last requested byte is outside address range
*******************************************************************************/
bool read_sst25v_spiflash( uint8_t * rx_container, 
                           uint32_t address, 
                           uint32_t num_bytes
                          )
{
    /* Check if SPI unit has not been initialised */
    if(!gIsFlashInit_SST25V)
    {
        /* SPI unit has not been initialised yet, return false */
        return false;
    }  
    
    /* Check if number of bytes to read is zero */
    if(0 == num_bytes)
    {
        
        /* Minimum number of bytes to read is 1, return false */
        return false;
    }
    
    /* Check if address input is outside device address range, or if the
       address of the last byte to read exceeds top address 
        Compiler warning may occur if BOTTOM_ADDRESS_SST25V is zero */
    if( (address > TOP_ADDRESS_SST25V) || \
        ((address + num_bytes) < BOTTOM_ADDRESS_SST25V) )
    {
        /* Memory address outside range of device, return false (failure) */
        return false;
    }
    
    /* Declare temporary array to hold address bytes */
    uint8_t addr_bytes[3u] = {0};
    
    /* Extract 24bit address from 32bit integer, and split into 3 bytes */
    addr_bytes[2] = (uint8_t)((address & 0x00FF0000u) >> 16u);
    addr_bytes[1] = (uint8_t)((address & 0x0000FF00u) >> 8u);
    addr_bytes[0] = (uint8_t)( address & 0x000000FFu);
        
    /* Declare return value variable */
    bool ret = true;
    
    /* Declare modifiable rx container pointer */
    uint8_t * rx_ptr = rx_container;
        
    /* Declare temporary rx buffer */
    uint8_t rx_buffer = 0;
        
    /* Start a streaming SPI transfer */
    ret = control_stream_io_spiflash(true);
    
    /* Check if starting stream transfer was successful */
    if(ret)    
    {
        /* Send the command byte (discard rx data) */
        rxtx_stream_io_spiflash(&rx_buffer, CMD_READ_SST25V);
        
        /* Send the address bytes (discard rx_data) */
        rxtx_stream_io_spiflash(&rx_buffer, addr_bytes[2u]);
        rxtx_stream_io_spiflash(&rx_buffer, addr_bytes[1u]);
        rxtx_stream_io_spiflash(&rx_buffer, addr_bytes[0u]);
        
        /* Declare counter variable, initialise as num_bytes  */
        uint32_t i = num_bytes;
        
        /* Start loop and iterate through until (num_bytes) have been read */
        while((0u != i) && (ret))
        {
            /* Send dummy byte, and fetch read data */
            ret = rxtx_stream_io_spiflash(rx_ptr, DUMMY_BYTE_SST25V);
                        
            /* Increment pointer to next location */
            rx_ptr++;
            
            /* Decrement loop counter */
            i--;
        }
    
        /* End the streaming SPI transfer */
        control_stream_io_spiflash(false);
    }
        
    /* Return from function */
    return ret;
}
/*******************************************************************************
* End of function read_sst25v_spiflash
*******************************************************************************/

/*******************************************************************************
* Outline     : write_page_sst25v_spiflash
* Module        : SPI Flash, SST25V Device HAL
* Description   : This function attempts to write multiple bytes to the flash
*                 memory device in a stream. The first byte is written to the
*                 address passed, and subsequent bytes are written into the
*                 next location. All write locations must be in the erased 
*                 state before writing, otherwise the write function will 
*                 report as succesful but bytes non-erased locations will be
*                 ignored. The number of bytes to write must be a multiple of
*                 two. This is a blocking function that waits the specified
*                 program time after each pair of bytes is written, and returns
*                 once all bytes are programmed.
* Argument      : uint8_t * tx_data  - Pointer to location to store write data
*                 uint32_t address   - Flash memory address to start read from
*                 uint16_t num_bytes - Number of bytes to read
* Return value  : Function returns true if successful, false if unsuccessful.
*                 Function will fail in the following conditions:
*                   1. SPI has not been initialised
*                   2. SPI data transfer fails
*                   3. The address passed is not in range
*                   4. The last requested byte is outside address range
*                   5. Number of bytes to write is zero, or is an odd number.
*******************************************************************************/
bool write_page_sst25v_spiflash( uint8_t * tx_data,
                                 uint32_t address,
                                 uint32_t num_bytes
                                )
{
    /* Check if SPI unit has not been initialised */
    if(!gIsFlashInit_SST25V)
    {
        /* SPI unit has not been initialised yet, return false */
        return false;
    }  
    
    /* Check if number of bytes to write is zero */
    if(0 == num_bytes)
    {
        /* Minimum number of bytes to write is 1, return false */
        return false;
    }
    
    /* Check if number of bytes is a multiple of 2 */
    if(num_bytes % 2)
    {
        /* Number of bytes must be a multiple of 2 */
        return false;
    }
    
    /* Check if address input is outside device address range, or if the
       address of the last byte to write exceeds top address.
       Compiler warning may occur if BOTTOM_ADDRESS_SST25V is zero */
    if( (address > TOP_ADDRESS_SST25V) || \
        ((address + num_bytes) < BOTTOM_ADDRESS_SST25V) )
    {
        /* Memory address outside range of device, return false (failure) */
        return false;
    }
    
    /* Enable write access, and check if it fails */
    if(!enable_write_sst25v_spiflash())
    {
        /* Failed enabling write access, return false */
        return false;
    }
    
    /* Declare temporary array to hold address bytes */
    uint8_t addr_bytes[3u] = {0};
    
    /* Extract 24bit address from 32bit integer, and split into 3 bytes */
    addr_bytes[2u] = (uint8_t)((address & 0x00FF0000u) >> 16u);
    addr_bytes[1u] = (uint8_t)((address & 0x0000FF00u) >> 8u);
    addr_bytes[0u] = (uint8_t)( address & 0x000000FFu);
        
    /* Declare return value variable */
    bool ret = true;
    
    /* Declare modifiable tx container pointer, set to user passed pointer */
    uint8_t * tx_ptr = tx_data;
        
    /* Declare temporary rx buffer */
    uint8_t rx_buffer[6u] = {0};
    
    /* Declare bytes remaining counter, intialised to num_bytes  */
    uint32_t bytes_remaining = num_bytes;
        
    /* Create temporary tx buffer. Send start AAI write command, followed by
       the 24bit memory address to start writes from (split into 3 bytes) */
    uint8_t tx_buffer[6u] = {
                                 CMD_AAI_WRITE_SST25V,
                                 addr_bytes[2u],
                                 addr_bytes[1u],
                                 addr_bytes[0u],
                                 *tx_ptr,
                                 0
                             };
    /* Increment data pointer, and insert second data byte into tx buffer */
    tx_ptr++;
    tx_buffer[5u] = *tx_ptr;
    tx_ptr++;
    
    /* Decrement remaining bytes counter by 2 */
    bytes_remaining -= 2u;

    /* Start SPI transfer (send enable AAI command), check if it fails */
    if(!rxtx_io_spiflash(tx_buffer, rx_buffer, 6u))
    {
        /* SPI transfer failed, set return value to false */
        ret = false;
    }   
    
    /* Wait for byte program to complete */
    delay_usec_io_spiflash(BYTE_PROGRAM_TIMEUS_SST25V);
    
    /* Check if send AAI command was successful */
    if(ret)    
    {        
        /* Start loop and iterate through until (num_bytes) has been
           written. Data must be written in pairs. */
        while((0u != bytes_remaining) && (ret))
        {
            /* Set tx buffer. Send AAI write command, followed by
               2 data bytes */
            tx_buffer[0u] = CMD_AAI_WRITE_SST25V;
            
            /* Insert data bytes into buffer, increment pointer after each */
            tx_buffer[1u] = *tx_ptr;
            tx_ptr++;
            tx_buffer[2u] = *tx_ptr;
            tx_ptr++;
            
            /* Start SPI transfer (send enable AAI command), check if it fails */
            if(!rxtx_io_spiflash(tx_buffer, rx_buffer, 3u))
            {
                /* SPI transfer failed, set return value to false */
                ret = false;
            }   
    
            /* Wait for byte program to complete */
            delay_usec_io_spiflash(BYTE_PROGRAM_TIMEUS_SST25V);
                                    
            /* Decrement bytes remaining counter by 2 */
            bytes_remaining -= 2u;
        }
        
        /* Set tx buffer to write disable command */
        tx_buffer[0] = CMD_WRITE_DISABLE_SST25V;
        
        /* Send write disable command to end AAI mode, check if it fails */
        if(!rxtx_io_spiflash(tx_buffer, rx_buffer, 1u))
        {
            /* SPI transfer failed, set return value to false */
            ret = false;
        }   
    }
        
    /* Return from function */
    return ret;
}
/*******************************************************************************
* End of function write_page_sst25v_spiflash
*******************************************************************************/

/*******************************************************************************
* Outline     : write_sst25v_spiflash
* Module        : SPI Flash, SST25V Device HAL
* Description   : This is a wrapper function that manages the write operation
*                 for any sized write. If 1 byte is passed, it performs a
*                 single byte write. If an even number of bytes is passed, if
*                 performs a page write. If an odd number of bytes (> 1) is
*                 passed it performs a byte write, then a page write. This is
*                 blocking function, than will return only when all bytes have
*                 been programed.
* Argument      : uint8_t * tx_data  - Pointer to location to store write data
*                 uint32_t address   - Flash memory address to start read from
*                 uint16_t num_bytes - Number of bytes to read
* Return value  : Function returns true if successful, false if unsuccessful.
*                 Function will fail in the following conditions:
*                   1. SPI has not been initialised
*                   2. SPI data transfer fails
*                   3. The address passed is not in range
*                   4. The last requested byte is outside address range
*                   5. Number of bytes passed is zero
*******************************************************************************/
bool write_sst225v_spiflash( uint8_t * data_ptr,
                             uint32_t address,
                             uint32_t num_bytes
                            )
{
    /* Check if SPI unit has not been initialised */
    if(!gIsFlashInit_SST25V)
    {
        /* SPI unit has not been initialised yet, return false */
        return false;
    }  
    
    /* Check if number of bytes to write is zero */
    if(0 == num_bytes)
    {
        /* Minimum number of bytes to write is 1, return false */
        return false;
    }
        
    /* Check if address input is outside device address range, or if the
       address of the last byte to write exceeds top address.
       Compiler warning may occur if BOTTOM_ADDRESS_SST25V is zero */
    if( (address > TOP_ADDRESS_SST25V) || \
        ((address + num_bytes) < BOTTOM_ADDRESS_SST25V) )
    {
        /* Memory address outside range of device, return false (failure) */
        return false;
    }  
    
    /* Declare return value container variable */
    bool ret = true;
    
    /* Check if number of bytes to write is 1 */
    if(1u == num_bytes)
    {
        /* Perform single byte write */
        ret = write_byte_sst25v_spiflash(*data_ptr, address);
    }
    /* Check if number of bytes is greater than 1, and a multiple of 2 */
    else if(0 == (num_bytes % 2))
    {
        /* Peform page write */
        ret = write_page_sst25v_spiflash(data_ptr, address, num_bytes);
    }
    /* Number is greater than 1, and not a multiple of 2 */
    else
    {
        /* Create modifable pointer, and initialise to data ptr */
        uint8_t * tx_ptr = data_ptr;

        /* Perform byte write for first data entry */
        ret = write_byte_sst25v_spiflash(*tx_ptr, address);
        
        /* Increment data pointer */
        tx_ptr++;

        /* Perform page write for remaining bytes to next address location */
        ret = write_page_sst25v_spiflash( tx_ptr, \
                                          (uint32_t)(address + 1u), \
                                          (uint32_t)(num_bytes - 1u)
                                         );
    }
    
    /* Return from function */
    return ret;
}
/*******************************************************************************
* End of function write_sst25v_spiflash
*******************************************************************************/

/*******************************************************************************
* Outline     : erase_block_sst25v_spiflash
* Module        : SPI Flash, SST25V Device HAL
* Description   : This function attemps to erase a memory block located at the
*                 address passed (block_addr). The address passed must be
*                 aligned to a block boundary. The address of each block can be
*                 retrieved using the get_block_addr_sst25v_spiflash function.
* Argument      : uint32_t block_addr -  Address of block to erase
* Return value  : Function returns true if successful, false if unsuccessful.
*                 Function will fail in the following conditions:
*                   1. SPI has not been initialised
*                   2. SPI data transfer fails
*                   3. The address passed is not in range, or is not aligned to
*                      a block boundary.
*******************************************************************************/
bool erase_block_sst25v_spiflash(uint32_t block_addr)
{
    /* Check if SPI unit has not been initialised */
    if(!gIsFlashInit_SST25V)
    {
        /* SPI unit has not been initialised yet, return false */
        return false;
    }  
            
    /* Check if address input is outside device address range
       Compiler warning may occur if BOTTOM_ADDRESS_SST25V is zero */
    if( (block_addr > TOP_ADDRESS_SST25V) || (block_addr < BOTTOM_ADDRESS_SST25V) )
    {
        /* Memory address outside range of device, return false (failure) */
        return false;
    }  
        
    /* Check if the address passed is not aligned to a block boundary
       (remove bottom address offset first) */ 
    if( (block_addr - BOTTOM_ADDRESS_SST25V) % NUM_BLOCK_BYTES_SST25V )
    {
        /* Memory address passed is not aligned to a block boundary,
           return false (failure) */
        return false;
    }
    
    /* Enable write access, and check if it fails */
    if(!enable_write_sst25v_spiflash())
    {
        /* Failed enabling write access, return false */
        return false;
    }
    
    /* Declare return value container variable */
    bool ret = true;
    
    /* Declare array to hold address bytes */
    uint8_t addr_bytes[3] = {0};
    
    /* Extract 24bit address from 32bit integer, and split into 3 bytes.
       Set least significant address byte to 0x0000 (force alignment) */
    addr_bytes[2u] = (uint8_t)((block_addr & 0x00FF0000u) >> 16u);
    addr_bytes[1u] = (uint8_t)((block_addr & 0x0000FF00u) >> 8u);
    addr_bytes[0u] = 0x00;
    
    /* Declare tx buffer, first send erase block command then the 
       block address */    
    uint8_t tx_buffer[4u] = {
                                 CMD_32K_ERASE_SST25V,
                                 addr_bytes[2u],
                                 addr_bytes[1u],
                                 addr_bytes[0u],
                             };
    
    /* Declare rx buffer */
    uint8_t rx_buffer[4u] = {0};
    
    /* Send write disable command to end AAI mode, check if it fails */
    if(!rxtx_io_spiflash(tx_buffer, rx_buffer, 4u))
    {
        /* SPI transfer failed, set return value to false */
        ret = false;
    }   
    
    /* Wait for the erase to complete */
    delay_msec_io_spiflash(BLOCK_ERASE_TIMEMS_SST25V);
    
    /* Return from function */
    return ret;
}
/*******************************************************************************
* End of function erase_block_sst25v_spiflash
*******************************************************************************/

/*******************************************************************************
* Outline     : get_block_addr_sst25v_spiflash
* Module        : SPI Flash, SST25V Device HAL
* Description   : This function fetches the address of the indexed block number
*                 specified. 
* Argument      : uint16_t   block_num - Indexed block number
*                 uint32_t * address   - Pointer to location to store address
* Return value  : Function returns true if successful, false if unsuccessful.
*                 Function will fail in the following conditions:
*                   1. Block number is greater than total number of blocks
*******************************************************************************/
bool get_block_addr_sst25v_spiflash(uint16_t block_num, uint32_t * address)
{
    /* Check if block_num is high than the last block */
    if(block_num > NUM_BLOCKS_SST25V)
    {
        /* Block number higher than the last block, return false (failure) */
        return false;
    }
    
    /* Calculate the block address, and store in the pointer */
    *address = BOTTOM_ADDRESS_SST25V;
    *address *= (block_num * NUM_BLOCK_BYTES_SST25V);
    
    /* Return true (successful) */
    return true;
}
/*******************************************************************************
* End of function get_block_addr_sst25v_spiflash
*******************************************************************************/
