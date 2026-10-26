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
/* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.   */
/*******************************************************************************
* File Name    : main_dtc.c
* Version     : 1.00
* Device     : R5F5630E
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX630
* Description   : This sample demonstrates the DTC unit by performing a DTC
*          transfer from S12AD unit into a 16 element array.
* Operation      : 1. Compile and download the sample code to the target. 
*
*          2. Right-click the variable 'gDTC_Destination' and select
*             'Add Watch'. Right-click the variable 'gDTC_Destination' 
*           in the Watch window and select 'Enable Auto Update'.
*
*          3. The contents of the variable 'gDTC_Destination' can be
*           viewed in the watch window.
*
*          4. Click 'Reset Go' icon on the 'Debug Run' toolbar to start
*           the software. Instructions will be displayed on the debug
*           LCD.
*
*          5. Adjust the potentiometer, RV1, and press the switch SW1. 
*           The result from the ADC conversion will be transferred 
*           into the first element of the transfer destination array.
*        
*          6. Re-press the switch SW1, and subsequent results will be
*             transferred into the the next array element. 
*          
*          7. Once the last array has been filled, the next transfer 
*             will clear the array contents and start from the first 
*           element.
*
*          8. Whenever the DTC completes transfer of 2 bytes of data,
*           the LED0 toggles its state.
*
* Note        : 1. In order to watch the contents of the gDTC_Destination 
*             array during execution, the sample must be built in 
*           'debug' mode.
*          2. Place a breakpoint inside the callback function CB_Switch  
*           at the call of 'memset' function.
*           This enables the viewing of the variable 'gDTC_Destination'
*           with every array element filled up before they are cleared. 
*******************************************************************************/
/*******************************************************************************
* History     : 23 Jan. 2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/
/* LCD controlling function prototypes & macro definitions */
#include "lcd.h"
/* Defines DTC functions used in this file */
#include "dtc.h"

#include "intrinsics.h"

#include "hwsetup.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Declare the DTC transfer destination array */
uint16_t gDTC_Destination[16];

/*******************************************************************************
* Outline     : main
* Description    : Main program function. This function first initialises the
*          debug LCD and displays instructions on the screen. Main then
*          calls the DTC initialisation function, which configures the 
*          ADC & DTC module. The function then returns to main, where it 
*            waits in an infinite while loop. When the user presses SW1,
*          the ADC data is transferred to the 'gDTC_Destination' by the 
*          DTC. The LED0 toggles every time the DTC transfers 16 bytes
*          of data.
* Argument    : none
* Return value  : none
******************************************************************************/
void main(void)
{
  
  HardwareSetup();
  
  __enable_interrupt();
  
  /* Initialise the debug LCD */
  Init_LCD();

  /* Display instructions on the debug LCD */
  Display_LCD(LCD_LINE1, "  DTC   ");
  Display_LCD(LCD_LINE2, "Push SW1");  
  
  /* Initialise the DTC module */
  Init_DTC();
  
  /* Infinite while loop */
  while (1);
}
/******************************************************************************
* End of function main
******************************************************************************/
