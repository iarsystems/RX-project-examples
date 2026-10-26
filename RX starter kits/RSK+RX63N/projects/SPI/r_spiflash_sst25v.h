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
* File Name     : r_spiflash_sst25v.h
* Version       : 1.00
* Device        : R5F563NB
* Tool-Chain    : Renesas RX Standard 1.2.0.0
* H/W Platform  : RSK+RX63N
* Description   : Header file for r_spiflash_sst25v.c
*******************************************************************************/
/*******************************************************************************
* History       : 13 Aug. 2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* Macro Definitions
*******************************************************************************/
/* Inclusion macro */
#ifndef R_SPIFLASH_SST25V_H
#define R_SPIFLASH_SST25V_H

/*******************************************************************************
* System Includes
*******************************************************************************/
/* Following header file provides standard integer type definitions. */
#include <stdint.h>
/* Following header file provides standard boolean type definitions. */
#include <stdbool.h>

/*******************************************************************************
* Macro Definitions
*******************************************************************************/
/* SST25V manufacturer's ID */
#define MANUFACTURER_ID_SST25V     (0xBFu)
/* SST25V device ID */
#define DEVICE_ID_SST25V           (0x41u)

/* Device is initialised with memory protection enabled? */
#define MEM_PROTECT_STARTUP_SST25V (true)

/* Command to read data from flash device */
#define CMD_READ_SST25V            (0x03u)
/* Command to read data from flash device at high speed (25MHz to 50MHz) */
#define CMD_READ_HSPEED_SST25V     (0x0Bu)
/* Command to erase 4KByte sector of memory from flash device */
#define CMD_4K_ERASE_SST25V        (0x20u)
/* Command to erase 32KByte block of memory from flash device */
#define CMD_32K_ERASE_SST25V       (0x52u)
/* Command to erase 64KByte block of memory from flash device */
#define CMD_64K_ERASE_SST25V       (0xD8u)
/* Command to erase entire flash memory device */
#define CMD_CHIP_ERASE_SST25V      (0x60u)
/* Command to perform single byte write to flash device */
#define CMD_BYTE_WRITE_SST25V      (0x02u)
/* Command to perform auto address increment writes to flash device */
#define CMD_AAI_WRITE_SST25V       (0xADu)
/* Command to read status register of flash device */
#define CMD_READ_STATUS_SST25V     (0x05u)
/* Command to enable access to the write status register of the flash device */
#define CMD_ENABLE_WSR_SST25V      (0x50u)
/* Command to write to the status register */
#define CMD_WRITE_STATUS_SST25V    (0x01u)
/* Command to enable writes to the flash device (must enable access to register first) */
#define CMD_WRITE_ENABLE_SST25V    (0x06u)
/* Command to disable write access to the flash device */
#define CMD_WRITE_DISABLE_SST25V   (0x04u)
/* Command to read manfacturers id from flash device */
#define CMD_READ_ID_SST25V         (0x90u)
/* Command to read JEDEC id from flash device */
#define CMD_READ_JEDEC_SST25V      (0x9Fu)
/* Command to enable the ready/busy signal output on SI pin */
#define CMD_ENABLE_RYBY_SST25V     (0x70u)
/* Command to disable the ready/busy signal */
#define CMD_DISABLE_RYBY_SST25V    (0x80u)

/* Write busy status 
   1: Write in progress, 0: Device idle */
#define BUSY_STATUSREG_SST25V      (0x01u)
/* Write enable status
   1: Write access enabled, 0: Write access disabled */
#define WEL_STATUSREG_SST25V       (0x02u)
/* Status register protection disabled value */
#define DISABLE_PROTECT_WSR_SST25V (0x00u) 
/* Status reigster protection enabled value */
#define ENABLE_PROTECT_WSR_SST25V  (0x1cu)
/* Status register enable auto address increment value */
#define ENABLE_AAI_WSR_SST25V      (0x40u)
/* Status register disable auto address increment value */
#define DISABLE_AAI_WSR_SST25V     (0x00u)

/* Dummy write value used when supplying clock to receive data */
#define DUMMY_BYTE_SST25V          (0xFFu)
/* NOP (No Operation) command */
#define NOP_BYTE_SST25V            (0x00u)

/* Chip erase time in milliseconds */
#define CHIP_ERASE_TIMEMS_SST25V   (50u)
/* Block erase time in milliseconds */
#define BLOCK_ERASE_TIMEMS_SST25V  (25u)
/* Sector erase time in milliseconds */
#define SECTOR_ERASE_TIMEMS_SST25V (25u)
/* Byte program time in microseconds */
#define BYTE_PROGRAM_TIMEUS_SST25V (10u)

/* Define top flash memory address */
#define TOP_ADDRESS_SST25V         (0x001FFFFFu)
/* Define bottom flash memory address. When this value is zero, the compiler 
   may generate warnings regarding pointless comparison with zero. This should
   be ignored. */
#define BOTTOM_ADDRESS_SST25V      (0x00000000u)
/* Define number of blocks (erase blocks) */
#define NUM_BLOCKS_SST25V         (61u)
/* Define number of bytes in a block */
#define NUM_BLOCK_BYTES_SST25V     (32768u)

/*******************************************************************************
* Global Function Prototypes
*******************************************************************************/
/* Get JEDEC data function prototype */
bool get_jedec_sst25v_spiflash(uint8_t *);
/* Initialise SPI function prototype */
bool initialise_sst25v_spiflash(void);
/* Read device status function prototype */
bool read_status_sst25v_spiflash(uint8_t *);
/* Erase chip function prototype */
bool erase_chip_sst25v_spiflash(void);
/* Erase block function prototype */
bool erase_block_sst25v_spiflash(uint32_t);
/* Write data function prototype */
bool write_sst225v_spiflash(uint8_t *, uint32_t, uint32_t);
/* Read data function prototype */
bool read_sst25v_spiflash(uint8_t *, uint32_t, uint32_t);
/* Enable memory protection function prototype */
bool enable_mem_protect_sst25v_spiflash(bool);
/* Fetch indexed block address function prototype */
bool get_block_addr_sst25v_spiflash(uint16_t, uint32_t *);

/* End of multiple inclusion prevention macro */
#endif
