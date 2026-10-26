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
* Copyright (C) 2015, 2016 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_tmr_user.c
* Version      : Applilet4 for RX130 V1.00.01.02 [07 Mar 2016]
* Device(s)    : R5F51305AxFN
* Tool-Chain   : IAR Systems ICCRX
* Description  : This file implements device driver for TMR module.
* Creation Date: 2016-09-23
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
#include "r_cg_tmr.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */

/* DEBOUNCE_TICKS - Number of 100ms counter increments for debounce delay */
#define DEBOUNCE_TICKS (2)

static volatile uint8_t one_ms_delay_complete = FALSE;

/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_tmr_cmia0_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = VECT_TMR0_CMIA0
#if FAST_INTERRUPT_VECTOR == VECT_TMR0_CMIA0
__fast_interrupt static void r_tmr_cmia0_interrupt(void)
#else
__interrupt static void r_tmr_cmia0_interrupt(void)
#endif
{
    /* Start user code. Do not edit comment generated here */

    one_ms_delay_complete = TRUE;

    /* End user code. Do not edit comment generated here */

}

/* Start user code for adding. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_TMR_MsDelay
* Description  : Uses TMR0 to wait for a specified number of milliseconds
* Arguments    : uint16_t millisecs, number of milliseconds to wait
* Return Value : None
***********************************************************************************************************************/
void R_TMR_MsDelay (const uint16_t millisec)
{
    uint16_t ms_count = 0;

    do
    {
        R_TMR0_Start();
        while (FALSE == one_ms_delay_complete)
        {
            /* Wait */
        }
        R_TMR0_Stop();
        one_ms_delay_complete = FALSE;
        ms_count++;
    } while (ms_count < millisec);
}
/***********************************************************************************************************************
* End of function R_TMR_MsDelay
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: R_TMR_DebounceDelay
* Description  : Waits for a pre-determined number of timer ticks
*              : before returning. Used to provide a delay when the user
*              : switches have been pressed to ensure switch bounce does not
*              : appear as multiple presses,
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TMR_DebounceDelay(void)
{
    R_TMR_MsDelay(DEBOUNCE_TICKS * 100);
}

/***********************************************************************************************************************
* End of function R_TMR_DebounceDelay
***********************************************************************************************************************/

/* End user code. Do not edit comment generated here */
