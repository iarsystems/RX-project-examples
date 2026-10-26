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
* File Name     : iic_eeprom_master_app.c
* Version       : 1.00
* Device        : R5F563TE
* Tool-Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX63T144
* Description   : Defines a program sequence that reads and writes to an EEPROM
*                 IIC, using functions from iic_eeprom_master.c - refer to this
*                 file for more detailed information.
*******************************************************************************/
/*******************************************************************************
* History       : 05.12.2013  Ver. 1.00 First Release
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
/* Defines R5F563TE LEDs and switches */
#include "rskrx63tdef.h"

/* Defines R5F563TE port registers */
#include <iorx63t.h>

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
static void bus_activity_iic(const bool control);

/*******************************************************************************
* Local Variables
*******************************************************************************/
/* Define read buffer array */
char g_read_buffer[] = "xxxxxxx";

/*******************************************************************************
* Function Name: MasterSequence_EEPROM
* Description  : This function performs an EEPROM
*                read when switch SW2 is pressed, and an EEPROM write when
*                switch SW3 is pressed. The written data is "XX RX63T",
*                where XX is an incrementing data identifier number. The read
*                operation checks the read data, and if it matches the written
*                data it reports success on the debug LCD, along with the
*                identifier belonging to that data entry. The last data byte in
*                the write_buffer array is used to indicate the end of
*                transmission. It is not transmitted to the slave device and
*                omitted from the comparison between the write_buffer and
*                g_read_buffer data arrays.
* Argument     : none
* Return value : none
*******************************************************************************/
void MasterSequence_EEPROM (void)
{
    /* Declare a status variable */
    bool            status = 0;

    /* Declare write buffer array, initialised with constant data written to the
    EEPROM each time. XX is replaced with the data identifier before writing
    and comapred to the g_read_buffer on master read.  Therefore it needs static
    storage class specifier to ensure persistence through successive calls to
    this function */
    static char     write_buffer[] = "XX RX63T";

    /* Declare the data identifier number array, stored as BCD ASCII values with
    range 0 - 99.  This is incremented on each write, so needs the static
    storage class specifier to ensure persistence through successive calls to
    this function */
    static char     write_count_bcd[2] = {'0', '0'};

    /* Define the read and write address variables, initialised to 0 */
    const uint8_t   write_address = 0x00;
    const uint8_t   read_address = 0x00;

    /* Check if switch SW2 has been pressed */
    if (g_switch_flag & SWITCHPRESS_2)
    {
        /* Store write count in first two bytes of write buffer */
        write_buffer[0] = write_count_bcd[0];
        write_buffer[1] = write_count_bcd[1];

        /* Turn LED1 on to indicate IIC bus activity */
        bus_activity_iic(true);

        /* Perform write */
        status = Write_EEPROM_Master(write_address, write_buffer, 9u);

        /* Turn off LED1 to indicate IIC bus activity ended */
        bus_activity_iic(false);

        /* Write operation was successful */
        if (true == status)
        {
            /* Initialise temporary LCD buffer string */
            char lcd_buffer[] = "Wrote  0";

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
    /* Check if switch SW3 has been pressed */
    else if (g_switch_flag & SWITCHPRESS_3)
    {
        /* Turn on LED1 to indicate IIC bus activity ended */
        bus_activity_iic(true);

        /* Clear the contents of the g_read_buffer array */
        memset(g_read_buffer, 0xFFu, sizeof(g_read_buffer));

        /* Read from the EEPROM device */
        status = Read_EEPROM_Master(read_address, 8u);

        /* Turn LED1 off to indicate IIC bus activity ended */
        bus_activity_iic(false);

        /* Check if read operation was successful */
        if (true == status)
        {
            /* Check if data read matches data written */
            if (memcmp((g_read_buffer), (write_buffer), 8u))
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
                lcd_buffer[6] = g_read_buffer[0];
                lcd_buffer[7] = g_read_buffer[1];

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
    else
    {
        /* Do nothing */
    }
}
/*******************************************************************************
* End of function MasterSequence_EEPROM
*******************************************************************************/

/*******************************************************************************
* Function Name: bus_activity_iic
* Description  : Simple function that toggles LED1 on and off, based on a
*                boolean input value. Intended to be used as an indicator of IIC
*                bus activity.
* Argument     : bool control - True turns LED1 on, false turns LED1 off.
* Return value : none
*******************************************************************************/
static void bus_activity_iic (const bool control)
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
            /* Wait */
        }

        /* Turn LED1 off */
        LED1 = LED_OFF;
    }
}
/*******************************************************************************
* End of function bus_activity_iic
*******************************************************************************/

