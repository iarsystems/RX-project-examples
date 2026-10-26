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
* File Name    : Config_RTC.c
* Version      : 1.1.0
* Device(s)    : R5F565NEDxFC
* Description  : This file implements device driver for Config_RTC.
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
#include "Config_RTC.h"
/* Start user code for include. Do not edit comment generated here */

#include "r_cg_rtc.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_RTC_Create
* Description  : This function initializes the RTC module
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_RTC_Create(void)
{
    uint32_t rw_count; 
    uint32_t temp_byte;

    /* Disable ALM, PRD and CUP interrupts */
    IEN(RTC, ALM) = 0U;
    IEN(RTC, PRD) = 0U;
    IEN(PERIB, INTB176) = 0U;
    /* RTC cold start with sub-clock source */
    if(0 == SYSTEM.RSTSR1.BIT.CWSF)
    {
        /* Set RTC clock source */
        RTC.RCR4.BYTE = _00_RTC_SOURCE_SELECT_SUB;
    
        /* Set sub-clock oscillator */
        RTC.RCR3.BIT.RTCEN = 1U;
        while (1U != RTC.RCR3.BIT.RTCEN)
        {
            /* waiting for sub-clock oscillator is operating */
        }
    } 

    /* RTC cold start with sub-clock source or RTC start with main clock source */
    if(0 == SYSTEM.RSTSR1.BIT.CWSF || _01_RTC_SOURCE_SELECT_MAIN_FORCED == RTC.RCR4.BYTE)
    {
        /* Stop all counters */
        RTC.RCR2.BYTE = 0x00U;
        while (0U != RTC.RCR2.BIT.START)
        {
            /* Ensure the clock is stopped while configuring it. */
        } 
        /* Select count mode */
        RTC.RCR2.BIT.CNTMD = 0U;
        while (0U != RTC.RCR2.BIT.CNTMD)
        {
            /* Wait for the calendar count mode complete setting */
        } 

        /* Execute RTC software reset */
        RTC.RCR2.BIT.RESET = 1U;
        while (0U != RTC.RCR2.BIT.RESET)
        {
            /* Wait for the reset to complete */
        }
        /* Stop RTC counter */
        RTC.RCR2.BIT.START = 0U;
        while (0U != RTC.RCR2.BIT.START)
        {
            /* Ensure the clock is stopped while configuring it. */
        }

        /* Clear ALM,PRD,CUP IR */
        IR(RTC, ALM) = 0U;
        IR(RTC, PRD) = 0U;
        IR(PERIB, INTB176) = 0U;
    
        /* Set counter time */
        RTC.RSECCNT.BYTE = _00_RTC_COUNT_SECOND_VALUE;
        RTC.RMINCNT.BYTE = _00_RTC_COUNT_MINUTE_VALUE;
        RTC.RHRCNT.BYTE = _00_RTC_COUNT_HOUR_VALUE | _40_RTC_HOUR_ATTRIB_PM;
        RTC.RWKCNT.BYTE = _07_RTC_COUNT_WEEK_VALUE;
        RTC.RDAYCNT.BYTE = _01_RTC_COUNT_DAY_VALUE;
        RTC.RMONCNT.BYTE = _01_RTC_COUNT_MONTH_VALUE;
        RTC.RYRCNT.WORD = _0017_RTC_COUNT_YEAR_VALUE;

        /* Set control registers */
        temp_byte = _00_RTC_ALARM_INT_DISABLE | _04_RTC_PERIOD_INT_ENABLE | _E0_RTC_PERIODIC_INT_PERIOD_1;
        RTC.RCR1.BYTE = temp_byte;
        while (temp_byte != RTC.RCR1.BYTE)
        {
            /* Wait for this write to complete. */
        }
        RTC.RCR2.BYTE |= _00_RTC_AUTO_ADJUSTMENT_DISABLE | _00_RTC_HOUR_MODE_12;

        /* Perform 4 read operations after writing */
        for(rw_count = 0U; rw_count < _04_FOUR_READ_COUNT; rw_count++)
        {
            temp_byte = RTC.RCR2.BYTE;
        } 
    }

    /* Set control registers */
    temp_byte = RTC.RCR1.BYTE | _02_RTC_CARRY_INT_ENABLE;
    RTC.RCR1.BYTE = temp_byte;
    while (temp_byte != RTC.RCR1.BYTE)
    {
        /* Wait for this write to complete. */
    }
    temp_byte = RTC.RCR2.BYTE | _00_RTC_RTCOUT_OUTPUT_DISABLE; 
    RTC.RCR2.BYTE = temp_byte;
    while (temp_byte != RTC.RCR2.BYTE)
    {
        /* Wait for this write to complete. */
    }

    /* Set CUP priority level */
    ICU.SLIBR176.BYTE = 0x31U;
    IPR(PERIB, INTB176) = _00_RTC_PRIORITY_LEVEL0;

    /* Set PRD priority level */
    IPR(RTC,PRD) = _0F_RTC_PRIORITY_LEVEL15; 

    /* Set time capture */
    RTC.RTCCR0.BYTE = _00_RTC_TIME_CAPTURE_NO_EVENT | _00_RTC_TIME_CAPTURE_NOISE_FILTER_OFF | 
                      _00_RTC_TIME_CAPTURE_EVENT_DISABLE;
    RTC.RTCCR1.BYTE = _00_RTC_TIME_CAPTURE_NO_EVENT | _00_RTC_TIME_CAPTURE_NOISE_FILTER_OFF | 
                      _00_RTC_TIME_CAPTURE_EVENT_DISABLE;
    RTC.RTCCR2.BYTE = _00_RTC_TIME_CAPTURE_NO_EVENT | _00_RTC_TIME_CAPTURE_NOISE_FILTER_OFF | 
                      _00_RTC_TIME_CAPTURE_EVENT_DISABLE; 

    R_Config_RTC_Create_UserInit();
}

/***********************************************************************************************************************
* Function Name: R_Config_RTC_Start
* Description  : This function starts RTC counter
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_RTC_Start(void)
{

    /* Enable CUP interrupt */
    IEN(PERIB, INTB176) = 1U;

    /* Enable PRD interrupt */
    IEN(RTC, PRD) = 1U; 

    /* Set the START bit to 1 */
    RTC.RCR2.BIT.START = 1U;
    while (1U != RTC.RCR2.BIT.START)
    {
        /* Ensure the clock is started while configuring it. */
    }
}

/***********************************************************************************************************************
* Function Name: R_Config_RTC_Stop
* Description  : This function stops RTC counter
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_RTC_Stop(void)
{

    /* Disable CUP interrupt */
    IEN(PERIB, INTB176) = 0U;

    /* Disable PRD interrupt */
    IEN(RTC, PRD) = 0U; 

    /* Stop all counters */
    RTC.RCR2.BIT.START = 0U;
    while (0U != RTC.RCR2.BIT.START)
    {
        /* Ensure the clock is stopped while configuring it. */
    }
}

/***********************************************************************************************************************
* Function Name: R_Config_RTC_Set_ConstPeriodInterruptOff
* Description  : This function disable RTC constant period interrupt(PRD)
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_RTC_Set_ConstPeriodInterruptOff(void)
{
    /* Disable ICU PRD interrupt */
    IEN(RTC, PRD) = 0U;

    /* Clear IR flag of PRD interrupt */
    IR(RTC, PRD) = 0U;

    /* Disable RTC PRD interrupt */
    RTC.RCR1.BIT.PIE = 0U;
    while (0U != RTC.RCR1.BIT.PIE)
    {
        /* Wait for this write to complete. */
    }
}

/***********************************************************************************************************************
* Function Name: R_Config_RTC_Set_CarryInterruptOn
* Description  : This function enable RTC carry interrupt(CUP)
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_RTC_Set_CarryInterruptOn(void)
{

    /* Clear IR flag of CUP interrupt */
    IR(PERIB, INTB176) = 0U;

    /* Enable ICU CUP interrupt */
    IEN(PERIB, INTB176) = 1U; 

    /* Enable RTC CUP interrupt */
    RTC.RCR1.BIT.CIE = 1U;
}

/***********************************************************************************************************************
* Function Name: R_Config_RTC_Set_CarryInterruptOff
* Description  : This function disable RTC carry interrupt(CUP)
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_RTC_Set_CarryInterruptOff(void)
{

    /* Clear IR flag of CUP interrupt */
    IR(PERIB, INTB176) = 0U;

    /* Disable ICU CUP interrupt */
    IEN(PERIB, INTB176) = 0U; 

    /* Disable RTC CUP interrupt */
    RTC.RCR1.BIT.CIE = 0U;

}

/***********************************************************************************************************************
* Function Name: R_Config_RTC_Restart
* Description  : This function restarts RTC counter
* Arguments    : counter_write_val -
*                    counter write value
* Return Value : None
***********************************************************************************************************************/

void R_Config_RTC_Restart(rtc_calendarcounter_value_t counter_write_val)
{
    uint32_t read_count;
    volatile uint32_t dummy; 
    /* Disable ALM, PRD and CUP interrupts */
    IEN(RTC, ALM) = 0U;
    IEN(RTC, PRD) = 0U;
    IEN(PERIB, INTB176) = 0U; 

    /* Stop all counters */
    RTC.RCR2.BIT.START = 0U;
    while (0U != RTC.RCR2.BIT.START)
    {
        /* Ensure the clock is stopped while configuring it. */
    }

    /* Execute RTC software reset */
    RTC.RCR2.BIT.RESET = 1U;
    while (0U != RTC.RCR2.BIT.RESET)
    {
        /* Wait for the reset to complete */
    } 

    /* Set control registers */
    RTC.RCR2.BYTE |= _00_RTC_RTCOUT_OUTPUT_DISABLE | _00_RTC_AUTO_ADJUSTMENT_DISABLE | _00_RTC_HOUR_MODE_12;

    /* Perform 4 read operations after writing */
    for(read_count = 0U; read_count < _04_FOUR_READ_COUNT; read_count++)
    {
        dummy = RTC.RCR2.BYTE;
    } 

    /* Set counter values */
    RTC.RSECCNT.BYTE = counter_write_val.rseccnt;
    RTC.RMINCNT.BYTE = counter_write_val.rmincnt;
    RTC.RHRCNT.BYTE = counter_write_val.rhrcnt;
    RTC.RWKCNT.BYTE = counter_write_val.rwkcnt;
    RTC.RDAYCNT.BYTE = counter_write_val.rdaycnt;
    RTC.RMONCNT.BYTE = counter_write_val.rmoncnt;
    RTC.RYRCNT.WORD = counter_write_val.ryrcnt;

    /* Enable CUP interrupt */
    IEN(PERIB, INTB176) = 1U; 

    /* Enable PRD interrupt */
    IEN(RTC, PRD) = 1U; 

    /* Set the START bit to 1 */
    RTC.RCR2.BIT.START = 1U;
    while (1U != RTC.RCR2.BIT.START)
    {
        /* Ensure the clock is started while configuring it.*/
    }
}

/***********************************************************************************************************************
* Function Name: R_Config_RTC_Get_CalendarCounterValue
* Description  : This function get RTC calendar counter value
* Arguments    : counter_read_val -
*                    counter read pointer
* Return Value : None
***********************************************************************************************************************/


void R_Config_RTC_Get_CalendarCounterValue(rtc_calendarcounter_value_t * const counter_read_val)
{
    /* Store interrupt status */
    uint32_t dummy = ICU.IER[IER_RTC_CUP].BIT.IEN_RTC_CUP;
    /* Disable ICU CUP interrupt */
    IEN(PERIB, INTB176) = 0U; 

    /* Enable RTC CUP interrupt */
    RTC.RCR1.BYTE |= _02_RTC_CARRY_INT_ENABLE;
    do
    {
        /* Clear IR flag of CUP interrupt */
        IR(PERIB, INTB176) = 0U;

        /* Read counter registers */
        counter_read_val->rseccnt = RTC.RSECCNT.BYTE;
        counter_read_val->rmincnt = RTC.RMINCNT.BYTE;
        counter_read_val->rhrcnt = RTC.RHRCNT.BYTE;
        counter_read_val->rwkcnt = RTC.RWKCNT.BYTE;
        counter_read_val->rdaycnt = RTC.RDAYCNT.BYTE;
        counter_read_val->rmoncnt = RTC.RMONCNT.BYTE;
        counter_read_val->ryrcnt = RTC.RYRCNT.WORD;
    } while (IR(PERIB, INTB176) == 1U);

    /* Restore back interrupt status */
    IEN(PERIB, INTB176) = dummy; 

}

/***********************************************************************************************************************
* Function Name: R_Config_RTC_Set_CalendarCounterValue
* Description  : This function set RTC calendar counter value
* Arguments    : counter_write_val -
*                    counter write value
* Return Value : None
***********************************************************************************************************************/

void R_Config_RTC_Set_CalendarCounterValue(rtc_calendarcounter_value_t counter_write_val)
{
    uint32_t rw_count;
    volatile uint32_t dummy;

    /* Stop all counters */
    RTC.RCR2.BIT.START = 0U;
    while (0U != RTC.RCR2.BIT.START)
    {
        /* Ensure the clock is stopped while configuring it. */
    }

    /* Execute RTC software reset */
    RTC.RCR2.BIT.RESET = 1U;
    while (1U != RTC.RCR2.BIT.RESET)
    {
        /* Wait for the reset to complete */
    }

    /* Set control registers */
    RTC.RCR2.BYTE |= _00_RTC_RTCOUT_OUTPUT_DISABLE | _00_RTC_AUTO_ADJUSTMENT_DISABLE | _00_RTC_HOUR_MODE_12;
    /* Perform 4 read operations after writing */
    for(rw_count = 0U; rw_count < _04_FOUR_READ_COUNT; rw_count++)
    {
        dummy = RTC.RCR2.BYTE;
    } 

    /* Set counter values */
    RTC.RSECCNT.BYTE = counter_write_val.rseccnt;
    RTC.RMINCNT.BYTE = counter_write_val.rmincnt;
    RTC.RHRCNT.BYTE = counter_write_val.rhrcnt;
    RTC.RWKCNT.BYTE = counter_write_val.rwkcnt;
    RTC.RDAYCNT.BYTE = counter_write_val.rdaycnt;
    RTC.RMONCNT.BYTE = counter_write_val.rmoncnt;
    RTC.RYRCNT.WORD = counter_write_val.ryrcnt; 

}

/***********************************************************************************************************************
* Function Name: R_Config_RTC_Set_ConstPeriodInterruptOn
* Description  : This function set RTC constant period interrupt and enable the interrupt(PRD)
* Arguments    : period -
*                    new period of constant period interrupt
* Return Value : None
***********************************************************************************************************************/

void R_Config_RTC_Set_ConstPeriodInterruptOn(rtc_int_period_t period)
{
    /* Disable ICU PRD interrupt */
    IEN(RTC, PRD) = 0U;

    /* Clear IR flag of PRD interrupt*/
    IR(RTC, PRD) = 0U;

    /* Set RTC control register 1 */
    RTC.RCR1.BYTE = (uint8_t)(period | (RTC.RCR1.BYTE & 0x0FU) | _04_RTC_PERIOD_INT_ENABLE);
 
    while ((1U != RTC.RCR1.BIT.PIE) || ((period >> 4U) != RTC.RCR1.BIT.PES))
    {
        nop();
    }

    /* Enable ICU PRD interrupt */
    IEN(RTC, PRD) = 1U;

}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

