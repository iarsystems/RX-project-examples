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
* File Name    : r_cg_cac.c
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
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_CAC_Create
* Description  : This function initializes the clock frequency accuracy measurement circuit.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_CAC_Create(void)
{
    /* Cancel CAC stop state in LPC */
    SYSTEM.MSTPCRC.BIT.MSTPC19 = 0U;

    /* Set CAC control registers */
    CAC.CACR1.BYTE = _08_CAC_OUTPUT_MEASURE_IWDTCLK | _00_CAC_TIMER_CLOCK_SOURCE_1;
    CAC.CACR2.BYTE = _01_CAC_REF_SIGNAL_INTERNAL | _0A_CAC_REF_CLOCK_PCLKB | _30_CAC_REF_CLOCK_SOURCE_8192 | 
                     _00_CAC_DIGITAL_FILTER_DISABLED;
    CAC.CAICR.BYTE = _01_CAC_FERRIE_INTERRUPT_ENABLE | _00_CAC_MENDIE_INTERRUPT_DISABLE | 
                     _00_CAC_OVFIE_INTERRUPT_DISABLE;

    /* Set upper and lower limit register values */
    CAC.CAULVR = _0204_CAC_UPPER_LIMIT;
    CAC.CALLVR = _01D2_CAC_LOWER_LIMIT;

    /* Set FERRF priority level */
    IPR(CAC,FERRF) = _0F_CAC_PRIORITY_LEVEL15;
}

/***********************************************************************************************************************
* Function Name: R_CAC_Start
* Description  : This function enables the clock frequency accuracy measurement circuit.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_CAC_Start(void)
{
    /* Enable CAC interrupts */
    IEN(CAC,FERRF) = 1U;

    /* Enable clock frequency measurement */
    CAC.CACR0.BIT.CFME = 1U;

    while (0U == CAC.CACR0.BIT.CFME);
}

/***********************************************************************************************************************
* Function Name: R_CAC_Stop
* Description  : This function stops the clock frequency accuracy measurement circuit.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_CAC_Stop(void)
{
    /* Disable CAC interrupts */
    IEN(CAC,FERRF) = 0U;

    /* Disable clock frequency measurement */
    CAC.CACR0.BIT.CFME = 0U;
    
    while (1U == CAC.CACR0.BIT.CFME);
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
