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
* Copyright (C) 2010(2011) Renesas Electronics Corporation
* and Renesas Solutions Corp. All rights reserved.
*******************************************************************************
* File Name    : r_usb_phid_ansi.c
* Version      : 1.10
* Device(s)    : Renesas
* Tool-Chain   : Renesas
* OS           : Common to None and uITRON
* H/W Platform : Independent
* Description  : USB HID application code
******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 30.07.2010 0.90    First Release
*         : 30.11.2010 0.95    USBSTDFW(Mass Production Release)
*         : 24.06.2011 1.10    Version 1.10 Release
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
#include "r_usb_api.h"
#include "r_usb_phid_define.h"      /* HID class define */
#include "r_usb_class_usrcfg.h"
#include "r_usb_phid_extern.h"


#if USB_ANSIIO_PP == USB_ANSIIO_USE_PP
/******************************************************************************
Section    <Section Difinition> , "Project Sections"
******************************************************************************/
#pragma section="_hid"

/******************************************************************************
Private global variables and functions
******************************************************************************/
int16_t     usb_open_phid( USB_UTR_t *ptr, struct usb_fninfo *p_fnstr, uint8_t devadr, uint8_t speed );
int32_t     usb_phid_read_data( int16_t fn, void *buf, uint32_t length );
int32_t     usb_phid_write_data( int16_t fn, void *buf, uint32_t length );

/*****************************************************************************
External variables and functions
******************************************************************************/
extern uint16_t     usb_phid_isconnected( void );

/******************************************************************************
Renesas Abstracted USB Driver functions
******************************************************************************/
/******************************************************************************
Function Name   : usb_open_phid
Description     : Peripheral HID Class Open Process
Argument        : USB_UTR_t *ptr             : IP Information(Mode,IP No.,Reg Address)
                : struct usb_fninfo *p_fnstr : File Structure pointer
                : uint8_t devadr             : Device address
                : uint8_t speed              : Device connected speed
Return          : Process rusult(NG(-1), OK(0))
******************************************************************************/
int16_t usb_open_phid( USB_UTR_t *ptr, struct usb_fninfo *p_fnstr, uint8_t devadr, uint8_t speed )
{
    uint16_t        buf, pipeno, start_pipe_no, end_pipe_no;

    /* Check complete Enumeration for Host HID */
    if( USB_ON != usb_phid_isconnected() )
    {
        /* Error end when not complete Enumeration */
        return (int16_t)-1;
    }

    /* Check Open request is HID Class ? */
    if( ptr->keyword == USB_CLASS_PHID )
    {
        /* When Data Class resource is Pipe6 */
        start_pipe_no = USB_PHID_USE_PIPE;
        end_pipe_no = USB_PHID_USE_PIPE;
    }
    else
    {
        return (int16_t)-1;
    }

    /* Search use Pipe number for USB TX */
    for( pipeno = start_pipe_no; pipeno <= end_pipe_no; pipeno++ )
    {
        /* Get Pipe Direction of pipe table. */
        buf = usb_phid_get_pipedir( usb_gphid_EpTbl, pipeno );

        /* Check get pipe information */
        if( USB_NG == buf )
        {
            return (int16_t)-1;
        }
        else
        {
            /* Check transfer direction is IN pipe ? */
            if(buf == USB_DIR_P_IN)
            {
                p_fnstr->inPipeNo = pipeno;         /* Pipe number registration for RX. */
            }
            else
            {
                p_fnstr->outPipeNo = pipeno;        /* Pipe number registration for TX. */
            }
        }
    }

    /* usb_fnInfo[FileNumber] target block is complete open */
    p_fnstr->isOpened       = USB_YES;

    /* Registration read(),write() function for PHID TX ans RX  */
    p_fnstr->class_read     = &usb_phid_read_data;
    p_fnstr->class_write    = &usb_phid_write_data;

    /* Registration close() function for PHID close  */
    p_fnstr->close_func     = &usb_close_peri;

    p_fnstr->usb_ip     = USB_PERI_USBIP_NUM;                       /* IP number(0or1) */
    p_fnstr->usb_ipp    = usb_cstd_GetUsbIpAdr( p_fnstr->usb_ip );    /* IP Address(USB0orUSB1) */

    return  USB_OK;
}   /* eof usb_open_phid() */

/******************************************************************************
Function Name   : usb_phid_read_data
Description     : Peripheral HID pipe receive process
Argument        : int16_t fn             : File No.
                : void *buf              : Receive data store address
                : uint32_t length        : Receive data size
Return          : Receive size
******************************************************************************/
int32_t usb_phid_read_data( int16_t fn, void *buf, uint32_t length )
{
    USB_MH_t        p_blf;
    USB_ER_t        err;
    USB_CLSINFO_t   *cp;

    /* Get mem pool blk */
    if( R_USB_PGET_BLK( USB_PHID_MPL, &p_blf ) == USB_E_OK )
    {
        cp = (USB_CLSINFO_t*)p_blf;
        cp->msghead = (USB_MH_t)NULL;
        cp->msginfo = USB_SET_REPORT;               /* Set Message informaion : SET_REPORT */
        cp->fn = fn;                                /* FileNumber Set */
        cp->tranadr = buf;                          /* Receive data store address */
        cp->tranlen = length;                       /* Receive data size */

        /* Send message */
        err = R_USB_SND_MSG( USB_PHID_MBX, (USB_MSG_t*)cp );
        if(err == USB_E_OK)
        {
            /* Transfer Start request send OK */
            usb_fnInfo[fn].read_state = USB_CTL_ERR_PROCESS_WAIT;
        }
        else
        {
            /* error */
            err = R_USB_REL_BLK( USB_PHID_MPL,(USB_MH_t)p_blf );
            USB_PRINTF0( "### usb_phid_read_data function snd_msg error\n" );
            /* Transfer Start request send NG */
            usb_fnInfo[fn].read_state = USB_CTL_ERR_PROCESS_ERR;
        }
    }
    else
    {
        /* error */
        USB_PRINTF0( "### usb_phid_read_data function pget_blk error\n" );
        /* Get mem pool blk NG */
        usb_fnInfo[fn].read_state = USB_CTL_ERR_PROCESS_ERR;
    }

    return -1;
}   /* eof usb_phid_read_data() */

/******************************************************************************
Function Name   : usb_phid_write_data
Description     : Peripheral HID pipe send process
Argument        : int16_t fn             : File No.
                : void *buf              : Send data store address
                : uint32_t length        : Send data size
Return          : Send size
******************************************************************************/
int32_t usb_phid_write_data( int16_t fn, void *buf, uint32_t length )
{
    USB_MH_t        p_blf;
    USB_ER_t        err;
    USB_CLSINFO_t   *cp;

    /* Get mem pool blk */
    if( R_USB_PGET_BLK(USB_PHID_MPL, &p_blf ) == USB_E_OK )
    {
        cp = (USB_CLSINFO_t*)p_blf;
        cp->msghead = (USB_MH_t)NULL;
        cp->msginfo = USB_SEND_DATA;        /* Set Message information : Transfer data */
        cp->fn = fn;                        /* FileNumber Set */
        cp->tranadr = buf;                  /* Send data store address */
        cp->tranlen = length;               /* Send data size */

        /* Send message */
        err = R_USB_SND_MSG( USB_PHID_MBX, (USB_MSG_t*)cp );
        if( err == USB_E_OK )
        {
            /* Transfer Start request send OK */
            usb_fnInfo[fn].write_state = USB_CTL_ERR_PROCESS_WAIT;
        }
        else
        {
            /* error */
            err = R_USB_REL_BLK( USB_PHID_MPL, (USB_MH_t)p_blf );
            USB_PRINTF0( "### usb_phid_write_data function snd_msg error\n" );
        }
    }
    else
    {
        /* error */
        USB_PRINTF0( "### usb_phid_write_data function pget_blk error\n" );
        /* Get mem pool blk NG */
        usb_fnInfo[fn].write_state = USB_CTL_ERR_PROCESS_ERR;
    }

    return -1;
}   /* eof usb_phid_write_data() */

#endif  /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */

/******************************************************************************
End  Of File
******************************************************************************/
