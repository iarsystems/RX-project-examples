/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*******************************************************************************/
/* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.   */
/*******************************************************************************
* File Name     : async.c
* Version       : 1.00
* Device        : R5F52206
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX220
* Description   : Defines SCI functions used in this sample.
*******************************************************************************/
/*******************************************************************************
* History       : 14.12.2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* System Includes
*******************************************************************************/

/* Defines standard boolean variable types used in this file */
#include <stdbool.h>

/* Following header file provides standard integer type definitions. */
#include <stdint.h>

/* Following header file provides string type definitions. */
#include <string.h>

/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/

/* Defines RX220 port registers */
#include <iorx220.h>

/* Defines RX220 LEDs and switches */
#include "rskrx220def.h"

/* Declares functions defined in this file */
#include "async.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/

/* Declare transmit and receive buffer variables used in this file */
uint8_t g_sci_tx_buffer[2] = {0x2F, 0x0};

/* Declaration of the command string to clear the terminal screen */
const char cmd_clr_scr[] = {27, 91, 50, 74, 0, 27, 91, 72, 0};

/* Declaration of the command string to bring the cursor home on the terminal */
const char cmd_cur_home[] = {27,'[','H',0};

/* Declaration of received data callback pointer */
void (*gSCIReceiveCallbackFunc)(char character) = NULL;


/*******************************************************************************
* Function Name: sci_init
* Description  : This function initialises the SCI channel connected to the
*                RS232 connector on the RSK. The channel is configured for
*                transmission and reception, and instructions are sent to the
*                terminal.
* Arguments    : (void *)receive_callback_func - Function pointer to call when
*                                                data is received.
* Return Value : none
*******************************************************************************/
void sci_init (void ( *const receive_callback_func)(char character))
{
    /* Protection off */
    SYSTEM.PRCR.WORD = PROTECTOFF;

    /* Cancel the SCI1 module clock stop mode */
    MSTP_SCI1 = 0x0;

    /* Protection on */
    SYSTEM.PRCR.WORD = PROTECTON;

    /* Configure SCI1's TXD1 and SCK1 pins for peripheral functions */
    PORT2.PMR.BIT.B6 = 0x1u;
    PORT2.PMR.BIT.B7 = 0x1u;

    /* Configure SCI1's RXD1 pin for peripheral functions */
    PORT3.PMR.BIT.B0 = 0x1u;

    /* PWPR.PFSWE write protect off */
    MPC.PWPR.BYTE = 0x00;

    /* PFS register write protect off */
    MPC.PWPR.BYTE = PFSPROTECTOFF;

    /* Configure SCI1's communication pins */
    /* Set Port2's pin 6 function to TXD1 */
    MPC.P26PFS.BYTE = 0x0Au;

    /* Set Port3's pin 0 function to RXD1 */
    MPC.P30PFS.BYTE = 0x0Au;

    /* Set Port2's pin 7 function to SCK1 */
    MPC.P27PFS.BYTE = 0x0Au;

    /* Turn on PFS register write protect */
    MPC.PWPR.BYTE = PFSPROTECTON;

    /* Disable All bits and CKE0,1 = 00 */
    SCI1.SCR.BYTE = 0x00;

    /* Set output port TXD1(P26) */
    PORT2.PDR.BIT.B6 = 0x1u;

    /* Set Receive port RXD1(P30)*/
    PORT3.PDR.BIT.B0 = 0x1u;

    /* disable All bits and CKE0,1 = 00 once more */
    SCI1.SCR.BYTE = 0x00;

    /* CKS[0-1] = 00: PCLK 0 div
       1Sop 8bits non-parity     */
    SCI1.SMR.BYTE = 0x00;

    /* SMIF = 0: Serial  */
    SCI1.SCMR.BYTE = 0xf2u;

    /* PCLK = 20MHz, Set baudrate to 19200 bps
       32 = 20MHz / (64*0.5*19200)-1 */
    SCI1.BRR = 32u;

    /* Declare a loop count variable */
    uint16_t i = 0;

    /* Allow a small delay */
    for (i = 0; i < 20u; i++)
    {
    }

    /* Clear bits ORER, PER and FER */
    SCI1.SSR.BYTE &= 0xC7u;

    /* Set SCI1 RXI1 interrupt priority level to 7 */
    IPR(SCI1,RXI1) = 0x7u;

    /* Enable SCI1 RXI1 interrupts */
    IEN(SCI1,RXI1) = 0x1u;

    /* Clear SCI1 RXI1 interrupt flag */
    IR(SCI1,RXI1) = 0x0;

    /* Set bits TIE, RIE, RE and TE. Enable receive interrupts */
    SCI1.SCR.BYTE = 0xF0u;

    /* Clear the text on terminal window */
    sci_transmit(cmd_clr_scr);
    sci_transmit(cmd_cur_home);

    /* Set the receive callback function */
    gSCIReceiveCallbackFunc = receive_callback_func;
}
/*******************************************************************************
* End of function sci_init
*******************************************************************************/

/******************************************************************************
* Function Name: sci_transmit
* Description  : Transmits null-terminated string.
* Arguments    : (uint8_t*) msg_string - null terminated string
* Return Value : None
******************************************************************************/
void sci_transmit (const char * const msg_string)
{
    /* Declare a count variable */
    uint8_t  i = 0;

    /* Declare a count variable */
    uint16_t j = 0;

    /* This loop reads the text string and puts it into the UART's
       transmit buffer */
    for (i = 0; msg_string[i]; i++)
    {
        /* Check that the last transmission was completed */
        while (0x0 == SCI1.SSR.BIT.TEND)
        {
        }

        /* Transmit the next character */
        SCI1.TDR = (unsigned char)msg_string[i];

        /* Count up to 1000 */
        for (j = 0; j < 1000; j++)
        {
            /* delay */
        }
    }
}
/******************************************************************************
* End of function sci_transmit
******************************************************************************/

/*******************************************************************************
* Function Name: Excep_SCI1_RXI1_isr
* Description  : SCI channel 1 recieve ISR. Calls the SCI receive callback
*                function
* Arguments    : none
* Return Value : none
*******************************************************************************/
#pragma vector=VECT_SCI1_RXI1
__interrupt  void Excep_SCI1_RXI1_isr(void)
{
    /* Disable recieve interrupt */
    IEN(SCI1, RXI1) = 0x0;

    /* Call the receive callback function */
    gSCIReceiveCallbackFunc((char)SCI1.RDR);

    /* Re-enable recieve Interrupt */
    IEN(SCI1, RXI1) = 0x1u;
}
/*******************************************************************************
* End of function Excep_SCI1_RXI1_isr
*******************************************************************************/

