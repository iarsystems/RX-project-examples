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
* File Name    : r_usb_cATAPIdefine.h
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
#ifndef __R_USB_PMSC_EXTERN_H__
#define __R_USB_PMSC_EXTERN_H__


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_usb_catapi_define.h"        /* Peripheral ATAPI Device extern define */


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

extern uint16_t         *usb_gpmsc_EpPtr[];
extern uint16_t         usb_gpmsc_EpTbl1[];
extern uint16_t         usb_gpmsc_EpTbl2[];
extern uint16_t         usb_gpmsc_EpTbl3[];
extern uint16_t         usb_gpmsc_EpTbl4[];
extern uint16_t         usb_gpmsc_EpTbl5[];
extern uint16_t         usb_gpmsc_Inpipe;
extern uint16_t         usb_gpmsc_Outpipe;
extern uint32_t         usb_gpmsc_Dtl;
extern uint32_t         usb_gpmsc_Tag;
extern uint8_t          *usb_gpmsc_ConPtr[];
extern uint8_t          *usb_gpmsc_ConPtrOther[];
extern uint8_t          *usb_gpmsc_StrPtr[];
extern uint8_t          usb_gpmsc_CbwCbLength;
extern uint8_t          usb_gpmsc_ConfigrationF1[];
extern uint8_t          usb_gpmsc_ConfigrationH1[];
extern uint8_t          usb_gpmsc_DeviceDescriptor[];
extern uint8_t          usb_gpmsc_InterfaceSubClass[];
extern uint8_t          usb_gpmsc_QualifierDescriptor[];
extern uint8_t          usb_gpmsc_Seq;
extern uint8_t          usb_gpmsc_StringDescriptor0[];
extern uint8_t          usb_gpmsc_StringDescriptor1[];
extern uint8_t          usb_gpmsc_StringDescriptor2[];
extern uint8_t          usb_gpmsc_StringDescriptor3[];
extern uint8_t          usb_gpmsc_StringDescriptor4[];
extern uint8_t          usb_gpmsc_StringDescriptor5[];
extern uint8_t          usb_gpmsc_StringDescriptor6[];
extern USB_MSC_CBW_t    usb_gpmsc_Cbw;
extern USB_MSC_CSW_t    usb_gpmsc_Csw;
extern USB_PMSC_CBM_t   usb_gpmsc_Message;
extern USB_UTR_t        usb_gpmsc_Mess;
extern uint8_t          usb_gpmsc_MediaArea[];

extern void usb_cstd_SetStall(USB_UTR_t *ptr, uint16_t pipe);

uint8_t     usb_pmsc_CheckCase13(uint32_t ul_size, uint8_t *uc_case);
uint8_t     usb_pmsc_CheckMeaning(uint8_t seq);
uint8_t     usb_pmsc_CheckValid(uint32_t length);
uint8_t     usb_pmsc_CommandCheck(uint8_t seq);
uint8_t     usb_pmsc_TransferMatrix(uint8_t uc_pmsc_case);
USB_ER_t    usb_pmsc_SmpAtapiInitMedia(void);
void        usb_pmsc_AtapiTransResult(USB_UTR_t *mess, uint16_t data1, uint16_t data2 );
void        usb_pmsc_CswSet(uint8_t ar_resp, uint32_t ul_size);
void        usb_pmsc_Error0(USB_UTR_t *ptr);
void        usb_pmsc_Error1(USB_UTR_t *ptr);
void        usb_pmsc_Error2(USB_UTR_t *ptr);
void        usb_pmsc_Error3(USB_UTR_t *ptr);
void        usb_pmsc_Error4(USB_UTR_t *ptr);
void        usb_pmsc_Error5(USB_UTR_t *ptr);
void        usb_pmsc_GetMaxLun(USB_UTR_t *ptr, uint16_t value, uint16_t index, uint16_t length);
void        usb_pmsc_MassStrageReset(USB_UTR_t *ptr, uint16_t value, uint16_t index, uint16_t length);
void        usb_pmsc_Task(USB_VP_INT_t);
void        usb_pmsc_UsbExecute(USB_UTR_t *mess);
void        usb_pstd_RequestBackup(USB_REQUEST_t *req);
void        usb_pmsc_SmpAtapiAnalyzeCbwCb(uint8_t *cbwcb);
void        usb_pmsc_SmpAtapiCommandExecute(USB_UTR_t *ptr, uint8_t *cbw, uint16_t status, USB_CB_t complete);
void        usb_pmsc_UsbPeriDriverSetting( USB_UTR_t *ptr );
void        usb_cstd_usbip_info_setting( USB_UTR_t *ptr );

void        usb_pmsc_UsbExecuteCont(USB_UTR_t *ptr, uint16_t, uint16_t);
void        usb_pmsc_ErrCswNg(USB_UTR_t *ptr, uint16_t data1, uint16_t data2);
void        usb_pmsc_ErrPhaseErr(USB_UTR_t *ptr, uint16_t data1, uint16_t data2);
void        usb_pmsc_ErrSetStall(USB_UTR_t *ptr, uint16_t data1, uint16_t data2);
void        usb_pmsc_ReqTransEnd(USB_UTR_t *ptr, uint16_t, uint16_t);


#endif  /* __R_USB_PMSC_EXTERN_H__ */
/******************************************************************************
End  Of File
******************************************************************************/
