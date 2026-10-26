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
* File Name    : r_cg_main.c
* Version      : Applilet4 for RX111 V1.00.00.05 [06 Jun 2013]
* Device(s)    : R5F51115AxFM
* Tool-Chain   : IAR Systems icc78k0r
* Description  : This file implements main function.
* Creation Date: 2013-08-26
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
#include "r_cg_cgc.h"
#include "r_cg_icu.h"
#include "r_cg_port.h"
#include "r_cg_cmt.h"
#include "r_cg_sci.h"
/* Start user code for include. Do not edit comment generated here */
/* Following header file provides string type definitions. */
#include <string.h>
#include "lcd.h"
#include "utility.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */

/* Master header transmission function prototype */
static void lin_master_header_transmit(void);

/* Flag to indicate when ADC data has been received from the slave */
volatile uint8_t g_ad_received = 0;

uint8_t rx_buf[3];

/* End user code. Do not edit comment generated here */


/* Set option bytes */
#pragma location = "OPTBYTE"
__root const uint32_t OFS0_location = 0xFFFFFFFFUL;
#pragma location = "OPTBYTE"
__root const uint32_t OFS1_location = 0xFFFFFF7FUL;


void R_MAIN_UserInit(void);
/***********************************************************************************************************************
* Function Name: main
* Description  : This function implements main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void main(void)
{
    R_MAIN_UserInit();
    /* Start user code. Do not edit comment generated here */

    /* Display sample name banner */
    Display_LCD(0, (uint8_t *)"LIN Master");

    /* Initialise SCI12 extended mode for LIN BREAK and SYNC fields */
    R_SCI12_ExtendedMode();

    /* Start the SCI12 UART module */
    R_SCI12_Start();

    /* Set up the receive buffer for reception of 3 bytes */
    R_SCI12_Serial_Receive(rx_buf, 3);

    while (1U)
    {

        /* used to time out reception */
        uint32_t count = 0;

        /* Send LIN Header */
        lin_master_header_transmit();

        /* Time out or receive ADC data in interrupt  */
        while ((0 == g_ad_received) && (count++ < 0x7FFF))
        {
            /* Wait */
        }

        /* If data has been received in the interrupt, display it on the LCD */
        if (1 == g_ad_received)
        {
            /* Buffer where we form ADC data string */
            char    g_ad_buf[10];

            /* Checksum of all 3 bytes should be 0xFF */
            uint8_t checksum = (uint8_t)(rx_buf[0] + rx_buf[1] + rx_buf[2]);

            if (0xFF == checksum)
            {
                /* first 2 bytes contain the ADC value returned from the LIN slave */
                uint16_t ad_value = (uint16_t)((rx_buf[0] << 8u) + rx_buf[1]);

                /* Convert to a string and display it on the LCD */
                strcpy(g_ad_buf, "=H\'WXYZ");
                uint16_to_string(g_ad_buf, 3u, ad_value);
            }
            else
            {
                strcpy(g_ad_buf, "CS_FAIL ");
            }

            Display_LCD(1, (uint8_t *)g_ad_buf);

            /* Reset the global flag that was set in the interrupt */
            g_ad_received = 0;

            /* Set up the receive buffer for reception of 3 bytes again */
            R_SCI12_Serial_Receive(rx_buf, 3);
        }
    }
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: R_MAIN_UserInit
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_MAIN_UserInit(void)
{
    /* Start user code. Do not edit comment generated here */
    Init_LCD();
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/*******************************************************************************
* Function Name : lin_master_header_transmit
* Description   : Sends the SYNC break, SYNC and id fields to the LIN slave
* Parameters    : none
* Return value  : none
*******************************************************************************/
static void lin_master_header_transmit (void)
{
    /* Byte used to transmit SYNC and ID bytes */
    uint8_t tx_byte;

    /* Wait 1 bit period before sending the break */
    R_Microsec_Delay(1u * BIT_PERIOD_US);

    /* Disable reception - we need to do this to prevent the UART from receiving
       on the bidirectional LIN line when we transmit */
    SCI12.SCR.BIT.RE = 0x0;

    /* Turn on PORTE pin 1 for peripheral function - this gets disabled in the
     * SCI12 transmit end interrupt handler */
    PORTE.PMR.BYTE |= 0x02U;

    /* Send the LIN break - first clear the STR register */
    SCI12.STCR.BYTE = 0x3F;

    /* Start the SCI12 counter */
    SCI12.TCR.BIT.TCST = 0x1;

    /* Wait for the Break Field Low Width Detection Flag to clear - which
       indicates completion of the break low field */
    while (0x0 == SCI12.STR.BIT.BFDF)
    {
        /* Wait */
        uint16_t wait = 0;
        wait++;
    }

    /* Clear the BFDF in the STR register */
    SCI12.STCR.BIT.BFDCL = 0x1;

    /* Stop the timer counting */
    SCI12.TCR.BIT.TCST = 0;

    /* Wait for 1 bit period delay */
    R_Microsec_Delay(1u * BIT_PERIOD_US);

    /* Send the SYNC byte */
    tx_byte = 0x55;
    R_SCI12_Serial_Send(&tx_byte, 1);

    /* Wait for transmission complete */
    while (0x0 == g_tx12_done)
    {
        /* Wait */
    }

    /* Sent it, reset the flag */
    g_tx12_done = 0;

    /* Wait for 1 bit period delay */
    R_Microsec_Delay(1u * BIT_PERIOD_US);

    /* Send the Slave ID byte */
    tx_byte = 0x10;

    R_SCI12_Serial_Send(&tx_byte, 1);

    /* Wait for transmission complete */
    while (0x0 == g_tx12_done)
    {
        /* Wait */
    }

    /* Sent it, reset the flag */
    g_tx12_done = 0;

    /* Status Flag Clear */
    SCI12.STCR.BYTE = 0x3F;

    /* Enable reception now */
    SCI12.SCR.BIT.RE = 0x1;
}
/*******************************************************************************
End of function lin_master_header_transmit
*******************************************************************************/

/* End user code. Do not edit comment generated here */
