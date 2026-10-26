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
* File Name    : adc_repeat.c
* Version     : 1.00
* Device     : R5F56107VNFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX610
* Description   : Defines functions used for ADC repeat function
*         
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
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* Defines functions used in the switch interrupts */
#include "switch.h"
/* Defines LCD functions used in this file */
#include "lcd.h"
/* Declares functions defined in this file */
#include "adc_repeat.h"


/******************************************************************************
Global Variables
******************************************************************************/
/* Char array container for ADC display string result */
char ad_string[8];
/* Global variable, ADC result container */
extern uint16_t usADC_Result;


/*””FUNC COMMENT””*************************************************************
* Outline     : ADC_Init
* Description   : ADC initialisation function
* Argument    : none
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
void ADC_Init(void)
{
  /* Configure the ADC channel that is connected to the potentiometer. A
     callback function 'ADC_callback' is set to be called everytime the ADC 
     completes a conversion */
    R_ADC_10_Create(0, PDL_ADC_10_CHANNELS_OPTION_1|PDL_ADC_10_TRIGGER_TMR,
          50E6, 6E-7, ADC_callback, 7);
  
  /* Configure timer */                     
  R_TMR_CreatePeriodic(PDL_TMR_TMR2, PDL_TMR_FREQUENCY, 50, 50, TMR_callback,
             PDL_NO_FUNC, 5);
}
/******************************************************************************
End of function ADC_Init
******************************************************************************/


/*””FUNC COMMENT””*************************************************************
* Outline     : ShortToString
* Description   : Converts a 16 bit interger, into an ASCII string
* Argument    : unsigned 16bit interger (Value to be converted)
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
void ShortToString(uint16_t num)
{
  /* Temporary container variable declaration */
  char a;

  /* "=H'" characters before string of hexadecimals */
  ad_string[0] = '=';
  ad_string[1] = 'H';
  ad_string[2] = '\'';
  
  /* Forming the string of hexadecimals */
  a = (char)((num & 0x0F00)>> 8);
  ad_string[3] = (a < 0x0A) ? (a+0x30):(a+0x37); 
  a = (char)((num & 0x00F0)>> 4);
  ad_string[4] = (a < 0x0A) ? (a+0x30):(a+0x37); 
  a = (char)(num & 0x000F);
  ad_string[5] = (a < 0x0A) ? (a+0x30):(a+0x37); 

  /* Add two spaces for string termination */
  ad_string[6] = ' ';
  ad_string[7] = ' ';
}
/******************************************************************************
End of function ShortToString
******************************************************************************/


/*””FUNC COMMENT””*************************************************************
* Outline     : TMR_callback
* Description   : Timer callback function, which called at the end of each
*          timer period. When executed, it initiates the AD conversion.
* Argument    : none
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
void TMR_callback(void)
{
  /* Start A/D conversion */
  R_ADC_10_Control(PDL_ADC_10_0_ON);  
}
/******************************************************************************
End of function TMR_callback
******************************************************************************/


/*””FUNC COMMENT””*************************************************************
* Outline     : ADC_callback
* Description   : ADC callback function, which is called upon successful AD
*          conversion. When executed, it fetches the ADC value and 
*          displays it on the LCD screen.
* Argument    : none
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
void ADC_callback(void)
{  
  /* fetch ADC value */
  R_ADC_10_Read(0, &usADC_Result);
  
  /* Convert the ADC result into a string */
  ShortToString(usADC_Result);
  
  /* Display the ADC result onto the LCD */
  DisplayLCD(LCD_LINE2, ad_string);
}
/******************************************************************************
End of function ADC_callback
******************************************************************************/
