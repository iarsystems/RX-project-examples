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
* Device        : R5F563T6
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX63T
* Description   : Defines sync serial functions used in this sample.
*******************************************************************************/
/*******************************************************************************
* History       : 05.02.2013  Ver. 1.00 First Release
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

#include "r_cg_macrodriver.h"
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
/* Declaration of the SCI1 transmit buffer */
uint8_t g_sci1_tx_buffer[] = "SCI1 to SCI12 Loopback Test";

/* Declaration of the SCI12 transmit buffer */
uint8_t g_sci12_tx_buffer[] = "SCI12 to SCI1 Loopback Test";

/* Declaration of SCI1 receive buffer */
uint8_t g_sci1_rx_buffer[MSG_LEN];

/* Declaration of SCI12 receive buffer */
uint8_t g_sci12_rx_buffer[MSG_LEN];

/* Declaration of loopback test success flag */
volatile bool g_test_pass = true;

/* Declare a SCI1 data receive counter */
uint8_t g_sci1_rx_counter = 0;

/* Declare a SCI12 data receive counter */
uint8_t g_sci12_rx_counter = 0;

/*******************************************************************************
* Function Name: Init_Sync
* Description  : This function reserves and clears 28 bytes of memory for 
*                SCI0 and SCI12's receive buffers.
* Argument     : none
* Return value : none
*******************************************************************************/
void Init_Sync (void)
{
    /* Clear the SCI1 and SCI12 receive buffers */
    memset(g_sci1_rx_buffer, 0x00, MSG_LEN);
    memset(g_sci12_rx_buffer, 0x00, MSG_LEN);

    /* Configure RXD1, TXD1 and SCK1 as input, output and output */
    /* Set Receive port RXD1(P15) - port direction input */
    PORT1.PDR.BIT.B5 = 0x0;
    
    /* Set output port TXD1(P16) - port direction output */
    PORT1.PDR.BIT.B6 = 0x1;
    
    /* Set Receive port SCK1(P17) - port direction output */
    PORT1.PDR.BIT.B7 = 0x1;
    
    /* Configure RXD12, TXD12 and SCK12 as input, output and input */
    /* Set Receive port RXD12(PE2) - port direction input */
    PORTE.PDR.BIT.B2 = 0x0;
    
    /* Set output port TXD12(PE1) - port direction output */
    PORTE.PDR.BIT.B1 = 0x1;
    
    /* Set Receive port SCK12(PE0) - port direction input */
    PORTE.PDR.BIT.B0 = 0x0;
    
    /* Configure SCI0's TXD0, RXD0 and SCK0 pins for peripheral functions */
    PORT1.PMR.BIT.B5 = 0x1;
    PORT1.PMR.BIT.B6 = 0x1;
    PORT1.PMR.BIT.B7 = 0x1;
    
    /* Configure Port B pins 5, 6 and 7 for peripheral use */
    PORTE.PMR.BIT.B2 = 0x1;
    PORTE.PMR.BIT.B1 = 0x1;
    PORTE.PMR.BIT.B0 = 0x1;
    
    /* Allow writing to the PFSWE bit in the PWPR register */
    MPC.PWPR.BIT.B0WI = 0x0;
    
    /* Then turn off PFS register write protection */
    MPC.PWPR.BIT.PFSWE = 0x1;
    
    /* Configure pin 7 of port 1 as SCK1 */
    MPC.P17PFS.BIT.PSEL = 0xA;
    
    /* Configure pin 5 of port 1 as RXD1 */
    MPC.P15PFS.BIT.PSEL = 0xA;
    
    /* Configure pin 6 of port 1 as TXD1 */
    MPC.P16PFS.BIT.PSEL = 0xA;
    
    /* Configure pin 0 of port E as SCK12 */
    MPC.PE0PFS.BIT.PSEL = 0xC;
    
    /* Configure pin 2 of port E as MISO */
    MPC.PE2PFS.BIT.PSEL = 0xC;
    
    /* Configure pin 1 of port E as MOSI */
    MPC.PE1PFS.BIT.PSEL = 0xC;

    /* Turn PFS register write protection back on */
    MPC.PWPR.BIT.PFSWE = 0x0;
    MPC.PWPR.BIT.B0WI = 0x1;
    
    /* Protection off */
    SYSTEM.PRCR.WORD = 0xA503;
    
    /* Cancel the SCI0 and SCI12 module clock stop modes */
    MSTP_SCI1 = 0x0;
    MSTP_SCI12 = 0x0; 
          
    /* Protection on */
    SYSTEM.PRCR.WORD = 0xA500;

    /* Set SCI1 RXI1 interrupt priority level to 7 */
    IPR(SCI1, RXI1) = 0x7;
    
    /* Enable SCI1 RXI1 interrupts */
    IEN(SCI1, RXI1) = 0x1;
    
    /* Clear SCI0 RXI5 IR flag */
    IR(SCI1, RXI1) = 0x0;
    
    /* Set SCI12 RXI12 interrupt priority level to 7 */
    IPR(SCI12, RXI12) = 0x7;
    
    /* Enable SCI12 RXI12 interrupts */
    IEN(SCI12, RXI12) = 0x1;
    
    /* Clear SCI12 RXI12 IR flag */
    IR(SCI12, RXI12) = 0x0;

}
/*******************************************************************************
* End of function Init_Sync
*******************************************************************************/

/*******************************************************************************
* Function Name: SCI1toSCI12Transfer_Sync
* Description  : This function configures SCI0 and SCI12's peripheral pins, as 
*                well as enabling reception interrupts. SCI0 is set as the 
*                transmit channel, supplying the transfer clock, and SCI12 as 
*                the receive channel. The function disables SCI0 reception and 
*                SCI12 transmission and finally sends the contents of the SCI0 
*                transmit buffer.
* Argument     : none
* Return value : none
*******************************************************************************/
void SCI1toSCI12Transfer_Sync (void)
{   
    /* Declare a transmit count variable */
    uint8_t i = 0;
    
    /* Initialise SCI0 */
    /* Select on-chip baudrate generator and output clock on SCK0 pin */
    SCI1.SCR.BIT.CKE = 0x1;
    
    /* Enable serial interface mode */
    SCI1.SIMR1.BIT.IICM = 0x0;
    
    /* No clock phase delay used */
    SCI1.SPMR.BIT.CKPH = 0x0;
    
    /* Select clock source frequency as PCLK/16=3MHz */
    SCI1.SMR.BIT.CKS = 0x2;
    
    /* Select clock synchronous mode */
    SCI1.SMR.BIT.CM = 0x1;
    
    /* Select serial communication interface mode */
    SCI1.SCMR.BYTE = 0x72;
    
    /* Set the baudrate to BAUD_RATE1 bps */
    /* See Hardware manual Table 27.8 */
    /* N = ((PCLK)/(8 * 8 * BAUD_RATE1)) - 1 */
    SCI1.BRR = (uint8_t)((48000000u / (64u * BAUD_RATE1)) - 1);
    
    /* Enable SCI1 transmission */
    SCI1.SCR.BIT.TE = 0x1;
    
    /* Initialise SCI12 */
    /* Clock is to be supplied externally */
    SCI12.SCR.BIT.CKE = 0x2;
    
    /* Ensure to disable Simple IIC mode */
    SCI12.SIMR1.BIT.IICM = 0x0;
    
    /* No clock phase delay */
    SCI12.SPMR.BIT.CKPH = 0x0;
    
    /* Select clock source frequency as PCLK/16=3MHz */
    SCI12.SMR.BIT.CKS = 0x2;
    
    /* Select clock synchronous mode */
    SCI12.SMR.BIT.CM = 0x1;
    
    /* Select the serial communication interface mode */
    SCI12.SCMR.BYTE = 0x72;
    
    /* Enable SCI12 reception, as well as reception interrupts */
    SCI12.SCR.BIT.RE = 0x1; 
    SCI12.SCR.BIT.RIE = 0x1; 

    /* Transmit data string from SCI1 to SCI12 */           
    for (i = 0; g_sci1_tx_buffer[i] != NULL; i++)
    {
        /* Write data to the transmit buffer */
        SCI1.TDR = g_sci1_tx_buffer[i];
        
        /* Ensure last transmission completed succssfully */
        while (0x0 == SCI1.SSR.BIT.TEND)
        {
            /* Wait */
        }
    }
}
/*******************************************************************************
* End of function SCI1toSCI12Transfer_Sync
*******************************************************************************/

/*******************************************************************************
* Function Name: SCI12toSCI1Transfer_Sync
* Description  : This function disables SCI1 and SCI12 transmission and 
*                reception. It then configures SCI12 as the transmit channel, 
*                supplying the transfer clock, and SCI1 as the receive channel. 
*                The function then enables SCI12 reception and SCI1 
*                transmission. The contents of the SCI12 transmit buffer are 
*                then sent to SCI1.
* Argument     : none
* Return value : none
*******************************************************************************/
void SCI12toSCI1Transfer_Sync (void)
{   
    /* Declare a count variable */
    uint8_t i = 0;
        
    /* Disable SCI1 transmission and reception, 
       as well as their respective interrupts */
    SCI1.SCR.BYTE &= 0x0F;
    
    /* Disable SCI12 transmission and reception, 
       as well as their respective interrupts */
    SCI12.SCR.BYTE &= 0x0F;
    
    /* Select on-chip baudrate generator and output clock on SCK12 pin */
    SCI12.SCR.BIT.CKE = 0x1;
    
    /* Set the baudrate to BAUD_RATE2 bps */
    /* See Hardware manual Table 27.8 */
    /* N = ((PCLK)/(8 * 8 * BAUD_RATE2)) - 1 */
    SCI12.BRR = (uint8_t)((48000000u / (64u * BAUD_RATE2)) - 1);
    
    /* SCI1 clock is to be supplied externally */
    SCI1.SCR.BIT.CKE = 0x2;
    
    /* Enable SCI1 reception and receive interrupts */
    SCI1.SCR.BIT.RE = 0x1;
    SCI1.SCR.BIT.RIE = 0x1;
    
    /* Enable SCI12 transmissions */
    SCI12.SCR.BIT.TE = 0x1;
            
    /* Transmit data string from SCI12 to SCI1 */           
    for (i = 0; g_sci12_tx_buffer[i] != NULL; i++)
    {
        /* Write data to the transmit buffer */
        SCI12.TDR = g_sci12_tx_buffer[i];
        
        /* Ensure last transmission completed succssfully */
        while (0x0 == SCI12.SSR.BIT.TEND)
        {
            /* Wait */
        }
    }
}
/*******************************************************************************
* End of function SCI12toSCI1Transfer_Sync
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
    /* Check if recevied SCI1 data matches sent SCI12 data */
    if (0 == strcmp((char *)g_sci1_rx_buffer, (char *)g_sci12_tx_buffer))
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
    
    /* Check if received SCI12 data matches sent SCI1 data */
    if (0 == strcmp((char *)g_sci12_rx_buffer, (char *)g_sci1_tx_buffer))
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
    
}
/*******************************************************************************
* End of function Transfer_End
*******************************************************************************/

/*******************************************************************************
* Function Name: Excep_SCI1_RXI0
* Description  : This interrupt handler is called on reception of new data 
*                received by SCI1. The function stores the data in the 
*                g_sci1_rx_buffer array location pointed by g_sci1_rx_counter; 
*                which is incremented by 1.
* Argument     : none
* Return value : none
*******************************************************************************/
#pragma vector=VECT_SCI1_RXI1
__interrupt void Excep_SCI1_RXI1 (void)
{
    if (g_sci1_rx_counter < MSG_LEN)
    {
        /* Read the received byte */
        g_sci1_rx_buffer[g_sci1_rx_counter++] = SCI1.RDR;
    }
}
/*******************************************************************************
* End of function Excep_SCI1_RXI0
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
#pragma vector=VECT_SCI12_RXI12
__interrupt void Excep_SCI12_RXI12 (void)
{
    if (g_sci12_rx_counter < MSG_LEN)
    {
        /* Read the received byte */
        g_sci12_rx_buffer[g_sci12_rx_counter++] = SCI12.RDR;
    }
}
/*******************************************************************************
* End of function Excep_SCI12_RXI12
*******************************************************************************/

