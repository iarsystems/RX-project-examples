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
* http://www.renesas.com/disclaimer *
* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name     : r_usb_phid_descriptor.c
* Version       : 2.10
* Description   : USB Peripheral HID class driver code.
*******************************************************************************/
/*******************************************************************************
* History   : DD.MM.YYYY Version Description
*           : 30.11.2012 2.00    First Release
*           : 31.05.2013 2.10    Corresponded to RL78/L1C, RX111
*******************************************************************************/

/******************************************************************************
Section definitions
******************************************************************************/

/******************************************************************************
Includes <System Includes> , "Project Includes"
******************************************************************************/
#include "r_usb_ctypedef.h"             /* Type define */
#include "r_usb_ckernelid.h"            /* Kernel ID definition */
#include "r_usb_usrconfig.h"            /* System definition, USB-FW Library Header */
#include "r_usb_cdefusbip.h"            /* USB-FW Library Header */
#include "r_usb_usbip.h"                /* Definition of the USB register & bit define */
#include "r_usb_phid_define.h"
#include "r_usb_phid_usrcfg.h"
#include "r_usb_phid_api.h"


/******************************************************************************
User define macro definitions
******************************************************************************/
/* There is no macro definition. */


/******************************************************************************
Private global variables and functions
******************************************************************************/
/************************************************************
 *  Pipe Define 1                                           *
 ************************************************************/
/* End point table for Demonstration */
uint16_t usb_gphid_EpTbl[] =
{
    USB_PHID_USE_PIPE,
    USB_INT | USB_DIR_P_IN | USB_EP6,
    USB_INTEPMAXP,
    USB_NULL,
    USB_PDTBLEND,
};


/************************************************************
 *  Device Descriptor                                       *
 ************************************************************/
uint8_t usb_gphid_SmplDeviceDescriptor[18] =
{
    18,                                 /*  0:bLength */
    USB_DT_DEVICE,                      /*  1:bDescriptorType */
    (uint8_t)( USB_BCDNUM & (uint8_t)0xff),                     /*  2:bcdUSB_lo */
    (uint8_t)((uint8_t)(USB_BCDNUM >> 8) & (uint8_t)0xff),      /*  3:bcdUSB_hi */
    0x00,                               /*  4:bDeviceClass */
    0x00,                               /*  5:bDeviceSubClass */
    0x00,                                /*  6:bDeviceProtocol */
    (uint8_t)USB_DCPMAXP,               /*  7:bMAXPacketSize(for DCP) */
    (uint8_t)(USB_VENDORID & (uint8_t)0xff),                    /*  8:idVendor_lo */
    (uint8_t)((uint8_t)(USB_VENDORID >> 8) & (uint8_t)0xff),    /*  9:idVendor_hi */
    (uint8_t)(USB_PRODUCTID & (uint8_t)0xff),                   /* 10:idProduct_lo */
    (uint8_t)((uint8_t)(USB_PRODUCTID >> 8) & (uint8_t)0xff),   /* 11:idProduct_hi */
    (uint8_t)(USB_RELEASE & (uint8_t)0xff),                     /* 12:bcdDevice_lo */
    (uint8_t)((uint8_t)(USB_RELEASE >> 8) & (uint8_t)0xff),     /* 13:bcdDevice_hi */
    1,                                  /* 14:iManufacturer */
    2,                                  /* 15:iProduct */
    3,                                  /* 16:iSerialNumber */
    USB_CONFIGNUM                       /* 17:bNumConfigurations */
};

/************************************************************
 *  Configuration Descriptor                                *
 ************************************************************/
#define USB_SMPLCONFLEN        34
uint8_t usb_gphid_Configuration[USB_SMPLCONFLEN + (USB_SMPLCONFLEN % 2)] =
{
    9,                                      /*  0:bLength */
    USB_DT_CONFIGURATION,                   /*  1:bDescriptorType */
    (uint8_t)(USB_SMPLCONFLEN % 256),       /*  2:wTotalLength(L) */
    (uint8_t)(USB_SMPLCONFLEN / 256),       /*  3:wTotalLength(H) */
    1,                                      /*  4:bNumInterfaces */
    1,                                      /*  5:bConfigurationValue */
    USB_ICONFIGBUM,                         /*  6:iConfiguration */
    (uint8_t)(USB_CF_RESERVED | USB_CF_RWUPON),     /*  7:bmAttributes */
    (uint8_t)(100 / 2),                     /*  8:bMaxPower (2mA unit) */
        /* Interface Descriptor */
        9,                              /*  0:bLength */
        USB_DT_INTERFACE,               /*  1:bDescriptor */
        0,                              /*  2:bInterfaceNumber */
        0,                              /*  3:bAlternateSetting */
        1,                              /*  4:bNumEndpoints */
        USB_IFCLS_HID,                  /*  5:bInterfaceClass(HID) */
        USB_IFSUB_NOBOOT,               /*  6:bInterfaceSubClass(NonBOOT) */
        USB_IFPROTOCOL,                 /*  7:bInterfaceProtocol */
        0,                              /*  8:iInterface */
        /* HID Descriptor */
        9,                              /*  0:bLength */
        USB_DT_TYPE_HIDDESCRIPTOR,      /*  1:bDescriptor */
        0x00,                           /*  2:HID Ver */
        0x01,                           /*  3:HID Ver */
        0x00,                           /*  4:bCountryCode */
        0x01,                           /*  5:bNumDescriptors */
        0x22,                           /*  6:bDescriptorType */
        USB_SMPLRPRTLEN,                /*  7:wItemLength(L) */
        0x00,                           /*  8:wItemLength(H) */
            /* Endpoint Descriptor 0 */
            7,                          /*  0:bLength */
            USB_DT_ENDPOINT,            /*  1:bDescriptorType */
            (uint8_t)(USB_EP_IN | USB_EP6),                 /*  2:bEndpointAddress */
            USB_EP_INT,                 /*  3:bmAttribute */
            USB_INTEPMAXP,              /*  4:wMaxPacketSize_lo */
            0,                          /*  5:wMaxPacketSize_hi */
            0x0A,                       /*  6:bInterval */
};


/************************************************************
 *  String Descriptor 0                                     *
 ************************************************************/
 /* UNICODE 0x0409 English (United States) */
uint8_t usb_gphid_StringDescriptor0[4] = {
    4,                      /*  0:bLength */
    USB_DT_STRING,          /*  1:bDescriptorType */
    0x09, 0x04              /*  2:wLANGID[0] */
};

/************************************************************
 *  String Descriptor 1                                     *
 ************************************************************/
/* 14:iManufacturer */
#define USB_SD1LEN  16
uint8_t usb_gphid_StringDescriptor1[USB_SD1LEN + (USB_SD1LEN % 2)] =
{
    USB_SD1LEN,             /*  0:bLength */
    USB_DT_STRING,          /*  1:bDescriptorType */
    'R', 0x00,              /*  2:wLANGID[0] */
    'e', 0x00,
    'n', 0x00,
    'e', 0x00,
    's', 0x00,
    'a', 0x00,
    's', 0x00,
};

/************************************************************
 *  String Descriptor 2                                     *
 ************************************************************/
/* 15:iProduct */
#define USB_SD2LEN  32
uint8_t usb_gphid_StringDescriptor2[USB_SD2LEN + (USB_SD2LEN % 2)] =
{
    USB_SD2LEN,             /*  0:bLength */
    USB_DT_STRING,          /*  1:bDescriptorType */
    'U', 0x00,              /*  2:wLANGID[0] */
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
    USB_SD3LEN,             /*  0:bLength */
    USB_DT_STRING,          /*  1:bDescriptorType */
    'P', 0x00,              /*  2:bString */
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
    STRING_PRODUCT_SIZE,    /* Length of this descriptor*/
    USB_DT_STRING,          /* Descriptor Type = STRING */
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
    USB_SD5LEN,             /*  0:bLength */
    USB_DT_STRING,          /*  1:bDescriptorType */
    /*  2:bString */
#ifdef  USB_FSPERI_PP
    'F', 0x00,
#endif  /* USB_FSPERI_PP */
#ifdef  USB_LSPERI_PP
    'L', 0x00,
#endif  /* USB_LSPERI_PP */
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
    USB_SD6LEN,             /*  0:bLength */
    USB_DT_STRING,          /*  1:bDescriptorType */
    /*  2:bString */
#ifdef  USB_FSPERI_PP
    'F', 0x00,
#endif  /* USB_FSPERI_PP */
#ifdef  USB_LSPERI_PP
    'L', 0x00,
#endif  /* USB_LSPERI_PP */
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
const uint8_t usb_gphid_ReportDiscriptor[USB_SMPLRPRTLEN + (USB_SMPLRPRTLEN % 2)] =
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
const uint8_t usb_gphid_ReportDiscriptor[USB_SMPLRPRTLEN + (USB_SMPLRPRTLEN % 2)] =
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
const uint8_t usb_gphid_ReportDiscriptor[USB_SMPLRPRTLEN + (USB_SMPLRPRTLEN % 2)] =
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
