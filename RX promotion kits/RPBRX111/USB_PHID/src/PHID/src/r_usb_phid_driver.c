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
* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name     : r_usb_phid_driver.c
* Version       : 2.10
* Description   : USB Peripheral HID class driver code.
*******************************************************************************/
/*******************************************************************************
* History   : DD.MM.YYYY Version Description
*           : 30.11.2012 2.00    First Release
*           : 31.05.2013 2.10    Corresponded to RL78/L1C, RX111
*******************************************************************************/


/******************************************************************************
Section definitions
******************************************************************************/

/******************************************************************************
Includes <System Includes> , "Project Includes"
******************************************************************************/
#include "r_usb_ctypedef.h"             /* Type define */
#include "r_usb_ckernelid.h"            /* Kernel ID definition */
#include "r_usb_usrconfig.h"            /* System definition, USB-FW Library Header */
#include "r_usb_usbip.h"                /* Definition of the USB register & bit define */
#include "r_usb_cdefusbip.h"            /* USB-FW Library Header */
#include "r_usb_api.h"                  /* USB API public function header */
#include "hw_resource.h"
#include "r_usb_phid_define.h"
#include "r_usb_phid_usrcfg.h"
#include "r_usb_phid_api.h"


/******************************************************************************
Macro definitions
******************************************************************************/
/* There is no macro definition. */
#define USB_PHID_INTERFACE_NO       0
#define USB_MSG_PCD_SUBMITUTR       0x12



/******************************************************************************
Typedef definitions
******************************************************************************/
/* There is no typedef definition. */


/******************************************************************************
Private global variables and functions
******************************************************************************/
/* variables */
USB_STATIC usb_utr_t   usb_gphid_SmplTrnMsg;
USB_STATIC uint8_t     usb_gphid_ClassData[18];
USB_STATIC uint8_t     usb_gphid_Duration;
USB_STATIC uint16_t    usb_gphid_DurationTmr;
USB_STATIC usb_cb_t    usb_gphid_tx_cb;            /* Call Back Function Info */
USB_STATIC usb_utr_t   usb_gphid_message_area1;

/* functions */
USB_STATIC usb_er_t    usb_phid_data_trans(usb_utr_t *ptr, usb_strct_t pipe, usb_leng_t size, uint8_t *table);
USB_STATIC void        usb_phid_in_trans_result(usb_utr_t *mess);
USB_STATIC void        usb_phid_DummyFunction( usb_utr_t *mess );
USB_STATIC void        usb_phid_ControlGetReport1(usb_request_t *ptr);
USB_STATIC void        usb_phid_ControlGetIdle1(usb_request_t *ptr);
USB_STATIC void        usb_phid_ControlSetReport2(usb_request_t *ptr);
USB_STATIC void        usb_phid_ControlSetIdle3(usb_request_t *ptr);


/*****************************************************************************
External variables and functions
******************************************************************************/
/* variables */
uint8_t             usb_gphid_TransData[8];
usb_request_t       usb_gphid_Request;
usb_utr_t           usb_gphid_message_area2;
volatile uint16_t   g_usb_pipe_toggle[USB_MAX_PIPE_NO + 1];

/* functions */
void        usb_phid_initialize(void);
void        usb_phid_task(void);
void        usb_phid_ControlRequest0(usb_request_t *ptr);
void        usb_phid_ControlRequest1(usb_request_t *ptr);
void        usb_phid_ControlRequest2(usb_request_t *ptr);
void        usb_phid_ControlRequest3(usb_request_t *ptr);
void        usb_phid_ControlRequest4(usb_request_t *ptr);
void        usb_phid_ControlRequest5(usb_request_t *ptr);
void        usb_phid_get_descriptor_interface(usb_request_t *request);
usb_pipe_t  usb_phid_epadr_2_pipe(void);


/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/
/* variables */
extern uint16_t         usb_gphid_EpTbl[];
extern uint8_t          usb_gphid_Configuration[];
extern const uint8_t    usb_gphid_ReportDiscriptor[];

/* functions */
#if USB_PHID_APL_MODE == USB_PHID_KEYBOARD_MODE
extern void     usb_phid_apl_keyboard_led_control( uint8_t leddata );
#endif
extern void     usb_phid_demo_mode_report( uint8_t *recdata );


/******************************************************************************
Renesas Abstracted USB Driver functions
******************************************************************************/

/******************************************************************************
Function Name   : usb_phid_initialize
Description     : PHID sample Initialize process
Arguments       : none
Return value    : none
******************************************************************************/
void usb_phid_initialize( void )
{
    uint16_t    i;
//  All global variables are initialized specifying it.
//  However, an unnecessary variable to initialize ends comment by "//".
//unnecessary   usb_gphid_SmplTrnMsg;
//unnecessary   usb_gphid_ClassData[]
//unnecessary   usb_gphid_message_area1;
//unnecessary   usb_gphid_message_area2;
//unnecessary   usb_gphid_Request;
    usb_gphid_Duration      = 0;            /* Only reporting when the report data is changed. */
    usb_gphid_DurationTmr   = 0;
    usb_gphid_tx_cb         = USB_NULL;
    for( i = USB_MIN_PIPE_NO; i <= USB_MAX_PIPE_NO; i++ )
    {
        g_usb_pipe_toggle[i] = USB_NULL;
    }
    for( i = 0; i < 8; i++ )
    {
        usb_gphid_TransData[i] = USB_NULL;
    }

}   /* eof usb_phid_initialize() */


/******************************************************************************
Function Name   : usb_phid_task
Description     : Peripheral HID driver Task
Arguments       : none
Return value    : none
******************************************************************************/
void usb_phid_task( void )
{
    usb_utr_t       *mess;
    usb_er_t        err;
    uint16_t        reqlength, i;
    uint8_t         *cfgtbl, *rpttbl;


    /* Receive message */
    err = R_USB_RCV_MSG( USB_PHID_MBX, (usb_msg_t**)&mess );
    if ( err != USB_E_OK )
    {
        return;
    }


    /* Branch by the message information. */
    switch( mess->msginfo )
    {
        case    USB_GET_IDLE:   /* HID Class Request : Get_Idle Request */
            /* Return duration by the Class Request Set Idle. */
            R_usb_pstd_ControlRead( (usb_leng_t)1, (uint8_t*)(&usb_gphid_Duration ) );
        break;

        case    USB_SET_IDLE:   /* HID Class Request : Set_Idle Request */
            /* Store duration */
            usb_gphid_Duration = (uint8_t)USB_GET_DT_TYPE( usb_gphid_Request.wValue );
            usb_gphid_DurationTmr = (uint16_t)( usb_gphid_Duration * 4 );           /* Duration Timer not used. */
            R_usb_pstd_ControlEnd( (uint16_t)USB_CTRL_END );                        /* End control transfer. */
        break;

        case    USB_GET_REPORT:     /* HID Class Request : Get_Report Request */
            /* Set transfer data. (Report data) */
            for( i = 0; i < USB_INTEPMAXP; i++ )
            {
                usb_gphid_ClassData[i] = usb_gphid_TransData[i];
            }

            /* Report transfer */
            if( usb_gphid_Request.wLength < USB_INTEPMAXP )
            {
                R_usb_pstd_ControlRead( (usb_leng_t)usb_gphid_Request.wLength, (uint8_t*)&usb_gphid_ClassData );
            }
            else
            {
                R_usb_pstd_ControlRead( (usb_leng_t)USB_INTEPMAXP, (uint8_t*)&usb_gphid_ClassData );
            }
        break;

        case    USB_SET_REPORT:     /* HID Class Request : Set_Report Request data stage */
            /* Receive data by the class request Set_Report. */
            R_usb_pstd_ControlWrite( (uint16_t)USB_PHID_SETREPORT_LENGTH, (uint8_t*)&usb_gphid_ClassData );
        break;

        case    USB_SET_REPORT_STATUS:      /* HID Class Request : Set_Report Request status stage */
#if USB_PHID_APL_MODE == USB_PHID_DEMO_MODE
            /* RSK demo ( Toggle LED or Read ADC or Update LCD ) */
            usb_phid_demo_mode_report( (uint8_t*)&usb_gphid_ClassData );
#endif  /* Peripheral HID Application demo mode */


#if USB_PHID_APL_MODE == USB_PHID_KEYBOARD_MODE
            usb_phid_apl_keyboard_led_control( usb_gphid_ClassData[0] );        /* RSK LED Off/On control. */
#endif  /* Peripheral HID Application keyboard mode */


#if USB_PHID_APL_MODE == USB_PHID_MOUSE_MODE
            R_usb_pstd_SetStallPipe0();                               /* Req Error */
#endif  /* Peripheral HID Application mouse mode */
            R_usb_pstd_ControlEnd( (uint16_t)USB_CTRL_END );              /* End control transfer. */
        break;

        case    USB_GET_REPORT_DESCRIPTOR:      /* Get class-specific descriptor : Report Descriptor */
            /* Report Discriptor address set. */
            rpttbl = (uint8_t*)&usb_gphid_ReportDiscriptor;

            /* Descriptor > wLength */
            if( usb_gphid_Request.wLength < USB_SMPLRPRTLEN )
            {
                /* Transfer Report Descriptor. */
                R_usb_pstd_ControlRead( (usb_leng_t)usb_gphid_Request.wLength, rpttbl );
            }
            else
            {
                /* Transfer Report Descriptor. */
                R_usb_pstd_ControlRead( (usb_leng_t)USB_SMPLRPRTLEN, rpttbl );
            }
        break;

        case    USB_GET_HID_DESCRIPTOR:     /* Get class-specific descriptor : HID Descriptor */
            /* Configuration Discriptor address set. */
            cfgtbl = (uint8_t*)usb_gphid_Configuration;

            /* Set HID Discriptor address from Configuration Discriptor. */
            cfgtbl = (uint8_t *)( (uint32_t)cfgtbl + (uint32_t)18 );
            /* Set HID Discriptor length from Configuration Discriptor. */
            reqlength = (uint16_t)( *(uint8_t*)cfgtbl );

            /* Descriptor > wLength */
            if( usb_gphid_Request.wLength < reqlength )
            {
                /* Transfer HID Descriptor. */
                R_usb_pstd_ControlRead( (usb_leng_t)usb_gphid_Request.wLength, cfgtbl );
            }
            else
            {
                /* Transfer HID Descriptor. */
                R_usb_pstd_ControlRead( (usb_leng_t)reqlength, cfgtbl );
            }
            break;

        /* USB Transfer request */
        case    USB_SEND_DATA:
            usb_gphid_tx_cb = mess->complete;
            mess->complete = (usb_cb_t)usb_phid_in_trans_result;    /* Callback set for Peripheral HID class. */

            /* USB Transfer request */
            err = usb_phid_data_trans( mess, (usb_strct_t)USB_PHID_USE_PIPE, mess->tranlen, mess->tranadr );

            /* Transfer Start request check */
            if( err != USB_E_OK )
            {
                if( mess->complete )
                {
                    mess->status = USB_DATA_ERR;

                    /* User application call back */
                        ((void (*)(usb_utr_t*))*mess->complete)(mess);
                }
            }
            break;

            /* USB Transfer complete */
        case    USB_MSG_PCD_SUBMITUTR:
            switch( mess->status )
            {
                case    USB_DATA_OK:
                case    USB_DATA_SHT:
                    /* data receive */
                break;

                case    USB_DATA_OVR:
                    USB_DEBUG_HOOK(USB_DEBUG_HOOK_CLASS | USB_DEBUG_HOOK_CODE1);    /* Transfer error stop. */
                break;

                case    USB_DATA_STALL:
                    USB_DEBUG_HOOK(USB_DEBUG_HOOK_CLASS | USB_DEBUG_HOOK_CODE2);    /* Transfer error stop. */
                break;

                case    USB_DATA_STOP:
                case    USB_DATA_ERR:
                break;

                case    USB_DATA_TMO:
                break;

                case    USB_DATA_DTCH:
                break;

                default:
                    USB_DEBUG_HOOK(USB_DEBUG_HOOK_CLASS | USB_DEBUG_HOOK_CODE3);    /* Error stop. */
                break;
            }

            if( usb_gphid_tx_cb )
            {
                /* User application call back */
                ((void (*)(usb_utr_t*))*usb_gphid_tx_cb)(mess);
            }
            break;

        default:
                USB_DEBUG_HOOK(USB_DEBUG_HOOK_CLASS | USB_DEBUG_HOOK_CODE3);    /* Error stop. */
        break;
    }
}   /* eof usb_phid_task() */


/******************************************************************************
Function Name   : usb_phid_data_trans
Description     : Start of data transfer
Arguments       : usb_utr_t   *ptr    : USB internal structure. Selects USB channel.
                : usb_strct_t pipe    : Pipe No
                : usb_leng_t  size    : Size
                : uint8_t     *table  : Transfer Data Start address
Return value    : none
******************************************************************************/
USB_STATIC usb_er_t usb_phid_data_trans(usb_utr_t *ptr, usb_strct_t pipe, usb_leng_t size, uint8_t *table)
{
    usb_gphid_SmplTrnMsg.pipenum    = pipe;                         /* Set pipe no. for USB transfer. */
    usb_gphid_SmplTrnMsg.tranadr    = table;                        /* Transfer data Start address */
    usb_gphid_SmplTrnMsg.tranlen    = size;                         /* Transfer data length */
    usb_gphid_SmplTrnMsg.setup      = 0;
    usb_gphid_SmplTrnMsg.complete   = ptr->complete;                /* Callback function */
    usb_gphid_SmplTrnMsg.pipectr    = g_usb_pipe_toggle[pipe];

    /* Peripheral HID Transfer Start. */
    return ( R_usb_pstd_TransferStart( (usb_utr_t *)&usb_gphid_SmplTrnMsg ) );
}   /* eof usb_phid_data_trans() */


/******************************************************************************
Function Name   : usb_phid_in_trans_result
Description     : IN Data Transfer Callback Function
Arguments       : usb_utr_t *mess       ; IN Transfer result information
Return value    : none
******************************************************************************/
USB_STATIC void usb_phid_in_trans_result( usb_utr_t *mess )
{
    usb_er_t    err;


    g_usb_pipe_toggle[mess->pipenum] = mess->pipectr;

    /* From USB device to PHID */
    mess->msginfo = (usb_strct_t)USB_MSG_PCD_SUBMITUTR;
    if( mess->status == USB_DATA_STOP )
    {
    }

    /* Message transfer for PHID Task */
    err = R_USB_SND_MSG( USB_PHID_MBX, (usb_msg_t*)mess );
    if( err != USB_E_OK )
    {
    }
}   /* usb_phid_in_trans_result() */


/******************************************************************************
Function Name   : usb_phid_send_message
Description     : Message send for PHID Task
Arguments       : usb_strct_t msginfo  : Message Information Kind
Return value    : none
******************************************************************************/
void usb_phid_send_message( usb_strct_t msginfo )
{
    usb_er_t    err;

    usb_gphid_message_area1.msginfo  = msginfo;
    usb_gphid_message_area1.pipenum  = (uint16_t)0;
    usb_gphid_message_area1.status   = (uint16_t)0;
    usb_gphid_message_area1.complete = (usb_cb_t)usb_phid_DummyFunction;


    /* Message transfer for PHID Task */
    err = R_USB_SND_MSG( USB_PHID_MBX, &usb_gphid_message_area1 );
    if (err != USB_E_OK)
    {
    }
}   /* eof usb_phid_send_message() */


/******************************************************************************
Function Name   : usb_phid_ControlRequest0
Description     : HID class request (idle or setup stage)
Arguments       : usb_request_t *ptr  : USB internal structure. Selects USB channel.
Return value    : none
******************************************************************************/
void usb_phid_ControlRequest0( usb_request_t *ptr )
{
}   /* eof usb_phid_ControlRequest0() */


/******************************************************************************
Function Name   : usb_phid_ControlRequest1
Description     : HID class request (control read data stage)
Arguments       : usb_request_t *ptr  : USB internal structure. Selects USB channel.
Return value    : none
******************************************************************************/
void usb_phid_ControlRequest1( usb_request_t *ptr )
{
    /* Is a request receive target Interface? */
    if( ptr->WORD.BIT.bRecipient == USB_INTERFACE )
    {
        /* Is a Request Type GetReport or GetIdle ? */
        switch( ptr->WORD.BIT.bRequest )
        {
            case USB_GET_REPORT:
                usb_phid_ControlGetReport1( ptr );                  /* GetReport request */
            break;

            case USB_GET_IDLE:
                usb_phid_ControlGetIdle1( ptr );                    /* GetIdle request */
            break;

            default:
                R_usb_pstd_SetStallPipe0();      /* Req Error */
            break;
        }
    }
    else
    {
        R_usb_pstd_SetStallPipe0();              /* Req Error */
    }
}   /* eof usb_phid_ControlRequest1() */


/******************************************************************************
Function Name   : usb_phid_ControlRequest2
Description     : HID class request (control write data stage)
Arguments       : usb_request_t *ptr  : USB internal structure. Selects USB channel.
Return value    : none
******************************************************************************/
void usb_phid_ControlRequest2( usb_request_t *ptr )
{
    /* Is a request receive target Interface? */
    if( ptr->WORD.BIT.bRecipient == USB_INTERFACE )
    {
        /* Is a Request Type SetReport ? */
        switch( ptr->WORD.BIT.bRequest )
        {
            case USB_SET_REPORT:
                usb_phid_ControlSetReport2( ptr );                  /* SetReport request */
            break;

            default:
                R_usb_pstd_SetStallPipe0();      /* Req Error */
            break;
        }
    }
    else
    {
        R_usb_pstd_SetStallPipe0();              /* Req Error */
    }
}   /* eof usb_phid_ControlRequest2() */


/******************************************************************************
Function Name   : usb_phid_ControlRequest3
Description     : HID class request (control write nodata status stage)
Arguments       : usb_request_t *ptr  : USB internal structure. Selects USB channel.
Return value    : none
******************************************************************************/
void usb_phid_ControlRequest3( usb_request_t *ptr )
{
    /* Is a request receive target Interface? */
    if( ptr->WORD.BIT.bRecipient == USB_INTERFACE )
    {
        /* Is a Request Type SetIdle ? */
        switch( ptr->WORD.BIT.bRequest )
        {
            case USB_SET_IDLE:
                usb_phid_ControlSetIdle3( ptr );    /* SetIdle request */
            break;

            default:
                R_usb_pstd_SetStallPipe0();         /* Req Error */
            break;
        }
    }
    else
    {
        R_usb_pstd_SetStallPipe0();              /* Req Error */
    }
}   /* eof usb_phid_ControlRequest3() */


/******************************************************************************
Function Name   : usb_phid_ControlRequest4
Description     : HID class request (control read status stage)
Arguments       : usb_request_t *ptr  : USB internal structure. Selects USB channel.
Return value    : none
******************************************************************************/
void usb_phid_ControlRequest4( usb_request_t *ptr )
{
    /* Is a request receive target Interface? */
    if( ptr->WORD.BIT.bRecipient == USB_INTERFACE )
    {
        /* Is a Request Type GET_REPORT or GET_IDLE ? */
        if ((ptr->WORD.BIT.bRequest != USB_GET_REPORT) && (ptr->WORD.BIT.bRequest != USB_GET_IDLE))
        {
            R_usb_pstd_SetStallPipe0();          /* Req Error */
        }
    }
    else
    {
        R_usb_pstd_SetStallPipe0();              /* Req Error */
    }

    R_usb_pstd_ControlEnd( (uint16_t)USB_CTRL_END );           /* End control transfer. */
}   /* eof usb_phid_ControlRequest4() */


/******************************************************************************
Function Name   : usb_phid_ControlRequest5
Description     : HID class request (control write status stage)
Arguments       : usb_request_t *ptr  : USB internal structure. Selects USB channel.
Return value    : none
******************************************************************************/
void usb_phid_ControlRequest5(usb_request_t *ptr )
{
    /* Is a request receive target Interface? */
    if( ptr->WORD.BIT.bRecipient == USB_INTERFACE )
    {
        /* Is a Request Type SET_REPORT ? */
        switch( ptr->WORD.BIT.bRequest )
        {
            case USB_SET_REPORT:
                usb_phid_send_message( (usb_strct_t)USB_SET_REPORT_STATUS );    /* Message send for PHID Task */
            break;

            default:
                /* Set pipe USB_PID_STALL */
                R_usb_pstd_SetStallPipe0();      /* Req Error */
            break;
        }
    }
    else
    {
        /* Set pipe USB_PID_STALL */
        R_usb_pstd_SetStallPipe0();              /* Req Error */
    }
}   /* eof usb_phid_ControlRequest5() */


/******************************************************************************
Function Name   : usb_phid_ControlGetReport1
Description     : GetReport request
Arguments       : usb_request_t *ptr  : USB internal structure. Selects USB channel.
Return value    : none
******************************************************************************/
USB_STATIC void usb_phid_ControlGetReport1( usb_request_t *ptr )
{
    /* Check Setup packet Request Index and Request Value */
    if ( (ptr->wIndex == USB_PHID_INTERFACE_NO) && ((ptr->wValue & USB_DT_INDEX) == 0) )
    {
        /* Report Type == Input */
        if( (uint8_t)USB_GET_DT_TYPE( ptr->wValue) == USB_DT_TYPE_GETREPORT )
        {
            usb_phid_send_message( (usb_strct_t)USB_GET_REPORT );           /* Message send for PHID Task */
        }
        else
        {
            /* Set pipe USB_PID_STALL */
            R_usb_pstd_SetStallPipe0();      /* Req Error */
        }
    }
    else
    {
        /* Set pipe USB_PID_STALL */
        R_usb_pstd_SetStallPipe0();          /* Req Error */
    }
}   /* eof usb_phid_ControlGetReport1() */


/******************************************************************************
Function Name   : usb_phid_ControlGetIdle1
Description     : GetIdle request
Arguments       : usb_request_t *ptr  : USB internal structure. Selects USB channel.
Return value    : none
******************************************************************************/
USB_STATIC void usb_phid_ControlGetIdle1( usb_request_t *ptr )
{
    /* Check Setup packet Request Index and Request Value */
    if ( (ptr->wIndex == USB_PHID_INTERFACE_NO) && (ptr->wValue == 0) )
    {
        /* Check Setup packet Length */
        if ( ptr->wLength == 1 )
        {
            usb_phid_send_message( (usb_strct_t)USB_GET_IDLE );             /* Message send for PHID Task */
        }
        else
        {
            /* Set pipe USB_PID_STALL */
            R_usb_pstd_SetStallPipe0();      /* Req Error */
        }
    }
    else
    {
        /* Set pipe USB_PID_STALL */
        R_usb_pstd_SetStallPipe0();          /* Req Error */
    }
}   /* eof usb_phid_ControlGetIdle1() */


/******************************************************************************
Function Name   : usb_phid_ControlSetReport2
Description     : SetReport request
Arguments       : usb_request_t *ptr  : USB internal structure. Selects USB channel.
Return value    : none
******************************************************************************/
USB_STATIC void usb_phid_ControlSetReport2( usb_request_t *ptr )
{
    /* Check Setup packet Request Index and Request Value */
    if ( (ptr->wIndex == USB_PHID_INTERFACE_NO) && ((ptr->wValue & USB_DT_INDEX) == 0) )
    {
        /* Class Request == SET_REPORT */
        if( (uint8_t)USB_GET_DT_TYPE( ptr->wValue ) == USB_DT_TYPE_SETREPORT )
        {
            usb_phid_send_message( (usb_strct_t)USB_SET_REPORT );           /* Message send for PHID Task */
        }
        else
        {
            /* Set pipe USB_PID_STALL */
            R_usb_pstd_SetStallPipe0();      /* Req Error */
        }
    }
    else
    {
        /* Set pipe USB_PID_STALL */
        R_usb_pstd_SetStallPipe0();          /* Req Error */
    }
}   /* eof usb_phid_ControlSetReport2() */


/******************************************************************************
Function Name   : usb_phid_ControlSetIdle3
Description     : SetIdle request
Arguments       : usb_request_t *ptr  : USB internal structure. Selects USB channel.
Return value    : none
******************************************************************************/
USB_STATIC void usb_phid_ControlSetIdle3( usb_request_t *ptr )
{
    /* Check Setup packet Request Index and Request Value */
    if ( (ptr->wIndex == USB_PHID_INTERFACE_NO) && ((ptr->wValue & USB_DT_INDEX) == 0) )
    {
        /* Check Setup packet Length */
        if (ptr->wLength == 0 )
        {
            usb_phid_send_message( (uint16_t)USB_SET_IDLE );             /* Message send for PHID Task */
        }
        else
        {
            /* Set pipe USB_PID_STALL */
            R_usb_pstd_SetStallPipe0();      /* Req Error */
        }
    }
    else
    {
        /* Set pipe USB_PID_STALL */
        R_usb_pstd_SetStallPipe0();          /* Req Error */
    }
}   /* eof usb_phid_ControlSetIdle3() */


/******************************************************************************
Function Name   : usb_phid_get_descriptor_interface
Description     : Get Descriptor request
Arguments       : usb_request_t *request  : USB internal structure. Selects USB channel.
Return value    : none
******************************************************************************/
/* Correspondence is scheduled at MP */
void usb_phid_get_descriptor_interface( usb_request_t *request )
{
    uint16_t        idx;
    usb_gphid_Request   = *request;

    idx = (uint16_t)( request->wValue & USB_DT_INDEX );

    /* Descriptor type check */
    if( (uint8_t)USB_GET_DT_TYPE( request->wValue ) == USB_DT_TYPE_RPTDESCRIPTOR )
    {
        /* Report Descriptor */
        usb_phid_send_message( (uint16_t)USB_GET_REPORT_DESCRIPTOR );    /* Message send for PHID Task */
    }
    else if( (uint8_t)USB_GET_DT_TYPE( request->wValue ) == USB_DT_TYPE_HIDDESCRIPTOR )
    {
        /* HID Descriptor */
        /* Check Index */
        if( ( request->wIndex == USB_PHID_INTERFACE_NO ) || ( idx == 0 ) )
        {
            usb_phid_send_message( (uint16_t)USB_GET_HID_DESCRIPTOR );   /* Message send for PHID Task */
        }
        else
        {
            /* Set pipe USB_PID_STALL */
            R_usb_pstd_SetStallPipe0();      /* Req Error */
        }
    }
    else
    {
        /* else Descriptor */
        /* Set pipe USB_PID_STALL */
        R_usb_pstd_SetStallPipe0();          /* Req Error */
    }
}   /* eof usb_phid_get_descriptor_interface() */


/******************************************************************************
Function Name   : usb_phid_DummyFunction
Description     : HID class dummy function
Arguments       : usb_utr_t *mess
Return value    : none
******************************************************************************/
USB_STATIC void usb_phid_DummyFunction( usb_utr_t *mess )
{
}   /* eof usb_phid_DummyFunction() */


/******************************************************************************
Function Name   : usb_phid_epadr_2_pipe
Description     : The pipe number is searched by endpoint address
Arguments       : none
Return value    : usb_pipe_t
******************************************************************************/
usb_pipe_t usb_phid_epadr_2_pipe(void)
{
    uint16_t    *table;
    uint8_t     direp, tmp;


    /* Get PIPE Number from Endpoint address */
    direp  =  (uint8_t)(usb_gphid_Request.wIndex & 0x0Fu);
    direp |= (uint8_t)((usb_gphid_Request.wIndex & 0x80u) >> 3);

    table = usb_gphid_EpTbl;

    /* Information table search */
    for( ; *table != USB_PDTBLEND; table += USB_EPL )
    {
        /* EP table endpoint dir check */
        tmp = (uint8_t)(table[1] & (USB_DIRFIELD | USB_EPNUMFIELD));
        if( direp == tmp )
        {
            return  (usb_pipe_t)(*table);
        }
    }
    return  USB_PIPE0;
}   /* eof usb_phid_epadr_2_pipe() */


/******************************************************************************
End  Of File
******************************************************************************/
