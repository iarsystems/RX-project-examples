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
* File Name     : main_serial_sio.c
* Version     : 1.00
* Device     : R5F5630E
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX630
* Description   : This sample demonstrates usage of the SCI unit in synchronous
*          mode by performing a loop back test between two SCI channels.
* Operation      : Before starting, configuration is set:
*         
*          Wire Connections
*          ----------------
*          JA3, pin 2 (SCK5-A) to JA3, pin 29 (SCK12)
*          JA3, pin 3 (RXD5-A) to JA3, pin 30 (TXD12)
*          JA3, pin 5 (TXD5-A) to JA3, pin 31 (RXD12)
*           
*         
*          1. Build and download the sample code to the RSK
*
*          2. Right click the variable g_SCI5_RXBuffer and select "Add
*             Watch", and do the same for the variable g_SCI12_TXBuffer, 
*           g_SCI5_TXBuffer and g_SCI12_RXBuffer. From the Watch 
*           window, right-click on one of the buffers and select:
*           'Enable Auto Update All'.
*                
*          3. Click 'Reset Go' to start the software. The name of the
*            sample will be displayed on the debug LCD.
*
*          4. The sample will then begin the data transfer. When the SCI
*             activity is complete, the second line of the debug LCD will
*           display either "Success" or "Failure".
*          
*          5. You can observe the contents of g_SCI5_RXBuffer having been 
*             filled with the contents of g_SCI12_TXBuffer. The contents 
*           of g_SCI12_RXBuffer having been filled with that of 
*           g_SCI5_TXBuffer.
*          
*          6. If the debug LCD reports "Failure", or shows " RX/TX " 
*           without changing; check the configuration specified is set
*           and click "Reset Go" to start again.
******************************************************************************/  
/******************************************************************************
* History     : 23 Jan. 2012  Ver. 1.00 First Release
******************************************************************************/

/******************************************************************************
* Project Includes
******************************************************************************/
/* Defines debug LCD functions used in this file */
#include "lcd.h"
/* Defines synchronous serial functions used in this file */
#include "sync.h"

#include "intrinsics.h"

#include "hwsetup.h"

/******************************************************************************
* Global Variables
******************************************************************************/
/* Declaration of the SCI5 transmit buffer */
extern uint8_t g_SCI5_TXBuffer[28];
/* Declaration of the SCI12 transmit buffer */
extern uint8_t g_SCI12_TXBuffer[28];
/* Declaration of SCI5 receive buffer */
extern uint8_t g_SCI5_RXBuffer[28];
/* Declaration of SCI12 receive buffer */
extern uint8_t g_SCI12_RXBuffer[28];

/******************************************************************************
* Outline      : main
* Description  : Main program function. This function first intialises the debug
*         LCD, and displays the name of the sample. Main then intialises
*         the SCI unit, and performs a data transfer from channel 5 to 
*         channel 12, then another transfer in the other direction. After
*         the second transfer, the debug LCD displays either "Success" or
*         "Failure". The main function finally waits in an infinite while
*         loop.
* Argument     : none
* Return value : none
*******************************************************************************/
void main(void)
{
  
  HardwareSetup();
  
  __enable_interrupt();
  
  /* Initialise the debug LCD */
  Init_LCD();
  
  /* Display the sample name on the debug LCD, and RX/TX to indicate the SCI
       data transfers are in progress */
  Display_LCD(LCD_LINE1, "Sync SCI");
  Display_LCD(LCD_LINE2, " RX/TX  ");
  
  /* Initialise the SCI unit */
  Init_Sync();
  
  /* Configure the SCI channels, and start a transfer from channel 5 to 12,
     then report the results onto the debug LCD */
  SCI5toSCI12Transfer_Sync();
  
  /* Configure the SCI channels, and start a transfer from channel 12 to 5 */
  SCI12toSCI5Transfer_Sync();
  
  /* Check if the transfers were successful */
  Transfer_End();

  /* Infinite while loop */
  while(1);
}
/******************************************************************************
* End of function main
******************************************************************************/
