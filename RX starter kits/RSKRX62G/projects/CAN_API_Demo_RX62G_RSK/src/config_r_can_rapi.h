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
* http://www.renesas.com/disclaimer *
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.    
*******************************************************************************/
/*******************************************************************************
* File Name    : R_CAN_API_CFG.H
* Version      : 1.00
* Description  : Edit this file to configure the CAN API.
******************************************************************************
* History
* Mar 22 '10  REA    For RX62N with new CAN API.
* May 12, 2011 REA    Port to RSKRX62T  
******************************************************************************/
//Set TAB to 4 spaces!

#ifndef R_CAN_API_CFG_H
#define R_CAN_API_CFG_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
/******************************************************************************
Typedef definitions
******************************************************************************/
/******************************************************************************
Macro definitions
******************************************************************************/
/* Set to 0 to use the CAN interrupts. Set to 1 if you want to poll CAN 
mailboxes for messages sent and received. */
#define USE_CAN_POLL            0   //1 or 0


/*** CAN interrupt ************************************************************/
/* Level */
#define CAN0_INT_LVL            2

/* In the interrupts, USE_CAN_API_SEARCH is default. Uncomment to use the 
mailbox search register. Should be faster if a lot of mailboxes to check. 
This is NOT verified, the opposite may be true. */
#define USE_CAN_API_SEARCH       1

/* Want to use FIFO? Set to 1. Otherwise 0. */
#define TEST_FIFO                0
#if TEST_FIFO
    #define TX_FIFO_MBX         24    //Cannot be changed.
#endif

/* Frame ID mode: Standard ID, Extended ID, or Mixed ID mode */
/* Must choose 1 and only 1 from: */
//#define FRAME_ID_MODE   STD_ID_MODE
//#define FRAME_ID_MODE   EXT_ID_MODE
#define FRAME_ID_MODE   MIXED_ID_MODE

/*** Board specific ports ******************************************************
Tell me where you want to map the transceiver control pins.
Some transceivers may have other control pins. You would have to configure this
yourself. */

/* Configure CAN0 STBn pin. 
Output. High = not standby. */
#define CAN0_TRX_STB_PORT       B
#define CAN0_TRX_STB_PIN        0
#define CAN0_TRX_STB_LVL        1   //High = Not standby.

/* Configure CAN0 EN pin. 
Output. High to enable CAN transceiver. */
#define CAN0_TRX_ENABLE_PORT    2
#define CAN0_TRX_ENABLE_PIN     4
#define CAN0_TRX_ENABLE_LVL     1   //High = Enable.

/*** Baudrate settings ********************************************************
    Calculation of baudrate:
    *********************************
    *    PCLK = 48 MHz = fcan.        *
    *    fcanclk = fcan/prescale        *
    *********************************
    
    Example 1) 
    Desired baudrate 500 kbps.
    Selecting prescale to 4.
    fcanclk = 48000000/4
    fcanclk = 12000000 Hz
    Bitrate = fcanclk/Tqtot
     or,
    Tqtot = fcanclk/bitrate
    Tqtot = 12000000/500000
    Tqtot = 120/5 = 24.
    Tqtot = TSEG1 + TSEG2 + SS
    Using TSEG1 = 15 Tq
          TSEG2 = 8 Tq
          SS = 1 Tq always
          Re-synchronization Control (SJW) should be 1-4 Tq (must be <=TSEG2). */
    #define CAN_BRP       4
    #define CAN_TSEG1    16
    #define CAN_TSEG2     8
    #define CAN_SJW       2
    
    /* 
    Example 2) Selecting prescale to 8.
    Desired baudrate 500 kbps.
    fcanclk = 48000000/8
    fcanclk = 6000000 Hz
    Tqtot = fcanclk/bitrate
    Tqtot = 6000000/500000
    Tqtot = 60/5 = 12.
    Tqtot = TSEG1 + TSEG2 + SS
    Using     TSEG1 = 8 Tq
            TSEG2 = 3 Tq
            SS = 1 Tq always
            SJW should be 1-4 Tq (<=TSEG2). *
    #define CAN_BRP        8
    #define CAN_TSEG1      8
    #define CAN_TSEG2      3
    #define CAN_SJW        1    */
    
/*** Other settings  *********************************************************/
/* Max time to poll a CAN register bit for expected value. Don't set to zero. */
#define MAX_CAN_REG_POLLTIME    4

/******************************************************************************
Constant definitions
*****************************************************************************/

/******************************************************************************
Variable Externs
******************************************************************************/

/******************************************************************************
Function prototypes
******************************************************************************/
#endif    /* R_CAN_API_CFG_H */
/* eof */
