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
* File Name    : main_iic_master.c
* Version      : 1.00
* Device       : R5F562GAADFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform : RSKRX62G
* Description  : Demonstrates usage of IIC unit in master mode, by reading and
*                writing to an EEPROM device.
*                Works with the Renesas R1EX24xxx series, 16Kbit EEPROM and
*                compatible devices. The EEPROM device should be in a circuit
*                recommended by the manufacturer to ensure it is operational.
* Operation    : 1. Build and download the sample code to the RSK.
*
*                2. Connect a compatiable EEPROM memory device as follows:
*
*                            EEPROM  |   RSK
*                           -------------------------------
*                            SDA    ->   SDA (JA1, pin 25)
*                            SCL    ->   SCL (JA1, pin 26)
*                            GND    ->   Any ground point.
*
*                3. Ensure the following resistor configuration is made:
*                   Fit: R113, R116 - Remove: R112, R115.
*
*                4. Click 'Reset Go' to start the software. The name of the
*                   sample will by displayed on the debug LCD.
*
*                5. Press switch SW2 to perform a write operation. If the write
*                   is successful, the data identifier will be displayed on the
*                   screen. Repress SW2 to another write.
*
*                6. Press switch SW3 to perform a read operation. If the read is
*                   successful, the indentifier of the data read will appear
*                   on the debug LCD. Repress SW3 to repeat the read instruction.
*
*                7. If a write error occurs, "Error W." will be displayed on the
*                   debug LCD. Stop the sample and disconnect power to the RSK,
*                   recheck the EEPROM connections, then reconnect power and
*                   start from step 1 again.
*******************************************************************************/
/*******************************************************************************
* History       : 06.09.2012 Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
/* Defines LCD functions used in this file */
#include "lcd.h"
/* Defines the EEPROM master sequence function used in this file */
#include "iic_eeprom_master_app.h"

#include "hwsetup.h"
#include "intrinsics.h"

/*******************************************************************************
* Outline      : main
* Description  : The main program function. This function first intialises the
*                debug LCD, and displays the sample name. It then calls the
*                MasterSequence_EEPROM function, from which is does not exit
*                from. The master sequence performs read and write operations to
*                the EEPROM device when user switches are pressed. Refer to
*                iic_eeprom_master_app.c for further details.
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

    /* Display IIC mode on the debug LCD */
    DisplayLCD(LCD_LINE1, "IIC Mast");

    /* Master EEPROM program sequence */
    MasterSequence_EEPROM();

    /* Infinite while loop */
    while(1);
}
/*******************************************************************************
* End of function main
*******************************************************************************/
