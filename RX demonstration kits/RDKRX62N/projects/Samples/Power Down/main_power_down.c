/******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Electronics Corp. and is
* only intended for use with Renesas products.  

* No other uses are authorized.

* This software is owned by Renesas Electronics Corp. and is 
* protected under the applicable laws, including copyright laws.

* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY,
* INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, 
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
* File Name     : main_power_down.c
* Version       : 1.1
* Device(s)     : R5F562N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.1
* OS            : None
* H/W Platform  : YRDKRX62N
* Description   : This sample demonstrates the MCU power down modes, by entering
*                 standby when a switch is pressed. The MCU then wakes up by any
*                 interrupt.
* Operation     : 1.  Build and download the sample code to the YRDK
*
*                 2.  Click 'Reset Go' to start the software.
*
*                 3.  The user LEDs will start to flash, and the current MCU power
*                     mode will be displayed on the debug LCD.
*
*                 4.  Press switch SW1 to enter the MCU into standby mode. The 
*                     debug LCD will update with the current power mode and the
*                     LEDs will stop flashing.
*
*                 5.  Holding down another switch whilst releasing SW1 will 
*                     prevent the MCU from entering standby until all switches are
*                     released. If another switch is held down whilst SW1 is 
*                     released, the red LEDs will be lit to indicate that the MCU cannot
*                     enter standby. Once all switches are released, the LEDs will
*                     turn off.
*          
*                 6.  Press any switch to wake the MCU from standby mode. The 
*                     debug LCD will display the new power mode, and the LEDs will
*                     stay constantly lit.
*
*                 7.  To repeat the sample, restart the software by clicking
*                     'Reset Go'
*******************************************************************************
* History : DD.MMM.YYYY     Version     Description
*         : 08.Oct.2010     1.00        First release
*         : 02.Dec.2010     1.10        Second YRDK release
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
#include <stdbool.h>

/* Defines debug LCD functions used in this file */
#include "lcd.h"
/* Defines standby power functions used in this file */
#include "power_down.h"
/* RSPI API library support */
#include "YRDKRX62N_RSPI_API.h"
/* Board specific definitions */
#include "YRDKRX62N.h"

#include "machine.h"
#include "intrinsics.h"
#include "hwsetup.h"

/*******************************************************************************
* Outline       : main
* Description   : Main program function. This function first displays the current
*                 MCU power mode on the debug LCD, then initialises the standby 
*                 registers with the Init_PowerDown function. The sample then
*                 executes the Flash_LEDs function, which toggles the LEDs until
*                 SW1 is pressed where it puts the MCU into standby mode. Any
*                 switch will wake the MCU, and the program enters an infinite
*                 while loop.
* Argument      : none
* Return value  : none
*******************************************************************************/
void main(void)
{
  HardwareSetup();

  /* Enable Interrupt */
  __enable_interrupt();

  /* Initialise the LCD on the RSPI bus */
  YRDKRX62N_RSPI_Init(RSPI_CHANNEL_0);
  InitialiseLCD();

  /* Display the current MCU power mode as active */
  DisplayLCD(LCD_LINE1, " Power Mode");
  DisplayLCD(LCD_LINE2, "   Active  ");

  /* Initialise the MCU standby power registers */
  Init_PowerDown();

  /* Run the LED flashing function */
  Flash_LEDs();

  /* Infinite while loop */
  while (1);
}
/*******************************************************************************
* End of function main
*******************************************************************************/
