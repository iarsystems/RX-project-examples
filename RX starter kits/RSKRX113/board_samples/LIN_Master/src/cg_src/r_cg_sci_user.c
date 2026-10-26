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
* File Name    : r_cg_sci_user.c
* Version      : Applilet4 for RX113 V1.01.00.02 [31 Jul 2014]
* Device(s)    : R5F51138AxFP
* Tool-Chain   : IAR Systems ICCRX
* Description  : This file implements device driver for SCI module.
* Creation Date: 2015-01-20
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
extern uint8_t * gp_sci12_tx_address;         /* SCI12 send buffer address */
extern uint16_t  g_sci12_tx_count;            /* SCI12 send data number */
extern uint8_t * gp_sci12_rx_address;         /* SCI12 receive buffer address */
extern uint16_t  g_sci12_rx_count;            /* SCI12 receive data number */
extern uint16_t  g_sci12_rx_length;           /* SCI12 receive data length */
/* Start user code for global. Do not edit comment generated here */

/* Flag to indicate when data sent from the sci12 UART */
volatile uint8_t g_tx12_done = 0;

/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_sci12_transmit_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = VECT_SCI12_TXI12
#if FAST_INTERRUPT_VECTOR == VECT_SCI12_TXI12
__fast_interrupt static void r_sci12_transmit_interrupt(void)
#else
__interrupt static void r_sci12_transmit_interrupt(void)
#endif
{
    if (g_sci12_tx_count > 0U)
    {
        SCI12.TDR = *gp_sci12_tx_address;
        gp_sci12_tx_address++;
        g_sci12_tx_count--;
    }
    else 
    {
        SCI12.SCR.BIT.TIE = 0U;
        SCI12.SCR.BIT.TEIE = 1U;
    }
}
/***********************************************************************************************************************
* Function Name: r_sci12_transmitend_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = VECT_SCI12_TEI12
#if FAST_INTERRUPT_VECTOR == VECT_SCI12_TEI12
__fast_interrupt static void r_sci12_transmitend_interrupt(void)
#else
__interrupt static void r_sci12_transmitend_interrupt(void)
#endif
{
    /* Set TXD12 pin */
    PORT1.PMR.BYTE &= 0xEFU;
    SCI12.SCR.BIT.TIE = 0U;
    SCI12.SCR.BIT.TE = 0U;
    SCI12.SCR.BIT.TEIE = 0U;

    r_sci12_callback_transmitend();
}
/***********************************************************************************************************************
* Function Name: r_sci12_receive_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = VECT_SCI12_RXI12
#if FAST_INTERRUPT_VECTOR == VECT_SCI12_RXI12
__fast_interrupt static void r_sci12_receive_interrupt(void)
#else
__interrupt static void r_sci12_receive_interrupt(void)
#endif
{
    if (g_sci12_rx_length > g_sci12_rx_count)
    {
        *gp_sci12_rx_address = SCI12.RDR;
        gp_sci12_rx_address++;
        g_sci12_rx_count++;

        if (g_sci12_rx_length == g_sci12_rx_count)
        {
            r_sci12_callback_receiveend();
        }
    }
}
/***********************************************************************************************************************
* Function Name: r_sci12_receiveerror_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = VECT_SCI12_ERI12
#if FAST_INTERRUPT_VECTOR == VECT_SCI12_ERI12
__fast_interrupt static void r_sci12_receiveerror_interrupt(void)
#else
__interrupt static void r_sci12_receiveerror_interrupt(void)
#endif
{
    uint8_t err_type;

    r_sci12_callback_receiveerror();

    /* Clear overrun, framing and parity error flags */
    err_type = SCI12.SSR.BYTE;
    SCI12.SSR.BYTE = err_type & 0xC7U;
}
/***********************************************************************************************************************
* Function Name: r_sci12_callback_transmitend
* Description  : This function is a callback function when SCI12 finishes transmission.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_sci12_callback_transmitend(void)
{
    /* Start user code. Do not edit comment generated here */
    g_tx12_done = 1;

    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: r_sci12_callback_receiveend
* Description  : This function is a callback function when SCI12 finishes reception.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_sci12_callback_receiveend(void)
{
    /* Start user code. Do not edit comment generated here */
    g_ad_received = 1;

    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: r_sci12_callback_receiveerror
* Description  : This function is a callback function when SCI12 reception encounters error.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_sci12_callback_receiveerror(void)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
