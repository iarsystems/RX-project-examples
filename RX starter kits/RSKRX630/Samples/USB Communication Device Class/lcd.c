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
/* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.   */
/*******************************************************************************
* File Name    : lcd.c
* Version     : 1.00
* Device     : R5F5630E
* Tool-Chain   : Renesas RX Toolchain 1.2.0.0
* H/W Platform  : RSKRX630
* Description   : LCD Module utility functions.
*          Written for KS0066u compatible LCD Module.
*            (8 characters by 2 lines)
*******************************************************************************/
/*******************************************************************************
* History     : 23 Jan. 2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/
/* Following header file provides C99 type definitions. */
#include <stdint.h>
/* Defines RX630 port registers */
#include "iorx630_usb.h"
/* Defines RX630 LEDs and switches */
#include "rskrx630def.h"
/* LCD controlling function prototypes & macro definitions */
#include "lcd.h"

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
/* Declare LCD nibble write function prototype */
static void LCD_nibble_write(uint8_t, uint8_t);

/*******************************************************************************
* Outline     : Init_LCD
* Description   : Initialises the LCD display. 
* Argument      : none
* Return value  : none
*******************************************************************************/
void Init_LCD(void)
{
  /* Power-up Delay for LCD Module */
  E_PIN = SET_BIT_HIGH;

  /* Add delay */
  DisplayDelay(14000);

  /* Set the enable pin low */
  E_PIN = SET_BIT_LOW;

  /* Display initialises in 8 bit mode - so send one write (seen as 8 bit)
  to set to 4 bit mode. */

  /* Function Set */
  LCD_nibble_write(CTRL_WR,0x03);
  LCD_nibble_write(CTRL_WR,0x03);

  /* Add delay */
  DisplayDelay(80);

  /* Configure display */
  LCD_nibble_write(CTRL_WR,0x03);
  LCD_nibble_write(CTRL_WR,0x02);
  LCD_nibble_write(CTRL_WR,(LCD_DISPLAY_ON | LCD_TWO_LINE ));
  LCD_nibble_write(CTRL_WR,(LCD_DISPLAY_ON | LCD_TWO_LINE ));

  /* Add delay */
  DisplayDelay(80);

  /* Display ON/OFF control */
  LCD_write(CTRL_WR,LCD_CURSOR_OFF);
  DisplayDelay(80);

  /* Display Clear */
  LCD_write(CTRL_WR,LCD_CLEAR);
  DisplayDelay(3060);

  /* Entry Mode Set */
  LCD_write(CTRL_WR,0x06);
  LCD_write(CTRL_WR,LCD_HOME_L1);
  DisplayDelay(100);
}
/*******************************************************************************
* End of function Init_LCD
*******************************************************************************/

/******************************************************************************
* Outline     : Display_LCD
* Description   : This function controls LCD writes to line 1 or 2 of the LCD.
*                 You need to use the defines LCD_LINE1 and LCD_LINE2 in order
*          to specify the starting position.
*          For example, to start at the 2nd position on line 1...
*               Display_LCD(LCD_LINE1 + 1, "Hello") 
* Argument    : (uint8_t)position -  Line number of display 
*            (const uint8_t *) string -Pointer to data to be written to
*                          display. Last character should be null.
* Return value  : none
*******************************************************************************/
void Display_LCD(uint8_t position,  const uint8_t* string)
{
  static uint8_t next_pos = 0xFF;

  /* Set a line position if needed. If current cursor position is outside of
  the NUMB_CHARS_PER_LINE value, the current data writes are skipped.
  If data is written byte to byte, no CTRL writes are written to LCD module
  (CTRL write takes more time) */
  if (next_pos != position)
  {
    /* Check if the next write should be 
       placed on the second line of the LCD */
    if(position < LCD_LINE2)
    {
      /* Configure LCD to write on Line 1 */
      LCD_write(CTRL_WR, (uint8_t)(LCD_HOME_L1 + position));
    }
    else
    {
      /* Configure LCD to write on Line 2 */
      LCD_write(CTRL_WR, (uint8_t)((LCD_HOME_L2 + position) - LCD_LINE2));
    }
  
    /* Set position index to known value */
    next_pos = position;
  }

  do
  {
    /* Write the display data to the LCD */
    LCD_write(DATA_WR,*string++);

    /* Add a delay */
    DisplayDelay(100);

    /* Increment position index */
    next_pos++;
  }
  while (*string);
}
/*******************************************************************************
* End of function Display_LCD
*******************************************************************************/

/*******************************************************************************
* Outline     : LCD_write
* Description   : This function controls LCD writes to line 1 or 2 of the LCD.
*                 You need to use the defines LCD_LINE1 and LCD_LINE2 in order
*          to specify the starting position.
*          For example, to start at the 2nd position on line 1...
*               DisplayString(LCD_LINE1 + 1, "Hello") 
* Argument    : (uint8_t)value - the value to write
*          (uint8_t) data_or_ctrl - To write value 
*          as DATA or CONTROL
*            1 = DATA
*            0 = CONTROL
* Return value  : none
*******************************************************************************/
void LCD_write(uint8_t data_or_ctrl, uint8_t value)
{
  /* Write upper nibble first */
  LCD_nibble_write(data_or_ctrl, (uint8_t)((value & 0xF0) >> 4));
  
  /* Write lower nibble second */
  LCD_nibble_write(data_or_ctrl, (uint8_t)(value & 0x0F));
}
/*******************************************************************************
* End of function LCD_write
*******************************************************************************/

/******************************************************************************
* Outline     : LCD_nibble_write
* Description   : Writes data to display. Sends command to display. 
* Argument      : (uint8_t) value - the value to write
*          (uint8_t) data_or_ctrl - To write value  
*            as DATA or CONTROL
*            1 = DATA
*            0 = CONTROL
* Return value  : None
*******************************************************************************/
static void LCD_nibble_write(uint8_t data_or_ctrl, uint8_t value)
{
  uint8_t ucStore = 0x00;

  /* Check for the type of data and set RS_PIN */
  if (data_or_ctrl == DATA_WR)
  {
    RS_PIN = SET_BIT_HIGH;
  }
  else
  {
    RS_PIN = SET_BIT_LOW;
  }

  /* There must be 40ns between RS write and EN write */
  DisplayDelay(3);
  
  /* EN enable chip (HIGH) */
  E_PIN = SET_BIT_HIGH;
  
  /* Tiny delay */
  DisplayDelay(5);

  /* Write upper 4 bits of nibble */
  LCD_DATA_PORT =  (uint8_t)((ucStore << 4) & 0xF0);

  /* Clear port bits used */  
  ucStore = LCD_DATA_PORT;
  ucStore &= ~DATA_PORT_MASK;
  
  /* OR in data */  
  ucStore |= ((value << DATA_PORT_SHIFT) & DATA_PORT_MASK );
  
  /* Write data to port */  
  LCD_DATA_PORT = ucStore;

  /* Write delay while En High */
  DisplayDelay(50);

  /* Latch data by dropping EN */
    E_PIN = SET_BIT_LOW;

  /* Write delay while EN High */
  DisplayDelay(50);

  /* Latch data by dropping EN */
  E_PIN = SET_BIT_LOW;
  
  /* Extra delay for write controls */
  DisplayDelay(50);
}
/******************************************************************************
* End of Function LCD_nibble_write
******************************************************************************/

/******************************************************************************
* Outline     : DisplayDelay
* Description  : Delay routine for LCD or any other devices.
* Argument    : (uint32_t) units - time in, approximately, microsec
* Return value  : None
******************************************************************************/
void DisplayDelay(uint32_t units)
{
  uint32_t counter = units * DELAY_TIMING;
  
  /* Decrement the counter and wait */
  while (counter--)
  {
    ;
  }
}
/******************************************************************************
* End of Function DisplayDelay
******************************************************************************/
