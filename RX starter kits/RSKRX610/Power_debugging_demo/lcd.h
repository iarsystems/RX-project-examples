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
/* Copyright (C) 2009. Renesas Technology Europe, All Rights Reserved */
/*””FILE COMMENT””******************************* Technical reference data ****
* File Name		: lcd.h
* Version 		: 1.00
* Device 		: R5F56107VNFP
* Tool Chain 	: RX Family C Compiler
* H/W Platform	: RSKRX610
* Description 	: This Header file contains the macro defines & prototype for 
*				  the functions used in lcd.c
*******************************************************************************/

/******************************************************************************
* History 		: 22.01.2010 Ver. 1.00 First Release
*””FILE COMMENT END””*********************************************************/

#ifndef LCD_H
#define LCD_H

#include "iorx610.h"

/******************************************************************************
Macro Defines
*******************************************************************************/
/* RS Register Select pin */
#define RS_PIN PORT8.DR.BIT.B5
#define RS_PIN_DDR PORT8.DDR.BIT.B5

/* Display Enable pin */	
#define E_PIN  PORT8.DR.BIT.B6
#define E_PIN_DDR  PORT8.DDR.BIT.B6

/* Display Data port */
#define LCD_DATA_PORT PORT9.DR.BYTE
#define LCD_DATA_PORT_DDR PORT9.DDR.BYTE

#define DATA_WR 1
#define CTRL_WR 0

/* Maximum characters per line of LCD display. */	
#define NUMB_CHARS_PER_LINE	8
/* Number of lines on the LCD display */
#define MAXIMUM_LINES		2		

#define LCD_LINE1 0
#define LCD_LINE2 16

/*****************************************************************************/
/* LCD commands - use LCD_write function to write these commands to the LCD. */
/*****************************************************************************/
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

/******************************************************************************
Function Prototypes
******************************************************************************/
void SetupLCDPorts(void);
void InitialiseLCD(void);
void DisplayLCD(unsigned char, const char *);
void LCD_write(unsigned char, char);
void ClearLCD(void);

#endif
