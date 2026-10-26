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
* File Name    : main_dmac.c
* Version      : 1.00
* Device       : R5F562N8
* Tool Chain   : IAR Embedded Workbench
* H/W Platform : RSK+RX62N
* Description  : This sample demonstrates the DMAC unit; by performing a DMA
*                transfer from one variable, and duplicating it into a 1024
*                element array.
* Operation    :
*
*         1. Build and download the sample code to the RSK+.
*
*         2. Open the 'Live Watch' window ('View' menu).
*
*         3. Drag the variables 'gDMA_DataBuff' and 'gDMA_DataSource'
*            to the 'Live Watch'. The contents of 'gDMA_DataBuff' and
*            'gDMA_DataSource' can now be observed.
*          
*         4. Click 'Go' and observe the contents of 'gDMA_DataBuff' being
*            filled with the contents of the variable 'gDMA_DataSource'.
*
*         5. Press SW3 to update the data in 'gDMA_DataSource' and
*            perform a new transfer of the updated data.
*******************************************************************************/
/*******************************************************************************
* History    : 30.07.2010 Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* System Includes
*******************************************************************************/
/* Defines standard variable types used in this file */
#include <stdint.h>

/*******************************************************************************
* Project Includes
*******************************************************************************/
/* Defines debug LCD functions used in this file */
#include "lcd.h"
/* Defines DMAC functions used in this file */
#include "dmac.h"
#include "hwsetup.h"
#include "intrinsics.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* DMAC transfer destination array */
uint8_t gDMA_DataBuff[1024];
/* DMAC transfer source variable */
uint8_t gDMA_DataSource = 'X';

/*******************************************************************************
* Outline      : main
* Description  : Main program function. This function first initialises the
*         debug LCD and displays the name of the sample on the screen.
*         Main then calls the DMAC initialisation function, which
*         configures and starts the DMAC transfer from gDMA_DataSource
*         to every element of gDMA_DataBuff.
* Argument     : none
* Return value : none
*******************************************************************************/
void main(void)
{
  HardwareSetup();
  __enable_interrupt();

  /* Initialise the debug LCD display */
  InitialiseLCD();
  
  /* Display instructions on the debug LCD */
  DisplayLCD(LCD_LINE1, "DMAC    ");
  DisplayLCD(LCD_LINE2, "Push SW3");

  /* Initialise the DMAC unit to transfer the contents of gDMA_DataSource to
     the array gDMA_DataBuff */
  Init_DMAC();
  
  /* Infinite while loop */
  while(1);
}
/*******************************************************************************
* End of function main
*******************************************************************************/
