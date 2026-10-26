/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. This 
* software is owned by Renesas Electronics Corporation and is protected under 
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING 
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT 
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR 
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE 
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software 
* and to discontinue the availability of this software.  By using this software, 
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_cmt_user.c
* Version      : Applilet4 for RX111 V1.00.00.05 [06 Jun 2013]
* Device(s)    : R5F51115AxFM
* Tool-Chain   : IAR Systems icc78k0r
* Description  : This file implements device driver for CMT module.
* Creation Date: 2013-08-26
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
#include "switch.h"
#include <stddef.h>
#include "r_cg_icu.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */

/* The functions in this file are generated using "Application Leading Tool (Applilet)" for e2 studio.
 * Warnings exist for interrupt handlers and callback functions. */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_cmt_cmi0_interrupt
* Description  : None
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

    /* Stop the CMT0 timer running */
    R_CMT0_Stop();

    /* Call the switch debounce callback function */
    switch_debounce_cb();

    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */

/******************************************************************************
* Function Name : start_debounce_timer
* Description   : Function starts the CMT timer to
*                 generate the switch debounce interrupt.
* Argument      : uint16_t compare_match    : compare match value to trigger
*                                             *interrupt at.
* Return value  : none
******************************************************************************/
void start_debounce_timer (const uint16_t compare_match)
{
    /* Set compare match to to generate debounce period */
    CMT0.CMCOR = compare_match;

    /* Reset count to zero */
    CMT0.CMCNT = 0x0000;

    /* Start timer */
    R_CMT0_Start();
}
/******************************************************************************
* End of function start_debounce_timer
******************************************************************************/

/******************************************************************************
* Function Name : switch_debounce_cb
* Description   : Switch debounce timer callback function. Function is executed
*                 by the CMT channel ISR. Function re-enables all switch all
*                 switch interrupts; then verifies a correct switch press was
*                 detected, and calls the user switch press callback function.
* Argument      : none
* Return value  : none
******************************************************************************/
void switch_debounce_cb (void)
{
    /* Clear IRQ0 interrupt flag */
    IR(ICU, IRQ0) = 0;

    /* Re-enable switch 1 interrupts */
    IEN(ICU, IRQ0) = 0x1;

    /* Clear IRQ1 interrupt flag */
    IR(ICU, IRQ1) = 0;

    /* Re-enable switch 2 interrupts */
    IEN(ICU, IRQ1) = 0x1;

    /* Clear IRQ4 interrupt flag */
    IR(ICU, IRQ4) = 0;

    /* Re-enable switch 3 interrupts */
    IEN(ICU, IRQ4) = 0x1;

    /* Check if switch 1 hold flag is set, IRQ0 */
    if ((g_switch_flag & 0x0F) & SWITCHHOLD_1)
    {
        /* Check if switch 1 pin level is high (switch press duration
           less than debounce, invalid switch press) */
        if (0x1 == SW1_PORT_VALUE)
        {
            /* Reset detection direction to falling edge */
            ICU.IRQCR[0].BIT.IRQMD = 0x1;

            /* Clear switch press flag */
            /* type cast required to remove warning for
                Implicit conversion: unsigned int to unsigned char*/
            g_switch_flag &= ((uint8_t)0xF0 | ~SWITCHHOLD_1);

            /* Increment detected switch faults counter */
            g_switch_faults_detected++;
        }
        else
        {
            /* Switch 1 pin level is low (valid switch press) */
            /* Check if switch press callback function is not NULL */
            if (g_switch_press_callback_func != NULL)
            {
                /* Execute user callback function */
                g_switch_press_callback_func();
            }
        }
    }

    /* Check if switch 2 hold flag is set, IRQ1 */
    if ((g_switch_flag & 0x0F) & SWITCHHOLD_2)
    {
        /* Check if switch 2 pin level is high (switch press duration
           less than debounce, invalid switch press) */
        if (0x1 == SW2_PORT_VALUE)
        {
            /* Reset detection direction to falling edge */
            ICU.IRQCR[1].BIT.IRQMD = 0x1;

            /* Clear switch press flag */
            /* type cast required to remove warning for
                Implicit conversion: unsigned int to unsigned char*/
            g_switch_flag &= ((uint8_t)0xF0 | ~SWITCHHOLD_2);

            /* Increment detected switch faults counter */
            g_switch_faults_detected++;
        }
        else
        {
            /* Switch 2 pin level is low (valid switch press) */
            /* Check if switch press callback function is not NULL */
            if (g_switch_press_callback_func != NULL)
            {
                /* Execute user callback function */
                g_switch_press_callback_func();
            }
        }
    }

    /* Check if switch 3 hold flag is set, IRQ4 */
    if ((g_switch_flag & 0x0F) & SWITCHHOLD_3)
    {
        /* Check if switch 3 pin level is high (switch press duration
           less than debounce, invalid switch press) */
        if (0x1 == SW3_PORT_VALUE)
        {
            /* Reset detection direction to falling edge */
            ICU.IRQCR[4].BIT.IRQMD = 0x1;

            /* Clear switch press flag */
            /* type cast required to remove warning for
                Implicit conversion: unsigned int to unsigned char*/
            g_switch_flag &= ((uint8_t)0xF0 | ~SWITCHHOLD_3);

            /* Increment detected switch faults counter */
            g_switch_faults_detected++;
        }
        else
        {
            /* Switch 3 pin level is low (valid switch press) */
            /* Check if switch press callback function is not NULL */
            if (g_switch_press_callback_func != NULL)
            {
                /* Execute user callback function */
                g_switch_press_callback_func();
            }
        }
    }

    /* Check if any switches are held down */
    if (0x00 == (g_switch_flag & 0x0F))
    {
        g_switch_standby_ready = true;
    }
}
/******************************************************************************
* End of function switch_debounce_cb
******************************************************************************/
/* End user code. Do not edit comment generated here */
