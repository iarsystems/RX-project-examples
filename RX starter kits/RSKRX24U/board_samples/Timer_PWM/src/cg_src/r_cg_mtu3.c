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
* Copyright (C) 2015, 2017 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_mtu3.c
* Version      : Applilet4 for RX24U V1.03.01.04 [02 Nov 2017]
* Device(s)    : R5F524UEAxFB
* Tool-Chain   : IAR Systems ICCRX
* Description  : This file implements device driver for MTU3 module.
* Creation Date: 2018-06-13
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
    MTU.TRWERA.BIT.RWE = 1U;
    MTU.TRWERB.BIT.RWE = 1U;

    /* Stop all channels */
    MTU.TSTRA.BYTE = _00_MTU_CST0_OFF | _00_MTU_CST1_OFF | _00_MTU_CST2_OFF | _00_MTU_CST3_OFF | _00_MTU_CST4_OFF | 
                     _00_MTU_CST9_OFF;
    MTU5.TSTR.BYTE = _00_MTU_CSTW5_OFF | _00_MTU_CSTV5_OFF | _00_MTU_CSTU5_OFF;
    MTU.TSTRB.BYTE = _00_MTU_CST6_OFF | _00_MTU_CST7_OFF;

    /* Set channel 9 interrupt priority level */
    IPR(MTU9, TGIA9) = _0F_MTU_PRIORITY_LEVEL15;

    /* Channel 9 is used as PWM1 mode */
    MTU.TSYRA.BIT.SYNC9 = 0U;
    MTU9.TCR.BYTE = _01_MTU_PCLK_4 | _00_MTU_CKEG_RISE | _20_MTU_CKCL_A;
    MTU9.TCR2.BYTE = _00_MTU_PCLK_1;
    MTU9.TIER.BYTE = _01_MTU_TGIEA_ENABLE | _00_MTU_TGIEB_DISABLE | _00_MTU_TGIEC_DISABLE | _00_MTU_TGIED_DISABLE | 
                     _00_MTU_TCIEU_DISABLE | _00_MTU_TCIEV_DISABLE | _00_MTU_TTGE_DISABLE;
    MTU9.TIER2.BYTE = _00_MTU_TGIEE_DISABLE | _00_MTU_TGIEF_DISABLE | _00_MTU_TTGE2_DISABLE;
    MTU9.TMDR1.BYTE = _02_MTU_PWM1;
    MTU9.TIORH.BYTE = _50_MTU_IOB_HL | _02_MTU_IOA_LH;
    MTU9.TIORL.BYTE = _00_MTU_IOC_DISABLE;
    MTU9.TGRA = _4E1F_TGRA9_VALUE;
    MTU9.TGRB = _07D0_TGRB9_VALUE;
    MTU9.TGRC = _0000_TGRC9_VALUE;
    MTU9.TGRD = _0000_TGRD9_VALUE;
    MTU9.TGRE = _0000_TGRE9_VALUE;
    MTU9.TGRF = _0000_TGRF9_VALUE;

    /* Disable read/write to MTU registers */
    MTU.TRWERA.BIT.RWE = 0U;
    MTU.TRWERB.BIT.RWE = 0U;

    /* Set MTIOC9A pin */
    MPC.P21PFS.BYTE = 0x01U;
    PORT2.PMR.BYTE |= 0x02U;
}
/***********************************************************************************************************************
* Function Name: R_MTU3_C9_Start
* Description  : This function starts MTU3 channel 9 counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_MTU3_C9_Start(void)
{
    /* Enable TGIA9 interrupt in ICU */
    IEN(MTU9,TGIA9) = 1U;

    /* Start count operation */
    MTU.TSTRA.BYTE |= _10_MTU_CST9_ON;
}
/***********************************************************************************************************************
* Function Name: R_MTU3_C9_Stop
* Description  : This function stops MTU3 channel 9 counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_MTU3_C9_Stop(void)
{
    /* Disable TGIA9 interrupt in ICU */
    IEN(MTU9,TGIA9) = 0U;

    /* Stop counting */
    MTU.TSTRA.BIT.CST9 = 0U;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
