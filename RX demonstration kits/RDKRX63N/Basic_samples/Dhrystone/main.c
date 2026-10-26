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

/*
 * main.c
 *
 *  Created on: 31 Jan 2012
 *      Author: RJW (base code). Modified for Dhrystone by IAR.
 *              Reneses Electronics Europe Ltd
 */

/******************************************************************************
*
*                             PROJECT OVERVIEW
*
* The project configures the RX63N to run from the PLL.
* MTU3 is configured as timer for Dhrystone. Result is printed on the LCD.
*
******************************************************************************/


/******************************************************************************
    System Includes                                                           
******************************************************************************/
#include <iorx63n.h>
#include <string.h>


/******************************************************************************
    User Includes                                                             
******************************************************************************/
#include "yrdkrx63n.h"
#include "yrdkrx62n_rspi_api.h"
#include "main.h"
#include "mtu.h"                 
#include "lcd.h"
#include "intrinsics.h"

/******************************************************************************
 Function Prototypes
******************************************************************************/
extern void HardwareSetup(void);

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/
extern char g_szDhrystone_msg[128];
extern void run_dry (char *msg, int n);

/******************************************************************************
   Function     : main
   Description  : Initialises MTU3 - 50ms interrupt
                  Runs and present a Dhrystone result on the LCD. 
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
  
  /* Initialise MTU3.  This timer will toggle LED15 at 50ms rate   *
   * We can use this to check that board is running and            *
   * check we have the clocks setup correctly                      */
  InitMTU3();
  
  /* Start the timer MTU3 */
  StartMTU3();

  /* Clear LCD */
  lcd_clear();

  /* Display heading */
  lcd_display(LCD_LINE1, "Dhrystone   ");
  lcd_display(LCD_LINE2, " result in  ");
  lcd_display(LCD_LINE3, " DMIPS/Mhz: ");

  /* Run Dhrystone */
  run_dry(g_szDhrystone_msg, sizeof(g_szDhrystone_msg));

  /* Display the result */
  lcd_display(LCD_LINE5, (const uint8_t *)g_szDhrystone_msg);

  while(1) 
  {
  }
}
