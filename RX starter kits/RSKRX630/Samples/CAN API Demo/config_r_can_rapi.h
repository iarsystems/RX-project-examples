/* Adapted for use with IAR Embedded Workbench */
/******************************************************************************
* DISCLAIMER
* Please refer to http://www.renesas.com/disclaimer
******************************************************************************
  Copyright (C) 2011. Renesas Electronics Corp., All Rights Reserved.
*******************************************************************************
* File Name    : config_r_can_rapi.h
* Version      : 2.00
* Description  : Edit this file to configure the CAN API.
******************************************************************************
* History       Ver.  Description
* Mar 22 '10  1.00  For RX62N with new CAN API.
* June 07, 2011     Port to RSKRX630.
* Dec 15 2011  2.00     Updated to support CAN extended ID mode.
******************************************************************************/
//Set TAB to 4 spaces!

#ifndef R_CAN_API_CFG_H
#define R_CAN_API_CFG_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "iorx630.h" 

/******************************************************************************
Typedef definitions
******************************************************************************/
/******************************************************************************
Macro definitions
******************************************************************************/
/* Include this line if you want to poll can mailboxes for messages received */
/* and sent. To use the CAN interrupts COMMENT this out.                     */
//#define USE_CAN_POLL      1

/* Level */
#define CAN0_INT_LVL      2
#define CAN1_INT_LVL      2
#define CAN2_INT_LVL      2

/* In the interrupts, USE_CAN_API_SEARCH is default. Uncomment to use the 
mailbox search register. Should be faster if a lot of mailboxes to check. 
This is NOT verified, the opposite may be true. */
#define USE_CAN_API_SEARCH    1

/* Frame ID mode: Standard ID, Extended ID, or Mixed ID mode */
/* Must choose 1 and only 1 from: */
#define FRAME_ID_MODE   STD_ID_MODE
//#define FRAME_ID_MODE   EXT_ID_MODE
//#define FRAME_ID_MODE   MIXED_ID_MODE
 
/*** Board specific ports ******************************************************

Map the transceiver control pins here. Tranceivers vary in the control pins present
and these may need I/O ports assigned to them. This example has "Enable" and
"Standby" control pins on its tranceiver.
If the board has more than one CAN port, then expand these defines as necessary */

/* Configure CAN0 STBn pin. 
Output. High = not standby. */
//#define CAN0_TRX_STB_PORT   ?
//#define CAN0_TRX_STB_PIN    ?
//#define CAN0_TRX_STB_LVL    1   //High = Not standby.

/* Configure CAN0 EN pin. Output. High to enable CAN transceiver. */
//#define CAN0_TRX_ENABLE_PORT    ?
//#define CAN0_TRX_ENABLE_PIN     ?
//#define CAN0_TRX_ENABLE_LVL     1   //High = Enable.

/* Configure CAN1 STBn pin. 
Output. High = not standby. */
#define CAN1_TRX_STB_PORT       4
#define CAN1_TRX_STB_PIN    7
#define CAN1_TRX_STB_LVL    1   //High = Not standby.

/* Configure CAN1 EN pin. Output. High to enable CAN transceiver. */
#define CAN1_TRX_ENABLE_PORT    4
#define CAN1_TRX_ENABLE_PIN     5
#define CAN1_TRX_ENABLE_LVL     1   //High = Enable.

/* Configure CAN2 STBn pin. 
Output. High = not standby. */
//#define CAN2_TRX_STB_PORT    ?
//#define CAN2_TRX_STB_PIN     ?
//#define CAN2_TRX_STB_LVL     1   //High = Not standby.

/* Configure CAN2 EN pin. Output. High to enable CAN transceiver. */
//#define CAN2_TRX_ENABLE_PORT   ?
//#define CAN2_TRX_ENABLE_PIN    ?
//#define CAN2_TRX_ENABLE_LVL    1   //High = Enable.


/*** RX and TX ports **********************************************************
 * The RX630 may have up to 3 CAN channels depending on version.              *
 * Check RX630 Group User Manual for port availability for your device        */

  /* Configure the CAN0 RX and TX pins to be used. */
  //#define CAN0_RX_PORT    P33
  //#define CAN0_RX_PORT    PD2
  //#define CAN0_TX_PORT    P32
  //#define CAN0_TX_PORT    PD1

  /* Configure the CAN1 RX and TX pins to be used. */
  //#define CAN1_RX_PORT    P15
#define CAN1_RX_PORT   P55
  //#define CAN1_TX_PORT    P14
#define CAN1_TX_PORT   P54

  /* Configure the CAN2 RX and TX pins if used. */
  //#define CAN2_RX_PORT    P67
  //#define CAN2_TX_PORT    P66


/*** Baudrate settings ********************************************************
  Calculation of baudrate:
  *********************************
  *  PCLK = 48 MHz = fcan.    *
  *  fcanclk = fcan/prescale    *
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
  #define CAN_BRP    4
  #define CAN_TSEG1  15
  #define CAN_TSEG2  8
  #define CAN_SJW    2
  
  /* 
  Example 2) Selecting prescale to 8.
    Desired baudrate 500 kbps.
  fcanclk = 48000000/8
  fcanclk = 6000000 Hz
  Tqtot = fcanclk/bitrate
  Tqtot = 6000000/500000
  Tqtot = 60/5 = 12.
  Tqtot = TSEG1 + TSEG2 + SS
  Using   TSEG1 = 8 Tq
      TSEG2 = 3 Tq
      SS = 1 Tq always
      SJW should be 1-4 Tq (<=TSEG2). *
  #define CAN_BRP    8
  #define CAN_TSEG1  8
  #define CAN_TSEG2  3
  #define CAN_SJW    1  */
    

/*** Other settings  *********************************************************/
/* Max time to poll a CAN register bit for expected value. Don't set to zero. */
#define MAX_CAN_REG_POLLTIME  4

/******************************************************************************
Constant definitions
*****************************************************************************/

/******************************************************************************
Variable Externs
******************************************************************************/

/******************************************************************************
Function prototypes
******************************************************************************/
#endif  /* R_CAN_API_CFG_H */
/* eof */
