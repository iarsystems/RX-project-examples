/* Adapted for use with IAR Embedded Workbench */
/*******************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only
 * intended for use with Renesas products. No other uses are authorized.
 * This software is owned by Renesas Electronics Corporation and is  protected
 * under all applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES
 * REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY,
 * INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR  A
 * PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE  EXPRESSLY
 * DISCLAIMED.
 * TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
 * ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE  LIABLE
 * FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES
 * FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS
 * AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this
 * software and to discontinue the availability of this software.
 * By using this software, you agree to the additional terms and
 * conditions found by accessing the following link:
 * http://www.renesas.com/disclaimer
 ******************************************************************************/
 /* Copyright (C) 2010 Renesas Electronics Corporation. All rights reserved.*/

/*””FILE COMMENT””******************************* Technical reference data ****
* File Name    : main.c 
* Version     : 1.00
* Device     : R5F56107VNFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX610
* Description   : Starts the USB HID Application.
*          See file usb_hid_app.c for details.
*          NOTE: This sample requires the Ethernet and USB Application board.
*            See the USB Sample Code User Manual for details.  
******************************************************************************/

/******************************************************************************
*   History     : 26.05.2010 Ver 1.0  
*""FILE COMMENT END""**********************************************************/

/***********************************************************************************
User Includes
***********************************************************************************/

/* The following header file provides a structure to access on-chip I/O registers. */
#include "iodefine.h"
/* The following header file defines functions used for the LCD */
#include "lcd.h"

/* Following header file provides prototype for functions contained in this  file. */
#include "main.h"

/* HID application header*/
#include "usb_hid_app.h"

/*UART header*/
#include "async.h"

/* uIP main from uIP demo part */
int uip_main(void);

/***********************************************************************************
User Program Code Prototypes
***********************************************************************************/

/**********************************************************************************
System Definitions & Global Variables
***********************************************************************************/

/***********************************************************************************
Function Name : main
Description    : This starts the HID Application, see file usb_HID_app.c for details.
        
Parameters    : none
Return value  : none
***********************************************************************************/
void main(void)
{
  /*Start APP*/
  /* Moved this to dhcpc.c, because we append the IP Address to  */
  /* the HID Product Data descriptor, and thus only after we got */
  /* an IP Address can we start USB.                             */
  //USB_HID_APP_Main();
  
  uip_main();

  /* Defines an infinite while loop */
  while(1)
  {
  }
  
}
/***********************************************************************************
End main function
***********************************************************************************/

/**********************************************************************************
Function Name:   abort
Description:  Required by standard lib. when using assert.
Parameters:   none
Return value:   none
***********************************************************************************/
void abort(void)
{
//  DEBUG_MSG_LOW(("\r\n *** ABORT *** \r\n"));

  while(1);
}
/**********************************************************************************
End of function abort
***********************************************************************************/ 















