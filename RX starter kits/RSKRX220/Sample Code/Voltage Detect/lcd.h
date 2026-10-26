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
* File Name     : lcd.h
* Version       : 1.00
* Device        : R5F52206
* Tool-Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX220
* Description   : This Header file contains the Macro Definitions & prototypes  
*                 for the functions used in lcd.c
*******************************************************************************/
/*******************************************************************************
* History       : 14.12.2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
Macro Definitions
*******************************************************************************/

/* Multiple inclusion prevention macro */
#ifndef LCD_H
#define LCD_H

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/

/* Defines standard variable types used in this file */
#include <stdint.h>

/*******************************************************************************
Macro Definitions
*******************************************************************************/

/* RS register select pin */
#define RS_PIN              PORTJ.PODR.BIT.B1

/* RS register select pin */
#define RS_PIN_DIR          PORTJ.PDR.BIT.B1

/* Display enable pin */    
#define E_PIN               PORTJ.PODR.BIT.B3

/* Display enable pin */    
#define E_PIN_DIR           PORTJ.PDR.BIT.B3

/* Data bus port */
#define LCD_DATA_PORT       PORTH.PODR.BYTE

/* Data bus direction port */
#define LCD_DATA_DIR_PORT   PORTH.PDR.BYTE

/* Bit mask from entire port */
#define DATA_PORT_MASK      0x0Fu

/* Number of bits data needs to shift */    
#define DATA_PORT_SHIFT     0x0

/* Data write definition */
#define DATA_WR             0x1u

/* Control write definition */
#define CTRL_WR             0x0

/* Set to ensure base delay, determined empirically */
#define DELAY_TIMING        0x004u

/* Standard display operation delay (us) */
#define DISPLAY_WAIT        300u

/* Display control operation delay (us) */
#define DISPLAY_CTRL_WAIT   (3u * DISPLAY_WAIT)

/* Maximum characters per line of LCD display. */
#define NUMB_CHARS_PER_LINE 8u

/* Number of lines on the LCD display */
#define MAXIMUM_LINES       2u

/* Starting position for line 1 */
#define LCD_LINE1           0

/* Starting position for line 2 */
#define LCD_LINE2           16u

/*****************************************************************************/
/* LCD commands - use LCD_write function to write these commands to the LCD. */
/*****************************************************************************/

/* Clear LCD display and home cursor */
#define LCD_CLEAR           0x01u

/* Move cursor to line 1 */
#define LCD_HOME_L1         0x80u

/* Move cursor to line 2 */
#define LCD_HOME_L2         0xC0u

/* Cursor auto decrement after R/W */
#define CURSOR_MODE_DEC     0x04u

/* Cursor auto increment after R/W */
#define CURSOR_MODE_INC     0x06u

/* Setup, 4 bits, 2 lines, 5X7 */
#define FUNCTION_SET        0x28u

/* Display on with cursor */
#define LCD_CURSOR_ON       0x0Eu

/* Display on with Cursor off */
#define LCD_CURSOR_OFF      0x0Cu

/* Display on with blinking cursor */
#define LCD_CURSOR_BLINK    0x0Du

/* Move cursor left one position */
#define LCD_CURSOR_LEFT     0x10u

/* Move cursor right one position */
#define LCD_CURSOR_RIGHT    0x14u

/* Turn ON the display */
#define LCD_DISPLAY_ON      0x04u

/* Initialise 2 line display */
#define LCD_TWO_LINE        0x08u

/* Initialise in 8-bit mode */
#define LCD_8_BIT           0x03u

/* Initialise in 4-bit mode */
#define LCD_4_BIT           0x02u

/******************************************************************************
Function Prototypes
******************************************************************************/

/* Initializes the debug LCD */
void Init_LCD (void);

/* This function writes to line 1 or 2 of the LCD */
void Display_LCD (const uint8_t position, const char * string);

/* Delay routine for LCD or any other devices */
void DisplayDelay (const uint32_t units);

#endif

