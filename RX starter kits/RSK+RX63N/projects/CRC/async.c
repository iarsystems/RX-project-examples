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
* Device        : R5F563NB
* Tool-Chain    : Renesas RX Standard 1.2.0.0
* H/W Platform  : RSK+RX63N
* Description   : Defines SCI functions used in this sample.
*******************************************************************************/
/*******************************************************************************
* History       : 13 Aug. 2012  Ver. 1.00 First Release
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
/* Defines RX63N port registers */
#include "iorx63n.h"
/* Defines RX63N LEDs and switches */
#include "rskrx63ndef.h"
/* Declares functions defined in this file */
#include "async.h"
/* Declares crc functions used in this file */
#include "crc.h"
/* Defines LCD functions used in this file */
#include "lcd.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Declare transmit and receive buffer variables used in this file */
uint8_t gSCI_TXBuffer[2]={0x2F, 0x0};
/* Declare a receive buffer variable */
volatile uint8_t gSCI_RXBuffer;
/* Declaration of the command string to clear the terminal screen */
const uint8_t cmd_clr_scr[] = {27, 91, 50, 74, 0, 27, 91, 72, 0};
/* Declaration of the command string to bring the cursor home on the terminal */
const uint8_t cmd_cur_home[] = {27, '[', 'H', 0};

/*******************************************************************************
* Outline      : Init_Async
* Description  : This function initialises the SCI channel connected to the 
*                 RS232 connector on the RSK. The channel is configured for 
*                 transmission and reception, and instructions are sent to the
*                 terminal.
* Argument     : none
* Return value : none
*******************************************************************************/
void SciInit_CRC(void)
{    
    /* Protection off */
    SYSTEM.PRCR.WORD = 0xA503u;
    
    /* Cancel the SCI0 module clock stop mode */
    MSTP_SCI0 = 0u;
    
    /* Protection on */
    SYSTEM.PRCR.WORD = 0xA500u;
    
    /* Configure SCI0's TXD0, RXD0 and SCK0 pins for peripheral functions */
    PORT2.PMR.BYTE = 0x07u;
    
    /* PWPR.PFSWE write protect off */
    MPC.PWPR.BYTE = 0x00u;    
    
    /* PFS register write protect off */
    MPC.PWPR.BYTE = 0x40u;    
    
    /* Configure SCI0's communication pins */
    /* Set Port2's pin 0 function to TXD0 */
    MPC.P20PFS.BYTE = 0x0Au; 
    
    /* Set Port2's pin 1 function to RXD0 */
    MPC.P21PFS.BYTE = 0x0Au; 
    
    /* Set Port2's pin 2 function to SCK0 */
    MPC.P22PFS.BYTE = 0x0Au; 
    
    /* Turn on PFS register write protect */
    MPC.PWPR.BYTE = 0x80u;    
    
    /* Cancel SCI0 module stop mode */
    SYSTEM.MSTPCRB.LONG = 0x7FFFFFFFu;
    
    /* Disable All bits and CKE0,1 = 00 */
    SCI0.SCR.BYTE = 0x00u;                                  

    /* Set output port TXD0(P20) */
    PORT2.PDR.BIT.B0 = 1u;
    
    /* Set Receive port RXD0(P21)*/
    PORT2.PDR.BIT.B1 = 1u;

    /* disable All bits and CKE0,1 = 00 once more */
    SCI0.SCR.BYTE = 0x00u;                                  

    /* CKS[0-1] = 00: PCLK 0 div        
       1Sop 8bits non-parity     */
    SCI0.SMR.BYTE = 0x00u;   
                                                              
    /* SMIF = 0: Serial  */
    SCI0.SCMR.BYTE = 0xf2u;                                   

    /* PCLK = 48MHz, Set baudrate to 19200 bps 
       77 = 48MHz / (64*0.5*19200)-1 */
    SCI0.BRR = 77u;                                     
                              
    /* Allow a small delay */
    for (volatile uint8_t i = 0u; i < 20u; i++ )
    {
    }                                 

    /* Clear bits ORER, PER and FER */
    SCI0.SSR.BYTE &= 0xC7u;                                      
    
    /* Set the SCI0 interrupt level as 4 */
    IPR(SCI0, ) = 0x07u;
    
    /* Enable Receive Interrupt */
    IEN(SCI0, RXI0) = 1u;
    
    /* Clear interrupt request */
    IR(SCI0, RXI0) = 0u;

    /* Set bits TIE, RIE, RE and TE. Enable receive interrupts */
    SCI0.SCR.BYTE = 0xF0u;  
    
    /* Clear the text on terminal window */
    SciTransmit_CRC(cmd_clr_scr, 9u);  
    SciTransmit_CRC(cmd_cur_home, 4u);    
    
    /* Send instructions to the terminal window */
    SciTransmit_CRC("\rRSKRX63N CRC Calculator Sample\r\n", 0);
    SciTransmit_CRC("------------------------------\r\n\n", 0);                       
    SciTransmit_CRC("Enter any printable character, and a corresponding ", 0);
    SciTransmit_CRC("CRC value will be generated.\r\n\n\n", 0);
}
/*******************************************************************************
* End of function SciInit_CRC
*******************************************************************************/

/******************************************************************************
* Outline       : SciTransmit_CRC
* Description   : Transmits a byte string over SCI. The string length is set by
*                 num_bytes. If num_bytes is set to 0, the function will keep
*                 sending bytes until a null character is found.
* Argument      : uint8_t* : Pointer to message string to be sent
*                 uint8_t  : Number of bytes to transmit
* Return value  : None
******************************************************************************/
void SciTransmit_CRC(const uint8_t* msg_string, const uint8_t num_bytes)
{     
    /* Check if num_bytes is greater than zero */
    if(num_bytes > 0u)
    {
        /* This loop reads the text string and puts it into the UART's 
           transmit buffer. Loop performs num_bytes iterations */
        for (uint8_t i = 0; i < num_bytes; i++)
        {        
            /* Wait until last transmission was completes */
            while(0u == (SCI0.SSR.BIT.TEND));
        
            /* Transmit the next character */
            SCI0.TDR = msg_string[i];
        }
    }
    /* Num bytes is zero */
    else
    {
           /* This loop reads the text string and puts it into the UART's 
           transmit buffer. Loop iterates until null character found. */
        for (uint8_t i = 0; msg_string[i]; i++)
        {        
            /* Wait until last transmission was completes */
            while(0u == (SCI0.SSR.BIT.TEND));
        
            /* Transmit the next character */
            SCI0.TDR = msg_string[i];
        }
    }
}
/******************************************************************************
* End of Function SciTransmit_CRC
******************************************************************************/

/*******************************************************************************
* Outline       : Excep_SCI0_RXI0
* Description   : SCI channel 0 recieve ISR. If a printable character is 
*                  recieved, the character is echoed back along with a checksum
*                  value (calculated using the CRC peripheral).
* Argument      : none
* Return value  : none
*******************************************************************************/
#pragma vector=VECT_SCI0_RXI0
__interrupt void Excep_SCI0_RXI0(void)
{    
    /* Disable recieve interrupt */
    IEN(SCI0, RXI0) = 0;
    
    /* Store the received byte */
    gSCI_RXBuffer = SCI0.RDR;
    
    /* Check if received byte is a printable ascii character */
    if((gSCI_RXBuffer > 0x1F) && (gSCI_RXBuffer < 0x7F))
    {
        /* Prepare transmit string */
        uint8_t tx_string[32] = "*, 0x0000 - CRC Value: 0x0000\r\0";
    
        /* Insert received character into tx_string */
        tx_string[0] = gSCI_RXBuffer;
    
        /* Calculate CRC value for the received character */
        uint16_t crc_val = Calculate_CRC((uint8_t)gSCI_RXBuffer);
    
        /* Convert recieved character ascii integer into string, and insert
           into tx_string */
        u16ToString(tx_string, 5u, (uint16_t)gSCI_RXBuffer);
    
        /* Convert CRC value integer into string, and insert into tx_string */
        u16ToString(tx_string, 25u, (uint16_t)crc_val);
    
        /* Transmit tx_string to the terminal */
        SciTransmit_CRC(tx_string, 0);
    }
    /* Recieved byte is not a printable ascii character */
    else
    {
        /* Transmit tx_string to the terminal */
        SciTransmit_CRC("Non-printable byte received                       \r",
                        0);
    }
    
    /* Re-enable recieve Interrupt */
    IEN(SCI0, RXI0) = 1;
}
/*******************************************************************************
* End of function Excep_SCI0_RXI0
*******************************************************************************/
