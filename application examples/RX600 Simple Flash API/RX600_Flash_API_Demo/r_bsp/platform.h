/* Adapted for use with IAR Embedded Workbench */
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
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.    
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name     : platform.h
* Version      : 1.0 
* Description  : The user chooses which MCU and board they are developing for in this file. If the board you are using
*                is not listed below, please add your own. You can define your MCU and board below, or you can add the
*                macro to your project defines.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 30.11.2011 1.00     First Release
***********************************************************************************************************************/

#ifndef _PLATFORM_H_
#define _PLATFORM_H_

/***********************************************************************************************************************
INCLUDE APPROPRIATE MCU AND BOARD FILES

  The correct constant is defined in each IAR EW project settings respectively.
***********************************************************************************************************************/

/* Handle include files based on board. */
#if defined(PLATFORM_BOARD_RSKRX610)
#include    <intrinsics.h>
#include    <iorx610.h>
#include    "./board/rskrx610/mcu_info.h"
#include    "./board/rskrx610/rskrx610.h"
#include    "./board/rskrx610/hwsetup.h"
#elif defined(PLATFORM_BOARD_RSKRX62N)
#include    <intrinsics.h>
#include    <iorx62n.h>
#include    "./board/rskrx62n/mcu_info.h"
#include    "./board/rskrx62n/rskrx62n.h"
#include    "./board/rskrx62n/hwsetup.h"
#elif defined(PLATFORM_BOARD_RSKRX62T)
#include    <intrinsics.h>
#include    <iorx62t.h>
#include    "./board/rskrx62t/mcu_info.h"
#include    "./board/rskrx62t/rskrx62t.h"
#include    "./board/rskrx62t/hwsetup.h"
#elif defined(PLATFORM_BOARD_RDKRX62N)
#include    <intrinsics.h>
#include    <iorx62n.h>
#include    "./board/rdkrx62n/yrdkrx62n.h"
#include    "./board/rdkrx62n/mcu_info.h"
#include    "./board/rdkrx62n/hwsetup.h"
#elif defined(PLATFORM_BOARD_RSKRX630)
#include    <intrinsics.h>
#include    <iorx630.h>
#include    "./board/rskrx630/rskrx630.h"
#include    "./board/rskrx630/mcu_info.h"
#include    "./board/rskrx630/hwsetup.h"
#elif defined(PLATFORM_BOARD_RSKRX63N)
#include    <intrinsics.h>
#include    <iorx63n.h>
#include    "./board/rskrx63n/rskrx63n.h"
#include    "./board/rskrx63n/mcu_info.h"
#include    "./board/rskrx63n/hwsetup.h"
#elif defined(PLATFORM_BOARD_RDKRX63N)
#include    <intrinsics.h>
#include    <iorx63n.h>
#include    "./board/rdkrx63n/yrdkrx63n.h"
#include    "./board/rdkrx63n/mcu_info.h"
#include    "./board/rdkrx63n/hwsetup.h"
#else
#error      "Please define valid board."
#endif

#endif /* _PLATFORM_H_ */

