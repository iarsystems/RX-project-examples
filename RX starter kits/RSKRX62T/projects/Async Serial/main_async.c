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
/*******************************************************************************
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.    */ 
/*******************************************************************************
* File Name    : main_async.c
* Version     : 2.00
* Device     : R5F562TAADFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX62T
* Description   : Demonstration of SCI in asynchronous mode. The program 
*            communicates from the RX62T to a terminal program via SCI and
*            RS-232.    
* Instructions  : 1. Connect the RSK to a PC via a 9-way, straight through
*           serial cable. Open a terminal program (e.g HyperTerminal).
*           
*          2. Ensure the terminal program connection settings are as
*             follows:
*           
*           Baud Rate.............19200
*           Data Bits.................8
*           Parity.................None
*           Stop Bits.................1
*           Flow Control...........None
*
*          3. Compile and download the sample code. Click 'Reset Go' to
*           start the software.
*           
*          4. Observe the output on the terminal window. The numbers 0
*           to 9 will appear repeatedly. Type 'z' (lowercase) into the
*           terminal to pause the UART writes, and any other key to
*           resume.  
*
*          5. While data is being transmitted, LED0 will be on and LED1
*             will be off. When transmission is inactive, LED1 will be
*           on and LED0 will be off. 
******************************************************************************/  
/******************************************************************************
* History     : 15.10.2010 Ver. 1.00 First Release
              : 21.05.2011 Ver. 2.00 Second Release (Updated to RPDL CS3)
******************************************************************************/

/******************************************************************************
* Project Includes
******************************************************************************/
/* IO Port RPDL function definitions */
#include "r_pdl_io_port.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* LCD function definitions */
#include "lcd.h"
/* Async UART function definitions */
#include "async.h"
#include "hwsetup.h"
#include <intrinsics.h>

/******************************************************************************
* Outline     : main
* Description   : The main program function. Function calls the UART 
          initialisation routine, displays instructions on the LCD,
          then enters an infinite while loop. Within the while loop,
          the function calls the Text_Write function with each loop
          iteration. If the rx_flag is raised (from a z press on
          terminal) the function turns on LED1 and waits for another 
          key press.
* Argument    : none
* Return value  : none
******************************************************************************/
void main(void)
{
  /* Set up hardware */
  HardwareSetup();
  
  /* Enable interrupt */
  __enable_interrupt();
  
  /* Initialise the LCD Display */
  InitialiseLCD();
  
  /* Initialises the UART device */
  Init_Async();
    
  /* Displays the Renesas splash screen */
  DisplayLCD(LCD_LINE1, " Async  ");
  DisplayLCD(LCD_LINE2, " Serial ");
  
  /* Infinite while loop */
  while(1);
}
/******************************************************************************
End of main
******************************************************************************/
