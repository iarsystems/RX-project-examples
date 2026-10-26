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
* Device     : R5F5630EDDFP
* Tool-Chain   : RX Family C Compiler, v1.01
* H/W Platform  : RSKRX630
* Description   : File provides async serial output transmission       
*******************************************************************************/
/*******************************************************************************
* History     : 23.01.2012 Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
User Includes
*******************************************************************************/
/* Defines I/O registers */
#include "iorx630_usb.h"
/* Defines RX630 LEDs and switches */
#include "rskrx630def.h"
/* Defines used for the SCI unit */
#include "async.h"

#include <string.h>

/*******************************************************************************
System Definitions & Global Variables
*******************************************************************************/
#define UART_BAUD       115200
#define UART_INT_PRIORITY  5

/*******************************************************************************
* Outline     : UART_
* Description   : Initialises UART 0 configuration - BAUD - 19200, 
*          8 data bits, 1 stop bit, no parity.none
* Argument    : none
* Return value  : none
*******************************************************************************/
void Init_UART(void)
{  
  /* Declare a loop count variable */
  uint16_t i=0;
  
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
  MSTP_SCI0 = 0x0;
  
  /* Disable All bits and CKE0,1 = 00 */
    SCI0.SCR.BYTE = 0x00;                                  

  /* Set output port TXD0(P20) */
  PORT2.PDR.BIT.B0 = 1;

  /* disable All bits and CKE0,1 = 00 once more */
    SCI0.SCR.BYTE = 0x00;                                  

  /* CKS[0-1] = 00: PCLK 0 div        
       1Sop 8bits non-parity     */
    SCI0.SMR.BYTE = 0x00;   
                                                            
    /* SMIF = 0: Sirial  */
    SCI0.SCMR.BYTE = 0xf2;                                   

    /* PCLK = 48MHz, Set baudrate to 115200 bps 
       115200 = 48MHz / (64*0.5*115200)-1 */                                     
   SCI0.BRR = ((48000000) / (32 * UART_BAUD) - 1);
                              
  /* Allow a small delay */
    for ( i = 0; i < 20; i++ )
  {
  }                               

  /* Clear bits ORER, PER and FER */
    SCI0.SSR.BYTE &= 0xC7;                                    
  
  /* Set bits TIE, RIE, RE and TE. Enable receive interrupts */
    SCI0.SCR.BYTE = 0xF0;                                     
}
/******************************************************************************
End of function Init_UART
******************************************************************************/

/******************************************************************************
* Outline     : charput
* Description   : Send a character
* Argument    : c
* Return value  : none
******************************************************************************/
void charput(uint8_t c)
{

  while(!(SCI0.SSR.BYTE & 0x80))
  {
    /* Ensure that the previous transmission is complete. */
  }

  /* Transmit next character. */
  SCI0.TDR = (uint16_t) c;
}
/******************************************************************************
End of function charput
******************************************************************************/

/******************************************************************************
* Outline     : charget
* Description   : Not implemented but required by stdio
* Argument    : none
* Return value  : none
******************************************************************************/
int8_t charget(void)
{
  /*Not supported*/
  return 0;
}
/******************************************************************************
End of function charget
******************************************************************************/

/*
  __write is based on the dlib prototype in __dbg_xxwrite.c
  This is dlib specific to override the terminal emulator
  and send it to UART instead.
*/

size_t __write(int handle, const unsigned char *buf, size_t size)
{

  while(!(SCI0.SSR.BYTE & 0x80))
  {
    /* Ensure that the previous transmission is complete. */
  }

  /* Transmit next character. */
  SCI0.TDR = (uint16_t) *buf;
  
  if (buf == 0)
  {
    /* Flushing is not supported by the debugger. */
    return 0;
  }
  return 1;
}