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
* Copyright (C) 2010(2011) Renesas Electronics Corpration
* and Renesas Solutions Corp. All rights reserved.
*******************************************************************************
* File Name    : r_usb_PCDCdriver.c
* Version      : 2.00
* Device(s)    : Renesas SH-Series, RX-Series
* Tool-Chain   : Renesas SuperH RISC engine Standard Toolchain
*              : Renesas RX Standard Toolchain
* OS           : Common to None and uITRON 4.0 Spec
* H/W Platform : Independent
* Description  : USB Host and Peripheral Interrupt code
*******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 06.08.2010 0.91    First Release
*         : 31.08.2010 0.92    PCDC Updated
*         : 30.09.2010 0.93    PCDC Updated
*         : 02.12.2010 0.96    Spelling modification
*         : 02.12.2010 0.96    Unnecessary Code Delete
*         : 01.06.2011 1.10    Version 1.10 Release
******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <string.h>
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
/* Peri Communications Devices Class Extern */
#include "r_usb_pcdc_extern.h"
#include "hw_resource.h"

/******************************************************************************
Section    <Section Difinition> , "Project Sections"
******************************************************************************/
#pragma section="_pcdc"


/******************************************************************************
Constant macro definitions
******************************************************************************/


/******************************************************************************
External variables and functions
******************************************************************************/
extern void usb_pstd_SetStall(USB_UTR_t *ptr, uint16_t pipe);

/* Serial Port driver functions */
#ifdef USB_UART_ENABLE
extern uint16_t usb_cpu_Sci_Set1(uint8_t *data_rate_top, uint8_t stop_bit, uint8_t parity, uint8_t data_bit);
#endif /* USB_UART_ENABLE */
extern uint16_t usb_cpu_Sci_SetDataChk(uint8_t *data_rate_top, uint8_t stop_bit, uint8_t parity, uint8_t data_bit);

/* Control Line Status data */
extern USB_PCDC_ControlLineState_t  usb_gcdc_ControlLineState;
/* Duration of Break */
extern uint16_t usb_gcdc_SendBreakDuration;

extern USB_CB_t usb_gpcdc_TxCB;         /* Call Back Function Info */
extern USB_CB_t usb_gpcdc_RxCB;         /* Call Back Function Info */
extern USB_CB_t usb_gpcdc_StatCB;       /* Call Back Function Info */


/******************************************************************************
Private global variables and functions
******************************************************************************/
USB_ER_t usb_pcdc_UserDataTrans(USB_UTR_t *ptr, uint16_t pipe, uint32_t size, uint8_t *table);
void usb_pcdc_InTransResult(USB_UTR_t *mess, uint16_t data1, uint16_t data2);
void usb_pcdc_OutTransResult(USB_UTR_t *mess, uint16_t data1, uint16_t data2);
uint16_t usb_get_pipedir( uint16_t  *tbl_p, uint16_t pipeno );

void usb_pcdc_SetControlLineState(USB_UTR_t *ptr, USB_CB_INFO_t complete);
void usb_pcdc_SetLineCoding(USB_UTR_t *ptr, USB_CB_INFO_t complete);
void usb_pcdc_GetLineCoding(USB_UTR_t *ptr, USB_CB_INFO_t complete);
void usb_pcdc_SendBreak(USB_UTR_t *ptr, USB_CB_INFO_t complete);

/* Pipe Direction from Host to Device */
uint16_t        usb_gpcdc_Outpipe;
/* Pipe Direction from Device to Host */
uint16_t        usb_gpcdc_Inpipe;
uint16_t        usb_gpcdc_Status_pipe;

/******************************************************************************
Renesas Abstracted Peripheral Communications Devices Class Driver API functions
******************************************************************************/

/******************************************************************************
Function Name   : usb_pcdc_task
Description     : Peripheral Communications Devices Class main task
Arguments       : USB_VP_INT_t stacd        : start code
Return value    : none
******************************************************************************/
void usb_pcdc_task(USB_VP_INT_t stacd)
{
    uint16_t ret;
    USB_UTR_t   *mess;
    USB_ER_t    err = 0l;

    /* Pipe Transfer */
    err = R_USB_TRCV_MSG( USB_PCDC_MBX, (USB_MSG_t**)&mess, (uint16_t)3000 );
    if( err == USB_E_OK )
    {
        switch( mess->msginfo  )
        {
        /* Get Line Coding */
        case USB_PCDC_PGET_LINE_CODING:
            /* CallBack function (SND_MSG) */
            (*mess->complete)(mess, 0, 0 );

            err = USB_REL_BLK(USB_PCDC_MPL, (USB_MH_t)mess);
            if( err != USB_E_OK )
            {
                USB_PRINTF1( "### pcdcTask case else rel_blk error (%ld)\n", err);
            }
            break;

        /* CDC Class Request Set Line Coding */
        case USB_PCDC_PSET_LINE_CODING:
            /* Line coding set parameter check */
            ret = usb_cpu_Sci_SetDataChk(
                (uint8_t *)&usb_gcdc_LineCodingReq.dwDTERate
                , usb_gcdc_LineCodingReq.bCharFormat
                , usb_gcdc_LineCodingReq.bParityType
                , usb_gcdc_LineCodingReq.bDataBits);
            if( ret == USB_OK )
            {
                /* Line Coding Parameter set */
                memcpy(&usb_gcdc_LineCoding,&usb_gcdc_LineCodingReq,7);
#ifdef USB_UART_ENABLE
                /* Line coding set */
                usb_cpu_Sci_Set1((uint8_t *)&usb_gcdc_LineCoding.dwDTERate
                    , usb_gcdc_LineCoding.bCharFormat
                    , usb_gcdc_LineCoding.bParityType
                    , usb_gcdc_LineCoding.bDataBits);
#endif  /* USB_UART_ENABLE */
            }

            /* CallBack function (SND_MSG) */
            (*mess->complete)(mess, 0, 0 );

            err = R_USB_REL_BLK(USB_PCDC_MPL, (USB_MH_t)mess);
            if( err != USB_E_OK )
            {
                USB_PRINTF1( "### pcdcTask case else rel_blk error (%ld)\n", err);
            }
            break;

        /* CDC Class Request Set Control Line State */
        case USB_PCDC_PSET_CONTROL_LINE_STATE:
            /* RS-232 signal RTS & DTR Set */
            /* If RTS/DTR control function is prepared, calls this function here */

            /* CallBack function (SND_MSG) */
            (*mess->complete)(mess, 0, 0 );

            err = R_USB_REL_BLK(USB_PCDC_MPL, (USB_MH_t)mess);
            if( err != USB_E_OK )
            {
                USB_PRINTF1( "### pcdcTask case else rel_blk error (%ld)\n", err);
            }
            break;

        /* CDC Class Request Send Break */
        case USB_PCDC_PSEND_BREAK:
            /* RS-232 style break */
            /* If BREAK signal output function is prepared, calls this function here */

            /* CallBack function (SND_MSG) */
            (*mess->complete)(mess, 0, 0 );
            err = R_USB_REL_BLK(USB_PCDC_MPL, (USB_MH_t)mess);
            if( err != USB_E_OK )
            {
                USB_PRINTF1( "### pcdcTask case else rel_blk error (%ld)\n", err);
            }
            break;

#if USB_ANSIIO_PP == USB_ANSIIO_USE_PP
        case USB_PCDC_PSERIAL_STATE_NOTIFICATION:
        case USB_PCDC_PSEND_DATA:
            usb_fnInfo[mess->fn].write_length = mess->tranlen;

            mess->complete = &usb_pcdc_InTransResult;
            mess->ip = usb_fnInfo[mess->fn].usb_ip;
            mess->ipp = usb_fnInfo[mess->fn].usb_ipp;

            err = usb_pcdc_UserDataTrans( mess, usb_fnInfo[mess->fn].inPipeNo, mess->tranlen, mess->tranadr);
            /* Transfer Start request check */
            if( err != USB_OK )
            {
                /* Transfer Start request NG */
                usb_fnInfo[mess->fn].write_state = USB_CTL_ERR_PROCESS_ERR;

                /* User application call back */
                usb_fnInfo[mess->fn].write_length = -1;
//              (*usb_fnInfo[mess->fn].write_complete)( mess, 0, 0 );
            }

            /* Class Notification Serial State for USB Host */
            err = R_USB_REL_BLK(USB_PCDC_MPL, (USB_MH_t)mess);
            if( err != USB_E_OK )
            {
                USB_PRINTF1("### pcdcTask SerialStateTx rel_blk error (%ld)\n", err);
            }
            break;

        case USB_PCDC_PRECEIVE_DATA:
            usb_fnInfo[mess->fn].read_length = mess->tranlen;

            mess->complete = &usb_pcdc_OutTransResult;
            mess->ip = usb_fnInfo[mess->fn].usb_ip;
            mess->ipp = usb_fnInfo[mess->fn].usb_ipp;

            err = usb_pcdc_UserDataTrans(mess, usb_fnInfo[mess->fn].outPipeNo, mess->tranlen, mess->tranadr);
            /* Transfer Start request check */
            if( err != USB_OK )
            {
                /* Transfer Start request NG */
                usb_fnInfo[mess->fn].read_state = USB_CTL_ERR_PROCESS_ERR;

                /* User application call back */
                usb_fnInfo[mess->fn].read_length = -1;
//              (*usb_fnInfo[mess->fn].read_complete)( mess, 0, 0 );
            }

            /* Class Notification Serial State for USB Host */
            err = R_USB_REL_BLK(USB_PCDC_MPL, (USB_MH_t)mess);
            if( err != USB_E_OK )
            {
                USB_PRINTF1("### pcdcTask SerialStateTx rel_blk error (%ld)\n", err);
            }
            break;
#else   /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */
        case USB_PCDC_PSEND_DATA:
            usb_gpcdc_TxCB  = (USB_CB_t)mess->complete;

            mess->complete = &usb_pcdc_InTransResult;
            /* Data send request for Host */
            err = usb_pcdc_UserDataTrans(mess, usb_gpcdc_Inpipe, mess->tranlen, mess->tranadr);

            err = R_USB_REL_BLK(USB_PCDC_MPL, (USB_MH_t)mess);
            if( err != USB_E_OK )
            {
                USB_PRINTF1("### pcdcTask SendData rel_blk error (%ld)\n", err);
            }
            break;
        case USB_PCDC_PRECEIVE_DATA:
            usb_gpcdc_RxCB  = (USB_CB_t)mess->complete;

            mess->complete = &usb_pcdc_OutTransResult;
            /* Data Receive request for Host */
            err = usb_pcdc_UserDataTrans(mess, usb_gpcdc_Outpipe, mess->tranlen, mess->tranadr);

            err = R_USB_REL_BLK(USB_PCDC_MPL, (USB_MH_t)mess);
            if( err != USB_E_OK )
            {
                USB_PRINTF1("### pcdcTask ReceiveData rel_blk error (%ld)\n", err);
            }
            break;
        case USB_PCDC_PSERIAL_STATE_NOTIFICATION:
            usb_gpcdc_StatCB    = (USB_CB_t)mess->complete;

            mess->complete = &usb_pcdc_StatusTransResult;
            /* Serial State send request for Host */
            err = usb_pcdc_UserDataTrans(mess, usb_gpcdc_Status_pipe, mess->tranlen, mess->tranadr);

            /* Class Notification Serial State for USB Host */
            err = R_USB_REL_BLK(USB_PCDC_MPL, (USB_MH_t)mess);
            if( err != USB_E_OK )
            {
                USB_PRINTF1("### pcdcTask SerialStateTx rel_blk error (%ld)\n", err);
            }
            break;
        case USB_PCDC_PDATASND:
            /* Data Send Transfer complete call back */
            (usb_gpcdc_TxCB)(mess,0,0);
            break;
        case USB_PCDC_PDATARCV:
            /* Data Receive Transfer complete call back */
            (usb_gpcdc_RxCB)(mess,0,0);
            break;
        case USB_PCDC_PSTATUSSND:
            /* Status Send Transfer complete call back */
            (usb_gpcdc_StatCB)(mess,0,0);
            break;
#endif  /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */
        default:
            break;
        }
    }
} /* eof usb_pcdc_task */


/******************************************************************************
Function Name   : usb_pcdc_SetControlLineState_req
Description     : Class request processing for SetControlLineState request
Arguments       : USB_UTR_t *ptr       ; USB system internal structure. Selects channel.
Return value    : none
******************************************************************************/
void usb_pcdc_SetControlLineState_req(USB_UTR_t *ptr)
{
    /* Class request Set_Control_Line_State */
    usb_pcdc_SetControlLineState(ptr, &usb_cstd_DummyFunction);
} /* eof usb_pcdc_SetControlLineState_req */

/******************************************************************************
Function Name   : usb_pcdc_SetLineCoding_req
Description     : Class request processing for SetLineCoding request
Arguments       : USB_UTR_t *ptr       ; USB system internal structure. Selects channel.
Return value    : none
******************************************************************************/
void usb_pcdc_SetLineCoding_req(USB_UTR_t *ptr)
{
    /* Class request Set_Line_Coding */
    usb_pcdc_SetLineCoding(ptr, &usb_cstd_DummyFunction);
} /* eof usb_pcdc_SetLineCoding_req */

/******************************************************************************
Function Name   : usb_pcdc_GetLineCoding_req
Description     : Class request processing for GetLineCoding request
Arguments       : USB_UTR_t *ptr       ; USB system internal structure. Selects channel.
Return value    : none
******************************************************************************/
void usb_pcdc_GetLineCoding_req(USB_UTR_t *ptr)
{
    /* Class request Get_Line_Coding */
    usb_pcdc_GetLineCoding(ptr, &usb_cstd_DummyFunction);
} /* eof usb_pcdc_GetLineCoding_req */

/******************************************************************************
Function Name   : usb_pcdc_SendBreak_req
Description     : Class request processing for SendBreak request
Arguments       : USB_UTR_t *ptr       ; USB system internal structure. Selects channel.
Return value    : none
******************************************************************************/
void usb_pcdc_SendBreak_req(USB_UTR_t *ptr)
{
    /* Class request Get_Line_Coding */
    usb_pcdc_SendBreak(ptr, &usb_cstd_DummyFunction);
} /* eof usb_pcdc_SendBreak_req */

/******************************************************************************
Function Name   : usb_pcdc_UserDataTrans
Description     : Data Transfer Processing to USB Host
Argument        : USB_UTR_t *ptr   ; USB system internal structure. Selects channel.
                : uint16_t pipe    ; Pipe No
                : uint32_t size    ; Size
                : uint8_t  *table  ; Transfer Data Start address
Return          : USB_ER_t         ; 
******************************************************************************/
USB_ER_t usb_pcdc_UserDataTrans(USB_UTR_t *ptr, uint16_t pipe, uint32_t size, uint8_t *table)
{   
    /* PIPE Transfer set */
    usb_gpcdc_Mess[pipe].msghead    = (USB_MH_t)NULL;
    usb_gpcdc_Mess[pipe].keyword    = pipe;
    usb_gpcdc_Mess[pipe].tranadr    = table;
    usb_gpcdc_Mess[pipe].tranlen    = size;
    usb_gpcdc_Mess[pipe].setup  = 0;
    usb_gpcdc_Mess[pipe].complete   = ptr->complete;
    usb_gpcdc_Mess[pipe].segment    = USB_TRAN_END;

    usb_gpcdc_Mess[pipe].ipp        = ptr->ipp;
    usb_gpcdc_Mess[pipe].ip     = ptr->ip;

    usb_gpcdc_Mess[pipe].fn     = ptr->fn;

    return( R_usb_pstd_TransferStart((USB_UTR_t *)&usb_gpcdc_Mess[pipe]) );
} /* eof usb_pcdc_UserDataTrans */

#if USB_ANSIIO_PP != USB_ANSIIO_USE_PP
/******************************************************************************
Function Name   : usb_pcdc_StatusTransResult
Description     : CallBack Function
Arguments       : USB_UTR_t *mess  ; Information of the result when UART state is sent to Host
                : uint16_t  data1  ; Not use
                : uint16_t  data2  ; Not use 
Return value    : none
******************************************************************************/
void usb_pcdc_StatusTransResult(USB_UTR_t *mess, uint16_t data1, uint16_t data2)
{
    USB_ER_t    err;

    /* From USB device to PCDC */
    mess->msginfo = (uint16_t)USB_PCDC_PSTATUSSND;
    if( mess->status == USB_DATA_STOP )
    {
        USB_PRINTF0("### Peripheral Communications Devices driver stoped !\n");
    }
    err = R_USB_SND_MSG(USB_PCDC_MBX, (USB_MSG_t*)mess);
    if( err != USB_E_OK )
    {
        /* Send message failure */
        USB_PRINTF1("### pcdcTransResult snd_msg error (%ld)\n", err);
    }
} /* eof usb_pcdc_StatusTransResult */
#endif  /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */

/******************************************************************************
Function Name   : usb_pcdc_InTransResult
Description     : IN Data Transfer Callback Function 
Argument        : USB_UTR_t *mess       ; IN Transfer result information
                : uint16_t  data1       ; Not use
                : uint16_t  data2       ; Not use
Return          : none
******************************************************************************/
void usb_pcdc_InTransResult(USB_UTR_t *mess, uint16_t data1, uint16_t data2)
{
#if USB_ANSIIO_PP == USB_ANSIIO_USE_PP
    /* Get File No. */
    if( mess->fn != -1 )
    {
        /* Write complete */
        usb_fnInfo[mess->fn].write_state = USB_CTL_ERR_PROCESS_COMPLETE;
        /* Transfer Status check */
        if(mess->status != USB_DATA_NONE)
        {
            /* Transfer NG */
            usb_fnInfo[mess->fn].write_length = -1;
        }
        (*usb_fnInfo[mess->fn].write_complete)( mess, 0, 0 );
    }
    else
    {
        /* Error loop */
//      while(1);
    }
#else   /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */
    USB_ER_t    err;

    /* From USB device to PCDC */
    mess->msginfo = (uint16_t)USB_PCDC_PDATASND;
    if( mess->status == USB_DATA_STOP )
    {
        USB_PRINTF0("### Peripheral Comm Devices driver stoped !\n");
    }
    err = R_USB_SND_MSG(USB_PCDC_MBX, (USB_MSG_t*)mess);
    if( err != USB_E_OK )
    {
        /* Send message failure */
        USB_PRINTF1("### pcdcTransResult snd_msg error (%ld)\n", err);
    }
#endif  /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */
} /* eof usb_pcdc_InTransResult */

/******************************************************************************
Function Name   : usb_pcdc_OutTransResult
Description     : OUT Data Transfer Callback Function 
Argument        : USB_UTR_t *mess      ; OUT Transfer result information
                : uint16_t  data1      ; Not use
                : uint16_t  data2      ; Not use
Return          : none
******************************************************************************/
void usb_pcdc_OutTransResult(USB_UTR_t *mess, uint16_t data1, uint16_t data2)
{
#if USB_ANSIIO_PP == USB_ANSIIO_USE_PP
    /* Get File No. */
    if( mess->fn != -1 )
    {
        /* Read complete */
        usb_fnInfo[mess->fn].read_state = USB_CTL_ERR_PROCESS_COMPLETE;
        if(mess->status == USB_DATA_OK)
        {
            /* Receive size = Request size */
        }
        else if(mess->status == USB_DATA_SHT)
        {
            /* Receive size < Request size */
            usb_fnInfo[mess->fn].read_length -= mess->tranlen;
        }
        else
        {
            usb_fnInfo[mess->fn].read_length = -1;
        }
        (*usb_fnInfo[mess->fn].read_complete)( mess, 0, 0 );
    }
    else
    {
        /* Error loop */
//      while(1);
    }
#else   /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */
    USB_ER_t    err;

    /* From USB device to PCDC */
    mess->msginfo = (uint16_t)USB_PCDC_PDATARCV;
    if( mess->status == USB_DATA_STOP )
    {
        USB_PRINTF0("### Peripheral Comm Devices driver stoped !\n");
    }
    err = R_USB_SND_MSG(USB_PCDC_MBX, (USB_MSG_t*)mess);
    if( err != USB_E_OK )
    {
        /* Send message failure */
        USB_PRINTF1("### pcdcTransResult snd_msg error (%ld)\n", err);
    }
#endif  /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */
} /* eof usb_pcdc_OutTransResult */

/******************************************************************************
Function Name   : usb_get_pipedir
Description     : Get Pipe Divection
Argument        : uint16_t *tbl_p           ; Pipe Information Table address
                : uint16_t pipeno           ; Pipe No.
Return          : Pipe Direction or Error Statue(USB_NG)
******************************************************************************/
uint16_t usb_get_pipedir( uint16_t  *tbl_p, uint16_t pipeno )
{
    uint16_t    idx, buf;

    for( idx = 0; ; idx += USB_EPL )
    {
        if( *(tbl_p + idx) == pipeno )
        {
            buf = *(tbl_p + idx + 1);
            return ( buf & USB_DIRFIELD );
        }
    }
    return USB_NG;
} /* eof usb_get_pipedir */

/******************************************************************************
Function Name   : usb_pcdc_DataTrans
Description     : Tranfer data from USB
Arguments       : USB_UTR_t *ptr       ; USB system internal structure. Selects channel.
                : uint16_t pipe        ; Pipe No
                : uint32_t size        ; Data Size
                : uint8_t *table       ; Data address
                : USB_CB_t complete    ; Callback function
Return value    : none
******************************************************************************/
void usb_pcdc_DataTrans(USB_UTR_t *ptr, uint16_t pipe, uint32_t size, uint8_t *table, USB_CB_t complete)
{
    /* PIPE Transfer set */
    usb_gpcdc_Mess[pipe].keyword        = pipe;                 /* Pipe No */
    usb_gpcdc_Mess[pipe].tranadr        = table;                /* Data address */
    usb_gpcdc_Mess[pipe].tranlen        = size;                 /* Data Size */
    usb_gpcdc_Mess[pipe].setup          = 0;                    /* Non Setup */
    usb_gpcdc_Mess[pipe].complete       = complete;             /* Callback function */
    usb_gpcdc_Mess[pipe].segment        = USB_TRAN_END;         /* Utr member (segment) */
    usb_gpcdc_Mess[pipe].ip             = ptr->ip;              /* IP number(0or1) */
    usb_gpcdc_Mess[pipe].ipp            = ptr->ipp;             /* IP Address(USB0orUSB1) */

    R_usb_pstd_TransferStart(&usb_gpcdc_Mess[pipe]);            /* USB Transfer Start */
} /* eof usb_pcdc_DataTrans */


/******************************************************************************
Function Name   : usb_pcdc_ControlTrans1
Description     : Class request processing (control read)
Arguments       : USB_UTR_t *ptr       ; USB system internal structure. Selects channel.
                : USB_REQUEST_t *req   ; Class request information
Return value    : none
******************************************************************************/
void usb_pcdc_ControlTrans1(USB_UTR_t *ptr, USB_REQUEST_t *req)
{
    /* Is a request receive target Interface? */
    if( (req->ReqIndex == 0) && (req->ReqTypeRecip == USB_INTERFACE) )
    {
        /* Is a Request Type GetLineCoding? */
        if( req->ReqRequest == USB_PCDC_GET_LINE_CODING )
        {
            /* send get_line_coding */
            R_usb_pstd_ControlRead(ptr, (uint32_t)USB_PCDC_GET_LINE_CODING_LENGTH
                , (uint8_t *)&usb_gcdc_LineCodingReq);
        }
        else
        {
            /* Set Stall */
            usb_pstd_SetStallPipe0( ptr );    /* Req Error */
        }
    }
    else
    {
        /* Set Stall */
        usb_pstd_SetStallPipe0( ptr );        /* Req Error */
    }
} /* eof usb_pcdc_ControlTrans1 */

/******************************************************************************
Function Name   : usb_pcdc_ControlTrans4
Description     : Class request processing (control read status stage)
Arguments       : USB_UTR_t *ptr       ; USB system internal structure. Selects channel.
                : USB_REQUEST_t *req   ; Pointer to Class request information
Return value    : none
******************************************************************************/
void usb_pcdc_ControlTrans4(USB_UTR_t *ptr, USB_REQUEST_t *req)
{
    /* Is a Request Type GetLineCoding? */
    if( req->ReqRequest == USB_PCDC_GET_LINE_CODING )
    {
        usb_cstd_SetBuf(ptr, (uint16_t)USB_PIPE0);          /* Set BUF */
        /* Get Line Coding Request */
        usb_pcdc_GetLineCoding_req(ptr);
    }
    else
    {
        /* Set Stall */
        usb_pstd_SetStallPipe0( ptr );        /* Req Error */
    }
    R_usb_pstd_ControlEnd(ptr, (uint16_t)USB_CTRL_END);
} /* eof usb_pcdc_ControlTrans4 */

/******************************************************************************
Function Name   : usb_pcdc_ControlTrans3
Description     : Class request processing(control no data write)
Arguments       : USB_UTR_t *ptr       ; USB system internal structure. Selects channel.
                : USB_REQUEST_t *req   ; Pointer to Class request information
Return value    : none
******************************************************************************/
void usb_pcdc_ControlTrans3(USB_UTR_t *ptr, USB_REQUEST_t *req)
{
    /* Is a request receive target Interface? */
    if( (req->ReqIndex == 0) && (req->ReqTypeRecip == USB_INTERFACE) )
    {
        /* Is a Request Type SetControlLineState? */
        if( req->ReqRequest == USB_PCDC_SET_CONTROL_LINE_STATE )
        {
            usb_cstd_SetBuf(ptr, (uint16_t)USB_PIPE0);          /* Set BUF */
            /* DTR & RTS set value store */
            *(uint16_t *)&usb_gcdc_ControlLineState = req->ReqValue;

            /* send Set_Control_Line_State */
            usb_pcdc_SetControlLineState_req(ptr);
        }
        else if( req->ReqRequest == USB_PCDC_SEND_BREAK )
        {
            /* When Request Type case SendBreak */
            /* Duration of Break value store */
            usb_gcdc_SendBreakDuration = req->ReqValue;

            /* send Send_Break */
            usb_pcdc_SendBreak_req(ptr);
        }
        else
        {
            /* Set Stall */
            usb_pstd_SetStallPipe0( ptr );    /* Req Error */
        }
    }
    else
    {
        /* Set Stall */
        usb_pstd_SetStallPipe0( ptr );        /* Req Error */
    }
    if( req->ReqRequest != 0 )
    {
        R_usb_pstd_ControlEnd(ptr, (uint16_t)USB_CTRL_END);
    }
} /* eof usb_pcdc_ControlTrans3 */

/******************************************************************************
Function Name   : usb_pcdc_ControlTrans0
Description     : Class request processing(idle or setup stage)
Arguments       : USB_UTR_t *ptr       ; USB system internal structure. Selects channel.
                : USB_REQUEST_t *req   ; Pointer to Class request information
Return value    : none
******************************************************************************/
void usb_pcdc_ControlTrans0(USB_UTR_t *ptr, USB_REQUEST_t *req)
{
} /* eof usb_pcdc_ControlTrans0 */

/******************************************************************************
Function Name   : usb_pcdc_ControlTrans2
Description     : Class request processing (control write data stage)
Arguments       : USB_UTR_t *ptr       ; USB system internal structure. Selects channel.
                : USB_REQUEST_t *req   ; Pointer to Class request information
Return value    : none
******************************************************************************/
void usb_pcdc_ControlTrans2(USB_UTR_t *ptr, USB_REQUEST_t *req)
{
    /* Is a request receive target Interface? */
    if( (req->ReqIndex == 0) && (req->ReqTypeRecip == USB_INTERFACE) )
    {
        /* Is a Request Type SetLineCoding? */
        if( req->ReqRequest == USB_PCDC_SET_LINE_CODING )
        {
            /* send Set_Line_Coding */
            R_usb_pstd_ControlWrite(ptr, 7, (uint8_t *)&usb_gcdc_LineCodingReq);
        }
        else
        {
            /* Set Stall */
            usb_pstd_SetStallPipe0( ptr );    /* Req Error */
        }
    }
    else
    {
        /* Set Stall */
        usb_pstd_SetStallPipe0( ptr );        /* Req Error */
    }
} /* eof usb_pcdc_ControlTrans2 */

/******************************************************************************
Function Name   : usb_pcdc_ControlTrans5
Description     : Process class request (control write status stage)
Arguments       : USB_UTR_t *ptr       ; USB system internal structure. Selects channel.
                : USB_REQUEST_t *req   ; Pointer to Class request information
Return value    : none
******************************************************************************/
void usb_pcdc_ControlTrans5(USB_UTR_t *ptr, USB_REQUEST_t *req)
{
    /* Is a request receive target Interface? */
    if( (req->ReqIndex == 0) && (req->ReqTypeRecip == USB_INTERFACE) )
    {
        /* Is a Request Type SetLineCoding? */
        if( req->ReqRequest == USB_PCDC_SET_LINE_CODING )
        {
            usb_cstd_SetBuf(ptr, (uint16_t)USB_PIPE0);          /* Set BUF */
            /* send Set_Line_Coding */
            usb_pcdc_SetLineCoding_req(ptr);
        }
        else
        {
            /* Set Stall */
            usb_pstd_SetStallPipe0( ptr );    /* Req Error */
        }
    }
    else
    {
        /* Set Stall */
        usb_pstd_SetStallPipe0( ptr );        /* Req Error */
    }
    R_usb_pstd_ControlEnd(ptr, (uint16_t)USB_CTRL_END);
} /* eof usb_pcdc_ControlTrans5 */


/******************************************************************************
Function Name   : usb_pcdc_SetControlLineState
Description     : Peripheral Set Control Line State request
Arguments       : USB_UTR_t     *ptr        ; USB system internal structure. Selects channel.
                : USB_CB_INFO_t complete    ; Pointer to Callback function
Return value    : none
******************************************************************************/
void usb_pcdc_SetControlLineState(USB_UTR_t *ptr, USB_CB_INFO_t complete)
{
    USB_MH_t    p_blf;
    USB_ER_t    err, err2;
    USB_UTR_t   *cp;

    /* Get mem pool blk */
    err = R_USB_PGET_BLK(USB_PCDC_MPL, &p_blf);
    if( err == USB_E_OK )
    {
        cp = (USB_UTR_t*)p_blf;
        cp->msghead = (USB_MH_t)NULL;
        cp->msginfo = (uint16_t)USB_PCDC_PSET_CONTROL_LINE_STATE;   /* Set message information. */
        cp->keyword = (uint16_t)USB_NOT_USED;                   /* Not use */
        cp->status      = (uint16_t)USB_NOT_USED;               /* Not use */
        cp->complete    = (USB_CB_t)complete;                   /* Set Callback function */
        cp->ip = ptr->ip;                                       /* IP number(0or1) */
        cp->ipp = ptr->ipp;                                     /* IP Address(USB0orUSB1) */

        /* Send message */
        err = R_USB_SND_MSG(USB_PCDC_MBX, (USB_MSG_t*)p_blf);
        if( err != USB_E_OK )
        {
            /* Send message failure */
            USB_PRINTF1( "### pcdcSetControlLineState snd_msg error (%ld)\n", err);
            err2 = R_USB_REL_BLK(USB_PCDC_MPL, (USB_MH_t)p_blf);
            if( err2 != USB_E_OK )
            {
                /* Release memoryblock failure */
                USB_PRINTF1( "### pcdcSetControlLineState rel_blk error (%ld)\n", err2 );
            }
        }
    }
    else
    {
        /* Get momoryblock failure */
        USB_PRINTF1( "### pcdcSetControlLineState pget_blk error (%ld)\n", err);
    }
} /* eof usb_pcdc_SetControlLineState */

/******************************************************************************
Function Name   : usb_pcdc_SetLineCoding
Description     : Peripheral Set Line Coding request
Arguments       : USB_UTR_t     *ptr        ; USB system internal structure. Selects channel.
                : USB_CB_INFO_t complete    ; Pointer to Callback function
Return value    : none
******************************************************************************/
void usb_pcdc_SetLineCoding(USB_UTR_t *ptr, USB_CB_INFO_t complete)
{
    USB_MH_t    p_blf;
    USB_ER_t    err, err2;
    USB_UTR_t   *cp;

    /* Get mem pool blk */
    err = R_USB_PGET_BLK(USB_PCDC_MPL, &p_blf);
    if( err == USB_E_OK )
    {
        cp = (USB_UTR_t*)p_blf;
        cp->msghead = (USB_MH_t)NULL;
        cp->msginfo = (uint16_t)USB_PCDC_PSET_LINE_CODING;      /* Set message information. */
        cp->keyword = (uint16_t)USB_NOT_USED;                   /* Not use */
        cp->status      = (uint16_t)USB_NOT_USED;               /* Not use */
        cp->complete    = (USB_CB_t)complete;                   /* Set Callback function */
        cp->ip = ptr->ip;                                       /* IP number(0or1) */
        cp->ipp = ptr->ipp;                                     /* IP Address(USB0orUSB1) */

        /* Send message */
        err = R_USB_SND_MSG(USB_PCDC_MBX, (USB_MSG_t*)p_blf);
        if( err != USB_E_OK )
        {
            /* Send message failure */
            USB_PRINTF1( "### pcdcGetLineCoding snd_msg error (%ld)\n", err );
            err2 = R_USB_REL_BLK(USB_PCDC_MPL, (USB_MH_t)p_blf);
            if( err2 != USB_E_OK )
            {
                /* Release memoryblock failure */
                USB_PRINTF1( "### pcdcGetLineCoding rel_blk error (%ld)\n", err2 );
            }
        }
    }
    else
    {
        /* Get momoryblock failure */
        USB_PRINTF1( "### pcdcGetLineCoding pget_blk error (%ld)\n", err );
    }
} /* eof usb_pcdc_SetLineCoding */

/******************************************************************************
Function Name   : usb_pcdc_GetLineCoding
Description     : Peripheral Get Line Coding request
Arguments       : USB_UTR_t     *ptr        ; USB system internal structure. Selects channel.
                : USB_CB_INFO_t complete    ; Callback function
Return value    : none
******************************************************************************/
void usb_pcdc_GetLineCoding(USB_UTR_t *ptr, USB_CB_INFO_t complete)
{
    USB_MH_t    p_blf;
    USB_ER_t    err, err2;
    USB_UTR_t   *cp;

    /* Get mem pool blk */
    err = R_USB_PGET_BLK(USB_PCDC_MPL, &p_blf);
    if( err == USB_E_OK )
    {
        cp = (USB_UTR_t*)p_blf;
        cp->msghead = (USB_MH_t)NULL;
        cp->msginfo = (uint16_t)USB_PCDC_PGET_LINE_CODING;      /* Set message information. */
        cp->keyword = (uint16_t)USB_NOT_USED;                   /* Not use */
        cp->status      = (uint16_t)USB_NOT_USED;               /* Not use */
        cp->complete    = (USB_CB_t)complete;                   /* Set Callback function */
        cp->ip = ptr->ip;                                       /* IP number(0or1) */
        cp->ipp = ptr->ipp;                                     /* IP Address(USB0orUSB1) */

        /* Send message */
        err = R_USB_SND_MSG(USB_PCDC_MBX, (USB_MSG_t*)p_blf);
        if( err != USB_E_OK )
        {
            /* Send message failure */
            USB_PRINTF1( "### pcdcSetLineCoding snd_msg error (%ld)\n", err );
            err2 = R_USB_REL_BLK(USB_PCDC_MPL, (USB_MH_t)p_blf);
            if( err2 != USB_E_OK )
            {
                /* Release memoryblock failure */
                USB_PRINTF1( "### pcdcSetLineCoding rel_blk error (%ld)\n", err2 );
            }
        }
    }
    else
    {
        /* Get momoryblock failure */
        USB_PRINTF1( "### pcdcSetLineCoding pget_blk error (%ld)\n", err );
    }
} /* eof usb_pcdc_GetLineCoding */

/******************************************************************************
Function Name   : usb_pcdc_SendBreak
Description     : Peripheral Send Break request
Arguments       : USB_UTR_t     *ptr        ; USB system internal structure. Selects channel.
                : USB_CB_INFO_t complete    ; Pointer to Callback function
Return value    : none
******************************************************************************/
void usb_pcdc_SendBreak(USB_UTR_t *ptr, USB_CB_INFO_t complete)
{
    USB_MH_t    p_blf;
    USB_ER_t    err, err2;
    USB_UTR_t   *cp;

    /* Get mem pool blk */
    err = R_USB_PGET_BLK(USB_PCDC_MPL, &p_blf);
    if( err == USB_E_OK )
    {
        cp = (USB_UTR_t*)p_blf;
        cp->msghead = (USB_MH_t)NULL;
        cp->msginfo = (uint16_t)USB_PCDC_PSEND_BREAK;           /* Set message information. */
        cp->keyword = (uint16_t)USB_NOT_USED;                   /* Not use */
        cp->status      = (uint16_t)USB_NOT_USED;               /* Not use */
        cp->complete    = (USB_CB_t)complete;                   /* Set Callback function */
        cp->ip = ptr->ip;                                       /* IP number(0or1) */
        cp->ipp = ptr->ipp;                                     /* IP Address(USB0orUSB1) */

        /* Send message */
        err = R_USB_SND_MSG(USB_PCDC_MBX, (USB_MSG_t*)p_blf);
        if( err != USB_E_OK )
        {
            /* Send message failure */
            USB_PRINTF1( "### pcdcGetLineCoding snd_msg error (%ld)\n", err );
            err2 = R_USB_REL_BLK(USB_PCDC_MPL, (USB_MH_t)p_blf);
            if( err2 != USB_E_OK )
            {
                /* Release memoryblock failure */
                USB_PRINTF1( "### pcdcSendBreak rel_blk error (%ld)\n", err2 );
            }
        }
    }
    else
    {
        /* Get momoryblock failure */
        USB_PRINTF1( "### pcdcSendBreak pget_blk error (%ld)\n", err );
    }
} /* eof usb_pcdc_SendBreak */

/******************************************************************************
End  Of File
******************************************************************************/
