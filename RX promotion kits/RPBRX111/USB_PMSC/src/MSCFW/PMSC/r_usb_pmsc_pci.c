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
* File Name    : r_usb_pmsc_pci.c
* Version      : 2.00
* Description  : PCD Interface
*******************************************************************************/
/*******************************************************************************
* History    : DD.MM.YYYY Version Description
*            : 09.11.2012 1.00         First Release
*            : 30.11.2012 2.00
*******************************************************************************/


/******************************************************************************
Section definitions
******************************************************************************/
#ifdef R8CUSB
#pragma section program P_smpl
#pragma section bss B_smpl
#pragma section data R_smpl
#endif

#ifdef RL78USB
#endif

#ifdef RL78USB_SECTION
#pragma section @@CODE   P_SMP
#pragma section @@BASE   T_SMP
#pragma section @@DATA   B_SMP
#endif

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include "r_usb_ctypedef.h"                /* Type define */
#include "r_usb_ckernelid.h"            /* Kernel ID definition */
#include "r_usb_cdefusbip.h"            /* USB-FW Library Header */
#include "r_usb_api.h"                    /* USB API public function header */

#include "r_usb_pmsc_api.h"
#include "r_usb_cmsc_define.h"      /* USB  Mass Storage Class Header */
#include "r_usb_pmsc_define.h"      /* Peri Mass Storage Class Driver define */
#include "r_usb_catapi_define.h"        /* Peripheral ATAPI Device extern define */
#include "r_usb_pmsc_extern.h"      /* Peri Mass Storage Class Extern */


/******************************************************************************
Constant macro definitions
******************************************************************************/


/******************************************************************************
External variables and functions
******************************************************************************/
volatile usb_utr_t               usb_gpmsc_Mess1;
volatile usb_utr_t               usb_gpmsc_Mess2;
volatile usb_utr_t               usb_gpmsc_Mess3;
volatile usb_utr_t               usb_gpmsc_Mess4;
volatile usb_utr_t               usb_gpmsc_Mess5;
volatile usb_utr_t               usb_gpmsc_Mess6;
volatile usb_utr_t               usb_gpmsc_Mess7;
volatile usb_utr_t               usb_gpmsc_Mess8;
volatile usb_utr_t               usb_gpmsc_Mess9;
volatile usb_utr_t               usb_gpmsc_Mess10;
volatile usb_utr_t               usb_gpmsc_Mess11;
volatile usb_utr_t               usb_gpmsc_Mess12;

volatile uint16_t    g_usb_pipe_toggle[10];


/******************************************************************************
Private global variables and functions
******************************************************************************/
uint8_t                 usb_gpmsc_CbwCbLength;




/* usb_gpmsc_CbwCbLength depends on bInterfaceSubClass parameter.*/
uint8_t usb_gpmsc_InterfaceSubClass[7] =
{
    0,      /* No definition */
    0,      /* (01h:??)  Reduced Block Command (RBC)T10 (Now Searching) */
    0,      /* (02h:??)  SFF-8020i (Now Searching) */
    0,      /* (03h:??)  QIC-157 (Now Searching) */
    0,      /* (04h:12)  UFI */
    12,     /* (05h:12)  SFF-8070i */
    0,      /* (06h:chg) SCSI transparent command set (Now Searhing) */
/* 07h-FFh Reserved for future use */
};


/******************************************************************************
Renesas Abstracted Peripheral Mass Storage Class Driver API functions
******************************************************************************/

/******************************************************************************
Function Name   : usb_pmsc_UsbExecuteCont
Description     : Execute Continue
Arguments       : uint16_t dat1             : not use
                : uint16_t dat2             : not use
Return value    : none
******************************************************************************/
void usb_pmsc_UsbExecuteCont(uint16_t dat1, uint16_t dat2)
{
    usb_er_t    err = USB_ERROR;
    
    usb_gpmsc_Mess7.msginfo  = (usb_strct_t)USB_PMSC_USB_EXECUTE;
    usb_gpmsc_Mess7.pipenum  = (usb_pipe_t)0u;
    usb_gpmsc_Mess7.status   = (usb_strct_t)0u;
    usb_gpmsc_Mess7.complete = (usb_cb_t)USB_NULL;
    /* Send message */
    err = R_USB_SND_MSG( USB_PMSC_MBX, (usb_msg_t*)&usb_gpmsc_Mess7 );
    if( err != USB_E_OK )
    {
        USB_PRINTF0("### Peripheral UsbExecute snd_msg error\n");
    }
}   /* eof usb_pmsc_UsbExecuteCont() */

/******************************************************************************
Function Name   : usb_pmsc_ErrCswNg
Description     : Error CSW
Arguments       : none
Return value    : none
******************************************************************************/
void usb_pmsc_ErrCswNg(uint16_t data1, uint16_t data2)
{
    usb_er_t    err = USB_ERROR;
    
    usb_gpmsc_Mess8.msginfo  = (usb_strct_t)USB_PMSC_ERR_CSW_NG;
    usb_gpmsc_Mess8.pipenum  = (usb_pipe_t)0u;
    usb_gpmsc_Mess8.status   = (usb_strct_t)0u;
    usb_gpmsc_Mess8.complete = (usb_cb_t)USB_NULL;

    /* Send message */
    err = R_USB_SND_MSG( USB_PMSC_MBX, (usb_msg_t*)&usb_gpmsc_Mess8 );
    if( err != USB_E_OK )
    {
        /* Send message failure */
        USB_PRINTF0("### Peripheral Error snd_msg error\n");
    }
}   /* eof usb_pmsc_ErrCswNg() */

/******************************************************************************
Function Name   : usb_pmsc_ErrPhaseErr
Description     : Phase error
Arguments       : none
Return value    : none
******************************************************************************/
void usb_pmsc_ErrPhaseErr(uint16_t data1, uint16_t data2)
{
    usb_er_t    err = USB_ERROR;
    
    usb_gpmsc_Mess9.msginfo  = (usb_strct_t)USB_PMSC_ERR_PHASE_ERR;
    usb_gpmsc_Mess9.pipenum  = (usb_pipe_t)0u;
    usb_gpmsc_Mess9.status   = (usb_strct_t)0u;
    usb_gpmsc_Mess9.complete = (usb_cb_t)USB_NULL;

    /* Send message */
    err = R_USB_SND_MSG( USB_PMSC_MBX, (usb_msg_t*)&usb_gpmsc_Mess9 );
    if( err != USB_E_OK )
    {
        /* Send message failure */
        USB_PRINTF0("### Peripheral Error snd_msg error\n");
    }
}   /* eof usb_pmsc_ErrPhaseErr() */

/******************************************************************************
Function Name   : usb_pmsc_ErrSetStall
Description     : Set stall
Arguments       : none
Return value    : none
******************************************************************************/
void usb_pmsc_ErrSetStall(uint16_t data1, uint16_t data2)
{
    usb_er_t    err = USB_ERROR;
    
    usb_gpmsc_Mess10.msginfo  = (usb_strct_t)USB_PMSC_ERR_SET_STALL;
    usb_gpmsc_Mess10.pipenum  = (usb_pipe_t)0u;
    usb_gpmsc_Mess10.status   = (usb_strct_t)0u;
    usb_gpmsc_Mess10.complete = (usb_cb_t)USB_NULL;
    
    /* Send message */
    err = R_USB_SND_MSG( USB_PMSC_MBX, (usb_msg_t*)&usb_gpmsc_Mess10 );
    if( err != USB_E_OK )
    {
        /* Send message failure */
        USB_PRINTF0("### Peripheral Error2 snd_msg error\n");
    }
}   /* eof usb_pmsc_ErrSetStall() */

/******************************************************************************
Function Name   : usb_pmsc_ReqTransEnd
Description     : Request transmit end
Arguments       : usb_utr_t *utr_data ; Not use
Return value    : none
******************************************************************************/
void usb_pmsc_ReqTransEnd(usb_utr_t *utr_data)
{
    usb_er_t    err = USB_ERROR;
    
    usb_gpmsc_Mess11.msginfo  = (usb_strct_t)USB_PMSC_REQ_TRANS_END;
    usb_gpmsc_Mess11.pipenum  = (usb_pipe_t)0u;
    usb_gpmsc_Mess11.status   = (usb_strct_t)0u;
    usb_gpmsc_Mess11.complete = (usb_cb_t)USB_NULL;

    /* Send message */
    err = R_USB_SND_MSG( USB_PMSC_MBX, (usb_msg_t*)&usb_gpmsc_Mess11 );
    if( err != USB_E_OK )
    {
        /* Send message failure */
        USB_PRINTF0("### Peripheral Mass Storage Reset request snd_msg error\n");
    }
}   /* eof usb_pmsc_ReqTransEnd() */

/******************************************************************************
Function Name   : usb_pmsc_Cont
Description     : Request send message
Arguments       : usb_cbinfo_t complete    : callback
Return value    : none
******************************************************************************/
void usb_pmsc_Cont(usb_utr_t *ptr, usb_cbinfo_t complete)
{
    usb_er_t    err = USB_ERROR;

    usb_gpmsc_Mess12.msginfo  = (usb_strct_t)USB_GET_MAX_LUN;
    usb_gpmsc_Mess12.pipenum  = (usb_pipe_t)0u;
    usb_gpmsc_Mess12.status   = (usb_strct_t)0u;
    usb_gpmsc_Mess12.complete = (usb_cb_t)complete;

    /* Send message */
    err = R_USB_SND_MSG( USB_PMSC_MBX, (usb_msg_t*)&usb_gpmsc_Mess12 );
    if( err != USB_E_OK )
    {
        /* Send message failure */
        USB_PRINTF0("### Peripheral Get Max LUN request snd_msg error\n");
    }
}   /* eof usb_pmsc_Cont() */

/******************************************************************************
End  Of File
******************************************************************************/
