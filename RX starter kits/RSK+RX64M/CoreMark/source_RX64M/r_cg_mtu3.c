/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIESREGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
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
* Copyright (C) 2013, 2014 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_mtu3.c
* Version      : Applilet4 for RX64M V1.01.00.02 [31 Jul 2014]
* Device(s)    : R5F564MLCxFC
* Tool-Chain   : IAR Systems ICCRX
* Description  : This file implements device driver for MTU3 module.
* Creation Date: 09.01.2015
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
#include "r_cg_mtu3.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_MTU3_Create
* Description  : This function initializes the MTU3 Unit0 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_MTU3_Create(void)
{
    /* Cancel MTU stop state in LPC */
    MSTP(MTU3) = 0U;

    /* Enable read/write to MTU registers */
    MTU.TRWERA.BYTE = _01_MTU_RWE_ENABLE;
    MTU.TRWERB.BYTE = _01_MTU_RWE_ENABLE;

    /* Stop all channels */
    MTU.TSTRA.BYTE = _00_MTU_CST0_OFF | _00_MTU_CST1_OFF | _00_MTU_CST2_OFF | _00_MTU_CST3_OFF | _00_MTU_CST4_OFF |
                     _00_MTU_CST8_OFF;
    MTU5.TSTR.BYTE = _00_MTU_CSTW5_OFF | _00_MTU_CSTV5_OFF | _00_MTU_CSTU5_OFF;
    MTU.TSTRB.BIT.CST6 = 0U;
    MTU.TSTRB.BIT.CST7 = 0U;
    ICU.SLIAR210.BYTE = 0x01U;
    /* Set interrupt priority level */
    IPR(PERIA, INTA210) = _0F_MTU_PRIORITY_LEVEL15;

    /* Channel 0 is used as normal mode */
    MTU0.TCR.BYTE = _00_MTU_CKEG_RISE | _20_MTU_CKCL_A;
    MTU0.TCR2.BYTE = _01_MTU_PCLK_2;
    MTU0.TIER.BYTE |= _01_MTU_TGIEA_ENABLE | _00_MTU_TGIEB_DISABLE | _00_MTU_TGIEC_DISABLE | _00_MTU_TGIED_DISABLE |
                      _00_MTU_TCIEU_DISABLE | _00_MTU_TCIEV_DISABLE | _00_MTU_TTGE2_DISABLE | _00_MTU_TTGE_DISABLE;
    MTU0.TIER2.BYTE = _00_MTU_TGIEE_DISABLE | _00_MTU_TGIEF_DISABLE;
    MTU0.TGRA = _EA60_TCNTA_VALUE;
    MTU0.TGRE = _EA60_TCNTE_VALUE;
    MTU0.TGRF = _1770_TCNTF_VALUE;
    MTU0.TMDR1.BYTE = _00_MTU_NORMAL;
    MTU.TSYRA.BYTE |= _00_MTU_SYNC0_OFF;
    MTU0.TIORH.BYTE = _80_MTU_IOB_IR | _00_MTU_IOA_DISABLE;
    MTU0.TIORL.BYTE = _80_MTU_IOD_IR | _08_MTU_IOC_IR;

    /* Disable read/write to MTU registers */
    MTU.TRWERA.BYTE = _00_MTU_RWE_DISABLE;
    MTU.TRWERB.BYTE = _00_MTU_RWE_DISABLE;

    /* Set MTIOC0B pin */
    MPC.P13PFS.BYTE = 0x01U;
    PORT1.PMR.BYTE |= 0x08U;
    /* Set MTIOC0C pin */
    MPC.P32PFS.BYTE = 0x01U;
    PORT3.PMR.BYTE |= 0x04U;
    /* Set MTIOC0D pin */
    MPC.P33PFS.BYTE = 0x01U;
    PORT3.PMR.BYTE |= 0x08U;
}
/***********************************************************************************************************************
* Function Name: R_MTU3_C0_Start
* Description  : This function starts MTU unit0 channel 0 counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_MTU3_C0_Start(void)
{
    /* Enable TGIA0 interrupt in ICU */
    IEN(PERIA, INTA210) = 1U;
    MTU.TSTRA.BYTE |= _01_MTU_CST0_ON;
}
/***********************************************************************************************************************
* Function Name: R_MTU3_C0_Stop
* Description  : This function stops MTU0 channel 0 counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_MTU3_C0_Stop(void)
{
    /* Disable TGIA0 interrupt in ICU */
    IEN(PERIA, INTA210) = 0U;
    MTU.TSTRA.BIT.CST0 = 0U;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
