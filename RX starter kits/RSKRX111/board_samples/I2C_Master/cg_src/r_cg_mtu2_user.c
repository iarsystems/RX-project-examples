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
* File Name    : r_cg_mtu2_user.c
* Version      : Applilet4 for RX111 V1.00.00.05 [06 Jun 2013]
* Device(s)    : R5F51115AxFM
* Tool-Chain   : CCRX
* Description  : This file implements device driver for MTU2 module.
* Creation Date: 19/06/2013
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

/* The functions in this file are generated using "Application Leading Tool (Applilet)" for e2 studio.
 * Warnings exist for interrupt handlers and callback functions. */

volatile uint8_t mtu2a_overflow = 0;
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_mtu2_u0_tgia0_interrupt
* Description  : This function MTU2 Unit0 TGIA0 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

#pragma vector=VECT_MTU0_TGIA0
__interrupt static void r_mtu2_u0_tgia0_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    mtu2a_overflow = 1;
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */

/*******************************************************************************
* Function Name: I2C_timeout
* Description  : This function informs if the MTU0 timer has overflowed
* Argument     : none
* Return value : uint8_t 0 = not overflowed, not 0 = overflowed
*******************************************************************************/
uint8_t I2C_timeout (void)
{
    return ((uint8_t)(1 == mtu2a_overflow));
}
/*******************************************************************************
* End of function I2C_timeout
*******************************************************************************/

/*******************************************************************************
* Function Name: I2C_timer_reset
* Description  : This function resets the MTU0 Timer function
* Argument     : none
* Return value : none
*******************************************************************************/
void I2C_timer_reset (void)
{
    R_MTU2_U0_C0_Stop();
    MTU0.TCNT = 0;
    mtu2a_overflow = 0;
}
/*******************************************************************************
* End of function I2C_timer_reset
*******************************************************************************/

/*******************************************************************************
* Function Name: I2C_timer_start
* Description  : This function MTU0 Timer from a reset state
* Argument     : none
* Return value : none
*******************************************************************************/
void I2C_timer_start (void)
{
    I2C_timer_reset();
    R_MTU2_U0_C0_Start();
}
/*******************************************************************************
* End of function I2C_timer_start
*******************************************************************************/
/* End user code. Do not edit comment generated here */
