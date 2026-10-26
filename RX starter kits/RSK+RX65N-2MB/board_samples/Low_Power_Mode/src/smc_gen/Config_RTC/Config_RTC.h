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
* File Name    : Config_RTC.h
* Version      : 1.1.0
* Device(s)    : R5F565NEDxFC
* Description  : This file implements device driver for Config_RTC.
* Creation Date: 2018-02-22
***********************************************************************************************************************/

#ifndef Config_RTC_H
#define Config_RTC_H

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_rtc.h"

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/
#define _04_FOUR_READ_COUNT                         (0x04U)/* Perform 4 read operations */
#define _21_RTC_SOSCWTCR_VALUE                      (0x21U)/* Sub-clock oscillator wait time */
#define _0005B8D9_RTC_1_64_SEC_CYCLE                (0x0005B8D9UL)
#define _00_RTC_COUNT_SECOND_VALUE                  (0x00U)
#define _00_RTC_COUNT_MINUTE_VALUE                  (0x00U)
#define _00_RTC_COUNT_HOUR_VALUE                    (0x00U)
#define _07_RTC_COUNT_WEEK_VALUE                    (0x07U)
#define _01_RTC_COUNT_DAY_VALUE                     (0x01U)
#define _01_RTC_COUNT_MONTH_VALUE                   (0x01U)
#define _0017_RTC_COUNT_YEAR_VALUE                  (0x0017U)

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void R_Config_RTC_Create(void);
void R_Config_RTC_Start(void);
void R_Config_RTC_Stop(void);
void R_Config_RTC_Restart(rtc_calendarcounter_value_t counter_write_val);
void R_Config_RTC_Get_CalendarCounterValue(rtc_calendarcounter_value_t * const counter_read_val);
void R_Config_RTC_Set_CalendarCounterValue(rtc_calendarcounter_value_t counter_write_val);
void R_Config_RTC_Set_ConstPeriodInterruptOn(rtc_int_period_t period);
void R_Config_RTC_Set_ConstPeriodInterruptOff(void);
void R_Config_RTC_Set_CarryInterruptOn(void);
void R_Config_RTC_Set_CarryInterruptOff(void);
void R_Config_RTC_Create_UserInit(void);
/* Start user code for function. Do not edit comment generated here */

/* Flag to indicate the rtc periodic interrupt has occurred */
extern uint8_t g_rtc_interrupt;

/* End user code. Do not edit comment generated here */
#endif

