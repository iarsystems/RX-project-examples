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
* File Name    : r_usb_PCDCpci.c
* Version      : 1.10
* Device(s)    : Renesas SH-Series, RX-Series
* Tool-Chain   : Renesas SuperH RISC engine Standard Toolchain
*              : Renesas RX Standard Toolchain
* OS           : Common to None and uITRON 4.0 Spec
* H/W Platform : Independent
* Description  : USB Host and Peripheral Interrupt code
*******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 06.08.2010 0.91    First Release
*         : 31.08.2010 0.92    PCDC Updated
*         : 02.12.2010 0.96    Spelling modification
*         : 02.12.2010 0.96    Unnecessary Code Delete
*         : 01.06.2011 1.10    Version 1.10 Release
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
#include "r_usb_cusb_bitdefine.h"

#include "r_usb_api.h"
/* Peri Communications Devices Class Extern */
#include "r_usb_pcdc_extern.h"


/******************************************************************************
Section    <Section Difinition> , "Project Sections"
******************************************************************************/
#pragma section="_pcdc"


/******************************************************************************
Constant macro definitions
******************************************************************************/

/******************************************************************************
External variables and functions
******************************************************************************/
extern void usb_pstd_SetStall(USB_UTR_t *ptr, uint16_t pipe);

extern uint8_t usb_gpcdc_SerialState_Table[];

/******************************************************************************
Private global variables and functions
******************************************************************************/

/* PCDC <--> USB massage */
USB_UTR_t usb_gpcdc_Mess[10];

/* Line Coding data */
USB_PCDC_LineCoding_t usb_gcdc_LineCoding;
/* Line Coding Set Request data */
USB_PCDC_LineCoding_t usb_gcdc_LineCodingReq;

/* Control Line Status data */
USB_PCDC_ControlLineState_t usb_gcdc_ControlLineState;
/* Duration of Break */
uint16_t usb_gcdc_SendBreakDuration;

#if USB_ANSIIO_PP != USB_ANSIIO_USE_PP
/* Call Back Function Info Transfer end */
USB_CB_t usb_gpcdc_TxCB;
/* Call Back Function Info Receive complete */
USB_CB_t usb_gpcdc_RxCB;
/* Call Back Function Info Serial Status Transfer end*/
USB_CB_t usb_gpcdc_StatCB;
#endif  /* USB_ANSIIO_PP != USB_ANSIIO_USE_PP */

/******************************************************************************
Renesas Abstracted Peripheral Communications Devices Class Driver API functions
******************************************************************************/

/******************************************************************************
Function Name   : R_usb_pcdc_task
Description     : Call Peripheral CDC task
Arguments       : USB_VP_INT_t stacd        : start code
Return value    : none
******************************************************************************/
void R_usb_pcdc_task(USB_VP_INT_t stacd)
{
    /* Peripheral CDC Task call */
    usb_pcdc_task( stacd );
} /* eof R_usb_pcdc_task */

/******************************************************************************
Function Name   : R_usb_pcdc_set_interface
Description     : Set_Interface
Arguments       : USB_UTR_t *ptr       ; USB system internal structure. Selects channel.
                : uint16_t data1       ; Not use
                : uint16_t data2       ; Not use
Return value    : none
******************************************************************************/
void R_usb_pcdc_set_interface(USB_UTR_t *ptr, uint16_t data1, uint16_t data2)
{
} /* eof R_usb_pcdc_set_interface */

/******************************************************************************
Function Name   : R_usb_pcdc_descriptor_change
Description     : Descriptor change
Arguments       : USB_UTR_t *ptr       ; USB system internal structure. Selects channel.
                : uint16_t mode        ; Speed mode
                : uint16_t data2       ; Not use
Return value    : none
******************************************************************************/
void R_usb_pcdc_descriptor_change(USB_UTR_t *ptr, uint16_t mode, uint16_t data2)
{
    /* Connect Speed = Hi-Speed? */
    if( mode == USB_HSCONNECT )
    {
        /* Set Descriptor type.  */
        /* Hi-Speed Mode */
        usb_gpcdc_ConfigrationH1[1]     = USB_DT_CONFIGURATION;
        usb_gpcdc_ConfigrationF1[1]     = USB_DT_OTHER_SPEED_CONF;
        /* HIGH */
        usb_gpcdc_ConPtr[0]             = usb_gpcdc_ConfigrationH1;
        /* FULL */
        usb_gpcdc_ConPtrOther[0]        = usb_gpcdc_ConfigrationF1;
        /* Max Packet Size set */
        usb_gpcdc_EpTbl1[3]             = 512;
        usb_gpcdc_EpTbl1[USB_EPL + 3u]  = 512;
    }
    else
    {
        /* Set Descriptor type. */
        /* Full-Speed Mode */
        usb_gpcdc_ConfigrationF1[1]     = USB_DT_CONFIGURATION;
        usb_gpcdc_ConfigrationH1[1]     = USB_DT_OTHER_SPEED_CONF;
        /* FULL */
        usb_gpcdc_ConPtr[0]             = usb_gpcdc_ConfigrationF1;
        /* HIGH */
        usb_gpcdc_ConPtrOther[0]        = usb_gpcdc_ConfigrationH1;
        /* Max Packet Size set */
        usb_gpcdc_EpTbl1[3]             = 64;
        usb_gpcdc_EpTbl1[USB_EPL + 3u]  = 64;
    }

#if USB_ANSIIO_PP != USB_ANSIIO_USE_PP
    usb_gpcdc_Inpipe    = usb_gpcdc_EpTbl1[0];              /* Pipe no. set for Data Class TX(BulkIn) */
    usb_gpcdc_Outpipe   = usb_gpcdc_EpTbl1[USB_EPL];        /* Pipe no. set for Data Class RX(BulkOut) */
    usb_gpcdc_Status_pipe = usb_gpcdc_EpTbl1[USB_EPL*2];    /* Pipe no. set for Communications Class */
#endif  /* USB_ANSIIO_PP != USB_ANSIIO_USE_PP */
} /* eof R_usb_pcdc_descriptor_change */

#if USB_ANSIIO_PP != USB_ANSIIO_USE_PP
/******************************************************************************
Function Name   : R_usb_pcdc_SendData
Description     : Data send request for Host
Arguments       : USB_UTR_t     *ptr        ; USB system internal structure. Selects channel.
                : uint8_t       *Table      ; Pointer to Data stored buffer 
                : uint32_t      size        ; Data size
                : USB_CB_INFO_t complete    ; Pointer to Callback function
Return value    : none
******************************************************************************/
void R_usb_pcdc_SendData(USB_UTR_t *ptr, uint8_t *Table, uint32_t size, USB_CB_t complete)
{
    USB_MH_t    p_blf;
    USB_ER_t    err, err2;
    USB_UTR_t   *cp;

    /* Get mem pool blk */
    err = R_USB_PGET_BLK(USB_PCDC_MPL, &p_blf);
    if( err == USB_E_OK )
    {
        cp = (USB_UTR_t*)p_blf;
        cp->msghead = (USB_MH_t)NULL;
        cp->msginfo = (uint16_t)USB_PCDC_PSEND_DATA;            /* Set message information. */
        cp->keyword = (uint16_t)USB_NOT_USED;                   /* Not use */
        cp->status      = (uint16_t)USB_NOT_USED;               /* Not use */
        cp->complete    = (USB_CB_t)complete;                   /* Set Callback function */
        cp->tranadr = (void *)Table;                            /* Transfer data Start address */
        cp->tranlen = (uint32_t)size;                           /* Transfer data length */
        cp->ip = ptr->ip;                                       /* IP number(0or1) */
        cp->ipp = ptr->ipp;                                     /* IP Address(USB0orUSB1) */

        /* Send message */
        err = R_USB_SND_MSG(USB_PCDC_MBX, (USB_MSG_t*)p_blf);
        if( err != USB_E_OK )
        {
            /* Send message failure */
            USB_PRINTF1( "### pcdcSendData snd_msg error (%ld)\n", err );
            err2 = R_USB_REL_BLK(USB_PCDC_MPL, (USB_MH_t)p_blf);
            if( err2 != USB_E_OK )
            {
                /* Release memoryblock failure */
                USB_PRINTF1( "### pcdcSendData rel_blk error (%ld)\n", err2 );
            }
        }
    }
    else
    {
        /* Get momoryblock failure */
        USB_PRINTF1( "### pcdcSendData pget_blk error (%ld)\n", err );
    }
} /* eof R_usb_pcdc_SendData */

/******************************************************************************
Function Name   : R_usb_pcdc_ReceiveData
Description     : Data Receive request for Host
Arguments       : USB_UTR_t     *ptr        ; USB system internal structure. Selects channel.
                : uint8_t       *Table      ; Pointer to Data stored buffer 
                : uint32_t      size        ; Data size
                : USB_CB_INFO_t complete    ; Pointer to Callback function
Return value    : none
******************************************************************************/
void R_usb_pcdc_ReceiveData(USB_UTR_t *ptr, uint8_t *Table, uint32_t size, USB_CB_t complete)
{
    USB_MH_t    p_blf;
    USB_ER_t    err, err2;
    USB_UTR_t   *cp;

    /* Get mem pool blk */
    err = R_USB_PGET_BLK(USB_PCDC_MPL, &p_blf);
    if( err == USB_E_OK )
    {
        cp = (USB_UTR_t*)p_blf;
        cp->msghead = (USB_MH_t)NULL;
        cp->msginfo = (uint16_t)USB_PCDC_PRECEIVE_DATA;         /* Set message information. */
        cp->keyword = (uint16_t)USB_NOT_USED;                   /* Not use */
        cp->status      = (uint16_t)USB_NOT_USED;               /* Not use */
        cp->complete    = (USB_CB_t)complete;                   /* Set Callback function */
        cp->tranadr = (void *)Table;                            /* Transfer data Start address */
        cp->tranlen = (uint32_t)size;                           /* Transfer data length */
        cp->ip = ptr->ip;                                       /* IP number(0or1) */
        cp->ipp = ptr->ipp;                                     /* IP Address(USB0orUSB1) */

        /* Send message */
        err = R_USB_SND_MSG(USB_PCDC_MBX, (USB_MSG_t*)p_blf);
        if( err != USB_E_OK )
        {
            /* Send message failure */
            USB_PRINTF1( "### pcdcReceiveData snd_msg error (%ld)\n", err );
            err2 = R_USB_REL_BLK(USB_PCDC_MPL, (USB_MH_t)p_blf);
            if( err2 != USB_E_OK )
            {
                /* Release memoryblock failure */
                USB_PRINTF1( "### pcdcReceiveData rel_blk error (%ld)\n", err2 );
            }
        }
    }
    else
    {
        /* Get momoryblock failure */
        USB_PRINTF1( "### pcdcReceiveData pget_blk error (%ld)\n", err );
    }
} /* eof R_usb_pcdc_ReceiveData */

/******************************************************************************
Function Name   : R_usb_pcdc_SerialStateNotification
Description     : Class Notification Serial State for USB Host
Arguments       : USB_UTR_t *ptr           ; USB system internal structure. Selects channel.
                : uint16_t  serial_state   ; State of UART
                : USB_CB_INFO_t complete   ; Pointer to Callback function
Return value    : none
******************************************************************************/
void R_usb_pcdc_SerialStateNotification(USB_UTR_t *ptr, uint16_t serial_state, USB_CB_t complete)
{
    USB_MH_t    p_blf;
    USB_ER_t    err, err2;
    USB_UTR_t   *cp;

    /* Get mem pool blk */
    err = R_USB_PGET_BLK(USB_PCDC_MPL, &p_blf);
    if( err == USB_E_OK )
    {
        cp = (USB_UTR_t*)p_blf;
        cp->msghead = (USB_MH_t)NULL;
        cp->msginfo = (uint16_t)USB_PCDC_PSERIAL_STATE_NOTIFICATION;    /* Set message information. */
        cp->status      = (uint16_t)USB_NOT_USED;               /* Not use */
        cp->complete    = (USB_CB_t)complete;                   /* Set Callback function */
        cp->ip = ptr->ip;                                       /* IP number(0or1) */
        cp->ipp = ptr->ipp;                                     /* IP Address(USB0orUSB1) */

        /* UART State bitmap set */
        usb_gpcdc_SerialState_Table[8] = (uint8_t)(serial_state & 0xff);
        usb_gpcdc_SerialState_Table[9] = (uint8_t)(serial_state >> 8);

        /* Transfer data Start address */
        cp->tranadr = (void *)&usb_gpcdc_SerialState_Table[0];
        /* Transfer data length */
        cp->tranlen = (uint32_t)10;

        /* Send message */
        err = R_USB_SND_MSG(USB_PCDC_MBX, (USB_MSG_t*)p_blf);
        if( err != USB_E_OK )
        {
            /* Send message failure */
            USB_PRINTF1( "### pcdcSerialStateNotification snd_msg error (%ld)\n", err );
            err2 = R_USB_REL_BLK(USB_PCDC_MPL, (USB_MH_t)p_blf);
            if( err2 != USB_E_OK )
            {
                /* Release memoryblock failure */
                USB_PRINTF1( "### pcdcSerialStateNotification rel_blk error (%ld)\n", err2 );
            }
        }
    }
    else
    {
        /* Get momoryblock failure */
        USB_PRINTF1( "### pcdcSerialStateNotification pget_blk error (%ld)\n", err );
    }
} /* eof R_usb_pcdc_SerialStateNotification */
#endif  /* USB_ANSIIO_PP != USB_ANSIIO_USE_PP */

/******************************************************************************
Function Name   : R_usb_pcdc_driver_start
Description     : USB Vendor Peripheral Driver Start
Argument        : none
Return          : none
******************************************************************************/
void R_usb_pcdc_driver_start( void )
{
    R_usb_cstd_SetTaskPri(USB_PCDC_TSK, USB_PRI_4);
} /* eof R_usb_pcdc_driver_start */

/******************************************************************************
Function Name   : R_usb_pcdc_usr_ctrl_trans_function
Description     : Class request processing for CDC 
Arguments       : USB_UTR_t *ptr       ; USB system internal structure. Selects channel.
                : USB_REQUEST_t *preq  ; Class request information
                : uint16_t ctsq        ; Control Stage
Return value    : none
******************************************************************************/
void R_usb_pcdc_usr_ctrl_trans_function(USB_UTR_t *ptr, USB_REQUEST_t *preq, uint16_t ctsq)
{
    if( preq->ReqTypeType == USB_CLASS )
    {
        switch( ctsq )
        {
        /* Idle or setup stage */
        case USB_CS_IDST:   usb_pcdc_ControlTrans0(ptr, preq);  break;
        /* Control read data stage */
        case USB_CS_RDDS:   usb_pcdc_ControlTrans1(ptr, preq);  break;
        /* Control write data stage */
        case USB_CS_WRDS:   usb_pcdc_ControlTrans2(ptr, preq);  break;
        /* Control write nodata status stage */
        case USB_CS_WRND:   usb_pcdc_ControlTrans3(ptr, preq);  break;
        /* Control read status stage */
        case USB_CS_RDSS:   usb_pcdc_ControlTrans4(ptr, preq);  break;
        /* Control write status stage */
        case USB_CS_WRSS:   usb_pcdc_ControlTrans5(ptr, preq);  break;

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
        USB_PRINTF2("R_usb_pcdc_usr_ctrl_trans_function Err: %x %x\n"
            , preq->ReqTypeType, ctsq);
        usb_pstd_SetStallPipe0( ptr );
    }
} /* eof R_usb_pcdc_usr_ctrl_trans_function */

/******************************************************************************
End  Of File
******************************************************************************/
