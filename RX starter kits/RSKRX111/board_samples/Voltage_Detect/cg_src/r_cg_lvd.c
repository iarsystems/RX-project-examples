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
* File Name    : r_cg_lvd.c
* Version      : Applilet4 for RX111 V1.00.00.05 [06 Jun 2013]
* Device(s)    : R5F51115AxFM
* Tool-Chain   : IAR Systems icc78k0r
* Description  : This file implements device driver for LVD module.
* Creation Date: 2013-08-29
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
#include "r_cg_lvd.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_LVD1_Create
* Description  : This function initializes the LVD1 channel.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_LVD1_Create(void)
{
    uint16_t protect_dummy = (uint16_t)(SYSTEM.PRCR.WORD & 0x000BU);

    /* Disable protect bit */
    SYSTEM.PRCR.WORD = 0xA508U;

    /* Disable LVD interrupt */
    IEN(LVD,LVD1) = 0U;

    /* Set control registers */
    SYSTEM.LVCMPCR.BIT.LVD1E = 0U;
    SYSTEM.LVCMPCR.BIT.EXVCCINP2 = 0U;
    SYSTEM.LVDLVLR.BYTE |= _05_LVD_LVD1LVL_5;
    SYSTEM.LVD1CR0.BIT.LVD1RI = 0U;
    SYSTEM.LVD1CR1.BYTE = _04_LVD_LVD1IRQSEL_M | _02_LVD_LVD1IDTSEL_B;

    /* Set LVD1 priority level */
    IPR(LVD,LVD1) = _0F_LVD_PRIORITY_LEVEL15;

    /* Restore the previous state of the protect register */
    SYSTEM.PRCR.WORD = (uint16_t)(0xA500U | protect_dummy);
}
/***********************************************************************************************************************
* Function Name: R_LVD1_Start
* Description  : This function starts the LVD1 channel monitoring.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_LVD1_Start(void)
{
    uint16_t protect_dummy = (uint16_t)(SYSTEM.PRCR.WORD & 0x000BU);
    uint8_t w_count;

    /* Disable protect bit */
    SYSTEM.PRCR.WORD = 0xA508U;

    SYSTEM.LVCMPCR.BIT.LVD1E = 1U;
    
    /* Wait for LVD voltage detection to start */
    for (w_count = 0U; w_count <= _31_LVD1_STABLE_WAIT_TIME; w_count++)
    {
        nop();
    }

    SYSTEM.LVD1CR0.BIT.LVD1CMPE = 1U;
    SYSTEM.LVD1SR.BIT.LVD1DET = 0U;
    SYSTEM.LVD1CR0.BIT.LVD1RIE = 1U;
    /* Enable LVD1 interrupt in ICU */
    IEN(LVD,LVD1) = 1U;

    /* Restore the previous state of the protect register */
    SYSTEM.PRCR.WORD = (uint16_t)(0xA500U | protect_dummy);
}
/***********************************************************************************************************************
* Function Name: R_LVD1_Stop
* Description  : This function stops the LVD1 channel monitoring.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_LVD1_Stop(void)
{
    uint16_t protect_dummy = (uint16_t)(SYSTEM.PRCR.WORD & 0x000BU);

    /* Disable protect bit */
    SYSTEM.PRCR.WORD = 0xA508U;

    /* Disable LVD1 interrupt in ICU */
    IEN(LVD,LVD1) = 0U;

    SYSTEM.LVD1CR0.BIT.LVD1RIE = 0U;
    SYSTEM.LVD1CR0.BIT.LVD1CMPE = 0U;
    SYSTEM.LVCMPCR.BIT.LVD1E = 0U;

    /* Restore the previous state of the protect register */
    SYSTEM.PRCR.WORD = (uint16_t)(0xA500U | protect_dummy);
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
