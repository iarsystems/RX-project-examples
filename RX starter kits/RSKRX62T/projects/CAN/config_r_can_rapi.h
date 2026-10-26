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
/*******************************************************************************
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.    */ 
/*******************************************************************************
* File Name    : config_r_can_rapi.h
* Version     : 2.00
* Device     : R5F562TAADFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX62T
* Description   : Edit this file to configure the CAN API. 
*******************************************************************************/
/*******************************************************************************
* History     : 15.10.2010 Ver. 1.00 First Release
              : 21.05.2011 Ver. 2.00 Second Release (Removed RPDL)
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/


#ifndef R_CAN_API_CFG_H
#define R_CAN_API_CFG_H

/******************************************************************************
Macro definitions
******************************************************************************/
/* Include this line if you want to poll can mailboxes for messages received 
and sent. COMMENT to use the CAN interrupts. */
//#define USE_CAN_POLL      1

/*** CAN interrupt ************************************************************/
/* Include this line if you are using CAN0 interrupts. */
#define CAPI_CFG_CAN0_ISR    1

/* Level */
#define CAN0_INT_LVL      2

/* In the interrupts, USE_CAN_API_SEARCH is default. Uncomment to use the 
mailbox search register. Should be faster if a lot of mailboxes to check. 
This is NOT verified, the opposite may be true. */
#define USE_CAN_API_SEARCH    1

/*** Board specific ports ******************************************************
Map the transceiver control pins here. Tranceivers vary in the control pins present
and these may need I/O ports assigned to them. This example has "Enable" and
"Standby" control pins on its tranceiver. */

/* Configure CAN0 STBn pin. 
Output. High = not standby. */
#define CAN0_TRX_STB_PORT       B
#define CAN0_TRX_STB_PIN    0
#define CAN0_TRX_STB_LVL    1   //High = Not standby.

/* Configure CAN0 EN pin. 
Output. High to enable CAN transceiver. */
#define CAN0_TRX_ENABLE_PORT    2
#define CAN0_TRX_ENABLE_PIN     4
#define CAN0_TRX_ENABLE_LVL     1   //High = Enable.

/*** Baudrate settings ********************************************************
  Calculation of baudrate:
  *********************************
  *  PCLK = 50 MHz = fcan.    *
  *  fcanclk = fcan/prescale    *
  *********************************
  
  Example 1) 
  Desired baudrate 500 kbps.
  Selecting prescale to 4.
    fcanclk = 50000000/4
  fcanclk = 12500000 Hz
  Bitrate = fcanclk/Tqtot
   or,
  Tqtot = fcanclk/bitrate
  Tqtot = 12500000/500000
  Tqtot = 125/5 = 25.
    Tqtot = TSEG1 + TSEG2 + SS
  Using TSEG1 = 16 Tq
      TSEG2 = 8 Tq
      SS = 1 Tq always
      Re-synchronization Control (SJW) should be 1-4 Tq (must be <=TSEG2). */
  
  #define CAN_BRP    4
  #define CAN_TSEG1  16     
  #define CAN_TSEG2  8  
  #define CAN_SJW    2
  
  /* 
  Example 2)
    Desired baudrate 500 kbps.   
  Selecting prescale to 5.
  fcanclk = 50000000/5
  fcanclk = 10000000 Hz
  Tqtot = fcanclk/bitrate
  Tqtot = 10000000/500000
  Tqtot = 100/5 = 20.
  Tqtot = TSEG1 + TSEG2 + SS
  Using   TSEG1 = 16 Tq
      TSEG2 = 3 Tq
      SS = 1 Tq always
      SJW should be 1-4 Tq (<=TSEG2). *
  #define CAN_BRP    5
  #define CAN_TSEG1  16  
  #define CAN_TSEG2  3
  #define CAN_SJW    2  */
  
  
  /*   
   *  The range of each segment:  Bit time = 8 Tq to 25 Tq
   *                SS = 1 Tq
   *                TSEG1 = 4 Tq to 16 Tq
   *                TSEG2 = 2 Tq to 8 Tq
   *                SJW = 1 Tq to 4 Tq
   *  Setting of TSEG1 and TSEG2: TSEG1 > TSEG2 >= SJW
  */
    

/*** Other settings  **********************************************************/
/* Max time to poll a CAN register bit for expected value. Don't set to zero. */
#define MAX_CAN_REG_POLLTIME  4

/* R_CAN_API_CFG_H */
#endif  

