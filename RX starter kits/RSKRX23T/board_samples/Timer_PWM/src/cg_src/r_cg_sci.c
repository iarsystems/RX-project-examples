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
* Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_sci.c
* Version      : Applilet4 for RX23T V1.00.01.01 [30 Oct 2015]
* Device(s)    : R5F523T5AxFM
* Tool-Chain   : IAR Systems ICCRX
* Description  : This file implements device driver for SCI module.
* Creation Date: 2016-02-15
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
uint8_t * gp_sci5_tx_address;               /* SCI5 transmit buffer address */
uint16_t  g_sci5_tx_count;                  /* SCI5 transmit data number */
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_SCI5_Create
* Description  : This function initializes SCI5.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_SCI5_Create(void)
{
    /* Cancel SCI5 module stop state */
    MSTP(SCI5) = 0U;

    /* Set interrupt priority */
    IPR(SCI5, ERI5) = _0F_SCI_PRIORITY_LEVEL15;

    /* Clear the control register */
    SCI5.SCR.BYTE = 0x00U;

    /* Set clock enable */
    SCI5.SCR.BYTE |= _01_SCI_INTERNAL_SCK_OUTPUT;

    /* Clear the SIMR1.IICM */
    SCI5.SIMR1.BIT.IICM = 0U;

    /* Set control registers */
    SCI5.SPMR.BYTE = _00_SCI_SS_PIN_DISABLE | _00_SCI_SPI_MASTER | _00_SCI_CLOCK_NOT_INVERTED | 
                     _00_SCI_CLOCK_NOT_DELAYED;
    SCI5.SMR.BYTE = _00_SCI_DATA_LENGTH_8 | _80_SCI_CLOCK_SYNCHRONOUS_MODE | _00_SCI_CLOCK_PCLK;
    SCI5.SCMR.BYTE = _00_SCI_SERIAL_MODE | _00_SCI_DATA_INVERT_NONE | _08_SCI_DATA_MSB_FIRST | 
                     _10_SCI_DATA_LENGTH_8_OR_7 | _62_SCI_SCMR_DEFAULT;
    SCI5.SEMR.BYTE = _00_SCI_BIT_MODULATION_DISABLE;

    /* Set bitrate */
    SCI5.BRR = 0x00U;

    /* Set SMOSI5 pin */
    MPC.PB5PFS.BYTE = 0x0AU;
    PORTB.PMR.BYTE |= 0x20U;

    /* Set SCK5 pin */
    MPC.PB7PFS.BYTE = 0x0AU;
    PORTB.PMR.BYTE |= 0x80U;
}
/***********************************************************************************************************************
* Function Name: R_SCI5_Start
* Description  : This function starts SCI5.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_SCI5_Start(void)
{
    /* Enable TXI and TEI interrupt */
    IR(SCI5, TXI5) = 0U;
    IEN(SCI5, TXI5) = 1U;
    IEN(SCI5, TEI5) = 1U;

}
/***********************************************************************************************************************
* Function Name: R_SCI5_Stop
* Description  : This function stops SCI5.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_SCI5_Stop(void)
{
    /* Set SMOSI5 pin */
    PORTB.PMR.BYTE &= 0xDFU;
    SCI5.SCR.BYTE &= 0xCF;    /* Disable serial transmit and receive */

    /* Disable TXI and TEI interrupt */
    IEN(SCI5, TXI5) = 0U;
    IEN(SCI5, TEI5) = 0U;

    /* Clear interrupt flags */
    IR(SCI5, TXI5) = 0U;
    IR(SCI5, RXI5) = 0U;
}
/***********************************************************************************************************************
* Function Name: R_SCI5_SPI_Master_Send
* Description  : This function sends SPI5 data to slave device.
* Arguments    : tx_buf -
*                    transfer buffer pointer (Not used when transmit data handled by DTC or DMAC)
*                tx_num -
*                    buffer size (Not used when receive data handled by DTC or DMAC)
* Return Value : status -
*                    MD_OK or MD_ARGERROR
***********************************************************************************************************************/
MD_STATUS R_SCI5_SPI_Master_Send(uint8_t * const tx_buf, uint16_t tx_num)
{
    MD_STATUS status = MD_OK;

    if (1U > tx_num)
    {
        status = MD_ARGERROR;
    }
    else
    {
        gp_sci5_tx_address = tx_buf;
        g_sci5_tx_count = tx_num;

        /* Set SMOSI5 pin */
        PORTB.PMR.BYTE |= 0x20U;
        SCI5.SCR.BIT.TIE = 1U;
        SCI5.SCR.BIT.TE = 1U;
    }

    return (status);
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
