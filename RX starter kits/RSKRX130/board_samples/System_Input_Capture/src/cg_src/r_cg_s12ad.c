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
* Version      : Applilet4 for RX130 V1.00.01.02 [07 Mar 2016]
* Device(s)    : R5F51305AxFN
* Tool-Chain   : IAR Systems ICCRX
* Description  : This file implements device driver for S12AD module.
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
* Function Name: R_S12AD_Create
* Description  : This function initializes the AD converter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_S12AD_Create(void)
{
    /* Cancel S12AD module stop state */
    MSTP(S12AD) = 0U;

    /* Disable and clear interrupt flags */
    S12AD.ADCSR.BIT.ADIE = 0U;
    IEN(S12AD, S12ADI0) = 0U;

    /* Set S12AD control registers */
    S12AD.ADCSR.WORD = _0000_AD_DBLTRIGGER_DISABLE | _1000_AD_SCAN_END_INTERRUPT_ENABLE | _0000_AD_SINGLE_SCAN_MODE;

    S12AD.ADHVREFCNT.BYTE |= _00_AD_HIGH_POTENTIAL_AVCC0 | _00_AD_LOW_POTENTIAL_AVSS0;
    S12AD.ADCER.WORD = _0000_AD_AUTO_CLEARING_DISABLE | _0000_AD_RIGHT_ALIGNMENT;
    S12AD.ADADC.BYTE = _00_AD_1_TIME_CONVERSION | _00_AD_ADDITION_MODE;

    /* Set channels and sampling time */
    S12AD.ADANSA0.WORD = _0001_AD_CHANNEL_SELECT_A0;
    S12AD.ADANSA1.WORD = _0000_AD_CHANNEL_SELECT_A1;
    S12AD.ADADS0.WORD = _0000_AD_ADDAVG_CHANNEL_SELECT0;
    S12AD.ADADS1.WORD = _0000_AD_ADDAVG_CHANNEL_SELECT1;
    S12AD.ADSSTR0 = _75_AD_SAMPLING_STATE_0;

    /* Set ELC scan end event generation condition */
    S12AD.ADELCCR.BYTE = _02_ALL_SCAN_COMPLETION;

    /* Set S12ADI0 priority level */
    IPR(S12AD, S12ADI0) = _0F_AD_PRIORITY_LEVEL15;

    /* Set AN000 pin */
    PORT4.PMR.BYTE &= 0xFEU;
    PORT4.PDR.BYTE &= 0xFEU;
    MPC.P40PFS.BYTE = 0x80U;
}

/***********************************************************************************************************************
* Function Name: R_S12AD_Start
* Description  : This function starts the AD converter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_S12AD_Start(void)
{
    IR(S12AD, S12ADI0) = 0U;
    IEN(S12AD, S12ADI0) = 1U;

    S12AD.ADCSR.BIT.ADST = 1U;
}

/***********************************************************************************************************************
* Function Name: R_S12AD_Stop
* Description  : This function stops the AD converter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_S12AD_Stop(void)
{
    S12AD.ADCSR.BIT.ADST = 0U;

    IEN(S12AD, S12ADI0) = 0U;
    IR(S12AD, S12ADI0) = 0U;
}

/***********************************************************************************************************************
* Function Name: R_S12AD_Get_ValueResult
* Description  : This function gets result from the AD converter.
* Arguments    : channel -
*                    channel of data register to be read
*                buffer -
*                    buffer pointer
* Return Value : None
***********************************************************************************************************************/
void R_S12AD_Get_ValueResult(ad_channel_t channel, uint16_t * const buffer)
{
    if (channel == ADSELFDIAGNOSIS)
    {
        *buffer = (uint16_t)(S12AD.ADRD.WORD);
    }
    else if (channel == ADCHANNEL0)
    {
        *buffer = (uint16_t)(S12AD.ADDR0);
    }
    else if (channel == ADCHANNEL1)
    {
        *buffer = (uint16_t)(S12AD.ADDR1);
    }
    else if (channel == ADCHANNEL2)
    {
        *buffer = (uint16_t)(S12AD.ADDR2);
    }
    else if (channel == ADCHANNEL3)
    {
        *buffer = (uint16_t)(S12AD.ADDR3);
    }
    else if (channel == ADCHANNEL4)
    {
        *buffer = (uint16_t)(S12AD.ADDR4);
    }
    else if (channel == ADCHANNEL5)
    {
        *buffer = (uint16_t)(S12AD.ADDR5);
    }
    else if (channel == ADCHANNEL6)
    {
        *buffer = (uint16_t)(S12AD.ADDR6);
    }
    else if (channel == ADCHANNEL7)
    {
        *buffer = (uint16_t)(S12AD.ADDR7);
    }
    else if (channel == ADCHANNEL16)
    {
        *buffer = (uint16_t)(S12AD.ADDR16);
    }
    else if (channel == ADCHANNEL17)
    {
        *buffer = (uint16_t)(S12AD.ADDR17);
    }
    else if (channel == ADCHANNEL18)
    {
        *buffer = (uint16_t)(S12AD.ADDR18);
    }
    else if (channel == ADCHANNEL19)
    {
        *buffer = (uint16_t)(S12AD.ADDR19);
    }
    else if (channel == ADCHANNEL20)
    {
        *buffer = (uint16_t)(S12AD.ADDR20);
    }
    else if (channel == ADCHANNEL21)
    {
        *buffer = (uint16_t)(S12AD.ADDR21);
    }
    else if (channel == ADCHANNEL24)
    {
        *buffer = (uint16_t)(S12AD.ADDR24);
    }
    else if (channel == ADCHANNEL25)
    {
        *buffer = (uint16_t)(S12AD.ADDR25);
    }
    else if (channel == ADCHANNEL26)
    {
        *buffer = (uint16_t)(S12AD.ADDR26);
    }
    else if (channel == ADTEMPSENSOR)
    {
        *buffer = (uint16_t)(S12AD.ADTSDR);
    }
    else if (channel == ADINTERREFVOLT)
    {
        *buffer = (uint16_t)(S12AD.ADOCDR);
    }
    else if (channel == ADDATADUPLICATION)
    {
        *buffer = (uint16_t)(S12AD.ADDBLDR);
    }
}

/* Start user code for adding. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_S12AD0_SWTriggerStart
* Description  : This function starts the AD0 converter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_S12AD0_SWTriggerStart(void)
{
    S12AD.ADEXICR.WORD = 0U;;
    S12AD.ADANSA0.WORD = _0001_AD_CHANNEL_SELECT_A0;
    IR(S12AD, S12ADI0) = 0U;
    IEN(S12AD, S12ADI0) = 1U;

    S12AD.ADCSR.BIT.ADST = 1U;
}
/***********************************************************************************************************************
End of function R_S12AD0_SWTriggerStart
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: R_S12AD_TempSWTriggerStart
* Description  : This function starts the AD0 converter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_S12AD_TempSWTriggerStart(void)
{
    S12AD.ADEXICR.WORD = _0100_AD_TEMP_GROUPA_ENABLE;
    S12AD.ADANSA0.WORD = 0U;
    S12AD.ADSSTRT = (0xFFU);
    IR(S12AD, S12ADI0) = 0U;
    IEN(S12AD, S12ADI0) = 1U;

    S12AD.ADCSR.BIT.ADST = 1U;
}
/***********************************************************************************************************************
* End of function R_S12AD_TempSWTriggerStart
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: R_S12AD_SWTriggerStop
* Description  : This function stops the AD0 converter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_S12AD_SWTriggerStop(void)
{
    S12AD.ADCSR.BIT.ADST = 0U;

    IEN(S12AD, S12ADI0) = 0U;
    IR(S12AD, S12ADI0) = 0U;
}
/***********************************************************************************************************************
* End of function R_S12AD_SWTriggerStop
***********************************************************************************************************************/

/* End user code. Do not edit comment generated here */
