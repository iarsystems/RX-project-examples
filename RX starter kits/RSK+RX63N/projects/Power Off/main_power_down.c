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
* File Name     : main_power_down.c
* Version       : 1.00
* Device        : R5F563NB
* Tool Chain    : Renesas RX Standard 1.2.0.0
* H/W Platform  : RSK+RX63N
* Description   : Demonstrates entering the MCU into low power sleep/standby
*                  mode.
* Operation     : 1. Compile and download the sample code to the target.
*                    Click the 'Resume' icon on the Renesas Debug toolbar to
*                    run to the beginning of the main() function, and click
*                    'Resume' again to run the software.
*
*                  2. Observe the flashing user LEDs. This indicates that the 
*                     MCU is in normal (active) mode, this is also displayed on
*                     debug LCD.
*
*                  3. Press SW1 to enter the MCU into standby mode. The user LEDs
*                     will all turn off, and 'Standby' will be displayed on the
*                     debug LCD.
*
*                  4. Press any switch to wake the MCU up, which will update the
*                     debug LCD accordingly and repeat flashing the user LEDs.
*******************************************************************************/
/*******************************************************************************
* History       : 13 Aug. 2012 Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/
/* Defines power down functions used in this file */
#include "power_off.h"
/* Defines LCD functions used in this file */
#include "lcd.h"

#include "intrinsics.h"
#include "hwsetup.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Prepare to enter standby flag variable (imported) */
extern volatile bool gEnterStandby;

/******************************************************************************
* Outline       : main
* Description   : Function sets the switch callback function.
* Argument      : none
* Return value  : none
******************************************************************************/
void main(void)
{
  
    HardwareSetup();
  
    __enable_interrupt(); 
    
    /* Initialise the debug LCD */
    Init_LCD();
    
    /* Display current MCU mode on the debug LCD */
    Display_LCD(LCD_LINE1, "PowerOff");
    Display_LCD(LCD_LINE2, " Active ");
    
    /* Initialise the standby power registers */
    Init_PowerOff();
    
    /* Infinite while loop */    
    while(1)
    {
        /* Toggle the user LEDs */
        FlashLEDs_PowerOff();
        
        /* Check if the gEnterStandby flag has been set true */
        if(gEnterStandby)
        {
            /* Enter stanby mode */
            EnterStandby_PowerOff();
            
            /* MCU returns from standby */
            
            /* Set the gEnterStandby to false */
            gEnterStandby = false;
        }
    }
}
