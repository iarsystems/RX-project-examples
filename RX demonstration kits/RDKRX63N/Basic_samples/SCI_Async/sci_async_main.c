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
/*******************************************************************************
* File Name     : sci_async_main.c
* Version       : 1.0
* Device(s)     : R5F563N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.0
* OS            : None
* H/W Platform  : YRDKRX63N
* Description   : This is the main sample code. Application demonstrates use of 
*               : the UART to send and receive data over the RS232 interface.
*
* Operation     : 1. Compile and download the sample code. 
*               : 2. Connect RS232 port to a PC or serial data terminal. 
*               :    Confirm terminal port settings: 
*               :      115200 baud, 8-bits, No parity, 1 stop bit, no handshake. 
*               : 3. Click 'Go' to start the software. 
*               : 4. Observe message print to terminal
*               : 5. Enter text on terminal and observe characters echoed back.
*                    LED4 will toggle for each key stroke.
*
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY     Version     Description
*         : 22.09.2011     1.00        First release
*******************************************************************************/

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <intrinsics.h>
#include "uart.h"
#include "lcd.h"
#include "yrdkrx63n.h"

/*******************************************************************************
* Prototypes for local functions
*******************************************************************************/

/******************************************************************************
* Function name: main
* Description  : Main program function
* Arguments    : none
* Return value : none
******************************************************************************/
void main(void)
{
  uint8_t new_char;
  
  __enable_interrupt();
  
  /* Initialize LCD */
  lcd_initialize();
  
  /* Clear LCD */
  lcd_clear();
  
  /* Display message on LCD */
  lcd_display(LCD_LINE1, "  RENESAS   ");
  lcd_display(LCD_LINE2, " YRDKRX63N  ");
  lcd_display(LCD_LINE3, "Asynchronous");    
  lcd_display(LCD_LINE4, "Serial Comm ");
  lcd_display(LCD_LINE5, "   (UART)   ");      
  lcd_display(LCD_LINE6, " 115200 N81 ");
  
  /* 
  Open the terminal I/O window to see the output.
  */
  printf("This is the debug console\r\n");
  
  sci_uart_init(); /* Initialize the SCI channel for asynchronous UART communications. */
  sci_tx_int_enable();
  sci_rx_int_enable(); 
  
  sci_put_string("\r\n\r\nRX63N UART Demonstration\r\n");
  
  /* This is the main loop. It excercises the UART and toggles LED4 */
  while (1)
  {
    while(!sci_read_count_get())   /* wait for a received character */
    {
    }
    
    LED4 = ~LED4;               /* Toggle LED after each character received. */
    
    new_char = sci_get_char();    /* Get the char waiting in buffer. */
    
    if('\r' == new_char)
    {
      sci_put_string("\r\n");      /* CRLF combo */         
    }
    else 
    {
      sci_put_char(new_char);   /* Transmit the same character back. */
    }
  }
} /* End of function main() */

