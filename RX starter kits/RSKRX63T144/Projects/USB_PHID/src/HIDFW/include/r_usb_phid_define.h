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
* Copyright (C) 2010(2011) Renesas Electronics Corporation
* and Renesas Solutions Corp. All rights reserved.
*******************************************************************************
* File Name    : r_usb_phid_define.h
* Version      : 1.10
* Device(s)    : Renesas
* Tool-Chain   : Renesas
* OS           : Common to None and uITRON
* H/W Platform : Independent
* Description  : USB User definition
******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 17.03.2010 0.80    First Release
*         : 30.07.2010 0.90    Updated comments
*         : 31.03.2011 1.00    Peripheral HID Class(Mass Production Release)
*         : 24.06.2011 1.10    Version 1.10 Release
******************************************************************************/

#ifndef __R_USB_PHID_DEFINE_H__
#define __R_USB_PHID_DEFINE_H__

/*****************************************************************************
Macro definitions
******************************************************************************/
//----- HID Mouse Class Request Defines --------
/* HID Class Request codes */
#define USB_GET_REPORT                  0x0100u
#define USB_GET_IDLE                    0x0200u
#define USB_GET_PROTOCOL                0x0300u
#define USB_SET_IDLE                    0x0A00u
#define USB_SET_REPORT                  0x0900u
#define USB_SET_PROTOCOL                0x0B00u

/* PHID reception message type definition */
#define USB_GET_REPORT_DESCRIPTOR       0x2200u
#define USB_GET_HID_DESCRIPTOR          0x2100u
#define USB_SET_REPORT_STATUS           0x3399u

/* ----- DESCRIPTOR Types ----- */
#define USB_DT_TYPE_GETREPORT           (uint8_t)0x01   /* Report Type Input */
#define USB_DT_TYPE_SETREPORT           (uint8_t)0x02   /* Report Type Output */
#define USB_DT_TYPE_HIDDESCRIPTOR       (uint8_t)0x21   /* HID descriptor type */
#define USB_DT_TYPE_RPTDESCRIPTOR       (uint8_t)0x22   /* Report descriptor type */

/* ----- Subclass Codes ----- */
#define USB_IFSUB_NOBOOT                (uint8_t)0x00   /* No Subclass */
#define USB_IFSUB_BOOT                  (uint8_t)0x01   /* Boot Interface Subclass */

/* ----- Protocol Codes ----- */
#define USB_IFPRO_DEM                   (uint8_t)0x00   /* Demonstration */
#define USB_IFPRO_KBD                   (uint8_t)0x01   /* Keyboard */
#define USB_IFPRO_MSE                   (uint8_t)0x02   /* Mouse */

/* Report Type set */
#define SET_DT_TYPE(v)                  ((v) << 8)

/* Peripheral HID Transfer Pipe */
#define USB_PHID_USE_PIPE               USB_PIPE6

/* Peripheral HID Max Packet Size */
#define USB_PHID_MXPS_DEMO              5u
#define USB_PHID_MXPS_KBD               8u
#define USB_PHID_MXPS_MSE               3u

#define USB_SEND_DATA                   0x1111u

/******************************************************************************
Typedef definitions
******************************************************************************/

#endif  /* #ifndef __R_USB_PHID_DEFINE_H__ */
/******************************************************************************
End  Of File
******************************************************************************/
