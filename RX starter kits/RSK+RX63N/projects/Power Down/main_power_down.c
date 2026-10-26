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
* Description   : This sample demonstrates usage of the power down modes, by 
*                  starting the real time clock and enter standby mode. The
*                  MCU then wakes up every second to update the clock, then goes
*                  back into standby.
* Operation     : 1. Compile and download the sample code to the target. 
*                    Click the 'Resume' icon on the Renesas Debug toolbar to
*                    run to the beginning of the main() function, and click
*                    'Resume' again to run the software.
*
*                 2. The real time clock will initialise at 11:59:30, and begin
*                    incrementing every second from there. LED0 will light when
*                    the MCU is in normal mode, and is updating the debug LCD
*                    (the time the MCU is in normal mode has been extended to
*                     make the transitions more visible).
*                
*                  3. An alarm interrupt has been set for 12:00:00, which will 
*                     turn on LED1.
*
* Note          : By removing R152, and fitting an ammeter across J10; it is 
*                 possible to observe the current consumption as the MCU
*                 switches between normal and standby mode.
*******************************************************************************/
/*******************************************************************************
* History       : 13 Aug. 2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/
/* Defines LCD functions used in this file */
#include "lcd.h"
/* Defines power down functions used in this file */
#include "power_down.h"
/* Defines RTC functions used in this file */
#include "rtc.h"

#include "intrinsics.h"
#include "hwsetup.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Enter standby boolean variable flag (imported) */
extern bool gEnterStandby;

/*******************************************************************************
* Outline       : main
* Description   : The main function intialises the debug LCD and displays the
*                 sample name, then calls the power down and RTC initialisation
*                 functions. The function then enters an infinite while loop, 
*                 which polls the gEnterStandby flag, and calls the enter 
*                 standby function if its true.
* Argument      : none
* Return value  : none
*******************************************************************************/
void main(void)
{

    HardwareSetup();
  
    __enable_interrupt(); 
  
    /* Initialise LCD */
    Init_LCD();
    
    /* Display sampple name on debug LCD */
    Display_LCD(LCD_LINE1, "Pwr Down");    
    
    /* Initialise the power down registers */
    Init_PowerDown();

    /* Initialise the RTC unit */
    Init_RTC();
    
    /* Infinite while loop */    
    while(1)
    {
        /* Check if the gEnterStandby flag is true */
        if(gEnterStandby)
        {
            /* Enter standby mode */
            EnterStandby_PowerDown();
            
            /* MCU wakes from standby mode here */
        
        }
    }
}
/*******************************************************************************
* End of function main
*******************************************************************************/
