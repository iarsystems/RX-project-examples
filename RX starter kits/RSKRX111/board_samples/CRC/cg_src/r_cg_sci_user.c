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
* File Name    : r_cg_sci_user.c
* Version      : Applilet4 for RX111 V1.00.00.05 [06 Jun 2013]
* Device(s)    : R5F51115AxFM
* Tool-Chain   : IAR Systems icc78k0r
* Description  : This file implements device driver for SCI module.
* Creation Date: 2013-08-16
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
extern uint8_t * gp_sci1_tx_address;         /* SCI1 send buffer address */
extern uint16_t  g_sci1_tx_count;            /* SCI1 send data number */
extern uint8_t * gp_sci1_rx_address;         /* SCI1 receive buffer address */
extern uint16_t  g_sci1_rx_count;            /* SCI1 receive data number */
extern uint16_t  g_sci1_rx_length;           /* SCI1 receive data length */
extern uint8_t * gp_sci5_tx_address;         /* SCI5 send buffer address */
extern uint16_t  g_sci5_tx_count;            /* SCI5 send data number */
/* Start user code for global. Do not edit comment generated here */

/* The functions in this file are generated using "Application Leading Tool (Applilet)" for e2 studio.
 * Warnings exist for interrupt handlers. */

uint8_t sci5_tx_is_busy(void);
uint8_t sci1_tx_is_busy(void);

volatile uint8_t g_sci1_data_available = 0;
volatile uint8_t byteReceived = 0U;
volatile uint8_t g_sci5_tx_in_process = 0;
volatile uint8_t g_sci1_tx_in_process = 0;
uint8_t gp_sci1_rx_buf[2] = "\0\0";
static void r_sci1_callback_transmitend(void);
static void r_sci1_callback_receiveend(void);
static void r_sci1_callback_receiveerror(void);
static void r_sci5_callback_transmitend(void);

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
    if(g_sci1_tx_count > 0U)
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
    MPC.P16PFS.BYTE = 0x00U;
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
    SCI1.SSR.BIT.ORER = 0U; /* clear overrun error flag */
    SCI1.SSR.BIT.FER = 0U;  /* clear framing error flag */
    SCI1.SSR.BIT.PER = 0U;  /* clear parity error flag */

    r_sci1_callback_receiveerror();
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
    g_sci1_tx_in_process = 0U;
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
    /* reset rx buffer for next byte */
    R_SCI1_Serial_Receive(gp_sci1_rx_buf, 1);
    g_sci1_data_available = 1U;
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
    if(g_sci5_tx_count > 0U)
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
    SCI5.SCR.BIT.TE = 0U;
    SCI5.SCR.BIT.TEIE = 0U;

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
     g_sci5_tx_in_process = 0U;
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/***********************************************************************************************************************
* Function Name: sendSPI5
* Description  : This function sends SPI5 data to slave device. Adds flagging around R_SCI5_SPI_Master_Send
* Arguments    : tx_buf -
*                    transfer buffer pointer (Not used when transmit data handled by DTC)
*                tx_num -
*                    buffer size
* Return Value : None
***********************************************************************************************************************/
void sendSPI5 (uint8_t * const tx_buf, uint16_t const tx_num)
{
    g_sci5_tx_in_process = 1U;
    R_SCI5_SPI_Master_Send(tx_buf, tx_num);
}
/******************************************************************************
* End of function sendSPI5
******************************************************************************/

/******************************************************************************
* Function Name : startSCI1
* Description   : enables SCI1
* Argument      : none
* Return value  : None
******************************************************************************/
void startSCI1 (void)
{
    R_SCI1_Start();
}
/******************************************************************************
* End of function startSCI1
******************************************************************************/

/***********************************************************************************************************************
* Function Name: sendSCI1
* Description  : This function transmits SCI1 data.
* Arguments    : tx_buf -
*                    transmit buffer pointer (Not used when transmit data handled by DTC)
*                tx_num -
*                    transmit buffer size
***********************************************************************************************************************/
void sendSCI1 (uint8_t * const tx_buf, uint16_t const tx_num)
{
    /* set busy flag */
    g_sci1_tx_in_process = 1U;
    g_sci1_tx_count = tx_num;
    gp_sci1_tx_address = tx_buf;
    MPC.P16PFS.BYTE = 0x0AU;
    PORT1.PMR.BYTE |= 0x40U;
    SCI1.SCR.BIT.TIE = 1U;
    SCI1.SCR.BIT.TE = 1U;
}
/******************************************************************************
* End of function sendSCI1
******************************************************************************/

/******************************************************************************
* Function Name : SCI1Receive
* Description   : set SCI1 reception to buffer rx_buf & nBytes bytes
* Argument      : none
* Return value  : None
******************************************************************************/
void SCI1Receive (uint8_t * const rx_buf, uint16_t const nBytes)
{
    gp_sci1_rx_address = rx_buf;
    g_sci1_rx_length = nBytes;
    g_sci1_rx_count = 0;
    MPC.P16PFS.BYTE = 0x0AU;
    PORT1.PMR.BYTE |= 0x40U;
    SCI1.SCR.BIT.RIE = 1U;
    SCI1.SCR.BIT.RE = 1U;
}
/******************************************************************************
* End of function SCI1Receive
******************************************************************************/

/******************************************************************************
* Function Name : initSCI1Receive
* Description   : initialises and enables SCI1 reception
* Argument      : none
* Return value  : None
******************************************************************************/
void initSCI1Receive (void)
{
    SCI1Receive(gp_sci1_rx_buf, 1);
}
/******************************************************************************
* End of function initSCI1Receive
******************************************************************************/

/******************************************************************************
* Function Name : sci5_tx_is_busy
* Description   : reports if sci5 is transmitting
* Argument      : none
* Return value  : None
******************************************************************************/
uint8_t sci5_tx_is_busy (void)
{
    return (g_sci5_tx_in_process);
}
/******************************************************************************
* End of function sci5_tx_is_busy
******************************************************************************/

/******************************************************************************
* Function Name : sci1_tx_is_busy
* Description   : reports if sci1 is transmitting
* Argument      : none
* Return value  : None
******************************************************************************/
uint8_t sci1_tx_is_busy (void)
{
    return (g_sci1_tx_in_process);
}
/******************************************************************************
* End of function sci1_tx_is_busy
******************************************************************************/
/* End user code. Do not edit comment generated here */
