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
* File Name     : async.c
* Version       : 1.00
* Device        : R5F563TE
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX63T144
* Description   : Defines ADC functions used for 12bit S12AD unit.
*******************************************************************************/
/*******************************************************************************
* History       : 05.12.2013  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
System Includes
*******************************************************************************/
/* Defines standard boolean variable types used in this file */
#include <stdbool.h>

/* Following header file provides standard integer type definitions. */
#include <stdint.h>

/* Following header file provides string type definitions. */
#include <string.h>

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/
/* Defines R5F563TE port registers */
#include <iorx63t.h>

/* Declares functions defined in this file */
#include "async.h"

/* Declares crc functions used in this file */
#include "crc.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Declare transmit and receive buffer variables used in this file */
char g_sci_tx_buffer[2] = {0x2F, 0x0};

/* Declare a receive buffer variable */
volatile char g_sci_rx_buffer;

/* Declaration of the command string to clear the terminal screen */
const char cmd_clr_scr[] = {27, 91, 50, 74, 0, 27, 91, 72, 0};

/* Declaration of the command string to bring the cursor home on the terminal */
const char cmd_cur_home[] = {27,'[','H',0};

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
/* 16bit integer to string function prototype */
static void uint16_to_string (char * const output_string, uint8_t pos,
                                const uint16_t input_number);

/*******************************************************************************
* Function Name: SciInit_CRC
* Description  : This function initialises the SCI channel connected to the
*                RS232 connector on the RSK. The channel is configured for
*                transmission and reception, and instructions are sent to the
*                terminal.
* Argument     : none
* Return value : none
*******************************************************************************/
void SciInit_CRC (void)
{
    /* Count variable */
    volatile uint8_t i;

    /* Protection off */
    SYSTEM.PRCR.WORD = 0xA503;

    /* Cancel the SCI1 module clock stop mode */
    MSTP_SCI1 = 0;

    /* Protection on */
    SYSTEM.PRCR.WORD = 0xA500;

    /* Configure SCI1's TXD0, RXD0 and SCK1 pins for peripheral functions */
    PORTD.PMR.BIT.B3 = 1;
    PORTD.PMR.BIT.B4 = 1;
    PORTD.PMR.BIT.B5 = 1;

    /* PWPR.PFSWE write protect off */
    MPC.PWPR.BYTE = 0x00;

    /* PFS register write protect off */
    MPC.PWPR.BYTE = 0x40;

    /* Configure SCI0's communication pins */
    /* Set PortD's pin 4 function to SCK1 */
    MPC.PD4PFS.BYTE = 0x0A;

    /* Set PortD's pin 5 function to RXD1 */
    MPC.PD5PFS.BYTE = 0x0A;

    /* Set PortD's pin 3 function to TXD1 */
    MPC.PD3PFS.BYTE = 0x0A;

    /* Turn on PFS register write protect */
    MPC.PWPR.BYTE = 0x80;

    /* Disable All bits and CKE0,1 = 00 */
    SCI1.SCR.BYTE = 0x00;

    /* Set output port TXD1(PD3) - port direction output */
    PORTD.PDR.BIT.B3 = 1;

    /* Set Receive port RXD1(PD5) - port direction input*/
    PORTD.PDR.BIT.B5 = 0;

    /* disable All bits and CKE0,1 = 00 once more */
    SCI1.SCR.BYTE = 0x00;

    /* CKS[0-1] = 00: PCLK 0 div
       1Sop 8bits non-parity     */
    SCI1.SMR.BYTE = 0x00;

    /* SMIF = 0: Serial  */
    SCI1.SCMR.BYTE = 0xf2;

    /* PCLK = 48MHz, Set baudrate to 19200 bps
       77 = 48MHz / (64*0.5*19200)-1 */
    SCI1.BRR = 77;

    /* Allow a small delay */
    for (i = 0; i < 20; i++)
    {
        /* delay */
    }

    /* Clear bits ORER, PER and FER */
    SCI1.SSR.BYTE &= 0xC7u;

    /* Set SCI0 RXI0 interrupt priority level to 7 */
    IPR(SCI1,RXI1) = 0x7;

    /* Enable SCI0 RXI0 interrupts */
    IEN(SCI1,RXI1) = 0x1;

    /* Clear SCI0 RXI0 interrupt flag */
    IR(SCI1,RXI1) = 0x0;

    /* Set bits TIE, RIE, RE and TE. Enable receive interrupts */
    SCI1.SCR.BYTE = 0xF0;

    /* Clear the text on terminal window */
    SciTransmit_CRC(cmd_clr_scr);
    SciTransmit_CRC(cmd_cur_home);
}
/*******************************************************************************
* End of function SciInit_CRC
*******************************************************************************/

/******************************************************************************
* Function Name : SciTransmit_CRC
* Description   : Transmits null-terminated string.
* Argument      : (char*) : Pointer to message string to be sent
* Return value  : None
******************************************************************************/
void SciTransmit_CRC (const char * const msg_string)
{
    /* Declare a count variable */
    uint8_t             i = 0;

    /* Declare a count variable */
    volatile uint16_t   j = 0;

    /* This loop reads the text string and puts it into the UART's
        transmit buffer */
    for (i = 0; msg_string[i]; i++)
    {
        /* Check that the last transmission was completed */
        while (0x0 == SCI1.SSR.BIT.TEND)
        {
            /* wait for tx complete */
        }

        /* Transmit the next character */
        SCI1.TDR = (uint8_t)msg_string[i];

        /* Count up to 1000 */
        for (j = 0; j < 1000; j++)
        {
            /* delay */
        }
    }
}
/******************************************************************************
* End of function SciTransmit_CRC
******************************************************************************/

/*******************************************************************************
* Function Name : Excep_SCI0_RXI0
* Description   : SCI channel 0 recieve ISR. If a printable character is
*                 recieved, the character is echoed back along with a checksum
*                 value (calculated using the CRC peripheral).
* Argument      : none
* Return value  : none
*******************************************************************************/
#pragma vector=VECT_SCI1_RXI1
__interrupt  void Excep_SCI1_RXI1(void)
{
    /* Disable recieve interrupt */
    IEN(SCI1, RXI1) = 0;

    /* Store the received character */
    /* cast required as register is defined as uint8_t */
    g_sci_rx_buffer = (char)SCI1.RDR;

    /* Check if received byte is a printable ascii character */
    if ((g_sci_rx_buffer > 0x1F) && (g_sci_rx_buffer < 0x7F))
    {
        /* Prepare transmit string */
        char            tx_string[32] = "*, 0x0000 - CRC Value: 0x0000\r\0";

        /* Insert received character into tx_string */
        tx_string[0] = g_sci_rx_buffer;

        /* Calculate CRC value for the received character */
        const uint16_t  crc_val       = Calculate_CRC((uint8_t)g_sci_rx_buffer);

        /* Convert recieved character ascii integer into string, and insert
            into tx_string */
        uint16_to_string(tx_string, 5u, (uint16_t)g_sci_rx_buffer);

        /* Convert CRC value integer into string, and insert into tx_string */
        uint16_to_string(tx_string, 25u, crc_val);

        /* Transmit tx_string to the terminal */
        SciTransmit_CRC(tx_string);
    }
    else
    {
        /* Recieved byte is not a printable ascii character */
        /* Transmit tx_string to the terminal */
        SciTransmit_CRC("Non-printable byte recieved                       \r");
    }

    /* Re-enable recieve Interrupt */
    IEN(SCI1, RXI1) = 1;
}
/*******************************************************************************
* End of function Excep_SCI0_RXI0
*******************************************************************************/

/*******************************************************************************
* Function Name : uint16_to_string
* Description   : Function converts a 16 bit integer into a character string,
*                 inserts it into the array via the pointer passed at execution.
* Argument      : * output_string : Pointer to uint8_t array that will hold
*                 character string.
*                 pos : uint8_t number, element number to begin
*                 inserting the character string from (offset).
*                 input_number : 16 bit integer to convert into a string.
* Return value  : none
* Note          : No input validation is used, so output data can overflow the
*                 array passed.
*******************************************************************************/
static void uint16_to_string (char * const output_string, uint8_t pos,
                                const uint16_t input_number)
{
    /* Declare temporary character storage variable, and bit_shift variable */
    uint8_t     a           = 0x00;
    uint8_t     bit_shift   = 12u;

    /* Declare 16bit mask variable */
    uint16_t    mask        = 0xF000;

    /* Loop through until each hex digit is converted to an ASCII character */
    while (bit_shift < 30u)
    {
        /* Mask and shift the hex digit, and store in temporary variable, a */
        a = (uint8_t)((input_number & mask) >> bit_shift);

        /* Convert the hex digit into an ASCII character, and store in output
            string */
        output_string[pos] = (char)((a < 0x0A) ? (a + 0x30) : (a + 0x37));

        /* Shift the bit mask 4 bits to the right, to convert the next digit */
        mask = (uint16_t) (mask >> 4u);

        /* Decrement the bit_shift counter by 4 (bits in a each digit) */
        bit_shift -= 4u;

        /* Increment the output string location */
        pos++;
    }
}
/*******************************************************************************
* End of function uint16_to_string
*******************************************************************************/

