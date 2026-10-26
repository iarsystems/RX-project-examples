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
* File Name    : power_down.c
* Version     : 1.00
* Device     : R5F56107VNFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX610
* Description   : Defines functions used for MCU power down functionality
******************************************************************************/

/******************************************************************************
* History     : 30.10.2008 Ver. 1.00 First Release
*””FILE COMMENT END””*********************************************************/

/******************************************************************************
User Includes
******************************************************************************/
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
/* LPC RPDL function definitions */
#include "r_pdl_lpc.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* Defines functions used in the switch interrupts */
#include "switch.h"
/* Defines LCD functions used in this file */
#include "lcd.h"
/* Declares functions defined in this file */
#include "power_down.h"


/******************************************************************************
Global Variables
******************************************************************************/
/* Sleep flag variable, used to indicate when to transition to sleep mode */
unsigned short SleepFlag = 0x0;


/*””FUNC COMMENT””*************************************************************
* Outline     : PowerDown_Init
* Description   : Function initialises the MCU sleep mode registers.      
* Argument    : none
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
void PowerDown_Init(void)
{
  /* Intialise low power mode registers */
  R_LPC_Create(PDL_NO_DATA, PDL_NO_DATA);
}
/******************************************************************************
End of function PowerDown_Init
******************************************************************************/


/*””FUNC COMMENT””*************************************************************
* Outline     : PowerDown_Sleep
* Description   : Function turns off the LEDs, displays the 'Sleep' on the LCD
          and puts the MCU into sleep mode.    
* Argument    : none
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
void PowerDown_Sleep(void)
{  
  /* Declare wait counter variable */
  long wait = 0xFFFFF;

  /* Turn off all user LEDs */
  R_IO_PORT_Write(PDL_IO_PORT_8_3, 1);
  R_IO_PORT_Write(PDL_IO_PORT_8_4, 1);
  R_IO_PORT_Write(PDL_IO_PORT_3_3, 1);
  R_IO_PORT_Write(PDL_IO_PORT_3_6, 1);
  
  /* Display MCU mode on LCD */
  DisplayLCD(LCD_LINE2, " Sleep  ");
  
  /* Put the MCU into sleep mode */
  R_LPC_Control(PDL_LPC_MODE_SLEEP);
  
  /* Wait for MCU to shutdown */
  while(wait--);
  
  /* Program continues here after wake up */
  
  /* Display MCU mode on LCD */
  DisplayLCD(LCD_LINE2, " Active ");
}
/******************************************************************************
End of function PowerDown_Sleep
******************************************************************************/


/*””FUNC COMMENT””*************************************************************
* Outline     : ToggleLEDs
* Description   : Function waits for approximately 100ms, then toggles the
*          state of the user LEDs.
* Argument    : none
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
void ToggleLEDs(void)
{
  /* Declare wait counter variable */
  long wait = 0xFFFFF;
  
  /* Wait in while loop until counter variable reaches 0 */
  while(wait--);
  
  /* Toggle LEDs 0 - 3 */
  R_IO_PORT_Modify(PDL_IO_PORT_8_3, PDL_IO_PORT_XOR, 1);
  R_IO_PORT_Modify(PDL_IO_PORT_8_4, PDL_IO_PORT_XOR, 1);
  R_IO_PORT_Modify(PDL_IO_PORT_3_3, PDL_IO_PORT_XOR, 1);
  R_IO_PORT_Modify(PDL_IO_PORT_3_6, PDL_IO_PORT_XOR, 1);
}
/******************************************************************************
End of function ToggleLEDs
******************************************************************************/
