/* Adapted for use with IAR Embedded Workbench */
/******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Electronics Corp. and is
* only intended for use with Renesas products.  

* No other uses are authorized.

* This software is owned by Renesas Electronics Corp. and is 
* protected under the applicable laws, including copyright laws.

* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY,
* INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH
* WARRANTIES ARE EXPRESSLY DISCLAIMED.

* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER 
* RENESAS ELECTRONICS CORP. NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR AND DIRECT, INDIRECT, SPECIAL, INCIDENTAL
* OR COSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE,
* EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE
* POSSIBILITIES OF SUCH DAMAGES.

* Renesas reserves the right, without notice, to make changes to this
* software and to discontinue availability of this software.
* By using this software, you agree to the additional terms and 
* conditions found by accessing the following link:
* http://www.renesas.com/disclaimer
*******************************************************************************/
/* Copyright (C) 2010. Renesas Electronics Corp., All Rights Reserved         */
/******************************************************************************
* File Name     : screen.h
* Version       : 1.0
* Device(s)     : R5F56108VNFP
* Tool-Chain    : RX Family C Compiler
* OS            : None
* H/W Platform  : RX62N
* Description   : Header file for 
* Operation     :
* Limitations   : None
*******************************************************************************
* History : DD.MM.YYYY  Version Description
*         : 14.09.2010  1.00    First release
*******************************************************************************/
#ifndef _SCREEN_H_
#define _SCREEN_H_

/* For the types uint8_t and uint16_t */
#include "stdint.h"
/* For VNC server*/
#include "vnc-out.h"

/* Renesas Logo BMP size */
#define LOGO_HGT   38
#define LOGO_WDT   224

/* White area definitions for above and below the logo on the frame buffer*/
#define UPPER_BOTTOM_EDGE   (uint8_t)((Y_SIZE - LOGO_HGT) / 2)
#define LOWER_TOP_EDGE     (uint8_t)((Y_SIZE + LOGO_HGT) / 2)

/* Pong board definitions */
#define BOARD_CENTRE   Y_SIZE / 2

/* Screen size */
/* For ease of conversion for the Renesas background logo, size is a multiple of 8 */
#define X_SIZE     224
#define Y_SIZE     224

/* Basic colours */
#define BLACK       0x00
#define WHITE       0xFF

/* With RGB222: */
#define VWHITE       0x3F
#define VBLK      0x3F
#define VBLU      0x03
#define VDARKBLUE    0x01
#define VDARKGREEN    0x04
#define VGREEN      0x0C
#define VLIGHTGREEN    0x08
#define VRED      0x30
#define VLIGHTRED    0x20
#define VBLACK      0x00
#define VALMOSTWHITE  0x2F

/* 2 x 2d arrays, the screen with 2 buffers */
extern unsigned char g_ucFrameBuf[Y_SIZE][X_SIZE];

/* Colour palette with 256 entries */
extern unsigned long colour_palette[];

/* Renesas logo, left to right, top to bottom */
extern const unsigned char renesaslogo_bmp[LOGO_HGT][LOGO_WDT];

/* Screen update function */
void updateVNC(int16_t x, int16_t y, int16_t dx, int16_t dy, uint8_t index);


#endif
