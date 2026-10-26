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
* File Name     : crc_demo_main.c
* Version       : 1.0
* Device(s)     : R5F563N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.0
* OS            : None
* H/W Platform  : YRDKRX63N
* Description   : This sample demonstrates usage of the CRC unit, by echoing
*                 characters typed in the terminal with their checksums.
*
* Operation     : 1. Build and download the sample code to the YRDK.
*
*                 2. Connect the YRDK to the host PC via an RS232 serial cable.
*
*                 3. Launch a suitable terminal program (e.g. Hyperterminal) and
*                    configure it to operate on the channel the YRDK is connected
*                    to with the following settings:
*
*                                    Baud...........115200
*                                    Data Bits...........8
*                                    Parity...........None
*                                    Stop Bits...........1
*                                    Flow Control.....None
*                
*                 4. Click 'Go' to start the software.
*                 
*                 5. Observe the terminal window - instructions will be displayed
*                    at the top, asking the user to input a character.
*
*                 6. Type any letter (printable characters only) into the
*                    terminal, and the character will be echoed back to the
*                    terminal display, along with its corresponding CRC checksum.
*******************************************************************************/
/*******************************************************************************
* History : DD.MMM.YYYY     Version     Description
*         : 22.Sep.2011     1.00        First release
*******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <intrinsics.h>
#include "uart.h"
#include "crc.h"
#include "lcd.h"

/******************************************************************************
* Function name: main
* Description  : Main program function. This function first initialises the
*                 LCD and displays the name of the program. The function then 
*                 calls the Init_CRC initialisation function, which configures 
*                 the SCI and CRC units. 
*                 When the MCU returns to main, it enters an infinite while loop.
* Arguments    : none
* Return value : none
******************************************************************************/
void main(void)
{ 
  __enable_interrupt();

  lcd_initialize();
  
  /* Clear LCD */
  lcd_clear();
  
  /* Display message on LCD */
  lcd_display(LCD_LINE1, "  RENESAS   ");
  lcd_display(LCD_LINE2, " YRDKRX63N  ");
  lcd_display(LCD_LINE3, "    CRC     ");    
  lcd_display(LCD_LINE4, " Calculator ");
  lcd_display(LCD_LINE5, "   Open     ");
  lcd_display(LCD_LINE6, "  terminal  ");
  lcd_display(LCD_LINE7, "program set ");
  lcd_display(LCD_LINE8, "115200,N,8,1");
  
  /* 
  Open the terminal I/O window to see the output. 
  */
  printf("This is the debug console\r\n");
  
  /* This demo will use a serial terminal to get characters from the keyboard 
     and to display the CRC results. 
     Set up the SCI channel for asynchronous UART (RS232) communications. */  
  sci_uart_init();
  sci_tx_interrupt_enable();
  sci_rx_interrupt_enable(); 
  
  sci_put_str("\r\n\nRenesas YRDKRX63N CRC Sample\r\n\n"); /* Send terminal header message. */
  sci_put_str("Enter any key, and the checksum will be displayed below\r\n\n");           
  
  crc_init();  /* Prepare the CRC calculator peripheral for operation. */
  
  /* This is the main loop. It repeats the CRC demo forever. */
  while (1)
  {
    crc_demo();  
  }
} /* End of function main() */
