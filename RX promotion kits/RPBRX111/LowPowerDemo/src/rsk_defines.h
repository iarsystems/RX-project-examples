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
* File Name     : rsk_defines.h
* Version       : 1.00
* Device        : R5F51115AxFM
* Tool Chain    : CCRX
* H/W Platform  : RPBRX111
* Description   : Defines for RPB Hardware
*******************************************************************************/
/*******************************************************************************
* History       : 12/08/2013  Ver. 1.00 First Release
*******************************************************************************/
/* Multiple inclusion prevention */
#ifndef RSK_DEFINES_H
#define RSK_DEFINES_H

/* RPB hardware pin definitions */
#include "rpbrx111.h"

/******************************************************************************
   Specify the desired CPU operation mode
   The options are:
    ---------------------------------------------------------------------------
    CPU_MIN
    ---------------------------------------------------------------------------
    Peripheral clock supply : STOP
    Clock Divider           : FCLK : ICLK : BCLK : PCLKB : PCLKD  
     (n: clock source freq)     64 : n    : 64   : 64    : 64      
    CPU operation           : NOP-LOOP 
    Peripheral function     : stop

    ---------------------------------------------------------------------------
    CPU_TYP
    ---------------------------------------------------------------------------
    Peripheral clock supply : Supply to all peripheral
    Clock divider           : FCLK : ICLK : BCLK : PCLKB : PCLKD 
     (n: clock source freq)     2n : n    : 2n   : 2n    : n       
    CPU operation           : NOP-LOOP 
    Peripheral function     : stop
       
    ---------------------------------------------------------------------------
    CPU_MAX
    ---------------------------------------------------------------------------
    Peripheral clock supply : Supply to all peripheral
    Clock divider           : FCLK : ICLK : BCLK : PCLKB : PCLKD 
     (n: clock source freq)     2n : n : 2n : 2n : n               
    CPU operation           : MUL operation - LOOP, DMAC operation 
    Peripheral function     : CMT / MTU2 - all ch operating    

******************************************************************************/

/* Switch LED2 on macro */
#define LED2_ON     (LED2 = LED_ON)

/* Switch LED2 off macro */
#define LED2_OFF    (LED2 = LED_OFF)

/* Switch LED1 on macro */
#define LED1_ON     (LED1 = LED_ON)

/* Switch LED1 off macro */
#define LED1_OFF    (LED1 = LED_OFF)

/* Switch LED0 on macro */
#define LED0_ON     (LED0 = LED_ON)

/* Switch LED0 off macro */
#define LED0_OFF    (LED0 = LED_OFF)

/* LED0 Bit Mask Value */
#define LED0MASK    0x01

/* LED1 Bit Mask Value */
#define LED1MASK    0x02

/* LED2 Bit Mask Value */
#define LED2MASK    0x04


/* Port Direction define: Output */
#define OUTPUT      1

/* Port Direction define: Input */
#define INPUT       0

/* Switch Input Port define: Not pressed */
#define NOT_PRESSED 1

/* Switch Input Port define: Pressed */
#define PRESSED     0
                    
/* Main menu options enumeration */
enum enum_Main_Menu
{
    RTC_MENU   = 0,
    SUB_32k    = 1,
    LOCO_4M    = 2,
    HOCO_32M   = 3,
    SW_SLEEP   = 4,
    DP_SLEEP   = 5,
    SW_STBY    = 6
};

/* Real Time Clock menu options enumeration */
enum enum_RTC_Options_Menu
{
    OUTPUT_RTC = 1,
    RESET_RTC  = 2,
    START_RTC  = 3,
    STOP_RTC   = 4
};

/* CPU Load menu options enumeration */
enum enum_CPU_Options_Menu
{
    CPU_MIN = 1,
    CPU_TYP = 2,
    CPU_MAX = 3
};

/* End of multiple inclusion prevention macro */
#endif 

/* End of file */
