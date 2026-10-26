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
* File Name    : r_cg_dac.h
* Version      : Applilet4 for RX111 V1.00.00.05 [06 Jun 2013]
* Device(s)    : R5F51115AxFM
* Tool-Chain   : IAR Systems icc78k0r
* Description  : This file implements device driver for DAC module.
* Creation Date: 2013-08-16
***********************************************************************************************************************/
#ifndef DAC_H
#define DAC_H

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/
/* 
    D/A Control Register (DACR)
*/
/* D/A Output Enable 0 (DAOE0) */
#define _00_DAC0_DISABLE           (0x00U) /* Analog output of channel 0 (DA0) is disabled */
#define _40_DAC0_ENABLE            (0x40U) /* D/A conversion of channel 0 is enabled */
/* D/A Output Enable 1 (DAOE1) */
#define _00_DAC1_DISABLE           (0x00U) /* Analog output of channel 1 (DA1) is disabled */
#define _80_DAC1_ENABLE            (0x80U) /* D/A conversion of channel 1 is enabled */

/*
    DADRm Format Select Register (DADPR)
*/
/* DADRm Format Select (DPSEL) */
#define _00_DAC_DPSEL_R            (0x00U) /* Data is flush with the right end of the D/A data register */
#define _80_DAC_DPSEL_L            (0x80U) /* Data is flush with the left end of the D/A data register */

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void R_DAC_Create(void);
void R_DAC0_Start(void);
void R_DAC0_Stop(void);
void R_DAC0_Set_ConversionValue(uint16_t reg_value);

/* Start user code for function. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#endif