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
/* Copyright (C) _#COPYRIGHTYEAR#_ Renesas Electronics Corporation. All rights reserved.   */
/*******************************************************************************
* File Name     : sci.h
* Version       : _#VERSION#_
* Device        : _#DEVICE#_
* Tool-Chain    : _#TOOLCHAIN#_
* H/W Platform  : _#HWPLATFORM#_
* Description   : This Header file contains the Macro Definitions & prototypes  
*                 for the functions used in r_cg_sci_user.c
*******************************************************************************/
/*******************************************************************************
* History       : _#DATE#_  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
Macro Definitions
*******************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef SCI2_H
#define SCI2_H

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/
/* Defines standard variable types used in this file */
#include <stdint.h>
void startSCI1(void);
void sendSCI1(uint8_t * const tx_buf, uint16_t tx_num);
void SCI1Receive(uint8_t * rx_buf, uint16_t nBytes);
void initSCI1Receive(void);
uint8_t sci1_tx_is_busy(void);

#endif
