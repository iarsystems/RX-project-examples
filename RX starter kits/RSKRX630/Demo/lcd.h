/******************************************************************************
 * DISCLAIMER:
 * The software supplied by Renesas Technology Europe Ltd is
 * intended and supplied for use on Renesas Technology products.
 * This software is owned by Renesas Technology Europe, Ltd. Or
 * Renesas Technology Corporation and is protected under applicable
 * copyright laws. All rights are reserved.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS". NO WARRANTIES, WHETHER EXPRESS,
 * IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * APPLY TO THIS SOFTWARE. RENESAS TECHNOLOGY AMERICA, INC. AND
 * AND RENESAS TECHNOLOGY CORPORATION RESERVE THE RIGHT, WITHOUT
 * NOTICE, TO MAKE CHANGES TO THIS SOFTWARE. NEITHER RENESAS
 * TECHNOLOGY AMERICA, INC. NOR RENESAS TECHNOLOGY CORPORATION SHALL,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES FOR ANY REASON WHATSOEVER ARISING OUT OF THE
 * USE OR APPLICATION OF THIS SOFTWARE.
 ******************************************************************************/

/* Copyright (C) 2010. Renesas Technology Corp, All Rights Reserved */
/* Copyright (C) 2010. Renesas Solutions Corp, All Rights Reserved */
/* Copyright (C) 2010. Renesas Technology Europe, All Rights Reserved */

/*""FILE COMMENT""******************************* Technical reference data ****
 * File Name		: lcd.h
 * DESCRIPTION   : 
 * Version 		: 1.00
 * Device 		: RX610
 * Tool Chain 	: RX Tool 1.00
 * H/W Platform	: RSKRX610 WS version
 *
 ******************************************************************************/
/******************************************************************************
 * History 		: 29.1.2010 Ver. 1.00 First Release
 *""FILE COMMENT END""*********************************************************/
#ifndef _LCD_H
#define _LCD_H

/******************************************************************************
User Includes (Project level includes)
******************************************************************************/
/* for RX610 SFR definition file */
//#include "RX62Niodefine.h"				

/******************************************************************************
Definition Functions
******************************************************************************/
void InitialiseDisplay( void );
void ClearLCD( void );
void DisplayString(unsigned char position, char * string);
void LCD_write(unsigned char data_or_ctrl, unsigned char value);
void LCD_nibble_write(unsigned char data_or_ctrl, unsigned char value);
void DisplayDelay(unsigned long int units);
void Convert_16BitNumber_ToString(unsigned int c, char* buffer);

/***********************************************************************/
/*  Definition of constant value                                       */
/***********************************************************************/
/* RS Register Select pin */
#define RS_PIN 				PORTJ.PODR.BIT.B3
/* Display Enable pin */	
#define EN_PIN				PORT3.PODR.BIT.B3

/* Data bus port */
#define DATA_PORT			PORTE.PODR.BYTE
/* Bit mask from entire port */	 	
#define DATA_PORT_MASK		0xf0
/* Number of bits data needs to shift */	
#define DATA_PORT_SHIFT		4		

#define DATA_WR 1
#define CTRL_WR 0

/* Set to ensure base delay of 1mS minimum */
#define DELAY_TIMING		0x05
/* number of lines on the LCD display */
#define NUMB_CHARS_PER_LINE	8
/* Maximum charactors per line of LCD display. */	
#define MAXIMUM_LINES		2		

#define LCD_LINE1 0
#define LCD_LINE2 16

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
/*Move Cursor Left One Position */
#define LCD_CURSOR_LEFT  0x10
/* Move Cursor Right One Position */
#define LCD_CURSOR_RIGHT 0x14     

#define LCD_DISPLAY_ON   0x04
#define LCD_TWO_LINE     0x08

#define SET_BIT_HIGH	(1)					
#define SET_BIT_LOW		(0)					
#define SET_BYTE_HIGH	(0xFF)				
#define SET_BYTE_LOW	(0x00)

#endif
