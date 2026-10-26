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
* http://www.renesas.com/disclaimer *
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.    
*******************************************************************************/ 
/******************************************************************************
* File Name     : crc.c
* Version       : 1.1
* Device(s)     : R5F562N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.1
* OS            : None
* H/W Platform  : YRDKRX62N
* Description   : Defines CRC functions used in this demo.
* Limitations   : None
*******************************************************************************
* History : DD.MM.YYYY     Description
*         : 14.09.2011     First release
*           20.01.2012     revised to CS
*******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdint.h>
#include <iorx63n.h>
#include "crc.h"
#include "uart.h"
#include "yrdkrx63n.h"


/*******************************************************************************
Local Function Prototypes
*******************************************************************************/
/* CRC checksum calculate function prototype delcaration */
uint16_t calculate_crc(uint8_t);

/* 16 bit integer to character string function prototype declaration */
static void uint16_to_string(uint8_t *, uint8_t, uint16_t);


/*******************************************************************************
* Function NAme: crc_init
* Description  : This function configures the CRC unit to perform ANSI
*                standard 16bit checksum calculations. 
* Argument     : none
* Return value : none
*******************************************************************************/
void crc_init(void)
{
  SYSTEM.PRCR.WORD = 0xA50B; /* Protect off */

  /* Cancel CRC module stop state */
  MSTP(CRC) = 0;  /* Expands to:  SYSTEM.MSTPCRB.BIT.MSTPB23 = 0; */
  
  SYSTEM.PRCR.WORD = 0xA500; /* Protect on  */

    
  /* Select RC generating polynomial */
  CRC.CRCCR.BIT.GPS = 0x2;    /* X^16 + X^15 + X^2 + 1  ANSI CRC-16 */
  //CRC.CRCCR.BIT.GPS = 0x3;    /* X^16 + X^12 + X^5 + 1  CCITT-16 */
  
  //CRC.CRCCR.BIT.LMS = 0;      /* LSB first */
  CRC.CRCCR.BIT.LMS = 1;      /* MSB first */   
  CRC.CRCCR.BIT.DORCLR = 1;   /* Clear the CRCDOR register. */
} /* End of function crc_init() */


/*******************************************************************************
* Function NAme: calculate_crc_initcrc
* Description  : Function takes an 8 bit input, writes it to the CRC unit,
*                fetches the CRC result and returns the value. The CRC checksum
*                method used will depend on how the CRC unit is configured.
*                Note: CRC is cleared after each call.
* Argument     : input_char  - 
*                   The 8-bit  input character.
* Return value : output_char - 
*                   The 16-bit checksum output.
*******************************************************************************/
uint16_t calculate_crc(uint8_t input_char)
{
  uint16_t crc_result  = 0;
  
  CRC.CRCDIR           = input_char; /* Write the char to the CRC data input  register. */
  crc_result           = CRC.CRCDOR; /* Get the result from the CRC data output register. */
  CRC.CRCCR.BIT.DORCLR = 1;          /* Clear the CRCDOR register. */
  return crc_result;
} /* End of function calculate_crc() */


/*******************************************************************************
* Function NAme: crc_demo
* Description  : When a character is received from the terminal, it is passed to
*                calculate_crc function to fetch the checksum for that character.
*                The checksum is then converted into a hexadecimal character 
*                string and both the original character and the checksum are 
*                displayed on the terminal. Serial UART driver must already be 
*                initialized. 
* Argument     : none
* Return value : none
*******************************************************************************/
void crc_demo(void)
{
  uint16_t checksum = 0x0000; 
  uint8_t  received_char; 
  
  /* Declare display string templates */
  uint8_t tx_blanks[] = "\r                                                 ";      
  uint8_t tx_buffer[] = "\r Character: X (0xYYYY) - Checksum: 0xZZZZ\0";  
  
  while(!sci_read_count_get())       /* wait for a received character */
  {
  }  
  
  received_char = sci_get_char();   /* Get the char waiting in buffer. */
  
  /* Replace X in the transmit buffer string with the received character */
  tx_buffer[13] = received_char;
  
  /* Convert the received character into a hex number, and insert into 
  transmit buffer string to replace YYYY */
  uint16_to_string(tx_buffer, 18, (uint16_t)received_char);    
  
  checksum = calculate_crc(received_char);
  
  /* Convert the checksum into a hex number, and insert into transmit
  buffer string to replace YYYY */
  uint16_to_string(tx_buffer, 38, checksum);
  
  sci_put_str(tx_blanks); /* Send blank line to display */
  sci_put_str(tx_buffer); /* Send the formatted CRC result string to display */
} /* End of function crc_demo() */


/*******************************************************************************
* Function NAme: uint16_to_string
* Description  : Function converts a 16 bit integer into a character string, 
*                 inserts it into the array via the pointer passed at execution.
* Argument     : * output_string - 
*                      Pointer to uint8_t array that will hold character string.                            
*                pos - 
*                      Output string location.
*                number -
*                      element number to begin inserting the character string 
*                      from (offset).
*                input_value -
*                      16 bit integer to convert into a string.
* Return value : none
*
* Note           : No input validation is used, so output data can overflow the
*                 array passed.
*******************************************************************************/
static void uint16_to_string(uint8_t * output_string, uint8_t pos, uint16_t input_value)
{
  uint8_t  a_char    = 0x00; 
  uint8_t  bit_shift = 12u;
  uint16_t mask      = 0xF000;
  
  /* Loop through until each hex digit is converted to an ASCII character */
  while (bit_shift < 30u)
  {
    /* Mask and shift the hex digit, and store in temporary variable, a */ 
    a_char = (input_value & mask) >> bit_shift;
    
    /* Convert the hex digit into an ASCII character, and store in output
    string */
    output_string[pos] = (a_char < 10) ? (a_char + '0') : (a_char - 10 + 'A');
    
    /* Shift the bit mask 4 bits to the right, to convert the next digit */
    mask = mask >> 4u;
    
    /* Decrement the bit_shift counter by 4 (bits in a each digit) */
    bit_shift -= 4u;
    
    /* Increment the output string location */
    pos++;
  }
} /* End of function uint16_to_string() */


/* end crc.c */
