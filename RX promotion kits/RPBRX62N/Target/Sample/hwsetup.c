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
/* Copyright (C) 2009. Renesas Technology Europe, All Rights Reserved        */
/******************************************************************************
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
/* Switch handler function definitions */
#include "switch.h"
/*Serial communication for debug*/
#ifndef RELEASE
  #include "async.h"
#endif

#include "rskrx62ndef.h"

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
void io_set_cpg(void);
void ConfigurePortPins(void);
void EnableuIPPeripheralModules(void);

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
  
  /* CPG setting */
//  io_set_cpg();

  /* Setup the port pins */
  ConfigurePortPins();
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
  bool bRPDLReturn;
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
    /* Initialise LEDs */
  LED0 = LED_OFF;
  LED1 = LED_OFF;
  LED2 = LED_OFF;
    /* Configure the LED pins */
  R_IO_PORT_Set
  (
    PDL_IO_PORT_A_0|PDL_IO_PORT_A_1|PDL_IO_PORT_A_2,
    PDL_IO_PORT_OUTPUT
  );
  
  /* Enable the joystick inputs */
  R_IO_PORT_Set(PDL_IO_PORT_D_0, PDL_IO_PORT_INPUT | PDL_IO_PORT_INPUT_BUFFER_ON);    /* Center */
  R_IO_PORT_Set(PDL_IO_PORT_D_1, PDL_IO_PORT_INPUT | PDL_IO_PORT_INPUT_BUFFER_ON);    /* Right */
  R_IO_PORT_Set(PDL_IO_PORT_D_2, PDL_IO_PORT_INPUT | PDL_IO_PORT_INPUT_BUFFER_ON);    /* Left */
  R_IO_PORT_Set(PDL_IO_PORT_D_3, PDL_IO_PORT_INPUT | PDL_IO_PORT_INPUT_BUFFER_ON);    /* Down */
  R_IO_PORT_Set(PDL_IO_PORT_D_4, PDL_IO_PORT_INPUT | PDL_IO_PORT_INPUT_BUFFER_ON);    /* Up */
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
  /* Configure switch interrupts */
  InitialiseSwitchInterrupts();
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
  #ifndef RELEASE
    UART_Init();
  #endif

  EnableuIPPeripheralModules();
}
/******************************************************************************
End of EnablePeripheralModules
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdint.h>

#include "hwsetup.h"
#include "hwEthernet.h"
#include "lcd.h"


/******************************************************************************
* Function Name: EnablePeripheralModules
* Description  : Enables Peripheral Modules before use
* Arguments    : none
* Return Value : none
******************************************************************************/
void EnableuIPPeripheralModules(void)
{
  /*  Module standby clear */
  SYSTEM.MSTPCRB.BIT.MSTPB15 = 0;        /* EtherC, EDMAC */
    SYSTEM.MSTPCRA.BIT.MSTPA15 = 0;             /* CMT0 */
}

/******************************************************************************
* Function Name: ConfigurePortPins
* Description  : Configures port pins.
* Arguments    : none
* Return Value : none
******************************************************************************/
void ConfigurePortPins(void)
{
/* Port pins default to inputs. To ensure safe initialisation set the pin states
before changing the data direction registers. This will avoid any unintentional
state changes on the external ports.
Many peripheral modules will override the setting of the port registers. Ensure
that the state is safe for external devices if the internal peripheral module is
disabled or powered down. */

  volatile int i=0;

  R_IO_PORT_Write(PDL_IO_PORT_A_7, 0);
  
  R_IO_PORT_Set
  (
    PDL_IO_PORT_A_7,
    PDL_IO_PORT_OUTPUT
  );
  
  R_IO_PORT_Write(PDL_IO_PORT_A_7, 0);
  for (i=0; i<100000; i++) {}
  R_IO_PORT_Write(PDL_IO_PORT_A_7, 1);

  R_IO_PORT_Set
  (
    PDL_IO_PORT_A_7,
    PDL_IO_PORT_INPUT
  );

  /* ==== MII/RMII Pins setting ==== */
  /*--------------------------------------*/
  /*    Port Function Control Register    */
  /*--------------------------------------*/
#if ETH_MODE_SEL == ETH_MII_MODE
  /*  EE=1, PHYMODE=1, ENETE3=1, ENETE2=0, ENETE1=1, ENETE0=0 (Ethernet)  */
  IOPORT.PFENET.BYTE = 0x9A;
#endif  /*  ETH_MODE_SEL  */
#if ETH_MODE_SEL == ETH_RMII_MODE
  /*  EE=1, PHYMODE=0, ENETE3=0, ENETE2=0, ENETE1=1, ENETE0=0 (Ethernet)  */
  IOPORT.PFENET.BYTE = 0x82;
#endif  /*  ETH_MODE_SEL  */
  /*-------------------------------------------*/
  /*    Input Buffer Control Register (ICR)    */
  /*-------------------------------------------*/
#if ETH_MODE_SEL == ETH_MII_MODE
  /*  PA5=1 Set ET_LINKSTA input  */
  PORTA.ICR.BIT.B1 = 1;
  /*  PA3=1 Set ET_MDIO input  */
  PORTA.ICR.BIT.B3 = 1;
  /*  PB1=1 Set ET_ERXD1 input  */
  PORTB.ICR.BIT.B1 = 1;
  /*  PB0=1 Set ET_ERXD0 input  */
  PORTB.ICR.BIT.B0 = 1;
  /*  PB2=1 Set ET_RX_CLK input  */
  PORTB.ICR.BIT.B2 = 1;
  /*  PB3=1 Set ET_RX_ER input  */
  PORTB.ICR.BIT.B3 = 1;
  /*  PB7=1 Set ET_CRS input  */
  PORTB.ICR.BIT.B7 = 1;
  /*  PC0=1 Set ET_ERXD3 input  */
  PORTC.ICR.BIT.B0 = 1;
  /*  PC1=1 Set ET_ERXD2 input  */
  PORTC.ICR.BIT.B1 = 1;
  /*  PC2=1 Set ET_RX_DV input  */
  PORTC.ICR.BIT.B2 = 1;
  /*  PC4=1 Set EX_TX_CLK input  */
  PORTC.ICR.BIT.B4 = 1;
  /*  PC7=1 Set ET_COL input  */
  PORTC.ICR.BIT.B7 = 1;
#endif  /*  ETH_MODE_SEL  */
#if ETH_MODE_SEL == ETH_RMII_MODE
  /*  P54=1 Set ET_LINKSTA input  */
  PORT5.ICR.BIT.B4 = 1;
  /*  P71=1 Set ET_MDIO input  */
  PORT7.ICR.BIT.B1 = 1;
  /* P74=1 Set RMII_RXD1 input  */
  PORT7.ICR.BIT.B4 = 1;
  /* P75=1 Set RMII_RXD0 input  */
  PORT7.ICR.BIT.B5 = 1;
  /* P76=1 Set REF50CLK input  */
  PORT7.ICR.BIT.B6 = 1;
  /* P77=1 Set RMII_RX_ER input  */
  PORT7.ICR.BIT.B7 = 1;
  /* P83=1 Set RMII_CRS_DV input  */
  PORT8.ICR.BIT.B3 = 1;
#endif  /*  ETH_MODE_SEL  */
}

/******************************************************************************
* Function Name: io_set_cpg
* Description  : Sets up operating speed
* Arguments    : none
* Return Value : none
******************************************************************************/
void io_set_cpg(void)
{
/* Set CPU PLL operating frequencies. Changes to the peripheral clock will require
changes to the debugger and flash kernel BRR settings. */

  /* ==== CPG setting ==== */
  SYSTEM.SCKCR.LONG = 0x00020100;  /* Clockin = 12MHz */
                  /* I Clock = 96MHz, B Clock = 24MHz, */
                  /* P Clock = 48MHz */

}

/******************************************************************************
* Function Name: SetLed0
* Description  : Sets LED0 on or off
* Arguments    : _iOn: 0 for off, 1 for on
* Return Value : none
******************************************************************************/
void SetLed0(int _iOn)
{
  if (_iOn)
  {
    LED0 = LED_ON;
  }
  else
  {
    LED0 = LED_OFF;
  }
}

/******************************************************************************
* Function Name: SetLed1
* Description  : Sets LED1 on or off
* Arguments    : _iOn: 0 for off, 1 for on
* Return Value : none
******************************************************************************/
void SetLed1(int _iOn)
{
  if (_iOn)
  {
    LED1 = LED_ON;
  }
  else
  {
    LED1 = LED_OFF;
  }
}

/******************************************************************************
* Function Name: SetLed2
* Description  : Sets LED2 on or off
* Arguments    : _iOn: 0 for off, 1 for on
* Return Value : none
******************************************************************************/
void SetLed2(int _iOn)
{
  if (_iOn)
  {
    LED2 = LED_ON;
  }
  else
  {
    LED2 = LED_OFF;
  }
}
