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
 * File Name    : config_r_ddlcd_panel.1
 * Version      : 1.00
 * Device(s)    : Renesas
 * Tool-Chain   : N/A
 * OS           : None
 * H/W Platform : RPB
 * Description  :
 * Definitions below used to configure the connected LCD panel to the MCU
 * Configured for: Midas MCT035T6TW320240LML, RS Part# 885-1465
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
#ifndef DDLCD_PANEL_H_INCLUDED
#define DDLCD_PANEL_H_INCLUDED

#include "platform.h"
/******************************************************************************
Macro definitions
******************************************************************************/
/* DOT CLOCK Inversion Control */
/* Define this symbol if dot inversion necessary */
#define DD_INVERT_DOT

/* Vertical pulse width (inactive pulse width) */
#define DD_V_LINES_PULSE            (8)

/* Vertical back porch (Lines Vsynch Active till display lines) */
#define DD_V_LINES_BACK_PORCH       (15)

/* Vertical lines (active data lines) */
#define DD_V_LINES_DISPLAY          (240L)

/* Minimum Vertical Front Porch (post display till Vsych inactive)...this may be extended by driver to meet requested behavior */
#define DD_V_LINES_FRONT_PORCH      (2)

/* Horizontal pulse width (dot clocks inactive)...specify value GREATER than min AND LESS than max (min < value < max) */
#define DD_H_DOT_PULSE              (68)

/* Horizontal back porch (dot clocks Hsync active till display data) */
#define DD_H_DOT_BACK_PORCH         (38)

/* Horizontal pixels (active data) */
#define DD_H_DOT_DISPLAY            (320L)

/* Horizontal front porch (post data till Hsynch inactive) */
#define DD_H_DOT_FRONT_PORCH       (20)

/* Minimum Frame Rate (Hz) supported by the panel */
#define DD_MIN_FRAME_RATE          (60)

/* Maximum Frame Rate (Hz) supported by the panel */
#define DD_MAX_FRAME_RATE          (63)

/* Initial Touch Screen Calibration data */
#define TC_LL_X         (0x0E0)    /* filtered (14 bit) A/D reading at lower left of screen */
#define TC_LL_Y         (0xDCA)

#define TC_LR_X         (0xF12)    /* filtered (14 bit) A/D reading at lower right of screen */
#define TC_LR_Y         (0xDC9)

#define TC_UL_X         (0x0DC)    /* filtered (14 bit) A/D reading at upper left of screen */
#define TC_UL_Y         (0x148)

#define TC_UR_X         (0xF15)    /* filtered (14 bit) A/D reading at upper right of screen */
#define TC_UR_Y         (0x158)

#endif //DDLCD_PANEL_H_INCLUDED

/******************************************************************************
 End  Of File
 ******************************************************************************/
