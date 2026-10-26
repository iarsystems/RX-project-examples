/******************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only
 * intended for use with Renesas products. No other uses are authorized. This
 * software is owned by Renesas Electronics Corporation and is protected under
 * all applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
 * LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
 * TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
 * ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
 * FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
 * ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
 * BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software
 * and to discontinue the availability of this software. By using this software,
 * you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *******************************************************************************
 * Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.    */
/******************************************************************************
 * File Name    : touchscreen.c
 * Version      : 1.00
 * Device(s)    : Renesas
 * Tool-Chain   : RXC
 * OS           : None
 * H/W Platform : RPB
 * Description  : Routines for detecting and locating presses on the resistive
 *                touch screen.
 *******************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 18.04.2016 1.00 First Release
 ******************************************************************************/

/******************************************************************************
 WARNING!  IN ACCORDANCE WITH THE USER LICENCE THIS CODE MUST NOT BE CONVEYED
 OR REDISTRIBUTED IN COMBINATION WITH ANY SOFTWARE LICENSED UNDER TERMS THE
 SAME AS OR SIMILAR TO THE GNU GENERAL PUBLIC LICENCE
 ******************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include "platform.h"
#include "Global.h"
#include "LCD_demo.h"
#include "task.h"
#include "touchscreen.h"
#include "ad_dtc_RX71M.h"

/******************************************************************************
 Macro Definitions
 ******************************************************************************/
#define TOUCH_PANEL_WIDTH         (DD_H_DOT_DISPLAY)
#define TOUCH_PANEL_HEIGHT        (DD_V_LINES_DISPLAY)
#define DEBOUNCE_TIME (5)

/******************************************************************************
 Enumerated Types
 ******************************************************************************/
typedef enum
{
    TC_LL, TC_LR, TC_UL, TC_UR
} touch_coords_t;

/******************************************************************************
 Private global variables and functions
 ******************************************************************************/
static _Bool check_touch (void);

DDLCD_Point_t g_touch_calibration[4] = /* calibration points 0=LL, 1=LR, 2=UL, 3=UR */
{
{ TC_LL_X, TC_LL_Y },
{ TC_LR_X, TC_LR_Y },
{ TC_UL_X, TC_UL_Y },
{ TC_UR_X, TC_UR_Y } };

/******************************************************************************
 Public Functions
 ******************************************************************************/

/*****************************************************************************
 Function Name: r_touch_init
 Description:   Initialize the touch panel.  Set up the I/O's and ADC
 Parameters:    None
 Return value:  None
 *****************************************************************************/
void r_touch_init (void)
{
    PINo(OUT_TS_XR) = 0;
    PINo(OUT_TS_XL) = 0;
    PINo(OUT_TS_YU) = 0;
    PINo(OUT_TS_YL) = 0;
    DDRo(OUT_TS_XR);
    DDRo(OUT_TS_XL);
    DDRo(OUT_TS_YU);
    DDRo(OUT_TS_YL);

    /* configure DTC to acquire multiple A/D samples */
    ad_init();
}
/*****************************************************************************
 End of function  r_touch_init
 ******************************************************************************/

/*****************************************************************************
 Function Name: r_touch_scantouch
 Description:   Scan the touch pad to see if it's been touched.
 Parameters:    pointer to a "point"
 Return value:  Message: TOUCH, NO_TOUCH or MOVE_TO
 *****************************************************************************/
MessageEvent_t r_touch_scantouch (DDLCD_Point_t *pt)
{
    static MessageEvent_t pressed = ME_RELEASE;
    _Bool touched = true;
    static _Bool touched_old = true;
    static portTickType touched_time;
    portTickType dtime;
    DDLCD_Point_t ad_new;
    static DDLCD_Point_t touch_point;

    touched = r_touch_getraw(&ad_new);

    /* Ensure rational readings */
    {
        int16_t index;
        DDLCD_Point_t test_min;
        DDLCD_Point_t test_max;

        /* test min x, y */
        for (index = 1, test_max = test_min = g_touch_calibration[0]; index < size_ra(g_touch_calibration); index++)
        {
            test_min.x = (test_min.x < g_touch_calibration[index].x) ? test_min.x : g_touch_calibration[index].x;
            test_min.y = (test_min.y < g_touch_calibration[index].y) ? test_min.y : g_touch_calibration[index].y;
            test_max.x = (test_max.x > g_touch_calibration[index].x) ? test_max.x : g_touch_calibration[index].x;
            test_max.y = (test_max.y > g_touch_calibration[index].y) ? test_max.y : g_touch_calibration[index].y;
        }
        if ((((ad_new.x < test_min.x) || (ad_new.y < test_min.y)) || (ad_new.x > test_max.x))
                || (ad_new.y > test_max.y))
        {
            touched = false;
        }
    }

    /* reset timeout if there's a transition */
    if (touched != touched_old)
    {
        touched_time = xTaskGetTickCount(); /* RTOS_USAGE */
        touched_old = touched;
    }
    {

        /* check debounce timeout for reporting */
        dtime = xTaskGetTickCount() - touched_time; /* RTOS_USAGE */
        dtime = port_TICK_to_mS(dtime); /* RTOS_USAGE */
        if (dtime < DEBOUNCE_TIME)
        {
            return (ME_INVALID);
        }
        else
        {
            touched_time = xTaskGetTickCount(); /* RTOS_USAGE */
        }
    }
    {
        /* Convert raw samples to Panel coordinates */
        /* get proportional location on screen...top/bottom/left/right */
        int32_t x_u;
        int32_t x_l;
        int32_t y_l;
        int32_t y_r;
        x_u = ((ad_new.x - g_touch_calibration[TC_UL].x) * 0x10000L)
                / (g_touch_calibration[TC_UR].x - g_touch_calibration[TC_UL].x);
        x_l = ((ad_new.x - g_touch_calibration[TC_LL].x) * 0x10000L)
                / (g_touch_calibration[TC_LR].x - g_touch_calibration[TC_LL].x);
        y_l = ((ad_new.y - g_touch_calibration[TC_LL].y) * 0x10000L)
                / (g_touch_calibration[TC_UL].y - g_touch_calibration[TC_LL].y);
        y_r = ((ad_new.y - g_touch_calibration[TC_LR].y) * 0x10000L)
                / (g_touch_calibration[TC_UR].y - g_touch_calibration[TC_LR].y);

        /* factor in location on screen to utilize 4 pt calibration data */
        touch_point.x = (int16_t) ((((((x_u - x_l) * ((y_l + y_r) / 2)) >> 16) + x_l) * TOUCH_PANEL_WIDTH) >> 16);
        touch_point.y = (int16_t) ((((((y_r - y_l) * ((x_l + x_u) / 2)) >> 16) + y_l) * TOUCH_PANEL_HEIGHT) >> 16);
        touch_point.x = (touch_point.x < 0) ? 0 : touch_point.x;
        touch_point.x = (touch_point.x >= TOUCH_PANEL_WIDTH) ? (TOUCH_PANEL_WIDTH - 1) : touch_point.x;
        touch_point.y = (touch_point.y < 0) ? 0 : touch_point.y;
        touch_point.y = (touch_point.y >= TOUCH_PANEL_HEIGHT) ? (TOUCH_PANEL_HEIGHT - 1) : touch_point.y;

        /* Note: Compensation for screen fliping is accomplished in the touch screen calibration values.   */
    }

    if (true == touched)
    {
        if (pressed == ME_RELEASE)
        {
            pressed = (ME_PRESS);
        }
        else
        {
            pressed = ME_MOVE;
        }
        *pt = touch_point;
    }
    else
    {
        if (pressed != ME_RELEASE)
        {
            pressed = ME_RELEASE;
        }
        else
        {
            return (ME_INVALID);
        }
    }
    return (pressed);
}
/*****************************************************************************
 End of function  r_touch_scantouch
 ******************************************************************************/

/*****************************************************************************
 Function Name: r_touch_getraw
 Description:   Read raw status of the touch screen.
 Parameters:    pointer to a "point"
 Return value:  0=no touch, 1=touched
 *****************************************************************************/
_Bool r_touch_getraw (DDLCD_Point_t *pRaw)
{
    _Bool touched = true;

    /* Check if screen is being touched */
    if (false == check_touch())
    {
        touched = false;
    }

    /* Read axis values...even if not touched to keep timing consistent */
    /* Assert Y axis drivers */
    PINo(OUT_TS_YU) = 1;
    PINo(OUT_TS_YL) = 1;

    /* Wait for inputs to settle */
    vTaskDelay(1);

    /* Read Y axis inputs */
    ad_acquire();
    pRaw->y = (int16_t) ad_value(ADC_TS_XL) + (int16_t) ad_value(ADC_TS_XR);

    /* De-assert Y axis drivers */
    PINo(OUT_TS_YU) = 0;
    PINo(OUT_TS_YL) = 0;

    /* Assert X axis drivers */
    PINo(OUT_TS_XL) = 1;
    PINo(OUT_TS_XR) = 1;

    /* Wait for inputs to settle */
    vTaskDelay(1);

    /* Read X axis input */
    ad_acquire();
    pRaw->x = (int16_t) ad_value(ADC_TS_YU) + (int16_t) ad_value(ADC_TS_YL);

    /* De-assert X axis drivers */
    PINo(OUT_TS_XL) = 0;
    PINo(OUT_TS_XR) = 0;
    vTaskDelay(1);

    /* Check if screen is still being touched */
    if (false == check_touch())
    {
        touched = false;
    }

    if (false == touched)
    {
        /* Provide known values */
        pRaw->y = 0;
        pRaw->x = 0;
    }

    return (touched);
}
/*****************************************************************************
 End of function  r_touch_getraw
 ******************************************************************************/

/******************************************************************************
 Private Functions
 ******************************************************************************/

/*****************************************************************************
 Function Name: check_touch
 Description:   Checks if the screen is being touched.
 Drives both Touch screen X-Axis terminals LOW,
 Y-Axis port pins set to inputs with internal pull ups,
 After a small settling time, reads the state of the Y-Axix.
 If low, the user has touched the screen.
 Parameters:    None
 Return value:  True - Touched
 False - No touch
 *****************************************************************************/
static _Bool check_touch (void)
{
    _Bool ret_val;

    /* De-assert drivers */
    PINo(OUT_TS_XR) = 0;
    PINo(OUT_TS_XL) = 0;
    PINo(OUT_TS_YU) = 0;
    PINo(OUT_TS_YL) = 0;

    /* Enable OUTPUT port pins on XL and XR driving Low */
    TOUCH_PORTO_XL = 0;
    TOUCH_PORTO_XR = 0;
    TOUCH_PORT_XL_PDR = 1;
    TOUCH_PORT_XR_PDR = 1;

    /* Enable internal pull up on on Y Axis Drivers */
    TOUCH_PORT_YU_PUP = 1;
    TOUCH_PORT_YL_PUP = 1;

    /* Set Y-axis as inputs */
    TOUCH_PORT_YL_PDR = 0;
    TOUCH_PORT_YU_PDR = 0;

    vTaskDelay(1); /* 1ms delay to allow inputs to settle before sampling */

    /* Check state of Y-input */
    if (0 == TOUCH_PORTI_YU)
    {
        /* User is touching the display */
        ret_val = true;
    }
    else
    {
        ret_val = false;
    }

    /* Put ports back as inputs with no pull up so ADC can sample */
    TOUCH_PORT_XL_PDR = 0;
    TOUCH_PORT_XR_PDR = 0;
    TOUCH_PORT_YU_PUP = 0;
    TOUCH_PORT_YL_PUP = 0;

    return (ret_val);
}
/*****************************************************************************
 End of function  check_touch
 ******************************************************************************/
