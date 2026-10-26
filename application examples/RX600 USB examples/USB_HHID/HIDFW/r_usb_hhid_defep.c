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
*******************************************************************************
* Copyright (C) 2010(2011) Renesas Electronics Corpration
* and Renesas Solutions Corp. All rights reserved.
*******************************************************************************
* File Name    : r_usb_hhid_defep.c
* Version      : 1.02
* Device(s)    : Renesas
* Tool-Chain   : Renesas
* OS           : Common to None and uITRON
* H/W Platform : Independent
* Description  : Host HID class data table
******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 30.07.2010 0.90    First Release
*         : 30.11.2010 0.95    USBSTDFW(Mass Production Release)
*         : 10.02.2011 1.00    Host HID Class(Mass Production Release)
*         : 17.06.2011 1.02    Support USB Basic Fw V110
******************************************************************************


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_usb_ctypedef.h"         /* Type define */
#include "r_usb_cdefusbip.h"        /* USB-FW Library Header */
#include "r_usb_hhid_define.h"      /* HID class define */
#include "r_usb_hhid_extern.h"

/******************************************************************************
Section    <Section Difinition> , "Project Sections"
******************************************************************************/
#pragma section="_hid"


/******************************************************************************
Private global variables and functions
******************************************************************************/
/* Host global area definition */
/* Host HID Pipe Information Table (or "endpoint table"). */
uint16_t usb_ghhid_SmplEpTbl[] = {
#if USB_ANSIIO_PP == USB_ANSIIO_USE_PP
    USB_NONE,
#else   /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */
    USB_HHID_USE_PIPE,
#endif  /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */
    /* TYPE  / BFRE        / DBLB     / CNTMD    / SHTNAK        / DIR      / EPNUM */
    USB_NONE | USB_BFREOFF | USB_NONE | USB_NONE | USB_SHTNAKOFF | USB_NONE | USB_NONE,
    USB_NONE,
    USB_NONE,
    USB_NONE,
    USB_CUSE,
    /* Pipe end */
    USB_PDTBLEND,
};

/* Host HID TPL */
const uint16_t usb_gapl_devicetpl[] =
{
    1,              /* Number of list */
    0,              /* Reserved */
    USB_NOVENDOR,   /* Vendor ID  : no-check */
    USB_NOPRODUCT,  /* Product ID : no-check */
};

/******************************************************************************
End  Of File
******************************************************************************/
