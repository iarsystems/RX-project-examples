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
* File Name    : r_usb_pmsc_api.c
* Description  : USB Host and Peripheral Interrupt code
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 04.01.2014 1.00 First Release
*         : 30.01.2015 1.01    Added RX71M.
*         : 30.09.2015 1.11    Added RX63N.
***********************************************************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_usb_basic_if.h"
#include "r_usb_api.h"
#include "r_usb_pmsc_api.h"
#include "r_usb_pmsc_define.h"      /* Peri Mass Storage Class Driver define */
#include "r_usb_pmsc_extern.h"      /* Peri Mass Storage Class Driver define */
#include "r_usb_pmsc_config.h"
#include "r_usb_pmsc_if.h"
/******************************************************************************
Constant macro definitions
******************************************************************************/


/******************************************************************************
External variables and functions
******************************************************************************/
extern USB_ER_t usb_pmsc_SmpAtapiCloseMedia(void);
/* Pipe Direction from Host to Device */
extern uint16_t usb_gpmsc_Outpipe;

extern void usb_pmsc_SmpAtapiTask(USB_VP_INT_t stacd);

/******************************************************************************
Private global variables and functions
******************************************************************************/
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
    uint8_t     subclass;

    /* Interface Descriptor bInterfaceSubClass */
    subclass = USB_INTERFACE_SUBCLASS;

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
Arguments       : USB_CB_t complete    : Callback function
Return value    : none
******************************************************************************/
void R_usb_pmsc_MassStrageReset(USB_UTR_t *ptr, USB_CB_t complete)
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
Arguments       : USB_CB_t complete    : Callback function
Return value    : none
******************************************************************************/
void R_usb_pmsc_GetMaxLun(USB_UTR_t *ptr, USB_CB_t complete)
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
Function Name   : R_usb_pmsc_driver_start
Description     : USB Peri Initialize process
Argument        : none
Return          : none
******************************************************************************/
void R_usb_pmsc_driver_start( USB_UTR_t *ptr )
{
    R_usb_cstd_SetTaskPri(USB_PMSC_TSK, USB_PRI_4);
    R_usb_cstd_SetTaskPri(USB_PFLSH_TSK, USB_PRI_5);    /* Set peripheral application task priority.  */
}
/******************************************************************************
End of function R_usb_pmsc_driver_start
******************************************************************************/


/******************************************************************************
Function Name   : R_usb_pmsc_StrgTaskOpen
Description     : Peripheral Mass Storage Class Task Open
Arguments       : none
Return value    : none
******************************************************************************/
void R_usb_pmsc_StrgTaskOpen(USB_UTR_t *ptr)
{
    /* Media Driver open */
}   /* eof R_usb_pmsc_StrgTaskOpen() */

/******************************************************************************
Function Name   : R_usb_pmsc_StrgTaskClose
Description     : Peripheral Mass Storage Class Task Close
Arguments       : none
Return value    : none
******************************************************************************/
void R_usb_pmsc_StrgTaskClose(USB_UTR_t *ptr)
{
    /* Media Driver close */
    usb_pmsc_SmpAtapiCloseMedia();
}   /* eof R_usb_pmsc_StrgTaskClose() */

/******************************************************************************
Function Name   : R_usb_pmsc_UsrCtrlTransFunction
Description     : Control transfer
Arguments       : USB_REQUEST_t *ptr        : request
                : uint16_t ctsq             : control stage
Return value    : none
******************************************************************************/
void R_usb_pmsc_UsrCtrlTransFunction(USB_UTR_t *ptr, USB_REQUEST_t *req, uint16_t ctsq)
{
    if( req->ReqTypeType == USB_CLASS )
    {
        switch( ctsq )
        {
        /* Idle or setup stage */
        case USB_CS_IDST:   R_usb_pmsc_ControlTrans0(ptr, req); break;
        /* Control read data stage */
        case USB_CS_RDDS:   R_usb_pmsc_ControlTrans1(ptr, req); break;
        /* Control write data stage */
        case USB_CS_WRDS:   R_usb_pmsc_ControlTrans2(ptr, req); break;
        /* Control write nodata status stage */
        case USB_CS_WRND:   R_usb_pmsc_ControlTrans3(ptr, req); break;
        /* Control read status stage */
        case USB_CS_RDSS:   R_usb_pmsc_ControlTrans4(ptr, req); break;
        /* Control write status stage */
        case USB_CS_WRSS:   R_usb_pmsc_ControlTrans5(ptr, req); break;

        /* Control sequence error */
        case USB_CS_SQER:
            R_usb_pstd_ControlEnd(ptr, (uint16_t)USB_DATA_ERR); break;
        /* Illegal */
        default:
            R_usb_pstd_ControlEnd(ptr, (uint16_t)USB_DATA_ERR); break;
        }
    }
    else
    {
        USB_PRINTF2("usb_pmsc_UsrCtrlTransFunction Err: %x %x\n"
            , req->ReqTypeType, ctsq);
        usb_pstd_SetStallPipe0( ptr );
    }
}   /* eof usb_pmsc_UsrCtrlTransFunction() */


/******************************************************************************
Function Name   : R_usb_pmsc_task
Description     : Peripheral Mass Storage Class main task
Arguments       : USB_VP_INT_t stacd        : start code
Return value    : none
******************************************************************************/
void R_usb_pmsc_task(USB_VP_INT_t stacd)
{
    usb_pmsc_Task( stacd );
}   /* eof R_usb_pmsc_task() */


/*******************************************************************************
* Function Name: R_usb_pmsc_atapi_task
* Description  : Execute ATAPI commands
* Arguments    : USB_VP_INT_t stacd        : start code
* Return value : none
*******************************************************************************/
void R_usb_pmsc_atapi_task(USB_VP_INT_t stacd)
{
    usb_pmsc_SmpAtapiTask( stacd );
}
/* End of function R_usb_pmsc_atapi_task(). */


/******************************************************************************
End  Of File
******************************************************************************/
