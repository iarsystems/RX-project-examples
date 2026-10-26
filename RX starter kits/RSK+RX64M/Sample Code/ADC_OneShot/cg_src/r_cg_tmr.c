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
* Copyright (C) 2013, 2014 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_tmr.c
* Version      : Applilet4 for RX64M V1.00.00.03 [28 Feb 2014]
* Device(s)    : R5F564MLCxFC
* Tool-Chain   : IAR Systems ICCRX
* Description  : This file implements device driver for TMR module.
* Creation Date: 28.03.2014
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
#include "r_cg_tmr.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_TMR_Create
* Description  : This function initializes the TMR module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TMR_Create(void)
{
    /* Disable TMR0 interrupts */
    IEN(PERIB, INTB128) = 0U;

    /* Cancel TMR module stop state */
    MSTP(TMR01) = 0U;

    /* Set counter clear and interrupt */
    TMR0.TCR.BYTE = _80_TMR_CMIB_INT_ENABLE | _10_TMR_CNT_CLR_COMP_MATCH_B;

    /* Set A/D trigger and output */
    TMR0.TCSR.BYTE = _10_TMR_AD_TRIGGER_ENABLE | _E0_TMR02_TCSR_DEFAULT;

    /* Set compare match value */
    TMR0.TCORA = _91_TMR0_COMP_MATCH_VALUE_A;
    TMR0.TCORB = _DA_TMR0_COMP_MATCH_VALUE_B;

    /* Configure TMR0 interrupts */
    ICU.SLIBXR128.BYTE = 0x04U;
    IPR(PERIB, INTB128) = _09_TMR_PRIORITY_LEVEL9;

}
/***********************************************************************************************************************
* Function Name: R_TMR0_Start
* Description  : This function starts TMR0 channel.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TMR0_Start(void)
{
    /* Enable TMR0 interrupts */
    IR(PERIB, INTB128) = 0U;
    IEN(PERIB, INTB128) = 1U;

    /* Start counting */
    TMR0.TCCR.BYTE = _08_TMR_CLK_SRC_PCLK | _06_TMR_PCLK_DIV_8192;
}
/***********************************************************************************************************************
* Function Name: R_TMR0_Stop
* Description  : This function starts TMR0 channel.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TMR0_Stop(void)
{
    /* Disable TMR0 interrupts */
    IEN(PERIB, INTB128) = 0U;

    /* Stop counting */
    TMR0.TCCR.BYTE = _00_TMR_CLK_DISABLED;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
