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
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : Config_MTU9.h
* Version      : 1.5.0
* Device(s)    : R5F572TKCxFB
* Description  : This file implements device driver for Config_MTU9.
* Creation Date: 2019-05-21
***********************************************************************************************************************/

#ifndef CFG_Config_MTU9_H
#define CFG_Config_MTU9_H

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_mtu3.h"

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define _C34F_TGRA9_VALUE               (0xC34FU) /* TGRA9 value */
#define _0320_TGRB9_VALUE               (0x0320U) /* TGRB9 value */
#define _0064_TGRC9_VALUE               (0x0064U) /* TGRC9 value */
#define _0064_TGRD9_VALUE               (0x0064U) /* TGRD9 value */
#define _0064_TGRE9_VALUE               (0x0064U) /* TGRE9 value */
#define _0064_TGRF9_VALUE               (0x0064U) /* TGRF9 value */

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void R_Config_MTU9_Create(void);
void R_Config_MTU9_Start(void);
void R_Config_MTU9_Stop(void);
void R_Config_MTU9_Create_UserInit(void);
/* Start user code for function. Do not edit comment generated here */

#define TICKS_PER_CYCLE (_C34F_TGRA9_VALUE)
extern volatile uint8_t g_vary_duty_cycle;
extern volatile uint16_t g_duty_update;

/* End user code. Do not edit comment generated here */
#endif
