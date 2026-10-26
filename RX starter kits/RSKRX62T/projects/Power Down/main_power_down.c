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
* File Name    : main_power_down.c
* Version      : 2.00
* Device      : R5F562TAADFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform : RSKRX62T
* Description  : This sample demonstrates the MCU power down modes, by entering
*         standby when a switch is pressed. The MCU then wakes up by any
*         interrupt.
* Operation     : 1. Build and download the sample code to the RSK
*
*         2. Click 'Reset Go' to start the software.
*
*         3. The user LEDs will start to flash, and the current MCU power
*          mode will be displayed on the debug LCD.
*
*         4. Press switch SW1 to enter the MCU into standby mode. The 
*          debug LCD will update with the current power mode and the
*          LEDs will stop flashing.
*
*         5. Press any switch to wake the MCU from standby mode. The 
*          debug LCD will display the new power mode, and the LEDs will
*          stay constantly lit.
*
*         6. To repeat the sample, restart the software by clicking
*           'Reset Go'
*******************************************************************************/
/*******************************************************************************
* History     : 15.10.2010 Ver. 1.00 First Release
              : 21.05.2011 Ver. 2.00 Second Release (Updated to RPDL CS3)
*******************************************************************************/

/******************************************************************************
* Project Includes
******************************************************************************/
/* Defines debug LCD functions used in this file */
#include "lcd.h"
/* Defines standby power functions used in this file */
#include "power_down.h"
#include "hwsetup.h"
#include <intrinsics.h>

/******************************************************************************
* Outline      : main
* Description  : Main program function. This function first displays the sample 
*         name and the power mode on the debug LCD, then initialises the 
*         standby registers with the Init_PowerDown function. The sample  
*         then calls the Flash_LEDs function, which toggles the LEDs 
*         until SW1 is pressed where it puts the MCU into standby mode. 
*         Pressing any switch during standby mode will wake the MCU, and 
*         the program enters an infinite while loop.
* Argument     : none
* Return value : none
******************************************************************************/
void main(void)
{
  /* Set up hardware */
  HardwareSetup();
  
  /* Enable interrupt */
  __enable_interrupt();
  
  /* Initialise the debug LCD */
  InitialiseLCD();
  
  /* Display the current MCU power mode as active */
  DisplayLCD(LCD_LINE1, "PWR Mode");
  DisplayLCD(LCD_LINE2, "Active  ");
  
  /* Initialise the MCU standby power registers */
  Init_PowerDown();
  
  /* Run the LED flashing function */
  Flash_LEDs();
  
  /* Infinite while loop */
  while(1);
}
/******************************************************************************
* End of function main
******************************************************************************/
