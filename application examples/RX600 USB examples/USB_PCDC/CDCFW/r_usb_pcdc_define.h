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
* Copyright (C) 2010(2011) Renesas Electronics Corpration
* and Renesas Solutions Corp. All rights reserved.
******************************************************************************
* File Name    : r_usb_pcdc_define.h
* Version      : 1.10
* Device(s)    : Renesas SH-Series, RX-Series
* Tool-Chain   : Renesas SuperH RISC engine Standard Toolchain
*              : Renesas RX Standard Toolchain
* OS           : Common to None and uITRON 4.0 Spec
* H/W Platform : Independent
* Description  : USB Host and Peripheral Interrupt code
******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 06.08.2010 0.91    First Release
*         : 31.08.2010 0.92    PCDC Updated
*         : 02.12.2010 0.96    Spelling modification
*         : 02.12.2010 0.96    Unnecessary Code Delete
*         : 01.06.2011 1.10    Version 1.10 Release
******************************************************************************/
#ifndef __R_USB_PCDC_DEFINE_H__
#define __R_USB_PCDC_DEFINE_H__

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_usb_ctypedef.h"         /* Type define */


/*****************************************************************************
Macro definitions
******************************************************************************/
/* CDC Class Requests IDs*/
#define USB_PCDC_SET_LINE_CODING            0x2000
#define USB_PCDC_GET_LINE_CODING            0x2100
#define USB_PCDC_SET_CONTROL_LINE_STATE 0x2200
#define USB_PCDC_SEND_BREAK             0x2300

/* Class-Specific Configuration Descriptors */
#define     USB_PCDC_CS_INTERFACE                               0x24

/* bDescriptor SubType in Communications Class Functional Descriptors */
/* Header Functional Descriptor */
#define     USB_PCDC_DT_SUBTYPE_HEADER_FUNC                 0x00
/* Call Management Functional Descriptor. */
#define     USB_PCDC_DT_SUBTYPE_CALL_MANAGE_FUNC                0x01
/* Abstract Control Management Functional Descriptor. */
#define     USB_PCDC_DT_SUBTYPE_ABSTRACT_CTR_MANAGE_FUNC        0x02
/* Union Functional Descriptor */
#define     USB_PCDC_DT_SUBTYPE_UNION_FUNC                      0x06

/* Communications Class Subclass Codes */
#define     USB_PCDC_CLASS_SUBCLASS_CODE_ABS_CTR_MDL            0x02

/* USB Class Definitions for Communications Devices Specification 
   release number in binary-coded decimal. */
#define     USB_PCDC_BCD_CDC                                    0x0110

/* GET_LINE_CODING request wLength */
#define     USB_PCDC_GET_LINE_CODING_LENGTH                 0x07

/* Descriptor length */
#define USB_PCDC_DD_BLENGTH     18
#define USB_PCDC_QD_BLENGTH     10
#define USB_PCDC_CD1_BLENGTH        67
#define USB_PCDC_SD0_BLENGTH        4
#define USB_PCDC_SD1_BLENGTH        16
#define USB_PCDC_SD2_BLENGTH        44
#define USB_PCDC_SD3_BLENGTH        46
#define USB_PCDC_SD4_BLENGTH        22
#define USB_PCDC_SD5_BLENGTH        18
#define USB_PCDC_SD6_BLENGTH        28

/*****************************************************************************
Enumerated Types
******************************************************************************/
/* USB Communications Devices Device Class Phase. */
enum usb_gpcdc_Phase
{
    /* Class Request SET_LINE_CODING */
    USB_PCDC_PSET_LINE_CODING               = 0x10,
    /* Class Request GET_LINE_CODING */
    USB_PCDC_PGET_LINE_CODING               = 0x11,
    /* Class Request SET_CONTROL_LINE_STATE */
    USB_PCDC_PSET_CONTROL_LINE_STATE        = 0x12,
    /* Class Request SEND_BREAK */
    USB_PCDC_PSEND_BREAK                    = 0x13,
    /* Send Data ( Device to Host ) */
    USB_PCDC_PDATASND                       = 0x14,
    /* Receive Data ( Host to Device ) */
    USB_PCDC_PDATARCV                       = 0x15,
    /* Data send request for USB Host */
    USB_PCDC_PSEND_DATA                 = 0x17,
    /* Data receive request for USB Host */
    USB_PCDC_PRECEIVE_DATA                  = 0x18,
    /* Class Notification Serial State */
    USB_PCDC_PSERIAL_STATE_NOTIFICATION = 0x19,
    /* Send Serial Status ( Device to Host ) */
    USB_PCDC_PSTATUSSND                 = 0x20,
};

/******************************************************************************
Typedef definitions
******************************************************************************/
typedef struct
{
    /* Data terminal rate, in bits per second */
    uint32_t                    dwDTERate;
    /* Stop bits */
    uint8_t                     bCharFormat;
    /* Parity */
    uint8_t                     bParityType;
    /* Data bits */
    uint8_t                     bDataBits;
    /* reserve */
    uint8_t                     rsv;
}
USB_PCDC_LineCoding_t;

typedef struct
{
    /* reserve */
    uint16_t                    rsv:14;
    /* RTS */
    uint16_t                    bRTS:1;
    /* DTR */
    uint16_t                    bDTR:1;
}
USB_PCDC_ControlLineState_t;

#endif  /* __R_USB_PCDC_DEFINE_H__ */
/******************************************************************************
End  Of File
******************************************************************************/
