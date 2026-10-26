/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
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
* Copyright (C) 2022 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name        : Config_ICU.c
* Component Version: 2.3.0
* Device(s)        : R5F56609HxFB
* Description      : This file implements device driver for Config_ICU.
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
#include "Config_ICU.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_ICU_Create
* Description  : This function initializes the ICU module
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_ICU_Create(void)
{
    /* Disable IRQ interrupts */
    ICU.IER[0x08].BYTE = 0x00U;
    ICU.IER[0x09].BYTE = 0x00U;

    /* Disable software interrupt */
    IEN(ICU,SWINT) = 0U;
    IEN(ICU,SWINT2) = 0U;

    /* Disable IRQ digital filter */
    ICU.IRQFLTE1.BYTE &= ~(_02_ICU_IRQ9_FILTER_ENABLE | _04_ICU_IRQ10_FILTER_ENABLE | _80_ICU_IRQ15_FILTER_ENABLE);

    /* Set IRQ9 pin */
    MPC.P91PFS.BYTE = 0x40U;
    PORT9.PDR.BYTE &= 0xFDU;
    PORT9.PMR.BYTE &= 0xFDU;

    /* Set IRQ10 pin */
    MPC.P92PFS.BYTE = 0x40U;
    PORT9.PDR.BYTE &= 0xFBU;
    PORT9.PMR.BYTE &= 0xFBU;

    /* Set IRQ15 pin */
    MPC.P07PFS.BYTE = 0x40U;
    PORT0.PDR.BYTE &= 0x7FU;
    PORT0.PMR.BYTE &= 0x7FU;

    /* Set IRQ detection type */
    ICU.IRQCR[9].BYTE = _04_ICU_IRQ_EDGE_FALLING;
    IR(ICU,IRQ9) = 0U;
    ICU.IRQCR[10].BYTE = _04_ICU_IRQ_EDGE_FALLING;
    IR(ICU,IRQ10) = 0U;
    ICU.IRQCR[15].BYTE = _04_ICU_IRQ_EDGE_FALLING;
    IR(ICU,IRQ15) = 0U;

    /* Set IRQ priority level */
    IPR(ICU,IRQ9) = _0F_ICU_PRIORITY_LEVEL15;
    IPR(ICU,IRQ10) = _0F_ICU_PRIORITY_LEVEL15;
    IPR(ICU,IRQ15) = _0F_ICU_PRIORITY_LEVEL15;

    R_Config_ICU_Create_UserInit();
}

/***********************************************************************************************************************
* Function Name: R_Config_ICU_IRQ9_Start
* Description  : This function enables IRQ9 interrupt
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_ICU_IRQ9_Start(void)
{
    /* Enable IRQ9 interrupt */
    IEN(ICU,IRQ9) = 1U;
}

/***********************************************************************************************************************
* Function Name: R_Config_ICU_IRQ9_Stop
* Description  : This function disables IRQ9 interrupt
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_ICU_IRQ9_Stop(void)
{
    /* Disable IRQ9 interrupt */
    IEN(ICU,IRQ9) = 0U;
}

/***********************************************************************************************************************
* Function Name: R_Config_ICU_IRQ10_Start
* Description  : This function enables IRQ10 interrupt
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_ICU_IRQ10_Start(void)
{
    /* Enable IRQ10 interrupt */
    IEN(ICU,IRQ10) = 1U;
}

/***********************************************************************************************************************
* Function Name: R_Config_ICU_IRQ10_Stop
* Description  : This function disables IRQ10 interrupt
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_ICU_IRQ10_Stop(void)
{
    /* Disable IRQ10 interrupt */
    IEN(ICU,IRQ10) = 0U;
}

/***********************************************************************************************************************
* Function Name: R_Config_ICU_IRQ15_Start
* Description  : This function enables IRQ15 interrupt
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_ICU_IRQ15_Start(void)
{
    /* Enable IRQ15 interrupt */
    IEN(ICU,IRQ15) = 1U;
}

/***********************************************************************************************************************
* Function Name: R_Config_ICU_IRQ15_Stop
* Description  : This function disables IRQ15 interrupt
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_ICU_IRQ15_Stop(void)
{
    /* Disable IRQ15 interrupt */
    IEN(ICU,IRQ15) = 0U;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
