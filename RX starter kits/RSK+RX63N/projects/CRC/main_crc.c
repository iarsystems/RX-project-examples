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
* File Name     : main_crc.c
* Version       : 1.00
* Device        : R5F563NB
* Tool-Chain    : Renesas RX Standard 1.2.0.0
* H/W Platform  : RSK+RX63N
* Description   : This sample demonstrates usage of the CRC unit, by echo back
*                  CRC checksums of ASCII characters recieved over SCI terminal.
* Operation        : Before starting this sample, connect the RSK to a PC running
*                  terminal emulator (e.g. hyperterminal) with a 9 way 
*                  serial cable. Ensure the terminal client is configured as
*                  follows:
*                                Baud Rate:         19200
*                                Data Bits:         8
*                                Parity:            None
*                                Stop Bits:         1
*                                Flow Control:      None
*                  1. Compile and download the sample code to the target. 
*                     Click the 'Resume' icon on the Renesas Debug toolbar to
*                     run to the beginning of the main() function, and click
*                     'Resume' again to run the software.
*
*                  2. Observe the instructions sent to the PC terminal. Enter any
*                     printable ASCII character using the PC keyboard.
*
*                  3. The RSK will echo back the entered character, its hex value
*                     and the CRC checksum of this hex value. Enter another
*                     character to repeat the test.
*******************************************************************************/
/*******************************************************************************
* History       : 13 Aug. 2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/
/* Defines LCD functions used in this file */
#include "lcd.h"
/* Defines CRC functions used in this file */
#include "crc.h"
/* Defines async functions used in this file */
#include "async.h"

#include "intrinsics.h"
#include "hwsetup.h"

/*******************************************************************************
* Outline      : main
* Description  : The main function first displays the sample name/instructions
*                 on the debug LCD, then calls the CRC and SCI intialisation
*                 functions; and then waits in an infinite while loop. The rest
*                 of the sample's functionality is done in interrupts.
* Argument     : none
* Return value : none
*******************************************************************************/
void main(void)
{
  
    HardwareSetup();
  
    __enable_interrupt(); 
    
    /* Initialise debug LCD */
    Init_LCD();
    
    /* Display sample name/instructions on the debug LCD */
    Display_LCD(LCD_LINE1, " CRC ");
    
    /* Initialise CRC unit */
    Init_CRC();
    
    /* Initialise SCI unit to communicate to PC terminal */
    SciInit_CRC();
    
    /* Infinite while loop */    
    while(1);    
}
/*******************************************************************************
* End of function main
*******************************************************************************/
