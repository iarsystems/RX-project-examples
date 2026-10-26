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
* Copyright (C) 2010 Renesas Electronics Corporation. All rights reserved.    */ 
/*******************************************************************************
* File Name    : lcd.h
* Version      : 1.00
* Device       : R5F562N8
* Tool Chain   : IAR Embedded Workbench
* H/W Platform : RSK+RX62N
* Description  : Provides varaible and function delarations for LCD.c file.
*******************************************************************************/
/*******************************************************************************
* History    : 25.06.2010 Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* Macro Definitions
*******************************************************************************/
/* Multi inclusion prevention macro */
#ifndef LCD_H
#define LCD_H
/* RS register select pin */
#define RS_PIN PDL_IO_PORT_8_4
/* Display enable pin */  
#define E_PIN  PDL_IO_PORT_8_5
/* Data write/read definition */ 
#define DATA_WR 1
/* Control write/read defintion */
#define CTRL_WR 0
/* Maximum characters per line of LCD display. */  
#define NUMB_CHARS_PER_LINE  8
/* Number of lines on the LCD display */
#define MAXIMUM_LINES    2    
/* Character position of LCD line 1 */
#define LCD_LINE1 0
/* Character position of LCD line 2 */
#define LCD_LINE2 16
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
/* Set to ensure base delay of 1mS minimum */
#define DELAY_TIMING    0x0F

/******************************************************************************
Global Function Prototypes
******************************************************************************/
/* LCD initialisation function declaration */
void InitialiseLCD(void);
/* Update display function declaration */
void DisplayLCD(unsigned char, const char *);
/* LCD write function delcaration */
void LCD_write(unsigned char, char);
/* Clear LCD function delcaration */
void ClearLCD(void);
/* added from uIP demo */
void DisplayClear(void);
void DisplayuIPDemo(void);
void DisplayIPAddress(const unsigned short ipaddr[]);
/* end added */

/* End of multi inclusion prevention macro */
#endif
