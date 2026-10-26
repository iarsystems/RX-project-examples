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
* File Name     : usb_cdc_app.h
* Version       : 1.00
* Device        : R5F563NB
* Tool-Chain    : Renesas RX Standard 1.2.0.0
* H/W Platform  : RSK+RX63N
* Description   : Application that uses the USB CDC class.
*             
*                This means the device will appear as a virtual COM port to the
*               host. When the host connects to this virtual COM port using a
*               terminal program such as MS HyperTerminal instructions will 
*               appear. A menu is provided that can be controlled by pressing
*               the RSK switches.
*        
*                Demonstrates CDC usage by providing an echo test mode,
*                where any characters read will be echoed back.
*        
*                For more details see 'USB Sample Code User's Manual'.
*        
*                See file async.c for baud rate for serial debug output.
********************************************************************************/
/********************************************************************************
* History       : 13 Aug. 2012  Ver. 1.00 First Release
********************************************************************************/

#ifndef USB_CDC_APP_H
#define USB_CDC_APP_H

/********************************************************************************
User Includes
********************************************************************************/

#include "usb_common.h"

/********************************************************************************
Function Prototypes
********************************************************************************/
void USB_CDC_APP_Main(void);

/* End of multiple inclusion prevention macro */
#endif
