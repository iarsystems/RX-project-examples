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

/* The functions in this file are generated using "Application Leading Tool (Applilet)" for e2 studio.
 * Warnings exist for interrupt handlers. */

static volatile uint16_t us_count = 0;
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_cmt_cmi1_interrupt
* Description  : None
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

    /* Increment the microsecond count variable */
    us_count++;

    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */

/*******************************************************************************
* Function Name : R_Microsec_Delay
* Description   : Uses the pre-configured CMT1 (set up for 1us interrupt)
*                 to wait for a specified number of microseconds
* Argument      : uint16_t: number of microseconds to wait
* Return value  : none
*******************************************************************************/
void R_Microsec_Delay (const uint16_t micro_secs)
{
    /* Reset the microsecond count variable */
    us_count = 0;

    /* Start the timer */
    R_CMT1_Start();

    /* Wait until the count exceeds the required delay */
    while (us_count < micro_secs)
    {
        /* Wait */
    }

    /* Stop the timer */
    R_CMT1_Stop();

}
/*******************************************************************************
End of function R_Microsec_Delay
*******************************************************************************/
/* End user code. Do not edit comment generated here */
