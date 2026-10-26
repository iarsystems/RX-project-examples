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
* File Name    : r_usb_hhid_define.h
* Version      : 1.02
* Device(s)    : Renesas
* Tool-Chain   : Renesas
* OS           : Common to None and uITRON
* H/W Platform : Independent
* Description  : USB Host HID class definition
******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 17.03.2010 0.80    First Release
*         : 30.07.2010 0.90    Updated comments
*         : 31.01.2011 1.00    Host HID Class(Mass Production Release)
*         : 17.06.2011 1.02    Support USB Basic Fw V110
******************************************************************************/

#ifndef __R_USB_HHID_DEFINE_H__
#define __R_USB_HHID_DEFINE_H__

/*****************************************************************************
Macro definitions
******************************************************************************/
/* ----- HID Mouse Class Request Defines -------- */
/* HID Class Request IDs */
/* It is used by the classification judging of a class request. */
#define USB_HID_GET_REPORT                  0x0100u
#define USB_HID_GET_IDLE                    0x0200u
#define USB_HID_GET_PROTOCOL                0x0300u
#define USB_HID_SET_IDLE                    0x0A00u
#define USB_HID_SET_REPORT                  0x0900u
#define USB_HID_SET_PROTOCOL                0x0B00u
#define USB_HID_GET_HID_DESCRIPTOR          0x2100u
#define USB_HID_GET_REPORT_DESCRIPTOR       0x2200u
#define USB_HID_GET_PHYSICAL_DESCRIPTOR     0x2300u

/* ----- DESCRIPTOR Types ----- */
#define USB_HID_DT_TYPE_GETREPORT           (uint8_t)0x01   /* Report Type Input */
#define USB_HID_DT_TYPE_SETREPORT           (uint8_t)0x02   /* Report Type Output */
#define USB_HID_DT_TYPE_HIDDESCRIPTOR       (uint8_t)0x21   /* HID descriptor type */
#define USB_HID_DT_TYPE_RPTDESCRIPTOR       (uint8_t)0x22   /* Report descriptor type */

/* ----- Subclass Codes ----- */
#define USB_HID_IFSUB_NONE                  (uint8_t)0x00   /* No Subclass */
#define USB_HID_IFSUB_BOOT                  (uint8_t)0x01   /* Boot Interface Subclass */

/* ----- Protocol Codes ----- */
#define USB_HHID_IFPRO_NONE                 (uint8_t)0x00
#define USB_HHID_IFPRO_KBD                  (uint8_t)0x01   /* Keyboard */
#define USB_HHID_IFPRO_MSE                  (uint8_t)0x02   /* Mouse */

/* Report Type set */
#define USB_HID_SET_DT_TYPE(v)              ((v) << 8)

/* Host HID Transfer Pipe */
#define USB_HHID_USE_PIPE                   USB_PIPE6

#define USB_HHID_CLSDATASIZE        512

/*****************************************************************************
Enumerated Types
******************************************************************************/
/* Host HID Task Command */
typedef enum 
{
    USB_HHID_TCMD_OPEN,
    USB_HHID_TCMD_SEND,
    USB_HHID_TCMD_RECEIVE,
    USB_HHID_TCMD_CLASS_REQ,
    USB_HHID_TCMD_DATA_TRANS
} USB_HHID_TCMD_t;

/* Enumeration Sequence */
typedef enum
{
    /* Enumeration Sequence Complete */
    USB_HHID_ENUM_COMPLETE = 0,
    /* Enumeration Sequence String Descriptor #0 receive request */
    USB_HHID_ENUM_STR_DT0_REQ,
    /* Enumeration Sequence String Descriptor #0 Receive complete */
    USB_HHID_ENUM_STR_DT0_WAIT,
    /* StringDescriptor iProduct Receive complete */
    USB_HHID_ENUM_STR_IPRODUCT_WAIT
} USB_HHID_ENUM_SEQ_t;

/*****************************************************************************
Struct definition
******************************************************************************/
typedef struct 
{
    uint16_t                        devadr;
    USB_REGADR_t                    ipp;            /* IP Address(USB0orUSB1)*/
    uint16_t                        ip;             /* IP number(0or1) */
    uint16_t                        bRequestCode;
    void                            *tranadr;       /* Transfer data Start address */
    uint32_t                        tranlen;        /* Transfer data length */
    uint16_t                        index;          
    uint16_t                        duration;
    uint8_t                         set_protocol;   
    USB_CB_t                        complete;       /* Call Back Function Info */
} USB_HHID_CLASS_REQUEST_PARM_t;


#endif  /* #ifndef __R_USB_HHID_DEFINE_H__ */
/******************************************************************************
End  Of File
******************************************************************************/
