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
* File Name     : main_iic_slave.c
* Version       : 1.00
* Device        : R5F563NB
* Tool-Chain    : Renesas RX Standard 1.2.0.0
* H/W Platform  : RSK+RX63N
* Description   : Demonstrates usage of IIC unit in slave mode, by simulating an
*                 EEPROM memory device, similar to the Renesas R1EX24xxx series.
*                 The memory capacity is 256 bytes, arranged as 265 x 8 bytes.
*
* Operation     : 1. Build and download the sample code to the RSK.
*
*                 2. Connect a compatiable EEPROM memory device as follows:
*
*                        IIC Master Device     |    RSK Slave
*                        -------------------------------------------------            
*                                    SDA     ->   SDA (JA1, pin 25)
*                                    SCL     ->   SCL (JA1, pin 26)
*                                    GND     ->     Any ground point.
*
*                        IIC Master device must be suitable powered.
*
*                 3. No resistor configuration is required.
*         
*                 4. Click the 'Resume' icon on the Renesas Debug toolbar to
*                    run to the beginning of the main() function, and click
*                    'Resume' again to run the software. The name of the sample
*                    will by displayed on the debug LCD.
*                    
*                 5. Open the Memory tab (Window->Show View->Memory). Right click
*                    and select 'Add Memory Monitor'. Set the address to
*                    0x00002000 and click Ok. Click the Real-time Refresh button
*                    on the Memory tab toolbar. You can view the contents of the
*                    simulated EEPROM memory in the memory viewer window. Note that
*                    if an RSK+RX63N with the IIC_Master sample is used as the master
*                    device, data will be written to the end of the simulated EEPROM,
*                    at address 0x000020F8.
*
*                 6. When a valid read request is sent from a master device, the
*                    RSK will respond correctly and display "Read M." on the
*                    debug LCD.
*
*                 7. When a valid write request is sent from a master device, the
*                    RSK will store the write data into the simulated EEPROM
*                     memory, and display "Write M." on the debug LCD.
*
*******************************************************************************/
/*******************************************************************************
* History       : 13 Aug. 2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
System Includes
*******************************************************************************/
/* Following header file provides standard integer type definitions. */
#include <stdint.h>
/* Following header file provides string type definitions. */
#include <string.h>

/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/
/* Defines RX63N LEDs and switches */
#include "rskrx63ndef.h"
/* Defines RX63N port registers */
#include "iorx63n.h"
/* Switch handler function definitions */
#include "switch.h"

/*******************************************************************************
* Project Includes
*******************************************************************************/
/* Defines IIC slave sequences used in this file */
#include "iic_eeprom_slave_app.h"
/* Defines LCD functions used in this file */
#include "lcd.h"

#include "intrinsics.h"
#include "hwsetup.h"

/*******************************************************************************
* Outline       : main
* Description   : Main program function. The function first initialises the debug
*                 LCD, and displays the sample name. Main then calls the IIC
*                 slave sequence function, which configures the RSK to behave as
*                 a simulated, 256 byte EEPROM device. See iic_eeprom_slave_app.c
*                 for more details.
* Argument      : none
* Return value  : none
*******************************************************************************/
void main(void)
{
  
    HardwareSetup();
  
    __enable_interrupt(); 
  
    /* Initialise the debug LCD */
    Init_LCD();
    
    /* Display the sample name on the debug LCD */
    Display_LCD(LCD_LINE1, "IIC SLAV");
    
    /* Start the simulated EEPROM slave sequence */
    SlaveSequence_EEPROM();
            
    /* Infinte while loop */
    while(1);
}
/*******************************************************************************
* End of function main
*******************************************************************************/
