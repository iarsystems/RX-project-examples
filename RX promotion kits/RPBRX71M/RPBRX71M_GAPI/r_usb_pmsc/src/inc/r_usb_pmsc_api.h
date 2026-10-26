/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2014(2015) Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name     : r_usb_pmsc_api.h
* Description   : USB Peripheral MSC Driver API declaration
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 04.01.2014 1.00 First Release
*         : 30.01.2015 1.01    Added RX71M.
*         : 30.09.2015 1.11    Added RX63N.
***********************************************************************************************************************/

#ifndef __R_USB_PMSC_API_H__
#define __R_USB_PMSC_API_H__

/*****************************************************************************
Public Functions (API)
******************************************************************************/
uint16_t    R_usb_pmsc_ProcessWaitTmo(uint16_t tmo);
USB_ER_t    R_usb_pmsc_Close(USB_UTR_t *ptr, uint16_t data1, uint16_t data2);
USB_ER_t    R_usb_pmsc_Open(USB_UTR_t *ptr, uint16_t data1, uint16_t data2);
void        R_usb_pmsc_ControlTrans0(USB_UTR_t *ptr, USB_REQUEST_t *req);
void        R_usb_pmsc_ControlTrans1(USB_UTR_t *ptr, USB_REQUEST_t *req);
void        R_usb_pmsc_ControlTrans2(USB_UTR_t *ptr, USB_REQUEST_t *req);
void        R_usb_pmsc_ControlTrans3(USB_UTR_t *ptr, USB_REQUEST_t *req);
void        R_usb_pmsc_ControlTrans4(USB_UTR_t *ptr, USB_REQUEST_t *req);
void        R_usb_pmsc_ControlTrans5(USB_UTR_t *ptr, USB_REQUEST_t *req);
void        R_usb_pmsc_DataTrans(USB_UTR_t *ptr, uint16_t pipe, uint32_t size, uint8_t *table, USB_CB_t complete);
void        R_usb_pmsc_GetMaxLun(USB_UTR_t *ptr, USB_CB_t complete);
void        R_usb_pmsc_MassStrageReset(USB_UTR_t *ptr, USB_CB_t complete);
void        R_usb_pmsc_SetConfig(USB_UTR_t *ptr);
void        R_usb_pmsc_SetInterface(USB_UTR_t *ptr, uint16_t data1, uint16_t data2);
void        R_usb_pmsc_StallClearResult(USB_UTR_t *ptr, uint16_t data,uint16_t dummy);
void        R_usb_pmsc_StrgTaskClose(USB_UTR_t *ptr);
void        R_usb_pmsc_StrgTaskOpen(USB_UTR_t *ptr);
void        R_usb_pmsc_TransResult(USB_UTR_t *mess, uint16_t data1, uint16_t data2 );

#endif  /* __R_USB_PMSC_API_H__ */
/******************************************************************************
End  Of File
******************************************************************************/
