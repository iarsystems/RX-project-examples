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
* File Name    : r_cg_rtc.c
* Version      : Applilet4 for RX111 V1.00.00.02 [27 Mar 2013]
* Device(s)    : R5F51115AxLF
* Tool-Chain   : IAR Systems icc78k0r
* Description  : This file implements device driver for RTC module.
* Creation Date: 4/4/2013
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
#include "r_cg_rtc.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_RTC_Create
* Description  : This function initializes the RTC module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_RTC_Create(void)
{
    /* Disable ALM, PRD and CUP interrupts */
    IEN(RTC,ALM) = 0U;
    IEN(RTC,PRD) = 0U;
    IEN(RTC,CUP) = 0U;

    /* Set sub-clock oscillator */
    RTC.RCR3.BYTE = _00_RTC_SUBCLOCK_DRIVE_MID | _01_RTC_SUBCLOCK_ENABLE;

    /* Stop all counters */
    RTC.RCR2.BIT.START = 0U;
    
    while (RTC.RCR2.BIT.START != 0U);

    /* Select count mode */
    RTC.RCR2.BIT.CNTMD = 1U;
    
    while (RTC.RCR2.BIT.CNTMD != 1U);

    /* Execute RTC software reset */
    RTC.RCR2.BIT.RESET = 1U;
    
    while (RTC.RCR2.BIT.RESET != 0U);
    
    /* Set control registers */
    RTC.RCR1.BYTE = _00_RTC_ALARM_INT_DISABLE | _00_RTC_CARRY_INT_DISABLE | _04_RTC_PERIOD_INT_ENABLE | 
                    _D0_RTC_PERIODIC_INT_PERIOD_1_2;

    /* Clear the counters */
    RTCB.BCNT0.BYTE = 0U;
    RTCB.BCNT1.BYTE = 0U;
    RTCB.BCNT2.BYTE = 0U;
    RTCB.BCNT3.BYTE = 0U;

    /* Set PRD priority level */
    IPR(RTC,PRD) = _0F_RTC_PRIORITY_LEVEL15;

}
/***********************************************************************************************************************
* Function Name: R_RTC_Start
* Description  : This function starts RTC counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_RTC_Start(void)
{
    /* Enable PRD interrupt */
    IEN(RTC,PRD) = 1U;

    /* Set the START bit to 1 */
    RTC.RCR2.BIT.START = 1U;
    
    while (RTC.RCR2.BIT.START != 1U);
}
/***********************************************************************************************************************
* Function Name: R_RTC_Stop
* Description  : This function stops RTC counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_RTC_Stop(void)
{
    /* Disable PRD interrupt */
    IEN(RTC,PRD) = 0U;

    /* Stop all counters */
    RTC.RCR2.BIT.START = 0U;
    
    while (RTC.RCR2.BIT.START != 0U);
}
/***********************************************************************************************************************
* Function Name: R_RTC_Restart
* Description  : This function restarts RTC counter.
* Arguments    : counter_write_val -
*                    counter write value
* Return Value : None
***********************************************************************************************************************/
void R_RTC_Restart(rtc_calendarcounter_value_t counter_write_val)
{
    /* Disable ALM, PRD and CUP interrupts */
    IEN(RTC,ALM) = 0U;
    IEN(RTC,PRD) = 0U;
    IEN(RTC,CUP) = 0U;

    /* Stop all counters */
    RTC.RCR2.BIT.START = 0U;
    
    while (RTC.RCR2.BIT.START != 0U);

    /* Execute RTC software reset */
    RTC.RCR2.BIT.RESET = 1U;
    
    while (RTC.RCR2.BIT.RESET != 0U);
    
    /* Set control registers */

    /* Clear counters */
    RTCB.BCNT0.BYTE = 0U;
    RTCB.BCNT1.BYTE = 0U;
    RTCB.BCNT2.BYTE = 0U;
    RTCB.BCNT3.BYTE = 0U;

    /* Enable PRD interrupt */
    IEN(RTC,PRD) = 1U;

    /* Set the START bit to 1 */
    RTC.RCR2.BIT.START = 1U;
    
    while (RTC.RCR2.BIT.START != 1U);
}
/***********************************************************************************************************************
* Function Name: R_RTC_Get_BinaryCounterValue
* Description  : This function get RTC binary counter value.
* Arguments    : counter_read_val -
*                    counter read pointer
* Return Value : None
***********************************************************************************************************************/
void R_RTC_Get_BinaryCounterValue(uint32_t * const counter_read_val)
{
    RTC.RCR1.BYTE |= _02_RTC_CARRY_INT_ENABLE;

    IR(RTC,CUP) = 0U;

    do
    {
        *counter_read_val = (uint32_t)RTCB.BCNT0.BYTE;
        *counter_read_val |= (uint32_t)RTCB.BCNT1.BYTE << 8;
        *counter_read_val |= (uint32_t)RTCB.BCNT2.BYTE << 16;
        *counter_read_val |= (uint32_t)RTCB.BCNT3.BYTE << 24;
    } while (IR(RTC,CUP) == 1U);

    RTC.RCR1.BYTE &= ~(_02_RTC_CARRY_INT_ENABLE);
}
/***********************************************************************************************************************
* Function Name: R_RTC_Set_BinaryCounterValue
* Description  : This function set RTC binary counter value.
* Arguments    : counter_write_val -
*                    counter write pointer
* Return Value : None
***********************************************************************************************************************/
void R_RTC_Set_BinaryCounterValue(uint32_t counter_write_val)
{
    /* Stop all counters */
    RTC.RCR2.BIT.START = 0U;
    
    while (RTC.RCR2.BIT.START != 0U);

    /* Execute RTC software reset */
    RTC.RCR2.BIT.RESET = 1U;
    
    while (RTC.RCR2.BIT.RESET != 0U);
    
    /* Set control registers */

    /* Set the counters */
    RTCB.BCNT0.BYTE = (uint8_t)(counter_write_val & 0x000000FFUL);
    RTCB.BCNT1.BYTE = (uint8_t)((counter_write_val & 0x0000FF00UL) >> 8);
    RTCB.BCNT2.BYTE = (uint8_t)((counter_write_val & 0x00FF0000UL) >> 16);
    RTCB.BCNT3.BYTE = (uint8_t)((counter_write_val & 0xFF000000UL) >> 24);
}
/***********************************************************************************************************************
* Function Name: R_RTC_Set_ConstPeriodInterruptOn
* Description  : This function set RTC constant period interrupt and enable the interrupt(PRD).
* Arguments    : period -
*                    new period of constant period interrupt
* Return Value : None
***********************************************************************************************************************/
void R_RTC_Set_ConstPeriodInterruptOn(rtc_int_period_t period)
{
    IEN(RTC,PRD) = 0U;
    IR(RTC,PRD) = 0U;

    RTC.RCR1.BYTE = period | (RTC.RCR1.BYTE & 0x0FU) | _04_RTC_PERIOD_INT_ENABLE;

    IEN(RTC,PRD) = 1U;
}
/***********************************************************************************************************************
* Function Name: R_RTC_Set_ConstPeriodInterruptOff
* Description  : This function disable RTC constant period interrupt(PRD).
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_RTC_Set_ConstPeriodInterruptOff(void)
{
    IEN(RTC,PRD) = 0U;
    IR(RTC,PRD) = 0U;

    RTC.RCR1.BIT.PIE = 0U;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
