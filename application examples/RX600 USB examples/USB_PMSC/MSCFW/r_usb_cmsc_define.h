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
* Copyright (C) 2010 Renesas Electronics Corporation. All rights reserved.
******************************************************************************
* File Name    : r_usb_cMSCdefine.h
* Version      : 1.00
* Device(s)    : Renesas SH-Series, RX-Series
* Tool-Chain   : Renesas SuperH RISC engine Standard Toolchain
*              : Renesas RX Standard Toolchain
* OS           : Common to None and uITRON 4.0 Spec
* H/W Platform : Independent
* Description  : USB Host and Peripheral Interrupt code
******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 04.02.2010 1.00    First Release
*         : 02.08.2010 0.91    Updated comments
*         : 29.10.2010 1.00    Mass Production Release
******************************************************************************/
#ifndef __R_USB_CMSC_DEFINE_H__
#define __R_USB_CMSC_DEFINE_H__
#pragma bitfields=reversed

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_usb_ctypedef.h"         /* Type define */

/*****************************************************************************
Macro definitions
******************************************************************************/
/* Media size */
#ifdef      USB_SDRAM_USE_PP
#define     USB_PMSC_MEDIASIZE          (8ul * 1024ul * 1024ul) /*  8Mbyte */
#else
#define     USB_PMSC_MEDIASIZE          (32ul * 1024ul)         /*  32Kbyte */
#endif  /* USB_SDRAM_USE_PP */

/* CBW Structure define */
#define     USB_MSC_CBWLENGTH           31
#define     USB_MSC_CBWCB_LENGTH        12
#define     USB_MSC_CSW_LENGTH          13

/* Condition compilation by the difference of the endian */
#if USB_CPUBYTE_PP == USB_BYTE_BIG_PP
 #define    USB_MSC_CBW_SIGNATURE       (uint32_t)0x55534243
 #define    USB_MSC_CSW_SIGNATURE       (uint32_t)0x55534253
#else   /* USB_CPUBYTE_PP == USB_BYTE_BIG_PP */
 #define    USB_MSC_CBW_SIGNATURE       (uint32_t)0x43425355
 #define    USB_MSC_CSW_SIGNATURE       (uint32_t)0x53425355
#endif  /* USB_CPUBYTE_PP == USB_BYTE_BIG_PP */

/* Sub_class code */
#define     USB_ATAPI                   (uint8_t)0x05
#define     USB_SCSI                    (uint8_t)0x06
#define     USB_BOTP                    (uint8_t)0x50
#define     USB_TOTALEP                 (uint8_t)0x02

/*****************************************************************************
Enumerated Types
******************************************************************************/

/*************** enumeration define ***********************/
/* USB Mass Storage Device Class Request code define. */
enum usb_gpmsc_Request
{
    USB_MASS_STORAGE_RESET  = 0xFF00,   /* Mass dtorage reset request */
    USB_GET_MAX_LUN         = 0xFE00,   /* Get max logical unit number */
};

/* GET_MAX_LUN request check code. */
enum usb_gpmsc_LunReqChk
{
    USB_MSC_LUN_LENGTH      = 0x01,     /* GET_MAX_LUN request wLength */
};

/* USB Mass Storage Devie Class Lapper check. */
enum usb_gpmsc_Case13check
{
    /* Device No Data */
    USB_MSC_DNXX    = 0x10,
    /* Device Send(IN) Data */
    USB_MSC_DIXX    = 0x20,
    /* Device Recieved(OUT) Data */
    USB_MSC_DOXX    = 0x30,
    /* Host No Data */
    USB_MSC_XXHN    = 0x01,
    /* Host Recieved(IN) Data */
    USB_MSC_XXHI    = 0x02,
    /* Host Send(OUT) Data */
    USB_MSC_XXHO    = 0x03,
    /* Device No Data & Host No Data */
    USB_MSC_DNHN    = 0x11,
    /* Device No Data & Host Recieved(IN) Data */
    USB_MSC_DNHI    = 0x12,
    /* Device No Data & Host Send(OUT) Data */
    USB_MSC_DNHO    = 0x13,
    /* Device Send(IN) Data & Host No Data */
    USB_MSC_DIHN    = 0x21,
    /* Device Send(IN) Data & Host Recieved(IN) Data */
    USB_MSC_DIHI    = 0x22,
    /* Device Send(IN) Data & Host Send(OUT) Data */
    USB_MSC_DIHO    = 0x23,
    /* Device Recieved(OUT) Data & Host No Data */
    USB_MSC_DOHN    = 0x31,
    /* Device Recieved(OUT) Data & Host Recieved(IN) Data */
    USB_MSC_DOHI    = 0x32,
    /* Device Recieved(OUT) Data & Host Send(OUT) Data */
    USB_MSC_DOHO    = 0x33,
};

/* USB Mass Storage Devie Class Lapper check. */
enum usb_gpmsc_Case13nun
{
    USB_MSC_CASE00  = 0x00, /* CBW check case00(Not Use) */
    USB_MSC_CASE01  = 0x01, /* CBW check case01 */
    USB_MSC_CASE02  = 0x02, /* CBW check case02 */
    USB_MSC_CASE03  = 0x03, /* CBW check case03 */
    USB_MSC_CASE04  = 0x04, /* CBW check case04 */
    USB_MSC_CASE05  = 0x05, /* CBW check case05 */
    USB_MSC_CASE06  = 0x06, /* CBW check case06 */
    USB_MSC_CASE07  = 0x07, /* CBW check case07 */
    USB_MSC_CASE08  = 0x08, /* CBW check case08 */
    USB_MSC_CASE09  = 0x09, /* CBW check case09 */
    USB_MSC_CASE10  = 0x10, /* CBW check case10 */
    USB_MSC_CASE11  = 0x11, /* CBW check case11 */
    USB_MSC_CASE12  = 0x12, /* CBW check case12 */
    USB_MSC_CASE13  = 0x13, /* CBW check case13 */
};

/* CSW STATUS */
enum usb_gcmsc_CswSts
{
    USB_MSC_CSW_OK          = (uint16_t)0x00,
    USB_MSC_CSW_NG          = (uint16_t)0x01,
    USB_MSC_CSW_PHASE_ERR   = (uint16_t)0x02
};

/******************************************************************************
Typedef definitions
******************************************************************************/

/* CBW Structure define.    */
typedef struct
{
    uint32_t    dCBWSignature;
    uint32_t    dCBWTag;
    uint8_t     dCBWDTL_Lo;
    uint8_t     dCBWDTL_ML;
    uint8_t     dCBWDTL_MH;
    uint8_t     dCBWDTL_Hi;
    struct
    {
/* Condition compilation by the difference of the bit order */
        uint8_t CBWdir:1;
        uint8_t reserved7:7;
    } bmCBWFlags;
    struct
    {
/* Condition compilation by the difference of the bit order */
        uint8_t reserved4:4;
        uint8_t bCBWLUN:4;
    } bCBWLUN;
    struct
    {
/* Condition compilation by the difference of the bit order */
        uint8_t reserved3:3;
        uint8_t bCBWCBLength:5;
    } bCBWCBLength;
#if USB_TARGET_CHIP_PP == USB_RX600_PP
    uint8_t CBWCB[(16 + 33)];
#else   /* USB_TARGET_CHIP_PP == USB_RX600_PP */
    uint8_t CBWCB[(16 + 481)];
#endif  /* USB_TARGET_CHIP_PP == USB_RX600_PP */
} USB_MSC_CBW_t;

/* CSW Structure define define. */
typedef struct
{
    uint32_t    dCSWSignature;
    uint32_t    dCSWTag;
    uint8_t     dCSWDataResidue_Lo;
    uint8_t     dCSWDataResidue_ML;
    uint8_t     dCSWDataResidue_MH;
    uint8_t     dCSWDataResidue_Hi;
    uint8_t     bCSWStatus;
    uint8_t     dummy;
} USB_MSC_CSW_t;

#pragma bitfields=default
#endif  /* __R_USB_CMSC_DEFINE_H__ */
/******************************************************************************
End  Of File
******************************************************************************/
