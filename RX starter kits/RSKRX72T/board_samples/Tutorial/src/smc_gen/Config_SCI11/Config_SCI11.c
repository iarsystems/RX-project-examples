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
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : Config_SCI11.c
* Version      : 1.5.0
* Device(s)    : R5F572TKCxFB
* Description  : This file implements device driver for Config_SCI11.
* Creation Date: 2019-05-21
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
#include "Config_SCI11.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
volatile uint8_t * gp_sci11_tx_address;                /* SCI11 transmit buffer address */
volatile uint16_t  g_sci11_tx_count;                   /* SCI11 transmit data number */
volatile uint8_t * gp_sci11_rx_address;                /* SCI11 receive buffer address */
volatile uint16_t  g_sci11_rx_count;                   /* SCI11 receive data number */
volatile uint16_t  g_sci11_rx_length;                  /* SCI11 receive data length */
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_SCI11_Create
* Description  : This function initializes the SCI11 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_SCI11_Create(void)
{
    /* Cancel SCI stop state */
    MSTP(SCI11) = 0U;

    /* Set interrupt priority */
    IPR(SCI11, RXI11) = _0F_SCI_PRIORITY_LEVEL15;
    IPR(SCI11, TXI11) = _0F_SCI_PRIORITY_LEVEL15;

    /* Clear the control register */
    SCI11.SCR.BYTE = 0x00U;

    /* Set clock enable */
    SCI11.SCR.BYTE = _00_SCI_INTERNAL_SCK_UNUSED;

    /* Clear the SIMR1.IICM, SPMR.CKPH, and CKPOL bit, and set SPMR */
    SCI11.SIMR1.BIT.IICM = 0U;
    SCI11.SPMR.BYTE = _00_SCI_RTS | _00_SCI_CLOCK_NOT_INVERTED | _00_SCI_CLOCK_NOT_DELAYED;

    /* Set control registers */
    SCI11.SMR.BYTE = _01_SCI_CLOCK_PCLK_4 | _00_SCI_MULTI_PROCESSOR_DISABLE | _00_SCI_STOP_1 | 
                     _00_SCI_PARITY_DISABLE | _00_SCI_DATA_LENGTH_8 | _00_SCI_ASYNCHRONOUS_OR_I2C_MODE;
    SCI11.SCMR.BYTE = _00_SCI_SERIAL_MODE | _00_SCI_DATA_INVERT_NONE | _00_SCI_DATA_LSB_FIRST | 
                      _10_SCI_DATA_LENGTH_8_OR_7 | _62_SCI_SCMR_DEFAULT;
    SCI11.SEMR.BYTE = _00_SCI_BIT_MODULATION_DISABLE | _08_SCI_6_BASE_CLOCK | _00_SCI_NOISE_FILTER_DISABLE | 
                      _80_SCI_FALLING_EDGE_START_BIT;

    /* Set bit rate */
    SCI11.BRR = 0xD8U;

    /* Set RXD11 pin */
    MPC.PB6PFS.BYTE = 0x0BU;
    PORTB.PMR.BYTE |= 0x40U;

    /* Set TXD11 pin */
    MPC.PB5PFS.BYTE = 0x0BU;
    PORTB.PODR.BYTE |= 0x20U;
    PORTB.PDR.BYTE |= 0x20U;

    R_Config_SCI11_Create_UserInit();
}

/***********************************************************************************************************************
* Function Name: R_Config_SCI11_Start
* Description  : This function starts the SCI11 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_SCI11_Start(void)
{
    /* Clear interrupt flag */
    IR(SCI11, TXI11) = 0U;
    IR(SCI11, RXI11) = 0U;

    /* Enable SCI interrupt */
    IEN(SCI11, TXI11) = 1U;
    ICU.GENAL0.BIT.EN12 = 1U;
    IEN(SCI11, RXI11) = 1U;
    ICU.GENAL0.BIT.EN13 = 1U;
}

/***********************************************************************************************************************
* Function Name: R_Config_SCI11_Stop
* Description  : This function stop the SCI11 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_SCI11_Stop(void)
{
    /* Set TXD11 pin */
    PORTB.PMR.BYTE &= 0xDFU;

    /* Disable serial transmit */
    SCI11.SCR.BIT.TE = 0U;

    /* Disable serial receive */
    SCI11.SCR.BIT.RE = 0U;

    /* Disable SCI interrupt */
    SCI11.SCR.BIT.TIE = 0U;
    SCI11.SCR.BIT.RIE = 0U;
    IEN(SCI11, TXI11) = 0U;
    ICU.GENAL0.BIT.EN12 = 0U;
    IR(SCI11, TXI11) = 0U;
    IEN(SCI11, RXI11) = 0U;
    ICU.GENAL0.BIT.EN13 = 0U;
    IR(SCI11, RXI11) = 0U;
}

/***********************************************************************************************************************
* Function Name: R_Config_SCI11_Serial_Receive
* Description  : This function receive SCI11data
* Arguments    : rx_buf -
*                    receive buffer pointer (Not used when receive data handled by DMAC/DTC)
*                rx_num -
*                    buffer size (Not used when receive data handled by DMAC/DTC)
* Return Value : status -
*                    MD_OK or MD_ARGERROR
***********************************************************************************************************************/

MD_STATUS R_Config_SCI11_Serial_Receive(uint8_t * const rx_buf, uint16_t rx_num)
{
    MD_STATUS status = MD_OK;

    if (1U > rx_num)
    {
        status = MD_ARGERROR;
    }
    else
    {
        g_sci11_rx_count = 0U;
        g_sci11_rx_length = rx_num;
        gp_sci11_rx_address = rx_buf;
        SCI11.SCR.BIT.RIE = 1U;
        SCI11.SCR.BIT.RE = 1U;
    }

    return (status);
}

/***********************************************************************************************************************
* Function Name: R_Config_SCI11_Serial_Send
* Description  : This function transmits SCI11data
* Arguments    : tx_buf -
*                    transfer buffer pointer (Not used when transmit data handled by DMAC/DTC)
*                tx_num -
*                    buffer size (Not used when transmit data handled by DMAC/DTC)
* Return Value : status -
*                    MD_OK or MD_ARGERROR
***********************************************************************************************************************/

MD_STATUS R_Config_SCI11_Serial_Send(uint8_t * const tx_buf, uint16_t tx_num)
{
    MD_STATUS status = MD_OK;

    if (1U > tx_num)
    {
        status = MD_ARGERROR;
    }
    else
    {
        gp_sci11_tx_address = tx_buf;
        g_sci11_tx_count = tx_num;

        /* Set TXD11 pin */
        PORTB.PMR.BYTE |= 0x20U;
        SCI11.SCR.BIT.TIE = 1U;
        SCI11.SCR.BIT.TE = 1U;
    }

    return (status);
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
