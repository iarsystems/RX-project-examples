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
* File Name    : main_async.c
* Version     : 1.00
* Device     : R5F56107VNFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX610
* Description   : Demonstration of SCI in asynchronous mode. The program
*            communicates from the RX610 to a terminal program via SCI and
*            RS-232.
* Instructions  : 1. Connect the RSK to a PC via a 9-way, straight through
*           serial cable. Open a terminal program (e.g HyperTerminal).
*
*          2. Ensure the terminal program connection settings are as
*             follows:
*
*           Baud Rate.............19200
*           Data Bits.................8
*           Parity.................None
*           Stop Bits.................1
*           Flow Control...........None
*
*          3. Compile and download the sample code. Click 'Go' to
*           start the software.
*
*          4. Observe the output on the terminal window. The numbers 0
*           to 9 will appear repeatedly. Type 'z' (lowercase) into the
*           terminal to pause the UART writes, and any other key to
*           resume.
*
*          5. While data is being transmitted, LED0 will be on and LED1
*             will be off. When transmission is inactive, LED1 will be
*           on and LED0 will be off.
******************************************************************************/
/******************************************************************************
* History     : 22.01.2010 Ver. 1.00 First Release
*””FILE COMMENT END””*********************************************************/

/******************************************************************************
User Includes (Project level includes)
******************************************************************************/
/* IO Port RPDL function definitions */
#include "r_pdl_io_port.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* LCD function definitions */
#include "lcd.h"
/* Async UART function definitions */
#include "async.h"


/******************************************************************************
System Definitions & Global Variables
******************************************************************************/
/* Global varaible rx_flag declaration */
extern unsigned short rx_flag;


/*””FUNC COMMENT””*************************************************************
* Outline     : main
* Description   : The main program function. Function calls the UART
          initialisation routine, displays instructions on the LCD,
          then enters an infinite while loop. Within the while loop,
          the function calls the Text_Write function with each loop
          iteration. If the rx_flag is raised (from a z press on
          terminal) the function turns on LED1 and waits for another
          key press.
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

  /* Initialises the UART device */
  UART_Init();

  /* Displays the Renesas splash screen */
  DisplayLCD(LCD_LINE1, " SERIAL  ");
  DisplayLCD(LCD_LINE2, "  UART  ");

  /* Defines an infinte loop to keep the MCU running */
  while(1)
  {
    /* Transmits an incrementing number across the UART */
    Text_Write();

    /* Defines a while loop that halts the program until
       a key is pressed. Test is via the rx_flag */
    while(rx_flag)
    {
      /* Turn on LED1 to indicate UART RX */
      R_IO_PORT_Write(PDL_IO_PORT_8_3, 1);
      R_IO_PORT_Write(PDL_IO_PORT_8_4, 0);

      /* Wait in this loop untill rx_flag is reset */
    }
  }
}
/******************************************************************************
End of function main
******************************************************************************/
