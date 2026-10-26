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
* File Name    : WDT.c
* Version     : 1.00
* Device     : RX610
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX610
* Description   : Defines WDT functions    
******************************************************************************/
/******************************************************************************
* History     : 22.01.2010 Ver. 1.00 First Release
*””FILE COMMENT END””*********************************************************/


/******************************************************************************
User Includes (Project level includes)
******************************************************************************/
/* PDL function definitions */
#include "r_pdl_io_port.h"
#include "r_pdl_adc_10.h"
#include "r_pdl_sci.h"
#include "r_pdl_intc.h"
#include "r_pdl_tmr.h"
#include "r_pdl_cgc.h"
#include "r_pdl_wdt.h"
#include "r_pdl_definitions.h"
/* Switch handler function definitions*/
#include "switch.h"
/* LCD function defintions */
#include "lcd.h"
/* Declares functions defined in this file */
#include "WDT.h"


/*””FUNC COMMENT””*************************************************************
* Outline     : WDT_Init
* Description   : Initializes the watch dog timer
* Argument    : none
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
void WDT_Init(void)
{
  /* Initialise timer unit to read ADC and reset WDT */
  R_TMR_CreateUnit(1, PDL_TMR_CLK_PCLK_DIV_8192| PDL_TMR_CLEAR_CM_A,0, 0,
           0xF, 0, PDL_NO_FUNC, TMR_Callback, PDL_NO_FUNC, 4);
          
  
  /* Initialise WDT */
  R_WDT_Create(PDL_WDT_PCLK_DIV_32768, WDT_Reset, 6);
  
  /* Initialise ADC to tigger from timer unit */
  R_ADC_10_Create(0, PDL_ADC_10_MODE_SINGLE | PDL_ADC_10_CHANNELS_OPTION_1,
          50E6, 6E-7, ADC_Callback, 5);
}
/******************************************************************************
End of function WDT_Init
******************************************************************************/


/*””FUNC COMMENT””*************************************************************
* Outline     : WDT_Reset
* Description   : Function is called when the WDT timer times out. Function
*          sets the LEDs all on staticly, and sits in an infinite loop.
* Argument    : none
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
/* Callback function to reset the WDT */
void WDT_Reset(void)
{
  /* Set LEDs on */
  R_IO_PORT_Modify(PDL_IO_PORT_8, PDL_IO_PORT_AND, 0xE7);
  R_IO_PORT_Modify(PDL_IO_PORT_3, PDL_IO_PORT_AND, 0xB7);
  
  /* Lock into infinite loop */
  while(1);
}
/******************************************************************************
End of function WDT_Reset
******************************************************************************/


/*””FUNC COMMENT””*************************************************************
* Outline     : TMR_Callback
* Description   : Callback function is called every period cycle of timer unit
*          1. Function intiates an AD conversion.
* Argument    : none
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
void TMR_Callback(void)
{
  /* Start A/D conversion */
  R_ADC_10_Control(PDL_ADC_10_0_ON);
}
/******************************************************************************
End of function TMR_Callback
******************************************************************************/


/*””FUNC COMMENT””*************************************************************
* Outline     : ADC_Callback
* Description   : Callback function executes after a succesful AD conversion.
*          function resets the WDT, fetches the ADC value and sets the
*          timer unit 1 frequency based on the ADC value.
* Argument    : none
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
void ADC_Callback(void)
{
  /* Initialises a temperary storage variable */
  uint16_t ad_value = 0x0;
  
  /* Reset the WDT counter */
  R_WDT_Control(PDL_WDT_RESET_COUNTER);
  
  /* Read the ADC value */
  R_ADC_10_Read(0, &ad_value);
  
  /* Toggle LEDs */
  R_IO_PORT_Modify(PDL_IO_PORT_8, PDL_IO_PORT_XOR, 0x18);
  R_IO_PORT_Modify(PDL_IO_PORT_3, PDL_IO_PORT_XOR, 0x48);
  
  /* Add a minimium value to ad_value to make flashes visible */
  ad_value += 0xF0;
  
  /* Update the timer interval */
  R_TMR_ControlUnit(1 , PDL_TMR_TIME_CONSTANT_A, 0, ad_value, 0);
}
/******************************************************************************
End of function ADC_Callback
******************************************************************************/

