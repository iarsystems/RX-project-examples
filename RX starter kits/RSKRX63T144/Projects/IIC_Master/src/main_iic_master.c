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
/* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.   */
/*******************************************************************************
* File Name     : main_iic_master.c
* Version       : 1.00
* Device        : R5F563TE
* Tool-Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX63T144
* Description   : Demonstrates usage of IIC unit in master mode, by reading and
*                 writing to EEPROM memory devices similar to the Renesas
*                 R1EX24xxx series (EEPROM device) IC. The EEPROM memory device
*                 address is set as 0xA0.
* Operation     : 1. Connect a compatible EEPROM memory device as follows:
*
*                         RSK IIC Master     |    RSK IIC Slave/EEPROM
*                       -------------------------------------------------
*                         SDA (JA1, pin 25)  ->   SDA
*                         SCL (JA1, pin 26)  ->   SCL
*                         Any ground point   ->   Ground
*
*                       Ensure target IIC slave device is correctly powered.
*
*                 2. Compile and download the sample code to the target. Click
*                    'Resume' icon on the 'Debug' toolbar to start the
*                    software.
*
*                 3. The name of the application will be displayed on the debug
*                    LCD.
*
*                 4. Press switch SW2 to perform a write operation. If the write
*                    is successful, the data identifier will be displayed on the
*                    screen. Re-press SW2 to write next data.
*
*                 5. Press switch SW3 to perform a read operation. If the read
*                    is successful, the identifier of the data read will appear
*                    on the debug LCD. Re-press SW3 to repeat the read
*                    instruction.
*
*                 6. If a write error occurs, "Error W." will be displayed on
*                    the debug LCD & if read error occurs, "Error R." will be
*                    displayed on debug LCD.
*                    An error will occur if a read operation is performed
*                    before doing a write operation. This is because the IIC
*                    master application compares the received data with the
*                    last data written.
* Note          : 1. If failure occurs, double check connections and
*                    power cycle the EEPROM
*******************************************************************************/
/*******************************************************************************
* History       : 05.12.2013  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/
/* LCD controlling function prototypes & macro definitions */
#include "lcd.h"

/* Defines the EEPROM master functions used in this file */
#include "iic_eeprom_master.h"

/* Defines the EEPROM master sequence function used in this file */
#include "iic_eeprom_master_app.h"

/* this module's header file */
#include "main_iic_master.h"

/* Intrinsics */
#include <intrinsics.h>

/*******************************************************************************
* Function Name : main
* Description  : The main program function. This function first initialises the
*                debug LCD, and displays the application name. It then calls
*                the MasterSequence_EEPROM function.
*                The master sequence performs read and write operations to
*                the EEPROM device when user switches are pressed. Refer to
*                iic_eeprom_master_app.c for further details.
* Argument     : none
* Return value : none
*******************************************************************************/
void main (void)
{
    /* Enable interrupts */
    __enable_interrupt();

    /* Initialise the debug LCD */
    Init_LCD();

    /* Display IIC mode on the debug LCD */
    Display_LCD(LCD_LINE1, "IIC Mast");

    /* Initalise the system for IIC Master */
    Init_EEPROM_Master();

    while (1)
    {
        /* Call the Master sequence function */
        MasterSequence_EEPROM();
    }
}
/*******************************************************************************
* End of function main
*******************************************************************************/

