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
* Copyright (C) 2010 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************
* File Name    : r_usb_hmsc_api.c
* Version      : 1.00
* Device(s)    : Renesas
* Tool-Chain   : Renesas
* OS           : Common to None and uITRON
* H/W Platform : Independent
* Description  : USB Peripheral Sample Code
*******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 17.03.2010 0.80    First Release
*         : 30.07.2010 0.90    Updated comments
*         : 02.08.2010 0.91    Updated comments
*         : 29.10.2010 1.00    Mass Production Release
******************************************************************************/



/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_usb_ctypedef.h"         /* Type define */
#include "r_usb_usrconfig.h"
//#include "r_usb_cDefUSBIP.h"        /* USB-FW Library Header */
#include "r_usb_ckernelid.h"        /* Kernel ID definition */
#include "r_usb_cmsc_define.h"      /* USB  Mass Storage Class Header */
#include "r_usb_catapi_define.h"        /* Peripheral ATAPI Device extern */
#include "r_usb_hmsc_define.h"      /* Host Mass Storage Class Driver */
#include "r_usb_hmsc_extern.h"      /* Host MSC grobal define */

#include "r_usb_api.h"
#include "r_usb_hmsc_api.h"

/******************************************************************************
Section    <Section Difinition> , "Project Sections"
******************************************************************************/
#ifdef  R8CUSB
#pragma section program P_hmsc
#pragma section bss B_hmsc
#endif
#ifdef  RL78USB
#endif
#ifdef  RL78USB_SECTION
#pragma section @@CODE   P_HMSC
#pragma section @@BASE   T_HMSC
#pragma section @@DATA   B_HMSC
#endif

/******************************************************************************
Renesas Host MSC Sample Code functions
******************************************************************************/

/******************************************************************************
Function Name   : R_usb_hmsc_driver_start
Description     : USB Host Initialize process
Argument        : none
Return          : none
******************************************************************************/
void R_usb_hmsc_driver_start()
{
}   /* eof R_usb_hmsc_driver_start() */

/******************************************************************************
Function Name   : R_usb_hmsc_ClearStall
Description     : Clear Stall
Argument        : uint16_t type             : Data transmit/Data Receive
                : uint16_t msgnum           : Message Number
                : usb_cb_t complete         : Callback Function
Return value    : none
******************************************************************************/
void R_usb_hmsc_ClearStall( uint16_t type, uint16_t msgnum, usb_cb_t complete)
{
    uint16_t        pipeno;

    switch( type ) 
    {
    case USB_HMSC_TYPE_TRANSMIT:                         /* Data transmit */
        pipeno = R_usb_hmsc_Information(usb_ghmsc_OutPipe[msgnum][0]);
        usb_ghmsc_OutPipe[msgnum][1] = 0;
        usb_hmsc_ClearStall( pipeno, complete);
        break;
    case USB_HMSC_TYPE_RECEIVE:                           /* Data recieve */
        pipeno = R_usb_hmsc_Information(usb_ghmsc_InPipe[msgnum][0]);
        usb_ghmsc_InPipe[msgnum][1] = 0;
        usb_hmsc_ClearStall( pipeno, complete);
        break;
    default:
        break;
    }
}   /* eof R_usb_hmsc_ClearStall() */
