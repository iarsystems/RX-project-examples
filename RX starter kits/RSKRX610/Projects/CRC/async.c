/* Adapted for use with IAR Embedded Workbench */
/******************************************************************************
* DISCLAIMER:
* The software supplied by Renesas Technology Europe Ltd is
* intended and supplied for use on Renesas Technology products.
* This software is owned by Renesas Technology Europe, Ltd. Or
* Renesas Technology Corporation and is protected under applicable
* copyright laws. All rights are reserved.
*
* THIS SOFTWARE IS PROVIDED "AS IS". NO WARRANTIES, WHETHER EXPRESS,
* IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* APPLY TO THIS SOFTWARE. RENESAS TECHNOLOGY AMERICA, INC. AND
* AND RENESAS TECHNOLOGY CORPORATION RESERVE THE RIGHT, WITHOUT
* NOTICE, TO MAKE CHANGES TO THIS SOFTWARE. NEITHER RENESAS
* TECHNOLOGY AMERICA, INC. NOR RENESAS TECHNOLOGY CORPORATION SHALL,
* IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
* CONSEQUENTIAL DAMAGES FOR ANY REASON WHATSOEVER ARISING OUT OF THE
* USE OR APPLICATION OF THIS SOFTWARE.
******************************************************************************/
/* Copyright (C) 2009. Renesas Technology Europe, All Rights Reserved */
/*””FILE COMMENT””******************************* Technical reference data ****
* File Name    : async.c
* Version     : 1.00
* Device     : R5F56107VNFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX610
* Description   : File provides async serial functionality         
******************************************************************************/
/******************************************************************************
* History     : 22.01.2010 Ver. 1.00 First Release
*””FILE COMMENT END””*********************************************************/


/******************************************************************************
User Includes (Project level includes)
******************************************************************************/
/* IO Port RPDL function definitions */
#include "r_pdl_io_port.h"
/* ADC RPDL function definitions */
#include "r_pdl_adc_10.h"
/* SCI RPDL function definitions */
#include "r_pdl_sci.h"
/* Interrupt RPDL function definitions */
#include "r_pdl_intc.h"
/* Timer RPDL function definitions */
#include "r_pdl_tmr.h"
/* CGC RPDL function definitions */
#include "r_pdl_cgc.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* CRC function definitions */
#include "CRC.h"
/* Declares functions defined in this file */
#include "async.h"


/******************************************************************************
System Definitions & Global Variables
******************************************************************************/
/* rx_flag variable indicates when 'z' has been pressed on the terminal */
uint16_t rx_flag, count = 0x0, test;
/* declares transmit and receive buffer variables used in this file */
uint8_t tx_buf[] =  " ,     -CRC Code:    \0", rx_buf;
/* Defines UART commands used to clear the screen, and return the cursor home */
const uint8_t cmd_clr_scr[] = {27,'[','2','J',0};
const uint8_t cmd_cur_home[] = {27,'[','H',0};


/*””FUNC COMMENT””*************************************************************
* Outline     : UART_Init
* Description   : Initialises the UART device and settings
* Argument    : none
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
void UART_Init(void)
{
  /* Set the Tx Pin Data Direction */
  R_IO_PORT_Set(PDL_IO_PORT_2_6, PDL_IO_PORT_OUTPUT, 0, 0, 0);
  
  /* Set the Rx Pin Data Direction */
  R_IO_PORT_Set(PDL_IO_PORT_2_5, PDL_IO_PORT_INPUT, 0, 0, 0);
  
  /* Initialise UART device */
  R_SCI_Create(1, PDL_SCI_8N1, 19200, 6);
  
  /* Initialise UART recieve interrupt */
  R_SCI_Receive(1, PDL_NO_DATA, &rx_buf, 1, rx_callback, PDL_NO_FUNC);
  
  /* Clear the terminal screen, and return the cursor to the start position */
  R_SCI_Send(1, 0, cmd_clr_scr, 0, PDL_NO_FUNC);
  R_SCI_Send(1, 0, cmd_cur_home, 0, PDL_NO_FUNC);
  
  /* Display splash screen on terminal window */
  R_SCI_Send(1, 0, "Renesas RX610 CRC Test \n\r", 0, PDL_NO_FUNC);
  R_SCI_Send(1, 0, "Enter a character\r\n\n", 0, PDL_NO_FUNC);
}
/******************************************************************************
End of function Uart_Init
******************************************************************************/


/*””FUNC COMMENT””*************************************************************
* Outline     : rx_callback
* Description   : Function executes when character is recieved from UART 
*          terminal. Function creates a CRC checksum from the recieved
          character, and sends it back to the terminal.
* Argument    : none
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
void rx_callback(void)
{  
  /* Declare temporary buffer variables */
  uint16_t CRC_buf;
  char a;
  
  /* Create CRC checksum from received keystroke */
  CRC_buf = (char)CRC_Generate(rx_buf);
  
  /* Convert input key and output checksum values into ASCII string */
  tx_buf[0] = (char)rx_buf;
  a = (char)((rx_buf & 0x0F00)>> 8);
  tx_buf[3] = (a < 0x0A) ? (a+0x30):(a+0x37); 
  a = (char)((rx_buf & 0x00F0)>> 4);
  tx_buf[4] = (a < 0x0A) ? (a+0x30):(a+0x37); 
  a = (char)(rx_buf & 0x000F);
  tx_buf[5] = (a < 0x0A) ? (a+0x30):(a+0x37); 
  a = (char)((CRC_buf & 0x0F00)>> 8);
  tx_buf[18] = (a < 0x0A) ? (a+0x30):(a+0x37); 
  a = (char)((CRC_buf & 0x00F0)>> 4);
  tx_buf[19] = (a < 0x0A) ? (a+0x30):(a+0x37); 
  a = (char)(CRC_buf & 0x000F);
  tx_buf[20] = (a < 0x0A) ? (a+0x30):(a+0x37); 

  /* Send the ASCII string to the UART terminal */
  R_SCI_Send(1, 0, tx_buf, 0, PDL_NO_FUNC);
  
  /* Return the cursor to the start of the line on the UART terminal */
  R_SCI_Send(1, 0, " \r", 0, PDL_NO_FUNC);
  
  /* Reinitialise the UART receive interrupt */
  R_SCI_Receive(1, PDL_NO_DATA, &rx_buf, 1, rx_callback, PDL_NO_FUNC);
}
/***********************************************************************************
End rx_callback
***********************************************************************************/
