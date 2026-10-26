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
* File Name    : Config_MTU9.c
* Version      : 1.5.0
* Device(s)    : R5F572TKCxFB
* Description  : This file implements device driver for Config_MTU9.
* Creation Date: 2019-05-21
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
#include "Config_MTU9.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_MTU9_Create
* Description  : This function initializes the MTU9 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_MTU9_Create(void)
{
    /* Release MTU channel 9 from stop state */
    MSTP(MTU9) = 0U;

    /* Stop MTU channel 9 counter */
    MTU.TSTRA.BIT.CST9 = 0U;

    /* Set A/D conversion signal output for ADSM0, ADSM1 pins */
    MTU.TADSTRGR0.BYTE = _00_MTU_TADSMEN_DISABLE;
    MTU.TADSTRGR1.BYTE = _00_MTU_TADSMEN_DISABLE;

    /* Set TGIA9 interrupt priority level */
    ICU.SLIAR246.BYTE = 0x2FU;
    IPR(PERIA, INTA246) = _0F_MTU_PRIORITY_LEVEL15;

    /* MTU channel 9 is used as PWM mode 2 */
    MTU.TSYRA.BIT.SYNC9 = 0U;
    MTU9.TCR.BYTE = _01_MTU_PCLK_4 | _00_MTU_CKEG_RISE | _20_MTU_CKCL_A;
    MTU9.TCR2.BYTE = _00_MTU_PCLK_1;
    MTU9.TIER.BYTE = _01_MTU_TGIEA_ENABLE | _00_MTU_TGIEB_DISABLE | _00_MTU_TGIEC_DISABLE | _00_MTU_TGIED_DISABLE | 
                     _00_MTU_TCIEV_DISABLE | _80_MTU_TTGE_ENABLE;
    MTU9.TIER2.BYTE = _00_MTU_TGIEE_DISABLE | _00_MTU_TGIEF_DISABLE | _00_MTU_TTGE2_DISABLE;
    MTU9.TMDR1.BYTE = _03_MTU_PWM2;
    MTU9.TIORH.BYTE = _00_MTU_IOA_DISABLE | _50_MTU_IOB_HL;
    MTU9.TIORL.BYTE = _00_MTU_IOC_DISABLE | _00_MTU_IOD_DISABLE;
    MTU9.TGRA = _C34F_TGRA9_VALUE;
    MTU9.TGRB = _0320_TGRB9_VALUE;
    MTU9.TGRC = _0064_TGRC9_VALUE;
    MTU9.TGRD = _0064_TGRD9_VALUE;
    MTU9.TGRE = _0064_TGRE9_VALUE;
    MTU9.TGRF = _0064_TGRF9_VALUE;

    /* Set MTIOC9B pin */
    MPC.PE0PFS.BYTE = 0x01U;
    PORTE.PMR.BYTE |= 0x01U;

    R_Config_MTU9_Create_UserInit();
}

/***********************************************************************************************************************
* Function Name: R_Config_MTU9_Start
* Description  : This function starts the MTU9 channel counter
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_MTU9_Start(void)
{
    /* Enable TGIA9 interrupt in ICU */
    IEN(PERIA, INTA246) = 1U;
    
    /* Start MTU channel 9 counter */
    MTU.TSTRA.BIT.CST9 = 1U;
}

/***********************************************************************************************************************
* Function Name: R_Config_MTU9_Stop
* Description  : This function stops the MTU9 channel counter
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_MTU9_Stop(void)
{
    /* Disable TGIA9 interrupt in ICU */
    IEN(PERIA, INTA246) = 0U;
    
    /* Stop MTU channel 9 counter */
    MTU.TSTRA.BIT.CST9 = 0U;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
