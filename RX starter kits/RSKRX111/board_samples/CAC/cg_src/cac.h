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
/* Copyright (C) _#COPYRIGHTYEAR#_ Renesas Electronics Corporation. All rights reserved.   */
/*******************************************************************************
* File Name     : sci.h
* Version       : _#VERSION#_
* Device        : _#DEVICE#_
* Tool-Chain    : _#TOOLCHAIN#_
* H/W Platform  : _#HWPLATFORM#_
* Description   : This Header file contains the Macro Definitions & prototypes  
*                 for the functions used in r_cg_sci_user.c
*******************************************************************************/
/*******************************************************************************
* History       : _#DATE#_  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
Macro Definitions
*******************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef CAC2_H
#define CAC2_H

/* Change this to 0 to use an external clock reference source on JA3-1 */
#define USE_INTERNAL_CLOCK 1

#if USE_INTERNAL_CLOCK
    /* Upper and lower frequency error values for 125KHz +/- 15%
       value = Fm/Fref where Fm is measurement frequency tolerance, Fref is
       reference clock including division, 30.517578125 Hz in this case */
    #define FREQ_ERROR_UPPER 4710u
    #define FREQ_ERROR_LOWER 3482u

    /* Conversion factor from count value to frequency in Hz */
    #define FREQ_CONVERSION  30.517578125f
#else
    /* Upper and lower frequency error values for 125KHz +/- 15%
       value = Fm/Fref where Fm is measurement frequency tolerance, Fref is
       reference clock, 100 Hz in this case */
    #define FREQ_ERROR_UPPER 1438u
    #define FREQ_ERROR_LOWER 1063u

    /* Conversion factor from count value to frequency in Hz */
    #define FREQ_CONVERSION  100u
#endif

/* Define a macro to indicate selection of a periodic timer mode */
#define PERIODIC_MODE 0x1u

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/
/* Defines standard variable types used in this file */
#include <stdint.h>
/* r_cg_cmt_user.c */
void Update_Frequency_LCD(void);
uint8_t Frequency_Measured(void);

/* r_cg_cac_user.c */
uint8_t Frequency_Error(void);
void Report_Frequency_Error(void);

#endif 
