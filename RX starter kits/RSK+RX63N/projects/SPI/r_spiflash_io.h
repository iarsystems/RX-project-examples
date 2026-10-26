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
* File Name     : r_spiflash_io.h
* Version       : 1.00
* Device        : R5F563NB
* Tool-Chain    : Renesas RX Standard 1.2.0.0
* H/W Platform  : RSK+RX63N
* Description   : Header file for r_spiflash_io.c
*******************************************************************************/
/*******************************************************************************
* History       : 13 Aug. 2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* Macro Definitions
*******************************************************************************/
/* Inclusion macro */
#ifndef R_SPIFLASH_IO_H
#define R_SPIFLASH_IO_H

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
/* Length (in milliseconds) to allow SPI unit to become idle before timeout */
#define BUSY_TIMEOUT_MSEC_IO_SPIFLASH        (10u)
/* Data register to write SPI channel TX data to */
#define RX_DATA_REGISTER_IO_SPIFLASH         (RSPI1.SPDR.LONG)
/* Data register to read SPI channel RX data from */
#define TX_DATA_REGISTER_IO_SPIFLASH         (RSPI1.SPDR.LONG)
/* Loop iterations per millisecond timing value */
#define MILLISECOND_TIME_IO_SPIFLASH         (8730u)
/* Loop iterations per microsecond timing value */
#define MICROSECOND_TIME_IO_SPIFLASH         (11u)

/*******************************************************************************
* Global Function Prototypes
*******************************************************************************/
/* Initialise SPI channel function prototype */
bool init_io_spiflash(void);
/* Millisecond delay function prototype */
void delay_msec_io_spiflash(uint32_t);
/* Microsecond delay function prototype */
void delay_usec_io_spiflash(uint32_t);
/* Simple send/receive SPI transaction function prototype */
bool rxtx_io_spiflash(uint8_t *, uint8_t *, uint32_t);
/* Control streaming send/receive SPI transaction function prototype */
bool control_stream_io_spiflash(bool);
/* Transfer byte in a streaming SPI transaction function prototype */
bool rxtx_stream_io_spiflash(uint8_t *, uint8_t);


/* End of multiple inclusion prevention macro */
#endif
