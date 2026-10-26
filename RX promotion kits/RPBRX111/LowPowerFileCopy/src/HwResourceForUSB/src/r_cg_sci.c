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
* File Name    : r_cg_sci.c
* Version      : Applilet4 for RX111 V1.00.00.05 [06 Jun 2013]
* Device(s)    : R5F51115AxFM
* Tool-Chain   : CCRX
* Description  : This file implements device driver for SCI module.
* Creation Date: 10/06/2013
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
#include "r_cg_sci.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
uint8_t * gp_sci1_tx_address;         /* SCI1 transmit buffer address */
uint16_t  g_sci1_tx_count;            /* SCI1 transmit data number */
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_SCI1_Create
* Description  : This function initializes the SCI1.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_SCI1_Create(void)
{
    /* Enable writing to MPC pin function control registers */
    MPC.PWPR.BIT.B0WI = 0U;
    MPC.PWPR.BIT.PFSWE = 1U;
    /* Cancel SCI1 module stop state */
    SYSTEM.MSTPCRB.BIT.MSTPB30 = 0U;

    /* Set interrupt priority */
    IPR(SCI1, ERI1) = _09_SCI_PRIORITY_LEVEL9;

    /* Clear the SCR.TIE, RIE, TE, RE and TEIE bits */
    SCI1.SCR.BIT.TIE = 0U;
    SCI1.SCR.BIT.RIE = 0U;
    SCI1.SCR.BIT.TE = 0U;
    SCI1.SCR.BIT.RE = 0U;

    /* Set SMISO1 pin */
    MPC.P15PFS.BYTE = 0x0AU;
    PORT1.PMR.BYTE |= 0x20U;

    /* Set SMOSI1 pin */
    MPC.PC7PFS.BYTE = 0x0BU;
    PORTC.PMR.BYTE |= 0x80U;

    /* Set SCK1 pin */
    MPC.PC5PFS.BYTE = 0x0BU;
    PORTC.PMR.BYTE |= 0x20U;

    /* Set the clock selection */
    SCI1.SCR.BYTE |= _01_SCI_INTERNAL_SCK_OUTPUT;

    /* Clear the SIMR1.IICM */
    SCI1.SIMR1.BIT.IICM = 0U;

    /* Set SPMR */
    SCI1.SPMR.BYTE |= _00_SCI_SS_PIN_DISABLE | _00_SCI_SPI_MASTER | _00_SCI_CLOCK_NOT_INVERTED |
                      _00_SCI_CLOCK_NOT_DELAYED;

    /* Set SMR, SCMR, SEMR */
    SCI1.SMR.BYTE |= _00_SCI_DATA_LENGTH_8 | _80_SCI_CLOCK_SYNCHRONOUS_MODE;
    SCI1.SCMR.BYTE |= _00_SCI_SERIAL_MODE | _00_SCI_DATA_INVERT_NONE | _08_SCI_DATA_MSB_FIRST;

    /* Set BRR */
    SCI1.BRR = 0x00U;

    /* Disable writing to MPC pin function control registers */
    MPC.PWPR.BIT.PFSWE = 0U;
    MPC.PWPR.BIT.B0WI = 1U;
}
/***********************************************************************************************************************
* Function Name: R_SCI1_Start
* Description  : This function starts the SCI1.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_SCI1_Start(void)
{
    /* Enable TXI and TEI interrupt */
    IEN(SCI1,TXI1) = 1U;
    IEN(SCI1,TEI1) = 1U;

    /* Clear interrupt flags */
    IR(SCI1,TXI1) = 0U;
    IR(SCI1,TEI1) = 0U;
    IR(SCI1,RXI1) = 0U;
    IR(SCI1,ERI1) = 0U;
}
/***********************************************************************************************************************
* Function Name: R_SCI1_Stop
* Description  : This function stops the SCI1.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_SCI1_Stop(void)
{
    /* Set SMOSI1 pin */
    MPC.PC7PFS.BYTE = 0x00U;
    PORTC.PMR.BYTE &= 0x7FU;
    /* Set SMISO1 pin */
    MPC.PC6PFS.BYTE = 0x00U;
    PORTC.PMR.BYTE &= 0xBFU;
    /* Disable TXI and TEI interrupt */
    IEN(SCI1,TXI1) = 0U;
    IEN(SCI1,TEI1) = 0U;

    /* Clear interrupt flags */
    IR(SCI1,TXI1) = 0U;
    IR(SCI1,TEI1) = 0U;
    IR(SCI1,RXI1) = 0U;
    IR(SCI1,ERI1) = 0U;
}
/***********************************************************************************************************************
* Function Name: R_SCI1_SPI_Master_Send
* Description  : This function sends SPI1 data to slave device.
* Arguments    : tx_buf -
*                    transfer buffer pointer (Not used when transmit data handled by DTC)
*                tx_num -
*                    buffer size
* Return Value : status -
*                    MD_OK or MD_ARGERROR
***********************************************************************************************************************/
MD_STATUS R_SCI1_SPI_Master_Send(uint8_t * const tx_buf, uint16_t tx_num)
{
    MD_STATUS status = MD_OK;

    if (tx_num < 1U)
    {
        status = MD_ARGERROR;
    }
    else
    {
        gp_sci1_tx_address = tx_buf;
        g_sci1_tx_count = tx_num;
        /* Set SMOSI1 pin */
        MPC.PC7PFS.BYTE = 0x0BU;
        PORTC.PMR.BYTE |= 0x80U;

        SCI1.SCR.BIT.TIE = 1U;
        SCI1.SCR.BIT.TE = 1U;
    }

    return (status);
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
