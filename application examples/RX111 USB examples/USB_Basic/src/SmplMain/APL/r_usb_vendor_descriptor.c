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
* File Name    : r_usb_vendor_descriptor.c
* Version      : 2.01
* Description  : USB Peripheral vendor descriptor code.
*******************************************************************************/
/*******************************************************************************
* History   : DD.MM.YYYY Version Description
*           : 31.03.2012 1.80    Basic FW mini Ver2.00(ES Version)
*           : 30.11.2012 2.00    Basic FW mini Ver2.00 Release
*           : 31.01.2013 2.01    Corresponded to RL78/L1C
*******************************************************************************/

/******************************************************************************
Section definitions
******************************************************************************/


/******************************************************************************
Includes <System Includes> , "Project Includes"
******************************************************************************/
#include "r_usb_ctypedef.h"             /* Type define */
#include "r_usb_usrconfig.h"            /* System definition, USB-FW Library Header */
#include <iorx111.h>

/******************************************************************************
Macro definitions
******************************************************************************/
#ifdef  USB_LSPERI_PP
  #define USB_PVDR_BLENGTH  32              /* Low Speed (PIPE 6-7) */
  #define USB_DCPMAXP       (8u)            /* DCP max packet size */
  #define USB_EPNUMS        (2)             /* Endpoint number */
  #define USB_INTEPMAXP     (8u)            /* Interrupt pipe max packet size */
#endif  /* USB_LSPERI_PP */
#ifdef  USB_FSPERI_PP
  #define USB_PVDR_BLENGTH  46              /* Full Speed (PIPE 4-7) */
  #define USB_DCPMAXP       (64u)           /* DCP max packet size */
  #define USB_EPNUMS        (4)             /* Endpoint number */
  #define USB_INTEPMAXP     (64u)           /* Interrupt pipe max packet size */
#endif  /* USB_FSPERI_PP */
#define USB_SD1LEN  16
#define USB_SD2LEN  (32+6)
#define USB_VENDORID        (0x045Bu)       /* Vendor ID, was (0x0000u) */
#define USB_PRODUCTID       (0x0512u)       /* Product ID, was (0x00FFu) */
#define USB_BCDNUM          (0x0200u)       /* bcdUSB */
#define USB_RELEASE         (0x0100u)       /* Release Number */
#define USB_CONFIGNUM       (1u)            /* Configuration number */


/******************************************************************************
Typedef definitions
******************************************************************************/
/* There is no typedef definition. */


/******************************************************************************
Private global variables and functions
******************************************************************************/
/* variables */
USB_STATIC uint8_t g_usb_psmpl_StringDescriptor0[4] =
{
    /* UNICODE 0x0409 English (United States) */
    4,                                              /*  0:bLength */
    USB_DT_STRING,                                  /*  1:bDescriptorType */
    0x09, 0x04                                      /*  2:wLANGID[0] */
};

USB_STATIC uint8_t g_usb_psmpl_StringDescriptor1[ USB_SD1LEN + ( USB_SD1LEN % 2 ) ] =
{
    USB_SD1LEN,                                     /*  0:bLength */
    USB_DT_STRING,                                  /*  1:bDescriptorType */
    'R', 0x00,                                      /*  2:wLANGID[0] */
    'E', 0x00,
    'N', 0x00,
    'E', 0x00,
    'S', 0x00,
    'A', 0x00,
    'S', 0x00,
};

/* iProduct */
USB_STATIC  uint8_t g_usb_psmpl_StringDescriptor2[ USB_SD2LEN + ( USB_SD2LEN % 2 ) ] =
{
    USB_SD2LEN,                                     /*  0:bLength */
    USB_DT_STRING,                                  /*  1:bDescriptorType */
    'U', 0x00,                                      /*  2:wLANGID[0] */
    'S', 0x00,
    'B', 0x00,
    ' ', 0x00,
    'S', 0x00,
    'T', 0x00,
    'A', 0x00,
    'N', 0x00,
    'D', 0x00,
    'A', 0x00,
    'R', 0x00,
    'D', 0x00,
    ' ', 0x00,
    'F', 0x00,
    'W', 0x00,
    'X', 0x00,
    'Y', 0x00,
    'Z', 0x00
};

/* iSerialNumber */
USB_STATIC  uint8_t g_usb_psmpl_StringDescriptor3[22] =
{
    22,                                             /*  0:bLength */
    USB_DT_STRING,                                  /*  1:bDescriptorType */
    '0', 0x00,                                      /*  2:bString */
    '1', 0x00,
    '2', 0x00,
    '3', 0x00,
    '4', 0x00,
    '5', 0x00,
    '6', 0x00,
    '7', 0x00,
    '8', 0x00,
    '9', 0x00,
};

#ifdef  USB_FSPERI_PP
/* iConfiguration */
USB_STATIC  uint8_t g_usb_psmpl_StringDescriptor4[22] =
{
    22,                                             /*  0:bLength */
    USB_DT_STRING,                                  /*  1:bDescriptorType */
    'F', 0x00,                                      /*  2:bString */
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
#endif  /* USB_FSPERI_PP */

#ifdef  USB_LSPERI_PP
/* iConfiguration */
USB_STATIC  uint8_t g_usb_psmpl_StringDescriptor4[20] =
{
    20,                                             /*  0:bLength */
    USB_DT_STRING,                                  /*  1:bDescriptorType */
    'L', 0x00,                                      /*  2:bString */
    'o', 0x00,
    'w', 0x00,
    '-', 0x00,
    'S', 0x00,
    'p', 0x00,
    'e', 0x00,
    'e', 0x00,
    'd', 0x00
};
#endif  /* USB_LSPERI_PP */

/* iInterface */
USB_STATIC  uint8_t g_usb_psmpl_StringDescriptor5[26] =
{
    26,                                             /*  0:bLength */
    USB_DT_STRING,                                  /*  1:bDescriptorType */
    'V', 0x00,                                      /*  2:bString */
    'E', 0x00,
    'N', 0x00,
    'D', 0x00,
    'O', 0x00,
    'R', 0x00,
    ' ', 0x00,
    'C', 0x00,
    'L', 0x00,
    'A', 0x00,
    'S', 0x00,
    'S', 0x00,
};

/* functions */


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
/* variables */
uint16_t g_usb_psmpl_EpTbl1[] =
{
#ifdef  USB_FSPERI_PP
    /* PIPE4 Definition */
    USB_PIPE4,
    USB_BULK | USB_BFREOFF | USB_DBLBON | USB_SHTNAKON | USB_DIR_P_OUT | USB_EP1,
    USB_MAX_PACKET(64), USB_NULL,
    /* PIPE5 Definition */
    USB_PIPE5,
    USB_BULK | USB_BFREOFF | USB_DBLBON | USB_SHTNAKON | USB_DIR_P_IN  | USB_EP2,
    USB_MAX_PACKET(64), USB_NULL,
#endif  /* USB_FSPERI_PP */
    /* PIPE6 Definition */
    USB_PIPE6,
    USB_INT  | USB_DIR_P_OUT | USB_EP3,
    USB_MAX_PACKET(USB_INTEPMAXP), USB_NULL,
    /* PIPE7 Definition */
    USB_PIPE7,
    USB_INT  | USB_DIR_P_IN  | USB_EP4,
    USB_MAX_PACKET(USB_INTEPMAXP), USB_NULL,
    USB_PDTBLEND,
};

uint8_t g_usb_psmpl_DeviceDescriptor[ 18 ] =
{
    18,                                         /*  0:bLength */
    USB_DT_DEVICE,                              /*  1:bDescriptorType */
    (uint8_t)(USB_BCDNUM),                      /*  3:bcdUSB_hi */
    (uint8_t)((uint16_t)USB_BCDNUM >> 8),       /*  3:bcdUSB_hi */
    0xff,                                       /*  4:bDeviceClass */
    0xff,                                       /*  5:bDeviceSubClass */
    0xff,                                       /*  6:bDeviceProtocol */
    (uint8_t)USB_DCPMAXP,                       /*  7:bMAXPacketSize(for DCP) */
    (uint8_t)(USB_VENDORID),                    /*  8:idVendor_lo */
    (uint8_t)((uint16_t)USB_VENDORID >> 8),     /*  9:idVendor_hi */
    (uint8_t)(USB_PRODUCTID),                   /* 10:idProduct_lo */
    (uint8_t)((uint16_t)USB_PRODUCTID >> 8),    /* 11:idProduct_hi */
    (uint8_t)(USB_RELEASE),                     /* 12:bcdDevice_lo */
    (uint8_t)((uint8_t)USB_RELEASE >> 8),       /* 13:bcdDevice_hi */
    1,                                          /* 14:iManufacturer */
    2,                                          /* 15:iProduct */
    3,                                          /* 16:iSerialNumber */
    USB_CONFIGNUM                               /* 17:bNumConfigurations */
};

uint8_t g_usb_psmpl_ConfigurationF_1[USB_PVDR_BLENGTH + ( USB_PVDR_BLENGTH % 2 ) ] =
{
    9,                                      /*  0:bLength */
    USB_DT_CONFIGURATION,
    (uint8_t)(USB_PVDR_BLENGTH % 256),      /*  2:wTotalLength(L) */
    (uint8_t)(USB_PVDR_BLENGTH / 256),      /*  3:wTotalLength(H) */
    1,                                      /*  4:bNumInterfaces */
    1,                                      /*  5:bConfigurationValue */
    4,                                      /*  6:iConfiguration */
    (uint8_t)(USB_CF_RESERVED|USB_CF_SELFP|USB_CF_RWUPON),  /*  7:bmAttributes */
    (uint8_t)(100 / 2),                     /*  8:bMaxPower (2mA unit) */
        /* Interface Descriptor */
        9,                                  /*  0:bLength */
        USB_DT_INTERFACE,                   /*  1:bDescriptor */
        0,                                  /*  2:bInterfaceNumber */
        0,                                  /*  3:bAlternateSetting */
        USB_EPNUMS,                         /*  4:bNumEndpoints */
        0xff,                               /*  5:bInterfaceClass */
        0xff,                               /*  6:bInterfaceSubClass */
        0xff,                               /*  7:bInterfaceProtocol */
        5,                                  /*  8:iInterface */
#ifdef  USB_FSPERI_PP
            /* Endpoint Descriptor 1 */
            7,                              /*  0:bLength */
            USB_DT_ENDPOINT,                /*  1:bDescriptorType */
            (uint8_t)(USB_EP_OUT | USB_EP1),/*  2:bEndpointAddress */
            USB_EP_BULK,                    /*  3:bmAttribute */
            64,                             /*  4:wMaxPacketSize_lo */
            0,                              /*  5:wMaxPacketSize_hi */
            1,                              /*  6:bInterval */
            /* Endpoint Descriptor 2 */
            7,                              /*  0:bLength */
            USB_DT_ENDPOINT,                /*  1:bDescriptorType */
            (uint8_t)(USB_EP_IN | USB_EP2), /*  2:bEndpointAddress */
            USB_EP_BULK,                    /*  3:bmAttribute */
            64,                             /*  4:wMaxPacketSize_lo */
            0,                              /*  5:wMaxPacketSize_hi */
            1,                              /*  6:bInterval */
#endif  /* USB_FSPERI_PP */
            /* Endpoint Descriptor 3 */
            7,                                      /*  0:bLength */
            USB_DT_ENDPOINT,                        /*  1:bDescriptorType */
            (uint8_t)(USB_EP_OUT | USB_EP3),        /*  2:bEndpointAddress */
            USB_EP_INT,                             /*  3:bmAttribute */
            USB_INTEPMAXP,                          /*  4:wMaxPacketSize_lo */
            0,                                      /*  5:wMaxPacketSize_hi */
            4,                                      /*  6:bInterval */
            /* Endpoint Descriptor 4 */
            7,                                      /*  0:bLength */
            USB_DT_ENDPOINT,                        /*  1:bDescriptorType */
            (uint8_t)(USB_EP_IN  | USB_EP4),        /*  2:bEndpointAddress */
            USB_EP_INT,                             /*  3:bmAttribute */
            USB_INTEPMAXP,                          /*  4:wMaxPacketSize_lo */
            0,                                      /*  5:wMaxPacketSize_hi */
            5,                                      /*  6:bInterval */
};

uint8_t *g_usb_psmpl_StringPtr[USB_STRINGNUM] =
{
    g_usb_psmpl_StringDescriptor0,
    g_usb_psmpl_StringDescriptor1,
    g_usb_psmpl_StringDescriptor2,
    g_usb_psmpl_StringDescriptor3,
    g_usb_psmpl_StringDescriptor4,
    g_usb_psmpl_StringDescriptor5,
    g_usb_psmpl_StringDescriptor0
};

/* functions */


/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/
/* There is no imported global variables and functions. */



/******************************************************************************
End of file
******************************************************************************/
