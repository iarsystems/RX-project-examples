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
* File Name    : r_cg_sci.c
* Version      : Applilet4 for RX24U V1.03.01.04 [02 Nov 2017]
* Device(s)    : R5F524UEAxFB
* Tool-Chain   : IAR Systems ICCRX
* Description  : This file implements device driver for SCI module.
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
#include "r_cg_sci.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
volatile uint8_t *gp_sci9_tx_address;               /* SCI9 transmit buffer address */
volatile uint16_t g_sci9_tx_count;                  /* SCI9 transmit data number */
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_SCI9_Create
* Description  : This function initializes SCI9.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_SCI9_Create(void)
{
    /* Cancel SCI9 module stop state */
    MSTP(SCI9) = 0U;

    /* Set interrupt priority */
    IPR(SCI9,ERI9) = _0F_SCI_PRIORITY_LEVEL15;

    /* Clear the control register */
    SCI9.SCR.BYTE = 0x00U;

    /* Set clock enable */
    SCI9.SCR.BYTE |= _01_SCI_INTERNAL_SCK_OUTPUT;

    /* Clear the SIMR1.IICM */
    SCI9.SIMR1.BIT.IICM = 0U;

    /* Set control registers */
    SCI9.SPMR.BYTE = _00_SCI_SS_PIN_DISABLE | _00_SCI_SPI_MASTER | _00_SCI_CLOCK_NOT_INVERTED | 
                     _00_SCI_CLOCK_NOT_DELAYED;
    SCI9.SMR.BYTE = _80_SCI_CLOCK_SYNCHRONOUS_MODE | _00_SCI_CLOCK_PCLK;
    SCI9.SCMR.BYTE = _00_SCI_SERIAL_MODE | _00_SCI_DATA_INVERT_NONE | _08_SCI_DATA_MSB_FIRST | _62_SCI_SCMR_DEFAULT;
    SCI9.SEMR.BYTE = _00_SCI_BIT_MODULATION_DISABLE;

    /* Set bitrate */
    SCI9.BRR = 0x00U;

    /* Set SMOSI9 pin */
    MPC.PG1PFS.BYTE = 0x0AU;
    PORTG.PMR.BYTE |= 0x02U;

    /* Set SCK9 pin */
    MPC.PG2PFS.BYTE = 0x0AU;
    PORTG.PMR.BYTE |= 0x04U;
}
/***********************************************************************************************************************
* Function Name: R_SCI9_Start
* Description  : This function starts SCI9.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_SCI9_Start(void)
{
    /* Enable TXI and TEI interrupt */
    IR(SCI9,TXI9) = 0U;
    IEN(SCI9,TXI9) = 1U;
    IEN(SCI9,TEI9) = 1U;
}
/***********************************************************************************************************************
* Function Name: R_SCI9_Stop
* Description  : This function stops SCI9.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_SCI9_Stop(void)
{
    /* Set SMOSI9 pin */
    PORTG.PMR.BYTE &= 0xFDU;

    /* Disable serial transmit and receive */
    SCI9.SCR.BYTE &= 0xCFU;

    /* Disable TXI and TEI interrupt */
    IEN(SCI9,TXI9) = 0U;
    IEN(SCI9,TEI9) = 0U;

    /* Clear interrupt flags */
    IR(SCI9,TXI9) = 0U;
    IR(SCI9,RXI9) = 0U;
}
/***********************************************************************************************************************
* Function Name: R_SCI9_SPI_Master_Send
* Description  : This function sends SPI9 data to slave device.
* Arguments    : tx_buf -
*                    transfer buffer pointer (Not used when transmit data handled by DTC)
*                tx_num -
*                    buffer size (Not used when receive data handled by DTC)
* Return Value : status -
*                    MD_OK or MD_ARGERROR
***********************************************************************************************************************/
MD_STATUS R_SCI9_SPI_Master_Send(
          uint8_t * const tx_buf, 
          uint16_t tx_num)
{
    MD_STATUS status = MD_OK;

    if (1U > tx_num)
    {
        status = MD_ARGERROR;
    }
    else
    {
        gp_sci9_tx_address = tx_buf;
        g_sci9_tx_count = tx_num;

        /* Set SMOSI9 pin */
        PORTG.PMR.BYTE |= 0x02U;

        SCI9.SCR.BIT.TIE = 1U;
        SCI9.SCR.BIT.TE = 1U;
    }

    return (status);
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
