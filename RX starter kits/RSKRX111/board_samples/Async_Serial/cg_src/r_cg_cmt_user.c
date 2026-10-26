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
* File Name    : r_cg_cmt_user.c
* Version      : Applilet4 for RX111 V1.00.00.05 [06 Jun 2013]
* Device(s)    : R5F51115AxFM
* Tool-Chain   : IAR Systems icc78k0r
* Description  : This file implements device driver for CMT module.
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
#include "r_cg_cmt.h"
/* Start user code for include. Do not edit comment generated here */
#include "r_cg_port.h"
#include "sci.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */

/* The functions in this file are generated using "Application Leading Tool (Applilet)" for e2 studio.
 * Warnings exist for interrupt handlers and callback functions. */

extern volatile uint8_t g_sci_flag;
/* Declare transmit and receive buffer variables used in this file */
volatile uint8_t g_sci_tx_buffer[2] = {0x2F, 0x0};
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_cmt_cmi0_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = VECT_CMT0_CMI0
#if FAST_INTERRUPT_VECTOR == VECT_CMT0_CMI0
__fast_interrupt static void r_cmt_cmi0_interrupt(void)
#else
__interrupt static void r_cmt_cmi0_interrupt(void)
#endif
{
    /* Start user code. Do not edit comment generated here */

    /* Check SCI status flag */
    if (g_sci_flag)
    {
        /* Increment number in transmit buffer */
        g_sci_tx_buffer[0]++;
        /* Check if hex value of transmit buffer is greater than 0x39 (9) */
        if (g_sci_tx_buffer[0] > 0x39)
        {
            /* Insert a new line and return the cursor left */
            sendSCI1("\r\n", 2);
            /* Reset the ASCII character in the transmit buffer back to 0 */
            g_sci_tx_buffer[0] = 0x2F;
        }
        else
        {
            /* Send the transmit buffer to the terminal */
            sendSCI1((uint8_t *)g_sci_tx_buffer, 1);
        }

        /* Turn on LED0 */
        LED0_On();
        /* Turn off LED1 */
        LED1_Off();
    }

    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
