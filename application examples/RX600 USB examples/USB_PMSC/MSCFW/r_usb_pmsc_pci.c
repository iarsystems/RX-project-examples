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
*******************************************************************************
* Copyright (C) 2010 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************
* File Name    : r_usb_PMSCpci.c
* Version      : 1.00
* Device(s)    : Renesas SH-Series, RX-Series
* Tool-Chain   : Renesas SuperH RISC engine Standard Toolchain
*              : Renesas RX Standard Toolchain
* OS           : Common to None and uITRON 4.0 Spec
* H/W Platform : Independent
* Description  : USB Host and Peripheral Interrupt code
*******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 17.03.2010 0.80    First Release
*         : 30.07.2010 0.90    Updated comments
*         : 02.08.2010 0.91    Updated comments
*         : 29.10.2010 1.00    Mass Production Release
******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_usb_ctypedef.h"     /* Type define */
#include "r_usb_usrconfig.h"
#include "r_usb_cdefusbip.h"        /* USB-FW Library Header */
#include "r_usb_cmacprint.h"        /* Standard IO macro */
#include "r_usb_ckernelid.h"        /* Kernel ID definition */
#include "r_usb_cmacsystemcall.h"   /* uITRON system call macro */
#include "r_usb_cextern.h"          /* USB-FW global define */
#include "r_usb_ansi.h"
#include "r_usb_control.h"
#include "r_usb_cusb_bitdefine.h"

#include "r_usb_api.h"
#include "r_usb_pmsc_api.h"
#include "r_usb_cmsc_define.h"      /* USB  Mass Storage Class Header */
#include "r_usb_pmsc_define.h"      /* Peri Mass Storage Class Driver define */
#include "r_usb_catapi_define.h"        /* Peripheral ATAPI Device extern define */
#include "r_usb_pmsc_extern.h"      /* Peri Mass Storage Class Extern */


/******************************************************************************
Section    <Section Difinition> , "Project Sections"
******************************************************************************/
#pragma section="_pmsc"


/******************************************************************************
Constant macro definitions
******************************************************************************/


/******************************************************************************
External variables and functions
******************************************************************************/


/******************************************************************************
Private global variables and functions
******************************************************************************/
uint8_t                 usb_gpmsc_CbwCbLength;
USB_UTR_t               usb_gpmsc_Mess;     /* PMSC <--> USB massage */

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
Function Name   : R_usb_pmsc_Open
Description     : Peripheral Mass Storage Class open function
Arguments       : uint16_t data1            : Not use
                : uint16_t data2            : Not use
Return value    : USB_ER_t                  : USB_E_OK etc
******************************************************************************/
USB_ER_t R_usb_pmsc_Open(USB_UTR_t *ptr, uint16_t data1, uint16_t data2)
{
    uint8_t     *cp;
    uint8_t     subclass;
    uint16_t    conf_num;

    /* usb_gpmsc_CbwCbLength parameter set */
    conf_num = usb_pstd_GetConfigNum();
    cp = usb_gpmsc_ConPtr[conf_num - 1];
    /* Interface Descriptor bInterfaceSubClass */
    subclass = (*(uint8_t*)((uint32_t)cp + 15ul));

    if( subclass > USB_PMSC_MAX_SUBCLASS_RANGE )
    {   /* Abnormal Condition */
        USB_PRINTF0(
        "*** Interface Descriptor bInterfaceSubClass is out of range ! ***\n");
        USB_PRINTF0("*** Mass Storage Class Not Initialized ***\n");
    }
    else
    {   /* Normal Condition */
        usb_gpmsc_CbwCbLength = usb_gpmsc_InterfaceSubClass[subclass];
        R_usb_pmsc_SetConfig(ptr);      /* Receive CBW */
    }

    return USB_E_OK;
}   /* eof R_usb_pmsc_Open() */

/******************************************************************************
Function Name   : R_usb_pmsc_Close
Description     : Peripheral Mass Storage Class close function
Arguments       : uint16_t data1            : Not use
                : uint16_t data2            : Not use
Return value    : USB_ER_t                  : USB_E_OK etc
******************************************************************************/
USB_ER_t R_usb_pmsc_Close(USB_UTR_t *ptr, uint16_t data1, uint16_t data2)
{
    return USB_E_OK;
}   /* eof R_usb_pmsc_Close() */

/******************************************************************************
Function Name   : R_usb_pmsc_MassStrageReset
Description     : Peripheral Mass Storage Reset request
Arguments       : USB_CB_INFO_t complete    : Callback function
Return value    : none
******************************************************************************/
void R_usb_pmsc_MassStrageReset(USB_UTR_t *ptr, USB_CB_INFO_t complete)
{
    USB_MH_t    p_blf;
    USB_ER_t    err, err2;
    USB_UTR_t   *cp;

    /* Get memorypool blk */
    err = R_USB_PGET_BLK(USB_PMSC_MPL, &p_blf);
    if( err == USB_E_OK )
    {
        cp = (USB_UTR_t*)p_blf; 
        cp->msghead = (USB_MH_t)NULL;
        cp->msginfo = (uint16_t)USB_MASS_STORAGE_RESET;
        cp->keyword = (uint16_t)0u;
        cp->status      = (uint16_t)0u;
        cp->complete    = (USB_CB_t)complete;   // callback function
        cp->ip = ptr->ip;
        cp->ipp = ptr->ipp;

        /* Send message */
        err = R_USB_SND_MSG(USB_PMSC_MBX, (USB_MSG_t*)p_blf);
        if( err != USB_E_OK )
        {
            /* Send message failure */
            USB_PRINTF1("### pmscMassStrageReset snd_msg error (%ld)\n"
                , err);
            err2 = R_USB_REL_BLK(USB_PMSC_MPL, (USB_MH_t)p_blf);
            if( err2 != USB_E_OK )
            {
                /* Release memory block failure */
                USB_PRINTF1("### pmscMassStrageReset rel_blk error (%ld)\n" , err2);
            }
        }
    }
    else
    {
        /* Get memoryblock failure */
        USB_PRINTF1("### pmscMassStrageReset pget_blk error (%ld)\n" , err);
    }
}   /* eof R_usb_pmsc_MassStrageReset() */

/******************************************************************************
Function Name   : R_usb_pmsc_GetMaxLun
Description     : Peripheral Get Max LUN request
Arguments       : USB_CB_INFO_t complete    : 
Return value    : none
******************************************************************************/
void R_usb_pmsc_GetMaxLun(USB_UTR_t *ptr, USB_CB_INFO_t complete)
{
    USB_MH_t    p_blf;
    USB_ER_t    err, err2;
    USB_UTR_t   *cp;

    /* Get mem pool blk */
    err = R_USB_PGET_BLK(USB_PMSC_MPL, &p_blf);
    if( err == USB_E_OK )
    {
        cp = (USB_UTR_t*)p_blf;
        cp->msghead = (USB_MH_t)NULL;
        cp->msginfo = (uint16_t)USB_GET_MAX_LUN;
        cp->keyword = (uint16_t)0u;
        cp->status      = (uint16_t)0u;
        cp->complete    = (USB_CB_t)complete;
        cp->ip = ptr->ip;
        cp->ipp = ptr->ipp;

        /* Send message */
        err = R_USB_SND_MSG(USB_PMSC_MBX, (USB_MSG_t*)p_blf);
        if( err != USB_E_OK )
        {
            /* Send message failure */
            USB_PRINTF1("### pmscGetMaxLun snd_msg error (%ld)\n", err);
            err2 = R_USB_REL_BLK(USB_PMSC_MPL, (USB_MH_t)p_blf);
            if( err2 != USB_E_OK )
            {
                /* Release memoryblock failure */
                USB_PRINTF1("### pmscGetMaxLun rel_blk error (%ld)\n" , err2);
            }
        }
    }
    else
    {
        /* Get momoryblock failure */
        USB_PRINTF1("### pmscGetMaxLun pget_blk error (%ld)\n", err);
    }
}   /* eof R_usb_pmsc_GetMaxLun() */

/******************************************************************************
Function Name   : R_usb_pmsc_SetConfig
Description     : Set_Configuration
Arguments       : none
Return value    : none
******************************************************************************/
void R_usb_pmsc_SetConfig(USB_UTR_t *ptr)
{
    usb_gpmsc_Dtl = 0x00ul;
    R_usb_pmsc_DataTrans(ptr, usb_gpmsc_Outpipe, (uint32_t)USB_MSC_CBWLENGTH
        , (uint8_t*)&usb_gpmsc_Cbw, &R_usb_pmsc_TransResult);
    /* Receive Command Block Wrapper */
    usb_gpmsc_Seq = USB_PMSC_PCBWRCV;
}   /* eof R_usb_pmsc_SetConfig() */

/******************************************************************************
Function Name   : R_usb_pmsc_SetInterface
Description     : Set_Interface
Arguments       : uint16_t data1            : Not use
                : uint16_t data2            : Not use
Return value    : none
******************************************************************************/
void R_usb_pmsc_SetInterface(USB_UTR_t *ptr, uint16_t data1, uint16_t data2)
{
    usb_gpmsc_Dtl = 0x00ul;
    R_usb_pmsc_DataTrans(ptr, usb_gpmsc_Outpipe, (uint32_t)USB_MSC_CBWLENGTH
        , (uint8_t*)&usb_gpmsc_Cbw, &R_usb_pmsc_TransResult);
    /* Receive Command Block Wrapper */
    usb_gpmsc_Seq = USB_PMSC_PCBWRCV;
}   /* eof R_usb_pmsc_SetInterface() */

/******************************************************************************
Function Name   : R_usb_pmsc_DescriptorChange
Description     : Descriptor change
Arguments       : uint16_t mode             : Speed mode
                : uint16_t data2            : Not use
Return value    : none
******************************************************************************/
void R_usb_pmsc_DescriptorChange(USB_UTR_t *ptr, uint16_t mode, uint16_t data2)
{
    if( mode == USB_HSCONNECT )
    {
        /* Hi-Speed Mode */
        usb_gpmsc_ConfigrationH1[1]     = USB_DT_CONFIGURATION;
        usb_gpmsc_ConfigrationF1[1]     = USB_DT_OTHER_SPEED_CONF;
        /* HIGH */
        usb_gpmsc_ConPtr[0]             = usb_gpmsc_ConfigrationH1;
        /* FULL */
        usb_gpmsc_ConPtrOther[0]        = usb_gpmsc_ConfigrationF1;
        usb_gpmsc_EpTbl1[3]             = 512;
        usb_gpmsc_EpTbl1[USB_EPL + 3u]  = 512;
    }
    else
    {
        /* Full-Speed Mode */
        usb_gpmsc_ConfigrationF1[1]     = USB_DT_CONFIGURATION;
        usb_gpmsc_ConfigrationH1[1]     = USB_DT_OTHER_SPEED_CONF;
        /* FULL */
        usb_gpmsc_ConPtr[0]             = usb_gpmsc_ConfigrationF1;
        /* HIGH */
        usb_gpmsc_ConPtrOther[0]        = usb_gpmsc_ConfigrationH1;
        usb_gpmsc_EpTbl1[3]             = 64;
        usb_gpmsc_EpTbl1[USB_EPL + 3u]  = 64;
    }

    usb_gpmsc_Inpipe    = usb_gpmsc_EpTbl1[0];
    usb_gpmsc_Outpipe   = usb_gpmsc_EpTbl1[USB_EPL];
}   /* eof R_usb_pmsc_DescriptorChange() */

/******************************************************************************
Function Name   : R_usb_pmsc_DataTrans
Description     : Tranfer data from USB
Arguments       : uint16_t pipe             : Pipe No
                : uint32_t size             : Data Size
                : uint8_t *table            : Data address
                : USB_CB_t complete         : Callback function
Return value    : none
******************************************************************************/
void R_usb_pmsc_DataTrans(USB_UTR_t *ptr, uint16_t pipe, uint32_t size, uint8_t *table
        , USB_CB_t complete)
{
    /* PIPE Transfer set */
    usb_gpmsc_Mess.keyword      = pipe;
    usb_gpmsc_Mess.tranadr      = table;
    usb_gpmsc_Mess.tranlen      = size;
    usb_gpmsc_Mess.setup        = 0;
    usb_gpmsc_Mess.complete     = complete;
    usb_gpmsc_Mess.segment      = USB_TRAN_END;
    usb_gpmsc_Mess.ip       = ptr->ip;
    usb_gpmsc_Mess.ipp      = ptr->ipp;
    usb_gpmsc_Mess.fn       = ptr->fn;
    R_usb_pstd_TransferStart(&usb_gpmsc_Mess);
}   /* eof R_usb_pmsc_DataTrans() */

/******************************************************************************
Function Name   : R_usb_pmsc_TransResult
Description     : CallBack Function
Arguments       : USB_UTR_t *mess           : message
Return value    : none
******************************************************************************/
void R_usb_pmsc_TransResult(USB_UTR_t *mess, uint16_t data1, uint16_t data2 )
{
    USB_ER_t    err;

    /* From USB device to PMSC */
    mess->msginfo = (uint16_t)USB_PMSC_USB2PMSC;
    if( mess->status == USB_DATA_STOP )
    {
        USB_PRINTF0("### Peripheral Mass Storage driver stoped !\n");
    }
    err = R_USB_SND_MSG(USB_PMSC_MBX, (USB_MSG_t*)mess);
    if( err != USB_E_OK )
    {
        /* Send message failure */
        USB_PRINTF1("### pmscTransResult snd_msg error (%ld)\n", err);
    }
}   /* eof R_usb_pmsc_TransResult() */

/******************************************************************************
Function Name   : R_usb_pmsc_StallClearResult
Description     : CallBack Function
Arguments       : uint16_t data             : keyword
                : uint16_t dummy            : not use
Return value    : none
******************************************************************************/
void R_usb_pmsc_StallClearResult(USB_UTR_t *ptr, uint16_t data, uint16_t dummy)
{
    USB_MH_t    p_blf;
    USB_ER_t    err, err2;
    USB_UTR_t   *cp;

    /* Get mem pool blk */
    err = R_USB_PGET_BLK(USB_PMSC_MPL, &p_blf);
    if( err == USB_E_OK )
    {
        cp = (USB_UTR_t*)p_blf; 
        cp->msghead = (USB_MH_t)NULL;
        cp->msginfo = (uint16_t)0u;
        cp->keyword = data;
        cp->status      = (uint16_t)USB_PMSC_PIPE_STALL_CLEAR;
        cp->ip = ptr->ip;
        cp->ipp = ptr->ipp;

        /* Send message */
        err = R_USB_SND_MSG(USB_PMSC_MBX, (USB_MSG_t*)p_blf);
        if( err != USB_E_OK )
        {
            /* Send message failure */
            USB_PRINTF1("### pmscStallClearResult snd_msg error (%ld)\n" , err);
            err2 = R_USB_REL_BLK(USB_PMSC_MPL,(USB_MH_t)p_blf);
            if( err2 != USB_E_OK )
            {
                USB_PRINTF1("### pmscStallClearResult rel_blk error (%ld)\n" , err2);
            }
        }
    }
    else
    {
        /* Get memoryblock faiure */
        USB_PRINTF1("### pmscStallClearResult pget_blk error (%ld)\n" , err);
    }
}   /* eof R_usb_pmsc_StallClearResult() */

/******************************************************************************
Function Name   : R_usb_pmsc_ProcessWaitTmo
Description     : CallBack Function
Arguments       : uint16_t tmo              : time out
Return value    : uint16_t                  : status
******************************************************************************/
uint16_t R_usb_pmsc_ProcessWaitTmo(uint16_t tmo)
{
    USB_UTR_t   *mess;
    USB_ER_t    err, err2;
    uint16_t    status;

    err = R_USB_TRCV_MSG(USB_PMSC_MBX,(USB_MSG_t**)&mess, (USB_TM_t)tmo);
    if( err == USB_E_TMOUT )
    {
        /* Timeout */
        /* USB_PRINTF0("### pmscProcessWaitTmo rcv_msg time out !\n"); */
        /* return (USB_ERROR); */
    }
    if( err != USB_E_OK )
    {
        /* Receive message failure without timeout */
        USB_PRINTF1("### pmscProcessWaitTmo rcv_msg error (%ld)\n", err);
        /* return (USB_ERROR); */
    }

    status = mess->status;
    err2 = R_USB_REL_BLK(USB_PMSC_MPL, (USB_MH_t)mess);
    if( err2 != USB_E_OK )
    {
        /* Release memory block failure */
        USB_PRINTF2("### pmscProcessWaitTmo (%x) rel_blk error (%ld)\n" , status, err2);
        return (USB_ERROR);
    }

    if( err != USB_E_OK )
    {
        return (USB_ERROR);
    }
    return (status);
}   /* eof R_usb_pmsc_ProcessWaitTmo() */

/******************************************************************************
Function Name   : R_usb_pmsc_ControlTrans1
Description     : Process class request (control read)
Arguments       : USB_REQUEST_t *req        : request
Return value    : none
******************************************************************************/
void R_usb_pmsc_ControlTrans1(USB_UTR_t *ptr, USB_REQUEST_t *req)
{
    if( (req->ReqIndex == 0) && (req->ReqTypeRecip == USB_INTERFACE) )
    {
        if( req->ReqRequest == USB_GET_MAX_LUN )
        {
            /* Get MaxLun of Mass storage */
            usb_pmsc_GetMaxLun(ptr, req->ReqValue, req->ReqIndex
                , req->ReqLength);
        }
        else
        {
            /* Set Stall */
            usb_pstd_SetStallPipe0( ptr );        /* Req Error */
        }
    }
    else
    {
        /* Set Stall */
        usb_pstd_SetStallPipe0( ptr );            /* Req Error */
    }
}   /* eof R_usb_pmsc_ControlTrans1() */

/******************************************************************************
Function Name   : R_usb_pmsc_ControlTrans4
Description     : Process class request (control read status stage)
Arguments       : USB_REQUEST_t *req        : request
Return value    : none
******************************************************************************/
void R_usb_pmsc_ControlTrans4(USB_UTR_t *ptr, USB_REQUEST_t *req)
{
    if( req->ReqRequest == USB_GET_MAX_LUN )
    {
        usb_cstd_SetBuf(ptr, (uint16_t)USB_PIPE0);      /* Set BUF */
    }
    else
    {
        /* Set Stall */
        usb_pstd_SetStallPipe0( ptr );        /* Req Error */
    }
    R_usb_pstd_ControlEnd(ptr, (uint16_t)USB_CTRL_END);
}   /* eof R_usb_pmsc_ControlTrans4() */

/******************************************************************************
Function Name   : R_usb_pmsc_ControlTrans3
Description     : Process class request (control no data write)
Arguments       : USB_REQUEST_t *req        : request
Return value    : none
******************************************************************************/
void R_usb_pmsc_ControlTrans3(USB_UTR_t *ptr, USB_REQUEST_t *req)
{
    if( (req->ReqIndex == 0) && (req->ReqTypeRecip == USB_INTERFACE) )
    {
        if( req->ReqRequest == USB_MASS_STORAGE_RESET )
        {
            /* Mass storage Reset */
            usb_pmsc_MassStrageReset(ptr, req->ReqValue, req->ReqIndex
                , req->ReqLength);
        }
        else
        {
            /* Set Stall */
            usb_pstd_SetStallPipe0( ptr );        /* Req Error */
        }
    }
    else
    {
        /* Set Stall */
        usb_pstd_SetStallPipe0( ptr );            /* Req Error */
    }
    if( req->ReqRequest != USB_MASS_STORAGE_RESET )
    {
        R_usb_pstd_ControlEnd(ptr, (uint16_t)USB_CTRL_END);
    }
}   /* eof R_usb_pmsc_ControlTrans3() */

/******************************************************************************
Function Name   : R_usb_pmsc_ControlTrans0
Description     : Process class request (idle or setup stage)
Arguments       : USB_REQUEST_t *req        : request
Return value    : none
******************************************************************************/
void R_usb_pmsc_ControlTrans0(USB_UTR_t *ptr, USB_REQUEST_t *req)
{
}   /* eof R_usb_pmsc_ControlTrans0() */

/******************************************************************************
Function Name   : R_usb_pmsc_ControlTrans2
Description     : Process class request (control write data stage)
Arguments       : USB_REQUEST_t *req        : request
Return value    : none
******************************************************************************/
void R_usb_pmsc_ControlTrans2(USB_UTR_t *ptr, USB_REQUEST_t *req)
{
    usb_pstd_SetStallPipe0( ptr );                /* Req Error */
}   /* eof R_usb_pmsc_ControlTrans2() */

/******************************************************************************
Function Name   : R_usb_pmsc_ControlTrans5
Description     : Process class request (control write status stage)
Arguments       : USB_REQUEST_t *req        : request
Return value    : none
******************************************************************************/
void R_usb_pmsc_ControlTrans5(USB_UTR_t *ptr, USB_REQUEST_t *req)
{
    usb_pstd_SetStallPipe0( ptr );                /* Req Error */
    R_usb_pstd_ControlEnd(ptr, (uint16_t)USB_CTRL_END);
}   /* eof R_usb_pmsc_ControlTrans5() */

/******************************************************************************
Function Name   : usb_pmsc_UsbExecuteCont
Description     : Execute Continue
Arguments       : uint16_t dat1             : not use
                : uint16_t dat2             : not use
Return value    : none
******************************************************************************/
void usb_pmsc_UsbExecuteCont(USB_UTR_t *ptr, uint16_t dat1, uint16_t dat2)
{
    USB_MH_t    p_blf;
    USB_ER_t    err = USB_ERROR;
    USB_UTR_t   *cp;
    
    /* Get mem pool blk */
    if( R_USB_PGET_BLK(USB_PMSC_MPL, &p_blf) == USB_E_OK )
    {
        cp = (USB_UTR_t*)p_blf; 
        cp->msghead = (USB_MH_t)NULL;
        cp->msginfo = (uint16_t)USB_PMSC_USB_EXECUTE;
        cp->keyword = (uint16_t)0u;
        cp->status  = (uint16_t)0u;
        cp->complete = (USB_CB_t)NULL;
        cp->ip = ptr->ip;
        cp->ipp = ptr->ipp;
        /* Send message */
        err = R_USB_SND_MSG( USB_PMSC_MBX, (USB_MSG_t*)p_blf );
        if( err != USB_E_OK )
        {
            err = R_USB_REL_BLK(USB_PMSC_MPL, (USB_MH_t)p_blf);
            USB_PRINTF0("### Peripheral UsbExecute snd_msg error\n");
        }
    }
    else
    {
        /* Get memory block failure */
        USB_PRINTF0("### Peripheral UsbExecute pget_blk error\n");
    }
}   /* eof usb_pmsc_UsbExecuteCont() */

/******************************************************************************
Function Name   : usb_pmsc_ErrCswNg
Description     : Error CSW
Arguments       : none
Return value    : none
******************************************************************************/
void usb_pmsc_ErrCswNg(USB_UTR_t *ptr, uint16_t data1, uint16_t data2)
{
    USB_MH_t    p_blf;
    USB_ER_t    err = USB_ERROR;
    USB_UTR_t   *cp;
    
    /* Get mem pool blk */
    if( R_USB_PGET_BLK(USB_PMSC_MPL, &p_blf) == USB_E_OK )
    {
        cp = (USB_UTR_t*)p_blf; 
        cp->msghead = (USB_MH_t)NULL;
        cp->msginfo = (uint16_t)USB_PMSC_ERR_CSW_NG;
        cp->keyword = (uint16_t)0u;
        cp->status  = (uint16_t)0u;
        cp->complete = (USB_CB_t)NULL;
        cp->ip = ptr->ip;
        cp->ipp = ptr->ipp;

        /* Send message */
        err = R_USB_SND_MSG( USB_PMSC_MBX, (USB_MSG_t*)p_blf );
        if( err != USB_E_OK )
        {
            /* Send message failure */
            err = R_USB_REL_BLK(USB_PMSC_MPL,(USB_MH_t)p_blf);
            USB_PRINTF0("### Peripheral Error snd_msg error\n");
        }
    }
    else
    {
        /* Get memory block failure */
        USB_PRINTF0("### Peripheral Error pget_blk error\n");
    }
}   /* eof usb_pmsc_ErrCswNg() */

/******************************************************************************
Function Name   : usb_pmsc_ErrPhaseErr
Description     : Phase error
Arguments       : none
Return value    : none
******************************************************************************/
void usb_pmsc_ErrPhaseErr(USB_UTR_t *ptr, uint16_t data1, uint16_t data2)
{
    USB_MH_t    p_blf;
    USB_ER_t    err = USB_ERROR;
    USB_UTR_t   *cp;
    
    /* Get mem pool blk */
    if( R_USB_PGET_BLK(USB_PMSC_MPL, &p_blf) == USB_E_OK )
    {
        cp = (USB_UTR_t*)p_blf; 
        cp->msghead = (USB_MH_t)NULL;
        cp->msginfo = (uint16_t)USB_PMSC_ERR_PHASE_ERR;
        cp->keyword = (uint16_t)0u;
        cp->status  = (uint16_t)0u;
        cp->complete = (USB_CB_t)NULL;
        cp->ip = ptr->ip;
        cp->ipp = ptr->ipp;

        /* Send message */
        err = R_USB_SND_MSG( USB_PMSC_MBX, (USB_MSG_t*)p_blf );
        if( err != USB_E_OK )
        {
            /* Send message failure */
            err = R_USB_REL_BLK(USB_PMSC_MPL, (USB_MH_t)p_blf);
            USB_PRINTF0("### Peripheral Error snd_msg error\n");
        }
    }
    else
    {
        /* Get memory block failure */
        USB_PRINTF0("### Peripheral Error pget_blk error\n");
    }
}   /* eof usb_pmsc_ErrPhaseErr() */

/******************************************************************************
Function Name   : usb_pmsc_ErrSetStall
Description     : Set stall
Arguments       : none
Return value    : none
******************************************************************************/
void usb_pmsc_ErrSetStall(USB_UTR_t *ptr, uint16_t data1, uint16_t data2)
{
    USB_MH_t    p_blf;
    USB_ER_t    err = USB_ERROR;
    USB_UTR_t   *cp;
    
    /* Get mem pool blk */
    if( R_USB_PGET_BLK(USB_PMSC_MPL, &p_blf) == USB_E_OK )
    {
        cp = (USB_UTR_t*)p_blf; 
        cp->msghead = (USB_MH_t)NULL;
        cp->msginfo = (uint16_t)USB_PMSC_ERR_SET_STALL;
        cp->keyword = (uint16_t)0u;
        cp->status  = (uint16_t)0u;
        cp->complete = (USB_CB_t)NULL;
        cp->ip = ptr->ip;
        cp->ipp = ptr->ipp;

        /* Send message */
        err = R_USB_SND_MSG( USB_PMSC_MBX, (USB_MSG_t*)p_blf );
        if( err != USB_E_OK )
        {
            /* Send message failure */
            err = R_USB_REL_BLK(USB_PMSC_MPL, (USB_MH_t)p_blf);
            USB_PRINTF0("### Peripheral Error2 snd_msg error\n");
        }
    }
    else
    {
        /* Get memory block failure */
        USB_PRINTF0("### Peripheral Error2 pget_blk error\n");
    }
}   /* eof usb_pmsc_ErrSetStall() */

/******************************************************************************
Function Name   : usb_pmsc_ReqTransEnd
Description     : Request transmit end
Arguments       : uint16_t dat1             : not use
                : uint16_t dat2             : not use
Return value    : none
******************************************************************************/
void usb_pmsc_ReqTransEnd(USB_UTR_t *ptr, uint16_t dat1, uint16_t dat2)
{
    USB_MH_t    p_blf;
    USB_ER_t    err = USB_ERROR;
    USB_UTR_t   *cp;
    
    /* Get mem pool blk */
    if( R_USB_PGET_BLK(USB_PMSC_MPL, &p_blf) == USB_E_OK )
    {
        cp = (USB_UTR_t*)p_blf; 
        cp->msghead = (USB_MH_t)NULL;
        cp->msginfo = (uint16_t)USB_PMSC_REQ_TRANS_END;
        cp->keyword = (uint16_t)0u;
        cp->status  = (uint16_t)0u;
        cp->complete = (USB_CB_t)NULL;
        cp->ip = ptr->ip;
        cp->ipp = ptr->ipp;

        /* Send message */
        err = R_USB_SND_MSG( USB_PMSC_MBX, (USB_MSG_t*)p_blf );
        if( err != USB_E_OK )
        {
            /* Send message failure */
            err = R_USB_REL_BLK(USB_PMSC_MPL, (USB_MH_t)p_blf);
            USB_PRINTF0( "### Peripheral Mass Storage Reset request snd_msg error\n");
        }
    }
    else
    {
        /* Get memory block failure */
        USB_PRINTF0( "### Peripheral Mass Storage Reset request pget_blk error\n");
    }
}   /* eof usb_pmsc_ReqTransEnd() */

/******************************************************************************
Function Name   : usb_pmsc_Cont
Description     : Request send message
Arguments       : USB_CB_INFO_t complete    : callback
Return value    : none
******************************************************************************/
void usb_pmsc_Cont(USB_UTR_t *ptr, USB_CB_INFO_t complete)
{
    USB_MH_t    p_blf;
    USB_ER_t    err = USB_ERROR;
    USB_UTR_t   *cp;

    /* Get mem pool blk */
    if( R_USB_PGET_BLK(USB_PMSC_MPL, &p_blf) == USB_E_OK )
    {
        cp = (USB_UTR_t*)p_blf; 
        cp->msghead = (USB_MH_t)NULL;
        cp->msginfo = (uint16_t)USB_GET_MAX_LUN;
        cp->keyword = (uint16_t)0u;
        cp->status  = (uint16_t)0u;
        cp->complete = (USB_CB_t)complete;  
        cp->ip = ptr->ip;
        cp->ipp = ptr->ipp;

        /* Send message */
        err = R_USB_SND_MSG( USB_PMSC_MBX, (USB_MSG_t*)p_blf );
        if( err != USB_E_OK )
        {
            /* Send message failure */
            err = R_USB_REL_BLK(USB_PMSC_MPL, (USB_MH_t)p_blf);
            USB_PRINTF0( "### Peripheral Get Max LUN request snd_msg error\n");
        }
    }
    else
    {
        /* Get memory block failure */
        USB_PRINTF0("### Peripheral Get Max LUN request pget_blk error\n");
    }
}   /* eof usb_pmsc_Cont() */

/******************************************************************************
End  Of File
******************************************************************************/
