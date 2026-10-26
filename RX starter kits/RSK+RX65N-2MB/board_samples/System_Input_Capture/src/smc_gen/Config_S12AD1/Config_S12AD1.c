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
* File Name    : Config_S12AD1.c
* Version      : 1.3.0
* Device(s)    : R5F565NEDxFC
* Description  : This file implements device driver for Config_S12AD1.
* Creation Date: 2018-02-22
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
#include "Config_S12AD1.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_S12AD1_Create
* Description  : This function initializes the S12AD1 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_S12AD1_Create(void)
{
    /* Cancel S12AD1 module stop state */
    MSTP(S12AD1) = 0U;
    MSTP(TEMPS) = 0U;

    /* Disable and clear interrupt flags of S12AD1 module */
    S12AD1.ADCSR.BIT.ADIE = 0U;
    S12AD1.ADCMPCR.BIT.CMPAIE = 0U;
    S12AD1.ADCMPCR.BIT.CMPBIE = 0U;
    IEN(PERIB, INTB189) = 0U;

    /* Set S12AD1 control registers */
    S12AD1.ADCSR.WORD = _0000_AD_DBLTRIGGER_DISABLE | _0000_AD_SYNCASYNCTRG_DISABLE | 
                        _1000_AD_SCAN_END_INTERRUPT_ENABLE | _0000_AD_SINGLE_SCAN_MODE;
    S12AD1.ADCER.WORD = _0000_AD_RESOLUTION_12BIT | _0000_AD_AUTO_CLEARING_DISABLE | _0000_AD_SELFTDIAGST_DISABLE | 
                        _0000_AD_RIGHT_ALIGNMENT;
    S12AD1.ADADC.BYTE = _01_AD_2_TIME_CONVERSION | _80_AD_AVERAGE_MODE;
    S12AD1.ADEXICR.WORD = _0001_AD_TEMP_ADD_USED | _0100_AD_TEMP_USED;

    /* Set channels and sampling time */
    S12AD1.ADSSTRT = _96_AD1_SAMPLING_STATE_T;

    /* Set compare control register */
    S12AD1.ADCMPCR.WORD = _0000_AD_WINDOWB_DISABLE | _0000_AD_WINDOWA_DISABLE | _0000_AD_WINDOWFUNCTION_DISABLE;

    /* Set interrupt and priority level */
    ICU.SLIBR189.BYTE = 0x44U;
    IPR(PERIB, INTB189) = _0F_AD_PRIORITY_LEVEL15;

    R_Config_S12AD1_Create_UserInit();
}

/***********************************************************************************************************************
* Function Name: R_Config_S12AD1_Start
* Description  : This function starts the AD1 converter
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_S12AD1_Start(void)
{
    uint16_t w_count;
    IR(PERIB, INTB189) = 0U;
    IEN(PERIB, INTB189) = 1U;
    TEMPS.TSCR.BIT.TSEN = 1U;

    for (w_count = 0U; w_count <= _02D1_TEMP_STABLE_WAIT; w_count++)
    {
        nop();
    }

    TEMPS.TSCR.BIT.TSOE = 1U;
    S12AD1.ADCSR.BIT.ADST = 1U;
}

/***********************************************************************************************************************
* Function Name: R_Config_S12AD1_Stop
* Description  : This function stop the AD1 converter
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_S12AD1_Stop(void)
{
    S12AD1.ADCSR.BIT.ADST = 0U;
    TEMPS.TSCR.BIT.TSOE = 0U;
    TEMPS.TSCR.BIT.TSEN = 0U;
    IEN(PERIB, INTB189) = 0U;
    IR(PERIB, INTB189) = 0U;
}

/***********************************************************************************************************************
* Function Name: R_Config_S12AD1_Get_ValueResult
* Description  : This function gets result from the AD1 converter
* Arguments    : channel -
*                    channel of data register to be read
*                buffer -
*                    buffer pointer
* Return Value : None
***********************************************************************************************************************/

void R_Config_S12AD1_Get_ValueResult(ad_channel_t channel, uint16_t * const buffer)
{
    if (channel == ADSELFDIAGNOSIS)
    {
        *buffer = (uint16_t)(S12AD1.ADRD.WORD);
    }
    else if (channel == ADCHANNEL0)
    {
        *buffer = (uint16_t)(S12AD1.ADDR0);
    }
    else if (channel == ADCHANNEL1)
    {
        *buffer = (uint16_t)(S12AD1.ADDR1);
    }
    else if (channel == ADCHANNEL2)
    {
        *buffer = (uint16_t)(S12AD1.ADDR2);
    }
    else if (channel == ADCHANNEL3)
    {
        *buffer = (uint16_t)(S12AD1.ADDR3);
    }
    else if (channel == ADCHANNEL4)
    {
        *buffer = (uint16_t)(S12AD1.ADDR4);
    }
    else if (channel == ADCHANNEL5)
    {
        *buffer = (uint16_t)(S12AD1.ADDR5);
    }
    else if (channel == ADCHANNEL6)
    {
        *buffer = (uint16_t)(S12AD1.ADDR6);
    }
    else if (channel == ADCHANNEL7)
    {
        *buffer = (uint16_t)(S12AD1.ADDR7);
    }
    else if (channel == ADCHANNEL8)
    {
        *buffer = (uint16_t)(S12AD1.ADDR8);
    }
    else if (channel == ADCHANNEL9)
    {
        *buffer = (uint16_t)(S12AD1.ADDR9);
    }
    else if (channel == ADCHANNEL10)
    {
        *buffer = (uint16_t)(S12AD1.ADDR10);
    }
    else if (channel == ADCHANNEL11)
    {
        *buffer = (uint16_t)(S12AD1.ADDR11);
    }
    else if (channel == ADCHANNEL12)
    {
        *buffer = (uint16_t)(S12AD1.ADDR12);
    }
    else if (channel == ADCHANNEL13)
    {
        *buffer = (uint16_t)(S12AD1.ADDR13);
    }
    else if (channel == ADCHANNEL14)
    {
        *buffer = (uint16_t)(S12AD1.ADDR14);
    }
    else if (channel == ADCHANNEL15)
    {
        *buffer = (uint16_t)(S12AD1.ADDR15);
    }
    else if (channel == ADCHANNEL16)
    {
        *buffer = (uint16_t)(S12AD1.ADDR16);
    }
    else if (channel == ADCHANNEL17)
    {
        *buffer = (uint16_t)(S12AD1.ADDR17);
    }
    else if (channel == ADCHANNEL18)
    {
        *buffer = (uint16_t)(S12AD1.ADDR18);
    }
    else if (channel == ADCHANNEL19)
    {
        *buffer = (uint16_t)(S12AD1.ADDR19);
    }
    else if (channel == ADCHANNEL20)
    {
        *buffer = (uint16_t)(S12AD1.ADDR20);
    }
    else if (channel == ADTEMPSENSOR)
    {
        *buffer = (uint16_t)(S12AD1.ADTSDR);
    }
    else if (channel == ADINTERREFVOLT)
    {
        *buffer = (uint16_t)(S12AD1.ADOCDR);
    }
    else if (channel == ADDATADUPLICATION)
    {
        *buffer = (uint16_t)(S12AD1.ADDBLDR);
    }
    else if (channel == ADDATADUPLICATIONA)
    {
        *buffer = (uint16_t)(S12AD1.ADDBLDRA);
    }
    else if (channel == ADDATADUPLICATIONB)
    {
        *buffer = (uint16_t)(S12AD1.ADDBLDRB);
    }
    else
    {
        /* Do Nothing */ 
    }
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
