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
* File Name    : r_usb_hMSCextern.h
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
#ifndef __R_USB_HHID_EXTERN_H__
#define __R_USB_HHID_EXTERN_H__


/*****************************************************************************
Public Valiables
******************************************************************************/
/* Pipe Information Table (or "endpoint table"). */
extern uint16_t         usb_ghhid_SmplEpTbl[];
extern const uint16_t   usb_gapl_devicetpl[];
/* Host HID Class Request Parametor */
extern USB_HHID_CLASS_REQUEST_PARM_t   usb_ghhid_class_request_parm;

/*****************************************************************************
Public Functions
******************************************************************************/
void        usb_hhid_task(USB_VP_INT stacd);
uint16_t    usb_hhid_pipe_info(uint8_t *table, uint16_t speed, uint16_t length, uint16_t count);
void        usb_hhid_hub_registration(USB_UTR_t *ptr);

uint16_t    usb_hhid_get_string_desc(USB_UTR_t *ptr, uint16_t addr, uint16_t string, USB_CB_t complete);

/*****************************************************************************
Public Functions (API)
******************************************************************************/

/*****************************************************************************
Public Functions(ANSI)
******************************************************************************/
int32_t     usb_hhid_read_data( int16_t fn, void *buf, uint32_t length );
int32_t     usb_hhid_write_data( int16_t fn, void *buf, uint32_t length );

#endif  /* __R_USB_HHID_EXTERN_H__ */
/******************************************************************************
End  Of File
******************************************************************************/
