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
* File Name    : hwsetup.c
* Version     : 1.00
* Device     : R5F562N8
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSK+RX62N
* Description   : Defines the initialisation routines used each time the MCU
*          is restarted. 
******************************************************************************/
/******************************************************************************
* History     : 23.08.2010 Ver. 1.00 First Release
******************************************************************************/

/******************************************************************************
* Project Includes
******************************************************************************/
/* IO Port RPDL function definitions */
#include "r_pdl_io_port.h"
/* CGC RPDL function definitions */
#include "r_pdl_cgc.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* Switch handler function definitions */
#include "switch.h"
/* Contains delcarations for the functions defined in this file */
#include "hwsetup.h"

/******************************************************************************
* Outline     : HardwareSetup
* Description   : Contains all the setup functions called at device restart
* Argument      : none
* Return value  : none
******************************************************************************/
void HardwareSetup(void)
{
  ConfigureOperatingFrequency();
  ConfigureOutputPorts();
  ConfigureInterrupts();
  EnablePeripheralModules();
}
/******************************************************************************
* End of function HardwareSetup
******************************************************************************/

/******************************************************************************
* Outline     : ConfigureOperatingFrequency
* Description   : Configures the clock settings for each of the device clocks
* Argument      : none
* Return value  : none
******************************************************************************/
void ConfigureOperatingFrequency(void)
{
  /* Declare error flag */
  bool err = true;
  
  /* Modify the MCU clocks */
  err &=   R_CGC_Set
      (
        12E6,
        96E6,
        48E6,
        24E6,
        PDL_NO_DATA
      );
  /* 
  Clock Description              Frequency
  ----------------------------------------
  Input Clock Frequency..............12MHz
  Internal Clock Frequency...........96MHz  
  Peripheral Clock Frequency.........48MHz
  External Bus Clock Frequency.......24MHz */
  
  /* Halt in while loop when RPDL errors detected */  
  while(!err);
}
/******************************************************************************
* End of function ConfigureOperatingFrequency
******************************************************************************/

/******************************************************************************
* Outline     : ConfigureOutputPorts
* Description   : Configures the port and pin direction settings, and sets the
*          pin outputs to a safe level.
* Argument    : none
* Return value  : none
******************************************************************************/
void ConfigureOutputPorts(void)
{
  /* Declare error flag */
  bool err = true;
  
    /* Configure the LED pins as outputs */
  err &=  R_IO_PORT_Set
      (
        PDL_IO_PORT_0_2|PDL_IO_PORT_0_3|PDL_IO_PORT_0_5,
        PDL_IO_PORT_OUTPUT
      );
      
  /* Configure the LED pins as outputs */
  err &=  R_IO_PORT_Set
      (
        PDL_IO_PORT_3_4,
        PDL_IO_PORT_OUTPUT
      );  
  
  /* Set LED pin outputs as high (LEDs off) */
  err &= R_IO_PORT_Write
      (
        PDL_IO_PORT_0_2|PDL_IO_PORT_0_3|PDL_IO_PORT_0_5,
        1
      );
      
  /* Set LED pin outputs as high (LEDs off) */
  err &= R_IO_PORT_Write
      (
        PDL_IO_PORT_3_4,
        1
      );
  
  /* Configure the debug LCD data pins as outputs */
  err &=  R_IO_PORT_Set
      (
        PDL_IO_PORT_9_4|PDL_IO_PORT_9_5|PDL_IO_PORT_9_6|
          PDL_IO_PORT_9_7,
        PDL_IO_PORT_OUTPUT
      );
      
  /* Set debug LCD data pin outputs as low */
  err &=  R_IO_PORT_Write
      (
        PDL_IO_PORT_9_4|PDL_IO_PORT_9_5|PDL_IO_PORT_9_6|
          PDL_IO_PORT_9_7,
        0
      );
  
  /* Configure the debug LCD control pins as outputs */
  err &=  R_IO_PORT_Set
      (
        PDL_IO_PORT_8_4|PDL_IO_PORT_8_5,
        PDL_IO_PORT_OUTPUT
      );
      
  /* Set the debug LCD control pin outputs as low */
  err &=  R_IO_PORT_Write
      (
        PDL_IO_PORT_8_4|PDL_IO_PORT_8_5,
        0
      );

  /* Configure the pin connected to the ADC Pot as an input */
  err &=  R_IO_PORT_Set
      (
        PDL_IO_PORT_4_0,
        PDL_IO_PORT_INPUT
      );
  
  /* Halt in while loop when RPDL errors detected */  
  while(!err);
}
/******************************************************************************
* End of function ConfigureOutputPorts
******************************************************************************/

/******************************************************************************
* Outline     : ConfigureInterrupts
* Description   : Configures interrupts used
* Argument      : none
* Return value  : none
******************************************************************************/
void ConfigureInterrupts(void)
{
  /* Configure switch interrupts */
  InitialiseSwitchInterrupts();
}
/******************************************************************************
* End of function ConfigureInterrupts
******************************************************************************/

/******************************************************************************
* Outline     : EnablePeripheralModules
* Description   : Enables and configures peripheral devices on the MCU
* Argument    : none
* Return value  : none
******************************************************************************/
void EnablePeripheralModules(void)
{

}
/******************************************************************************
* End of function EnablePeripheralModules
******************************************************************************/
