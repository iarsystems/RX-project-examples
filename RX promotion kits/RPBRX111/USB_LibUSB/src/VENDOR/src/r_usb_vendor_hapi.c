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
* File Name    : r_usb_vendor_hapi.c
* Version      : 2.01
* Description  : This is the USB host vendor class driver API function code.
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
extern usb_leng_t       g_usb_hvndr_TrnSize[];
extern uint16_t         g_usb_hvndr_DefEpTbl[];
extern uint16_t         g_usb_hvndr_Sequence;
extern uint8_t          *g_usb_hvndr_DevTbl;
extern uint8_t          *g_usb_hvndr_CnfgTbl;
extern usb_addr_t       g_usb_hvndr_DevAddr;
extern usb_port_t       g_usb_hvndr_DevPort;
extern usb_tskinfo_t    g_usb_hvndr_Message;                        /* Pcd Task recive message */
extern uint8_t          g_usb_hvndr_UsePipe[];

/* functions */
extern void             g_usb_hvndr_task(void);
extern void             usb_hvndr_state_diagram(uint16_t data, uint16_t state);
extern void             usb_hvndr_dummy_function(void);



/******************************************************************************
Renesas Host Sample Vendor Driver API functions
******************************************************************************/

/******************************************************************************
Function Name   : R_usb_hvndr_Task
Description     : USB Host vendor driver task
Arguments       : none
Return value    : none
******************************************************************************/
void R_usb_hvndr_Task(void)
{
    g_usb_hvndr_task();
}   /* eof R_usb_hvndr_Task() */


/******************************************************************************
Function Name   : R_usb_hvndr_TransferStart
Description     : USB Host vendor transfer start
Arguments       : utr_table
Return value    : none
******************************************************************************/
usb_er_t R_usb_hvndr_TransferStart(usb_utr_t *utr_table)
{
    if ((g_usb_hvndr_TrnSize[utr_table->pipenum] == 0) && (g_usb_hvndr_UsePipe[utr_table->pipenum] == USB_YES))
    {
        utr_table->msginfo  = USB_SMPL_TRANSFER_START;
        return  R_USB_SND_MSG(USB_HVEN_MBX, (usb_msg_t*)utr_table);
    }
    return  USB_E_ERROR;
}   /* eof R_usb_hvndr_TransferStart() */


/******************************************************************************
Function Name   : R_usb_hvndr_TransferStop
Description     : USB Host vendor rtansfer stop
Arguments       : pipe
Return value    : none
******************************************************************************/
usb_er_t R_usb_hvndr_TransferStop(usb_pipe_t pipe)
{
    if (g_usb_hvndr_TrnSize[pipe] != 0)
    {
        /* Class check of enumeration sequence move to class function */
        if( R_USB_GET_SND(USB_HVEN_MBX, USB_SMPL_TRANSFER_STOP, usb_hvndr_dummy_function, pipe) != USB_E_OK )
        {
            USB_DEBUG_HOOK(USB_DEBUG_HOOK_CLASS | USB_DEBUG_HOOK_CODE1);
        }
    }
    return  USB_E_ERROR;
}   /* eof R_usb_hvndr_TransferStop() */


/******************************************************************************
Function Name   : R_usb_hvndr_DriverStart
Description     : Sample Open
Arguments       : none
Return value    : none
******************************************************************************/
void R_usb_hvndr_DriverStart(void)
{
    uint16_t    i;
//  All global variables are initialized specifying it. 
//  However, an unnecessary variable to initialize ends comment by "//". 
//unnecessary   g_usb_hvndr_TrnMsg[USB_MAX_PIPE_NO];
//unnecessary   *g_usb_hvndr_TrnPtr[USB_MAX_PIPE_NO];
//unnecessary   usb_cb_t    g_usb_hvndr_Callback[USB_MAX_PIPE_NO];
//unnecessary   usb_request_t   g_usb_SmplRequest;
    for( i = USB_MIN_PIPE_NO; i <= USB_MAX_PIPE_NO; i++ )
    {
        g_usb_hvndr_TrnSize[i]  = 0;
        g_usb_hvndr_UsePipe[i]  = USB_NO;
    }
}   /* eof R_usb_hvndr_DriverStart() */

/******************************************************************************
Function Name   : R_usb_hvndr_DriverStop
Description     : Sample Close
Arguments       : none
Return value    : none
******************************************************************************/
void R_usb_hvndr_DriverStop(void)
{
}   /* eof R_usb_hvndr_DriverStop() */


/******************************************************************************
Function Name   : R_usb_hvndr_StateCallback
Description     : Device State CallBack Check
Arguments       : uint16_t state
Return value    : uint16_t data
******************************************************************************/
void R_usb_hvndr_StateCallback(uint16_t data, uint16_t state)
{
    usb_hvndr_state_diagram(data, state);
}   /* eof R_usb_hvndr_StateCallback() */


/******************************************************************************
Function Name   : R_usb_hvndr_ControlCallback
Description     : 
Arguments       : usb_request_t *data1
                : uint16_t      data2       : Not Use
Return value    : none
******************************************************************************/
void R_usb_hvndr_ClassCheck(uint8_t **table)
{
    g_usb_hvndr_DevTbl          = table[0];
    g_usb_hvndr_CnfgTbl         = table[1];
    g_usb_hvndr_DevAddr         = (usb_addr_t)(*table[2]);
    g_usb_hvndr_Sequence        = USB_SEQ_00;
    g_usb_hvndr_Message.msginfo = USB_SMPL_CHECKREQUEST;
    g_usb_hvndr_DevPort         = USB_NO;

    /* Class check of enumeration sequence move to class function */
    if( R_USB_SND_MSG(USB_HVEN_MBX, (usb_msg_t*)&g_usb_hvndr_Message) != USB_E_OK )
    {
        USB_DEBUG_HOOK(USB_DEBUG_HOOK_CLASS | USB_DEBUG_HOOK_CODE2);
    }
}   /* eof R_usb_hvndr_ClassCheck() */


/******************************************************************************
Function Name   : R_usb_hvndr_PipeRegistration
Description     : 
Arguments       : none
Return value    : none
******************************************************************************/
void R_usb_hvndr_PipeRegistration(void)
{
    R_usb_hstd_SetPipeRegistration(g_usb_hvndr_DefEpTbl, USB_YES);
}   /* eof R_usb_hvndr_PipeRegistration() */

/******************************************************************************
Function Name   : R_usb_hvndr_UsePipe
Description     : 
Arguments       : usb_pipe_t pipenum  : Pipe number
Return value    : uint8_t               : USB_YES/USB_NO
******************************************************************************/
uint8_t R_usb_hvndr_UsePipe(usb_pipe_t pipenum)
{
    return g_usb_hvndr_UsePipe[pipenum];
}   /* eof R_usb_hvndr_UsePipe() */


/******************************************************************************
End of file
******************************************************************************/
