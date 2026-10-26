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
* File Name    : r_usb_vendor_papi.c
* Version      : 2.01
* Description  : This is the USB peripheral vendor class driver API function code.
*******************************************************************************/
/*******************************************************************************
* History   : DD.MM.YYYY Version Description
*           : 31.03.2012 1.80    Basic FW mini Ver2.00(ES Version)
*           : 30.11.2012 2.00    Basic FW mini Ver2.00 Release
*           : 31.01.2013 2.01    Corresponded to RL78/L1C
*******************************************************************************/

/******************************************************************************
Section definitions
******************************************************************************/
#ifdef  R8CUSB
#pragma section program P_smpl
#pragma section bss B_smpl
#pragma section data R_smpl
#endif
#ifdef  RL78USB
#endif
#ifdef  RL78USB_SECTION
#pragma section @@CODE   P_SMP
#pragma section @@BASE   T_SMP
#pragma section @@DATA   B_SMP
#endif


/******************************************************************************
Includes <System Includes> , "Project Includes"
******************************************************************************/
#include "r_usb_ctypedef.h"             /* Type define */
#include "r_usb_ckernelid.h"            /* Kernel ID definition */
#include "r_usb_usrconfig.h"            /* System definition, USB-FW Library Header */
#include "r_usb_api.h"                  /* USB API public function header */
#include "r_usb_vendor_api.h"


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


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
/* Exported global variables and functions are declared with the header file. */


/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/
/* variables */
extern usb_leng_t       usb_pvndr_TrnSize[];
extern uint8_t          usb_pvndr_ConfiguratuionNum;

/* functions */
extern void             usb_pvndr_task(void);
extern void             usb_pvndr_state_diagram(uint16_t data, uint16_t state);
extern void             usb_pvndr_control_callback(usb_request_t *request, uint16_t data);
extern void             usb_pvndr_dummy_function(void);



/******************************************************************************
Renesas Peripheral Sample Vendor Driver API functions
******************************************************************************/

/******************************************************************************
Function Name   : R_usb_pvndr_Task
Description     : USB Peripheral vendor driver task
Arguments       : none
Return value    : none
******************************************************************************/
void R_usb_pvndr_Task(void)
{
    usb_pvndr_task();
}   /* eof R_usb_pvndr_Task() */


/******************************************************************************
Function Name   : R_usb_pvndr_TransferStart
Description     : USB Peripheral vendor transfer start
Arguments       : utr_table
Return value    : none
******************************************************************************/
usb_er_t R_usb_pvndr_TransferStart(usb_utr_t *utr_table)
{
    if (usb_pvndr_TrnSize[utr_table->pipenum] == 0)
    {
        utr_table->msginfo  = USB_SMPL_TRANSFER_START;
        return  R_USB_SND_MSG(USB_PVEN_MBX, (usb_msg_t*)utr_table);
    }
    return  USB_E_ERROR;
}   /* eof R_usb_pvndr_TransferStart() */


/******************************************************************************
Function Name   : R_usb_pvndr_TransferStop
Description     : USB Peripheral vendor rtansfer stop
Arguments       : pipe
Return value    : none
******************************************************************************/
usb_er_t R_usb_pvndr_TransferStop(usb_pipe_t pipe)
{
    if (usb_pvndr_TrnSize[pipe] != 0)
    {
        /* Class check of enumeration sequence move to class function */
        if( R_USB_GET_SND(USB_PVEN_MBX, USB_SMPL_TRANSFER_STOP, usb_pvndr_dummy_function, pipe) != USB_E_OK )
        {
            USB_DEBUG_HOOK(USB_DEBUG_HOOK_CLASS | USB_DEBUG_HOOK_CODE7);
        }
    }
    return  USB_E_ERROR;
}   /* eof R_usb_pvndr_TransferStop() */


/******************************************************************************
Function Name   : R_usb_pvndr_DriverStart
Description     : Sample Open
Arguments       : none
Return value    : none
******************************************************************************/
void R_usb_pvndr_DriverStart(void)
{
    uint16_t    i;
//  All global variables are initialized specifying it. 
//  However, an unnecessary variable to initialize ends comment by "//". 
//unnecessary   usb_pvndr_TrnMsg[USB_MAX_PIPE_NO];
//unnecessary   *usb_pvndr_TrnPtr[USB_MAX_PIPE_NO];
//unnecessary   usb_cb_t    usb_pvndr_Callback[USB_MAX_PIPE_NO];
//unnecessary   usb_request_t   g_usb_SmplRequest;
    for( i = USB_MIN_PIPE_NO; i <= USB_MAX_PIPE_NO; i++ )
    {
        usb_pvndr_TrnSize[i]    = 0;
    }
    usb_pvndr_ConfiguratuionNum     = (uint8_t)0;
}   /* eof R_usb_pvndr_DriverStart() */

/******************************************************************************
Function Name   : R_usb_pvndr_DriverStop
Description     : Sample Close
Arguments       : none
Return value    : none
******************************************************************************/
void R_usb_pvndr_DriverStop(void)
{
}   /* eof R_usb_pvndr_DriverStop() */


/******************************************************************************
Function Name   : R_usb_pvndr_StateCallback
Description     : Device State CallBack Check
Arguments       : uint16_t state
Return value    : uint16_t data
******************************************************************************/
void R_usb_pvndr_StateCallback(uint16_t data, uint16_t state)
{
    usb_pvndr_state_diagram(data, state);
}   /* eof R_usb_pvndr_StateCallback() */


/******************************************************************************
Function Name   : R_usb_pvndr_ControlCallback
Description     : 
Arguments       : usb_request_t *data1
                : uint16_t      data2       : Not Use
Return value    : none
******************************************************************************/
void R_usb_pvndr_ControlCallback(usb_request_t *request, uint16_t data)
{
    usb_pvndr_control_callback(request, data);
}   /* eof R_usb_pvndr_ControlCallback() */


/******************************************************************************
End of file
******************************************************************************/
