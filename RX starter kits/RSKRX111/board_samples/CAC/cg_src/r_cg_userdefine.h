/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. This 
* software is owned by Renesas Electronics Corporation and is protected under 
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING 
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT 
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR 
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE 
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software 
* and to discontinue the availability of this software.  By using this software, 
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_userdefine.h
* Version      : Applilet4 for RX111 V1.00.00.05 [06 Jun 2013]
* Device(s)    : R5F51115AxFM
* Tool-Chain   : IAR Systems icc78k0r
* Description  : This file includes user definition.
* Creation Date: 2013-08-16
***********************************************************************************************************************/
#ifndef _USER_DEF_H
#define _USER_DEF_H

/***********************************************************************************************************************
User definitions
***********************************************************************************************************************/
#define FAST_INTERRUPT_VECTOR 0

/* Start user code for function. Do not edit comment generated here */

#define CLOCK_FREQ             (16000000)
#define PCLK_FREQ              (CLOCK_FREQ / 64)

#define IWDT_NOMINAL_FREQUENCY (15000)
#define IWDT_TOLERANCE_PERCENT (5)
#define IWDT_UPPER_LIMIT       (IWDT_NOMINAL_FREQUENCY*(100 + IWDT_TOLERANCE_PERCENT) / 100)
#define IWDT_LOWER_LIMIT       (IWDT_NOMINAL_FREQUENCY*(100 - IWDT_TOLERANCE_PERCENT) / 100)

#define CAC_DIVISOR            (8192)
#define CAC_REF_FREQUENCY      (PCLK_FREQ / CAC_DIVISOR)

#define IWDT_UPPER_LIMIT_COUNT (IWDT_UPPER_LIMIT / CAC_REF_FREQUENCY)
#define IWDT_LOWER_LIMIT_COUNT (IWDT_LOWER_LIMIT / CAC_REF_FREQUENCY)

/* End user code. Do not edit comment generated here */
#endif