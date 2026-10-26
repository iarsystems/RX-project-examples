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
* Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_sci.c
* Version      : Applilet4 for RX113 V1.01.00.02 [31 Jul 2014]
* Device(s)    : R5F51138AxFP
* Tool-Chain   : IAR Systems ICCRX
* Description  : This file implements device driver for SCI module.
* Creation Date: 2015-01-27
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
uint8_t * gp_sci12_tx_address;         /* SCI12 transmit buffer address */
uint16_t  g_sci12_tx_count;            /* SCI12 transmit data number */
uint8_t * gp_sci12_rx_address;         /* SCI12 receive buffer address */
uint16_t  g_sci12_rx_count;            /* SCI12 receive data number */
uint16_t  g_sci12_rx_length;           /* SCI12 receive data length */
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_SCI12_Create
* Description  : This function initializes the SCI12.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_SCI12_Create(void)
{
    /* Cancel SCI12 module stop state */
    MSTP(SCI12) = 0U;

    /* Set interrupt priority */
    IPR(SCI12, ERI12) = _0F_SCI_PRIORITY_LEVEL15;

    /* Clear the SCR.TIE, RIE, TE, RE and TEIE bits */
    SCI12.SCR.BIT.TIE = 0U;
    SCI12.SCR.BIT.RIE = 0U;
    SCI12.SCR.BIT.TE = 0U;
    SCI12.SCR.BIT.RE = 0U;
    SCI12.SCR.BIT.TEIE = 0U;

    /* Set RXD12 pin */
    MPC.P17PFS.BYTE = 0x0CU;
    PORT1.PMR.BYTE |= 0x80U;
    /* Set TXD12 pin */
    MPC.P14PFS.BYTE = 0x0CU;
    PORT1.PODR.BYTE |= 0x10U;
    PORT1.PDR.BYTE |= 0x10U;
    PORT1.PMR.BYTE |= 0x10U;

    /* Set clock enable */
    SCI12.SCR.BYTE = _00_SCI_INTERNAL_SCK_UNUSED;

    /* Clear the SIMR1.IICM, SPMR.CKPH, and CKPOL bit */
    SCI12.SIMR1.BIT.IICM = 0U;
    SCI12.SPMR.BYTE = _00_SCI_RTS | _00_SCI_CLOCK_NOT_INVERTED | _00_SCI_CLOCK_NOT_DELAYED;

    /* Set control registers */
    SCI12.SMR.BYTE = _01_SCI_CLOCK_PCLK_4 | _00_SCI_STOP_1 | _00_SCI_PARITY_EVEN | _00_SCI_PARITY_DISABLE | 
                     _00_SCI_DATA_LENGTH_8 | _00_SCI_MULTI_PROCESSOR_DISABLE | _00_SCI_ASYNCHRONOUS_MODE;
    SCI12.SCMR.BYTE = _00_SCI_SERIAL_MODE | _00_SCI_DATA_INVERT_NONE | _00_SCI_DATA_LSB_FIRST | _72_SCI_SCMR_DEFAULT;

    /* Set SEMR, SNFR */
    SCI12.SEMR.BYTE = _00_SCI_LOW_LEVEL_START_BIT | _00_SCI_NOISE_FILTER_DISABLE | _10_SCI_8_BASE_CLOCK;

    /* Set bitrate */
    SCI12.BRR = 0x19U;
}
/***********************************************************************************************************************
* Function Name: R_SCI12_Start
* Description  : This function starts the SCI12.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_SCI12_Start(void)
{
    IR(SCI12,TXI12) = 0U;
    IR(SCI12,TEI12) = 0U;
    IR(SCI12,RXI12) = 0U;
    IR(SCI12,ERI12) = 0U;
    IEN(SCI12,TXI12) = 1U;
    IEN(SCI12,TEI12) = 1U;
    IEN(SCI12,RXI12) = 1U;
    IEN(SCI12,ERI12) = 1U;
}
/***********************************************************************************************************************
* Function Name: R_SCI12_Stop
* Description  : This function stops the SCI12.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_SCI12_Stop(void)
{
    /* Set TXD12 pin */
    PORT1.PMR.BYTE &= 0xEFU;
    SCI12.SCR.BIT.TE = 0U;      /* Disable serial transmit */
    SCI12.SCR.BIT.RE = 0U;      /* Disable serial receive */
    SCI12.SCR.BIT.TIE = 0U;     /* Disable TXI interrupt */
    SCI12.SCR.BIT.RIE = 0U;     /* Disable RXI and ERI interrupt */
    IR(SCI12,TXI12) = 0U;
    IR(SCI12,TEI12) = 0U;
    IR(SCI12,RXI12) = 0U;
    IR(SCI12,ERI12) = 0U;
}
/***********************************************************************************************************************
* Function Name: R_SCI12_Serial_Receive
* Description  : This function receives SCI12 data.
* Arguments    : rx_buf -
*                    receive buffer pointer (Not used when receive data handled by DTC)
*                rx_num -
*                    buffer size
* Return Value : status -
*                    MD_OK or MD_ARGERROR
***********************************************************************************************************************/
MD_STATUS R_SCI12_Serial_Receive(uint8_t * const rx_buf, uint16_t rx_num)
{
    MD_STATUS status = MD_OK;

    if (rx_num < 1U)
    {
        status = MD_ARGERROR;
    }
    else
    {
        g_sci12_rx_count = 0U;
        g_sci12_rx_length = rx_num;
        gp_sci12_rx_address = rx_buf;
        SCI12.SCR.BIT.RIE = 1U;
        SCI12.SCR.BIT.RE = 1U;
    }

    return (status);
}
/***********************************************************************************************************************
* Function Name: R_SCI12_Serial_Send
* Description  : This function transmits SCI12 data.
* Arguments    : tx_buf -
*                    transfer buffer pointer (Not used when transmit data handled by DTC)
*                tx_num -
*                    buffer size
* Return Value : status -
*                    MD_OK or MD_ARGERROR
***********************************************************************************************************************/
MD_STATUS R_SCI12_Serial_Send(uint8_t * const tx_buf, uint16_t tx_num)
{
    MD_STATUS status = MD_OK;

    if (tx_num < 1U)
    {
        status = MD_ARGERROR;
    }
    else
    {
        gp_sci12_tx_address = tx_buf;
        g_sci12_tx_count = tx_num;
        /* Set TXD12 pin */
        PORT1.PMR.BYTE |= 0x10U;
        SCI12.SCR.BIT.TIE = 1U;
        SCI12.SCR.BIT.TE = 1U;
    }

    return (status);
}

/* Start user code for adding. Do not edit comment generated here */

/*******************************************************************************
 * Function Name : R_SCI12_LIN_Slave_Mode
 * Description   : Configures the SCI12 UART for extended serial mode (LIN Slave)
 * Argument      : none
 * Return value  : none
 *******************************************************************************/
void R_SCI12_LIN_Slave_Mode (void)
{
    /* See the flowchart of Figure 26.59 of Hardware Manual */

    /* Enable the extended serial mode control section */
    SCI12.ESMER.BIT.ESME = 0x1;

    /* Disable features for Start Frame Reception - See HW Manual Sec. 26.9.2 */
    /* Control Field 0 Reception Disable */
    SCI12.CR1.BIT.CF0RE = 0x0;
    /* Control Field 1 (LIN SYNC) Data Register : value in PCF1DR */
    SCI12.CR1.BIT.CF1DS = 0x0;
    /* Priority Interrupt Bit Disable. */
    SCI12.CR1.BIT.PIBE = 0x0;

    /* Disable digital filter for SCI12 */
    SCI12.SEMR.BIT.NFEN = 0x0;
    SCI12.CR2.BIT.DFCS = 0x0;

    /* Use SCI base clock for bus collision detection */
    SCI12.CR2.BIT.BCCS = 0x0;

    /* Select 16 SCI base clock cycles for 1-bit period */
    SCI12.SEMR.BIT.ABCS = 0x0;

    /* Rising edge of the 8th cycle of SCI base clock is used to sample the
     received serial signal i.e. middle of the bit period */
    SCI12.CR2.BIT.RTS = 0x0;

    /* Use normal (non-inverted) polarity for RXDX12 and TXDX12 signals,
     non-multiplexed */
    SCI12.PCR.BIT.RXDXPS = 0x0;
    SCI12.PCR.BIT.TXDXPS = 0x0;
    SCI12.PCR.BIT.SHARPS = 0x0;

    /* Use PCLK as the Timer Count Clock Source = 31.25 ns period */
    SCI12.TMR.BIT.TCSS = 0x0;

}
/*******************************************************************************
 End of function R_SCI12_LIN_Slave_Mode
 *******************************************************************************/

/* End user code. Do not edit comment generated here */
