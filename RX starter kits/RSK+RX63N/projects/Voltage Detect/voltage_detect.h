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
/* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.   */
/*******************************************************************************
* File Name     : voltage_detect.h
* Version       : 1.00
* Device        : R5F563NB
* Tool-Chain    : RX Family C Compiler
* H/W Platform  : RSK+RX63N
* Description   : Hardware setup header file.  
*******************************************************************************/
/*******************************************************************************
* History       : xxx  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* Macro Definitions
*******************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef VOLTAGE_DETECT_H
#define VOLTAGE_DETECT_H

/*******************************************************************************
* Global Function Prototypes
*******************************************************************************/
/* Voltage detect intialisation function prototype */
void Init_VoltageDetect(void);
/* LED flash function prototype */
void LedFlash_VoltageDetect(void);

/* End of multiple inclusion prevention macro */
#endif
