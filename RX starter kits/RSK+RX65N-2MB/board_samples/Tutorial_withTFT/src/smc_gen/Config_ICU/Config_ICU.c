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
* Copyright (C) 2016, 2017 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : Config_ICU.c
* Version      : 1.3.0
* Device(s)    : R5F565NEDxFC
* Description  : This file implements device driver for Config_ICU.
* Creation Date: 2018-02-21
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
    ICU.IER[0x08].BYTE = _00_ICU_IRQ0_DISABLE | _00_ICU_IRQ1_DISABLE | _00_ICU_IRQ2_DISABLE | _00_ICU_IRQ3_DISABLE |
                         _00_ICU_IRQ4_DISABLE | _00_ICU_IRQ5_DISABLE | _00_ICU_IRQ6_DISABLE | _00_ICU_IRQ7_DISABLE;
    ICU.IER[0x09].BYTE = _00_ICU_IRQ8_DISABLE | _00_ICU_IRQ9_DISABLE | _00_ICU_IRQ10_DISABLE | _00_ICU_IRQ11_DISABLE |
                         _00_ICU_IRQ12_DISABLE | _00_ICU_IRQ13_DISABLE | _00_ICU_IRQ14_DISABLE | _00_ICU_IRQ15_DISABLE;

    /* Disable software interrupt */
    IEN(ICU,SWINT) = 0U;
    IEN(ICU,SWINT2) = 0U;

    /* Disable IRQ8~15 digital filter */
    ICU.IRQFLTE1.BYTE &= ~( _08_ICU_IRQ11_FILTER_ENABLE | _20_ICU_IRQ13_FILTER_ENABLE);

    /* Set IRQ detection type */
    ICU.IRQCR[11].BYTE = _04_ICU_IRQ_EDGE_FALLING;
    IR(ICU,IRQ11) = 0U;
    ICU.IRQCR[13].BYTE = _04_ICU_IRQ_EDGE_FALLING;
    IR(ICU,IRQ13) = 0U;

    /* Set IRQ11 priority level */
    IPR(ICU,IRQ11) = _0F_ICU_PRIORITY_LEVEL15;

    /* Set IRQ13 priority level */
    IPR(ICU,IRQ13) = _0F_ICU_PRIORITY_LEVEL15;

    /* Set IRQ11 pin */
    MPC.P03PFS.BYTE = 0x40U;
    PORT0.PDR.BYTE &= 0xF7U;
    PORT0.PMR.BYTE &= 0xF7U;

    /* Set IRQ13 pin */
    MPC.P05PFS.BYTE = 0x40U;
    PORT0.PDR.BYTE &= 0xDFU;
    PORT0.PMR.BYTE &= 0xDFU;

    R_Config_ICU_Create_UserInit();
}

/***********************************************************************************************************************
* Function Name: R_Config_ICU_IRQ11_Start
* Description  : This function enables IRQ11 interrupt
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_ICU_IRQ11_Start(void)
{
    /* Enable IRQ11 interrupt */
    IEN(ICU,IRQ11) = 1U;
}

/***********************************************************************************************************************
* Function Name: R_Config_ICU_IRQ11_Stop
* Description  : This function disables IRQ11 interrupt
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_ICU_IRQ11_Stop(void)
{
    /* Disable IRQ11 interrupt */
    IEN(ICU,IRQ11) = 0U;
}

/***********************************************************************************************************************
* Function Name: R_Config_ICU_IRQ13_Start
* Description  : This function enables IRQ13 interrupt
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_ICU_IRQ13_Start(void)
{
    /* Enable IRQ13 interrupt */
    IEN(ICU,IRQ13) = 1U;
}

/***********************************************************************************************************************
* Function Name: R_Config_ICU_IRQ13_Stop
* Description  : This function disables IRQ13 interrupt
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_ICU_IRQ13_Stop(void)
{
    /* Disable IRQ13 interrupt */
    IEN(ICU,IRQ13) = 0U;
}

/* Start user code for adding. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_ICU_IRQIsFallingEdge
* Description  : This function returns 1 if the specified ICU_IRQ is set to falling edge triggered, otherwise 0.
* Arguments    : uint8_t irq_no
* Return Value : 1 if falling edge triggered, 0 if not
***********************************************************************************************************************/
uint8_t R_ICU_IRQIsFallingEdge (const uint8_t irq_no)
{
    uint8_t falling_edge_trig = 0x0;

    if (ICU.IRQCR[irq_no].BYTE & _04_ICU_IRQ_EDGE_FALLING)
    {
        falling_edge_trig = 1;
    }

    return (falling_edge_trig);

}

/***********************************************************************************************************************
* End of function R_ICU_IRQIsFallingEdge
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: R_ICU_IRQSetFallingEdge
* Description  : This function sets/clears the falling edge trigger for the specified ICU_IRQ.
* Arguments    : uint8_t irq_no
*                uint8_t set_f_edge, 1 if setting falling edge triggered, 0 if clearing
* Return Value : None
***********************************************************************************************************************/
void R_ICU_IRQSetFallingEdge (const uint8_t irq_no, const uint8_t set_f_edge)
{
    if (1 == set_f_edge)
    {
        ICU.IRQCR[irq_no].BYTE |= _04_ICU_IRQ_EDGE_FALLING;
    }
    else
    {
        ICU.IRQCR[irq_no].BYTE &= (uint8_t) ~_04_ICU_IRQ_EDGE_FALLING;
    }
}

/***********************************************************************************************************************
* End of function R_ICU_IRQSetFallingEdge
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: R_ICU_IRQSetRisingEdge
* Description  : This function sets/clear the rising edge trigger for the specified ICU_IRQ.
* Arguments    : uint8_t irq_no
*                uint8_t set_r_edge, 1 if setting rising edge triggered, 0 if clearing
* Return Value : None
***********************************************************************************************************************/
void R_ICU_IRQSetRisingEdge (const uint8_t irq_no, const uint8_t set_r_edge)
{
    if (1 == set_r_edge)
    {
        ICU.IRQCR[irq_no].BYTE |= _08_ICU_IRQ_EDGE_RISING;
    }
    else
    {
        ICU.IRQCR[irq_no].BYTE &= (uint8_t) ~_08_ICU_IRQ_EDGE_RISING;
    }
}

/***********************************************************************************************************************
* End of function R_ICU_IRQSetRisingEdge
***********************************************************************************************************************/

/* End user code. Do not edit comment generated here */

