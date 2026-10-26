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
* File Name     : main_timer_mode.c
* Version       : 1.00
* Device        : R5F563NB
* Tool-Chain    : Renesas RX Standard 1.2.0.0
* H/W Platform  : RSK+RX63N
* Description   : This sample uses channel 3 of the MTU2a to output a 1KHz 
*                  square waveform on the MTIOC3B output pin.      
* Operation     : 1. Connect an oscilloscope probe to the header pin JA3-42. 
*
*                 2. Compile and download the sample code to the target.
*                    Click the 'Resume' icon on the Renesas Debug toolbar to
*                    run to the beginning of the main() function, and click
*                    'Resume' again to run the software.
*
*                 3. A 1 KHz square wave output will be displayed on the
*                    oscilloscope. LED0 will also toggle at 1KHz.
*******************************************************************************/
/*******************************************************************************
* History       : 13 Aug. 2012  Ver. 1.00 First Release
*******************************************************************************/

/******************************************************************************
User Includes (Project Level Includes)
******************************************************************************/
/* LCD controlling function prototypes & macro defines */
#include "lcd.h"
/* Prototypes for Timer control functions */
#include "timer_mode.h"

#include "intrinsics.h"
#include "hwsetup.h"

/******************************************************************************
* Outline       : main
* Description   : The main program function. The function displays the splash
*                  screen on the LCD display, initialises the timer output, then
*                  waits in an infinite while loop.
* Argument      : none
* Return value  : none
******************************************************************************/
void main(void)
{
  
    HardwareSetup();
  
    __enable_interrupt();
    
    /* Initialise the LCD Display */
    Init_LCD();
    
    /* Display Splash Screen */
    Display_LCD(LCD_LINE1, " 1 KHz  ");
    Display_LCD(LCD_LINE2, "JA3 P42 ");
    
    /* Initialise timer */
    Init_TimerMode();
    
    /* Infinite while loop */
    while(1);
}
/******************************************************************************
* End of function main
******************************************************************************/
