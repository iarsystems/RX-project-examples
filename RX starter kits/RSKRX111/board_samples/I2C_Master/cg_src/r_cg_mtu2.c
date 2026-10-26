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
* File Name    : r_cg_mtu2.c
* Version      : Applilet4 for RX111 V1.00.00.05 [06 Jun 2013]
* Device(s)    : R5F51115AxFM
* Tool-Chain   : CCRX
* Description  : This file implements device driver for MTU2 module.
* Creation Date: 19/06/2013
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_mtu2.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_MTU2_U0_Create
* Description  : This function initializes the MTU2 Unit0 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_MTU2_U0_Create(void)
{
    /* Cancel MTU stop state in LPC */
    SYSTEM.MSTPCRA.BIT.MSTPA9 = 0U;
    MTU.TRWER.BYTE = _01_MTU_RWE_ENABLE;

    /* Stop all channels */
    MTU.TSTR.BYTE = _00_MTU_CST0_OFF | _00_MTU_CST1_OFF | _00_MTU_CST2_OFF | _00_MTU_CST3_OFF | _00_MTU_CST4_OFF;
    MTU5.TSTR.BYTE = _00_MTU_CSTW5_OFF | _00_MTU_CSTV5_OFF | _00_MTU_CSTU5_OFF;

    /* Set interrupt priority level */
    IPR(MTU0, TGIA0) |= _0F_MTU_PRIORITY_LEVEL15;
    MTU0.TCR.BYTE = _03_MTU_PCLK_64 | _00_MTU_CKEG_RISE | _20_MTU_CKCL_A;
    MTU0.TIER.BYTE |= _01_MTU_TGIEA_ENABLE | _00_MTU_TGIEB_DISABLE | _00_MTU_TGIEC_DISABLE | _00_MTU_TGIED_DISABLE | 
                      _00_MTU_TCIEU_DISABLE | _00_MTU_TCIEV_DISABLE | _00_MTU_TTGE2_DISABLE | _00_MTU_TTGE_DISABLE;
    MTU0.TIER2.BYTE = _00_MTU_TGIEE_DISABLE | _00_MTU_TGIEF_DISABLE;
    MTU0.TIORH.BYTE = _80_MTU_IOB_IR | _01_MTU_IOA_LL;
    MTU0.TIORL.BYTE = _C0_MTU_IOD_EX | _08_MTU_IOC_IR;
    MTU0.TGRA = _61A7_TCNTA_VALUE;
    MTU0.TGRE = _0018_TCNTE_VALUE;
    MTU0.TGRF = _0018_TCNTF_VALUE;

    /* Channel 0 is used as normal mode */
    MTU0.TMDR.BYTE = _00_MTU_NORMAL;
    MTU.TSYR.BYTE |= _00_MTU_SYNC0_OFF;
    MTU.TRWER.BYTE = _00_MTU_RWE_DISABLE;

    /* Set MTIOC0B pin */
    MPC.P15PFS.BYTE = 0x01U;
    PORT1.PDR.BYTE &= 0xDFU;
    PORT1.PMR.BYTE |= 0x20U;
    /* Set MTIOC0A pin */
    MPC.PE3PFS.BYTE = 0x03U;
    PORTE.PDR.BYTE |= 0x08U;
    PORTE.PMR.BYTE |= 0x08U;
    /* Set MTIOC0C pin */
    MPC.P32PFS.BYTE = 0x01U;
    PORT3.PDR.BYTE &= 0xFBU;
    PORT3.PMR.BYTE |= 0x04U;
}
/***********************************************************************************************************************
* Function Name: R_MTU2_U0_C0_Start
* Description  : This function starts MTU unit0 channel 0 counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_MTU2_U0_C0_Start(void)
{
    /* Enable TGIA0 interrupt in ICU */
    IEN(MTU0,TGIA0) = 1U;
    MTU.TSTR.BYTE |= _01_MTU_CST0_ON;
}
/***********************************************************************************************************************
* Function Name: R_MTU2_U0_C0_Stop
* Description  : This function stops TAU0 channel 0 counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_MTU2_U0_C0_Stop(void)
{
    /* Disable TGIA0 interrupt in ICU */
    IEN(MTU0,TGIA0) = 0U;
    MTU.TSTR.BIT.CST0 = 0U;
}







/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
