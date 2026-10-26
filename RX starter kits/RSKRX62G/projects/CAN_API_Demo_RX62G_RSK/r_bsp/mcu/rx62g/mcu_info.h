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
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.    
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name     : mcu_info.h
* Device(s)    : RX62G
* Description  : Information about the MCU on this board.   
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 29.08.2012 1.00     First Release
***********************************************************************************************************************/

#ifndef _MCU_INFO
#define _MCU_INFO

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Gets MCU configuration information. */
#include "r_bsp_config.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* MCU Series. */
#if   MCU_PART_SERIES == 0x0
    #define MCU_SERIES_RX600    (1)
#else
    #error "ERROR - MCU_PART_SERIES - Unknown MCU Series chosen in r_bsp_config.h"
#endif

/* MCU Group name. */
#if   MCU_PART_GROUP == 0x0
    #define MCU_RX62G           (1)
    #define MCU_RX62x           (1)
#else
    #error "ERROR - MCU_PART_GROUP - Unknown MCU Group chosen in r_bsp_config.h"
#endif

/* Package. */
#if   MCU_PART_PACKAGE == 0x0
    #define PACKAGE_LQFP112     (1)
#elif MCU_PART_PACKAGE == 0x1
    #define PACKAGE_LQFP100     (1)
#else
    #error "ERROR - MCU_PART_PACKAGE - Unknown package chosen in r_bsp_config.h"
#endif

/* Memory size of your MCU. */
#if MCU_PART_MEMORY_SIZE == 0x7
    #define ROM_SIZE_BYTES      (131072)
    #define RAM_SIZE_BYTES      (8192)
    #define DF_SIZE_BYTES       (8192)
#elif MCU_PART_MEMORY_SIZE == 0xA
    #define ROM_SIZE_BYTES      (262144)
    #define RAM_SIZE_BYTES      (16384)
    #define DF_SIZE_BYTES       (32768)
#else
    #error "ERROR - MCU_PART_MEMORY_SIZE - Unknown memory size chosen in r_bsp_config.h"
#endif

/* System clock speed in Hz. */
#define ICLK_HZ             (XTAL_HZ*ICK_MUL)
/* Peripheral clock speed in Hz. */
#define PCLK_HZ             (XTAL_HZ*PCK_MUL)

#endif /* _MCU_INFO */
