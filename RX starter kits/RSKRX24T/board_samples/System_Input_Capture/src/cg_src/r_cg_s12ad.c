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
* File Name    : r_cg_s12ad.c
* Version      : Applilet4 for RX24T V1.00.01.02 [07 Mar 2016]
* Device(s)    : R5F524TAAxFP
* Tool-Chain   : IAR Systems ICCRX
* Description  : This file implements device driver for S12AD module.
* Creation Date: 2016-07-08
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
#include "r_cg_s12ad.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_S12AD0_Create
* Description  : This function initializes the AD converter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_S12AD0_Create(void)
{
    /* Cancel S12AD module stop state */
    MSTP(S12AD) = 0U;

    /* Disable and clear interrupt flags of S12AD module  */
    S12AD.ADCSR.BIT.ADIE = 0U;
    S12AD.ADCSR.BIT.GBADIE = 0U;
    S12AD.ADGCTRGR.BIT.GCADIE = 0U;

    IEN(S12AD,S12ADI) = 0U;
    IEN(S12AD,GBADI) = 0U;
    IEN(S12AD,GCADI) = 0U;

    /* Set S12AD control registers */
    S12AD.ADDISCR.BYTE = _00_AD_DISCONECT_SETTING;
    S12AD.ADCSR.WORD = _0000_AD_DBLTRIGGER_DISABLE | _1000_AD_SCAN_END_INTERRUPT_ENABLE | _0000_AD_SINGLE_SCAN_MODE;
    S12AD.ADCER.WORD = _0000_AD_AUTO_CLEARING_DISABLE | _0000_AD_RIGHT_ALIGNMENT;
    S12AD.ADADC.BYTE = _00_AD_1_TIME_CONVERSION | _00_AD_ADDITION_MODE;

    /* Set channels and sampling time */
    S12AD.ADANSA0.WORD = _0001_AD_CHANNEL_SELECT_A0;
    S12AD.ADANSA1.WORD = _0000_AD_CHANNEL_SELECT_A1;
    S12AD.ADADS0.WORD = _0000_AD_ADDAVG_CHANNEL_SELECT0;
    S12AD.ADADS1.WORD = _0000_AD_ADDAVG_CHANNEL_SELECT1;
    S12AD.ADSSTR0 = _93_AD_SAMPLING_STATE_0;

    /* Set S12ADI priority level */
    IPR(S12AD,S12ADI) = _0F_AD_PRIORITY_LEVEL15;

    /* Set AN000 pin */
    PORT4.PDR.BYTE &= 0xFEU;
    MPC.P40PFS.BYTE = 0x80U;
}
/***********************************************************************************************************************
* Function Name: R_S12AD0_Start
* Description  : This function starts the AD converter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_S12AD0_Start(void)
{
    IR(S12AD,S12ADI) = 0U;
    IEN(S12AD,S12ADI) = 1U;

    S12AD.ADCSR.BIT.ADST = 1U;
}
/***********************************************************************************************************************
* Function Name: R_S12AD0_Stop
* Description  : This function stops the AD converter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_S12AD0_Stop(void)
{
    S12AD.ADCSR.BIT.ADST = 0U;

    IEN(S12AD,S12ADI) = 0U;
    IR(S12AD,S12ADI) = 0U;
}
/***********************************************************************************************************************
* Function Name: R_S12AD0_Get_ValueResult
* Description  : This function gets result from the AD converter.
* Arguments    : channel -
*                    channel of data register to be read
*                buffer -
*                    buffer pointer
* Return Value : None
***********************************************************************************************************************/
void R_S12AD0_Get_ValueResult(ad_channel_t channel, uint16_t * const buffer)
{
    if (ADSELFDIAGNOSIS == channel)
    {
        *buffer = (uint16_t)(S12AD.ADRD);
    }
    else if (ADCHANNEL0 == channel)
    {
        *buffer = (uint16_t)(S12AD.ADDR0);
    }
    else if (ADCHANNEL1 == channel)
    {   
        *buffer = (uint16_t)(S12AD.ADDR1);
    }
    else if (ADCHANNEL2 == channel)
    {
        *buffer = (uint16_t)(S12AD.ADDR2);
    }
    else if (ADCHANNEL3 == channel)
    {
        *buffer = (uint16_t)(S12AD.ADDR3);
    }
    else if (ADCHANNEL16 == channel)
    {
        *buffer = (uint16_t)(S12AD.ADDR16);
    }
    else if (ADDATADUPLICATION == channel)
    {
        *buffer = (uint16_t)(S12AD.ADDBLDR);
    }
    else if (ADDATADUPLICATIONA == channel)
    {
        *buffer = (uint16_t)(S12AD.ADDBLDRA);
    }
    else if (ADDATADUPLICATIONB == channel)
    {
        *buffer = (uint16_t)(S12AD.ADDBLDRB);
    }
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
