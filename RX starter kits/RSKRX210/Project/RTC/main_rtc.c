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
* File Name    : main_rtc.c
* Version     : 1.00
* Device     : R5F52108DFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX210
* Description   : Demonstration of the RTC module. The program presets the 
*          time & date of the RTC. It configures the RTC to generate 
*          an interrupt every second. The alarm is also configured to 
*          trigger at a specified time.  
* Operation    : 1. Compile and download the sample code to the target. click 
*           'Reset Go' icon on the 'Debug Run' toolbar to start the 
*           software.
*           
*          2. The debug LCD will display the RTC time starting from 
*           11:59:30.  
*        
*          3. The RTC periodic interrupt will occur every second and 
*           the interrupt handler will update the time on the LCD.
*           
*          4. The alarm will be triggered at 12:00:00. An interrupt 
*             handler  for alarm will be executed and LED1 will be
*           turned on.
*******************************************************************************/
/*******************************************************************************
* History     : 05.08.2011  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/
/* LCD controlling function prototypes & macro definitions */
#include "lcd.h"
/* Contains prototypes for RTC initialisation function  */
#include "rtc.h"

/*******************************************************************************
* Outline     : main
* Description   : The main program function initialises the debug LCD and 
*          displays the sample name and the clock. The function 
*          'Init_RTC' is called to initialise the RTC's module registers.  
*          The function then enters an infinite while loop.
* Argument    : none
* Return value  : none
*******************************************************************************/
#include "intrinsics.h"
#include "hwsetup.h"

void main(void)
{
  /* Hardware setup */
  HardwareSetup();

  /* Enable Interrupt */
  __enable_interrupt();

  /* Initialise the debug LCD */
  Init_LCD();
  
  /* Display application name on the debug LCD */
  Display_LCD(LCD_LINE1, "  RTC   ");
  
  /* Initialize RTC  */
  Init_RTC();
  
  /* Infinite while loop */
  while(1);
}
/*******************************************************************************
* End of function main
*******************************************************************************/
