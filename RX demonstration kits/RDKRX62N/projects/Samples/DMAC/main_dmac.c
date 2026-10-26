/******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Electronics Corp. and is
* only intended for use with Renesas products.  

* No other uses are authorized.

* This software is owned by Renesas Electronics Corp. and is 
* protected under the applicable laws, including copyright laws.

* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY,
* INCLUDING BUT NOT LIMITED TO WAWRRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH
* WARRANTIES ARE EXPRESSLY DISCLAIMED.

* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER 
* RENESAS ELECTRONICS CORP. NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR AND DIRECT, INDIRECT, SPECIAL, INCIDENTAL
* OR COSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE,
* EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE
* POSSIBILITIES OF SUCH DAMAGES.

* Renesas reserves the right, without notice, to make changes to this
* software and to discontinue availability of this software.
* By using this software, you agree to the additional terms and 
* conditions found by accessing the following link:
* http://www.renesas.com/disclaimer
*******************************************************************************/
/* Copyright (C) 2010. Renesas Electronics Corp., All Rights Reserved         */
/******************************************************************************
* File Name     : main_dmac.c
* Version       : 1.1
* Device(s)     : R5F562N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.1
* OS            : None
* H/W Platform  : YRDKRX62N
* Description   : This sample demonstrates the DMAC unit; by performing a DMA
*                 transfer from one variable, and duplicating it into a 1024
*                 element array.
* Operation     : 1. Build and download the sample code to the YRDK.
*
*                 2. Right-click the variable 'gDMA_DataBuff' and select 'Instant
*                    Watch', clicking 'Add' on the subsequent dialog.
*                    
*                 3. The contents of 'gDMA_DataBuff can now be observed in the 
*                    variable watch window.
*                
*                 4. Click 'Reset Go' to start the software.
*                 
*                 5. Observe the contents of 'gDMA_DataBuff' being filled with 
*                    the contents of the variable 'gDMA_DataSource'.  The DMAC
*                    is set to perform a block transfer 1000 bytes long, so only 
*                    the first 1000 elements of gDMA_DataBuff will be filled in.  
*                    The remaining elements in the array will remain unchanged.
*
* Note          : In order to watch the contents of the gDMA_DataBuff array
*                 during execution, the sample must be built in 'debug' mode.
*******************************************************************************
* History : DD.MMM.YYYY     Description
*         : 08.Oct.2010     First release
*         : 02.Dec.2010     Second YRDK release
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
/* Defines standard variable types used in this file */
#include <stdint.h>
#include <stdbool.h>
/* Defines debug LCD functions used in this file */
#include "lcd.h"
/* Defines DMAC functions used in this file */
#include "dmac.h"

/* RSPI API library support */
#include "YRDKRX62N_RSPI_API.h"

/* Board specific definitions */
#include "YRDKRX62N.h"

#include "machine.h"
#include "intrinsics.h"
#include "hwsetup.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* DMAC transfer destination array */
uint8_t gDMA_DataBuff[1024];
/* DMAC transfer source variable */
uint8_t gDMA_DataSource = 'X';

/*******************************************************************************
* Outline       : main
* Description   : Main program function. This function first initialises the
*                 debug LCD and displays the name of the sample on the screen.
*                 Main then calls the DMAC initialisation function, which
*                 configures and starts the DMAC transfer from gDMA_DataSource
*                 to every element of gDMA_DataBuff.
* Argument      : none
* Return value  : none
*******************************************************************************/
void main(void)
{
  HardwareSetup();

  /* Enable Interrupt */
  __enable_interrupt();

  /* Initialize RSPI peripheral */
  YRDKRX62N_RSPI_Init(RSPI_CHANNEL_0);

  /* Initialise the debug LCD display */
  InitialiseLCD();

  /* Display instructions on the debug LCD */
  DisplayLCD(LCD_LINE1, "DMAC    ");
  DisplayLCD(LCD_LINE2, "        ");

  /* Initialise the DMAC unit to transfer the contents of gDMA_DataSource to
  the array gDMA_DataBuff */
  Init_DMAC();

  /* Infinite while loop */
  while (1);
}
/*******************************************************************************
* End of function main
*******************************************************************************/
