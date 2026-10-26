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
* File Name      : main_spi.c
* Version        : 1.00
* Device     : R5F5630E
* Tool Chain     : Renesas RX Toolchain 1.2.0.0
* H/W Platform  : RSKRX630
* Description    : This sample demonstrates the SPI unit, by transfering data
*          from one SPI channel to another.
* Operation       : Before starting this sample, the following wire connections
*          must be made:
*
*            Function    Pin     ->  Pin      Function
*            ------------------------------------------
*            RSPCKA    JA3, 9      JA3, 30    RSPCKB
*            MOSIA    JA3, 7     JA3, 31    MOSIB
*            MISOA    JA3, 8     JA3, 32    MISOB          
*
*         1. Compile and download the sample code to the target. 
*           Click 'Reset Go' icon on the 'Debug Run' toolbar to 
*           start the software.
*
*         2. The SPI unit will transfer data from SPI master (channel
*          1, pins B) to SPI slave (channel 0, pins A).
*
*         3. If the transfer is successful, the debug LCD will display
*          a SPI success message. Otherwise it will indicate SPI
*          failure.
*
* Note         : If the transfer fails, or the 'Sending' message on the debug
*          LCD persists; double check the wire connections.
*
*          You can watch the receive buffer fill with data from the SPI
*          unit by right clicking the gMasterRxBuffer variable and 
*          selecting 'Add watch'.
*******************************************************************************/  
/*******************************************************************************
* History     : 23 Jan. 2012 Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
/* Defines SPI functions used in this file */
#include "spi.h"
/* Defines LCD functions used in this file */
#include "lcd.h"

#include "intrinsics.h"

#include "hwsetup.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Transmit message buffer variable (editable) */
uint8_t gTransmitMessage[32] = "Renesas RX630 SPI Test";
/* SPI master receive buffer variable (imported) */
extern volatile uint8_t gMasterRxBuffer[32];

/*******************************************************************************
* Outline     : main
* Description   : The main function first initialises the debug LCD and dislays
*          sample name, then initialises the SPI master and slave 
*          channels. The main function finally calls the MasterSend_SPI
*          function to transmit the contents of gTransmitMessage. Once
*            this completes it enters an infinite while loop.
* Argument      : none
* Return value  : none
*******************************************************************************/
void main(void)
{
  
  HardwareSetup();
  
  __enable_interrupt();
  
  /* Initialise the debug LCD */
  Init_LCD();
  
  /* Display sample name and sending on debug LCD */
  Display_LCD(LCD_LINE1, " SPI    ");
  Display_LCD(LCD_LINE2, " Sending");
  
  /* Initialise the SPI unit */
  Init_SPI();
  
  /* Transmit the contents of the gTransmitMessage string */
  MasterSend_SPI(sizeof(gTransmitMessage), gTransmitMessage);
  
  /* Infinite while loop */
  while(1);
}
/*******************************************************************************
* End of function main
*******************************************************************************/
