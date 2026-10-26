/* Adapted for use with IAR Embedded Workbench */
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
/* Copyright (C) 2010 Renesas Electronics Corporation. All rights reserved. */
/**********************************************************************
* File Name    : mandel.h
* Version     : 1.00
* Device     : Renesas Generic MCU.
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSK Generic
* Description   : Mandelbrot demo.
**********************************************************************/

/**********************************************************************
* History     : 29.09.2010 Ver. 1.00 First Release
**********************************************************************/

#ifndef __MANDELBROT_H__
#define __MANDELBROT_H__

void mandelbrot(void *data);
void mandelbrot_init(void);

void updateMandelJoystickPosition(uint8_t _bCenter, 
                  uint8_t _bRight, 
                  uint8_t _bDown, 
                  uint8_t _bLeft, 
                  uint8_t _bUp, 
                  uint8_t _bSW1);

#endif
