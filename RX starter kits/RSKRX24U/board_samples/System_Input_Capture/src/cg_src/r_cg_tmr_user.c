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
* Copyright (C) 2015, 2017 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_tmr_user.c
* Version      : Applilet4 for RX24U V1.03.01.04 [02 Nov 2017]
* Device(s)    : R5F524UEAxFB
* Tool-Chain   : IAR Systems ICCRX
* Description  : This file implements device driver for TMR module.
* Creation Date: 2018-06-13
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

static volatile uint8_t timer_count = 0;

/* Flag used to indicate data acquisition event triggered by the TMR system */
volatile uint8_t g_data_acq_flag;

extern uint8_t g_acq_period_count;

/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_tmr_cmia2_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = VECT_TMR2_CMIA2
#if FAST_INTERRUPT_VECTOR == VECT_TMR2_CMIA2
__fast_interrupt static void r_tmr_cmia2_interrupt(void)
#else
__interrupt static void r_tmr_cmia2_interrupt(void)
#endif
{
    /* Start user code. Do not edit comment generated here */

    timer_count++;

    if( timer_count == g_acq_period_count )
    {
        g_data_acq_flag = TRUE;
        timer_count = 0;
    }

    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_TMR_ClearCount
* Description  : Clear timer count
*
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void R_TMR_ClearCount(void)
{
    timer_count = 0;
}

/***********************************************************************************************************************
* End of function R_TMR_ClearCount
***********************************************************************************************************************/

/* End user code. Do not edit comment generated here */
