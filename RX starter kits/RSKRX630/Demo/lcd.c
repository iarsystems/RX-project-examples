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
 * File Name		: lcd.c
 * DESCRIPTION   : Control for LCD module of RX610 CPU board
 * Version 		: 1.00
 * Device 		: RX610
 * Tool Chain 	: RX Tool 1.00
 * H/W Platform	: RSKRX610 WS version
 * Description 	: Intialise and disple dato via LCD module
 *
 ******************************************************************************/
/******************************************************************************
 * History 		: 29.1.2010 Ver. 1.00 First Release
 *""FILE COMMENT END""*********************************************************/

/******************************************************************************
System Includes (Project level includes)
******************************************************************************/

#include <machine.h>

/******************************************************************************
User Includes (Project level includes)
******************************************************************************/
/* for RX610 SFR definition file */
#include "iorx630.h"				
#include "lcd.h"

/*""FUNC COMMENT""*************************************************************
 * Outline 		: InitialiseDisplay
 * Description 	: Initializes the LCD display. 
 * Argument  	: none
 * Return value  : none
 *""FUNC COMMENT END""*********************************************************/
void InitialiseDisplay( void )
{
  /* set LCD control signal */
  RS_PIN = 0;    
  EN_PIN = 0;
  DATA_PORT = 0x00;
  PORTJ.PDR.BIT.B3 = 1;	//DLCDRS
  PORT3.PDR.BIT.B3 = 1;	//DLCDE
  PORTE.PDR.BYTE |= 0xf0;	//DLCDD4-7
  
  /* wait for the port direction setting */
  DisplayDelay(2000); 
  
  /* Power Up Delay for LCD Module */
  EN_PIN = SET_BIT_HIGH;
  DisplayDelay(25000);
  EN_PIN = SET_BIT_LOW;
  
  /* Display initialises in 8 bit mode - so send one write (seen as 8 bit)
     to set to 4 bit mode. */
  /* Function Set */
  LCD_nibble_write(CTRL_WR,0x03);
  LCD_nibble_write(CTRL_WR,0x03);
  DisplayDelay(200); 
  
  /* Configure display */
  LCD_nibble_write(CTRL_WR,0x03);
  LCD_nibble_write(CTRL_WR,0x02);
  LCD_nibble_write(CTRL_WR,(LCD_DISPLAY_ON | LCD_TWO_LINE ));
  LCD_nibble_write(CTRL_WR,(LCD_DISPLAY_ON | LCD_TWO_LINE ));
  DisplayDelay(200);
  
  /* Display ON/OFF control */
  LCD_write(CTRL_WR,LCD_CURSOR_OFF);
  DisplayDelay(200);
  
  /* Display Clear */
  LCD_write(CTRL_WR,LCD_CLEAR);
  DisplayDelay(25000);
  
  /* Entry Mode Set */
  LCD_write(CTRL_WR,0x06);
  LCD_write(CTRL_WR,LCD_HOME_L1);
  DisplayDelay(25000);
}
/******************************************************************************
End of InitialiseDisplay
******************************************************************************/
/*""FUNC COMMENT""*************************************************************
 * Outline 		: ClearLCD
 * Description 	: Clear display of LCD
 * Argument  	: none
 * Return value  : none
 *""FUNC COMMENT END""*********************************************************/
void ClearLCD( void )
{
  /* Display Clear */
  LCD_write(CTRL_WR,LCD_CLEAR);
  DisplayDelay(25000);
}
/******************************************************************************
End of ClearLCD
******************************************************************************/

/*""FUNC COMMENT""*************************************************************
 * Outline 		: DisplayString
 * Description 	: This function controls LCD writes to line 1 or 2 of the LCD.
 *                You need to use the defines LCD_LINE1 and LCD_LINE2 in order
 *	          to specify the starting position.
 *		  For example, to start at the 2nd position on line 1...
 *	          DisplayString(LCD_LINE1 + 1, "Hello") 
 * Argument  	: (unsigned char)position - Line number of display 
 *		  (const char *) string -   Pointer to data to be written to 
 *                display.
 *	   	  Last character should be null.
 * Return value  : none
 *""FUNC COMMENT END""*********************************************************/
void DisplayString(unsigned char position, char * string)
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
      LCD_write(CTRL_WR, (unsigned char)(LCD_HOME_L1 + position) );
    }
    else
    {
      /* Display on Line 2 */
      LCD_write(CTRL_WR, (unsigned char)(LCD_HOME_L2 + position - LCD_LINE2) );
    }
    /* set position index to known value */
    next_pos = position;		
    DisplayDelay(25000);					
  }
  
  do
  {
    LCD_write(DATA_WR,*string++);
    DisplayDelay(25000);					
    /* increment position index */
    next_pos++;				
  }
  while(*string);
}
/******************************************************************************
End of DisplyString
******************************************************************************/

/*""FUNC COMMENT""*************************************************************
 * Outline		: LCD_write
 * Description	: Writes data to display. Sends command to display.
 * Augments	: (unsigned char) value - the value to write
 *		  (unsigned char) data_or_ctrl - To write value as DATA or 
 *                CONTROL
 *		1 = DATA
 *		0 = CONTROL
 * Returns Value	: None
 *""FUNC COMMENT END""*********************************************************/
void LCD_write(unsigned char data_or_ctrl, unsigned char value)
{
  /* Write upper nibble first */
  LCD_nibble_write(data_or_ctrl, (value & 0xF0) >> 4);
  /* Write lower nibble second */
  LCD_nibble_write(data_or_ctrl, (value & 0x0F));
}
/******************************************************************************
End of LCD_write
******************************************************************************/

/*""FUNC COMMENT""*************************************************************
 * Outline 		: LCD_nibble_write
 * Description 	: Writes data to display. Sends command to display. 
 * Argument  	: (unsigned char) value - the value to write
 *		  (char) data_or_ctrl - To write value  
 *		   as DATA or CONTROL
 *		1 = DATA
 *		0 = CONTROL
 * Return value  : none
*""FUNC COMMENT END""*********************************************************/
void LCD_nibble_write(unsigned char data_or_ctrl, unsigned char value)
{
  unsigned char ucStore;
  
  /* Set Register Select pin high for Data */
  if (data_or_ctrl == DATA_WR)
  {
    RS_PIN = SET_BIT_HIGH;
  }
  else
  {
    RS_PIN = SET_BIT_LOW;
  }
  /* There must be 40ns between RS write and EN write */
  DisplayDelay(10);					
  /* EN enable chip (HIGH) */
  EN_PIN = SET_BIT_HIGH;
  /* Tiny delay */  			
  DisplayDelay(10);
  /* Clear port bits used */	
  ucStore = DATA_PORT;
  ucStore &= ~DATA_PORT_MASK;
  /* OR in data */	
  ucStore |= ((value << DATA_PORT_SHIFT) & DATA_PORT_MASK );
  /* Write data to port */	
  DATA_PORT = ucStore;
  /* write delay while En High */	            
  DisplayDelay(60);
  /* Latch data by dropping EN */					
  EN_PIN = SET_BIT_LOW;
  /* Data hold delay */				
  DisplayDelay(1000);					
  if (data_or_ctrl == CTRL_WR)
  {
    /* Extra delay needed for control writes */
    DisplayDelay(25000);
  }				
}
/******************************************************************************
End of LCD_nibble_write
******************************************************************************/

/*""FUNC COMMENT""*************************************************************
 * Outline 	: DisplyDelay
 * Description 	: Software Delay
 * Argument  	: Delay counter
 *		  Decrease Delay counetr until 0
 * Return value  : none
 *""FUNC COMMENT END""*********************************************************/
void DisplayDelay(unsigned long int units)
{
  unsigned long counter = units * DELAY_TIMING;
  while(counter--)
  {
    nop();
  }
}
/******************************************************************************
End of DisplyDelay
******************************************************************************/

/*""FUNC COMMENT""*************************************************************
 * Outline	: Convert_16BitNumber_ToString
 * Description 	: The function takes an unsigned short variable, and converts it
 *                into a 8 character string, allowing it to be used with the LCD
 * Argument  	: num - any unsigned short integer
 * Return value  : none
 *""FUNC COMMENT END""*********************************************************/
void Convert_16BitNumber_ToString(unsigned int c, char* buffer)
{
  char a;
  
  buffer[0] = '=';
  buffer[1] = 'H';
  buffer[2] = '\'';
  a = (char)((c & 0x0F00)>> 8);
  buffer[3] = (a < 0x0A) ? (a+0x30):(a+0x37);
  a = (char)((c & 0x00F0)>> 4);
  buffer[4] = (a < 0x0A) ? (a+0x30):(a+0x37);
  a = (char)(c & 0x000F);
  buffer[5] = (a < 0x0A) ? (a+0x30):(a+0x37);
  
  /*	Add Spaces and a string terminating character.	*/
  buffer[6] = ' ';
  buffer[7] = ' ';
  buffer[8] = '\0';
}
/******************************************************************************
End of Convert_16BitNumber_ToString
******************************************************************************/
