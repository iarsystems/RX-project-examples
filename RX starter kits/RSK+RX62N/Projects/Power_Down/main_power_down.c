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
* File Name    : main_power_down.c
* Version      : 1.00
* Device       : R5F562N8
* Tool Chain   : IAR Embedded Workbench
* H/W Platform : RSK+RX62N
* Description  : This sample demonstrates the MCU power down modes, by entering
*                standby when a switch is pressed. The MCU then wakes up by any
*                interrupt.
* Operation    :
*
*         1. Build and download the sample code to the RSK+
*
*         2. The user LEDs will start to flash, and the current MCU power
*            mode will be displayed on the debug LCD.
*
*         3. Press switch SW1 to enter the MCU into standby mode. The 
*            debug LCD will update with the current power mode and the
*            LEDs will stop flashing.
*
*         4. Holding down another switch whilst releasing SW1 will 
*            prevent the MCU from entering standby until all switches are
*            released. If another switch is held down whilst SW1 is 
*            released, LED3 will lit to indicate that the MCU cannot
*            enter standby. Once all switches are released, LED3 will
*            turn off.
*          
*         6. Press any switch to wake the MCU from standby mode. The 
*            debug LCD will display the new power mode, and the LEDs will
*            stay constantly lit.
*
*         7. To repeat the sample, restart the software by clicking the
*            buttons 'Break', 'Reset' and 'Go'.
*******************************************************************************/
/*******************************************************************************
* History    : 02.08.2010 Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
/* Defines debug LCD functions used in this file */
#include "lcd.h"
/* Defines standby power functions used in this file */
#include "power_down.h"
#include "hwsetup.h"
#include "intrinsics.h"

/*******************************************************************************
* Outline      : main
* Description  : Main program function. This function first displays the current
*         MCU power mode on the debug LCD, then initialises the standby 
*         registers with the Init_PowerDown function. The sample then
*         executes the Flash_LEDs function, which toggles the LEDs until
*         SW1 is pressed where it puts the MCU into standby mode. Any
*         switch will wake the MCU, and the program enters an infinite
*         while loop.
* Argument     : none
* Return value : none
*******************************************************************************/
void main(void)
{
  HardwareSetup();
  __enable_interrupt();

  /* Initialise the debug LCD */
  InitialiseLCD();
  
  /* Display the current MCU power mode as active */
  DisplayLCD(LCD_LINE1, "Pwr Mode");
  DisplayLCD(LCD_LINE2, "Active  ");
  
  /* Initialise the MCU standby power registers */
  Init_PowerDown();
  
  /* Run the LED flashing function */
  Flash_LEDs();
  
  /* Infinite while loop */
  while(1);
}
/*******************************************************************************
* End of function main
*******************************************************************************/
