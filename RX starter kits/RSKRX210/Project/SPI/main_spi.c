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
/* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.   */
/*******************************************************************************
* File Name    : main_spi.c
* Version     : 1.00
* Device     : R5F52108DFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX210
* Description    : This sample demonstrates usage of the serial peripheral 
*          interface through loop-back mode whereby the transmitter and 
*          receiver pins are internally connected. In this mode, the  
*          transmitted data will be received in the receive buffer.
*
* Operation       : 1. Compile and download the sample code to the target. Click 
*           'Reset Go' icon on the 'Debug Run' toolbar to start the 
*           software.
*
*          2. Adjust the potentiometer 'RV1'.
*
*          3. Press switch SW3 to display the 12-bit A/D value 
*             corresponding to the potentiometer position on the debug
*           LCD. 
*
*          4. A message will be displayed on the debug LCD if the 
*           transfer fails.
*
*          5. The data can be watched in the variable 'gMasterRxData'
*             Right click the variable, and select 'Add Watch'. Click  
*           on 'gTxData' variable and select 'Add Watch'. The contents
*           of the 'gTxData' (transmitted data)& 'gMasterRxData' 
*           (received data) should match.
*                    
*          6. Go to Step 2 to repeat the test.
*******************************************************************************/
/*******************************************************************************
* History     : 05.08.2011  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/
/* LCD controlling function prototypes & macro definitions */
#include "lcd.h"
/* defines the serial peripheral interface's functions used in this file */
#include "spi.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Declare storage variable for received data */
volatile uint32_t gMasterRxData=0;
/* Stores the data to be transmitted */
volatile uint32_t gTxData;

/******************************************************************************
* Outline      : main
* Description  : Main program function. This function first initialises the 
*         debug LCD, and displays the name of the sample. The function
*         then calls the Init_SPI function, which configures the 
*         Serial Peripheral Interface module. The program then waits in
*         an infinite loop.
* Argument     : none
* Return value : none
******************************************************************************/
#include "intrinsics.h"
#include "hwsetup.h"

void main(void)
{
  /* Hardware setup */
  HardwareSetup();

  /* Enable Interrupt */
  __enable_interrupt();

  /* Initialise the debug LCD display */
  Init_LCD();
  
  /* Display the application name on the debug LCD */
  Display_LCD(LCD_LINE1, "  SPI   ");
  Display_LCD(LCD_LINE2, "Push SW3");
  
  /* Configure the SPI module */
  Init_SPI();
    
  /* Infinite while loop */
  while (1);
}
/******************************************************************************
* End of function main
******************************************************************************/
