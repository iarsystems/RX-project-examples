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
* File Name     : flash_bgo.h
* Version       : 1.00
* Device        : R5F52206
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX220
* Description   : Provides declarations for functions defined in flash_bgo.c and
*                 main_fla_bgo.c 
******************************************************************************/
/******************************************************************************
* History       : 14.12.2012  Ver. 1.00 First Release
******************************************************************************/

/*******************************************************************************
* Macro Definitions
*******************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef FLASH_BGO_H
#define FLASH_BGO_H

/* Define FLASH start address */
#define D_FLASH_START   0x00100000u

/* Define FLASH size */
#define D_FLASH_SIZE    0x8000u  

/* Define PROG_SIZE (see Flash API Application note for further details) */
#define PROG_SIZE       256u

/* Define BUFF_SIZE (see Flash API Application note for further details) */
#define BUFF_SIZE       256u

/* Define a delay length to read the debug LCD */
#define LCD_MSG_DELAY   0x5FFFFu

/* Read/Write enable mask */
#define FCU_ACCESS_MASK 0xFFFFu

/* Define the data source for the DTC transfer as the ADC result register */ 
#define DTC_SOURCE      (&S12AD.ADDR0)

/*******************************************************************************
* Global Function Prototypes
*******************************************************************************/

/* Initialise flash sample function prototype declaration */
void init_flash_bgo (void);

/* Flash write function prototype declaration */
void write_flash_data (void);

/* Flash erase function prototype declaration */
void erase_flash_data (void);

/* End of multiple inclusion prevention macro */
#endif

