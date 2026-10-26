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
* File Name     : cac.h
* Version       : 1.00
* Device        : R5F563TE
* Tool-Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX63T144
* Description   : Contains prototypes for the CAC functions 
*******************************************************************************/
/*******************************************************************************
* History       : 05.12.2013  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* Macro Definitions
*******************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef CAC_H
#define CAC_H

/* Change this to 0 to use an external clock reference source on JA3-1 */
#define USE_INTERNAL_CLOCK 1

#if USE_INTERNAL_CLOCK
    /* Upper and lower frequency error values for 125KHz +/- 15%
       value = Fm/Fref where Fm is measurement frequency tolerance, Fref is
       reference clock including division, 22.888183 Hz in this case */
    #define FREQ_ERROR_UPPER 6280u
    #define FREQ_ERROR_LOWER 4642u
    
    /* Conversion factor from count value to frequency in Hz */
    #define FREQ_CONVERSION  22.88183f
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
* Global Function Prototypes
*******************************************************************************/

/* CAC initialisation function prototype declaration */
void Init_CAC (void);

/* End of multiple inclusion prevention macro */
#endif

