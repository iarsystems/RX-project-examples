/******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. 
* This software is owned by Renesas Electronics Corporation and is protected 
* under all applicable laws, including copyright laws.
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
* File Name     : main_spi.c
* Version       : 1.00
* Device        : R5F563NB
* Tool-Chain    : Renesas RX Standard 1.2.0.0
* H/W Platform  : RSK+RX63N
* Description   : This demonstrates usage of the SPI unit by reading and writing
*                 from the onboard SPI flash device.
* Operation     : 1. Compile and download the sample code to the target.
*                    Click the 'Resume' icon on the Renesas Debug toolbar to
*                    run to the beginning of the main() function, and click
*                    'Resume' again to run the software.
*                     
*                 2. Press SW1 to perform a write to the SPI Flash device.
*                    After a succesful write, the LCD will display "Wrote X"
*                    where X is an incrementing digit.
*
*                 3. Press SW2 to perform a read from the SPI Flash device. 
*                    After a successful write, the write digit from the last
*                    successful write will be displayed on the debug LCD.
*
*                 4. The read and write data is stored in the global variables 
*                    gWriteData_SPI and gReadData_SPI, which can be watched by
*                    clicking 'Add new expression' in the Expressions tab, and
*                    filling in their names.
*
* Note           : 1. If read or write operation fails, the failure will be 
*                     displayed on the debug LCD. Disconnect the RSK power 
*                     supply, wait 10 seconds and reconnect and try again. 
*                  2. Each time the sample is run, the entire contents of the
*                     first memory block (32KByte) are erased; so do not store
*                     important data here.
*******************************************************************************
* History       : 13 Aug. 2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/
/* Defines SPI functions used in this file */
#include "spi.h"
/* Defines LCD functions used in this file */
#include "lcd.h"

#include "intrinsics.h"
#include "hwsetup.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Provide extern reference to SPI write buffer */
extern uint8_t gWriteData_SPI[32u];
/* Provide extern reference to SPI read buffer */
extern uint8_t gReadData_SPI[32u];

/*******************************************************************************
* Outline       : main
* Description   : 
* Argument      : none
* Return value  : none
*******************************************************************************/
void main(void)
{
  
    HardwareSetup();
  
    __enable_interrupt();
    
    /* Initialise the debug LCD */
    Init_LCD();

    /* Display sample name and instructions on debug LCD */
    Display_LCD(LCD_LINE1, "SPIFlash");
    Display_LCD(LCD_LINE2, "Push SW1");

    /* Initialise the SPI unit */
    Init_SPI();
        
    /* Infinite while loop */
    while(1);
}
/*******************************************************************************
* End of function main
*******************************************************************************/
