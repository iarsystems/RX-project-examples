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
* File Name    : r_usb_pmsc_request.c
* Version      : 2.00
* Description  : Peripheral MSC Request
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
#include "r_usb_ctypedef.h"         /* Type define */
#include "r_usb_ckernelid.h"        /* Kernel ID definition */
#include "r_usb_cdefusbip.h"            /* USB-FW Library Header */
#include "r_usb_usbip.h"            /* Definition of the USB register & bit define */
#include "r_usb_api.h"              /* USB API public function header */

#include "r_usb_pmsc_api.h"
#include "r_usb_cmsc_define.h"      /* USB  Mass Storage Class Header */
#include "r_usb_pmsc_define.h"      /* Peri Mass Storage Class Driver define */
#include "r_usb_catapi_define.h"    /* Peripheral ATAPI Device extern define */
#include "r_usb_pmsc_extern.h"      /* Peri Mass Storage Class Extern */


/******************************************************************************
Constant macro definitions
******************************************************************************/


/******************************************************************************
External variables and functions
******************************************************************************/
volatile uint8_t    g_pmsc_SetStall_Callback_no = USB_PMSC_NOOPERATION;

usb_request_t    g_usb_pmsc_Request;

extern volatile uint16_t    g_usb_pipe_toggle[10];

extern uint16_t             usb_gpmsc_EpTbl1[9];



/******************************************************************************
Private global variables and functions
******************************************************************************/
usb_pipe_t usb_pmsc_epadr_2_pipe(void);
void usb_pmsc_set_alternate(void);
void usb_pmsc_get_alternate(void);


/******************************************************************************
Renesas Abstracted Peripheral Mass Storage Class Driver API functions
******************************************************************************/

/******************************************************************************
Function Name   : usb_pmsc_UsrCtrlTransFunction
Description     : Control transfer
Arguments       : usb_request_t *req        : request
                : uint16_t ctsq             : control stage
Return value    : none
******************************************************************************/
void usb_pmsc_UsrCtrlTransFunction(usb_request_t *req, uint16_t ctsq)
{
    usb_pipe_t    pipe;
    
    
    g_usb_pmsc_Request = *req;
    
    switch ( g_usb_pmsc_Request.WORD.BIT.bType )
    {
        case USB_STANDARD:
            switch ( ctsq )
            {
                case USB_SET_INTERFACE:
                    usb_pmsc_set_alternate();
                break;
                case USB_GET_INTERFACE:
                    usb_pmsc_get_alternate();
                break;
                case USB_SETREMOTE:
                break;
                case USB_CLEARREMOTE:
                break;
                case USB_SETSTALL:
                break;
                case USB_CLEARSTALL:
                    pipe = usb_pmsc_epadr_2_pipe();
                    g_usb_pipe_toggle[pipe] = USB_NULL;

                    switch( g_pmsc_SetStall_Callback_no )
                    {
                        case    USB_PMSC_ERRPHASEERR:
                            usb_pmsc_ErrPhaseErr(0, 0);
                            g_pmsc_SetStall_Callback_no = USB_PMSC_NOOPERATION;
                        break;
                        case    USB_PMSC_USBEXECUTECONT:
                            usb_pmsc_UsbExecuteCont(0, 0);
                            g_pmsc_SetStall_Callback_no = USB_PMSC_NOOPERATION;
                        break;
                        case    USB_PMSC_ERRCSWNG:
                            usb_pmsc_ErrCswNg(0, 0);
                            g_pmsc_SetStall_Callback_no = USB_PMSC_NOOPERATION;
                        break;
                        case    USB_PMSC_ERRSETSTALL:
                            usb_pmsc_ErrSetStall(0, 0);
                            g_pmsc_SetStall_Callback_no = USB_PMSC_NOOPERATION;
                        break;
                        default:
                        break;
                    }
                break;

                case USB_RECIPIENT:
                break;

                case USB_STS_CONFIGURED:
                break;

                default:
                break;
            }
        break;
        
        case USB_CLASS:
            switch( ctsq )
            {
            /* Idle or setup stage */
            case USB_CS_IDST:
                R_usb_pmsc_ControlTrans0(req);
            break;
            /* Control read data stage */
            case USB_CS_RDDS:
                R_usb_pmsc_ControlTrans1(req);
            break;
            /* Control write data stage */
            case USB_CS_WRDS:
                R_usb_pmsc_ControlTrans2(req);
            break;
            /* Control write nodata status stage */
            case USB_CS_WRND:
                R_usb_pmsc_ControlTrans3(req);
            break;
            /* Control read status stage */
            case USB_CS_RDSS:
                R_usb_pmsc_ControlTrans4(req);
            break;
            /* Control write status stage */
            case USB_CS_WRSS:
                R_usb_pmsc_ControlTrans5(req);
            break;
            
            /* Control sequence error */
            case USB_CS_SQER:
                R_usb_pstd_ControlEnd((uint16_t)USB_DATA_ERR);
            break;
            /* Illegal */
            default:
                R_usb_pstd_ControlEnd((uint16_t)USB_DATA_ERR);
            break;
            }
        break;
        
        case USB_VENDOR:
            switch( ctsq )
            {
                case USB_CS_IDST:                            /* Idle or setup stage */
                case USB_CS_RDDS:                            /* Control read data stage */
                case USB_CS_WRDS:                            /* Control write data stage */
                case USB_CS_WRND:                            /* Control write nodata status stage */
                case USB_CS_RDSS:                            /* Control read status stage */
                case USB_CS_WRSS:                            /* Control write status stage */
                case USB_CS_SQER:                            /* Control sequence error */
                default:                                    /* Illegal */
                break;
            }
            R_usb_pstd_SetStallPipe0();
        break;
        
        default:
        break;
    }
}   /* eof usb_pmsc_UsrCtrlTransFunction() */



/******************************************************************************
Function Name   : 
Description     : 
Arguments       : void
Return value    : usb_pipe_t
******************************************************************************/
usb_pipe_t usb_pmsc_epadr_2_pipe(void)
{
    uint16_t    *table;
    uint8_t        direp, tmp;

    /* Get PIPE Number from Endpoint address */
    direp  =  (uint8_t)(g_usb_pmsc_Request.wIndex & 0x0Fu);
    direp |= (uint8_t)((g_usb_pmsc_Request.wIndex & 0x80u) >> 3);

    table = usb_gpmsc_EpTbl1;

    /* Information table search */
    for( ; *table != USB_PDTBLEND; table += USB_EPL )
    {
        /* EP table endpoint dir check */
        tmp = (uint8_t)(table[1] & (USB_DIRFIELD | USB_EPNUMFIELD));
        if( direp == tmp )
        {
            return    (usb_pipe_t)(*table);
        }
    }
    return    USB_PIPE0;
}   /* eof usb_pmsc_epadr_2_pipe() */


/******************************************************************************
Function Name   : usb_pmsc_set_alternate
Description     : 
Arguments       : none
Return value    : none
******************************************************************************/
void usb_pmsc_set_alternate(void)
{
    if ( g_usb_pmsc_Request.wIndex == USB_0 )
    {
        R_usb_pstd_SetPipeRegister(usb_gpmsc_EpTbl1, USB_YES);
    }
}   /* eof usb_pmsc_set_alternate() */



/******************************************************************************
Function Name   : usb_pmsc_get_alternate
Description     : 
Arguments       : none
Return value    : none
******************************************************************************/
void usb_pmsc_get_alternate(void)
{
    uint8_t    alternate_num;
    
    alternate_num = USB_0;
    R_usb_pstd_ControlRead(USB_1, &alternate_num);
}   /* eof usb_pmsc_set_alternate() */

/******************************************************************************
End  Of File
******************************************************************************/
