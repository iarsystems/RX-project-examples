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
* Copyright (C) 2010(2011) Renesas Electronics Corporation
* and Renesas Solutions Corp. All rights reserved.
*******************************************************************************
* File Name    : r_USB_PHIDdescriptor.c
* Version      : 1.10
* Device(s)    : Renesas
* Tool-Chain   : Renesas
* OS           : Common to None and uITRON
* H/W Platform : Independent
* Description  : USB HID application code
******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 30.07.2010 0.90    First Release
*         : 30.11.2010 0.95    USBSTDFW(Mass Production Release)
*         : 31.03.2011 1.00    Peripheral HID Class(Mass Production Release)
*         : 24.06.2011 1.10    Version 1.10 Release
******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <string.h>
#include "r_usb_ctypedef.h"     /* Type define */
#include "r_usb_usrconfig.h"
#include "r_usb_cdefusbip.h"        /* USB-FW Library Header */
#include "r_usb_cmacprint.h"        /* Standard IO macro */
#include "r_usb_ckernelid.h"        /* Kernel ID definition */
#include "r_usb_cmacsystemcall.h"   /* uITRON system call macro */
#include "r_usb_cextern.h"          /* USB-FW global define */
#include "r_usb_ansi.h"
#include "r_usb_control.h"
#include "r_usb_cusb_bitdefine.h"

#include "r_usb_api.h"

#include "r_usb_phid_define.h"
#include "r_usb_class_usrcfg.h"
#include "r_usb_phid_extern.h"

/******************************************************************************
User define macro definitions
******************************************************************************/
#define USB_VENDORID    0x0000
#if USB_PHID_APL_MODE == USB_PHID_DEMO_MODE
#define USB_PRODUCTID   0x0003
#endif  /* Peripheral HID Application demo mode */

#if USB_PHID_APL_MODE == USB_PHID_KEYBOARD_MODE
#define USB_PRODUCTID   0x0013
#endif  /* Peripheral HID Application keyboard mode */

#if USB_PHID_APL_MODE == USB_PHID_MOUSE_MODE
#define USB_PRODUCTID   0x0023
#endif  /* Peripheral HID Application mouse mode */

/******************************************************************************
Section    <Section Difinition> , "Project Sections"
******************************************************************************/
#pragma section="_hid"


/******************************************************************************
Private global variables and functions
******************************************************************************/
/************************************************************
 *  Pipe Define 1                                           *
 ************************************************************/
/* End point table for Demonstration */
uint16_t usb_gphid_EpTbl[] =
{
#if USB_PHID_APL_MODE == USB_PHID_DEMO_MODE
    USB_PHID_USE_PIPE,
    USB_INT | USB_DIR_P_IN | USB_EP1,
    USB_NONE,
    USB_PHID_MXPS_DEMO,
    USB_NONE,
    USB_CUSE,
    USB_PDTBLEND,
#endif  /* Peripheral HID Application demo mode */

#if USB_PHID_APL_MODE == USB_PHID_KEYBOARD_MODE
    USB_PHID_USE_PIPE,
    USB_INT | USB_DIR_P_IN | USB_EP1,
    USB_NONE,
    USB_PHID_MXPS_KBD,
    USB_NONE,
    USB_CUSE,
    USB_PDTBLEND,
#endif  /* Peripheral HID Application keyboard mode */

#if USB_PHID_APL_MODE == USB_PHID_MOUSE_MODE
    USB_PHID_USE_PIPE,
    USB_INT | USB_DIR_P_IN | USB_EP1,
    USB_NONE,
    USB_PHID_MXPS_MSE,
    USB_NONE,
    USB_CUSE,
    USB_PDTBLEND,
#endif  /* Peripheral HID Application mouse mode */
};

uint16_t *usb_gphid_EpPtr[] =
{
    usb_gphid_EpTbl,
};

/************************************************************
 *  Device Descriptor                                       *
 ************************************************************/
uint8_t usb_gphid_SmplDeviceDescriptor[18] =
{
    /*  0:bLength */
    18,
    /*  1:bDescriptorType */
    USB_DT_DEVICE,
    /*  2:bcdUSB_lo */
    (uint8_t)( USB_BCDNUM & (uint8_t)0xff),
    /*  3:bcdUSB_hi */
    (uint8_t)((uint8_t)(USB_BCDNUM >> 8) & (uint8_t)0xff),
    /*  4:bDeviceClass */
    0x00,
    /*  5:bDeviceSubClass */
    0x00,
    /*  6:bDeviceProtocol */
    0x00,
    /*  7:bMAXPacketSize(for DCP) */
    (uint8_t)USB_DCPMAXP,
    /*  8:idVendor_lo */
    (uint8_t)(USB_VENDORID & (uint8_t)0xff),
    /*  9:idVendor_hi */
    (uint8_t)((uint8_t)(USB_VENDORID >> 8) & (uint8_t)0xff),
    /* 10:idProduct_lo */
    (uint8_t)(USB_PRODUCTID & (uint8_t)0xff),
    /* 11:idProduct_hi */
    (uint8_t)((uint8_t)(USB_PRODUCTID >> 8) & (uint8_t)0xff),
    /* 12:bcdDevice_lo */
    (uint8_t)(USB_RELEASE & (uint8_t)0xff),
    /* 13:bcdDevice_hi */
    (uint8_t)((uint8_t)(USB_RELEASE >> 8) & (uint8_t)0xff),
    /* 14:iManufacturer */
    1,
    /* 15:iProduct */
    2,
    /* 16:iSerialNumber */
    3,
    /* 17:bNumConfigurations */
    USB_CONFIGNUM
};

/************************************************************
 *  Qualifier Descriptor                                    *
 ************************************************************/
uint8_t usb_gphid_SmplQualifierDescriptor[10] =
{
    /*  0:bLength */
    10,
    /*  1:bDescriptorType */
    USB_DT_DEVICE_QUALIFIER,
    /*  2:bcdUSB_lo */
    (uint8_t)(USB_BCDNUM & (uint8_t)0xff),
    /*  3:bcdUSB_hi */
    (uint8_t)((uint8_t)(USB_BCDNUM >> 8) & (uint8_t)0xff),
    /*  4:bDeviceClass */
    0x00,
    /*  5:bDeviceSubClass */
    0x00,
    /*  6:bDeviceProtocol */
    0x00,
    /*  7:bMAXPacketSize(for DCP) */
    (uint8_t)USB_DCPMAXP,
    /*  8:bNumConfigurations */
    USB_CONFIGNUM,
    /*  9:bReserved */
    0
};

/************************************************************
 *  Configuration Descriptor                                *
 ************************************************************/
#if USB_PHID_APL_MODE == USB_PHID_DEMO_MODE
#define USB_SMPLCONFLEN        34
uint8_t usb_gphid_Configuration[USB_SMPLCONFLEN + ( USB_SMPLCONFLEN % 2 )] =
{
    /*  0:bLength */
    9,
    /*  1:bDescriptorType */
    USB_DT_CONFIGURATION,
    /*  2:wTotalLength(L) */
    (uint8_t)(USB_SMPLCONFLEN % 256),
    /*  3:wTotalLength(H) */
    (uint8_t)(USB_SMPLCONFLEN / 256),
    /*  4:bNumInterfaces */
    1,
    /*  5:bConfigurationValue */
    1,
    /*  6:iConfiguration */
    4,
    /*  7:bmAttributes */
    (uint8_t)(USB_CF_RESERVED | USB_CF_RWUPON),
    /*  8:bMaxPower (2mA unit) */
    (uint8_t)(100 / 2),
        /* Interface Descriptor */
        /*  0:bLength */
        9,
        /*  1:bDescriptor */
        USB_DT_INTERFACE,
        /*  2:bInterfaceNumber */
        0,
        /*  3:bAlternateSetting */
        0,
        /*  4:bNumEndpoints */
        1,
        /*  5:bInterfaceClass(HID) */
        USB_IFCLS_HID,
        /*  6:bInterfaceSubClass(NonBOOT) */
        USB_IFSUB_NOBOOT,
        /*  7:bInterfaceProtocol(No Protocol) */
        USB_IFPRO_DEM,
        /*  8:iInterface */
        0,
        /* HID Descriptor */
        /*  0:bLength */
        9,
        /*  1:bDescriptor */
        USB_DT_TYPE_HIDDESCRIPTOR,
        /*  2:HID Ver */
        0x00,
        /*  3:HID Ver */
        0x01,
        /*  4:bCountryCode */
        0x00,
        /*  5:bNumDescriptors */
        0x01,
        /*  6:bDescriptorType */
        0x22,
        /*  7:wItemLength(L) */
        0x22,
        /*  8:wItemLength(H) */
        0x00,
            /* Endpoint Descriptor 0 */
            /*  0:bLength */
            7,
            /*  1:bDescriptorType */
            USB_DT_ENDPOINT,
            /*  2:bEndpointAddress */
            (uint8_t)(USB_EP_IN | USB_EP1),
            /*  3:bmAttribute */
            USB_EP_INT,
            /*  4:wMaxPacketSize_lo */
            16,
            /*  5:wMaxPacketSize_hi */
            0,
            /*  6:bInterval */
            0x0A,
};
#endif  /* Peripheral HID Application demo mode */

#if USB_PHID_APL_MODE == USB_PHID_KEYBOARD_MODE
/*  Appendix E(HIDspec) is 105keyboard & pointing (2interfaces) */
#define USB_SMPLCONFLEN        34
uint8_t usb_gphid_Configuration[USB_SMPLCONFLEN + (USB_SMPLCONFLEN % 2)] =
{
    /*  0:bLength */
    9,
    /*  1:bDescriptorType */
    USB_DT_CONFIGURATION,
    /*  2:wTotalLength(L) */
    (uint8_t)(USB_SMPLCONFLEN % 256),
    /*  3:wTotalLength(H) */
    (uint8_t)(USB_SMPLCONFLEN / 256),
    /*  4:bNumInterfaces */
    1,
    /*  5:bConfigurationValue */
    1,
    /*  6:iConfiguration */
    5,
    /*  7:bmAttributes */
    (uint8_t)(USB_CF_RESERVED|USB_CF_RWUPON),
    /*  8:bMaxPower (2mA unit) */
    (uint8_t)(100 / 2),
        /* Interface Descriptor */
        /*  0:bLength */
        9,
        /*  1:bDescriptor */
        USB_DT_INTERFACE,
        /*  2:bInterfaceNumber */
        0,
        /*  3:bAlternateSetting */
        0,
        /*  4:bNumEndpoints */
        1,
        /*  5:bInterfaceClass(HID) */
        USB_IFCLS_HID,
        /*  6:bInterfaceSubClass(NonBOOT) */
        USB_IFSUB_NOBOOT,
        /*  7:bInterfaceProtocol(Keyboard) */
        USB_IFPRO_KBD,
        /*  8:iInterface */
        0,
        /* HID Descriptor */
        /*  0:bLength */
        9,
        /*  1:bDescriptor */
        USB_DT_TYPE_HIDDESCRIPTOR,
        /*  2:HID Ver */
        0x00,
        /*  3:HID Ver */
        0x01,
        /*  4:bCountryCode */
        0x00,
        /*  5:bNumDescriptors */
        0x01,
        /*  6:bDescriptorType */
        0x22,
        /*  7:wItemLength(L) */
        0x3F,
        /*  8:wItemLength(H) */
        0x00,
            /* Endpoint Descriptor 0 */
            /*  0:bLength */
            7,
            /*  1:bDescriptorType */
            USB_DT_ENDPOINT,
            /*  2:bEndpointAddress */
            (uint8_t)(USB_EP_IN  | USB_EP1),
            /*  3:bmAttribute */
            USB_EP_INT,
            /*  4:wMaxPacketSize_lo */
            8,
            /*  5:wMaxPacketSize_hi */
            0,
            /*  6:bInterval */
            0x0A,
};
#endif  /* Peripheral HID Application keyboard mode */

#if USB_PHID_APL_MODE == USB_PHID_MOUSE_MODE
#define USB_SMPLCONFLEN        34
uint8_t usb_gphid_Configuration[USB_SMPLCONFLEN + (USB_SMPLCONFLEN % 2)] =
{
    /*  0:bLength */
    9,
    /*  1:bDescriptorType */
    USB_DT_CONFIGURATION,
    /*  2:wTotalLength(L) */
    (uint8_t)(USB_SMPLCONFLEN % 256),
    /*  3:wTotalLength(H) */
    (uint8_t)(USB_SMPLCONFLEN / 256),
    /*  4:bNumInterfaces */
    1,
    /*  5:bConfigurationValue */
    1,
    /*  6:iConfiguration */
    6,
    /*  7:bmAttributes */
    (uint8_t)(USB_CF_RESERVED|USB_CF_RWUPON),
    /*  8:bMaxPower (2mA unit) */
    (uint8_t)(100 / 2),
        /* Interface Descriptor */
        /*  0:bLength */
        9,
        /*  1:bDescriptor */
        USB_DT_INTERFACE,
        /*  2:bInterfaceNumber */
        0,
        /*  3:bAlternateSetting */
        0,
        /*  4:bNumEndpoints */
        1,
        /*  5:bInterfaceClass(HID) */
        USB_IFCLS_HID,
        /*  6:bInterfaceSubClass(NonBOOT) */
        USB_IFSUB_NOBOOT,
        /*  7:bInterfaceProtocol(Mouse) */
        USB_IFPRO_MSE,
        0,/*  8:Interface */
        /* HID Descriptor */
        /*  0:bLength */
        9,
        /*  1:bDescriptor */
        USB_DT_TYPE_HIDDESCRIPTOR,
        /*  2:HID Ver */
        0x00,
        /*  3:HID Ver */
        0x01,
        /*  4:bCountryCode */
        0x00,
        /*  5:bNumDescriptors */
        0x01,
        /*  6:bDescriptorType */
        0x22,
        /*  7:wItemLength(L) */
        0x32,
        /*  8:wItemLength(H) */
        0x00,
            /* Endpoint Descriptor 0 */
            /*  0:bLength */
            7,
            /*  1:bDescriptorType */
            USB_DT_ENDPOINT,
            /*  2:bEndpointAddress */
            (uint8_t)(USB_EP_IN | USB_EP1),
            /*  3:bmAttribute */
            USB_EP_INT,
            /*  4:wMaxPacketSize_lo */
            4,
            /*  5:wMaxPacketSize_hi */
            0,
            /*  6:bInterval */
            0x0A,
};
#endif  /* Peripheral HID Application mouse mode */

/* Configuration */
uint8_t *usb_gphid_ConPtr[] =
{
    (uint8_t*)usb_gphid_Configuration,
};

/************************************************************
 *  String Descriptor 0                                     *
 ************************************************************/
 /* UNICODE 0x0409 English (United States) */
uint8_t usb_gphid_StringDescriptor0[4] = {
    /*  0:bLength */
    4,
    /*  1:bDescriptorType */
    USB_DT_STRING,
    /*  2:wLANGID[0] */
    0x09, 0x04
};

/************************************************************
 *  String Descriptor 1                                     *
 ************************************************************/
/* 14:iManufacturer */
#define USB_SD1LEN  16
uint8_t usb_gphid_StringDescriptor1[USB_SD1LEN + (USB_SD1LEN % 2)] =
{
    /*  0:bLength */
    USB_SD1LEN,
    /*  1:bDescriptorType */
    USB_DT_STRING,
    /*  2:wLANGID[0] */
    'R', 0x00,                          /*  2:wLANGID[0] */
    'E', 0x00,
    'N', 0x00,
    'E', 0x00,
    'S', 0x00,
    'A', 0x00,
    'S', 0x00,
};

/************************************************************
 *  String Descriptor 2                                     *
 ************************************************************/
/* 15:iProduct */
#define USB_SD2LEN  32
uint8_t usb_gphid_StringDescriptor2[USB_SD2LEN + (USB_SD2LEN % 2)] =
{
    /*  0:bLength */
    USB_SD2LEN,
    /*  1:bDescriptorType */
    USB_DT_STRING,
    /*  2:wLANGID[0] */
    'U', 0x00,
    'S', 0x00,
    'B', 0x00,
    ' ', 0x00,
    'P', 0x00,
    'e', 0x00,
    'r', 0x00,
    'i', 0x00,
    ' ', 0x00,
    'H', 0x00,
    'I', 0x00,
    'D', 0x00,
    ' ', 0x00,
    'F', 0x00,
    'W', 0x00,
};

/************************************************************
 *  String Descriptor 3                                     *
 ************************************************************/
/* 16:iSerialNumber */
#define USB_SD3LEN  10
uint8_t usb_gphid_StringDescriptor3[USB_SD3LEN + (USB_SD3LEN % 2)] =
{
    /*  0:bLength */
    USB_SD3LEN,
    /*  1:bDescriptorType */
    USB_DT_STRING,
    /*  2:bString */
    'P', 0x00,
    '0', 0x00,
    '0', 0x00,
    '1', 0x00
};

/************************************************************
 *  String Descriptor 4                                     *
 ************************************************************/
/* "HID USB Demonstration" */
#define STRING_PRODUCT_SIZE 44
uint8_t usb_gphid_StringDescriptor4[STRING_PRODUCT_SIZE] = 
{
    /* Length of this descriptor*/
    STRING_PRODUCT_SIZE,
    /* Descriptor Type = STRING */
    USB_DT_STRING,
    /* Descriptor Text (unicode) */
    'H', 0x00, 'I', 0x00, 'D', 0x00, ' ', 0x00,
    'U', 0x00, 'S', 0x00, 'B', 0x00, ' ', 0x00,
    'D', 0x00, 'e', 0x00, 'm', 0x00, 'o', 0x00,
    'n', 0x00, 's', 0x00, 't', 0x00, 'r', 0x00,
    'a', 0x00, 't', 0x00, 'i', 0x00, 'o', 0x00,
    'n', 0x00
};

/************************************************************
 *  String Descriptor 5                                     *
 ************************************************************/
/*  6:iConfiguration */
#define USB_SD5LEN  24
uint8_t usb_gphid_StringDescriptor5[USB_SD5LEN + (USB_SD5LEN % 2)] =
{
    /*  0:bLength */
    USB_SD5LEN,
    /*  1:bDescriptorType */
    USB_DT_STRING,
    /*  2:bString */
    'F', 0x00,
    'S', 0x00,
    ' ', 0x00,
    'K', 0x00,
    'e', 0x00,
    'y', 0x00,
    'B', 0x00,
    'o', 0x00,
    'a', 0x00,
    'r', 0x00,
    'd', 0x00
};

/************************************************************
 *  String Descriptor 6                                     *
 ************************************************************/
/*  6:iConfiguration */
#define USB_SD6LEN  18
uint8_t usb_gphid_StringDescriptor6[USB_SD6LEN + (USB_SD6LEN % 2)] =
{
    /*  0:bLength */
    USB_SD6LEN,
    /*  1:bDescriptorType */
    USB_DT_STRING,
    /*  2:bString */
    'F', 0x00,
    'S', 0x00,
    ' ', 0x00,
    'M', 0x00,
    'o', 0x00,
    'u', 0x00,
    's', 0x00,
    'e', 0x00,
};
/************************************************************
 *  String Descriptor Table address                         *
 ************************************************************/
uint8_t *usb_gphid_StrPtr[USB_STRINGNUM] =
{
    usb_gphid_StringDescriptor0,
    usb_gphid_StringDescriptor1,
    usb_gphid_StringDescriptor2,
    usb_gphid_StringDescriptor3,
    usb_gphid_StringDescriptor4,
    usb_gphid_StringDescriptor5,
    usb_gphid_StringDescriptor6,
};

#if USB_PHID_APL_MODE == USB_PHID_DEMO_MODE
/************************************************************
 *  HID Report Discriptor (demo)                            *
 ************************************************************/
#define USB_SMPLRPRTLEN    34
uint8_t usb_gphid_ReportDiscriptor[USB_SMPLRPRTLEN + (USB_SMPLRPRTLEN % 2)] =
{
    /* Usage Page - Vendor defined*/
    0x06, 0xA0, 0xFF,
    /* Usage ID within this page (Vendor defined)*/
    0x09, 0x00,
    /* Collection App (Windows requires an Application Collection) */
    0xA1, 0x01,
    
    /* *** The INPUT REPORT *** */
    /* Usage ID within this page*/
    0x09, 0x00,
    /*Logical Min 0 */
    0x15, 0x00,
    /*Logical Max 255 */
    0x26, 0xFF, 0x00,   
    /* Size 8 Bits (Each Field will be 8bits) */
    0x75, 0x08,
    /* Count (Number of fields(bytes) in INPUT report) */
    0x95, 0x05,
    /* Input Report - type variable data */
    0x81, 0x02,
    
    /* *** The OUTPUR REPORT *** */
    /* Usage ID within this page (Vendor defined)*/
    0x09, 0x00,
    /*Logical Min 0 */
    0x15, 0x00,
    /*Logical Max 255 */
    0x26, 0xFF, 0x00,   
    /* Size 8 Bits (Each Field will be 8bits) */
    0x75, 0x08,
    /* Count (Number of fields(bytes) in OUTPUT report) */
    0x95, 0x11,
    /* Output Report - type variable data */
    0x91, 0x02,
    
    /* End collection */
    0xC0
};
#endif  /* Peripheral HID Application demo mode */

#if USB_PHID_APL_MODE == USB_PHID_KEYBOARD_MODE
/************************************************************
 *  HID Report Discriptor (keyboard)                        *
 ************************************************************/
#define USB_SMPLRPRTLEN    63
uint8_t usb_gphid_ReportDiscriptor[USB_SMPLRPRTLEN + (USB_SMPLRPRTLEN % 2)] =
{
    0x05, 0x01,
    0x09, 0x06,
    0xA1, 0x01,
    0x05, 0x07,
    0x19, 0xE0,
    0x29, 0xE7,
    0x15, 0x00,
    0x25, 0x01,
    0x75, 0x01,
    0x95, 0x08,
    0x81, 0x02,
    0x95, 0x01,
    0x75, 0x08,
    0x81, 0x01,
    0x95, 0x05,
    0x75, 0x01,
    0x05, 0x08,
    0x19, 0x01,
    0x29, 0x05,
    0x91, 0x02,
    0x95, 0x01,
    0x75, 0x03,
    0x91, 0x01,
    0x95, 0x06,
    0x75, 0x08,
    0x15, 0x00,
    0x25, 0x65,
    0x05, 0x07,
    0x19, 0x00,
    0x29, 0x65,
    0x81, 0x00,
    0xC0
};
#endif  /* Peripheral HID Application keyboard mode */

#if USB_PHID_APL_MODE == USB_PHID_MOUSE_MODE
/************************************************************
 *  HID Report Discriptor (mouse)                           *
 ************************************************************/
#define USB_SMPLRPRTLEN    50
uint8_t usb_gphid_ReportDiscriptor[USB_SMPLRPRTLEN + (USB_SMPLRPRTLEN % 2)] =
{
    0x05, 0x01,
    0x09, 0x02,
    0xA1, 0x01,
    0x09, 0x01,
    0xA1, 0x00,
    0x05, 0x09,
    0x19, 0x01,
    0x29, 0x03,
    0x15, 0x00,
    0x25, 0x01,
    0x95, 0x03,
    0x75, 0x01,
    0x81, 0x02,
    0x95, 0x01,
    0x75, 0x05,
    0x81, 0x01,
    0x05, 0x01,
    0x09, 0x30,
    0x09, 0x31,
    0x15, 0x81,
    0x25, 0x7F,
    0x75, 0x08,
    0x95, 0x02,
    0x81, 0x06,
    0xC0, 0xC0
};
#endif  /* Peripheral HID Application mouse mode */


/******************************************************************************
End  Of File
******************************************************************************/
