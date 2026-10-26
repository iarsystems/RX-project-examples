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
* File Name     : r_usb_phid_api.c
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
#include "r_usb_phid_define.h"
#include "r_usb_phid_usrcfg.h"
#include "r_usb_phid_api.h"


/******************************************************************************
Macro definitions
******************************************************************************/
/* There is no macro definition. */


/******************************************************************************
Typedef definitions
******************************************************************************/
/* There is no typedef definition. */


/******************************************************************************
Private global variables and functions
******************************************************************************/
/* There is no orivate global variables and functions. */


/*****************************************************************************
External variables and functions
******************************************************************************/
/* Exported global variables and functions are declared with the header file. */


/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/
/* variables */
extern usb_request_t        usb_gphid_Request;
extern usb_utr_t            usb_gphid_message_area2;
extern volatile uint16_t    g_usb_pipe_toggle[];

/* functions */
extern void         usb_phid_initialize(void);
extern void         usb_phid_task(void);
extern void         usb_phid_ControlRequest0(usb_request_t *ptr);
extern void         usb_phid_ControlRequest1(usb_request_t *ptr);
extern void         usb_phid_ControlRequest2(usb_request_t *ptr);
extern void         usb_phid_ControlRequest3(usb_request_t *ptr);
extern void         usb_phid_ControlRequest4(usb_request_t *ptr);
extern void         usb_phid_ControlRequest5(usb_request_t *ptr);
extern void         usb_phid_get_descriptor_interface(usb_request_t *request);
extern usb_pipe_t   usb_phid_epadr_2_pipe(void);


/******************************************************************************
Renesas Abstracted USB Driver functions
******************************************************************************/

/******************************************************************************
Function Name   : R_usb_phid_task
Description     : Peripheral HID driver Task
Arguments       : none
Return value    : none
******************************************************************************/
void R_usb_phid_task( void )
{
    usb_phid_task();
}   /* eof R_usb_phid_task() */


/******************************************************************************
Function Name   : R_usb_phid_driver_start
Description     : PHID sample Initialize process
Arguments       : none
Return value    : none
******************************************************************************/
void R_usb_phid_driver_start( void )
{
    usb_phid_initialize();
}   /* eof R_usb_phid_driver_start() */


/******************************************************************************
Function Name   : R_usb_phid_DriverRelease
Description     : Clear the registered information of Peripheral HID.
Arguments       : none
Return value    : none
******************************************************************************/
void R_usb_phid_DriverRelease(void)
{
}   /* eof R_usb_phid_DriverRelease() */


/******************************************************************************
Function Name   : R_usb_phid_TransferEnd
Description     : End of data transfer
Arguments       : none
Return value    : none
******************************************************************************/
void R_usb_phid_TransferEnd( void )
{
    R_usb_pstd_TransferEnd( USB_PHID_USE_PIPE, USB_DATA_STOP );
}   /* eof R_usb_phid_TransferEnd() */


/******************************************************************************
Function Name   : R_usb_phid_DeviceInformation
Description     : Get USB Device information
Arguments       : uint16_t    *deviceinfo
Return value    : none
******************************************************************************/
void R_usb_phid_DeviceInformation( uint16_t *deviceinfo )
{
    R_usb_pstd_DeviceInformation( deviceinfo );
}   /* eof R_usb_phid_DeviceInformation() */


/******************************************************************************
Function Name   : R_usb_phid_ChangeDeviceState
Description     : Change USB Peripheral HID Device State
Arguments       : uint16_t msginfo  : Message Information Kind
Return value    : none
******************************************************************************/
void R_usb_phid_ChangeDeviceState( uint16_t msginfo )
{
    R_usb_pstd_PcdChangeDeviceState( (usb_strct_t)msginfo );
}   /* eof R_usb_phid_ChangeDeviceState() */


/******************************************************************************
Function Name   : R_usb_phid_control_callback
Description     : User Transmit Function
Arguments       : usb_request_t *request,
                : uint16_t data     : Control Transfer Stage
Return value    : none
******************************************************************************/
void R_usb_phid_control_callback(usb_request_t *request, uint16_t data)
{
    usb_pipe_t  pipe;


    usb_gphid_Request = *request;


    switch ( request->WORD.BIT.bType )
    {
        case USB_STANDARD:
            switch ( data )
            {
                case USB_SETREMOTE:
                    /* Processing from which remote wake up is permitted */
                break;
                case USB_CLEARREMOTE:
                    /* Processing from which remote wake up is prohibited */
                break;
                case USB_SETSTALL:
                    /* Processing when the communicating pipe(PIPEx) is set to STALL. */
                break;
                case USB_CLEARSTALL:
                    /* Processing when the communicating pipe(PIPEx) is clear to STALL. */
                    pipe = usb_phid_epadr_2_pipe();
                    g_usb_pipe_toggle[pipe] = USB_NULL;
                break;

                case USB_RECIPIENT:
                    /* The report descriptor or and so on respond if it is HID class.  */
                    usb_phid_get_descriptor_interface( request );   /* Get Descriptor request */
                break;

                default:
                break;
            }
        break;

        case USB_CLASS:
            switch ( data )
            {
                case USB_CS_IDST:
                    usb_phid_ControlRequest0( request );    /* HID class request (idle or setup stage) */
                break;

                case USB_CS_RDDS:
                    usb_phid_ControlRequest1( request );    /* HID class request (control read data stage) */
                break;

                case USB_CS_WRDS:
                    usb_phid_ControlRequest2( request );    /* HID class request (control write data stage) */
                break;

                case USB_CS_WRND:
                    usb_phid_ControlRequest3( request );    /* HID class request (control write nodata status stage) */
                break;

                case USB_CS_RDSS:
                    usb_phid_ControlRequest4( request );    /* HID class request (control read status stage) */
                break;

                case USB_CS_WRSS:
                    usb_phid_ControlRequest5( request );    /* HID class request (control write status stage) */
                break;

                case USB_CS_SQER:
                    R_usb_pstd_ControlEnd( (uint16_t)USB_DATA_ERR );    /* End control transfer. */
                break;

                default:
                    R_usb_pstd_ControlEnd( (uint16_t)USB_DATA_ERR );    /* End control transfer. */
                break;
            }
        break;

        case USB_VENDOR:
            switch( data )
            {
                case USB_CS_IDST:                           /* Idle or setup stage */
                case USB_CS_RDDS:                           /* Control read data stage */
                case USB_CS_WRDS:                           /* Control write data stage */
                case USB_CS_WRND:                           /* Control write nodata status stage */
                case USB_CS_RDSS:                           /* Control read status stage */
                case USB_CS_WRSS:                           /* Control write status stage */
                case USB_CS_SQER:                           /* Control sequence error */
                default:                                    /* Illegal */
                break;
            }
            R_usb_pstd_SetStallPipe0();
        break;

        default:
        break;
    }
}   /* eof R_usb_phid_control_callback() */


/******************************************************************************
Function Name   : R_usb_phid_send_data
Description     : Data send request for Host
Arguments       : uint8_t          *Table      ; Pointer to Data stored buffer
                : usb_leng_t size  size        ; Data size
                : usb_cb_t         complete    ; Pointer to Callback function
Return value    : none
******************************************************************************/
void R_usb_phid_send_data( uint8_t *Table, usb_leng_t size, usb_cb_t complete )
{
    usb_er_t    err;


    usb_gphid_message_area2.msginfo  = (uint16_t)USB_SEND_DATA;              /* Set message information. */
    usb_gphid_message_area2.pipenum  = (uint16_t)0u;                         /* Not use */
    usb_gphid_message_area2.status   = (uint16_t)0u;                         /* Not use */
    usb_gphid_message_area2.complete = (usb_cb_t)complete;                   /* Set Callback function */
    usb_gphid_message_area2.tranadr  = (void *)Table;                        /* Transfer data Start address */
    usb_gphid_message_area2.tranlen  = (usb_leng_t)size;                     /* Transfer data length */

    /* Send message */
    err = R_USB_SND_MSG( USB_PHID_MBX, (usb_msg_t*)&usb_gphid_message_area2 );
    if ( err != USB_E_OK )
    {
    }
}   /* eof R_usb_phid_send_data() */



/******************************************************************************
End  Of File
******************************************************************************/
