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
* File Name     : main_adc12_oneshot.c
* Version       : 1.00
* Device        : R5F563NB
* Tool-Chain    : Renesas RX Standard 1.2.0.0
* H/W Platform  : RSK+RX63N
* Description   : Demonstration of the S12AD ADC module, in single shot mode. 
*                  The program configures the ADC to perform oneshot AD conversions
*                  and display the results on the LCD.
* Operation        :   1. Compile and download the sample code to the target. 
*                       Click the 'Resume' icon on the Renesas Debug toolbar to
*                       run to the beginning of the main() function, and click
*                       'Resume' again to run the software.
*                        
*                      2. Adjust the potentiometer, press SW3 to carry out a 
*                       12-bit A/D conversion of the ADC channel AN000 and 
*                       observe the change in the value being displayed on the 
*                         LCD.
*                       
*                      3. The user may also examine the ADC conversion result in  
*                       the global variable gADC_Result.    
*                       Repeat Step 2 to carry out further ADC readings. 
*******************************************************************************/
/*******************************************************************************
* History       : 13 Aug. 2012   Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/
/* LCD controlling function prototypes & macro definitions */
#include "lcd.h"
/* ADC oneshot function definitions */
#include "adc12oneshot.h"

#include "intrinsics.h"

#include "hwsetup.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* External variable used to store ADC results */
extern volatile uint16_t gADC_Result;

/*******************************************************************************
* Outline       : main
* Description   : The main program function. This function initialises the 
*                  debug LCD and displays instructions. It then calls the S12AD 
*                  ADC initialisation function, then waits in an infinite while 
*                  loop whilst the ADC readings take place in the switch callback 
*                  function.
* Argument      : none
* Return value  : none
*******************************************************************************/
void main(void)
{    
  
    HardwareSetup();
  
    __enable_interrupt();
  
    /* Initialise the debug LCD */
    Init_LCD();
    
    /* Display instructions on the debug LCD */    
    Display_LCD(LCD_LINE1, "AD12 One");    
    Display_LCD(LCD_LINE2, "Push SW3");    

    /* Initialise the 12-bit ADC module */
    Init_ADC12Oneshot();
        
    /* Infinite while loop */
    while(1);
}
/******************************************************************************
* End of function main
******************************************************************************/
