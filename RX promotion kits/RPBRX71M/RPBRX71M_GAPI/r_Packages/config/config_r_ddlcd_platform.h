/******************************************************************************
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
 *******************************************************************************
 * Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.    */
/******************************************************************************
 * File Name    : config_r_ddlcd_platform.h
 * Version      : 1.00
 * Device(s)    : Renesas
 * Tool-Chain   : RXC
 * OS           : None
 * H/W Platform : RPB
 * Description  : Direct Drive LCD platform configuration - defines the peripheral
 *                and channels used to drive the external LCD Panel.
 *
 *******************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 19.04.2016 1.00 First Release
 ******************************************************************************/

/******************************************************************************
 WARNING!  IN ACCORDANCE WITH THE USER LICENCE THIS CODE MUST NOT BE CONVEYED
 OR REDISTRIBUTED IN COMBINATION WITH ANY SOFTWARE LICENSED UNDER TERMS THE
 SAME AS OR SIMILAR TO THE GNU GENERAL PUBLIC LICENCE
 ******************************************************************************/
#ifndef DDLCD_PLATFORM_H_INCLUDED
#define DDLCD_PLATFORM_H_INCLUDED

#include "platform.h"

/******************************************************************************
Macro definitions
******************************************************************************/

/* use the definitions below to change the MCU ports used for interfacing to the panel */
/* Define which CS is used for the frame buffer */
#define DD_FRAME_CS                     3

/* control vsync */
#define DD_VSYNC_PORT       PORTB
#define DD_VSYNC_MPC        PB
#define DD_VSYNC_PIN        5

/* control hsync */
#define DD_HSYNC_PORT       PORTC
#define DD_HSYNC_MPC        PC
#define DD_HSYNC_PIN        0

/* control hden */
#define DD_HDEN_PORT        PORTC
#define DD_HDEN_MPC         PC
#define DD_HDEN_PIN         1

/* control dotclock */
#define DD_DOTCLK_PORT      PORTB
#define DD_DOTCLK_MPC       PB
#define DD_DOTCLK_PIN       3


/* turn on power to LCD backlight */
#define DD_BACKLIGHT_PORT   PORTA
#define DD_BACKLIGHT_PIN    5

/* LCD Panel Reset Pin */
#define DD_LCDRESET_PORT   PORT4
#define DD_LCDRESET_PIN    7

#define LCD_RESET_TIME (1 / portTICK_RATE_MS) /* Time to assert LCD Panel reset signal */


/* DMA Channel used for direct drive */
#define DD_DMAC DMAC0

/* specify timer unit used for direct drive */
#define DD_TMR_MTU

/* Timer Channel Used for the Dot Clock Signal and period (must be seperate channel from other timer channels) */
#define DD_DOTCLK_CHANNEL  0
#define DD_DOTCLK_TIOC     A     /* must be A or C because of PWM1 operation */
#define DD_DOTPER_CHANNEL  0     /* must be same as DD_DOTCLK_CHANNEL */
#define DD_DOTPER_TIOC     B     /* must be complementary pin to DD_DOTCLK_TIOC A->B, C->D */

/* Timer Channels used for Horizontal Synch Signal */
#define DD_HSYNC_CHANNEL   3
#define DD_HSYNC_TIOC      C     /* must be A or C because of PWM1 operation */
#define DD_HPER_CHANNEL    3     /* must be same as DD_HSYNC_CHANNEL */
#define DD_HPER_TIOC       D     /* must be complementary pin to DD_HSYNC_TIOC A->B, C->D */

/* Timer Channels used for Vertical Synch Signal */
#define DD_VSYNC_CHANNEL   2
#define DD_VSYNC_TIOC      A     /* must be A or C because of PWM1 operation */
#define DD_VPER_CHANNEL    2     /* must be same as DD_VSYNC_CHANNEL */
#define DD_VPER_TIOC       B     /* must be complementary pin to DD_VSYNC_TIOC A->B, C->D */

/* Timer Channels used for Horizontal Data Enable Signal */
#define DD_HDEN_CHANNEL    3
#define DD_HDEN_TIOC       A     /* must be A or C because of PWM1 operation */
#define DD_HDEN2_CHANNEL   3     /* must be same as DD_HDEN_CHANNEL */
#define DD_HDEN2_TIOC      B     /* must be complementary pin to DD_HDEN_TIOC A->B, C->D */

/* Timer Channels used for Panel Data Enable Signal - Interrupt is used as a trigger for the DMA */
#define DD_PDEN_CHANNEL    1
#define DD_PDEN_TIOC       A     /* must be A or C because of PWM1 operation */
#define DD_PDEN2_CHANNEL   1     /* must be same as DD_PDEN_CHANNEL */
#define DD_PDEN2_TIOC      B     /* must be complementary pin to DD_PDEN_TIOC A->B, C->D */

#endif //DDLCD_PLATFORM_H_INCLUDED

/******************************************************************************
 End  Of File
 ******************************************************************************/

