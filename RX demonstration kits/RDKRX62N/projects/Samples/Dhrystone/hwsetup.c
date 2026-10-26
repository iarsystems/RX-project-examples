/* Adapted for use with IAR Embedded Workbench */
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
* File Name     : HWSetup.c
* Version       : 1.0
* Device(s)     : R5F56108VNFP
* Tool-Chain    : RX Family C Compiler
* OS            : None
* H/W Platform  : YRDKRX62N
* Description   : Hardware setup function - sets initial state of IO pins and system clock
* Limitations   : None
*******************************************************************************
* History : DD.MM.YYYY    Version   Description
*         : 10.Jul.2010   1.00      First release
*******************************************************************************/

/******************************************************************************
Include files
******************************************************************************/
#include "platform.h"
#include  <machine.h>
#include  <iorx62n.h>
#include  "hwsetup.h"

/******************************************************************************
* Function name : HardwareSetup
* Description   : Initializes the board
* Arguments     : None
* Return Value  : None
******************************************************************************/
void HardwareSetup (void)
{

#if ICLK_MHZ==96
  /*
  ICLK = 0 = EXTAL*8 = 12M*8 = 96 MHz
  PCLK = 1 = EXTAL*4 = 12M*4 = 48 MHz
  BCLK = 2 = EXTAL*2 = 12M*2 = 24 MHz
  Enable BCLK output(High level)
  */
  SYSTEM.SCKCR.LONG = (unsigned long)0x00020100;
#elif ICLK_MHZ==48
  /*
  ICLK = 0 = EXTAL*4 = 12M*4 = 48 MHz
  PCLK = 1 = EXTAL*4 = 12M*4 = 48 MHz
  BCLK = 2 = EXTAL*2 = 12M*2 = 24 MHz
  Enable BCLK output(High level)
  */
  SYSTEM.SCKCR.LONG = (unsigned long)0x01020100;
#else
#error INVALID ICLK SETTING!
#endif


  PORT0.DDR.BYTE = 0x00 ;     /* Port 0: inputs (IRQ's from ethernet & WiFi) */
  PORT1.DDR.BYTE = 0x00 ;     /* Port 1: inputs (IIC and USB settings will override these later) */
  PORT2.DDR.BYTE = 0x1A ;     /* Port 2: USB signals */
  PORT3.DDR.BYTE = 0x04 ;     /* Port 3: JTAG (P30, P31, P34), CAN (P32=Tx, P33=Rx), NMI (P35) */
  PORT4.DDR.BYTE = 0x00 ;     /* Port 4: Switches (P40-P42), AIN (P43-P47) */
  PORT5.DDR.BYTE = 0x3B ;     /* Port 5: Audio (P55,P54), BCLK (P53), SCI (P52=Rx, P50=Tx), LCD-RS (P51) */

  PORTA.DR.BYTE  = 0x00 ;     /* Port A outputs all LOW to start */
  PORTA.DDR.BYTE = 0xFF ;     /* Port A: Expansion (PA0-PA2), Ether (PA3-PA5), Audio (PA6-PA7) */

  PORTB.DR.BYTE  = 0x00 ;
  PORTB.DDR.BYTE = 0x70 ;     /* Port B: Ether */

  PORTC.DR.BYTE  = 0xF7 ;     /* Port C: Chip selects, clock = high; IO reset = low */
  PORTC.DDR.BYTE = 0x7F ;     /* Port C: SPI (PC0-2, PC4-7), IO reset (PC3) */

  PORTD.DR.BYTE  = 0xFF ;     /* All LED's off */
  PORTD.DDR.BYTE = 0xFF ;     /* Port D: LED's */

  PORTE.DR.BYTE  = 0xFF ;     /* All LED's off */
  PORTE.DDR.BYTE = 0xFF ;     /* Port E: LED's (PE0-PE3), LCD data D0-D3) (PE4-PE7) */
}
