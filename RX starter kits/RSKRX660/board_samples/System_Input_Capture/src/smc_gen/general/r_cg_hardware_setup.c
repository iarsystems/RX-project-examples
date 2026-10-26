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
* File Name        : r_cg_hardware_setup.c
* Version          : 1.0.20
* Device(s)        : R5F56609HxFB
* Description      : Initialization file for code generation configurations.
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
#include "Config_CMT0.h"
#include "Config_CMT3.h"
#include "Config_ICU.h"
#include "Config_PORT.h"
#include "Config_LPC.h"
#include "Config_RTC.h"
#include "Config_SCI10.h"
#include "Config_RIIC0.h"
#include "Config_S12AD0.h"
#include "Config_SCI6.h"
#include "r_smc_cgc.h"
#include "r_smc_interrupt.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_undefined_exception
* Description  : This function is undefined interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void r_undefined_exception(void)
{
    /* Start user code for r_undefined_exception. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: R_Systeminit
* Description  : This function initializes every configuration
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Systeminit(void)
{
    /* Enable writing to registers related to operating modes, LPC, CGC and software reset */
    SYSTEM.PRCR.WORD = 0xA50BU;

    /* Enable writing to MPC pin function control registers */
    MPC.PWPR.BIT.B0WI = 0U;
    MPC.PWPR.BIT.PFSWE = 1U;

    /* Write 0 to the target bits in the POECR2 registers */
    POE3.POECR2.WORD = 0x0000U;

    /* Initialize clocks settings */
    R_CGC_Create();

    /* Set peripheral settings */
    R_Config_PORT_Create();
    R_Config_CMT0_Create();
    R_Config_CMT3_Create();
    R_Config_ICU_Create();
    R_Config_LPC_Create();
    R_Config_RTC_Create();
    R_Config_SCI10_Create();
    R_Config_RIIC0_Create();
    R_Config_S12AD0_Create();
    R_Config_SCI6_Create();

    /* Set interrupt settings */
    R_Interrupt_Create();

    /* Register undefined interrupt */
    R_BSP_InterruptWrite(BSP_INT_SRC_UNDEFINED_INTERRUPT,(bsp_int_cb_t)r_undefined_exception);

    /* Register group AL0 interrupt TEI10 (SCI10) */
    R_BSP_InterruptWrite(BSP_INT_SRC_AL0_SCI10_TEI10,(bsp_int_cb_t)r_Config_SCI10_transmitend_interrupt);

    /* Register group AL0 interrupt ERI10 (SCI10) */
    R_BSP_InterruptWrite(BSP_INT_SRC_AL0_SCI10_ERI10,(bsp_int_cb_t)r_Config_SCI10_receiveerror_interrupt);

    /* Register group BL1 interrupt TEI0 (RIIC0) */
    R_BSP_InterruptWrite(BSP_INT_SRC_BL1_RIIC0_TEI0,(bsp_int_cb_t)r_Config_RIIC0_transmitend_interrupt);

    /* Register group BL1 interrupt EEI0 (RIIC0) */
    R_BSP_InterruptWrite(BSP_INT_SRC_BL1_RIIC0_EEI0,(bsp_int_cb_t)r_Config_RIIC0_error_interrupt);

    /* Register group BL0 interrupt TEI6 (SCI6) */
    R_BSP_InterruptWrite(BSP_INT_SRC_BL0_SCI6_TEI6,(bsp_int_cb_t)r_Config_SCI6_transmitend_interrupt);

    /* Disable writing to MPC pin function control registers */
    MPC.PWPR.BIT.PFSWE = 0U;
    MPC.PWPR.BIT.B0WI = 1U;

    /* Enable protection */
    SYSTEM.PRCR.WORD = 0xA500U;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

