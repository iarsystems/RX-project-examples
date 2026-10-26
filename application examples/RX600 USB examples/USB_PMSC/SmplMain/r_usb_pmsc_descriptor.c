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
* Copyright (C) 2010 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************
* File Name    : r_usb_PMSCdescriptor.c
* Version      : 1.00
* Device(s)    : Renesas SH-Series, RX-Series
* Tool-Chain   : Renesas SuperH RISC engine Standard Toolchain
*              : Renesas RX Standard Toolchain
* OS           : Common to None and uITRON 4.0 Spec
* H/W Platform : Independent
* Description  : USB Host and Peripheral Interrupt code
*******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 17.03.2010 0.80    First Release
*         : 30.07.2010 0.90    Updated comments
*         : 02.08.2010 0.91    Updated comments
*         : 29.10.2010 1.00    Mass Production Release
******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
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
#include "r_usb_cmsc_define.h"      /* USB  Mass Storage Class Header */
#include "r_usb_pmsc_define.h"      /* Peri Mass Storage Class Driver define */
#include "r_usb_catapi_define.h"        /* Peripheral ATAPI Device extern define */
//#include "r_usb_pmsc_extern.h"        /* Peri Mass Storage Class Extern */

extern USB_PMSC_CBM_t   usb_gpmsc_Message;

/******************************************************************************
Section    <Section Difinition> , "Project Sections"
******************************************************************************/
#pragma section="_pmsc"


/******************************************************************************
User define macro definitions
******************************************************************************/
#define USB_VENDORID    0x0000
#define USB_PRODUCTID   0x0008

/******************************************************************************
Constant macro definitions
******************************************************************************/
#define USB_PMSC_DD_BLENGTH         18
#define USB_PMSC_QD_BLENGTH         10
#define USB_PMSC_CD1_BLENGTH        32
#define USB_PMSC_SD0_BLENGTH        4
#define USB_PMSC_SD1_BLENGTH        16
#define USB_PMSC_SD2_BLENGTH        34
#define USB_PMSC_SD3_BLENGTH        32
#define USB_PMSC_SD4_BLENGTH        22
#define USB_PMSC_SD5_BLENGTH        18
#define USB_PMSC_SD6_BLENGTH        28


/******************************************************************************
External variables and functions
******************************************************************************/


/******************************************************************************
Private global variables and functions
******************************************************************************/
/* Configuration 1 */
uint16_t usb_gpmsc_EpTbl1[] =
{
    USB_PIPE1,
    USB_BULK | USB_BFREOFF | USB_DBLBON | USB_CNTMDON | USB_SHTNAKON
        | USB_DIR_P_IN | USB_EP1,
    (uint16_t)USB_BUF_SIZE(512u) | USB_BUF_NUMB(8u),
    USB_SOFT_CHANGE,
    USB_IFISOFF | USB_IITV_TIME(0u),
    USB_CUSE,

    USB_PIPE2,
    USB_BULK | USB_BFREOFF | USB_DBLBON | USB_CNTMDON | USB_SHTNAKON
        | USB_DIR_P_OUT | USB_EP2,
    (uint16_t)USB_BUF_SIZE(512u) | USB_BUF_NUMB(24u),
    USB_SOFT_CHANGE,
    USB_IFISOFF | USB_IITV_TIME(0u),
    USB_CUSE,

/* Pipe end */
    USB_PDTBLEND,
};

/* Configuration 2 */
uint16_t usb_gpmsc_EpTbl2[] =
{
    0
};
/* Configuration 3 */
uint16_t usb_gpmsc_EpTbl3[] =
{
    0
};
/* Configuration 4 */
uint16_t usb_gpmsc_EpTbl4[] =
{
    0
};
/* Configuration 5 */
uint16_t usb_gpmsc_EpTbl5[] =
{
    0
};


uint16_t *usb_gpmsc_EpPtr[] =
{
    usb_gpmsc_EpTbl1,
    usb_gpmsc_EpTbl2,
    usb_gpmsc_EpTbl3,
    usb_gpmsc_EpTbl4,
    usb_gpmsc_EpTbl5
};

/******************************************************************************
Private global variables and functions
******************************************************************************/

/* Standard Device Descriptor */
uint8_t usb_gpmsc_DeviceDescriptor
    [ USB_PMSC_DD_BLENGTH + ( USB_PMSC_DD_BLENGTH % 2 ) ] =
{
    /*  0:bLength */
    USB_PMSC_DD_BLENGTH,
    /*  1:bDescriptorType */
    USB_DT_DEVICE,
    /*  2:bcdUSB_lo */
    (USB_BCDNUM                     & (uint8_t)0xffu),
    /*  3:bcdUSB_hi */
    ((uint8_t)(USB_BCDNUM >> 8)     & (uint8_t)0xffu),
    /*  4:bDeviceClass */
    0,
    /*  5:bDeviceSubClass */
    0,
    /*  6:bDeviceProtocol */
    0,
    /*  7:bMAXPacketSize(for DCP) */
    (uint8_t)USB_DCPMAXP,
    /*  8:idVendor_lo */
    (USB_VENDORID                   & (uint8_t)0xffu),
    /*  9:idVendor_hi */
    ((uint8_t)(USB_VENDORID >> 8)   & (uint8_t)0xffu),
    /* 10:idProduct_lo */
    ((uint16_t)USB_PRODUCTID        & (uint8_t)0xffu),
    /* 11:idProduct_hi */
    ((uint8_t)(USB_PRODUCTID >> 8)  & (uint8_t)0xffu),
    /* 12:bcdDevice_lo */
    (USB_RELEASE                    & (uint8_t)0xffu),
    /* 13:bcdDevice_hi */
    ((uint8_t)(USB_RELEASE >> 8)    & (uint8_t)0xffu),
    /* 14:iManufacturer */
    1,
    /* 15:iProduct */
    2,
    /* 16:iSerialNumber */
    6,
    /* 17:bNumConfigurations */
    USB_CONFIGNUM
};

/* Device Qualifier Descriptor */
uint8_t usb_gpmsc_QualifierDescriptor
    [ USB_PMSC_QD_BLENGTH + ( USB_PMSC_QD_BLENGTH % 2 ) ] =
{
    /*  0:bLength */
    USB_PMSC_QD_BLENGTH,
    /*  1:bDescriptorType */
    USB_DT_DEVICE_QUALIFIER,
    /*  2:bcdUSB_lo */
    (USB_BCDNUM                     & (uint8_t)0xffu),
    /*  3:bcdUSB_hi */
    ((uint8_t)(USB_BCDNUM >> 8)     & (uint8_t)0xffu),
    /*  4:bDeviceClass */
    0,
    /*  5:bDeviceSubClass */
    0,
    /*  6:bDeviceProtocol */
    0,
    /*  7:bMAXPacketSize(for DCP) */
    (uint8_t)USB_DCPMAXP,
    /*  8:bNumConfigurations */
    USB_CONFIGNUM,
    /*  9:bReserved */
    0
};


/************************************************************
 *  Configuration Or Other_Speed_Configuration Descriptor   *
 ************************************************************/
/* For Full-Speed */
uint8_t usb_gpmsc_ConfigrationF1
    [ USB_PMSC_CD1_BLENGTH + ( USB_PMSC_CD1_BLENGTH % 2 ) ] =
{
    9,                                  /*  0:bLength */
    USB_SOFT_CHANGE,                    /*  1:bDescriptorType */
    USB_PMSC_CD1_BLENGTH % 256,         /*  2:wTotalLength(L) */
    USB_PMSC_CD1_BLENGTH / 256,         /*  3:wTotalLength(H) */
    1,                                  /*  4:bNumInterfaces */
    1,                                  /*  5:bConfigurationValue */
    4,                                  /*  6:iConfiguration */
    USB_CF_RESERVED | USB_CF_SELFP,     /*  7:bmAttributes */
    (10 / 2),                           /*  8:MAXPower (2mA unit) */

    /* Interface Descriptor */
        9,                              /*  0:bLength */
        USB_DT_INTERFACE,               /*  1:bDescriptor */
        0,                              /*  2:bInterfaceNumber */
        0,                              /*  3:bAlternateSetting */
        2,                              /*  4:bNumEndpoints */
        USB_IFCLS_MAS,                  /*  5:bInterfaceClass */
        USB_ATAPI,                      /*  6:bInterfaceSubClass */
        USB_BOTP,                       /*  7:bInterfaceProtocol */
        3,                              /*  8:iInterface */

        /* Endpoint Descriptor 0 */
            7,                          /*  0:bLength */
            USB_DT_ENDPOINT,            /*  1:bDescriptorType */
            USB_EP_IN  | USB_EP1,       /*  2:bEndpointAddress */
            USB_EP_BULK,                /*  3:bmAttribute */
            64,                         /*  4:wMAXPacketSize_lo */
            0,                          /*  5:wMAXPacketSize_hi */
            0,                          /*  6:bInterval */

        /* Endpoint Descriptor 1 */
            7,                          /*  0:bLength */
            USB_DT_ENDPOINT,            /*  1:bDescriptorType */
            USB_EP_OUT | USB_EP2,       /*  2:bEndpointAddress */
            USB_EP_BULK,                /*  3:bmAttribute */
            64,                         /*  4:wMAXPacketSize_lo */
            0,                          /*  5:wMAXPacketSize_hi */
            1,                          /*  6:bInterval */
};

uint8_t usb_gpmsc_ConfigrationH1
    [ USB_PMSC_CD1_BLENGTH + ( USB_PMSC_CD1_BLENGTH % 2 ) ] =
{
    9,                                  /*  0:bLength */
    USB_SOFT_CHANGE,                    /*  1:bDescriptorType */
    USB_PMSC_CD1_BLENGTH % 256,         /*  2:wTotalLength(L) */
    USB_PMSC_CD1_BLENGTH / 256,         /*  3:wTotalLength(H) */
    1,                                  /*  4:bNumInterfaces */
    1,                                  /*  5:bConfigurationValue */
    5,                                  /*  6:iConfiguration */
    USB_CF_RESERVED | USB_CF_SELFP,     /*  7:bmAttributes */
    (10 / 2),                           /*  8:MAXPower (2mA unit) */

    /* Interface Descriptor */
        9,                              /*  0:bLength */
        USB_DT_INTERFACE,               /*  1:bDescriptor */
        0,                              /*  2:bInterfaceNumber */
        0,                              /*  3:bAlternateSetting */
        2,                              /*  4:bNumEndpoints */
        USB_IFCLS_MAS,                  /*  5:bInterfaceClass */
        USB_ATAPI,                      /*  6:bInterfaceSubClass */
        USB_BOTP,                       /*  7:bInterfaceProtocol */
        3,                              /*  8:iInterface */

        /* Endpoint Descriptor 0 */
            7,                          /*  0:bLength */
            USB_DT_ENDPOINT,            /*  1:bDescriptorType */
            USB_EP_IN  | USB_EP1,       /*  2:bEndpointAddress */
            USB_EP_BULK,                /*  3:bmAttribute */
            0,                          /*  4:wMAXPacketSize_lo */
            2,                          /*  5:wMAXPacketSize_hi */
            0,                          /*  6:bInterval */

        /* Endpoint Descriptor 1 */
            7,                          /*  0:bLength */
            USB_DT_ENDPOINT,            /*  1:bDescriptorType */
            USB_EP_OUT | USB_EP2,       /*  2:bEndpointAddress */
            USB_EP_BULK,                /*  3:bmAttribute */
            0,                          /*  4:wMAXPacketSize_lo */
            2,                          /*  5:wMAXPacketSize_hi */
            1,                          /*  6:bInterval */
};


/* Configuration */
uint8_t *usb_gpmsc_ConPtr[] =
{
    (uint8_t*)USB_SOFT_CHANGE
};

/* Other_Speed_Configuration */
uint8_t *usb_gpmsc_ConPtrOther[] =
{
    (uint8_t*)USB_SOFT_CHANGE
};


/*************************************
 *    String Descriptor              *
 *************************************/
uint8_t usb_gpmsc_StringDescriptor0
    [ USB_PMSC_SD0_BLENGTH + ( USB_PMSC_SD0_BLENGTH % 2 ) ] =
{   /* UNICODE 0x0409 English (United States) */
    USB_PMSC_SD0_BLENGTH,               /*  0:bLength */
    USB_DT_STRING,                      /*  1:bDescriptorType */
    0x09, 0x04                          /*  2:wLANGID[0] */
};

uint8_t usb_gpmsc_StringDescriptor1
    [ USB_PMSC_SD1_BLENGTH + ( USB_PMSC_SD1_BLENGTH % 2 ) ] =
{   /* iManufacturer */
    USB_PMSC_SD1_BLENGTH,               /*  0:bLength */
    USB_DT_STRING,                      /*  1:bDescriptorType */
    'R', 0x00,                          /*  2:wLANGID[0] */
    'E', 0x00,
    'N', 0x00,
    'E', 0x00,
    'S', 0x00,
    'A', 0x00,
    'S', 0x00,
};

uint8_t usb_gpmsc_StringDescriptor2
    [ USB_PMSC_SD2_BLENGTH + ( USB_PMSC_SD2_BLENGTH % 2 ) ] =
{   /* iProduct */
    USB_PMSC_SD2_BLENGTH,               /*  0:bLength */
    USB_DT_STRING,                      /*  1:bDescriptorType */
    'U', 0x00,
    'S', 0x00,
    'B', 0x00,
    ' ', 0x00,
    'M', 0x00,
    'a', 0x00,
    's', 0x00,
    's', 0x00,
    ' ', 0x00,
    'S', 0x00,
    't', 0x00,
    'o', 0x00,
    'r', 0x00,
    'a', 0x00,
    'g', 0x00,
    'e', 0x00
};

uint8_t usb_gpmsc_StringDescriptor3
    [ USB_PMSC_SD3_BLENGTH + ( USB_PMSC_SD3_BLENGTH % 2 ) ] =
{   /* iInterface */
    USB_PMSC_SD3_BLENGTH,               /*  0:bLength */
    USB_DT_STRING,                      /*  1:bDescriptorType */
    'R', 0x00,
    'e', 0x00,
    'm', 0x00,
    'o', 0x00,
    'v', 0x00,
    'a', 0x00,
    'b', 0x00,
    'l', 0x00,
    'e', 0x00,
    ' ', 0x00,
    'D', 0x00,
    'r', 0x00,
    'i', 0x00,
    'v', 0x00,
    'e', 0x00
};

uint8_t usb_gpmsc_StringDescriptor4
    [ USB_PMSC_SD4_BLENGTH + ( USB_PMSC_SD4_BLENGTH % 2 ) ] =
{   /* iConfiguration */
    USB_PMSC_SD4_BLENGTH,               /*  0:bLength */
    USB_DT_STRING,                      /*  1:bDescriptorType */
    'F', 0x00,                          /*  2:wLANGID[0] */
    'u', 0x00,
    'l', 0x00,
    'l', 0x00,
    '-', 0x00,
    'S', 0x00,
    'p', 0x00,
    'e', 0x00,
    'e', 0x00,
    'd', 0x00
};

uint8_t usb_gpmsc_StringDescriptor5
    [ USB_PMSC_SD5_BLENGTH + ( USB_PMSC_SD5_BLENGTH % 2 ) ] =
{   /* iConfiguration */
    USB_PMSC_SD5_BLENGTH,               /*  0:bLength */
    USB_DT_STRING,                      /*  1:bDescriptorType */
    'H', 0x00,                          /*  2:wLANGID[0] */
    'i', 0x00,
    '-', 0x00,
    'S', 0x00,
    'p', 0x00,
    'e', 0x00,
    'e', 0x00,
    'd', 0x00
};

uint8_t usb_gpmsc_StringDescriptor6
    [ USB_PMSC_SD6_BLENGTH + ( USB_PMSC_SD6_BLENGTH % 2 ) ] =
{   /* iSerialNumber */
    USB_PMSC_SD6_BLENGTH,               /*  0:bLength */
    USB_DT_STRING,                      /*  1:bDescriptorType */
    '0', 0x00,                          /*  2:wLANGID[0] */
    '0', 0x00,
    '0', 0x00,
    '0', 0x00,
    '0', 0x00,
    '0', 0x00,
    '0', 0x00,
    '0', 0x00,
    '0', 0x00,
    '0', 0x00,
    '0', 0x00,
    '0', 0x00,
    '1', 0x00,
};

uint8_t *usb_gpmsc_StrPtr[USB_STRINGNUM] =
{
    usb_gpmsc_StringDescriptor0,
    usb_gpmsc_StringDescriptor1,
    usb_gpmsc_StringDescriptor2,
    usb_gpmsc_StringDescriptor3,
    usb_gpmsc_StringDescriptor4,
    usb_gpmsc_StringDescriptor5,
    usb_gpmsc_StringDescriptor6
};


/******************************************************************************
Renesas Abstracted Peripheral Mass Storage Class Driver API functions
******************************************************************************/


/******************************************************************************
End  Of File
******************************************************************************/
