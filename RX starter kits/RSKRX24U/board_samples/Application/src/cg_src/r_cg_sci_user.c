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
* File Name    : r_cg_sci_user.c
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
extern uint8_t *gp_sci9_tx_address;                /* SCI9 send buffer address */
extern uint16_t g_sci9_tx_count;                   /* SCI9 send data number */
/* Start user code for global. Do not edit comment generated here */

static volatile uint8_t sci9_txdone;

/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_sci9_transmit_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = VECT_SCI9_TXI9
#if FAST_INTERRUPT_VECTOR == VECT_SCI9_TXI9
__fast_interrupt static void r_sci9_transmit_interrupt(void)
#else
__interrupt static void r_sci9_transmit_interrupt(void)
#endif
{
    if (0U < g_sci9_tx_count)
    {
        SCI9.TDR = *gp_sci9_tx_address;
        gp_sci9_tx_address++;
        g_sci9_tx_count--;
    }
    else
    {
        SCI9.SCR.BIT.TIE = 0U;
        SCI9.SCR.BIT.TEIE = 1U;
    }
}
/***********************************************************************************************************************
* Function Name: r_sci9_transmitend_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = VECT_SCI9_TEI9
#if FAST_INTERRUPT_VECTOR == VECT_SCI9_TEI9
__fast_interrupt static void r_sci9_transmitend_interrupt(void)
#else
__interrupt static void r_sci9_transmitend_interrupt(void)
#endif
{
    SCI9.SCR.BIT.TIE = 0U;
    SCI9.SCR.BIT.TEIE = 0U;

    /* Clear TE and RE bits */
    if(0U == SCI9.SCR.BIT.RIE)
    {
        SCI9.SCR.BYTE &= 0xCFU;
    }

    r_sci9_callback_transmitend();
}
/***********************************************************************************************************************
* Function Name: r_sci9_callback_transmitend
* Description  : This function is a callback function when SCI9 finishes transmission.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void r_sci9_callback_transmitend(void)
{
    /* Start user code. Do not edit comment generated here */

    sci9_txdone = TRUE;

    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/*******************************************************************************
* Function Name: R_SCI9_SPIMasterTransmit
* Description  : This function sends SPI9 data to slave device.
* Arguments    : tx_buf -
*                    transfer buffer pointer
*                tx_num -
*                    buffer size
* Return Value : status -
*                    MD_OK or MD_ARGERROR
*******************************************************************************/
MD_STATUS R_SCI9_SPIMasterTransmit (uint8_t * const tx_buf,
                                      const uint16_t tx_num)
{
    MD_STATUS status = MD_OK;

    /* clear the flag before initiating a new transmission */
    sci9_txdone = FALSE;

    /* Send the data using the API */
    status = R_SCI9_SPI_Master_Send(tx_buf, tx_num);

    /* Wait for the transmit end flag */
    while (FALSE == sci9_txdone)
    {
        /* Wait */
    }

    return (status);
}

/*******************************************************************************
* End of function R_SCI9_SPIMasterTransmit
*******************************************************************************/

/* End user code. Do not edit comment generated here */
