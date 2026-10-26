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
* File Name    : lcd.c
* Version     : 1.00
* Device     : R5F562N8
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSK+RX62N
* Description   : LCD Module utility functions.
*          Written for KS0066u compatible LCD Module.
*            (8 characters by 2 lines)
******************************************************************************/
/******************************************************************************
* History     : 03.08.2010 Ver. 1.00 First Release
******************************************************************************/

/******************************************************************************
* System Includes
******************************************************************************/
/* Defines standard variable types used in this function */
#include <stdint.h>

/******************************************************************************
* Project Includes
******************************************************************************/
/* IO Port RPDL function definitions */
#include "r_pdl_io_port.h"
/* CMT RPDL function definitions */
#include "r_pdl_cmt.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* Following header file provides function prototypes for LCD controlling
   functions & macro defines */
#include "lcd.h"

/******************************************************************************
* Local Function Prototypes
******************************************************************************/
/* Declare LCD nibble write function prototype */
static void LCD_nibble_write(uint8_t, uint8_t);

/******************************************************************************
* Outline     : InitialiseLCD
* Description   : Initializes the LCD display. 
* Argument      : none
* Return value  : none
******************************************************************************/
void InitialiseLCD(void)
{
  /* Declare error flag */
  bool err = true;
  
  /* Power Up Delay for the LCD Module */
  err &= R_CMT_CreateOneShot(0, PDL_NO_DATA, 50E-3, PDL_NO_FUNC, 0);

  /* Display initialises in 8 bit mode - so send one write (seen as 8 bit)
  to set to 4 bit mode. */
  LCD_nibble_write(CTRL_WR, 0x03);
  err &= R_CMT_CreateOneShot(0, PDL_NO_DATA, 5E-3, PDL_NO_FUNC, 0);
  LCD_nibble_write(CTRL_WR, 0x03);
  err &= R_CMT_CreateOneShot(0, PDL_NO_DATA, 5E-3, PDL_NO_FUNC, 0);
  LCD_nibble_write(CTRL_WR, 0x03);
  err &= R_CMT_CreateOneShot(0, PDL_NO_DATA, 5E-3, PDL_NO_FUNC, 0);
    
  /* Function Set */
  LCD_nibble_write(CTRL_WR, 0x02);
  err &= R_CMT_CreateOneShot(0, PDL_NO_DATA, 39E-6, PDL_NO_FUNC, 0);
  LCD_nibble_write(CTRL_WR, 0x02);
  LCD_nibble_write(CTRL_WR, (LCD_DISPLAY_ON | LCD_TWO_LINE ));
  err &= R_CMT_CreateOneShot(0, PDL_NO_DATA, 39E-6, PDL_NO_FUNC, 0);
 
  /* Display ON/OFF control */
  LCD_write(CTRL_WR, LCD_CURSOR_OFF);
  err &= R_CMT_CreateOneShot(0, PDL_NO_DATA, 39E-6, PDL_NO_FUNC, 0);

  /* Display Clear */
  LCD_write(CTRL_WR, LCD_CLEAR);
  err &= R_CMT_CreateOneShot(0, PDL_NO_DATA, 2E-3, PDL_NO_FUNC, 0);

  /* Entry Mode Set */
  LCD_write(CTRL_WR, 0x06);
  err &= R_CMT_CreateOneShot(0, PDL_NO_DATA, 39E-6, PDL_NO_FUNC, 0);

    /* Home the cursor */
  LCD_write(CTRL_WR, LCD_HOME_L1);
  err &= R_CMT_CreateOneShot(0, PDL_NO_DATA, 5E-3, PDL_NO_FUNC, 0);
  
  /* Halt in while loop when RPDL errors detected */  
  while(!err);
}
/******************************************************************************
* End of function InitialiseDisplay
******************************************************************************/

/******************************************************************************
* Outline     : ClearLCD
* Description   : Clear the LCD.
* Argument    : None.
* Return value  : None.
******************************************************************************/
void ClearLCD(void)
{
  /* Declare error flag */
  bool err = 1;
  
  /* Display Clear */
  LCD_write(CTRL_WR, LCD_CLEAR);
  err &= R_CMT_CreateOneShot(0, PDL_NO_DATA, 2E-3, PDL_NO_FUNC, 0);

  /* Halt in while loop when RPDL errors detected */  
  while(!err);
}
/******************************************************************************
* End of function ClearLCD
******************************************************************************/

/******************************************************************************
* Outline     : DisplayLCD
* Description   : This function controls LCD writes to line 1 or 2 of the LCD.
*                 You need to use the defines LCD_LINE1 and LCD_LINE2 in order
*          to specify the starting position.
*          For example, to start at the 2nd position on line 1...
*               DisplayLCD(LCD_LINE1 + 1, "Hello") 
* Argument    : (unsigned char)position -  Line number of display 
*              (const char *) string -  Pointer to data to be written to
*                      display. Last character should be
*                      null.
* Return value  : none
******************************************************************************/
void DisplayLCD(uint8_t position, const uint8_t * string)
{
  /* Declare next position variable */
  static uint8_t next_pos = 0xFF;

  /* Declare error flag */
  bool err = 1;
  
  /* Set line position if needed. We don't want to if we don't need 
     to because LCD control operations take longer than LCD data
     operations. */
  if( next_pos != position)
  {
    if(position < LCD_LINE2)
    {
      /* Display on Line 1 */
      LCD_write(CTRL_WR, ((uint8_t)(LCD_HOME_L1 + position)));
    }
    else
    {
      /* Display on Line 2 */
      LCD_write(CTRL_WR, ((uint8_t)((LCD_HOME_L2 + position) - LCD_LINE2)));
    }
        err &= R_CMT_CreateOneShot(0, PDL_NO_DATA, 39E-6, PDL_NO_FUNC, 0);
    /* set position index to known value */
    next_pos = position;    
  }

  do
  {
    LCD_write(DATA_WR,*string++);
    err &= R_CMT_CreateOneShot(0, PDL_NO_DATA, 43E-6, PDL_NO_FUNC, 0);
    
    /* increment position index */
    next_pos++;        
  } 
  while(*string);
  
  /* Halt in while loop when RPDL errors detected */  
  while(!err);
}
/******************************************************************************
* End of function DisplayString
******************************************************************************/

/******************************************************************************
* Outline     : LCD_write
* Description   : This function controls LCD writes to line 1 or 2 of the LCD.
*                 You need to use the defines LCD_LINE1 and LCD_LINE2 in order
*          to specify the starting position.
*          For example, to start at the 2nd position on line 1...
*               DisplayString(LCD_LINE1 + 1, "Hello") 
* Argument    : (unsigned char)value - the value to write
*          (char) data_or_ctrl - To write value 
*          as DATA or CONTROL
*            1 = DATA
*            0 = CONTROL
* Return value  : none
******************************************************************************/
void LCD_write(uint8_t data_or_ctrl, uint8_t value)
{
  /* Write upper nibble first */
  LCD_nibble_write(data_or_ctrl, (uint8_t)((value & 0xF0) >> 4));
  
  /* Write lower nibble second */
  LCD_nibble_write(data_or_ctrl, (uint8_t)(value & 0x0F));
}
/******************************************************************************
* End of function LCD_write
******************************************************************************/

/******************************************************************************
* Outline     : LCD_nibble_write
* Description   : Writes data to display. Sends command to display. 
* Argument      : (unsigned char) value - the value to write
*          (char) data_or_ctrl - To write value  
*            as DATA or CONTROL
*            1 = DATA
*            0 = CONTROL
* Return value  : none
******************************************************************************/
static void LCD_nibble_write(uint8_t data_or_ctrl, uint8_t value)
{
  /* Declare error flag */
  bool err = 1;
  
  /* Set Register Select pin high for Data */
  if (data_or_ctrl == DATA_WR)
  {
    R_IO_PORT_Write(RS_PIN, 1);
  }
  else
  {
    R_IO_PORT_Write(RS_PIN, 0);
  }
  
  /* tsu1 delay */
  err &= R_CMT_CreateOneShot(0, PDL_NO_DATA, 60E-9, PDL_NO_FUNC, 0);
  
    /* EN enable chip (HIGH) */
  err &= R_IO_PORT_Write(E_PIN, 1);
  
  /* Output the data */
  err &= R_IO_PORT_Write(PDL_IO_PORT_9, (uint8_t)(value << 4u));
  
  /* tw delay */              
  err &= R_CMT_CreateOneShot(0, PDL_NO_DATA, 450E-9, PDL_NO_FUNC, 0);
  
  /* Latch data by dropping E */          
  err &= R_IO_PORT_Write(E_PIN, 0);
  
  /* th2 delay */        
  err &= R_CMT_CreateOneShot(0, PDL_NO_DATA, 10E-9, PDL_NO_FUNC, 0);;
  
  /* tc delay */        
  err &= R_CMT_CreateOneShot(0, PDL_NO_DATA, 48E-8, PDL_NO_FUNC, 0);
  
  /* Halt in while loop when RPDL errors detected */  
  while(!err);
}
/******************************************************************************
* End of function LCD_nibble_write
******************************************************************************/
