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
* File Name    : r_cg_cmt_user.c
* Version      : Applilet4 for RX65N V1.00.00.08 [25 Aug 2016]
* Device(s)    : R5F565N9AxFB
* Tool-Chain   : IAR Systems ICCRX
* Description  : This file implements device driver for CMT module.
* Creation Date: 2016-10-12
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
#include "r_cg_cmt.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */

/* DEBOUNCE_TICKS - Number of counter increments for debounce delay */
#define DEBOUNCE_TICKS (2)

static volatile uint8_t one_ms_delay_complete = FALSE;

/* Global counter variable - incremented within CMT0 ISR */
volatile uint32_t g_ms_100_counter = 0x00;

/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_cmt_cmi0_interrupt
* Description  : This function is CMI0 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = VECT_CMT0_CMI0
#if FAST_INTERRUPT_VECTOR == VECT_CMT0_CMI0
__fast_interrupt static void r_cmt_cmi0_interrupt(void)
#else
__interrupt static void r_cmt_cmi0_interrupt(void)
#endif
{
    /* Start user code. Do not edit comment generated here */

    one_ms_delay_complete = TRUE;

    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: r_cmt_cmi1_interrupt
* Description  : This function is CMI1 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = VECT_CMT1_CMI1
#if FAST_INTERRUPT_VECTOR == VECT_CMT1_CMI1
__fast_interrupt static void r_cmt_cmi1_interrupt(void)
#else
__interrupt static void r_cmt_cmi1_interrupt(void)
#endif
{
    /* Start user code. Do not edit comment generated here */

    /* Increment count */
    g_ms_100_counter++;

    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_CMT_MsDelay
* Description  : Uses CMT0 to wait for a specified number of milliseconds
* Arguments    : uint16_t millisecs, number of milliseconds to wait
* Return Value : None
***********************************************************************************************************************/
void R_CMT_MsDelay (const uint16_t millisec)
{
    uint16_t ms_count = 0;

    do
    {
        R_CMT0_Start();
        while (FALSE == one_ms_delay_complete)
        {
            /* Wait */
        }
        R_CMT0_Stop();
        one_ms_delay_complete = FALSE;
        ms_count++;
    } while (ms_count < millisec);
}

/***********************************************************************************************************************
End of function R_CMT_MsDelay
***********************************************************************************************************************/

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
