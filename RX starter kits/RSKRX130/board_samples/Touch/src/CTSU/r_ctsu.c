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
* Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_ctsu.c
* Version      : 1.00
* Device(s)    : R5F5113xAxFP,R5F5231xAxFP,R5F51305AxFN
* Description  : This file controls Capacitive Touch.
***********************************************************************************************************************/

/***********************************************************************************************************************
* History      : DD.MM.YYYY Version    Description
*              : xx.xx.2015   1.00     First Release
***********************************************************************************************************************/
#define __R_CTSU_C__

/***********************************************************************************************************************
* Pragma directive
***********************************************************************************************************************/

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
/* System include header */
#include "r_cg_macrodriver.h"

/* H/W include header */
#include "iodefine.h"
/*
*CONVERTER REPLACED|REMOVED:
*#include "r_cg_vect.h"
*/

#include "r_mpc.h"
#include "r_dtc.h"
#include "r_ctsu.h"
#include "r_cgc.h"
#ifdef  SCI_SERIAL_USE
    #include "r_sci.h"
#endif  // SCI_SERIAL_USE

/* S/W include header */
#include "r_cg_userdefine.h"
#include "r_touch.h"
#include "r_touch_API.h"

#if (MCU_SERIES_T == R5F5113 || MCU_SERIES_T == R5F5231)
    #include "r_serial_control.h"
#endif

/***********************************************************************************************************************
* Global variables and functions
***********************************************************************************************************************/
const uint8_t g_ctsu_txvsel[] = 
{
#if ( SELF_METHOD_NUM == 1 )
    0,
#endif
#if ( MUTUAL_METHOD_NUM >= 1 )
    MUTUAL0_CTSUTXVSEL,
#endif
#if ( MUTUAL_METHOD_NUM >= 2 )
    MUTUAL1_CTSUTXVSEL,
#endif
#if ( MUTUAL_METHOD_NUM >= 3 )
    MUTUAL2_CTSUTXVSEL,
#endif
#if ( MUTUAL_METHOD_NUM >= 4 )
    MUTUAL3_CTSUTXVSEL,
#endif
#if ( MUTUAL_METHOD_NUM >= 5 )
    MUTUAL4_CTSUTXVSEL,
#endif
#if ( MUTUAL_METHOD_NUM >= 6 )
    MUTUAL5_CTSUTXVSEL,
#endif
#if ( MUTUAL_METHOD_NUM >= 7 )
    MUTUAL6_CTSUTXVSEL,
#endif
#if ( MUTUAL_METHOD_NUM >= 8 )
    MUTUAL7_CTSUTXVSEL
#endif
};

const uint8_t g_ctsu_atune0[] = 
{
#if ( SELF_METHOD_NUM == 1 )
    SELF_CTSUATUNE0,
#endif
#if ( MUTUAL_METHOD_NUM >= 1 )
    MUTUAL0_CTSUATUNE0,
#endif
#if ( MUTUAL_METHOD_NUM >= 2 )
    MUTUAL1_CTSUATUNE0,
#endif
#if ( MUTUAL_METHOD_NUM >= 3 )
    MUTUAL2_CTSUATUNE0,
#endif
#if ( MUTUAL_METHOD_NUM >= 4 )
    MUTUAL3_CTSUATUNE0,
#endif
#if ( MUTUAL_METHOD_NUM >= 5 )
    MUTUAL4_CTSUATUNE0,
#endif
#if ( MUTUAL_METHOD_NUM >= 6 )
    MUTUAL5_CTSUATUNE0,
#endif
#if ( MUTUAL_METHOD_NUM >= 7 )
    MUTUAL6_CTSUATUNE0,
#endif
#if ( MUTUAL_METHOD_NUM >= 8 )
    MUTUAL7_CTSUATUNE0
#endif
};

const uint8_t g_ctsu_atune1[] = 
{
#if ( SELF_METHOD_NUM == 1 )
    SELF_CTSUATUNE1,
#endif
#if ( MUTUAL_METHOD_NUM >= 1 )
    MUTUAL0_CTSUATUNE1,
#endif
#if ( MUTUAL_METHOD_NUM >= 2 )
    MUTUAL1_CTSUATUNE1,
#endif
#if ( MUTUAL_METHOD_NUM >= 3 )
    MUTUAL2_CTSUATUNE1,
#endif
#if ( MUTUAL_METHOD_NUM >= 4 )
    MUTUAL3_CTSUATUNE1,
#endif
#if ( MUTUAL_METHOD_NUM >= 5 )
    MUTUAL4_CTSUATUNE1,
#endif
#if ( MUTUAL_METHOD_NUM >= 6 )
    MUTUAL5_CTSUATUNE1,
#endif
#if ( MUTUAL_METHOD_NUM >= 7 )
    MUTUAL6_CTSUATUNE1,
#endif
#if ( MUTUAL_METHOD_NUM >= 8 )
    MUTUAL7_CTSUATUNE1
#endif
};

const uint8_t g_ctsu_prratio[] = 
{
#if ( SELF_METHOD_NUM == 1 )
    SELF_CTSUPRRATIO,
#endif
#if ( MUTUAL_METHOD_NUM >= 1 )
    MUTUAL0_CTSUPRRATIO,
#endif
#if ( MUTUAL_METHOD_NUM >= 2 )
    MUTUAL1_CTSUPRRATIO,
#endif
#if ( MUTUAL_METHOD_NUM >= 3 )
    MUTUAL2_CTSUPRRATIO,
#endif
#if ( MUTUAL_METHOD_NUM >= 4 )
    MUTUAL3_CTSUPRRATIO,
#endif
#if ( MUTUAL_METHOD_NUM >= 5 )
    MUTUAL4_CTSUPRRATIO,
#endif
#if ( MUTUAL_METHOD_NUM >= 6 )
    MUTUAL5_CTSUPRRATIO,
#endif
#if ( MUTUAL_METHOD_NUM >= 7 )
    MUTUAL6_CTSUPRRATIO,
#endif
#if ( MUTUAL_METHOD_NUM >= 8 )
    MUTUAL7_CTSUPRRATIO
#endif
};

const uint8_t g_ctsu_prmode[] = 
{
#if ( SELF_METHOD_NUM == 1 )
    SELF_CTSUPRMODE,
#endif
#if ( MUTUAL_METHOD_NUM >= 1 )
    MUTUAL0_CTSUPRMODE,
#endif
#if ( MUTUAL_METHOD_NUM >= 2 )
    MUTUAL1_CTSUPRMODE,
#endif
#if ( MUTUAL_METHOD_NUM >= 3 )
    MUTUAL2_CTSUPRMODE,
#endif
#if ( MUTUAL_METHOD_NUM >= 4 )
    MUTUAL3_CTSUPRMODE,
#endif
#if ( MUTUAL_METHOD_NUM >= 5 )
    MUTUAL4_CTSUPRMODE,
#endif
#if ( MUTUAL_METHOD_NUM >= 6 )
    MUTUAL5_CTSUPRMODE,
#endif
#if ( MUTUAL_METHOD_NUM >= 7 )
    MUTUAL6_CTSUPRMODE,
#endif
#if ( MUTUAL_METHOD_NUM >= 8 )
    MUTUAL7_CTSUPRMODE
#endif
};

const uint8_t g_ctsu_soff[] = 
{
#if ( SELF_METHOD_NUM == 1 )
    SELF_CTSUSOFF,
#endif
#if ( MUTUAL_METHOD_NUM >= 1 )
    MUTUAL0_CTSUSOFF,
#endif
#if ( MUTUAL_METHOD_NUM >= 2 )
    MUTUAL1_CTSUSOFF,
#endif
#if ( MUTUAL_METHOD_NUM >= 3 )
    MUTUAL2_CTSUSOFF,
#endif
#if ( MUTUAL_METHOD_NUM >= 4 )
    MUTUAL3_CTSUSOFF,
#endif
#if ( MUTUAL_METHOD_NUM >= 5 )
    MUTUAL4_CTSUSOFF,
#endif
#if ( MUTUAL_METHOD_NUM >= 6 )
    MUTUAL5_CTSUSOFF,
#endif
#if ( MUTUAL_METHOD_NUM >= 7 )
    MUTUAL6_CTSUSOFF,
#endif
#if ( MUTUAL_METHOD_NUM >= 8 )
    MUTUAL7_CTSUSOFF
#endif
};

/*===== DTC transmit data ========================================================*/
#if ( SELF_METHOD_NUM == 1 )
const uint16_t g_self_ctsussc[];
const uint16_t g_self_ctsuso0[];
const uint16_t g_self_ctsuso1[];
#endif    // ( SELF_METHOD_NUM == 1 )

/*=====    Mutual 0    DTC transmit data ========================================================*/
#if ( MUTUAL_METHOD_NUM > 0 )
const uint16_t g_mutual0_ctsussc[];
const uint16_t g_mutual0_ctsuso0[];
const uint16_t g_mutual0_ctsuso1[];
#endif    // ( MUTUAL_METHOD_NUM > 0 )

/*=====    Mutual 1    DTC transmit data ========================================================*/
#if ( MUTUAL_METHOD_NUM > 1 )
const uint16_t g_mutual1_ctsussc[];
const uint16_t g_mutual1_ctsuso0[];
const uint16_t g_mutual1_ctsuso1[];
#endif    // ( MUTUAL_METHOD_NUM > 1 )

/*=====    Mutual 2    DTC transmit data ========================================================*/
#if ( MUTUAL_METHOD_NUM > 2 )
const uint16_t g_mutual2_ctsussc[];
const uint16_t g_mutual2_ctsuso0[];
const uint16_t g_mutual2_ctsuso1[];
#endif  // ( MUTUAL_METHOD_NUM > 2 )

/*=====    Mutual 3    DTC transmit data ========================================================*/
#if ( MUTUAL_METHOD_NUM > 3 )
const uint16_t g_mutual3_ctsussc[];
const uint16_t g_mutual3_ctsuso0[];
const uint16_t g_mutual3_ctsuso1[];
#endif  // ( MUTUAL_METHOD_NUM > 3 )

/*=====    Mutual 4    DTC transmit data ========================================================*/
#if ( MUTUAL_METHOD_NUM > 4 )
const uint16_t g_mutual4_ctsussc[];
const uint16_t g_mutual4_ctsuso0[];
const uint16_t g_mutual4_ctsuso1[];
#endif  // ( MUTUAL_METHOD_NUM > 4 )

/*=====    Mutual 5    DTC transmit data ========================================================*/
#if ( MUTUAL_METHOD_NUM > 5 )
const uint16_t g_mutual5_ctsussc[];
const uint16_t g_mutual5_ctsuso0[];
const uint16_t g_mutual5_ctsuso1[];
#endif  // ( MUTUAL_METHOD_NUM > 5 )

/*=====    Mutual 6    DTC transmit data ========================================================*/
#if ( MUTUAL_METHOD_NUM > 6 )
const uint16_t g_mutual6_ctsussc[];
const uint16_t g_mutual6_ctsuso0[];
const uint16_t g_mutual6_ctsuso1[];
#endif  // ( MUTUAL_METHOD_NUM > 6 )

/*=====    Mutual 7    DTC transmit data ========================================================*/
#if ( MUTUAL_METHOD_NUM > 7 )
const uint16_t g_mutual7_ctsussc[];
const uint16_t g_mutual7_ctsuso0[];
const uint16_t g_mutual7_ctsuso1[];
#endif  // ( MUTUAL_METHOD_NUM > 7 )

/***********************************************************************************************************************
* Function Name: ctsu_sensor_average_address_set
* Description  : ctsu parameter address_setting
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void ctsu_sensor_average_address_set( void )
{
    uint8_t num;

    num = 0;

#if ( SELF_METHOD_NUM == 1 )
    g_dtc_write_data[num]                        = &g_self_write[0];
    g_self_ico_data_pt[num]                      = &g_self_ico_data[0];
    g_self_ico_sensor_pt[num]                    = &g_self_ico_data[0].sen;
    g_self_ico_reference_pt[num]                 = &g_self_ico_data[0].ref;
    g_self_add_sensor_pt[num]                    = &g_self_add_sensor[0];
    g_self_add_reference_pt[num]                 = &g_self_add_reference[0];
    g_self_sensor_cnt_pt[num]                    = &g_self_sensor_cnt[0];
    g_self_reference_cnt_pt[num]                 = &g_self_reference_cnt[0];
    g_ctsu_transmit_register_data[num].ctsu_ssc  = &g_self_ctsussc[0];
    g_ctsu_transmit_register_data[num].ctsu_so0  = &g_self_ctsuso0[0];
    g_ctsu_transmit_register_data[num].ctsu_so1  = &g_self_ctsuso1[0];

    g_ctsu_parameter[num].ctsu_chac01            = CTSUCHAC0_SELF_ENABLE_TS | (CTSUCHAC1_SELF_ENABLE_TS << 8);
    g_ctsu_parameter[num].ctsu_chac23            = CTSUCHAC2_SELF_ENABLE_TS | (CTSUCHAC3_SELF_ENABLE_TS << 8);
    g_ctsu_parameter[num].ctsu_chac4             = CTSUCHAC4_SELF_ENABLE_TS;
    g_mpc_set_sensor0                            = g_ctsu_parameter[num].ctsu_chac01;
    g_mpc_set_sensor1                            = g_ctsu_parameter[num].ctsu_chac23;
    g_mpc_set_sensor2                            = g_ctsu_parameter[num].ctsu_chac4;

    g_current_offset_pm_pt[num]                  = &g_self_current_offset_pm[0];

    num  = (uint8_t)(num + 1);
#endif    // ( SELF_METHOD_NUM == 1 )

#if ( MUTUAL_METHOD_NUM >= 1 )
    g_dtc_write_data[num]                        = &g_mutual0_write[0];
    g_mutual_ico_data_pt[num]                    = &g_mutual0_ico_data[0];
    g_mutual_ico_pri_sensor_pt[num]              = &g_mutual0_ico_data[0].pri_sen;
    g_mutual_ico_pri_reference_pt[num]           = &g_mutual0_ico_data[0].pri_ref;
    g_mutual_ico_snd_sensor_pt[num]              = &g_mutual0_ico_data[0].snd_sen;
    g_mutual_ico_snd_reference_pt[num]           = &g_mutual0_ico_data[0].snd_ref;
    g_mutual_add_pri_sensor_pt[num]              = &g_mutual0_add_pri_sensor[0];
    g_mutual_add_pri_reference_pt[num]           = &g_mutual0_add_pri_reference[0];
    g_mutual_add_snd_sensor_pt[num]              = &g_mutual0_add_snd_sensor[0];
    g_mutual_add_snd_reference_pt[num]           = &g_mutual0_add_snd_reference[0];
    g_mutual_sensor_diff_pt[num]                 = &g_mutual0_sensor_diff[0];
    g_mutual_reference_diff_pt[num]              = &g_mutual0_reference_diff[0];
    g_ctsu_transmit_register_data[num].ctsu_ssc  = &g_mutual0_ctsussc[0];
    g_ctsu_transmit_register_data[num].ctsu_so0  = &g_mutual0_ctsuso0[0];
    g_ctsu_transmit_register_data[num].ctsu_so1  = &g_mutual0_ctsuso1[0];

    g_ctsu_parameter[num].ctsu_chac01            = CTSUCHAC0_MUTUAL0_ENABLE_TS | (CTSUCHAC1_MUTUAL0_ENABLE_TS << 8);
    g_ctsu_parameter[num].ctsu_chac23            = CTSUCHAC2_MUTUAL0_ENABLE_TS | (CTSUCHAC3_MUTUAL0_ENABLE_TS << 8);
    g_ctsu_parameter[num].ctsu_chac4             = CTSUCHAC4_MUTUAL0_ENABLE_TS;
    g_mpc_set_sensor0                            = g_mpc_set_sensor0 | g_ctsu_parameter[num].ctsu_chac01;
    g_mpc_set_sensor1                            = g_mpc_set_sensor1 | g_ctsu_parameter[num].ctsu_chac23;
    g_mpc_set_sensor2                            = g_mpc_set_sensor2 | g_ctsu_parameter[num].ctsu_chac4;

    g_ctsu_parameter[num].ctsu_chtrc01           = CTSUCHTRC0_MUTUAL0_TRANSMIT_TS | (CTSUCHTRC1_MUTUAL0_TRANSMIT_TS << 8);
    g_ctsu_parameter[num].ctsu_chtrc23           = CTSUCHTRC2_MUTUAL0_TRANSMIT_TS | (CTSUCHTRC3_MUTUAL0_TRANSMIT_TS << 8);
    g_ctsu_parameter[num].ctsu_chtrc4            = CTSUCHTRC4_MUTUAL0_TRANSMIT_TS;

    g_current_offset_pm_pt[num]                  = &g_mutual0_current_offset_pm[0];

    num  = (uint8_t)(num + 1);
#endif    // ( MUTUAL_METHOD_NUM >= 1 )

#if ( MUTUAL_METHOD_NUM >= 2 )
    g_dtc_write_data[num]                        = &g_mutual1_write[0];
    g_mutual_ico_data_pt[num]                    = &g_mutual1_ico_data[0];
    g_mutual_ico_pri_sensor_pt[num]              = &g_mutual1_ico_data[0].pri_sen;
    g_mutual_ico_pri_reference_pt[num]           = &g_mutual1_ico_data[0].pri_ref;
    g_mutual_ico_snd_sensor_pt[num]              = &g_mutual1_ico_data[0].snd_sen;
    g_mutual_ico_snd_reference_pt[num]           = &g_mutual1_ico_data[0].snd_ref;
    g_mutual_add_pri_sensor_pt[num]              = &g_mutual1_add_pri_sensor[0];
    g_mutual_add_pri_reference_pt[num]           = &g_mutual1_add_pri_reference[0];
    g_mutual_add_snd_sensor_pt[num]              = &g_mutual1_add_snd_sensor[0];
    g_mutual_add_snd_reference_pt[num]           = &g_mutual1_add_snd_reference[0];
    g_mutual_sensor_diff_pt[num]                 = &g_mutual1_sensor_diff[0];
    g_mutual_reference_diff_pt[num]              = &g_mutual1_reference_diff[0];
    g_ctsu_transmit_register_data[num].ctsu_ssc  = &g_mutual1_ctsussc[0];
    g_ctsu_transmit_register_data[num].ctsu_so0  = &g_mutual1_ctsuso0[0];
    g_ctsu_transmit_register_data[num].ctsu_so1  = &g_mutual1_ctsuso1[0];

    g_ctsu_parameter[num].ctsu_chac01            = CTSUCHAC0_MUTUAL1_ENABLE_TS | (CTSUCHAC1_MUTUAL1_ENABLE_TS << 8);
    g_ctsu_parameter[num].ctsu_chac23            = CTSUCHAC2_MUTUAL1_ENABLE_TS | (CTSUCHAC3_MUTUAL1_ENABLE_TS << 8);
    g_ctsu_parameter[num].ctsu_chac4             = CTSUCHAC4_MUTUAL1_ENABLE_TS;
    g_mpc_set_sensor0                            = g_mpc_set_sensor0 | g_ctsu_parameter[num].ctsu_chac01;
    g_mpc_set_sensor1                            = g_mpc_set_sensor1 | g_ctsu_parameter[num].ctsu_chac23;
    g_mpc_set_sensor2                            = g_mpc_set_sensor2 | g_ctsu_parameter[num].ctsu_chac4;

    g_ctsu_parameter[num].ctsu_chtrc01           = CTSUCHTRC0_MUTUAL1_TRANSMIT_TS | (CTSUCHTRC1_MUTUAL1_TRANSMIT_TS << 8);
    g_ctsu_parameter[num].ctsu_chtrc23           = CTSUCHTRC2_MUTUAL1_TRANSMIT_TS | (CTSUCHTRC3_MUTUAL1_TRANSMIT_TS << 8);
    g_ctsu_parameter[num].ctsu_chtrc4            = CTSUCHTRC4_MUTUAL1_TRANSMIT_TS;

    g_current_offset_pm_pt[num]                  = &g_mutual1_current_offset_pm[0];

    num  = (uint8_t)(num + 1);
#endif    // ( MUTUAL_METHOD_NUM >= 2 )

#if ( MUTUAL_METHOD_NUM >= 3 )
    g_dtc_write_data[num]                        = &g_mutual2_write[0];
    g_mutual_ico_data_pt[num]                    = &g_mutual2_ico_data[0];
    g_mutual_ico_pri_sensor_pt[num]              = &g_mutual2_ico_data[0].pri_sen;
    g_mutual_ico_pri_reference_pt[num]           = &g_mutual2_ico_data[0].pri_ref;
    g_mutual_ico_snd_sensor_pt[num]              = &g_mutual2_ico_data[0].snd_sen;
    g_mutual_ico_snd_reference_pt[num]           = &g_mutual2_ico_data[0].snd_ref;
    g_mutual_add_pri_sensor_pt[num]              = &g_mutual2_add_pri_sensor[0];
    g_mutual_add_pri_reference_pt[num]           = &g_mutual2_add_pri_reference[0];
    g_mutual_add_snd_sensor_pt[num]              = &g_mutual2_add_snd_sensor[0];
    g_mutual_add_snd_reference_pt[num]           = &g_mutual2_add_snd_reference[0];
    g_mutual_sensor_diff_pt[num]                 = &g_mutual2_sensor_diff[0];
    g_mutual_reference_diff_pt[num]              = &g_mutual2_reference_diff[0];
    g_ctsu_transmit_register_data[num].ctsu_ssc  = &g_mutual2_ctsussc[0];
    g_ctsu_transmit_register_data[num].ctsu_so0  = &g_mutual2_ctsuso0[0];
    g_ctsu_transmit_register_data[num].ctsu_so1  = &g_mutual2_ctsuso1[0];

    g_ctsu_parameter[num].ctsu_chac01            = CTSUCHAC0_MUTUAL2_ENABLE_TS | (CTSUCHAC1_MUTUAL2_ENABLE_TS << 8);
    g_ctsu_parameter[num].ctsu_chac23            = CTSUCHAC2_MUTUAL2_ENABLE_TS | (CTSUCHAC3_MUTUAL2_ENABLE_TS << 8);
    g_ctsu_parameter[num].ctsu_chac4             = CTSUCHAC4_MUTUAL2_ENABLE_TS;
    g_mpc_set_sensor0                            = g_mpc_set_sensor0 | g_ctsu_parameter[num].ctsu_chac01;
    g_mpc_set_sensor1                            = g_mpc_set_sensor1 | g_ctsu_parameter[num].ctsu_chac23;
    g_mpc_set_sensor2                            = g_mpc_set_sensor2 | g_ctsu_parameter[num].ctsu_chac4;

    g_ctsu_parameter[num].ctsu_chtrc01           = CTSUCHTRC0_MUTUAL2_TRANSMIT_TS | (CTSUCHTRC1_MUTUAL2_TRANSMIT_TS << 8);
    g_ctsu_parameter[num].ctsu_chtrc23           = CTSUCHTRC2_MUTUAL2_TRANSMIT_TS | (CTSUCHTRC3_MUTUAL2_TRANSMIT_TS << 8);
    g_ctsu_parameter[num].ctsu_chtrc4            = CTSUCHTRC4_MUTUAL2_TRANSMIT_TS;

    g_current_offset_pm_pt[num]                  = &g_mutual2_current_offset_pm[0];

    num  = (uint8_t)(num + 1);
#endif    // ( MUTUAL_METHOD_NUM >= 3 )

#if ( MUTUAL_METHOD_NUM >= 4 )
    g_dtc_write_data[num]                        = &g_mutual3_write[0];
    g_mutual_ico_data_pt[num]                    = &g_mutual3_ico_data[0];
    g_mutual_ico_pri_sensor_pt[num]              = &g_mutual3_ico_data[0].pri_sen;
    g_mutual_ico_pri_reference_pt[num]           = &g_mutual3_ico_data[0].pri_ref;
    g_mutual_ico_snd_sensor_pt[num]              = &g_mutual3_ico_data[0].snd_sen;
    g_mutual_ico_snd_reference_pt[num]           = &g_mutual3_ico_data[0].snd_ref;
    g_mutual_add_pri_sensor_pt[num]              = &g_mutual3_add_pri_sensor[0];
    g_mutual_add_pri_reference_pt[num]           = &g_mutual3_add_pri_reference[0];
    g_mutual_add_snd_sensor_pt[num]              = &g_mutual3_add_snd_sensor[0];
    g_mutual_add_snd_reference_pt[num]           = &g_mutual3_add_snd_reference[0];
    g_mutual_sensor_diff_pt[num]                 = &g_mutual3_sensor_diff[0];
    g_mutual_reference_diff_pt[num]              = &g_mutual3_reference_diff[0];
    g_ctsu_transmit_register_data[num].ctsu_ssc  = &g_mutual3_ctsussc[0];
    g_ctsu_transmit_register_data[num].ctsu_so0  = &g_mutual3_ctsuso0[0];
    g_ctsu_transmit_register_data[num].ctsu_so1  = &g_mutual3_ctsuso1[0];

    g_ctsu_parameter[num].ctsu_chac01            = CTSUCHAC0_MUTUAL3_ENABLE_TS | (CTSUCHAC1_MUTUAL3_ENABLE_TS << 8);
    g_ctsu_parameter[num].ctsu_chac23            = CTSUCHAC2_MUTUAL3_ENABLE_TS | (CTSUCHAC3_MUTUAL3_ENABLE_TS << 8);
    g_ctsu_parameter[num].ctsu_chac4             = CTSUCHAC4_MUTUAL3_ENABLE_TS;
    g_mpc_set_sensor0                            = g_mpc_set_sensor0 | g_ctsu_parameter[num].ctsu_chac01;
    g_mpc_set_sensor1                            = g_mpc_set_sensor1 | g_ctsu_parameter[num].ctsu_chac23;
    g_mpc_set_sensor2                            = g_mpc_set_sensor2 | g_ctsu_parameter[num].ctsu_chac4;

    g_ctsu_parameter[num].ctsu_chtrc01           = CTSUCHTRC0_MUTUAL3_TRANSMIT_TS | (CTSUCHTRC1_MUTUAL3_TRANSMIT_TS << 8);
    g_ctsu_parameter[num].ctsu_chtrc23           = CTSUCHTRC2_MUTUAL3_TRANSMIT_TS | (CTSUCHTRC3_MUTUAL3_TRANSMIT_TS << 8);
    g_ctsu_parameter[num].ctsu_chtrc4            = CTSUCHTRC4_MUTUAL3_TRANSMIT_TS;

    g_current_offset_pm_pt[num]                  = &g_mutual3_current_offset_pm[0];

    num  = (uint8_t)(num + 1);
#endif    // ( MUTUAL_METHOD_NUM >= 4 )

#if ( MUTUAL_METHOD_NUM >= 5 )
    g_dtc_write_data[num]                        = &g_mutual4_write[0];
    g_mutual_ico_data_pt[num]                    = &g_mutual4_ico_data[0];
    g_mutual_ico_pri_sensor_pt[num]              = &g_mutual4_ico_data[0].pri_sen;
    g_mutual_ico_pri_reference_pt[num]           = &g_mutual4_ico_data[0].pri_ref;
    g_mutual_ico_snd_sensor_pt[num]              = &g_mutual4_ico_data[0].snd_sen;
    g_mutual_ico_snd_reference_pt[num]           = &g_mutual4_ico_data[0].snd_ref;
    g_mutual_add_pri_sensor_pt[num]              = &g_mutual4_add_pri_sensor[0];
    g_mutual_add_pri_reference_pt[num]           = &g_mutual4_add_pri_reference[0];
    g_mutual_add_snd_sensor_pt[num]              = &g_mutual4_add_snd_sensor[0];
    g_mutual_add_snd_reference_pt[num]           = &g_mutual4_add_snd_reference[0];
    g_mutual_sensor_diff_pt[num]                 = &g_mutual4_sensor_diff[0];
    g_mutual_reference_diff_pt[num]              = &g_mutual4_reference_diff[0];
    g_ctsu_transmit_register_data[num].ctsu_ssc  = &g_mutual4_ctsussc[0];
    g_ctsu_transmit_register_data[num].ctsu_so0  = &g_mutual4_ctsuso0[0];
    g_ctsu_transmit_register_data[num].ctsu_so1  = &g_mutual4_ctsuso1[0];

    g_ctsu_parameter[num].ctsu_chac01            = CTSUCHAC0_MUTUAL4_ENABLE_TS | (CTSUCHAC1_MUTUAL4_ENABLE_TS << 8);
    g_ctsu_parameter[num].ctsu_chac23            = CTSUCHAC2_MUTUAL4_ENABLE_TS | (CTSUCHAC3_MUTUAL4_ENABLE_TS << 8);
    g_ctsu_parameter[num].ctsu_chac4             = CTSUCHAC4_MUTUAL4_ENABLE_TS;
    g_mpc_set_sensor0                            = g_mpc_set_sensor0 | g_ctsu_parameter[num].ctsu_chac01;
    g_mpc_set_sensor1                            = g_mpc_set_sensor1 | g_ctsu_parameter[num].ctsu_chac23;
    g_mpc_set_sensor2                            = g_mpc_set_sensor2 | g_ctsu_parameter[num].ctsu_chac4;

    g_ctsu_parameter[num].ctsu_chtrc01           = CTSUCHTRC0_MUTUAL4_TRANSMIT_TS | (CTSUCHTRC1_MUTUAL4_TRANSMIT_TS << 8);
    g_ctsu_parameter[num].ctsu_chtrc23           = CTSUCHTRC2_MUTUAL4_TRANSMIT_TS | (CTSUCHTRC3_MUTUAL4_TRANSMIT_TS << 8);
    g_ctsu_parameter[num].ctsu_chtrc4            = CTSUCHTRC4_MUTUAL4_TRANSMIT_TS;

    g_current_offset_pm_pt[num]                  = &g_mutual4_current_offset_pm[0];

    num  = (uint8_t)(num + 1);
#endif    // ( MUTUAL_METHOD_NUM >= 5 )

#if ( MUTUAL_METHOD_NUM >= 6 )
    g_dtc_write_data[num]                        = &g_mutual5_write[0];
    g_mutual_ico_data_pt[num]                    = &g_mutual5_ico_data[0];
    g_mutual_ico_pri_sensor_pt[num]              = &g_mutual5_ico_data[0].pri_sen;
    g_mutual_ico_pri_reference_pt[num]           = &g_mutual5_ico_data[0].pri_ref;
    g_mutual_ico_snd_sensor_pt[num]              = &g_mutual5_ico_data[0].snd_sen;
    g_mutual_ico_snd_reference_pt[num]           = &g_mutual5_ico_data[0].snd_ref;
    g_mutual_add_pri_sensor_pt[num]              = &g_mutual5_add_pri_sensor[0];
    g_mutual_add_pri_reference_pt[num]           = &g_mutual5_add_pri_reference[0];
    g_mutual_add_snd_sensor_pt[num]              = &g_mutual5_add_snd_sensor[0];
    g_mutual_add_snd_reference_pt[num]           = &g_mutual5_add_snd_reference[0];
    g_mutual_sensor_diff_pt[num]                 = &g_mutual5_sensor_diff[0];
    g_mutual_reference_diff_pt[num]              = &g_mutual5_reference_diff[0];
    g_ctsu_transmit_register_data[num].ctsu_ssc  = &g_mutual5_ctsussc[0];
    g_ctsu_transmit_register_data[num].ctsu_so0  = &g_mutual5_ctsuso0[0];
    g_ctsu_transmit_register_data[num].ctsu_so1  = &g_mutual5_ctsuso1[0];

    g_ctsu_parameter[num].ctsu_chac01            = CTSUCHAC0_MUTUAL5_ENABLE_TS | (CTSUCHAC1_MUTUAL5_ENABLE_TS << 8);
    g_ctsu_parameter[num].ctsu_chac23            = CTSUCHAC2_MUTUAL5_ENABLE_TS | (CTSUCHAC3_MUTUAL5_ENABLE_TS << 8);
    g_ctsu_parameter[num].ctsu_chac4             = CTSUCHAC4_MUTUAL5_ENABLE_TS;
    g_mpc_set_sensor0                            = g_mpc_set_sensor0 | g_ctsu_parameter[num].ctsu_chac01;
    g_mpc_set_sensor1                            = g_mpc_set_sensor1 | g_ctsu_parameter[num].ctsu_chac23;
    g_mpc_set_sensor2                            = g_mpc_set_sensor2 | g_ctsu_parameter[num].ctsu_chac4;

    g_ctsu_parameter[num].ctsu_chtrc01           = CTSUCHTRC0_MUTUAL5_TRANSMIT_TS | (CTSUCHTRC1_MUTUAL5_TRANSMIT_TS << 8);
    g_ctsu_parameter[num].ctsu_chtrc23           = CTSUCHTRC2_MUTUAL5_TRANSMIT_TS | (CTSUCHTRC3_MUTUAL5_TRANSMIT_TS << 8);
    g_ctsu_parameter[num].ctsu_chtrc4            = CTSUCHTRC4_MUTUAL5_TRANSMIT_TS;

    g_current_offset_pm_pt[num]                  = &g_mutual5_current_offset_pm[0];

    num  = (uint8_t)(num + 1);
#endif    // ( MUTUAL_METHOD_NUM >= 6 )

#if ( MUTUAL_METHOD_NUM >= 7 )
    g_dtc_write_data[num]                        = &g_mutual6_write[0];
    g_mutual_ico_data_pt[num]                    = &g_mutual6_ico_data[0];
    g_mutual_ico_pri_sensor_pt[num]              = &g_mutual6_ico_data[0].pri_sen;
    g_mutual_ico_pri_reference_pt[num]           = &g_mutual6_ico_data[0].pri_ref;
    g_mutual_ico_snd_sensor_pt[num]              = &g_mutual6_ico_data[0].snd_sen;
    g_mutual_ico_snd_reference_pt[num]           = &g_mutual6_ico_data[0].snd_ref;
    g_mutual_add_pri_sensor_pt[num]              = &g_mutual6_add_pri_sensor[0];
    g_mutual_add_pri_reference_pt[num]           = &g_mutual6_add_pri_reference[0];
    g_mutual_add_snd_sensor_pt[num]              = &g_mutual6_add_snd_sensor[0];
    g_mutual_add_snd_reference_pt[num]           = &g_mutual6_add_snd_reference[0];
    g_mutual_sensor_diff_pt[num]                 = &g_mutual6_sensor_diff[0];
    g_mutual_reference_diff_pt[num]              = &g_mutual6_reference_diff[0];
    g_ctsu_transmit_register_data[num].ctsu_ssc  = &g_mutual6_ctsussc[0];
    g_ctsu_transmit_register_data[num].ctsu_so0  = &g_mutual6_ctsuso0[0];
    g_ctsu_transmit_register_data[num].ctsu_so1  = &g_mutual6_ctsuso1[0];

    g_ctsu_parameter[num].ctsu_chac01            = CTSUCHAC0_MUTUAL6_ENABLE_TS | (CTSUCHAC1_MUTUAL6_ENABLE_TS << 8);
    g_ctsu_parameter[num].ctsu_chac23            = CTSUCHAC2_MUTUAL6_ENABLE_TS | (CTSUCHAC3_MUTUAL6_ENABLE_TS << 8);
    g_ctsu_parameter[num].ctsu_chac4             = CTSUCHAC4_MUTUAL6_ENABLE_TS;
    g_mpc_set_sensor0                            = g_mpc_set_sensor0 | g_ctsu_parameter[num].ctsu_chac01;
    g_mpc_set_sensor1                            = g_mpc_set_sensor1 | g_ctsu_parameter[num].ctsu_chac23;
    g_mpc_set_sensor2                            = g_mpc_set_sensor2 | g_ctsu_parameter[num].ctsu_chac4;

    g_ctsu_parameter[num].ctsu_chtrc01           = CTSUCHTRC0_MUTUAL6_TRANSMIT_TS | (CTSUCHTRC1_MUTUAL6_TRANSMIT_TS << 8);
    g_ctsu_parameter[num].ctsu_chtrc23           = CTSUCHTRC2_MUTUAL6_TRANSMIT_TS | (CTSUCHTRC3_MUTUAL6_TRANSMIT_TS << 8);
    g_ctsu_parameter[num].ctsu_chtrc4            = CTSUCHTRC4_MUTUAL6_TRANSMIT_TS;

    g_current_offset_pm_pt[num]                  = &g_mutual6_current_offset_pm[0];

    num  = (uint8_t)(num + 1);
#endif    // ( MUTUAL_METHOD_NUM >= 7 )

#if ( MUTUAL_METHOD_NUM >= 8 )
    g_dtc_write_data[num]                        = &g_mutual7_write[0];
    g_mutual_ico_data_pt[num]                    = &g_mutual7_ico_data[0];
    g_mutual_ico_pri_sensor_pt[num]              = &g_mutual7_ico_data[0].pri_sen;
    g_mutual_ico_pri_reference_pt[num]           = &g_mutual7_ico_data[0].pri_ref;
    g_mutual_ico_snd_sensor_pt[num]              = &g_mutual7_ico_data[0].snd_sen;
    g_mutual_ico_snd_reference_pt[num]           = &g_mutual7_ico_data[0].snd_ref;
    g_mutual_add_pri_sensor_pt[num]              = &g_mutual7_add_pri_sensor[0];
    g_mutual_add_pri_reference_pt[num]           = &g_mutual7_add_pri_reference[0];
    g_mutual_add_snd_sensor_pt[num]              = &g_mutual7_add_snd_sensor[0];
    g_mutual_add_snd_reference_pt[num]           = &g_mutual7_add_snd_reference[0];
    g_mutual_sensor_diff_pt[num]                 = &g_mutual7_sensor_diff[0];
    g_mutual_reference_diff_pt[num]              = &g_mutual7_reference_diff[0];
    g_ctsu_transmit_register_data[num].ctsu_ssc  = &g_mutual7_ctsussc[0];
    g_ctsu_transmit_register_data[num].ctsu_so0  = &g_mutual7_ctsuso0[0];
    g_ctsu_transmit_register_data[num].ctsu_so1  = &g_mutual7_ctsuso1[0];

    g_ctsu_parameter[num].ctsu_chac01            = CTSUCHAC0_MUTUAL7_ENABLE_TS | (CTSUCHAC1_MUTUAL7_ENABLE_TS << 8);
    g_ctsu_parameter[num].ctsu_chac23            = CTSUCHAC2_MUTUAL7_ENABLE_TS | (CTSUCHAC3_MUTUAL7_ENABLE_TS << 8);
    g_ctsu_parameter[num].ctsu_chac4             = CTSUCHAC4_MUTUAL7_ENABLE_TS;
    g_mpc_set_sensor0                            = g_mpc_set_sensor0 | g_ctsu_parameter[num].ctsu_chac01;
    g_mpc_set_sensor1                            = g_mpc_set_sensor1 | g_ctsu_parameter[num].ctsu_chac23;
    g_mpc_set_sensor2                            = g_mpc_set_sensor2 | g_ctsu_parameter[num].ctsu_chac4;

    g_ctsu_parameter[num].ctsu_chtrc01           = CTSUCHTRC0_MUTUAL7_TRANSMIT_TS | (CTSUCHTRC1_MUTUAL7_TRANSMIT_TS << 8);
    g_ctsu_parameter[num].ctsu_chtrc23           = CTSUCHTRC2_MUTUAL7_TRANSMIT_TS | (CTSUCHTRC3_MUTUAL7_TRANSMIT_TS << 8);
    g_ctsu_parameter[num].ctsu_chtrc4            = CTSUCHTRC4_MUTUAL7_TRANSMIT_TS;

    g_current_offset_pm_pt[num]                  = &g_mutual7_current_offset_pm[0];

    num  = (uint8_t)(num + 1);
#endif    // ( MUTUAL_METHOD_NUM >= 8 )
}    /* End of function ctsu_sensor_average_address_set() */

/***********************************************************************************************************************
* Function Name: CTSU_register_initial_value_ram_set
* Description  : This function storage CTSU register initial value for RAM
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void CTSU_register_initial_value_ram_set( void )
{
    uint8_t method;

    for (method = 0; method < METHOD_NUM; method++)
    {
        /*****    A0900h CTSU channel register0 value storage    ******************************************************/
        g_ctsu_parameter[method].ctsu_snz          = _0_CTSUSNZ_LOW_POWER_DISABLE;
        g_ctsu_parameter[method].ctsu_txvsel       = g_ctsu_txvsel[method];

        /*****    A0901h CTSU channel register1 value storage    ******************************************************/
        g_ctsu_parameter[method].ctsu_pon          = _1_CTSUPON_HW_POWER_ON;
        g_ctsu_parameter[method].ctsu_sw           = _1_CTSUCSW_ON;
        g_ctsu_parameter[method].ctsu_atune0       = g_ctsu_atune0[method];
        g_ctsu_parameter[method].ctsu_clk          = _00_CTSUCLK_PCLK;
        g_ctsu_parameter[method].ctsu_atune1       = g_ctsu_atune1[method];

        if (0 == g_key_info[method].mode)
        {
            g_ctsu_parameter[method].ctsu_md       = _01_CTSUMD_SELF_MULTI;
        }
        else
        {
            g_ctsu_parameter[method].ctsu_md       = _11_CTSUMD_MUTUAL;
        }

        /*****    A0902h CTSU Sensor drive pulse spectrum diffusion set register value storage    *********************/
        g_ctsu_parameter[method].ctsu_prratio      = g_ctsu_prratio[method];
        g_ctsu_parameter[method].ctsu_prmode       = g_ctsu_prmode[method];
        g_ctsu_parameter[method].ctsu_soff         = g_ctsu_soff[method];

        /*****    A0903h CTSU Sensor wait time register value storage    **********************************************/
        g_ctsu_parameter[method].ctsu_sst          = _00010000_CTSUSST_RECOMMEND;

        /*****    A0904h CTSU Measurement channel register0 value storage    ******************************************/
        if (_00_CTSUMD_SELF_SINGLE == g_ctsu_parameter[method].ctsu_md)
        {
            g_ctsu_parameter[method].ctsu_mch0     = _000000_CTSUMCH0_TS00;
        }

        /*****    A0906h CTSU Channel enable control register0 value storage    ***************************************/
        /*****    A0907h CTSU Channel enable control register1 value storage    ***************************************/

        /*====    Prameter set by ctsu_sensor_average_address_set()    ===============================================*/

        /*****    A090Bh CTSU Channel send and receive control register0 value storage    *****************************/
        /*****    A090Ch CTSU Channel send and receive control register0 value storage    *****************************/

        /*====    Prameter set by ctsu_sensor_average_address_set()    ===============================================*/

        /*****    A0912h CTSU Spectrum diffusion control register value storage    ************************************/
        g_ctsu_parameter[method].ctsu_ssmod         = _00_CTSUSSMOD;
        g_ctsu_parameter[method].ctsu_sscnt         = _11_CTSUSSCNT;
    }
}    /* End of function CTSU_register_initial_value_ram_set() */

/***********************************************************************************************************************
* Function Name: R_Set_CTSU_All_Register
* Description  : This function implements main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
uint16_t R_Set_CTSU_All_Register( uint8_t method )
{
    volatile uint16_t ret_val;
    volatile uint16_t status;

    status = 0;

    /* TSCAP Capacitor discharge setting */
    R_Set_CTSU_TSCAP_Discharge();

    /* Multi-Function Pin Controller setting */
    MPC_CTSU_set( g_mpc_set_sensor0, g_mpc_set_sensor1, g_mpc_set_sensor2 );

    /* Operation mode Register change protect off setting */
    R_Set_CTSU_Module_Operation(_0_MSTPD10_CTSU_ENABLE);

#if (MCU_SERIES_T == R5F5130)
    /* CTSU transmit power supply set process call */
    ret_val = R_Set_CTSU_Transmit_Power_Supply(g_ctsu_parameter[method].ctsu_txvsel);
    status = ret_val;
#endif    // (MCU_SERIES_T == R5F5130)

    /* CTSU analog adjustment Signal0 set process call */
    ret_val = R_Set_CTSU_Power_Operation_Mode(g_ctsu_parameter[method].ctsu_atune0);
    status  = status | (ret_val << 1);

    /* CTSU analog adjustment Signal1 set  process call */
    ret_val = R_Set_CTSU_Power_Capacity_Adjustment(g_ctsu_parameter[method].ctsu_atune1);
    status  = status | (ret_val << 2);

    /* CTSU count Source select process call */
    ret_val = R_Set_CTSU_Operation_Clock(g_ctsu_parameter[method].ctsu_clk);
    status  = status | (ret_val << 3);

    /* CTSU sensor wait time set process call */
    ret_val = R_Set_CTSU_Sensor_Stabilization_Wait_Time(g_ctsu_parameter[method].ctsu_sst);
    status  = status | (ret_val << 4);

    /* CTSU hard macro power supply switch change process call */
    ret_val = R_Set_CTSU_Power_Supply(g_ctsu_parameter[method].ctsu_pon);
    status  = status | (ret_val << 5);

    /* CTSU measurement mode select process call */
    ret_val = R_Set_CTSU_Measurement_Mode(g_ctsu_parameter[method].ctsu_md);
    status  = status | (ret_val << 6);

    /* CTSU Sensor Drive Pulse Spectrum Diffusion Setting Register (CTSUSDPRS) */
    ret_val = R_Set_CTSU_Synchronous_Noise_Reduction(g_ctsu_parameter[method].ctsu_prratio, g_ctsu_parameter[method].ctsu_prmode, g_ctsu_parameter[method].ctsu_soff);
    status  = status | (ret_val << 7);

    /* CTSU measurement channel set process call */
    ret_val = R_Set_CTSU_Measurement_Channel(g_ctsu_parameter[method].ctsu_mch0);

    /* CTSU channel enable set process call */
    ret_val = R_Set_CTSU_Channel_Enable0(g_ctsu_parameter[method].ctsu_chac01);
    status  = status | (ret_val << 9);

#if (MCU_SERIES_T == R5F5231 || MCU_SERIES_T == R5F5130 || MCU_SERIES_T == R5F5230)
    ret_val = R_Set_CTSU_Channel_Enable1(g_ctsu_parameter[method].ctsu_chac23);
    status  = status | (ret_val << 10);
    ret_val = R_Set_CTSU_Channel_Enable2(g_ctsu_parameter[method].ctsu_chac4);
    status  = status | (ret_val << 11);
#endif    // (MCU_SERIES_T == R5F5231 || MCU_SERIES_T == R5F5130 || MCU_SERIES_T == R5F5230)

    /* CTSU Channel transmit/receive control process call */
    ret_val = R_Set_CTSU_Channel_Transmit_Receive0(g_ctsu_parameter[method].ctsu_chtrc01);
    status  = status | (ret_val << 12);

#if (MCU_SERIES_T == R5F5231 || MCU_SERIES_T == R5F5130 || MCU_SERIES_T == R5F5230)
    ret_val = R_Set_CTSU_Channel_Transmit_Receive1(g_ctsu_parameter[method].ctsu_chtrc23);
    status  = status | (ret_val << 13);
    ret_val = R_Set_CTSU_Channel_Transmit_Receive2(g_ctsu_parameter[method].ctsu_chtrc4);
    status  = status | (ret_val << 14);
#endif    // (MCU_SERIES_T == R5F5231 || MCU_SERIES_T == R5F5130 || MCU_SERIES_T == R5F5230)

    /* CTSU diffusion clock mode and oscillation frequency select process call */
    ret_val = R_Set_CTSU_High_Pass_Noise_Reduction(g_ctsu_parameter[method].ctsu_ssmod, g_ctsu_parameter[method].ctsu_sscnt);
    status  = status | (ret_val << 15);

    return status;
}    /* End of function R_Set_CTSU_All_Register() */

/***********************************************************************************************************************
* Function Name: DTC_transmit_data_set
* Description  : DTC transmi write data storage
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void DTC_transmit_data_set(void)
{
    uint8_t method;
    uint8_t loop;
    uint8_t pt;

    for (method = 0; method < METHOD_NUM; method++)
    {
        for (loop = 0, pt = 0; loop < g_key_info[method].ena_num; loop++)
        {
            /* CTSU Spectrum Diffusion Control Register initial value    */
             *(g_dtc_write_data[method]  +  pt     )  = *(g_ctsu_transmit_register_data[method].ctsu_ssc + loop);

            /* CTSU Sensor Offset Register 0 initial value    */
             *(g_dtc_write_data[method]  + (pt + 1))  = *(g_ctsu_transmit_register_data[method].ctsu_so0 + loop);

            /* CTSU Sensor Offset Register 1 initial value    */
             *(g_dtc_write_data[method]  + (pt + 2))  = *(g_ctsu_transmit_register_data[method].ctsu_so1 + loop);

            /* next buffer pointer set    */
            pt = (uint8_t)(pt + 3);
        }
    }
}    /* End of function DTC_transmit_data_set() */

/***********************************************************************************************************************
* Function Name: touch_data_moving_average
* Description  : touch data moving average
* Arguments    : uint8_t method    : Maeasurement method : 0-8
* Return Value : none
***********************************************************************************************************************/
void touch_data_moving_average( uint8_t method )
{
    uint8_t     pt;
    uint8_t     offset;
    uint32_t    scount_work;
    uint32_t    scount_work2;
    uint32_t    rcount_work;
    uint32_t    rcount_work2;

    offset  = 0;

    for (pt = 0; pt < g_key_info[method].ena_num; pt++)
    {
        if (0 == g_touch_function[method].flag.average)
        {
            if (0 == g_key_info[method].mode)
            {
               *(g_self_add_sensor_pt[method]    +  pt) = *(g_self_ico_sensor_pt[method]     + (pt + offset));
               *(g_self_add_reference_pt[method] +  pt) = *(g_self_ico_reference_pt[method]  + (pt + offset));
            }
            else
            {
               *(g_mutual_add_pri_sensor_pt[method]    + pt) = *(g_mutual_ico_pri_sensor_pt[method]     + (pt + offset));
               *(g_mutual_add_pri_reference_pt[method] + pt) = *(g_mutual_ico_pri_reference_pt[method]  + (pt + offset));
               *(g_mutual_add_snd_sensor_pt[method]    + pt) = *(g_mutual_ico_snd_sensor_pt[method]     + (pt + offset));
               *(g_mutual_add_snd_reference_pt[method] + pt) = *(g_mutual_ico_snd_reference_pt[method]  + (pt + offset));
            }
        }
        else
        {
            if (0 == g_key_info[method].mode)
            {
                scount_work  = *(g_self_add_sensor_pt[method]    + pt);             /* Get Add data            */
                rcount_work  = *(g_self_add_reference_pt[method] + pt);             /* Get Add data            */
                scount_work -= scount_work / ADD_TIME;                                    /* Average / ADD_TIME      */
                rcount_work -= rcount_work / ADD_TIME;                                    /* Average / ADD_TIME      */
                scount_work += *(g_self_ico_sensor_pt[method]     + (pt + offset)) / ADD_TIME;     /* Add Now data / ADD_TIME     */
                rcount_work += *(g_self_ico_reference_pt[method]  + (pt + offset)) / ADD_TIME;     /* Add Now data / ADD_TIME     */

               *(g_self_add_sensor_pt[method]    + pt) = scount_work;               /* Data store for next         */
               *(g_self_add_reference_pt[method] + pt) = rcount_work;               /* Data store for next         */
            }
            else
            {
                scount_work    = *(g_mutual_add_pri_sensor_pt[method]    + pt);         /* Get Add data            */
                rcount_work    = *(g_mutual_add_pri_reference_pt[method] + pt);         /* Get Add data            */
                scount_work2   = *(g_mutual_add_snd_sensor_pt[method]    + pt);         /* Get Add data            */
                rcount_work2   = *(g_mutual_add_snd_reference_pt[method] + pt);         /* Get Add data            */
                scount_work   -= scount_work / ADD_TIME;                                  /* Average / ADD_TIME      */
                rcount_work   -= rcount_work / ADD_TIME;                                  /* Average / ADD_TIME      */
                scount_work2  -= scount_work2 / ADD_TIME;                                 /* Average / ADD_TIME      */
                rcount_work2  -= rcount_work2 / ADD_TIME;                                 /* Average / ADD_TIME      */
                scount_work   += *(g_mutual_ico_pri_sensor_pt[method]    + (pt + offset)) / ADD_TIME;  /* Add Now data / ADD_TIME     */
                rcount_work   += *(g_mutual_ico_pri_reference_pt[method] + (pt + offset)) / ADD_TIME;  /* Add Now data / ADD_TIME     */
                scount_work2  += *(g_mutual_ico_snd_sensor_pt[method]    + (pt + offset)) / ADD_TIME;  /* Add Now data / ADD_TIME     */
                rcount_work2  += *(g_mutual_ico_snd_reference_pt[method] + (pt + offset)) / ADD_TIME;  /* Add Now data / ADD_TIME     */

               *(g_mutual_add_pri_sensor_pt[method]    + pt) = scount_work;       /* Data store for next         */
               *(g_mutual_add_pri_reference_pt[method] + pt) = rcount_work;       /* Data store for next         */
               *(g_mutual_add_snd_sensor_pt[method]    + pt) = scount_work2;      /* Data store for next         */
               *(g_mutual_add_snd_reference_pt[method] + pt) = rcount_work2;      /* Data store for next         */
            }
        }

        if (0 == g_key_info[method].mode)
        {
             *(g_self_sensor_cnt_pt[method]    + pt) = *(g_self_add_sensor_pt[method]    + pt);
             *(g_self_reference_cnt_pt[method] + pt) = *(g_self_add_reference_pt[method] + pt);
             offset = offset + 1;
        }
        else
        {
             *(g_mutual_ico_pri_sensor_pt[method]    + (pt + offset)) = *(g_mutual_add_pri_sensor_pt[method]    + pt);
             *(g_mutual_ico_pri_reference_pt[method] + (pt + offset)) = *(g_mutual_add_pri_reference_pt[method] + pt);
             *(g_mutual_ico_snd_sensor_pt[method]    + (pt + offset)) = *(g_mutual_add_snd_sensor_pt[method]    + pt);
             *(g_mutual_ico_snd_reference_pt[method] + (pt + offset)) = *(g_mutual_add_snd_reference_pt[method] + pt);

            if (*(g_mutual_ico_snd_sensor_pt[method] + (pt + offset)) > *(g_mutual_ico_pri_sensor_pt[method]    + (pt + offset)))
            {
                 *(g_mutual_sensor_diff_pt[method] + pt) = (uint16_t)(*(g_mutual_ico_snd_sensor_pt[method] + (pt + offset)) - *(g_mutual_ico_pri_sensor_pt[method]    + (pt + offset)));
            }
            else
            {
                 *(g_mutual_sensor_diff_pt[method] + pt) = 0;
            }
            offset = offset + 3;
        }
    }

    if ( 0 == g_touch_function[method].flag.average)
    {
        g_touch_function[method].flag.average = 1;
    }
}    /* End of function touch_data_moving_average() */

/***********************************************************************************************************************
* Function Name: initial_offset_tuning
* Description  : 
* Arguments    : uint8_t method    : Maeasurement method : 0-8
*              : uint8_t number    : Key number
* Return Value : none
***********************************************************************************************************************/
uint8_t initial_offset_tuning( uint8_t method, uint8_t number )
{
             uint8_t  loop;
             uint8_t  pt;
    volatile uint8_t  offset;
    volatile uint8_t  status;

    volatile uint16_t sensor_raw_data[64];       /* ICO sensor counter storage buffer[Maximum number of keys = 64] */
    volatile uint16_t reference_raw_data[64];    /* ICO reference counter storage buffer[Maximum number of keys = 64] */

    status = INIT_TUNING_FINISH;
    pt     = 0;
    offset = 1;

    for (loop = 0; loop < number; loop++ )
    {
        if (0 == *(g_touch_tuning_info[method].result + loop))
        {
            if (0 == g_key_info[method].mode)
            {
                sensor_raw_data[loop]    = *(g_self_ico_sensor_pt[method]    + (loop + pt));
                reference_raw_data[loop] = *(g_self_ico_reference_pt[method] + (loop + pt));
            }
            else
            {
                sensor_raw_data[loop]    = *(g_mutual_ico_pri_sensor_pt[method]    + (loop + pt));
                reference_raw_data[loop] = *(g_mutual_ico_pri_reference_pt[method] + (loop + pt));
            }

           *(g_touch_tuning_info[method].ctsuso  + loop) = *(g_dtc_write_data[method] + offset) & 0x03FF;

            if (reference_raw_data[loop] < (sensor_raw_data[loop] - TUNING_UPPER_LIMIT))
            {
                if (0x03FF != *(g_touch_tuning_info[method].ctsuso  + loop))
                {
                   *(g_dtc_write_data[method] + offset) =  (*(g_dtc_write_data[method] + offset) & 0xFC00) + (*(g_touch_tuning_info[method].ctsuso + loop) + 1);
                   *(g_touch_tuning_info[method].result + loop)  = 0;
                   *(g_current_offset_pm_pt[method] + loop) = *(g_current_offset_pm_pt[method] + loop) + 1;
                }
                else
                {
                   *(g_touch_tuning_info[method].result + loop) = 1;
                }
            }
            else if (reference_raw_data[loop] > (sensor_raw_data[loop] + TUNING_LOWER_LIMIT))
            {
                if (0x0000 != *(g_touch_tuning_info[method].ctsuso  + loop))
                {
                   *(g_dtc_write_data[method] + offset)  = (*(g_dtc_write_data[method] + offset) & 0xFC00) + (*(g_touch_tuning_info[method].ctsuso + loop) - 1);
                   *(g_touch_tuning_info[method].result + loop) = 0;
                   *(g_current_offset_pm_pt[method] + loop) = *(g_current_offset_pm_pt[method] + loop) - 1;
                }
                else
                {
                   *(g_touch_tuning_info[method].result + loop) = 1;
                }
            }
            else
            {
               *(g_touch_tuning_info[method].result + loop) = 1;
            }
        }

        if (10 == g_current_offset_count[method])
        {
            if ((OFFSET_CNT_PLUS >= (*(g_current_offset_pm_pt[method] + loop))) && (OFFSET_CNT_MINUS <= (*(g_current_offset_pm_pt[method] + loop))))
            {
                *(g_touch_tuning_info[method].result + loop) = 1;
            }
            else
            {
                g_current_offset_count[method] = 0;
               *(g_current_offset_pm_pt[method] + loop) = 0;
            }
        }

        if (0 == g_key_info[method].mode)
        {
            pt     = pt + 1;
        }
        else
        {
            pt     = pt + 3;
        }
        offset = offset + 3;
    }
    g_ctsu_status[method].flag.data_update = 0;

    if (1 != g_touch_function[method].flag.tuning)
    {
        g_current_offset_count[method] = g_current_offset_count[method] + 1;
    }

    for (loop = 0; loop < number; loop++)
    {
        if (1 != *(g_touch_tuning_info[method].result + loop))
        {
            status = INIT_TUNING_RUN;
            return status;
        }
    }

    g_touch_function[method].flag.tuning = 1;

    for (loop = 0; loop < METHOD_NUM; loop++)
    {
        if (1 != g_touch_function[loop].flag.tuning)
        {
            status = INIT_TUNING_RUN;
            return status;
        }
    }

    return status;
}    /* End of function initial_offset_tuning() */

/***********************************************************************************************************************
* Function Name: touch_internal_tuning
* Description  : 
* Arguments    : uint8_t mode            0 : Internal tuning stop
*              :                         1 : Internal tuning start
*              : uint16_t count          0 : Set Error
*              :                   1-65535 : Internal tuning timing
* Return Value : none
***********************************************************************************************************************/
void touch_internal_tuning( uint8_t method, uint16_t count )
{

    switch (g_ctsu_offset_mode)
    {
        case TUNING_START:
            if (0 != count)
            {
                if (0 == g_tuning_counter[method])
                {
                    g_tuning_counter[method] = (uint16_t)(count * 50);
                    touch_offset_tuning( method );
                }
                else
                {
                    g_tuning_counter[method] = (uint16_t)(g_tuning_counter[method] - 1);
                }
            }
            break;
        case TUNING_STOP:
            break;
        default:
            break;
    }
}    /* End of function touch_Internal_tuning() */

/***********************************************************************************************************************
* Function Name: touch_offset_tuning
* Description  : 
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void touch_offset_tuning( uint8_t method )
{
             uint8_t  key_num;
             uint8_t  pt;
    volatile uint8_t  offset;
    volatile uint16_t so_storage;

    offset = 1;
    pt = 0;

    for (key_num = 0; key_num < g_key_info[method].ena_num; key_num++)
    {
        so_storage = (*(g_dtc_write_data[method] + offset) & 0x03FF);

        if (0 == g_key_info[method].mode)
        {
            if ((*(g_self_ico_reference_pt[method] + key_num + pt)) < ((*(g_self_ico_sensor_pt[method] + key_num + pt)) - OFFSET_UPPER_LIMIT))
            {
                if (0x03FF != so_storage)
                {
                    *(g_dtc_write_data[method] + offset) = (*(g_dtc_write_data[method] + offset) & 0xFC00) + (so_storage + 1);
                }
            }
            else if ((*(g_self_ico_reference_pt[method] + key_num + pt)) > ((*(g_self_ico_sensor_pt[method] + key_num + pt)) + OFFSET_LOWER_LIMIT))
            {
                if (0x0000 != so_storage)
                {
                    *(g_dtc_write_data[method] + offset) = (*(g_dtc_write_data[method] + offset) & 0xFC00) + (so_storage - 1);
                }
            }
            offset = offset + 3;
            pt = pt + 1;
        }
        else
        {
            if ((*(g_mutual_ico_pri_reference_pt[method] + key_num + pt)) < ((*(g_mutual_ico_pri_sensor_pt[method] + key_num + pt)) - OFFSET_UPPER_LIMIT))
            {
                *(g_dtc_write_data[method] + offset) = (*(g_dtc_write_data[method] + offset) & 0xFC00) + (so_storage + 1);
            }
            else if ((*(g_mutual_ico_pri_reference_pt[method] + key_num + pt)) > ((*(g_mutual_ico_pri_sensor_pt[method] + key_num + pt)) + OFFSET_LOWER_LIMIT))
            {
                if (0x0000 != so_storage)
                {
                    *(g_dtc_write_data[method] + offset) = (*(g_dtc_write_data[method] + offset) & 0xFC00) + (so_storage - 1);
                }
            }
            offset = offset + 3;
            pt = pt + 3;
        }
    }
}    /* End of function touch_offset_tuning() */

/***********************************************************************************************************************
* Function Name: Excep_CTSU_CTSUFN
* Description  : Measurement finish interruption process.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma message("Important change for EWRX. vector nuber and __interrupt must be added")
#pragma vector = 62
__interrupt void Excep_CTSU_CTSUFN( void )
{
    CTSUInterrupt();
}    /* End of function Excep_CTSU_CTSUFN() */

/***********************************************************************************************************************
* Function Name: CTSUInterrupt
* Description  : CTSU measurement end interrupt function
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void CTSUInterrupt( void )
{
    uint8_t err_status;

    if (0 != CTSUGetTscapVoltageError())
    {
        g_ctsu_status[g_method_stor].flag.icomp_error = 1;                        /* TSCAP voltage error              */
        R_Set_CTSU_Power_Supply_Only(0x00);                                       /* TSCUPON off -> TSCUICOMP clear   */
        nop();
        nop();
        R_Set_CTSU_Power_Supply_Only(0x01);                                       /* TSCUPON on                       */
    }
    else
    {
         g_ctsu_status[g_method_stor].flag.icomp_error = 0;            /* TSCAP voltage error clear        */
    }

    err_status = R_Get_CTSU_Counter_Overflow_flag();

    switch (err_status)
    {
        case SENS_OK:
            g_ctsu_status[g_method_stor].flag.sens_over = 0;
            g_ctsu_status[g_method_stor].flag.ref_over = 0;
            if (g_ctsu_status[g_method_stor].flag.icomp_error != 1)         /* TSCAP voltage error                    */
            {
                g_ctsu_status[g_method_stor].flag.data_update = 1;          /* Measurement data updata status set     */
            }
            g_ctsu_soft_mode = CTSU_FINISH_MODE;
            g_ctsu_status[g_method_stor].flag.ctsu_measure = 1;
            break;
        case SENS_OVER:
            g_ctsu_status[g_method_stor].flag.ref_over = 0;
            g_ctsu_status[g_method_stor].flag.sens_over = 1;                /* Sensor Counter overflow status set     */
            g_ctsu_soft_mode = CTSU_FINISH_MODE;
            break;
        case REF_OVER:
            g_ctsu_status[g_method_stor].flag.sens_over = 0;
            g_ctsu_status[g_method_stor].flag.ref_over = 1;                 /* Reference Counter overflow status set  */
            g_ctsu_soft_mode = CTSU_FINISH_MODE;
            break;
        case SENS_REF_OVER:
            g_ctsu_status[g_method_stor].flag.sens_over = 1;                /* Sensor Counter overflow status set     */
            g_ctsu_status[g_method_stor].flag.ref_over = 1;                 /* Reference Counter overflow status set  */
            g_ctsu_soft_mode = CTSU_FINISH_MODE;
            break;
        default:
            g_ctsu_soft_mode = CTSU_STOP_MODE;
            break;
    }

#if (MCU_SERIES_T == R5F5113 || MCU_SERIES_T == R5F5231)
    #ifdef WORKBENCH_COMMAND_USE
        if( g_ctsu_soft_mode == CTSU_FINISH_MODE )
        {
            BurstMonitorSendSensorValue();
        }
    #endif    // WORKBENCH_COMMAND_USE
#endif    // (MCU_SERIES_T == R5F5113 || MCU_SERIES_T == R5F5231)

//      R_SCI6_Serial_Send();

}    /* End of function CTSUInterrupt() */

/*===== DTC transmit data ========================================================*/
#if ( SELF_METHOD_NUM == 1 )
const uint16_t g_self_ctsussc[] =
{ 
    #if SELF_ENABLE_TS00 == 1
        CTSUSSC_TS00,
    #endif
    #if SELF_ENABLE_TS01 == 1
        CTSUSSC_TS01,
    #endif
    #if SELF_ENABLE_TS02 == 1
        CTSUSSC_TS02,
    #endif
    #if SELF_ENABLE_TS03 == 1
        CTSUSSC_TS03,
    #endif
    #if SELF_ENABLE_TS04 == 1
        CTSUSSC_TS04,
    #endif
    #if SELF_ENABLE_TS05 == 1
        CTSUSSC_TS05,
    #endif
    #if SELF_ENABLE_TS06 == 1
        CTSUSSC_TS06,
    #endif
    #if SELF_ENABLE_TS07 == 1
        CTSUSSC_TS07,
    #endif
    #if SELF_ENABLE_TS08 == 1
        CTSUSSC_TS08,
    #endif
    #if SELF_ENABLE_TS09 == 1
        CTSUSSC_TS09,
    #endif
    #if SELF_ENABLE_TS10 == 1
        CTSUSSC_TS10,
    #endif
    #if SELF_ENABLE_TS11 == 1
        CTSUSSC_TS11,
    #endif
    #if SELF_ENABLE_TS12 == 1
        CTSUSSC_TS12,
    #endif
    #if SELF_ENABLE_TS13 == 1
        CTSUSSC_TS13,
    #endif
    #if SELF_ENABLE_TS14 == 1
        CTSUSSC_TS14,
    #endif
    #if SELF_ENABLE_TS15 == 1
        CTSUSSC_TS15,
    #endif
    #if SELF_ENABLE_TS16 == 1
        CTSUSSC_TS16,
    #endif
    #if SELF_ENABLE_TS17 == 1
        CTSUSSC_TS17,
    #endif
    #if SELF_ENABLE_TS18 == 1
        CTSUSSC_TS18,
    #endif
    #if SELF_ENABLE_TS19 == 1
        CTSUSSC_TS19,
    #endif
    #if SELF_ENABLE_TS20 == 1
        CTSUSSC_TS20,
    #endif
    #if SELF_ENABLE_TS21 == 1
        CTSUSSC_TS21,
    #endif
    #if SELF_ENABLE_TS22 == 1
        CTSUSSC_TS22,
    #endif
    #if SELF_ENABLE_TS23 == 1
        CTSUSSC_TS23,
    #endif
    #if SELF_ENABLE_TS24 == 1
        CTSUSSC_TS24,
    #endif
    #if SELF_ENABLE_TS25 == 1
        CTSUSSC_TS25,
    #endif
    #if SELF_ENABLE_TS26 == 1
        CTSUSSC_TS26,
    #endif
    #if SELF_ENABLE_TS27 == 1
        CTSUSSC_TS27,
    #endif
    #if SELF_ENABLE_TS28 == 1
        CTSUSSC_TS28,
    #endif
    #if SELF_ENABLE_TS29 == 1
        CTSUSSC_TS29,
    #endif
    #if SELF_ENABLE_TS30 == 1
        CTSUSSC_TS30,
    #endif
    #if SELF_ENABLE_TS31 == 1
        CTSUSSC_TS31,
    #endif
    #if SELF_ENABLE_TS32 == 1
        CTSUSSC_TS32,
    #endif
    #if SELF_ENABLE_TS33 == 1
        CTSUSSC_TS33,
    #endif
    #if SELF_ENABLE_TS34 == 1
        CTSUSSC_TS34,
    #endif
    #if SELF_ENABLE_TS35 == 1
        CTSUSSC_TS35,
    #endif
};

const uint16_t g_self_ctsuso0[] =
{ 
    #if SELF_ENABLE_TS00 == 1
        CTSUSO0_TS00,
    #endif
    #if SELF_ENABLE_TS01 == 1
        CTSUSO0_TS01,
    #endif
    #if SELF_ENABLE_TS02 == 1
        CTSUSO0_TS02,
    #endif
    #if SELF_ENABLE_TS03 == 1
        CTSUSO0_TS03,
    #endif
    #if SELF_ENABLE_TS04 == 1
        CTSUSO0_TS04,
    #endif
    #if SELF_ENABLE_TS05 == 1
        CTSUSO0_TS05,
    #endif
    #if SELF_ENABLE_TS06 == 1
        CTSUSO0_TS06,
    #endif
    #if SELF_ENABLE_TS07 == 1
        CTSUSO0_TS07,
    #endif
    #if SELF_ENABLE_TS08 == 1
        CTSUSO0_TS08,
    #endif
    #if SELF_ENABLE_TS09 == 1
        CTSUSO0_TS09,
    #endif
    #if SELF_ENABLE_TS10 == 1
        CTSUSO0_TS10,
    #endif
    #if SELF_ENABLE_TS11 == 1
        CTSUSO0_TS11,
    #endif
    #if SELF_ENABLE_TS12 == 1
        CTSUSO0_TS12,
    #endif
    #if SELF_ENABLE_TS13 == 1
        CTSUSO0_TS13,
    #endif
    #if SELF_ENABLE_TS14 == 1
        CTSUSO0_TS14,
    #endif
    #if SELF_ENABLE_TS15 == 1
        CTSUSO0_TS15,
    #endif
    #if SELF_ENABLE_TS16 == 1
        CTSUSO0_TS16,
    #endif
    #if SELF_ENABLE_TS17 == 1
        CTSUSO0_TS17,
    #endif
    #if SELF_ENABLE_TS18 == 1
        CTSUSO0_TS18,
    #endif
    #if SELF_ENABLE_TS19 == 1
        CTSUSO0_TS19,
    #endif
    #if SELF_ENABLE_TS20 == 1
        CTSUSO0_TS20,
    #endif
    #if SELF_ENABLE_TS21 == 1
        CTSUSO0_TS21,
    #endif
    #if SELF_ENABLE_TS22 == 1
        CTSUSO0_TS22,
    #endif
    #if SELF_ENABLE_TS23 == 1
        CTSUSO0_TS23,
    #endif
    #if SELF_ENABLE_TS24 == 1
        CTSUSO0_TS24,
    #endif
    #if SELF_ENABLE_TS25 == 1
        CTSUSO0_TS25,
    #endif
    #if SELF_ENABLE_TS26 == 1
        CTSUSO0_TS26,
    #endif
    #if SELF_ENABLE_TS27 == 1
        CTSUSO0_TS27,
    #endif
    #if SELF_ENABLE_TS28 == 1
        CTSUSO0_TS28,
    #endif
    #if SELF_ENABLE_TS29 == 1
        CTSUSO0_TS29,
    #endif
    #if SELF_ENABLE_TS30 == 1
        CTSUSO0_TS30,
    #endif
    #if SELF_ENABLE_TS31 == 1
        CTSUSO0_TS31,
    #endif
    #if SELF_ENABLE_TS32 == 1
        CTSUSO0_TS32,
    #endif
    #if SELF_ENABLE_TS33 == 1
        CTSUSO0_TS33,
    #endif
    #if SELF_ENABLE_TS34 == 1
        CTSUSO0_TS34,
    #endif
    #if SELF_ENABLE_TS35 == 1
        CTSUSO0_TS35,
    #endif
};

const uint16_t g_self_ctsuso1[] =
{ 
    #if SELF_ENABLE_TS00 == 1
        CTSUSO1_TS00,
    #endif
    #if SELF_ENABLE_TS01 == 1
        CTSUSO1_TS01,
    #endif
    #if SELF_ENABLE_TS02 == 1
        CTSUSO1_TS02,
    #endif
    #if SELF_ENABLE_TS03 == 1
        CTSUSO1_TS03,
    #endif
    #if SELF_ENABLE_TS04 == 1
        CTSUSO1_TS04,
    #endif
    #if SELF_ENABLE_TS05 == 1
        CTSUSO1_TS05,
    #endif
    #if SELF_ENABLE_TS06 == 1
        CTSUSO1_TS06,
    #endif
    #if SELF_ENABLE_TS07 == 1
        CTSUSO1_TS07,
    #endif
    #if SELF_ENABLE_TS08 == 1
        CTSUSO1_TS08,
    #endif
    #if SELF_ENABLE_TS09 == 1
        CTSUSO1_TS09,
    #endif
    #if SELF_ENABLE_TS10 == 1
        CTSUSO1_TS10,
    #endif
    #if SELF_ENABLE_TS11 == 1
        CTSUSO1_TS11,
    #endif
    #if SELF_ENABLE_TS12 == 1
        CTSUSO1_TS12,
    #endif
    #if SELF_ENABLE_TS13 == 1
        CTSUSO1_TS13,
    #endif
    #if SELF_ENABLE_TS14 == 1
        CTSUSO1_TS14,
    #endif
    #if SELF_ENABLE_TS15 == 1
        CTSUSO1_TS15,
    #endif
    #if SELF_ENABLE_TS16 == 1
        CTSUSO1_TS16,
    #endif
    #if SELF_ENABLE_TS17 == 1
        CTSUSO1_TS17,
    #endif
    #if SELF_ENABLE_TS18 == 1
        CTSUSO1_TS18,
    #endif
    #if SELF_ENABLE_TS19 == 1
        CTSUSO1_TS19,
    #endif
    #if SELF_ENABLE_TS20 == 1
        CTSUSO1_TS20,
    #endif
    #if SELF_ENABLE_TS21 == 1
        CTSUSO1_TS21,
    #endif
    #if SELF_ENABLE_TS22 == 1
        CTSUSO1_TS22,
    #endif
    #if SELF_ENABLE_TS23 == 1
        CTSUSO1_TS23,
    #endif
    #if SELF_ENABLE_TS24 == 1
        CTSUSO1_TS24,
    #endif
    #if SELF_ENABLE_TS25 == 1
        CTSUSO1_TS25,
    #endif
    #if SELF_ENABLE_TS26 == 1
        CTSUSO1_TS26,
    #endif
    #if SELF_ENABLE_TS27 == 1
        CTSUSO1_TS27,
    #endif
    #if SELF_ENABLE_TS28 == 1
        CTSUSO1_TS28,
    #endif
    #if SELF_ENABLE_TS29 == 1
        CTSUSO1_TS29,
    #endif
    #if SELF_ENABLE_TS30 == 1
        CTSUSO1_TS30,
    #endif
    #if SELF_ENABLE_TS31 == 1
        CTSUSO1_TS31,
    #endif
    #if SELF_ENABLE_TS32 == 1
        CTSUSO1_TS32,
    #endif
    #if SELF_ENABLE_TS33 == 1
        CTSUSO1_TS33,
    #endif
    #if SELF_ENABLE_TS34 == 1
        CTSUSO1_TS34,
    #endif
    #if SELF_ENABLE_TS35 == 1
        CTSUSO1_TS35,
    #endif
};
#endif    // ( SELF_METHOD_NUM == 1 )

/*=====    Mutual 0    DTC transmit data ========================================================*/
#if ( MUTUAL_METHOD_NUM > 0 )
const uint16_t g_mutual0_ctsussc[] =
{
    #if ( MUTUAL0_NUM >= 1 )
        CTSUSSC_MUTUAL0_KEY00,
    #endif
    #if ( MUTUAL0_NUM >= 2 )
        CTSUSSC_MUTUAL0_KEY01,
    #endif
    #if ( MUTUAL0_NUM >= 3 )
        CTSUSSC_MUTUAL0_KEY02,
    #endif
    #if ( MUTUAL0_NUM >= 4 )
        CTSUSSC_MUTUAL0_KEY03,
    #endif
    #if ( MUTUAL0_NUM >= 5 )
        CTSUSSC_MUTUAL0_KEY04,
    #endif
    #if ( MUTUAL0_NUM >= 6 )
        CTSUSSC_MUTUAL0_KEY05,
    #endif
    #if ( MUTUAL0_NUM >= 7 )
        CTSUSSC_MUTUAL0_KEY06,
    #endif
    #if ( MUTUAL0_NUM >= 8 )
        CTSUSSC_MUTUAL0_KEY07,
    #endif
    #if ( MUTUAL0_NUM >= 9 )
        CTSUSSC_MUTUAL0_KEY08,
    #endif
    #if ( MUTUAL0_NUM >= 10 )
        CTSUSSC_MUTUAL0_KEY09,
    #endif
    #if ( MUTUAL0_NUM >= 11 )
        CTSUSSC_MUTUAL0_KEY10,
    #endif
    #if ( MUTUAL0_NUM >= 12 )
        CTSUSSC_MUTUAL0_KEY11,
    #endif
    #if ( MUTUAL0_NUM >= 13 )
        CTSUSSC_MUTUAL0_KEY12,
    #endif
    #if ( MUTUAL0_NUM >= 14 )
        CTSUSSC_MUTUAL0_KEY13,
    #endif
    #if ( MUTUAL0_NUM >= 15 )
        CTSUSSC_MUTUAL0_KEY14,
    #endif
    #if ( MUTUAL0_NUM >= 16 )
        CTSUSSC_MUTUAL0_KEY15,
    #endif
    #if ( MUTUAL0_NUM >= 17 )
        CTSUSSC_MUTUAL0_KEY16,
    #endif
    #if ( MUTUAL0_NUM >= 18 )
        CTSUSSC_MUTUAL0_KEY17,
    #endif
    #if ( MUTUAL0_NUM >= 19 )
        CTSUSSC_MUTUAL0_KEY18,
    #endif
    #if ( MUTUAL0_NUM >= 20 )
        CTSUSSC_MUTUAL0_KEY19,
    #endif
    #if ( MUTUAL0_NUM >= 21 )
        CTSUSSC_MUTUAL0_KEY20,
    #endif
    #if ( MUTUAL0_NUM >= 22 )
        CTSUSSC_MUTUAL0_KEY21,
    #endif
    #if ( MUTUAL0_NUM >= 23 )
        CTSUSSC_MUTUAL0_KEY22,
    #endif
    #if ( MUTUAL0_NUM >= 24 )
        CTSUSSC_MUTUAL0_KEY23,
    #endif
    #if ( MUTUAL0_NUM >= 25 )
        CTSUSSC_MUTUAL0_KEY24,
    #endif
    #if ( MUTUAL0_NUM >= 26 )
        CTSUSSC_MUTUAL0_KEY25,
    #endif
    #if ( MUTUAL0_NUM >= 27 )
        CTSUSSC_MUTUAL0_KEY26,
    #endif
    #if ( MUTUAL0_NUM >= 28 )
        CTSUSSC_MUTUAL0_KEY27,
    #endif
    #if ( MUTUAL0_NUM >= 29 )
        CTSUSSC_MUTUAL0_KEY28,
    #endif
    #if ( MUTUAL0_NUM >= 30 )
        CTSUSSC_MUTUAL0_KEY29,
    #endif
    #if ( MUTUAL0_NUM >= 31 )
        CTSUSSC_MUTUAL0_KEY30,
    #endif
    #if ( MUTUAL0_NUM >= 32 )
        CTSUSSC_MUTUAL0_KEY31,
    #endif
    #if ( MUTUAL0_NUM >= 33 )
        CTSUSSC_MUTUAL0_KEY32,
    #endif
    #if ( MUTUAL0_NUM >= 34 )
        CTSUSSC_MUTUAL0_KEY33,
    #endif
    #if ( MUTUAL0_NUM >= 35 )
        CTSUSSC_MUTUAL0_KEY34,
    #endif
    #if ( MUTUAL0_NUM >= 36 )
        CTSUSSC_MUTUAL0_KEY35,
    #endif
    #if ( MUTUAL0_NUM >= 37 )
        CTSUSSC_MUTUAL0_KEY36,
    #endif
    #if ( MUTUAL0_NUM >= 38 )
        CTSUSSC_MUTUAL0_KEY37,
    #endif
    #if ( MUTUAL0_NUM >= 39 )
        CTSUSSC_MUTUAL0_KEY38,
    #endif
    #if ( MUTUAL0_NUM >= 40 )
        CTSUSSC_MUTUAL0_KEY39,
    #endif
    #if ( MUTUAL0_NUM >= 41 )
        CTSUSSC_MUTUAL0_KEY40,
    #endif
    #if ( MUTUAL0_NUM >= 42 )
        CTSUSSC_MUTUAL0_KEY41,
    #endif
    #if ( MUTUAL0_NUM >= 43 )
        CTSUSSC_MUTUAL0_KEY42,
    #endif
    #if ( MUTUAL0_NUM >= 44 )
        CTSUSSC_MUTUAL0_KEY43,
    #endif
    #if ( MUTUAL0_NUM >= 45 )
        CTSUSSC_MUTUAL0_KEY44,
    #endif
    #if ( MUTUAL0_NUM >= 46 )
        CTSUSSC_MUTUAL0_KEY45,
    #endif
    #if ( MUTUAL0_NUM >= 47 )
        CTSUSSC_MUTUAL0_KEY46,
    #endif
    #if ( MUTUAL0_NUM >= 48 )
        CTSUSSC_MUTUAL0_KEY47,
    #endif
    #if ( MUTUAL0_NUM >= 49 )
        CTSUSSC_MUTUAL0_KEY48,
    #endif
    #if ( MUTUAL0_NUM >= 50 )
        CTSUSSC_MUTUAL0_KEY49,
    #endif
    #if ( MUTUAL0_NUM >= 51 )
        CTSUSSC_MUTUAL0_KEY50,
    #endif
    #if ( MUTUAL0_NUM >= 52 )
        CTSUSSC_MUTUAL0_KEY51,
    #endif
    #if ( MUTUAL0_NUM >= 53 )
        CTSUSSC_MUTUAL0_KEY52,
    #endif
    #if ( MUTUAL0_NUM >= 54 )
        CTSUSSC_MUTUAL0_KEY53,
    #endif
    #if ( MUTUAL0_NUM >= 55 )
        CTSUSSC_MUTUAL0_KEY54,
    #endif
    #if ( MUTUAL0_NUM >= 56 )
        CTSUSSC_MUTUAL0_KEY55,
    #endif
    #if ( MUTUAL0_NUM >= 57 )
        CTSUSSC_MUTUAL0_KEY56,
    #endif
    #if ( MUTUAL0_NUM >= 58 )
        CTSUSSC_MUTUAL0_KEY57,
    #endif
    #if ( MUTUAL0_NUM >= 59 )
        CTSUSSC_MUTUAL0_KEY58,
    #endif
    #if ( MUTUAL0_NUM >= 60 )
        CTSUSSC_MUTUAL0_KEY59,
    #endif
    #if ( MUTUAL0_NUM >= 61 )
        CTSUSSC_MUTUAL0_KEY60,
    #endif
    #if ( MUTUAL0_NUM >= 62 )
        CTSUSSC_MUTUAL0_KEY61,
    #endif
    #if ( MUTUAL0_NUM >= 63 )
        CTSUSSC_MUTUAL0_KEY62,
    #endif
    #if ( MUTUAL0_NUM >= 64 )
        CTSUSSC_MUTUAL0_KEY63
    #endif
};

const uint16_t g_mutual0_ctsuso0[] =
{
    #if ( MUTUAL0_NUM >= 1 )
        CTSUSO0_MUTUAL0_KEY00,
    #endif
    #if ( MUTUAL0_NUM >= 2 )
        CTSUSO0_MUTUAL0_KEY01,
    #endif
    #if ( MUTUAL0_NUM >= 3 )
        CTSUSO0_MUTUAL0_KEY02,
    #endif
    #if ( MUTUAL0_NUM >= 4 )
        CTSUSO0_MUTUAL0_KEY03,
    #endif
    #if ( MUTUAL0_NUM >= 5 )
        CTSUSO0_MUTUAL0_KEY04,
    #endif
    #if ( MUTUAL0_NUM >= 6 )
        CTSUSO0_MUTUAL0_KEY05,
    #endif
    #if ( MUTUAL0_NUM >= 7 )
        CTSUSO0_MUTUAL0_KEY06,
    #endif
    #if ( MUTUAL0_NUM >= 8 )
        CTSUSO0_MUTUAL0_KEY07,
    #endif
    #if ( MUTUAL0_NUM >= 9 )
        CTSUSO0_MUTUAL0_KEY08,
    #endif
    #if ( MUTUAL0_NUM >= 10 )
        CTSUSO0_MUTUAL0_KEY09,
    #endif
    #if ( MUTUAL0_NUM >= 11 )
        CTSUSO0_MUTUAL0_KEY10,
    #endif
    #if ( MUTUAL0_NUM >= 12 )
        CTSUSO0_MUTUAL0_KEY11,
    #endif
    #if ( MUTUAL0_NUM >= 13 )
        CTSUSO0_MUTUAL0_KEY12,
    #endif
    #if ( MUTUAL0_NUM >= 14 )
        CTSUSO0_MUTUAL0_KEY13,
    #endif
    #if ( MUTUAL0_NUM >= 15 )
        CTSUSO0_MUTUAL0_KEY14,
    #endif
    #if ( MUTUAL0_NUM >= 16 )
        CTSUSO0_MUTUAL0_KEY15,
    #endif
    #if ( MUTUAL0_NUM >= 17 )
        CTSUSO0_MUTUAL0_KEY16,
    #endif
    #if ( MUTUAL0_NUM >= 18 )
        CTSUSO0_MUTUAL0_KEY17,
    #endif
    #if ( MUTUAL0_NUM >= 19 )
        CTSUSO0_MUTUAL0_KEY18,
    #endif
    #if ( MUTUAL0_NUM >= 20 )
        CTSUSO0_MUTUAL0_KEY19,
    #endif
    #if ( MUTUAL0_NUM >= 21 )
        CTSUSO0_MUTUAL0_KEY20,
    #endif
    #if ( MUTUAL0_NUM >= 22 )
        CTSUSO0_MUTUAL0_KEY21,
    #endif
    #if ( MUTUAL0_NUM >= 23 )
        CTSUSO0_MUTUAL0_KEY22,
    #endif
    #if ( MUTUAL0_NUM >= 24 )
        CTSUSO0_MUTUAL0_KEY23,
    #endif
    #if ( MUTUAL0_NUM >= 25 )
        CTSUSO0_MUTUAL0_KEY24,
    #endif
    #if ( MUTUAL0_NUM >= 26 )
        CTSUSO0_MUTUAL0_KEY25,
    #endif
    #if ( MUTUAL0_NUM >= 27 )
        CTSUSO0_MUTUAL0_KEY26,
    #endif
    #if ( MUTUAL0_NUM >= 28 )
        CTSUSO0_MUTUAL0_KEY27,
    #endif
    #if ( MUTUAL0_NUM >= 29 )
        CTSUSO0_MUTUAL0_KEY28,
    #endif
    #if ( MUTUAL0_NUM >= 30 )
        CTSUSO0_MUTUAL0_KEY29,
    #endif
    #if ( MUTUAL0_NUM >= 31 )
        CTSUSO0_MUTUAL0_KEY30,
    #endif
    #if ( MUTUAL0_NUM >= 32 )
        CTSUSO0_MUTUAL0_KEY31,
    #endif
    #if ( MUTUAL0_NUM >= 33 )
        CTSUSO0_MUTUAL0_KEY32,
    #endif
    #if ( MUTUAL0_NUM >= 34 )
        CTSUSO0_MUTUAL0_KEY33,
    #endif
    #if ( MUTUAL0_NUM >= 35 )
        CTSUSO0_MUTUAL0_KEY34,
    #endif
    #if ( MUTUAL0_NUM >= 36 )
        CTSUSO0_MUTUAL0_KEY35,
    #endif
    #if ( MUTUAL0_NUM >= 37 )
        CTSUSO0_MUTUAL0_KEY36,
    #endif
    #if ( MUTUAL0_NUM >= 38 )
        CTSUSO0_MUTUAL0_KEY37,
    #endif
    #if ( MUTUAL0_NUM >= 39 )
        CTSUSO0_MUTUAL0_KEY38,
    #endif
    #if ( MUTUAL0_NUM >= 40 )
        CTSUSO0_MUTUAL0_KEY39,
    #endif
    #if ( MUTUAL0_NUM >= 41 )
        CTSUSO0_MUTUAL0_KEY40,
    #endif
    #if ( MUTUAL0_NUM >= 42 )
        CTSUSO0_MUTUAL0_KEY41,
    #endif
    #if ( MUTUAL0_NUM >= 43 )
        CTSUSO0_MUTUAL0_KEY42,
    #endif
    #if ( MUTUAL0_NUM >= 44 )
        CTSUSO0_MUTUAL0_KEY43,
    #endif
    #if ( MUTUAL0_NUM >= 45 )
        CTSUSO0_MUTUAL0_KEY44,
    #endif
    #if ( MUTUAL0_NUM >= 46 )
        CTSUSO0_MUTUAL0_KEY45,
    #endif
    #if ( MUTUAL0_NUM >= 47 )
        CTSUSO0_MUTUAL0_KEY46,
    #endif
    #if ( MUTUAL0_NUM >= 48 )
        CTSUSO0_MUTUAL0_KEY47,
    #endif
    #if ( MUTUAL0_NUM >= 49 )
        CTSUSO0_MUTUAL0_KEY48,
    #endif
    #if ( MUTUAL0_NUM >= 50 )
        CTSUSO0_MUTUAL0_KEY49,
    #endif
    #if ( MUTUAL0_NUM >= 51 )
        CTSUSO0_MUTUAL0_KEY50,
    #endif
    #if ( MUTUAL0_NUM >= 52 )
        CTSUSO0_MUTUAL0_KEY51,
    #endif
    #if ( MUTUAL0_NUM >= 53 )
        CTSUSO0_MUTUAL0_KEY52,
    #endif
    #if ( MUTUAL0_NUM >= 54 )
        CTSUSO0_MUTUAL0_KEY53,
    #endif
    #if ( MUTUAL0_NUM >= 55 )
        CTSUSO0_MUTUAL0_KEY54,
    #endif
    #if ( MUTUAL0_NUM >= 56 )
        CTSUSO0_MUTUAL0_KEY55,
    #endif
    #if ( MUTUAL0_NUM >= 57 )
        CTSUSO0_MUTUAL0_KEY56,
    #endif
    #if ( MUTUAL0_NUM >= 58 )
        CTSUSO0_MUTUAL0_KEY57,
    #endif
    #if ( MUTUAL0_NUM >= 59 )
        CTSUSO0_MUTUAL0_KEY58,
    #endif
    #if ( MUTUAL0_NUM >= 60 )
        CTSUSO0_MUTUAL0_KEY59,
    #endif
    #if ( MUTUAL0_NUM >= 61 )
        CTSUSO0_MUTUAL0_KEY60,
    #endif
    #if ( MUTUAL0_NUM >= 62 )
        CTSUSO0_MUTUAL0_KEY61,
    #endif
    #if ( MUTUAL0_NUM >= 63 )
        CTSUSO0_MUTUAL0_KEY62,
    #endif
    #if ( MUTUAL0_NUM >= 64 )
        CTSUSO0_MUTUAL0_KEY63
    #endif
};

const uint16_t g_mutual0_ctsuso1[] =
{
    #if ( MUTUAL0_NUM >= 1 )
        CTSUSO1_MUTUAL0_KEY00,
    #endif
    #if ( MUTUAL0_NUM >= 2 )
        CTSUSO1_MUTUAL0_KEY01,
    #endif
    #if ( MUTUAL0_NUM >= 3 )
        CTSUSO1_MUTUAL0_KEY02,
    #endif
    #if ( MUTUAL0_NUM >= 4 )
        CTSUSO1_MUTUAL0_KEY03,
    #endif
    #if ( MUTUAL0_NUM >= 5 )
        CTSUSO1_MUTUAL0_KEY04,
    #endif
    #if ( MUTUAL0_NUM >= 6 )
        CTSUSO1_MUTUAL0_KEY05,
    #endif
    #if ( MUTUAL0_NUM >= 7 )
        CTSUSO1_MUTUAL0_KEY06,
    #endif
    #if ( MUTUAL0_NUM >= 8 )
        CTSUSO1_MUTUAL0_KEY07,
    #endif
    #if ( MUTUAL0_NUM >= 9 )
        CTSUSO1_MUTUAL0_KEY08,
    #endif
    #if ( MUTUAL0_NUM >= 10 )
        CTSUSO1_MUTUAL0_KEY09,
    #endif
    #if ( MUTUAL0_NUM >= 11 )
        CTSUSO1_MUTUAL0_KEY10,
    #endif
    #if ( MUTUAL0_NUM >= 12 )
        CTSUSO1_MUTUAL0_KEY11,
    #endif
    #if ( MUTUAL0_NUM >= 13 )
        CTSUSO1_MUTUAL0_KEY12,
    #endif
    #if ( MUTUAL0_NUM >= 14 )
        CTSUSO1_MUTUAL0_KEY13,
    #endif
    #if ( MUTUAL0_NUM >= 15 )
        CTSUSO1_MUTUAL0_KEY14,
    #endif
    #if ( MUTUAL0_NUM >= 16 )
        CTSUSO1_MUTUAL0_KEY15,
    #endif
    #if ( MUTUAL0_NUM >= 17 )
        CTSUSO1_MUTUAL0_KEY16,
    #endif
    #if ( MUTUAL0_NUM >= 18 )
        CTSUSO1_MUTUAL0_KEY17,
    #endif
    #if ( MUTUAL0_NUM >= 19 )
        CTSUSO1_MUTUAL0_KEY18,
    #endif
    #if ( MUTUAL0_NUM >= 20 )
        CTSUSO1_MUTUAL0_KEY19,
    #endif
    #if ( MUTUAL0_NUM >= 21 )
        CTSUSO1_MUTUAL0_KEY20,
    #endif
    #if ( MUTUAL0_NUM >= 22 )
        CTSUSO1_MUTUAL0_KEY21,
    #endif
    #if ( MUTUAL0_NUM >= 23 )
        CTSUSO1_MUTUAL0_KEY22,
    #endif
    #if ( MUTUAL0_NUM >= 24 )
        CTSUSO1_MUTUAL0_KEY23,
    #endif
    #if ( MUTUAL0_NUM >= 25 )
        CTSUSO1_MUTUAL0_KEY24,
    #endif
    #if ( MUTUAL0_NUM >= 26 )
        CTSUSO1_MUTUAL0_KEY25,
    #endif
    #if ( MUTUAL0_NUM >= 27 )
        CTSUSO1_MUTUAL0_KEY26,
    #endif
    #if ( MUTUAL0_NUM >= 28 )
        CTSUSO1_MUTUAL0_KEY27,
    #endif
    #if ( MUTUAL0_NUM >= 29 )
        CTSUSO1_MUTUAL0_KEY28,
    #endif
    #if ( MUTUAL0_NUM >= 30 )
        CTSUSO1_MUTUAL0_KEY29,
    #endif
    #if ( MUTUAL0_NUM >= 31 )
        CTSUSO1_MUTUAL0_KEY30,
    #endif
    #if ( MUTUAL0_NUM >= 32 )
        CTSUSO1_MUTUAL0_KEY31,
    #endif
    #if ( MUTUAL0_NUM >= 33 )
        CTSUSO1_MUTUAL0_KEY32,
    #endif
    #if ( MUTUAL0_NUM >= 34 )
        CTSUSO1_MUTUAL0_KEY33,
    #endif
    #if ( MUTUAL0_NUM >= 35 )
        CTSUSO1_MUTUAL0_KEY34,
    #endif
    #if ( MUTUAL0_NUM >= 36 )
        CTSUSO1_MUTUAL0_KEY35,
    #endif
    #if ( MUTUAL0_NUM >= 37 )
        CTSUSO1_MUTUAL0_KEY36,
    #endif
    #if ( MUTUAL0_NUM >= 38 )
        CTSUSO1_MUTUAL0_KEY37,
    #endif
    #if ( MUTUAL0_NUM >= 39 )
        CTSUSO1_MUTUAL0_KEY38,
    #endif
    #if ( MUTUAL0_NUM >= 40 )
        CTSUSO1_MUTUAL0_KEY39,
    #endif
    #if ( MUTUAL0_NUM >= 41 )
        CTSUSO1_MUTUAL0_KEY40,
    #endif
    #if ( MUTUAL0_NUM >= 42 )
        CTSUSO1_MUTUAL0_KEY41,
    #endif
    #if ( MUTUAL0_NUM >= 43 )
        CTSUSO1_MUTUAL0_KEY42,
    #endif
    #if ( MUTUAL0_NUM >= 44 )
        CTSUSO1_MUTUAL0_KEY43,
    #endif
    #if ( MUTUAL0_NUM >= 45 )
        CTSUSO1_MUTUAL0_KEY44,
    #endif
    #if ( MUTUAL0_NUM >= 46 )
        CTSUSO1_MUTUAL0_KEY45,
    #endif
    #if ( MUTUAL0_NUM >= 47 )
        CTSUSO1_MUTUAL0_KEY46,
    #endif
    #if ( MUTUAL0_NUM >= 48 )
        CTSUSO1_MUTUAL0_KEY47,
    #endif
    #if ( MUTUAL0_NUM >= 49 )
        CTSUSO1_MUTUAL0_KEY48,
    #endif
    #if ( MUTUAL0_NUM >= 50 )
        CTSUSO1_MUTUAL0_KEY49,
    #endif
    #if ( MUTUAL0_NUM >= 51 )
        CTSUSO1_MUTUAL0_KEY50,
    #endif
    #if ( MUTUAL0_NUM >= 52 )
        CTSUSO1_MUTUAL0_KEY51,
    #endif
    #if ( MUTUAL0_NUM >= 53 )
        CTSUSO1_MUTUAL0_KEY52,
    #endif
    #if ( MUTUAL0_NUM >= 54 )
        CTSUSO1_MUTUAL0_KEY53,
    #endif
    #if ( MUTUAL0_NUM >= 55 )
        CTSUSO1_MUTUAL0_KEY54,
    #endif
    #if ( MUTUAL0_NUM >= 56 )
        CTSUSO1_MUTUAL0_KEY55,
    #endif
    #if ( MUTUAL0_NUM >= 57 )
        CTSUSO1_MUTUAL0_KEY56,
    #endif
    #if ( MUTUAL0_NUM >= 58 )
        CTSUSO1_MUTUAL0_KEY57,
    #endif
    #if ( MUTUAL0_NUM >= 59 )
        CTSUSO1_MUTUAL0_KEY58,
    #endif
    #if ( MUTUAL0_NUM >= 60 )
        CTSUSO1_MUTUAL0_KEY59,
    #endif
    #if ( MUTUAL0_NUM >= 61 )
        CTSUSO1_MUTUAL0_KEY60,
    #endif
    #if ( MUTUAL0_NUM >= 62 )
        CTSUSO1_MUTUAL0_KEY61,
    #endif
    #if ( MUTUAL0_NUM >= 63 )
        CTSUSO1_MUTUAL0_KEY62,
    #endif
    #if ( MUTUAL0_NUM >= 64 )
        CTSUSO1_MUTUAL0_KEY63
    #endif
};
#endif    // ( MUTUAL_METHOD_NUM > 0 )

/*=====    Mutual 1    DTC transmit data ========================================================*/
#if ( MUTUAL_METHOD_NUM > 1 )
const uint16_t g_mutual1_ctsussc[] =
{
    #if ( MUTUAL1_NUM >= 1 )
        CTSUSSC_MUTUAL1_KEY00,
    #endif
    #if ( MUTUAL1_NUM >= 2 )
        CTSUSSC_MUTUAL1_KEY01,
    #endif
    #if ( MUTUAL1_NUM >= 3 )
        CTSUSSC_MUTUAL1_KEY02,
    #endif
    #if ( MUTUAL1_NUM >= 4 )
        CTSUSSC_MUTUAL1_KEY03,
    #endif
    #if ( MUTUAL1_NUM >= 5 )
        CTSUSSC_MUTUAL1_KEY04,
    #endif
    #if ( MUTUAL1_NUM >= 6 )
        CTSUSSC_MUTUAL1_KEY05,
    #endif
    #if ( MUTUAL1_NUM >= 7 )
        CTSUSSC_MUTUAL1_KEY06,
    #endif
    #if ( MUTUAL1_NUM >= 8 )
        CTSUSSC_MUTUAL1_KEY07,
    #endif
    #if ( MUTUAL1_NUM >= 9 )
        CTSUSSC_MUTUAL1_KEY08,
    #endif
    #if ( MUTUAL1_NUM >= 10 )
        CTSUSSC_MUTUAL1_KEY09,
    #endif
    #if ( MUTUAL1_NUM >= 11 )
        CTSUSSC_MUTUAL1_KEY10,
    #endif
    #if ( MUTUAL1_NUM >= 12 )
        CTSUSSC_MUTUAL1_KEY11,
    #endif
    #if ( MUTUAL1_NUM >= 13 )
        CTSUSSC_MUTUAL1_KEY12,
    #endif
    #if ( MUTUAL1_NUM >= 14 )
        CTSUSSC_MUTUAL1_KEY13,
    #endif
    #if ( MUTUAL1_NUM >= 15 )
        CTSUSSC_MUTUAL1_KEY14,
    #endif
    #if ( MUTUAL1_NUM >= 16 )
        CTSUSSC_MUTUAL1_KEY15,
    #endif
    #if ( MUTUAL1_NUM >= 17 )
        CTSUSSC_MUTUAL1_KEY16,
    #endif
    #if ( MUTUAL1_NUM >= 18 )
        CTSUSSC_MUTUAL1_KEY17,
    #endif
    #if ( MUTUAL1_NUM >= 19 )
        CTSUSSC_MUTUAL1_KEY18,
    #endif
    #if ( MUTUAL1_NUM >= 20 )
        CTSUSSC_MUTUAL1_KEY19,
    #endif
    #if ( MUTUAL1_NUM >= 21 )
        CTSUSSC_MUTUAL1_KEY20,
    #endif
    #if ( MUTUAL1_NUM >= 22 )
        CTSUSSC_MUTUAL1_KEY21,
    #endif
    #if ( MUTUAL1_NUM >= 23 )
        CTSUSSC_MUTUAL1_KEY22,
    #endif
    #if ( MUTUAL1_NUM >= 24 )
        CTSUSSC_MUTUAL1_KEY23,
    #endif
    #if ( MUTUAL1_NUM >= 25 )
        CTSUSSC_MUTUAL1_KEY24,
    #endif
    #if ( MUTUAL1_NUM >= 26 )
        CTSUSSC_MUTUAL1_KEY25,
    #endif
    #if ( MUTUAL1_NUM >= 27 )
        CTSUSSC_MUTUAL1_KEY26,
    #endif
    #if ( MUTUAL1_NUM >= 28 )
        CTSUSSC_MUTUAL1_KEY27,
    #endif
    #if ( MUTUAL1_NUM >= 29 )
        CTSUSSC_MUTUAL1_KEY28,
    #endif
    #if ( MUTUAL1_NUM >= 30 )
        CTSUSSC_MUTUAL1_KEY29,
    #endif
    #if ( MUTUAL1_NUM >= 31 )
        CTSUSSC_MUTUAL1_KEY30,
    #endif
    #if ( MUTUAL1_NUM >= 32 )
        CTSUSSC_MUTUAL1_KEY31,
    #endif
    #if ( MUTUAL1_NUM >= 33 )
        CTSUSSC_MUTUAL1_KEY32,
    #endif
    #if ( MUTUAL1_NUM >= 34 )
        CTSUSSC_MUTUAL1_KEY33,
    #endif
    #if ( MUTUAL1_NUM >= 35 )
        CTSUSSC_MUTUAL1_KEY34,
    #endif
    #if ( MUTUAL1_NUM >= 36 )
        CTSUSSC_MUTUAL1_KEY35,
    #endif
    #if ( MUTUAL1_NUM >= 37 )
        CTSUSSC_MUTUAL1_KEY36,
    #endif
    #if ( MUTUAL1_NUM >= 38 )
        CTSUSSC_MUTUAL1_KEY37,
    #endif
    #if ( MUTUAL1_NUM >= 39 )
        CTSUSSC_MUTUAL1_KEY38,
    #endif
    #if ( MUTUAL1_NUM >= 40 )
        CTSUSSC_MUTUAL1_KEY39,
    #endif
    #if ( MUTUAL1_NUM >= 41 )
        CTSUSSC_MUTUAL1_KEY40,
    #endif
    #if ( MUTUAL1_NUM >= 42 )
        CTSUSSC_MUTUAL1_KEY41,
    #endif
    #if ( MUTUAL1_NUM >= 43 )
        CTSUSSC_MUTUAL1_KEY42,
    #endif
    #if ( MUTUAL1_NUM >= 44 )
        CTSUSSC_MUTUAL1_KEY43,
    #endif
    #if ( MUTUAL1_NUM >= 45 )
        CTSUSSC_MUTUAL1_KEY44,
    #endif
    #if ( MUTUAL1_NUM >= 46 )
        CTSUSSC_MUTUAL1_KEY45,
    #endif
    #if ( MUTUAL1_NUM >= 47 )
        CTSUSSC_MUTUAL1_KEY46,
    #endif
    #if ( MUTUAL1_NUM >= 48 )
        CTSUSSC_MUTUAL1_KEY47,
    #endif
    #if ( MUTUAL1_NUM >= 49 )
        CTSUSSC_MUTUAL1_KEY48,
    #endif
    #if ( MUTUAL1_NUM >= 50 )
        CTSUSSC_MUTUAL1_KEY49,
    #endif
    #if ( MUTUAL1_NUM >= 51 )
        CTSUSSC_MUTUAL1_KEY50,
    #endif
    #if ( MUTUAL1_NUM >= 52 )
        CTSUSSC_MUTUAL1_KEY51,
    #endif
    #if ( MUTUAL1_NUM >= 53 )
        CTSUSSC_MUTUAL1_KEY52,
    #endif
    #if ( MUTUAL1_NUM >= 54 )
        CTSUSSC_MUTUAL1_KEY53,
    #endif
    #if ( MUTUAL1_NUM >= 55 )
        CTSUSSC_MUTUAL1_KEY54,
    #endif
    #if ( MUTUAL1_NUM >= 56 )
        CTSUSSC_MUTUAL1_KEY55,
    #endif
    #if ( MUTUAL1_NUM >= 57 )
        CTSUSSC_MUTUAL1_KEY56,
    #endif
    #if ( MUTUAL1_NUM >= 58 )
        CTSUSSC_MUTUAL1_KEY57,
    #endif
    #if ( MUTUAL1_NUM >= 59 )
        CTSUSSC_MUTUAL1_KEY58,
    #endif
    #if ( MUTUAL1_NUM >= 60 )
        CTSUSSC_MUTUAL1_KEY59,
    #endif
    #if ( MUTUAL1_NUM >= 61 )
        CTSUSSC_MUTUAL1_KEY60,
    #endif
    #if ( MUTUAL1_NUM >= 62 )
        CTSUSSC_MUTUAL1_KEY61,
    #endif
    #if ( MUTUAL1_NUM >= 63 )
        CTSUSSC_MUTUAL1_KEY62,
    #endif
    #if ( MUTUAL1_NUM >= 64 )
        CTSUSSC_MUTUAL1_KEY63
    #endif
};

const uint16_t g_mutual1_ctsuso0[] =
{
    #if ( MUTUAL1_NUM >= 1 )
        CTSUSO0_MUTUAL1_KEY00,
    #endif
    #if ( MUTUAL1_NUM >= 2 )
        CTSUSO0_MUTUAL1_KEY01,
    #endif
    #if ( MUTUAL1_NUM >= 3 )
        CTSUSO0_MUTUAL1_KEY02,
    #endif
    #if ( MUTUAL1_NUM >= 4 )
        CTSUSO0_MUTUAL1_KEY03,
    #endif
    #if ( MUTUAL1_NUM >= 5 )
        CTSUSO0_MUTUAL1_KEY04,
    #endif
    #if ( MUTUAL1_NUM >= 6 )
        CTSUSO0_MUTUAL1_KEY05,
    #endif
    #if ( MUTUAL1_NUM >= 7 )
        CTSUSO0_MUTUAL1_KEY06,
    #endif
    #if ( MUTUAL1_NUM >= 8 )
        CTSUSO0_MUTUAL1_KEY07,
    #endif
    #if ( MUTUAL1_NUM >= 9 )
        CTSUSO0_MUTUAL1_KEY08,
    #endif
    #if ( MUTUAL1_NUM >= 10 )
        CTSUSO0_MUTUAL1_KEY09,
    #endif
    #if ( MUTUAL1_NUM >= 11 )
        CTSUSO0_MUTUAL1_KEY10,
    #endif
    #if ( MUTUAL1_NUM >= 12 )
        CTSUSO0_MUTUAL1_KEY11,
    #endif
    #if ( MUTUAL1_NUM >= 13 )
        CTSUSO0_MUTUAL1_KEY12,
    #endif
    #if ( MUTUAL1_NUM >= 14 )
        CTSUSO0_MUTUAL1_KEY13,
    #endif
    #if ( MUTUAL1_NUM >= 15 )
        CTSUSO0_MUTUAL1_KEY14,
    #endif
    #if ( MUTUAL1_NUM >= 16 )
        CTSUSO0_MUTUAL1_KEY15,
    #endif
    #if ( MUTUAL1_NUM >= 17 )
        CTSUSO0_MUTUAL1_KEY16,
    #endif
    #if ( MUTUAL1_NUM >= 18 )
        CTSUSO0_MUTUAL1_KEY17,
    #endif
    #if ( MUTUAL1_NUM >= 19 )
        CTSUSO0_MUTUAL1_KEY18,
    #endif
    #if ( MUTUAL1_NUM >= 20 )
        CTSUSO0_MUTUAL1_KEY19,
    #endif
    #if ( MUTUAL1_NUM >= 21 )
        CTSUSO0_MUTUAL1_KEY20,
    #endif
    #if ( MUTUAL1_NUM >= 22 )
        CTSUSO0_MUTUAL1_KEY21,
    #endif
    #if ( MUTUAL1_NUM >= 23 )
        CTSUSO0_MUTUAL1_KEY22,
    #endif
    #if ( MUTUAL1_NUM >= 24 )
        CTSUSO0_MUTUAL1_KEY23,
    #endif
    #if ( MUTUAL1_NUM >= 25 )
        CTSUSO0_MUTUAL1_KEY24,
    #endif
    #if ( MUTUAL1_NUM >= 26 )
        CTSUSO0_MUTUAL1_KEY25,
    #endif
    #if ( MUTUAL1_NUM >= 27 )
        CTSUSO0_MUTUAL1_KEY26,
    #endif
    #if ( MUTUAL1_NUM >= 28 )
        CTSUSO0_MUTUAL1_KEY27,
    #endif
    #if ( MUTUAL1_NUM >= 29 )
        CTSUSO0_MUTUAL1_KEY28,
    #endif
    #if ( MUTUAL1_NUM >= 30 )
        CTSUSO0_MUTUAL1_KEY29,
    #endif
    #if ( MUTUAL1_NUM >= 31 )
        CTSUSO0_MUTUAL1_KEY30,
    #endif
    #if ( MUTUAL1_NUM >= 32 )
        CTSUSO0_MUTUAL1_KEY31,
    #endif
    #if ( MUTUAL1_NUM >= 33 )
        CTSUSO0_MUTUAL1_KEY32,
    #endif
    #if ( MUTUAL1_NUM >= 34 )
        CTSUSO0_MUTUAL1_KEY33,
    #endif
    #if ( MUTUAL1_NUM >= 35 )
        CTSUSO0_MUTUAL1_KEY34,
    #endif
    #if ( MUTUAL1_NUM >= 36 )
        CTSUSO0_MUTUAL1_KEY35,
    #endif
    #if ( MUTUAL1_NUM >= 37 )
        CTSUSO0_MUTUAL1_KEY36,
    #endif
    #if ( MUTUAL1_NUM >= 38 )
        CTSUSO0_MUTUAL1_KEY37,
    #endif
    #if ( MUTUAL1_NUM >= 39 )
        CTSUSO0_MUTUAL1_KEY38,
    #endif
    #if ( MUTUAL1_NUM >= 40 )
        CTSUSO0_MUTUAL1_KEY39,
    #endif
    #if ( MUTUAL1_NUM >= 41 )
        CTSUSO0_MUTUAL1_KEY40,
    #endif
    #if ( MUTUAL1_NUM >= 42 )
        CTSUSO0_MUTUAL1_KEY41,
    #endif
    #if ( MUTUAL1_NUM >= 43 )
        CTSUSO0_MUTUAL1_KEY42,
    #endif
    #if ( MUTUAL1_NUM >= 44 )
        CTSUSO0_MUTUAL1_KEY43,
    #endif
    #if ( MUTUAL1_NUM >= 45 )
        CTSUSO0_MUTUAL1_KEY44,
    #endif
    #if ( MUTUAL1_NUM >= 46 )
        CTSUSO0_MUTUAL1_KEY45,
    #endif
    #if ( MUTUAL1_NUM >= 47 )
        CTSUSO0_MUTUAL1_KEY46,
    #endif
    #if ( MUTUAL1_NUM >= 48 )
        CTSUSO0_MUTUAL1_KEY47,
    #endif
    #if ( MUTUAL1_NUM >= 49 )
        CTSUSO0_MUTUAL1_KEY48,
    #endif
    #if ( MUTUAL1_NUM >= 50 )
        CTSUSO0_MUTUAL1_KEY49,
    #endif
    #if ( MUTUAL1_NUM >= 51 )
        CTSUSO0_MUTUAL1_KEY50,
    #endif
    #if ( MUTUAL1_NUM >= 52 )
        CTSUSO0_MUTUAL1_KEY51,
    #endif
    #if ( MUTUAL1_NUM >= 53 )
        CTSUSO0_MUTUAL1_KEY52,
    #endif
    #if ( MUTUAL1_NUM >= 54 )
        CTSUSO0_MUTUAL1_KEY53,
    #endif
    #if ( MUTUAL1_NUM >= 55 )
        CTSUSO0_MUTUAL1_KEY54,
    #endif
    #if ( MUTUAL1_NUM >= 56 )
        CTSUSO0_MUTUAL1_KEY55,
    #endif
    #if ( MUTUAL1_NUM >= 57 )
        CTSUSO0_MUTUAL1_KEY56,
    #endif
    #if ( MUTUAL1_NUM >= 58 )
        CTSUSO0_MUTUAL1_KEY57,
    #endif
    #if ( MUTUAL1_NUM >= 59 )
        CTSUSO0_MUTUAL1_KEY58,
    #endif
    #if ( MUTUAL1_NUM >= 60 )
        CTSUSO0_MUTUAL1_KEY59,
    #endif
    #if ( MUTUAL1_NUM >= 61 )
        CTSUSO0_MUTUAL1_KEY60,
    #endif
    #if ( MUTUAL1_NUM >= 62 )
        CTSUSO0_MUTUAL1_KEY61,
    #endif
    #if ( MUTUAL1_NUM >= 63 )
        CTSUSO0_MUTUAL1_KEY62,
    #endif
    #if ( MUTUAL1_NUM >= 64 )
        CTSUSO0_MUTUAL1_KEY63
    #endif
};

const uint16_t g_mutual1_ctsuso1[] =
{
    #if ( MUTUAL1_NUM >= 1 )
        CTSUSO1_MUTUAL1_KEY00,
    #endif
    #if ( MUTUAL1_NUM >= 2 )
        CTSUSO1_MUTUAL1_KEY01,
    #endif
    #if ( MUTUAL1_NUM >= 3 )
        CTSUSO1_MUTUAL1_KEY02,
    #endif
    #if ( MUTUAL1_NUM >= 4 )
        CTSUSO1_MUTUAL1_KEY03,
    #endif
    #if ( MUTUAL1_NUM >= 5 )
        CTSUSO1_MUTUAL1_KEY04,
    #endif
    #if ( MUTUAL1_NUM >= 6 )
        CTSUSO1_MUTUAL1_KEY05,
    #endif
    #if ( MUTUAL1_NUM >= 7 )
        CTSUSO1_MUTUAL1_KEY06,
    #endif
    #if ( MUTUAL1_NUM >= 8 )
        CTSUSO1_MUTUAL1_KEY07,
    #endif
    #if ( MUTUAL1_NUM >= 9 )
        CTSUSO1_MUTUAL1_KEY08,
    #endif
    #if ( MUTUAL1_NUM >= 10 )
        CTSUSO1_MUTUAL1_KEY09,
    #endif
    #if ( MUTUAL1_NUM >= 11 )
        CTSUSO1_MUTUAL1_KEY10,
    #endif
    #if ( MUTUAL1_NUM >= 12 )
        CTSUSO1_MUTUAL1_KEY11,
    #endif
    #if ( MUTUAL1_NUM >= 13 )
        CTSUSO1_MUTUAL1_KEY12,
    #endif
    #if ( MUTUAL1_NUM >= 14 )
        CTSUSO1_MUTUAL1_KEY13,
    #endif
    #if ( MUTUAL1_NUM >= 15 )
        CTSUSO1_MUTUAL1_KEY14,
    #endif
    #if ( MUTUAL1_NUM >= 16 )
        CTSUSO1_MUTUAL1_KEY15,
    #endif
    #if ( MUTUAL1_NUM >= 17 )
        CTSUSO1_MUTUAL1_KEY16,
    #endif
    #if ( MUTUAL1_NUM >= 18 )
        CTSUSO1_MUTUAL1_KEY17,
    #endif
    #if ( MUTUAL1_NUM >= 19 )
        CTSUSO1_MUTUAL1_KEY18,
    #endif
    #if ( MUTUAL1_NUM >= 20 )
        CTSUSO1_MUTUAL1_KEY19,
    #endif
    #if ( MUTUAL1_NUM >= 21 )
        CTSUSO1_MUTUAL1_KEY20,
    #endif
    #if ( MUTUAL1_NUM >= 22 )
        CTSUSO1_MUTUAL1_KEY21,
    #endif
    #if ( MUTUAL1_NUM >= 23 )
        CTSUSO1_MUTUAL1_KEY22,
    #endif
    #if ( MUTUAL1_NUM >= 24 )
        CTSUSO1_MUTUAL1_KEY23,
    #endif
    #if ( MUTUAL1_NUM >= 25 )
        CTSUSO1_MUTUAL1_KEY24,
    #endif
    #if ( MUTUAL1_NUM >= 26 )
        CTSUSO1_MUTUAL1_KEY25,
    #endif
    #if ( MUTUAL1_NUM >= 27 )
        CTSUSO1_MUTUAL1_KEY26,
    #endif
    #if ( MUTUAL1_NUM >= 28 )
        CTSUSO1_MUTUAL1_KEY27,
    #endif
    #if ( MUTUAL1_NUM >= 29 )
        CTSUSO1_MUTUAL1_KEY28,
    #endif
    #if ( MUTUAL1_NUM >= 30 )
        CTSUSO1_MUTUAL1_KEY29,
    #endif
    #if ( MUTUAL1_NUM >= 31 )
        CTSUSO1_MUTUAL1_KEY30,
    #endif
    #if ( MUTUAL1_NUM >= 32 )
        CTSUSO1_MUTUAL1_KEY31,
    #endif
    #if ( MUTUAL1_NUM >= 33 )
        CTSUSO1_MUTUAL1_KEY32,
    #endif
    #if ( MUTUAL1_NUM >= 34 )
        CTSUSO1_MUTUAL1_KEY33,
    #endif
    #if ( MUTUAL1_NUM >= 35 )
        CTSUSO1_MUTUAL1_KEY34,
    #endif
    #if ( MUTUAL1_NUM >= 36 )
        CTSUSO1_MUTUAL1_KEY35,
    #endif
    #if ( MUTUAL1_NUM >= 37 )
        CTSUSO1_MUTUAL1_KEY36,
    #endif
    #if ( MUTUAL1_NUM >= 38 )
        CTSUSO1_MUTUAL1_KEY37,
    #endif
    #if ( MUTUAL1_NUM >= 39 )
        CTSUSO1_MUTUAL1_KEY38,
    #endif
    #if ( MUTUAL1_NUM >= 40 )
        CTSUSO1_MUTUAL1_KEY39,
    #endif
    #if ( MUTUAL1_NUM >= 41 )
        CTSUSO1_MUTUAL1_KEY40,
    #endif
    #if ( MUTUAL1_NUM >= 42 )
        CTSUSO1_MUTUAL1_KEY41,
    #endif
    #if ( MUTUAL1_NUM >= 43 )
        CTSUSO1_MUTUAL1_KEY42,
    #endif
    #if ( MUTUAL1_NUM >= 44 )
        CTSUSO1_MUTUAL1_KEY43,
    #endif
    #if ( MUTUAL1_NUM >= 45 )
        CTSUSO1_MUTUAL1_KEY44,
    #endif
    #if ( MUTUAL1_NUM >= 46 )
        CTSUSO1_MUTUAL1_KEY45,
    #endif
    #if ( MUTUAL1_NUM >= 47 )
        CTSUSO1_MUTUAL1_KEY46,
    #endif
    #if ( MUTUAL1_NUM >= 48 )
        CTSUSO1_MUTUAL1_KEY47,
    #endif
    #if ( MUTUAL1_NUM >= 49 )
        CTSUSO1_MUTUAL1_KEY48,
    #endif
    #if ( MUTUAL1_NUM >= 50 )
        CTSUSO1_MUTUAL1_KEY49,
    #endif
    #if ( MUTUAL1_NUM >= 51 )
        CTSUSO1_MUTUAL1_KEY50,
    #endif
    #if ( MUTUAL1_NUM >= 52 )
        CTSUSO1_MUTUAL1_KEY51,
    #endif
    #if ( MUTUAL1_NUM >= 53 )
        CTSUSO1_MUTUAL1_KEY52,
    #endif
    #if ( MUTUAL1_NUM >= 54 )
        CTSUSO1_MUTUAL1_KEY53,
    #endif
    #if ( MUTUAL1_NUM >= 55 )
        CTSUSO1_MUTUAL1_KEY54,
    #endif
    #if ( MUTUAL1_NUM >= 56 )
        CTSUSO1_MUTUAL1_KEY55,
    #endif
    #if ( MUTUAL1_NUM >= 57 )
        CTSUSO1_MUTUAL1_KEY56,
    #endif
    #if ( MUTUAL1_NUM >= 58 )
        CTSUSO1_MUTUAL1_KEY57,
    #endif
    #if ( MUTUAL1_NUM >= 59 )
        CTSUSO1_MUTUAL1_KEY58,
    #endif
    #if ( MUTUAL1_NUM >= 60 )
        CTSUSO1_MUTUAL1_KEY59,
    #endif
    #if ( MUTUAL1_NUM >= 61 )
        CTSUSO1_MUTUAL1_KEY60,
    #endif
    #if ( MUTUAL1_NUM >= 62 )
        CTSUSO1_MUTUAL1_KEY61,
    #endif
    #if ( MUTUAL1_NUM >= 63 )
        CTSUSO1_MUTUAL1_KEY62,
    #endif
    #if ( MUTUAL1_NUM >= 64 )
        CTSUSO1_MUTUAL1_KEY63
    #endif
};
#endif    // ( MUTUAL_METHOD_NUM > 1 )

/*=====    Mutual 2    DTC transmit data ========================================================*/
#if ( MUTUAL_METHOD_NUM > 2 )
const uint16_t g_mutual2_ctsussc[] =
{
    #if ( MUTUAL2_NUM > 0 )
        CTSUSSC_MUTUAL2_KEY00,
    #endif
    #if ( MUTUAL2_NUM > 1 )
        CTSUSSC_MUTUAL2_KEY01,
    #endif
    #if ( MUTUAL2_NUM > 2 )
        CTSUSSC_MUTUAL2_KEY02,
    #endif
    #if ( MUTUAL2_NUM > 3 )
        CTSUSSC_MUTUAL2_KEY03,
    #endif
    #if ( MUTUAL2_NUM > 4 )
        CTSUSSC_MUTUAL2_KEY04,
    #endif
    #if ( MUTUAL2_NUM > 5 )
        CTSUSSC_MUTUAL2_KEY05,
    #endif
    #if ( MUTUAL2_NUM > 6 )
        CTSUSSC_MUTUAL2_KEY06,
    #endif
    #if ( MUTUAL2_NUM > 7 )
        CTSUSSC_MUTUAL2_KEY07,
    #endif
    #if ( MUTUAL2_NUM > 8 )
        CTSUSSC_MUTUAL2_KEY08,
    #endif
    #if ( MUTUAL2_NUM > 9 )
        CTSUSSC_MUTUAL2_KEY09,
    #endif
    #if ( MUTUAL2_NUM > 10 )
        CTSUSSC_MUTUAL2_KEY10,
    #endif
    #if ( MUTUAL2_NUM > 11 )
        CTSUSSC_MUTUAL2_KEY11,
    #endif
    #if ( MUTUAL2_NUM > 12 )
        CTSUSSC_MUTUAL2_KEY12,
    #endif
    #if ( MUTUAL2_NUM > 13 )
        CTSUSSC_MUTUAL2_KEY13,
    #endif
    #if ( MUTUAL2_NUM > 14 )
        CTSUSSC_MUTUAL2_KEY14,
    #endif
    #if ( MUTUAL2_NUM > 15 )
        CTSUSSC_MUTUAL2_KEY15,
    #endif
    #if ( MUTUAL2_NUM > 16 )
        CTSUSSC_MUTUAL2_KEY16,
    #endif
    #if ( MUTUAL2_NUM > 17 )
        CTSUSSC_MUTUAL2_KEY17,
    #endif
    #if ( MUTUAL2_NUM > 18 )
        CTSUSSC_MUTUAL2_KEY18,
    #endif
    #if ( MUTUAL2_NUM > 19 )
        CTSUSSC_MUTUAL2_KEY19,
    #endif
    #if ( MUTUAL2_NUM > 20 )
        CTSUSSC_MUTUAL2_KEY20,
    #endif
    #if ( MUTUAL2_NUM > 21 )
        CTSUSSC_MUTUAL2_KEY21,
    #endif
    #if ( MUTUAL2_NUM > 22 )
        CTSUSSC_MUTUAL2_KEY22,
    #endif
    #if ( MUTUAL2_NUM > 23 )
        CTSUSSC_MUTUAL2_KEY23,
    #endif
    #if ( MUTUAL2_NUM > 24 )
        CTSUSSC_MUTUAL2_KEY24,
    #endif
    #if ( MUTUAL2_NUM > 25 )
        CTSUSSC_MUTUAL2_KEY25,
    #endif
    #if ( MUTUAL2_NUM > 26 )
        CTSUSSC_MUTUAL2_KEY26,
    #endif
    #if ( MUTUAL2_NUM > 27 )
        CTSUSSC_MUTUAL2_KEY27,
    #endif
    #if ( MUTUAL2_NUM > 28 )
        CTSUSSC_MUTUAL2_KEY28,
    #endif
    #if ( MUTUAL2_NUM > 29 )
        CTSUSSC_MUTUAL2_KEY29,
    #endif
    #if ( MUTUAL2_NUM > 30 )
        CTSUSSC_MUTUAL2_KEY30,
    #endif
    #if ( MUTUAL2_NUM > 31 )
        CTSUSSC_MUTUAL2_KEY31,
    #endif
    #if ( MUTUAL2_NUM > 32 )
        CTSUSSC_MUTUAL2_KEY32,
    #endif
    #if ( MUTUAL2_NUM > 33 )
        CTSUSSC_MUTUAL2_KEY33,
    #endif
    #if ( MUTUAL2_NUM > 34 )
        CTSUSSC_MUTUAL2_KEY34,
    #endif
    #if ( MUTUAL2_NUM > 35 )
        CTSUSSC_MUTUAL2_KEY35,
    #endif
    #if ( MUTUAL2_NUM > 36 )
        CTSUSSC_MUTUAL2_KEY36,
    #endif
    #if ( MUTUAL2_NUM > 37 )
        CTSUSSC_MUTUAL2_KEY37,
    #endif
    #if ( MUTUAL2_NUM > 38 )
        CTSUSSC_MUTUAL2_KEY38,
    #endif
    #if ( MUTUAL2_NUM > 39 )
        CTSUSSC_MUTUAL2_KEY39,
    #endif
    #if ( MUTUAL2_NUM > 40 )
        CTSUSSC_MUTUAL2_KEY40,
    #endif
    #if ( MUTUAL2_NUM > 41 )
        CTSUSSC_MUTUAL2_KEY41,
    #endif
    #if ( MUTUAL2_NUM > 42 )
        CTSUSSC_MUTUAL2_KEY42,
    #endif
    #if ( MUTUAL2_NUM > 43 )
        CTSUSSC_MUTUAL2_KEY43,
    #endif
    #if ( MUTUAL2_NUM > 44 )
        CTSUSSC_MUTUAL2_KEY44,
    #endif
    #if ( MUTUAL2_NUM > 45 )
        CTSUSSC_MUTUAL2_KEY45,
    #endif
    #if ( MUTUAL2_NUM > 46 )
        CTSUSSC_MUTUAL2_KEY46,
    #endif
    #if ( MUTUAL2_NUM > 47 )
        CTSUSSC_MUTUAL2_KEY47,
    #endif
    #if ( MUTUAL2_NUM > 48 )
        CTSUSSC_MUTUAL2_KEY48,
    #endif
    #if ( MUTUAL2_NUM > 49 )
        CTSUSSC_MUTUAL2_KEY49,
    #endif
    #if ( MUTUAL2_NUM > 50 )
        CTSUSSC_MUTUAL2_KEY50,
    #endif
    #if ( MUTUAL2_NUM > 51 )
        CTSUSSC_MUTUAL2_KEY51,
    #endif
    #if ( MUTUAL2_NUM > 52 )
        CTSUSSC_MUTUAL2_KEY52,
    #endif
    #if ( MUTUAL2_NUM > 53 )
        CTSUSSC_MUTUAL2_KEY53,
    #endif
    #if ( MUTUAL2_NUM > 54 )
        CTSUSSC_MUTUAL2_KEY54,
    #endif
    #if ( MUTUAL2_NUM > 55 )
        CTSUSSC_MUTUAL2_KEY55,
    #endif
    #if ( MUTUAL2_NUM > 56 )
        CTSUSSC_MUTUAL2_KEY56,
    #endif
    #if ( MUTUAL2_NUM > 57 )
        CTSUSSC_MUTUAL2_KEY57,
    #endif
    #if ( MUTUAL2_NUM > 58 )
        CTSUSSC_MUTUAL2_KEY58,
    #endif
    #if ( MUTUAL2_NUM > 59 )
        CTSUSSC_MUTUAL2_KEY59,
    #endif
    #if ( MUTUAL2_NUM > 60 )
        CTSUSSC_MUTUAL2_KEY60,
    #endif
    #if ( MUTUAL2_NUM > 61 )
        CTSUSSC_MUTUAL2_KEY61,
    #endif
    #if ( MUTUAL2_NUM > 62 )
        CTSUSSC_MUTUAL2_KEY62,
    #endif
    #if ( MUTUAL2_NUM > 63 )
        CTSUSSC_MUTUAL2_KEY63
    #endif
};

const uint16_t g_mutual2_ctsuso0[] =
{
#if ( MUTUAL2_NUM > 0 )
    CTSUSO0_MUTUAL2_KEY00,
#endif
#if ( MUTUAL2_NUM > 1 )
    CTSUSO0_MUTUAL2_KEY01,
#endif
#if ( MUTUAL2_NUM > 2 )
    CTSUSO0_MUTUAL2_KEY02,
#endif
#if ( MUTUAL2_NUM > 3 )
    CTSUSO0_MUTUAL2_KEY03,
#endif
#if ( MUTUAL2_NUM > 4 )
    CTSUSO0_MUTUAL2_KEY04,
#endif
#if ( MUTUAL2_NUM > 5 )
    CTSUSO0_MUTUAL2_KEY05,
#endif
#if ( MUTUAL2_NUM > 6 )
    CTSUSO0_MUTUAL2_KEY06,
#endif
#if ( MUTUAL2_NUM > 7 )
    CTSUSO0_MUTUAL2_KEY07,
#endif
#if ( MUTUAL2_NUM > 8 )
    CTSUSO0_MUTUAL2_KEY08,
#endif
#if ( MUTUAL2_NUM > 9 )
    CTSUSO0_MUTUAL2_KEY09,
#endif
#if ( MUTUAL2_NUM > 10 )
    CTSUSO0_MUTUAL2_KEY10,
#endif
#if ( MUTUAL2_NUM > 11 )
    CTSUSO0_MUTUAL2_KEY11,
#endif
#if ( MUTUAL2_NUM > 12 )
    CTSUSO0_MUTUAL2_KEY12,
#endif
#if ( MUTUAL2_NUM > 13 )
    CTSUSO0_MUTUAL2_KEY13,
#endif
#if ( MUTUAL2_NUM > 14 )
    CTSUSO0_MUTUAL2_KEY14,
#endif
#if ( MUTUAL2_NUM > 15 )
    CTSUSO0_MUTUAL2_KEY15,
#endif
#if ( MUTUAL2_NUM > 16 )
    CTSUSO0_MUTUAL2_KEY16,
#endif
#if ( MUTUAL2_NUM > 17 )
    CTSUSO0_MUTUAL2_KEY17,
#endif
#if ( MUTUAL2_NUM > 18 )
    CTSUSO0_MUTUAL2_KEY18,
#endif
#if ( MUTUAL2_NUM > 19 )
    CTSUSO0_MUTUAL2_KEY19,
#endif
#if ( MUTUAL2_NUM > 20 )
    CTSUSO0_MUTUAL2_KEY20,
#endif
#if ( MUTUAL2_NUM > 21 )
    CTSUSO0_MUTUAL2_KEY21,
#endif
#if ( MUTUAL2_NUM > 22 )
    CTSUSO0_MUTUAL2_KEY22,
#endif
#if ( MUTUAL2_NUM > 23 )
    CTSUSO0_MUTUAL2_KEY23,
#endif
#if ( MUTUAL2_NUM > 24 )
    CTSUSO0_MUTUAL2_KEY24,
#endif
#if ( MUTUAL2_NUM > 25 )
    CTSUSO0_MUTUAL2_KEY25,
#endif
#if ( MUTUAL2_NUM > 26 )
    CTSUSO0_MUTUAL2_KEY26,
#endif
#if ( MUTUAL2_NUM > 27 )
    CTSUSO0_MUTUAL2_KEY27,
#endif
#if ( MUTUAL2_NUM > 28 )
    CTSUSO0_MUTUAL2_KEY28,
#endif
#if ( MUTUAL2_NUM > 29 )
    CTSUSO0_MUTUAL2_KEY29,
#endif
#if ( MUTUAL2_NUM > 30 )
    CTSUSO0_MUTUAL2_KEY30,
#endif
#if ( MUTUAL2_NUM > 31 )
    CTSUSO0_MUTUAL2_KEY31,
#endif
#if ( MUTUAL2_NUM > 32 )
    CTSUSO0_MUTUAL2_KEY32,
#endif
#if ( MUTUAL2_NUM > 33 )
    CTSUSO0_MUTUAL2_KEY33,
#endif
#if ( MUTUAL2_NUM > 34 )
    CTSUSO0_MUTUAL2_KEY34,
#endif
#if ( MUTUAL2_NUM > 35 )
    CTSUSO0_MUTUAL2_KEY35,
#endif
#if ( MUTUAL2_NUM > 36 )
    CTSUSO0_MUTUAL2_KEY36,
#endif
#if ( MUTUAL2_NUM > 37 )
    CTSUSO0_MUTUAL2_KEY37,
#endif
#if ( MUTUAL2_NUM > 38 )
    CTSUSO0_MUTUAL2_KEY38,
#endif
#if ( MUTUAL2_NUM > 39 )
    CTSUSO0_MUTUAL2_KEY39,
#endif
#if ( MUTUAL2_NUM > 40 )
    CTSUSO0_MUTUAL2_KEY40,
#endif
#if ( MUTUAL2_NUM > 41 )
    CTSUSO0_MUTUAL2_KEY41,
#endif
#if ( MUTUAL2_NUM > 42 )
    CTSUSO0_MUTUAL2_KEY42,
#endif
#if ( MUTUAL2_NUM > 43 )
    CTSUSO0_MUTUAL2_KEY43,
#endif
#if ( MUTUAL2_NUM > 44 )
    CTSUSO0_MUTUAL2_KEY44,
#endif
#if ( MUTUAL2_NUM > 45 )
    CTSUSO0_MUTUAL2_KEY45,
#endif
#if ( MUTUAL2_NUM > 46 )
    CTSUSO0_MUTUAL2_KEY46,
#endif
#if ( MUTUAL2_NUM > 47 )
    CTSUSO0_MUTUAL2_KEY47,
#endif
#if ( MUTUAL2_NUM > 48 )
    CTSUSO0_MUTUAL2_KEY48,
#endif
#if ( MUTUAL2_NUM > 49 )
    CTSUSO0_MUTUAL2_KEY49,
#endif
#if ( MUTUAL2_NUM > 50 )
    CTSUSO0_MUTUAL2_KEY50,
#endif
#if ( MUTUAL2_NUM > 51 )
    CTSUSO0_MUTUAL2_KEY51,
#endif
#if ( MUTUAL2_NUM > 52 )
    CTSUSO0_MUTUAL2_KEY52,
#endif
#if ( MUTUAL2_NUM > 53 )
    CTSUSO0_MUTUAL2_KEY53,
#endif
#if ( MUTUAL2_NUM > 54 )
    CTSUSO0_MUTUAL2_KEY54,
#endif
#if ( MUTUAL2_NUM > 55 )
    CTSUSO0_MUTUAL2_KEY55,
#endif
#if ( MUTUAL2_NUM > 56 )
    CTSUSO0_MUTUAL2_KEY56,
#endif
#if ( MUTUAL2_NUM > 57 )
    CTSUSO0_MUTUAL2_KEY57,
#endif
#if ( MUTUAL2_NUM > 58 )
    CTSUSO0_MUTUAL2_KEY58,
#endif
#if ( MUTUAL2_NUM > 59 )
    CTSUSO0_MUTUAL2_KEY59,
#endif
#if ( MUTUAL2_NUM > 60 )
    CTSUSO0_MUTUAL2_KEY60,
#endif
#if ( MUTUAL2_NUM > 61 )
    CTSUSO0_MUTUAL2_KEY61,
#endif
#if ( MUTUAL2_NUM > 62 )
    CTSUSO0_MUTUAL2_KEY62,
#endif
#if ( MUTUAL2_NUM > 63 )
    CTSUSO0_MUTUAL2_KEY63,
#endif
};

const uint16_t g_mutual2_ctsuso1[] =
{
#if ( MUTUAL2_NUM > 0 )
    CTSUSO1_MUTUAL2_KEY00,
#endif
#if ( MUTUAL2_NUM > 1 )
    CTSUSO1_MUTUAL2_KEY01,
#endif
#if ( MUTUAL2_NUM > 2 )
    CTSUSO1_MUTUAL2_KEY02,
#endif
#if ( MUTUAL2_NUM > 3 )
    CTSUSO1_MUTUAL2_KEY03,
#endif
#if ( MUTUAL2_NUM > 4 )
    CTSUSO1_MUTUAL2_KEY04,
#endif
#if ( MUTUAL2_NUM > 5 )
    CTSUSO1_MUTUAL2_KEY05,
#endif
#if ( MUTUAL2_NUM > 6 )
    CTSUSO1_MUTUAL2_KEY06,
#endif
#if ( MUTUAL2_NUM > 7 )
    CTSUSO1_MUTUAL2_KEY07,
#endif
#if ( MUTUAL2_NUM > 8 )
    CTSUSO1_MUTUAL2_KEY08,
#endif
#if ( MUTUAL2_NUM > 9 )
    CTSUSO1_MUTUAL2_KEY09,
#endif
#if ( MUTUAL2_NUM > 10 )
    CTSUSO1_MUTUAL2_KEY10,
#endif
#if ( MUTUAL2_NUM > 11 )
    CTSUSO1_MUTUAL2_KEY11,
#endif
#if ( MUTUAL2_NUM > 12 )
    CTSUSO1_MUTUAL2_KEY12,
#endif
#if ( MUTUAL2_NUM > 13 )
    CTSUSO1_MUTUAL2_KEY13,
#endif
#if ( MUTUAL2_NUM > 14 )
    CTSUSO1_MUTUAL2_KEY14,
#endif
#if ( MUTUAL2_NUM > 15 )
    CTSUSO1_MUTUAL2_KEY15,
#endif
#if ( MUTUAL2_NUM > 16 )
    CTSUSO1_MUTUAL2_KEY16,
#endif
#if ( MUTUAL2_NUM > 17 )
    CTSUSO1_MUTUAL2_KEY17,
#endif
#if ( MUTUAL2_NUM > 18 )
    CTSUSO1_MUTUAL2_KEY18,
#endif
#if ( MUTUAL2_NUM > 19 )
    CTSUSO1_MUTUAL2_KEY19,
#endif
#if ( MUTUAL2_NUM > 20 )
    CTSUSO1_MUTUAL2_KEY20,
#endif
#if ( MUTUAL2_NUM > 21 )
    CTSUSO1_MUTUAL2_KEY21,
#endif
#if ( MUTUAL2_NUM > 22 )
    CTSUSO1_MUTUAL2_KEY22,
#endif
#if ( MUTUAL2_NUM > 23 )
    CTSUSO1_MUTUAL2_KEY23,
#endif
#if ( MUTUAL2_NUM > 24 )
    CTSUSO1_MUTUAL2_KEY24,
#endif
#if ( MUTUAL2_NUM > 25 )
    CTSUSO1_MUTUAL2_KEY25,
#endif
#if ( MUTUAL2_NUM > 26 )
    CTSUSO1_MUTUAL2_KEY26,
#endif
#if ( MUTUAL2_NUM > 27 )
    CTSUSO1_MUTUAL2_KEY27,
#endif
#if ( MUTUAL2_NUM > 28 )
    CTSUSO1_MUTUAL2_KEY28,
#endif
#if ( MUTUAL2_NUM > 29 )
    CTSUSO1_MUTUAL2_KEY29,
#endif
#if ( MUTUAL2_NUM > 30 )
    CTSUSO1_MUTUAL2_KEY30,
#endif
#if ( MUTUAL2_NUM > 31 )
    CTSUSO1_MUTUAL2_KEY31,
#endif
#if ( MUTUAL2_NUM > 32 )
    CTSUSO1_MUTUAL2_KEY32,
#endif
#if ( MUTUAL2_NUM > 33 )
    CTSUSO1_MUTUAL2_KEY33,
#endif
#if ( MUTUAL2_NUM > 34 )
    CTSUSO1_MUTUAL2_KEY34,
#endif
#if ( MUTUAL2_NUM > 35 )
    CTSUSO1_MUTUAL2_KEY35,
#endif
#if ( MUTUAL2_NUM > 36 )
    CTSUSO1_MUTUAL2_KEY36,
#endif
#if ( MUTUAL2_NUM > 37 )
    CTSUSO1_MUTUAL2_KEY37,
#endif
#if ( MUTUAL2_NUM > 38 )
    CTSUSO1_MUTUAL2_KEY38,
#endif
#if ( MUTUAL2_NUM > 39 )
    CTSUSO1_MUTUAL2_KEY39,
#endif
#if ( MUTUAL2_NUM > 40 )
    CTSUSO1_MUTUAL2_KEY40,
#endif
#if ( MUTUAL2_NUM > 41 )
    CTSUSO1_MUTUAL2_KEY41,
#endif
#if ( MUTUAL2_NUM > 42 )
    CTSUSO1_MUTUAL2_KEY42,
#endif
#if ( MUTUAL2_NUM > 43 )
    CTSUSO1_MUTUAL2_KEY43,
#endif
#if ( MUTUAL2_NUM > 44 )
    CTSUSO1_MUTUAL2_KEY44,
#endif
#if ( MUTUAL2_NUM > 45 )
    CTSUSO1_MUTUAL2_KEY45,
#endif
#if ( MUTUAL2_NUM > 46 )
    CTSUSO1_MUTUAL2_KEY46,
#endif
#if ( MUTUAL2_NUM > 47 )
    CTSUSO1_MUTUAL2_KEY47,
#endif
#if ( MUTUAL2_NUM > 48 )
    CTSUSO1_MUTUAL2_KEY48,
#endif
#if ( MUTUAL2_NUM > 49 )
    CTSUSO1_MUTUAL2_KEY49,
#endif
#if ( MUTUAL2_NUM > 50 )
    CTSUSO1_MUTUAL2_KEY50,
#endif
#if ( MUTUAL2_NUM > 51 )
    CTSUSO1_MUTUAL2_KEY51,
#endif
#if ( MUTUAL2_NUM > 52 )
    CTSUSO1_MUTUAL2_KEY52,
#endif
#if ( MUTUAL2_NUM > 53 )
    CTSUSO1_MUTUAL2_KEY53,
#endif
#if ( MUTUAL2_NUM > 54 )
    CTSUSO1_MUTUAL2_KEY54,
#endif
#if ( MUTUAL2_NUM > 55 )
    CTSUSO1_MUTUAL2_KEY55,
#endif
#if ( MUTUAL2_NUM > 56 )
    CTSUSO1_MUTUAL2_KEY56,
#endif
#if ( MUTUAL2_NUM > 57 )
    CTSUSO1_MUTUAL2_KEY57,
#endif
#if ( MUTUAL2_NUM > 58 )
    CTSUSO1_MUTUAL2_KEY58,
#endif
#if ( MUTUAL2_NUM > 59 )
    CTSUSO1_MUTUAL2_KEY59,
#endif
#if ( MUTUAL2_NUM > 60 )
    CTSUSO1_MUTUAL2_KEY60,
#endif
#if ( MUTUAL2_NUM > 61 )
    CTSUSO1_MUTUAL2_KEY61,
#endif
#if ( MUTUAL2_NUM > 62 )
    CTSUSO1_MUTUAL2_KEY62,
#endif
#if ( MUTUAL2_NUM > 63 )
    CTSUSO1_MUTUAL2_KEY63,
#endif
};
#endif    // ( MUTUAL_METHOD_NUM > 2 )

/*=====    Mutual 3    DTC transmit data ========================================================*/
#if ( MUTUAL_METHOD_NUM > 3 )
const uint16_t g_mutual3_ctsussc[] =
{
#if ( MUTUAL3_NUM > 0 )
    CTSUSSC_MUTUAL3_KEY00,
#endif
#if ( MUTUAL3_NUM > 1 )
    CTSUSSC_MUTUAL3_KEY01,
#endif
#if ( MUTUAL3_NUM > 2 )
    CTSUSSC_MUTUAL3_KEY02,
#endif
#if ( MUTUAL3_NUM > 3 )
    CTSUSSC_MUTUAL3_KEY03,
#endif
#if ( MUTUAL3_NUM > 4 )
    CTSUSSC_MUTUAL3_KEY04,
#endif
#if ( MUTUAL3_NUM > 5 )
    CTSUSSC_MUTUAL3_KEY05,
#endif
#if ( MUTUAL3_NUM > 6 )
    CTSUSSC_MUTUAL3_KEY06,
#endif
#if ( MUTUAL3_NUM > 7 )
    CTSUSSC_MUTUAL3_KEY07,
#endif
#if ( MUTUAL3_NUM > 8 )
    CTSUSSC_MUTUAL3_KEY08,
#endif
#if ( MUTUAL3_NUM > 9 )
    CTSUSSC_MUTUAL3_KEY09,
#endif
#if ( MUTUAL3_NUM > 10 )
    CTSUSSC_MUTUAL3_KEY10,
#endif
#if ( MUTUAL3_NUM > 11 )
    CTSUSSC_MUTUAL3_KEY11,
#endif
#if ( MUTUAL3_NUM > 12 )
    CTSUSSC_MUTUAL3_KEY12,
#endif
#if ( MUTUAL3_NUM > 13 )
    CTSUSSC_MUTUAL3_KEY13,
#endif
#if ( MUTUAL3_NUM > 14 )
    CTSUSSC_MUTUAL3_KEY14,
#endif
#if ( MUTUAL3_NUM > 15 )
    CTSUSSC_MUTUAL3_KEY15,
#endif
#if ( MUTUAL3_NUM > 16 )
    CTSUSSC_MUTUAL3_KEY16,
#endif
#if ( MUTUAL3_NUM > 17 )
    CTSUSSC_MUTUAL3_KEY17,
#endif
#if ( MUTUAL3_NUM > 18 )
    CTSUSSC_MUTUAL3_KEY18,
#endif
#if ( MUTUAL3_NUM > 19 )
    CTSUSSC_MUTUAL3_KEY19,
#endif
#if ( MUTUAL3_NUM > 20 )
    CTSUSSC_MUTUAL3_KEY20,
#endif
#if ( MUTUAL3_NUM > 21 )
    CTSUSSC_MUTUAL3_KEY21,
#endif
#if ( MUTUAL3_NUM > 22 )
    CTSUSSC_MUTUAL3_KEY22,
#endif
#if ( MUTUAL3_NUM > 23 )
    CTSUSSC_MUTUAL3_KEY23,
#endif
#if ( MUTUAL3_NUM > 24 )
    CTSUSSC_MUTUAL3_KEY24,
#endif
#if ( MUTUAL3_NUM > 25 )
    CTSUSSC_MUTUAL3_KEY25,
#endif
#if ( MUTUAL3_NUM > 26 )
    CTSUSSC_MUTUAL3_KEY26,
#endif
#if ( MUTUAL3_NUM > 27 )
    CTSUSSC_MUTUAL3_KEY27,
#endif
#if ( MUTUAL3_NUM > 28 )
    CTSUSSC_MUTUAL3_KEY28,
#endif
#if ( MUTUAL3_NUM > 29 )
    CTSUSSC_MUTUAL3_KEY29,
#endif
#if ( MUTUAL3_NUM > 30 )
    CTSUSSC_MUTUAL3_KEY30,
#endif
#if ( MUTUAL3_NUM > 31 )
    CTSUSSC_MUTUAL3_KEY31,
#endif
#if ( MUTUAL3_NUM > 32 )
    CTSUSSC_MUTUAL3_KEY32,
#endif
#if ( MUTUAL3_NUM > 33 )
    CTSUSSC_MUTUAL3_KEY33,
#endif
#if ( MUTUAL3_NUM > 34 )
    CTSUSSC_MUTUAL3_KEY34,
#endif
#if ( MUTUAL3_NUM > 35 )
    CTSUSSC_MUTUAL3_KEY35,
#endif
#if ( MUTUAL3_NUM > 36 )
    CTSUSSC_MUTUAL3_KEY36,
#endif
#if ( MUTUAL3_NUM > 37 )
    CTSUSSC_MUTUAL3_KEY37,
#endif
#if ( MUTUAL3_NUM > 38 )
    CTSUSSC_MUTUAL3_KEY38,
#endif
#if ( MUTUAL3_NUM > 39 )
    CTSUSSC_MUTUAL3_KEY39,
#endif
#if ( MUTUAL3_NUM > 40 )
    CTSUSSC_MUTUAL3_KEY40,
#endif
#if ( MUTUAL3_NUM > 41 )
    CTSUSSC_MUTUAL3_KEY41,
#endif
#if ( MUTUAL3_NUM > 42 )
    CTSUSSC_MUTUAL3_KEY42,
#endif
#if ( MUTUAL3_NUM > 43 )
    CTSUSSC_MUTUAL3_KEY43,
#endif
#if ( MUTUAL3_NUM > 44 )
    CTSUSSC_MUTUAL3_KEY44,
#endif
#if ( MUTUAL3_NUM > 45 )
    CTSUSSC_MUTUAL3_KEY45,
#endif
#if ( MUTUAL3_NUM > 46 )
    CTSUSSC_MUTUAL3_KEY46,
#endif
#if ( MUTUAL3_NUM > 47 )
    CTSUSSC_MUTUAL3_KEY47,
#endif
#if ( MUTUAL3_NUM > 48 )
    CTSUSSC_MUTUAL3_KEY48,
#endif
#if ( MUTUAL3_NUM > 49 )
    CTSUSSC_MUTUAL3_KEY49,
#endif
#if ( MUTUAL3_NUM > 50 )
    CTSUSSC_MUTUAL3_KEY50,
#endif
#if ( MUTUAL3_NUM > 51 )
    CTSUSSC_MUTUAL3_KEY51,
#endif
#if ( MUTUAL3_NUM > 52 )
    CTSUSSC_MUTUAL3_KEY52,
#endif
#if ( MUTUAL3_NUM > 53 )
    CTSUSSC_MUTUAL3_KEY53,
#endif
#if ( MUTUAL3_NUM > 54 )
    CTSUSSC_MUTUAL3_KEY54,
#endif
#if ( MUTUAL3_NUM > 55 )
    CTSUSSC_MUTUAL3_KEY55,
#endif
#if ( MUTUAL3_NUM > 56 )
    CTSUSSC_MUTUAL3_KEY56,
#endif
#if ( MUTUAL3_NUM > 57 )
    CTSUSSC_MUTUAL3_KEY57,
#endif
#if ( MUTUAL3_NUM > 58 )
    CTSUSSC_MUTUAL3_KEY58,
#endif
#if ( MUTUAL3_NUM > 59 )
    CTSUSSC_MUTUAL3_KEY59,
#endif
#if ( MUTUAL3_NUM > 60 )
    CTSUSSC_MUTUAL3_KEY60,
#endif
#if ( MUTUAL3_NUM > 61 )
    CTSUSSC_MUTUAL3_KEY61,
#endif
#if ( MUTUAL3_NUM > 62 )
    CTSUSSC_MUTUAL3_KEY62,
#endif
#if ( MUTUAL3_NUM > 63 )
    CTSUSSC_MUTUAL3_KEY63,
#endif
};

const uint16_t g_mutual3_ctsuso0[] =
{
#if ( MUTUAL3_NUM > 0 )
    CTSUSO0_MUTUAL3_KEY00,
#endif
#if ( MUTUAL3_NUM > 1 )
    CTSUSO0_MUTUAL3_KEY01,
#endif
#if ( MUTUAL3_NUM > 2 )
    CTSUSO0_MUTUAL3_KEY02,
#endif
#if ( MUTUAL3_NUM > 3 )
    CTSUSO0_MUTUAL3_KEY03,
#endif
#if ( MUTUAL3_NUM > 4 )
    CTSUSO0_MUTUAL3_KEY04,
#endif
#if ( MUTUAL3_NUM > 5 )
    CTSUSO0_MUTUAL3_KEY05,
#endif
#if ( MUTUAL3_NUM > 6 )
    CTSUSO0_MUTUAL3_KEY06,
#endif
#if ( MUTUAL3_NUM > 7 )
    CTSUSO0_MUTUAL3_KEY07,
#endif
#if ( MUTUAL3_NUM > 8 )
    CTSUSO0_MUTUAL3_KEY08,
#endif
#if ( MUTUAL3_NUM > 9 )
    CTSUSO0_MUTUAL3_KEY09,
#endif
#if ( MUTUAL3_NUM > 10 )
    CTSUSO0_MUTUAL3_KEY10,
#endif
#if ( MUTUAL3_NUM > 11 )
    CTSUSO0_MUTUAL3_KEY11,
#endif
#if ( MUTUAL3_NUM > 12 )
    CTSUSO0_MUTUAL3_KEY12,
#endif
#if ( MUTUAL3_NUM > 13 )
    CTSUSO0_MUTUAL3_KEY13,
#endif
#if ( MUTUAL3_NUM > 14 )
    CTSUSO0_MUTUAL3_KEY14,
#endif
#if ( MUTUAL3_NUM > 15 )
    CTSUSO0_MUTUAL3_KEY15,
#endif
#if ( MUTUAL3_NUM > 16 )
    CTSUSO0_MUTUAL3_KEY16,
#endif
#if ( MUTUAL3_NUM > 17 )
    CTSUSO0_MUTUAL3_KEY17,
#endif
#if ( MUTUAL3_NUM > 18 )
    CTSUSO0_MUTUAL3_KEY18,
#endif
#if ( MUTUAL3_NUM > 19 )
    CTSUSO0_MUTUAL3_KEY19,
#endif
#if ( MUTUAL3_NUM > 20 )
    CTSUSO0_MUTUAL3_KEY20,
#endif
#if ( MUTUAL3_NUM > 21 )
    CTSUSO0_MUTUAL3_KEY21,
#endif
#if ( MUTUAL3_NUM > 22 )
    CTSUSO0_MUTUAL3_KEY22,
#endif
#if ( MUTUAL3_NUM > 23 )
    CTSUSO0_MUTUAL3_KEY23,
#endif
#if ( MUTUAL3_NUM > 24 )
    CTSUSO0_MUTUAL3_KEY24,
#endif
#if ( MUTUAL3_NUM > 25 )
    CTSUSO0_MUTUAL3_KEY25,
#endif
#if ( MUTUAL3_NUM > 26 )
    CTSUSO0_MUTUAL3_KEY26,
#endif
#if ( MUTUAL3_NUM > 27 )
    CTSUSO0_MUTUAL3_KEY27,
#endif
#if ( MUTUAL3_NUM > 28 )
    CTSUSO0_MUTUAL3_KEY28,
#endif
#if ( MUTUAL3_NUM > 29 )
    CTSUSO0_MUTUAL3_KEY29,
#endif
#if ( MUTUAL3_NUM > 30 )
    CTSUSO0_MUTUAL3_KEY30,
#endif
#if ( MUTUAL3_NUM > 31 )
    CTSUSO0_MUTUAL3_KEY31,
#endif
#if ( MUTUAL3_NUM > 32 )
    CTSUSO0_MUTUAL3_KEY32,
#endif
#if ( MUTUAL3_NUM > 33 )
    CTSUSO0_MUTUAL3_KEY33,
#endif
#if ( MUTUAL3_NUM > 34 )
    CTSUSO0_MUTUAL3_KEY34,
#endif
#if ( MUTUAL3_NUM > 35 )
    CTSUSO0_MUTUAL3_KEY35,
#endif
#if ( MUTUAL3_NUM > 36 )
    CTSUSO0_MUTUAL3_KEY36,
#endif
#if ( MUTUAL3_NUM > 37 )
    CTSUSO0_MUTUAL3_KEY37,
#endif
#if ( MUTUAL3_NUM > 38 )
    CTSUSO0_MUTUAL3_KEY38,
#endif
#if ( MUTUAL3_NUM > 39 )
    CTSUSO0_MUTUAL3_KEY39,
#endif
#if ( MUTUAL3_NUM > 40 )
    CTSUSO0_MUTUAL3_KEY40,
#endif
#if ( MUTUAL3_NUM > 41 )
    CTSUSO0_MUTUAL3_KEY41,
#endif
#if ( MUTUAL3_NUM > 42 )
    CTSUSO0_MUTUAL3_KEY42,
#endif
#if ( MUTUAL3_NUM > 43 )
    CTSUSO0_MUTUAL3_KEY43,
#endif
#if ( MUTUAL3_NUM > 44 )
    CTSUSO0_MUTUAL3_KEY44,
#endif
#if ( MUTUAL3_NUM > 45 )
    CTSUSO0_MUTUAL3_KEY45,
#endif
#if ( MUTUAL3_NUM > 46 )
    CTSUSO0_MUTUAL3_KEY46,
#endif
#if ( MUTUAL3_NUM > 47 )
    CTSUSO0_MUTUAL3_KEY47,
#endif
#if ( MUTUAL3_NUM > 48 )
    CTSUSO0_MUTUAL3_KEY48,
#endif
#if ( MUTUAL3_NUM > 49 )
    CTSUSO0_MUTUAL3_KEY49,
#endif
#if ( MUTUAL3_NUM > 50 )
    CTSUSO0_MUTUAL3_KEY50,
#endif
#if ( MUTUAL3_NUM > 51 )
    CTSUSO0_MUTUAL3_KEY51,
#endif
#if ( MUTUAL3_NUM > 52 )
    CTSUSO0_MUTUAL3_KEY52,
#endif
#if ( MUTUAL3_NUM > 53 )
    CTSUSO0_MUTUAL3_KEY53,
#endif
#if ( MUTUAL3_NUM > 54 )
    CTSUSO0_MUTUAL3_KEY54,
#endif
#if ( MUTUAL3_NUM > 55 )
    CTSUSO0_MUTUAL3_KEY55,
#endif
#if ( MUTUAL3_NUM > 56 )
    CTSUSO0_MUTUAL3_KEY56,
#endif
#if ( MUTUAL3_NUM > 57 )
    CTSUSO0_MUTUAL3_KEY57,
#endif
#if ( MUTUAL3_NUM > 58 )
    CTSUSO0_MUTUAL3_KEY58,
#endif
#if ( MUTUAL3_NUM > 59 )
    CTSUSO0_MUTUAL3_KEY59,
#endif
#if ( MUTUAL3_NUM > 60 )
    CTSUSO0_MUTUAL3_KEY60,
#endif
#if ( MUTUAL3_NUM > 61 )
    CTSUSO0_MUTUAL3_KEY61,
#endif
#if ( MUTUAL3_NUM > 62 )
    CTSUSO0_MUTUAL3_KEY62,
#endif
#if ( MUTUAL3_NUM > 63 )
    CTSUSO0_MUTUAL3_KEY63,
#endif
};

const uint16_t g_mutual3_ctsuso1[] =
{
#if ( MUTUAL3_NUM > 0 )
    CTSUSO1_MUTUAL3_KEY00,
#endif
#if ( MUTUAL3_NUM > 1 )
    CTSUSO1_MUTUAL3_KEY01,
#endif
#if ( MUTUAL3_NUM > 2 )
    CTSUSO1_MUTUAL3_KEY02,
#endif
#if ( MUTUAL3_NUM > 3 )
    CTSUSO1_MUTUAL3_KEY03,
#endif
#if ( MUTUAL3_NUM > 4 )
    CTSUSO1_MUTUAL3_KEY04,
#endif
#if ( MUTUAL3_NUM > 5 )
    CTSUSO1_MUTUAL3_KEY05,
#endif
#if ( MUTUAL3_NUM > 6 )
    CTSUSO1_MUTUAL3_KEY06,
#endif
#if ( MUTUAL3_NUM > 7 )
    CTSUSO1_MUTUAL3_KEY07,
#endif
#if ( MUTUAL3_NUM > 8 )
    CTSUSO1_MUTUAL3_KEY08,
#endif
#if ( MUTUAL3_NUM > 9 )
    CTSUSO1_MUTUAL3_KEY09,
#endif
#if ( MUTUAL3_NUM > 10 )
    CTSUSO1_MUTUAL3_KEY10,
#endif
#if ( MUTUAL3_NUM > 11 )
    CTSUSO1_MUTUAL3_KEY11,
#endif
#if ( MUTUAL3_NUM > 12 )
    CTSUSO1_MUTUAL3_KEY12,
#endif
#if ( MUTUAL3_NUM > 13 )
    CTSUSO1_MUTUAL3_KEY13,
#endif
#if ( MUTUAL3_NUM > 14 )
    CTSUSO1_MUTUAL3_KEY14,
#endif
#if ( MUTUAL3_NUM > 15 )
    CTSUSO1_MUTUAL3_KEY15,
#endif
#if ( MUTUAL3_NUM > 16 )
    CTSUSO1_MUTUAL3_KEY16,
#endif
#if ( MUTUAL3_NUM > 17 )
    CTSUSO1_MUTUAL3_KEY17,
#endif
#if ( MUTUAL3_NUM > 18 )
    CTSUSO1_MUTUAL3_KEY18,
#endif
#if ( MUTUAL3_NUM > 19 )
    CTSUSO1_MUTUAL3_KEY19,
#endif
#if ( MUTUAL3_NUM > 20 )
    CTSUSO1_MUTUAL3_KEY20,
#endif
#if ( MUTUAL3_NUM > 21 )
    CTSUSO1_MUTUAL3_KEY21,
#endif
#if ( MUTUAL3_NUM > 22 )
    CTSUSO1_MUTUAL3_KEY22,
#endif
#if ( MUTUAL3_NUM > 23 )
    CTSUSO1_MUTUAL3_KEY23,
#endif
#if ( MUTUAL3_NUM > 24 )
    CTSUSO1_MUTUAL3_KEY24,
#endif
#if ( MUTUAL3_NUM > 25 )
    CTSUSO1_MUTUAL3_KEY25,
#endif
#if ( MUTUAL3_NUM > 26 )
    CTSUSO1_MUTUAL3_KEY26,
#endif
#if ( MUTUAL3_NUM > 27 )
    CTSUSO1_MUTUAL3_KEY27,
#endif
#if ( MUTUAL3_NUM > 28 )
    CTSUSO1_MUTUAL3_KEY28,
#endif
#if ( MUTUAL3_NUM > 29 )
    CTSUSO1_MUTUAL3_KEY29,
#endif
#if ( MUTUAL3_NUM > 30 )
    CTSUSO1_MUTUAL3_KEY30,
#endif
#if ( MUTUAL3_NUM > 31 )
    CTSUSO1_MUTUAL3_KEY31,
#endif
#if ( MUTUAL3_NUM > 32 )
    CTSUSO1_MUTUAL3_KEY32,
#endif
#if ( MUTUAL3_NUM > 33 )
    CTSUSO1_MUTUAL3_KEY33,
#endif
#if ( MUTUAL3_NUM > 34 )
    CTSUSO1_MUTUAL3_KEY34,
#endif
#if ( MUTUAL3_NUM > 35 )
    CTSUSO1_MUTUAL3_KEY35,
#endif
#if ( MUTUAL3_NUM > 36 )
    CTSUSO1_MUTUAL3_KEY36,
#endif
#if ( MUTUAL3_NUM > 37 )
    CTSUSO1_MUTUAL3_KEY37,
#endif
#if ( MUTUAL3_NUM > 38 )
    CTSUSO1_MUTUAL3_KEY38,
#endif
#if ( MUTUAL3_NUM > 39 )
    CTSUSO1_MUTUAL3_KEY39,
#endif
#if ( MUTUAL3_NUM > 40 )
    CTSUSO1_MUTUAL3_KEY40,
#endif
#if ( MUTUAL3_NUM > 41 )
    CTSUSO1_MUTUAL3_KEY41,
#endif
#if ( MUTUAL3_NUM > 42 )
    CTSUSO1_MUTUAL3_KEY42,
#endif
#if ( MUTUAL3_NUM > 43 )
    CTSUSO1_MUTUAL3_KEY43,
#endif
#if ( MUTUAL3_NUM > 44 )
    CTSUSO1_MUTUAL3_KEY44,
#endif
#if ( MUTUAL3_NUM > 45 )
    CTSUSO1_MUTUAL3_KEY45,
#endif
#if ( MUTUAL3_NUM > 46 )
    CTSUSO1_MUTUAL3_KEY46,
#endif
#if ( MUTUAL3_NUM > 47 )
    CTSUSO1_MUTUAL3_KEY47,
#endif
#if ( MUTUAL3_NUM > 48 )
    CTSUSO1_MUTUAL3_KEY48,
#endif
#if ( MUTUAL3_NUM > 49 )
    CTSUSO1_MUTUAL3_KEY49,
#endif
#if ( MUTUAL3_NUM > 50 )
    CTSUSO1_MUTUAL3_KEY50,
#endif
#if ( MUTUAL3_NUM > 51 )
    CTSUSO1_MUTUAL3_KEY51,
#endif
#if ( MUTUAL3_NUM > 52 )
    CTSUSO1_MUTUAL3_KEY52,
#endif
#if ( MUTUAL3_NUM > 53 )
    CTSUSO1_MUTUAL3_KEY53,
#endif
#if ( MUTUAL3_NUM > 54 )
    CTSUSO1_MUTUAL3_KEY54,
#endif
#if ( MUTUAL3_NUM > 55 )
    CTSUSO1_MUTUAL3_KEY55,
#endif
#if ( MUTUAL3_NUM > 56 )
    CTSUSO1_MUTUAL3_KEY56,
#endif
#if ( MUTUAL3_NUM > 57 )
    CTSUSO1_MUTUAL3_KEY57,
#endif
#if ( MUTUAL3_NUM > 58 )
    CTSUSO1_MUTUAL3_KEY58,
#endif
#if ( MUTUAL3_NUM > 59 )
    CTSUSO1_MUTUAL3_KEY59,
#endif
#if ( MUTUAL3_NUM > 60 )
    CTSUSO1_MUTUAL3_KEY60,
#endif
#if ( MUTUAL3_NUM > 61 )
    CTSUSO1_MUTUAL3_KEY61,
#endif
#if ( MUTUAL3_NUM > 62 )
    CTSUSO1_MUTUAL3_KEY62,
#endif
#if ( MUTUAL3_NUM > 63 )
    CTSUSO1_MUTUAL3_KEY63,
#endif
};
#endif  // ( MUTUAL_METHOD_NUM > 3 )

/*=====    Mutual 4    DTC transmit data ========================================================*/
#if ( MUTUAL_METHOD_NUM > 4 )
const uint16_t g_mutual4_ctsussc[] =
{
#if ( MUTUAL4_NUM > 0 )
    CTSUSSC_MUTUAL4_KEY00,
#endif
#if ( MUTUAL4_NUM > 1 )
    CTSUSSC_MUTUAL4_KEY01,
#endif
#if ( MUTUAL4_NUM > 2 )
    CTSUSSC_MUTUAL4_KEY02,
#endif
#if ( MUTUAL4_NUM > 3 )
    CTSUSSC_MUTUAL4_KEY03,
#endif
#if ( MUTUAL4_NUM > 4 )
    CTSUSSC_MUTUAL4_KEY04,
#endif
#if ( MUTUAL4_NUM > 5 )
    CTSUSSC_MUTUAL4_KEY05,
#endif
#if ( MUTUAL4_NUM > 6 )
    CTSUSSC_MUTUAL4_KEY06,
#endif
#if ( MUTUAL4_NUM > 7 )
    CTSUSSC_MUTUAL4_KEY07,
#endif
#if ( MUTUAL4_NUM > 8 )
    CTSUSSC_MUTUAL4_KEY08,
#endif
#if ( MUTUAL4_NUM > 9 )
    CTSUSSC_MUTUAL4_KEY09,
#endif
#if ( MUTUAL4_NUM > 10 )
    CTSUSSC_MUTUAL4_KEY10,
#endif
#if ( MUTUAL4_NUM > 11 )
    CTSUSSC_MUTUAL4_KEY11,
#endif
#if ( MUTUAL4_NUM > 12 )
    CTSUSSC_MUTUAL4_KEY12,
#endif
#if ( MUTUAL4_NUM > 13 )
    CTSUSSC_MUTUAL4_KEY13,
#endif
#if ( MUTUAL4_NUM > 14 )
    CTSUSSC_MUTUAL4_KEY14,
#endif
#if ( MUTUAL4_NUM > 15 )
    CTSUSSC_MUTUAL4_KEY15,
#endif
#if ( MUTUAL4_NUM > 16 )
    CTSUSSC_MUTUAL4_KEY16,
#endif
#if ( MUTUAL4_NUM > 17 )
    CTSUSSC_MUTUAL4_KEY17,
#endif
#if ( MUTUAL4_NUM > 18 )
    CTSUSSC_MUTUAL4_KEY18,
#endif
#if ( MUTUAL4_NUM > 19 )
    CTSUSSC_MUTUAL4_KEY19,
#endif
#if ( MUTUAL4_NUM > 20 )
    CTSUSSC_MUTUAL4_KEY20,
#endif
#if ( MUTUAL4_NUM > 21 )
    CTSUSSC_MUTUAL4_KEY21,
#endif
#if ( MUTUAL4_NUM > 22 )
    CTSUSSC_MUTUAL4_KEY22,
#endif
#if ( MUTUAL4_NUM > 23 )
    CTSUSSC_MUTUAL4_KEY23,
#endif
#if ( MUTUAL4_NUM > 24 )
    CTSUSSC_MUTUAL4_KEY24,
#endif
#if ( MUTUAL4_NUM > 25 )
    CTSUSSC_MUTUAL4_KEY25,
#endif
#if ( MUTUAL4_NUM > 26 )
    CTSUSSC_MUTUAL4_KEY26,
#endif
#if ( MUTUAL4_NUM > 27 )
    CTSUSSC_MUTUAL4_KEY27,
#endif
#if ( MUTUAL4_NUM > 28 )
    CTSUSSC_MUTUAL4_KEY28,
#endif
#if ( MUTUAL4_NUM > 29 )
    CTSUSSC_MUTUAL4_KEY29,
#endif
#if ( MUTUAL4_NUM > 30 )
    CTSUSSC_MUTUAL4_KEY30,
#endif
#if ( MUTUAL4_NUM > 31 )
    CTSUSSC_MUTUAL4_KEY31,
#endif
#if ( MUTUAL4_NUM > 32 )
    CTSUSSC_MUTUAL4_KEY32,
#endif
#if ( MUTUAL4_NUM > 33 )
    CTSUSSC_MUTUAL4_KEY33,
#endif
#if ( MUTUAL4_NUM > 34 )
    CTSUSSC_MUTUAL4_KEY34,
#endif
#if ( MUTUAL4_NUM > 35 )
    CTSUSSC_MUTUAL4_KEY35,
#endif
#if ( MUTUAL4_NUM > 36 )
    CTSUSSC_MUTUAL4_KEY36,
#endif
#if ( MUTUAL4_NUM > 37 )
    CTSUSSC_MUTUAL4_KEY37,
#endif
#if ( MUTUAL4_NUM > 38 )
    CTSUSSC_MUTUAL4_KEY38,
#endif
#if ( MUTUAL4_NUM > 39 )
    CTSUSSC_MUTUAL4_KEY39,
#endif
#if ( MUTUAL4_NUM > 40 )
    CTSUSSC_MUTUAL4_KEY40,
#endif
#if ( MUTUAL4_NUM > 41 )
    CTSUSSC_MUTUAL4_KEY41,
#endif
#if ( MUTUAL4_NUM > 42 )
    CTSUSSC_MUTUAL4_KEY42,
#endif
#if ( MUTUAL4_NUM > 43 )
    CTSUSSC_MUTUAL4_KEY43,
#endif
#if ( MUTUAL4_NUM > 44 )
    CTSUSSC_MUTUAL4_KEY44,
#endif
#if ( MUTUAL4_NUM > 45 )
    CTSUSSC_MUTUAL4_KEY45,
#endif
#if ( MUTUAL4_NUM > 46 )
    CTSUSSC_MUTUAL4_KEY46,
#endif
#if ( MUTUAL4_NUM > 47 )
    CTSUSSC_MUTUAL4_KEY47,
#endif
#if ( MUTUAL4_NUM > 48 )
    CTSUSSC_MUTUAL4_KEY48,
#endif
#if ( MUTUAL4_NUM > 49 )
    CTSUSSC_MUTUAL4_KEY49,
#endif
#if ( MUTUAL4_NUM > 50 )
    CTSUSSC_MUTUAL4_KEY50,
#endif
#if ( MUTUAL4_NUM > 51 )
    CTSUSSC_MUTUAL4_KEY51,
#endif
#if ( MUTUAL4_NUM > 52 )
    CTSUSSC_MUTUAL4_KEY52,
#endif
#if ( MUTUAL4_NUM > 53 )
    CTSUSSC_MUTUAL4_KEY53,
#endif
#if ( MUTUAL4_NUM > 54 )
    CTSUSSC_MUTUAL4_KEY54,
#endif
#if ( MUTUAL4_NUM > 55 )
    CTSUSSC_MUTUAL4_KEY55,
#endif
#if ( MUTUAL4_NUM > 56 )
    CTSUSSC_MUTUAL4_KEY56,
#endif
#if ( MUTUAL4_NUM > 57 )
    CTSUSSC_MUTUAL4_KEY57,
#endif
#if ( MUTUAL4_NUM > 58 )
    CTSUSSC_MUTUAL4_KEY58,
#endif
#if ( MUTUAL4_NUM > 59 )
    CTSUSSC_MUTUAL4_KEY59,
#endif
#if ( MUTUAL4_NUM > 60 )
    CTSUSSC_MUTUAL4_KEY60,
#endif
#if ( MUTUAL4_NUM > 61 )
    CTSUSSC_MUTUAL4_KEY61,
#endif
#if ( MUTUAL4_NUM > 62 )
    CTSUSSC_MUTUAL4_KEY62,
#endif
#if ( MUTUAL4_NUM > 63 )
    CTSUSSC_MUTUAL4_KEY63,
#endif
};

const uint16_t g_mutual4_ctsuso0[] =
{
#if ( MUTUAL4_NUM > 0 )
    CTSUSO0_MUTUAL4_KEY00,
#endif
#if ( MUTUAL4_NUM > 1 )
    CTSUSO0_MUTUAL4_KEY01,
#endif
#if ( MUTUAL4_NUM > 2 )
    CTSUSO0_MUTUAL4_KEY02,
#endif
#if ( MUTUAL4_NUM > 3 )
    CTSUSO0_MUTUAL4_KEY03,
#endif
#if ( MUTUAL4_NUM > 4 )
    CTSUSO0_MUTUAL4_KEY04,
#endif
#if ( MUTUAL4_NUM > 5 )
    CTSUSO0_MUTUAL4_KEY05,
#endif
#if ( MUTUAL4_NUM > 6 )
    CTSUSO0_MUTUAL4_KEY06,
#endif
#if ( MUTUAL4_NUM > 7 )
    CTSUSO0_MUTUAL4_KEY07,
#endif
#if ( MUTUAL4_NUM > 8 )
    CTSUSO0_MUTUAL4_KEY08,
#endif
#if ( MUTUAL4_NUM > 9 )
    CTSUSO0_MUTUAL4_KEY09,
#endif
#if ( MUTUAL4_NUM > 10 )
    CTSUSO0_MUTUAL4_KEY10,
#endif
#if ( MUTUAL4_NUM > 11 )
    CTSUSO0_MUTUAL4_KEY11,
#endif
#if ( MUTUAL4_NUM > 12 )
    CTSUSO0_MUTUAL4_KEY12,
#endif
#if ( MUTUAL4_NUM > 13 )
    CTSUSO0_MUTUAL4_KEY13,
#endif
#if ( MUTUAL4_NUM > 14 )
    CTSUSO0_MUTUAL4_KEY14,
#endif
#if ( MUTUAL4_NUM > 15 )
    CTSUSO0_MUTUAL4_KEY15,
#endif
#if ( MUTUAL4_NUM > 16 )
    CTSUSO0_MUTUAL4_KEY16,
#endif
#if ( MUTUAL4_NUM > 17 )
    CTSUSO0_MUTUAL4_KEY17,
#endif
#if ( MUTUAL4_NUM > 18 )
    CTSUSO0_MUTUAL4_KEY18,
#endif
#if ( MUTUAL4_NUM > 19 )
    CTSUSO0_MUTUAL4_KEY19,
#endif
#if ( MUTUAL4_NUM > 20 )
    CTSUSO0_MUTUAL4_KEY20,
#endif
#if ( MUTUAL4_NUM > 21 )
    CTSUSO0_MUTUAL4_KEY21,
#endif
#if ( MUTUAL4_NUM > 22 )
    CTSUSO0_MUTUAL4_KEY22,
#endif
#if ( MUTUAL4_NUM > 23 )
    CTSUSO0_MUTUAL4_KEY23,
#endif
#if ( MUTUAL4_NUM > 24 )
    CTSUSO0_MUTUAL4_KEY24,
#endif
#if ( MUTUAL4_NUM > 25 )
    CTSUSO0_MUTUAL4_KEY25,
#endif
#if ( MUTUAL4_NUM > 26 )
    CTSUSO0_MUTUAL4_KEY26,
#endif
#if ( MUTUAL4_NUM > 27 )
    CTSUSO0_MUTUAL4_KEY27,
#endif
#if ( MUTUAL4_NUM > 28 )
    CTSUSO0_MUTUAL4_KEY28,
#endif
#if ( MUTUAL4_NUM > 29 )
    CTSUSO0_MUTUAL4_KEY29,
#endif
#if ( MUTUAL4_NUM > 30 )
    CTSUSO0_MUTUAL4_KEY30,
#endif
#if ( MUTUAL4_NUM > 31 )
    CTSUSO0_MUTUAL4_KEY31,
#endif
#if ( MUTUAL4_NUM > 32 )
    CTSUSO0_MUTUAL4_KEY32,
#endif
#if ( MUTUAL4_NUM > 33 )
    CTSUSO0_MUTUAL4_KEY33,
#endif
#if ( MUTUAL4_NUM > 34 )
    CTSUSO0_MUTUAL4_KEY34,
#endif
#if ( MUTUAL4_NUM > 35 )
    CTSUSO0_MUTUAL4_KEY35,
#endif
#if ( MUTUAL4_NUM > 36 )
    CTSUSO0_MUTUAL4_KEY36,
#endif
#if ( MUTUAL4_NUM > 37 )
    CTSUSO0_MUTUAL4_KEY37,
#endif
#if ( MUTUAL4_NUM > 38 )
    CTSUSO0_MUTUAL4_KEY38,
#endif
#if ( MUTUAL4_NUM > 39 )
    CTSUSO0_MUTUAL4_KEY39,
#endif
#if ( MUTUAL4_NUM > 40 )
    CTSUSO0_MUTUAL4_KEY40,
#endif
#if ( MUTUAL4_NUM > 41 )
    CTSUSO0_MUTUAL4_KEY41,
#endif
#if ( MUTUAL4_NUM > 42 )
    CTSUSO0_MUTUAL4_KEY42,
#endif
#if ( MUTUAL4_NUM > 43 )
    CTSUSO0_MUTUAL4_KEY43,
#endif
#if ( MUTUAL4_NUM > 44 )
    CTSUSO0_MUTUAL4_KEY44,
#endif
#if ( MUTUAL4_NUM > 45 )
    CTSUSO0_MUTUAL4_KEY45,
#endif
#if ( MUTUAL4_NUM > 46 )
    CTSUSO0_MUTUAL4_KEY46,
#endif
#if ( MUTUAL4_NUM > 47 )
    CTSUSO0_MUTUAL4_KEY47,
#endif
#if ( MUTUAL4_NUM > 48 )
    CTSUSO0_MUTUAL4_KEY48,
#endif
#if ( MUTUAL4_NUM > 49 )
    CTSUSO0_MUTUAL4_KEY49,
#endif
#if ( MUTUAL4_NUM > 50 )
    CTSUSO0_MUTUAL4_KEY50,
#endif
#if ( MUTUAL4_NUM > 51 )
    CTSUSO0_MUTUAL4_KEY51,
#endif
#if ( MUTUAL4_NUM > 52 )
    CTSUSO0_MUTUAL4_KEY52,
#endif
#if ( MUTUAL4_NUM > 53 )
    CTSUSO0_MUTUAL4_KEY53,
#endif
#if ( MUTUAL4_NUM > 54 )
    CTSUSO0_MUTUAL4_KEY54,
#endif
#if ( MUTUAL4_NUM > 55 )
    CTSUSO0_MUTUAL4_KEY55,
#endif
#if ( MUTUAL4_NUM > 56 )
    CTSUSO0_MUTUAL4_KEY56,
#endif
#if ( MUTUAL4_NUM > 57 )
    CTSUSO0_MUTUAL4_KEY57,
#endif
#if ( MUTUAL4_NUM > 58 )
    CTSUSO0_MUTUAL4_KEY58,
#endif
#if ( MUTUAL4_NUM > 59 )
    CTSUSO0_MUTUAL4_KEY59,
#endif
#if ( MUTUAL4_NUM > 60 )
    CTSUSO0_MUTUAL4_KEY60,
#endif
#if ( MUTUAL4_NUM > 61 )
    CTSUSO0_MUTUAL4_KEY61,
#endif
#if ( MUTUAL4_NUM > 62 )
    CTSUSO0_MUTUAL4_KEY62,
#endif
#if ( MUTUAL4_NUM > 63 )
    CTSUSO0_MUTUAL4_KEY63,
#endif
};

const uint16_t g_mutual4_ctsuso1[] =
{
#if ( MUTUAL4_NUM > 0 )
    CTSUSO1_MUTUAL4_KEY00,
#endif
#if ( MUTUAL4_NUM > 1 )
    CTSUSO1_MUTUAL4_KEY01,
#endif
#if ( MUTUAL4_NUM > 2 )
    CTSUSO1_MUTUAL4_KEY02,
#endif
#if ( MUTUAL4_NUM > 3 )
    CTSUSO1_MUTUAL4_KEY03,
#endif
#if ( MUTUAL4_NUM > 4 )
    CTSUSO1_MUTUAL4_KEY04,
#endif
#if ( MUTUAL4_NUM > 5 )
    CTSUSO1_MUTUAL4_KEY05,
#endif
#if ( MUTUAL4_NUM > 6 )
    CTSUSO1_MUTUAL4_KEY06,
#endif
#if ( MUTUAL4_NUM > 7 )
    CTSUSO1_MUTUAL4_KEY07,
#endif
#if ( MUTUAL4_NUM > 8 )
    CTSUSO1_MUTUAL4_KEY08,
#endif
#if ( MUTUAL4_NUM > 9 )
    CTSUSO1_MUTUAL4_KEY09,
#endif
#if ( MUTUAL4_NUM > 10 )
    CTSUSO1_MUTUAL4_KEY10,
#endif
#if ( MUTUAL4_NUM > 11 )
    CTSUSO1_MUTUAL4_KEY11,
#endif
#if ( MUTUAL4_NUM > 12 )
    CTSUSO1_MUTUAL4_KEY12,
#endif
#if ( MUTUAL4_NUM > 13 )
    CTSUSO1_MUTUAL4_KEY13,
#endif
#if ( MUTUAL4_NUM > 14 )
    CTSUSO1_MUTUAL4_KEY14,
#endif
#if ( MUTUAL4_NUM > 15 )
    CTSUSO1_MUTUAL4_KEY15,
#endif
#if ( MUTUAL4_NUM > 16 )
    CTSUSO1_MUTUAL4_KEY16,
#endif
#if ( MUTUAL4_NUM > 17 )
    CTSUSO1_MUTUAL4_KEY17,
#endif
#if ( MUTUAL4_NUM > 18 )
    CTSUSO1_MUTUAL4_KEY18,
#endif
#if ( MUTUAL4_NUM > 19 )
    CTSUSO1_MUTUAL4_KEY19,
#endif
#if ( MUTUAL4_NUM > 20 )
    CTSUSO1_MUTUAL4_KEY20,
#endif
#if ( MUTUAL4_NUM > 21 )
    CTSUSO1_MUTUAL4_KEY21,
#endif
#if ( MUTUAL4_NUM > 22 )
    CTSUSO1_MUTUAL4_KEY22,
#endif
#if ( MUTUAL4_NUM > 23 )
    CTSUSO1_MUTUAL4_KEY23,
#endif
#if ( MUTUAL4_NUM > 24 )
    CTSUSO1_MUTUAL4_KEY24,
#endif
#if ( MUTUAL4_NUM > 25 )
    CTSUSO1_MUTUAL4_KEY25,
#endif
#if ( MUTUAL4_NUM > 26 )
    CTSUSO1_MUTUAL4_KEY26,
#endif
#if ( MUTUAL4_NUM > 27 )
    CTSUSO1_MUTUAL4_KEY27,
#endif
#if ( MUTUAL4_NUM > 28 )
    CTSUSO1_MUTUAL4_KEY28,
#endif
#if ( MUTUAL4_NUM > 29 )
    CTSUSO1_MUTUAL4_KEY29,
#endif
#if ( MUTUAL4_NUM > 30 )
    CTSUSO1_MUTUAL4_KEY30,
#endif
#if ( MUTUAL4_NUM > 31 )
    CTSUSO1_MUTUAL4_KEY31,
#endif
#if ( MUTUAL4_NUM > 32 )
    CTSUSO1_MUTUAL4_KEY32,
#endif
#if ( MUTUAL4_NUM > 33 )
    CTSUSO1_MUTUAL4_KEY33,
#endif
#if ( MUTUAL4_NUM > 34 )
    CTSUSO1_MUTUAL4_KEY34,
#endif
#if ( MUTUAL4_NUM > 35 )
    CTSUSO1_MUTUAL4_KEY35,
#endif
#if ( MUTUAL4_NUM > 36 )
    CTSUSO1_MUTUAL4_KEY36,
#endif
#if ( MUTUAL4_NUM > 37 )
    CTSUSO1_MUTUAL4_KEY37,
#endif
#if ( MUTUAL4_NUM > 38 )
    CTSUSO1_MUTUAL4_KEY38,
#endif
#if ( MUTUAL4_NUM > 39 )
    CTSUSO1_MUTUAL4_KEY39,
#endif
#if ( MUTUAL4_NUM > 40 )
    CTSUSO1_MUTUAL4_KEY40,
#endif
#if ( MUTUAL4_NUM > 41 )
    CTSUSO1_MUTUAL4_KEY41,
#endif
#if ( MUTUAL4_NUM > 42 )
    CTSUSO1_MUTUAL4_KEY42,
#endif
#if ( MUTUAL4_NUM > 43 )
    CTSUSO1_MUTUAL4_KEY43,
#endif
#if ( MUTUAL4_NUM > 44 )
    CTSUSO1_MUTUAL4_KEY44,
#endif
#if ( MUTUAL4_NUM > 45 )
    CTSUSO1_MUTUAL4_KEY45,
#endif
#if ( MUTUAL4_NUM > 46 )
    CTSUSO1_MUTUAL4_KEY46,
#endif
#if ( MUTUAL4_NUM > 47 )
    CTSUSO1_MUTUAL4_KEY47,
#endif
#if ( MUTUAL4_NUM > 48 )
    CTSUSO1_MUTUAL4_KEY48,
#endif
#if ( MUTUAL4_NUM > 49 )
    CTSUSO1_MUTUAL4_KEY49,
#endif
#if ( MUTUAL4_NUM > 50 )
    CTSUSO1_MUTUAL4_KEY50,
#endif
#if ( MUTUAL4_NUM > 51 )
    CTSUSO1_MUTUAL4_KEY51,
#endif
#if ( MUTUAL4_NUM > 52 )
    CTSUSO1_MUTUAL4_KEY52,
#endif
#if ( MUTUAL4_NUM > 53 )
    CTSUSO1_MUTUAL4_KEY53,
#endif
#if ( MUTUAL4_NUM > 54 )
    CTSUSO1_MUTUAL4_KEY54,
#endif
#if ( MUTUAL4_NUM > 55 )
    CTSUSO1_MUTUAL4_KEY55,
#endif
#if ( MUTUAL4_NUM > 56 )
    CTSUSO1_MUTUAL4_KEY56,
#endif
#if ( MUTUAL4_NUM > 57 )
    CTSUSO1_MUTUAL4_KEY57,
#endif
#if ( MUTUAL4_NUM > 58 )
    CTSUSO1_MUTUAL4_KEY58,
#endif
#if ( MUTUAL4_NUM > 59 )
    CTSUSO1_MUTUAL4_KEY59,
#endif
#if ( MUTUAL4_NUM > 60 )
    CTSUSO1_MUTUAL4_KEY60,
#endif
#if ( MUTUAL4_NUM > 61 )
    CTSUSO1_MUTUAL4_KEY61,
#endif
#if ( MUTUAL4_NUM > 62 )
    CTSUSO1_MUTUAL4_KEY62,
#endif
#if ( MUTUAL4_NUM > 63 )
    CTSUSO1_MUTUAL4_KEY63,
#endif
};
#endif  // ( MUTUAL_METHOD_NUM > 4 )

/*=====    Mutual 5    DTC transmit data ========================================================*/
#if ( MUTUAL_METHOD_NUM > 5 )
const uint16_t g_mutual5_ctsussc[] =
{
#if ( MUTUAL5_NUM > 0 )
    CTSUSSC_MUTUAL5_KEY00,
#endif
#if ( MUTUAL5_NUM > 1 )
    CTSUSSC_MUTUAL5_KEY01,
#endif
#if ( MUTUAL5_NUM > 2 )
    CTSUSSC_MUTUAL5_KEY02,
#endif
#if ( MUTUAL5_NUM > 3 )
    CTSUSSC_MUTUAL5_KEY03,
#endif
#if ( MUTUAL5_NUM > 4 )
    CTSUSSC_MUTUAL5_KEY04,
#endif
#if ( MUTUAL5_NUM > 5 )
    CTSUSSC_MUTUAL5_KEY05,
#endif
#if ( MUTUAL5_NUM > 6 )
    CTSUSSC_MUTUAL5_KEY06,
#endif
#if ( MUTUAL5_NUM > 7 )
    CTSUSSC_MUTUAL5_KEY07,
#endif
#if ( MUTUAL5_NUM > 8 )
    CTSUSSC_MUTUAL5_KEY08,
#endif
#if ( MUTUAL5_NUM > 9 )
    CTSUSSC_MUTUAL5_KEY09,
#endif
#if ( MUTUAL5_NUM > 10 )
    CTSUSSC_MUTUAL5_KEY10,
#endif
#if ( MUTUAL5_NUM > 11 )
    CTSUSSC_MUTUAL5_KEY11,
#endif
#if ( MUTUAL5_NUM > 12 )
    CTSUSSC_MUTUAL5_KEY12,
#endif
#if ( MUTUAL5_NUM > 13 )
    CTSUSSC_MUTUAL5_KEY13,
#endif
#if ( MUTUAL5_NUM > 14 )
    CTSUSSC_MUTUAL5_KEY14,
#endif
#if ( MUTUAL5_NUM > 15 )
    CTSUSSC_MUTUAL5_KEY15,
#endif
#if ( MUTUAL5_NUM > 16 )
    CTSUSSC_MUTUAL5_KEY16,
#endif
#if ( MUTUAL5_NUM > 17 )
    CTSUSSC_MUTUAL5_KEY17,
#endif
#if ( MUTUAL5_NUM > 18 )
    CTSUSSC_MUTUAL5_KEY18,
#endif
#if ( MUTUAL5_NUM > 19 )
    CTSUSSC_MUTUAL5_KEY19,
#endif
#if ( MUTUAL5_NUM > 20 )
    CTSUSSC_MUTUAL5_KEY20,
#endif
#if ( MUTUAL5_NUM > 21 )
    CTSUSSC_MUTUAL5_KEY21,
#endif
#if ( MUTUAL5_NUM > 22 )
    CTSUSSC_MUTUAL5_KEY22,
#endif
#if ( MUTUAL5_NUM > 23 )
    CTSUSSC_MUTUAL5_KEY23,
#endif
#if ( MUTUAL5_NUM > 24 )
    CTSUSSC_MUTUAL5_KEY24,
#endif
#if ( MUTUAL5_NUM > 25 )
    CTSUSSC_MUTUAL5_KEY25,
#endif
#if ( MUTUAL5_NUM > 26 )
    CTSUSSC_MUTUAL5_KEY26,
#endif
#if ( MUTUAL5_NUM > 27 )
    CTSUSSC_MUTUAL5_KEY27,
#endif
#if ( MUTUAL5_NUM > 28 )
    CTSUSSC_MUTUAL5_KEY28,
#endif
#if ( MUTUAL5_NUM > 29 )
    CTSUSSC_MUTUAL5_KEY29,
#endif
#if ( MUTUAL5_NUM > 30 )
    CTSUSSC_MUTUAL5_KEY30,
#endif
#if ( MUTUAL5_NUM > 31 )
    CTSUSSC_MUTUAL5_KEY31,
#endif
#if ( MUTUAL5_NUM > 32 )
    CTSUSSC_MUTUAL5_KEY32,
#endif
#if ( MUTUAL5_NUM > 33 )
    CTSUSSC_MUTUAL5_KEY33,
#endif
#if ( MUTUAL5_NUM > 34 )
    CTSUSSC_MUTUAL5_KEY34,
#endif
#if ( MUTUAL5_NUM > 35 )
    CTSUSSC_MUTUAL5_KEY35,
#endif
#if ( MUTUAL5_NUM > 36 )
    CTSUSSC_MUTUAL5_KEY36,
#endif
#if ( MUTUAL5_NUM > 37 )
    CTSUSSC_MUTUAL5_KEY37,
#endif
#if ( MUTUAL5_NUM > 38 )
    CTSUSSC_MUTUAL5_KEY38,
#endif
#if ( MUTUAL5_NUM > 39 )
    CTSUSSC_MUTUAL5_KEY39,
#endif
#if ( MUTUAL5_NUM > 40 )
    CTSUSSC_MUTUAL5_KEY40,
#endif
#if ( MUTUAL5_NUM > 41 )
    CTSUSSC_MUTUAL5_KEY41,
#endif
#if ( MUTUAL5_NUM > 42 )
    CTSUSSC_MUTUAL5_KEY42,
#endif
#if ( MUTUAL5_NUM > 43 )
    CTSUSSC_MUTUAL5_KEY43,
#endif
#if ( MUTUAL5_NUM > 44 )
    CTSUSSC_MUTUAL5_KEY44,
#endif
#if ( MUTUAL5_NUM > 45 )
    CTSUSSC_MUTUAL5_KEY45,
#endif
#if ( MUTUAL5_NUM > 46 )
    CTSUSSC_MUTUAL5_KEY46,
#endif
#if ( MUTUAL5_NUM > 47 )
    CTSUSSC_MUTUAL5_KEY47,
#endif
#if ( MUTUAL5_NUM > 48 )
    CTSUSSC_MUTUAL5_KEY48,
#endif
#if ( MUTUAL5_NUM > 49 )
    CTSUSSC_MUTUAL5_KEY49,
#endif
#if ( MUTUAL5_NUM > 50 )
    CTSUSSC_MUTUAL5_KEY50,
#endif
#if ( MUTUAL5_NUM > 51 )
    CTSUSSC_MUTUAL5_KEY51,
#endif
#if ( MUTUAL5_NUM > 52 )
    CTSUSSC_MUTUAL5_KEY52,
#endif
#if ( MUTUAL5_NUM > 53 )
    CTSUSSC_MUTUAL5_KEY53,
#endif
#if ( MUTUAL5_NUM > 54 )
    CTSUSSC_MUTUAL5_KEY54,
#endif
#if ( MUTUAL5_NUM > 55 )
    CTSUSSC_MUTUAL5_KEY55,
#endif
#if ( MUTUAL5_NUM > 56 )
    CTSUSSC_MUTUAL5_KEY56,
#endif
#if ( MUTUAL5_NUM > 57 )
    CTSUSSC_MUTUAL5_KEY57,
#endif
#if ( MUTUAL5_NUM > 58 )
    CTSUSSC_MUTUAL5_KEY58,
#endif
#if ( MUTUAL5_NUM > 59 )
    CTSUSSC_MUTUAL5_KEY59,
#endif
#if ( MUTUAL5_NUM > 60 )
    CTSUSSC_MUTUAL5_KEY60,
#endif
#if ( MUTUAL5_NUM > 61 )
    CTSUSSC_MUTUAL5_KEY61,
#endif
#if ( MUTUAL5_NUM > 62 )
    CTSUSSC_MUTUAL5_KEY62,
#endif
#if ( MUTUAL5_NUM > 63 )
    CTSUSSC_MUTUAL5_KEY63,
#endif
};

const uint16_t g_mutual5_ctsuso0[] =
{
#if ( MUTUAL5_NUM > 0 )
    CTSUSO0_MUTUAL5_KEY00,
#endif
#if ( MUTUAL5_NUM > 1 )
    CTSUSO0_MUTUAL5_KEY01,
#endif
#if ( MUTUAL5_NUM > 2 )
    CTSUSO0_MUTUAL5_KEY02,
#endif
#if ( MUTUAL5_NUM > 3 )
    CTSUSO0_MUTUAL5_KEY03,
#endif
#if ( MUTUAL5_NUM > 4 )
    CTSUSO0_MUTUAL5_KEY04,
#endif
#if ( MUTUAL5_NUM > 5 )
    CTSUSO0_MUTUAL5_KEY05,
#endif
#if ( MUTUAL5_NUM > 6 )
    CTSUSO0_MUTUAL5_KEY06,
#endif
#if ( MUTUAL5_NUM > 7 )
    CTSUSO0_MUTUAL5_KEY07,
#endif
#if ( MUTUAL5_NUM > 8 )
    CTSUSO0_MUTUAL5_KEY08,
#endif
#if ( MUTUAL5_NUM > 9 )
    CTSUSO0_MUTUAL5_KEY09,
#endif
#if ( MUTUAL5_NUM > 10 )
    CTSUSO0_MUTUAL5_KEY10,
#endif
#if ( MUTUAL5_NUM > 11 )
    CTSUSO0_MUTUAL5_KEY11,
#endif
#if ( MUTUAL5_NUM > 12 )
    CTSUSO0_MUTUAL5_KEY12,
#endif
#if ( MUTUAL5_NUM > 13 )
    CTSUSO0_MUTUAL5_KEY13,
#endif
#if ( MUTUAL5_NUM > 14 )
    CTSUSO0_MUTUAL5_KEY14,
#endif
#if ( MUTUAL5_NUM > 15 )
    CTSUSO0_MUTUAL5_KEY15,
#endif
#if ( MUTUAL5_NUM > 16 )
    CTSUSO0_MUTUAL5_KEY16,
#endif
#if ( MUTUAL5_NUM > 17 )
    CTSUSO0_MUTUAL5_KEY17,
#endif
#if ( MUTUAL5_NUM > 18 )
    CTSUSO0_MUTUAL5_KEY18,
#endif
#if ( MUTUAL5_NUM > 19 )
    CTSUSO0_MUTUAL5_KEY19,
#endif
#if ( MUTUAL5_NUM > 20 )
    CTSUSO0_MUTUAL5_KEY20,
#endif
#if ( MUTUAL5_NUM > 21 )
    CTSUSO0_MUTUAL5_KEY21,
#endif
#if ( MUTUAL5_NUM > 22 )
    CTSUSO0_MUTUAL5_KEY22,
#endif
#if ( MUTUAL5_NUM > 23 )
    CTSUSO0_MUTUAL5_KEY23,
#endif
#if ( MUTUAL5_NUM > 24 )
    CTSUSO0_MUTUAL5_KEY24,
#endif
#if ( MUTUAL5_NUM > 25 )
    CTSUSO0_MUTUAL5_KEY25,
#endif
#if ( MUTUAL5_NUM > 26 )
    CTSUSO0_MUTUAL5_KEY26,
#endif
#if ( MUTUAL5_NUM > 27 )
    CTSUSO0_MUTUAL5_KEY27,
#endif
#if ( MUTUAL5_NUM > 28 )
    CTSUSO0_MUTUAL5_KEY28,
#endif
#if ( MUTUAL5_NUM > 29 )
    CTSUSO0_MUTUAL5_KEY29,
#endif
#if ( MUTUAL5_NUM > 30 )
    CTSUSO0_MUTUAL5_KEY30,
#endif
#if ( MUTUAL5_NUM > 31 )
    CTSUSO0_MUTUAL5_KEY31,
#endif
#if ( MUTUAL5_NUM > 32 )
    CTSUSO0_MUTUAL5_KEY32,
#endif
#if ( MUTUAL5_NUM > 33 )
    CTSUSO0_MUTUAL5_KEY33,
#endif
#if ( MUTUAL5_NUM > 34 )
    CTSUSO0_MUTUAL5_KEY34,
#endif
#if ( MUTUAL5_NUM > 35 )
    CTSUSO0_MUTUAL5_KEY35,
#endif
#if ( MUTUAL5_NUM > 36 )
    CTSUSO0_MUTUAL5_KEY36,
#endif
#if ( MUTUAL5_NUM > 37 )
    CTSUSO0_MUTUAL5_KEY37,
#endif
#if ( MUTUAL5_NUM > 38 )
    CTSUSO0_MUTUAL5_KEY38,
#endif
#if ( MUTUAL5_NUM > 39 )
    CTSUSO0_MUTUAL5_KEY39,
#endif
#if ( MUTUAL5_NUM > 40 )
    CTSUSO0_MUTUAL5_KEY40,
#endif
#if ( MUTUAL5_NUM > 41 )
    CTSUSO0_MUTUAL5_KEY41,
#endif
#if ( MUTUAL5_NUM > 42 )
    CTSUSO0_MUTUAL5_KEY42,
#endif
#if ( MUTUAL5_NUM > 43 )
    CTSUSO0_MUTUAL5_KEY43,
#endif
#if ( MUTUAL5_NUM > 44 )
    CTSUSO0_MUTUAL5_KEY44,
#endif
#if ( MUTUAL5_NUM > 45 )
    CTSUSO0_MUTUAL5_KEY45,
#endif
#if ( MUTUAL5_NUM > 46 )
    CTSUSO0_MUTUAL5_KEY46,
#endif
#if ( MUTUAL5_NUM > 47 )
    CTSUSO0_MUTUAL5_KEY47,
#endif
#if ( MUTUAL5_NUM > 48 )
    CTSUSO0_MUTUAL5_KEY48,
#endif
#if ( MUTUAL5_NUM > 49 )
    CTSUSO0_MUTUAL5_KEY49,
#endif
#if ( MUTUAL5_NUM > 50 )
    CTSUSO0_MUTUAL5_KEY50,
#endif
#if ( MUTUAL5_NUM > 51 )
    CTSUSO0_MUTUAL5_KEY51,
#endif
#if ( MUTUAL5_NUM > 52 )
    CTSUSO0_MUTUAL5_KEY52,
#endif
#if ( MUTUAL5_NUM > 53 )
    CTSUSO0_MUTUAL5_KEY53,
#endif
#if ( MUTUAL5_NUM > 54 )
    CTSUSO0_MUTUAL5_KEY54,
#endif
#if ( MUTUAL5_NUM > 55 )
    CTSUSO0_MUTUAL5_KEY55,
#endif
#if ( MUTUAL5_NUM > 56 )
    CTSUSO0_MUTUAL5_KEY56,
#endif
#if ( MUTUAL5_NUM > 57 )
    CTSUSO0_MUTUAL5_KEY57,
#endif
#if ( MUTUAL5_NUM > 58 )
    CTSUSO0_MUTUAL5_KEY58,
#endif
#if ( MUTUAL5_NUM > 59 )
    CTSUSO0_MUTUAL5_KEY59,
#endif
#if ( MUTUAL5_NUM > 60 )
    CTSUSO0_MUTUAL5_KEY60,
#endif
#if ( MUTUAL5_NUM > 61 )
    CTSUSO0_MUTUAL5_KEY61,
#endif
#if ( MUTUAL5_NUM > 62 )
    CTSUSO0_MUTUAL5_KEY62,
#endif
#if ( MUTUAL5_NUM > 63 )
    CTSUSO0_MUTUAL5_KEY63,
#endif
};

const uint16_t g_mutual5_ctsuso1[] =
{
#if ( MUTUAL5_NUM > 0 )
    CTSUSO1_MUTUAL5_KEY00,
#endif
#if ( MUTUAL5_NUM > 1 )
    CTSUSO1_MUTUAL5_KEY01,
#endif
#if ( MUTUAL5_NUM > 2 )
    CTSUSO1_MUTUAL5_KEY02,
#endif
#if ( MUTUAL5_NUM > 3 )
    CTSUSO1_MUTUAL5_KEY03,
#endif
#if ( MUTUAL5_NUM > 4 )
    CTSUSO1_MUTUAL5_KEY04,
#endif
#if ( MUTUAL5_NUM > 5 )
    CTSUSO1_MUTUAL5_KEY05,
#endif
#if ( MUTUAL5_NUM > 6 )
    CTSUSO1_MUTUAL5_KEY06,
#endif
#if ( MUTUAL5_NUM > 7 )
    CTSUSO1_MUTUAL5_KEY07,
#endif
#if ( MUTUAL5_NUM > 8 )
    CTSUSO1_MUTUAL5_KEY08,
#endif
#if ( MUTUAL5_NUM > 9 )
    CTSUSO1_MUTUAL5_KEY09,
#endif
#if ( MUTUAL5_NUM > 10 )
    CTSUSO1_MUTUAL5_KEY10,
#endif
#if ( MUTUAL5_NUM > 11 )
    CTSUSO1_MUTUAL5_KEY11,
#endif
#if ( MUTUAL5_NUM > 12 )
    CTSUSO1_MUTUAL5_KEY12,
#endif
#if ( MUTUAL5_NUM > 13 )
    CTSUSO1_MUTUAL5_KEY13,
#endif
#if ( MUTUAL5_NUM > 14 )
    CTSUSO1_MUTUAL5_KEY14,
#endif
#if ( MUTUAL5_NUM > 15 )
    CTSUSO1_MUTUAL5_KEY15,
#endif
#if ( MUTUAL5_NUM > 16 )
    CTSUSO1_MUTUAL5_KEY16,
#endif
#if ( MUTUAL5_NUM > 17 )
    CTSUSO1_MUTUAL5_KEY17,
#endif
#if ( MUTUAL5_NUM > 18 )
    CTSUSO1_MUTUAL5_KEY18,
#endif
#if ( MUTUAL5_NUM > 19 )
    CTSUSO1_MUTUAL5_KEY19,
#endif
#if ( MUTUAL5_NUM > 20 )
    CTSUSO1_MUTUAL5_KEY20,
#endif
#if ( MUTUAL5_NUM > 21 )
    CTSUSO1_MUTUAL5_KEY21,
#endif
#if ( MUTUAL5_NUM > 22 )
    CTSUSO1_MUTUAL5_KEY22,
#endif
#if ( MUTUAL5_NUM > 23 )
    CTSUSO1_MUTUAL5_KEY23,
#endif
#if ( MUTUAL5_NUM > 24 )
    CTSUSO1_MUTUAL5_KEY24,
#endif
#if ( MUTUAL5_NUM > 25 )
    CTSUSO1_MUTUAL5_KEY25,
#endif
#if ( MUTUAL5_NUM > 26 )
    CTSUSO1_MUTUAL5_KEY26,
#endif
#if ( MUTUAL5_NUM > 27 )
    CTSUSO1_MUTUAL5_KEY27,
#endif
#if ( MUTUAL5_NUM > 28 )
    CTSUSO1_MUTUAL5_KEY28,
#endif
#if ( MUTUAL5_NUM > 29 )
    CTSUSO1_MUTUAL5_KEY29,
#endif
#if ( MUTUAL5_NUM > 30 )
    CTSUSO1_MUTUAL5_KEY30,
#endif
#if ( MUTUAL5_NUM > 31 )
    CTSUSO1_MUTUAL5_KEY31,
#endif
#if ( MUTUAL5_NUM > 32 )
    CTSUSO1_MUTUAL5_KEY32,
#endif
#if ( MUTUAL5_NUM > 33 )
    CTSUSO1_MUTUAL5_KEY33,
#endif
#if ( MUTUAL5_NUM > 34 )
    CTSUSO1_MUTUAL5_KEY34,
#endif
#if ( MUTUAL5_NUM > 35 )
    CTSUSO1_MUTUAL5_KEY35,
#endif
#if ( MUTUAL5_NUM > 36 )
    CTSUSO1_MUTUAL5_KEY36,
#endif
#if ( MUTUAL5_NUM > 37 )
    CTSUSO1_MUTUAL5_KEY37,
#endif
#if ( MUTUAL5_NUM > 38 )
    CTSUSO1_MUTUAL5_KEY38,
#endif
#if ( MUTUAL5_NUM > 39 )
    CTSUSO1_MUTUAL5_KEY39,
#endif
#if ( MUTUAL5_NUM > 40 )
    CTSUSO1_MUTUAL5_KEY40,
#endif
#if ( MUTUAL5_NUM > 41 )
    CTSUSO1_MUTUAL5_KEY41,
#endif
#if ( MUTUAL5_NUM > 42 )
    CTSUSO1_MUTUAL5_KEY42,
#endif
#if ( MUTUAL5_NUM > 43 )
    CTSUSO1_MUTUAL5_KEY43,
#endif
#if ( MUTUAL5_NUM > 44 )
    CTSUSO1_MUTUAL5_KEY44,
#endif
#if ( MUTUAL5_NUM > 45 )
    CTSUSO1_MUTUAL5_KEY45,
#endif
#if ( MUTUAL5_NUM > 46 )
    CTSUSO1_MUTUAL5_KEY46,
#endif
#if ( MUTUAL5_NUM > 47 )
    CTSUSO1_MUTUAL5_KEY47,
#endif
#if ( MUTUAL5_NUM > 48 )
    CTSUSO1_MUTUAL5_KEY48,
#endif
#if ( MUTUAL5_NUM > 49 )
    CTSUSO1_MUTUAL5_KEY49,
#endif
#if ( MUTUAL5_NUM > 50 )
    CTSUSO1_MUTUAL5_KEY50,
#endif
#if ( MUTUAL5_NUM > 51 )
    CTSUSO1_MUTUAL5_KEY51,
#endif
#if ( MUTUAL5_NUM > 52 )
    CTSUSO1_MUTUAL5_KEY52,
#endif
#if ( MUTUAL5_NUM > 53 )
    CTSUSO1_MUTUAL5_KEY53,
#endif
#if ( MUTUAL5_NUM > 54 )
    CTSUSO1_MUTUAL5_KEY54,
#endif
#if ( MUTUAL5_NUM > 55 )
    CTSUSO1_MUTUAL5_KEY55,
#endif
#if ( MUTUAL5_NUM > 56 )
    CTSUSO1_MUTUAL5_KEY56,
#endif
#if ( MUTUAL5_NUM > 57 )
    CTSUSO1_MUTUAL5_KEY57,
#endif
#if ( MUTUAL5_NUM > 58 )
    CTSUSO1_MUTUAL5_KEY58,
#endif
#if ( MUTUAL5_NUM > 59 )
    CTSUSO1_MUTUAL5_KEY59,
#endif
#if ( MUTUAL5_NUM > 60 )
    CTSUSO1_MUTUAL5_KEY60,
#endif
#if ( MUTUAL5_NUM > 61 )
    CTSUSO1_MUTUAL5_KEY61,
#endif
#if ( MUTUAL5_NUM > 62 )
    CTSUSO1_MUTUAL5_KEY62,
#endif
#if ( MUTUAL5_NUM > 63 )
    CTSUSO1_MUTUAL5_KEY63,
#endif
};
#endif  // ( MUTUAL_METHOD_NUM > 5 )

/*=====    Mutual 6    DTC transmit data ========================================================*/
#if ( MUTUAL_METHOD_NUM > 6 )
const uint16_t g_mutual6_ctsussc[] =
{
#if ( MUTUAL6_NUM > 0 )
    CTSUSSC_MUTUAL6_KEY00,
#endif
#if ( MUTUAL6_NUM > 1 )
    CTSUSSC_MUTUAL6_KEY01,
#endif
#if ( MUTUAL6_NUM > 2 )
    CTSUSSC_MUTUAL6_KEY02,
#endif
#if ( MUTUAL6_NUM > 3 )
    CTSUSSC_MUTUAL6_KEY03,
#endif
#if ( MUTUAL6_NUM > 4 )
    CTSUSSC_MUTUAL6_KEY04,
#endif
#if ( MUTUAL6_NUM > 5 )
    CTSUSSC_MUTUAL6_KEY05,
#endif
#if ( MUTUAL6_NUM > 6 )
    CTSUSSC_MUTUAL6_KEY06,
#endif
#if ( MUTUAL6_NUM > 7 )
    CTSUSSC_MUTUAL6_KEY07,
#endif
#if ( MUTUAL6_NUM > 8 )
    CTSUSSC_MUTUAL6_KEY08,
#endif
#if ( MUTUAL6_NUM > 9 )
    CTSUSSC_MUTUAL6_KEY09,
#endif
#if ( MUTUAL6_NUM > 10 )
    CTSUSSC_MUTUAL6_KEY10,
#endif
#if ( MUTUAL6_NUM > 11 )
    CTSUSSC_MUTUAL6_KEY11,
#endif
#if ( MUTUAL6_NUM > 12 )
    CTSUSSC_MUTUAL6_KEY12,
#endif
#if ( MUTUAL6_NUM > 13 )
    CTSUSSC_MUTUAL6_KEY13,
#endif
#if ( MUTUAL6_NUM > 14 )
    CTSUSSC_MUTUAL6_KEY14,
#endif
#if ( MUTUAL6_NUM > 15 )
    CTSUSSC_MUTUAL6_KEY15,
#endif
#if ( MUTUAL6_NUM > 16 )
    CTSUSSC_MUTUAL6_KEY16,
#endif
#if ( MUTUAL6_NUM > 17 )
    CTSUSSC_MUTUAL6_KEY17,
#endif
#if ( MUTUAL6_NUM > 18 )
    CTSUSSC_MUTUAL6_KEY18,
#endif
#if ( MUTUAL6_NUM > 19 )
    CTSUSSC_MUTUAL6_KEY19,
#endif
#if ( MUTUAL6_NUM > 20 )
    CTSUSSC_MUTUAL6_KEY20,
#endif
#if ( MUTUAL6_NUM > 21 )
    CTSUSSC_MUTUAL6_KEY21,
#endif
#if ( MUTUAL6_NUM > 22 )
    CTSUSSC_MUTUAL6_KEY22,
#endif
#if ( MUTUAL6_NUM > 23 )
    CTSUSSC_MUTUAL6_KEY23,
#endif
#if ( MUTUAL6_NUM > 24 )
    CTSUSSC_MUTUAL6_KEY24,
#endif
#if ( MUTUAL6_NUM > 25 )
    CTSUSSC_MUTUAL6_KEY25,
#endif
#if ( MUTUAL6_NUM > 26 )
    CTSUSSC_MUTUAL6_KEY26,
#endif
#if ( MUTUAL6_NUM > 27 )
    CTSUSSC_MUTUAL6_KEY27,
#endif
#if ( MUTUAL6_NUM > 28 )
    CTSUSSC_MUTUAL6_KEY28,
#endif
#if ( MUTUAL6_NUM > 29 )
    CTSUSSC_MUTUAL6_KEY29,
#endif
#if ( MUTUAL6_NUM > 30 )
    CTSUSSC_MUTUAL6_KEY30,
#endif
#if ( MUTUAL6_NUM > 31 )
    CTSUSSC_MUTUAL6_KEY31,
#endif
#if ( MUTUAL6_NUM > 32 )
    CTSUSSC_MUTUAL6_KEY32,
#endif
#if ( MUTUAL6_NUM > 33 )
    CTSUSSC_MUTUAL6_KEY33,
#endif
#if ( MUTUAL6_NUM > 34 )
    CTSUSSC_MUTUAL6_KEY34,
#endif
#if ( MUTUAL6_NUM > 35 )
    CTSUSSC_MUTUAL6_KEY35,
#endif
#if ( MUTUAL6_NUM > 36 )
    CTSUSSC_MUTUAL6_KEY36,
#endif
#if ( MUTUAL6_NUM > 37 )
    CTSUSSC_MUTUAL6_KEY37,
#endif
#if ( MUTUAL6_NUM > 38 )
    CTSUSSC_MUTUAL6_KEY38,
#endif
#if ( MUTUAL6_NUM > 39 )
    CTSUSSC_MUTUAL6_KEY39,
#endif
#if ( MUTUAL6_NUM > 40 )
    CTSUSSC_MUTUAL6_KEY40,
#endif
#if ( MUTUAL6_NUM > 41 )
    CTSUSSC_MUTUAL6_KEY41,
#endif
#if ( MUTUAL6_NUM > 42 )
    CTSUSSC_MUTUAL6_KEY42,
#endif
#if ( MUTUAL6_NUM > 43 )
    CTSUSSC_MUTUAL6_KEY43,
#endif
#if ( MUTUAL6_NUM > 44 )
    CTSUSSC_MUTUAL6_KEY44,
#endif
#if ( MUTUAL6_NUM > 45 )
    CTSUSSC_MUTUAL6_KEY45,
#endif
#if ( MUTUAL6_NUM > 46 )
    CTSUSSC_MUTUAL6_KEY46,
#endif
#if ( MUTUAL6_NUM > 47 )
    CTSUSSC_MUTUAL6_KEY47,
#endif
#if ( MUTUAL6_NUM > 48 )
    CTSUSSC_MUTUAL6_KEY48,
#endif
#if ( MUTUAL6_NUM > 49 )
    CTSUSSC_MUTUAL6_KEY49,
#endif
#if ( MUTUAL6_NUM > 50 )
    CTSUSSC_MUTUAL6_KEY50,
#endif
#if ( MUTUAL6_NUM > 51 )
    CTSUSSC_MUTUAL6_KEY51,
#endif
#if ( MUTUAL6_NUM > 52 )
    CTSUSSC_MUTUAL6_KEY52,
#endif
#if ( MUTUAL6_NUM > 53 )
    CTSUSSC_MUTUAL6_KEY53,
#endif
#if ( MUTUAL6_NUM > 54 )
    CTSUSSC_MUTUAL6_KEY54,
#endif
#if ( MUTUAL6_NUM > 55 )
    CTSUSSC_MUTUAL6_KEY55,
#endif
#if ( MUTUAL6_NUM > 56 )
    CTSUSSC_MUTUAL6_KEY56,
#endif
#if ( MUTUAL6_NUM > 57 )
    CTSUSSC_MUTUAL6_KEY57,
#endif
#if ( MUTUAL6_NUM > 58 )
    CTSUSSC_MUTUAL6_KEY58,
#endif
#if ( MUTUAL6_NUM > 59 )
    CTSUSSC_MUTUAL6_KEY59,
#endif
#if ( MUTUAL6_NUM > 60 )
    CTSUSSC_MUTUAL6_KEY60,
#endif
#if ( MUTUAL6_NUM > 61 )
    CTSUSSC_MUTUAL6_KEY61,
#endif
#if ( MUTUAL6_NUM > 62 )
    CTSUSSC_MUTUAL6_KEY62,
#endif
#if ( MUTUAL6_NUM > 63 )
    CTSUSSC_MUTUAL6_KEY63,
#endif
};

const uint16_t g_mutual6_ctsuso0[] =
{
#if ( MUTUAL6_NUM > 0 )
    CTSUSO0_MUTUAL6_KEY00,
#endif
#if ( MUTUAL6_NUM > 1 )
    CTSUSO0_MUTUAL6_KEY01,
#endif
#if ( MUTUAL6_NUM > 2 )
    CTSUSO0_MUTUAL6_KEY02,
#endif
#if ( MUTUAL6_NUM > 3 )
    CTSUSO0_MUTUAL6_KEY03,
#endif
#if ( MUTUAL6_NUM > 4 )
    CTSUSO0_MUTUAL6_KEY04,
#endif
#if ( MUTUAL6_NUM > 5 )
    CTSUSO0_MUTUAL6_KEY05,
#endif
#if ( MUTUAL6_NUM > 6 )
    CTSUSO0_MUTUAL6_KEY06,
#endif
#if ( MUTUAL6_NUM > 7 )
    CTSUSO0_MUTUAL6_KEY07,
#endif
#if ( MUTUAL6_NUM > 8 )
    CTSUSO0_MUTUAL6_KEY08,
#endif
#if ( MUTUAL6_NUM > 9 )
    CTSUSO0_MUTUAL6_KEY09,
#endif
#if ( MUTUAL6_NUM > 10 )
    CTSUSO0_MUTUAL6_KEY10,
#endif
#if ( MUTUAL6_NUM > 11 )
    CTSUSO0_MUTUAL6_KEY11,
#endif
#if ( MUTUAL6_NUM > 12 )
    CTSUSO0_MUTUAL6_KEY12,
#endif
#if ( MUTUAL6_NUM > 13 )
    CTSUSO0_MUTUAL6_KEY13,
#endif
#if ( MUTUAL6_NUM > 14 )
    CTSUSO0_MUTUAL6_KEY14,
#endif
#if ( MUTUAL6_NUM > 15 )
    CTSUSO0_MUTUAL6_KEY15,
#endif
#if ( MUTUAL6_NUM > 16 )
    CTSUSO0_MUTUAL6_KEY16,
#endif
#if ( MUTUAL6_NUM > 17 )
    CTSUSO0_MUTUAL6_KEY17,
#endif
#if ( MUTUAL6_NUM > 18 )
    CTSUSO0_MUTUAL6_KEY18,
#endif
#if ( MUTUAL6_NUM > 19 )
    CTSUSO0_MUTUAL6_KEY19,
#endif
#if ( MUTUAL6_NUM > 20 )
    CTSUSO0_MUTUAL6_KEY20,
#endif
#if ( MUTUAL6_NUM > 21 )
    CTSUSO0_MUTUAL6_KEY21,
#endif
#if ( MUTUAL6_NUM > 22 )
    CTSUSO0_MUTUAL6_KEY22,
#endif
#if ( MUTUAL6_NUM > 23 )
    CTSUSO0_MUTUAL6_KEY23,
#endif
#if ( MUTUAL6_NUM > 24 )
    CTSUSO0_MUTUAL6_KEY24,
#endif
#if ( MUTUAL6_NUM > 25 )
    CTSUSO0_MUTUAL6_KEY25,
#endif
#if ( MUTUAL6_NUM > 26 )
    CTSUSO0_MUTUAL6_KEY26,
#endif
#if ( MUTUAL6_NUM > 27 )
    CTSUSO0_MUTUAL6_KEY27,
#endif
#if ( MUTUAL6_NUM > 28 )
    CTSUSO0_MUTUAL6_KEY28,
#endif
#if ( MUTUAL6_NUM > 29 )
    CTSUSO0_MUTUAL6_KEY29,
#endif
#if ( MUTUAL6_NUM > 30 )
    CTSUSO0_MUTUAL6_KEY30,
#endif
#if ( MUTUAL6_NUM > 31 )
    CTSUSO0_MUTUAL6_KEY31,
#endif
#if ( MUTUAL6_NUM > 32 )
    CTSUSO0_MUTUAL6_KEY32,
#endif
#if ( MUTUAL6_NUM > 33 )
    CTSUSO0_MUTUAL6_KEY33,
#endif
#if ( MUTUAL6_NUM > 34 )
    CTSUSO0_MUTUAL6_KEY34,
#endif
#if ( MUTUAL6_NUM > 35 )
    CTSUSO0_MUTUAL6_KEY35,
#endif
#if ( MUTUAL6_NUM > 36 )
    CTSUSO0_MUTUAL6_KEY36,
#endif
#if ( MUTUAL6_NUM > 37 )
    CTSUSO0_MUTUAL6_KEY37,
#endif
#if ( MUTUAL6_NUM > 38 )
    CTSUSO0_MUTUAL6_KEY38,
#endif
#if ( MUTUAL6_NUM > 39 )
    CTSUSO0_MUTUAL6_KEY39,
#endif
#if ( MUTUAL6_NUM > 40 )
    CTSUSO0_MUTUAL6_KEY40,
#endif
#if ( MUTUAL6_NUM > 41 )
    CTSUSO0_MUTUAL6_KEY41,
#endif
#if ( MUTUAL6_NUM > 42 )
    CTSUSO0_MUTUAL6_KEY42,
#endif
#if ( MUTUAL6_NUM > 43 )
    CTSUSO0_MUTUAL6_KEY43,
#endif
#if ( MUTUAL6_NUM > 44 )
    CTSUSO0_MUTUAL6_KEY44,
#endif
#if ( MUTUAL6_NUM > 45 )
    CTSUSO0_MUTUAL6_KEY45,
#endif
#if ( MUTUAL6_NUM > 46 )
    CTSUSO0_MUTUAL6_KEY46,
#endif
#if ( MUTUAL6_NUM > 47 )
    CTSUSO0_MUTUAL6_KEY47,
#endif
#if ( MUTUAL6_NUM > 48 )
    CTSUSO0_MUTUAL6_KEY48,
#endif
#if ( MUTUAL6_NUM > 49 )
    CTSUSO0_MUTUAL6_KEY49,
#endif
#if ( MUTUAL6_NUM > 50 )
    CTSUSO0_MUTUAL6_KEY50,
#endif
#if ( MUTUAL6_NUM > 51 )
    CTSUSO0_MUTUAL6_KEY51,
#endif
#if ( MUTUAL6_NUM > 52 )
    CTSUSO0_MUTUAL6_KEY52,
#endif
#if ( MUTUAL6_NUM > 53 )
    CTSUSO0_MUTUAL6_KEY53,
#endif
#if ( MUTUAL6_NUM > 54 )
    CTSUSO0_MUTUAL6_KEY54,
#endif
#if ( MUTUAL6_NUM > 55 )
    CTSUSO0_MUTUAL6_KEY55,
#endif
#if ( MUTUAL6_NUM > 56 )
    CTSUSO0_MUTUAL6_KEY56,
#endif
#if ( MUTUAL6_NUM > 57 )
    CTSUSO0_MUTUAL6_KEY57,
#endif
#if ( MUTUAL6_NUM > 58 )
    CTSUSO0_MUTUAL6_KEY58,
#endif
#if ( MUTUAL6_NUM > 59 )
    CTSUSO0_MUTUAL6_KEY59,
#endif
#if ( MUTUAL6_NUM > 60 )
    CTSUSO0_MUTUAL6_KEY60,
#endif
#if ( MUTUAL6_NUM > 61 )
    CTSUSO0_MUTUAL6_KEY61,
#endif
#if ( MUTUAL6_NUM > 62 )
    CTSUSO0_MUTUAL6_KEY62,
#endif
#if ( MUTUAL6_NUM > 63 )
    CTSUSO0_MUTUAL6_KEY63,
#endif
};

const uint16_t g_mutual6_ctsuso1[] =
{
#if ( MUTUAL6_NUM > 0 )
    CTSUSO1_MUTUAL6_KEY00,
#endif
#if ( MUTUAL6_NUM > 1 )
    CTSUSO1_MUTUAL6_KEY01,
#endif
#if ( MUTUAL6_NUM > 2 )
    CTSUSO1_MUTUAL6_KEY02,
#endif
#if ( MUTUAL6_NUM > 3 )
    CTSUSO1_MUTUAL6_KEY03,
#endif
#if ( MUTUAL6_NUM > 4 )
    CTSUSO1_MUTUAL6_KEY04,
#endif
#if ( MUTUAL6_NUM > 5 )
    CTSUSO1_MUTUAL6_KEY05,
#endif
#if ( MUTUAL6_NUM > 6 )
    CTSUSO1_MUTUAL6_KEY06,
#endif
#if ( MUTUAL6_NUM > 7 )
    CTSUSO1_MUTUAL6_KEY07,
#endif
#if ( MUTUAL6_NUM > 8 )
    CTSUSO1_MUTUAL6_KEY08,
#endif
#if ( MUTUAL6_NUM > 9 )
    CTSUSO1_MUTUAL6_KEY09,
#endif
#if ( MUTUAL6_NUM > 10 )
    CTSUSO1_MUTUAL6_KEY10,
#endif
#if ( MUTUAL6_NUM > 11 )
    CTSUSO1_MUTUAL6_KEY11,
#endif
#if ( MUTUAL6_NUM > 12 )
    CTSUSO1_MUTUAL6_KEY12,
#endif
#if ( MUTUAL6_NUM > 13 )
    CTSUSO1_MUTUAL6_KEY13,
#endif
#if ( MUTUAL6_NUM > 14 )
    CTSUSO1_MUTUAL6_KEY14,
#endif
#if ( MUTUAL6_NUM > 15 )
    CTSUSO1_MUTUAL6_KEY15,
#endif
#if ( MUTUAL6_NUM > 16 )
    CTSUSO1_MUTUAL6_KEY16,
#endif
#if ( MUTUAL6_NUM > 17 )
    CTSUSO1_MUTUAL6_KEY17,
#endif
#if ( MUTUAL6_NUM > 18 )
    CTSUSO1_MUTUAL6_KEY18,
#endif
#if ( MUTUAL6_NUM > 19 )
    CTSUSO1_MUTUAL6_KEY19,
#endif
#if ( MUTUAL6_NUM > 20 )
    CTSUSO1_MUTUAL6_KEY20,
#endif
#if ( MUTUAL6_NUM > 21 )
    CTSUSO1_MUTUAL6_KEY21,
#endif
#if ( MUTUAL6_NUM > 22 )
    CTSUSO1_MUTUAL6_KEY22,
#endif
#if ( MUTUAL6_NUM > 23 )
    CTSUSO1_MUTUAL6_KEY23,
#endif
#if ( MUTUAL6_NUM > 24 )
    CTSUSO1_MUTUAL6_KEY24,
#endif
#if ( MUTUAL6_NUM > 25 )
    CTSUSO1_MUTUAL6_KEY25,
#endif
#if ( MUTUAL6_NUM > 26 )
    CTSUSO1_MUTUAL6_KEY26,
#endif
#if ( MUTUAL6_NUM > 27 )
    CTSUSO1_MUTUAL6_KEY27,
#endif
#if ( MUTUAL6_NUM > 28 )
    CTSUSO1_MUTUAL6_KEY28,
#endif
#if ( MUTUAL6_NUM > 29 )
    CTSUSO1_MUTUAL6_KEY29,
#endif
#if ( MUTUAL6_NUM > 30 )
    CTSUSO1_MUTUAL6_KEY30,
#endif
#if ( MUTUAL6_NUM > 31 )
    CTSUSO1_MUTUAL6_KEY31,
#endif
#if ( MUTUAL6_NUM > 32 )
    CTSUSO1_MUTUAL6_KEY32,
#endif
#if ( MUTUAL6_NUM > 33 )
    CTSUSO1_MUTUAL6_KEY33,
#endif
#if ( MUTUAL6_NUM > 34 )
    CTSUSO1_MUTUAL6_KEY34,
#endif
#if ( MUTUAL6_NUM > 35 )
    CTSUSO1_MUTUAL6_KEY35,
#endif
#if ( MUTUAL6_NUM > 36 )
    CTSUSO1_MUTUAL6_KEY36,
#endif
#if ( MUTUAL6_NUM > 37 )
    CTSUSO1_MUTUAL6_KEY37,
#endif
#if ( MUTUAL6_NUM > 38 )
    CTSUSO1_MUTUAL6_KEY38,
#endif
#if ( MUTUAL6_NUM > 39 )
    CTSUSO1_MUTUAL6_KEY39,
#endif
#if ( MUTUAL6_NUM > 40 )
    CTSUSO1_MUTUAL6_KEY40,
#endif
#if ( MUTUAL6_NUM > 41 )
    CTSUSO1_MUTUAL6_KEY41,
#endif
#if ( MUTUAL6_NUM > 42 )
    CTSUSO1_MUTUAL6_KEY42,
#endif
#if ( MUTUAL6_NUM > 43 )
    CTSUSO1_MUTUAL6_KEY43,
#endif
#if ( MUTUAL6_NUM > 44 )
    CTSUSO1_MUTUAL6_KEY44,
#endif
#if ( MUTUAL6_NUM > 45 )
    CTSUSO1_MUTUAL6_KEY45,
#endif
#if ( MUTUAL6_NUM > 46 )
    CTSUSO1_MUTUAL6_KEY46,
#endif
#if ( MUTUAL6_NUM > 47 )
    CTSUSO1_MUTUAL6_KEY47,
#endif
#if ( MUTUAL6_NUM > 48 )
    CTSUSO1_MUTUAL6_KEY48,
#endif
#if ( MUTUAL6_NUM > 49 )
    CTSUSO1_MUTUAL6_KEY49,
#endif
#if ( MUTUAL6_NUM > 50 )
    CTSUSO1_MUTUAL6_KEY50,
#endif
#if ( MUTUAL6_NUM > 51 )
    CTSUSO1_MUTUAL6_KEY51,
#endif
#if ( MUTUAL6_NUM > 52 )
    CTSUSO1_MUTUAL6_KEY52,
#endif
#if ( MUTUAL6_NUM > 53 )
    CTSUSO1_MUTUAL6_KEY53,
#endif
#if ( MUTUAL6_NUM > 54 )
    CTSUSO1_MUTUAL6_KEY54,
#endif
#if ( MUTUAL6_NUM > 55 )
    CTSUSO1_MUTUAL6_KEY55,
#endif
#if ( MUTUAL6_NUM > 56 )
    CTSUSO1_MUTUAL6_KEY56,
#endif
#if ( MUTUAL6_NUM > 57 )
    CTSUSO1_MUTUAL6_KEY57,
#endif
#if ( MUTUAL6_NUM > 58 )
    CTSUSO1_MUTUAL6_KEY58,
#endif
#if ( MUTUAL6_NUM > 59 )
    CTSUSO1_MUTUAL6_KEY59,
#endif
#if ( MUTUAL6_NUM > 60 )
    CTSUSO1_MUTUAL6_KEY60,
#endif
#if ( MUTUAL6_NUM > 61 )
    CTSUSO1_MUTUAL6_KEY61,
#endif
#if ( MUTUAL6_NUM > 62 )
    CTSUSO1_MUTUAL6_KEY62,
#endif
#if ( MUTUAL6_NUM > 63 )
    CTSUSO1_MUTUAL6_KEY63,
#endif
};
#endif  // ( MUTUAL_METHOD_NUM > 6 )

/*=====    Mutual 7    DTC transmit data ========================================================*/
#if ( MUTUAL_METHOD_NUM > 7 )
const uint16_t g_mutual7_ctsussc[] =
{
#if ( MUTUAL7_NUM > 0 )
    CTSUSSC_MUTUAL7_KEY00,
#endif
#if ( MUTUAL7_NUM > 1 )
    CTSUSSC_MUTUAL7_KEY01,
#endif
#if ( MUTUAL7_NUM > 2 )
    CTSUSSC_MUTUAL7_KEY02,
#endif
#if ( MUTUAL7_NUM > 3 )
    CTSUSSC_MUTUAL7_KEY03,
#endif
#if ( MUTUAL7_NUM > 4 )
    CTSUSSC_MUTUAL7_KEY04,
#endif
#if ( MUTUAL7_NUM > 5 )
    CTSUSSC_MUTUAL7_KEY05,
#endif
#if ( MUTUAL7_NUM > 6 )
    CTSUSSC_MUTUAL7_KEY06,
#endif
#if ( MUTUAL7_NUM > 7 )
    CTSUSSC_MUTUAL7_KEY07,
#endif
#if ( MUTUAL7_NUM > 8 )
    CTSUSSC_MUTUAL7_KEY08,
#endif
#if ( MUTUAL7_NUM > 9 )
    CTSUSSC_MUTUAL7_KEY09,
#endif
#if ( MUTUAL7_NUM > 10 )
    CTSUSSC_MUTUAL7_KEY10,
#endif
#if ( MUTUAL7_NUM > 11 )
    CTSUSSC_MUTUAL7_KEY11,
#endif
#if ( MUTUAL7_NUM > 12 )
    CTSUSSC_MUTUAL7_KEY12,
#endif
#if ( MUTUAL7_NUM > 13 )
    CTSUSSC_MUTUAL7_KEY13,
#endif
#if ( MUTUAL7_NUM > 14 )
    CTSUSSC_MUTUAL7_KEY14,
#endif
#if ( MUTUAL7_NUM > 15 )
    CTSUSSC_MUTUAL7_KEY15,
#endif
#if ( MUTUAL7_NUM > 16 )
    CTSUSSC_MUTUAL7_KEY16,
#endif
#if ( MUTUAL7_NUM > 17 )
    CTSUSSC_MUTUAL7_KEY17,
#endif
#if ( MUTUAL7_NUM > 18 )
    CTSUSSC_MUTUAL7_KEY18,
#endif
#if ( MUTUAL7_NUM > 19 )
    CTSUSSC_MUTUAL7_KEY19,
#endif
#if ( MUTUAL7_NUM > 20 )
    CTSUSSC_MUTUAL7_KEY20,
#endif
#if ( MUTUAL7_NUM > 21 )
    CTSUSSC_MUTUAL7_KEY21,
#endif
#if ( MUTUAL7_NUM > 22 )
    CTSUSSC_MUTUAL7_KEY22,
#endif
#if ( MUTUAL7_NUM > 23 )
    CTSUSSC_MUTUAL7_KEY23,
#endif
#if ( MUTUAL7_NUM > 24 )
    CTSUSSC_MUTUAL7_KEY24,
#endif
#if ( MUTUAL7_NUM > 25 )
    CTSUSSC_MUTUAL7_KEY25,
#endif
#if ( MUTUAL7_NUM > 26 )
    CTSUSSC_MUTUAL7_KEY26,
#endif
#if ( MUTUAL7_NUM > 27 )
    CTSUSSC_MUTUAL7_KEY27,
#endif
#if ( MUTUAL7_NUM > 28 )
    CTSUSSC_MUTUAL7_KEY28,
#endif
#if ( MUTUAL7_NUM > 29 )
    CTSUSSC_MUTUAL7_KEY29,
#endif
#if ( MUTUAL7_NUM > 30 )
    CTSUSSC_MUTUAL7_KEY30,
#endif
#if ( MUTUAL7_NUM > 31 )
    CTSUSSC_MUTUAL7_KEY31,
#endif
#if ( MUTUAL7_NUM > 32 )
    CTSUSSC_MUTUAL7_KEY32,
#endif
#if ( MUTUAL7_NUM > 33 )
    CTSUSSC_MUTUAL7_KEY33,
#endif
#if ( MUTUAL7_NUM > 34 )
    CTSUSSC_MUTUAL7_KEY34,
#endif
#if ( MUTUAL7_NUM > 35 )
    CTSUSSC_MUTUAL7_KEY35,
#endif
#if ( MUTUAL7_NUM > 36 )
    CTSUSSC_MUTUAL7_KEY36,
#endif
#if ( MUTUAL7_NUM > 37 )
    CTSUSSC_MUTUAL7_KEY37,
#endif
#if ( MUTUAL7_NUM > 38 )
    CTSUSSC_MUTUAL7_KEY38,
#endif
#if ( MUTUAL7_NUM > 39 )
    CTSUSSC_MUTUAL7_KEY39,
#endif
#if ( MUTUAL7_NUM > 40 )
    CTSUSSC_MUTUAL7_KEY40,
#endif
#if ( MUTUAL7_NUM > 41 )
    CTSUSSC_MUTUAL7_KEY41,
#endif
#if ( MUTUAL7_NUM > 42 )
    CTSUSSC_MUTUAL7_KEY42,
#endif
#if ( MUTUAL7_NUM > 43 )
    CTSUSSC_MUTUAL7_KEY43,
#endif
#if ( MUTUAL7_NUM > 44 )
    CTSUSSC_MUTUAL7_KEY44,
#endif
#if ( MUTUAL7_NUM > 45 )
    CTSUSSC_MUTUAL7_KEY45,
#endif
#if ( MUTUAL7_NUM > 46 )
    CTSUSSC_MUTUAL7_KEY46,
#endif
#if ( MUTUAL7_NUM > 47 )
    CTSUSSC_MUTUAL7_KEY47,
#endif
#if ( MUTUAL7_NUM > 48 )
    CTSUSSC_MUTUAL7_KEY48,
#endif
#if ( MUTUAL7_NUM > 49 )
    CTSUSSC_MUTUAL7_KEY49,
#endif
#if ( MUTUAL7_NUM > 50 )
    CTSUSSC_MUTUAL7_KEY50,
#endif
#if ( MUTUAL7_NUM > 51 )
    CTSUSSC_MUTUAL7_KEY51,
#endif
#if ( MUTUAL7_NUM > 52 )
    CTSUSSC_MUTUAL7_KEY52,
#endif
#if ( MUTUAL7_NUM > 53 )
    CTSUSSC_MUTUAL7_KEY53,
#endif
#if ( MUTUAL7_NUM > 54 )
    CTSUSSC_MUTUAL7_KEY54,
#endif
#if ( MUTUAL7_NUM > 55 )
    CTSUSSC_MUTUAL7_KEY55,
#endif
#if ( MUTUAL7_NUM > 56 )
    CTSUSSC_MUTUAL7_KEY56,
#endif
#if ( MUTUAL7_NUM > 57 )
    CTSUSSC_MUTUAL7_KEY57,
#endif
#if ( MUTUAL7_NUM > 58 )
    CTSUSSC_MUTUAL7_KEY58,
#endif
#if ( MUTUAL7_NUM > 59 )
    CTSUSSC_MUTUAL7_KEY59,
#endif
#if ( MUTUAL7_NUM > 60 )
    CTSUSSC_MUTUAL7_KEY60,
#endif
#if ( MUTUAL7_NUM > 61 )
    CTSUSSC_MUTUAL7_KEY61,
#endif
#if ( MUTUAL7_NUM > 62 )
    CTSUSSC_MUTUAL7_KEY62,
#endif
#if ( MUTUAL7_NUM > 63 )
    CTSUSSC_MUTUAL7_KEY63,
#endif
};

const uint16_t g_mutual7_ctsuso0[] =
{
#if ( MUTUAL7_NUM > 0 )
    CTSUSO0_MUTUAL7_KEY00,
#endif
#if ( MUTUAL7_NUM > 1 )
    CTSUSO0_MUTUAL7_KEY01,
#endif
#if ( MUTUAL7_NUM > 2 )
    CTSUSO0_MUTUAL7_KEY02,
#endif
#if ( MUTUAL7_NUM > 3 )
    CTSUSO0_MUTUAL7_KEY03,
#endif
#if ( MUTUAL7_NUM > 4 )
    CTSUSO0_MUTUAL7_KEY04,
#endif
#if ( MUTUAL7_NUM > 5 )
    CTSUSO0_MUTUAL7_KEY05,
#endif
#if ( MUTUAL7_NUM > 6 )
    CTSUSO0_MUTUAL7_KEY06,
#endif
#if ( MUTUAL7_NUM > 7 )
    CTSUSO0_MUTUAL7_KEY07,
#endif
#if ( MUTUAL7_NUM > 8 )
    CTSUSO0_MUTUAL7_KEY08,
#endif
#if ( MUTUAL7_NUM > 9 )
    CTSUSO0_MUTUAL7_KEY09,
#endif
#if ( MUTUAL7_NUM > 10 )
    CTSUSO0_MUTUAL7_KEY10,
#endif
#if ( MUTUAL7_NUM > 11 )
    CTSUSO0_MUTUAL7_KEY11,
#endif
#if ( MUTUAL7_NUM > 12 )
    CTSUSO0_MUTUAL7_KEY12,
#endif
#if ( MUTUAL7_NUM > 13 )
    CTSUSO0_MUTUAL7_KEY13,
#endif
#if ( MUTUAL7_NUM > 14 )
    CTSUSO0_MUTUAL7_KEY14,
#endif
#if ( MUTUAL7_NUM > 15 )
    CTSUSO0_MUTUAL7_KEY15,
#endif
#if ( MUTUAL7_NUM > 16 )
    CTSUSO0_MUTUAL7_KEY16,
#endif
#if ( MUTUAL7_NUM > 17 )
    CTSUSO0_MUTUAL7_KEY17,
#endif
#if ( MUTUAL7_NUM > 18 )
    CTSUSO0_MUTUAL7_KEY18,
#endif
#if ( MUTUAL7_NUM > 19 )
    CTSUSO0_MUTUAL7_KEY19,
#endif
#if ( MUTUAL7_NUM > 20 )
    CTSUSO0_MUTUAL7_KEY20,
#endif
#if ( MUTUAL7_NUM > 21 )
    CTSUSO0_MUTUAL7_KEY21,
#endif
#if ( MUTUAL7_NUM > 22 )
    CTSUSO0_MUTUAL7_KEY22,
#endif
#if ( MUTUAL7_NUM > 23 )
    CTSUSO0_MUTUAL7_KEY23,
#endif
#if ( MUTUAL7_NUM > 24 )
    CTSUSO0_MUTUAL7_KEY24,
#endif
#if ( MUTUAL7_NUM > 25 )
    CTSUSO0_MUTUAL7_KEY25,
#endif
#if ( MUTUAL7_NUM > 26 )
    CTSUSO0_MUTUAL7_KEY26,
#endif
#if ( MUTUAL7_NUM > 27 )
    CTSUSO0_MUTUAL7_KEY27,
#endif
#if ( MUTUAL7_NUM > 28 )
    CTSUSO0_MUTUAL7_KEY28,
#endif
#if ( MUTUAL7_NUM > 29 )
    CTSUSO0_MUTUAL7_KEY29,
#endif
#if ( MUTUAL7_NUM > 30 )
    CTSUSO0_MUTUAL7_KEY30,
#endif
#if ( MUTUAL7_NUM > 31 )
    CTSUSO0_MUTUAL7_KEY31,
#endif
#if ( MUTUAL7_NUM > 32 )
    CTSUSO0_MUTUAL7_KEY32,
#endif
#if ( MUTUAL7_NUM > 33 )
    CTSUSO0_MUTUAL7_KEY33,
#endif
#if ( MUTUAL7_NUM > 34 )
    CTSUSO0_MUTUAL7_KEY34,
#endif
#if ( MUTUAL7_NUM > 35 )
    CTSUSO0_MUTUAL7_KEY35,
#endif
#if ( MUTUAL7_NUM > 36 )
    CTSUSO0_MUTUAL7_KEY36,
#endif
#if ( MUTUAL7_NUM > 37 )
    CTSUSO0_MUTUAL7_KEY37,
#endif
#if ( MUTUAL7_NUM > 38 )
    CTSUSO0_MUTUAL7_KEY38,
#endif
#if ( MUTUAL7_NUM > 39 )
    CTSUSO0_MUTUAL7_KEY39,
#endif
#if ( MUTUAL7_NUM > 40 )
    CTSUSO0_MUTUAL7_KEY40,
#endif
#if ( MUTUAL7_NUM > 41 )
    CTSUSO0_MUTUAL7_KEY41,
#endif
#if ( MUTUAL7_NUM > 42 )
    CTSUSO0_MUTUAL7_KEY42,
#endif
#if ( MUTUAL7_NUM > 43 )
    CTSUSO0_MUTUAL7_KEY43,
#endif
#if ( MUTUAL7_NUM > 44 )
    CTSUSO0_MUTUAL7_KEY44,
#endif
#if ( MUTUAL7_NUM > 45 )
    CTSUSO0_MUTUAL7_KEY45,
#endif
#if ( MUTUAL7_NUM > 46 )
    CTSUSO0_MUTUAL7_KEY46,
#endif
#if ( MUTUAL7_NUM > 47 )
    CTSUSO0_MUTUAL7_KEY47,
#endif
#if ( MUTUAL7_NUM > 48 )
    CTSUSO0_MUTUAL7_KEY48,
#endif
#if ( MUTUAL7_NUM > 49 )
    CTSUSO0_MUTUAL7_KEY49,
#endif
#if ( MUTUAL7_NUM > 50 )
    CTSUSO0_MUTUAL7_KEY50,
#endif
#if ( MUTUAL7_NUM > 51 )
    CTSUSO0_MUTUAL7_KEY51,
#endif
#if ( MUTUAL7_NUM > 52 )
    CTSUSO0_MUTUAL7_KEY52,
#endif
#if ( MUTUAL7_NUM > 53 )
    CTSUSO0_MUTUAL7_KEY53,
#endif
#if ( MUTUAL7_NUM > 54 )
    CTSUSO0_MUTUAL7_KEY54,
#endif
#if ( MUTUAL7_NUM > 55 )
    CTSUSO0_MUTUAL7_KEY55,
#endif
#if ( MUTUAL7_NUM > 56 )
    CTSUSO0_MUTUAL7_KEY56,
#endif
#if ( MUTUAL7_NUM > 57 )
    CTSUSO0_MUTUAL7_KEY57,
#endif
#if ( MUTUAL7_NUM > 58 )
    CTSUSO0_MUTUAL7_KEY58,
#endif
#if ( MUTUAL7_NUM > 59 )
    CTSUSO0_MUTUAL7_KEY59,
#endif
#if ( MUTUAL7_NUM > 60 )
    CTSUSO0_MUTUAL7_KEY60,
#endif
#if ( MUTUAL7_NUM > 61 )
    CTSUSO0_MUTUAL7_KEY61,
#endif
#if ( MUTUAL7_NUM > 62 )
    CTSUSO0_MUTUAL7_KEY62,
#endif
#if ( MUTUAL7_NUM > 63 )
    CTSUSO0_MUTUAL7_KEY63,
#endif
};

const uint16_t g_mutual7_ctsuso1[] =
{
#if ( MUTUAL7_NUM > 0 )
    CTSUSO1_MUTUAL7_KEY00,
#endif
#if ( MUTUAL7_NUM > 1 )
    CTSUSO1_MUTUAL7_KEY01,
#endif
#if ( MUTUAL7_NUM > 2 )
    CTSUSO1_MUTUAL7_KEY02,
#endif
#if ( MUTUAL7_NUM > 3 )
    CTSUSO1_MUTUAL7_KEY03,
#endif
#if ( MUTUAL7_NUM > 4 )
    CTSUSO1_MUTUAL7_KEY04,
#endif
#if ( MUTUAL7_NUM > 5 )
    CTSUSO1_MUTUAL7_KEY05,
#endif
#if ( MUTUAL7_NUM > 6 )
    CTSUSO1_MUTUAL7_KEY06,
#endif
#if ( MUTUAL7_NUM > 7 )
    CTSUSO1_MUTUAL7_KEY07,
#endif
#if ( MUTUAL7_NUM > 8 )
    CTSUSO1_MUTUAL7_KEY08,
#endif
#if ( MUTUAL7_NUM > 9 )
    CTSUSO1_MUTUAL7_KEY09,
#endif
#if ( MUTUAL7_NUM > 10 )
    CTSUSO1_MUTUAL7_KEY10,
#endif
#if ( MUTUAL7_NUM > 11 )
    CTSUSO1_MUTUAL7_KEY11,
#endif
#if ( MUTUAL7_NUM > 12 )
    CTSUSO1_MUTUAL7_KEY12,
#endif
#if ( MUTUAL7_NUM > 13 )
    CTSUSO1_MUTUAL7_KEY13,
#endif
#if ( MUTUAL7_NUM > 14 )
    CTSUSO1_MUTUAL7_KEY14,
#endif
#if ( MUTUAL7_NUM > 15 )
    CTSUSO1_MUTUAL7_KEY15,
#endif
#if ( MUTUAL7_NUM > 16 )
    CTSUSO1_MUTUAL7_KEY16,
#endif
#if ( MUTUAL7_NUM > 17 )
    CTSUSO1_MUTUAL7_KEY17,
#endif
#if ( MUTUAL7_NUM > 18 )
    CTSUSO1_MUTUAL7_KEY18,
#endif
#if ( MUTUAL7_NUM > 19 )
    CTSUSO1_MUTUAL7_KEY19,
#endif
#if ( MUTUAL7_NUM > 20 )
    CTSUSO1_MUTUAL7_KEY20,
#endif
#if ( MUTUAL7_NUM > 21 )
    CTSUSO1_MUTUAL7_KEY21,
#endif
#if ( MUTUAL7_NUM > 22 )
    CTSUSO1_MUTUAL7_KEY22,
#endif
#if ( MUTUAL7_NUM > 23 )
    CTSUSO1_MUTUAL7_KEY23,
#endif
#if ( MUTUAL7_NUM > 24 )
    CTSUSO1_MUTUAL7_KEY24,
#endif
#if ( MUTUAL7_NUM > 25 )
    CTSUSO1_MUTUAL7_KEY25,
#endif
#if ( MUTUAL7_NUM > 26 )
    CTSUSO1_MUTUAL7_KEY26,
#endif
#if ( MUTUAL7_NUM > 27 )
    CTSUSO1_MUTUAL7_KEY27,
#endif
#if ( MUTUAL7_NUM > 28 )
    CTSUSO1_MUTUAL7_KEY28,
#endif
#if ( MUTUAL7_NUM > 29 )
    CTSUSO1_MUTUAL7_KEY29,
#endif
#if ( MUTUAL7_NUM > 30 )
    CTSUSO1_MUTUAL7_KEY30,
#endif
#if ( MUTUAL7_NUM > 31 )
    CTSUSO1_MUTUAL7_KEY31,
#endif
#if ( MUTUAL7_NUM > 32 )
    CTSUSO1_MUTUAL7_KEY32,
#endif
#if ( MUTUAL7_NUM > 33 )
    CTSUSO1_MUTUAL7_KEY33,
#endif
#if ( MUTUAL7_NUM > 34 )
    CTSUSO1_MUTUAL7_KEY34,
#endif
#if ( MUTUAL7_NUM > 35 )
    CTSUSO1_MUTUAL7_KEY35,
#endif
#if ( MUTUAL7_NUM > 36 )
    CTSUSO1_MUTUAL7_KEY36,
#endif
#if ( MUTUAL7_NUM > 37 )
    CTSUSO1_MUTUAL7_KEY37,
#endif
#if ( MUTUAL7_NUM > 38 )
    CTSUSO1_MUTUAL7_KEY38,
#endif
#if ( MUTUAL7_NUM > 39 )
    CTSUSO1_MUTUAL7_KEY39,
#endif
#if ( MUTUAL7_NUM > 40 )
    CTSUSO1_MUTUAL7_KEY40,
#endif
#if ( MUTUAL7_NUM > 41 )
    CTSUSO1_MUTUAL7_KEY41,
#endif
#if ( MUTUAL7_NUM > 42 )
    CTSUSO1_MUTUAL7_KEY42,
#endif
#if ( MUTUAL7_NUM > 43 )
    CTSUSO1_MUTUAL7_KEY43,
#endif
#if ( MUTUAL7_NUM > 44 )
    CTSUSO1_MUTUAL7_KEY44,
#endif
#if ( MUTUAL7_NUM > 45 )
    CTSUSO1_MUTUAL7_KEY45,
#endif
#if ( MUTUAL7_NUM > 46 )
    CTSUSO1_MUTUAL7_KEY46,
#endif
#if ( MUTUAL7_NUM > 47 )
    CTSUSO1_MUTUAL7_KEY47,
#endif
#if ( MUTUAL7_NUM > 48 )
    CTSUSO1_MUTUAL7_KEY48,
#endif
#if ( MUTUAL7_NUM > 49 )
    CTSUSO1_MUTUAL7_KEY49,
#endif
#if ( MUTUAL7_NUM > 50 )
    CTSUSO1_MUTUAL7_KEY50,
#endif
#if ( MUTUAL7_NUM > 51 )
    CTSUSO1_MUTUAL7_KEY51,
#endif
#if ( MUTUAL7_NUM > 52 )
    CTSUSO1_MUTUAL7_KEY52,
#endif
#if ( MUTUAL7_NUM > 53 )
    CTSUSO1_MUTUAL7_KEY53,
#endif
#if ( MUTUAL7_NUM > 54 )
    CTSUSO1_MUTUAL7_KEY54,
#endif
#if ( MUTUAL7_NUM > 55 )
    CTSUSO1_MUTUAL7_KEY55,
#endif
#if ( MUTUAL7_NUM > 56 )
    CTSUSO1_MUTUAL7_KEY56,
#endif
#if ( MUTUAL7_NUM > 57 )
    CTSUSO1_MUTUAL7_KEY57,
#endif
#if ( MUTUAL7_NUM > 58 )
    CTSUSO1_MUTUAL7_KEY58,
#endif
#if ( MUTUAL7_NUM > 59 )
    CTSUSO1_MUTUAL7_KEY59,
#endif
#if ( MUTUAL7_NUM > 60 )
    CTSUSO1_MUTUAL7_KEY60,
#endif
#if ( MUTUAL7_NUM > 61 )
    CTSUSO1_MUTUAL7_KEY61,
#endif
#if ( MUTUAL7_NUM > 62 )
    CTSUSO1_MUTUAL7_KEY62,
#endif
#if ( MUTUAL7_NUM > 63 )
    CTSUSO1_MUTUAL7_KEY63,
#endif
};
#endif  // ( MUTUAL_METHOD_NUM > 7 )

/***********************************************************************************************************************
* End of File
***********************************************************************************************************************/