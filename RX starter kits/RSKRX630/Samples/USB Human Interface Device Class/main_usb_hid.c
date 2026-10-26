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
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.    */ 
/*******************************************************************************
* File Name    : main_usb_hid.c 
* Version     : 1.00
* Device     : R5F5630EDDFP
* Tool-Chain   : RX Family C Compiler, v1.01
* H/W Platform  : RSKRX630
* Description   : Complete Initialisation and then start the USB Application.
*                 Refer to usb_hid_app.c for further information.
*******************************************************************************/
/*******************************************************************************
* History     : 23.01.2012 Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
User Includes
*******************************************************************************/
/* Provides a structure to access on-chip I/O registers. */
#include "iorx630_usb.h"
/* Common defines for widely used items. */
#include "rskrx630def.h"
/* The following header file defines functions used for the LCD */
#include "lcd.h"
/* Defines switch/ADC functions */
#include "switch.h"
/* Defines UART functions and variables */
#include "async.h"
/*USB Application*/
#include "usb_hid_app.h"

#include "hwsetup.h"


/*******************************************************************************
* Outline     : main
* Description   : Starts the USB Application.
* Argument    : none
* Return value  : none
*******************************************************************************/
void main(void)
{
  
  HardwareSetup();

  /*Turn LEDS off*/
  LED1 = LED_OFF;
  LED2 = LED_OFF;
  LED3 = LED_OFF;  
    
  /* Initialise UART0 */
  Init_UART();  
  
  /* Start the USB Application */
  USB_HID_APP_Main();
   
  
  while(1)
  {
  }
}
/*******************************************************************************
* End of function main
*******************************************************************************/
