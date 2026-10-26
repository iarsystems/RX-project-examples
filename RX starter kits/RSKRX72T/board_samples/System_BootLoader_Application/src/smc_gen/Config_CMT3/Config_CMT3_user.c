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
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : Config_CMT3_user.c
* Version      : 1.7.0
* Device(s)    : R5F572TKCxFB
* Description  : This file implements device driver for Config_CMT3.
* Creation Date: 2019-05-21
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
#include "Config_CMT3.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */

/* DEBOUNCE_TICKS - Number of counter increments for debounce delay */
#define DEBOUNCE_TICKS (2)

/* Global counter variable - incremented within CMT0 ISR */
volatile uint32_t g_ms_100_counter = 0x00;

/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_CMT3_Create_UserInit
* Description  : This function adds user code after initializing the CMT3 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_CMT3_Create_UserInit(void)
{
    /* Start user code for user init. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_Config_CMT3_cmi3_interrupt
* Description  : This function is CMI3 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

#pragma vector = VECT_CMT3_CMI3
#if FAST_INTERRUPT_VECTOR == VECT_CMT3_CMI3
__fast_interrupt static void r_Config_CMT3_cmi3_interrupt(void)
#else
__interrupt static void r_Config_CMT3_cmi3_interrupt(void)
#endif
{
    /* Start user code for r_Config_CMT3_cmi3_interrupt. Do not edit comment generated here */

    /* Increment count */
    g_ms_100_counter++;

    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_CMT_DebounceDelay
* Description  : Waits for a pre-determined number of timer ticks
*              : before returning. Used to provide a delay when the user
*              : switches have been pressed to ensure switch bounce does not
*              : appear as multiple presses,
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_CMT_DebounceDelay (void)
{
    uint32_t current_counter;

    /* Take a snapshot of the current counter value */
    current_counter = g_ms_100_counter;

    /* A loop until counter has reached an increment of DEBOUNCE_TICKS */
    while (g_ms_100_counter < (current_counter + DEBOUNCE_TICKS))
    {
        ;
    }
}

/***********************************************************************************************************************
End of Function R_CMT_DebounceDelay
***********************************************************************************************************************/

/* End user code. Do not edit comment generated here */

