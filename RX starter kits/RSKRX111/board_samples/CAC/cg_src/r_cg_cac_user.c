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
* File Name    : r_cg_cac_user.c
* Version      : Applilet4 for RX111 V1.00.00.05 [06 Jun 2013]
* Device(s)    : R5F51115AxFM
* Tool-Chain   : IAR Systems icc78k0r
* Description  : This file implements device driver for CAC module.
* Creation Date: 2013-08-16
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
#include "r_cg_cac.h"
/* Start user code for include. Do not edit comment generated here */
#include "cac.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
volatile uint8_t Frequency_Error_Flag = 0U;
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_cac_ferrf_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = VECT_CAC_FERRF
#if FAST_INTERRUPT_VECTOR == VECT_CAC_FERRF
__fast_interrupt static void r_cac_ferrf_interrupt(void)
#else
__interrupt static void r_cac_ferrf_interrupt(void)
#endif
{
    /* Start user code. Do not edit comment generated here */

    /* Set Flag to print a message to the Debug LCD */
    Frequency_Error_Flag = 1U;

    /* Clear the frequency error flag */
    CAC.CAICR.BIT.FERRFCL = 0x1u;

    /* End user code. Do not edit comment generated here */
}


/* Start user code for adding. Do not edit comment generated here */

/******************************************************************************
* Function Name: Frequency_Error
* Description  : Report status of frequency error flag
* Arguments    : none
* Return Value : none
******************************************************************************/
uint8_t Frequency_Error (void)
{
    if (Frequency_Error_Flag)
    {
        /* reset flag */
        Frequency_Error_Flag = 0U;
        return (1);
    }
    return (0);
}
/******************************************************************************
* End of function Frequency_Error
******************************************************************************/

/******************************************************************************
* Function Name: Frequency_Value
* Description  : Calculates the current frequency based on counter value in
*              : CACNTBR register
* Arguments    : none
* Return Value : none
******************************************************************************/
uint32_t Frequency_Value (void)
{
    return ((uint32_t)((CAC.CACNTBR) * CAC_REF_FREQUENCY));
}
/******************************************************************************
* End of function Frequency_Value
******************************************************************************/

/******************************************************************************
* Function Name: Upper_Frequency_Limit
* Description  : Calculates the upper frequency limit based on counter value in
*              : CAULVR register
* Arguments    : none
* Return Value : none
******************************************************************************/
uint32_t Upper_Frequency_Limit (void)
{
    return ((uint32_t)((CAC.CAULVR) * CAC_REF_FREQUENCY));
}
/******************************************************************************
* End of function Upper_Frequency_Limit
******************************************************************************/

/******************************************************************************
* Function Name: Lower_Frequency_Limit
* Description  : Calculates the upper frequency limit based on counter value in
*              : CALLVR register
* Arguments    : none
* Return Value : none
******************************************************************************/
uint32_t Lower_Frequency_Limit (void)
{
    return ((uint32_t)((CAC.CALLVR) * CAC_REF_FREQUENCY));
}
/******************************************************************************
* End of function Lower_Frequency_Limit
******************************************************************************/

/* End user code. Do not edit comment generated here */
