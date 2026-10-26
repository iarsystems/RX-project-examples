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
* File Name    : timer_event.c
* Version     : 1.00
* Device     : R5F56107VNFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX610
* Description   : Defines timer function prototypes    
******************************************************************************/
/******************************************************************************
* History     : 22.01.2010 Ver. 1.00 First Release
*””FILE COMMENT END””*********************************************************/


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
/* LCD function definitions */
#include "lcd.h"
/* Declares function prototypes used in this file */
#include "timer_event.h"

/******************************************************************************
System Definitions & Global Variables
******************************************************************************/
/* Defines LCD display string */
char CountDisplay[8];
/* Defines count value buffer variable */
uint8_t usCount = 0;


/*””FUNC COMMENT””*************************************************************
* Outline     : Init_Timer
* Description   : Initializes timer channels 2 and 3
* Argument    : none
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
void Init_Timer(void)
{
  /* Input capture timer initialisation */
  R_TMR_CreateChannel(2, PDL_TMR_CLK_EXT_FALLING, PDL_NO_DATA, 0, 0,
            0, PDL_NO_FUNC, PDL_NO_FUNC, PDL_NO_FUNC, 0);
  
  /* Display update timer */
  R_TMR_CreatePeriodic(PDL_TMR_TMR3, PDL_TMR_FREQUENCY, 30, 50, PDL_NO_FUNC,
             TMR_callback, 5);
}
/******************************************************************************
End of function 
******************************************************************************/


/*””FUNC COMMENT””*************************************************************
* Outline     : TMR_callback
* Description   : Callback function is executed every period cycle of timer
*          channel 3. Function reads the count value of timer 2 and 
*          displays it on the LCD. 
* Argument    : none
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
void TMR_callback(void)
{
  /* Fetch count value from timer channel 2 */
  R_TMR_ReadChannel(2, PDL_NO_PTR, &usCount, PDL_NO_PTR, PDL_NO_PTR);
    
  /* Convert count value into char string for LCD display */
  ShortToString(usCount);
  
  /* Display the count value onto the LCD */
  DisplayLCD(LCD_LINE2, CountDisplay);
}
/******************************************************************************
End of function TMR_callback
******************************************************************************/


/*””FUNC COMMENT””*************************************************************
* Outline     : ShortToString
* Description   : Converts 16 bit integer into a char string, compatiable with
*          the the LCD module.  
* Argument    : 16bit unsigned integer input
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
void ShortToString(short num)
{
  /* Initialises temporary storage variable */
  char a;
  
  /* Sets the first 3 string elements to blank spaces */
  CountDisplay[0] = ' ';
  CountDisplay[1] = ' ';
  CountDisplay[2] = ' ';
  
  /* Forming the string of hexadecimals */
  a = (char)((num & 0x00F0)>> 4);
  CountDisplay[3] = (a < 0x0A) ? (a+0x30):(a+0x37); 
  a = (char)(num & 0x000F);
  CountDisplay[4] = (a < 0x0A) ? (a+0x30):(a+0x37); 

  /* Add three spaces for string termination */
  CountDisplay[5] = ' ';
  CountDisplay[6] = ' ';
  CountDisplay[7] = ' ';
}
/******************************************************************************
End of function ShortToString
******************************************************************************/
