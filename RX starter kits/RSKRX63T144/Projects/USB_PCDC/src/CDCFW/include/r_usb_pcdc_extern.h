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
******************************************************************************
* File Name    : r_usb_pCDCextern.h
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
*         : 30.09.2010 0.93    PCDC Updated
*         : 02.12.2010 0.96    Unnecessary Code Delete
*         : 01.06.2011 1.10    Version 1.10 Release
******************************************************************************/
#ifndef __R_USB_PCDC_EXTERN_H__
#define __R_USB_PCDC_EXTERN_H__


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_usb_pcdc_define.h"

/*****************************************************************************
Macro definitions
******************************************************************************/


/*****************************************************************************
Enumerated Types
******************************************************************************/


/******************************************************************************
Typedef definitions
******************************************************************************/


/******************************************************************************
External variables and functions
******************************************************************************/

extern uint16_t         *usb_gpcdc_EpPtr[];
extern uint16_t         usb_gpcdc_EpTbl1[];
extern uint16_t         usb_gpcdc_EpTbl2[];
extern uint16_t         usb_gpcdc_EpTbl3[];
extern uint16_t         usb_gpcdc_EpTbl4[];
extern uint16_t         usb_gpcdc_EpTbl5[];
extern uint8_t          *usb_gpcdc_ConPtr[];
extern uint8_t          *usb_gpcdc_ConPtrOther[];
extern uint8_t          *usb_gpcdc_StrPtr[];
extern uint8_t          usb_gpcdc_ConfigrationF1[];
extern uint8_t          usb_gpcdc_ConfigrationH1[];
extern uint8_t          usb_gpcdc_DeviceDescriptor[];
extern uint8_t          usb_gpcdc_QualifierDescriptor[];
extern uint8_t          usb_gpcdc_StringDescriptor0[];
extern uint8_t          usb_gpcdc_StringDescriptor1[];
extern uint8_t          usb_gpcdc_StringDescriptor2[];
extern uint8_t          usb_gpcdc_StringDescriptor3[];
extern uint8_t          usb_gpcdc_StringDescriptor4[];
extern uint8_t          usb_gpcdc_StringDescriptor5[];
extern uint8_t          usb_gpcdc_StringDescriptor6[];
extern USB_UTR_t        usb_gpcdc_Mess[];

/* Line Coding data */
extern USB_PCDC_LineCoding_t    usb_gcdc_LineCoding;
/* Line Coding Set Request data */
extern USB_PCDC_LineCoding_t    usb_gcdc_LineCodingReq;

#if USB_ANSIIO_PP != USB_ANSIIO_USE_PP
extern uint16_t         usb_gpcdc_Inpipe;
extern uint16_t         usb_gpcdc_Outpipe;
extern uint16_t         usb_gpcdc_Status_pipe;

#endif  /* USB_ANSIIO_PP != USB_ANSIIO_USE_PP */

USB_ER_t    usb_pcdc_close(USB_UTR_t *ptr, uint16_t data1, uint16_t data2);
USB_ER_t    usb_pcdc_open(USB_UTR_t *ptr, uint16_t data1, uint16_t data2);
void        usb_pcdc_ControlTrans0(USB_UTR_t *ptr, USB_REQUEST_t *req);
void        usb_pcdc_ControlTrans1(USB_UTR_t *ptr, USB_REQUEST_t *req);
void        usb_pcdc_ControlTrans2(USB_UTR_t *ptr, USB_REQUEST_t *req);
void        usb_pcdc_ControlTrans3(USB_UTR_t *ptr, USB_REQUEST_t *req);
void        usb_pcdc_ControlTrans4(USB_UTR_t *ptr, USB_REQUEST_t *req);
void        usb_pcdc_ControlTrans5(USB_UTR_t *ptr, USB_REQUEST_t *req);
void        usb_pcdc_DataTrans(USB_UTR_t *ptr, uint16_t Pipe, uint32_t Size, uint8_t *Table, USB_CB_t complete);
void        usb_pcdc_StatusTransResult(USB_UTR_t *mess, uint16_t data1, uint16_t data2);
void        usb_pcdc_task(USB_VP_INT_t);


void        usb_pcdc_SetControlLineState_req(USB_UTR_t *ptr);
void        usb_pcdc_SetLineCoding_req(USB_UTR_t *ptr);
void        usb_pcdc_GetLineCoding_req(USB_UTR_t *ptr);
void        usb_pcdc_SendBreak_req(USB_UTR_t *ptr);
uint16_t    usb_pcdc_is_connected(void);

void        usb_pcdc_SetControlLineState(USB_UTR_t *ptr, USB_CB_INFO_t complete);
void        usb_pcdc_SetLineCoding(USB_UTR_t *ptr, USB_CB_INFO_t complete);
void        usb_pcdc_GetLineCoding(USB_UTR_t *ptr, USB_CB_INFO_t complete);
void        usb_pcdc_SendBreak(USB_UTR_t *ptr, USB_CB_INFO_t complete);

void        usb_pcdc_StatusTransResult(USB_UTR_t *mess, uint16_t data1, uint16_t data2);
void        usb_pcdc_InTransResult(USB_UTR_t *mess, uint16_t data1, uint16_t data2);
void        usb_pcdc_OutTransResult(USB_UTR_t *mess, uint16_t data1, uint16_t data2);

#endif  /* __R_USB_PCDC_EXTERN_H__ */
/******************************************************************************
End  Of File
******************************************************************************/
