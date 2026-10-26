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
* File Name    : iic_eeprom_master_app.c 
* Version      : 1.00
* Device       : R5F562GAADFP
* Tool Chain   : RX Family C Compiler 1.2.1.0
* H/W Platform : RSKRX62G
* Description  : Defines a program sequence that reads and writes to an EEPROM
*                IIC, using functions from iic_eeprom_master.c - refer to this
*                file for more detailed information.
*******************************************************************************/
/*******************************************************************************
* History       : 06.09.2012 Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
/* Defines standard string functions used in this file */
#include <string.h>
/* IO port RPDL functions used in this file */
#include "r_pdl_io_port.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* Defines LCD functions used in this file */
#include "lcd.h"
/* Defines switch functions and variables used in this file */
#include "switch.h"
/* Defines IIC EEPROM functions used in this file */
#include "iic_eeprom_master.h"
/* Provides declarations of functions defined in this file */
#include "iic_eeprom_master_app.h"

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
/* Bus activity indicator function prototype declaration */
void BusActivity_IIC(bool control);

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Imported global switch press flag variable */
extern volatile uint8_t gSwitchFlag;

/*******************************************************************************
* Outline      : MasterSequence_EEPROM
* Description  : This function waits in a while loop, and performs an EEPROM read
*                when switch SW2 is pressed, and an EEPROM write when switch SW3
*                is pressed. The written data is "XXRenesas IIC   ", where XX is
*                an incrementing data indentifier number. The read operation
*                checks the read data, and if it matches the written data it 
*                reports success on the debug LCD, along with the identifier 
*                belong to that data entry.
* Argument     : none
* Return value : none
*******************************************************************************/
void MasterSequence_EEPROM(void)
{
    /* Declare write buffer array, initialised with constant data written to the
       EEPROM each time. XX is replaced with the data identifier. */
    uint8_t write_buffer[] = "XXRenesas IIC   ";
    
    /* Declare the data identifier number array, stored as BCD ACII values with
       range 0 - 99 */
    uint8_t write_count_bcd[2] = "00";
    
    /* Define the read and write address container variables, intialised to 0 */
    uint16_t write_address = 0x0000, read_address = 0x0000;
    
    /* Define read buffer array */
    uint8_t read_buffer[16];
    
    /* Initialise the IIC unit to operate in master mode */
    Init_EEPROM_Master();
    
    /* Enter infinite while loop, to continuously poll the user switches */
    while(1)
    {
        /* Check if switch SW2 has been pressed */
        if(gSwitchFlag & SWITCHPRESS_2)
        {       
            /* Store write count in first two bytes of write buffer */
            write_buffer[0] = write_count_bcd[0];
            write_buffer[1] = write_count_bcd[1];
            
            /* Turn LED1 on to indicate IIC bus acitivity */
            BusActivity_IIC(true);
            
            /* Perform write */
            Write_EEPROM_Master(write_address, write_buffer, 16u);
        
            /* Turn off LED1 to indicate IIC bus acitivity ended */
            BusActivity_IIC(false);
        
            /* Write operation was successful */
            if(CheckStatus_EEPROM_Master())
            {
                /* Initialise temporary LCD buffer string */
                uint8_t lcd_buffer[] = "Wrote  0";
                
                /* Insert the write count into the first two characters of the
                   write buufer */
                lcd_buffer[6] = write_count_bcd[0];
                lcd_buffer[7] = write_count_bcd[1];
                
                /* Display contents of lcd buffer to debug LCD */
                DisplayLCD(LCD_LINE1, lcd_buffer);
                
                /* Increment write counter */
                write_count_bcd[1]++;
                
                /* Ensure write count stays within valid ascii decimal values */
                if(write_count_bcd[1] > '9')
                {
                    /* Increment tens decimal digit, and set units back to 0 */
                    write_count_bcd[0]++;
                    write_count_bcd[1] = '0';
                    
                    /* If write count exceeds 99, reset back to 0 */
                    if(write_count_bcd[0] > '9')
                    {
                        /* Set tens digit back to 0 */
                        write_count_bcd[0] = '0';
                    }
                }
            }
            /* Write operation failure */
            else
            {
                /* Display write error on debug LCD */
                DisplayLCD(LCD_LINE1, "Error W.");
            }
                    
            /* Clear the switch press flag */
            gSwitchFlag = 0x00;
        }
        
        /* Check if switch SW3 has been pressed */
        else if(gSwitchFlag & SWITCHPRESS_3)
        {
            /* Turn on LED1 to indicate IIC bus acitivity ended */
            BusActivity_IIC(true);
            
            /* Read from the EEPROM device */
            Read_EEPROM_Master(read_address, read_buffer, 16u);
            
            /* Turn LED1 off to indicate IIC bus activity ended */
            BusActivity_IIC(false);
            
            /* Check if read operation was successful */
            if(CheckStatus_EEPROM_Master())
            {
                /* Check if data read matches data written */
                if(memcmp(read_buffer + 2u, write_buffer + 2u, 14u))
                {
                    /* Mismatch between data sent and received, display error on
                       debug lcd */
                    DisplayLCD(LCD_LINE1, "Error R.");
                }
                /* Read data matches written data */
                else
                {
                    /* Declare lcd buffer string array */
                    uint8_t lcd_buffer[] = "Read  XX";
                
                    /* Replace the initialised 'XX' with the count value of the
                       read string */
                    lcd_buffer[6] = read_buffer[0];
                    lcd_buffer[7] = read_buffer[1];
                
                    /* Write the contents of the lcd buffer to the debug lcd */
                    DisplayLCD(LCD_LINE1, lcd_buffer);
                }
            }
            /* Read operation unsuccessful */
            else
            {
                /* Read operation failed, report failure to debug lcd */
                DisplayLCD(LCD_LINE1, "Error R.");
            }
            
            /* Clear the switch press flag */
            gSwitchFlag = 0x00;
        }   
    
    }
}
/*******************************************************************************
* End of function MasterSequence_EEPROM
*******************************************************************************/

/*******************************************************************************
* Outline      : BusActivity_IIC
* Description  : Simple function that toggles LED1 on and off, based on a
*                boolean input value. Intended to be used as an indicator of IIC
*                bus acitivity.
* Argument     : bool control - True turns LED1 on, false turns LED1 off.
* Return value : none
*******************************************************************************/
void BusActivity_IIC(bool control)
{
    /* Declare error flag */
    bool err = true;
    
    /* Invert control value, as LEDs operate active low */
    control = (bool)!control;
    
    /* Turn LED1 on or off, depending on control argument */
    err &=  R_IO_PORT_Write
            (
                PDL_IO_PORT_7_1,
                (uint8_t)control
            );
    
    /* Halt in while loop when RPDL errors detected */
    while(!err);
}
/*******************************************************************************
* End of function BusActivity_IIC
*******************************************************************************/
