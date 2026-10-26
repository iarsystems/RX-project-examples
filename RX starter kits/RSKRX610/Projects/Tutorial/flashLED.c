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
* File Name    : flashLED.c
* Version     : 1.00
* Device     : RX610
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX610
* Description   : Defines flashing LED functions      
******************************************************************************/
/******************************************************************************
* History     : 22.01.2010 Ver. 1.00 First Release
*””FILE COMMENT END””*********************************************************/


/*””FILE COMMENT””******************************* Technical reference data ****
* File Name    : flashLED.c
* Version     : 1.00
* Device     : RX610
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX610
* Description   : Defines functions used for flashing the LEDs
*         
******************************************************************************/

/******************************************************************************
* History     : 30.10.2008 Ver. 1.00 First Release
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
/* Flashing LED function definitions */
#include "flashLED.h"
/* LCD function definitions */
#include "lcd.h"


/**********************************************************************************
System Definitions & Global Variables
***********************************************************************************/
/* The global variable KeyPress is used as a flag whenever a switch is pressed */
extern unsigned short KeyPress;
/* Variable used to count down the number of LED flashes */
unsigned short flash_count = 0x190;


/*””FUNC COMMENT””*************************************************************
* Outline     : FlashLED
* Description   : The flash function used at the beginning of the program
* Argument    : none
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
void FlashLED (void)
{
  /* PDL function creates a timer to flash the LEDs at 1Hz */         
  R_TMR_CreatePeriodic(PDL_TMR_UNIT1, PDL_TMR_FREQUENCY, 4, 50, PDL_NO_FUNC,
             TMR_Callback, 5);

  /* While loop keeps the function waiting */
  while(1)
  {
    /* Checks if the flash count has been reached, or if a button has been pressed */
    if((KeyPress)||(flash_count > 0x190))
    {
      /* Reset the KeyPress flag variable */
      KeyPress = 0;
      
      /* Exit from the while loop */
      break;
    }
  }
  
  /* Destroy Timer */
  R_TMR_Destroy(1);

}
/******************************************************************************
End of function FlashLED
******************************************************************************/


/*””FUNC COMMENT””*************************************************************
* Outline     : TMR_Callback
* Description   : Callback function that is executed everytime a compare match 
*          occurs in the timer perhiperal. Function toggles LEDs and
*          decrements flash counter.
* Argument    : none
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
void TMR_Callback(void)
{
  /* Blink LEDs */
  ToggleLEDs();
  
  /* Decrement the flash count */
  flash_count--;
}
/******************************************************************************
End of function TMR_Callback
******************************************************************************/


/*””FUNC COMMENT””*************************************************************
* Outline     : ToggleLEDs
* Description   : Function used to toggle the output of LEDs 0 - 3
* Argument    : none
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
void ToggleLEDs(void)
{
  /* Perform a logical XOR with each LED pin to 1, to invert the pin 
     each time ToggleLEDs() is called */
  R_IO_PORT_Modify(PDL_IO_PORT_8_3, PDL_IO_PORT_XOR, 1);
  R_IO_PORT_Modify(PDL_IO_PORT_8_4, PDL_IO_PORT_XOR, 1);
  R_IO_PORT_Modify(PDL_IO_PORT_3_3, PDL_IO_PORT_XOR, 1);
  R_IO_PORT_Modify(PDL_IO_PORT_3_6, PDL_IO_PORT_XOR, 1);
}
/******************************************************************************
End of function ToggleLEDs
******************************************************************************/



