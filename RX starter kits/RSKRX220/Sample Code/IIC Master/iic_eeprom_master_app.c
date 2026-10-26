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
* File Name     : iic_eeprom_master_app.c 
* Version       : 1.00
* Device        : R5F52206
* Tool-Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX220
* Description   : Defines a program sequence that reads and writes to an EEPROM
*                 IIC, using functions from iic_eeprom_master.c - refer to this
*                 file for more detailed information.
*******************************************************************************/
/*******************************************************************************
* History       : 14.12.2012  Ver. 1.00 First Release
*******************************************************************************/
 
/*******************************************************************************
System Includes
*******************************************************************************/

/* Following header file provides standard integer type definitions. */
#include <stdint.h>

/* Following header file provides string type definitions. */
#include <string.h>

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/

/* Defines RX220 LEDs and switches */
#include "rskrx220def.h"

/* Defines RX220 port registers */
#include <iorx220.h>

/* LCD controlling function prototypes & macro definitions */
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
void bus_activity_iic (const bool control);

/* Write data to the EEPROM and update the LCD */
static void write_data_eeprom (void);

/* Read data from the EEPROM, compare to what was written, and update the LCD */
static void read_data_eeprom (void);

/*******************************************************************************
* Global Variables
*******************************************************************************/

/* Imported global switch press flag variable */
extern volatile uint8_t g_switch_flag;

/* Declare write buffer array, initialised with constant data written to the
   EEPROM each time. XX is replaced with the data identifier. */
char write_buffer[] = "XX RX220";

/* Define read buffer array */
uint8_t read_buffer[] = "xxxxxxxxx";

/* Declare the data identifier number array, stored as BCD ASCII values with
       range 0 - 99 */
char write_count_bcd[2] = {'0', '0'};

/*******************************************************************************
* Function Name: master_sequence_eeprom
* Description  : This function waits in a while loop, and performs an EEPROM 
*                read when switch SW3 is pressed, and an EEPROM write when 
*                switch SW2 is pressed.  
* Arguments    : none
* Return Value : none
*******************************************************************************/
void master_sequence_eeprom (void)
{
    /* Initialise the IIC unit to operate in master mode */
    init_eeprom_master();
    
    /* Enter infinite while loop, to continuously poll the user switches */
    while (1)
    {
        /* Check if switch SW2 has been pressed */
        if (g_switch_flag & SWITCHPRESS_2)
        {
            /* Write data to the EEPROM */
            write_data_eeprom();
        }
        
        /* Check if switch SW3 has been pressed */
        else if (g_switch_flag & SWITCHPRESS_3)
        {
            /* Read data from the EEPROM */
            read_data_eeprom();
        }
        else
        {
            /* Do nothing unless SW2/SW3 pressed */
        }   
    }
}
/*******************************************************************************
* End of function master_sequence_eeprom
*******************************************************************************/

/*******************************************************************************
* Function Name: write_data_eeprom
* Description  : Function to write data to the EEPROM and report success/failure
*                on the debug LCD. The written data is "XX RX220", where XX is
*                an incrementing data identifier number. The last data byte in 
*                the write_buffer array is used to indicate the end of 
*                transmission. It is not transmitted to the slave device and 
*                omitted from the comparison between the write_buffer and 
*                read_buffer data arrays.
* Arguments    : None
* Return Value : None
*******************************************************************************/
static void write_data_eeprom (void)
{
    /* Declare a status variable */
    bool status = 0;
    
    /* Store write count in first two bytes of write buffer */
    write_buffer[0] = write_count_bcd[0];
    write_buffer[1] = write_count_bcd[1];
    
    /* Turn LED1 on to indicate IIC bus activity */
    bus_activity_iic(true);
    
    /* Perform write */
    status = write_eeprom_master(EEPROM_MEMORY_LOCATION, 
            (uint8_t * )write_buffer, DATA_LENGTH);

    /* Turn off LED1 to indicate IIC bus activity ended */
    bus_activity_iic(false);

    /* Write operation was successful */
    if (true == status)
    {
        /* Initialise temporary LCD buffer string */
        char lcd_buffer[] = "Wrote XX";
        
        /* Insert the write count into the first two characters of the
           write buffer */
        lcd_buffer[6] = write_count_bcd[0];
        lcd_buffer[7] = write_count_bcd[1];
        
        /* Display contents of lcd buffer to debug LCD */
        Display_LCD(LCD_LINE2, lcd_buffer);
        
        /* Increment write counter */
        write_count_bcd[1]++;
        
        /* Ensure write count stays within valid ASCII decimal values */
        if (write_count_bcd[1] > '9')
        {
            /* Increment tens decimal digit, and set units back to 0 */
            write_count_bcd[0]++;
            write_count_bcd[1] = '0';
            
            /* If write count exceeds 99, reset back to 0 */
            if (write_count_bcd[0] > '9')
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
        Display_LCD(LCD_LINE2, "Error W.");
    }
            
    /* Clear the switch press flag */
    g_switch_flag = 0x00;
}
/*******************************************************************************
* End of function write_data_eeprom
*******************************************************************************/

/*******************************************************************************
* Function Name: read_data_eeprom
* Description  : Function to read and compare data from the EEPROM and report 
*                the result on the debug LCD. The read operation checks the read
*                data, and if it matches the written data it reports success on 
*                the debug LCD, along with the identifier belonging to that data
*                entry. The last data byte in the write_buffer array is used to 
*                indicate the end of transmission. It is not transmitted to the 
*                slave device and omitted from the comparison between the 
*                write_buffer and read_buffer data arrays.
* Arguments    : None
* Return Value : None
*******************************************************************************/
static void read_data_eeprom (void)
{
    /* Declare a status variable */
    bool status = 0;
    
    /* Turn on LED1 to indicate IIC bus activity ended */
    bus_activity_iic(true);
    
    /* Clear the contents of the read_buffer array */
    if (NULL == memset(read_buffer, 0xFFu, sizeof(read_buffer)))
    {
        /* Halt in while loop when errors detected */
        while (1)
        {
            /* Errors in clearing process - sample cannot continue */
        }
    }           
    
    /* Read from the EEPROM device */
    status = read_eeprom_master(EEPROM_MEMORY_LOCATION, DATA_LENGTH + 1);
    
    /* Turn LED1 off to indicate IIC bus activity ended */
    bus_activity_iic(false);
    
    /* Check if read operation was successful */
    if (true == status)
    {
        /* Check if data read matches data written */
        if (memcmp((read_buffer), (write_buffer), DATA_LENGTH - 1))
        {
            /* Mismatch between data sent and received, display error on
               debug lcd */
            Display_LCD(LCD_LINE2, "Error R.");
        }
        
        /* Read data matches written data */
        else
        {
            /* Declare lcd buffer string array */
            char lcd_buffer[] = "Read  XX";
        
            /* Replace the initialised 'XX' with the count value of the
               read string */
            lcd_buffer[6] = (char)read_buffer[0];
            lcd_buffer[7] = (char)read_buffer[1];
        
            /* Write the contents of the lcd buffer to the debug lcd */
            Display_LCD(LCD_LINE2, lcd_buffer);
        }
    }
    
    /* Read operation unsuccessful */
    else
    {
        /* Read operation failed, report failure to debug lcd */
        Display_LCD(LCD_LINE2, "Error R.");
    }
    
    /* Clear the switch press flag */
    g_switch_flag = 0x00;
}
/*******************************************************************************
* End of function read_data_eeprom
*******************************************************************************/

/*******************************************************************************
* Function Name: bus_activity_iic
* Description  : Simple function that toggles LED1 on and off, based on a
*                boolean input value. Intended to be used as an indicator of IIC
*                bus activity.
* Arguments    : (const bool)control - True turns LED1 on, false turns LED1 off.
* Return Value : none
*******************************************************************************/
void bus_activity_iic (const bool control)
{
    /* Check if control value is true */
    if (true == control)
    {
        /* Turn LED1 on */
        LED1 = LED_ON;
    }
    
    /* Control value is false */
    else
    {
        /* Create delay to make LED flash visible */
        uint32_t volatile wait = 0xFFFFF;
        while (wait--)
        {
            /* Wait briefly so LED flash is visible */
        }
        
        /* Turn LED1 off */
        LED1 = LED_OFF;
    }
}
/*******************************************************************************
* End of function bus_activity_iic
*******************************************************************************/

