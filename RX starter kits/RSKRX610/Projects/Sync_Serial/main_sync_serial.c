/* Adapted for use with IAR Embedded Workbench */
/******************************************************************************
* DISCLAIMER:
* The software supplied by Renesas Technology Europe Ltd is
* intended and supplied for use on Renesas Technology products.
* This software is owned by Renesas Technology Europe, Ltd. Or
* Renesas Technology Corporation and is protected under applicable
* copyright laws. All rights are reserved.
*
* THIS SOFTWARE IS PROVIDED "AS IS". NO WARRANTIES, WHETHER EXPRESS,
* IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* APPLY TO THIS SOFTWARE. RENESAS TECHNOLOGY AMERICA, INC. AND
* AND RENESAS TECHNOLOGY CORPORATION RESERVE THE RIGHT, WITHOUT
* NOTICE, TO MAKE CHANGES TO THIS SOFTWARE. NEITHER RENESAS
* TECHNOLOGY AMERICA, INC. NOR RENESAS TECHNOLOGY CORPORATION SHALL,
* IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
* CONSEQUENTIAL DAMAGES FOR ANY REASON WHATSOEVER ARISING OUT OF THE
* USE OR APPLICATION OF THIS SOFTWARE.
******************************************************************************/
/* Copyright (C) 2009. Renesas Technology Europe, All Rights Reserved */
/*””FILE COMMENT””******************************* Technical reference data ****
* File Name    : main_sync_serial.c
* Version     : 1.00
* Device     : R5F56107VNFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX610
* Description   : Demonstration of the SCI module, in synchronous mode. Program
*          transmits a data string from one unit to another in loopback
*          mode.
* Instructions  : 1. Connect the following header pins together:
*
*          | Header | Pin |  Info    |    | Header | Pin |  Info    |
*          |    J1   | 30  | SCI1 Clk | -> |   J2   | 6   | SCI3 Clk |
*          |    J1   | 31  | SCI1 Tx  | -> |   J2   | 4   | SCI3 Rx  |
*          |   J1   | 32  | SCI1 Rx  | -> |   J2   | 2   | SCI3 Tx  |
*
*          2. Compile the sample code, and download to the RSK.
*
*          3. Right click the variable gSCI_ReceiveBuffer, and select
*             'Add to Watch'.
*
*          4. Click the 'Go' button to start the sample software.
*
*          5. Observe the variable gSCI_ReceiveBuffer, and watch it
*           load with the contents of the variable gSCI_TransmitBuffer
*           - if this does not occur, the serial transmission has
*           failed.
*
*          6. The transmitted data string will also be displayed on the
*             LCD.
******************************************************************************/
/******************************************************************************
* History     : 22.01.2010 Ver. 1.00 First Release
*””FILE COMMENT END””*********************************************************/

/******************************************************************************
User Includes (Project level includes)
******************************************************************************/
/* LCD function definitions */
#include "lcd.h"
/* WDT function definitions */
#include "sync_serial.h"
/* IO Port RPDL function definitions */
#include "r_pdl_io_port.h"
/* ADC RPDL function definitions */
#include "r_pdl_adc_10.h"
/* SCI RPDL function definitions */
#include "r_pdl_sci.h"
/* Interrupt RPDL function definitions */
#include "r_pdl_intc.h"
/* Timer RPDL function definitions */
#include "r_pdl_tmr.h"
/* CGC RPDL function definitions */
#include "r_pdl_cgc.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"


/******************************************************************************
System Definitions & Global Variables
******************************************************************************/
/* SCI transmit data buffer */
uint8_t gSCI_TransmitBuffer[8] = "Renesas ";
/* SCI receive data buffer */
uint8_t gSCI_ReceiveBuffer[8];


/*””FUNC COMMENT””*************************************************************
* Outline     : main
* Description   : The main program function. Function displays the splash
*          screen on the LCD display, calls the SCI intialisation
*          routine, transmits the contents of the global variable
*          gSCI_TransmitBuffer, then waits in an infinite while loop.
* Argument    : none
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
#include "intrinsics.h"
#include "hwsetup.h"

void main(void)
{
  HardwareSetup();

  /* Enable Interrupt */
  __enable_interrupt();

  /* Initialise the LCD Display */
  InitialiseLCD();

  /* Display Splash Screen */
  DisplayLCD(LCD_LINE1, "Sync SCI");

  /* Initialise the SCI module */
  SCI_Init();

  /* Send the data string 'Renesas ' */
  SCI_Send();

  /* Infinite While Loop */
  while (1);
}
/******************************************************************************
End of function main
******************************************************************************/
