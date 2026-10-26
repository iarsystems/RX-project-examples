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
/* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.   */
/*******************************************************************************
* File Name     : sync.c
* Version       : 1.00
* Device        : R5F563TE
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX63T144
* Description   : Defines sync serial functions used in this sample.
*******************************************************************************/
/*******************************************************************************
* History       : 05.12.2013  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* System Includes
*******************************************************************************/
/* Following header file provides standard integer type definitions. */
#include <stdint.h>

/* Following header file provides string type definitions. */
#include <string.h>

/* Following header file provides Boolean type definitions */
#include <stdbool.h>

/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/
/* Defines  R5F563TE port registers */
#include <iorx63t.h>

/* Defines LCD functions used in this file */
#include "lcd.h"

/* Provides declarations of functions defined in this file */
#include "sync.h"

/*******************************************************************************
User Defines
*******************************************************************************/
/* Define macros for baud rate and bit period - from 2.4 to 38.4 kbps */
#define BAUD_RATE1       19200
#define BAUD_RATE2        9600

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Declaration of the SCI0 transmit buffer */
uint8_t g_sci0_tx_buffer[] = "SCI0 to SCI2 Loopback Test";

/* Declaration of the SCI12 transmit buffer */
uint8_t g_sci2_tx_buffer[] = "SCI2 to SCI0 Loopback Test";

/* Declaration of SCI0 receive buffer */
uint8_t g_sci0_rx_buffer[BUF_SIZ];

/* Declaration of SCI12 receive buffer */
uint8_t g_sci2_rx_buffer[BUF_SIZ];

/* Declaration of loopback test success flag */
volatile bool g_test_pass = true;

/* Declare a SCI0 data receive counter */
uint8_t g_sci0_rx_counter = 0;

/* Declare a SCI12 data receive counter */
uint8_t g_sci2_rx_counter = 0;

/*******************************************************************************
* Function Name: Init_Sync
* Description  : This function reserves and clears BUF_SIZ bytes of memory for
*                SCI0 and SCI2's receive buffers.
* Argument     : none
* Return value : none
*******************************************************************************/
void Init_Sync (void)
{
    /* Clear the SCI0 and SCI2 receive buffers */
    memset(g_sci0_rx_buffer, 0x00, BUF_SIZ);
    memset(g_sci2_rx_buffer, 0x00, BUF_SIZ);

    /* Configure RXD0, TXD0 and SCK0 as input, output and output */
    /* Set Receive port RXD0(PB1) - port direction input */
    PORTB.PDR.BIT.B1 = 0x0;

    /* Set output port TXD0(PB2) - port direction output */
    PORTB.PDR.BIT.B2 = 0x1;

    /* Set Receive port SCK0(PB3) - port direction output */
    PORTB.PDR.BIT.B3 = 0x1;

    /* Configure RXD2, TXD2 and SCK2 as input, output and input */
    /* Set Receive port RXD2(P03) - port direction input */
    PORT0.PDR.BIT.B3 = 0x0;

    /* Set output port TXD2(P02) - port direction output */
    PORT0.PDR.BIT.B2 = 0x1;

    /* Set Receive port SCK2(P14) - port direction input */
    PORT1.PDR.BIT.B4 = 0x0;

    /* Configure SCI0's TXD0, RXD0 and SCK0 pins for peripheral functions */
    PORTB.PMR.BIT.B1 = 0x1;
    PORTB.PMR.BIT.B2 = 0x1;
    PORTB.PMR.BIT.B3 = 0x1;

    /* Configure Port 0 pins 2 and 3 and Port 1 pin 4 for peripheral use */
    PORT0.PMR.BIT.B2 = 0x1;
    PORT0.PMR.BIT.B3 = 0x1;
    PORT1.PMR.BIT.B4 = 0x1;

    /* Allow writing to the PFSWE bit in the PWPR register */
    MPC.PWPR.BIT.B0WI = 0x0;

    /* Then turn off PFS register write protection */
    MPC.PWPR.BIT.PFSWE = 0x1;

    /* Configure pin 3 of port B as SCK0 */
    MPC.PB3PFS.BIT.PSEL = 0xA;

    /* Configure pin 1 of port B as RXD0 */
    MPC.PB1PFS.BIT.PSEL = 0xA;

    /* Configure pin 2 of port B as TXD0 */
    MPC.PB2PFS.BIT.PSEL = 0xA;

    /* Configure pin 4 of port 1 as SCK2 */
    MPC.P14PFS.BIT.PSEL = 0xA;

    /* Configure pin 3 of port 0 as RXD2 */
    MPC.P03PFS.BIT.PSEL = 0xA;

    /* Configure pin 2 of port 0 as TXD2 */
    MPC.P02PFS.BIT.PSEL = 0xA;

    /* Turn PFS register write protection back on */
    MPC.PWPR.BIT.PFSWE = 0x0;
    MPC.PWPR.BIT.B0WI = 0x1;

    /* Protection off */
    SYSTEM.PRCR.WORD = 0xA503;

    /* Cancel the SCI0 and SCI2 module clock stop modes */
    MSTP_SCI0 = 0x0;
    MSTP_SCI2 = 0x0;

    /* Protection on */
    SYSTEM.PRCR.WORD = 0xA500;

    /* Set SCI0 RXI0 interrupt priority level to 7 */
    IPR(SCI0, RXI0) = 0x7;

    /* Enable SCI0 RXI0 interrupts */
    IEN(SCI0, RXI0) = 0x1;

    /* Clear SCI0 RXI5 IR flag */
    IR(SCI0, RXI0) = 0x0;

    /* Set SCI2 RXI2 interrupt priority level to 7 */
    IPR(SCI2, RXI2) = 0x7;

    /* Enable SCI2 RXI2 interrupts */
    IEN(SCI2, RXI2) = 0x1;

    /* Clear SCI2 RXI2 IR flag */
    IR(SCI2, RXI2) = 0x0;

}
/*******************************************************************************
* End of function Init_Sync
*******************************************************************************/

/*******************************************************************************
* Function Name: SCI0toSCI2Transfer_Sync
* Description  : This function configures SCI0 and SCI2's peripheral pins, as
*                well as enabling reception interrupts. SCI0 is set as the
*                transmit channel, supplying the transfer clock, and SCI2 as
*                the receive channel. The function disables SCI0 reception and
*                SCI2 transmission and finally sends the contents of the SCI0
*                transmit buffer.
* Argument     : none
* Return value : none
*******************************************************************************/
void SCI0toSCI2Transfer_Sync (void)
{
    /* Declare a transmit count variable */
    uint8_t i = 0;

    /* Initialise SCI0 */
    /* Select on-chip baudrate generator and output clock on SCK0 pin */
    SCI0.SCR.BIT.CKE = 0x1;

    /* Enable serial interface mode */
    SCI0.SIMR1.BIT.IICM = 0x0;

    /* No clock phase delay used */
    SCI0.SPMR.BIT.CKPH = 0x0;

    /* Select clock source frequency as PCLK/16=3MHz */
    SCI0.SMR.BIT.CKS = 0x2;

    /* Select clock synchronous mode */
    SCI0.SMR.BIT.CM = 0x1;

    /* Select serial communication interface mode */
    SCI0.SCMR.BYTE = 0x72;

    /* Set the baudrate to BAUD_RATE1 bps */
    /* See Hardware manual Table 27.8 */
    /* N = ((PCLK)/(8 * 8 * BAUD_RATE1)) - 1 */
    SCI0.BRR = (uint8_t)((48000000u / (64u * BAUD_RATE1)) - 1);

    /* Enable SCI0 transmission */
    SCI0.SCR.BIT.TE = 0x1;

    /* Initialise SCI2 */
    /* Clock is to be supplied externally */
    SCI2.SCR.BIT.CKE = 0x2;

    /* Ensure to disable Simple IIC mode */
    SCI2.SIMR1.BIT.IICM = 0x0;

    /* No clock phase delay */
    SCI2.SPMR.BIT.CKPH = 0x0;

    /* Select clock source frequency as PCLK/16=3MHz */
    SCI2.SMR.BIT.CKS = 0x2;

    /* Select clock synchronous mode */
    SCI2.SMR.BIT.CM = 0x1;

    /* Select the serial communication interface mode */
    SCI2.SCMR.BYTE = 0x72;

    /* Enable SCI2 reception, as well as reception interrupts */
    SCI2.SCR.BIT.RE = 0x1;
    SCI2.SCR.BIT.RIE = 0x1;

    /* Transmit data string from SCI0 to SCI12 */
    for (i = 0; g_sci0_tx_buffer[i] != NULL; i++)
    {
        /* Write data to the transmit buffer */
        SCI0.TDR = g_sci0_tx_buffer[i];

        /* Ensure last transmission completed successfully */
        while (0x0 == SCI0.SSR.BIT.TEND)
        {
            /* Wait */
        }
    }
}
/*******************************************************************************
* End of function SCI0toSCI2Transfer_Sync
*******************************************************************************/

/*******************************************************************************
* Function Name: SCI2toSCI0Transfer_Sync
* Description  : This function disables SCI0 and SCI2 transmission and
*                reception. It then configures SCI2 as the transmit channel,
*                supplying the transfer clock, and SCI0 as the receive channel.
*                The function then enables SCI2 reception and SCI0
*                transmission. The contents of the SCI2 transmit buffer are
*                then sent to SCI0.
* Argument     : none
* Return value : none
*******************************************************************************/
void SCI2toSCI0Transfer_Sync (void)
{
    /* Declare a count variable */
    uint8_t i = 0;

    /* Disable SCI0 transmission and reception,
       as well as their respective interrupts */
    SCI0.SCR.BYTE &= 0x0F;

    /* Disable SCI2 transmission and reception,
       as well as their respective interrupts */
    SCI2.SCR.BYTE &= 0x0F;

    /* Select on-chip baudrate generator and output clock on SCK2 pin */
    SCI2.SCR.BIT.CKE = 0x1;

    /* Set the baudrate to BAUD_RATE2 bps */
    /* See Hardware manual Table 27.8 */
    /* N = ((PCLK)/(8 * 8 * BAUD_RATE2)) - 1 */
    SCI2.BRR = (uint8_t)((48000000u / (64u * BAUD_RATE2)) - 1);

    /* SCI0 clock is to be supplied externally */
    SCI0.SCR.BIT.CKE = 0x2;

    /* Enable SCI0 reception and receive interrupts */
    SCI0.SCR.BIT.RE = 0x1;
    SCI0.SCR.BIT.RIE = 0x1;

    /* Enable SCI12 transmissions */
    SCI2.SCR.BIT.TE = 0x1;

    /* Transmit data string from SCI12 to SCI0 */
    for (i = 0; g_sci2_tx_buffer[i] != NULL; i++)
    {
        /* Write data to the transmit buffer */
        SCI2.TDR = g_sci2_tx_buffer[i];

        /* Ensure last transmission completed succssfully */
        while (0x0 == SCI2.SSR.BIT.TEND)
        {
            /* Wait */
        }
    }
}
/*******************************************************************************
* End of function SCI2toSCI0Transfer_Sync
*******************************************************************************/

/*******************************************************************************
* Function Name: Transfer_End
* Description  : This function is executed after all transfers are completed
*                on both channels. The function compares the received data
*                against the sent data, and sets the g_test_pass flag
*                accordingly. The function then displays either "Success" or
*                "Failure" on the debug LCD, depending on the status of the
*                g_test_pass flag.
* Argument     : none
* Return value : none
*******************************************************************************/
void Transfer_End (void)
{
    /* Check if recevied SCI0 data matches sent SCI12 data */
    if (0 == strcmp((char *)g_sci0_rx_buffer, (char *)g_sci2_tx_buffer))
    {
        /* Sent and received data matches, set test pass flag to true
          (if already true)*/
        g_test_pass &= true;
    }
    else
    {
        /* Sent and received data does not match, set test pass flag to false */
        g_test_pass = false;
    }

    /* Check if received SCI2 data matches sent SCI0 data */
    if (0 == strcmp((char *)g_sci2_rx_buffer, (char *)g_sci0_tx_buffer))
    {
        /* Sent and received data matches, set test pass flag to true
           (if already true)*/
        g_test_pass &= true;
    }
    /* Sent and received data match */
    else
    {
        /* Sent and received data does not match, set test pass flag to false */
        g_test_pass = false;
    }

    /* Check the status of the test pass flag */
    if (true == g_test_pass)
    {
        /* Display success on debug LCD */
        Display_LCD(LCD_LINE2, "Success ");
    }
    /* Test pass flag false */
    else
    {
        /* Display failure on debug LCD */
        Display_LCD(LCD_LINE2, "Failure ");
    }
}
/*******************************************************************************
* End of function Transfer_End
*******************************************************************************/

/*******************************************************************************
* Function Name: Excep_SCI0_RXI0
* Description  : This interrupt handler is called on reception of new data
*                received by SCI0. The function stores the data in the
*                g_sci0_rx_buffer array location pointed by g_sci0_rx_counter;
*                which is incremented by 1.
* Argument     : none
* Return value : none
*******************************************************************************/
#pragma vector=VECT_SCI0_RXI0
__interrupt  void Excep_SCI0_RXI0(void)
{
    /* Read the received byte */
    g_sci0_rx_buffer[g_sci0_rx_counter++] = SCI0.RDR;
}
/*******************************************************************************
* End of function Excep_SCI0_RXI0
*******************************************************************************/

/*******************************************************************************
* Function Name: Excep_SCI12_RXI12
* Description  : This interrupt handler is called on reception of new data
*                received by SCI12. The function stores the data in the
*                g_sci12_rx_buffer array location pointed by g_sci12_rx_counter;
*                which is incremented by 1.
* Argument     : none
* Return value : none
*******************************************************************************/
#pragma vector=VECT_SCI2_RXI2
__interrupt  void Excep_SCI2_RXI2(void)
{
    /* Read the received byte */
    g_sci2_rx_buffer[g_sci2_rx_counter++] = SCI2.RDR;
}
/*******************************************************************************
* End of function Excep_SCI12_RXI12
*******************************************************************************/

