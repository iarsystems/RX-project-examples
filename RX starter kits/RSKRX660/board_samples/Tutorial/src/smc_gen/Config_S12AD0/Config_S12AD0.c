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
* Copyright (C) 2022 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name        : Config_S12AD0.c
* Component Version: 2.5.0
* Device(s)        : R5F56609HxFB
* Description      : This file implements device driver for Config_S12AD0.
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
#include "Config_S12AD0.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_S12AD0_Create
* Description  : This function initializes the S12AD0 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_S12AD0_Create(void)
{
    /* Cancel S12AD0 module stop state */
    MSTP(S12AD) = 0U;

    /* Disable and clear interrupt flags of S12AD0 module */
    S12AD.ADCSR.BIT.ADIE = 0U;
    S12AD.ADCMPCR.BIT.CMPAIE = 0U;
    S12AD.ADCMPCR.BIT.CMPBIE = 0U;
    IEN(S12ADC0, S12ADI0) = 0U;

    /* Set S12AD0 control registers */
    S12AD.ADCSR.WORD = _0000_AD_DBLTRIGGER_DISABLE | _0100_AD_ASYNC_TRIGGER | _0200_AD_SYNCASYNCTRG_ENABLE | 
                       _0000_AD_SINGLE_SCAN_MODE;

    /* Set AD conversion start trigger sources */
    S12AD.ADSTRGR.WORD = _0000_AD_TRSA_ADTRG;
    S12AD.ADADC.BYTE = _00_AD_1_TIME_CONVERSION | _00_AD_ADDITION_MODE;
    S12AD.ADDISCR.BYTE = _00_AD_DISCONECT_UNUSED;

    /* Set channels and sampling time */
    S12AD.ADSSTR0 = _1B_AD0_SAMPLING_STATE_0;
    S12AD.ADANSA0.WORD = _0001_AD_ANx00_USED;
    S12AD.ADVREFCR.BYTE = _00_VREFSEL_AVCC0_AVSS0;
    S12AD.ADCER.WORD = _0000_AD_AUTO_CLEARING_DISABLE | _0000_AD_SELFTDIAGST_DISABLE | _0000_AD_RIGHT_ALIGNMENT;
    S12AD.ADELCCR.BYTE = _02_ALL_SCAN_COMPLETION;
    S12AD.ADCSR.WORD |= _1000_AD_SCAN_END_INTERRUPT_ENABLE;

    /* Set compare control register */
    S12AD.ADCMPCR.WORD = _0000_AD_WINDOWB_DISABLE | _0000_AD_WINDOWA_DISABLE | _0000_AD_WINDOWFUNCTION_DISABLE;

    /* Set interrupt and priority level */
    IPR(S12ADC0, S12ADI0) = _0F_AD_PRIORITY_LEVEL15;

    /* Set AN000 pin */
    PORT4.PMR.BYTE &= 0xFEU;
    PORT4.PDR.BYTE &= 0xFEU;
    MPC.P40PFS.BYTE = 0x80U;

    /* Set ADTRG0# pin */
    MPC.P07PFS.BYTE = 0x09U;
    PORT0.PMR.BYTE |= 0x80U;

    R_Config_S12AD0_Create_UserInit();
}

/***********************************************************************************************************************
* Function Name: R_Config_S12AD0_Start
* Description  : This function starts the AD0 converter
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_S12AD0_Start(void)
{
    IR(S12ADC0, S12ADI0) = 0U;
    IEN(S12ADC0, S12ADI0) = 1U;
    S12AD.ADCSR.BIT.TRGE = 1U;
}

/***********************************************************************************************************************
* Function Name: R_Config_S12AD0_Stop
* Description  : This function stops the AD0 converter
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_S12AD0_Stop(void)
{
    S12AD.ADCSR.BIT.TRGE = 0U;
    S12AD.ADCSR.BIT.ADST = 0U;
    IR(S12ADC0, S12ADI0) = 0U;
    IEN(S12ADC0, S12ADI0) = 0U;
}

/***********************************************************************************************************************
* Function Name: R_Config_S12AD0_Get_ValueResult
* Description  : This function gets result from the AD0 converter
* Arguments    : channel -
*                    channel of data register to be read
*                buffer -
*                    buffer pointer
* Return Value : None
***********************************************************************************************************************/

void R_Config_S12AD0_Get_ValueResult(ad_channel_t channel, uint16_t * const buffer)
{
    switch (channel)
    {
        case ADSELFDIAGNOSIS:
        {
            *buffer = (uint16_t)(S12AD.ADRD.WORD);
            break;
        }
        case ADCHANNEL0:
        {
            *buffer = (uint16_t)(S12AD.ADDR0);
            break;
        }
        case ADCHANNEL1:
        {
            *buffer = (uint16_t)(S12AD.ADDR1);
            break;
        }
        case ADCHANNEL2:
        {
            *buffer = (uint16_t)(S12AD.ADDR2);
            break;
        }
        case ADCHANNEL3:
        {
            *buffer = (uint16_t)(S12AD.ADDR3);
            break;
        }
        case ADCHANNEL4:
        {
            *buffer = (uint16_t)(S12AD.ADDR4);
            break;
        }
        case ADCHANNEL5:
        {
            *buffer = (uint16_t)(S12AD.ADDR5);
            break;
        }
        case ADCHANNEL6:
        {
            *buffer = (uint16_t)(S12AD.ADDR6);
            break;
        }
        case ADCHANNEL7:
        {
            *buffer = (uint16_t)(S12AD.ADDR7);
            break;
        }
        case ADCHANNEL8:
        {
            *buffer = (uint16_t)(S12AD.ADDR8);
            break;
        }
        case ADCHANNEL9:
        {
            *buffer = (uint16_t)(S12AD.ADDR9);
            break;
        }
        case ADCHANNEL10:
        {
            *buffer = (uint16_t)(S12AD.ADDR10);
            break;
        }
        case ADCHANNEL11:
        {
            *buffer = (uint16_t)(S12AD.ADDR11);
            break;
        }
        case ADCHANNEL12:
        {
            *buffer = (uint16_t)(S12AD.ADDR12);
            break;
        }
        case ADCHANNEL13:
        {
            *buffer = (uint16_t)(S12AD.ADDR13);
            break;
        }
        case ADCHANNEL14:
        {
            *buffer = (uint16_t)(S12AD.ADDR14);
            break;
        }
        case ADCHANNEL15:
        {
            *buffer = (uint16_t)(S12AD.ADDR15);
            break;
        }
        case ADCHANNEL16:
        {
            *buffer = (uint16_t)(S12AD.ADDR16);
            break;
        }
        case ADCHANNEL17:
        {
            *buffer = (uint16_t)(S12AD.ADDR17);
            break;
        }
        case ADCHANNEL18:
        {
            *buffer = (uint16_t)(S12AD.ADDR18);
            break;
        }
        case ADCHANNEL19:
        {
            *buffer = (uint16_t)(S12AD.ADDR19);
            break;
        }
        case ADCHANNEL20:
        {
            *buffer = (uint16_t)(S12AD.ADDR20);
            break;
        }
        case ADCHANNEL21:
        {
            *buffer = (uint16_t)(S12AD.ADDR21);
            break;
        }
        case ADCHANNEL22:
        {
            *buffer = (uint16_t)(S12AD.ADDR22);
            break;
        }
        case ADCHANNEL23:
        {
            *buffer = (uint16_t)(S12AD.ADDR23);
            break;
        }
        case ADTEMPSENSOR:
        {
            *buffer = (uint16_t)(S12AD.ADTSDR);
            break;
        }
        case ADINTERREFVOLT:
        {
            *buffer = (uint16_t)(S12AD.ADOCDR);
            break;
        }
        case ADDATADUPLICATIONA:
        {
            *buffer = (uint16_t)(S12AD.ADDBLDRA);
            break;
        }
        case ADDATADUPLICATIONB:
        {
            *buffer = (uint16_t)(S12AD.ADDBLDRB);
            break;
        }
        case ADDATADUPLICATION:
        {
            *buffer = (uint16_t)(S12AD.ADDBLDR);
            break;
        }
        default:
        {
            break;
        }
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
    /* Clear interrupt flags */
    IR(S12ADC0, S12ADI0) = 0U;

    /* Enable S12ADI interrupt */
    IEN(S12ADC0, S12ADI0) = 1U;

    /* A/D conversion start */
    S12AD.ADCSR.BIT.ADST = 1U;
}

/***********************************************************************************************************************
* End of function R_S12AD0_SWTriggerStart
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: R_S12AD0_SWTriggerStop
* Description  : This function stops the AD0 converter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_S12AD0_SWTriggerStop(void)
{
    /* A/D conversion stop */
    S12AD.ADCSR.BIT.ADST = 0U;

    /* Disable S12ADI interrupt */
    IEN(S12ADC0, S12ADI0) = 0U;

    /* Clear interrupt flags */
    IR(S12ADC0, S12ADI0) = 0U;
}

/***********************************************************************************************************************
* End of function R_S12AD0_SWTriggerStop
***********************************************************************************************************************/

/* End user code. Do not edit comment generated here */
