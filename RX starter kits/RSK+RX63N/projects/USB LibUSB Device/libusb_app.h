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
* File Name     : libusb_app.c
* Version       : 1.00
* Device        : R5F563NB
* Tool Chain    : Renesas RX Standard 1.2.0.0
* H/W Platform  : RSK+RX63N
* Description : This LibUSB application will allow a host using LibUSB to
        connect to this device.

        Please note that LibUSB must be installed on your host PC before using this. 
        For more details see 'Renesas Starter Kit USB Sample Code User's Manual'.

        The host will be able to send data, via BULK OUT,
        to the device to make it:
          1. Toggle a LED.
          2. Read the ADC. (Device responds with BULK IN)
          3. Write to the LCD.
        
        The Device will tell the host, via INTERRUPT IN, when a user
        has pressed a switch.
        See below "FORMAT OF DATA TRANSFERED BETWEEN HOST AND DEVICE".
********************************************************************************/
/********************************************************************************
* History       : 23.01.2012 Ver. 1.00 First Release
********************************************************************************/

#ifndef LIBUSB_APP_H
#define LIBUSB_APP_H 

/********************************************************************************
User Includes
********************************************************************************/

#include "usb_common.h"

/********************************************************************************
Function Prototypes
********************************************************************************/
USB_ERR LibUSB_APP_Main(void);

/* End of multiple inclusion prevention macro */
#endif
