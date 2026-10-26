/******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Electronics Corp. and is
* only intended for use with Renesas products.

* No other uses are authorized.

* This software is owned by Renesas Electronics Corp. and is
* protected under the applicable laws, including copyright laws.

* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY,
* INCLUDING BUT NOT LIMITED TO WAWRRANTIES OF MERCHANTABILITY,
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
* File Name     : main_crc.c
* Version       : 1.1
* Device(s)     : R5F562N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.1
* OS            : None
* H/W Platform  : YRDKRX62N
* Description   : This sample demonstrates usage of the CRC unit, by echoing
*                 characters typed in the terminal with their checksums.
* Operation     : 1. Build and download the sample code to the YRDK.
*
*                 2. Connect the YRDK to the host PC via an RS232 serial cable.
*
*                 3. Launch a suitable terminal program (e.g. Hyperterminal) and
*                    configure it to operate on the channel the YRDK is connected
*                    to with the following settings:
*
*                                    Baud............19200
*                                    Data Bits...........8
*                                    Parity...........None
*                                    Stop Bits...........1
*                                    Flow Control.....None
*
*                 4. Click 'Reset Go' to start the software.
*
*                 5. Observe the terminal window - instructions will be displayed
*                    at the top, asking the user to input a character.
*
*                 6. Type any letter (printable characters only) into the
*                    terminal, and the character will be echoed back to the
*                    terminal display, along with its corresponding CRC checksum.
* Limitations   : None
*******************************************************************************
* History : DD.MMM.YYYY     Description
*         : 08.Oct.2010     First release
*         : 02.Dec.2010     Second YRDK release
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
#include <stdbool.h>
/* Defines debug LCD functions used in this file */
#include "lcd.h"
/* Defines CRC functions used in this file */
#include "crc.h"

/* RSPI API library support */
#include "YRDKRX62N_RSPI_API.h"

/* Board specific definitions */
#include "YRDKRX62N.h"

#include "machine.h"
#include "intrinsics.h"
#include "hwsetup.h"

/*******************************************************************************
* Outline      : main
* Description  : Main program function. This function first initialises the
*                 debug LCD, and displays the name of the sample on the screen.
*                 The function then calls the Init_CRC initialisation function,
*                 which configures the SCI and CRC units. When the MCU returns
*                 to main, it enters an infinite while loop.
* Argument     : none
* Return value : none
*******************************************************************************/
void main(void)
{
  HardwareSetup();

  /* Enable Interrupt */
  __enable_interrupt();

  /* Initialize RSPI peripheral */
  YRDKRX62N_RSPI_Init(RSPI_CHANNEL_0);

  /* Initialise the debug LCD display */
  InitialiseLCD();

  /* Display instructions on the debug LCD */
  DisplayLCD(LCD_LINE1, "CRC Calc    ");
  DisplayLCD(LCD_LINE3, "Connect to  ");
  DisplayLCD(LCD_LINE4, "serial port ");
  DisplayLCD(LCD_LINE5, "@ 19200 N81 ");
  DisplayLCD(LCD_LINE6, "for more    ");
  DisplayLCD(LCD_LINE7, "instructions");

  /* Initialise the CRC unit */
  Init_CRC();

  /* Infinite while loop */
  while (1);
}
/*******************************************************************************
* End of function main
*******************************************************************************/
