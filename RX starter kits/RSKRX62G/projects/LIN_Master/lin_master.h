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
/*******************************************************************************
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.    */ 
/*******************************************************************************
* File Name     : lin_master.h
* Version       : 1.00
* Device        : R5F562GAADFP
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX62G
* Description   : Defines the functions used for LIN operations. Selects the LIN 
*                 operation as either normal master mode or in loopback mode.
*******************************************************************************/
/*******************************************************************************
* History       : 06.09.2012 Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
/* General RPDL function definitions */
#include "r_pdl_definitions.h"


/******************************************************************************
Macro Definitions
******************************************************************************/
/* Multi inclusion prevention macro */
#ifndef LIN_MASTER_H
#define LIN_MASTER_H

/* Performs LIN self-test when the following line is uncommented */
#define LIN_LOOPBACK_MODE

/******************************************************************************
* Global Function Prototypes
******************************************************************************/
/* Init_LIN function protottype */
void Init_LIN(void);
/* CB_ReadStatus callback function protottype */
void CB_ReadStatus(void);
/* CB_FrameTransmit callback function protottype */
void CB_FrameTransmit(void);

/* Only include if LIN_LOOPBACK_MODE has been defined */
#ifdef LIN_LOOPBACK_MODE

/* LIN_Selft_Test function prototype */
void LIN_Self_Test(void);
/* Selft_Test function prototype */
void CB_Self_Test(void);
/* End pre=processor inclusion of LIN test function prototypes */
#endif

/* uint16_ToString function prototype */
void uint16_ToString(uint8_t *, uint8_t, uint16_t);
/* Convert_16BitNumber_ToString function prototype */
void Convert_16BitNumber_ToString(uint16_t);
/* End of multi inclusion prevention macro */
#endif


