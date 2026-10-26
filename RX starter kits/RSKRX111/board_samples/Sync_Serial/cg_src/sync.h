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
/* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.   */
/*******************************************************************************
* File Name     : sync.h
* Version       : 1.00
* Device        : R5F563T6
* Tool-Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX63T
* Description   : Provides declarations for sync SCI functions defined in syn.c
*******************************************************************************/
/*******************************************************************************
* History       : 05.02.2013  Ver. 1.00 First Release
*******************************************************************************/
/*******************************************************************************
* Macro Definitions
*******************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef SYNC_H
#define SYNC_H

/*******************************************************************************
* System Includes
*******************************************************************************/
/* Defines standard integer variable types used in this file */
#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************
User Defines
*******************************************************************************/
#define MSG_LEN                28

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Declaration of the SCI0 transmit buffer */
extern uint8_t g_sci1_tx_buffer[MSG_LEN];

/* Declaration of the SCI12 transmit buffer */
extern uint8_t g_sci12_tx_buffer[MSG_LEN];

/* External declaration of SCI0 receive buffer */
extern uint8_t g_sci1_rx_buffer[MSG_LEN];

/* External declaration of SCI12 receive buffer */
extern uint8_t g_sci12_rx_buffer[MSG_LEN];

/* Declaration of loopback test success flag */
extern volatile bool g_test_pass;


/*******************************************************************************
* Global Function Prototypes
*******************************************************************************/
/* Sync SCI initialisation function prototype declaration */
void Init_Sync(void);

/* Sync SCI0 to SCI12 transfer function prototype declaration */
void SCI1toSCI12Transfer_Sync(void);

/* Sync SCI12 to SCI0 transfer function prototype declaration */
void SCI12toSCI1Transfer_Sync(void);

/* Transfer_End function prototype declaration */
void Transfer_End(void);

/* End of multiple inclusion prevention macro */
#endif

