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
* File Name    : r_usb_phid_extern.h
* Version      : 1.00
* Device(s)    : Renesas SH-Series, RX-Series
* Tool-Chain   : Renesas SuperH RISC engine Standard Toolchain
*              : Renesas RX Standard Toolchain
* OS           : Common to None and uITRON 4.0 Spec
* H/W Platform : Independent
* Description  : USB common uItron header
******************************************************************************
* History : DD.MM.YYYY Version Description
*         : xx.xx.xxxx x.xx    
******************************************************************************/
#ifndef __R_USB_PHID_EXTERN_H__
#define __R_USB_PHID_EXTERN_H__


/*****************************************************************************
Public Valiables
******************************************************************************/
extern uint16_t         usb_gphid_EpTbl[];
extern uint8_t          usb_gphid_SmplDeviceDescriptor[];
extern uint8_t          usb_gphid_SmplQualifierDescriptor[];
extern uint8_t          usb_gphid_Configuration[];
extern uint8_t          usb_gphid_StringDescriptor0[];
extern uint8_t          usb_gphid_StringDescriptor1[];
extern uint8_t          usb_gphid_StringDescriptor2[];
extern uint8_t          usb_gphid_StringDescriptor3[];
extern uint8_t          usb_gphid_StringDescriptor4[];
extern uint8_t          usb_gphid_StringDescriptor5[];
extern uint8_t          usb_gphid_StringDescriptor6[];
extern uint16_t         *usb_gphid_EpPtr[];
extern uint8_t          *usb_gphid_ConPtr[];
extern uint8_t          *usb_gphid_StrPtr[];

#if USB_PHID_APL_MODE == USB_PHID_DEMO_MODE
extern uint8_t          usb_gphid_ReportDiscriptor[34];
#endif  /* Peripheral HID Application demo mode */

#if USB_PHID_APL_MODE == USB_PHID_KEYBOARD_MODE
extern uint8_t          usb_gphid_ReportDiscriptor[64];
#endif  /* Peripheral HID Application keyboard mode */

#if USB_PHID_APL_MODE == USB_PHID_MOUSE_MODE
extern uint8_t          usb_gphid_ReportDiscriptor[50];
#endif  /* Peripheral HID Application mouse mode */


extern uint8_t          usb_gphid_TransData[];

/*****************************************************************************
Public Functions
******************************************************************************/
void        usb_phid_DriverOpen(uint16_t data1, uint16_t data2);
void        usb_phid_DriverClose(uint16_t data1, uint16_t data2);
void        usb_phid_ControlMessage(USB_UTR_t *ptr, uint16_t msginfo);
void        usb_phid_ControlRequest0(USB_UTR_t *ptr);
void        usb_phid_ControlRequest1(USB_UTR_t *ptr);
void        usb_phid_ControlRequest2(USB_UTR_t *ptr);
void        usb_phid_ControlRequest3(USB_UTR_t *ptr);
void        usb_phid_ControlRequest4(USB_UTR_t *ptr);
void        usb_phid_ControlRequest5(USB_UTR_t *ptr);
void        usb_phid_ControlGetReport1(USB_UTR_t *ptr);
void        usb_phid_ControlGetIdle1(USB_UTR_t *ptr);
void        usb_phid_ControlSetReport2(USB_UTR_t *ptr);
void        usb_phid_ControlSetIdle3(USB_UTR_t *ptr);
uint16_t    usb_phid_get_pipedir(uint16_t *tbl_p, uint16_t pipeno);




/*****************************************************************************
Public Functions (API)
******************************************************************************/
void        R_usb_phid_task(USB_VP_INT_t stacd);
void        R_usb_phid_DriverRelease(void);
void        R_usb_phid_SetTaskPri(void);
void        R_usb_phid_TransferEnd(USB_UTR_t *ptr);
void        R_usb_phid_DeviceInformation(USB_UTR_t *ptr, uint16_t *deviceinfo);
void        R_usb_phid_ChangeDeviceState(USB_UTR_t *ptr, uint16_t msginfo);
void        R_usb_phid_driver_start(USB_UTR_t *ptr);
uint32_t    R_usb_phid_transfer_length(void);
void        R_usb_phid_control_transfer(USB_UTR_t *ptr, USB_REQUEST_t *request, uint16_t ctsq);
void        R_usb_phid_send_data( USB_UTR_t *ptr, uint8_t *Table, uint32_t size, USB_CB_t complete );

#endif  /* __R_USB_PHID_EXTERN_H__ */
/******************************************************************************
End Of File
******************************************************************************/
