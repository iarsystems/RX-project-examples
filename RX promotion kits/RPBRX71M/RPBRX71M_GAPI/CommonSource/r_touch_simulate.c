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
 * File Name    : r_touch_simulate.c
 * Version      : 1.00
 * Device(s)    : Renesas
 * Tool-Chain   : RXC
 * OS           : None
 * H/W Platform : RPB
 * Description  : Routines for simulating touch events
 *
 *******************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 04.04.2016 1.00 First Release
 ******************************************************************************/

/******************************************************************************
 WARNING!  IN ACCORDANCE WITH THE USER LICENCE THIS CODE MUST NOT BE CONVEYED
 OR REDISTRIBUTED IN COMBINATION WITH ANY SOFTWARE LICENSED UNDER TERMS THE
 SAME AS OR SIMILAR TO THE GNU GENERAL PUBLIC LICENCE
 ******************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/

#include "Global.h"
#include "LCD_demo.h"
#include "r_touch_simulate.h"
#include "r_touch_sim_stimuli.h"

/******************************************************************************
 Macro Definitions
 ******************************************************************************/
#define FIRST_SIM_EVENT_DELAY (300)    /* Number of ticks before starting the first sim event */

/******************************************************************************
 Enumerated Types
 ******************************************************************************/

/******************************************************************************
 Typedef definitions
 ******************************************************************************/

/******************************************************************************
 Private global variables and functions
 ******************************************************************************/
static _Bool touch_replay_enabled = 0;
static uint16_t replay_list_num = 0; /* Index in to ptouch_stim_list */

/* This is the list of t_touchevent arrays that is run though
 * during touch simulation. It must end with a null terminator
 */
static const touchevent_t* ptouch_stim_list[] =
{
{ screen_thermo_stim },
{ screen_med_stim },
{ screen_refrig_stim },
{ NULL }, };

/******************************************************************************
 Function Prototypes
 ******************************************************************************/

/******************************************************************************
 Constant Data
 ******************************************************************************/

/******************************************************************************
 Public Functions
 ******************************************************************************/

/*****************************************************************************
 Function Name: R_TOUCH_SimulateTask
 Description:   Replays previously captured touch events by simulating touch screen
 events.
 Parameters:    None
 Return value:  None
 *****************************************************************************/
void R_TOUCH_SimulateTask (void *pvParameters)
{
    EVENT_MSG msg;
    uint16_t sim_event = 0;
    touchevent_t* ptouch_event;

    for (;;)
    {
        if (true == touch_replay_enabled)
        {
            ptouch_event = (touchevent_t*) ptouch_stim_list[replay_list_num];

            if (0 == sim_event)
            {
                vTaskDelay(FIRST_SIM_EVENT_DELAY); /* First delay defined not relative to any prev event */
            }

            msg.source = MS_TOUCHSCREEN;
            msg.event = ptouch_event[sim_event].touch_event;
            msg.param.coord[0] = ptouch_event[sim_event].cursor_location.x;
            msg.param.coord[1] = ptouch_event[sim_event].cursor_location.y;
            (int16_t) xQueueSend(EventQueue, (void * )&msg, 0UL); /* RTOS_USAGE */

            vTaskDelay(ptouch_event[sim_event].ticks_to_next_touch);

            sim_event++;

            /* Check we have not reached the high tide mark, and if we have, move on to the next list  */
            if (0 == ptouch_event[sim_event].ticks_to_next_touch)
            {
                replay_list_num++;
                sim_event = 0;

                /* Check for the end of lists */
                if (NULL == ptouch_stim_list[replay_list_num])
                {
                    replay_list_num = 0; /* Start from the first entry */
                }
            }

        }
        else
        {
            vTaskDelay(300); /* Just sleep before checking if enabled */
        }
    }
}

/*****************************************************************************
 End of function  R_TOUCH_SimulateTask
 ******************************************************************************/

/*****************************************************************************
 Function Name: R_TOUCH_EnableSimulation
 Description:   Enables simulated screen presses
 Parameters:    None
 Return value:  None
 *****************************************************************************/
void R_TOUCH_EnableSimulation (void)
{
    touch_replay_enabled = 1;
}
/*****************************************************************************
 End of function  R_TOUCH_EnableSimulation
 ******************************************************************************/

/*****************************************************************************
 Function Name: R_TOUCH_DisableSimulation
 Description:   Disables simulated screen presses
 Parameters:    None
 Return value:  None
 *****************************************************************************/
void R_TOUCH_DisableSimulation (void)
{
    touch_replay_enabled = 0;
}
/*****************************************************************************
 End of function  R_TOUCH_DisableSimulation
 ******************************************************************************/

/******************************************************************************
 Private Functions
 ******************************************************************************/

/******************************************************************************
 End  Of File
 ******************************************************************************/
