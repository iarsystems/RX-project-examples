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
* Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_mtu2.c
* Version      : Applilet4 for RX113 V1.01.00.02 [31 Jul 2014]
* Device(s)    : R5F51138AxFP
* Tool-Chain   : IAR Systems ICCRX
* Description  : This file implements device driver for MTU2 module.
* Creation Date: 2015-01-21
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
* Function Name: R_MTU2_Create
* Description  : This function initializes the MTU2 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_MTU2_Create(void)
{
    /* Cancel MTU stop state in LPC */
    MSTP(MTU) = 0U;
    MTU.TRWER.BYTE = _01_MTU_RWE_ENABLE;

    /* Stop all channels */
    MTU.TSTR.BYTE = _00_MTU_CST0_OFF | _00_MTU_CST1_OFF | _00_MTU_CST2_OFF | _00_MTU_CST3_OFF | _00_MTU_CST4_OFF;
    MTU5.TSTR.BYTE = _00_MTU_CSTW5_OFF | _00_MTU_CSTV5_OFF | _00_MTU_CSTU5_OFF;

    /* Set interrupt priority level */
    IPR(MTU4, TGIA4) |= _0F_MTU_PRIORITY_LEVEL15;

    /* Channel 4 is used as PWM1 mode */
    MTU4.TCR.BYTE = _00_MTU_PCLK_1 | _00_MTU_CKEG_RISE | _20_MTU_CKCL_A;
    MTU4.TGRA = _7D00_TGRA_VALUE;
    MTU4.TGRB = _3E80_TGRB_VALUE;
    MTU4.TIER.BYTE |= _01_MTU_TGIEA_ENABLE | _00_MTU_TGIEB_DISABLE | _00_MTU_TGIEC_DISABLE | _00_MTU_TGIED_DISABLE | 
                      _00_MTU_TCIEU_DISABLE | _00_MTU_TCIEV_DISABLE | _00_MTU_TTGE2_DISABLE | _00_MTU_TTGE_DISABLE;
    MTU4.TGRC = _0000_TGRC_VALUE;
    MTU4.TGRD = _0000_TGRD_VALUE;
    MTU.TSYR.BYTE |= _00_MTU_SYNC4_OFF;
    MTU4.TCNT = 0x0000U;
    MTU4.TADCR.WORD = _0000_MTU_UT4AE_DISABLE | _0000_MTU_UT4BE_DISABLE | _0000_MTU_BF_DISABLE;
    MTU4.TMDR.BYTE |= _02_MTU_PWM1;
    MTU.TOER.BYTE |= _C2_MTU_OE4A_ENABLE;
    MTU4.TIORH.BYTE = _50_MTU_IOB_HL | _07_MTU_IOA_HT;
    MTU4.TIORL.BYTE = _00_MTU_IOC_DISABLE;
    MTU.TRWER.BYTE = _00_MTU_RWE_DISABLE;

    /* Set MTIOC4A pin */
    MPC.P24PFS.BYTE = 0x01U;
    PORT2.PMR.BYTE |= 0x10U;
}
/***********************************************************************************************************************
* Function Name: R_MTU2_C4_Start
* Description  : This function starts MTU2 channel 4 counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_MTU2_C4_Start(void)
{
    /* Enable TGIA4 interrupt in ICU */
    IEN(MTU4,TGIA4) = 1U;
    MTU.TSTR.BYTE |= _80_MTU_CST4_ON;
}
/***********************************************************************************************************************
* Function Name: R_MTU2_C4_Stop
* Description  : This function stops MTU2 channel 4 counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_MTU2_C4_Stop(void)
{
    /* Disable TGIA4 interrupt in ICU */
    IEN(MTU4,TGIA4) = 0U;
    MTU.TSTR.BIT.CST4 = 0U;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
