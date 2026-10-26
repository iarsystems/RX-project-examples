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
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name    : r_usb_pmsc_ddi.c
* Version      : 2.00
* Description  : Device Driver Interface
*******************************************************************************/
/*******************************************************************************
* History    : DD.MM.YYYY Version Description
*            : 09.11.2012 1.00         First Release
*            : 30.11.2012 2.00
*******************************************************************************/


/******************************************************************************
Section definitions
******************************************************************************/
#ifdef R8CUSB
#pragma section program P_smpl
#pragma section bss B_smpl
#pragma section data R_smpl
#endif

#ifdef RL78USB
#endif

#ifdef RL78USB_SECTION
#pragma section @@CODE   P_SMP
#pragma section @@BASE   T_SMP
#pragma section @@DATA   B_SMP
#endif

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include "r_usb_ctypedef.h"                /* Type define */
#include "r_usb_ckernelid.h"            /* Kernel ID definition */
#include "r_usb_usrconfig.h"            /* System definition, USB-FW Library Header */
#include "r_usb_cdefusbip.h"            /* USB-FW Library Header */
#include "r_usb_api.h"                    /* USB API public function header */

#include "r_usb_pmsc_api.h"
#include "r_usb_cmsc_define.h"      /* USB  Mass Storage Class Header */
#include "r_usb_pmsc_define.h"      /* Peri Mass Storage Class Driver define */
#include "r_usb_pmsc_extern.h"      /* Peri Mass Storage Class Driver define */
#include "r_usb_catapi_define.h"    /* Peripheral ATAPI Device extern define */
#include "hw_resource.h"

/******************************************************************************
Constant macro definitions
******************************************************************************/


/******************************************************************************
External variables and functions
******************************************************************************/
extern usb_er_t usb_pmsc_SmpAtapiCloseMedia(void);


/******************************************************************************
Private global variables and functions
******************************************************************************/
usb_utr_t    usb_gpmsc_apl_message_area2;

void        usb_pmsc_UsrCtrlTransFunction(usb_request_t *req, uint16_t Ctsq);
void        usb_pmsc_change_device_state( uint16_t data1, uint16_t device_state );
usb_er_t    usb_pmsc_apl_DummyFunction(uint16_t data1, uint16_t device_state);


/******************************************************************************
Renesas Abstracted Peripheral Mass Storage Class Driver API functions
******************************************************************************/

/******************************************************************************
Function Name   : usb_pmsc_AtapiTransResult
Description     : CallBack Function from ATAPI to Peripheral Mass Storage Class
Arguments       : usb_utr_t *mess           : Utr Pointer
Return value    : none
******************************************************************************/
void usb_pmsc_AtapiTransResult(usb_utr_t *mess, uint16_t data1, uint16_t data2 )
{
    usb_er_t        err;

    mess->msginfo = (usb_strct_t)USB_PMSC_PFLASH2PMSC;
    err = R_USB_SND_MSG(USB_PMSC_MBX, (usb_utr_t*)mess);
    if( err != USB_E_OK )
    {
        USB_PRINTF1("### pmscAtapiTransResult snd_msg error (%ld)\n", err);
    }
}   /* eof usb_pmsc_AtapiTransResult() */


/******************************************************************************
Function Name   : usb_pmsc_change_device_state
Description     : 
Arguments       : none
Return          : none
******************************************************************************/
void usb_pmsc_change_device_state( uint16_t data1, uint16_t device_state )
{
    switch ( device_state )
    {
    case USB_STS_ATTACH:
        usb_pmsc_apl_DummyFunction(data1, device_state);
        break;
    case USB_STS_CONFIGURED:
        R_usb_pmsc_Open(data1, device_state );
    break;
    case USB_STS_DETACH:
        R_usb_pmsc_Close(data1, device_state );
#ifdef USB_STOP_MODE_ENABLE
  #ifdef __RX
        usb_cpu_soft_stanby_mode();
  #endif  /* __RX */
#endif /* USB_STOP_MODE_ENABLE */
    break;
    case USB_STS_SUSPEND:
        usb_pmsc_apl_DummyFunction(data1, device_state);
    break;
    case USB_STS_RESUME:
        usb_pmsc_apl_DummyFunction(data1, device_state);
    break;
    case USB_STS_DEFAULT:
        usb_pmsc_apl_DummyFunction(data1, device_state);
        R_usb_pstd_TransferEnd(usb_gpmsc_Outpipe, USB_DO_TRANSFER_TMO);
        R_usb_pstd_TransferEnd(usb_gpmsc_Inpipe,  USB_DO_TRANSFER_TMO);
    break;
    default:
        usb_pmsc_apl_DummyFunction(data1, device_state);
    break;
    }
}   /* usb_pmsc_change_device_state() */


/******************************************************************************
Function Name   : usb_pmsc_apl_DummyFunction
Description     : Application dummy function
Arguments       : uint16_t data1         : Not use
                : uint16_t device_state  : Not use
Return value    : usb_er_t               : USB_E_OK etc
******************************************************************************/
usb_er_t usb_pmsc_apl_DummyFunction(uint16_t data1, uint16_t device_state )
{
    return USB_E_OK;
}   /* usb_pmsc_apl_DummyFunction() */

/******************************************************************************
End  Of File
******************************************************************************/
