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
* File Name    : main.c 
* Version      : 1.00
* Device       : R5F562N8
* Tool Chain   : IAR Embedded Workbench
* H/W Platform : YRDKRX62N
* Description  : Starts the USB CDC Application.
*                Demonstrates CDC usage by providing an echo test mode,
*                where any characters read will be echoed back.
*
* Operation    : 1. Build and download the tutorial project to the YRDK.
*                2. Start the application and the device will appear as a 
*                   virtual COM port to the host.
*                3. Connect to this virtual COM port using a terminal program 
*                   such as MS HyperTerminal. Instructions will appear.
*                4. A menu is provided that can be controlled by pressing 
*                   the YRDK switches.
*******************************************************************************/
/*******************************************************************************
*   History     : 26.05.2010 Ver 1.0  
*******************************************************************************/

/*******************************************************************************
User Includes
*******************************************************************************/
/* Provides a structure to access on-chip I/O registers. */
#include <iorx62n.h>
/* The following header file defines functions used for the LCD */
#include "lcd.h"
/* Contains delcarations for the functions defined in this file */
#include "main.h"
/* CDC application header*/
#include "usb_cdc_app.h"
/*UART header*/
#include "async.h"

#include "machine.h"
#include "intrinsics.h"
#include "hwsetup.h"

/*******************************************************************************
User Program Code Prototypes
*******************************************************************************/

/*******************************************************************************
System Definitions & Global Variables
*******************************************************************************/

/*******************************************************************************
* Function Name : main
* Description  : This starts the CDC Application, see file usb_CDC_app.c 
*           for details.      
* Parameters  : none
* Return value  : none
*******************************************************************************/
void main(void)
{
  HardwareSetup();

  /*Start APP*/
  USB_CDC_APP_Main();

  /* Defines an infinite while loop */
  while(1)
  {
  }

}
/*******************************************************************************
End main function
*******************************************************************************/

/*******************************************************************************
Function Name:   abort
Description:  Required by standard lib. when using assert.
Parameters:   none
Return value:   none
*******************************************************************************/
void abort(void)
{
  DEBUG_MSG_LOW(("\r\n *** ABORT *** \r\n"));

  while(1);
}
/*******************************************************************************
End of function abort
*******************************************************************************/
