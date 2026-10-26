/*******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. 

* No other uses are authorized.
*
* This software is owned by Renesas Electronics Corp. and is protected under
* all applicable laws, including copyright laws.
*
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
*
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
*
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.    
*******************************************************************************/
/******************************************************************************
* File Name     : lcd.c
* Version       : 1.0
* Device(s)     : R5F563NB
* Tool-Chain    : Renesas RX Standard Toolchain 1.1.0.0
* OS            : None
* H/W Platform  : RSK+RX63N
* Description   : Contains 2x8 debug LCD drivers
*******************************************************************************
* History : DD.MM.YYYY     Version     Description
*         : 01.10.2011     1.00        First release
*         : 16.05.2012     1.01        Modified for Ethernet demo 
*******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
/* Defines standard type structures */
#include <stdint.h>
/* Defines boolean type structures */
#include <stdbool.h>
/* Defines RX machine instruction functions */
#include <machine.h>
/* Defines standard IO functions */
#include <stdio.h>

/* Defines Special Flag Registers (SFR)s for RX63N */
#include "iorx63n.h"
#include "lcd.h"

/***********************************************************************************
Macro Defines
***********************************************************************************/
#define RS_PIN          (PORTJ.PODR.BIT.B5)
#define EN_PIN          (PORTF.PODR.BIT.B5)
#define DATA_PORT       (PORT8.PODR.BYTE)

#define LCD_RS_PDR      (PORTJ.PDR.BIT.B5)
#define LCD_EN_PDR      (PORTF.PDR.BIT.B5)
#define LCD_DATA_PDR    (PORT8.PDR.BYTE)

#define PDR_OUTPUT      (1)
#define PDR_INPUT       (0)

#define DATA_PORT_MASK  (0xF0)              /* Bit mask from entire port */
#define DATA_PORT_SHIFT (4)                 /* Port 8 bits 4-7 are data */


#define SET_BIT_HIGH    (1)
#define SET_BIT_LOW     (0)

#define DATA_WR         (1)
#define CTRL_WR         (0)

/* Set to ensure base delay of 1mS minimum */
#define DELAY_TIMING     (0x0A)

#define LCD_DISPLAY_ON   (0x04)
#define LCD_TWO_LINE     (0x08)

/* Clear LCD display and home cursor */
#define LCD_CLEAR        (0x01)
/* move cursor to line 1 */
#define LCD_HOME_L1      (0x80)
/* move cursor to line 2 */      
#define LCD_HOME_L2      (0xC0)
/* Cursor auto decrement after R/W */  
#define CURSOR_MODE_DEC  (0x04)
/* Cursor auto increment after R/W */
#define CURSOR_MODE_INC  (0x06)
/* Setup, 4 bits,2 lines, 5X7 */
#define FUNCTION_SET     (0x28)
/* Display ON with Cursor */
#define LCD_CURSOR_ON    (0x0E)
/* Display ON with Cursor off */
#define LCD_CURSOR_OFF   (0x0C)
/* Display on with blinking cursor */
#define LCD_CURSOR_BLINK (0x0D)
/*Move Cursor Left One Position */
#define LCD_CURSOR_LEFT  (0x10)
/* Move Cursor Right One Position */
#define LCD_CURSOR_RIGHT (0x14)

/******************************************************************************
Private global variables and functions
******************************************************************************/
static void lcd_write(uint8_t data_or_ctrl, uint8_t value);
static void lcd_nibble_write(uint8_t data_or_ctrl, uint8_t value);
static void lcd_delay(uint32_t units);

/******************************************************************************
* Function name: lcd_initialize
* Description  : Sets up LCD port pins and initailizes the display. 
* Arguments    : none
* Return value : none
******************************************************************************/
void lcd_initialize(void)
{
    /* set up LCD control signals */
    RS_PIN = 0;                         /* Set initial state of I/O pins data low */
    EN_PIN = 0;
    DATA_PORT = 0x00;
    LCD_RS_PDR = PDR_OUTPUT;            /* Set reset signal pin as output */
    LCD_EN_PDR = PDR_OUTPUT;            /* Set LCD enable signal pin as output */
    LCD_DATA_PDR |= DATA_PORT_MASK;     /* Set all data port bits as output */
    
    /* wait for the port direction setting */
    lcd_delay(2000); 

    /* Power Up Delay for LCD Module */
    EN_PIN = SET_BIT_HIGH;
    lcd_delay(25000);
    EN_PIN = SET_BIT_LOW;

    /* Display initialises in 8 bit mode - so send one write (seen as 8 bit)
    to set to 4 bit mode. */
    /* Function Set */
    lcd_nibble_write(CTRL_WR,0x03);
    lcd_nibble_write(CTRL_WR,0x03);
    lcd_delay(200); 
 
    /* Configure display */
    lcd_nibble_write(CTRL_WR,0x03);
    lcd_nibble_write(CTRL_WR,0x02);
    lcd_nibble_write(CTRL_WR,(LCD_DISPLAY_ON | LCD_TWO_LINE ));
    lcd_nibble_write(CTRL_WR,(LCD_DISPLAY_ON | LCD_TWO_LINE ));
    lcd_delay(200);

    /* Display ON/OFF control */
    lcd_write(CTRL_WR,LCD_CURSOR_OFF);
    lcd_delay(200);

    /* Display Clear */
    lcd_write(CTRL_WR,LCD_CLEAR);
    lcd_delay(25000);

    /* Entry Mode Set */
    lcd_write(CTRL_WR,0x06);
    lcd_write(CTRL_WR,LCD_HOME_L1);
    lcd_delay(25000);
}

/******************************************************************************
* Function name: lcd_display
* Description  : This function controls LCD writes to line 1 or 2 of the LCD.
*                You need to use the defines LCD_LINE1 and LCD_LINE2 in order
*                to specify the starting position.                           
*                For example, to start at the 2nd position on line 1...      
*                lcd_display(LCD_LINE1 + 1, "Hello")                    
* Arguments    : position - 
*                   line number of display
*                string - 
*                   pointer to NULL terminated string
* Return value : none
******************************************************************************/
void lcd_display(uint8_t position, uint8_t * string)
{
    static uint8_t next_pos = 0xFF;

    /* Set line position if needed. We don't want to if we don't need 
       to because LCD control operations take longer than LCD data
       operations. */
    if( next_pos != position)
    {
        if(position < LCD_LINE2)
        {
            /* Display on Line 1 */
            lcd_write(CTRL_WR, (uint8_t)(LCD_HOME_L1 + position) );
        }
        else
        {
            /* Display on Line 2 */
            lcd_write(CTRL_WR, (uint8_t)(LCD_HOME_L2 + position - LCD_LINE2) );
        }
        /* set position index to known value */
        next_pos = position;        
    }

    do
    {
        lcd_write(DATA_WR,*string++);
        /* increment position index */
        next_pos++;             
    }
    while(*string);


}

/******************************************************************************
* Function name: lcd_write
* Description  : Writes data and sends commands to display.
* Arguments    : data_or_ctrl - 
*                   1 = data
*                   0 = control
*                value - 
*                   the value to write
* Return value : none
******************************************************************************/
static void lcd_write(uint8_t data_or_ctrl, uint8_t value)
{
    /* Write upper nibble first */
    lcd_nibble_write(data_or_ctrl, (value & 0xF0) >> 4);
    /* Write lower nibble second */
    lcd_nibble_write(data_or_ctrl, (value & 0x0F));
}

/******************************************************************************
* Function name: lcd_nibble_write
* Description  : Writes data and sends commands to display in nibbles.
* Arguments    : data_or_ctrl - 
*                   1 = data
*                   0 = control
*                value - 
*                   the value to write
* Return value : none
******************************************************************************/
static void lcd_nibble_write(uint8_t data_or_ctrl, uint8_t value)
{
    uint8_t ucStore;

    /* Set Register Select pin high for Data */
    if (data_or_ctrl == DATA_WR)
    {
        RS_PIN = SET_BIT_HIGH;
    }
    else
    {
        RS_PIN = SET_BIT_LOW;
    }

    /* There must be 40ns between RS write and EN write */
    lcd_delay(5);                    
    /* EN enable chip (HIGH) */
    EN_PIN = SET_BIT_HIGH;
    /* Tiny delay */            
    lcd_delay(5);

    /* Clear port bits used */  
    ucStore = DATA_PORT;
    ucStore &= ~DATA_PORT_MASK;
    /* OR in data */    
    ucStore |= ((value << DATA_PORT_SHIFT) & DATA_PORT_MASK );
    /* Write data to port */    
    DATA_PORT = ucStore;
    /* write delay while En High */             
    lcd_delay(80);
    /* Latch data by dropping EN */                 
    EN_PIN = SET_BIT_LOW;
    /* Data hold delay */               
    lcd_delay(80);                   

    if (data_or_ctrl == CTRL_WR)
    {
        /* Extra delay needed for control writes */
        lcd_delay(0x7FF);
    }               
}

/******************************************************************************
* Function name: lcd_delay
* Description  : Delay routine for LCD display. 
* Arguments    : units - 
*                   Number of microseconds (approximately) 
* Return value : none
******************************************************************************/
#pragma optimize=no_inline
static void lcd_delay(uint32_t units)
{
    uint32_t counter = units * DELAY_TIMING;

    while(counter--)
    {
        nop();                      
    }
}

/**********************************************************************************
Name:           lcd_uIP_demo
Description:    Clears the LCD and displays "uIP Demo" on line 1.    
Parameters:     None
Returns:        None
**********************************************************************************/
void lcd_uip_demo(void)
{
    lcd_clear();
    lcd_display(LCD_LINE1, "uIP Demo");
}

/**********************************************************************************
Name:           lcd_link_missing
Description:    Clears the LCD and displays a message instructing the user to
                check the Ethernet link (cable).    
Parameters:     None
Returns:        None
**********************************************************************************/
void lcd_link_missing(void)
{
    lcd_clear();
    lcd_display(LCD_LINE1, "  Link  ");
    lcd_display(LCD_LINE2, "  down  ");
}


/**********************************************************************************
Name:           lcd_ip_address
Description:    Clears the LCD and displays IP address in dotted decimal format. 
                First two decimals are displyed on line 1 and the others on
                line 2.    
Parameters:     (unsigned short []) ipaddr - IP address to display
Returns:        None
**********************************************************************************/
void lcd_ip_address(const unsigned short ipaddr[])
{
    char dis_buf[9];
    char *p = (char *)ipaddr;
    lcd_clear();
    sprintf(dis_buf, "%d.%d.", *p, *(p+1));
    lcd_display(LCD_LINE1, (uint8_t *)dis_buf);
    sprintf(dis_buf, "%d.%d.", *(p+2), *(p+3));
    lcd_display(LCD_LINE2, (uint8_t *)dis_buf);
}

/**********************************************************************************
Name:           lcd_clear
Description:    Clears the LCD.      
Parameters:     None
Returns:        None
**********************************************************************************/
void lcd_clear(void)
{
    lcd_display(LCD_LINE1, "        ");
    lcd_display(LCD_LINE2, "        ");
}


