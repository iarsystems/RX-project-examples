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
* Copyright (C) 2010 Renesas Electronics Corporation. All rights reserved.    */ 
/*******************************************************************************
* File Name    : main_uart.c
* Version      : 1.00
* Device       : R5F562N8
* Tool Chain   : IAR Embedded Workbench
* H/W Platform : RSK+RX62N
* Description  : This sample demonstrates the SCI unit in asynchronous mode, by
*                transmitting the numbers 0 to 9 to the terminal.
* Operation    :
*
*         1. Connect the RSK+ to the host PC via an RS232 serial cable.
*
*         2. Ensure that jumpers J6 and J15 are set to the 'TXD2-A'
*            position, and J5 & J16 are set to the 'RXD2-A' position.
*
*         3. Launch a suitable terminal program (e.g. Hyperterminal) and
*            configure it to operate on the channel the RSK+ is connected
*            to with the following settings:
*
*                  Baud............19200
*                  Data Bits...........8
*                  Parity...........None
*                  Stop Bits...........1
*                  Flow Control.....None
*        
*         4. Build and download the sample code to the RSK+.
*
*         5. Observe the terminal window - instructions will be displayed
*            at the top, then the numbers 0 - 9 will be written across
*            the screen repeatedly. 
*
*         6. Type 'z' (lowercase) into the terminal to pause the SCI
*            transmission, and any other key to resume it.
*******************************************************************************/
/*******************************************************************************
* History    : 13.08.2010 Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
/* Defines debug LCD functions used in this file */
#include "lcd.h"
/* Defines DMAC functions used in this file */
#include "async.h"
#include "hwsetup.h"
#include "intrinsics.h"

/*******************************************************************************
* Outline      : main
* Description  : Main program function. This function first displays on the 
*         debug LCD; then calls the async initialisation function, which
*         configures and starts the SCI unit to transmit the numbers 0 to
*         9 on the terminal untill the character 'z' is typed into the
*         terminal. Any other key will resume the transfer.
*         every element of gDMA_DataBuff.
* Argument     : none
* Return value : none
*******************************************************************************/
void main(void)
{
  HardwareSetup();
  __enable_interrupt();

  /* Initialise the debug LCD */
  InitialiseLCD();
  
  /* Display instructions onto the LCD */
  DisplayLCD(LCD_LINE1, "Async   ");
  DisplayLCD(LCD_LINE2, "Serial  ");
  
  /* Initialise the SCI unit for asynchronous operation */
  Init_Async();
  
  /* Infinite while loop */
  while(1);

}
/*******************************************************************************
* End of function main
*******************************************************************************/

