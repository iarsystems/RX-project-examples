/******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Electronics Corp. and is
* only intended for use with Renesas products.  

* No other uses are authorized.

* This software is owned by Renesas Electronics Corp. and is 
* protected under the applicable laws, including copyright laws.

* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY,
* INCLUDING BUT NOT LIMITED TO WAWRRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH
* WARRANTIES ARE EXPRESSLY DISCLAIMED.

* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER 
* RENESAS ELECTRONICS CORP. NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR AND DIRECT, INDIRECT, SPECIAL, INCIDENTAL
* OR COSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE,
* EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE
* POSSIBILITIES OF SUCH DAMAGES.

* Renesas reserves the right, without notice, to make changes to this
* software and to discontinue availability of this software.
* By using this software, you agree to the additional terms and 
* conditions found by accessing the following link:
* http://www.renesas.com/disclaimer
*******************************************************************************/
/* Copyright (C) 2010. Renesas Electronics Corp., All Rights Reserved         */
/******************************************************************************
* File Name     : main_dtc.c
* Version       : 1.1
* Device(s)     : R5F562N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.1
* OS            : None
* H/W Platform  : YRDKRX62N
* Description   : This sample demonstrates the DTC unit; by utilizing the DTC
*                 to transfer an ADC conversion to a 16 element array.
*       
* Operation     : 1. Build and download the sample code to the YRDK.
*
*                 2. Right-click the variable 'gDTC_Destination' and select
*                   'Instant Watch', clicking 'Add' on the subsequent dialog.
*
*                 3. The contents of the variable 'gDTC_Destination' can be
*                    viewed in the watch window.
*
*                 4. Click 'Reset Go' to start the software. Instructions will be
*                    displayed on the debug LCD.
*
*                 5. Adjust the potentiometer, VR1, and press the switch SW3. The
*                    result from the ADC conversion will be transferred into the
*                    first element of the transfer destination array.
*                
*                 6. Repress the switch SW3, and subsequent results will be
*                    transferred into the the next array element. 
*                    
*                 7. Once the last array has been filled, the next transfer will
*                    clear the array contents and start from the first element.
*
* Note          : In order to watch the contents of the gDTC_Destination array
*                 during execution, the sample must be built in 'debug' mode.
* Limitations   : None
*******************************************************************************
* History : DD.MMM.YYYY     Version     Description
*         : 08.Oct.2010     1.00        First release
*         : 02.Dec.2010     1.10        Second YRDK release
*******************************************************************************/


/*******************************************************************************
* Project Includes
*******************************************************************************/
#include <stdbool.h>

/* Defines LCD functions used in this file */
#include "lcd.h"
/* Defines DTC functions used in this file */
#include "dtc.h"

/* RSPI API library support */
#include "YRDKRX62N_RSPI_API.h"

/* Board specific definitions */
#include "YRDKRX62N.h"

#include "machine.h"
#include "intrinsics.h"
#include "hwsetup.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* DTC transfer destination array declaration */
extern uint16_t gDTC_Destination[16];

/*******************************************************************************
* Outline      : main
* Description  : Main program function. This function first initialises the
*                 debug LCD and displays instructions on the screen. Main then
*                 calls the DTC initialisation function, which configures the DTC
*                 transfer to be triggered from each ADC conversion. The
*                 initialisation function also configures switch SW3 to call a
*                 callback function which triggers an ADC read. The function then
*                 returns to main, which waits in an infinite while loop.
* Argument     : none
* Return value : none
*******************************************************************************/
void main(void)
{
  HardwareSetup();

  /* Enable Interrupt */
  __enable_interrupt();

  /* Initialize RSPI peripheral */
  YRDKRX62N_RSPI_Init(RSPI_CHANNEL_0);

  /* Initialise the debug LCD */
  InitialiseLCD();

  /* Display instructions on the debug LCD */
  DisplayLCD(LCD_LINE1, "DTC Demo");

  DisplayLCD(LCD_LINE3, "Adjust pot");  
  DisplayLCD(LCD_LINE4, "Push SW3");  

  DisplayLCD(LCD_LINE6, "Check watch");  
  DisplayLCD(LCD_LINE7, "window in  ");  
  DisplayLCD(LCD_LINE8, "debugger");  

  /* Initialise the DTC unit */
  Init_DTC();

  /* Infinite while loop */
  while (1);
}
/*******************************************************************************
* End of function main
*******************************************************************************/
