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
* File Name    : hwsetup.c
* Version     : 1.00
* Device     : R5F56107VNFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX610
* Description   : Defines the initialisation routines used each time the MCU
*          is restarted. 
******************************************************************************/
/******************************************************************************
* History     : 22.01.2010 Ver. 1.00 First Release
*””FILE COMMENT END””*********************************************************/


/******************************************************************************
User Includes (Project level includes)
******************************************************************************/
/* IO Port RPDL function defintions */
#include "r_pdl_io_port.h"
/* ADC RPDL function defintions */
#include "r_pdl_adc_10.h"
/* SCI RPDL function defintions */
#include "r_pdl_sci.h"
/* Interrupt RPDL function defintions */
#include "r_pdl_intc.h"
/* Timer RPDL function defintions */
#include "r_pdl_tmr.h"
/* CGC RPDL function defintions */
#include "r_pdl_cgc.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* Switch handler function definitions */
#include "switch.h"
/* LCD function definitions */
#include "lcd.h"
/* Contains delcarations for the functions defined in this file */
#include "hwsetup.h"


/*””FUNC COMMENT””*************************************************************
* Outline     : HardwareSetup
* Description   : Contains all the setup functions called at device restart
* Argument    : none
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
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


/*””FUNC COMMENT””*************************************************************
* Outline     : ConfigureOperatingFrequency
* Description   : Configures the clock settings for each of the device clocks
* Argument    : none
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
void ConfigureOperatingFrequency(void)
{
  /* Modify the MCU clocks */
    R_CGC_Set(12.5E6, 100E6, 50E6, 25E6, PDL_CGC_BCLK_DISABLE);
  
  /* 
  Clock Description              Frequency
  ----------------------------------------
  Main Clock Frequency.............12.5MHz
  Internal Clock Frequency..........100MHz  
  Peripheral Clock Frequency.........50MHz
  External Bus Clock Frequency.......25MHz */
}
/******************************************************************************
End of ConfigureOperatingFrequency
******************************************************************************/


/*””FUNC COMMENT””*************************************************************
* Outline     : ConfigureOutputPorts
* Description   : Configures the port and pin direction settings
* Argument    : none
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
void ConfigureOutputPorts(void)
{
    /* Configure the LED control pins */
  R_IO_PORT_Set(PDL_IO_PORT_8_3, PDL_IO_PORT_OUTPUT, 0, 0, 0);
    R_IO_PORT_Set(PDL_IO_PORT_8_4, PDL_IO_PORT_OUTPUT, 0, 0, 0);
    R_IO_PORT_Set(PDL_IO_PORT_3_3, PDL_IO_PORT_OUTPUT, 0, 0, 0);
    R_IO_PORT_Set(PDL_IO_PORT_3_6, PDL_IO_PORT_OUTPUT, 0, 0, 0);
  R_IO_PORT_Write(PDL_IO_PORT_8_3, 1);
  R_IO_PORT_Write(PDL_IO_PORT_8_4, 1);
  R_IO_PORT_Write(PDL_IO_PORT_3_3, 1);
  R_IO_PORT_Write(PDL_IO_PORT_3_6, 1);
  
    /* Configure the LCD control pins */
    R_IO_PORT_Write(RS_PIN, 0);
    R_IO_PORT_Write(E_PIN, 0);
    R_IO_PORT_Write(PDL_IO_PORT_9, 0x00);
    R_IO_PORT_Set(PDL_IO_PORT_9, 0xF0, 0, 0, 0);
    R_IO_PORT_Set(PDL_IO_PORT_8_5, PDL_IO_PORT_OUTPUT, 0, 0, 0);
    R_IO_PORT_Set(PDL_IO_PORT_8_6, PDL_IO_PORT_OUTPUT, 0, 0, 0);

  /* Enable the push switch inputs */
    R_IO_PORT_Set(PDL_IO_PORT_0_0, PDL_IO_PORT_INPUT,
          PDL_IO_PORT_INPUT_BUFFER_ON, 0, 0);
    R_IO_PORT_Set(PDL_IO_PORT_0_1, PDL_IO_PORT_INPUT,
          PDL_IO_PORT_INPUT_BUFFER_ON, 0, 0);
    R_IO_PORT_Set(PDL_IO_PORT_1_3, PDL_IO_PORT_INPUT, 
          PDL_IO_PORT_INPUT_BUFFER_ON, 0, 0);
  
  /* Configure the ADC Pot input */
  R_IO_PORT_Set(PDL_IO_PORT_4_0, PDL_IO_PORT_INPUT, 0,0,0);
}
/******************************************************************************
End of ConfigureOutputPorts
******************************************************************************/


/*””FUNC COMMENT””*************************************************************
* Outline     : ConfigureInterrupts
* Description   : Configures interrupts used
* Argument    : none
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
void ConfigureInterrupts(void)
{
    /* Enable the SW1 interrupt */
    R_INTC_CreateExtInterrupt(PDL_INTC_IRQ8, PDL_INTC_FALLING | PDL_INTC_A,
                SW1_handler, 7);
  
    /* Enable the SW2 interrupt */
  R_INTC_CreateExtInterrupt(PDL_INTC_IRQ9, PDL_INTC_FALLING | PDL_INTC_A,
                SW2_handler, 7);
  
    /* Enable the SW2 interrupt */
     R_INTC_CreateExtInterrupt(PDL_INTC_IRQ3, PDL_INTC_FALLING | PDL_INTC_B,
                SW3_handler, 7);  
}
/******************************************************************************
End of ConfigureInterrupts
******************************************************************************/


/*””FUNC COMMENT””*************************************************************
* Outline     : EnablePeripheralModules
* Description   : Enables and configures peripheral devices on the MCU
* Argument    : none
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
void EnablePeripheralModules(void)
{

}
/******************************************************************************
End of EnablePeripheralModules
******************************************************************************/
