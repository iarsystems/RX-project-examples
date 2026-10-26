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
* File Name      : main_fla_data.c
* Version        : 1.00
* Device     : R5F5630E
* Tool Chain     : Renesas RX Toolchain 1.2.0.0
* H/W Platform  : RSKRX630
* Description    : This sample demonstrates the FCU unit, by writing ADC results
*          to incrementing data flash memory locations.
* Operation       : 1. Build and download the sample code to the RSK.
*
*         2. Open the memory viewing, right click in the window and click
*          'address'. Enter the Display address 0x00100000, then click
*          OK. 
*
*         3. Ensure the 'Auto Refresh' option in the memory viewer is
*          disabled before proceeding. (It is not possible to use this
*          feature when observing flash memory).
*
*         4. Click 'Reset Go' to start the sample. Instructions will be
*           displayed on the debug LCD.
*
*         5. Push switch SW1 to trigger an ADC conversion. The value will
*          be written into the data flash memory. Click 'Stop' to view
*          the updated flash memory. Click 'Run' or press F5 to resume.
*
*         6. The data written to flash and its memory location are
*          displayed on the debug LCD.
*
*         7. Repress switch SW1 to trigger another ADC conversion. The 
*          result will be stored in an incremented memory location,
*          displayed on the debug LCD.
*
*         8. Press switch SW3 to reset the contents of the entire flash
*          memory block, and set the flash write pointer back to the
*          start of the block (0x00100000). Click 'Stop' to view the
*          flash memory to confirm this, and press F5 to resume.
* Note         : Memory locations which have not initialised do not have a
*         defined state, and will continuously change with random values.
*******************************************************************************/  
/*******************************************************************************
* History     : 23 Jan. 2012 Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
/* Defines LCD functions used in this file */
#include "lcd.h"
/* Defines flash functions used in this file */
#include "flash_data.h"

#include "intrinsics.h"

#include "hwsetup.h"

/*******************************************************************************
* Outline      : main
* Description  : Main program function. This function first initialises the
*         debug LCD and displays instructions on the screen. Main then
*         calls the flash intialisation function which configures the FCU
*         unit, and sets up callback functions for the switch presses.
*         The function then waits in an infinite while loop.
* Argument     : none
* Return value : none
*******************************************************************************/
void main(void)
{
  
  HardwareSetup();
  
  __enable_interrupt();
  
  /* Initialise the debug LCD */
  Init_LCD();

  /* Display instructions on the debug LCD */  
  Display_LCD(LCD_LINE1, "Data Fla");
  Display_LCD(LCD_LINE2, "Push SW1");
  
  /* Initialise the flash controller unit */
  Init_FlashData();
  
  /* Infinite while loop */  
  while(1);
}
/******************************************************************************
* End of function main
******************************************************************************/
