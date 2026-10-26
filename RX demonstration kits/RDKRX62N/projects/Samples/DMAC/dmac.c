/******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Electronics Corp. and is
* only intended for use with Renesas products.  

* No other uses are authorized.

* This software is owned by Renesas Electronics Corp. and is 
* protected under the applicable laws, including copyright laws.

* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY,
* INCLUDING BUT NOT LIMITED TO WAWRRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH
* WARRANTIES ARE EXPRESSLY DISCLAIMED.

* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER 
* RENESAS ELECTRONICS CORP. NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR AND DIRECT, INDIRECT, SPECIAL, INCIDENTAL
* OR COSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE,
* EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE
* POSSIBILITIES OF SUCH DAMAGES.

* Renesas reserves the right, without notice, to make changes to this
* software and to discontinue availability of this software.
* By using this software, you agree to the additional terms and 
* conditions found by accessing the following link:
* http://www.renesas.com/disclaimer
*******************************************************************************/
/* Copyright (C) 2010. Renesas Electronics Corp., All Rights Reserved         */
/******************************************************************************
* File Name     : dmac.c
* Version       : 1.1
* Device(s)     : R5F562N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.1
* OS            : None
* H/W Platform  : YRDKRX62N
* Description   : Defines DMAC functions used in this sample.
* Limitations   : None
*******************************************************************************
* History : DD.MMM.YYYY     Description
*         : 08.Oct.2010     First release
*         : 02.Dec.2010     Second YRDK release
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
/* DMAC RPDL function definitions */
#include "r_pdl_dmac.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* Provides declarations of functions defined in this file */
#include "dmac.h"

/*******************************************************************************
* Imported Global Variables
*******************************************************************************/
/* Imported global variable, DMA transfer destination array */
extern uint8_t gDMA_DataBuff[1024];
/* Imported global variable, DMA transfer source variable */
extern uint8_t gDMA_DataSource;

/*******************************************************************************
* Outline       : Init_DMAC
* Description   : This function configures channel 1 for a single block 
*                 software-triggered transfer.
* Argument      : none
* Return value  : none
*******************************************************************************/
void Init_DMAC(void)
{
  /* Declare error flag */
  bool err = true;

  /* Configure the DMAC to perform a single block transfer of 1024 bytes, from
  a single byte repeat address area to an incremented destination. Transfer
  is software-triggered */ 
  err &=  R_DMAC_Create
    (
    1,                   // channel selection
    PDL_DMAC_BLOCK | PDL_DMAC_SOURCE | PDL_DMAC_SOURCE_ADDRESS_FIXED // configuration selection
    | PDL_DMAC_DESTINATION_ADDRESS_PLUS | PDL_DMAC_SIZE_8,
    PDL_DMAC_TRIGGER_SW, // trigger selection
    &gDMA_DataSource,    // source      start address
    &gDMA_DataBuff,      // destination start address
    1,                   // transfer count
    1000,                // repeat or block size (block size must be 1-1023, 0 is invalid)
    PDL_NO_DATA,         // address offset
    PDL_NO_DATA,         // source      address extended repeat area
    PDL_NO_DATA,         // destination address extended repeat area
    PDL_NO_FUNC,         // callback function
    0                    // interrupt priority level
    );

  /* Enable and trigger the DMAC transfer */    
  err &=  R_DMAC_Control
    (
    1,                   // channel selection
    PDL_DMAC_ENABLE | PDL_DMAC_START, // control options
    PDL_NO_PTR,          // source      start address
    PDL_NO_PTR,          // destination start address
    PDL_NO_DATA,         // transfer count
    PDL_NO_DATA,         // repeat or block size
    PDL_NO_DATA,         // address offset
    PDL_NO_DATA,         // source      address extended repeat area
    PDL_NO_DATA          // destination address extended repeat area
    );

  /* Halt in while loop when RPDL errors detected */  
  while (!err);
}
/*******************************************************************************
* End of function Init_DMAC
*******************************************************************************/

