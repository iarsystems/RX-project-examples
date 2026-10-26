/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2014(2015) Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_usb_pmsc_define.h
* Description  : USB Host and Peripheral Interrupt code
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 04.01.2014 1.00 First Release
*         : 30.01.2015 1.01    Added RX71M.
*         : 30.09.2015 1.11    Added RX63N.
***********************************************************************************************************************/

#ifndef __R_USB_PMSC_DEFINE_H__
#define __R_USB_PMSC_DEFINE_H__

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_usb_ctypedef.h"         /* Type define */


/*****************************************************************************
Macro definitions
******************************************************************************/
/* CBW Structure define */
#define     USB_MSC_CBWLENGTH           31
#define     USB_MSC_CBWCB_LENGTH        12
#define     USB_MSC_CSW_LENGTH          13

/* Condition compilation by the difference of the endian */
#if USB_CPUBYTE_PP == USB_BYTE_BIG_PP
 #define    USB_MSC_CBW_SIGNATURE       (uint32_t)0x55534243
 #define    USB_MSC_CSW_SIGNATURE       (uint32_t)0x55534253
#else   /* USB_CPUBYTE_PP == USB_BYTE_BIG_PP */
 #define    USB_MSC_CBW_SIGNATURE       (uint32_t)0x43425355
 #define    USB_MSC_CSW_SIGNATURE       (uint32_t)0x53425355
#endif  /* USB_CPUBYTE_PP == USB_BYTE_BIG_PP */



/* Pipe stall clear result */
#define     USB_PMSC_PIPE_STALL_CLEAR       0xFC00
/* Subclass Max. Range */
#define     USB_PMSC_MAX_SUBCLASS_RANGE     6u

#define    USB_PMSC_USB_EXECUTE             0x0001
#define    USB_PMSC_ERR_CSW_NG              0x0002
#define    USB_PMSC_ERR_SET_STALL           0x0003
#define    USB_PMSC_ERR_PHASE_ERR           0x0004
#define    USB_PMSC_REQ_TRANS_END           0x0005


/*****************************************************************************
Enumerated Types
******************************************************************************/

/*************** enumeration define ***********************/
/* USB Mass Storage Device Class Request code define. */
enum usb_gpmsc_Request
{
    USB_MASS_STORAGE_RESET  = 0xFF00,   /* Mass dtorage reset request */
    USB_GET_MAX_LUN         = 0xFE00,   /* Get max logical unit number */
};

/* GET_MAX_LUN request check code. */
enum usb_gpmsc_LunReqChk
{
    USB_MSC_LUN_LENGTH      = 0x01,     /* GET_MAX_LUN request wLength */
};

/* USB Mass Storage Devie Class Lapper check. */
enum usb_gpmsc_Case13check
{
    /* Device No Data */
    USB_MSC_DNXX    = 0x10,
    /* Device Send(IN) Data */
    USB_MSC_DIXX    = 0x20,
    /* Device Recieved(OUT) Data */
    USB_MSC_DOXX    = 0x30,
    /* Host No Data */
    USB_MSC_XXHN    = 0x01,
    /* Host Recieved(IN) Data */
    USB_MSC_XXHI    = 0x02,
    /* Host Send(OUT) Data */
    USB_MSC_XXHO    = 0x03,
    /* Device No Data & Host No Data */
    USB_MSC_DNHN    = 0x11,
    /* Device No Data & Host Recieved(IN) Data */
    USB_MSC_DNHI    = 0x12,
    /* Device No Data & Host Send(OUT) Data */
    USB_MSC_DNHO    = 0x13,
    /* Device Send(IN) Data & Host No Data */
    USB_MSC_DIHN    = 0x21,
    /* Device Send(IN) Data & Host Recieved(IN) Data */
    USB_MSC_DIHI    = 0x22,
    /* Device Send(IN) Data & Host Send(OUT) Data */
    USB_MSC_DIHO    = 0x23,
    /* Device Recieved(OUT) Data & Host No Data */
    USB_MSC_DOHN    = 0x31,
    /* Device Recieved(OUT) Data & Host Recieved(IN) Data */
    USB_MSC_DOHI    = 0x32,
    /* Device Recieved(OUT) Data & Host Send(OUT) Data */
    USB_MSC_DOHO    = 0x33,
};

/* USB Mass Storage Devie Class Lapper check. */
enum usb_gpmsc_Case13nun
{
    USB_MSC_CASE00  = 0x00, /* CBW check case00(Not Use) */
    USB_MSC_CASE01  = 0x01, /* CBW check case01 */
    USB_MSC_CASE02  = 0x02, /* CBW check case02 */
    USB_MSC_CASE03  = 0x03, /* CBW check case03 */
    USB_MSC_CASE04  = 0x04, /* CBW check case04 */
    USB_MSC_CASE05  = 0x05, /* CBW check case05 */
    USB_MSC_CASE06  = 0x06, /* CBW check case06 */
    USB_MSC_CASE07  = 0x07, /* CBW check case07 */
    USB_MSC_CASE08  = 0x08, /* CBW check case08 */
    USB_MSC_CASE09  = 0x09, /* CBW check case09 */
    USB_MSC_CASE10  = 0x10, /* CBW check case10 */
    USB_MSC_CASE11  = 0x11, /* CBW check case11 */
    USB_MSC_CASE12  = 0x12, /* CBW check case12 */
    USB_MSC_CASE13  = 0x13, /* CBW check case13 */
};

/* CSW STATUS */
enum usb_gcmsc_CswSts
{
    USB_MSC_CSW_OK          = (uint16_t)0x00,
    USB_MSC_CSW_NG          = (uint16_t)0x01,
    USB_MSC_CSW_PHASE_ERR   = (uint16_t)0x02
};

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
/*
*CONVERTER REPLACED|REMOVED:
*#pragma bit_order left
*/
#pragma bitfields=reversed

/* CBW Structure define.    */
typedef struct
{
    uint32_t    dCBWSignature;
    uint32_t    dCBWTag;
    uint8_t     dCBWDTL_Lo;
    uint8_t     dCBWDTL_ML;
    uint8_t     dCBWDTL_MH;
    uint8_t     dCBWDTL_Hi;
    struct
    {
/* Condition compilation by the difference of the bit order */
        uint8_t CBWdir:1;
        uint8_t reserved7:7;
    } bmCBWFlags;
    struct
    {
/* Condition compilation by the difference of the bit order */
        uint8_t reserved4:4;
        uint8_t bCBWLUN:4;
    } bCBWLUN;
    struct
    {
/* Condition compilation by the difference of the bit order */
        uint8_t reserved3:3;
        uint8_t bCBWCBLength:5;
    } bCBWCBLength;
#if  (((USB_FUNCSEL_USBIP0_PP == USB_PERI_PP) && (USB0_IPTYPE_PP == USB_HS_PP))\
    ||((USB_FUNCSEL_USBIP1_PP == USB_PERI_PP) && (USB1_IPTYPE_PP == USB_HS_PP)))
    uint8_t CBWCB[(16 + 481)];
#else   /* USB0_IPTYPE_PP == USB_HS_PP || USB1_IPTYPE_PP == USB_HS_PP */
    uint8_t CBWCB[(16 + 33)];
#endif  /* USB0_IPTYPE_PP == USB_HS_PP || USB1_IPTYPE_PP == USB_HS_PP */
} USB_MSC_CBW_t;

/* CSW Structure define define. */
typedef struct
{
    uint32_t    dCSWSignature;
    uint32_t    dCSWTag;
    uint8_t     dCSWDataResidue_Lo;
    uint8_t     dCSWDataResidue_ML;
    uint8_t     dCSWDataResidue_MH;
    uint8_t     dCSWDataResidue_Hi;
    uint8_t     bCSWStatus;
    uint8_t     dummy;
} USB_MSC_CSW_t;

/*
*CONVERTER REPLACED|REMOVED:
*#pragma bit_order
*/
#pragma bitfields=default

/* Peripheral MSC Driver Task */
#define USB_PMSC_TSK        USB_TID_3           /* Task ID */
#define USB_PMSC_MBX        USB_PMSC_TSK        /* Mailbox ID */
#define USB_PMSC_MPL        USB_PMSC_TSK        /* Memorypool ID */

/* Peripheral MSC Sample Task */
#define USB_PFLSH_TSK       USB_TID_4           /* Task ID */
#define USB_PFLSH_MBX       USB_PFLSH_TSK       /* Mailbox ID */
#define USB_PFLSH_MPL       USB_PFLSH_TSK       /* Memorypool ID */

#endif  /* __R_USB_PMSC_DEFINE_H__ */
/******************************************************************************
End  Of File
******************************************************************************/
