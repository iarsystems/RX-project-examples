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
* File Name    : r_cg_s12adb.c
* Version      : Applilet4 for RX111 V1.00.00.05 [06 Jun 2013]
* Device(s)    : R5F51115AxFM
* Tool-Chain   : IAR Systems icc78k0r
* Description  : This file implements device driver for S12ADb module.
* Creation Date: 2013-08-15
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
#include "r_cg_s12adb.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */


/***********************************************************************************************************************
* Function Name: R_S12ADb_Create
* Description  : This function initializes the AD converter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_S12ADb_Create(void)
{
    SYSTEM.MSTPCRA.BIT.MSTPA17 = 0U;    /* Cancel S12AD module stop state */
    S12AD.ADCSR.BIT.ADIE = 0U;          /* Disable S12ADI0 interrupt */
    IR(S12AD,S12ADI0) = 0U;             /* Clear S12ADI0 interrupt flag */
    S12AD.ADCSR.WORD |= _0000_AD_GBADI_DISABLE | _0000_AD_DBLTRIGGER_DISABLE | _0000_AD_NORMAL_CONVERSION | 
                        _1000_AD_SCAN_END_INTERRUPT_ENABLE | _4000_AD_CONTINUOUS_SCAN_MODE;
    S12AD.ADCER.WORD = _0000_AD_AUTO_CLEARING_DISABLE | _0000_AD_RIGHT_ALIGNMENT;
    
    /* Set S12ADI0 priority */
    IPR(S12AD,S12ADI0) = _0F_AD_PRIORITY_LEVEL15;

    /* Set channels */
    S12AD.ADANSA.WORD = _0001_AD_CHANNEL_SELECT_A;
    S12AD.ADANSB.WORD = _0000_AD_CHANNEL_SELECT_B;
    S12AD.ADADS.WORD = _0000_AD_VALUE_ADDITION_SELECT;
    S12AD.ADADC.BYTE = _00_AD_1_TIME_CONVERSION;
    S12AD.ADEXICR.WORD = _0002_AD_IREF_ADDITION_ENABLE | _0000_AD_TEMPERATURE_SENSOR_DISABLE | 
                         _0000_AD_IREF_CONVERSION_DISABLE;
    S12AD.ADSSTR0 = _0A_AD_SAMPLING_STATE_0;
    MPC.PJ6PFS.BYTE = _00_AD_REF_HIGH_AVCC0;
    MPC.PJ7PFS.BYTE = _00_AD_REF_LOW_AVSS0;

    /* Set ANO00 pin */
    MPC.P40PFS.BYTE = 0x80U;
    PORT4.PMR.BYTE &= 0xFEU;
    PORT4.PDR.BYTE &= 0xFEU;
}

/***********************************************************************************************************************
* Function Name: R_S12ADb_Start
* Description  : This function starts the AD converter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_S12ADb_Start(void)
{
    IR(S12AD,S12ADI0) = 0U;     /* Clear S12ADI0 interrupt flag */
    IEN(S12AD,S12ADI0) = 1U;    /* Enable S12ADI0 interrupt */
    S12AD.ADCSR.BIT.ADST = 1U;  /* Enable AD conversion */
}

/***********************************************************************************************************************
* Function Name: R_S12ADb_Stop
* Description  : This function stops the AD converter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_S12ADb_Stop(void)
{
    S12AD.ADCSR.BIT.ADST = 0U;  /* Disable AD conversion */
    IEN(S12AD,S12ADI0) = 0U;    /* Disable S12ADI0 interrupt */
    IR(S12AD,S12ADI0) = 0U;     /* Clear S12ADI0 interrupt flag */
}


/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
