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
/*******************************************************************************
* Copyright (C) 2010 Renesas Electronics Corporation. All rights reserved.    */ 
/*******************************************************************************
* File Name    : main_dtc.c
* Version      : 1.00
* Device       : R5F562N8
* Tool Chain   : IAR Embedded Workbench
* H/W Platform : RSK+RX62N
* Description  : This sample demonstrates the DTC unit; by performing a data
*                transfer triggered by ADC completion.
* Operation    :
*
*         1. Build and download the sample code to the RSK+.
*
*         2. Open the 'Live Watch' window ('View' menu).
*
*         3. Drag the variable 'gDTC_Destination' to the 'Live Watch'.
*            The contents of 'gDTC_Destination' can now be observed.
*
*         4. Click 'Go' to start the software. Instructions will be
*            displayed on the debug LCD.
*
*         5. Adjust the potentiometer, RV1, and press the switch SW3. The
*            result from the ADC conversion will be transferred into the
*            first element of the transfer destination array.
*        
*         6. Repress the switch SW3, and subsequent results will be
*            transferred into the the next array element. 
*          
*         7. Once the last array has been filled, the next transfer will
*            clear the array contents and start from the first element.
*******************************************************************************/
/*******************************************************************************
* History     : 23.08.2010 Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
/* Defines LCD functions used in this file */
#include "lcd.h"
/* Defines DTC functions used in this file */
#include "dtc.h"
#include "hwsetup.h"
#include "intrinsics.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* DTC transfer destination array declaration */
extern uint16_t gDTC_Destination[16];

/*******************************************************************************
* Outline      : main
* Description  : Main program function. This function first initialises the
*         debug LCD and displays instructions on the screen. Main then
*         calls the DTC initialisation function, which configures the DTC
*         transfer to be triggered from each ADC conversion. The
*         initialisation function also configures switch SW3 to call a
*         callback function which triggers an ADC read. The function then
*         returns to main, which waits in an infinite while loop.
* Argument     : none
* Return value : none
*******************************************************************************/
void main(void)
{
   HardwareSetup();
  __enable_interrupt();

 /* Initialise the debug LCD */
  InitialiseLCD();

  /* Display instructions on the debug LCD */
  DisplayLCD(LCD_LINE1, "DTC     ");
  DisplayLCD(LCD_LINE2, "Push SW3");  

  /* Initialise the DTC unit */
  Init_DTC();

  /* Infinite while loop */
  while(1);
}
/*******************************************************************************
* End of function main
*******************************************************************************/
