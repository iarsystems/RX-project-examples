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
* Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_mtu2_user.c
* Version      : Applilet4 for RX113 V1.01.00.02 [31 Jul 2014]
* Device(s)    : R5F51138AxFP
* Tool-Chain   : IAR Systems ICCRX
* Description  : This file implements device driver for MTU2 module.
* Creation Date: 2015-01-21
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
#include "r_cg_mtu2.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
volatile uint8_t g_vary_duty_cycle = TRUE;
volatile uint16_t g_duty_update;

/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_mtu2_tgia4_interrupt
* Description  : This function MTU2 TGIA4 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = VECT_MTU4_TGIA4
#if FAST_INTERRUPT_VECTOR == VECT_MTU4_TGIA4
__fast_interrupt static void r_mtu2_tgia4_interrupt(void)
#else
__interrupt static void r_mtu2_tgia4_interrupt(void)
#endif
{
    /* Start user code. Do not edit comment generated here */
    /* Stop the TCNT counter */
    R_MTU2_C4_Stop();

    /* Update the duty cycle if SW1 hasn't been pressed. */
    if (1 == g_vary_duty_cycle)
    {
        /* Decrease the duty cycle by 0.025 % */
        /* Cast required to remove compiler warning  -
         Implicit conversion: int to unsigned short. */
        MTU4.TGRB += (uint16_t) (TICKS_PER_CYCLE / 4000);

        /* Read duty cycle */
        g_duty_update = MTU4.TGRB;

        /* Check if the duty cycle equals 10% */
        if (g_duty_update > (TICKS_PER_CYCLE - (TICKS_PER_CYCLE / 10)))
        {
            /* Reset the duty cycle to 90% */
            MTU4.TGRB = (TICKS_PER_CYCLE / 10);
        }
    }

    /* Start the TCNT counter */
    R_MTU2_C4_Start();

    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
