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
* File Name      : main_dmac.c
* Version     : 1.00
* Device     : R5F5630E
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX630
* Description    : This sample demonstrates the DMAC unit by performing a DMA
*          transfer from one variable, and duplicating it into a 1024
*          element array.
* Operation       : 1. Compile and download the sample code to the target. 
*
*          2. Right-click the buffer 'gDMA_DataBuff' and select 'Add
*            Watch'. Within the Watch window right-click on 
*           'gDMA_DataBuff' and select 'Enable Auto Update'.
*          
*          3. The contents of 'gDMA_DataBuff can now be observed in the 
*            variable watch window.
*        
*          4. Click 'Reset Go' to start the software.
*         
*          5. The contents of 'gDMA_DataBuff' will be cleared to 0 
*           before being filled with the content of the variable 
*           'gDMA_DataSource', being character 'X'. If the transfer 
*           is successful, LED1 will turn on.
*
*         Note  : In order to watch the contents of the 'gDMA_DataBuff' array
*          during execution, the sample must be built in 'debug' mode.
*******************************************************************************/
/*******************************************************************************
* History     : 23 Jan. 2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/
/* LCD controlling function prototypes & macro definitions */
#include "lcd.h"
/* Defines DMAC functions used in this file */
#include "dmac.h"

#include "intrinsics.h"

#include "hwsetup.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* DMAC transfer destination array */
extern uint8_t gDMA_DataBuff[DMA_BUFFER_SIZE];

/*******************************************************************************
* Outline      : main
* Description  : Main program function. This function first initialises the
*         debug LCD and displays the name of the application on the 
*         screen. Main then calls the DMAC initialisation function, which
*         configures and starts the DMAC transfer from 'gDMA_DataSource'
*         to every element of 'gDMA_DataBuff' buffer.
* Argument     : none
* Return value : none
*******************************************************************************/
void main(void)
{
  
  HardwareSetup();
  
  __enable_interrupt();
  
  /* Initialise the debug LCD display */
  Init_LCD();
  
  /* Display instructions on the debug LCD */
  Display_LCD(LCD_LINE1, "  DMAC  ");
  Display_LCD(LCD_LINE2, "        ");

  /* Initialise the DMAC unit to transfer the contents 
     of 'gDMA_DataSource' to the array 'gDMA_DataBuff' */
  Init_DMAC();
  
  /* Infinite while loop */
  while(1);
}
/*******************************************************************************
* End of function main
*******************************************************************************/
