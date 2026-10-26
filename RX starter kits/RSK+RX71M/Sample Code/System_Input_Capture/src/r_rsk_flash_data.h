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
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_rsk_flash_data.h
* Version      : 1.0
* Device(s)    : R5F571MLCxFC
* Tool-Chain   : IAR Embedded Workbench
* Description  : This file contains functions and definitions used in the data_flash.c file.
* Creation Date: 08/10/2013
***********************************************************************************************************************/

#ifndef R_RSK_FLASH_DATA_H
#define R_RSK_FLASH_DATA_H

/***********************************************************************************************************************
Includes <System Includes> , �Project Includes
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro Definitions
***********************************************************************************************************************/
#define FLASH_BLOCK_SIZE (1024)
#define FLASH_NO_BLOCKS     (8)
#define FLASH_SIZE_BYTES (FLASH_BLOCK_SIZE*FLASH_NO_BLOCKS)
/***********************************************************************************************************************
Function Declarations
***********************************************************************************************************************/
/* FLASH Data APIs */
uint8_t R_FLASH_DataOpen(void);
uint8_t R_FLASH_DataRead(const uint16_t addr, uint8_t * const data, const uint16_t no_bytes);
uint8_t R_FLASH_DataWrite(const uint16_t addr, uint8_t * const data, const uint16_t no_bytes);
uint8_t R_FLASH_DataErase(void);

#endif
