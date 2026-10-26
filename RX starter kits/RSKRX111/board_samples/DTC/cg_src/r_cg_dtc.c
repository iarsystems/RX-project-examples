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
* File Name    : r_cg_dtc.c
* Version      : Applilet4 for RX111 V1.00.00.05 [06 Jun 2013]
* Device(s)    : R5F51115AxFM
* Tool-Chain   : IAR Systems icc78k0r
* Description  : This file implements device driver for DTC module.
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
#include "r_cg_dtc.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

uint32_t * dtc_vectortable = (uint32_t * ) 0x00001000U;
/***********************************************************************************************************************
* Function Name: R_DTC_Create
* Description  : This function initializes the DTC module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_DTC_Create(void)
{
    /* Cancel module stop state */
    SYSTEM.MSTPCRA.BIT.MSTPA28 = 0U;

    /* Disable tansfer data read skip to clear the flag */
    DTC.DTCCR.BYTE = _08_DTC_TRANSFER_READSKIP_DISABLE;

    /* Set DTC0 transfer data */
    dtc_transferdata0.mra_sar = ((uint32_t)(_00_DTC_SRC_ADDRESS_FIXED | _10_DTC_TRANSFER_SIZE_16BIT | _00_DTC_TRANSFER_MODE_NORMAL) << 24U) | (_00089020_DTC0_SRC_ADDRESS & 0x00FFFFFF);
    dtc_transferdata0.mrb_dar = ((uint32_t)(_08_DTC_DST_ADDRESS_INCREMENTED | _00_DTC_INTERRUPT_COMPLETED) << 24U) | (_00001400_DTC0_DST_ADDRESS & 0x00FFFFFF);
    dtc_transferdata0.cra_crb = (uint32_t)(_0010_DTC0_TRANSFER_COUNT_CRA) << 16U;

    /* Set transfer data start address in DTC vector table */
    * (dtc_vectortable + 102) = _00001500_DTC0_TRANSDATA_ADDRESS;

    /* Set address mode */
    DTC.DTCADMOD.BYTE = _01_DTC_ADDRESS_MODE_SHORT;

    /* Set base address */
    DTC.DTCVBR = (void *)0x00001000U;

    /* Enable DTC module start */
    DTC.DTCST.BYTE = _01_DTC_MODULE_START;
}
/***********************************************************************************************************************
* Function Name: R_DTC0_Start
* Description  : This function enables DTC0 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_DTC0_Start(void)
{
    /* Set enable bit for start source interrupt */
    ICU.DTCER[102].BIT.DTCE = 1U;
}
/***********************************************************************************************************************
* Function Name: R_DTC0_Stop
* Description  : This function disables DTC0 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_DTC0_Stop(void)
{
    /* Clear enable bit for start source interrupt */
    ICU.DTCER[102].BIT.DTCE = 0U;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
