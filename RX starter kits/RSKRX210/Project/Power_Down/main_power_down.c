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
* File Name      : main_power_down.c
* Version        : 1.00
* Device     : R5F52108DFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX210
* Description    : This sample demonstrates the MCU power down mode, by entering
*          standby mode. The RTC periodic interrupt will wake up the 
*          CPU, update the time on the debug LCD & will put the MCU in 
*          standby mode again.
* Operation     : 1. Compile and download the sample code to the target. click 
*          'Reset Go' icon on the 'Debug Run' toolbar to start the 
*          software.
*
*         2. The RTC periodic interrupt (set to occur every second) will
*           display the current time on the debug LCD & will then put 
*          the CPU in the standby mode. 
*******************************************************************************/
/*******************************************************************************
* History     : 05.08.2011  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/
/* Low power consumption RPDL function definitions */
#include "r_pdl_lpc.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* LCD controlling function prototypes & macro definitions */
#include "lcd.h"
/* Contains prototypes of the functions called in this file */
#include "power_down.h"

/******************************************************************************
* Outline      : main
* Description  : Main program function. This function first displays the sample 
*         name and the power mode on the debug LCD, then initialises the 
*         RTC & the standby registers in the 'Init_RTC_STANDBY' function. 
*         The CPU then enters the standby mode. Every second when the
*         RTC periodic interrupt triggers, the MCU will wake up & 
*         display the latest time on the LCD & will enter standby mode
*         again.
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

  /* Declare error flag */
  bool err = true;

  /* Initialise the debug LCD */
  Init_LCD();
  
  /* Display the splash screen */
  Display_LCD(LCD_LINE1, "PWR MODE");
  Display_LCD(LCD_LINE2, "Active  ");
  
  /* Initialise the RTC & then put the device in standby mode */
  Init_PowerDown();
  
  /* Infinite while loop */
  while (1)
  {
    /* Place the MCU in standby mode */
    err &=   R_LPC_Control
        (
          PDL_LPC_MODE_SOFTWARE_STANDBY       
        );  
  }
}
/******************************************************************************
* End of function main
******************************************************************************/
