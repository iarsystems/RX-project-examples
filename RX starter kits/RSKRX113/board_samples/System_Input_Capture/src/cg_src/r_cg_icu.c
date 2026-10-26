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
* File Name    : r_cg_icu.c
* Version      : Applilet4 for RX113 V1.01.00.02 [31 Jul 2014]
* Device(s)    : R5F51138AxFP
* Tool-Chain   : IAR Systems ICCRX
* Description  : This file implements device driver for ICU module.
* Creation Date: 2015-01-20
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
#include "r_cg_icu.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_ICU_Create
* Description  : This function initializes ICU module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_ICU_Create(void)
{
    /* Disable IRQ0~7 interrupts */
    ICU.IER[0x08].BYTE = _00_ICU_IRQ0_DISABLE | _00_ICU_IRQ1_DISABLE | _00_ICU_IRQ2_DISABLE | _00_ICU_IRQ3_DISABLE | 
                         _00_ICU_IRQ4_DISABLE | _00_ICU_IRQ5_DISABLE | _00_ICU_IRQ6_DISABLE | _00_ICU_IRQ7_DISABLE;

    /* Set IRQ settings */
    ICU.IRQCR[2].BYTE = _00_ICU_IRQ_EDGE_LOW_LEVEL;
    ICU.IRQCR[3].BYTE = _00_ICU_IRQ_EDGE_LOW_LEVEL;

    /* Set IRQ2 priority level */
    IPR(ICU,IRQ2) = _0F_ICU_PRIORITY_LEVEL15;

    /* Set IRQ3 priority level */
    IPR(ICU,IRQ3) = _0F_ICU_PRIORITY_LEVEL15;

    /* Set IRQ2 pin */
    MPC.P32PFS.BYTE = 0x40U;
    PORT3.PDR.BYTE &= 0xFBU;
    PORT3.PMR.BYTE &= 0xFBU;

    /* Set IRQ3 pin */
    MPC.P27PFS.BYTE = 0x40U;
    PORT2.PDR.BYTE &= 0x7FU;
    PORT2.PMR.BYTE &= 0x7FU;
}
/***********************************************************************************************************************
* Function Name: R_ICU_IRQ2_Start
* Description  : This function enables IRQ2 interrupt.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_ICU_IRQ2_Start(void)
{
    /* Enable IRQ2 interrupt */
    IEN(ICU,IRQ2) = 1U; 
}
/***********************************************************************************************************************
* Function Name: R_ICU_IRQ2_Stop
* Description  : This function disables IRQ2 interrupt.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_ICU_IRQ2_Stop(void)
{
    /* Disable IRQ2 interrupt */
    IEN(ICU,IRQ2) = 0U; 
}
/***********************************************************************************************************************
* Function Name: R_ICU_IRQ3_Start
* Description  : This function enables IRQ3 interrupt.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_ICU_IRQ3_Start(void)
{
    /* Enable IRQ3 interrupt */
    IEN(ICU,IRQ3) = 1U; 
}
/***********************************************************************************************************************
* Function Name: R_ICU_IRQ3_Stop
* Description  : This function disables IRQ3 interrupt.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_ICU_IRQ3_Stop(void)
{
    /* Disable IRQ3 interrupt */
    IEN(ICU,IRQ3) = 0U; 
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
