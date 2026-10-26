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
* File Name     : usb_hid.h
* Version       : 1.00
* Device        : R5F563NB
* Tool-Chain    : Renesas RX Standard 1.2.0.0
* H/W Platform  : RSK+RX63N
* Description   : Human Interface Device (HID) USB Class.
        Supports an IN and an OUT HID Report.

        NOTE: This module does not have any knowledge of the
        contents of the HID reports.    
******************************************************************************/
/******************************************************************************
* History       : 13 Aug. 2012  Ver. 1.00 First Release
******************************************************************************/


#ifndef USB_HID_H
#define USB_HID_H

/******************************************************************************
* User Includes
******************************************************************************/
#include "usb_common.h"
#include "usbdescriptors.h"

/******************************************************************************
* Type Definitions
******************************************************************************/
typedef void(*CB_REPORT_OUT)(uint8_t(*)[OUTPUT_REPORT_SIZE]);

/******************************************************************************
* Function Prototypes
******************************************************************************/
USB_ERR USBHID_Init(uint8_t (*_ReportIN)[INPUT_REPORT_SIZE], CB_REPORT_OUT _cb);
          
USB_ERR USBHID_ReportIN(uint8_t (*_ReportIN)[INPUT_REPORT_SIZE]);

/*USB_HID_H*/
#endif 
