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
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : Config_ICU.c
* Version      : 1.7.0
* Device(s)    : R5F572TKCxFB
* Description  : This file implements device driver for Config_ICU.
* Creation Date: 2019-05-21
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
    ICU.IRQFLTE0.BYTE &= ~( _01_ICU_IRQ0_FILTER_ENABLE);
    ICU.IRQFLTE1.BYTE &= ~( _02_ICU_IRQ9_FILTER_ENABLE);

    /* Set IRQ detection type */
    ICU.IRQCR[0].BYTE = _04_ICU_IRQ_EDGE_FALLING;
    IR(ICU,IRQ0) = 0U;
    ICU.IRQCR[9].BYTE = _04_ICU_IRQ_EDGE_FALLING;
    IR(ICU,IRQ9) = 0U;

    /* Set IRQ priority level */
    IPR(ICU,IRQ0) = _0F_ICU_PRIORITY_LEVEL15;
    IPR(ICU,IRQ9) = _0F_ICU_PRIORITY_LEVEL15;

    /* Set IRQ0 pin */
    MPC.P10PFS.BYTE = 0x40U;
    PORT1.PDR.BYTE &= 0xFEU;
    PORT1.PMR.BYTE &= 0xFEU;

    /* Set IRQ9 pin */
    MPC.PB3PFS.BYTE = 0x40U;
    PORTB.PDR.BYTE &= 0xF7U;
    PORTB.PMR.BYTE &= 0xF7U;

    R_Config_ICU_Create_UserInit();
}

/***********************************************************************************************************************
* Function Name: R_Config_ICU_IRQ0_Start
* Description  : This function enables IRQ0 interrupt
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_ICU_IRQ0_Start(void)
{
    /* Enable IRQ0 interrupt */
    IEN(ICU,IRQ0) = 1U;
}

/***********************************************************************************************************************
* Function Name: R_Config_ICU_IRQ0_Stop
* Description  : This function disables IRQ0 interrupt
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_ICU_IRQ0_Stop(void)
{
    /* Disable IRQ0 interrupt */
    IEN(ICU,IRQ0) = 0U;
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

    /* Check if IRQCRx is falling edge setting */
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
        /* Set IRQCRx to falling edge */
        ICU.IRQCR[irq_no].BYTE |= _04_ICU_IRQ_EDGE_FALLING;
    }
    else
    {
        /* Clear IRQCRx */
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
        /* Set IRQCRx to rising edge */
        ICU.IRQCR[irq_no].BYTE |= _08_ICU_IRQ_EDGE_RISING;
    }
    else
    {
        /* Clear IRQCRx */
        ICU.IRQCR[irq_no].BYTE &= (uint8_t) ~_08_ICU_IRQ_EDGE_RISING;
    }
}

/***********************************************************************************************************************
* End of function R_ICU_IRQSetRisingEdge
***********************************************************************************************************************/

/* End user code. Do not edit comment generated here */

