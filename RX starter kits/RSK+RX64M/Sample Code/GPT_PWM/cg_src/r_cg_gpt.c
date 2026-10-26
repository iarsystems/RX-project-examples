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
* File Name    : r_cg_gpt.c
* Version      : Applilet4 for RX64M V1.00.00.03 [28 Feb 2014]
* Device(s)    : R5F564MLCxFC
* Tool-Chain   : IAR Systems ICCRX
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
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_GPT_Create
* Description  : This function initializes the GPT module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_GPT_Create(void)
{
    /* Disable GPT0 interrupts */
    IEN(PERIA, INTA210) = 0U;
    IEN(PERIA, INTA211) = 0U;
    IEN(PERIA, INTA212) = 0U;

    /* Cancel GPT module stop state*/
    MSTP(GPT) = 0U;

    /* Disable GPT0 control registers write protect */
    GPT.GTWP.BIT.WP0 = 0U;

    /* Set GPT0 mode, and counter clear */
    GPT0.GTCR.WORD = _0000_GPTn_COUNTER_CLEAR_NONE | _0000_GPTn_MODE_SAW_WAVE_PWM;

    /* Set GPT0 count direction */
    GPT0.GTUDC.WORD = _0001_GPTn_COUNT_UP;

    /* Set GPT0 count clock */
    GPT0.GTCR.BIT.TPCS = _00_GPTn_COUNT_CLK_PCLKA_1;

    /* Set GPT0 cycle */
    GPT0.GTPR = _FFF0_GPT0_CYCLE_VALUE;

    /* Set GPT0 counter initial value */
    GPT0.GTCNT = _0000_GPT0_INIT_COUNT;

    /* Set GTIOC0A, GTIOC0B pin function */
    GPT0.GTIOR.WORD = _0000_GPTn_GTIOCA_OUTPUT_ON_START_SET | _0000_GPTn_GTIOCA_OUTPUT_ON_STOP_0 | 
                      _0000_GPTn_GTCCRA_COMP_MATCH | _0010_GPTn_GTIOCA_INIT_OUT_HIGH | 
                      _0000_GPTn_GTIOCA_CYCLE_END_RETAIN | _0001_GPTn_GTIOCA_COMP_MATCH_LOW | 
                      _0000_GPTn_GTIOCB_OUTPUT_ON_START_SET | _0000_GPTn_GTIOCB_OUTPUT_ON_STOP_0 | 
                      _0000_GPTn_GTCCRB_COMP_MATCH | _1000_GPTn_GTIOCB_INIT_OUT_HIGH | 
                      _0000_GPTn_GTIOCB_CYCLE_END_RETAIN | _0100_GPTn_GTIOCB_COMP_MATCH_LOW;

    /* Enable GTIOC0A, GTIOC0B pin output and set negation */
    GPT0.GTONCR.WORD = _8000_GPTn_GTCIOCB_OUTPUT_ENABLE | _4000_GPTn_GTCIOCA_OUTPUT_ENABLE;

    /* Enable/Disable GPT0 buffers */
    GPT.GTBDR.BIT.BD00 = 1U;
    GPT.GTBDR.BIT.BD02 = 1U;
    GPT.GTBDR.BIT.BD03 = 1U;

    /* Set GPT0 buffer operations */
    GPT0.GTBER.WORD = _0000_GPTn_GTADTRB_BUF_TRANS_NONE | _0000_GPTn_GTADTRA_BUF_TRANS_NONE | 
                      _0010_GPTn_GTPR_BUF_SINGLE | _0000_GPTn_GTCCRB_BUF_NONE | _0000_GPTn_GTCCRA_BUF_NONE;

    /* Set GPT0 compare match value */
    GPT0.GTCCRA = _4E20_GPT0_COMP_MATCH_A_VALUE;
    GPT0.GTCCRB = _9C40_GPT0_COMP_MATCH_B_VALUE;
    GPT0.GTCCRC = _0064_GPT0_COMP_MATCH_C_VALUE;
    GPT0.GTCCRD = _0064_GPT0_COMP_MATCH_D_VALUE;
    GPT0.GTCCRE = _0064_GPT0_COMP_MATCH_E_VALUE;
    GPT0.GTCCRF = _0064_GPT0_COMP_MATCH_F_VALUE;

    /* Set GPT0 buffer value */
    GPT0.GTPBR = _FFF0_GPT0_CYCLE_VALUE;

    /* Set GPT0 dead time setting */
    GPT0.GTDTCR.WORD = _0000_GPTn_DEAD_TIME_DISABLE;

    /* Set GPT0 interrupt and A/D conversion request generation */
    GPT0.GTINTAD.WORD = _0040_GPTn_GTCIV_INTERRUPT_ENABLE | _0002_GPTn_GTCIB_INTERRUPT_ENABLE | 
                        _0001_GPTn_GTCIA_INTERRUPT_ENABLE;

    /* Set GPT0 interrupt and A/D conversion request skipping */
    GPT0.GTITC.WORD = _0000_GPTn_GTCI_SKIPPING_DISABLE;

    /* Configure GPT0 interrupts */
    ICU.SLIAR210.BYTE = 0x2FU;
    IPR(PERIA, INTA210) = _09_GPT_PRIORITY_LEVEL9;
    ICU.SLIAR211.BYTE = 0x30U;
    IPR(PERIA, INTA211) = _09_GPT_PRIORITY_LEVEL9;
    ICU.SLIAR212.BYTE = 0x36U;
    IPR(PERIA, INTA212) = _0B_GPT_PRIORITY_LEVEL11;
    /* Set GTIOC0A pin */
    MPC.PE5PFS.BYTE = 0x1EU;
    PORTE.PMR.BYTE |= 0x20U;
    /* Set GTIOC0B pin */
    MPC.PE2PFS.BYTE = 0x1EU;
    PORTE.PMR.BYTE |= 0x04U;
}
/***********************************************************************************************************************
* Function Name: R_GPT0_Start
* Description  : This function starts GPT0 channel.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_GPT0_Start(void)
{
    /* Enable GPT[#n] interrupts */
    IR(PERIA, INTA210) = 0U;
    IEN(PERIA, INTA210) = 1U;
    IR(PERIA, INTA211) = 0U;
    IEN(PERIA, INTA211) = 1U;
    IR(PERIA, INTA212) = 0U;
    IEN(PERIA, INTA212) = 1U;

    /* Disable GPT0 start write protect */
    GPT.GTSWP.BIT.SWP0 = 0U;

    /* Start GPT0 counting */
    GPT.GTSTR.BIT.CST0 = 1U;

    /* Enable GPT0 start write protect */
    GPT.GTSWP.BIT.SWP0 = 1U;
}
/***********************************************************************************************************************
* Function Name: R_GPT0_Stop
* Description  : This function stops GPT0 channel.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_GPT0_Stop(void)
{
    /* Disable GPT[#n] interrupts */
    IEN(PERIA, INTA210) = 0U;
    IEN(PERIA, INTA211) = 0U;
    IEN(PERIA, INTA212) = 0U;

    /* Disable GPT0 start write protect */
    GPT.GTSWP.BIT.SWP0 = 0U;

    /* Stop GPT0 counting */
    GPT.GTSTR.BIT.CST0 = 0U;

    /* Enable GPT0 start write protect */
    GPT.GTSWP.BIT.SWP0 = 1U;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
