/******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized.

* This software is owned by Renesas Electronics Corporation and is protected under 
* all applicable laws, including copyright laws.

* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES 
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, 
* INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
* PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY 
* DISCLAIMED.

* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES 
* FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS 
* AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

* Renesas reserves the right, without notice, to make changes to this 
* software and to discontinue the availability of this software.  
* By using this software, you agree to the additional terms and 
* conditions found by accessing the following link:
* http://www.renesas.com/disclaimer
******************************************************************************/
/* Copyright (C) 2024 Renesas Electronics Corporation. All rights reserved.  */
/******************************************************************************	
* File Name    : 
* Device(s)    : 
* Tool-Chain   : 
* H/W Platform : 
* Description  : 
******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 25.06.2024 1.00    First Release
******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdint.h>
#include "R5K574M.h"
#include "r_pz.h"

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/
extern void root_world_check_error(void);
extern uint32_t get_wid(void);
extern void init_TRCTB_WINT(void);

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/


/******************************************************************************
Private global variables and functions
******************************************************************************/
static void r_protectzone_enable(void);
static void r_protectzone_disable(void);

/*******************************************************************************************************************//**
 * R_ProtectZone_Open() is the ProtectZone start function.
 **********************************************************************************************************************/
void R_ProtectZone_Open(void)
{
    volatile bool b_ret;

    /* Protected zone enabled */
    r_protectzone_enable();

    /* WID check */
    b_ret = R_ProtectZone_CheckWid(R_PZ_ROOT);
    if (b_ret != true)
    {
        root_world_check_error();
    }

    /* Initialize TRCTB table and WINTB table */
    init_TRCTB_WINT();

    R_SYSTEM->PRCR = 0xA510;

    /* SRAM area partitioning(use Secure or Non-Secure) */
    R_CPSCU->SRAMSABAR0=0x00008000;      /* 0x0200_0000 to 0x0200_7FFF Not assigned (unavailable)   */
                                         /* 0x0200_8000 to 0x0200_FFFF (SRAM0 32KB) Non-Secure area */
                                         /* 0x8200_0000 to 0x8200_7FFF (SRAM0 32KB) Secure area     */
                                         /* 0x8200_8000 to 0x8200_FFFF Not assigned (unavailable)   */

    R_SYSTEM->PRCR = 0xA500;
}

/*******************************************************************************************************************//**
 * R_ProtectZone_Close() is the ProtectZone termination function.
 **********************************************************************************************************************/
void R_ProtectZoneClose(void)
{
    r_protectzone_disable();
}

/*******************************************************************************************************************//**
 * R_ProtectZone_CheckWid() checks the WID.
 **********************************************************************************************************************/
bool R_ProtectZone_CheckWid(r_pz_wid_t expect_wid)
{
	uint32_t wid = 0x00000003;

    wid = get_wid();
    if((wid & 0x00000003) != expect_wid)
    {
    	return false;
    }

    return true;
}

/*******************************************************************************************************************//**
 * r_protectzone_enable() enables the ProtectZone filter.
 **********************************************************************************************************************/
static void r_protectzone_enable(void)
{
    R_SYSTEM->PRCR = 0xA510;
    R_CPSCU->PRTZCR=0x00000001;
    R_SYSTEM->PRCR = 0xA500;
}

/*******************************************************************************************************************//**
 * r_protectzone_disable() disables the ProtectZone filter.
 **********************************************************************************************************************/
static void r_protectzone_disable(void)
{
    R_SYSTEM->PRCR = 0xA510;
    R_CPSCU->PRTZCR=0x00000000;
    R_SYSTEM->PRCR = 0xA500;
}
