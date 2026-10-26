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
* File Name    : r_usb_PMSCrequest.c
* Version      : 1.00
* Device(s)    : Renesas SH-Series, RX-Series
* Tool-Chain   : Renesas SuperH RISC engine Standard Toolchain
*              : Renesas RX Standard Toolchain
* OS           : Common to None and uITRON 4.0 Spec
* H/W Platform : Independent
* Description  : USB Host and Peripheral Interrupt code
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
#include "r_usb_pmsc_api.h"
#include "r_usb_cmsc_define.h"      /* USB  Mass Storage Class Header */
#include "r_usb_pmsc_define.h"      /* Peri Mass Storage Class Driver define */
#include "r_usb_pmsc_extern.h"      /* Peri Mass Storage Class Driver define */
#include "r_usb_catapi_define.h"    /* Peripheral ATAPI Device extern define */


/******************************************************************************
Section    <Section Difinition> , "Project Sections"
******************************************************************************/
#pragma section="_pmsc"


/******************************************************************************
Constant macro definitions
******************************************************************************/


/******************************************************************************
External variables and functions
******************************************************************************/


/******************************************************************************
Private global variables and functions
******************************************************************************/


/******************************************************************************
Renesas Abstracted Peripheral Mass Storage Class Driver API functions
******************************************************************************/

/******************************************************************************
Function Name   : usb_pmsc_UsrCtrlTransFunction
Description     : Control transfer
Arguments       : USB_REQUEST_t *ptr        : request
                : uint16_t ctsq             : control stage
Return value    : none
******************************************************************************/
void usb_pmsc_UsrCtrlTransFunction(USB_UTR_t *ptr, USB_REQUEST_t *req, uint16_t ctsq)
{
    if( req->ReqTypeType == USB_CLASS )
    {
        switch( ctsq )
        {
        /* Idle or setup stage */
        case USB_CS_IDST:   R_usb_pmsc_ControlTrans0(ptr, req); break;
        /* Control read data stage */
        case USB_CS_RDDS:   R_usb_pmsc_ControlTrans1(ptr, req); break;
        /* Control write data stage */
        case USB_CS_WRDS:   R_usb_pmsc_ControlTrans2(ptr, req); break;
        /* Control write nodata status stage */
        case USB_CS_WRND:   R_usb_pmsc_ControlTrans3(ptr, req); break;
        /* Control read status stage */
        case USB_CS_RDSS:   R_usb_pmsc_ControlTrans4(ptr, req); break;
        /* Control write status stage */
        case USB_CS_WRSS:   R_usb_pmsc_ControlTrans5(ptr, req); break;

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
        USB_PRINTF2("usb_pmsc_UsrCtrlTransFunction Err: %x %x\n"
            , req->ReqTypeType, ctsq);
        usb_pstd_SetStallPipe0( ptr );
    }
}   /* eof usb_pmsc_UsrCtrlTransFunction() */

/******************************************************************************
End  Of File
******************************************************************************/
