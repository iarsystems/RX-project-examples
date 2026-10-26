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
* Creation Date: 2015-01-21
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
#include <string.h>
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
uint8_t g_sci1_rx_char; /* Buffer for serial Rx characters */
uint8_t g_sci_rx_ready; /* Flag to indicate a Rx character is ready */

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

    g_sci_rx_ready = TRUE;

    /* Set up SCI1 receive buffer again */
    R_SCI1_Serial_Receive(&g_sci1_rx_char, sizeof(g_sci1_rx_char));

    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */

/***********************************************************************************************************************
 * Function Name: R_SCI1_SerialRxInit
 * Description  : This function initialises SCI for single char Rx
 * Arguments    : none
 * Return Value : none
 ***********************************************************************************************************************/
void R_SCI1_SerialRxInit (void)
{
    g_sci_rx_ready = FALSE;
    R_SCI1_Serial_Receive(&g_sci1_rx_char, sizeof(g_sci1_rx_char));
}

/***********************************************************************************************************************
 * End of function R_SCI1_SerialRxInit
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_SCI1_SerialRxCheck
 * Description  : This function tests for a byte received
 * Arguments    : none
 * Return Value : TRUE  - byte received
 *                FALSE - no byte received
 ***********************************************************************************************************************/
uint8_t R_SCI1_SerialRxCheck (void)
{
    uint8_t ret_val;

    ret_val = FALSE;

    if (TRUE == g_sci_rx_ready)
    {
        ret_val = TRUE;
    }

    return (ret_val);
}

/***********************************************************************************************************************
 * End of function R_SCI1_SerialRxCheck
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_SCI1_SerialRxGetChar
 * Description  : This function returns a received byte
 * Arguments    : none
 * Return Value : none
 ***********************************************************************************************************************/
void R_SCI1_SerialRxGetChar (uint8_t* rxchar)
{
    (*rxchar) = g_sci1_rx_char;
    g_sci_rx_ready = FALSE; /* Indicate the char has been received */
}

/***********************************************************************************************************************
 * End of function R_SCI1_SerialRxGetChar
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_SCI1_SerialTxChar
 * Description  : This function sends a single byte and stalls until it has left the SCI.
 * Arguments    : none
 * Return Value : none
 ***********************************************************************************************************************/
void R_SCI1_SerialTxChar (uint8_t txchar)
{
    R_SCI1_Serial_Send((uint8_t*) &txchar, 1);

    /* Wait for the transmit end flag */
    while (FALSE == sci1_txdone)
    {
        /* Wait */
    }
}

/***********************************************************************************************************************
 * End of function R_SCI1_SerialTxChar
 ***********************************************************************************************************************/



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

    /* Check and ensure there is a valid character in the buffer */
    if (0x00 != (*tx_buf))
    {
        /* clear the flag before initiating a new transmission */
        sci1_txdone = FALSE;

        /* Send the data using the API */
        status = R_SCI1_Serial_Send(tx_buf, tx_num);

        /* Wait for the transmit end flag */
        while (FALSE == sci1_txdone)
        {
            /* Wait */
        }
    }
    return (status);
}

/***********************************************************************************************************************
 * End of function R_SCI1_AsyncTransmit
 ***********************************************************************************************************************/

/*******************************************************************************
 * Function Name : R_SCI1_SerialTxString
 * Description   : Transmits null-terminated string.
 * Argument      : (char*) msg_string - null terminated string
 * Return value  : None
 *******************************************************************************/
void R_SCI1_SerialTxString (const char* msg_string)
{
    R_SCI1_AsyncTransmit((uint8_t *) msg_string, (uint16_t) strlen(msg_string));
}

/*******************************************************************************
 * End of function R_SCI1_SerialTxString
 *******************************************************************************/

/* End user code. Do not edit comment generated here */
