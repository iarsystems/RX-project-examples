/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2022 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_okaya_lcd.h
* Version      : 1.00
* Device(s)    : R5F56609HxFB
* Tool-Chain   : CCRX
* H/W Platform : RSKRX660
* Description  : This Header file contains the Macro Definitions & prototypes for the functions used in r_okaya_lcd.c
*                
*                This function is created to drive the Okaya LCD display with either ST7735 or ST7715 driver device. 
*                The commands for both the devices are the same.
*                
*                The display is controlled using the SPI bus. In this example, the SCIx is used. This can be modified 
*                to the SCI connected to the PMOD interface. The SCI driver file will also be required.
*                
*                The display memory has an offset with respect to the actual pixel. This is not documented but 
*                realised from driving the display. The offset is set as LEFT MARGIN and TOP MARGIN. this offset is 
*                catered for internally, so as far as the user is concerned, cursor position 0,0 is the top left pixel.
* Creation Date: 2022/01/18
***********************************************************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef R_OKAYA_LCD_H
#define R_OKAYA_LCD_H

/***********************************************************************************************************************
Macro Definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/

/***********************************************************************************************************************
*
*  SCREEN
*
*  The screen size is 128 x 128 pixels, with coordinate 0,0 at the top left.
*  The display controller is ST7735 or ST7715.
*
***********************************************************************************************************************/
#define SCREEN_HEIGHT (128U)           /* 16 lines @ 8 bits = 128. */
#define SCREEN_WIDTH  (128U)

/* Pin defines common to both displays */
/* PMOD1-CS(Display enable) pin */
#define ENABLE_PIN                (PORTJ.PODR.BIT.B3)
#define ENABLE_PIN_DIR            (PORTJ.PDR.BIT.B3)

/* PMOD1-IO1(Reset) pin */
#define RESET_PIN                 (PORTL.PODR.BIT.B0)
#define RESET_PIN_DIR             (PORTL.PDR.BIT.B0)
#define RESET_PIN_PMR             (PORTL.PMR.BIT.B0)

/* PMOD1-IO2(DATA/COMMAND select) pin */
#define DATA_CMD_PIN              (PORT7.PODR.BIT.B1)
#define DATA_CMD_PIN_DIR          (PORT7.PDR.BIT.B1)

/* PMOD1-IO3(Vdd enable) pin */
#define BL_ENABLE_PIN             (PORT7.PODR.BIT.B2)
#define BL_ENABLE_PIN_DIR         (PORT7.PDR.BIT.B2)

/* Automatic calculation of parameters */
#define FONT_WIDTH                (6U)  /* Including a space */
#define FONT_HEIGHT               (8U)  /* Including 1 pixel space */
#define MAX_LINES                 (SCREEN_HEIGHT / FONT_HEIGHT)
#define CHAR_PER_LINE             (SCREEN_WIDTH / FONT_WIDTH)

/* Allow 2 pixel margin on the Left and Top */
#define LEFT_MARGIN               (2U)
#define TOP_MARGIN                (3U)

/* Ascii code */
#define CR                        (0x0dU)
#define LF                        (0x0aU)
#define BS                        (0x08U)

/***********************************************************************************************************************

  DISPLAY COLOR DEFINITIONS (16 bits) R5G6B5 format

  Only Primary & secondary colors are defined here. Other colors can be created using RGB values.

***********************************************************************************************************************/
#define COL_BLACK       (0X0000U)
#define COL_RED         (0Xf800U)
#define COL_GREEN       (0X07E0U)
#define COL_BLUE        (0X001FU)
#define COL_YELLOW      (0XFFE0U)
#define COL_CYAN        (0X07FFU)
#define COL_MAGENTA     (0XF81FU)
#define COL_WHITE       (0XFFFFU)

/***********************************************************************************************************************

  DISPLAY COMMAND SET ST7735

***********************************************************************************************************************/
#define ST7735_NOP      (0x0U)
#define ST7735_SWRESET  (0x01U)
#define ST7735_SLPIN    (0x10U)
#define ST7735_SLPOUT   (0x11U)
#define ST7735_PTLON    (0x12U)
#define ST7735_NORON    (0x13U)
#define ST7735_INVOFF   (0x20U)
#define ST7735_INVON    (0x21U)
#define ST7735_DISPON   (0x29U)
#define ST7735_CASET    (0x2AU)
#define ST7735_RASET    (0x2BU)
#define ST7735_RAMWR    (0x2CU)
#define ST7735_COLMOD   (0x3AU)
#define ST7735_MADCTL   (0x36U)
#define ST7735_FRMCTR1  (0xB1U)
#define ST7735_INVCTR   (0xB4U)
#define ST7735_DISSET5  (0xB6U)
#define ST7735_PWCTR1   (0xC0U)
#define ST7735_PWCTR2   (0xC1U)
#define ST7735_PWCTR3   (0xC2U)
#define ST7735_VMCTR1   (0xC5U)
#define ST7735_PWCTR6   (0xFCU)
#define ST7735_GMCTRP1  (0xE0U)
#define ST7735_GMCTRN1  (0xE1U)

/* Delay for delay counter */
#define DELAY_TIMING              (8U)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/
/* Initializes the debug LCD */
void R_LCD_Init (void);

/* Display string at specific line of display */
void R_LCD_Display (uint8_t const position,  uint8_t * const string);

/* Display the string at current cursor position */
void R_LCD_DisplayStr (uint8_t * str);

/* Display the string at the center of the specified line */
void R_LCD_DisplayCenter (uint8_t const line_num, uint8_t * const str);

/* Clears the display */
void R_LCD_ClearDisplay (const uint16_t color);

/* Clear a specified line */
void R_LCD_DisplayClearLine (const uint8_t line_num);

/* Set the current cursor position */
void R_LCD_DisplaySetCursor (uint8_t const x, uint8_t const y);

/* Set Font color */
void R_LCD_DisplaySetFontColor (uint16_t const col);

/* Set Background color */
void R_LCD_DisplaySetBackColor (uint16_t const col);

#endif /* R_OKAYA_LCD_H */

