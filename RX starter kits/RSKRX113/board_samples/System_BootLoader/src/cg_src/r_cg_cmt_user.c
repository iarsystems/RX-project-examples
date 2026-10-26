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
* File Name    : r_cg_cmt_user.c
* Version      : Applilet4 for RX113 V1.01.00.02 [31 Jul 2014]
* Device(s)    : R5F51138AxFP
* Tool-Chain   : IAR Systems ICCRX
* Description  : This file implements device driver for CMT module.
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
#include "r_cg_cmt.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
uint32_t g_1s_counter = 0;
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
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: r_cmt_cmi3_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = VECT_CMT3_CMI3
#if FAST_INTERRUPT_VECTOR == VECT_CMT3_CMI3
__fast_interrupt static void r_cmt_cmi3_interrupt(void)
#else
__interrupt static void r_cmt_cmi3_interrupt(void)
#endif
{
    /* Start user code. Do not edit comment generated here */

    g_1s_counter++; /* Increment the global 1-second counter */

    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */

/***********************************************************************************************************************
 * Function Name: r_cmt_resetstart_1s_counter
 * Description  : Resets the 1s global counter to 0;
 *
 * Arguments    : None
 * Return Value : None
 ***********************************************************************************************************************/
void r_cmt_resetstart_1s_counter (void)
{
    R_CMT3_Stop();
    g_1s_counter = 0;
    R_CMT3_Start();
}

/***********************************************************************************************************************
 * End of function r_cmt_resetstart_1s_counter
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: r_cmt_get_1s_counter
 * Description  : returns the value of the global 1s counter
 *
 * Arguments    : None
 * Return Value : None
 ***********************************************************************************************************************/
uint32_t r_cmt_get_1s_counter (void)
{
    return (g_1s_counter);
}

/***********************************************************************************************************************
 * End of function r_cmt_get_1s_counter
 ***********************************************************************************************************************/

/* End user code. Do not edit comment generated here */
