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
* File Name    : r_usb_pmsc_api.c
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
#include "r_usb_cdefusbip.h"            /* USB-FW Library Header */
#include "r_usb_ckernelid.h"            /* Kernel ID definition */
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
extern volatile usb_utr_t               usb_gpmsc_Mess1;
extern volatile usb_utr_t               usb_gpmsc_Mess2;
extern volatile usb_utr_t               usb_gpmsc_Mess3;
extern volatile usb_utr_t               usb_gpmsc_Mess4;
extern volatile usb_utr_t               usb_gpmsc_Mess5;
extern volatile usb_utr_t               usb_gpmsc_Mess6;
extern volatile usb_utr_t               usb_gpmsc_Mess7;
extern volatile usb_utr_t               usb_gpmsc_Mess8;
extern volatile usb_utr_t               usb_gpmsc_Mess9;
extern volatile usb_utr_t               usb_gpmsc_Mess10;
extern volatile usb_utr_t               usb_gpmsc_Mess11;
extern volatile usb_utr_t               usb_gpmsc_Mess12;

extern volatile uint16_t    g_usb_pipe_toggle[10];
extern uint8_t                 usb_gpmsc_CbwCbLength;
/* usb_gpmsc_CbwCbLength depends on bInterfaceSubClass parameter.*/
extern uint8_t usb_gpmsc_InterfaceSubClass[7];

extern void        usb_pmsc_UsrCtrlTransFunction(usb_request_t *req, uint16_t Ctsq);
extern void        usb_pmsc_change_device_state( uint16_t data1, uint16_t device_state );

/******************************************************************************
Renesas Abstracted Peripheral Mass Storage Class Driver API functions
******************************************************************************/

/******************************************************************************
Function Name   : R_usb_pmsc_Open
Description     : Peripheral Mass Storage Class open function
Arguments       : uint16_t data1            : Not use
                : uint16_t data2            : Not use
Return value    : usb_er_t                  : USB_E_OK etc
******************************************************************************/
usb_er_t R_usb_pmsc_Open(uint16_t data1, uint16_t data2)
{
    uint8_t     subclass;
    usb_pipe_t    pipe_no;

    for ( pipe_no = 0; pipe_no < 10; pipe_no++ )
    {
        g_usb_pipe_toggle[ pipe_no ] = USB_NULL;
    }

    /* Interface Descriptor bInterfaceSubClass */
    subclass = usb_gpmsc_ConfigrationF1[15];

    if( subclass > USB_PMSC_MAX_SUBCLASS_RANGE )
    {   /* Abnormal Condition */
        USB_PRINTF0("*** Interface Descriptor bInterfaceSubClass is out of range ! ***\n");
        USB_PRINTF0("*** Mass Storage Class Not Initialized ***\n");
    }
    else
    {   /* Normal Condition */
        usb_gpmsc_CbwCbLength = usb_gpmsc_InterfaceSubClass[subclass];
        R_usb_pmsc_SetConfig();      /* Receive CBW */
    }

    return USB_E_OK;
}   /* eof R_usb_pmsc_Open() */

/******************************************************************************
Function Name   : R_usb_pmsc_Close
Description     : Peripheral Mass Storage Class close function
Arguments       : uint16_t data1            : Not use
                : uint16_t data2            : Not use
Return value    : usb_er_t                  : USB_E_OK etc
******************************************************************************/
usb_er_t R_usb_pmsc_Close(uint16_t data1, uint16_t data2)
{
    return USB_E_OK;
}   /* eof R_usb_pmsc_Close() */

/******************************************************************************
Function Name   : R_usb_pmsc_MassStrageReset
Description     : Peripheral Mass Storage Reset request
Arguments       : usb_cb_t complete    : Callback function
Return value    : none
******************************************************************************/
void R_usb_pmsc_MassStrageReset(usb_cb_t complete)
{
    usb_er_t    err;


    usb_gpmsc_Mess1.msginfo  = (usb_strct_t)USB_MASS_STORAGE_RESET;
    usb_gpmsc_Mess1.pipenum  = (usb_strct_t)0u;
    usb_gpmsc_Mess1.status   = (usb_strct_t)0u;
    usb_gpmsc_Mess1.complete = complete;   // callback function

    /* Send message */
    err = R_USB_SND_MSG(USB_PMSC_MBX, (usb_msg_t*)&usb_gpmsc_Mess1);
    if( err != USB_E_OK )
    {
        /* Send message failure */
        USB_PRINTF1("### pmscMassStrageReset snd_msg error (%ld)\n", err);
    }
}   /* eof R_usb_pmsc_MassStrageReset() */

/******************************************************************************
Function Name   : R_usb_pmsc_GetMaxLun
Description     : Peripheral Get Max LUN request
Arguments       : usb_cb_t complete    : 
Return value    : none
******************************************************************************/
void R_usb_pmsc_GetMaxLun(usb_cb_t complete)
{
    usb_er_t    err;

    usb_gpmsc_Mess2.msginfo  = (usb_strct_t)USB_GET_MAX_LUN;
    usb_gpmsc_Mess2.pipenum  = (usb_strct_t)0u;
    usb_gpmsc_Mess2.status   = (usb_strct_t)0u;
    usb_gpmsc_Mess2.complete = complete;

    /* Send message */
    err = R_USB_SND_MSG(USB_PMSC_MBX, (usb_msg_t*)&usb_gpmsc_Mess2);
    if( err != USB_E_OK )
    {
        /* Send message failure */
        USB_PRINTF1("### pmscGetMaxLun snd_msg error (%ld)\n", err);
    }
}   /* eof R_usb_pmsc_GetMaxLun() */

/******************************************************************************
Function Name   : R_usb_pmsc_SetConfig
Description     : Set_Configuration
Arguments       : none
Return value    : none
******************************************************************************/
void R_usb_pmsc_SetConfig(void)
{
    usb_gpmsc_Dtl = 0x00ul;
    R_usb_pmsc_DataTrans_Out(usb_gpmsc_Outpipe, (usb_leng_t)USB_MSC_CBWLENGTH, (uint8_t*)&usb_gpmsc_Cbw, (usb_cb_t)&R_usb_pmsc_TransResult);
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
void R_usb_pmsc_SetInterface(uint16_t data1, uint16_t data2)
{
    usb_gpmsc_Dtl = 0x00ul;
    R_usb_pmsc_DataTrans_Out(usb_gpmsc_Outpipe, (usb_leng_t)USB_MSC_CBWLENGTH, (uint8_t*)&usb_gpmsc_Cbw, (usb_cb_t)&R_usb_pmsc_TransResult);
    /* Receive Command Block Wrapper */
    usb_gpmsc_Seq = USB_PMSC_PCBWRCV;
}   /* eof R_usb_pmsc_SetInterface() */

/******************************************************************************
Function Name   : R_usb_pmsc_DataTrans_In
Description     : Tranfer data from USB
Arguments       : usb_pipe_t pipe           : Pipe No
                : usb_leng_t size           : Data Size
                : uint8_t    *table         : Data address
                : usb_cb_t   complete       : Callback function
Return value    : none
******************************************************************************/
void R_usb_pmsc_DataTrans_In(usb_pipe_t pipe, usb_leng_t size, uint8_t *table, usb_cb_t complete)
{
    /* PIPE Transfer set */
    usb_gpmsc_Mess3.pipenum      = pipe;
    usb_gpmsc_Mess3.tranadr      = table;
    usb_gpmsc_Mess3.tranlen      = size;
    usb_gpmsc_Mess3.setup        = 0;
    usb_gpmsc_Mess3.complete     = complete;
    usb_gpmsc_Mess3.pipectr      = g_usb_pipe_toggle[pipe];
    R_usb_pstd_TransferStart((usb_utr_t *)&usb_gpmsc_Mess3);
}   /* eof R_usb_pmsc_DataTrans_In() */


/******************************************************************************
Function Name   : R_usb_pmsc_DataTrans_In_CSW
Description     : Tranfer data from USB
Arguments       : usb_pipe_t pipe           : Pipe No
                : usb_leng_t size           : Data Size
                : uint8_t    *table         : Data address
                : usb_cb_t   complete       : Callback function
Return value    : none
******************************************************************************/
void R_usb_pmsc_DataTrans_In_CSW(usb_pipe_t pipe, usb_leng_t size, uint8_t *table, usb_cb_t complete)
{
    /* PIPE Transfer set */
    usb_gpmsc_Mess4.pipenum      = pipe;
    usb_gpmsc_Mess4.tranadr      = table;
    usb_gpmsc_Mess4.tranlen      = size;
    usb_gpmsc_Mess4.setup        = 0;
    usb_gpmsc_Mess4.complete     = complete;
    usb_gpmsc_Mess4.pipectr      = g_usb_pipe_toggle[pipe];
    R_usb_pstd_TransferStart((usb_utr_t *)&usb_gpmsc_Mess4);
}   /* eof R_usb_pmsc_DataTrans_In_CSW() */


/******************************************************************************
Function Name   : R_usb_pmsc_DataTrans_Out
Description     : Tranfer data from USB
Arguments       : usb_pipe_t pipe           : Pipe No
                : usb_leng_t size           : Data Size
                : uint8_t    *table         : Data address
                : usb_cb_t   complete       : Callback function
Return value    : none
******************************************************************************/
void R_usb_pmsc_DataTrans_Out(usb_pipe_t pipe, usb_leng_t size, uint8_t *table, usb_cb_t complete)
{
    /* PIPE Transfer set */
    usb_gpmsc_Mess5.pipenum      = pipe;
    usb_gpmsc_Mess5.tranadr      = table;
    usb_gpmsc_Mess5.tranlen      = size;
    usb_gpmsc_Mess5.setup        = (uint16_t *)0;
    usb_gpmsc_Mess5.complete     = complete;
    usb_gpmsc_Mess5.pipectr      = g_usb_pipe_toggle[pipe];
    R_usb_pstd_TransferStart((usb_utr_t *)&usb_gpmsc_Mess5);
}   /* eof R_usb_pmsc_DataTrans_Out() */

/******************************************************************************
Function Name   : R_usb_pmsc_TransResult
Description     : CallBack Function
Arguments       : usb_utr_t *mess           : message
Return value    : none
******************************************************************************/
void R_usb_pmsc_TransResult(usb_utr_t *mess)
{
    usb_er_t    err;


    g_usb_pipe_toggle[mess->pipenum] = mess->pipectr;


    /* From USB device to PMSC */
    mess->msginfo = (usb_strct_t)USB_PMSC_USB2PMSC;
    if( mess->status == USB_DATA_STOP )
    {
        USB_PRINTF0("### Peripheral Mass Storage driver stoped !\n");
    }
    err = R_USB_SND_MSG(USB_PMSC_MBX, (usb_msg_t*)mess);
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
void R_usb_pmsc_StallClearResult(usb_utr_t *ptr, uint16_t data, uint16_t dummy)
{
    usb_er_t    err;

    usb_gpmsc_Mess6.msginfo  = (usb_strct_t)0u;
    usb_gpmsc_Mess6.pipenum  = (usb_pipe_t)data;
    usb_gpmsc_Mess6.status   = (usb_strct_t)USB_PMSC_PIPE_STALL_CLEAR;
    usb_gpmsc_Mess6.complete = (usb_cb_t)0;   // callback function

    /* Send message */
    err = R_USB_SND_MSG(USB_PMSC_MBX, (usb_msg_t*)&usb_gpmsc_Mess6);
    if( err != USB_E_OK )
    {
        /* Send message failure */
        USB_PRINTF1("### pmscStallClearResult snd_msg error (%ld)\n", err);
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
    usb_utr_t   *mess;
    usb_er_t    err;
    uint16_t    status;

    err = R_USB_RCV_MSG(USB_PMSC_MBX,(usb_msg_t**)&mess);
    if( err != USB_E_OK )
    {
        /* Receive message failure without timeout */
        USB_PRINTF1("### pmscProcessWaitTmo rcv_msg error (%ld)\n", err);
        /* return (USB_ERROR); */
    }

    status = mess->status;

    if( err != USB_E_OK )
    {
        return (USB_ERROR);
    }
    return (status);
}   /* eof R_usb_pmsc_ProcessWaitTmo() */

/******************************************************************************
Function Name   : R_usb_pmsc_ControlTrans1
Description     : Process class request (control read)
Arguments       : usb_request_t *req        : request
Return value    : none
******************************************************************************/
void R_usb_pmsc_ControlTrans1(usb_request_t *req)
{
    if( (req->wIndex == 0) && (req->WORD.BIT.bRecipient == USB_INTERFACE) )
    {
        if( req->WORD.BIT.bRequest == USB_GET_MAX_LUN )
        {
            /* Get MaxLun of Mass storage */
            usb_pmsc_GetMaxLun(req->wValue, req->wIndex, req->wLength);
        }
        else
        {
            /* Set Stall */
            R_usb_pstd_SetStallPipe0();        /* Req Error */
        }
    }
    else
    {
        /* Set Stall */
        R_usb_pstd_SetStallPipe0();            /* Req Error */
    }
}   /* eof R_usb_pmsc_ControlTrans1() */

/******************************************************************************
Function Name   : R_usb_pmsc_ControlTrans4
Description     : Process class request (control read status stage)
Arguments       : usb_request_t *req        : request
Return value    : none
******************************************************************************/
void R_usb_pmsc_ControlTrans4(usb_request_t *req)
{
    if( req->WORD.BIT.bRequest == USB_GET_MAX_LUN )
    {
        R_usb_cstd_SetBufPipe0();      /* Set BUF */
    }
    else
    {
        /* Set Stall */
        R_usb_pstd_SetStallPipe0();        /* Req Error */
    }
    R_usb_pstd_ControlEnd((uint16_t)USB_CTRL_END);
}   /* eof R_usb_pmsc_ControlTrans4() */

/******************************************************************************
Function Name   : R_usb_pmsc_ControlTrans3
Description     : Process class request (control no data write)
Arguments       : usb_request_t *req        : request
Return value    : none
******************************************************************************/
void R_usb_pmsc_ControlTrans3(usb_request_t *req)
{
    if( (req->wIndex == 0) && (req->WORD.BIT.bRecipient == USB_INTERFACE) )
    {
        if( req->WORD.BIT.bRequest == USB_MASS_STORAGE_RESET )
        {
            /* Mass storage Reset */
            usb_pmsc_MassStrageReset(req->wValue, req->wIndex, req->wLength);
        }
        else
        {
            /* Set Stall */
            R_usb_pstd_SetStallPipe0();        /* Req Error */
        }
    }
    else
    {
        /* Set Stall */
        R_usb_pstd_SetStallPipe0();            /* Req Error */
    }
    if( req->WORD.BIT.bRequest != USB_MASS_STORAGE_RESET )
    {
        R_usb_pstd_ControlEnd((uint16_t)USB_CTRL_END);
    }
}   /* eof R_usb_pmsc_ControlTrans3() */

/******************************************************************************
Function Name   : R_usb_pmsc_ControlTrans0
Description     : Process class request (idle or setup stage)
Arguments       : usb_request_t *req        : request
Return value    : none
******************************************************************************/
void R_usb_pmsc_ControlTrans0(usb_request_t *req)
{
}   /* eof R_usb_pmsc_ControlTrans0() */

/******************************************************************************
Function Name   : R_usb_pmsc_ControlTrans2
Description     : Process class request (control write data stage)
Arguments       : usb_request_t *req        : request
Return value    : none
******************************************************************************/
void R_usb_pmsc_ControlTrans2(usb_request_t *req)
{
    R_usb_pstd_SetStallPipe0();                /* Req Error */
}   /* eof R_usb_pmsc_ControlTrans2() */

/******************************************************************************
Function Name   : R_usb_pmsc_ControlTrans5
Description     : Process class request (control write status stage)
Arguments       : usb_request_t *req        : request
Return value    : none
******************************************************************************/
void R_usb_pmsc_ControlTrans5(usb_request_t *req)
{
    R_usb_pstd_SetStallPipe0();                /* Req Error */
    R_usb_pstd_ControlEnd((uint16_t)USB_CTRL_END);
}   /* eof R_usb_pmsc_ControlTrans5() */

/******************************************************************************
Function Name   : R_usb_pmsc_driver_start
Description     : USB Peri Initialize process
Argument        : none
Return          : none
******************************************************************************/
void R_usb_pmsc_driver_start(void)
{
    uint8_t        index;
    
    
    for ( index = 0; index < 10; index++ )
    {
        g_usb_pipe_toggle[index] = USB_NULL;
    }

    /* Task Start Processing */
    R_usb_pstd_PcdOpen();            /* Pcd open */
}   /* eof R_usb_pmsc_driver_start() */

/******************************************************************************
Function Name   : R_usb_pmsc_Registration
Description     : Registration of peripheral Mass Strage Class Driver
Arguments       : none
Return value    : none
******************************************************************************/
void R_usb_pmsc_Registration(void)
{
    usb_pcdreg_t    driver;

    /* Driver registration */
    /* Pipe Define Table address */
    driver.pipetbl      = usb_gpmsc_EpPtr[0];
    /* Device descriptor Table address */
    driver.devicetbl    = (uint8_t *)&usb_gpmsc_DeviceDescriptor;
    /* Configuration descriptor Table address */
    driver.configtbl    = &usb_gpmsc_ConfigrationF1[0];
    /* String descriptor Table address */
    driver.stringtbl    = (uint8_t**)&usb_gpmsc_StrPtr[0];
    /* String descriptor Table address */
    driver.statediagram = (usb_cbinfo_t)&usb_pmsc_change_device_state;
    /* Control Transfer */
    driver.ctrltrans    = &usb_pmsc_UsrCtrlTransFunction;
    R_usb_pstd_DriverRegistration(&driver);
}   /* eof R_usb_pmsc_Registration() */


/******************************************************************************
End  Of File
******************************************************************************/
