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
* File Name     : lcd.c
* Version       : 1.00
* Device        : R5F52206
* Tool-Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX220
* Description   : LCD Module utility functions.
*                 Written for KS0066u compatible LCD Module.
*                 (8 characters by 2 lines)
*******************************************************************************/
/*******************************************************************************
* History       : 14.12.2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/

/* Following header file provides C99 type definitions. */
#include <stdint.h>

/* Defines RX220 port registers */
#include <iorx220.h>

/* Defines RX220 LEDs and switches */
#include "rskrx220def.h"

/* LCD controlling function prototypes & macro definitions */
#include "lcd.h"

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/

/* Writes data to display or sends command to display */
static void lcd_write (const uint8_t data_or_ctrl, const uint8_t value);

/* Declare LCD nibble write function prototype */
static void lcd_nibble_write (const uint8_t data_or_ctrl, const uint8_t value);

/*******************************************************************************
* Function Name: Init_LCD
* Description  : Initialises the LCD display. 
* Arguments    : none
* Return Value : none
*******************************************************************************/
void Init_LCD (void)
{
    /* Power-up Delay for LCD Module */
    E_PIN = SET_BIT_HIGH;

    /* Add delay */
    DisplayDelay(200000u);

    /* Set the enable pin low */
    E_PIN = SET_BIT_LOW;

    /* Reset LCD into 8-bit mode in case not already */
    lcd_nibble_write(CTRL_WR, LCD_8_BIT);
    lcd_nibble_write(CTRL_WR, LCD_8_BIT);

    /* Add delay */
    DisplayDelay(DISPLAY_CTRL_WAIT);

    /* Set display into 4-bit mode */
    lcd_nibble_write(CTRL_WR, LCD_8_BIT);
    lcd_nibble_write(CTRL_WR, LCD_4_BIT);
    
    /* Configure display */
    lcd_nibble_write(CTRL_WR, (LCD_DISPLAY_ON | LCD_TWO_LINE));
    lcd_nibble_write(CTRL_WR, (LCD_DISPLAY_ON | LCD_TWO_LINE));

    /* Add delay */
    DisplayDelay(DISPLAY_CTRL_WAIT);

    /* Display ON/OFF control */
    lcd_write(CTRL_WR, LCD_CURSOR_OFF);
    DisplayDelay(DISPLAY_CTRL_WAIT);

    /* Display Clear */
    lcd_write(CTRL_WR, LCD_CLEAR);
    DisplayDelay(20000);

    /* Entry Mode Set */
    lcd_write(CTRL_WR, 0x06u);
    lcd_write(CTRL_WR, LCD_HOME_L1);
    DisplayDelay(DISPLAY_CTRL_WAIT);
}
/*******************************************************************************
* End of function Init_LCD
*******************************************************************************/

/******************************************************************************
* Function Name: Display_LCD
* Description  : This function controls LCD writes to line 1 or 2 of the LCD.
*                You need to use the defines LCD_LINE1 and LCD_LINE2 in order
*                to specify the starting position.
*                For example, to start at the 2nd position on line 1...
*                      Display_LCD(LCD_LINE1 + 1, "Hello") 
* Arguments    : (uint8_t)position        - Line number of display. 
*                (const uint8_t *) string - Pointer to data to be written to
*                                           display. Last character should be
*                                           null.
* Return Value : none
*******************************************************************************/
void Display_LCD (const uint8_t position,  const char * string)
{
    static uint8_t next_pos = 0xFFu;

    /* Set a line position if needed. If current cursor position is outside of
    the NUMB_CHARS_PER_LINE value, the current data writes are skipped.
    If data is written byte to byte, no CTRL writes are written to LCD module
    (CTRL write takes more time) */
    if (next_pos != position)
    {
        /* Check if the next write should be 
           placed on the second line of the LCD */
        if (LCD_LINE2 > position)
        {
            /* Configure LCD to write on Line 1 */
            lcd_write(CTRL_WR, (uint8_t)(LCD_HOME_L1 + position));
        }
        else
        {
            /* Configure LCD to write on Line 2 */
            lcd_write(CTRL_WR, (uint8_t)((LCD_HOME_L2 + position) - LCD_LINE2));
        }
    
        /* Set position index to known value */
        next_pos = position;
    }

    do
    {
        /* Write the display data to the LCD */
        lcd_write(DATA_WR, (uint8_t) * string);
        
        /* Increment the current character pointer */
        string++;

        /* Add a delay */
        DisplayDelay(400u);

        /* Increment position index */
        next_pos++;
    }
    while (*string);
}
/*******************************************************************************
* End of function Display_LCD
*******************************************************************************/

/*******************************************************************************
* Function Name: lcd_write
* Description  : This function controls LCD writes to line 1 or 2 of the LCD.
*                You need to use the defines LCD_LINE1 and LCD_LINE2 in order
*                to specify the starting position.
*                For example, to start at the 2nd position on line 1...
*                      DisplayString(LCD_LINE1 + 1, "Hello") 
* Arguments    : (uint8_t)value         - The value to write
*                (uint8_t) data_or_ctrl - To write value as DATA or CONTROL
*                                         1 = DATA
*                                         0 = CONTROL
* Return Value : none
*******************************************************************************/
static void lcd_write (const uint8_t data_or_ctrl, const uint8_t value)
{
    /* Write upper nibble first */
    lcd_nibble_write(data_or_ctrl, (uint8_t)((value & ~DATA_PORT_MASK) >> 4u));
    
    /* Write lower nibble second */
    lcd_nibble_write(data_or_ctrl, (uint8_t)(value & DATA_PORT_MASK));
}
/*******************************************************************************
* End of function lcd_write
*******************************************************************************/

/******************************************************************************
* Function Name: lcd_nibble_write
* Description  : Writes data to display. Sends command to display. 
* Arguments    : (uint8_t) value        - the value to write
*                (uint8_t) data_or_ctrl - To write value as DATA or CONTROL
*                                         1 = DATA
*                                         0 = CONTROL
* Return Value : None
*******************************************************************************/
static void lcd_nibble_write (const uint8_t data_or_ctrl, const uint8_t value)
{
    uint8_t ucStore = 0x00;

    /* Check for the type of data and set RS_PIN */
    if (DATA_WR == data_or_ctrl)
    {
        RS_PIN = SET_BIT_HIGH;
    }
    else
    {
        RS_PIN = SET_BIT_LOW;
    }

    /* There must be a delay between RS write and EN write */
    DisplayDelay(12u);
    
    /* EN enable chip (HIGH) */
    E_PIN = SET_BIT_HIGH;
    
    /* Tiny delay  */
    DisplayDelay(20u);

    /* Get current data from data port */
    ucStore = LCD_DATA_PORT;
    
    /* Clear LCD bits */  
    ucStore &= ~DATA_PORT_MASK;
    
    /* Set new LCD bits */    
    ucStore |= value & DATA_PORT_MASK;
    
    /* Write data to port */    
    LCD_DATA_PORT = ucStore;

    /* Write delay while En High */
    DisplayDelay(DISPLAY_WAIT);

    /* Latch data by dropping EN */
    E_PIN = SET_BIT_LOW;
    
    /* Extra delay for write controls */
    DisplayDelay(DISPLAY_WAIT);
}
/******************************************************************************
* End of function lcd_nibble_write
******************************************************************************/

/******************************************************************************
* Function Name: DisplayDelay
* Description  : Delay routine for LCD or any other devices.
* Arguments    : (uint32_t) units - time in, approximately, microsec
* Return Value : None
******************************************************************************/
void DisplayDelay (const uint32_t units)
{
    volatile uint32_t counter = units * DELAY_TIMING;
    
    /* Decrement the counter and wait */
    while (counter--)
    {
        /* Do nothing until the delay time has passed */
    }
}
/******************************************************************************
* End of function DisplayDelay
******************************************************************************/

