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
******************************************************************************
* Copyright (C) 2010 Renesas Electronics Corporation. All rights reserved.
******************************************************************************
* File Name    : r_usb_pMSCdefine.h
* Version      : 1.00
* Device(s)    : Renesas SH-Series, RX-Series
* Tool-Chain   : Renesas SuperH RISC engine Standard Toolchain
*              : Renesas RX Standard Toolchain
* OS           : Common to None and uITRON 4.0 Spec
* H/W Platform : Independent
* Description  : USB Host and Peripheral Interrupt code
******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 04.02.2010 1.00    First Release
*         : 02.08.2010 0.91    Updated comments
*         : 29.10.2010 1.00    Mass Production Release
******************************************************************************/
#ifndef __R_USB_PMSC_DEFINE_H__
#define __R_USB_PMSC_DEFINE_H__

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/


/*****************************************************************************
Macro definitions
******************************************************************************/

/* Pipe stall clear result */
#define     USB_PMSC_PIPE_STALL_CLEAR       0xFC00
/* Subclass Max. Range */
#define     USB_PMSC_MAX_SUBCLASS_RANGE     6u

 #define    USB_PMSC_USB_EXECUTE            0x0001
 #define    USB_PMSC_ERR_CSW_NG             0x0002
 #define    USB_PMSC_ERR_SET_STALL          0x0003
 #define    USB_PMSC_ERR_PHASE_ERR          0x0004
 #define    USB_PMSC_REQ_TRANS_END          0x0005


/*****************************************************************************
Enumerated Types
******************************************************************************/

/* USB Mass Storage Device Class Phase. */
enum usb_gpmsc_Phase
{
    USB_PMSC_PCBWRCV        = 0x00, /* Receive Command Block Wrapper */
    USB_PMSC_PDATASND       = 0x01, /* Send Data ( Device to Host ) */
    USB_PMSC_PDATASNDWAIT   = 0xF1,
    USB_PMSC_PDATARCV       = 0x02, /* Receive Data ( Host to Device ) */
    USB_PMSC_PDATARCVWAIT   = 0xF2,
    USB_PMSC_PCSWSND        = 0x03, /* Send Command Status Wrapper */
    USB_PMSC_PERROR0        = 0x04, /* Endpoint Stall */
    USB_PMSC_PERROR1        = 0x05,
    USB_PMSC_PERROR2        = 0x06,
    USB_PMSC_PERROR3        = 0x07, /* Wait Reset Recovery */
    USB_PMSC_PERROR4        = 0x08,
    USB_PMSC_PERROR5        = 0x09,
    USB_PMSC_PERROR6        = 0x0A,
    USB_PMSC_MSROK          = 0x0F,
    USB_PMSC_MSRWAIT        = 0xF0,
    USB_PMSC_PCHECK         = 0xFF, /* Command Check */
};


/* USB_UTR_t.msginfo parameter */
enum usb_gpmsc_MsgDir
{
    USB_PMSC_USB2PMSC       = 0x71, /* USB_PMSC_MBX Message from USB */
    USB_PMSC_PFLASH2PMSC    = 0x72, /* USB_PMSC_MBX Message from PFLASH */
};

/* USB_UTR_t.status parameter in the case of msginfo == PFLASH2PMSC. */
enum usb_gpmsc_Status
{
    USB_PMSC_CMD_COMPLETE   = 0x00, /* Command passed */
    USB_PMSC_CMD_FAILED     = 0x01, /* Command failed */
    USB_PMSC_CMD_CONTINUE   = 0x02, /* (In/Out)Data   */
    USB_PMSC_CMD_ERROR      = 0x03, /* Command Error  */
};


/******************************************************************************
Typedef definitions
******************************************************************************/


#endif  /* __R_USB_PMSC_DEFINE_H__ */
/******************************************************************************
End  Of File
******************************************************************************/
