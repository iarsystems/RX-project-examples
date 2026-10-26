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
* File Name     : rpbrx111.h
* Version       : 1.00
* Device        : R5F51115AxFM
* Tool Chain    : CCRX
* H/W Platform  : RPBRX111
* Description   : Pin assignment definitions for RPBRX111
*******************************************************************************/
/*******************************************************************************
* History       : 12/08/2013  Ver. 1.00 First Release
*******************************************************************************/

#ifndef RPBRX111_H
#define RPBRX111_H

/* LED Output Pins */
#define LED0        PORT0.PODR.BIT.B5
#define LED1        PORT4.PODR.BIT.B1
#define LED2        PORTB.PODR.BIT.B7
#define LED3        PORTB.PODR.BIT.B6

/* LED Port Direction Registers */
#define LED0_PDR    PORT0.PDR.BIT.B5
#define LED1_PDR    PORT4.PDR.BIT.B1
#define LED2_PDR    PORTB.PDR.BIT.B7
#define LED3_PDR    PORTB.PDR.BIT.B6

/* Switch input pins */
#define SW1         PORTB.PIDR.BIT.B1
#define SW2         PORTE.PIDR.BIT.B3

/* IDLE_VCC rail control pin */
#define IDLE_EN    PORTE.PODR.BIT.B4
#define IDLE_EN_PDR PORTE.PDR.BIT.B4

/* USB Host VBUS IC power control pin */
#define VBUS_EN    PORT5.PODR.BIT.B5
#define VBUS_EN_PDR PORT5.PDR.BIT.B5

/* POT_VCC rail control pin */
#define POT_EN     PORT0.PODR.BIT.B3
#define POT_EN_PDR  PORT0.PDR.BIT.B3

/* Supercapacitor charge enable pin */
#define CHARGE_EN  PORT1.PODR.BIT.B6
#define CHG_EN_PDR  PORT1.PDR.BIT.B6
#define CHG_EN_ODR  PORT1.ODR1.BIT.B4

/* Supercapacitor voltage thresholds (2V - 4.8V) and hysteresis factor (0.2V) */
#define SUPERCAP_UPPER      ((uint16_t) 3600)
#define SUPERCAP_LOWER      ((uint16_t) 2380)
#define SUPERCAP_HYSTERISIS ((uint16_t) 30)

/* LDO enable control pin */
#define LDO_EN      PORTA.PODR.BIT.B0
#define LDO_EN_PDR  PORTA.PDR.BIT.B0

/* LED State control */
#define LED_ON      0x0
#define LED_OFF     0x1u

/* Power State control */
#define POWER_ON    0x0
#define POWER_OFF   0x1u

/* Protection register settings */
#define PROTECTOFF 0xA507u
#define PROTECTON  0xA500u

#endif /* RPBRX111_H */
