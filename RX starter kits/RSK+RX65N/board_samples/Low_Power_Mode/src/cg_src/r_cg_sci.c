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
* Version      : Applilet4 for RX65N V1.00.00.08 [25 Aug 2016]
* Device(s)    : R5F565N9AxFB
* Tool-Chain   : IAR Systems ICCRX
* Description  : This file implements device driver for SCI module.
* Creation Date: 2016-10-11
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
uint8_t *gp_sci7_tx_address;               /* SCI7 transmit buffer address */
uint16_t g_sci7_tx_count;                  /* SCI7 transmit data number */
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_SCI7_Create
* Description  : This function initializes SCI7.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_SCI7_Create(void)
{
    /* Cancel SCI7 module stop state */
    MSTP(SCI7) = 0U;

    /* Set interrupt priority */
    IPR(SCI7,TXI7) = _0F_SCI_PRIORITY_LEVEL15;

    /* Clear the control register */
    SCI7.SCR.BYTE = 0x00U;

    /* Set clock enable */
    SCI7.SCR.BYTE |= _01_SCI_INTERNAL_SCK_OUTPUT;

    /* Clear the SIMR1.IICM */
    SCI7.SIMR1.BIT.IICM = 0U;

    /* Set control registers */
    SCI7.SPMR.BYTE = _00_SCI_SS_PIN_DISABLE | _00_SCI_SPI_MASTER | _00_SCI_CLOCK_NOT_INVERTED | 
                     _00_SCI_CLOCK_NOT_DELAYED;
    SCI7.SMR.BYTE = _80_SCI_CLOCK_SYNCHRONOUS_OR_SPI_MODE | _00_SCI_CLOCK_PCLK;
    SCI7.SCMR.BYTE = _00_SCI_SERIAL_MODE | _00_SCI_DATA_INVERT_NONE | _08_SCI_DATA_MSB_FIRST | _62_SCI_SCMR_DEFAULT;
    SCI7.SEMR.BYTE = _00_SCI_BIT_MODULATION_DISABLE;

    /* Set bit rate */
    SCI7.BRR = 0x00U;

    /* Set SMOSI7 pin */
    MPC.P90PFS.BYTE = 0x0AU;
    PORT9.PMR.BYTE |= 0x01U;

    /* Set SCK7 pin */
    MPC.P91PFS.BYTE = 0x0AU;
    PORT9.PMR.BYTE |= 0x02U;
}
/***********************************************************************************************************************
* Function Name: R_SCI7_Start
* Description  : This function starts SCI7.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_SCI7_Start(void)
{
    /* Enable TXI and TEI interrupt */
    IR(SCI7,TXI7) = 0U;
    IEN(SCI7,TXI7) = 1U;
    ICU.GENBL0.BIT.EN14 = 1U;
}
/***********************************************************************************************************************
* Function Name: R_SCI7_Stop
* Description  : This function stops SCI7.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_SCI7_Stop(void)
{
    /* Set SMOSI7 pin */
    PORT9.PMR.BYTE &= 0xFEU;

    /* Disable serial transmit and receive */
    SCI7.SCR.BYTE &= 0xCFU;

    /* Disable TXI and TEI interrupt */
    IEN(SCI7,TXI7) = 0U;
    ICU.GENBL0.BIT.EN14 = 0U;

    /* Clear interrupt flags */
    IR(SCI7,TXI7) = 0U;
    IR(SCI7,RXI7) = 0U;
}
/***********************************************************************************************************************
* Function Name: R_SCI7_SPI_Master_Send
* Description  : This function sends SPI7 data to slave device.
* Arguments    : tx_buf -
*                    transfer buffer pointer (Not used when transmit data handled by DTC or DMAC)
*                tx_num -
*                    buffer size (Not used when receive data handled by DTC or DMAC)
* Return Value : status -
*                    MD_OK or MD_ARGERROR
***********************************************************************************************************************/
MD_STATUS R_SCI7_SPI_Master_Send(uint8_t * const tx_buf, uint16_t tx_num)
{
    MD_STATUS status = MD_OK;

    if (1U > tx_num)
    {
        status = MD_ARGERROR;
    }
    else
    {
        gp_sci7_tx_address = tx_buf;
        g_sci7_tx_count = tx_num;

        /* Set SMOSI7 pin */
        PORT9.PMR.BYTE |= 0x01U;

        SCI7.SCR.BIT.TIE = 1U;
        SCI7.SCR.BIT.TE = 1U;
    }

    return (status);
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
