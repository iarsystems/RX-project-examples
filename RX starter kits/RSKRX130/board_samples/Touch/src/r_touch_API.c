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
* File Name    : r_touch_API.c
* Version      : 1.00
* Device(s)    : R5F5113xAxFP,R5F5231xAxFP,R5F51305AxFN
* Description  : This file is the touch API.
***********************************************************************************************************************/

/***********************************************************************************************************************
* History      : DD.MM.YYYY Version    Description
*              : xx.xx.2015   1.00     First Release
***********************************************************************************************************************/

#define __TOUCH_API_C__
/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
/* System include header */
#include <string.h>
#include "r_cg_macrodriver.h"

/* H/W include header */
#include "iodefine.h"
#include "r_mpc.h"
#include "r_dtc.h"
#include "r_ctsu.h"
#include "r_cgc.h"

/* S/W include header */
#include "r_cg_userdefine.h"
#include "r_touch_API.h"
#include "r_touch.h"

/***********************************************************************************************************************
* Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
* Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
* Exported global variables
***********************************************************************************************************************/

/***********************************************************************************************************************
* Private global variables and functions
***********************************************************************************************************************/
    uint8_t tune_check;
/***********************************************************************************************************************
* Function Name: R_Set_Cap_Touch_Create
* Description  : Capacitive Touch Sensor create API
* Arguments    : uint8_t method    : Measurement method(0-8)
* Return Value : None
***********************************************************************************************************************/
uint8_t R_Set_Cap_Touch_Create( uint8_t method )
{
    uint8_t  status;
    uint16_t ret_val;

    status = CREATE_ERROR;

    touch_parameter_address_set();                               /* Touch parameter top address setting               */
    ctsu_sensor_average_address_set();                           /* CTSU parameter top address setting                */
    CTSU_register_initial_value_ram_set();                       /* CTSU register value ram setting                   */
    DTC_Set_Initial_of_CTSU( method );                           /* Default method DTC setting                        */
    ret_val = R_Set_CTSU_All_Register( method );                 /* Default method CTSU register setting              */
    if (0x0000 == ret_val)
    {
        DTC_transmit_data_set();                                 /* DTC transmit data settintg                        */
        touch_parameter_set();                                   /* Touch parameter setting                           */
        R_Set_Cap_Touch_Tuning_Cntrol( TUNING_START );
//        R_Set_Cap_Touch_Tuning_Cntrol( TUNING_STOP );
        R_Set_Cap_Touch_Offset_Timing( method, g_offset_time[method] );
        status = CREATE_SUCCESS;
    }

    return status;
}    /* End of function R_Set_Cap_Touch_Create() */

/***********************************************************************************************************************
* Function Name: R_Set_Cap_Touch_Next_Method_Change
* Description  : Measurement method change API
* Arguments    : uint8_t method    : Maeasurement method(0-8)
* Return Value : None
***********************************************************************************************************************/
uint8_t R_Set_Cap_Touch_Next_Method_Change( uint8_t method )
{
    if ((CTSU_FINISH_MODE == g_ctsu_soft_mode) && (0 == g_ctsu_status[method].flag.data_update))
    {
        method = method + 1;
        if(METHOD_NUM <= method)
        {
            method = 0;
        }
        R_Set_CTSU_All_Register( method );
        g_ctsu_soft_mode = CTSU_READY_MODE;
    }
    return method;
}    /* End of function R_Set_Cap_Touch_Next_Method_Change() */

/***********************************************************************************************************************
* Function Name: R_Set_Cap_Touch_Measurement_Start
* Description  : Capacitive touch sensing start API
* Arguments    : uint8_t method    : Maeasurement method(0-8)
* Return Value : None
***********************************************************************************************************************/
void R_Set_Cap_Touch_Measurement_Start( uint8_t method )
{
    uint8_t tune_sta;

    tune_sta = 0;

    if ((TOUCH_MAIN_CYCLE_US / 1000) > g_touch_main_cycle)
    {
        g_touch_main_cycle += TOUCH_MAIN_TIMING_MS;
    }

    if ((CTSU_READY_MODE == g_ctsu_soft_mode) && ((TOUCH_MAIN_CYCLE_US / 1000) <= g_touch_main_cycle))
    {
        g_touch_main_cycle = 0;
        g_method_stor = method;

        if ((1 == g_ctsu_status[method].flag.ctsu_measure) && (0 == g_ctsu_status[method].flag.icomp_error) && (1 == g_touch_function[method].flag.tuning))
        {
            touch_data_moving_average( method );                     /* Make Touch count value */
        }

        if (1 == g_touch_system.flag.initial)
        {
            tune_sta = offset_tuning_stop_judgement( method );
            if (OFF == tune_sta)
            {
                touch_internal_tuning( method, g_offset_time[method]);                            /* Auto Tuning */
            }
        }

        DTC_Set_Start_of_CTSU( method );                          /* set CTSU Start proc.*/
        g_ctsu_soft_mode = CTSU_RUN_MODE;
        R_Set_CTSU_Measurement_Start( 0x01,0x00 );
    }
}    /* End of function R_Set_Cap_Touch_Measurement_Start() */

/***********************************************************************************************************************
* Function Name: R_Get_Cap_Touch_Initial_Status
* Description  : 
* Arguments    : 
* Return Value : 
***********************************************************************************************************************/
uint8_t R_Get_Cap_Touch_Initial_Status( void )
{
    uint8_t status;

    if (0 == g_touch_system.flag.initial)
    {
        status = INITIAL_NOW;
    }
    else
    {
        status = INITIAL_END;
    }
    return status;
}    /* End of function R_Get_Cap_Touch_Initial_Status() */

/***********************************************************************************************************************
* Function Name: R_Set_Cap_Touch_Initial_Tuning
* Description  : Capacitive touch sensor counter initial offset tuning API
* Arguments    : uint8_t method    : Maeasurement method : 0-8
* Return Value : uint8_t status    : TUNE_CONTINUE : 0
*              :                   : TUNE_COMPLETE : 1
***********************************************************************************************************************/
uint8_t R_Set_Cap_Touch_Initial_Tuning( uint8_t method )
{
    uint8_t    offset_status;
    uint8_t    initial_status;
    uint8_t    id;

    if (TUNING_STOP != g_ctsu_offset_mode)
    {
        offset_status = initial_offset_tuning( method , g_key_info[method].ena_num);
        initial_status = touch_calibration_check( method, offset_status );
    }
    else
    {
        for (id = 0; id < METHOD_NUM; id++)
        {
            g_ctsu_status[method].flag.data_update = 0;
            g_touch_function[method].flag.tuning = 1;
        }
        offset_status = 1;
        initial_status = touch_calibration_check( method, offset_status );
    }

    return initial_status;
}    /* End of function R_Set_Cap_Touch_Initial_Tuning() */

/***********************************************************************************************************************
* Function Name: R_Set_Cap_Touch_Offset_Timing
* Description  : 
* Arguments    : uint8_t mode            0 : Internal tuning stop
*              :                         1 : Internal tuning start
*              : uint16_t count          0 : Set Error
*              :                   1-65535 : Internal tuning timing
* Return Value : none
***********************************************************************************************************************/
uint8_t R_Set_Cap_Touch_Offset_Timing( uint8_t method, uint16_t count )
{
    uint8_t status;

    if (TUNING_START == g_ctsu_offset_mode)
    {
        g_tuning_counter[method] = 0;
        g_offset_time[method]    = count;
        status                   = SET_OK;
    }
    else
    {
        status                   = SET_NG;
    }

    return status;
}    /* End of function R_Set_Cap_Touch_Offset_Timing() */

/***********************************************************************************************************************
* Function Name: R_Get_Cap_Touch_Data_Check
* Description  : Capacitive touch measurement data get check API
* Arguments    : uint8_t method    : Maeasurement method : 0-8
* Return Value : uint8_t status    : GET_OK                   : 0x00
*              :                   : GET_NG (Not data updata) : 0x01
*              :                   : SENS_CNT_OVER_NG         : 0x02
*              :                   : REF_CNT_OVER_NG          : 0x03
*              :                   : SENS_REF_CNT_OVER_NG     : 0x04
*              :                   : TSCAP_ERR_NG             : 0x05
***********************************************************************************************************************/
uint8_t R_Get_Cap_Touch_Data_Check( uint8_t method )
{
    uint8_t    status;

    if (1 == g_ctsu_status[method].flag.data_update)             /* Data get OK (non-error)                           */
    {
        status = GET_OK;                                         /* 0x00                                              */
    }
    else if ((1 == g_ctsu_status[method].flag.sens_over) && (1 == g_ctsu_status[method].flag.ref_over))
    {
        status = SENS_REF_CNT_OVER_NG;                           /* 0x04                                              */
    }
    else if (1 == g_ctsu_status[method].flag.sens_over)          /* Sensor counter over error                         */
    {
        status = SENS_CNT_OVER_NG;                               /* 0x02                                              */
    }
    else if (1 == g_ctsu_status[method].flag.ref_over)           /* Reference counter over error                      */
    {
        status = REF_CNT_OVER_NG;                                /* 0x03                                              */
    }
    else if (1 == g_ctsu_status[method].flag.icomp_error)        /* TSCAP voltage error                               */
    {
        status = TSCAP_ERR_NG;                                   /* 0x05                                              */
    }
    else                                                         /* Not data updata                                   */
    {
        status = GET_NG;                                         /* 0x01                                              */
    }
    return status;
}    /* End of function R_Get_Cap_Touch_Data_Check() */

/***********************************************************************************************************************
* Function Name: R_Get_Cap_Touch_Sensor_Data
* Description  : Sensor data get
* Arguments    : uint8_t method    : Maeasurement method : 0-8
*              : uint8_t mode
* Return Value : 
***********************************************************************************************************************/
uint16_t R_Get_Cap_Touch_Sensor_Data( uint8_t method, uint8_t mode, uint8_t index_num )
{
    uint16_t data;

    switch (mode)
    {
        case BUTTON:
            data = *(g_self_sensor_cnt_pt[method] + (*(g_key_info[method].sensor_index + index_num)));
            break;
        case SLIDER:
            data = *(g_self_sensor_cnt_pt[method] + index_num);
            break;
        case WHEEL:
            data = *(g_self_sensor_cnt_pt[method] + index_num);
            break;
        case MATRIX:
            data = *(g_mutual_sensor_diff_pt[method] + index_num);
            break;
        default:
            data = 0xFFFF;
            break;
    }
    return data;
}    /* End of function R_Get_Touch_Cap_Sensor_Data() */

/***********************************************************************************************************************
* Function Name: R_Set_Cap_Touch_Result_Create
* Description  : 
* Arguments    : uint8_t method    : Maeasurement method : 0-8
* Return Value : uint8_t status    : CREATE_SUCCESS(0)
*              :                   : CREATE_ERROR(1)
***********************************************************************************************************************/
uint16_t R_Set_Cap_Touch_Result_Create( uint8_t method )
{
    uint8_t  loop;
#ifdef SLIDER_USE
    uint8_t     slider_id;
#endif    // SLIDER_USE
#ifdef WHEEL_USE
    uint8_t     wheel_id;
#endif    // WHEEL_USE
    uint16_t sensor_val;
    uint16_t status;

    status = CREATE_ERROR;

    if (0 == g_key_info[method].mode)
    {
        if (0 != g_key_info[method].key_num)
        {
            for (loop = 0; loop < MAX_TS; loop++)
            {
                if (KEY_ENABLE == touch_key_function_check( method, loop))
                {
                    sensor_val = R_Get_Cap_Touch_Sensor_Data(method, BUTTON, loop);
                    touch_key_decode(method, sensor_val, loop);
                }
            }

            for (loop = 0; loop < g_key_info[method].key_max_group; loop++)
            {
                g_touch_all_result[method].button[loop]  = *(g_key_info[method].touch_result  + loop);
                status = CREATE_SUCCESS;
            }
        }

#ifdef SLIDER_USE
        if (0 != SLIDER_NUMBER)
        {
            for (slider_id = 0; slider_id < SLIDER_NUMBER; slider_id++)
            {
                for (loop = 0; loop < g_sliderInfo[slider_id].num; loop++)
                {
                    sensor_val = R_Get_Cap_Touch_Sensor_Data(method, SLIDER, g_slider_index[slider_id].data_num[loop]);
                    g_slider_data[loop]  = sensor_val;
                }
                g_slider_pos[slider_id] = slider_decode( slider_id );
                g_touch_all_result[method].slider[slider_id] = g_slider_pos[slider_id];
                status = CREATE_SUCCESS;
            }
        }
#endif    // SLIDER_USE

#ifdef WHEEL_USE
        if (0 != WHEEL_NUMBER)
        {
            for (wheel_id = 0; wheel_id < WHEEL_NUMBER; wheel_id++)
            {
                for (loop = 0; loop < g_wheelInfo[wheel_id].num; loop++)
                {
                    sensor_val = R_Get_Cap_Touch_Sensor_Data(method, WHEEL, g_wheel_index[wheel_id].data_num[loop]);
                    g_wheel_data[loop] = sensor_val;
                }
                g_wheel_pos[wheel_id]  = wheel_decode( wheel_id );
                g_touch_all_result[method].wheel[wheel_id] = g_wheel_pos[wheel_id];
                status = CREATE_SUCCESS;
            }
        }
#endif    // WHEEL_USE

    }
    else
    {
        if (0 != g_key_info[method].key_num)
        {
            for (loop = 0; loop < g_key_info[method].ena_num; loop++)
            {
                sensor_val = R_Get_Cap_Touch_Sensor_Data(method, MATRIX, loop);
                touch_key_decode(method, sensor_val, loop);
            }

            for (loop = 0; loop < g_key_info[method].key_max_group; loop++)
            {
                g_touch_all_result[method].matrix[loop]  = *(g_key_info[method].touch_result  + loop);
                status = CREATE_SUCCESS;
            }
        }
    }
    g_ctsu_status[method].flag.data_update = 0;

    return status;
}    /* End of function R_Set_Cap_Touch_Result_Create() */

/***********************************************************************************************************************
* Function Name: R_Get_Cap_Touch_Result
* Description  : 
* Arguments    : uint8_t method    : Maeasurement method : 0-8
* Return Value : uint8_t status    : CREATE_SUCCESS(0)
*              :                   : CREATE_ERROR(1)
***********************************************************************************************************************/
touch_result_t R_Get_Cap_Touch_Result( uint8_t method )
{
    return g_touch_all_result[method];
}

/***********************************************************************************************************************
* Function Name: R_Get_Cap_Touch_Reference_Data
* Description  : Reference data get
* Arguments    : ---
* Return Value : 
***********************************************************************************************************************/
uint16_t R_Get_Cap_Touch_Reference_Data( uint8_t method, uint8_t index_num )
{
    if (0xFF != *(g_key_info[method].sensor_index + index_num))
    {
        return  (*g_key_info[method].ref + (*(g_key_info[method].sensor_index + index_num)));
    }
    else
    {
        return DATA_GET_ERROR;
    }
}    /* End of function R_Get_Cap_Touch_Reference_Data() */

/***********************************************************************************************************************
* Function Name: R_Set_Cap_Touch_Tuning_Cntrol
* Description  : Offset tuning start or stop setting
* Arguments    : uint8_t mode    : TUNING_STOP  (0)
*              :                 : TUNING_START (1)
*              :                 : Others
* Return Value : uint8_t status  : SET_OK (0)
*              :                 : SET_NG (1)
***********************************************************************************************************************/
uint8_t R_Set_Cap_Touch_Tuning_Cntrol( uint8_t mode )
{
    uint8_t status;

    if (TUNING_STOP == mode)
    {
        g_ctsu_offset_mode = TUNING_STOP;
        status = SET_OK;
    }
    else if (TUNING_START == mode)
    {
        g_ctsu_offset_mode = TUNING_START;
        status = SET_OK;
    }
    else
    {
        status = SET_NG;
    }

    return status;
}    /* End of function R_Set_Cap_Touch_Tuning_Cntrol() */

/***********************************************************************************************************************
* End of File
***********************************************************************************************************************/
