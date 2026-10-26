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
* History     : 24.06.2010 Ver. 1.00 First Release
******************************************************************************/

/******************************************************************************
System Includes
******************************************************************************/
#include <assert.h>
/******************************************************************************
Project Includes
******************************************************************************/
/* IO Port RPDL function defintions */
#include "r_pdl_io_port.h"
/* CGC RPDL function defintions */
#include "r_pdl_cgc.h"
/* Interrupt function definitions */
#include "r_pdl_intc.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* Contains delcarations for the functions defined in this file */
#include "hwsetup.h"

/* Serial communication for debug output */
#if defined(USE_DEBUG_OUTPUT) && !defined(NDEBUG)
  #include "async.h"
#endif

/******************************************************************************
* Outline     : HardwareSetup
* Description   : Contains all the setup functions called at device restart
* Argument    : none
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
End of HardwareSetup
******************************************************************************/

/******************************************************************************
* Outline     : ConfigureOperatingFrequency
* Description   : Configures the clock settings for each of the device clocks
* Argument    : none
* Return value  : none
******************************************************************************/
void ConfigureOperatingFrequency(void)
{
  volatile bool bRPDLReturn;
  /* Modify the MCU clocks */
  bRPDLReturn = R_CGC_Set
  (
    12E6,
    96E6,
    48E6,
    0,
    PDL_NO_DATA
  );
  assert(true == bRPDLReturn);
  /* 
  Clock Description              Frequency
  ----------------------------------------
  Input Clock Frequency..............12MHz
  Internal Clock Frequency..........96MHz  
  Peripheral Clock Frequency.........48MHz
  External Bus Clock Frequency.......0MHz */
}
/******************************************************************************
End of ConfigureOperatingFrequency
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
  /* Configure the LED pins as outputs */
  R_IO_PORT_Set
  (
    PDL_IO_PORT_0_2|PDL_IO_PORT_0_3|PDL_IO_PORT_0_5,
    PDL_IO_PORT_OUTPUT
  );
  /* Configure the LED pins as outputs */
  R_IO_PORT_Set
  (
    PDL_IO_PORT_3_4,
    PDL_IO_PORT_OUTPUT
  );  
  /* Set LED pin outputs as high (LEDs off) */
  R_IO_PORT_Write
  (
    PDL_IO_PORT_0_2|PDL_IO_PORT_0_3|PDL_IO_PORT_0_5,
    1
  );
  /* Set LED pin outputs as high (LEDs off) */
  R_IO_PORT_Write
  (
    PDL_IO_PORT_3_4,
    1
  );
  
  /* Configure the debug LCD data pins as outputs */
  R_IO_PORT_Set
  (
    PDL_IO_PORT_9_4|PDL_IO_PORT_9_5|PDL_IO_PORT_9_6|PDL_IO_PORT_9_7,
    PDL_IO_PORT_OUTPUT
  );
  /* Set debug LCD data pin outputs as low */
  R_IO_PORT_Write
  (
    PDL_IO_PORT_9_4|PDL_IO_PORT_9_5|PDL_IO_PORT_9_6|PDL_IO_PORT_9_7,
    0
  );
  
  /* Configure the debug LCD control pins as outputs */
  R_IO_PORT_Set
  (
    PDL_IO_PORT_8_4|PDL_IO_PORT_8_5,
    PDL_IO_PORT_OUTPUT
  );
  /* Set the debug LCD control pin outputs as low */
  R_IO_PORT_Write
  (
    PDL_IO_PORT_8_4|PDL_IO_PORT_8_5,
    0
  );

  /* Configure the pin connected to the ADC Pot as an input */
  R_IO_PORT_Set
  (
    PDL_IO_PORT_4_0,
    PDL_IO_PORT_INPUT
  );
}
/******************************************************************************
End of ConfigureOutputPorts
******************************************************************************/

/******************************************************************************
* Outline     : ConfigureInterrupts
* Description   : Configures interrupts used
* Argument    : none
* Return value  : none
******************************************************************************/
void ConfigureInterrupts(void)
{
}
/******************************************************************************
End of ConfigureInterrupts
******************************************************************************/

/******************************************************************************
* Outline     : EnablePeripheralModules
* Description   : Enables and configures peripheral devices on the MCU
* Argument    : none
* Return value  : none
******************************************************************************/
void EnablePeripheralModules(void)
{
  /*Initialise UART for serial Debug messages.*/
#if defined(USE_DEBUG_OUTPUT) && !defined(NDEBUG)
  UART_Init();
#endif
}
/******************************************************************************
End of EnablePeripheralModules
******************************************************************************/
