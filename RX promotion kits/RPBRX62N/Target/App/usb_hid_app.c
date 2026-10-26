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
/* Copyright (C) 2010 Renesas Electronics Corporation. All rights reserved. */
/**********************************************************************
* File Name : usb_hid_app.c
* Version : 1.00
* Device : R5F562N8
* Tool Chain : RX Family C Compiler
* H/W Platform : RSKRX62N
* Description : A USB HID Application.
      This USB HID application allows a host to perform the following operations
            by sending an OUTPUT report:-
          1. Toggle a LED.
          2. Read the ADC.
          3. Write to the LCD.
      The Device will tell the host, via an INPUT report, when a user
      has pressed a switch.
      See below "FORMAT OF DATA TRANSFERED BETWEEN HOST AND DEVICE".
      
      For more details see 'USB Sample Code User's Manual'.
**********************************************************************/
/**********************************************************************
* History : 27.07.2010 Ver. 1.00 First Release
**********************************************************************/

/***********************************************************************************
System Includes
***********************************************************************************/
#include <stdio.h>
#include <string.h>
#include <assert.h>

/* IO Port RPDL function defintions */
#include "r_pdl_io_port.h"
/* ADC RPDL function definitions */
#include "r_pdl_adc_10.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"

/***********************************************************************************
User Includes
***********************************************************************************/
#include "hwsetup.h"
#include "switch.h"
#include "lcd.h"
#include "usb_hid.h"
#include "usb_hid_app.h"
#include "mandel.h"

/***********************************************************************************
Defines
***********************************************************************************/

/***********************************************************************************
Global variables
***********************************************************************************/
extern unsigned char g_ucRunHid;

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
/* Joystick information */
uint8_t jCenter, jRight, jDown, jLeft, jUp, jSW1 = 0x01;
/* Joystick active switch */
bool gJoyActive = false;

/***********************************************************************************
Private Function Prototypes
***********************************************************************************/
static void CBReportOut(uint8_t(*_ReportOut)[OUTPUT_REPORT_SIZE]);
static void CreateINReport(void);
static void SendINReport(void);

/**********************************************************************
* Outline     : USB_HID_APP_Main
* Description   : Start the HID USB sample application.
*          This function does not return.
* Argument    : none
* Return value  : none
**********************************************************************/

void USB_HID_APP_Main(void)
{
  /*Initialise USB IN Report */
  CreateINReport();
  
  /*Initialise the USB HID Class*/
  USBHID_Init((uint8_t(*)[INPUT_REPORT_SIZE])g_INReport, CBReportOut);
}

/***********************************************************************************
End of function USB_HID_APP_Main
***********************************************************************************/

/**********************************************************************
* Outline     : CBReportOut
* Description   : Host has sent an OUT report.
*          Decipher it.
* Argument    : _ReportOut: Output report
* Return value  : none
**********************************************************************/
static void CBReportOut(uint8_t(*_ReportOut)[OUTPUT_REPORT_SIZE])
{
  /* Empty because obsolete code was deleted */
}
/***********************************************************************************
End of function CBReportOut
***********************************************************************************/

/**********************************************************************
* Outline     : CreateINReport
* Description   : Create the IN report based on supplied params.
* Argument    : _bSWPressed:true if switch pressed.
* Return value  : none
**********************************************************************/

static void CreateINReport()
{
  static uint8_t incrementer = 0x00;
  
  /*Clear to start*/
  g_INReport[0] = 2;  /*Must be 2 to indicate mouse report*/
  g_INReport[1] = 0;  /*Button byte, lower 3 bits, M R L */
  g_INReport[2] = 0;  /*X axis*/
  g_INReport[3] = 0;  /*Y axis*/
  g_INReport[4] = 0;  /*Wheel*/

  /* Variable used to increase speed of mouse movement */
  if (gJoyActive == true)
  {
    if (incrementer < 0x70)
      incrementer++;
  }
  else
  {
    incrementer = 0x00;
  }

  if (!jSW1)
  {
    /* Send Left Click */
    g_INReport[1] = 0x01;
  }
  if (!jCenter)
  {
    /* Send Right Click */
    g_INReport[1] = 0x02;
  }
  if (!jRight)
  {
    /* Move mouse right */
    g_INReport[2] = (0x01 + incrementer);
  }
  if (!jDown)
  {
    /* Move mouse down */
    g_INReport[3] = (0x01 + incrementer);
  }
  if (!jLeft)
  {
    /* Move mouse left */
    g_INReport[2] = (0xFE - incrementer);
  }
  if (!jUp)
  {
    /* Move mouse up */
    g_INReport[3] = (0xFE - incrementer);
  }
}

/***********************************************************************************
End of function CreateINReport
***********************************************************************************/

/******************************************************************************
* Outline     : ReadJoystick
* Description   : 
* Argument      : none
* Return value  : none

******************************************************************************/
void gReadJoystick()
{
  static int iLastCenter = 1;
  
  R_IO_PORT_Read(PDL_IO_PORT_D_0,&jCenter);  /* Center */
  R_IO_PORT_Read(PDL_IO_PORT_D_1,&jRight);  /* Right */
  R_IO_PORT_Read(PDL_IO_PORT_D_2,&jLeft);    /* Left */
  R_IO_PORT_Read(PDL_IO_PORT_D_3,&jDown);    /* Down */
  R_IO_PORT_Read(PDL_IO_PORT_D_4,&jUp);    /* Up */
  
  /* Switches are 0 when pressed and 1 otherwise */
  if (!jCenter || !jRight || !jDown || !jLeft || !jUp || !jSW1)
  {
    /* Set global flag to true */
    gJoyActive = true;
    /* Send IN report to host */
    SendINReport();
    /* Tell Mandelbrot demo the updated joystick position */
    updateMandelJoystickPosition(1, jRight, jDown, jLeft, jUp, jSW1);
    /* */
    iLastCenter = jCenter;
  }
  else
  {
    if (gJoyActive == true)
    {
      /* If the joystick or switch has just been released
       * Send one IN report to host to 'release' any clicks
       */
      gJoyActive = false;
      SendINReport();
      /* If Center was pressed, tell Mandelbrot */
      if (iLastCenter == 0)
      {
           /* Tell Mandelbrot demo the updated joystick position */
           updateMandelJoystickPosition(iLastCenter, jRight, jDown, jLeft, jUp, jSW1);
           /* */
           iLastCenter = jCenter;
      }
    }
  }
  
}
/******************************************************************************
* End of ReadJoystick
******************************************************************************/

/**********************************************************************
* Outline     : SendINReport
* Description   : Create and then send IN report
* Argument    : none
* Return value  : none
**********************************************************************/

static void SendINReport()
{
  if (g_ucRunHid)
  {
    /*Create report*/
    CreateINReport();
  
    /*Send to host*/
    USBHID_ReportIN((uint8_t(*)[INPUT_REPORT_SIZE])g_INReport);
  }
}

/***********************************************************************************
End of function SendINReport
***********************************************************************************/
