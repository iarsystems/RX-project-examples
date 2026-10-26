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
* File Name    : Config_ICU_user.c
* Version      : 1.3.0
* Device(s)    : R5F565NEDxFC
* Description  : This file implements device driver for Config_ICU.
* Creation Date: 2018-02-22
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

/* SW1 flag global - used to indicate SW1 press from interrupt routine */
volatile uint8_t g_sw1_flag = FALSE;

/* SW2 flag global - used to indicate SW2 press from interrupt routine */
volatile uint8_t g_sw2_flag = FALSE;

/* SW3 flag global - used to indicate SW3 press from interrupt routine */
volatile uint8_t g_sw3_flag = FALSE;

/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_ICU_Create_UserInit
* Description  : This function adds user code after initializing the ICU module
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_ICU_Create_UserInit(void)
{
    /* Start user code for user init. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_Config_ICU_irq11_interrupt
* Description  : This function is IRQ11 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

#pragma vector = VECT_ICU_IRQ11
#if FAST_INTERRUPT_VECTOR == VECT_ICU_IRQ11
__fast_interrupt static void r_Config_ICU_irq11_interrupt(void)
#else
__interrupt static void r_Config_ICU_irq11_interrupt(void)
#endif
{
    /* Start user code for r_Config_ICU_irq11_interrupt. Do not edit comment generated here */
	
    /* Indicate SW1 pressed */
    g_sw1_flag = TRUE;

    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_Config_ICU_irq13_interrupt
* Description  : This function is IRQ13 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

#pragma vector = VECT_ICU_IRQ13
#if FAST_INTERRUPT_VECTOR == VECT_ICU_IRQ13
__fast_interrupt static void r_Config_ICU_irq13_interrupt(void)
#else
__interrupt static void r_Config_ICU_irq13_interrupt(void)
#endif
{
    /* Start user code for r_Config_ICU_irq13_interrupt. Do not edit comment generated here */

    /* Indicate SW2 pressed */
    g_sw2_flag = TRUE;

    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_Config_ICU_irq15_interrupt
* Description  : This function is IRQ15 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

#pragma vector = VECT_ICU_IRQ15
#if FAST_INTERRUPT_VECTOR == VECT_ICU_IRQ15
__fast_interrupt static void r_Config_ICU_irq15_interrupt(void)
#else
__interrupt static void r_Config_ICU_irq15_interrupt(void)
#endif
{
    /* Start user code for r_Config_ICU_irq15_interrupt. Do not edit comment generated here */

    /* Indicate SW3 pressed */
    g_sw3_flag = TRUE;

    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

