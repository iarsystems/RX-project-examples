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
* Copyright (C) 2016, 2017 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : Config_TMR2_TMR3.c
* Version      : 1.3.0
* Device(s)    : R5F566TEAxFP
* Description  : This file implements device driver for Config_TMR2_TMR3.
* Creation Date: 2018-11-20
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
#include "Config_TMR2_TMR3.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_TMR2_TMR3_Create
* Description  : This function initializes the TMR2 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_TMR2_TMR3_Create(void)
{
    /* Disable TMR2 interrupts */
    IEN(TMR2, CMIA2) = 0U;

    /* Cancel TMR module stop state */
    MSTP(TMR2) = 0U;

    /* Set timer counter control setting */
    TMR2.TCCR.BYTE = _18_TMR_CLK_TMR1_OVRF | _00_TMR_CLK_DISABLED;

    /* Set counter clear and interrupt */
    TMR2.TCR.BYTE = _40_TMR_CMIA_INT_ENABLE | _08_TMR_CNT_CLR_COMP_MATCH_A | _00_TMR_CMIB_INT_DISABLE | 
                    _00_TMR_OVI_INT_DISABLE;

    /* Set A/D trigger and output */
    TMR2.TCSR.BYTE = _00_TMR_AD_TRIGGER_DISABLE | _E0_TMR02_TCSR_DEFAULT;

    /* Set compare match value */
    TMR23.TCORA = _4C4A_TMR23_COMP_MATCH_VALUE_A;
    TMR23.TCORB = _0186_TMR23_COMP_MATCH_VALUE_B;

    /* Configure TMR2 interrupts */
    IPR(TMR2, CMIA2) = _0F_TMR_PRIORITY_LEVEL15;

    R_Config_TMR2_TMR3_Create_UserInit();
}

/***********************************************************************************************************************
* Function Name: R_Config_TMR2_TMR3_Start
* Description  : This function starts the TMR2 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_TMR2_TMR3_Start(void)
{
    /* Enable TMR2 interrupt */
    IR(TMR2, CMIA2) = 0U;
    IEN(TMR2, CMIA2) = 1U;

    /* Start counting */
    TMR3.TCCR.BYTE = _08_TMR_CLK_SRC_PCLK | _05_TMR_PCLK_DIV_1024;
}

/***********************************************************************************************************************
* Function Name: R_Config_TMR2_TMR3_Stop
* Description  : This function stops the TMR2 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_TMR2_TMR3_Stop(void)
{
    /* Disable TMR2 interrupt */
    IEN(TMR2, CMIA2) = 0U;

    /* Stop counting */
    TMR3.TCCR.BYTE = _00_TMR_CLK_DISABLED;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

