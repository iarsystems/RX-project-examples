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
* File Name    : r_usb_vendor_pdriver.c
* Version      : 2.01
* Description  : USB Peripheral vendor class driver function code.
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
#pragma section rom C_smpl
#endif
#ifdef  RL78USB
#endif
#ifdef  RL78USB_SECTION
#pragma section @@CODE   P_SMP
#pragma section @@BASE   T_SMP
#pragma section @@CNST   C_SMP
#pragma section @@R_INIT D_SMP
#pragma section @@DATA   B_SMP
#pragma section @@INIT   R_SMP
#endif


/******************************************************************************
Includes <System Includes> , "Project Includes"
******************************************************************************/
#include "r_usb_ctypedef.h"             /* Type define */
#include "r_usb_ckernelid.h"            /* Kernel ID definition */
#include "r_usb_usrconfig.h"            /* System definition, USB-FW Library Header */
#include "r_usb_usbip.h"                /* Definition of the USB register & bit define */
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
/* variables */

/* functions */


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
/* variables */
usb_utr_t       usb_pvndr_TrnMsg[USB_MAX_PIPE_NO+1];
usb_leng_t      usb_pvndr_TrnSize[USB_MAX_PIPE_NO+1];
uint8_t         *usb_pvndr_TrnPtr[USB_MAX_PIPE_NO+1];
uint16_t        usb_pvndr_PipeCtr[USB_MAX_PIPE_NO+1];
usb_cb_t        usb_pvndr_Callback[USB_MAX_PIPE_NO+1];
usb_request_t   usb_pvndr_Request;
uint8_t         usb_pvndr_ConfiguratuionNum;

/* functions */
void        usb_pvndr_task(void);
void        usb_pvndr_state_diagram(uint16_t data, uint16_t state);
void        usb_pvndr_control_callback(usb_request_t *data1, uint16_t data2);
usb_pipe_t  usb_pvndr_epadr_2_pipe(void);
void        usb_pvndr_get_alternate(void);
usb_er_t    usb_pvndr_data_transfer(usb_pipe_t Pipe);
void        usb_pvndr_transfer_result(usb_utr_t *mess);
void        usb_pvndr_dummy_function(void);


/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/
/* variables */
extern uint16_t g_usb_psmpl_EpTbl1[];

/* functions */



/******************************************************************************
Renesas Peripheral Sample Vendor Driver functions
******************************************************************************/

/******************************************************************************
Function Name   : usb_pvndr_task
Description     : Sample vendor class driver
Arguments       : none
Return value    : none
******************************************************************************/
void    usb_pvndr_task(void)
{
    usb_utr_t       *mess;
    usb_pipe_t      pipe;

    if (R_USB_RCV_MSG(USB_PVEN_MBX, (usb_msg_t**)&mess) != USB_E_OK)
    {
        return;
    }

    switch( mess->msginfo )
    {
        case USB_SMPL_TRANSFER_START:
            pipe    = mess->pipenum;
            usb_pvndr_TrnPtr[pipe]      = mess->tranadr;
            usb_pvndr_TrnSize[pipe] = mess->tranlen;
            usb_pvndr_Callback[pipe]    = mess->complete;
            if (usb_pvndr_data_transfer(pipe) == USB_E_ERROR)
            {
                usb_pvndr_TrnSize[pipe] = 0;
            }
        break;
        case USB_SMPL_TRANSFER_STOP:
            R_usb_pstd_TransferEnd(mess->pipenum, USB_DO_TRANSFER_STP);
            R_USB_REL_BLK(mess->flag);
        break;

        case USB_SMPL_TRANSFER_END:
            pipe    = mess->pipenum;
            switch(mess->status)
            {
                case USB_DATA_OK:
                case USB_DATA_SHT:
                case USB_DATA_OVR:
                    usb_pvndr_PipeCtr[pipe] = mess->pipectr;
                case USB_DATA_STOP:         /* Forced ending */
                    usb_pvndr_TrnSize[pipe] = 0;
                    ((void (*)(usb_utr_t*))*usb_pvndr_Callback[pipe])(mess);
                break;
                case USB_DATA_DTCH:         /* detach */
                case USB_DATA_TMO:          /* time out */
                case USB_DATA_ERR:
                default:
                break;
            }   
        break;
        default:
        break;
    }
}   /* eof usb_pvndr_task() */

/******************************************************************************
Function Name   : usb_pvndr_state_diagram
Description     : Device State CallBack Check
Arguments       : uint16_t state
Return value    : uint16_t data
******************************************************************************/
void usb_pvndr_state_diagram(uint16_t data, uint16_t state)
{
    uint16_t    i;

    switch( state )
    {
        case USB_STS_DETACH:
            usb_pvndr_ConfiguratuionNum = (uint8_t)0;
        break;
        case USB_STS_DEFAULT:
            for( i = USB_MIN_PIPE_NO; i <= USB_MAX_PIPE_NO; i++ )
            {
                usb_pvndr_TrnSize[i]    = 0;
            }
            usb_pvndr_ConfiguratuionNum = (uint8_t)0;
        break;
        case USB_STS_ADDRESS:
            usb_pvndr_ConfiguratuionNum = (uint8_t)0;
        break;
        case USB_STS_CONFIGURED:
            /* Begin to operate by the specified configuration number. */
            usb_pvndr_ConfiguratuionNum = (uint8_t)data;
            for( i = USB_MIN_PIPE_NO; i <= USB_MAX_PIPE_NO; i++ )
            {
                usb_pvndr_PipeCtr[i]    = 0;
            }
        break;
        case USB_STS_SUSPEND:
        break;
        case USB_STS_RESUME:
        break;
        default:
        break;
    }
}   /* eof usb_pvndr_state_diagram() */

/******************************************************************************
Function Name   : usb_pvndr_control_callback
Description     : 
Arguments       : usb_request_t *request
                : uint16_t      data
Return value    : none
******************************************************************************/
void usb_pvndr_control_callback(usb_request_t *request, uint16_t data)
{
    usb_pipe_t  pipe;

    usb_pvndr_Request   = *request;

    switch( usb_pvndr_Request.WORD.BIT.bType )
    {
        case    USB_STANDARD:
            /* The second argument is a request type. */
            switch( data )
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
                    pipe    = usb_pvndr_epadr_2_pipe();         /* To  hope to make API function. */
                    usb_pvndr_PipeCtr[pipe] = USB_NULL;
                break;

                case USB_RECIPIENT:
                    /* The report descriptor or and so on respond if it is HID class.  */
                break;

                default:
                break;
            }
        break;

        case    USB_CLASS:
            /* The second argument is a stage of the control transfer. */
            R_usb_pstd_ControlEnd(USB_DATA_ERR);
        break;

        case    USB_VENDOR:
            /* The second argument is a stage of the control transfer. */
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
        default:                                            /* Special function */
        break;
    }
}   /* eof usb_pvndr_control_callback() */

/******************************************************************************
Function Name   : usb_pvndr_control_callback
Description     : 
Arguments       : usb_request_t *data1
                : uint16_t      data2       : Not Use
Return value    : none
******************************************************************************/
usb_pipe_t usb_pvndr_epadr_2_pipe(void)
{
    uint16_t    *table;
    uint8_t     direp, tmp;

    /* Get PIPE Number from Endpoint address */
    direp           = (uint8_t)(usb_pvndr_Request.wIndex & 0x0Fu);
    direp           |= (uint8_t)((usb_pvndr_Request.wIndex & 0x80u) >> 3);

    table           = g_usb_psmpl_EpTbl1;

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
}

    /* eof usb_pvndr_control_callback() */


/******************************************************************************
Function Name   : usb_pvndr_data_transfer
Description     : Data Transfer
Arguments       : uint16_t pipe             : Pipe No
                : usb_leng_t size           : Size
                : uint8_t  *table           : Trasfer Data Start address
Return value    : usb_er_t                  : error
******************************************************************************/
usb_er_t usb_pvndr_data_transfer(usb_pipe_t pipe)
{
    usb_er_t    err;

    /* PIPE Transfer set */
    usb_pvndr_TrnMsg[pipe].pipenum  = pipe;
    usb_pvndr_TrnMsg[pipe].tranadr  = usb_pvndr_TrnPtr[pipe];
    usb_pvndr_TrnMsg[pipe].tranlen  = usb_pvndr_TrnSize[pipe];
    usb_pvndr_TrnMsg[pipe].pipectr  = usb_pvndr_PipeCtr[pipe];
    usb_pvndr_TrnMsg[pipe].setup    = 0;
    usb_pvndr_TrnMsg[pipe].complete = (usb_cb_t)&usb_pvndr_transfer_result;
    err = R_usb_pstd_TransferStart((usb_utr_t *)&usb_pvndr_TrnMsg[pipe]);
    return err;
}/* eof usb_pvndr_data_transfer() */


/******************************************************************************
Function Name   : usb_pvndr_transfer_result
Description     : Data Transfer Callback Function 
Arguments       : usb_utr_t *mess           : Utr Pointer
Return value    : none
******************************************************************************/
void usb_pvndr_transfer_result(usb_utr_t *mess)
{
    usb_er_t        err;

    mess->msginfo = USB_SMPL_TRANSFER_END;

    err = R_USB_SND_MSG(USB_PVEN_MBX, (usb_msg_t*)mess);
    if( err != USB_E_OK )
    {
        USB_DEBUG_HOOK(USB_DEBUG_HOOK_CLASS | USB_DEBUG_HOOK_CODE8);
    }
}/* eof usb_pvndr_transfer_result() */


/******************************************************************************
Function Name   : usb_pvndr_dummy_function
Description     : Vendor dummy function
Arguments       : none
Return value    : none
******************************************************************************/
void usb_pvndr_dummy_function(void)
{
}/* eof usb_pvndr_dummy_function() */


/******************************************************************************
End of file
******************************************************************************/
