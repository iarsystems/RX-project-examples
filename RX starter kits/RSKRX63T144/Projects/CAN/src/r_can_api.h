/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_can_api.h
* Version      : 2.00
* Description  : Exports the Renesas CAN API
************************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 22.3.2010  1.00     For RX62N with new CAN API.
*         : 15.4.2010           R_CAN_Control 'Enter Sleep' added.
*                               R_CAN_TxStopMsg added.
*         : 15.06.2011          Added typedefs and defines to support multiple CAN channel selection in RX630
*         : 8.12.2011  2.00     Added support for extended ID frame mode.
***********************************************************************************************************************/

#ifndef _R_CAN_API_H
#define _R_CAN_API_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdint.h>
#include "iorx63t_clr.h"
#include "config_r_can_rapi.h"

/******************************************************************************
Typedef definitions
******************************************************************************/
/* Standard data frame message definition object. */
typedef struct
{
    uint32_t id;
    uint8_t dlc;
    uint8_t data[8];
} can_frame_t;

/* A pointer to the CAN peripheral registers memory map structure. */
typedef volatile struct st_can __sfr * can_st_ptr;

/* A set of pointers to the registers of the I/O ports assigned to CAN pins. */
typedef struct
{
    volatile __sfr unsigned char * p_CAN_Rx_Pin_MPC;
    volatile __sfr unsigned char * p_CAN_Rx_Pin_PMR;
    volatile __sfr unsigned char * p_CAN_Rx_Pin_PDR;
    volatile __sfr unsigned char * p_CAN_Rx_Pin_PIDR;
    volatile uint8_t Rx_Pin_mask;
    volatile __sfr unsigned char * p_CAN_Tx_Pin_MPC;
    volatile __sfr unsigned char * p_CAN_Tx_Pin_PMR;
    volatile __sfr unsigned char * p_CAN_Tx_Pin_PDR;
    volatile __sfr unsigned char * p_CAN_Tx_Pin_PODR;
    volatile uint8_t Tx_Pin_mask;
} CAN_port_map_t;

/******************************************************************************
Definitions Used to make the port pin selection logic work
******************************************************************************/
#define    PE0        E0
#define    PB6        B6
#define    P22        22
#define    PD7        D7
#define    PB5        B5
#define    P23        23


/* CAN1 RX port configuration */
#if (CAN1_RX_PORT) && (CAN1_RX_PORT == PE0)
    #define p_CAN1_RX_PIN_MPC        &MPC.PE0.PFS.BYTE
    #define p_CAN1_RX_PIN_PMR         &PORTE.PMR.BYTE
    #define p_CAN1_RX_PIN_PDR         &PORTE.PDR.BYTE
    #define p_CAN1_RX_PIN_PIDR        &PORTE.PIDR.BYTE
    #define   CAN1_RX_PIN_MASK        0x01 /* bit 0 */
#elif (CAN1_RX_PORT) && (CAN1_RX_PORT == PB6)
    #define p_CAN1_RX_PIN_MPC        &MPC.PB6PFS.BYTE
    #define p_CAN1_RX_PIN_PMR         &PORTB.PMR.BYTE
    #define p_CAN1_RX_PIN_PDR         &PORTB.PDR.BYTE
    #define p_CAN1_RX_PIN_PIDR        &PORTB.PIDR.BYTE
    #define   CAN1_RX_PIN_MASK        0x40 /* bit 6 */
#elif (CAN1_RX_PORT) && (CAN1_RX_PORT == P22)
    #define p_CAN1_RX_PIN_MPC        &MPC.P22PFS.BYTE
    #define p_CAN1_RX_PIN_PMR         &PORT2.PMR.BYTE
    #define p_CAN1_RX_PIN_PDR         &PORT2.PDR.BYTE
    #define p_CAN1_RX_PIN_PIDR        &PORT2.PIDR.BYTE
    #define   CAN1_RX_PIN_MASK        0x04 /* bit 2 */
#else
    #define p_CAN1_RX_PIN_MPC         0
    #define p_CAN1_RX_PIN_PMR         0
    #define p_CAN1_RX_PIN_PDR         0
    #define p_CAN1_RX_PIN_PIDR        0
    #define   CAN1_RX_PIN_MASK        0
#endif
/* CAN1 TX port configuration */
#if (CAN1_TX_PORT) && (CAN1_TX_PORT == PD7)
    #define p_CAN1_TX_PIN_MPC        &MPC.PD7PFS.BYTE
    #define p_CAN1_TX_PIN_PMR         &PORTD.PMR.BYTE
    #define p_CAN1_TX_PIN_PDR        &PORTD.PDR.BYTE
    #define p_CAN1_TX_PIN_PODR        &PORTD.PODR.BYTE
    #define   CAN1_TX_PIN_MASK        0x80 /* bit 7 */
#elif (CAN1_TX_PORT) && (CAN1_TX_PORT == PB5)
    #define p_CAN1_TX_PIN_MPC        &MPC.PB5PFS.BYTE
    #define p_CAN1_TX_PIN_PMR         &PORTB.PMR.BYTE
    #define p_CAN1_TX_PIN_PDR        &PORTB.PDR.BYTE
    #define p_CAN1_TX_PIN_PODR        &PORTB.PODR.BYTE
    #define   CAN1_TX_PIN_MASK        0x20 /* bit 5 */
#elif (CAN1_TX_PORT) && (CAN1_TX_PORT == P23)
    #define p_CAN1_TX_PIN_MPC        &MPC.P23PFS.BYTE
    #define p_CAN1_TX_PIN_PMR         &PORT2.PMR.BYTE
    #define p_CAN1_TX_PIN_PDR        &PORT2.PDR.BYTE
    #define p_CAN1_TX_PIN_PODR        &PORT2.PODR.BYTE
    #define   CAN1_TX_PIN_MASK        0x08 /* bit 3 */
#else
    #define p_CAN1_TX_PIN_MPC         0
    #define p_CAN1_TX_PIN_PMR         0
    #define p_CAN1_TX_PIN_PDR         0
    #define p_CAN1_TX_PIN_PODR        0
    #define   CAN1_TX_PIN_MASK        0
#endif


#define PINFUNC_CAN 0x10    /* Value to set MPC PFS registers for CAN operation. */

/******************************************************************************
Macro definitions
******************************************************************************/
/*** CAN API ACTION TYPES ***/
#define DISABLE                         0
#define ENABLE                          1
/* Periph CAN modes */
#define EXITSLEEP_CANMODE               2
#define ENTERSLEEP_CANMODE              3
#define RESET_CANMODE                   4
#define HALT_CANMODE                    5
#define OPERATE_CANMODE                 6
/* Port mode actions */
#define CANPORT_TEST_LISTEN_ONLY        7
#define CANPORT_TEST_0_EXT_LOOPBACK     8
#define CANPORT_TEST_1_INT_LOOPBACK     9
#define CANPORT_RETURN_TO_NORMAL        10

/* Local sleep mode for CAN module */
#define CAN_NOT_SLEEP   0
#define CAN_SLEEP       1

/* Tranceiver port pin macros.  */
#define CAN_TRX_DDR(x, y)               CAN_TRX_DDR_PREPROC(x, y)
#define CAN_TRX_DDR_PREPROC(x, y)       (PORT ## x .PDR.BIT.B ## y)
#define CAN_TRX_DR(x, y)                CAN_TRX_DR_PREPROC(x, y)
#define CAN_TRX_DR_PREPROC(x, y)        (PORT ## x .PODR.BIT.B ## y)

/*** CAN API return values *********************/
#define     R_CAN_OK                (uint32_t)0x00000000
#define     R_CAN_NOT_OK            (uint32_t)0x00000001
#define     R_CAN_MODULE_STOP_ERR   (uint32_t)0x00000002
#define     R_CAN_MSGLOST           (uint32_t)0x00000004
#define     R_CAN_NO_SENTDATA       (uint32_t)0x00000008
#define     R_CAN_RXPOLL_TMO        (uint32_t)0x00000010
#define     R_CAN_BAD_CH_NR         (uint32_t)0x00000020
#define     R_CAN_SW_BAD_MBX        (uint32_t)0x00000040
#define     R_CAN_BAD_ACTION_TYPE   (uint32_t)0x00000080
/* CAN peripheral timeout reasons. */
#define     R_CAN_SW_WAKEUP_ERR     (uint32_t)0x00000100
#define     R_CAN_SW_SLEEP_ERR      (uint32_t)0x00000200
#define     R_CAN_SW_HALT_ERR       (uint32_t)0x00000400
#define     R_CAN_SW_RST_ERR        (uint32_t)0x00000800
#define     R_CAN_SW_TSRC_ERR       (uint32_t)0x00001000
#define     R_CAN_SW_SET_TX_TMO     (uint32_t)0x00002000
#define     R_CAN_SW_SET_RX_TMO     (uint32_t)0x00004000
#define     R_CAN_SW_ABORT_ERR      (uint32_t)0x00008000
/* CAN STATE CODES */
#define     R_CAN_STATUS_ERROR_ACTIVE   (uint32_t)0x0000001
#define     R_CAN_STATUS_ERROR_PASSIVE  (uint32_t)0x0000002
#define     R_CAN_STATUS_BUSOFF         (uint32_t)0x0000004
/********************************************************/

/* CAN Frame ID modes */
#define     STD_ID_MODE     0
#define     EXT_ID_MODE     1
#define     MIXED_ID_MODE   2

/* Mailbox search modes. */
#define     RX_SEARCH       0
#define     TX_SEARCH       1
#define     MSGLOST_SEARCH  2
#define     CHANNEL_SEARCH  3

/* CAN1 Control Register (CTLR) b9, b8 CANM[1:0] CAN Operating Mode Select. */
#define CAN_OPERATION       0    //CAN operation mode
#define CAN_RESET           1    //CAN reset mode
#define CAN_HALT            2    //CAN halt mode
#define CAN_RESET_FORCE     3    //CAN reset mode (forcible transition)

/* Frame types */
#define DATA_FRAME          0
#define REMOTE_FRAME        1

/* Bit set defines */
#define        MBX_0        0x00000001
#define        MBX_1        0x00000002
#define        MBX_2        0x00000004
#define        MBX_3        0x00000008
#define        MBX_4        0x00000010
#define        MBX_5        0x00000020
#define        MBX_6        0x00000040
#define        MBX_7        0x00000080
#define        MBX_8        0x00000100
#define        MBX_9        0x00000200
#define        MBX_10       0x00000400
#define        MBX_11       0x00000800
#define        MBX_12       0x00001000
#define        MBX_13       0x00002000
#define        MBX_14       0x00004000
#define        MBX_15       0x00008000
#define        MBX_16       0x00010000
#define        MBX_17       0x00020000
#define        MBX_18       0x00040000
#define        MBX_19       0x00080000
#define        MBX_20       0x00100000
#define        MBX_21       0x00200000
#define        MBX_22       0x00400000
#define        MBX_23       0x00800000
#define        MBX_24       0x01000000
#define        MBX_25       0x02000000
#define        MBX_26       0x04000000
#define        MBX_27       0x08000000
#define        MBX_28       0x10000000
#define        MBX_29       0x20000000
#define        MBX_30       0x40000000
#define        MBX_31       0x80000000

/*****************************************************************************
Constant definitions
*****************************************************************************/
/*****************************************************************************
Variable Externs
******************************************************************************/
/*****************************************************************************
Function prototypes
******************************************************************************/
/*****************************************************************
                R X   C A N   A P I
******************************************************************/
/* INITIALIZATION */
uint32_t    R_CAN_Create(const uint32_t ch_nr);
uint32_t    R_CAN_PortSet(const uint32_t ch_nr, const uint32_t action_type);
uint32_t    R_CAN_Control(const uint32_t ch_nr, const uint32_t action_type);
void        R_CAN_SetBitrate(const uint32_t ch_nr);
void        R_CAN_ModuleStopStateCancel(const uint32_t ch_nr);

/* TRANSMIT */
uint32_t    R_CAN_TxSet(const uint32_t ch_nr, const uint32_t mbox_nr, const can_frame_t* frame_p, const uint32_t frame_type);
uint32_t    R_CAN_TxSetXid(const uint32_t ch_nr, const uint32_t mbox_nr, can_frame_t* frame_p, const uint32_t frame_type);
uint32_t    R_CAN_Tx(const uint32_t ch_nr, const uint32_t mbox_nr);
uint32_t    R_CAN_TxCheck(const uint32_t ch_nr, const uint32_t mbox_nr);
uint32_t    R_CAN_TxStopMsg(const uint32_t ch_nr, const uint32_t mbox_nr);

/* RECEIVE */
uint32_t    R_CAN_RxSet(const uint32_t ch_nr, const uint32_t mbox_nr, const uint32_t sid, const uint32_t frame_type);
uint32_t    R_CAN_RxSetXid(const uint32_t ch_nr, const uint32_t mbox_nr, uint32_t xid, const uint32_t frame_type);
uint32_t    R_CAN_RxPoll(const uint32_t ch_nr, const uint32_t mbox_nr);
uint32_t    R_CAN_RxRead(const uint32_t ch_nr, const uint32_t mbox_nr, can_frame_t * const frame_p);
void       R_CAN_RxSetMask(const uint32_t ch_nr, const uint32_t mbox_nr, const uint32_t mask_value);

/* ERRORS */
uint32_t    R_CAN_CheckErr(const uint32_t ch_nr);

#endif    /* R_CAN_API.H */
/* eof */