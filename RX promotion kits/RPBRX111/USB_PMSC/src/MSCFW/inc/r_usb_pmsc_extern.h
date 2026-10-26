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
* File Name    : r_usb_pmsc_extern.h
* Version      : 2.00
* Description  : MSC External Data and Function
*******************************************************************************/
/*******************************************************************************
* History    : DD.MM.YYYY Version Description
*            : 09.11.2012 1.00         First Release
*            : 30.11.2012 2.00
*******************************************************************************/

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
extern usb_pipe_t       usb_gpmsc_Inpipe;
extern usb_pipe_t       usb_gpmsc_Outpipe;
extern uint32_t         usb_gpmsc_Dtl;
extern uint32_t         usb_gpmsc_Tag;
extern uint8_t          *usb_gpmsc_ConPtr[];
extern uint8_t          *usb_gpmsc_ConPtrOther[];
extern uint8_t          *usb_gpmsc_StrPtr[];
extern uint8_t          usb_gpmsc_CbwCbLength;
extern uint8_t          usb_gpmsc_ConfigrationF1[];
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
extern usb_utr_t        usb_gpmsc_Mess;
extern volatile uint8_t usb_gpmsc_MediaArea[];

uint8_t     usb_pmsc_CheckCase13(uint32_t ul_size, uint8_t *uc_case);
uint8_t     usb_pmsc_CheckMeaning(uint8_t seq);
uint8_t     usb_pmsc_CheckValid(uint32_t length);
uint8_t     usb_pmsc_CommandCheck(uint8_t seq);
uint8_t     usb_pmsc_TransferMatrix(uint8_t uc_pmsc_case);
usb_er_t    usb_pmsc_SmpAtapiInitMedia(void);
void        usb_pmsc_AtapiTransResult(usb_utr_t *mess, uint16_t data1, uint16_t data2 );
void        usb_pmsc_CswSet(uint8_t ar_resp, uint32_t ul_size);
void        usb_pmsc_Error0(void);
void        usb_pmsc_Error1(void);
void        usb_pmsc_Error2(void);
void        usb_pmsc_Error3(void);
void        usb_pmsc_Error4(void);
void        usb_pmsc_Error5(void);
void        usb_pmsc_GetMaxLun(uint16_t value, uint16_t index, uint16_t length);
void        usb_pmsc_MassStrageReset(uint16_t value, uint16_t index, uint16_t length);
void        usb_pmsc_Task(void);
void        usb_pmsc_UsbExecute(usb_utr_t *mess);
void        usb_pstd_RequestBackup(usb_request_t *req);
void        usb_pmsc_SmpAtapiAnalyzeCbwCb(uint8_t *cbwcb);
void        usb_pmsc_SmpAtapiCommandExecute(uint8_t *cbw, usb_strct_t status, usb_cb_t complete);
void        usb_pmsc_UsbPeriDriverSetting( usb_utr_t *ptr );


void        usb_pmsc_UsbExecuteCont(uint16_t, uint16_t);
void        usb_pmsc_ErrCswNg(uint16_t data1, uint16_t data2);
void        usb_pmsc_ErrPhaseErr(uint16_t data1, uint16_t data2);
void        usb_pmsc_ErrSetStall(uint16_t data1, uint16_t data2);
void        usb_pmsc_ReqTransEnd(usb_utr_t *utr_data);


#endif  /* __R_USB_PMSC_EXTERN_H__ */
/******************************************************************************
End  Of File
******************************************************************************/
