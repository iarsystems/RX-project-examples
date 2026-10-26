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
* File Name    : main_iic_slave.c
* Version      : 1.00
* Device       : R5F563TE
* Tool-Chain   : RX Family C Compiler 1.2.1.0
* H/W Platform : RSKRX63T
* Description  : Demonstrates usage of IIC unit in slave mode, by simulating an
*                EEPROM memory device, similar to the Renesas R1EX24xxx series.
*                The memory capacity is 256 bytes, arranged as 265 x 8 bits.
*
* Operation    : 1. Connect a compatible IIC Master device as follows:
*
*                       IIC Master Device   |    RSK Slave
*                       -------------------------------------------------
*                                   SDA     ->   SDA (JA1, pin 25)
*                                   SCL     ->   SCL (JA1, pin 26)
*                                   GND     ->   Any ground point.
*
*                       The SDA and SCL lines must be pulled high
*                       with a suitable pull up resistor.
*                       Ensure target IIC Master device is correctly powered.
*
*                2. Build and download the sample code to the RSK.
*
*                3. No resistor configuration is required.
*
*                4. Click 'Resume' to start the software. The name of the
*                   sample will by displayed on the debug LCD.
*
*                5. Click the 'Memory' button on the e2 studio interface, and set
*                   the display address to 0x00001f00 and click OK. You can view
*                   the contents of the simulated EEPROM memory in the memory
*                   viewer window.
*
*                6. When a valid read request is sent from a master device, the
*                   RSK will respond correctly and display "Read M." on the
*                   debug LCD.
*
*                7. When a valid write request is sent from a master device, the
*                   RSK will store the write data into the simulated EEPROM
*                   memory, and display "Write M." on the debug LCD.
*
*******************************************************************************/
/*******************************************************************************
* History       : 05.12.2013  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
/* Defines functions in this file */
#include "main_iic_slave.h"

/* Defines IIC slave functions used in this file */
#include "iic_eeprom_slave.h"

/* Defines LCD functions used in this file */
#include "lcd.h"

/* Intrinsics */
#include <intrinsics.h>

/*******************************************************************************
* Function Name : main
* Description  : Main program function. The function first initialises the debug
*                LCD, and displays the sample name. Main then calls the IIC
*                slave sequence function, which configures the RSK to behave as
*                a simulated, 256 byte EEPROM device. See iic_eeprom_slave_app.c
*                for more details.
* Argument     : none
* Return value : none
*******************************************************************************/
void main (void)
{
    /* Enable interrupts */
    __enable_interrupt();

    /* Initialise the debug LCD */
    Init_LCD();

    /* Display the sample name on the debug LCD */
    Display_LCD(LCD_LINE1, "IIC SLAV");

    /* Initialise the IIC unit for slave mode */
    Init_EEPROM_Slave();

    /* Enter infinite while loop */
    while (1)
    {
        /* Run the simulated EEPROM slave sequence */
        SlaveSequence_EEPROM();
    }

}
/*******************************************************************************
* End of function main
*******************************************************************************/

