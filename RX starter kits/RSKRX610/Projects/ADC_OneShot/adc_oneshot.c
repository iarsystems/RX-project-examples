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
/* Copyright (C) 2008. Renesas Technology Europe, All Rights Reserved */
/*””FILE COMMENT””******************************* Technical reference data ****
* File Name    : adc_oneshot.c
* Version     : 1.00
* Device     : R5F56107VNFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX610
* Description   : Defines functions used AD conversion     
******************************************************************************/

/******************************************************************************
* History     : 22.01.2010 Ver. 1.00 First Release
*””FILE COMMENT END””*********************************************************/

/******************************************************************************
User Includes (Project level includes)
******************************************************************************/
/* IO Port RPDL function defintions */
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
/* Switch handler function definitions */
#include "switch.h"
/* LCD function definitions */
#include "lcd.h"
/* ADC oneshot function definitions */
#include "adc_oneshot.h"


/******************************************************************************
Global Variables
******************************************************************************/
/* KeyPress flag indicates which SW (1 - 3) has been pressed */
extern unsigned short KeyPress;
/* Char string declaration, to store LCD text */
char ad_string[8];
/* Global variable, ADC result container */
extern uint16_t usADC_Result;


/*””FUNC COMMENT””*************************************************************
* Outline     : ADC_Init
* Description   : Initializes the ADC periperhal
* Argument    : none
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
void ADC_Init(void)
{
  /* Configure the ADC channel that is connected to the potentiometer. */
    R_ADC_10_Create(0, PDL_ADC_10_MODE_SINGLE | PDL_ADC_10_CHANNELS_OPTION_1,
          50E6, 6E-7, PDL_NO_FUNC, 7);
}
/******************************************************************************
End of function ADC_Init
******************************************************************************/


/*””FUNC COMMENT””*************************************************************
* Outline     : ReadADC
* Description   : Reads the value of the ADC, converts it to an ASCII string
*          and displays it onto the LCD.
* Argument    : none
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
void ReadADC(void)
{  
    /* Start AD conversion */
    R_ADC_10_Control(PDL_ADC_10_0_ON );
    
    /* Fetch ADC value */
    R_ADC_10_Read(0, &usADC_Result);
    
    /* Convert the ADC result into a string */
    ShortToString(usADC_Result);
  
    /* Display the ADC result onto the LCD */
    DisplayLCD(LCD_LINE2, ad_string);
}
/******************************************************************************
End of function ReadADC
******************************************************************************/


/*””FUNC COMMENT””*************************************************************
* Outline     : ShortToString
* Description   : Converts a 16bit integer into an ASCII char string, for
*          displaying on an LCD
* Argument    : 16bit unsigned integer to convert
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
void ShortToString(uint16_t num)
{
  /*Declare temporary char variable */
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
