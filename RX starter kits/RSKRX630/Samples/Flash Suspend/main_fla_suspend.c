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
* File Name      : main_fla_suspend.c
* Version        : 1.00
* Device     : R5F5630E
* Tool Chain     : Renesas RX Toolchain 1.2.0.0
* H/W Platform  : RSKRX630
* Description    : This sample demonstrates FCU, and its ability to suspend flash
*          write operations midway through, and then resume them.
* Operation       :1. Build and download the sample code to the RSK.
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
*         5. Push switch SW1 to start a flash write. The flash will then
*          be suspend while all the user LEDs flash, and the debug LCD
*          will indicate a flash write has been suspended.
*
*         6. After approximately 5 seconds, the user LEDs will stop
*          flashing, and the data flash write will resume. Once it is
*          complete, the debug LCD will indicate this briefly before
*          returning to the original message.
*
*         7. Repress switch SW1 to trigger more data flash writes, that
*          will be suspended. Once a flash write is complete, you can
*          click 'stop', and view the written data in the memory
*          viewer. Press F5 or click 'Run' to continue.
*
*         8. Press SW3 to erase the entire data flash block.
*
* Note         : Memory locations which have not initialised do not have a
*          defined state, and will continuously change with random values.
*        
*          If the code is stopped whilst a flash write is suspended, the
*          entire data flash area is unreadable until the write has been 
*          resumed. If the MCU is reset during flash write suspension,
*            the half complete write can be seen in the memory viewer.
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
#include "flash_suspend.h"

#include "intrinsics.h"

#include "hwsetup.h"

/*******************************************************************************
* Outline      : main
* Description  : The main function intialises the debug LCD, displays the sample
*         name and instructions on the debug LCD, then initialises the
*         FCU and switch callback functions. The function then enters an
*         infinite while loop. The rest of the sample's functionality is
*         done in interrupts.  
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
  Display_LCD(LCD_LINE1, "F. Susp.");
  Display_LCD(LCD_LINE2, "Push SW1");

  /* Initialise the flash controller unit */
  Init_FlashSuspend();
  
  /* Infinite while loop */  
  while(1);
}
/******************************************************************************
* End of function main
******************************************************************************/

