/* Adapted for use with IAR Embedded Workbench */
/******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Technology Corp. and is only 
* intended for use with Renesas products. No other uses are authorized.

* This software is owned by Renesas Technology Corp. and is protected under 
* all applicable laws, including copyright laws.

* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, 
* INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
* PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY 
* DISCLAIMED.

* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* TECHNOLOGY CORP. NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES 
* FOR ANY REASON RELATED TO THE THIS SOFTWARE, EVEN IF RENESAS OR ITS 
* AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

* Renesas reserves the right, without notice, to make changes to this 
* software and to discontinue the availability of this software.  
* By using this software, you agree to the additional terms and 
* conditions found by accessing the following link:
* http://www.renesas.com/disclaimer
******************************************************************************
* Copyright (C) 2008. Renesas Technology Corp., All Rights Reserved.
*******************************************************************************  
* File Name    : ldc.c
* Version      : 1.00
* Description  : LCD Module utility functions.             
*              : Written for KS0066u compatible LCD Module.
*              : (8 characters by 2 lines)                 
******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 20.06.2007 1.00    First Release
******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <machine.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "lcd.h"
#include "glyph_api.h"

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
T_glyphHandle G_lcd ;

/*****************************************************************************
Name:            InitDisplay 
Parameters:     none                
Returns:        none
Description:    Intializes the LCD display. 
*****************************************************************************/
void InitialiseDisplay( void )
{
  if (GlyphOpen(&G_lcd, 0) == GLYPH_ERROR_NONE) 
  {
    /* use Glyph full access direct functions */
    GlyphNormalScreen(G_lcd) ;
    GlyphSetFont(G_lcd, GLYPH_FONT_8_BY_16) ;
    GlyphClearScreen(G_lcd) ;
  }
}

/*****************************************************************************
Name:          DisplayString   
Parameters:    position  Line number of display
string    Pointer to data to be written to display.
Last character should be null.
Returns:        none
Description:    This function controls LCD writes to line 1 or 2 of the LCD.
You need to use the defines LCD_LINE1 and LCD_LINE2 in order
to specify the starting position.
For example, to start at the 2nd position on line 1...
DisplayString(LCD_LINE1 + 1, "Hello")
*****************************************************************************/
void DisplayString(unsigned char lineNum, const char * string)
{
  // Figure out the number of characters
  // Clear the screen for n characters
  // Write the new characters

  // TODO: These two lines need to change to write more than 8 characters/line
  int8_t y = lineNum - (lineNum % 8);
  int8_t xOffset = (lineNum % 8) << 3 ;

  /* Draw text lines, 16 pixels high, 96 pixels wide */
  /* Clear the rectangle of this line */
  GlyphEraseBlock(G_lcd, xOffset, y, (95 - xOffset), y + 15);
  GlyphSetXY (G_lcd, xOffset, y);
  GlyphString(G_lcd, (uint8_t *)string, strlen(string));

}

/*****************************************************************************
Name:           Convert_16bitNumber_ToString
Description:    To convert a char number to hex ascii string of length of
one line of display hence suitable for use in a call to DisplayString.
Buffer supplied must be at least 9 chars long.
Parameters:     char, buffer
Return Value:   none
*****************************************************************************/
void Convert_16BitNumber_ToString(unsigned int c, char* buffer)
{
  char a;

  buffer[0] = '=';
  buffer[1] = 'H';
  buffer[2] = '\'';
  a = (char)((c & 0xF000) >> 12);
  buffer[3] = (a < 0x0A) ? (a+0x30) : (a+0x37);
  a = (char)((c & 0x0F00) >> 8);
  buffer[4] = (a < 0x0A) ? (a+0x30) : (a+0x37);
  a = (char)((c & 0x00F0) >> 4);
  buffer[5] = (a < 0x0A) ? (a+0x30) : (a+0x37);
  a = (char)(c & 0x000F);
  buffer[6] = (a < 0x0A) ? (a+0x30) : (a+0x37);

  /*  Add Spaces and a string terminating character.  */
  buffer[7] = ' ';
  buffer[8] = '\0';
}

/**********************************************************************************
Name:      DisplayuIPDemo
Description:  Clears the LCD and displays "uIP Demo" on line 1.    
Parameters:    None
Returns:    None
**********************************************************************************/
void DisplayuIPDemo(void)
{
  DisplayClear();
  GlyphClearScreen(G_lcd) ;
  DisplayString(LCD_LINE1, "uIP Demo");
}

/**********************************************************************************
Name:      DisplayIPAddress
Description:  Clears the LCD and displays IP address in dotted decimal format. 
First two decimals are displyed on line 1 and the others on
line 2.    
Parameters:    (unsigned short []) ipaddr - IP address to display
Returns:    None
**********************************************************************************/
void DisplayIPAddress(const unsigned short ipaddr[])
{
  char dis_buf[9];
  char *p = (char *)ipaddr;
  DisplayClear();
  sprintf(dis_buf, "%d.%d.", *p, *(p+1));
  DisplayString(LCD_LINE1, dis_buf);
  sprintf(dis_buf, "%d.%d.", *(p+2), *(p+3));
  DisplayString(LCD_LINE2, dis_buf);
}

/**********************************************************************************
Name:      DisplayClear
Description:  Clears the LCD.      
Parameters:    None
Returns:    None
**********************************************************************************/
void DisplayClear(void)
{
  GlyphClearScreen(G_lcd) ;
}


