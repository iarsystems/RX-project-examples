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
* File Name    : r_cg_dac.c
* Version      : Applilet4 for RX111 V1.00.00.05 [06 Jun 2013]
* Device(s)    : R5F51115AxFM
* Tool-Chain   : IAR Systems icc78k0r
* Description  : This file implements device driver for DAC module.
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
#include "r_cg_dac.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_DAC_Create
* Description  : This function initializes the DA converter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_DAC_Create(void)
{
    /* Set DA control registers */
    DA.DADPR.BYTE = _00_DAC_DPSEL_R;

    /* Cancel DAC stop state in LPC */
    SYSTEM.MSTPCRA.BIT.MSTPA19 = 0U;

    /* Set MPC for DA0 pin */
    MPC.P03PFS.BYTE = 0x80U;

    /* Set P03 pin */
    PORT0.PDR.BYTE &= 0xF7U;
    PORT0.PMR.BYTE &= 0xF7U;
}

/***********************************************************************************************************************
* Function Name: R_DAC0_Start
* Description  : This function enables the DA0 converter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_DAC0_Start(void)
{
    DA.DACR.BIT.DAOE0 = 1U;  /* Enable DA0 conversion */
}
/***********************************************************************************************************************
* Function Name: R_DAC0_Stop
* Description  : This function stops the DA0 converter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_DAC0_Stop(void)
{
    DA.DACR.BIT.DAOE0 = 0U;  /* Stop DA0 conversion */
}
/***********************************************************************************************************************
* Function Name: R_DAC0_Set_ConversionValue
* Description  : This function sets the DA0 converter value.
* Arguments    : reg_value -
*                    value of conversion
* Return Value : None
***********************************************************************************************************************/
void R_DAC0_Set_ConversionValue(uint16_t reg_value)
{
    DA.DADR0 = (uint16_t)(reg_value & 0x00FFU);
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
