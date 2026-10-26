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
 * File Name    : r_touch_sim_stimuli.h
 * Version      : 1.00
 * Device(s)    : Renesas
 * Tool-Chain   : RXC
 * OS           : None
 * H/W Platform : RPB
 * Description  : Touch screen stimuli for r_touch_cap_and_sim.c
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
#ifndef TOUCHSTIM_H_INCLUDED
    #define TOUCHSTIM_H_INCLUDED

/******************************************************************************
 Macro definitions
 ******************************************************************************/
static const touchevent_t screen_thermo_stim[] =
{

{
{ 39, 85 }, 100, ME_PRESS },
{
{ 39, 85 }, 1000, ME_RELEASE },

/* Sequence through the days */
{
{ 9, 21 }, 100, ME_PRESS },
{
{ 9, 21 }, 1500, ME_RELEASE },

{
{ 60, 17 }, 100, ME_PRESS },
{
{ 60, 17 }, 1500, ME_RELEASE },

{
{ 108, 17 }, 100, ME_PRESS },
{
{ 150, 17 }, 1500, ME_RELEASE },

{
{ 154, 19 }, 100, ME_PRESS },
{
{ 153, 19 }, 1500, ME_RELEASE },

{
{ 202, 19 }, 100, ME_PRESS },
{
{ 202, 19 }, 1500, ME_RELEASE },

{
{ 250, 21 }, 100, ME_PRESS },
{
{ 250, 21 }, 1500, ME_RELEASE },

{
{ 294, 16 }, 100, ME_PRESS },
{
{ 294, 16 }, 1500, ME_RELEASE },

/* 3x '+' presses */
{
{ 275, 75 }, 100, ME_PRESS },
{
{ 275, 75 }, 1500, ME_RELEASE },

{
{ 275, 75 }, 100, ME_PRESS },
{
{ 275, 75 }, 1500, ME_RELEASE },

{
{ 275, 75 }, 100, ME_PRESS },
{
{ 275, 75 }, 1500, ME_RELEASE },

/* 3x '-' presses */
{
{ 229, 78 }, 100, ME_PRESS },
{
{ 229, 78 }, 1500, ME_RELEASE },

{
{ 229, 78 }, 100, ME_PRESS },
{
{ 229, 78 }, 1500, ME_RELEASE },

{
{ 229, 78 }, 100, ME_PRESS },
{
{ 229, 78 }, 1500, ME_RELEASE },

/* Back Home */
{
{ 304, 224 }, 100, ME_PRESS },
{
{ 304, 224 }, 1000, ME_RELEASE },

{
{ 0, 0 }, 0000, ME_INVALID } /* Null terminator */
};

static const touchevent_t screen_med_stim[] =
{
{
{ 105, 85 }, 100, ME_PRESS },
{
{ 105, 85 }, 1000, ME_RELEASE },

/* 'RUN' press - demo functions are within the screen */
{
{ 230, 123 }, 100, ME_PRESS },
{
{ 230, 123 }, 11000, ME_RELEASE },

/* 'STOP' press */
{
{ 230, 123 }, 100, ME_PRESS },
{
{ 230, 123 }, 1000, ME_RELEASE },

/* Back Home */
{
{ 304, 224 }, 100, ME_PRESS },
{
{ 304, 224 }, 1000, ME_RELEASE },

{
{ 0, 0 }, 0000, ME_INVALID } /* Null terminator */
};

static const touchevent_t screen_refrig_stim[] =
{
{
{ 177, 85 }, 100, ME_PRESS },
{
{ 177, 85 }, 1000, ME_RELEASE },

/* Freezer '+' presses  */
{
{ 9, 74 }, 100, ME_PRESS },
{
{ 9, 74 }, 1500, ME_RELEASE },

{
{ 9, 74 }, 100, ME_PRESS },
{
{ 9, 74 }, 1500, ME_RELEASE },

{
{ 9, 74 }, 100, ME_PRESS },
{
{ 9, 74 }, 1500, ME_RELEASE },

{
{ 59, 74 }, 100, ME_PRESS },
{
{ 59, 74 }, 1500, ME_RELEASE },

{
{ 59, 74 }, 100, ME_PRESS },
{
{ 59, 74 }, 1500, ME_RELEASE },

/* Fridge '+' , '+' & '-' press  */
{
{ 222, 74 }, 200, ME_PRESS },
{
{ 222, 74 }, 1000, ME_RELEASE },

{
{ 222, 74 }, 200, ME_PRESS },
{
{ 222, 74 }, 1000, ME_RELEASE },

{
{ 271, 74 }, 200, ME_PRESS },
{
{ 271, 74 }, 1000, ME_RELEASE },

/* Dispenser presses */
{
{ 134, 69 }, 100, ME_PRESS },
{
{ 134, 69 }, 1000, ME_RELEASE },

{
{ 134, 114 }, 100, ME_PRESS },
{
{ 134, 114 }, 1000, ME_RELEASE },

{
{ 134, 161 }, 100, ME_PRESS },
{
{ 134, 161 }, 1000, ME_RELEASE },

{
{ 134, 161 }, 100, ME_PRESS },
{
{ 134, 161 }, 1000, ME_RELEASE },

{
{ 134, 114 }, 100, ME_PRESS },
{
{ 134, 114 }, 1000, ME_RELEASE },

{
{ 134, 69 }, 100, ME_PRESS },
{
{ 134, 69 }, 1000, ME_RELEASE },

/* Filter presses */
{
{ 116, 11 }, 100, ME_PRESS },
{
{ 116, 11 }, 1000, ME_RELEASE },

{
{ 116, 11 }, 100, ME_PRESS },
{
{ 116, 11 }, 1000, ME_RELEASE },

{
{ 116, 11 }, 100, ME_PRESS },
{
{ 116, 11 }, 1000, ME_RELEASE },

/* Lock/Unlock slide */
{
{ 65, 15 }, 100, ME_PRESS },
{
{ 65, 15 }, 1000, ME_RELEASE },

{
{ 14, 15 }, 100, ME_PRESS },
{
{ 14, 15 }, 1000, ME_RELEASE },

/* Light slide */
{
{ 280, 15 }, 100, ME_PRESS },
{
{ 280, 15 }, 1000, ME_RELEASE },

{
{ 233, 15 }, 100, ME_PRESS },
{
{ 233, 15 }, 1000, ME_RELEASE },

/* Back Home */
{
{ 304, 224 }, 100, ME_PRESS },
{
{ 304, 224 }, 1000, ME_RELEASE },

{
{ 0, 0 }, 0000, ME_INVALID } /* Null terminator */
};

/******************************************************************************
 Typedef definitions
 ******************************************************************************/

/******************************************************************************
 Function Prototypes
 ******************************************************************************/

/******************************************************************************
 End  Of File
 ******************************************************************************/

#endif //TOUCHSTIM_H_INCLUDED
