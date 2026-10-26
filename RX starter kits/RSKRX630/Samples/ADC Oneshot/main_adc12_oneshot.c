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
* File Name    : main_adc12_oneshot.c
* Version     : 1.00
* Device     : R5F5630E
* Tool-Chain   : Renesas RX Toolchain 1.2.0.0
* H/W Platform  : RSKRX630
* Description   : Demonstration of the S12AD ADC module, in single shot mode. 
*          The program configures the ADC to perform oneshot AD conversions
*          and display the results on the LCD.
* Operation    :   1. Compile and download the sample code to the target. 
*             Click 'Reset Go' icon on the 'Debug Run' toolbar to 
*             start the software.
*           
*            2. Observe the LCD display - the RX630 CPU will initially 
*             display the ADC reading as 
*            =H'xxxx
*              
*            3. Adjust the potentiometer, press SW3 to carry out a 
*             12-bit A/D conversion of the ADC channel AN000 and 
*             observe the change in the value being displayed on the 
*               LCD.
*             
*            4. The user may also examine the ADC conversion result in  
*             the global variable gADC_Result.  
*             Repeat Step 3 to carry out further ADC readings. 
*******************************************************************************/
/*******************************************************************************
* History     : 23 Jan. 2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/
#include "iorx630.h"
/* Defines macros relating to the RX630 user LEDs and switches */
#include "rskrx630def.h"
/* LCD controlling function prototypes & macro definitions */
#include "lcd.h"
/* Defines switch functions and variables used in this file */
#include "switch.h"
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
* Outline     : main
* Description   : The main program function. This function initialises the 
*          debug LCD and displays instructions. It then calls the S12AD 
*          ADC initialisation function, then waits in an infinite while 
*          loop whilst the ADC readings take place in the switch callback 
*          function.
* Argument    : none
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
  Display_LCD(LCD_LINE2, "=H'xxxx ");  

  /* Initialise the 12-bit ADC module */
  Init_ADC12Oneshot();
  
  /* Configure a switch press callback function */
  SetSwitchPressCallback(CB_Switch_Press);
  
  /* Infinte while loop */
  while(1);
}
/******************************************************************************
* End of function main
******************************************************************************/
