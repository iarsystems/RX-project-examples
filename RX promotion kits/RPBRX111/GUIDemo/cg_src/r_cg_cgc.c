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
* File Name    : r_cg_cgc.c
* Version      : Applilet4 for RX111 V1.00.00.05 [06 Jun 2013]
* Device(s)    : R5F51115AxFM
* Tool-Chain   : IAR Systems icc78k0r
* Description  : This file implements device driver for CGC module.
* Creation Date: 08.10.2013
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
    /* Set main clock control registers */
    SYSTEM.MOFCR.BYTE = _00_CGC_MAINOSC_RESONATOR | _20_CGC_MAINOSC_OVER10M;
    SYSTEM.MOSCWTCR.BYTE = _00_CGC_OSC_WAIT_CYCLE_2;

    /* Set main clock operation */
    SYSTEM.MOSCCR.BIT.MOSTP = 0U;

    /* Wait for main clock oscillator wait counter overflow */
    while (1U != SYSTEM.OSCOVFSR.BIT.MOOVF);

    /* Set system clock */
    SYSTEM.SCKCR.LONG = _00000006_CGC_PCLKD_DIV_64 | _00000600_CGC_PCLKB_DIV_64 | _00000000_CGC_ICLK_DIV_1 | 
                        _60000000_CGC_FCLK_DIV_64;

    /* Disable sub-clock */
    SYSTEM.SOSCCR.BIT.SOSTP = 1U;

    /* Wait for the register modification to complete */
    while (1U != SYSTEM.SOSCCR.BIT.SOSTP);

    /* Set clock source */
    SYSTEM.SCKCR3.WORD = _0200_CGC_CLOCKSOURCE_MAINCLK;
}
/***********************************************************************************************************************
* Function Name: R_CGC_Set_ClockMode
* Description  : This function changes clock generator operation mode.
* Arguments    : mode -
*                    clock generator operation mode
* Return Value : status -
*                    MD_OK, MD_ARGERROR, MD_ERROR1 or MD_ERROR2
***********************************************************************************************************************/
MD_STATUS R_CGC_Set_ClockMode(clock_mode_t mode)
{
    MD_STATUS status = MD_OK;
    clock_mode_t old_mode;

    /* Protection off */
    SYSTEM.PRCR.WORD = 0xA501;

    if (SYSTEM.SCKCR3.WORD == _0000_CGC_CLOCKSOURCE_LOCO)
    {
        old_mode = LOCO;
    }
    else if (SYSTEM.SCKCR3.WORD == _0100_CGC_CLOCKSOURCE_HOCO)
    {
        old_mode = HOCO;
    }
    else if (SYSTEM.SCKCR3.WORD == _0200_CGC_CLOCKSOURCE_MAINCLK)
    {
        old_mode = MAINCLK;
    }
    else if (SYSTEM.SCKCR3.WORD == _0300_CGC_CLOCKSOURCE_SUBCLK)
    {
        old_mode = SUBCLK;
    }
    else if (SYSTEM.SCKCR3.WORD == _0400_CGC_CLOCKSOURCE_PLL)
    {
        old_mode = PLLCLK;
    }

    if (mode != old_mode)
    {
        switch (mode)
        {
            case LOCO:

                if (0U != SYSTEM.LOCOCR.BIT.LCSTP)
                {
                    status = MD_ERROR1;
                }
                else
                {
                    SYSTEM.SCKCR3.WORD = _0000_CGC_CLOCKSOURCE_LOCO;
                    
                    while (SYSTEM.SCKCR3.WORD != _0000_CGC_CLOCKSOURCE_LOCO);
                }

                break;

            case HOCO:

                if (0U != SYSTEM.HOCOCR.BIT.HCSTP)
                {
                    status = MD_ERROR1;
                }
                else
                {
                    SYSTEM.SCKCR3.WORD = _0100_CGC_CLOCKSOURCE_HOCO;
                    
                    while (SYSTEM.SCKCR3.WORD != _0100_CGC_CLOCKSOURCE_HOCO);
                }
                break;

            case MAINCLK:

                if (0U != SYSTEM.MOSCCR.BIT.MOSTP)
                {
                    status = MD_ERROR1;
                }
                else
                {
                    SYSTEM.SCKCR3.WORD = _0200_CGC_CLOCKSOURCE_MAINCLK;
                    
                    while (SYSTEM.SCKCR3.WORD != _0200_CGC_CLOCKSOURCE_MAINCLK);
                }

                break;

            case SUBCLK:

                if (0U != SYSTEM.SOSCCR.BIT.SOSTP)
                {
                    status = MD_ERROR1;
                }
                else
                {
                    SYSTEM.SCKCR3.WORD = _0300_CGC_CLOCKSOURCE_SUBCLK;
                    
                    while (SYSTEM.SCKCR3.WORD != _0300_CGC_CLOCKSOURCE_SUBCLK);
                }

                break;

            case PLLCLK:

                if (0U != SYSTEM.PLLCR2.BIT.PLLEN)
                {
                    status = MD_ERROR1;
                }
                else
                {
                    SYSTEM.SCKCR3.WORD = _0400_CGC_CLOCKSOURCE_PLL;
                    
                    while (SYSTEM.SCKCR3.WORD != _0400_CGC_CLOCKSOURCE_PLL);
                }

                break;
        }
    }

    /* Protection on */
    SYSTEM.PRCR.WORD = 0xA500;

    return (status);
}

/* Start user code for adding. Do not edit comment generated here */
/***********************************************************************************************************************
* Function Name: R_CGC_RegisterWrite_Cgc
* Description  : This function enables/disables writing to the registers related to the clock generation circuit.
* Arguments    : enable -
*                    setting of register write protection
* Return Value : None
***********************************************************************************************************************/
void R_CGC_RegisterWrite_Cgc (const protect_mode_t enable)
{
    if (enable == WRITE_ENABLED)
    {
        /* Get value of lower 8 bits */
        uint16_t data = (uint16_t) (SYSTEM.PRCR.WORD & 0x00FFU);

        /* Add A5h to higher 8 bits, plus PRC0  */
        data |= 0xA501U;

        /* Set write enabled */
        SYSTEM.PRCR.WORD = data;
    }
    else
    {
        /* Get value of lower 8 bits except B0 */
        uint16_t data = (uint16_t) (SYSTEM.PRCR.WORD & 0x00FEU);

        /* Add A5h to higher 8 bits */
        data |= 0xA500U;

        /* Set write disabled */
        SYSTEM.PRCR.WORD = data;
    }
}
/*******************************************************************************
* End of function R_CGC_RegisterWrite_Cgc
*******************************************************************************/
/* End user code. Do not edit comment generated here */
