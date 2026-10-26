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
* File Name    : power_off.h
* Version     : 1.00
* Device     : R5F5630EDDFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX630
* Description   : header file for power_off.c
*******************************************************************************/
/*******************************************************************************
* History     : 21.11.2011  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
Macro Definitions
*******************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef POWER_OFF_H
#define POWER_OFF_H

/* Defines Power Modes */
#define POWER_OFF_MODE  1
#define ACTIVE_MODE    0

/*******************************************************************************
* Project Includes
*******************************************************************************/
/* Defines standard boolean variable types used in this file */
#include <stdbool.h>

/*******************************************************************************
* Global Variables
*******************************************************************************/
extern volatile bool gEnterStandby;

/*******************************************************************************
* Global Function Prototypes
*******************************************************************************/
void Init_PowerOff(void);
void EnterStandby_PowerOff(void);
void FlashLEDs_PowerOff(void);

/* End of multiple inclusion prevention macro */
#endif
