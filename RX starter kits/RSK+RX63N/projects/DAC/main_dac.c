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
* File Name     : main_dac.c
* Version       : 1.00
* Device        : R5F563NB
* Tool-Chain    : Renesas RX Standard 1.2.0.0
* H/W Platform  : RSK+RX63N
* Description   : Before starting the sample, the following board modifications
*                 must be made: Fit R41, Remove R40.
*                 1. Compile and download the sample code to the target. 
*                    Click the 'Resume' icon on the Renesas Debug toolbar to
*                    run to the beginning of the main() function, and click
*                    'Resume' again to run the software.
*                 2. Connect a digital multi meter or an oscillioscope to 
*                    application header JA1, pin 13 (DAC output pin).
*                 3. Vary the position of the potentiometer RV1, and observe the
*                    changing output voltage on the DAC output pin.
*******************************************************************************/
/*******************************************************************************
* History       : 13 Aug. 2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/
/* Defines DAC functions used in this file */
#include "dac.h"
/* Defines LCD functions used in this file */
#include "lcd.h"

#include "intrinsics.h"
#include "hwsetup.h"

/******************************************************************************
* Outline       : main
* Description   : This function displays instructions of the debug LCD, then 
*                 calls the Init_DAC function, which initialises the DAC, ADC
*                 and timer unit. The sample then enters an infinite while loop,
*                 whilst the rest of the sample's action is performed in
*                 interrupts. 
* Argument      : none
* Return value  : none
******************************************************************************/
void main(void)
{
  
    HardwareSetup();
  
    __enable_interrupt(); 
    
    /* Initialise the debug LCD */
    Init_LCD();
    
    /* Display instructions on the debug LCD */
    Display_LCD(LCD_LINE1, " DAC    ");
    Display_LCD(LCD_LINE2, " JA1 P13");
    
    /* Enter user code */
    Init_DAC();
    
    /* Infinite while loop */
    while(1);
}
/******************************************************************************
* End of function main
******************************************************************************/
