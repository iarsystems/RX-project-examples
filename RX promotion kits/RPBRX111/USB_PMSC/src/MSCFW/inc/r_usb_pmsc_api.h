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
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name    : r_usb_pmsc_api.h
* Version      : 2.00
* Description  : USB Peripheral MSC Driver API declaration
*******************************************************************************/
/*******************************************************************************
* History    : DD.MM.YYYY Version Description
*            : 09.11.2012 1.00         First Release
*            : 30.11.2012 2.00
*******************************************************************************/

#ifndef __R_USB_PMSC_API_H__
#define __R_USB_PMSC_API_H__

/*****************************************************************************
Public Functions (API)
******************************************************************************/
uint16_t    R_usb_pmsc_ProcessWaitTmo(uint16_t tmo);
usb_er_t    R_usb_pmsc_Close(uint16_t data1, uint16_t data2);
usb_er_t    R_usb_pmsc_Open(uint16_t data1, uint16_t data2);
void        R_usb_pmsc_ControlTrans0(usb_request_t *req);
void        R_usb_pmsc_ControlTrans1(usb_request_t *req);
void        R_usb_pmsc_ControlTrans2(usb_request_t *req);
void        R_usb_pmsc_ControlTrans3(usb_request_t *req);
void        R_usb_pmsc_ControlTrans4(usb_request_t *req);
void        R_usb_pmsc_ControlTrans5(usb_request_t *req);
void        R_usb_pmsc_DataTrans_In(usb_pipe_t pipe, usb_leng_t size, uint8_t *table, usb_cb_t complete);
void        R_usb_pmsc_DataTrans_In_CSW(usb_pipe_t pipe, usb_leng_t size, uint8_t *table, usb_cb_t complete);
void        R_usb_pmsc_DataTrans_Out(usb_pipe_t pipe, usb_leng_t size, uint8_t *table, usb_cb_t complete);
void        R_usb_pmsc_GetMaxLun(usb_cb_t complete);
void        R_usb_pmsc_MassStrageReset(usb_cb_t complete);
void        R_usb_pmsc_Registration(void);
void        R_usb_pmsc_SetConfig(void);
void        R_usb_pmsc_SetInterface(uint16_t data1, uint16_t data2);
void        R_usb_pmsc_StallClearResult(usb_utr_t *ptr, uint16_t data, uint16_t dummy);
void        R_usb_pmsc_TransResult(usb_utr_t *mess);
void        R_usb_pmsc_driver_start(void);


#endif  /* __R_USB_PMSC_API_H__ */
/******************************************************************************
End  Of File
******************************************************************************/
