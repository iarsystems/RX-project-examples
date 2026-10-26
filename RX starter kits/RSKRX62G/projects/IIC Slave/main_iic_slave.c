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
/*******************************************************************************
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.    */
/*******************************************************************************
* File Name    : main_iic_slave.c
* Version      : 1.00
* Device       : R5F562GAADFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform : RSKRX62G
* Description  : Demonstrates usage of IIC unit in slave mode, by simulating an
*                EEPROM memory device, similar to the Renesas R1EX24xxx series.
*                The memory capacity is 2Kbytes, arranged as 256 x 8 bytes.
* Operation    : 1. Build and download the sample code to the RSK.
*
*                2. Connect a compatiable EEPROM Master device as follows:
*
*                            RSK Slave |  IIC Master Device
*                           ------------------------------------
*                               SDA    ->   SDA (JA1, pin 25)
*                               SCL    ->   SCL (JA1, pin 26)
*                               GND    ->   Any ground point.
*
*                       The SDA and SCL lines must be pulled high
*                       with a suitable pull up resistor.
*
*                3. Ensure the following resistor configuration is made:
*                   Fit: R113, R116 - Remove: R112, R115.
*
*                4. Click 'Reset Go' to start the software. The name of the
*                   sample will by displayed on the debug LCD.
*
*                5. Click the 'Memory' button on the HEW interface, and set the
*                   display address to 00003000 and click OK. You can view the
*                   contents of the simulated EEPROM memory in the memory viewer
*                   window.
*
*                6. When a valid read request is sent from a master device, the
*                   RSK will respond correctly and display "Read M." on the
*                   debug LCD.
*
*                7. When a valid write request is sent from a master device, the
*                   RSK will store the write data into the simulated EEPROM
*                   memory, and display "Write M." on the debug LCD.
*
*                8. If the RSK displays "IIC Fail", it means program has
*                   detected a failed IIC transmission. Reset both the master
*                   slave devices and retry.
*
*******************************************************************************/
/*******************************************************************************
* History       : 06.09.2012 Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
/* Defines IIC slave sequences used in this file */
#include "iic_eeprom_slave_app.h"
/* Defines LCD functions used in thuis file */
#include "lcd.h"

#include "hwsetup.h"
#include "intrinsics.h"

/*******************************************************************************
* Outline      : main
* Description  : Main program function. The function first initialises the debug
*                LCD, and displays the sample name. Main then calls the IIC
*                slave sequence function, which configures the RSK to behave as
*                a simulated, 2Kbyte EEPROM device. See iic_eeprom_slave_app.c
*                for more details.
* Argument     : none
* Return value : none
*******************************************************************************/
void main(void)
{
    /* Do the hardware setup */
    HardwareSetup();

    /* Enable interrupts */
    __enable_interrupt();

    /* Initialise the debug LCD */
    InitialiseLCD();

    /* Display the sample name on the debug LCD */
    DisplayLCD(LCD_LINE1, "IIC SLAV");

    /* Start the simulated EEPROM slave sequence */
    SlaveSequence_EEPROM();

    /* Infinite while loop */
    while(1);
}
/*******************************************************************************
* End of function main
*******************************************************************************/
