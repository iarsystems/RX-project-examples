/******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Electronics Corp. and is
* only intended for use with Renesas products.  

* No other uses are authorized.

* This software is owned by Renesas Electronics Corp. and is 
* protected under the applicable laws, including copyright laws.

* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY,
* INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH
* WARRANTIES ARE EXPRESSLY DISCLAIMED.

* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER 
* RENESAS ELECTRONICS CORP. NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR AND DIRECT, INDIRECT, SPECIAL, INCIDENTAL
* OR COSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE,
* EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE
* POSSIBILITIES OF SUCH DAMAGES.

* Renesas reserves the right, without notice, to make changes to this
* software and to discontinue availability of this software.
* By using this software, you agree to the additional terms and 
* conditions found by accessing the following link:
* http://www.renesas.com/disclaimer
*******************************************************************************/
/* Copyright (C) 2010. Renesas Electronics Corp., All Rights Reserved         */
/******************************************************************************
* File Name     : TMR_1_shot.c 
* Version       : 1.1
* Device(s)     : R5F562N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.1
* OS            : None
* H/W Platform  : YRDKRX62N
* Description   : Defines timer functions used in this sample
*******************************************************************************
* History : DD.MMM.YYYY     Version     Description
*         : 08.Oct.2010     1.00        First release
*         : 02.Dec.2010     1.10        Second YRDK release
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/

/* General RPDL function definitions */
#include "r_pdl_tmr.h"

#include "r_pdl_definitions.h"

/* Defines switch functions and variables used in this file */
#include "switch.h"
/* Defines LCD functions used in this file */
#include "lcd.h"
/* Provides declarations of functions defined in this file */
#include "tmr_1_shot.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/
bool armed = false;

/* Global switch flag variable, indicates which switches have been pressed */
extern volatile uint8_t gSwitchFlag;
extern uint8_t  lcd_buffer1[];
extern uint8_t  lcd_buffer2[];

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
void CB_Switch(void);
void CB_1_Shot(void);

/*******************************************************************************
* Outline       : Init_TMR_1_Shot
* Description   : ADC initialisation function. This function configures the ADC
*                 unit for repeat operation, then configures a CMT timer for a
*                 periodic interrupt every 100ms.
* Argument      : none
* Return value  : none
*******************************************************************************/
void Init_TMR_1_Shot(void)
{
  /* Declare error flag */
  bool err = true;

  /* Configure switch callback function */
  SetSwitchReleaseCallback(CB_Switch);

  /* Halt in while loop when RPDL errors detected */  
  while (!err);
}
/*******************************************************************************
* End of function Init_TMR_1_Shot
*******************************************************************************/
/*******************************************************************************
* Outline       : CB_Switch
* Description   : Callback called when switch is pressed
* Argument      : none
* Return value  : none
*******************************************************************************/
void CB_Switch(void)
{
  /* Declare error flag */
  bool err = true;

  /* Declare temporary character string */
  uint8_t lcd_buffer3[] = "  Waiting   ";
  uint8_t lcd_buffer4[] = " 5 seconds  ";
  uint8_t lcd_buffer5[] = "    for     ";  
  uint8_t lcd_buffer6[] = "One Shot TMR";
  uint8_t lcd_buffer7[] = "  to pop    ";

  /* Check if switch pressed was SW3 */
  if (gSwitchFlag & SWITCHPRESS_3)
  {
    /* Display the contents of the local string lcd_buffer */
    ClearLCD();
    DisplayLCD(LCD_LINE3, lcd_buffer3);
    DisplayLCD(LCD_LINE4, lcd_buffer4);  
    DisplayLCD(LCD_LINE5, lcd_buffer5);  
    DisplayLCD(LCD_LINE6, lcd_buffer6);  
    DisplayLCD(LCD_LINE7, lcd_buffer7);

    if ( armed == false )
    {
      /* Fire the one shot timer */
      err &=  R_TMR_CreateOneShot
        (
        PDL_TMR_UNIT0,
        PDL_TMR_OUTPUT_OFF,
        5,
        CB_1_Shot,
        4
        );

      armed = true;
    }
  }

  /* Clear switch press flags */
  gSwitchFlag &= 0x0F;

  /* Halt in while loop when RPDL errors detected */  
  while (!err);
}
/*******************************************************************************
* End of function CB_Switch
*******************************************************************************/


/*******************************************************************************
* Outline       : CB_1_Shot
* Description   : CMT interrupt callback function. This callback function is
*                 executed after every period of the CMT timer. The function 
*                 fetches the ADC result, and stores it in the global variable
*                 usADC_Result. The function then converts the ADC result into
*                 a character string, and displays it on the debug LCD.
* Argument      : none
* Return value  : none
*******************************************************************************/
void CB_1_Shot(void)
{
  /* Declare error flag */
  bool err = true;

  /* re-arm the push button */
  armed = false;

  /* Display the contents of the local string lcd_buffer */
  ClearLCD();
  DisplayLCD(LCD_LINE1, lcd_buffer1);
  DisplayLCD(LCD_LINE2, lcd_buffer2);    


  /* Halt in while loop when RPDL errors detected */  
  while (!err);
}
/*******************************************************************************
* End of function CB_1_Shot
*******************************************************************************/



