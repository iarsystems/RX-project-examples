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
* Version      : Applilet4 for RX111 V1.00.00.05 [06 Jun 2013]
* Device(s)    : R5F51115AxFM
* Tool-Chain   : IAR Systems icc78k0r
* Description  : This file implements device driver for RTC module.
* Creation Date: 08.10.2013
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
    uint32_t w_count;

    /* Disable ALM, PRD and CUP interrupts */
    IEN(RTC,ALM) = 0U;
    IEN(RTC,PRD) = 0U;
    IEN(RTC,CUP) = 0U;

    /* Set sub-clock oscillator */
    RTC.RCR3.BYTE = _00_RTC_SUBCLOCK_DRIVE_MID | _01_RTC_SUBCLOCK_ENABLE;

    for (w_count = 0U; w_count <= _000001B8_RTC_RTCSCLK_STABLE_CYCLE; w_count++)
    {
        nop();
    }

    /* Stop all counters */
    RTC.RCR2.BIT.START = 0U;
    
    while (RTC.RCR2.BIT.START != 0U);

    /* Select count mode */
    RTC.RCR2.BIT.CNTMD = 0U;
    
    while (RTC.RCR2.BIT.CNTMD != 0U);

    /* Execute RTC software reset */
    RTC.RCR2.BIT.RESET = 1U;
    
    while (RTC.RCR2.BIT.RESET != 0U);
    
    /* Set control registers */
    RTC.RCR1.BYTE = _01_RTC_ALARM_INT_ENABLE | _00_RTC_CARRY_INT_DISABLE | _00_RTC_PERIOD_INT_DISABLE | 
                    _00_RTC_PERIODIC_INT_PERIOD_DISABLE;
    RTC.RCR2.BYTE = _40_RTC_HOUR_MODE_24;

    /* Set alarm */
    RTC.RCR2.BIT.START = 1U;
    
    while (RTC.RCR2.BIT.START != 1U);


    for (w_count = 0U; w_count <= _0000927D_RTC_1_64_SEC_CYCLE; w_count++)
    {
        nop();
    }

    RTC.RCR2.BIT.START = 0U;
    
    while (RTC.RCR2.BIT.START != 0U);

    IR(RTC,ALM) = 0U;
    IR(RTC,PRD) = 0U;
    IR(RTC,CUP) = 0U;

    /* Set time */
    RTC.RSECCNT.BYTE = _00_RTC_COUNT_SECOND_VALUE;
    RTC.RMINCNT.BYTE = _55_RTC_COUNT_MINUTE_VALUE;
    RTC.RHRCNT.BYTE = _11_RTC_COUNT_HOUR_VALUE;
    RTC.RWKCNT.BYTE = _05_RTC_COUNT_WEEK_VALUE;
    RTC.RDAYCNT.BYTE = _26_RTC_COUNT_DAY_VALUE;
    RTC.RMONCNT.BYTE = _07_RTC_COUNT_MONTH_VALUE;
    RTC.RYRCNT.WORD = _0013_RTC_COUNT_YEAR_VALUE;

    /* Set ALM priority level */
    IPR(RTC,ALM) = _0F_RTC_PRIORITY_LEVEL15;

}
/***********************************************************************************************************************
* Function Name: R_RTC_Start
* Description  : This function starts RTC counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_RTC_Start(void)
{
    /* Enable ALM interrupt */
    IEN(RTC,ALM) = 1U;

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
    /* Disable ALM interrupt */
    IEN(RTC,ALM) = 0U;

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
    RTC.RCR2.BYTE = _40_RTC_HOUR_MODE_24;

    /* Set counter values */
    RTC.RSECCNT.BYTE = counter_write_val.rseccnt;
    RTC.RMINCNT.BYTE = counter_write_val.rmincnt;
    RTC.RHRCNT.BYTE = counter_write_val.rhrcnt;
    RTC.RWKCNT.BYTE = counter_write_val.rwkcnt;
    RTC.RDAYCNT.BYTE = counter_write_val.rdaycnt;
    RTC.RMONCNT.BYTE = counter_write_val.rmoncnt;
    RTC.RYRCNT.WORD = counter_write_val.ryrcnt;

    /* Enable ALM interrupt */
    IEN(RTC,ALM) = 1U;

    /* Set the START bit to 1 */
    RTC.RCR2.BIT.START = 1U;
    
    while (RTC.RCR2.BIT.START != 1U);
}
/***********************************************************************************************************************
* Function Name: R_RTC_Get_CalendarCounterValue
* Description  : This function get RTC calendar counter value.
* Arguments    : counter_read_val -
*                    counter read pointer
* Return Value : None
***********************************************************************************************************************/
void R_RTC_Get_CalendarCounterValue(rtc_calendarcounter_value_t * const counter_read_val)
{
    IR(RTC,CUP) = 0U;
    RTC.RCR1.BYTE |= _02_RTC_CARRY_INT_ENABLE;

    do
    {
        counter_read_val->rseccnt = RTC.RSECCNT.BYTE;
        counter_read_val->rmincnt = RTC.RMINCNT.BYTE;
        counter_read_val->rhrcnt = RTC.RHRCNT.BYTE;
        counter_read_val->rwkcnt = RTC.RWKCNT.BYTE;
        counter_read_val->rdaycnt = RTC.RDAYCNT.BYTE;
        counter_read_val->rmoncnt = RTC.RMONCNT.BYTE;
        counter_read_val->ryrcnt = RTC.RYRCNT.WORD;
    } while (IR(RTC,CUP) == 1U);

    RTC.RCR1.BYTE &= ~(_02_RTC_CARRY_INT_ENABLE);
}
/***********************************************************************************************************************
* Function Name: R_RTC_Set_CalendarCounterValue
* Description  : This function set RTC calendar counter value.
* Arguments    : counter_write_val -
*                    counter write value
* Return Value : None
***********************************************************************************************************************/
void R_RTC_Set_CalendarCounterValue(rtc_calendarcounter_value_t counter_write_val)
{
    /* Stop all counters */
    RTC.RCR2.BIT.START = 0U;
    
    while (RTC.RCR2.BIT.START != 0U);

    /* Execute RTC software reset */
    RTC.RCR2.BIT.RESET = 1U;
    
    while (RTC.RCR2.BIT.RESET != 0U);
    
    /* Set control registers */
    RTC.RCR2.BYTE = _40_RTC_HOUR_MODE_24;

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
* Function Name: R_RTC_Set_CalendarAlarm
* Description  : This function set RTC calendar alarm enable.
* Arguments    : alarm_enable -
*                    set RTC calendar alarm enable bits
*                alarm_val -
*                    structure containing new alarm value
* Return Value : None
***********************************************************************************************************************/
void R_RTC_Set_CalendarAlarm(rtc_calendar_alarm_enable_t alarm_enable, rtc_calendar_alarm_value_t alarm_val)
{
    uint32_t w_count;

    IEN(RTC,ALM) = 0U;

    /* Set the START bit to 1 */
    RTC.RCR2.BIT.START = 1U;
    
    while (RTC.RCR2.BIT.START != 1U);

    RTC.RSECAR.BYTE = (uint8_t)((alarm_enable.sec_enb & 0x80U) | (alarm_val.rsecar & 0x7FU));
    RTC.RMINAR.BYTE = (uint8_t)((alarm_enable.min_enb & 0x80U) | (alarm_val.rminar & 0x7FU));
    RTC.RHRAR.BYTE = (uint8_t)((alarm_enable.hr_enb & 0x80U) | (alarm_val.rhrar & 0x7FU));
    RTC.RDAYAR.BYTE = (uint8_t)((alarm_enable.day_enb & 0x80U) | (alarm_val.rdayar & 0x3FU));
    RTC.RWKAR.BYTE = (uint8_t)((alarm_enable.wk_enb & 0x80U) | (alarm_val.rwkar & 0x07U));
    RTC.RMONAR.BYTE = (uint8_t)((alarm_enable.mon_enb & 0x80U) | (alarm_val.rmonar & 0x1F));
    RTC.RYRAR.WORD = (uint16_t)(alarm_val.ryrar & 0x00FFU);
    RTC.RYRAREN.BYTE = (uint8_t)(alarm_enable.yr_enb & 0x80U);

    RTC.RCR1.BIT.AIE = 1U;

    for (w_count = 0U; w_count <= _0000927D_RTC_1_64_SEC_CYCLE; w_count++)
    {
        nop();
    }
    
    IR(RTC,ALM) = 0U;
    IEN(RTC,ALM) = 1U;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
