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
* Copyright (C) 2010(2011) Renesas Electronics Corpration
* and Renesas Solutions Corp. All rights reserved.
*******************************************************************************
* File Name    : r_usb_hhid_ansi.c
* Version      : 2.00
* Device(s)    : Renesas SH-Series, RX-Series
* Tool-Chain   : Renesas SuperH RISC engine Standard Toolchain
*              : Renesas RX Standard Toolchain
* OS           : Common to None and uITRON 4.0 Spec
* H/W Platform : Independent
* Description  : USB Host Control Manager
*******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 08.12.2011 0.50    First Release
*         : 31.05.2011 2.00    Support USB Basic FW v. 2.00.
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_usb_ctypedef.h"         /* Type define */
#include "r_usb_cdefusbip.h"        /* USB-FW Library Header */
#include "r_usb_cmacsystemcall.h"   /* uITRON system call macro */
#include "r_usb_ckernelid.h"        /* Kernel ID definition */
#include "r_usb_cmacprint.h"        /* Standard IO macro */
#include "r_usb_cextern.h"          /* USB-FW global define */
#include "r_usb_hhid_define.h"      /* HID class define */
#include "r_usb_hhid_extern.h"
#include "r_usb_ansi.h"
#include "r_usb_control.h"

#include "r_usb_api.h"
#include "r_usb_hhid_api.h"

/******************************************************************************
Section    <Section Definition> , "Project Sections"
******************************************************************************/
#pragma section="_hid"

#if USB_ANSIIO_PP == USB_ANSIIO_USE_PP
/******************************************************************************
External variables and functions
******************************************************************************/
extern uint16_t usb_ghstd_ConfigurationDescriptor[];
extern uint16_t usb_ghstd_DeviceDescriptor[];
extern uint8_t  usb_ghstd_ClassData[][512];
extern uint8_t  *usb_ghhid_ConfigTable; /* Configuration Descriptor Table */
extern uint8_t  *usb_hcls_SmplDeviceTable;

/* Host HID Class Request Parametor */
extern USB_HHID_CLASS_REQUEST_PARM_t   usb_ghhid_class_request_parm;

/******************************************************************************
Private global variables and functions
******************************************************************************/
static  int16_t         usb_ghhid_SmplClassSeq;
USB_OPEN_PARM_t usb_ghhid_open_prm;

static void     usb_cstd_AnsiCallBack(USB_UTR_t *ptr, uint16_t unused, uint16_t status);
void usb_hhid_smpl_Enue( USB_UTR_t *ptr, struct usb_fninfo *p_fnstr, uint8_t devadr, uint8_t speed );

/******************************************************************************
Function Name   : usb_open_hhid
Description     : Open request for Host HID task.
Arguments       : USB_UTR_t *ptr             : IP info (mode, IP no., reg address).
                : struct usb_fninfo *p_fnstr : File structure pointer
                : uint8_t devadr             : Device address
                : uint8_t speed              : Connected speed of device
Return          : Process rusult(NG(-1), OK(0))
******************************************************************************/
int16_t usb_open_hhid(USB_UTR_t *ptr, struct usb_fninfo *p_fnstr, uint8_t devadr, uint8_t speed)
{
    USB_MH_t            p_blf;
    USB_ER_t            err;
    USB_CLSINFO_t       *cp;
    int16_t             ret = -1;

    /* Open stateu check */
    if( p_fnstr->open_state == USB_CTL_ERR_PROCESS_COMPLETE )
    {
        /* Open OK */
        p_fnstr->open_state = USB_CTL_ERR_NONE;
        ret = 0;                                                /* OPEN COMPLETE */
    }
    else if( p_fnstr->open_state == USB_CTL_ERR_NONE )
    {
        /* Open request send  */
        /* Get mem block from pool. */
        if( R_USB_PGET_BLK(USB_HHID_MPL, &p_blf) == USB_E_OK )
        {
            cp = (USB_CLSINFO_t*)p_blf;
            cp->msghead = (USB_MH_t)NULL;
            cp->msginfo = USB_HHID_TCMD_OPEN;                   /* Set message information : Open */
            cp->ip = ptr->ip;                                   /* IP number(0or1) */
            cp->ipp = ptr->ipp;                                 /* IP address (USB0 or USB1) */
            usb_ghhid_open_prm.p_fnstr = p_fnstr;               /* File structure pointer */
            usb_ghhid_open_prm.devadr = devadr;                 /* device address */
            usb_ghhid_open_prm.speed = speed;                   /* Set USB connected speed. */

            /* Send message */
            err = R_USB_SND_MSG( USB_HHID_MBX, (USB_MSG_t*)cp );
            if( err == USB_E_OK )
            {
                /* Open request send OK */
                p_fnstr->open_state = USB_CTL_ERR_PROCESS_WAIT;
            }
            else
            {
                /* Error */
                err = R_USB_REL_BLK(USB_HHID_MPL,(USB_MH_t)p_blf);
                USB_PRINTF0( "### usb_open_hstd function snd_msg error\n");
                /* Open request send NG */
                p_fnstr->open_state = USB_CTL_ERR_PROCESS_ERR;
            }
        }
        else
        {
            /* Error */
            USB_PRINTF0( "### usb_open_hstd function pget_blk error\n");
            /* Get mem. pool blk NG */
            p_fnstr->open_state = USB_CTL_ERR_PROCESS_ERR;
        }
    }
    else if( p_fnstr->open_state == USB_CTL_ERR_PROCESS_WAIT )
    {
        /* Open process wait */
    }
    else
    {
        /* Error */
    }
    return ret;
}   /* eof usb_open_hhid() */

/******************************************************************************
Function Name   : usb_hhid_read_data
Description     : Host pipe receive process.
Argument        : int16_t fn             : File no.
                : void *buf              : Receive data storage address
                : uint32_t length        : Receive data size
Return          : Receive size
******************************************************************************/
int32_t usb_hhid_read_data( int16_t fn, void *buf, uint32_t length )
{
    USB_MH_t            p_blf;
    USB_ER_t            err;
    USB_CLSINFO_t       *cp;

    /* Get mem block from pool. */
    if( R_USB_PGET_BLK(USB_HHID_MPL, &p_blf) == USB_E_OK )
    {
        cp = (USB_CLSINFO_t*)p_blf;
        cp->msghead = (USB_MH_t)NULL;
        cp->msginfo = USB_HHID_TCMD_RECEIVE;        /* Set message information :USB receive */
        cp->fn = fn;                                /* File no. */
        cp->tranadr = buf;                          /* Receive data storage address */
        cp->tranlen = length;                                   /* Receive data size */

        /* Send message */
        err = R_USB_SND_MSG( USB_HHID_MBX, (USB_MSG_t*)cp );
        if( err == USB_E_OK )
        {
            /* Transfer start request send OK */
            usb_fnInfo[fn].read_state = USB_CTL_ERR_PROCESS_WAIT;
        }
        else
        {
            /* Error */
            err = R_USB_REL_BLK(USB_HHID_MPL,(USB_MH_t)p_blf);
            USB_PRINTF0( "### usb_hhid_read_data function snd_msg error\n");
            /* Transfer start request send NG */
            usb_fnInfo[fn].read_state = USB_CTL_ERR_PROCESS_ERR;
        }
    }
    else
    {
        /* Error */
        USB_PRINTF0( "### usb_hhid_read_data function pget_blk error\n");
        /* Get mem block from pool. NG */
        usb_fnInfo[fn].read_state = USB_CTL_ERR_PROCESS_ERR;
    }

    return -1;
}   /* eof usb_hhid_read_data() */

/******************************************************************************
Function Name   : usb_hhid_write_data
Description     : Host pipe send process.
Argument        : int16_t fn             : File no.
                : void *buf              : Send data storage address
                : uint32_t length        : Send data size
Return          : Send size
******************************************************************************/
int32_t usb_hhid_write_data( int16_t fn, void *buf, uint32_t length )
{
    USB_MH_t            p_blf;
    USB_ER_t            err;
    USB_CLSINFO_t       *cp;

    /* Get mem block from pool. */
    if( R_USB_PGET_BLK(USB_HHID_MPL, &p_blf) == USB_E_OK )
    {
        cp = (USB_CLSINFO_t*)p_blf;
        cp->msghead = (USB_MH_t)NULL;
        cp->msginfo = USB_HHID_TCMD_SEND;       /* Set message information :USB transfer */
        cp->fn = fn;                            /* File no. */
        cp->tranadr = buf;                      /* Send data storage address */
        cp->tranlen = length;                                   /* Send data size */

        /* Send message */
        err = R_USB_SND_MSG( USB_HHID_MBX, (USB_MSG_t*)cp );
        if( err == USB_E_OK )
        {
            /* Transfer start request send OK */
            usb_fnInfo[fn].write_state = USB_CTL_ERR_PROCESS_WAIT;
        }
        else
        {
            /* Error */
            err = R_USB_REL_BLK(USB_HHID_MPL,(USB_MH_t)p_blf);
            USB_PRINTF0( "### usb_hhid_write_data function snd_msg error\n");
        }
    }
    else
    {
        /* Error */
        USB_PRINTF0( "### usb_hhid_write_data function pget_blk error\n");
        /* Get mem block from pool. NG */
        usb_fnInfo[fn].write_state = USB_CTL_ERR_PROCESS_ERR;
    }

    return -1;
}   /* eof usb_hhid_write_data() */

/******************************************************************************
Function Name   : usb_control_class_request
Description     : class request message send for host HID task
Arguments       : none
Return value    : USB_ER_t          : Error info
******************************************************************************/
USB_ER_t usb_control_class_request(void *pram)
{
    return R_usb_hhid_class_request( pram );
}   /* eof usb_control_class_request() */

/******************************************************************************
Function Name   : usb_hhid_smpl_Enue
Description     : Class Enumeration
Argument        : USB_CLSINFO_t *mess       : Class Info Data Pointer
Return          : none
******************************************************************************/
void usb_hhid_smpl_Enue( USB_UTR_t *ptr, struct usb_fninfo *p_fnstr, uint8_t devadr, uint8_t speed )
{
    int16_t     ret = -1;
    uint8_t     *p_desc, *p_iftable;
    uint16_t    desc_len;
    uint16_t    inpipe;
    uint16_t    status;
    uint8_t     string;

    /* Manager Mode Change */
    switch( usb_ghhid_SmplClassSeq )
    {
    case USB_SEQ_0:
        R_usb_hstd_EnumGetDescriptor( ptr, devadr, 2, usb_cstd_AnsiCallBack);
        break;

    case USB_SEQ_1:
        R_usb_hstd_EnumGetDescriptor( ptr, devadr, 3, usb_cstd_AnsiCallBack);
        break;

    case USB_SEQ_2:
        R_usb_hstd_EnumGetDescriptor( ptr, devadr, 4, usb_cstd_AnsiCallBack);
        break;

    case USB_SEQ_3:
        /* String descriptor */
        usb_hstd_GetStringDescriptor1(ptr, devadr
            , (uint16_t)15,(USB_CB_t)&usb_cstd_AnsiCallBack);
        break;

    case USB_SEQ_4:
        string = usb_hcls_SmplDeviceTable[15];
        /* String descriptor */
        usb_hstd_GetStringDescriptor2(ptr, devadr
            , (uint16_t)string,(USB_CB_t)&usb_cstd_AnsiCallBack);
        break;

    case USB_SEQ_5:
        p_desc = (uint8_t *)usb_ghstd_ConfigurationDescriptor;

        desc_len = ((uint16_t)*(p_desc + 3)) << 8;
        desc_len += (uint16_t)*(p_desc + 2);

    // If the result of class check is OK, 
    // this function request to allocate HW Pipe to HCD.
        inpipe  = R_usb_hstd_allocatePipe( ptr, USB_INT );

        if( USB_NG == inpipe )
        {
            // Can not allocate HW Pipe
//          usb_shhid_classErrorInfo = ;
            p_fnstr->open_state = USB_CTL_ERR_PROCESS_ERR;
        }

        p_fnstr->inPipeNo   = inpipe;
        if( USB_NG ==  usb_setEpTbl( inpipe, usb_ghhid_SmplEpTbl ))
        {
//          usb_shhid_classErrorInfo = ;
            p_fnstr->open_state = USB_CTL_ERR_PROCESS_ERR;
        }

        // Searching InterfaceDescriptor 
        p_iftable = usb_search_ifDesc(USB_IFCLS_HID);

        desc_len = desc_len - (p_iftable - p_desc);

        /* Pipe Information table set */
        status = usb_hhid_pipe_info(p_iftable, speed, desc_len, 0);
        if( USB_ERROR == status )
        {
//          usb_shhid_classErrorInfo = ;
            p_fnstr->open_state = USB_CTL_ERR_PROCESS_ERR;
        }

        p_fnstr->isOpened       = USB_YES;

        p_fnstr->inPipeNo       = inpipe;

        p_fnstr->class_read = &usb_hhid_read_data;
        p_fnstr->class_write = &usb_hhid_write_data;
        p_fnstr->close_func = &usb_close_host;

        // Request to send SetConfiguration to Manager Task.
        R_usb_hstd_MgrEnumSetConfiguration( ptr, devadr, *(p_desc + 5), usb_cstd_AnsiCallBack);
        break;

    case USB_SEQ_6:
        p_fnstr->open_state = USB_CTL_ERR_PROCESS_COMPLETE;
        ret = USB_OK;
        break;

    default:
        break;
    }

    if( ret == USB_OK )
    {
        usb_ghhid_SmplClassSeq = 0;
    }
    else
    {
        usb_ghhid_SmplClassSeq++;
    }
}   /* eof usb_hhid_smpl_Enue() */

/******************************************************************************
Function Name   : usb_cstd_AnsiCallBack
Description     : Callback function. Called by HCD when a requested data trans-
                : mission/reception over USB is completed. (Releasing task 
                : waiting for completion of data transmission/reception.)
Argument        : USB_UTR_t *ptr    : IP info (mode, IP no, reg. address).
                : uint16_t unused   : No used
                : uint16_t status   : Transfer result status
Return          : none
******************************************************************************/
void usb_cstd_AnsiCallBack(USB_UTR_t *ptr, uint16_t unused, uint16_t status)
{
    USB_MH_t            p_blf;
    USB_ER_t            err;
    USB_CLSINFO_t       *cp;

    /* Open request send  */
    /* Get mem pool blk */
    if( R_USB_PGET_BLK(USB_HHID_MPL, &p_blf) == USB_E_OK )
    {
        cp = (USB_CLSINFO_t*)p_blf;
        cp->msghead = (USB_MH_t)NULL;
        cp->msginfo = USB_HHID_TCMD_OPEN;
        cp->ip = ptr->ip;
        cp->ipp = ptr->ipp;

        /* Send message */
        err = R_USB_SND_MSG( USB_HHID_MBX, (USB_MSG_t*)cp );
        if( err == USB_E_OK )
        {
            /* Open request send OK */
        }
        else
        {
            /* error */
            err = R_USB_REL_BLK(USB_HHID_MPL,(USB_MH_t)p_blf);
            USB_PRINTF0( "### usb_open_hstd function snd_msg error\n");
            /* Open request send NG */
            usb_ghhid_open_prm.p_fnstr->open_state = USB_CTL_ERR_PROCESS_ERR;
        }
    }
    else
    {
        /* error */
        USB_PRINTF0( "### usb_open_hstd function pget_blk error\n");
        /* Get mem pool blk NG */
        usb_ghhid_open_prm.p_fnstr->open_state = USB_CTL_ERR_PROCESS_ERR;
    }
}   /* eof usb_cstd_AnsiCallBack() */

#endif  /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */

