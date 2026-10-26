/******************************************************************************
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
******************************************************************************
* Copyright (C) 2010(2011,2012) Renesas Electronics Corporation
* and Renesas Solutions Corp. All rights reserved.
******************************************************************************
* File Name    : r_usb_class_usrcfg.h
* Version      : 2.00
* Device(s)    : Renesas SH-Series, RX-Series
* Tool-Chain   : Renesas SuperH RISC engine Standard Toolchain
*              : Renesas RX Standard Toolchain
* OS           : Common to None and uITRON 4.0 Spec
* H/W Platform : Independent
* Description  : USB Peripheral HID User definition
******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 06.04.2012 0.50    First Release
******************************************************************************/



#ifndef __R_USB_PHID_USRCFG_H__
#define __R_USB_PHID_USRCFG_H__

/*****************************************************************************
Macro definitions (USER DEFINE)
******************************************************************************/
#define USB_PHID_DEMO_MODE          0           /* Peripheral HID Application demo mode */
#define USB_PHID_KEYBOARD_MODE      1           /* Peripheral HID Application keyboard mode */
#define USB_PHID_MOUSE_MODE         2           /* Peripheral HID Application mouse mode */


/* Select HID Demo sample Application mode  */
#define USB_PHID_APL_MODE       USB_PHID_DEMO_MODE      /* Peripheral HID Application demo mode */
//#define USB_PHID_APL_MODE       USB_PHID_KEYBOARD_MODE  /* Peripheral HID Application keyboard mode */
//#define USB_PHID_APL_MODE       USB_PHID_MOUSE_MODE     /* Peripheral HID Application mouse mode */

#endif  /* __R_USB_PHID_USRCFG_H__ */
/******************************************************************************
End  Of File
******************************************************************************/
