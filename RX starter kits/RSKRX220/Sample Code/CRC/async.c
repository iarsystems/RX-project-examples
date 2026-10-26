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

/* Declares crc functions used in this file */
#include "crc.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/

/* Declare transmit and receive buffer variables used in this file */
char g_sci_tx_buffer[2] = {0x2Fu, 0x0};

/* Declare a receive buffer variable */
volatile char gSCI_RXBuffer;

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
* Function Name: sci_init_crc
* Description  : This function initialises the SCI channel connected to the
*                RS232 connector on the RSK. The channel is configured for
*                transmission and reception, and instructions are sent to the
*                terminal.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void sci_init_crc (void)
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
    SCI1.BRR = 32;

    /* Declare a loop count variable */
    volatile uint16_t i = 0;

    /* Allow a small delay */
    for (i = 0; i < 20; i++)
    {
        /* Do nothing until delay has passed */
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
    sci_transmit_crc(cmd_clr_scr);
    sci_transmit_crc(cmd_cur_home);

    /* Send instructions to the terminal window */
    sci_transmit_crc("\rRSKRX220 CRC Calculator Sample\r\n");
    sci_transmit_crc("------------------------------\r\n\n");
    sci_transmit_crc("Enter any printable character, and a corresponding ");
    sci_transmit_crc("CRC value will be generated.\r\n\n\n");
}
/*******************************************************************************
* End of function sci_init_crc
*******************************************************************************/

/******************************************************************************
* Function Name: sci_transmit_crc
* Description  : Transmits null-terminated string.
* Arguments    : (char*) msg_string - null terminated string
* Return Value : None
******************************************************************************/
void sci_transmit_crc (const char * const msg_string)
{
    /* Declare a count variable */
    uint8_t  i = 0;

    /* Declare a count variable */
    volatile uint16_t j = 0;

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
        for (j = 0; j < 1000u; j++)
        {
            /* delay */
        }
    }
}
/******************************************************************************
* End of function sci_transmit_crc
******************************************************************************/

/*******************************************************************************
* Function Name: Excep_SCI1_RXI1_isr
* Description  : SCI channel 1 recieve ISR. If a printable character is
*                recieved, the character is echoed back along with a checksum
*                value (calculated using the CRC peripheral).
* Arguments    : none
* Return Value : none
*******************************************************************************/
#pragma vector=VECT_SCI1_RXI1
__interrupt  void Excep_SCI1_RXI1_isr(void)
{
    /* Disable recieve interrupt */
    IEN(SCI1, RXI1) = 0x0;

    /* Store the received byte */
    gSCI_RXBuffer = (char)SCI1.RDR;

    /* Check if received byte is a printable ascii character */
    if ((gSCI_RXBuffer > 0x1F) && (gSCI_RXBuffer < 0x7F))
    {
        /* Prepare transmit string */
        char tx_string[32] = "*, 0x0000 - CRC Value: 0x0000\r\0";

        /* Insert received character into tx_string */
        tx_string[0] = gSCI_RXBuffer;

        /* Calculate CRC value for the received character */
        const uint16_t crc_val = calculate_crc((uint16_t)gSCI_RXBuffer);

        /* Convert recieved character ascii integer into string, and insert
           into tx_string */
        uint16_to_string(tx_string, 5u, (uint16_t)gSCI_RXBuffer);

        /* Convert CRC value integer into string, and insert into tx_string */
        uint16_to_string(tx_string, 25u, (uint16_t)crc_val);

        /* Transmit tx_string to the terminal */
        sci_transmit_crc(tx_string);
    }

    /* Recieved byte is not a printable ascii character */
    else
    {
        /* Transmit tx_string to the terminal */
        sci_transmit_crc
            ("\r\nNon-printable byte recieved. Reset sample to continue\r\n");
    }

    /* Re-enable receive Interrupt */
    IEN(SCI1, RXI1) = 1;
}
/*******************************************************************************
* End of function Excep_SCI1_RXI1_isr
*******************************************************************************/

/*******************************************************************************
* Function Name: uint16_to_string
* Description  : Function converts a 16 bit integer into a character string,
*                inserts it into the array via the pointer passed at execution.
* Arguments    : (char *)output_string        - Pointer to char array that will
*                                               hold character string.
*                (uint8_t)pos                 - uint8_t number, element number
*                                               to begin inserting the character
*                                               string from (offset).
*                (const uint16_t)input_number - 16 bit integer to convert into
*                                               a string.
* Return Value : none
* Note         : No input validation is used, so output data can overflow the
*                array passed.
*******************************************************************************/
static void uint16_to_string (char * const output_string, uint8_t pos,
                                                    const uint16_t input_number)
{
    /* Declare temporary character storage variable, and bit_shift variable */
    char     a         = 0x00;
    uint8_t  bit_shift = 12u;

    /* Declare 16bit mask variable */
    uint16_t mask = 0xF000u;

    /* Loop through until each hex digit is converted to an ASCII character */
    while (bit_shift < 30u)
    {
        /* Mask and shift the hex digit, and store in temporary variable, a */
        a = (char)((input_number & mask) >> bit_shift);

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

