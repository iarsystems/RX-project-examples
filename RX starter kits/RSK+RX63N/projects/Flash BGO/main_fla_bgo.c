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
* File Name     : hwsetup.c
* Version       : 1.00
* Device        : R5F563NB
* Tool-Chain    : Renesas RX Standard 1.2.0.0
* H/W Platform  : RSK+RX63N
* Description   : This sample demonstrates the FCU unit, by writing ADC results
*                 to RAM using the DTC, then copying the data from RAM to 
*                 incrementing data flash memory locations.
* Operation     :
*                 1. Click the 'Connect' icon to connect to the RSK.
*
*                 2. Build and download the sample code to the RSK.
*
*                 3. Open the Memory tab (Window->Show View->Memory).
*                    Right click and select 'Add Memory Monitor'. Set the address
*                    to 0x00100000.
*
*                 4. Open the 'Expressions' tab, select 'Add new expression' and
*                    enter 'usStore'. Right-click on usStore and  select
*                    'Monitor in Real-Time'.
*
*                 5. Click the 'Resume' icon on the Renesas Debug toolbar to
*                    run to the beginning of the main() function, and click
*                    'Resume' again to run the software. Instructions will be
*                     displayed on the debug LCD.
*
*                 6. LED2 will turn on briefly to indicate the flash erase operation. 
*                    ADC conversions will then begin and triggers the DTC to transfer 
*                    the ADC values to the usStore buffer, seen in the Watch1 window. 
*                    After 256 conversions, the DTC resets its destination pointer 
*                    to the begin of the usStore buffer and repeats the process.
*                    
*                 7. Adjust the potentiometer to see the values change. 
*
*                 8. Press switch SW2 to stop ADC conversions; the message "ADC Unit 
*                    Stop" will appear briefly on the second line of the LCD. Press 
*                    SW1 to erase the data flash block before writing the usStore buffer 
*                    contents to the data flash memory. "F.Data Match" will appear 
*                    on the LCD after a memory comparison, followed by "WR. PASS" and 
*                    LED1 will also turn on then off.
*
*                 9. Click the 'Suspend' icon to halt program execution. Verify
*                    the flash memory area matches that of usStore.
*
*                10. Click 'Resume' to resume program execution. Press SW2 to
*                    restart ADC conversions. "ADC Unit Run " will briefly appear on 
*                    the second line of the LCD. Pressing SW3 will erase the 
*                    data flash memory whilst conversions are occuring. 
*                    "F.Erase Pass" will briefly appear on the second line of the 
*                    LCD
*
*                11. Press SW1 to write the DTC transferred values to the flash 
*                    data memory locations. No memory comparison will take place 
*                    whilst ADC conversion are occuring.
*
* Note               : Memory locations which have not initialised do not have a
*                  defined state, and will continuously change with random 
*                  values.
*                  Should the program halt as a result of a FLASH operation, 
*                  when debugging using an E1 click 'Disconnect' and go to 
*                  Step 1, otherwise disconnect then recoonect the power supply.
*******************************************************************************/  
/*******************************************************************************
* History       : 13 Aug. 2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
/* Defines LCD functions used in this file */
#include "lcd.h"
/* Defines switch functions and variables used in this file */
#include "switch.h"
/* Defines flash functions used in this file */
#include "flash_bgo.h"

#include "intrinsics.h"
#include "hwsetup.h"

/******************************************************************************
* Global Variables
******************************************************************************/
/* External declaration of variable used for clearing the debug LCD */
extern bool gClear_LCD;
/* External declare of a flag to indicate flash write operation statuses */
extern bool gWrite_Status;
/* External declare of a flag to indicate ADC status */
extern bool gADC_Status;

/*******************************************************************************
* Outline      : main
* Description  : Main program function. This function first initialises the
*                 debug LCD and displays instructions on the screen. Main then
*                calls the Init_FlashBGO function which initialises the ADC, 
*                DTC and flash unit. Main then enters an infinite while loop, 
*                where it repeatedly calls the SwitchPressHandler_BGO function
*                to poll the user switches.
* Argument     : none
* Return value : none
*******************************************************************************/
void main(void)
{    

    HardwareSetup();
  
    __enable_interrupt(); 
 
    /* Initialise the debug LCD */
    Init_LCD();
    
    /* Display instructions on the debug LCD */    
    Display_LCD(LCD_LINE1, "Data BGO");
    Display_LCD(LCD_LINE2, "Push SW ");
    
    /* Initialise the flash controller unit */
    Init_FlashBGO();
    
    /* Infinite while loop */
    while(1)
    {
        /* Call switch press handler function */
        SwitchPressHandler_BGO();
    }
}
/******************************************************************************
* End of function main
******************************************************************************/
