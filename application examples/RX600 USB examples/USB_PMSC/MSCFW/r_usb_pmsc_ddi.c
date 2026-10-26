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
* File Name    : r_usb_pmsc_ddi.c
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
extern USB_ER_t usb_pmsc_SmpAtapiCloseMedia(void);


/******************************************************************************
Private global variables and functions
******************************************************************************/
void usb_pmsc_UsrCtrlTransFunction(USB_UTR_t *ptr, USB_REQUEST_t *req, uint16_t Ctsq);


/******************************************************************************
Renesas Abstracted Peripheral Mass Storage Class Driver API functions
******************************************************************************/

/******************************************************************************
Function Name   : usb_pmsc_AtapiTransResult
Description     : CallBack Function from ATAPI to Peripheral Mass Storage Class
Arguments       : USB_UTR_t *mess           : Utr Pointer
Return value    : none
******************************************************************************/
void usb_pmsc_AtapiTransResult(USB_UTR_t *mess, uint16_t data1, uint16_t data2 )
{
    USB_ER_t        err;

    mess->msginfo = (uint16_t)USB_PMSC_PFLASH2PMSC;     
    err = R_USB_SND_MSG(USB_PMSC_MBX, (USB_MSG_t*)mess);
    if( err != USB_E_OK )
    {
        USB_PRINTF1("### pmscAtapiTransResult snd_msg error (%ld)\n", err);
    }
}   /* eof usb_pmsc_AtapiTransResult() */

/******************************************************************************
Function Name   : R_usb_pmsc_StrgTaskOpen
Description     : Peripheral Mass Storage Class Task Open
Arguments       : none
Return value    : none
******************************************************************************/
void R_usb_pmsc_StrgTaskOpen(USB_UTR_t *ptr)
{
    /* Media Driver open */
    usb_pmsc_SmpAtapiInitMedia();
    /* Registration of peripheral Mass Strage Class Driver */
    R_usb_pmsc_Registration(ptr);
}   /* eof R_usb_pmsc_StrgTaskOpen() */

/******************************************************************************
Function Name   : R_usb_pmsc_StrgTaskClose
Description     : Peripheral Mass Storage Class Task Close
Arguments       : none
Return value    : none
******************************************************************************/
void R_usb_pmsc_StrgTaskClose(USB_UTR_t *ptr)
{
    /* Media Driver close */
    usb_pmsc_SmpAtapiCloseMedia();
}   /* eof R_usb_pmsc_StrgTaskClose() */

/******************************************************************************
Function Name   : R_usb_pmsc_Registration
Description     : Registration of peripheral Mass Strage Class Driver
Arguments       : none
Return value    : none
******************************************************************************/
void R_usb_pmsc_Registration(USB_UTR_t *ptr)
{
    USB_PCDREG_t    driver;

    /* Driver registration */
    /* Pipe Define Table address */
    driver.pipetbl      = &usb_gpmsc_EpPtr[0];
    /* Device descriptor Table address */
    driver.devicetbl    = (uint8_t*)&usb_gpmsc_DeviceDescriptor;
    /* Qualifier descriptor Table address */
    driver.qualitbl     = (uint8_t*)&usb_gpmsc_QualifierDescriptor;
    /* Configuration descriptor Table address */
    driver.configtbl    = (uint8_t**)&usb_gpmsc_ConPtr;
    /* Other configuration descriptor Table address */
    driver.othertbl     = (uint8_t**)&usb_gpmsc_ConPtrOther;
    /* String descriptor Table address */
    driver.stringtbl    = (uint8_t**)&usb_gpmsc_StrPtr;
    /* Driver init */
    driver.classinit    = &usb_cstd_DummyFunction;
    /* Device default */
    driver.devdefault   = &R_usb_pmsc_DescriptorChange;
    /* Device configuered */
    driver.devconfig    = (USB_CB_INFO_t)&R_usb_pmsc_Open;
    /* Device detach */
    driver.devdetach    = (USB_CB_INFO_t)&R_usb_pmsc_Close;
    /* Device suspend */
    driver.devsuspend   = &usb_cstd_DummyFunction;
    /* Device resume */
    driver.devresume    = &usb_cstd_DummyFunction;
    /* Interfaced change */
    driver.interface    = &R_usb_pmsc_SetInterface;
    /* Control Transfer */
    driver.ctrltrans    = &usb_pmsc_UsrCtrlTransFunction;
    R_usb_pstd_DriverRegistration(ptr, &driver);
}   /* eof R_usb_pmsc_Registration() */

/******************************************************************************
End  Of File
******************************************************************************/
