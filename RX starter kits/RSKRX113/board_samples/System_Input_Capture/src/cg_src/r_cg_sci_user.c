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
#include "rskrx113def.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
extern uint8_t * gp_sci1_tx_address;         /* SCI1 send buffer address */
extern uint16_t  g_sci1_tx_count;            /* SCI1 send data number */
extern uint8_t * gp_sci1_rx_address;         /* SCI1 receive buffer address */
extern uint16_t  g_sci1_rx_count;            /* SCI1 receive data number */
extern uint16_t  g_sci1_rx_length;           /* SCI1 receive data length */
/* Start user code for global. Do not edit comment generated here */

/* Global used to receive a character from the PC terminal */
volatile uint8_t g_rx_char;

/* Flag used to control transmission to PC terminal */
volatile uint8_t g_tx_flag = FALSE;

/* Flag used to indicate a character has been received in to g_rx_char */
volatile uint8_t g_rx_flag = FALSE;

/* Flag used locally to detect transmission complete */
static volatile uint8_t sci1_txdone;

/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_sci1_transmit_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = VECT_SCI1_TXI1
#if FAST_INTERRUPT_VECTOR == VECT_SCI1_TXI1
__fast_interrupt static void r_sci1_transmit_interrupt(void)
#else
__interrupt static void r_sci1_transmit_interrupt(void)
#endif
{
    if (g_sci1_tx_count > 0U)
    {
        SCI1.TDR = *gp_sci1_tx_address;
        gp_sci1_tx_address++;
        g_sci1_tx_count--;
    }
    else 
    {
        SCI1.SCR.BIT.TIE = 0U;
        SCI1.SCR.BIT.TEIE = 1U;
    }
}
/***********************************************************************************************************************
* Function Name: r_sci1_transmitend_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = VECT_SCI1_TEI1
#if FAST_INTERRUPT_VECTOR == VECT_SCI1_TEI1
__fast_interrupt static void r_sci1_transmitend_interrupt(void)
#else
__interrupt static void r_sci1_transmitend_interrupt(void)
#endif
{
    /* Set TXD1 pin */
    PORT1.PMR.BYTE &= 0xBFU;
    SCI1.SCR.BIT.TIE = 0U;
    SCI1.SCR.BIT.TE = 0U;
    SCI1.SCR.BIT.TEIE = 0U;

    r_sci1_callback_transmitend();
}
/***********************************************************************************************************************
* Function Name: r_sci1_receive_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = VECT_SCI1_RXI1
#if FAST_INTERRUPT_VECTOR == VECT_SCI1_RXI1
__fast_interrupt static void r_sci1_receive_interrupt(void)
#else
__interrupt static void r_sci1_receive_interrupt(void)
#endif
{
    if (g_sci1_rx_length > g_sci1_rx_count)
    {
        *gp_sci1_rx_address = SCI1.RDR;
        gp_sci1_rx_address++;
        g_sci1_rx_count++;

        if (g_sci1_rx_length == g_sci1_rx_count)
        {
            r_sci1_callback_receiveend();
        }
    }
}
/***********************************************************************************************************************
* Function Name: r_sci1_receiveerror_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = VECT_SCI1_ERI1
#if FAST_INTERRUPT_VECTOR == VECT_SCI1_ERI1
__fast_interrupt static void r_sci1_receiveerror_interrupt(void)
#else
__interrupt static void r_sci1_receiveerror_interrupt(void)
#endif
{
    uint8_t err_type;

    r_sci1_callback_receiveerror();

    /* Clear overrun, framing and parity error flags */
    err_type = SCI1.SSR.BYTE;
    SCI1.SSR.BYTE = err_type & 0xC7U;
}
/***********************************************************************************************************************
* Function Name: r_sci1_callback_transmitend
* Description  : This function is a callback function when SCI1 finishes transmission.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_sci1_callback_transmitend(void)
{
    /* Start user code. Do not edit comment generated here */

    sci1_txdone = TRUE;

    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: r_sci1_callback_receiveend
* Description  : This function is a callback function when SCI1 finishes reception.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_sci1_callback_receiveend(void)
{
    /* Start user code. Do not edit comment generated here */

	/* Contents of g_rx_char gets checked in R_MAIN_Serial_Handler*/
	g_rx_flag = TRUE;

    /* Set up SCI1 receive buffer again */
    R_SCI1_Serial_Receive((uint8_t *)&g_rx_char, 1);


    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: r_sci1_callback_receiveerror
* Description  : This function is a callback function when SCI1 reception encounters error.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_sci1_callback_receiveerror(void)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_SCI1_AsyncTransmit
* Description  : This function sends SCI1 data and waits for the transmit end flag.
* Arguments    : tx_buf -
*                    transfer buffer pointer
*                tx_num -
*                    buffer size
* Return Value : status -
*                    MD_OK or MD_ARGERROR
***********************************************************************************************************************/
MD_STATUS R_SCI1_AsyncTransmit (uint8_t * const tx_buf, const uint16_t tx_num)
{
    MD_STATUS status = MD_OK;

    /* clear the flag before initiating a new transmission */
    sci1_txdone = FALSE;

    /* Send the data using the API */
    status = R_SCI1_Serial_Send(tx_buf, tx_num);

    /* Wait for the transmit end flag */
    while (FALSE == sci1_txdone)
    {
        /* Wait */
    }
    return (status);
}

/***********************************************************************************************************************
* End of function R_SCI1_AsyncTransmit
***********************************************************************************************************************/

/* End user code. Do not edit comment generated here */
