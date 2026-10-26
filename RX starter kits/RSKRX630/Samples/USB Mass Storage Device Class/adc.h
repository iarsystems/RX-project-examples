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
/* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.   */
/*******************************************************************************
* File Name    : adc.h
* Version     : 1.00
* Device         : RX630
* Tool Chain   : IAR Embedded Workbench
* H/W Platform   : RSKRX630
* Description   : Defines ADC function prototypes    
*******************************************************************************/
/*******************************************************************************
* History     : 23.01.2012 Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* Macro Defintions
*******************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef ADC_H
#define ADC_H

/*******************************************************************************
* System Includes
*******************************************************************************/
/* Includes standard integer type definitions */
#include <stdint.h>

/*******************************************************************************
Function Prototypes
*******************************************************************************/
/* ADC initialisation function declaration */
void Init_ADC(void);

/* Start_ADC function prototype */
void Start_ADC(void);
/* Read_ADC function prototype */
uint16_t Read_ADC(void);

/* End of multiple inclusion prevention macro */
#endif










