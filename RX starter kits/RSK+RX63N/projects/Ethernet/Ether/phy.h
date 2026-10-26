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
* File Name     : phy.h
* Version       : 1.00
* Device        : R5F563NB
* Tool-Chain    : Renesas RX Standard 1.2.0.0
* H/W Platform  : RSK+RX63N
* Description   : Header file for phy.c
*******************************************************************************/
/*******************************************************************************
* History       : 13 Aug. 2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* Macro Definitions
*******************************************************************************/
#ifndef    PHY_H
#define    PHY_H

/******************************************************************************
* Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdint.h>

/******************************************************************************
Macro definitions
******************************************************************************/
/* Standard PHY Registers */
#define    BASIC_MODE_CONTROL_REG         0       
#define    BASIC_MODE_STATUS_REG         1       
#define    PHY_IDENTIFIER1_REG         2       
#define    PHY_IDENTIFIER2_REG         3       
#define    AN_ADVERTISEMENT_REG         4       
#define    AN_LINK_PARTNER_ABILITY_REG     5       
#define    AN_EXPANSION_REG         6

/* Media Independent Interface */
#define  PHY_ST    1
#define  PHY_READ  2
#define  PHY_WRITE 1
#define  PHY_ADDR  0x1F

#define  MDC_WAIT  2

/* PHY return definitions */
#define R_PHY_OK    0
#define R_PHY_ERROR -1

/* Auto-Negotiation Link Partner Status */
#define PHY_AN_LINK_PARTNER_100BASE    0x0180
#define PHY_AN_LINK_PARTNER_FULL    0x0140

#define PHY_AN_LINK_PARTNER_100FULL    0x0100
#define PHY_AN_LINK_PARTNER_100HALF    0x0080
#define PHY_AN_LINK_PARTNER_10FULL    0x0040
#define PHY_AN_LINK_PARTNER_10HALF    0x0020

/*
 *    Wait counter definitions of PHY-LSI initialization
 *    ICLK = 96MHz
*/
#define PHY_RESET_WAIT                0x00020000L
#define PHY_AUTO_NEGOTIATON_WAIT    0x00007000L

/******************************************************************************
Functions Prototypes
******************************************************************************/
/**
 * External prototypes
 **/
int16_t    phy_init( void );
void    phy_set_100full( void );
void    phy_set_10half( void );
int16_t    phy_set_autonegotiate( void );

#endif /* PHY_H */

