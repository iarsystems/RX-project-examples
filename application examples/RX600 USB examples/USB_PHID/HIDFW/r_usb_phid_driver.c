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
* File Name    : r_usb_PhidDriver.c
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
#include <string.h>
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

#include "r_usb_class_usrcfg.h"
#include "r_usb_phid_define.h"      /* HID class define */
#include "r_usb_phid_extern.h"
#include "rx_rsk_extern.h"
/******************************************************************************
Section    <Section Difinition> , "Project Sections"
******************************************************************************/
#pragma section="_hid"

/******************************************************************************
Private global variables and functions
******************************************************************************/
USB_UTR_t       usb_gphid_SmplTrnMsg;
uint8_t         usb_gphid_ClassData[18];
uint8_t         usb_gphid_TransData[8];
USB_REQUEST_t   usb_gphid_Request;
uint8_t         usb_gphid_Duration;
uint16_t        usb_gphid_DurationTmr;

static  void            usb_phid_get_descriptor_interface(USB_UTR_t *ptr, USB_REQUEST_t *request);
static  USB_ER_t        usb_phid_data_trans(USB_UTR_t *ptr, uint16_t pipe, uint32_t size, uint8_t *table);
static  void            usb_phid_in_trans_result(USB_UTR_t *mess, uint16_t data1, uint16_t data2);

#if USB_ANSIIO_PP != USB_ANSIIO_USE_PP
USB_CB_t        usb_gphid_tx_cb;            /* Call Back Function Info */
#endif  /* USB_ANSIIO_PP != USB_ANSIIO_USE_PP */

/*****************************************************************************
External variables and functions
******************************************************************************/
extern void usb_pstd_SetStall( USB_UTR_t *ptr, uint16_t pipe );

extern void     usb_phid_apl_keyboard_led_control( uint8_t leddata );
extern void     usb_phid_demo_mode_report( USB_UTR_t *ptr, uint8_t *recdata );


/******************************************************************************
Renesas Abstracted USB Driver functions
******************************************************************************/

/******************************************************************************
Function Name   : R_usb_phid_task
Description     : Peripheral HID driver Task
Arguments       : none
Return value    : none
******************************************************************************/
void R_usb_phid_task( USB_VP_INT_t stacd )
{
    USB_UTR_t*      mess;
    USB_ER_t        err;
    uint16_t        reqlength, i;
    uint8_t         *cfgtbl, *rpttbl;
    uint32_t        tblsize;

    /* Receive message */
    err = R_USB_TRCV_MSG( USB_PHID_MBX, (USB_MSG_t**)&mess, (uint16_t)5000 );
    if( err == USB_OK )
    {
        /* Branch by the message information. */
        switch( mess->msginfo )
        {
        /* HID Class Request : Get_Idle Request */
        case    USB_GET_IDLE:
            /* Return duration by the Class Request Set Idle. */
            R_usb_pstd_ControlRead( mess, (uint32_t)1, (uint8_t*)(&usb_gphid_Duration ) );
            break;
        
        /* HID Class Request : Set_Idle Request */
        case    USB_SET_IDLE:
            /* Store duration */
            usb_gphid_Duration = (uint8_t)USB_GET_DT_TYPE( usb_gphid_Request.ReqValue );
            usb_gphid_DurationTmr = (uint16_t)( usb_gphid_Duration * 4 );   /* Duration Timer not used. */
            usb_cstd_SetBuf( mess, (uint16_t)USB_PIPE0 );                   /* Set pipe PID_BUF */
            R_usb_pstd_ControlEnd( mess, (uint16_t)USB_CTRL_END );          /* End control transfer. */
            break;
        
        /* HID Class Request : Get_Report Request */
        case    USB_GET_REPORT:
            /* Get transfer(report) length */
            reqlength = R_usb_phid_transfer_length();

            /* Set transfer data. (Report data) */
            for( i = 0; i < reqlength; i++ )
            {
                usb_gphid_ClassData[i] = usb_gphid_TransData[i];
            }

            /* Report transfer */
            if( usb_gphid_Request.ReqLength < reqlength )
            {
                R_usb_pstd_ControlRead( mess, (uint32_t)usb_gphid_Request.ReqLength,
                    (uint8_t*)&usb_gphid_ClassData );
            }
            else
            {
                R_usb_pstd_ControlRead( mess, (uint32_t)reqlength, (uint8_t*)&usb_gphid_ClassData );
            }
            break;
        
        /* HID Class Request : Set_Report Request data stage */
        case    USB_SET_REPORT:
#if USB_PHID_APL_MODE == USB_PHID_DEMO_MODE
            /* Receive data by the class request Set_Report.(Demo mode) */
            R_usb_pstd_ControlWrite( mess, (uint32_t)17, (uint8_t*)&usb_gphid_ClassData );
#endif  /* Peripheral HID Application demo mode */

#if USB_PHID_APL_MODE == USB_PHID_KEYBOARD_MODE
            /* Receive data by the class request Set_Report.(Keyboard mode) */
            R_usb_pstd_ControlWrite( mess, (uint32_t)1, (uint8_t*)&usb_gphid_ClassData );
#endif  /* Peripheral HID Application keyboard mode */

#if USB_PHID_APL_MODE == USB_PHID_MOUSE_MODE
            /* Receive data by the class request Set_Report.(Mouse mode) */
            R_usb_pstd_ControlWrite( mess, (uint32_t)1, (uint8_t*)&usb_gphid_ClassData );
#endif  /* Peripheral HID Application mouse mode */
            break;
        
        /* HID Class Request : Set_Report Request status stage */
        case    USB_SET_REPORT_STATUS:
#if USB_PHID_APL_MODE == USB_PHID_DEMO_MODE
            /* RSK demo ( Toggle LED or Read ADC or Update LCD ) */
            usb_phid_demo_mode_report( mess, (uint8_t*)&usb_gphid_ClassData );
            usb_cstd_SetBuf( mess, (uint16_t)USB_PIPE0 );                       /* Set pipe PID_BUF */
#endif  /* Peripheral HID Application demo mode */

#if USB_PHID_APL_MODE == USB_PHID_KEYBOARD_MODE
            usb_phid_apl_keyboard_led_control( usb_gphid_ClassData[0] );        /* RSK LED Off/On control. */
            usb_cstd_SetBuf( mess, (uint16_t)USB_PIPE0 );                       /* Set pipe PID_BUF */
#endif  /* Peripheral HID Application keyboard mode */

#if USB_PHID_APL_MODE == USB_PHID_MOUSE_MODE
            usb_pstd_SetStallPipe0( mess );                               /* Req Error */
#endif  /* Peripheral HID Application mouse mode */
            R_usb_pstd_ControlEnd( mess, (uint16_t)USB_CTRL_END );              /* End control transfer. */
            break;

        /* Get class-specific descriptor : Report Descriptor */
        case    USB_GET_REPORT_DESCRIPTOR:
            /* Report Discriptor address set. */
            rpttbl = (uint8_t*)&usb_gphid_ReportDiscriptor;
            /* Get Discriptor table size. */
            tblsize = sizeof( usb_gphid_ReportDiscriptor );

            /* Descriptor > wLength */
            if( usb_gphid_Request.ReqLength < tblsize )
            {
                /* Transfer Report Descriptor. */
                R_usb_pstd_ControlRead( mess, (uint32_t)usb_gphid_Request.ReqLength, rpttbl );
            }
            else
            {
                /* Transfer Report Descriptor. */
                R_usb_pstd_ControlRead( mess, tblsize, rpttbl );
            }
            break;

        /* Get class-specific descriptor : HID Descriptor */
        case    USB_GET_HID_DESCRIPTOR:
            /* Configuration Discriptor address set. */
            cfgtbl = (uint8_t*)usb_gphid_Configuration;

            /* Set HID Discriptor address from Configuration Discriptor. */
            cfgtbl = (uint8_t *)( (uint32_t)cfgtbl + (uint32_t)18 );
            /* Set HID Discriptor length from Configuration Discriptor. */
            reqlength = (uint16_t)( *(uint8_t*)cfgtbl );

            /* Descriptor > wLength */
            if( usb_gphid_Request.ReqLength < reqlength )
            {
                /* Transfer HID Descriptor. */
                R_usb_pstd_ControlRead( mess, (uint32_t)usb_gphid_Request.ReqLength, cfgtbl );
            }
            else
            {
                /* Transfer HID Descriptor. */
                R_usb_pstd_ControlRead( mess, (uint32_t)reqlength, cfgtbl );
            }
            break;

        /* USB Transfer request */
        case    USB_SEND_DATA:
#if USB_ANSIIO_PP == USB_ANSIIO_USE_PP
            usb_fnInfo[mess->fn].write_length = mess->tranlen;      /* Transfer length store. */

            mess->complete = &usb_phid_in_trans_result;              /* Callback set for Peripheral HID class. */
            mess->ip = usb_fnInfo[mess->fn].usb_ip;                 /* IP number(0or1) */
            mess->ipp = usb_fnInfo[mess->fn].usb_ipp;               /* IP Address(USB0orUSB1) */

            /* USB Transfer request */
            err = usb_phid_data_trans( mess, usb_fnInfo[mess->fn].inPipeNo, mess->tranlen,
                mess->tranadr );
            /* Transfer Start request check */
            if( err != USB_OK )
            {
                /* Transfer Start request NG */
                usb_fnInfo[mess->fn].write_state = USB_CTL_ERR_PROCESS_ERR;

                /* User application call back */
                usb_fnInfo[mess->fn].write_length = -1;
                ( *usb_fnInfo[mess->fn].write_complete )( mess, 0, 0 );
            }
#else   /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */
            usb_gphid_tx_cb = mess->complete;
            mess->complete = &usb_phid_in_trans_result;              /* Callback set for Peripheral HID class. */

            /* USB Transfer request */
            err = usb_phid_data_trans( mess, USB_PHID_USE_PIPE, mess->tranlen, mess->tranadr );

            /* Transfer Start request check */
            if( err != USB_OK )
            {
                if( mess->complete )
                {
                    mess->status = USB_ERROR;

                    /* User application call back */
                    ( mess->complete )( mess, 0, 0 );
                }
            }
#endif  /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */
            break;

#if USB_ANSIIO_PP != USB_ANSIIO_USE_PP
        /* USB Transfer complete */
        case    USB_MSG_PCD_SUBMITUTR:
            switch( mess->status )
            {
            case    USB_DATA_OK:
            case    USB_DATA_READING:
            case    USB_DATA_SHT:
                /* data receive */
                break;

            case    USB_DATA_NONE:
            case    USB_DATA_WRITING:
                /* data transmission */
                break;

            case    USB_DATA_OVR:
                while( 1 );                     /* Transfer error stop. */
                break;

            case    USB_DATA_STALL:
                while( 1 );                     /* Transfer error stop. */
                break;

            case    USB_DATA_STOP:
            case    USB_DATA_ERR:
                break;

            default:
                while( 1 );                     /* Error stop. */
                break;
            }
            if( usb_gphid_tx_cb )
            {
                /* User application call back */
                ( usb_gphid_tx_cb )( mess, 0, 0 );
            }
            break;
#endif /* USB_ANSIIO_PP != USB_ANSIIO_USE_PP */

        default:
            while( 1 );                         /* Error stop. */
            break;
        }

        /* Release the memory block for the peripheral hid task */
        err = R_USB_REL_BLK( USB_PHID_MPL, (USB_MH_t)mess );
        if( err != USB_E_OK )
        {
            USB_PRINTF1( "### phidTask case else rel_blk error (%ld)\n", err );
        }
    }
}   /* eof R_usb_phid_task() */

/******************************************************************************
Function Name   : R_usb_phid_driver_start
Description     : PHID sample Initialize process
Arguments       : USB_UTR_t *ptr  : USB internal structure. Selects USB channel.
Return value    : none
******************************************************************************/
void R_usb_phid_driver_start( USB_UTR_t *ptr )
{
    /* Registration of a sample task priority */
    R_usb_cstd_SetTaskPri( USB_PHID_TSK, USB_PRI_4 );
}   /* eof R_usb_phid_driver_start() */

/******************************************************************************
Function Name   : R_usb_phid_DriverRelease
Description     : Clear the registered information of Peripheral HID.
Arguments       : none
Return value    : none
******************************************************************************/
void R_usb_phid_DriverRelease(void)
{
    /* Clear the information registered in the structure USB_PCDREG_t. */
    R_usb_pstd_DriverRelease();
}   /* eof R_usb_phid_DriverRelease() */

/******************************************************************************
Function Name   : R_usb_phid_TransferEnd
Description     : End of data transfer
Arguments       : USB_UTR_t *ptr  : USB internal structure. Selects USB channel.
Return value    : none
******************************************************************************/
void R_usb_phid_TransferEnd( USB_UTR_t *ptr )
{
    /* End of data transfer(USB0-IP peripheral function) */
    R_usb_pstd_TransferEnd( ptr, USB_PHID_USE_PIPE, USB_DATA_STOP );
}   /* eof R_usb_phid_TransferEnd() */

#if USB_ANSIIO_PP != USB_ANSIIO_USE_PP
/******************************************************************************
Function Name   : R_usb_phid_DeviceInformation
Description     : Get USB Device information
Arguments       : USB_UTR_t *ptr  : USB internal structure. Selects USB channel.
                : uint16_t    *deviceinfo
Return value    : none
******************************************************************************/
void R_usb_phid_DeviceInformation( USB_UTR_t *ptr, uint16_t *deviceinfo )
{
    R_usb_pstd_DeviceInformation( ptr, deviceinfo );
}   /* eof R_usb_phid_DeviceInformation() */
#endif  /* USB_ANSIIO_PP != USB_ANSIIO_USE_PP */

/******************************************************************************
Function Name   : R_usb_phid_ChangeDeviceState
Description     : Change USB Peripheral HID Device State
Arguments       : USB_UTR_t *ptr  : USB internal structure. Selects USB channel.
                : uint16_t msginfo  : Message Information Kind
Return value    : none
******************************************************************************/
void R_usb_phid_ChangeDeviceState( USB_UTR_t *ptr, uint16_t msginfo )
{
    R_usb_pstd_PcdChangeDeviceState( ptr, msginfo, (uint16_t)0, (USB_CB_INFO_t)usb_cstd_DummyFunction );
}   /* eof R_usb_phid_ChangeDeviceState() */

/******************************************************************************
Function Name   : R_usb_phid_transfer_length
Description     : Get transfer(report) length by PHID mode.
Arguments       : none
Return value    : uint32_t    TransferLength
******************************************************************************/
uint32_t R_usb_phid_transfer_length( void )
{
#if USB_PHID_APL_MODE == USB_PHID_DEMO_MODE
    return (uint32_t)USB_PHID_MXPS_DEMO;                  /* Demo mode report length set. */
#endif  /* Peripheral HID Application demo mode */

#if USB_PHID_APL_MODE == USB_PHID_KEYBOARD_MODE
    return (uint32_t)USB_PHID_MXPS_KBD;                   /* Keyboard mode report length set. */
#endif  /* Peripheral HID Application keyboard mode */

#if USB_PHID_APL_MODE == USB_PHID_MOUSE_MODE
    return (uint32_t)USB_PHID_MXPS_MSE;                   /* Mouse mode report length set. */
#endif  /* Peripheral HID Application mouse mode */
}   /* eof R_usb_phid_transfer_length() */

/******************************************************************************
Function Name   : usb_phid_data_trans
Description     : Start of data transfer
Arguments       : USB_UTR_t *ptr  : USB internal structure. Selects USB channel.
                : uint16_t pipe             : Pipe No
                : uint32_t size             : Size
                : uint8_t  *table           : Transfer Data Start address
Return value    : none
******************************************************************************/
USB_ER_t usb_phid_data_trans(USB_UTR_t *ptr, uint16_t pipe, uint32_t size, uint8_t *table)
{
    usb_gphid_SmplTrnMsg.msghead    = (USB_MH_t)NULL;
    usb_gphid_SmplTrnMsg.keyword    = pipe;                         /* Set pipe no. for USB transfer. */
    usb_gphid_SmplTrnMsg.tranadr    = table;                        /* Transfer data Start address */
    usb_gphid_SmplTrnMsg.tranlen    = size;                         /* Transfer data length */
    usb_gphid_SmplTrnMsg.setup      = 0;
    usb_gphid_SmplTrnMsg.complete   = ptr->complete;                /* Callback function */
    usb_gphid_SmplTrnMsg.segment    = (uint8_t)USB_TRAN_END;
    
    usb_gphid_SmplTrnMsg.ipp        = ptr->ipp;                     /* IP Address(USB0orUSB1) */
    usb_gphid_SmplTrnMsg.ip         = ptr->ip;                      /* IP number(0or1) */
    usb_gphid_SmplTrnMsg.fn         = ptr->fn;                      /* FileNumber Set */

    /* Peripheral HID Transfer Start. */
    return( R_usb_pstd_TransferStart( (USB_UTR_t *)&usb_gphid_SmplTrnMsg) );
}   /* eof usb_phid_data_trans() */

/******************************************************************************
Function Name   : usb_phid_in_trans_result
Description     : IN Data Transfer Callback Function 
Argument        : USB_UTR_t *mess       ; IN Transfer result information
                : uint16_t  data1       ; Not use
                : uint16_t  data2       ; Not use
Return          : none
******************************************************************************/
void usb_phid_in_trans_result( USB_UTR_t *mess, uint16_t data1, uint16_t data2 )
{
#if USB_ANSIIO_PP == USB_ANSIIO_USE_PP
    /* Get File No. */
    if( mess->fn != -1 )
    {
        /* Write complete */
        usb_fnInfo[mess->fn].write_state = USB_CTL_ERR_PROCESS_COMPLETE;

        /* Transfer Status check */
        if( mess->status != USB_DATA_NONE )
        {
            /* Transfer NG */
            usb_fnInfo[mess->fn].write_length = -1;
        }

        /* Call-back entry check */
        if( usb_fnInfo[mess->fn].write_complete )
        {
            /* Write complete call-back */
            ( *usb_fnInfo[mess->fn].write_complete )( mess, 0, 0 );
        }
    }
    else
    {
        /* Error loop */
//      while(1);
    }
#else   /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */
    USB_ER_t    err;

    /* From USB device to PHID */
    mess->msginfo = (uint16_t)USB_MSG_PCD_SUBMITUTR;
    if( mess->status == USB_DATA_STOP )
    {
        USB_PRINTF0( "### Peripheral HID Devices driver stoped !\n" );
    }

    /* Message transfer for PHID Task */
    err = R_USB_SND_MSG( USB_PHID_MBX, (USB_MSG_t*)mess );
    if( err != USB_E_OK )
    {
        /* Send message failure */
        USB_PRINTF1( "### PHID TransResult snd_msg error (%ld)\n", err );
    }
#endif  /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */
}   /* eof usb_phid_in_trans_result() */

/******************************************************************************
Function Name   : R_usb_phid_control_transfer
Description     : User Transmit Function
Arguments       : USB_UTR_t *ptr  : USB internal structure. Selects USB channel.
                : USB_REQUEST_t *request, 
                : uint16_t ctsq     : Control Transfer Stage
Return value    : none
******************************************************************************/
void R_usb_phid_control_transfer( USB_UTR_t *ptr, USB_REQUEST_t* request, uint16_t ctsq )
{
    usb_gphid_Request   = *request;

    /* Check Request type TYPE */
    if( usb_gphid_Request.ReqTypeType == USB_CLASS )
    {
        /* Branch by the Control Transfer Stage */
        switch( ctsq )
        {
            case USB_CS_IDST:
                usb_phid_ControlRequest0( ptr );            /* HID class request (idle or setup stage) */
                break;
            
            case USB_CS_RDDS:
                usb_phid_ControlRequest1( ptr );            /* HID class request (control read data stage) */
                break;
            
            case USB_CS_WRDS:
                usb_phid_ControlRequest2( ptr );            /* HID class request (control write data stage) */
                break;
            
            case USB_CS_WRND:
                usb_phid_ControlRequest3( ptr );            /* HID class request (control write nodata status stage) */
                break;
            
            case USB_CS_RDSS:
                usb_phid_ControlRequest4( ptr );            /* HID class request (control read status stage) */
                break;
            
            case USB_CS_WRSS:
                usb_phid_ControlRequest5( ptr );            /* HID class request (control write status stage) */
                break;
            
            case USB_CS_SQER:
                R_usb_pstd_ControlEnd( ptr, (uint16_t)USB_DATA_ERR );       /* End control transfer. */
                break;
            
            default:
                R_usb_pstd_ControlEnd( ptr, (uint16_t)USB_DATA_ERR );       /* End control transfer. */
                break;
        }
    }
    else
    {
        /* RequestType = Standard( or Vendor) */
        usb_phid_get_descriptor_interface( ptr, &usb_gpstd_ReqReg );          /* Get Descriptor request */
    }
}   /* eof R_usb_phid_control_transfer() */

/******************************************************************************
Function Name   : usb_phid_send_message
Description     : Message send for PHID Task
Arguments       : USB_UTR_t *ptr  : USB internal structure. Selects USB channel.
                : uint16_t msginfo  : Message Information Kind
Return value    : none
******************************************************************************/
void usb_phid_send_message( USB_UTR_t *ptr, uint16_t msginfo )
{
    USB_MH_t    p_blf;
    USB_UTR_t   *cp;
    USB_ER_t    err, err2;

    /* Get mem pool blk */
    err = R_USB_PGET_BLK(USB_PHID_MPL, &p_blf);
    if( err == USB_E_OK )
    {
        cp = (USB_UTR_t *)p_blf;
        cp->msghead     = (USB_MH_t)NULL;
        cp->msginfo     = (uint16_t)msginfo;                /* Message Information Kind set */
        cp->keyword     = (uint16_t)0u;
        cp->status      = (uint16_t)0u;
        cp->complete    = &usb_cstd_DummyFunction;          /* Call-back function dummy set */
        cp->ip          = ptr->ip;                          /* IP number(0or1) */
        cp->ipp         = ptr->ipp;                         /* IP Address(USB0orUSB1) */

        /* Message transfer for PHID Task */
        err = R_USB_SND_MSG( USB_PHID_MBX, (USB_MSG_t*)p_blf );
        if (err != USB_E_OK)
        {
            USB_PRINTF1( "Peri Sample snd_msg error %x\n", err );
            /* Release the memory block */
            err2 = R_USB_REL_BLK( USB_PHID_MPL, (USB_MH_t)p_blf );
            if( err2 != USB_E_OK )
            {
                /* Release memoryblock failure */
                USB_PRINTF1( "### usb_phid_send_message rel_blk error (%ld)\n", err2 );
            }
        }
    }
    else
    {
        /* Memory block get failure */
        USB_PRINTF0( "### usb_phid_send_message USB_PGET_BLK error\n" );
    }
}   /* eof usb_phid_send_message() */

/******************************************************************************
Function Name   : usb_phid_ControlRequest0
Description     : HID class request (idle or setup stage)
Arguments       : USB_UTR_t *ptr  : USB internal structure. Selects USB channel.
Return value    : none
******************************************************************************/
void usb_phid_ControlRequest0( USB_UTR_t *ptr )
{
}   /* eof usb_phid_ControlRequest0() */

/******************************************************************************
Function Name   : usb_phid_ControlRequest1
Description     : HID class request (control read data stage)
Arguments       : USB_UTR_t *ptr  : USB internal structure. Selects USB channel.
Return value    : none
******************************************************************************/
void usb_phid_ControlRequest1( USB_UTR_t *ptr )
{
    /* Is a request receive target Interface? */
    if( usb_gphid_Request.ReqTypeRecip == USB_INTERFACE )
    {
        /* Is a Request Type GetReport or GetIdle ? */
        switch( usb_gphid_Request.ReqRequest )
        {
            case USB_GET_REPORT:
                usb_phid_ControlGetReport1( ptr );                  /* GetReport request */
                break;
            
            case USB_GET_IDLE:
                usb_phid_ControlGetIdle1( ptr );                    /* GetIdle request */
                break;
            
            default:
                usb_pstd_SetStallPipe0( ptr );      /* Req Error */
                break;
        }
    }
    else
    {
        usb_pstd_SetStallPipe0( ptr );              /* Req Error */
    }
}   /* eof usb_phid_ControlRequest1() */

/******************************************************************************
Function Name   : usb_phid_ControlRequest2
Description     : HID class request (control write data stage)
Arguments       : USB_UTR_t *ptr  : USB internal structure. Selects USB channel.
Return value    : none
******************************************************************************/
void usb_phid_ControlRequest2( USB_UTR_t *ptr )
{
    /* Is a request receive target Interface? */
    if( usb_gphid_Request.ReqTypeRecip == USB_INTERFACE )
    {
        /* Is a Request Type SetReport ? */
        switch( usb_gphid_Request.ReqRequest )
        {
            case USB_SET_REPORT:
                usb_phid_ControlSetReport2( ptr );                  /* SetReport request */
                break;
            
            default:
                usb_pstd_SetStallPipe0( ptr );      /* Req Error */
                break;
        }
    }
    else
    {
        usb_pstd_SetStallPipe0( ptr );              /* Req Error */
    }
}   /* eof usb_phid_ControlRequest2() */

/******************************************************************************
Function Name   : usb_phid_ControlRequest3
Description     : HID class request (control write nodata status stage)
Arguments       : USB_UTR_t *ptr  : USB internal structure. Selects USB channel.
Return value    : none
******************************************************************************/
void usb_phid_ControlRequest3( USB_UTR_t *ptr )
{
    /* Is a request receive target Interface? */
    if( usb_gphid_Request.ReqTypeRecip == USB_INTERFACE )
    {
        /* Is a Request Type SetIdle ? */
        switch( usb_gphid_Request.ReqRequest )
        {
            case USB_SET_IDLE:
                usb_phid_ControlSetIdle3( ptr );                    /* SetIdle request */
                break;
            
            default:
                usb_pstd_SetStallPipe0( ptr );      /* Req Error */
                break;
        }
    }
    else
    {
        usb_pstd_SetStallPipe0( ptr );              /* Req Error */
    }
}   /* eof usb_phid_ControlRequest3() */

/******************************************************************************
Function Name   : usb_phid_ControlRequest4
Description     : HID class request (control read status stage)
Arguments       : USB_UTR_t *ptr  : USB internal structure. Selects USB channel.
Return value    : none
******************************************************************************/
void usb_phid_ControlRequest4( USB_UTR_t *ptr )
{
    /* Is a request receive target Interface? */
    if( usb_gphid_Request.ReqTypeRecip == USB_INTERFACE )
    {
        /* Is a Request Type GET_REPORT or GET_IDLE ? */
        switch( usb_gphid_Request.ReqRequest )
        {
            case USB_GET_REPORT:
                usb_cstd_SetBuf( ptr, (uint16_t)USB_PIPE0 );        /* Set pipe PID_BUF */
                break;
            
            case USB_GET_IDLE:
                usb_cstd_SetBuf( ptr, (uint16_t)USB_PIPE0 );        /* Set pipe PID_BUF */
                break;
            
            default:
                usb_pstd_SetStallPipe0( ptr );      /* Req Error */
                break;
        }
    }
    else
    {
        usb_pstd_SetStallPipe0( ptr );              /* Req Error */
    }
    R_usb_pstd_ControlEnd( ptr, (uint16_t)USB_CTRL_END );           /* End control transfer. */
}   /* eof usb_phid_ControlRequest4() */

/******************************************************************************
Function Name   : usb_phid_ControlRequest5
Description     : HID class request (control write status stage)
Arguments       : USB_UTR_t *ptr  : USB internal structure. Selects USB channel.
Return value    : none
******************************************************************************/
void usb_phid_ControlRequest5( USB_UTR_t *ptr )
{
    /* Is a request receive target Interface? */
    if( usb_gphid_Request.ReqTypeRecip == USB_INTERFACE )
    {
        /* Is a Request Type SET_REPORT ? */
        switch( usb_gphid_Request.ReqRequest )
        {
            case USB_SET_REPORT:
                usb_phid_send_message( ptr, (uint16_t)USB_SET_REPORT_STATUS );    /* Message send for PHID Task */
                break;
            
            default:
                /* Set pipe USB_PID_STALL */
                usb_pstd_SetStallPipe0( ptr );      /* Req Error */
                break;
        }
    }
    else
    {
        /* Set pipe USB_PID_STALL */
        usb_pstd_SetStallPipe0( ptr );              /* Req Error */
    }
}   /* eof usb_phid_ControlRequest5() */

/******************************************************************************
Function Name   : usb_phid_ControlGetReport1
Description     : GetReport request
Arguments       : USB_UTR_t *ptr  : USB internal structure. Selects USB channel.
Return value    : none
******************************************************************************/
void usb_phid_ControlGetReport1( USB_UTR_t *ptr )
{
    /* Check Setup packet Request Index and Request Value */
    if ( (usb_gphid_Request.ReqIndex == 0 ) && ( (usb_gphid_Request.ReqValue & USB_DT_INDEX) == 0 ) )
    {
        /* Report Type == Input */
        if( (uint8_t)USB_GET_DT_TYPE( usb_gphid_Request.ReqValue) == USB_DT_TYPE_GETREPORT )
        {
            usb_phid_send_message( ptr, (uint16_t)USB_GET_REPORT );           /* Message send for PHID Task */
        }
        else
        {
            /* Set pipe USB_PID_STALL */
            usb_pstd_SetStallPipe0( ptr );      /* Req Error */
        }
    }
    else
    {
        /* Set pipe USB_PID_STALL */
        usb_pstd_SetStallPipe0( ptr );          /* Req Error */
    }
}   /* eof usb_phid_ControlGetReport1() */

/******************************************************************************
Function Name   : usb_phid_ControlGetIdle1
Description     : GetIdle request
Arguments       : USB_UTR_t *ptr  : USB internal structure. Selects USB channel.
Return value    : none
******************************************************************************/
void usb_phid_ControlGetIdle1(USB_UTR_t *ptr )
{
    /* Check Setup packet Request Index and Request Value */
    if ( (usb_gphid_Request.ReqIndex == 0 ) && (usb_gphid_Request.ReqValue == 0 ) )
    {
        /* Check Setup packet Length */
        if ( usb_gphid_Request.ReqLength == 1 )
        {
            usb_phid_send_message( ptr, (uint16_t)USB_GET_IDLE );             /* Message send for PHID Task */
        }
        else
        {
            /* Set pipe USB_PID_STALL */
            usb_pstd_SetStallPipe0( ptr );      /* Req Error */
        }
    }
    else
    {
        /* Set pipe USB_PID_STALL */
        usb_pstd_SetStallPipe0( ptr );          /* Req Error */
    }
}   /* eof usb_phid_ControlGetIdle1() */

/******************************************************************************
Function Name   : usb_phid_ControlSetReport1
Description     : SetReport request
Arguments       : USB_UTR_t *ptr  : USB internal structure. Selects USB channel.
Return value    : none
******************************************************************************/
void usb_phid_ControlSetReport2( USB_UTR_t *ptr )
{
    /* Check Setup packet Request Index and Request Value */
    if ( (usb_gphid_Request.ReqIndex == 0 ) && ( ( usb_gphid_Request.ReqValue & USB_DT_INDEX ) == 0 ) )
    {
        /* Class Request == SET_REPORT */
        if( (uint8_t)USB_GET_DT_TYPE( usb_gphid_Request.ReqValue ) == USB_DT_TYPE_SETREPORT )
        {
            usb_phid_send_message( ptr, (uint16_t)USB_SET_REPORT );           /* Message send for PHID Task */
        }
        else
        {
            /* Set pipe USB_PID_STALL */
            usb_pstd_SetStallPipe0( ptr );      /* Req Error */
        }
    }
    else
    {
        /* Set pipe USB_PID_STALL */
        usb_pstd_SetStallPipe0( ptr );          /* Req Error */
    }
}   /* eof usb_phid_ControlSetReport2() */

/******************************************************************************
Function Name   : usb_phid_ControlSetIdle3
Description     : SetIdle request
Arguments       : USB_UTR_t *ptr  : USB internal structure. Selects USB channel.
Return value    : none
******************************************************************************/
void usb_phid_ControlSetIdle3( USB_UTR_t *ptr )
{
    /* Check Setup packet Request Index and Request Value */
    if ( ( usb_gphid_Request.ReqIndex == 0 ) && ( ( usb_gphid_Request.ReqValue & USB_DT_INDEX) == 0 ) )
    {
        /* Check Setup packet Length */
        if (usb_gphid_Request.ReqLength == 0 )
        {
            usb_phid_send_message( ptr, (uint16_t)USB_SET_IDLE );             /* Message send for PHID Task */
        }
        else
        {
            /* Set pipe USB_PID_STALL */
            usb_pstd_SetStallPipe0( ptr );      /* Req Error */
        }
    }
    else
    {
        /* Set pipe USB_PID_STALL */
        usb_pstd_SetStallPipe0( ptr );          /* Req Error */
    }
}   /* eof usb_phid_ControlSetIdle3() */

/******************************************************************************
Function Name   : usb_phid_get_descriptor_interface
Description     : Get Descriptor request
Arguments       : USB_UTR_t *ptr  : USB internal structure. Selects USB channel.
Return value    : none
******************************************************************************/
/* Correspondence is scheduled at MP */
void usb_phid_get_descriptor_interface( USB_UTR_t *ptr, USB_REQUEST_t *request )
{
    uint16_t        idx;
    usb_gphid_Request   = *request;

    idx = (uint16_t)( usb_gphid_Request.ReqValue & USB_DT_INDEX );

    /* Descriptor type check */
    if( (uint8_t)USB_GET_DT_TYPE( usb_gphid_Request.ReqValue ) == USB_DT_TYPE_RPTDESCRIPTOR )
    {
        /* Report Descriptor */
        usb_phid_send_message( ptr, (uint16_t)USB_GET_REPORT_DESCRIPTOR );    /* Message send for PHID Task */
    }
    else if( (uint8_t)USB_GET_DT_TYPE( usb_gphid_Request.ReqValue ) == USB_DT_TYPE_HIDDESCRIPTOR )
    {
        /* HID Descriptor */
        /* Check Index */
        if( ( usb_gphid_Request.ReqIndex == 0 ) || ( idx == 0 ) )
        {
            usb_phid_send_message( ptr, (uint16_t)USB_GET_HID_DESCRIPTOR );   /* Message send for PHID Task */
        }
        else
        {
            /* Set pipe USB_PID_STALL */
            usb_pstd_SetStallPipe0( ptr );      /* Req Error */
        }
    }
    else
    {
        /* else Descriptor */
        /* Set pipe USB_PID_STALL */
        usb_pstd_SetStallPipe0( ptr );          /* Req Error */
    }
}   /* eof usb_phid_get_descriptor_interface() */

/******************************************************************************
Function Name   : usb_phid_get_pipedir
Description     : Get Pipe Direction
Argument        : uint16_t *tbl_p           : Pipe Information Table address
                : uint16_t pipeno           : Pipe No.
Return          : Pipe Direction or Error Statue(USB_NG)
******************************************************************************/
uint16_t usb_phid_get_pipedir( uint16_t *tbl_p, uint16_t pipeno )
{
    uint16_t    idx, buf;

    /* Loop Endpoint table */
    for( idx = 0; ; idx += USB_EPL )
    {
        /* Check pipe no. */
        if( *( tbl_p + idx ) == pipeno )
        {
            buf = *( tbl_p + idx + 1 );             /* Get Pipe definition item 2 */
            return ( buf & USB_DIRFIELD );          /* Return Pipe Direction */
        }
    }
    return USB_NG;
}   /* eof usb_phid_get_pipedir() */

#if USB_ANSIIO_PP != USB_ANSIIO_USE_PP
/******************************************************************************
Function Name   : R_usb_phid_send_data
Description     : Data send request for Host
Arguments       : USB_UTR_t     *ptr        ; USB system internal structure. Selects channel.
                : uint8_t       *Table      ; Pointer to Data stored buffer 
                : uint32_t      size        ; Data size
                : USB_CB_INFO_t complete    ; Pointer to Callback function
Return value    : none
******************************************************************************/
void R_usb_phid_send_data( USB_UTR_t *ptr, uint8_t *Table, uint32_t size, USB_CB_t complete )
{
    USB_MH_t    p_blf;
    USB_ER_t    err, err2;
    USB_UTR_t   *cp;

    /* Get mem pool blk */
    err = R_USB_PGET_BLK( USB_PHID_MPL, &p_blf );
    if( err == USB_E_OK )
    {
        cp = (USB_UTR_t*)p_blf;
        cp->msghead = (USB_MH_t)NULL;
        cp->msginfo = (uint16_t)USB_SEND_DATA;                  /* Set message information. */
        cp->keyword = (uint16_t)USB_NOT_USED;                   /* Not use */
        cp->status      = (uint16_t)USB_NOT_USED;               /* Not use */
        cp->complete    = (USB_CB_t)complete;                   /* Set Callback function */
        cp->tranadr = (void *)Table;                            /* Transfer data Start address */
        cp->tranlen = (uint32_t)size;                           /* Transfer data length */
        cp->ip = ptr->ip;                                       /* IP number(0or1) */
        cp->ipp = ptr->ipp;                                     /* IP Address(USB0orUSB1) */

        /* Send message */
        err = R_USB_SND_MSG( USB_PHID_MBX, (USB_MSG_t*)p_blf );
        if( err != USB_E_OK )
        {
            /* Send message failure */
            USB_PRINTF1( "### phid send data snd_msg error (%ld)\n", err );
            err2 = R_USB_REL_BLK( USB_PHID_MPL, (USB_MH_t)p_blf );
            if( err2 != USB_E_OK )
            {
                /* Release memoryblock failure */
                USB_PRINTF1( "### phid send data rel_blk error (%ld)\n", err2 );
            }
        }
    }
    else
    {
        /* Get momoryblock failure */
        USB_PRINTF1( "### phid send data pget_blk error (%ld)\n", err );
    }
} /* eof R_usb_phid_send_data */
#endif  /* USB_ANSIIO_PP != USB_ANSIIO_USE_PP */


/******************************************************************************
End  Of File
******************************************************************************/
