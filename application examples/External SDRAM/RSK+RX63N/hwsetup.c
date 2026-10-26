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
/* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.   */
/*******************************************************************************
* File Name     : hwsetup.c
* Version       : 1.00
* Device        : R5F563NB
* Tool-Chain    : Renesas RX Standard 1.2.0.0
* H/W Platform  : RSK+RX63N
* Description   : Defines the hardware initialisation routines used each time
*                  the MCU is restarted.
*******************************************************************************/
/*******************************************************************************
* History       : 13 Aug. 2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* System Includes
*******************************************************************************/
/* Includes assembley level definitions */
#include <machine.h>
/* Following header file provides C99 type definitions. */
#include <stdint.h>

/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/
/* Defines RX63N port registers */
#include "iorx63n.h"
/* Defines macros relating to the RX63N user LEDs and switches */
#include "rskrx63ndef.h"
/* LCD controlling function prototypes & macro definitions */
#include "lcd.h"
/* Switch handler function definitions */
#include "switch.h"
/* Contains declarations for the functions defined in this file */
#include "hwsetup.h"

#define REFW          12
#define RFC            0
#define EXENB          0

/*******************************************************************************
* Outline       : HardwareSetup
* Description   : Contains all the setup functions called at device restart
* Argument      : none
* Return value  : none
*******************************************************************************/
void HardwareSetup(void)
{
    ConfigureOperatingFrequency();
    ConfigureOutputPorts();
    ConfigureInterrupts();
    EnablePeripheralModules();
    ConfigureSDRAM();
}
/*******************************************************************************
* End of function HardwareSetup
*******************************************************************************/

/*******************************************************************************
* Outline       : ConfigureOperatingFrequency
* Description   : Configures the clock settings for each of the device clocks
* Argument      : none
* Return value  : none
*******************************************************************************/
void ConfigureOperatingFrequency(void)
{

  unsigned int i;
  SYSTEM.PRCR.WORD     = 0xA50B;           // Protect off
  SYSTEM.SOSCCR.BYTE   = 0x00;             // Enable sub-clock for RTC
  SYSTEM.HOCOPCR.BYTE  = 0x00;             // Enable HOCO
  SYSTEM.MOSCWTCR.BYTE = 0x0E;             // Main Clock Oscillator Wait Control Register (262144 states)
  SYSTEM.PLLWTCR.BYTE  = 0x0E;             // PLL Wait Control Register (2097152 states)
  SYSTEM.MOSCCR.BYTE   = 0x00;             // EXTAL ON
  SYSTEM.PLLCR2.BYTE   = 0x00;             // PLL OFF
  SYSTEM.PLLCR.WORD    = 0x0F00;           // x16 @PLL
  SYSTEM.MOSCCR.BYTE   = 0x00;             // EXTAL ON
  SYSTEM.PLLCR2.BYTE   = 0x00;             // PLL ON
  for(i = 0;i< 2500;i++) {    // Wait over 20ms
  asm("nop");
  }

  //
  // SCKCR - System Clock Control Register
  //  FCK[3:0]    0x02 = Flash clock: PLL/4 = (192 / 4) = 48 MHz
  //  ICK[3:0]    0x01 = System clock: PLL/2 = (192 / 2) = 96 MHz
  //  PSTOP1      0x00 = BCLK pin output is enabled
  //  BCK[3:0]    0x03 = BCLK: PLL/8 = 24 MHz
  //  PCKB[3:0]   0x02 = Peripheral clock B: PLL/4 = 48 MHz
  //
  SYSTEM.SCKCR.LONG = 0x21031222;          // ICK=PLL/2,BCK,FCK,PCK=PLL/4
  //  SCKCR = 0x21842222;      //
  //
  // SCKCR2 - System Clock Control Register 2
  //  UCK[3:0]    0x03 = USB clock is PLL/4 = 48 MHz
  //  IEBCK[3:0]  0x01 = IE Bus clock is PLL/2 = 96 MHz
  //
  SYSTEM.SCKCR2.WORD = 0x0031;
  SYSTEM.SCKCR3.WORD = 0x0400;             // ICLK, PCLKB, FCLK, BCLK, IECLK, and USBCLK all come from PLL circuit */

  //
  // Set MSTPCRA, MSTPCRB and MSTPCRC to default setting
  //
  SYSTEM.MSTPCRA.LONG = 0x47FFFFFFuL;      // Set to initial value (Reset condition).
  SYSTEM.MSTPCRB.LONG = 0xFFFFFFFFuL;      // All modules in MSTPCRB stop.
  SYSTEM.MSTPCRC.LONG = 0xFFFF0000uL;      // b0: RAM0, 0 => enabled
                               // b1: RAM1, 0 => enabled


}
/*******************************************************************************
* End of function ConfigureOperatingFrequency
*******************************************************************************/

/*******************************************************************************
* Outline       : ConfigureOutputPorts
* Description   : Configures the port and pin direction settings, and sets the
*                  pin outputs to a safe level.
* Argument      : none
* Return value  : none
*******************************************************************************/
void ConfigureOutputPorts(void)
{
  /* Configure the LED pins (LED0-LED3) as outputs */
  LED0_PIN_DIR = OUTPUT_PIN;
  LED1_PIN_DIR = OUTPUT_PIN;
  LED2_PIN_DIR = OUTPUT_PIN;
  LED3_PIN_DIR = OUTPUT_PIN;

  /* Set LED pin outputs as high (all LEDs off) */
  LED0 = LED_OFF;
  LED1 = LED_OFF;
  LED2 = LED_OFF;
  LED3 = LED_OFF;

  /* Configure the debug LCD data pins (DLCD7-DLCD4) as outputs */
  LCD_DATA_DIR_PORT = 0xF0;

  /* Set debug LCD data pin (DLCD7-DLCD4) outputs as low */
  LCD_DATA_PORT = 0x00;

  /* Configure the debug LCD control pin (DLCDRS) and enable pin (DLCDE)
  as output */
  RS_PIN_DIR = 0x1;
  E_PIN_DIR  = 0x1;

  /* Set the debug LCD control pin (DLCDRS) and enable pin (DLCDE) as output
  low */
  RS_PIN = 0x0;
  E_PIN  = 0x0;
}
/*******************************************************************************
* End of function ConfigureOutputPorts
*******************************************************************************/

/*******************************************************************************
* Outline     : ConfigureInterrupts
* Description   : Configures the interrupts used
* Argument    : none
* Return value  : none
*******************************************************************************/
void ConfigureInterrupts(void)
{
  /* Configure switch interrupts */
  InitialiseSwitchInterrupts();
}
/*******************************************************************************
* End of function ConfigureInterrupts
*******************************************************************************/

/*******************************************************************************
* Outline     : EnablePeripheralModules
* Description   : Enables and configures peripheral devices on the MCU
* Argument    : none
* Return value  : none
*******************************************************************************/
void EnablePeripheralModules(void)
{
  /* Peripherals initialised in specific initialisation functions */
}

/*******************************************************************************
* Outline     : ConfigureSDRAM
* Description   : Enables and configures SDRAM on the RSK+RX63N
* Argument    : none
* Return value  : none
*******************************************************************************/
void ConfigureSDRAM(void)
{
  unsigned int i;

  //
  // SDRAM Initialization
  //
  MPC.PWPR.BYTE |= 0x00;          // Disable write protect

  MPC.PWPR.BYTE |= 0x40;          // Enable write access


  MPC.PFAOE0.BYTE |= 0xFF;        // Enable A8-A15

  MPC.PFBCR0.BYTE |= (1 << 0);    // Enable A0-A7
  MPC.PFBCR0.BYTE |= (1 << 4);    // Enable D8 to D15
  MPC.PFBCR0.BYTE |= (1 << 6);    // Configures P5.1 as the WR1# or BC1# pin

  MPC.PFCSE.BYTE  |= (1 << 2);    // Enable CS3
  MPC.PFCSS0.BYTE |= (1 << 6);    // CS3 = P7.3

  MPC.PFBCR1.BYTE |=(1 << 7);     // Enable SDCLK Output
  MPC.PFBCR1.BYTE |=(1 << 6);     // Bit 6
  MPC.PFBCR1.BYTE |=(1 << 4);     // Bit 4 Bit 6
                                  //   0    x     : Only the P70/SDCLK pin is controled by the SDCLKE bit
                                  //   1    0     : SDRAM is enabled except for the P67/DQM1 pin
                                  //   1    1     : SDRAM is enabled (for all the pins)
  MPC.PFBCR1.BYTE |=(1 << 0);     // WAIT# = P5.5

  i = 100*100;
  while (--i > 0);                // Delay at least 100uS after SDCLK active.

  BSC.CS3CR.WORD  |= 0x0001;      // CS3 is enabled
                                  // 16 bit data
                                  // Endian of external data same as CPU
                                  // Seperate address and data (not multiplexed)

  BSC.CS3MOD.WORD |= 0x0000;      // Byte strobe mode
                                  // External wait disabled
                                  // Page read access disabled
                                  // Page write access disabled
                                  // Normal access compatible mode

  //
  // Configure SDRAM
  //
  BSC.SDIR.WORD   |= 0x00F0;              // SDRAM initialization...AutoRefresh interval=3 cycles, Precharge cycles count=3cycles, 15 times
  BSC.SDICR.BYTE  |= 0x01;                // Start SDRAM initialization
  while((BSC.SDSR.BYTE & 0x08) == 0x08);  // Wait for initialization complete

  BSC.SDCMOD.BYTE  = 0x00;                // Set endian to little
  BSC.SDCCR.BYTE   = 0x00;                // Set SDRAM to 16bit access
  BSC.SDMOD.WORD   = 0x0230;              // Set write single location, CL=2
  BSC.SDRFCR.WORD  = (4uL << REFW) | (719uL << RFC);
  BSC.SDTR.LONG    = 0x000003;            // Set RAS=1, RCD=1, RP=1, WR=1, CL=2, cycles
  BSC.SDADR.BYTE   = 0;                   // Set shift to 8 bits
  BSC.SDRFEN.BYTE  = 1;                   // Enable auto refresh
  BSC.SDCCR.BYTE  |= (1uL << EXENB);      // Enable SDRAM operation...this should be last operation

  SYSTEM.SYSCR0.WORD = 0x5A03;            // Enable the external bus...must also set key and internal ROME bit

  MPC.PWPR.BYTE    = 0x80;                // Enable write protect
  SYSTEM.PRCR.WORD = 0xA500;              // Protect on
}
/*******************************************************************************
* End of function EnablePeripheralModules
*******************************************************************************/
