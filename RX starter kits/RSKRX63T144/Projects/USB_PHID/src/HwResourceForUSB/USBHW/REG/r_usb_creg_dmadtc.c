/*******************************************************************************
* DISCLAIMER
*
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. 
*
* No other uses are authorized.
*
* This software is owned by Renesas Electronics Corp. and is protected under
* all applicable laws, including copyright laws.
*
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
*
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE  LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
*
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.    
*******************************************************************************
* File Name    : r_usb_creg_dmadtc.c
* Version      : 2.00
* Device(s)    : Renesas SH-Series, RX-Series
* Tool-Chain   : Renesas SuperH RISC engine Standard Toolchain
*              : Renesas RX Standard Toolchain
* OS           : Common to None and uITRON 4.0 Spec
* H/W Platform : Independent
* Description  : Setting code of DMA/DTC
*******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 29.07.2011 0.50    First Release
*         : 1.3.2012           Comments tranlation (REA). Changed tabs to 4 spa-
*                              ces. SET EDITOR TO INSERT SPACES WHEN TAB PRESSED.
******************************************************************************/



/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_usb_ctypedef.h"     /* Type define */
#include "r_usb_usrconfig.h"
#include "r_usb_ckernelid.h"        /* Kernel ID definition */
#include "r_usb_cdefusbip.h"        /* USB-FW Library Header */
#include "r_usb_cmacprint.h"        /* Standard IO macro */
#include "r_usb_cmacsystemcall.h"   /* System call macro */
#include "r_usb_cextern.h"          /* USB-FW global definition */
#include <iorx63t.h>
#include "r_usb_reg_access.h"
#include "r_usb_cusb_bitdefine.h"

extern void     usb_cpu_d0fifo_enable_dma(USB_UTR_t *ptr );
extern void     usb_cpu_d0fifo_disable_dma(USB_UTR_t *ptr );
extern uint32_t usb_cpu_get_dtc_Source_address(USB_UTR_t *ptr);
extern uint16_t usb_cpu_get_dtc_block_count(USB_UTR_t *ptr);
extern USB_UTR_t       usb_gcstd_IntMsgD0fifo;
extern USB_UTR_t       usb2_gcstd_IntMsgD0fifo;

/******************************************************************************
Section    <Section Definition> , "Project Sections"
******************************************************************************/
#pragma section="_usb"

/******************************************************************************
Function Name   : usb_cstd_Buf2D0fifoStartUsb
Description     : Setup to start DMA/DTC transfer from data buffer to D0FIFO.
Arguments       : USB_UTR_t *ptr  : USB internal structure. Selects USB channel.
Return value    : none
******************************************************************************/
void usb_cstd_Buf2D0fifoStartUsb(USB_UTR_t *ptr)
{
    uint16_t    mbw;

    /* Write (MEMORY -> FIFO) : USB register set */
    if( (usb_gcstd_Dma0Size[ptr->ip] & (uint16_t)0x0001) != 0 )
    {
        mbw = USB_MBW_8;
    }
    else
    {
        mbw = USB_MBW_16;
    }
    /* Change MBW setting */
    usb_creg_set_mbw( ptr, USB_D0DMA, mbw );

    /* DTC(D0FIFO) interrupt enable */
    usb_cpu_d0fifo_enable_dma(ptr);

    /* Set DREQ enable */
    usb_creg_set_dreqe( ptr, USB_D0DMA );
}/* eof usb_cstd_Buf2D0fifoStartUsb() */

/******************************************************************************
Function Name   : usb_cstd_D0fifo2BufStartUsb
Description     : Setup to start DMA/DTC transfer D0FIFO to buffer.
Arguments       : USB_UTR_t *ptr  : USB internal structure. Selects USB channel.
Return value    : none
******************************************************************************/
void usb_cstd_D0fifo2BufStartUsb(USB_UTR_t *ptr)
{
    /* Read (FIFO -> MEMORY) : USB register set */
    /* DMA buffer clear mode & MBW set */
    usb_creg_set_mbw( ptr, USB_D0DMA, (uint16_t)(USB_MBW_16) );
    usb_creg_clr_dclrm( ptr, USB_D0DMA );

    /* Set DREQ enable */
    usb_creg_set_dreqe( ptr, USB_D0DMA );
}/* eof usb_cstd_D0fifo2BufStartUsb */

/******************************************************************************
Function Name   : usb_cstd_D0fifoStopUsb
Description     : Setup external variables used for USB data transfer; to reg-
                : ister if you want to stop the transfer of DMA/DTC.
Arguments       : USB_UTR_t *ptr  : USB internal structure. Selects USB channel.
Return value    : none
******************************************************************************/
void usb_cstd_D0fifoStopUsb(USB_UTR_t *ptr)
{
    uint16_t    ip = ptr->ip;
    uint16_t    pipe = usb_gcstd_Dma0Pipe[ip];
    uint32_t    transfer_size = usb_gcstd_Dma0Size[ip];
    uint32_t    *request_size = &usb_gcstd_DataCnt[ip][pipe];
    uint8_t     *tran_data_ptr = (uint8_t *)&usb_gcstd_DataPtr[ip][pipe];

    usb_creg_clr_dreqe( ptr, USB_D0DMA );

    /* Direction check */
    if( usb_gcstd_Dma0Dir[ip] == USB_BUF2FIFO )
    {
        /* Buffer to FIFO */
        if( *request_size < transfer_size )
        {
            /* >yes then set BVAL */
            *tran_data_ptr += *request_size;
            *request_size = (uint32_t)0u;
            /* Disable Ready Interrupt */
            usb_creg_clr_brdyenb(ptr, pipe);
            /* Set BVAL */
            usb_creg_set_bval( ptr, USB_D0DMA );
        }
        else
        {
            *tran_data_ptr += transfer_size;
            /* Set data count to remain */
            *request_size -= transfer_size;
        }
    }
    else
    {
        /* FIFO to Buffer */
        *tran_data_ptr += transfer_size;
        /* Set data count to remain */
        if( *request_size < transfer_size )
        {
            *request_size = transfer_size - *request_size;
        }
        else
        {
            *request_size -= transfer_size;
        }
    }
}/* eof usb_cstd_D0fifoStopUsb() */

/******************************************************************************
Function Name   : usb_cstd_D0fifoInt
Description     : Set end of DMA/DTC transfer. Set to restart DMA/DTC trans-
                : fer according to data size of remaining functions to be pro-
                : cessed.
Arguments       : USB_UTR_t *ptr  : USB internal structure. Selects USB channel.
Return value    : none
******************************************************************************/
void usb_cstd_D0fifoInt(USB_UTR_t *ptr)
{
    uint16_t pipe;
    uint16_t ip;
    uint16_t maxps;
    uint32_t *transfer_count;

    ip = ptr->ip;
    pipe = usb_gcstd_Dma0Pipe[ip];
    maxps = usb_gcstd_Dma0Fifo[ip];
    transfer_count = &usb_gcstd_DataCnt[ip][pipe];

    /* DMA Transfer Request Clear */
    usb_creg_clr_dreqe( ptr, USB_D0DMA );

    /* Transfer count > MAXPS */
    if ( *transfer_count > maxps )
    {
        /* DMA Transfer count update */
        *transfer_count %= maxps;
        /* Transfer continue check */
        if( *transfer_count != 0 )
        {
            /* Transfer count != MAXPS * N */
            /* Odd size data check  */
            if( (*transfer_count & 0x0001u) != 0u )
            {
                /* if count == odd */
                usb_creg_set_mbw( ptr, USB_D0DMA, USB_MBW_8 );
            }
            /* DMA Transfer size update */
            usb_gcstd_Dma0Size[ip] = *transfer_count;
            /* DMA Restart */
            usb_cpu_d0fifo_restart_dma(ptr);
            /* DMA Transfer Request Set */
            usb_creg_set_dreqe( ptr, USB_D0DMA );
        }
    }
    else if( *transfer_count == 0 )
    {
        /* More than enough Interrupt */
        return;
    }
    else
    {
        /* Transfer count < MAXPS */
        usb_creg_set_bval( ptr, USB_D0DMA );
        /* Transfer complete */
        *transfer_count = 0;
    }

    /* Transfer complete check */
    if( *transfer_count == 0 )
    {
        /* Transfer complete */
        usb_cpu_d0fifo_stop_dma(ptr);

        /* Enable Empty Interrupt */
        usb_creg_set_bempenb(ptr, pipe);
    }

}/* eof usb_cstd_D0fifoInt() */

/******************************************************************************
Function Name   : usb_cstd_Buf2fifoStartDma
Description     : Start transfer using DMA/DTC. If transfer size is 0, write 
                : more data to buffer.
Arguments       : USB_UTR_t *ptr  : USB internal structure. Selects USB channel.
                : uint16_t pipe   : Pipe nr.
                : uint16_t useport: FIFO select
Return value    : none
******************************************************************************/
void    usb_cstd_Buf2fifoStartDma( USB_UTR_t *ptr, uint16_t pipe, uint16_t useport )
{
    /* Transfer size check */
    if( usb_gcstd_Dma0Size[ptr->ip] != 0 )
    {
        /* 16bit access */
        if ( (usb_gcstd_Dma0Size[ptr->ip] & 0x0001u) == 0u )
        {
            /* DMA access Buffer to FIFO start */
            usb_cpu_buf2d0fifo_start_dma(ptr, usb_cstd_GetD0fifo16Adr(ptr));
        }
        else
        {
            /* 8bit access */
            /* DMA access Buffer to FIFO start */
            usb_cpu_buf2d0fifo_start_dma(ptr, usb_cstd_GetD0fifo8Adr(ptr));
        }
        /* Changes the FIFO port by the pipe. */
        usb_cstd_chg_curpipe(ptr, pipe, useport, USB_NO);
        /* Enable Not Ready Interrupt */
        usb_cstd_NrdyEnable(ptr, pipe);
        /* CPU access Buffer to FIFO start */
        usb_cstd_Buf2D0fifoStartUsb(ptr);
    }
    else
    {
        /* Buffer to FIFO data write */
        usb_cstd_Buf2Fifo(ptr, pipe, useport);
    }
}/* eof usb_cstd_Buf2fifoStartDma() */

/******************************************************************************
Function Name   : usb_cstd_Fifo2BufStartDma
Description     : Start transfer using DMA/DTC. If transfer size is 0, clear DMA. 
Arguments       : USB_UTR_t *ptr  : USB internal structure. Selects USB channel.
                : uint16_t pipe   : Pipe nr.
                : uint16_t useport: FIFO select
                : uint32_t length
Return value    : none
******************************************************************************/
void    usb_cstd_Fifo2BufStartDma( USB_UTR_t *ptr, uint16_t pipe, uint16_t useport, uint32_t length )
{
    uint16_t        mxps;

    /* Data size check */
    if( usb_gcstd_Dma0Size[ptr->ip] != 0u )
    {
        /* 16bit access */
        if( (usb_gcstd_Dma0Size[ptr->ip] & 0x0001u) == 0u )
        {
            /* D0FIFO to Buffer DMA read start */
            usb_cpu_d0fifo2buf_start_dma(ptr, usb_cstd_GetD0fifo16Adr(ptr));
        }
        else
        {
            /* 8bit access */
            /* D0FIFO to Buffer DMA read start */
            usb_cpu_d0fifo2buf_start_dma(ptr, usb_cstd_GetD0fifo8Adr(ptr));
        }

        /* Changes the FIFO port by the pipe. */
        usb_cstd_chg_curpipe(ptr, pipe, useport, USB_NO);
        /* Max Packet Size */
        mxps = usb_cstd_GetMaxPacketSize(ptr, pipe);
        if( length != (uint32_t)0u )
        {
            /* Data length check */
            if( (length % mxps) == (uint32_t)0u )
            {
                /* Set Transaction counter */
                usb_cstd_SetTransactionCounter(ptr, pipe, (uint16_t)(length / mxps));
            }
            else
            {
                /* Set Transaction counter */
                usb_cstd_SetTransactionCounter(ptr, pipe, (uint16_t)((length / mxps) + (uint32_t)1u));
            }
        }
        /* Set BUF */
        usb_cstd_SetBuf(ptr, pipe);
        /* Enable Ready Interrupt */
        usb_creg_set_brdyenb(ptr, pipe);
        /* Enable Not Ready Interrupt */
        usb_cstd_NrdyEnable(ptr, pipe);
        usb_cstd_D0fifo2BufStartUsb(ptr);
    }
    else
    {
        /* Changes the FIFO port by the pipe. */
        usb_cstd_chg_curpipe(ptr, pipe, useport, USB_NO);
        /* DMA buffer clear mode set */
        usb_creg_set_dclrm( ptr, USB_D0DMA );
        /* Set BUF */
        usb_cstd_SetBuf(ptr, pipe);
        /* Enable Ready Interrupt */
        usb_creg_set_brdyenb(ptr, pipe);
        /* Enable Not Ready Interrupt */
        usb_cstd_NrdyEnable(ptr, pipe);
    }
}/* eof usb_cstd_Fifo2BufStartDma() */

#if USB_FUNCSEL_USBIP0_PP == USB_HOST_PP || USB_FUNCSEL_USBIP0_PP == USB_PERI_PP
/******************************************************************************
Function Name   : usb_cstd_d0fifo_handler
Description     : DMA interrupt routine. Send message to PCD/HCD task.
Arguments       : none
Return value    : none
******************************************************************************/
void usb_cstd_d0fifo_handler(void)
{
    USB_UTR_t       *ptr;

    ptr = &usb_gcstd_IntMsgD0fifo;
    ptr->msghead = (USB_MH_t)USB_NULL;
    ptr->keyword = 0;
    ptr->status  = 0;
    ptr->ip  = USB_USBIP_0;
    ptr->ipp = usb_cstd_GetUsbIpAdr( ptr->ip );

    if( usb_cstd_is_host_mode(ptr) == USB_NO )
    {
#if USB_FUNCSEL_USBIP0_PP == USB_PERI_PP
        USB_ER_t        err;

        /* Peripheral Function */
        ptr->msginfo = USB_MSG_PCD_D0FIFO_INT;
        /* Send message */
        err = USB_ISND_MSG(USB_PCD_MBX, (USB_MSG_t*)ptr);
        if( err != USB_E_OK )
        {
                USB_PRINTF1("### DmaHandler DEF1 isnd_msg error (%ld)\n", err);
        }
#endif  /* USB_FUNCSEL_USBIP0_PP == USB_PERI_PP */
    }
    else
    {
#if USB_FUNCSEL_USBIP0_PP == USB_HOST_PP
        USB_ER_t        err;

        ptr->msginfo = USB_MSG_HCD_D0FIFO_INT;
        /* Send message */
        err = USB_ISND_MSG(USB_HCD_MBX, (USB_MSG_t*)ptr);
        if( err != USB_E_OK )
        {
            USB_PRINTF1("### DmaHandler DEF2 isnd_msg error (%ld)\n", err);
        }
#endif  /* USB_FUNCSEL_USBIP0_PP == USB_HOST_PP */
    }
} /* eof usb_cstd_d0fifo_handler() */
#endif  /* USB_FUNCSEL_USBIP0_PP == USB_HOST_PP || USB_FUNCSEL_USBIP0_PP == USB_PERI_PP */

#if USB_FUNCSEL_USBIP1_PP == USB_HOST_PP || USB_FUNCSEL_USBIP1_PP == USB_PERI_PP
/******************************************************************************
Function Name   : usb2_cstd_d0fifo_handler
Description     : DMA interrupt routine. Send message to PCD/HCD task.
Arguments       : none
Return value    : none
******************************************************************************/
void usb2_cstd_d0fifo_handler(void)
{
/* Conditional compile dep. on difference of USB function */
#if USB_FUNCSEL_USBIP1_PP != USB_NOUSE_PP
    USB_UTR_t       *ptr;

    ptr = &usb2_gcstd_IntMsgD0fifo;
    ptr->msghead = (USB_MH_t)USB_NULL;
    ptr->keyword = 0;
    ptr->status  = 0;
    ptr->ip  = USB_USBIP_1;
    ptr->ipp = usb_cstd_GetUsbIpAdr( ptr->ip );

    if( usb_cstd_is_host_mode(ptr) == USB_NO )
    {
#if USB_FUNCSEL_USBIP1_PP == USB_PERI_PP
        USB_ER_t        err;

        /* Peripheral Function */
        ptr->msginfo = USB_MSG_PCD_D0FIFO_INT;
        /* Send message */
        err = USB_ISND_MSG(USB_PCD_MBX, (USB_MSG_t*)ptr);
        if( err != USB_E_OK )
        {
            USB_PRINTF1("### DmaHandler DEF1 isnd_msg error (%ld)\n", err);
        }
#endif /* USB_FUNCSEL_USBIP1_PP != USB_NOUSE_PP */
    }
    else
    {
#if USB_FUNCSEL_USBIP1_PP == USB_HOST_PP
        USB_ER_t        err;

        /* Host Function */
        ptr->msginfo = USB_MSG_HCD_D0FIFO_INT;
        /* Send message */
        err = USB_ISND_MSG(USB_HCD_MBX, (USB_MSG_t*)ptr);
        if( err != USB_E_OK )
        {
            USB_PRINTF1("### DmaHandler DEF2 isnd_msg error (%ld)\n", err);
        }
#endif /* USB_FUNCSEL_USBIP1_PP == USB_HOST_PP */
    }
#endif  /* USB_FUNCSEL_USBIP1_PP != USB_NOUSE_PP */
} /* eof usb2_cstd_d0fifo_handler() */
#endif  /* USB_FUNCSEL_USBIP1_PP == USB_HOST_PP || USB_FUNCSEL_USBIP1_PP == USB_PERI_PP */

/******************************************************************************
Function Name   : usb_cstd_brdy_pipe
Description     : Search for the PIPE No. that BRDY interrupt occurred, and 
                  request data transmission/reception from the PIPE
Arguments       : USB_UTR_t *ptr
                : uint16_t bitsts       ; BRDYSTS Register & BRDYENB Register
Return value    : none
******************************************************************************/
void usb_cstd_brdy_pipe(USB_UTR_t *ptr, uint16_t bitsts)
{
    uint16_t        useport, buffer, i;
    uint16_t ip;
#if USB_TARGET_CHIP_PP == USB_RX600_PP
    uint16_t maxps;
    uint16_t set_dtc_brock_cnt;
    uint16_t trans_dtc_block_cnt;
#endif /* USB_TARGET_CHIP_PP == USB_RX600_PP */

    ip = ptr->ip;

#if USB_TARGET_CHIP_PP == USB_RX600_PP
    maxps = usb_gcstd_Dma0Fifo[ip];
#endif /* USB_TARGET_CHIP_PP == USB_RX600_PP */

    for( i = USB_PIPE1; i <= USB_MAX_PIPE_NO; i++ )
    {
        if( (bitsts & USB_BITSET(i)) != 0 )
        {
            /* Interrupt check */
            usb_creg_clr_sts_bemp( ptr, i );

            if( usb_gcstd_Pipe[ip][i] != USB_NULL )
            {
                /* Pipe number to FIFO port select */
                useport = usb_cstd_Pipe2Fport(ptr, i);
                if( useport == USB_D0DMA )
                {
#if USB_TARGET_CHIP_PP == USB_RX600_PP
                    /* DMA Transfer request disable */
                    usb_creg_clr_dreqe( ptr, USB_D0DMA );
#endif /* USB_TARGET_CHIP_PP == USB_RX600_PP */
                
                    /* DMA stop */
                    usb_cpu_d0fifo_stop_dma(ptr);
                
#if USB_TARGET_CHIP_PP == USB_RX600_PP
                    /* Changes FIFO port by the pipe. */
                    buffer = usb_cstd_is_set_frdy(ptr, i, useport, USB_NO);

                    set_dtc_brock_cnt = (uint16_t)((usb_gcstd_DataCnt[ip][usb_gcstd_Dma0Pipe[ip]] -1)
                        / usb_gcstd_Dma0Fifo[ip]) +1;

                    trans_dtc_block_cnt = usb_cpu_get_dtc_block_count(ptr);
                    /* Get D0fifo Receive Data Length */
                    usb_gcstd_Dma0Size[ip]
                        = (uint32_t)(buffer & USB_DTLN) + (set_dtc_brock_cnt - (trans_dtc_block_cnt + 1)) * maxps;

                    /* Check data count */
                    if( usb_gcstd_Dma0Size[ip] == usb_gcstd_DataCnt[ptr->ip][i] )
#else   /* USB_TARGET_CHIP_PP == USB_RX600_PP */
                    /* Check data count */
                    if( usb_gcstd_DataCnt[ip][i] == (uint32_t)0u )
#endif  /* USB_TARGET_CHIP_PP == USB_RX600_PP */
                    {
                        usb_gcstd_DataCnt[ip][i] = 0;
                        /* End of data transfer */
                        usb_cstd_DataEnd(ptr, i, (uint16_t)USB_DATA_OK);
                    }
                    else if( usb_gcstd_Dma0Size[ip] > usb_gcstd_DataCnt[ip][i] )
                    {
                        /* D0FIFO access DMA stop */
                        usb_cstd_D0fifoStopUsb(ptr);
                        /* End of data transfer */
                        usb_cstd_DataEnd(ptr, i, (uint16_t)USB_DATA_OVR);
                    }
                    else
                    {
#if USB_TARGET_CHIP_PP != USB_RX600_PP
                        /* Changes FIFO port by the pipe. */
                        buffer = usb_cstd_is_set_frdy(ptr, i, useport, USB_NO);
                        usb_gcstd_Dma0Size[ip] = (uint16_t)(buffer & USB_DTLN);
#endif /* USB_TARGET_CHIP_PP != USB_RX600_PP */
                        /* D0FIFO access DMA stop */
                        usb_cstd_D0fifoStopUsb(ptr);
                        /* End of data transfer */
                        usb_cstd_DataEnd(ptr, i, (uint16_t)USB_DATA_SHT);
                    }
#if USB_TARGET_CHIP_PP == USB_RX600_PP
                    /* Set BCLR */
                    usb_creg_set_bclr( ptr, USB_D0DMA );
#endif /* USB_TARGET_CHIP_PP == USB_RX600_PP */
                }
                else
                {
                    if( usb_cstd_GetPipeDir(ptr, i) == USB_BUF2FIFO )
                    {
                        /* Buffer to FIFO data write */
                        usb_cstd_Buf2Fifo(ptr, i, useport);
                    }
                    else
                    {
                        /* FIFO to Buffer data read */
                        usb_cstd_Fifo2Buf(ptr, i, useport);
                    }
                }
            }
        }
    }
}/* eof usb_cstd_brdy_pipe() */

/******************************************************************************
End of file
******************************************************************************/
