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
* Copyright (C) 2015, 2016 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_lpc.c
* Version      : Applilet4 for RX24T V1.00.01.02 [07 Mar 2016]
* Device(s)    : R5F524TAAxFP
* Tool-Chain   : IAR Systems ICCRX
* Description  : This file implements device driver for LPC module.
* Creation Date: 2016-07-08
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
#include "r_cg_lpc.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_LPC_Create
* Description  : This function initializes the LPC module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_LPC_Create(void)
{
    /* Set operating power control */
    SYSTEM.OPCCR.BIT.OPCM = _00_LPC_HIGH_SPEED_MODE;

    while (1U == SYSTEM.OPCCR.BIT.OPCMTSF);
}
/***********************************************************************************************************************
* Function Name: R_LPC_Sleep
* Description  : This function enables sleep mode.
* Arguments    : None
* Return Value : status -
*                    MD_OK
***********************************************************************************************************************/
MD_STATUS R_LPC_Sleep(void)
{
    MD_STATUS status = MD_OK;
    uint16_t protect_dummy = (uint16_t)(SYSTEM.PRCR.WORD & 0x000BU);
    
    /* Disable protect bit */
    SYSTEM.PRCR.WORD = 0xA503U;

    /* Select sleep mode */
    SYSTEM.SBYCR.BIT.SSBY = 0U;

    /* Prevent out-of-order execution */
    while (0U != SYSTEM.SBYCR.BIT.SSBY);

    /* Initiate the low-power mode */
    wait();

    /* Restore the previous state of the protect register */
    SYSTEM.PRCR.WORD = (uint16_t)(0xA500U | protect_dummy);
    
    return status;
}

/***********************************************************************************************************************
* Function Name: R_LPC_DeepSleep
* Description  : This function enables deep sleep mode.
* Arguments    : None
* Return Value : status -
*                    MD_OK
***********************************************************************************************************************/
MD_STATUS R_LPC_DeepSleep(void)
{
    MD_STATUS status = MD_OK;
    uint16_t protect_dummy = (uint16_t)(SYSTEM.PRCR.WORD & 0x000BU);
    
    /* Save the current state of DTC */
    uint8_t dummy_mstpa28 = (uint8_t)SYSTEM.MSTPCRA.BIT.MSTPA28;
    uint8_t dummy_dtcst = DTC.DTCST.BIT.DTCST;

    /* Disable protect bit */
    SYSTEM.PRCR.WORD = 0xA503U;
    
    /* Set DTC module to stop state */
    DTC.DTCST.BIT.DTCST = 0U;
    SYSTEM.MSTPCRA.BIT.MSTPA28 = 1UL;

    /* Enable deep sleep mode */
    SYSTEM.MSTPCRC.BIT.DSLPE = 1UL;

    /* Select deep sleep mode */
    SYSTEM.SBYCR.BIT.SSBY = 0U;

    /* Prevent out-of-order execution */
    while (0U != SYSTEM.SBYCR.BIT.SSBY);

    /* Initiate the low-power mode */
    wait();

    /* Restore the state of DTC if necessary */
    if (0U == dummy_mstpa28)
    {
        SYSTEM.MSTPCRA.BIT.MSTPA28 = 0UL;
        DTC.DTCST.BIT.DTCST = dummy_dtcst;
    }
    
    /* Restore the previous state of the protect register */
    SYSTEM.PRCR.WORD = (uint16_t)(0xA500U | protect_dummy);

    return status;
}
/***********************************************************************************************************************
* Function Name: R_LPC_SoftwareStandby
* Description  : This function enables software standby mode.
* Arguments    : None
* Return Value : status -
*                    MD_OK or MD_ERROR1
***********************************************************************************************************************/
MD_STATUS R_LPC_SoftwareStandby(void)
{
    MD_STATUS status = MD_OK;
    uint16_t protect_dummy = (uint16_t)(SYSTEM.PRCR.WORD & 0x000BU);
    uint8_t dummy_dtcst = DTC.DTCST.BIT.DTCST;

    /* Disable protect bit */
    SYSTEM.PRCR.WORD = 0xA503U;

    /* Set DTC module to stop state */
    DTC.DTCST.BIT.DTCST = 0U;

    /* When oscillation stop detection function is enabled, SSBY bit is invalid */
    if (1U == SYSTEM.OSTDCR.BIT.OSTDE)
    {
        status = MD_ERROR1;
        return status;
    }
    else
    {
        /* Select standby mode */
        SYSTEM.SBYCR.BIT.SSBY = 1U;

        /* Prevent out-of-order execution */
        while (1U != SYSTEM.SBYCR.BIT.SSBY);

        /* Initiate the low-power mode */
        wait();
    }

    /* Restore the state of DTC if necessary */
    DTC.DTCST.BIT.DTCST = dummy_dtcst;

    /* Restore the previous state of the protect register */
    SYSTEM.PRCR.WORD = (uint16_t)(0xA500U | protect_dummy);
    
    return status;
}
/***********************************************************************************************************************
* Function Name: R_LPC_ChangeOperatingPowerControl
* Description  : This function allows operating power control to be changed.
* Arguments    : mode -
*                    clock operation mode
* Return Value : status -
*                    MD_OK, MD_ERROR1 or MD_ARGERROR
***********************************************************************************************************************/
MD_STATUS R_LPC_ChangeOperatingPowerControl(operating_mode_t mode)
{
    MD_STATUS status = MD_OK;
    operating_mode_t old_mode;

    uint16_t protect_dummy = (uint16_t)(SYSTEM.PRCR.WORD & 0x000BU);
    
    /* Get the current mode */
    if (0U == SYSTEM.OPCCR.BIT.OPCM)
    {
        old_mode = HIGH_SPEED;
    }
    else
    {
        old_mode = MIDDLE_SPEED;
    }

    /* Disable protect bit */
    SYSTEM.PRCR.WORD = 0xA503U;
    
    if (mode != old_mode)
    {
        /* Set the desired operating power control mode */
        switch (mode)
        {
            case MIDDLE_SPEED:
            case HIGH_SPEED:

                /* OPCCR should not be set when OPCMTSF bit is 1 */
                if (0U == SYSTEM.OPCCR.BIT.OPCMTSF)
                {
                    if (MIDDLE_SPEED == mode)
                    {
                        /* Set middle-speed operating mode */
                        SYSTEM.OPCCR.BIT.OPCM = 2U;
                    }
                    else
                    {
                        /* Set high-speed operating mode */
                        SYSTEM.OPCCR.BIT.OPCM = 0U;
                    }

                    while (0U != SYSTEM.OPCCR.BIT.OPCMTSF);
                }
                else
                {
                    status = MD_ERROR1;
                }

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
