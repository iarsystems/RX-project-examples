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
* File Name     : r_ether.h
* Version       : 1.00
* Device        : R5F563NB
* Tool-Chain    : Renesas RX Standard 1.2.0.0
* H/W Platform  : RSK+RX63N
* Description   : Header file for r_ether.c
*******************************************************************************/
/*******************************************************************************
* History       : 13 Aug. 2012  Ver. 1.00 First Release
*******************************************************************************/

#ifndef    R_ETHER_H
#define    R_ETHER_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdint.h>

/******************************************************************************
Typedef definitions
******************************************************************************/
struct Descriptor
{
    uint32_t status;
#if __LIT
    uint16_t    size;
    uint16_t    bufsize;
#else
    uint16_t    bufsize;
    uint16_t    size;
#endif
    int8_t                    *buf_p;
    struct Descriptor        *next;
};


typedef struct Descriptor ethfifo;

/******************************************************************************
Macro definitions
******************************************************************************/
#define    BUFSIZE      1536  /* Must be 32-byte aligned */
#define ENTRY     8     /* Number of RX and TX buffers */


/** 
 * Renesas Ethernet API return defines
 **/
#define R_ETHER_OK          0
#define R_ETHER_ERROR       -1

/*    Ether Interface definitions    */
#define ETH_RMII_MODE    0
#define ETH_MII_MODE    1
/*    Select Ether Interface Mode     */
#define ETH_MODE_SEL    ETH_MII_MODE

#define ETH_OPEN_WAIT   (0x1000000u)

/******************************************************************************
Variable Externs
******************************************************************************/

/******************************************************************************
Functions Prototypes
******************************************************************************/
/** 
 * Renesas Ethernet API prototypes
 **/
int32_t R_Ether_Open(uint32_t ch, uint8_t mac_addr[]);
int32_t R_Ether_Close(uint32_t ch);
int32_t R_Ether_Write(uint32_t ch, void *buf, uint32_t len);
int32_t R_Ether_Read(uint32_t ch, void *buf);


/****************************************************/
/* Ethernet statistic collection data */
struct enet_stats
{
  uint32_t  rx_packets;      /* total packets received    */
  uint32_t  tx_packets;      /* total packets transmitted  */
  uint32_t  rx_errors;       /* bad packets received      */
  uint32_t  tx_errors;       /* packet transmit problems    */
  uint32_t  rx_dropped;      /* no space in buffers      */
  uint32_t  tx_dropped;      /* no space available      */
  uint32_t  multicast;       /* multicast packets received  */
  uint32_t  collisions;

  /* detailed rx_errors: */
  uint32_t  rx_length_errors;
  uint32_t  rx_over_errors;    /* receiver ring buffer overflow  */
  uint32_t  rx_crc_errors;     /* recved pkt with crc error  */
  uint32_t  rx_frame_errors;   /* recv'd frame alignment error  */
  uint32_t  rx_fifo_errors;    /* recv'r fifo overrun      */
  uint32_t  rx_missed_errors;  /* receiver missed packet    */

  /* detailed tx_errors */
  uint32_t  tx_aborted_errors;
  uint32_t  tx_carrier_errors;
  uint32_t  tx_fifo_errors;
  uint32_t  tx_heartbeat_errors;
  uint32_t  tx_window_errors;
};

struct ei_device
{
  const int8_t      *name;
  uint8_t           open;
  uint8_t           Tx_act;
  uint8_t           Rx_act;
  uint8_t           txing;        /* Transmit Active */
  uint8_t           irqlock;      /* EDMAC's interrupt disabled when '1'. */
  uint8_t           dmaing;       /* EDMAC Active */
  ethfifo           *rxcurrent;   /* current receive discripter */
  ethfifo           *txcurrent;   /* current transmit discripter */
  uint8_t           save_irq;     /* Original dev->irq value. */
  struct enet_stats stat;
  uint8_t           mac_addr[6];
};

#endif /* R_ETHER_H */

