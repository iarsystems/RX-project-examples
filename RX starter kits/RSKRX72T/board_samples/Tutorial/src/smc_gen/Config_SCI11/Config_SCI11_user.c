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
* File Name    : Config_SCI11_user.c
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
extern volatile uint8_t * gp_sci11_tx_address;                /* SCI11 transmit buffer address */
extern volatile uint16_t  g_sci11_tx_count;                   /* SCI11 transmit data number */
extern volatile uint8_t * gp_sci11_rx_address;                /* SCI11 receive buffer address */
extern volatile uint16_t  g_sci11_rx_count;                   /* SCI11 receive data number */
extern volatile uint16_t  g_sci11_rx_length;                  /* SCI11 receive data length */
/* Start user code for global. Do not edit comment generated here */

/* Global used to receive a character from the PC terminal */
uint8_t g_rx_char;

/* Flag used locally to detect transmission complete */
static volatile uint8_t gs_sci11_txdone;

/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_SCI11_Create_UserInit
* Description  : This function adds user code after initializing the SCI11 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_SCI11_Create_UserInit(void)
{
    /* Start user code for user init. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI11_transmit_interrupt
* Description  : This function is TXI11 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

#pragma vector = VECT_SCI11_TXI11
#if FAST_INTERRUPT_VECTOR == VECT_SCI11_TXI11
__fast_interrupt static void r_Config_SCI11_transmit_interrupt(void)
#else
__interrupt static void r_Config_SCI11_transmit_interrupt(void)
#endif
{
    if (0U < g_sci11_tx_count)
    {
        SCI11.TDR = *gp_sci11_tx_address;
        gp_sci11_tx_address++;
        g_sci11_tx_count--;
    }
    else
    {
        SCI11.SCR.BIT.TIE = 0U;
        SCI11.SCR.BIT.TEIE = 1U;
    }
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI11_transmitend_interrupt
* Description  : This function is TEI11 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void r_Config_SCI11_transmitend_interrupt(void)
{
    /* Set TXD11 pin */
    PORTB.PMR.BYTE &= 0xDFU;

    SCI11.SCR.BIT.TIE = 0U;
    SCI11.SCR.BIT.TE = 0U;
    SCI11.SCR.BIT.TEIE = 0U;
    
    r_Config_SCI11_callback_transmitend();
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI11_receive_interrupt
* Description  : This function is RXI11 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

#pragma vector = VECT_SCI11_RXI11
#if FAST_INTERRUPT_VECTOR == VECT_SCI11_RXI11
__fast_interrupt static void r_Config_SCI11_receive_interrupt(void)
#else
__interrupt static void r_Config_SCI11_receive_interrupt(void)
#endif
{
    if (g_sci11_rx_length > g_sci11_rx_count)
    {
        *gp_sci11_rx_address = SCI11.RDR;
        gp_sci11_rx_address++;
        g_sci11_rx_count++;
    }
    
    if (g_sci11_rx_length <= g_sci11_rx_count)
    {
        /* All data received */
        SCI11.SCR.BIT.RIE = 0U;
        SCI11.SCR.BIT.RE = 0U;
        r_Config_SCI11_callback_receiveend();
    }
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI11_receiveerror_interrupt
* Description  : This function is ERI11 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void r_Config_SCI11_receiveerror_interrupt(void)
{
    uint8_t err_type;
    
    r_Config_SCI11_callback_receiveerror();
    
    /* Clear overrun, framing and parity error flags */
    err_type = SCI11.SSR.BYTE;
    err_type &= 0xC7U;
    err_type |= 0xC0U;
    SCI11.SSR.BYTE = err_type;
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI11_callback_transmitend
* Description  : This function is a callback function when SCI11 finishes transmission
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

static void r_Config_SCI11_callback_transmitend(void)
{
    /* Start user code for r_Config_SCI11_callback_transmitend. Do not edit comment generated here */

    gs_sci11_txdone = TRUE;

    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI11_callback_receiveend
* Description  : This function is a callback function when SCI11 finishes reception
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

static void r_Config_SCI11_callback_receiveend(void)
{
    /* Start user code for r_Config_SCI11_callback_receiveend. Do not edit comment generated here */

    /* Check the contents of g_rx_char */
    if (('c' == g_rx_char) || ('C' == g_rx_char))
    {
        g_adc_trigger = TRUE;
    }

    /* Set up SCI11 receive buffer and callback function again */
    R_Config_SCI11_Serial_Receive((uint8_t *)&g_rx_char, 1);

    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI11_callback_receiveerror
* Description  : This function is a callback function when SCI11 reception encounters error
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

static void r_Config_SCI11_callback_receiveerror(void)
{
    /* Start user code for r_Config_SCI11_callback_receiveerror. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_SCI11_AsyncTransmit
* Description  : This function sends SCI11 data and waits for the transmit end flag.
* Arguments    : tx_buf -
*                    transfer buffer pointer
*                tx_num -
*                    buffer size
* Return Value : status -
*                    MD_OK or MD_ARGERROR
***********************************************************************************************************************/
MD_STATUS R_SCI11_AsyncTransmit(uint8_t * const tx_buf, const uint16_t tx_num)
{
    MD_STATUS status = MD_OK;
    
    /* clear the flag before initiating a new transmission */
    gs_sci11_txdone = FALSE;

    /* Send the data using the API */
    status = R_Config_SCI11_Serial_Send(tx_buf, tx_num);

    /* Wait for the transmit end flag */
    while (FALSE == gs_sci11_txdone)
    {
        /* Wait */
    }
    return (status);
}

/***********************************************************************************************************************
* End of function R_SCI11_AsyncTransmit
***********************************************************************************************************************/

/* End user code. Do not edit comment generated here */
