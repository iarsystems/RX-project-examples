/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2016, 2017 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : Config_TMR2_TMR3.h
* Version      : 1.3.0
* Device(s)    : R5F566TEAxFP
* Description  : This file implements device driver for Config_TMR2_TMR3.
* Creation Date: 2018-11-20
***********************************************************************************************************************/

#ifndef CFG_Config_TMR2_TMR3_H
#define CFG_Config_TMR2_TMR3_H

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_tmr.h"

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define _4C4A_TMR23_COMP_MATCH_VALUE_A                (0x4C4AU)
#define _0186_TMR23_COMP_MATCH_VALUE_B                (0x0186U)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void R_Config_TMR2_TMR3_Create(void);
void R_Config_TMR2_TMR3_Start(void);
void R_Config_TMR2_TMR3_Stop(void);
void R_Config_TMR2_TMR3_Create_UserInit(void);
/* Start user code for function. Do not edit comment generated here */

/* Count equivalent to for specifed time   */
#define    PERIOD_HALF_SEC_COUNT  (1u)
#define    PERIOD_ONE_SEC_COUNT   (2u)
#define    PERIOD_ONE_MIN_COUNT   (120u)

/* Flag used to indicate data acquisition event triggered by the TMR system */
extern volatile uint8_t g_data_acq_flag;

void R_TMR_ClearCount(void);

/* End user code. Do not edit comment generated here */
#endif
