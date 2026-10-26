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
* Copyright (C) 2015, 2016 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_mtu2.c
* Version      : Applilet4 for RX130 V1.00.01.02 [07 Mar 2016]
* Device(s)    : R5F51305AxFN
* Tool-Chain   : IAR Systems ICCRX
* Description  : This file implements device driver for MTU2 module.
* Creation Date: 2016-09-23
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

    /* Enable read/write to MTU registers */
    MTU.TRWER.BYTE = _01_MTU_RWE_ENABLE;

    /* Stop all channels */
    MTU.TSTR.BYTE = _00_MTU_CST0_OFF | _00_MTU_CST1_OFF | _00_MTU_CST2_OFF | _00_MTU_CST3_OFF | _00_MTU_CST4_OFF;
    MTU5.TSTR.BYTE = _00_MTU_CSTW5_OFF | _00_MTU_CSTV5_OFF | _00_MTU_CSTU5_OFF;

    /* Set interrupt priority level */
    IPR(MTU1, TGIA1) = _0F_MTU_PRIORITY_LEVEL15;

    /* Channel 1 is used as PWM2 mode */
    MTU1.TCR.BYTE = _01_MTU_PCLK_4 | _00_MTU_CKEG_RISE | _20_MTU_CKCL_A;
    MTU.TSYR.BIT.SYNC1 = 0U;
    MTU1.TMDR.BYTE |= _03_MTU_PWM2;
    MTU1.TIOR.BYTE = _50_MTU_IOB_HL | _00_MTU_IOA_DISABLE;
    MTU1.TGRA = _1F3F_TGRA_VALUE;
    MTU1.TGRB = _0320_TGRB_VALUE;
    MTU1.TIER.BYTE = _01_MTU_TGIEA_ENABLE | _00_MTU_TGIEB_DISABLE | _00_MTU_TGIEC_DISABLE | _00_MTU_TGIED_DISABLE | 
                     _00_MTU_TCIEU_DISABLE | _00_MTU_TCIEV_DISABLE | _00_MTU_TTGE_DISABLE;

    /* Disable read/write to MTU registers */
    MTU.TRWER.BYTE = _00_MTU_RWE_DISABLE;

    /* Set MTIOC1B pin */
    MPC.P21PFS.BYTE = 0x01U;
    PORT2.PMR.BYTE |= 0x02U;
}
/***********************************************************************************************************************
* Function Name: R_MTU2_C1_Start
* Description  : This function starts MTU2 channel 1 counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_MTU2_C1_Start(void)
{

    /* Enable TGIA1 interrupt in ICU */
    IEN(MTU1,TGIA1) = 1U;
    MTU.TSTR.BYTE |= _02_MTU_CST1_ON;
}
/***********************************************************************************************************************
* Function Name: R_MTU2_C1_Stop
* Description  : This function stops MTU2 channel 1 counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_MTU2_C1_Stop(void)
{

    /* Disable TGIA1 interrupt in ICU */
    IEN(MTU1,TGIA1) = 0U;
    MTU.TSTR.BIT.CST1 = 0U;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
