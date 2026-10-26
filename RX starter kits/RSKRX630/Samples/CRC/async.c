/* Adapted for use with IAR Embedded Workbench */
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
* File Name    : async.c
* Version     : 1.00
* Device     : R5F5630E
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX630
* Description   : Defines SCI functions used in this sample.
*******************************************************************************/
/*******************************************************************************
* History     : 23 Jan. 2012  Ver. 1.00 First Release
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
/* Defines RX630 port registers */
#include "iorx630.h"
/* Defines RX630 LEDs and switches */
#include "rskrx630def.h"
/* Declares functions defined in this file */
#include "async.h"
/* Declares crc functions used in this file */
#include "crc.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Declare transmit and receive buffer variables used in this file */
uint8_t gSCI_TXBuffer[2]={0x2F, 0x0};
/* Declare a receive buffer variable */
volatile uint8_t gSCI_RXBuffer;
/* Declaration of the command string to clear the terminal screen */
uint8_t cmd_clr_scr[] = {27, 91, 50, 74, 0, 27, 91, 72, 0};
/* Declaration of the command string to bring the cursor home on the terminal */
uint8_t cmd_cur_home[] = {27,'[','H',0};

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
/* 16bit integer to string function prototype */
static void uint16_ToString(uint8_t *, uint8_t, uint16_t);

/*******************************************************************************
* Outline      : Init_Async
* Description  : This function initialises the SCI channel connected to the 
*         RS232 connector on the RSK. The channel is configured for 
*         transmission and reception, and instructions are sent to the
*         terminal.
* Argument     : none
* Return value : none
*******************************************************************************/
void SciInit_CRC(void)
{  
  /* Protection off */
  SYSTEM.PRCR.WORD = 0xA503;
  
  /* Cancel the SCI0 module clock stop mode */
  MSTP_SCI0 = 0;
  
  /* Protection on */
  SYSTEM.PRCR.WORD = 0xA500;
  
  /* Configure SCI0's TXD0, RXD0 and SCK0 pins for peripheral functions */
  PORT2.PMR.BYTE = 0x07;
  
  /* PWPR.PFSWE write protect off */
  MPC.PWPR.BYTE = 0x00;  
  
  /* PFS register write protect off */
  MPC.PWPR.BYTE = 0x40;  
  
  /* Configure SCI0's communication pins */
  /* Set Port2's pin 0 function to TXD0 */
  MPC.P20PFS.BYTE = 0x0A; 
  
  /* Set Port2's pin 1 function to RXD0 */
  MPC.P21PFS.BYTE = 0x0A; 
  
  /* Set Port2's pin 2 function to SCK0 */
  MPC.P22PFS.BYTE = 0x0A; 
  
  /* Turn on PFS register write protect */
  MPC.PWPR.BYTE = 0x80;  
  
  /* Cancel SCI0 module stop mode */
  SYSTEM.MSTPCRB.LONG = 0x7FFFFFFF;
  
  /* Disable All bits and CKE0,1 = 00 */
    SCI0.SCR.BYTE = 0x00;                                  

  /* Set output port TXD0(P20) */
  PORT2.PDR.BIT.B0 = 1;
  
  /* Set Receive port RXD0(P21)*/
  PORT2.PDR.BIT.B1 = 1;

  /* disable All bits and CKE0,1 = 00 once more */
    SCI0.SCR.BYTE = 0x00;                                  

  /* CKS[0-1] = 00: PCLK 0 div        
       1Sop 8bits non-parity     */
    SCI0.SMR.BYTE = 0x00;   
                                                            
    /* SMIF = 0: Serial  */
    SCI0.SCMR.BYTE = 0xf2;                                   

    /* PCLK = 48MHz, Set baudrate to 19200 bps 
       77 = 48MHz / (64*0.5*19200)-1 */
    SCI0.BRR = 77;                                     
                              
  /* Allow a small delay */
    for (uint8_t i = 0; i < 20; i++ )
  {
  }                               

  /* Clear bits ORER, PER and FER */
    SCI0.SSR.BYTE &= 0xC7;                                    
  
  /* Set the SCI0 interrupt level as 4 */
  IPR(SCI0, ) = 0x07;
  
  /* Enable Receive Interrupt */
  IEN(SCI0, RXI0) = 1;
  
  /* Clear interrupt request */
  IR(SCI0, RXI0) = 0;

  /* Set bits TIE, RIE, RE and TE. Enable receive interrupts */
    SCI0.SCR.BYTE = 0xF0;  
  
  /* Clear the text on terminal window */
  SciTransmit_CRC(cmd_clr_scr);  
  SciTransmit_CRC(cmd_cur_home);    
  
  /* Send instructions to the terminal window */
  SciTransmit_CRC("\rRSKRX630 CRC Calculator Sample\r\n");
  SciTransmit_CRC("------------------------------\r\n\n");                            
  SciTransmit_CRC("Enter any printable character, and a corresponding ");
  SciTransmit_CRC("CRC value will be generated.\r\n\n\n");
}
/*******************************************************************************
* End of function SciInit_CRC
*******************************************************************************/

/******************************************************************************
* Outline    : SciTransmit_CRC
* Description  : Transmits null-terminated string.
* Argument    : uint8_t* : Pointer to message string to be sent
* Return value  : None
******************************************************************************/
void SciTransmit_CRC(uint8_t* msg_string)
{
  /* Declare a count variable */
  uint8_t i = 0;
  
  /* Declare a count variable */
  uint16_t j = 0;

  /* This loop reads the text string and puts it into the UART's 
     transmit buffer */
  for (i=0; msg_string[i]; i++)
  {    
    /* Check that the last transmission was completed */
    while(!(SCI0.SSR.BYTE & 0x80));
    
    /* Transmit the next character */
    SCI0.TDR = msg_string[i];
    
    /* Count up to 1000 */
    for (j = 0; j < 1000; j++)
    {
      /* delay */
    }
  }
}
/******************************************************************************
* End of Function SciTransmit_CRC
******************************************************************************/

/*******************************************************************************
* Outline     : Excep_SCI0_RXI0
* Description   : SCI channel 0 recieve ISR. If a printable character is 
*          recieved, the character is echoed back along with a checksum
*          value (calculated using the CRC peripheral).
* Argument    : none
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
    uint16_t crc_val = Calculate_CRC((uint16_t)gSCI_RXBuffer);
  
    /* Convert recieved character ascii integer into string, and insert
       into tx_string */
    uint16_ToString(tx_string, 5u, (uint16_t)gSCI_RXBuffer);
  
    /* Convert CRC value integer into string, and insert into tx_string */
    uint16_ToString(tx_string, 25u, (uint16_t)crc_val);
  
    /* Transmit tx_string to the terminal */
    SciTransmit_CRC(tx_string);
  }
  /* Recieved byte is not a printable ascii character */
  else
  {
    /* Transmit tx_string to the terminal */
    SciTransmit_CRC("Non-printable byte recieved                       \r");
  }
  
  /* Re-enable recieve Interrupt */
  IEN(SCI0, RXI0) = 1;
}
/*******************************************************************************
* End of function Excep_SCI0_RXI0
*******************************************************************************/

/*******************************************************************************
* Outline      : uint16_ToString
* Description  : Function converts a 16 bit integer into a character string, 
*         inserts it into the array via the pointer passed at execution.
* Argument     : * output_string : Pointer to uint8_t array that will hold 
*                   character string.
*               pos : uint8_t number, element number to begin 
*                   inserting the character string from (offset).
*          input_number : 16 bit integer to convert into a string.
* Return value : none
* Note       : No input validation is used, so output data can overflow the
*         array passed.
*******************************************************************************/
static void uint16_ToString(uint8_t *output_string, uint8_t pos, 
      uint16_t input_number)
{
  /* Declare temporary character storage variable, and bit_shift variable */  
  uint8_t a = 0x00, bit_shift = 12u;
  
  /* Declare 16bit mask variable */
  uint16_t mask = 0xF000;
  
  /* Loop through until each hex digit is converted to an ASCII character */
  while(bit_shift < 30u)
  {
    /* Mask and shift the hex digit, and store in temporary variable, a */ 
    a = (uint8_t)((input_number & mask) >> bit_shift);
    
    /* Convert the hex digit into an ASCII character, and store in output
       string */
    output_string[pos] = (uint8_t)((a < 0x0A) ? (a + 0x30) : (a + 0x37));
    
    /* Shift the bit mask 4 bits to the right, to convert the next digit */
    mask = (uint16_t) (mask >> 4u);
    
    /* Decrement the bit_shift counter by 4 (bits in a each digit) */
    bit_shift -= 4u;
    
    /* Increment the output string location */
    pos++;
  }
}
/*******************************************************************************
* End of function uint16_t_ToString
*******************************************************************************/
