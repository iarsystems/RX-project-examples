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
* File Name    : r_cg_sci_user.c
* Version      : Applilet4 for RX23T V1.00.01.01 [30 Oct 2015]
* Device(s)    : R5F523T5AxFM
* Tool-Chain   : IAR Systems ICCRX
* Description  : This file implements device driver for SCI module.
* Creation Date: 2016-02-12
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
extern uint8_t * gp_sci5_tx_address;                /* SCI5 send buffer address */
extern uint16_t  g_sci5_tx_count;                   /* SCI5 send data number */
/* Start user code for global. Do not edit comment generated here */

static volatile uint8_t sci5_txdone;

/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_sci5_transmit_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = VECT_SCI5_TXI5
#if FAST_INTERRUPT_VECTOR == VECT_SCI5_TXI5
__fast_interrupt static void r_sci5_transmit_interrupt(void)
#else
__interrupt static void r_sci5_transmit_interrupt(void)
#endif
{
    if (0U < g_sci5_tx_count)
    {
        SCI5.TDR = *gp_sci5_tx_address;
        gp_sci5_tx_address++;
        g_sci5_tx_count--;
    }
    else
    {
        SCI5.SCR.BIT.TIE = 0U;
        SCI5.SCR.BIT.TEIE = 1U;
    }
}
/***********************************************************************************************************************
* Function Name: r_sci5_transmitend_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = VECT_SCI5_TEI5
#if FAST_INTERRUPT_VECTOR == VECT_SCI5_TEI5
__fast_interrupt static void r_sci5_transmitend_interrupt(void)
#else
__interrupt static void r_sci5_transmitend_interrupt(void)
#endif
{
    SCI5.SCR.BIT.TIE = 0U;
    SCI5.SCR.BIT.TEIE = 0U;

    /* Clear TE and RE bits */
    if(0U == SCI5.SCR.BIT.RIE)
    {
        SCI5.SCR.BYTE &= 0xCFU;
    }

    r_sci5_callback_transmitend();
}
/***********************************************************************************************************************
* Function Name: r_sci5_callback_transmitend
* Description  : This function is a callback function when SCI5 finishes transmission.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_sci5_callback_transmitend(void)
{
    /* Start user code. Do not edit comment generated here */

    sci5_txdone = TRUE;

    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */

/*******************************************************************************
* Function Name: R_SCI5_SPIMasterTransmit
* Description  : This function sends SPI5 data to slave device.
* Arguments    : tx_buf -
*                    transfer buffer pointer
*                tx_num -
*                    buffer size
* Return Value : status -
*                    MD_OK or MD_ARGERROR
*******************************************************************************/
MD_STATUS R_SCI5_SPIMasterTransmit (uint8_t * const tx_buf,
                                      const uint16_t tx_num)
{
    MD_STATUS status = MD_OK;

    /* clear the flag before initiating a new transmission */
    sci5_txdone = FALSE;

    /* Send the data using the API */
    status = R_SCI5_SPI_Master_Send(tx_buf, tx_num);

    /* Wait for the transmit end flag */
    while (FALSE == sci5_txdone)
    {
        /* Wait */
    }

    return (status);
}

/*******************************************************************************
* End of function R_SCI5_SPIMasterTransmit
*******************************************************************************/

/* End user code. Do not edit comment generated here */
