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
* Copyright (C) 2015, 2017 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_cgc.c
* Version      : Applilet4 for RX24U V1.03.01.04 [02 Nov 2017]
* Device(s)    : R5F524UEAxFB
* Tool-Chain   : IAR Systems ICCRX
* Description  : This file implements device driver for CGC module.
* Creation Date: 2018-06-13
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
#include "r_cg_cgc.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_CGC_Create
* Description  : This function initializes the clock generator.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_CGC_Create(void)
{
    uint32_t sckcr_dummy;
    uint16_t w_count;

    /* Set main clock control registers */
    SYSTEM.MOFCR.BYTE = _00_CGC_MAINOSC_RESONATOR | _20_CGC_MAINOSC_OVER10M;
    SYSTEM.MOSCWTCR.BYTE = _04_CGC_OSC_WAIT_CYCLE_8192;

    /* Set main clock operation */
    SYSTEM.MOSCCR.BIT.MOSTP = 0U;

    /* Wait for main clock oscillator wait counter overflow */
    while (1U != SYSTEM.OSCOVFSR.BIT.MOOVF);

    /* Set PLL circuit */
    SYSTEM.PLLCR.WORD = _0001_CGC_PLL_FREQ_DIV_2 | _0000_CGC_PLL_SOURCE_MAIN | _0F00_CGC_PLL_FREQ_MUL_8_0;
    SYSTEM.PLLCR2.BIT.PLLEN = 0U;

    /* Wait for PLL wait counter overflow */
    while (1U != SYSTEM.OSCOVFSR.BIT.PLOVF);

    /* Change to high-speed operating mode */
    if (0U != SYSTEM.OPCCR.BIT.OPCM)
    {
      SYSTEM.OPCCR.BIT.OPCM = 0U;
      
      while (0U != SYSTEM.OPCCR.BIT.OPCMTSF);
    }

    /* Set clock source */
    SYSTEM.SCKCR3.WORD = _0400_CGC_CLOCKSOURCE_PLL;

    while (_0400_CGC_CLOCKSOURCE_PLL != SYSTEM.SCKCR3.WORD);

    /* Wait for the stabilization time */
    for (w_count = 0U; w_count < _0321_CGC_STABLE_WAIT; w_count++)
    {
        nop();
    }

    /* Set SCKCR register to change to 16MHz < ICLK <= 32MHz */
    SYSTEM.SCKCR.BIT.ICK = 2UL;

    while (2UL != SYSTEM.SCKCR.BIT.ICK);

    /* Wait for the stabilization time */
    for (w_count = 0U; w_count < _0321_CGC_STABLE_WAIT; w_count++)
    {
        nop();
    }

    /* Set memory wait cycle setting register */
    SYSTEM.MEMWAIT.BIT.MEMWAIT = 1U;

    while (1U != SYSTEM.MEMWAIT.BIT.MEMWAIT);

    /* Set SCKCR register to change to 32MHz < ICLK <= 64MHz */
    SYSTEM.SCKCR.BIT.ICK = 1UL;

    while (1UL != SYSTEM.SCKCR.BIT.ICK);

    /* Wait for the stabilization time */
    for (w_count = 0U; w_count < _0321_CGC_STABLE_WAIT; w_count++)
    {
        nop();
    }

    /* Set memory wait cycle setting register */
    SYSTEM.MEMWAIT.BIT.MEMWAIT = 2U;

    while (2U != SYSTEM.MEMWAIT.BIT.MEMWAIT);

    /* Set system clock */
    sckcr_dummy = _00000001_CGC_PCLKD_DIV_2 | _00000100_CGC_PCLKB_DIV_2 | _00000000_CGC_PCLKA_DIV_1 | 
                  _00000000_CGC_ICLK_DIV_1 | _20000000_CGC_FCLK_DIV_4 | _00010000_CGC_SCKCR_RESERVED;
    SYSTEM.SCKCR.LONG = sckcr_dummy;

    while (SYSTEM.SCKCR.LONG != sckcr_dummy);

    /* Wait for the stabilization time */
    for (w_count = 0U; w_count < _0321_CGC_STABLE_WAIT; w_count++)
    {
        nop();
    }

    /* Set LOCO */
    SYSTEM.LOCOCR.BIT.LCSTP = 0U;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
