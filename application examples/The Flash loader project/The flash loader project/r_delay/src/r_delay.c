/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No 
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all 
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM 
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES 
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS 
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of 
* this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer 
*
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.    
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name	   : r_delay.c
* Description  : This module provides a convenient API for users to implement delays in their system. This module is 
*                flexible in that delays can be implemented using hardware or software. The user also has the option of 
*                configuring this module so that a user defined function is called for implementing delays. This allows 
*                the user to replace this module’s implementation with their own (e.g. use their RTOS’ functionality) 
*                without impacting their already written code that uses this module.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version Description           
*         : 07.11.2012 1.00    First Release     
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Includes board and MCU related header files. */
#include "platform.h"
/* Public interface header file for this package. */
#include "r_delay_if.h"
/* Configuration for this package. */
#include "r_delay_config.h"
/* Private header file for this package. */
#include "r_delay.h"

#if DELAY_CFG_SOFTWARE_ONLY_ENABLE == 0
/* Use r_cmt_rx for hardware delay implementation. */
#include "r_cmt_rx_if.h"
#endif

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
 
/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/
/* Internal functions. */
#if !defined(DELAY_CFG_USER_DELAY_FUNCTION)
    #if DELAY_CFG_SOFTWARE_ONLY_ENABLE == 0
static bool delay_hw_wait(delay_unit_t wait_units, uint32_t wait_time);
    #endif
static bool delay_sw_wait(delay_unit_t wait_units, uint32_t wait_time);
#endif

/***********************************************************************************************************************
* Function Name: R_DELAY_Wait
* Description  : This function will block execution in the current task for the amount of time sent in by the user.
* Arguments    : wait_units - 
*                    The unit of the time sent in (e.g. milliseconds, microseconds, etc). See r_delay_if.h for available
*                    options.
*                wait_time -
*                    The amount of time to wait (in units of delay_units). This is an integer value, to use a value like
*                    1.5ms step down to the next smallest unit (e.g. 1.5ms = 1500us).
* Return Value : true - 
*                    Delay time has passed.
*                false -
*                    Invalid parameters input.
***********************************************************************************************************************/
bool R_DELAY_Wait (delay_unit_t wait_units, uint32_t wait_time)
{
    bool ret = false;

#if !defined(DELAY_CFG_USER_DELAY_FUNCTION)
    #if DELAY_CFG_SOFTWARE_ONLY_ENABLE == 0
    /* Try to use hardware first. */
    ret = delay_hw_wait(wait_units, wait_time);
    #endif

    /* Use software if HW was not able, or if SW is only option. */
    if (false == ret)
    {
        ret = delay_sw_wait(wait_units, wait_time);
    }
#else
    /* Call user-defined function with input parameters. */
    ret = DELAY_CFG_USER_DELAY_FUNCTION(wait_units, wait_time);
#endif

    return ret;
} 

#if !defined(DELAY_CFG_USER_DELAY_FUNCTION)
    #if DELAY_CFG_SOFTWARE_ONLY_ENABLE == 0
/***********************************************************************************************************************
* Function Name: delay_hw_wait
* Description  : Attempt to implement delay using hardware resources (i.e. timer).
* Arguments    : wait_units - 
*                    The unit of the time sent in (e.g. milliseconds, microseconds, etc). See r_delay_if.h for available
*                    options.
*                wait_time -
*                    The amount of time to wait (in units of delay_units). This is an integer value, to use a value like
*                    1.5ms step down to the next smallest unit (e.g. 1.5ms = 1500us).
* Return Value : true - 
*                    Delay time has passed.
*                false -
*                    Invalid parameters input or hardware resource not available.
***********************************************************************************************************************/
static bool delay_hw_wait (delay_unit_t wait_units, uint32_t wait_time)
{
    bool     ret = true;
    bool     is_running;
    uint32_t channel;
    uint32_t time_us;

    /* CMT function takes in microsecond parameter so convert wait time to microseconds if needed. */
    if (DELAY_UNIT_MS == wait_units)
    {
        /* Convert ms to us. */
        time_us = wait_time * 1000;
    }
    else if (DELAY_UNIT_US == wait_units)
    {
        time_us = wait_time;
    }
    else
    {
        /* Error, this wait_unit is not supported. */
        ret = false;
    }

    if (true == ret)
    {
        /* Try to allocate CMT channel for delay. */
        ret = R_CMT_CreateOneShot(time_us, FIT_NO_FUNC, &channel);

        /* If channel was available then wait for it to expire. */
        if (true == ret)
        {
            do
            {
                R_CMT_Control(channel, CMT_RX_CMD_IS_CHANNEL_COUNTING, &is_running);
            }
            while (true == is_running);
        }
    }

    return ret;
} 
    #endif

/***********************************************************************************************************************
* Function Name: delay_sw_wait
* Description  : Implement delay using software.
* Arguments    : wait_units - 
*                    The unit of the time sent in (e.g. milliseconds, microseconds, etc). See r_delay_if.h for available
*                    options.
*                wait_time -
*                    The amount of time to wait (in units of delay_units). This is an integer value, to use a value like
*                    1.5ms step down to the next smallest unit (e.g. 1.5ms = 1500us).
* Return Value : true - 
*                    Delay time has passed.
*                false -
*                    Invalid parameters input or hardware resource not available.
***********************************************************************************************************************/
static bool delay_sw_wait (delay_unit_t wait_units, uint32_t wait_time)
{
    /* Declared volatile for software delay purposes. */
    volatile uint32_t i;
    uint32_t time_us;
    bool     ret = true;

    /* CMT function takes in microsecond parameter so convert wait time to microseconds if needed. */
    if (DELAY_UNIT_MS == wait_units)
    {
        /* Convert ms to us. */
        time_us = wait_time * 1000;
    }
    else if (DELAY_UNIT_US == wait_units)
    {
        time_us = wait_time;
    }
    else
    {
        /* Error, this wait_unit is not supported. */
        ret = false;
    }

    if (true == ret)
    {
        /* The delay loops used below have been measured to take 12 cycles per iteration. This has been verified using the 
           Renesas RX Toolchain with optimizations set to 2, size priority. The same result was obtained using 2, speed 
           priority. The amount of times to run the loop is adjusted linearly based on this info along with the speed
           at which the core is running. */
        for(i = 0; i < ((time_us * (BSP_ICLK_HZ/1000000))/12); i++)             
        {
            nop() ;
        }
    }
    
    return ret;
} 
#endif

/***********************************************************************************************************************
* Function Name: R_DELAY_GetVersion
* Description  : Returns the current version of this module. The version number is encoded where the top 2 bytes are the
*                major version number and the bottom 2 bytes are the minor version number. For example, Version 4.25 
*                would be returned as 0x00040019.
* Arguments    : none
* Return Value : Version of this module.
***********************************************************************************************************************/
uint32_t R_DELAY_GetVersion (void)
{
    /* These version macros are defined in r_delay_if.h. */
    return ((((uint32_t)DELAY_VERSION_MAJOR) << 16) | (uint32_t)DELAY_VERSION_MINOR);
} 



