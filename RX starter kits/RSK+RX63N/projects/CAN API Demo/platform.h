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
* File Name     : platform.h
* Version       : 1.0
* Device(s)     : R5F563NB
* Tool-Chain    : Renesas RX Standard Toolchain 1.1.0.0
* OS            : None
* H/W Platform  : RSK+RX63N
* Description   : Contains board level settings
*******************************************************************************
* History : DD.MM.YYYY     Version     Description
*         : 01.10.2011     1.00        First release
*******************************************************************************/
#ifndef _PLATFORM_H
#define _PLATFORM_H

/******************************************************************************
Macro definitions
******************************************************************************/

/* System Clock Settings */
#define XTAL_FREQUENCY      (12000000L)
#define FCLK_MUL            (4)
#define ICLK_MUL            (8)
#define BCLK_MUL            (4)
#define PCLKA_MUL           (8)
#define PCLKB_MUL           (4)
#define UCLK_MUL            (4)
#define IECLK_MUL           (2)

#define FCLK_FREQUENCY      (XTAL_FREQUENCY * FCLK_MUL)
#define ICLK_FREQUENCY      (XTAL_FREQUENCY * ICLK_MUL)
#define BCLK_FREQUENCY      (XTAL_FREQUENCY * BCLK_MUL)
#define PCLKA_FREQUENCY     (XTAL_FREQUENCY * PCLKA_MUL)
#define PCLKB_FREQUENCY     (XTAL_FREQUENCY * PCLKB_MUL)
#define UCLK_FREQUENCY      (XTAL_FREQUENCY * UCLK_MUL)
#define IECLK_FREQUENCY     (XTAL_FREQUENCY * IECLK_MUL)


#endif
