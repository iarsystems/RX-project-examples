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
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.    */ 
/*******************************************************************************
* File Name    : lcd.h
* Version     : 2.00
* Device     : R5F562TAADFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX62T
* Description   : This Header file contains the macro defines & prototype for 
*          the functions used in lcd.c
*******************************************************************************/
/*******************************************************************************
* History     : 15.10.2010 Ver. 1.00 First Release
              : 21.05.2011 Ver. 2.00 Second Release (Removed RPDL)
*******************************************************************************/

/*******************************************************************************
User Includes (Project level includes)
*******************************************************************************/
/* Defines standard variable types used in this file */
#include "stdint.h"

/*******************************************************************************
* Macro Definitions
*******************************************************************************/
/* Multi inclusion prevention macro */
#ifndef LCD_H
#define LCD_H

/* Defines for setting LCD signal states */
#define SET_BIT_HIGH        1
#define SET_BIT_LOW         0

/* Macro used for clearing the LCD display */
#define LCD_FLASH  {Delay(0x00800000); DisplayString(LCD_LINE1, "        "); \
           DisplayString(LCD_LINE2, "        "); Delay(0x00002000);}

/*******************************************************************************
Macro Defines
*******************************************************************************/
/* RS register select pin */
#define RS_PIN         PORTE.DR.BIT.B1
/* Display enable pin */  
#define EN_PIN        PORTE.DR.BIT.B0

/* Data bus port */
#define DATA_PORT      PORT9.DR.BYTE
/* Bit mask from entire port */     
#define DATA_PORT_MASK    0xF0
/* Number of bits data needs to shift */  
#define DATA_PORT_SHIFT    4    

/* Data write/read definition */ 
#define DATA_WR 1
/* Control write/read defintion */
#define CTRL_WR 0

/* Set to ensure base delay of 1mS minimum */
#define DELAY_TIMING    0x0F

/* Maximum characters per line of LCD display. */  
#define NUMB_CHARS_PER_LINE  8
/* Number of lines on the LCD display */
#define MAXIMUM_LINES    2    
/* Character position of LCD line 1 */
#define LCD_LINE1       0
/* Character position of LCD line 2 */
#define LCD_LINE2       16

#define LINE1_1STCH     0
#define LINE2_1STCH     16

/******************************************************************************/
/* LCD commands - use LCD_write function to write these commands to the LCD.  */
/******************************************************************************/
/* Clear LCD display and home cursor */
#define LCD_CLEAR        0x01
/* move cursor to line 1 */
#define LCD_HOME_L1      0x80
/* move cursor to line 2 */      
#define LCD_HOME_L2      0xC0
/* Cursor auto decrement after R/W */  
#define CURSOR_MODE_DEC  0x04
/* Cursor auto increment after R/W */
#define CURSOR_MODE_INC  0x06
/* Setup, 4 bits,2 lines, 5X7 */
#define FUNCTION_SET     0x28
/* Display ON with Cursor */
#define LCD_CURSOR_ON    0x0E
/* Display ON with Cursor off */
#define LCD_CURSOR_OFF   0x0C
/* Display on with blinking cursor */
#define LCD_CURSOR_BLINK 0x0D
/* Move Cursor Left One Position */
#define LCD_CURSOR_LEFT  0x10
/* Move Cursor Right One Position */
#define LCD_CURSOR_RIGHT 0x14     
/* Enable LCD display */
#define LCD_DISPLAY_ON   0x04
/* Enable both LCD lines */
#define LCD_TWO_LINE     0x08

#define FOUR_DIGITS_PLUS_NULL   5

/*******************************************************************************
Function Prototypes
*******************************************************************************/
void InitialiseDisplay( void );
void DisplayString(uint8_t position, uint8_t * string);
void LCD_write(unsigned char data_or_ctrl, unsigned char value);
void LCD_nibble_write(unsigned char data_or_ctrl, unsigned char value);
void DisplayDelay(unsigned long int units);
void Convert_16BitNumber_ToString(unsigned int c, char* buffer);
void LcdShow2DigHex( uint8_t  data, uint8_t  position);
void LcdShow4DigHex(  uint8_t  msbyte, 
            uint8_t  lsbyte,
            uint8_t   position);
void IntToAsciiHex( uint8_t*    dest_string,
          uint8_t    min_digits, 
          uint32_t   value );
#endif
