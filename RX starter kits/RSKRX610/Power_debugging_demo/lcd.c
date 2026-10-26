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
* File Name		: lcd.c
* Version 		: 1.00
* Device 		: R5F56107VNFP
* Tool Chain 	: RXC
* H/W Platform	: RSKRX610
* Description   : LCD Module utility functions.
*				  Written for KS0066u compatible LCD Module.
*	    		  (8 characters by 2 lines)
******************************************************************************/

/******************************************************************************
* History 		: 31.3.2009 Ver. 1.00 First Release
*””FILE COMMENT END””*********************************************************/

/******************************************************************************
User Includes (Project level includes)
******************************************************************************/

/* Contains declarations for functions defined in this file */
#include "lcd.h"

/******************************************************************************
Function Prototypes
******************************************************************************/
static void LCD_nibble_write(unsigned char, unsigned char);


/******************************************************************************
User Program Code
******************************************************************************/

void lcd_delay(void)
{
    for (int i=0;i<6000;i++) {
      asm("nop");
    }
}

void SetupLCDPorts(void)
{
  RS_PIN = 0;
  E_PIN  = 0;
  LCD_DATA_PORT = 0;
  
  LCD_DATA_PORT_DDR = 0xF0;
  RS_PIN_DDR = 1;
  E_PIN_DDR = 1;
}


/*””FUNC COMMENT””*************************************************************
* Outline 		: InitialiseDisplay
* Description 	: Initializes the LCD display. 
* Argument  	: none
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
void InitialiseLCD(void)
{
  /* Power Up Delay for the LCD Module */
  lcd_delay();

  /* Display initialises in 8 bit mode - so send one write (seen as 8 bit)
      to set to 4 bit mode. */
  LCD_nibble_write(CTRL_WR, 0x03);
  lcd_delay();
  
  LCD_nibble_write(CTRL_WR, 0x03);
  lcd_delay();

  LCD_nibble_write(CTRL_WR, 0x03);
  lcd_delay();
    
  /* Function Set */
  LCD_nibble_write(CTRL_WR, 0x02);
  lcd_delay();
  LCD_nibble_write(CTRL_WR, 0x02);
  LCD_nibble_write(CTRL_WR, (LCD_DISPLAY_ON | LCD_TWO_LINE ));
  lcd_delay();
 
  /* Display ON/OFF control */
  LCD_write(CTRL_WR, LCD_CURSOR_OFF);
  lcd_delay();

  /* Display Clear */
  LCD_write(CTRL_WR, LCD_CLEAR);
  lcd_delay();

  /* Entry Mode Set */
  LCD_write(CTRL_WR, 0x06);
  lcd_delay();

  /* Home the cursor */
  LCD_write(CTRL_WR, LCD_HOME_L1);
  lcd_delay();
}
/******************************************************************************
End of function InitialiseDisplay
******************************************************************************/


/*””FUNC COMMENT””*************************************************************
* Outline 		: ClearLCD
* Description 	: Clear the LCD.
* Argument  	: None.
* Return value  : None.
*””FUNC COMMENT END””*********************************************************/
void ClearLCD(void)
{
  /* Display Clear */
  LCD_write(CTRL_WR, LCD_CLEAR);
  lcd_delay();
}


/*””FUNC COMMENT””*************************************************************
* Outline 		: DisplayLCD
* Description 	: This function controls LCD writes to line 1 or 2 of the LCD.
*                 You need to use the defines LCD_LINE1 and LCD_LINE2 in order
*				  to specify the starting position.
*				  For example, to start at the 2nd position on line 1...
*				   		DisplayLCD(LCD_LINE1 + 1, "Hello") 
* Argument  	: (unsigned char)position -	Line number of display 
*			      (const char *) string -	Pointer to data to be written to display.
*			    	           		Last character should be null.
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
void DisplayLCD(unsigned char position, const char * string)
{
  static unsigned char next_pos = 0xFF;

  /* Set line position if needed. We don't want to if we don't need 
	   to because LCD control operations take longer than LCD data
	   operations. */
  if( next_pos != position)
  {
    if(position < LCD_LINE2)
    {
      /* Display on Line 1 */
      LCD_write(CTRL_WR, ((char)(LCD_HOME_L1 + position)));
    }
    else
    {
      /* Display on Line 2 */
      LCD_write(CTRL_WR, ((char)((LCD_HOME_L2 + position) - LCD_LINE2)));
    }
    lcd_delay();
    
    /* set position index to known value */
    next_pos = position;
  }

  do
  {
    LCD_write(DATA_WR,*string++);
    lcd_delay();
    
    /* increment position index */
    next_pos++;
  } 
  while(*string);
}
/******************************************************************************
End of function DisplayString
******************************************************************************/


/*””FUNC COMMENT””*************************************************************
* Outline 		: LCD_write
* Description 	: This function controls LCD writes to line 1 or 2 of the LCD.
*                 You need to use the defines LCD_LINE1 and LCD_LINE2 in order
*				  to specify the starting position.
*				  For example, to start at the 2nd position on line 1...
*				   		DisplayString(LCD_LINE1 + 1, "Hello") 
* Argument  	: (unsigned char)value - the value to write
*				  (char) data_or_ctrl - To write value 
*					as DATA or CONTROL
*						1 = DATA
*						0 = CONTROL
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
void LCD_write(unsigned char data_or_ctrl, char value)
{
  /* Write upper nibble first */
  LCD_nibble_write(data_or_ctrl, (unsigned char)((value & 0xF0) >> 4));

  /* Write lower nibble second */
  LCD_nibble_write(data_or_ctrl, (unsigned char)(value & 0x0F));
}
/******************************************************************************
End of function LCD_write
******************************************************************************/


/*””FUNC COMMENT””*************************************************************
* Outline 		: LCD_nibble_write
* Description 	: Writes data to display. Sends command to display. 
* Argument  	: (unsigned char) value - the value to write
*				  (char) data_or_ctrl - To write value  
*				    as DATA or CONTROL
*						1 = DATA
*						0 = CONTROL
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
static void LCD_nibble_write(unsigned char data_or_ctrl, unsigned char value)
{
  /* Set Register Select pin high for Data */
  if (data_or_ctrl == DATA_WR)
  {
    RS_PIN = 1;
  }
  else
  {
    RS_PIN = 0;
  }

  /* tsu1 delay */
  lcd_delay();
	
  /* EN enable chip (HIGH) */
  E_PIN = 1;

  /* Output the data */
  LCD_DATA_PORT = (unsigned char)(value << 4u);

  /* tw delay */
  lcd_delay();

  /* Latch data by dropping E */
  E_PIN = 0;

  /* th2 delay */
  lcd_delay();
	
  /* tc delay */
  lcd_delay();
}
/******************************************************************************
End of function LCD_nibble_write
******************************************************************************/
