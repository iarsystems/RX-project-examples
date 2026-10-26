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
* Copyright (C) 2013, 2014 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_gpt_user.c
* Version      : Applilet4 for RX71M V1.00.00.03 [28 Feb 2014]
* Device(s)    : R5F571MLCxFC
* Tool-Chain   : IAR Embedded Workbench
* Description  : This file implements device driver for GPT module.
* Creation Date: 28.03.2014
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
#include "r_cg_gpt.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
#define UP    1
#define DOWN  0
uint8_t UpDownA, UpDownB;
uint16_t Interval=40;
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_gpt_gtcia0_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = VECT_PERIA_INTA210
#if FAST_INTERRUPT_VECTOR == VECT_PERIA_INTA210
__fast_interrupt static void r_gpt_gtcia0_interrupt(void)
#else
__interrupt static void r_gpt_gtcia0_interrupt(void)
#endif
{
    /* Start user code. Do not edit comment generated here */
    if (UpDownA==DOWN)
    {
      if (GPT0.GTCCRA > (2*Interval))
      {
        GPT0.GTCCRA -= Interval;
      }
      else
      {
        UpDownA = UP;
      }
    }
    else
    {
      if (GPT0.GTCCRA < (GPT0.GTPR - Interval))
      {
        GPT0.GTCCRA += Interval;
      }
      else
      {
        UpDownA = DOWN;
      }
    }
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: r_gpt_gtcib0_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = VECT_PERIA_INTA211
#if FAST_INTERRUPT_VECTOR == VECT_PERIA_INTA211
__fast_interrupt static void r_gpt_gtcib0_interrupt(void)
#else
__interrupt static void r_gpt_gtcib0_interrupt(void)
#endif
{
    /* Start user code. Do not edit comment generated here */
    if (UpDownB==DOWN)
    {
      if (GPT0.GTCCRB > (2*Interval))
      {
        GPT0.GTCCRB -= Interval;
      }
      else
      {
        UpDownB = UP;
      }
    }
    else
    {
      if (GPT0.GTCCRB < (GPT0.GTPR - Interval))
      {
        GPT0.GTCCRB += Interval;
      }
      else
      {
        UpDownB = DOWN;
      }
    }
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: r_gpt_gtciv0_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = VECT_PERIA_INTA212
#if FAST_INTERRUPT_VECTOR == VECT_PERIA_INTA212
__fast_interrupt static void r_gpt_gtciv0_interrupt(void)
#else
__interrupt static void r_gpt_gtciv0_interrupt(void)
#endif
{
    /* Start user code. Do not edit comment generated here */
  R_GPT0_Stop();
  R_GPT0_Start();
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
