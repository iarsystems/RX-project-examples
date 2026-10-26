/******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized.
* This software is owned by Renesas Electronics Corporation and is  protected
* under all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING 
* BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
* PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. 
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE  LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this
* software and to discontinue the availability of this software.
*    By using this software, you agree to the additional terms and conditions 
* found by accessing the following link: 
* http://www.renesas.com/disclaimer
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.    
*******************************************************************************
* File Name    : r_usb_cintfifo.c
* Version      : 2.00
* Device(s)    : Renesas SH-Series, RX-Series
* Tool-Chain   : Renesas SuperH RISC engine Standard Toolchain
*              : Renesas RX Standard Toolchain
* OS           : Common to None and uITRON 4.0 Spec
* H/W Platform : Independent
* Description  : USB Host and Peripheral interrupt code
*******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 29.07.2011 0.50    First Release
*         : 3.27.2012          Comments tranlation (REA). Changed tabs to 4 spa-
*                              ces. SET EDITOR TO INSERT SPACES WHEN TAB PRESSED.
******************************************************************************/



/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_usb_ctypedef.h"         /* Type define */
#include "r_usb_usrconfig.h"        /* USB-H/W register set (user define) */
#include "r_usb_cdefusbip.h"        /* USB-FW Library Header */
#include "r_usb_cmacprint.h"        /* Standard IO macro */
#include "r_usb_cmacsystemcall.h"   /* System call macro */
#include "r_usb_cextern.h"          /* USB-FW global define */
#include "r_usb_reg_access.h"
#include "r_usb_cusb_bitdefine.h"


/******************************************************************************
Section    <Section Definition> , "Project Sections"
******************************************************************************/
#pragma section="_usb"

/******************************************************************************
Constant macro definitions
******************************************************************************/


/******************************************************************************
External variables and functions
******************************************************************************/
void    usb_cstd_nrdy_endprocess( USB_UTR_t *ptr, uint16_t pipe );
extern void usb_cstd_brdy_pipe(void);
/******************************************************************************
Private global variables and functions
******************************************************************************/


/******************************************************************************
Function Name   : usb_cstd_BrdyPipe
Description     : Search for the PIPE No. that BRDY interrupt occurred, and 
                  request data transmission/reception from the PIPE
Arguments       : USB_UTR_t *ptr
                : uint16_t bitsts       ; BRDYSTS Register & BRDYENB Register
Return value    : none
******************************************************************************/
void usb_cstd_BrdyPipe(USB_UTR_t *ptr, uint16_t bitsts)
{
    usb_cstd_brdy_pipe();
}
/******************************************************************************
End of function usb_cstd_BrdyPipe
******************************************************************************/


/******************************************************************************
Function Name   : usb_cstd_NrdyPipe
Description     : Search for PIPE No. that occurred NRDY interrupt, and execute 
                  the process for PIPE when NRDY interrupt occurred
Arguments       : USB_UTR_t *ptr
                : uint16_t bitsts       ; NRDYSTS Register & NRDYENB Register
Return value    : none
******************************************************************************/
void usb_cstd_NrdyPipe(USB_UTR_t *ptr, uint16_t bitsts)
{
/* PERI spec                                                            */
/*  Transmitting pipe                                                   */
/* @5       a) only NRDY                                                */
/* @1       b) NRDY+OVRN    (Isochronous)                               */
/*  Receive pipe                                                        */
/* @5       a) only NRDY                                                */
/* @1       b) NRDY+OVRN    (Isochronous)                               */
/* @2       c) only NRDY    (interval error of isochronous)             */
/* HOST spec                                                            */
/*  Transmitting pipe                                                   */
/* @1       a) NRDY+OVRN    (Isochronous)                               */
/* @4       b) NRDY+NAK     (Ignore)                                    */
/* @3       c) NRDY+STALL   (Receive STALL)                             */
/*  Receive pipe                                                        */
/* @1       a) NRDY+OVRN    (Isochronous)                               */
/* @4       b) NRDY+NAK     (Ignore)                                    */
/* @2       c) NRDY         (Ignore of isochronous)                     */
/* @2       d) NRDY         (CRC error of isochronous)                  */
/* @3       e) NRDY+STALL   (Receive STALL)                             */

    uint16_t        buffer, i;

    for( i = USB_MIN_PIPE_NO; i <= USB_MAX_PIPE_NO; i++ )
    {
        if( (bitsts & USB_BITSET(i)) != 0 )
        {
            /* Interrupt check */
            if( usb_gcstd_Pipe[ptr->ip][i] != USB_NULL )
            {
                if( usb_cstd_GetPipeType(ptr, i) == USB_ISO )
                {
                    /* Wait for About 60ns */
                    buffer = usb_creg_read_frmnum( ptr );
                    if( (buffer & USB_OVRN) == USB_OVRN )
                    {
                        /* @1 */
                        /* End of data transfer */
                        usb_cstd_ForcedTermination(ptr, i, (uint16_t)USB_DATA_OVR);
                        USB_PRINTF1("###ISO OVRN %d\n", usb_gcstd_DataCnt[ptr->ip][i]);
                    }
                    else
                    {
                        /* @2 */
                        /* End of data transfer */
                        usb_cstd_ForcedTermination(ptr, i, (uint16_t)USB_DATA_ERR);
                    }
                }
                else
                {
                    usb_cstd_nrdy_endprocess( ptr, i );
                }
            }
        }
    }
}
/******************************************************************************
End of function usb_cstd_NrdyPipe
******************************************************************************/


/******************************************************************************
Function Name   : usb_cstd_BempPipe
Description     : Search for PIPE No. that BEMP interrupt occurred, and complete data transmission for the PIPE
Arguments       : USB_UTR_t *ptr
                : uint16_t bitsts       ; BEMPSTS Register & BEMPENB Register
Return value    : none
******************************************************************************/
void usb_cstd_BempPipe(USB_UTR_t *ptr, uint16_t bitsts)
{
    uint16_t        buffer, i;
    uint16_t        useport;

    for( i = USB_MIN_PIPE_NO; i <= USB_PIPE5; i++ )
    {
        if( (bitsts & USB_BITSET(i)) != 0 )
        {
            /* Interrupt check */
            if( usb_gcstd_Pipe[ptr->ip][i] != USB_NULL )
            {
                buffer = usb_cstd_GetPid(ptr, i);
                /* MAX packet size error ? */
                if( (buffer & USB_PID_STALL) == USB_PID_STALL )
                {
                    USB_PRINTF1("### STALL Pipe %d\n", i);
                    usb_cstd_ForcedTermination(ptr, i, (uint16_t)USB_DATA_STALL);
                }
                else
                {
                    if( (usb_creg_read_pipectr( ptr, i ) & USB_INBUFM) != USB_INBUFM )
                    {
                        /* Pipe number to FIFO port select */
                        useport = usb_cstd_Pipe2Fport(ptr, i);
                        if( useport == USB_D0DMA )
                        {
#if USB_TARGET_CHIP_PP == USB_RX600_PP
                            usb_creg_clr_dreqe( ptr, USB_D0DMA );
                            /* DMA stop */
                            usb_cpu_d0fifo_stop_dma(ptr);
#endif /* USB_TARGET_CHIP_PP == USB_RX600_PP */
                            usb_creg_clr_sts_bemp( ptr, i );
                        }
                        /* End of data transfer */
                        usb_cstd_DataEnd(ptr, i, (uint16_t)USB_DATA_NONE);
                    }
                }
            }
        }
    }
    for( i = USB_PIPE6; i <= USB_MAX_PIPE_NO; i++ )
    {
        /* Interrupt check */
        if( (bitsts & USB_BITSET(i)) != 0 )
        {
            if( usb_gcstd_Pipe[ptr->ip][i] != USB_NULL )
            {
                buffer = usb_cstd_GetPid(ptr, i);
                /* MAX packet size error ? */
                if( (buffer & USB_PID_STALL) == USB_PID_STALL )
                {
                    USB_PRINTF1("### STALL Pipe %d\n", i);
                    usb_cstd_ForcedTermination(ptr, i, (uint16_t)USB_DATA_STALL);
                }
                else
                {
                    /* End of data transfer */
                    usb_cstd_DataEnd(ptr, i, (uint16_t)USB_DATA_NONE);
                }
            }
        }
    }
}
/******************************************************************************
End of function usb_cstd_BempPipe
******************************************************************************/

/******************************************************************************
End  Of File
******************************************************************************/
