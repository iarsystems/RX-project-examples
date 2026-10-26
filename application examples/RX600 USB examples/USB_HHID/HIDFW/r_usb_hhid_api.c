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
* File Name    : r_usb_hhid_api.c
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
*         : 10.02.2011 1.00    Host HID class(Mass Production Release)
*         : 17.06.2011 1.02    Support USB Basic Fw V110
*         : 31.05.2011 2.00    Support USB Basic FW v. 2.00.
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
#include "r_usb_api.h"
#include "r_usb_class_usrcfg.h"
/******************************************************************************
Constant macro definitions
******************************************************************************/
#define USB_B_INTERFACE_PROTOCOL    7   /* Interface Descriptor bInterfaceProtocol */

/*****************************************************************************
Enumerated Types
******************************************************************************/

/******************************************************************************
External variables and functions
******************************************************************************/
extern uint8_t      *usb_ghhid_ConfigTable;         /* Configuration Descriptor Table */
extern uint16_t     usb_ghhid_Devaddr;              /* Device Address */
extern uint16_t     usb_ghhid_maxps;                /* Max Packet Size */
extern uint8_t      *usb_ghhid_interface_table;     /* Interface Descriptor Table */
extern uint8_t      *usb_hcls_SmplDeviceTable;
extern uint8_t      *usb_ghhid_device_table;        /* Device Descriptor Table */
extern uint16_t     usb_ghhid_Speed;                /* Device speed */
extern uint16_t     usb_ghhid_enum_seq;             /* Enumeration Sequence */
extern USB_UTR_t    usb_ghhid_transfer_message;     /* USB Data Transfer Request for HCD */
extern uint8_t      usb_ghhid_str_desc_data[USB_HHID_CLSDATASIZE];

extern void     R_usb_cstd_UsbIpInit( USB_UTR_t *ptr, uint16_t usb_mode );
extern void     usb_hhid_hub_registration(USB_UTR_t *ptr);
extern uint16_t usb_hstd_GetRootport(USB_UTR_t *ptr, uint16_t addr);
extern uint16_t usb_hstd_CheckDescriptor(uint8_t *table, uint16_t spec);
extern uint16_t usb_hstd_GetStringDescriptor(USB_UTR_t *ptr, uint16_t devaddr, uint16_t index);
extern void     usb_hapl_task_start( USB_UTR_t *ptr );
extern void     usb_hapl_registration(USB_UTR_t *ptr);
extern USB_ER_t usb_hhid_data_trans(USB_UTR_t *ptr, uint16_t pipe, uint32_t size, uint8_t *table,
    USB_CB_t complete);

/******************************************************************************
Section    <Section Difinition> , "Project Sections"
******************************************************************************/
#pragma section="_hid"

/******************************************************************************
Private global variables and functions
******************************************************************************/

/******************************************************************************
Renesas Abstracted USB Driver functions
******************************************************************************/

/******************************************************************************
Function Name   : R_usb_hhid_task
Description     : Call Host HID task
Arguments       : USB_VP_INT_t stacd        : start code
Return value    : none
******************************************************************************/
void R_usb_hhid_task(USB_VP_INT_t stacd)
{
    /* Host HID Task call */
    usb_hhid_task( stacd );
} /* eof R_usb_hhid_task */

/******************************************************************************
Function Name   : R_usb_hhid_DriverRelease
Description     : Release of Host Human Intergface Devices class Driver
Arguments       : USB_UTR_t *ptr             : IP info (mode, IP no., reg address).
Return value    : none
******************************************************************************/
void R_usb_hhid_DriverRelease(USB_UTR_t *ptr)
{
    /* Release the Host HID Device class Driver. */
    R_usb_hstd_DriverRelease(ptr, USB_IFCLS_HID);
}   /* eof R_usb_hhid_DriverRelease() */

/******************************************************************************
Function Name   : R_usb_hhid_TransferEnd
Description     : End of data transfer
Arguments       : USB_UTR_t *ptr             : IP info (mode, IP no., reg address).
Return value    : USB_ER_t                 ; USB_E_OK etc
******************************************************************************/
USB_ER_t R_usb_hhid_TransferEnd(USB_UTR_t *ptr)
{
    USB_ER_t    err;

#ifndef USB_HHID_GET_REPORT_PIPE0
    /* End of data transfer(USB1-IP host function) */
    err = R_usb_hstd_TransferEnd(ptr, USB_HHID_USE_PIPE, USB_DATA_STOP);
#else   /* USB_HHID_GET_REPORT_PIPE0 == USB_OFF */
    /* End of data transfer(USB1-IP host function) */
    err = R_usb_hstd_TransferEnd(ptr, USB_PIPE0, USB_DATA_STOP);
#endif  /* USB_HHID_GET_REPORT_PIPE0 == USB_OFF */

    return err;
}   /* eof R_usb_hhid_TransferEnd() */

/******************************************************************************
Function Name   : R_usb_hhid_DeviceInformation
Description     : Get Device Information
Arguments       : USB_UTR_t *ptr             : IP info (mode, IP no., reg address).
                : uint16_t  *deviceinfo      : Table Pointer
Return value    : none
******************************************************************************/
void R_usb_hhid_DeviceInformation(USB_UTR_t *ptr, uint16_t *deviceinfo)
{
    /* Return to device information */
    R_usb_hstd_DeviceInformation(ptr, usb_ghhid_Devaddr, deviceinfo);
}   /* eof R_usb_hhid_DeviceInformation() */

/******************************************************************************
Function Name   : R_usb_hhid_ChangeDeviceState
Description     : Host HID Device State Change
Arguments       : USB_UTR_t *ptr             : IP info (mode, IP no., reg address).
                : uint16_t  msginfo           : Message Info
Return value    : none
******************************************************************************/
void R_usb_hhid_ChangeDeviceState(USB_UTR_t *ptr, uint16_t msginfo)
{
    /* Change HUB down port device status */
    R_usb_hstd_MgrChangeDeviceState(ptr, (USB_CB_INFO_t)&usb_cstd_DummyFunction, msginfo, usb_ghhid_Devaddr);
}   /* eof R_usb_hhid_ChangeDeviceState() */

/******************************************************************************
Function Name   : R_usb_hhid_GetReportLength
Description     : Get Report Length
Argument        : none
Return value    : uint16_t                : Report Length
******************************************************************************/
uint16_t R_usb_hhid_GetReportLength(void)
{
    /* Max Packet Size */
    return usb_ghhid_maxps;
}   /* eof R_usb_hhid_GetReportLength() */

/******************************************************************************
Function Name   : R_usb_hhid_SetPipeRegistration
Description     : Host HID Pipe registration
Arguments       : USB_UTR_t *ptr             : IP info (mode, IP no., reg address).
                : uint16_t  devadr          : Device address
Return          : none
******************************************************************************/
void R_usb_hhid_SetPipeRegistration(USB_UTR_t *ptr, uint16_t devadr)
{
    /* Device Address Set for Endpoint Table */
    usb_ghhid_SmplEpTbl[3] |= (uint16_t)(devadr << USB_DEVADDRBIT);

    /* Set pipe configuration request(Interrupt IN) */
    R_usb_hstd_SetPipeRegistration(ptr, (uint16_t*)&usb_ghhid_SmplEpTbl[0], usb_ghhid_SmplEpTbl[0]);
}   /* eof R_usb_hhid_SetPipeRegistration() */

/******************************************************************************
Function Name   : R_usb_hhid_get_hid_protocol
Description     : Get HID Protocol
Argument        : none
Return          : Hid Protocol
******************************************************************************/
uint8_t R_usb_hhid_get_hid_protocol(void)
{
    uint8_t         *table;

    /* Get Configuration descriptor Address */
    table   = usb_ghhid_interface_table;

    /* table[7]:Interface Descriptor-bInterfaceProtocol */
    return table[USB_B_INTERFACE_PROTOCOL];
}
/******************************************************************************
End of function R_usb_hhid_get_hid_protocol
******************************************************************************/

/******************************************************************************
Function Name   : R_usb_hhid_driver_start
Description     : USB Host Initialize process
Arguments       : USB_UTR_t *ptr             : IP info (mode, IP no., reg address).
Return          : none
******************************************************************************/
void R_usb_hhid_driver_start( USB_UTR_t *ptr )
{
    R_usb_cstd_SetTaskPri(USB_HHID_TSK, USB_PRI_3);             /* Host HID task priority set */
}
/******************************************************************************
End of function R_usb_hvendor_driver_start()
******************************************************************************/

/******************************************************************************
Function Name   : R_usb_hhid_class_request
Description     : class request message Send for Host HID task
Arguments       : void *pram                : class request Parametor
Return value    : USB_ER_t                             : Error info
******************************************************************************/
USB_ER_t R_usb_hhid_class_request(void *pram)
{
    USB_MH_t    p_blf;
    USB_ER_t    err, err2;
    USB_UTR_t   *cp;

    /* Class request Parametor Set */
    usb_ghhid_class_request_parm = *(USB_HHID_CLASS_REQUEST_PARM_t*)pram;

    /* Get mem block from pool. */
    err = R_USB_PGET_BLK(USB_HHID_MPL, &p_blf);
    if( err == USB_E_OK )
    {
        cp = (USB_UTR_t*)p_blf;
        cp->msghead = (USB_MH_t)NULL;
        cp->msginfo = (uint16_t)USB_HHID_TCMD_CLASS_REQ;        /* Message information :Class request */
        cp->keyword = (uint16_t)USB_NOT_USED;                   /* Not used */
        cp->status      = (uint16_t)USB_NOT_USED;               /* Not used */
        cp->complete    = (USB_CB_t)USB_NOT_USED;               /* Not used */

        /* Send message */
        err = R_USB_SND_MSG(USB_HHID_MBX, (USB_MSG_t*)p_blf);
        if( err != USB_E_OK )
        {
            /* Send message failure */
            USB_PRINTF1("### hhidSerialStateNotification snd_msg error (%ld)\n", err);
            err2 = R_USB_REL_BLK(USB_HHID_MPL, (USB_MH_t)p_blf);
            if( err2 != USB_E_OK )
            {
                /* Release memoryblock failure */
                USB_PRINTF1("### hhidSerialStateNotification rel_blk error (%ld)\n", err2);
            }
        }
    }
    else
    {
        /* Get momoryblock failure */
        USB_PRINTF1("### hhidSerialStateNotification pget_blk error (%ld)\n", err);
    }
    return err;
}
/******************************************************************************
End of function R_usb_hcdc_class_request
******************************************************************************/

/******************************************************************************
Function Name   : R_usb_hhid_PipeTransferExample
Description     : Start of data transfer
Arguments       : USB_UTR_t *ptr            : IP info (mode, IP no., reg address).
                : uint8_t *table            : data address
                : uint32_t size             : data Size
Return value    : none
******************************************************************************/
USB_ER_t R_usb_hhid_PipeTransferExample(USB_UTR_t *ptr, uint8_t *table, uint32_t size,
    USB_CB_t complete)
{
    USB_MH_t            p_blf;
    USB_ER_t            err;
    USB_CLSINFO_t       *cp;

    /* Get mem block from pool. */
    if( R_USB_PGET_BLK(USB_HHID_MPL, &p_blf) == USB_E_OK )
    {
        cp = (USB_CLSINFO_t*)p_blf;
        cp->msghead = (USB_MH_t)NULL;
        cp->msginfo = USB_HHID_TCMD_DATA_TRANS;                 /* Set message information :USB transfer. */
        cp->keyword = USB_HHID_USE_PIPE;                        /* Set pipe no. for USB transfer. */
        cp->tranadr = table;                                    /* Transfer data start address */
        cp->tranlen = size;                                     /* Transfer data length */
        cp->complete = complete;                                /* Callback function */
        cp->ip = ptr->ip;                                       /* IP number(0or1) */
        cp->ipp = ptr->ipp;                                     /* IP address (USB0 or USB1) */

        /* Send message */
        err = R_USB_SND_MSG( USB_HHID_MBX, (USB_MSG_t*)cp );
        if( err != USB_E_OK )
        {
            /* Error */
            err = R_USB_REL_BLK(USB_HHID_MPL,(USB_MH_t)p_blf);
            USB_PRINTF0( "### usb_hhid_read_data function snd_msg error\n");
            /* Transfer start request send NG */
        }
    }
    else
    {
        /* Error */
        USB_PRINTF0( "### usb_hhid_read_data function pget_blk error\n");
        /* Get mem block from pool. NG */
    }

    return err;
}   /* eof R_usb_hhid_PipeTransferExample() */

/******************************************************************************
Function Name   : R_usb_hhid_class_check
Description     : 
Argument        : USB_UTR_t *ptr
                : uint16_t  **table
Return value    : none
******************************************************************************/
void R_usb_hhid_class_check(USB_UTR_t *ptr, uint16_t **table)
{
#if USB_ANSIIO_PP == USB_ANSIIO_USE_PP
    uint8_t         *config_table;
    uint16_t        *result;

    usb_hcls_SmplDeviceTable    = (uint8_t*)((uint16_t*)table[0]);
    config_table        = (uint8_t*)((uint16_t*)table[1]);
    usb_ghhid_Devaddr   = (uint16_t)(*table[7]);
    /* Configuration Descriptor address store */
    usb_ghhid_ConfigTable = config_table;
    usb_ghhid_interface_table = (uint8_t*)((uint16_t*)table[2]);    /* Interface Descriptor Table */
    result  = table[3];
    *result = USB_DONE;
#else   /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */
    USB_ER_t        Err;

    usb_ghhid_device_table          = (uint8_t*)((uint16_t*)table[0]);  /* Device Descriptor Table */
    usb_ghhid_ConfigTable           = (uint8_t*)((uint16_t*)table[1]);  /* Configuration Descriptor Table */
    usb_ghhid_interface_table       = (uint8_t*)((uint16_t*)table[2]);  /* Interface Descriptor Table */
    usb_ghhid_Speed                 = (uint16_t)(*table[6]);            /* Device speed */
    usb_ghhid_Devaddr               = (uint16_t)(*table[7]);            /* Device Address */

    /* Enumeration Sequence String Descriptor #0 receive request */
    usb_ghhid_enum_seq  = (uint16_t)USB_HHID_ENUM_STR_DT0_REQ;
    /* Descriptor check result */
    *table[3]                       = USB_DONE;

    usb_ghhid_transfer_message.ip = ptr->ip;
    usb_ghhid_transfer_message.ipp = ptr->ipp;
    usb_ghhid_transfer_message.msginfo = USB_HHID_TCMD_OPEN;

    /* Send message(Enumearation check start) */
    Err = R_USB_SND_MSG(USB_HHID_MBX, (USB_MSG_t*)&usb_ghhid_transfer_message);
    if (Err != USB_E_OK)
    {
        USB_PRINTF1("Host Sample snd_msg error %x\n", Err);
    }
#endif  /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */
}   /* eof R_usb_hhid_class_check() */

/******************************************************************************
End  Of File
******************************************************************************/
