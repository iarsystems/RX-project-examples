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
/*��FILE COMMENT��******************************* Technical reference data ****
* File Name    : timer_capture.c
* Version     : 1.00
* Device     : R5F56107VNFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX610
* Description   : File provides timer captiure functions    
******************************************************************************/
/******************************************************************************
* History     : 22.01.2010 Ver. 1.00 First Release
*��FILE COMMENT END��*********************************************************/


/******************************************************************************
User Includes (Project level includes)
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
/* Timer function definitions */
#include "timer_capture.h"
/* LCD function defintions */
#include "lcd.h"


/******************************************************************************
System Definitions & Global Variables
******************************************************************************/
/* Defines timer status and LCd string variables */
char Timer_Status = 0x0, string[8];
/* Defines count variable */
uint16_t usCount;


/*��FUNC COMMENT��*************************************************************
* Outline     : Timer_Init
* Description   : Initializes the timer channel 
* Argument    : none
* Return value  : none
*��FUNC COMMENT END��*********************************************************/
void Timer_Init(void)
{
  /* Intialise Timer */
  R_TMR_CreatePeriodic(PDL_TMR_UNIT1, PDL_TMR_FREQUENCY, 5, 40,
             PDL_NO_FUNC, PDL_NO_FUNC, 2);
              
  /* Stop Timer & Clear Count */
  R_TMR_ControlUnit(1, PDL_TMR_STOP|PDL_TMR_COUNTER, 0, 0, 0);
  
}
/******************************************************************************
End of function 
******************************************************************************/


/*��FUNC COMMENT��*************************************************************
* Outline     : SW3_callback
* Description   : Function that is exectuted on a SW3 button press. Function 
*          toggles the timer on and off. When turning the timer off, the
*          timer count value is displayed onto the LCD
* Argument    : none
* Return value  : none
*��FUNC COMMENT END��*********************************************************/
void SW3_callback(void)
{
  /* Toggle timer status flag */
  Timer_Status = !Timer_Status;
  
  /* Is the timer off? */
  if(!Timer_Status)
  {
    /* Start Timer */
    R_TMR_ControlUnit(1, PDL_TMR_START, 0, 0, 0);
    
    /* Turn on LED to indicate timer is active */
    R_IO_PORT_Modify(PDL_IO_PORT_8_3, PDL_IO_PORT_AND, 0);
  }
  
  /* Else timer is on */
  else
  {
    /* Read Counter Value */
    R_TMR_ReadUnit(1, PDL_NO_PTR, &usCount, PDL_NO_PTR, PDL_NO_PTR);
    
    /* Stop Timer & Clear Count */
    R_TMR_ControlUnit(1, PDL_TMR_STOP|PDL_TMR_COUNTER, 0, 0, 0);
    
    /* Turn off LED to indicate timer is inactive */
    R_IO_PORT_Modify(PDL_IO_PORT_8_3, PDL_IO_PORT_OR, 1);
    
    /* Convert count value to LCD string */
    ShortToString(usCount);
    
    /* Display count value to LCD */
    DisplayLCD(LCD_LINE2, string);
  }      
}
/******************************************************************************
End of function SW3_callback
******************************************************************************/


/*��FUNC COMMENT��*************************************************************
* Outline     : ShortToString
* Description   : Converts 16bit integer input into ASCII compatiable char
*          string, to be displayed on the LCD.
* Argument    : 16bit unsigned integer
* Return value  : none
*��FUNC COMMENT END��*********************************************************/
void ShortToString(short num)
{
  char a;

  /* "=H'" characters before string of hexadecimals */
  string[0] = ' ';
  string[1] = ' ';
  string[2] = ' ';
  
  /* Forming the string of hexadecimals */
  a = (char)((num & 0x0F00)>> 8);
  string[3] = (a < 0x0A) ? (a+0x30):(a+0x37); 
  a = (char)((num & 0x00F0)>> 4);
  string[4] = (a < 0x0A) ? (a+0x30):(a+0x37); 
  a = (char)(num & 0x000F);
  string[5] = (a < 0x0A) ? (a+0x30):(a+0x37); 

  /* Add two spaces for string termination */
  string[6] = ' ';
  string[7] = ' ';
}
/******************************************************************************
End of function ShortToString
******************************************************************************/
    
    
    
