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
* File Name    : r_cg_sci.c
* Version      : Applilet4 for RX130 V1.00.01.02 [07 Mar 2016]
* Device(s)    : R5F51305AxFN
* Tool-Chain   : IAR Systems ICCRX
* Description  : This file implements device driver for SCI module.
* Creation Date: 2016-09-23
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
uint8_t * gp_sci6_tx_address;               /* SCI6 transmit buffer address */
uint16_t  g_sci6_tx_count;                  /* SCI6 transmit data number */
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_SCI6_Create
* Description  : This function initializes SCI6.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_SCI6_Create(void)
{
    /* Cancel SCI6 module stop state */
    MSTP(SCI6) = 0U;

    /* Set interrupt priority */
    IPR(SCI6, ERI6) = _0F_SCI_PRIORITY_LEVEL15;

    /* Clear the control register */
    SCI6.SCR.BYTE = 0x00U;

    /* Set clock enable */
    SCI6.SCR.BYTE |= _01_SCI_INTERNAL_SCK_OUTPUT;

    /* Clear the SIMR1.IICM */
    SCI6.SIMR1.BIT.IICM = 0U;

    /* Set control registers */
    SCI6.SPMR.BYTE = _00_SCI_SS_PIN_DISABLE | _00_SCI_SPI_MASTER | _00_SCI_CLOCK_NOT_INVERTED | 
                     _00_SCI_CLOCK_NOT_DELAYED;
    SCI6.SMR.BYTE = _80_SCI_CLOCK_SYNCHRONOUS_MODE | _00_SCI_CLOCK_PCLK;
    SCI6.SCMR.BYTE = _00_SCI_SERIAL_MODE | _00_SCI_DATA_INVERT_NONE | _08_SCI_DATA_MSB_FIRST | _62_SCI_SCMR_DEFAULT;
    SCI6.SEMR.BYTE = _00_SCI_BIT_MODULATION_DISABLE;

    /* Set bitrate */
    SCI6.BRR = 0x00U;

    /* Set SMOSI6 pin */
    MPC.PB1PFS.BYTE = 0x0BU;
    PORTB.PMR.BYTE |= 0x02U;

    /* Set SCK6 pin */
    MPC.PB3PFS.BYTE = 0x0BU;
    PORTB.PMR.BYTE |= 0x08U;
}
/***********************************************************************************************************************
* Function Name: R_SCI6_Start
* Description  : This function starts SCI6.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_SCI6_Start(void)
{
    /* Enable TXI and TEI interrupt */
    IR(SCI6, TXI6) = 0U;
    IEN(SCI6, TXI6) = 1U;
    IEN(SCI6, TEI6) = 1U;

}
/***********************************************************************************************************************
* Function Name: R_SCI6_Stop
* Description  : This function stops SCI6.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_SCI6_Stop(void)
{
    /* Set SMOSI6 pin */
    PORTB.PMR.BYTE &= 0xFDU;

    SCI6.SCR.BYTE &= 0xCFU;      /* Disable serial transmit and receive */

    /* Disable TXI and TEI interrupt */
    IEN(SCI6, TXI6) = 0U;
    IEN(SCI6, TEI6) = 0U;

    /* Clear interrupt flags */
    IR(SCI6, TXI6) = 0U;
    IR(SCI6, RXI6) = 0U;
}
/***********************************************************************************************************************
* Function Name: R_SCI6_SPI_Master_Send
* Description  : This function sends SPI6 data to slave device.
* Arguments    : tx_buf -
*                    transfer buffer pointer (Not used when transmit data handled by DTC)
*                tx_num -
*                    buffer size (Not used when receive data handled by DTC)
* Return Value : status -
*                    MD_OK or MD_ARGERROR
***********************************************************************************************************************/
MD_STATUS R_SCI6_SPI_Master_Send(uint8_t * const tx_buf, uint16_t tx_num)
{
    MD_STATUS status = MD_OK;

    if (1U > tx_num)
    {
        status = MD_ARGERROR;
    }
    else
    {
        gp_sci6_tx_address = tx_buf;
        g_sci6_tx_count = tx_num;

        /* Set SMOSI6 pin */
        PORTB.PMR.BYTE |= 0x02U;
        SCI6.SCR.BIT.TIE = 1U;
        SCI6.SCR.BIT.TE = 1U;
    }

    return (status);
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
