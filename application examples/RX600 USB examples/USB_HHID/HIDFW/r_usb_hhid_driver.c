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
* File Name    : r_usb_HHIDdriver.c
* Version      : 1.02
* Device(s)    : Renesas
* Tool-Chain   : Renesas
* OS           : Common to None and uITRON
* H/W Platform : Independent
* Description  : Host HID class driver code
******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 30.07.2010 0.90    First Release
*         : 30.11.2010 0.95    USBSTDFW(Mass Production Release)
*         : 10.02.2011 1.00    Host HID Class(Mass Production Release)
*         : 17.06.2011 1.02    Support USB Basic Fw V110
******************************************************************************

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
External variables and functions
******************************************************************************/
extern uint16_t         usb_ghhub_DefEPTbl;
extern const uint16_t   usb_ghhub_TPL;
extern uint8_t  usb_ghstd_ClassData[][512];

extern void usb_cpu_LcdDisp(uint8_t pos, uint8_t *str);

#if USB_ANSIIO_PP == USB_ANSIIO_USE_PP
extern  USB_OPEN_PARM_t usb_ghhid_open_prm;

extern  void usb_hhid_smpl_Enue( USB_UTR_t *ptr, struct usb_fninfo *p_fnstr, uint8_t devadr, uint8_t speed );
#endif  /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */

/******************************************************************************
Section    <Section Difinition> , "Project Sections"
******************************************************************************/
#pragma section="_hid"

/******************************************************************************
Private global variables and functions
******************************************************************************/
USB_UTR_t   usb_ghhid_transfer_message;             /* USB Data Transfer Request for HCD */
uint16_t    usb_shhid_class_request_setup[5];       /* Control Transfer Request field */
uint8_t     *usb_ghhid_ConfigTable;                 /* Configuration Descriptor Table */
uint16_t    usb_ghhid_Devaddr;                      /* Device Address */
uint16_t    usb_ghhid_maxps;                        /* Max Packet Size */

uint8_t     usb_ghhid_str_desc_data[USB_HHID_CLSDATASIZE];

/* Host HID Class Request Parametor */
USB_HHID_CLASS_REQUEST_PARM_t   usb_ghhid_class_request_parm;

static USB_UTR_t    usb_shhid_class_control;        /* Request for HID to USB Request */
uint8_t         *usb_ghhid_device_table;        /* Device Descriptor Table */
uint16_t        usb_ghhid_enum_seq;             /* Enumeration Sequence */
uint16_t        usb_ghhid_Speed;                /* Device speed */
static uint16_t usb_hhid_cmd_submit(USB_UTR_t *ptr, USB_CB_t complete);
uint8_t         *usb_hcls_SmplDeviceTable;
uint8_t         *usb_ghhid_interface_table;     /* Interface Descriptor Table */

static void     usb_hhid_InTransferResult(USB_UTR_t *mess, uint16_t data1, uint16_t data2);
#if USB_ANSIIO_PP == USB_ANSIIO_USE_PP
static void     usb_hhid_OutTransferResult(USB_UTR_t *mess, uint16_t data1, uint16_t data2);
#endif  /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */

void        usb_hhid_check_result( USB_UTR_t *ptr, uint16_t, uint16_t );
static USB_ER_t usb_hhid_class_request_process(void);

/******************************************************************************
Renesas Abstracted USB Driver functions
******************************************************************************/

/******************************************************************************
Function Name   : usb_hhid_data_trans
Description     : Tranfer data from USB
Arguments       : uint16_t pipe             : Pipe no.
                : uint32_t size             : data Size
                : uint8_t *table            : data address
                : USB_CB_t complete         : Callback function
Return value    : none
******************************************************************************/
USB_ER_t usb_hhid_data_trans(USB_UTR_t *ptr, uint16_t pipe, uint32_t size, uint8_t *table, USB_CB_t complete)
{
    /* Start of data transfer(USB-IP host function) */
    /* Transfer pipe no set */
    usb_ghhid_transfer_message.keyword      = pipe;
    /* data message address set */
    usb_ghhid_transfer_message.tranadr      = table;
    /* Transfer data length */
    usb_ghhid_transfer_message.tranlen      = size;
    /* Setup message address set */
    usb_ghhid_transfer_message.setup            = 0;
    /* Callback set */
    usb_ghhid_transfer_message.complete     = complete;
    usb_ghhid_transfer_message.segment      = USB_TRAN_END;
    usb_ghhid_transfer_message.fn = ptr->fn;
    usb_ghhid_transfer_message.ip = ptr->ip;
    usb_ghhid_transfer_message.ipp = ptr->ipp;

    /* Transfer start */
    return( R_usb_hstd_TransferStart(&usb_ghhid_transfer_message) );
}   /* eof usb_hhid_data_trans() */

/******************************************************************************
Function Name   : usb_hhid_check_result
Description     : 
Argument        : 
Return          : 
******************************************************************************/
void usb_hhid_check_result(USB_UTR_t *ptr, uint16_t unused, uint16_t status)
{
    USB_MH_t            p_blf;
    USB_ER_t            err;
    USB_CLSINFO_t       *cp;

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
        if( err != USB_E_OK )
        {
            /* error */
            err = R_USB_REL_BLK(USB_HHID_MPL,(USB_MH_t)p_blf);
            USB_PRINTF0( "### usb_open_hstd function snd_msg error\n");
        }
    }
    else
    {
        /* error */
        USB_PRINTF0( "### usb_open_hstd function pget_blk error\n");
    }
}   /* eof usb_hhid_check_result() */

/******************************************************************************
Function Name   : usb_hhid_enumeration_sequence
Description     : 
Argument        : USB_UTR_t *mess
Return value    : none
******************************************************************************/
void    usb_hhid_enumeration_sequence(USB_UTR_t *mess)
{
    uint16_t        retval;
    uint8_t         string;
    uint8_t         *p_desc;
    uint8_t         *p_iftable;
    uint16_t        desc_len;
    uint8_t         protocol;
#ifdef USB_DEBUGPRINT_PP
    uint8_t         *table;
    uint8_t         pdata[32],j;
#endif /* #ifdef USB_DEBUGPRINT_PP */

    /* Branch for Enumeration Sequence */
    switch(usb_ghhid_enum_seq)
    {
    /* Enumeration Sequence String Descriptor #0 receive request */
    case    USB_HHID_ENUM_STR_DT0_REQ:
        /* Get String descriptor */
        usb_hhid_get_string_desc(mess, usb_ghhid_Devaddr, (uint16_t)0, (USB_CB_t)&usb_hhid_check_result);
        /* String Descriptor #0 Receive wait */
        usb_ghhid_enum_seq++;
        break;
    /* Enumeration Sequence String Descriptor #0 Receive complete */
    case    USB_HHID_ENUM_STR_DT0_WAIT:
        /* String descriptor check */
        if(mess->status==(USB_ER_t)USB_CTRL_END)
        {
            /* Set iProduct */
            string = usb_ghhid_device_table[15];
            /* Get String descriptor */
            usb_hhid_get_string_desc(mess, usb_ghhid_Devaddr, (uint16_t)string, (USB_CB_t)&usb_hhid_check_result);
        }
        /* StringDescriptor iProduct Receive */
        usb_ghhid_enum_seq++;
        break;
    /* StringDescriptor iProduct Receive complete */
    case    USB_HHID_ENUM_STR_IPRODUCT_WAIT:
        /* String descriptor check */
        if(mess->status==(USB_ER_t)USB_CTRL_END)
        {
#ifdef USB_DEBUGPRINT_PP
            /* String descriptor iProduct Address set */
            table   = (uint8_t*)&usb_ghstd_ClassData;
            /* String descriptor bLength check */
            if( table[0] < (uint8_t)( 32 * 2 + 2 ) )
            {
                /* Number of characters = bLength /2 -1  */
                /* 1 character 16bit(UNICODE String) */
                table[0]    = (uint8_t)( table[0] / 2 -1);
            }
            else
            {
                /* Number of characters = 32 set */
                table[0]    = (uint8_t)32;
            }
            for( j = 0; j < table[0]; j++ )
            {
                pdata[j]    = table[ j * 2 + 2 ];
            }
            pdata[ table[0] ] = 0;
            /* Display Product name */
            USB_PRINTF1("  %s\n", pdata);
#endif /* #ifdef USB_DEBUGPRINT_PP */
        }
        else
        {
            USB_PRINTF0("*** Product name error\n");
        }

        protocol = R_usb_hhid_get_hid_protocol();               /* Get HID Protocol */
        if( ( protocol != USB_HHID_IFPRO_KBD ) && ( protocol != USB_HHID_IFPRO_MSE ) )
        {
            USB_PRINTF0("### Device information error !\n");
            /* Enumeration Sequence Complete */
            usb_ghhid_enum_seq  = USB_HHID_ENUM_COMPLETE;
            /* Enumeration class check error */
            retval  = USB_ERROR;
        }

        p_desc = usb_ghhid_ConfigTable;

        desc_len = ((uint16_t)*(p_desc + 3)) << 8;
        desc_len += (uint16_t)*(p_desc + 2);

        // Searching InterfaceDescriptor 
        p_iftable = usb_ghhid_interface_table;

        desc_len = desc_len - (p_iftable - p_desc);

        /* pipe information table set */
        retval = usb_hhid_pipe_info(p_iftable, usb_ghhid_Speed, desc_len, 0);
        if( retval == USB_ERROR )
        {
            USB_PRINTF0("### Device information error !\n");
            /* Enumeration Sequence Complete */
            usb_ghhid_enum_seq  = USB_HHID_ENUM_COMPLETE;
            /* Enumeration class check error */
            retval  = USB_ERROR;
        }
        else
        {
            /* Enumeration Sequence Complete */
            usb_ghhid_enum_seq  = USB_HHID_ENUM_COMPLETE;
            /* Enumeration class check OK */
            retval  = USB_DONE;
        }
        /* return to MGR */
        R_usb_hstd_ReturnEnuMGR(mess, retval);
        break;
    default:
        break;
    }
}
/******************************************************************************
End of function usb_hhid_enumeration_sequence
******************************************************************************/

USB_CB_t usb_shhid_data_tran_complete;
/******************************************************************************
Function Name   : usb_hhid_task
Description     : Host HIT Task
Argument        : none
Return value    : none
******************************************************************************/
void usb_hhid_task(USB_VP_INT stacd)
{
    USB_UTR_t   *mess;
    USB_ER_t    err = 0l;

    err = R_USB_TRCV_MSG(USB_HHID_MBX,(USB_MSG_t**)&mess,(uint16_t)3000);
    if( err == USB_E_OK )
    {
        switch( mess->msginfo  )
        {
#if USB_ANSIIO_PP == USB_ANSIIO_USE_PP
            case USB_HHID_TCMD_OPEN:
                /* Class Enumeration */
                usb_hhid_smpl_Enue( mess, usb_ghhid_open_prm.p_fnstr, usb_ghhid_open_prm.devadr,
                    usb_ghhid_open_prm.speed );
                break;

            /* HID data Receive Request */
            case USB_HHID_TCMD_RECEIVE:
                /* data Receive request */
                usb_fnInfo[mess->fn].read_length = mess->tranlen;
                mess->ip = usb_fnInfo[mess->fn].usb_ip;
                mess->ipp = usb_fnInfo[mess->fn].usb_ipp;
                err = usb_hhid_data_trans(mess, usb_fnInfo[mess->fn].inPipeNo, mess->tranlen,
                    (uint8_t*)mess->tranadr, &usb_hhid_InTransferResult);
                /* Transfer Start request check */
                if( err != USB_OK )
                {
                    /* Transfer Start request NG */
                    usb_fnInfo[mess->fn].read_state = USB_CTL_ERR_PROCESS_ERR;

                    /* User application call back */
                    usb_fnInfo[mess->fn].read_length = -1;
                    (*usb_fnInfo[mess->fn].read_complete)( mess, 0, 0 );
                }
                break;

            /* HID data Send Request */
            case USB_HHID_TCMD_SEND:
                /* data send request*/
                usb_fnInfo[mess->fn].write_length = mess->tranlen;
                mess->ip = usb_fnInfo[mess->fn].usb_ip;
                mess->ipp = usb_fnInfo[mess->fn].usb_ipp;
                err = usb_hhid_data_trans(mess, usb_fnInfo[mess->fn].outPipeNo, mess->tranlen,
                    (uint8_t*)mess->tranadr, &usb_hhid_OutTransferResult);
                /* Transfer Start request check */
                if( err != USB_OK )
                {
                    /* Transfer Start request NG */
                    usb_fnInfo[mess->fn].write_state = USB_CTL_ERR_PROCESS_ERR;

                    /* User application call back */
                    usb_fnInfo[mess->fn].write_length = -1;
                    (*usb_fnInfo[mess->fn].write_complete)( mess, 0, 0 );
                }
                break;
#else   /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */
            case USB_HHID_TCMD_OPEN:
                usb_hhid_enumeration_sequence(mess);
                break;

            case USB_HHID_TCMD_DATA_TRANS:
                /* data Receive request */
                usb_shhid_data_tran_complete = mess->complete;
                err = usb_hhid_data_trans(mess, mess->keyword, mess->tranlen,
                    (uint8_t*)mess->tranadr, &usb_hhid_InTransferResult);
                /* Transfer Start request check */
                if( err != USB_OK )
                {
                    (mess->complete)( mess, 0, 0 );
                }
                break;
#endif  /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */

            case USB_HHID_TCMD_CLASS_REQ:
                err = usb_hhid_class_request_process();
                break;
            default:
                break;
        }
        err = R_USB_REL_BLK(USB_HHID_MPL,(USB_MH_t)mess);
        if( err != USB_E_OK )
        {
            USB_PRINTF0("### USB HHID Task rel_blk error\n");
        }
    }
    else
    {
//      USB_PRINTF0("Host HID driver rcv_msg error !\n");
        return;
    }
}   /* eof usb_hhid_task() */

/******************************************************************************
Function Name   : usb_hhid_InTransferResult
Description     : data Transfer
Argument        : USB_UTR_t *mess           : Utr Pointer
Return          : none
******************************************************************************/
void usb_hhid_InTransferResult(USB_UTR_t *mess, uint16_t data1, uint16_t data2)
{
#if USB_ANSIIO_PP == USB_ANSIIO_USE_PP
    /* Get File No. */
    if( mess->fn != -1 )
    {
        /* Read complete */
        usb_fnInfo[mess->fn].read_state = USB_CTL_ERR_PROCESS_COMPLETE;
        if(mess->status == USB_DATA_OK)
        {
            /* Receive size = Request size */
        }
        else if(mess->status == USB_DATA_SHT)
        {
            /* Receive size < Request size */
            usb_fnInfo[mess->fn].read_length -= mess->tranlen;
        }
        else
        {
            usb_fnInfo[mess->fn].read_length = -1;
        }
        (*usb_fnInfo[mess->fn].read_complete)( mess, 0, 0 );
    }
    else
    {
        /* Error loop */
//      while(1);
    }
#else
    (*usb_shhid_data_tran_complete)(mess,0,0);
#endif  /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */
}   /* eof usb_hhid_InTransferResult() */

#if USB_ANSIIO_PP == USB_ANSIIO_USE_PP
/******************************************************************************
Function Name   : usb_hhid_OutTransferResult
Description     : data Transfer
Argument        : USB_UTR_t *mess           : Utr Pointer
Return          : none
******************************************************************************/
void usb_hhid_OutTransferResult(USB_UTR_t *mess, uint16_t data1, uint16_t data2)
{
    /* Get File No. */
    if( mess->fn != -1 )
    {
        /* Write complete */
        usb_fnInfo[mess->fn].write_state = USB_CTL_ERR_PROCESS_COMPLETE;
        /* Transfer Status check */
        if(mess->status != USB_DATA_NONE)
        {
            /* Transfer NG */
            usb_fnInfo[mess->fn].write_length = -1;
        }
        (*usb_fnInfo[mess->fn].write_complete)( mess, 0, 0 );
    }
    else
    {
        /* Error loop */
//      while(1);
    }
}   /* eof usb_hhid_OutTransferResult() */
#endif  /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */

/******************************************************************************
Function Name   : usb_hhid_pipe_info
Description     : pipe information table set
Argument        : uint8_t *table          : Descriptor store address
                : uint16_t speed          : Conect Dpeed(Hi/Full/...)
                : uint16_t length         : Descriptor total lenght
                : uint16_t count
Return value    : uint16_t                : Error info
******************************************************************************/
uint16_t usb_hhid_pipe_info(uint8_t *table, uint16_t speed, uint16_t length, uint16_t count)
{
    /* Offset for Descriptor Top Address */
    uint16_t        ofdsc;
    uint16_t        retval;

    /* Check configuration descriptor */
    if( table[1] != USB_DT_INTERFACE )
    {
        USB_PRINTF0("### Descriptor is not Interface descriptor.\n");
        return  USB_ERROR;
    }

    /* Descriptor Address set */
    ofdsc = table[0];
    /* Loop for Endpoint Descriptor search */
    while( ofdsc < length )
    {
        /* Branch descriptor type */
        switch(table[ofdsc + 1])
        {
        case USB_DT_DEVICE:
        case USB_DT_CONFIGURATION:
        case USB_DT_STRING:
        case USB_DT_INTERFACE:
            USB_PRINTF0("### Endpoint Descriptor error(HUB).\n");
            return USB_ERROR;
            break;

        /* Endpoint Descriptor */
        case USB_DT_ENDPOINT:
            /* Check pipe information */
            retval = R_usb_hstd_ChkPipeInfo(speed, (uint16_t*)&usb_ghhid_SmplEpTbl[count], (uint8_t*)&table[ofdsc]);

            if( retval == USB_DIR_H_IN )
            {
                /* Get Max Packet size */
                usb_ghhid_maxps = (uint16_t)table[ofdsc + 4];
                usb_ghhid_maxps |= (uint16_t)table[ofdsc + 5] << 8;
                return USB_DONE;
            }
            /* Next descriptor point set */
            ofdsc   += table[ofdsc];
            break;

        case USB_DT_DEVICE_QUALIFIER:
        case USB_DT_OTHER_SPEED_CONF:
        case USB_DT_INTERFACE_POWER:
            USB_PRINTF0("### Endpoint Descriptor error.\n");
            return USB_ERROR;
            break;

        default:
            /* Next descriptor point set */
            ofdsc   +=  table[ofdsc];
            break;
        }
    }
    return USB_ERROR;
}   /* eof usb_hhid_pipe_info() */

/******************************************************************************
Function Name   : usb_hhid_class_request_process
Description     : Host HID Class Request Process
Arguments       : none
Return value    : USB_ER_t                 : USB_ERROR/USB_DONE
******************************************************************************/
USB_ER_t usb_hhid_class_request_process(void)
{
    USB_ER_t        err;

    switch(usb_ghhid_class_request_parm.bRequestCode)
    {
        case USB_HID_GET_HID_DESCRIPTOR:
            /* Setup message bmRequestType and bRequest set */
            usb_shhid_class_request_setup[0]    = USB_GET_DESCRIPTOR | USB_DEV_TO_HOST | USB_STANDARD | USB_INTERFACE;
            /* Setup message Report Type and Report ID set */
            usb_shhid_class_request_setup[1]    = (uint16_t)USB_HID_GET_HID_DESCRIPTOR;
            /* Setup message Interface set */
            usb_shhid_class_request_setup[2]    = (uint16_t)0x0000;
            /* Setup message Report Length set */
            usb_shhid_class_request_setup[3]    = (uint16_t)9;
            break;

        case USB_HID_GET_REPORT_DESCRIPTOR:
            /* Setup message bmRequestType and bRequest set */
            usb_shhid_class_request_setup[0]    = USB_GET_DESCRIPTOR | USB_DEV_TO_HOST | USB_STANDARD | USB_INTERFACE;
            /* Setup message Report Type and Report ID set */
            usb_shhid_class_request_setup[1]    = (uint16_t)USB_HID_GET_REPORT_DESCRIPTOR;
            /* Setup message Interface set */
            usb_shhid_class_request_setup[2]    = (uint16_t)0x0000;
            /* Setup message Report Length set */
            usb_shhid_class_request_setup[3]    = (uint16_t)usb_ghhid_class_request_parm.tranlen;
            break;

        case USB_HID_GET_PHYSICAL_DESCRIPTOR:   /* Setup message bmRequestType and bRequest set */
            usb_shhid_class_request_setup[0]    = USB_GET_DESCRIPTOR | USB_DEV_TO_HOST | USB_STANDARD | USB_INTERFACE;
            /* Setup message Report Type and Report ID set */
            usb_shhid_class_request_setup[1]
                = (uint16_t)USB_HID_GET_PHYSICAL_DESCRIPTOR | usb_ghhid_class_request_parm.index;
            /* Setup message Interface set */
            usb_shhid_class_request_setup[2]    = (uint16_t)0x0000;
            /* Setup message Report Length set */
            usb_shhid_class_request_setup[3]    = (uint16_t)usb_ghhid_class_request_parm.tranlen;
            break;

        case USB_HID_SET_REPORT:
            /* Setup message bmRequestType and bRequest set */
            usb_shhid_class_request_setup[0]    = USB_HID_SET_REPORT | USB_HOST_TO_DEV | USB_CLASS | USB_INTERFACE;
            /* Setup message Report Type and Report ID set */
            usb_shhid_class_request_setup[1]    = (uint16_t)USB_HID_SET_DT_TYPE((uint16_t)USB_HID_DT_TYPE_SETREPORT);
            /* Setup message Interface set */
            usb_shhid_class_request_setup[2]    = (uint16_t)0x0000;
            /* Setup message Report Length set */
            usb_shhid_class_request_setup[3]    = (uint16_t)usb_ghhid_class_request_parm.tranlen;
            break;

        case USB_HID_GET_REPORT:
            /* Setup message bmRequestType and bRequest set */
            usb_shhid_class_request_setup[0]    = USB_HID_GET_REPORT | USB_DEV_TO_HOST | USB_CLASS | USB_INTERFACE;
            /* Setup message Report Type and Report ID set */
            usb_shhid_class_request_setup[1]    = (uint16_t)USB_HID_SET_DT_TYPE((uint16_t)USB_HID_DT_TYPE_GETREPORT);
            /* Setup message Interface set */
            usb_shhid_class_request_setup[2]    = (uint16_t)0x0000;
            /* Setup message Report Length set */
            usb_shhid_class_request_setup[3]    = (uint16_t)usb_ghhid_class_request_parm.tranlen;
            break;

        case USB_HID_SET_IDLE:
            /* Setup message bmRequestType and bRequest set */
            usb_shhid_class_request_setup[0]    = USB_HID_SET_IDLE | USB_HOST_TO_DEV | USB_CLASS | USB_INTERFACE;
            /* Setup message Report Type and Report ID set */
            usb_shhid_class_request_setup[1]    = (usb_ghhid_class_request_parm.duration << 8);
            /* Setup message Interface set */
            usb_shhid_class_request_setup[2]    = (uint16_t)0x0000;
            /* Setup message Report Length set */
            usb_shhid_class_request_setup[3]    = (uint16_t)0;
            break;

        case USB_HID_GET_IDLE:
            /* Setup message bmRequestType and bRequest set */
            usb_shhid_class_request_setup[0]    = USB_HID_GET_IDLE | USB_DEV_TO_HOST | USB_CLASS | USB_INTERFACE;
            /* Setup message Report Type and Report ID set */
            usb_shhid_class_request_setup[1]    = (uint16_t)0;
            /* Setup message Interface set */
            usb_shhid_class_request_setup[2]    = (uint16_t)0x0000;
            /* Setup message Report Length set */
            usb_shhid_class_request_setup[3]    = (uint16_t)1;
            break;

        case USB_HID_SET_PROTOCOL:
            /* Setup message bmRequestType and bRequest set */
            usb_shhid_class_request_setup[0]    = USB_HID_SET_PROTOCOL | USB_HOST_TO_DEV | USB_CLASS | USB_INTERFACE;
            /* Setup message Report Type and Report ID set */
            usb_shhid_class_request_setup[1]    = (uint16_t)usb_ghhid_class_request_parm.set_protocol;
            /* Setup message Interface set */
            usb_shhid_class_request_setup[2]    = (uint16_t)0x0000;
            /* Setup message Report Length set */
            usb_shhid_class_request_setup[3]    = (uint16_t)0;
            break;

        case USB_HID_GET_PROTOCOL:
            /* Setup message bmRequestType and bRequest set */
            usb_shhid_class_request_setup[0]    = USB_HID_GET_PROTOCOL | USB_DEV_TO_HOST | USB_CLASS | USB_INTERFACE;
            /* Setup message wValue set */
            usb_shhid_class_request_setup[1]    = (uint16_t)0x0000;
            /* Setup message Interface set */
            usb_shhid_class_request_setup[2]    = (uint16_t)0x0000;
            /* Setup message Report Length set */
            usb_shhid_class_request_setup[3]    = (uint16_t)1;
            break;

        default:
            break;
    }

    usb_shhid_class_control.tranadr     = usb_ghhid_class_request_parm.tranadr;
    usb_shhid_class_control.complete    = usb_ghhid_class_request_parm.complete;
    usb_shhid_class_control.tranlen     = usb_ghhid_class_request_parm.tranlen;
    usb_shhid_class_control.keyword     = USB_PIPE0;
    usb_shhid_class_control.setup       = usb_shhid_class_request_setup;
    usb_shhid_class_control.segment     = USB_TRAN_END;
    usb_shhid_class_request_setup[4]    = usb_ghhid_class_request_parm.devadr;

    usb_shhid_class_control.ip      = usb_ghhid_class_request_parm.ip;
    usb_shhid_class_control.ipp     = usb_ghhid_class_request_parm.ipp;

    err = R_usb_hstd_TransferStart(&usb_shhid_class_control);

    return err;
}   /* eof usb_hhid_class_request_process() */

/******************************************************************************
Function Name   : usb_hhid_get_string_desc
Description     : Set GetDescriptor
Arguments       : uint16_t addr            : device address
                : uint16_t string          : descriptor index
                : USB_CB_t complete        : callback function
Return value    : uint16_t                 : error info
******************************************************************************/
uint16_t usb_hhid_get_string_desc(USB_UTR_t *ptr, uint16_t addr, uint16_t string, USB_CB_t complete)
{
    uint16_t        i;

    if( string == 0 )
    {
        usb_shhid_class_request_setup[2] = (uint16_t)0x0000;
        usb_shhid_class_request_setup[3] = (uint16_t)0x0004;
    }
    else
    {
        /* Set LanguageID */
        usb_shhid_class_request_setup[2] = (uint16_t)(usb_ghhid_str_desc_data[2]);
        usb_shhid_class_request_setup[2] |= (uint16_t)((uint16_t)(usb_ghhid_str_desc_data[3]) << 8);
        usb_shhid_class_request_setup[3] = (uint16_t)USB_HHID_CLSDATASIZE;
    }
    usb_shhid_class_request_setup[0]    = USB_GET_DESCRIPTOR | USB_DEV_TO_HOST | USB_STANDARD | USB_DEVICE;
    usb_shhid_class_request_setup[1]    = (uint16_t)(USB_STRING_DESCRIPTOR + string);
    usb_shhid_class_request_setup[4]    = addr;

    for(i = 0; i < usb_shhid_class_request_setup[3]; i++)
    {
        usb_ghhid_str_desc_data[i] = (uint8_t)0xFF;
    }

    return usb_hhid_cmd_submit(ptr, complete);
}   /* eof usb_hhid_get_string_desc() */

/******************************************************************************
Function Name   : usb_hstd_CmdSubmit
Description     : command submit
Arguments       : USB_UTR_t *ptr
                  USB_CB_t  complete         : callback info
Return value    : uint16_t                   : USB_DONE
******************************************************************************/
uint16_t usb_hhid_cmd_submit(USB_UTR_t *ptr, USB_CB_t complete)
{
    usb_shhid_class_control.tranadr = (void*)usb_ghhid_str_desc_data;
    usb_shhid_class_control.complete    = complete;
    usb_shhid_class_control.tranlen = (uint32_t)usb_shhid_class_request_setup[3];
    usb_shhid_class_control.keyword = USB_PIPE0;
    usb_shhid_class_control.setup   = usb_shhid_class_request_setup;
    usb_shhid_class_control.segment = USB_TRAN_END;

    usb_shhid_class_control.ip      = ptr->ip;
    usb_shhid_class_control.ipp     = ptr->ipp;

    R_usb_hstd_TransferStart(&usb_shhid_class_control);
    
    return USB_DONE;
}   /* eof usb_hhid_cmd_submit() */

/******************************************************************************
Function Name   : usb_hhid_hub_registration
Description     : Hub data registration
Argument        : none
Return          : none
******************************************************************************/
void usb_hhid_hub_registration(USB_UTR_t *ptr)
{
    USB_HCDREG_t    driver;

    R_usb_cstd_SetTaskPri(USB_HUB_TSK,  USB_PRI_3);

    /* Driver registration */
    driver.ifclass      = (uint16_t)USB_IFCLS_HUB;          /* Interface Class : HUB */
    driver.tpl          = (uint16_t*)&usb_ghhub_TPL;        /* Target peripheral list */
    driver.pipetbl      = (uint16_t*)&usb_ghhub_DefEPTbl;   /* Pipe Define Table address */
    driver.classinit    = &usb_hhub_Initial;                /* Driver init */
    driver.classcheck   = &usb_hhub_ChkClass;               /* Driver check */
    driver.devconfig    = (USB_CB_INFO_t)&R_usb_hhub_Open;  /* Device configuered */
    driver.devdetach    = (USB_CB_INFO_t)&R_usb_hhub_Close; /* Device detach */
    driver.devsuspend   = &usb_cstd_DummyFunction;          /* Device suspend */
    driver.devresume    = &usb_cstd_DummyFunction;          /* Device resume */
    driver.overcurrent  = &usb_cstd_DummyFunction;          /* Device over current */

    R_usb_hhub_Registration(ptr, &driver);
}   /* eof usb_hhid_hub_registration() */

/******************************************************************************
End  Of File
******************************************************************************/
