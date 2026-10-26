/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No 
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all 
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM 
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES 
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS 
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of 
* this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer 
*
* Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : platform.h
* Description  : The user chooses which MCU and board they are developing for in this file. If the board you are using
*                is not listed below, please add your own or use the default 'User Board'.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*			04.11.2014 1.00     First version - based on example presented in R01AN1890EU.
*
*
***********************************************************************************************************************/

#ifndef PLATFORM_CUSTOM_H
#define PLATFORM_CUSTOM_H

/***********************************************************************************************************************
DEFINE YOUR SYSTEM - UNCOMMENT THE INCLUDE PATH FOR THE PLATFORM YOU ARE USING.
***********************************************************************************************************************/

/***************************************************************************
Includes <System Includes> , "Project Includes"
***************************************************************************/
#include <stdint.h>
#include <stdbool.h>
/* Access to intrinsic functions. */
#include <machine.h>
/* Register access. */
#include "iorx113.h"
/* Not all configuration items in r_bsp_config.h are being used. */
#include "r_bsp_config.h"
#include "mcu_info.h"
/* Locking used by some FIT Modules. */
#include "locking.h"
#include "mcu_locks.h"

/***************************************************************************
Macro definitions
***************************************************************************/
/* This takes care of FIT Modules that check for the version of the r_bsp */
#define R_BSP_VERSION_MAJOR (2)
#define R_BSP_VERSION_MINOR (30)


#endif /* PLATFORM_CUSTOM_H */

