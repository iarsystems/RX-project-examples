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
* Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_mtu3.c
* Version      : Applilet4 for RX23T V1.00.01.01 [30 Oct 2015]
* Device(s)    : R5F523T5AxFM
* Tool-Chain   : IAR Systems ICCRX
* Description  : This file implements device driver for MTU3 module.
* Creation Date: 2016-02-15
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
* Description  : This function initializes the MTU3 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_MTU3_Create(void)
{
    /* Cancel MTU stop state in LPC */
    MSTP(MTU) = 0U;

    /* Enable read/write to MTU registers */
    MTU.TRWERA.BYTE = _01_MTU_RWE_ENABLE;

    /* Stop all channels */
    MTU.TSTRA.BYTE = _00_MTU_CST0_OFF | _00_MTU_CST1_OFF | _00_MTU_CST2_OFF | _00_MTU_CST3_OFF | _00_MTU_CST4_OFF;
    MTU5.TSTR.BYTE = _00_MTU_CSTW5_OFF | _00_MTU_CSTV5_OFF | _00_MTU_CSTU5_OFF;

    /* Set interrupt priority level */
    IPR(MTU4, TGIA4) = _0F_MTU_PRIORITY_LEVEL15;

    /* Channel 4 is used as PWM1 mode */
    MTU.TOERA.BYTE |= _C2_MTU_OE4A_ENABLE;
    MTU4.TCR.BYTE = _01_MTU_PCLK_4 | _00_MTU_CKEG_RISE | _20_MTU_CKCL_A;
    MTU4.TCR2.BYTE = _00_MTU_PCLK_1;
    MTU.TSYRA.BIT.SYNC4 = 0U;
    MTU4.TMDR1.BYTE = _02_MTU_PWM1;
    MTU4.TCNT = 0x0000U;
    MTU4.TADCR.WORD = _0000_MTU_UT4AE_DISABLE | _0000_MTU_UT4BE_DISABLE | _0000_MTU_BF_DISABLE;
    MTU4.TIORH.BYTE = _50_MTU_IOB_HL | _02_MTU_IOA_LH;
    MTU4.TIORL.BYTE = _00_MTU_IOC_DISABLE;
    MTU4.TGRA = _270F_TGRA_VALUE;
    MTU4.TGRB = _03E6_TGRB_VALUE;
    MTU4.TGRC = _0063_TGRC_VALUE;
    MTU4.TGRD = _0063_TGRD_VALUE;
    MTU4.TIER.BYTE = _01_MTU_TGIEA_ENABLE | _00_MTU_TGIEB_DISABLE | _00_MTU_TGIEC_DISABLE | _00_MTU_TGIED_DISABLE | 
                     _00_MTU_TCIEU_DISABLE | _00_MTU_TCIEV_DISABLE | _00_MTU_TTGE2_DISABLE | _00_MTU_TTGE_DISABLE;

    /* Disable read/write to MTU registers */
    MTU.TRWERA.BYTE = _00_MTU_RWE_DISABLE;

    /* Set MTIOC4A pin */
    MPC.P72PFS.BYTE = 0x01U;
    PORT7.PMR.BYTE |= 0x04U;
}
/***********************************************************************************************************************
* Function Name: R_MTU3_C4_Start
* Description  : This function starts MTU3 channel 4 counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_MTU3_C4_Start(void)
{
    /* Enable TGIA4 interrupt in ICU */
    IEN(MTU4,TGIA4) = 1U;

    MTU.TSTRA.BYTE |= _80_MTU_CST4_ON;
}
/***********************************************************************************************************************
* Function Name: R_MTU3_C4_Stop
* Description  : This function stops MTU3 channel 4 counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_MTU3_C4_Stop(void)
{
    /* Disable TGIA4 interrupt in ICU */
    IEN(MTU4,TGIA4) = 0U;

    MTU.TSTRA.BIT.CST4 = 0U;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
