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
* File Name    : main.c 
* Version     : 1.00
* Device     : R5F5630EDDFP
* Tool-Chain   : RX Family C Compiler, v1.01
* H/W Platform  : RSKRX630
* Description   : Starts the USB CDC Application.
*          See file usb_cdc_app.c for details.
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
/* Defines UART functions and variables */
#include "async.h"

#include "hwsetup.h"

/* CDC application header*/
#include "usb_cdc_app.h"


/*******************************************************************************
* Outline     : main
* Description  : This starts the CDC Application, see file usb_CDC_app.c 
*           for details.      
* Argument    : none
* Return value  : none
*******************************************************************************/
void main(void)
{   
 
  HardwareSetup();
  
  /* Initialise UART0 */
  Init_UART();  
  
  USB_CDC_APP_Main();
  
  /* Defines an infinite while loop */
  while(1)
  {
  }
}
/*******************************************************************************
* End of function main
*******************************************************************************/
