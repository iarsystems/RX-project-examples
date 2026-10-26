/* Adapted for use with IAR Embedded Workbench */


/******************************************************************************
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
******************************************************************************/

/******************************************************************************
*
*                             PROJECT OVERVIEW
*
* This project demonstrates the 2 modes of the WDT. 
* The 2 modes are AutoStart mode and RegisterStart mode.
* Either mode can be selected at build time via the setting in wdt.h
* To enable AutoStart mode, set:
*   #define     AUTO_START_WDT  1
*
* To enable RegisterStart mode, set:
*   #define     AUTO_START_WDT  0
*
* In both modes the WDT is configured to generate a NMI on undeflow, not a 
* RESET.
* The WDT timer value can be reset at any time by pressing SW1
* If the WDT does undeflow then NMI will chnage state.
*
* If using AutoStart Mode the Option Function Select Register 0 value is 
* created by the macro values in wdt.c
* NOTE: In this project we only set values for the WDT, not the IWDT
*
******************************************************************************/


/*
 * main.c
 *
 *  Created on: 01 Oct 2011
 *      Author: RJW
 *              Reneses Electronics Europe Ltd
 */


/******************************************************************************
System Includes                                                           
******************************************************************************/
#include <iorx63n.h>


/******************************************************************************
    User Includes                                                             
******************************************************************************/
#include "yrdkrx63n.h"
#include "yrdkrx62n_rspi_api.h"
#include "main.h"
#include "wdt.h"
#include "lcd.h"
#include "intrinsics.h"

      
/******************************************************************************
 Function Prototypes
******************************************************************************/
extern void HardwareSetup(void);


/******************************************************************************
   Function     : main
   Description   : Initialises the WDT is AutoStart mode isn't selected.
                  Clears the WDT if SW1 is pressed      
   Argument    : none
   Return value : none
******************************************************************************/
void main(void)
{
  HardwareSetup();
  __enable_interrupt();
  
  /* Initialize LCD */
  YRDKRX62N_RSPI_Init();
  lcd_initialize();
  
  /* Clear LCD */
  lcd_clear();
  
  /* Display message on LCD */
  lcd_display(LCD_LINE1, "WDT         ");  
#if (AUTO_START_WDT == 1)
  lcd_display(LCD_LINE2, "Autostart:  ");
  lcd_display(LCD_LINE4, "LED15 toggle");
  lcd_display(LCD_LINE5, "when the WDT");
  lcd_display(LCD_LINE6, "underflows. ");
#else
  lcd_display(LCD_LINE2, "Register-   ");  
  lcd_display(LCD_LINE3, "start:      ");
  lcd_display(LCD_LINE5, "Press SW1 to");  
  lcd_display(LCD_LINE6, "pet the WDT.");  
#endif
  
  ICU.NMIER.BIT.WDTEN = 1;
  
#if (AUTO_START_WDT != 1)
  InitWDT();
  StartWDT();
#endif
  
  while(1)
  {
    if(SW1 == SW_ACTIVE)
    {
      WDT.WDTRR = 0x00;
      WDT.WDTRR = 0xff;
    }
  }
}

/******************************************************************************
   Function     : NonMaskableInterrupt
   Description   : Called if the WDT undeflowes
                  Toggles LED15    
   Argument    : none
   Return value : none
******************************************************************************/
__interrupt void __NMI_handler()
{        
  LED15 = 1 - LED15;
  
  ICU.NMICLR.BIT.WDTCLR = 1;
}
