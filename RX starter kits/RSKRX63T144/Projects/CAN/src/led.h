/*******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. 

* No other uses are authorized.
*
* This software is owned by Renesas Electronics Corp. and is protected under
* all applicable laws, including copyright laws.
*
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
*
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
*
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.    
*******************************************************************************/
/******************************************************************************
* File Name     : led.h
* Version       : 1.0
* Device(s)     : R5F563NB
* Tool-Chain    : Renesas RX Standard Toolchain 1.1.0.0
* OS            : None
* H/W Platform  : RSK+RX63N
* Description   : Contains general purpose LED drivers
*******************************************************************************
* History : DD.MM.YYYY     Version     Description
*         : 01.10.2011     1.00        First release
*******************************************************************************/
#ifndef _LED_H
#define _LED_H

/******************************************************************************
Macro definitions
******************************************************************************/
/* led numbers used in below APIs */
#define LED0            (0)
#define LED1            (1)
#define LED2            (2)
#define LED3            (3)

/* led state used in below APIs */
#define LED_ON          (0)
#define LED_OFF         (1)

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/
void led_initialize(void);
void led_set(uint32_t led, uint32_t state);
void led_toggle(uint32_t led);
void led_test(uint32_t nr_led_flashes);

#endif
