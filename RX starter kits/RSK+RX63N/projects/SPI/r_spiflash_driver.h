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
*******************************************************************************/
/*******************************************************************************
* History       : 13 Aug. 2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* Macro Definitions
*******************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef SPI_H
#define SPI_H

/*******************************************************************************
* System Includes
*******************************************************************************/
/* Following header file provides standard integer type definitions. */
#include <stdint.h>
/* Following header file provides standard boolean type definitions. */
#include <stdbool.h>

/* SPI flash memory write function prototype */
bool R_SPIFLASH_Write(uint8_t *, uint32_t, uint32_t);
/* SPI flash memory read function prototype */
bool R_SPIFLASH_Read(uint8_t *, uint32_t, uint32_t);
/* SPI flash memory initialise function prototype */
bool R_SPIFLASH_Initialise(void);
/* SPI flash memory erase chip function prototype */
bool R_SPIFLASH_EraseChip(void);
/* SPI flash memory erase block function prototype */
bool R_SPIFLASH_EraseBlock(uint32_t);
/* SPI flash memory protection control function prototype */
bool R_SPIFLASH_MemoryProtection(bool);
/* SPI flash memory JEDEC ID fetch function prototype */
bool R_SPIFLASH_GetJEDEC(uint8_t *);
/* SPI flash memory fetch size function prototype */
bool R_SPIFLASH_GetSize(uint32_t *, uint16_t *);
/* SPI flash memory fetch block address function prototype */
bool R_SPIFLASH_GetBlockAddress(uint16_t, uint32_t *);

/* End of multiple inclusion prevention macro */
#endif
