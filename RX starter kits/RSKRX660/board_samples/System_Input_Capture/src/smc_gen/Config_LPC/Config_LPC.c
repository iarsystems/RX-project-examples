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
* File Name        : Config_LPC.c
* Component Version: 2.3.0
* Device(s)        : R5F56609HxFB
* Description      : This file implements device driver for Config_LPC.
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
#include "Config_LPC.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_LPC_Create
* Description  : This function initializes the LPC module
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_LPC_Create(void)
{
    /* Set standby and deep standby control */
    SYSTEM.SBYCR.BIT.OPE = 1U;
    SYSTEM.DPSBYCR.BIT.IOKEEP = 0U;

    /* Set deep standby interrupt registers */
    SYSTEM.DPSIER0.BYTE = _00_LPC_DIRQ0E_DISABLED | _00_LPC_DIRQ1E_DISABLED | _00_LPC_DIRQ2E_DISABLED | 
                          _00_LPC_DIRQ3E_DISABLED | _00_LPC_DIRQ4E_DISABLED | _00_LPC_DIRQ5E_DISABLED | 
                          _00_LPC_DIRQ6E_DISABLED | _00_LPC_DIRQ7E_DISABLED;
    SYSTEM.DPSIER1.BYTE = _00_LPC_DIRQ8E_DISABLED | _00_LPC_DIRQ9E_DISABLED | _00_LPC_DIRQ10E_DISABLED | 
                          _00_LPC_DIRQ11E_DISABLED | _00_LPC_DIRQ12E_DISABLED | _00_LPC_DIRQ13E_DISABLED | 
                          _00_LPC_DIRQ14E_DISABLED | _00_LPC_DIRQ15E_DISABLED;
    SYSTEM.DPSIER2.BYTE = _00_LPC_DNMIE_DISABLED | _00_LPC_DLVD1IE_DISABLED | _00_LPC_DLVD2IE_DISABLED | 
                          _00_LPC_DRTCIIE_DISABLED | _00_LPC_DRTCAIE_DISABLED;

    /* Set sleep mode return clock source */
    SYSTEM.RSTCKCR.BYTE = _00_LPC_RETURN_DISABLED;

    R_Config_LPC_Create_UserInit();
}

/***********************************************************************************************************************
* Function Name: R_Config_LPC_AllModuleClockStop
* Description  : This function stops all module clock
* Arguments    : None
* Return Value : status -
*                    MD_OK
***********************************************************************************************************************/

MD_STATUS R_Config_LPC_AllModuleClockStop(void)
{
    MD_STATUS status = MD_OK;

    uint16_t protect_dummy = (uint16_t)(SYSTEM.PRCR.WORD & 0x000BU);
    uint32_t mstpcra_dummy = SYSTEM.MSTPCRA.LONG;
    uint32_t mstpcrb_dummy = SYSTEM.MSTPCRB.LONG;
    uint32_t mstpcrc_dummy = SYSTEM.MSTPCRC.LONG;
    uint32_t mstpcrd_dummy = SYSTEM.MSTPCRD.LONG;
    uint8_t dummy_dtcst = DTC.DTCST.BIT.DTCST;
    uint8_t dummy_dmac = DMAC.DMAST.BIT.DMST;

    /* Prevent activating DTC and DMAC */
    DTC.DTCST.BIT.DTCST = 0U;
    DMAC.DMAST.BIT.DMST = 0U;

    /* Disable protect bit */
    SYSTEM.PRCR.WORD = 0xA502U | protect_dummy;

    /* Set module stop for CMTW, TMR, MTU, CMT, AD, DA and DMAC/DTC */
    SYSTEM.MSTPCRA.LONG = 0xFFFFFFFFUL;

    /* Set module stop for SCI, DOC, ELC, CMPC, RSPI, RIIC and CRC */
    SYSTEM.MSTPCRB.LONG = 0xFFFFFFFFUL;

    /* Set module stop for RIIC, CAC and SCI */
    SYSTEM.MSTPCRC.LONG = 0xFFFF0000UL;

    /* Set module stop for RSCI, REMC and CANFD */
    SYSTEM.MSTPCRD.LONG = 0xFFFFFFFFUL;

    /* Select sleep or all-module clock stop */
    SYSTEM.SBYCR.BIT.SSBY = 0U;

    /* Prevent out-of-order execution */
    while (0U != SYSTEM.SBYCR.BIT.SSBY)
    {
        nop();
    }

    /* Initiate the low-power mode */
    wait();

    /* Restore the modules that were enabled before stopping */
    SYSTEM.MSTPCRA.LONG = mstpcra_dummy;
    SYSTEM.MSTPCRB.LONG = mstpcrb_dummy;
    SYSTEM.MSTPCRC.LONG = mstpcrc_dummy;
    SYSTEM.MSTPCRD.LONG = mstpcrd_dummy;

    /* Restore the state of DTC and DMAC */
    DTC.DTCST.BIT.DTCST = dummy_dtcst;
    DMAC.DMAST.BIT.DMST = dummy_dmac;

    /* Restore the previous state of the protect register */
    SYSTEM.PRCR.WORD = (uint16_t)(0xA500U | protect_dummy);

    return status;
}

/***********************************************************************************************************************
* Function Name: R_Config_LPC_Sleep
* Description  : This function enables sleep mode
* Arguments    : None
* Return Value : status -
*                    MD_OK or MD_ERROR1
***********************************************************************************************************************/

MD_STATUS R_Config_LPC_Sleep(void)
{
    MD_STATUS status = MD_OK;

    uint16_t protect_dummy = (uint16_t)(SYSTEM.PRCR.WORD & 0x000BU);

    /* When RSTCKEN is enable, the possible clock source is LOCO oscillator */
    if ((SYSTEM.RSTCKCR.BIT.RSTCKEN == 1U) && (SYSTEM.SCKCR3.BIT.CKSEL != 0x00U))
    {
        status = MD_ERROR1;
        return status;
    }

    /* Disable protect bit */
    SYSTEM.PRCR.WORD = 0xA502U | protect_dummy;

    /* Select sleep mode */
    SYSTEM.SBYCR.BIT.SSBY = 0U;

    /* Prevent out-of-order execution */
    while (0U != SYSTEM.SBYCR.BIT.SSBY)
    {
        nop();
    }

    /* Initiate the low-power mode */
    wait();

    /* Restore the previous state of the protect register */
    SYSTEM.PRCR.WORD = (uint16_t)(0xA500U | protect_dummy);

    return status;
}

/***********************************************************************************************************************
* Function Name: R_Config_LPC_SoftwareStandby
* Description  : This function enables software standby mode
* Arguments    : None
* Return Value : status -
*                    MD_OK or MD_ERROR1
***********************************************************************************************************************/

MD_STATUS R_Config_LPC_SoftwareStandby(void)
{
    /* Note: please do not use digital filter in Interrupt Controller */
    MD_STATUS status = MD_OK;

    uint16_t protect_dummy = (uint16_t)(SYSTEM.PRCR.WORD & 0x000BU);
    uint8_t dummy_dtcst = DTC.DTCST.BIT.DTCST;
    uint8_t dummy_dmac = DMAC.DMAST.BIT.DMST;

    /* When oscillation stop detection function is enabled, SSBY bit is invalid */
    /* Do not allow the chip to enter software standby mode while the FLL function of the HOCO is enabled */
    if ((1U == SYSTEM.OSTDCR.BIT.OSTDE) || (1U == SYSTEM.FLLCR1.BIT.FLLEN))
    {
        status = MD_ERROR1;
        return status;
    }
    else
    {
        /* Set DTC and DMAC module to stop state */
        DTC.DTCST.BIT.DTCST = 0U;
        DMAC.DMAST.BIT.DMST = 0U;

        /* Disable protect bit */
        SYSTEM.PRCR.WORD = 0xA502U | protect_dummy;

        /* Select standby mode */
        SYSTEM.SBYCR.BIT.SSBY = 1U;
        SYSTEM.DPSBYCR.BIT.DPSBY = 0U;

        /* Prevent out-of-order execution */
        while (1U != SYSTEM.SBYCR.BIT.SSBY)
        {
            nop();
        }

        /* Initiate the low-power mode */
        wait();
    }

    /* Restore the state of DTC and DMAC if necessary */
    DTC.DTCST.BIT.DTCST = dummy_dtcst;
    DMAC.DMAST.BIT.DMST = dummy_dmac;

    /* Restore the previous state of the protect register */
    SYSTEM.PRCR.WORD = (uint16_t)(0xA500U | protect_dummy);

    return status;
}

/***********************************************************************************************************************
* Function Name: R_Config_LPC_DeepSoftwareStandby
* Description  : This function enables deep software standby mode
* Arguments    : None
* Return Value : status -
*                    MD_OK
***********************************************************************************************************************/

MD_STATUS R_Config_LPC_DeepSoftwareStandby(void)
{
    MD_STATUS status = MD_OK;

    uint16_t protect_dummy = (uint16_t)(SYSTEM.PRCR.WORD & 0x000BU);

    /* Save the current state of DMAC and DTC */
    uint8_t dummy_mstpa28 = SYSTEM.MSTPCRA.BIT.MSTPA28;
    uint8_t dummy_dmac = DMAC.DMAST.BIT.DMST;
    uint8_t dummy_dtcst = DTC.DTCST.BIT.DTCST;

    /* Disable protect bit */
    SYSTEM.PRCR.WORD = 0xA502U | protect_dummy;

    /* Set DMAC and DTC module to stop state */
    DMAC.DMAST.BIT.DMST = 0U;
    DTC.DTCST.BIT.DTCST = 0U;
    SYSTEM.MSTPCRA.BIT.MSTPA28 = 1U;

    /* Enable deep software standby mode */
    SYSTEM.SBYCR.BIT.SSBY = 1U;
    SYSTEM.DPSBYCR.BIT.DPSBY = 1U;

    /* Clear DPSIFR0 */
    if (0x00U != SYSTEM.DPSIFR0.BYTE)
    {
        SYSTEM.DPSIFR0.BYTE = 0x00U;
    }

    /* Clear DPSIFR1 */
    if (0x00U != SYSTEM.DPSIFR1.BYTE)
    {
        SYSTEM.DPSIFR1.BYTE = 0x00U;
    }

    /* Clear DPSIFR2 */
    if (0x00U != SYSTEM.DPSIFR2.BYTE)
    {
        SYSTEM.DPSIFR2.BYTE = 0x00U;
    }

    /* Prevent out-of-order execution */
    while (0x00U != SYSTEM.DPSIFR2.BYTE)
    {
        nop();
    }

    /* Initiate the low-power mode */
    wait();

    /* Restore the state of DMAC and DTC if necessary */
    if (0U == dummy_mstpa28)
    {
        SYSTEM.MSTPCRA.BIT.MSTPA28 = 0U;
        DMAC.DMAST.BIT.DMST = dummy_dmac;
        DTC.DTCST.BIT.DTCST = dummy_dtcst;
    }

    /* Restore the previous state of the protect register */
    SYSTEM.PRCR.WORD = (uint16_t)(0xA500U | protect_dummy);

    return status;
}

/***********************************************************************************************************************
* Function Name: R_Config_LPC_ChangeSleepModeReturnClock
* Description  : This function allows sleep mode return clock to be changed
* Arguments    : clock -
*                    return clock operation mode
* Return Value : status -
*                    MD_OK, MD_ARGERROR or MD_ERROR1
***********************************************************************************************************************/

MD_STATUS R_Config_LPC_ChangeSleepModeReturnClock(return_clock_t clock)
{
    MD_STATUS status = MD_OK;
    return_clock_t old_clock;

    uint16_t protect_dummy = (uint16_t)(SYSTEM.PRCR.WORD & 0x000BU);

    /* Get the current return clock */
    if (0U == SYSTEM.RSTCKCR.BIT.RSTCKEN)
    {
        old_clock = RETURN_DISABLE;
    }
    else
    {
        if (1U == SYSTEM.RSTCKCR.BIT.RSTCKSEL)
        {
            old_clock = RETURN_HOCO;
        }
        else if (2U == SYSTEM.RSTCKCR.BIT.RSTCKSEL)
        {
            old_clock = RETURN_MAIN_CLOCK;
        }
        else
        {
            return MD_ERROR1;
        }
    }

    /* Disable protect bit */
    SYSTEM.PRCR.WORD = 0xA502U | protect_dummy;

    if (clock != old_clock)
    {
        /* Set the desired sleep mode return clock source */
        switch (clock)
        {
            case RETURN_DISABLE:

                SYSTEM.RSTCKCR.BYTE = _00_LPC_RETURN_DISABLED;

                break;

            case RETURN_HOCO:

                SYSTEM.RSTCKCR.BYTE = _81_LPC_RETURN_CLK_HOCO;

                break;

            case RETURN_MAIN_CLOCK:

                SYSTEM.RSTCKCR.BYTE = _82_LPC_RETURN_CLK_MAIN_OSC;

                break;

            default:

                status = MD_ARGERROR;

                break;
        }
    }

    /* Restore the previous state of the protect register */
    SYSTEM.PRCR.WORD = (uint16_t)(0xA500U | protect_dummy);

    return status;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
