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
/* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.   */
/*******************************************************************************
* File Name   : usb_hid_app.c
* Version     : 1.00
* Device     : R5F5630EDDFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform   : RSKRX630
* Description   : A USB HID Application.
*
*      This USB HID application allows a host to perform the following 
*           operations by sending an OUTPUT report:-
*          1. Toggle a LED.
*          2. Read the ADC.
*          3. Write to the LCD.
*      The Device will tell the host, via an INPUT report, when a user
*      has pressed a switch.
*      See below "FORMAT OF DATA TRANSFERED BETWEEN HOST AND DEVICE".
*      
*      For more details see 'USB Sample Code User's Manual'.
*      
***********************************************************************************/
/***********************************************************************************
* History     : 23.01.2012 Ver. 1.00 First Release
***********************************************************************************/

/***********************************************************************************
System Includes
***********************************************************************************/
#include <stdio.h>
#include <string.h>
#include <assert.h>

/* Provides a structure to access on-chip I/O registers. */
#include "iorx630_usb.h"

/* Common defines for widely used items. */
#include "intrinsics.h"


/***********************************************************************************
User Includes
***********************************************************************************/
/* The following header file defines common functions used for the rskrx630def */
#include "rskrx630def.h"
/* The following header file defines functions used for the ADC */
#include "adc.h"
/* The following header file defines functions used in file hwsetup.c */
#include "hwsetup.h"
/* The following header file defines functions used in file switch.c */
#include "switch.h"
/* The following header file defines functions used for the LCD */
#include "lcd.h"
/* The following header file defines functions used in file usb_hid.c */
#include "usb_hid.h"
/* The following header file defines functions used in file usb_hid_app */
#include "usb_hid_app.h"

/***********************************************************************************
Defines
***********************************************************************************/

/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
/* FORMAT OF DATA TRANSFERED BETWEEN HOST AND DEVICE*/
/*
OUT
The host sends (OUT transfer) 17 bytes.
The first byte is a cmd byte the following 16 are the 16
ASCII LCD characters values.
The bits in the cmd byte are defined below:
*/
#define OUT_BIT_LED 0x01 /*Toggle LED*/
#define OUT_BIT_ADC 0x02 /*Read ADC*/
#define OUT_BIT_LCD 0x04 /*Update LCD*/
/*
IN
The device sends (IN transfer) 5 bytes.
The first byte contains information as defined by the bits below.
The remaining 4 bytes (may) hold a 32 bit ADC value in little endian. 
*/
#define IN_BIT_LED  0x01 /*LED state (1 = ON, 0 = OFF)*/
#define IN_BIT_ADC  0x02 /*ADC data is valid*/
#define IN_BIT_SW  0x04 /*A switch has been pressed.*/
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

/***********************************************************************************
Variables
***********************************************************************************/
/*In Report buffer*/
static uint8_t g_INReport[INPUT_REPORT_SIZE];


/***********************************************************************************
Private Function Prototypes
***********************************************************************************/
static void CBReportOut(uint8_t(*_ReportOut)[OUTPUT_REPORT_SIZE]);
static void CreateINReport(bool _bADC, bool _bSWPressed, uint32_t _ADCValue);
static void SendINReport(bool _bADC, bool _bSWPressed, uint32_t _ADCValue);

void SwitchHandler(void);

/**********************************************************************************
* Outline     : USB_HID_APP_Main
* Description   : Start the HID USB sample application.
*          This function does not return.
* Argument    : none
* Return value  : none
**********************************************************************************/
void USB_HID_APP_Main(void)
{
  
  /* LCD */
  Init_LCD();
  
  /* Display splash screen on the debug LCD */
  Display_LCD(LCD_LINE1,"Renesas ");
  Display_LCD(LCD_LINE2,"USB HID ");
  
  /* Display debug messages */
  DEBUG_MSG_LOW( ("\r\n*** HID App Starting ***\r\n"));
  
  /* Initialise the 12-bit ADC module */
  Init_ADC();
  
  /*Initialise USB IN Report */
  CreateINReport(false,false,0);
  
  /*Initialise the USB HID Class*/
  USBHID_Init((uint8_t(*)[INPUT_REPORT_SIZE])g_INReport, CBReportOut);
  
  /*Register a callback for when an RSK switch is pressed*/
  SetSwitchReleaseCallback(SwitchHandler);
  
  __enable_interrupt();
  
  /*The rest is interrupt driven.*/
  while(1);
}
/***********************************************************************************
* End of function USB_HID_APP_Main
***********************************************************************************/

/***********************************************************************************
* Outline     : CBReportOut
* Description   : Host has sent an OUT report.
*          Decipher it.
* Argument    : _ReportOut: Output report
* Return value  : none
***********************************************************************************/
static void CBReportOut(uint8_t(*_ReportOut)[OUTPUT_REPORT_SIZE])
{
  uint16_t ADCValue;
  volatile bool bRPDLReturn;
  
  /* Toggle LED? */
  if(0 != ((*_ReportOut)[0] & OUT_BIT_LED))
  {
    LED1 = ~LED1;
  }
  
  /*Read ADC?*/
  if(0 != ((*_ReportOut)[0] & OUT_BIT_ADC))
  {
    /* Start AD conversion */
    Start_ADC();

    /* Wait for the conversion to end */
    while(S12AD.ADCSR.BIT.ADST == 1);

    /* Fetch the ADC value */
    ADCValue = Read_ADC();
    
    DEBUG_MSG_LOW( ("HIDApp: ADC Value = h'%04x\r\n", ADCValue));
    
    /*Send response*/
    SendINReport(true, false, (uint32_t)ADCValue);
  }
  
  /*Update LCD*/
  if(0 != ((*_ReportOut[0]) & OUT_BIT_LCD))
  {
    char szLine1[NUMB_CHARS_PER_LINE+1];
    char szLine2[NUMB_CHARS_PER_LINE+1];
    szLine1[NUMB_CHARS_PER_LINE] = '\0';
    szLine2[NUMB_CHARS_PER_LINE] = '\0';
    memcpy(szLine1, &(*_ReportOut)[1], NUMB_CHARS_PER_LINE);
    memcpy(szLine2, &(*_ReportOut)[1+NUMB_CHARS_PER_LINE],
           NUMB_CHARS_PER_LINE);
    
    Display_LCD(LCD_LINE1, (uint8_t*)szLine1);
    Display_LCD(LCD_LINE2, (uint8_t*)szLine2);
  }
}
/***********************************************************************************
* End of function CBReportOut
***********************************************************************************/

/***********************************************************************************
* Outline     : CreateINReport
* Description   : Create the IN report based on supplied params.
* Argument    : _bADC:    true if providing new ADC value.
*          _bSWPressed:true if switch pressed.
*          _ADCValue:  ADC Value.
* Return value  : none
***********************************************************************************/
static void CreateINReport(bool _bADC, bool _bSWPressed, uint32_t _ADCValue)
{
  uint8_t value;
  
  /*Clear to start*/
  g_INReport[0] = 0;
  
  /*Set LED Bit in report after reading IO port to see if LED is On or Off*/
  value = LED1;
  
  if(LED_ON == value)
  {
    g_INReport[0] |= IN_BIT_LED;
  }

  /*Set new ADC value bit?*/
  if(0 != _bADC)
  {
    g_INReport[0] |= IN_BIT_ADC;
  }
  
  /*Set switch toggled  bit?*/
  if(0 != _bSWPressed)
  {
    g_INReport[0] |= IN_BIT_SW;
  }
  
  /*Set ADC Value (even if not setting new ADC value bit)*/
  /*Little Endian*/
  g_INReport[1] = (uint8_t)(_ADCValue & 0xFF);
  g_INReport[2] = (uint8_t)((_ADCValue >> 8) & 0xFF);
  g_INReport[3] = (uint8_t)((_ADCValue >> 12) & 0xFF);
  g_INReport[4] = (uint8_t)((_ADCValue >> 16) & 0xFF);
}
/***********************************************************************************
*  End of function CreateINReport
***********************************************************************************/

/***********************************************************************************
* Outline     : SendINReport
* Description   : Create and then send IN report
* Argument    : none
* Return value  : none
***********************************************************************************/
static void SendINReport(bool _bADC, bool _bSWPressed, uint32_t _ADCValue)
{
  /*Create report*/
  CreateINReport(_bADC, _bSWPressed, _ADCValue);
  
  /*Send to host*/
  USBHID_ReportIN((uint8_t(*)[INPUT_REPORT_SIZE])g_INReport);
}
/***********************************************************************************
* End of function SendINReport
***********************************************************************************/

/***********************************************************************************
* Outline     : SwitchHandler  
* Description   : Called when a RSK switch is pressed.
* Argument    : none
* Return value  : none
***********************************************************************************/
void SwitchHandler(void)
{
  /*Send IN report to host */
  SendINReport(false, true, 0);
} 
/***********************************************************************************
* End of function SwitchHandler
***********************************************************************************/
