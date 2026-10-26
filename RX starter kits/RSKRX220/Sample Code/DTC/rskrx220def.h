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
* File Name     : rskrx220def.h
* Version       : 1.00
* Device        : R5F52206
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX220
* Description   : Defines macros relating to the RX220 user LEDs and switches
*******************************************************************************/
/*******************************************************************************
* History       : 14/12/2012 Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* Macro Definitions
*******************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef RSKRX220_H
#define RSKRX220_H

/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/

/* General Values */
#define LED_ON          (0x0)
#define LED_OFF         (0x1u)
#define SET_BIT_HIGH    (0x1u)
#define SET_BIT_LOW     (0x0)
#define SET_BYTE_HIGH   (0xFF)
#define SET_BYTE_LOW    (0x00)

/* Switch port pins state settings */
#define SW1             PORTD.PIDR.BIT.1
#define SW2             PORTD.PIDR.BIT.3
#define SW3             PORTD.PIDR.BIT.4

/* LED ouptut pin settings */
#define LED0            PORT1.PODR.BIT.B4
#define LED1            PORT1.PODR.BIT.B5
#define LED2            PORT1.PODR.BIT.B6
#define LED3            PORT1.PODR.BIT.B7

/* LED data direction */
#define LED0_PORT_DIR   PORT1.PDR.BIT.B4
#define LED1_PORT_DIR   PORT1.PDR.BIT.B5
#define LED2_PORT_DIR   PORT1.PDR.BIT.B6
#define LED3_PORT_DIR   PORT1.PDR.BIT.B7

/* Register protection setting */
#define PROTECTOFF      0xA503u
#define PROTECTON       0xA500u
#define PFSPROTECTOFF   0x40u
#define PFSPROTECTON    0x80u

/* End of multiple inclusion prevention macro */
#endif

