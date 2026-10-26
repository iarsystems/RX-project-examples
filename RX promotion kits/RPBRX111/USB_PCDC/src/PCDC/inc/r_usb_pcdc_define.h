/******************************************************************************
* File Name     : r_usb_pcdc_define.h
* Version       : 2.00
* Description   : USB Peripheral PCDC Header
******************************************************************************
* History   : DD.MM.YYYY Version Description
*           : 30.11.2012 2.00   First Release
******************************************************************************/
#ifndef __R_USB_PCDC_DEFINE_H__
#define __R_USB_PCDC_DEFINE_H__


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_usb_ctypedef.h"             /* Type define */
#include "r_usb_ckernelid.h"            /* Kernel ID definition */
//#include "r_usb_cdefusbip.h"          /* USB-FW Library Header */
#include "r_usb_usrconfig.h"            /* System definition, USB-FW Library Header */
#include "r_usb_api.h"                  /* USB API public function header */
#include "hw_resource.h"


/*****************************************************************************
Macro definitions
******************************************************************************/
/* CDC Class Requests IDs*/
#define USB_PCDC_SET_LINE_CODING            0x20
#define USB_PCDC_GET_LINE_CODING            0x21
#define USB_PCDC_SET_CONTROL_LINE_STATE     0x22
#define USB_PCDC_SEND_BREAK                 0x23

/* GET_LINE_CODING request wLength */
#define USB_PCDC_GET_LINE_CODING_LENGTH     0x07


/*****************************************************************************
Enumerated Types
******************************************************************************/
/* USB Communications Devices Device Class Phase. */
enum usb_gpcdc_Phase
{
    USB_PCDC_PCONTROL   = 0x10,     /* Class Request */
    USB_PCDC_PTRANSFER  = 0x11      /* Data Transfer */
};


/******************************************************************************
Typedef definitions
******************************************************************************/
typedef struct
{
    uint32_t    dwDTERate;      /* Data terminal rate, in bits per second */
    uint8_t     bCharFormat;    /* Stop bits */
    uint8_t     bParityType;    /* Parity */
    uint8_t     bDataBits;      /* Data bits */
    uint8_t     rsv;            /* reserve */
} usb_pcdc_LineCoding_t;


typedef struct
{
    uint16_t    bDTR:1; /* DTR */
    uint16_t    bRTS:1; /* RTS */
    uint16_t    rsv:14; /* reserve */
} usb_pcdc_ControlLineState_t;


typedef struct
{
    
    uint16_t    bRxCarrier:1;   /* DCD signal */
    uint16_t    bTxCarrier:1;   /* DSR signal */
    uint16_t    bBreak:1;       /* State of break detection mechanism of teh device */
    uint16_t    bRingSignal:1;  /* State of ring signal detection of the device */
    uint16_t    bFraming:1;     /* Framing error */
    uint16_t    bParity:1;      /* Framing error */
    uint16_t    bOverRun:1;     /* Over Run error */
    uint16_t    rsv:9;          /* reserve */
} usb_pcdc_SerialState_t;


typedef struct
{
    uint16_t        *pipetbl;       /* Pipe Define Table address */
    uint8_t         *devicetbl;     /* Device descriptor address */
    uint8_t         *configtbl;     /* Configuration descriptor address */
    uint8_t         **stringtbl;    /* String descriptor address */
    usb_cbinfo_t    statediagram;   /* Device status */
    usb_cb_trn_t    ctrltrans;      /* Control Transfer */
    usb_cb_t        aplcb;          /* Application callback function */
} usb_cdcreg_t;


#endif  /* __R_USB_PCDC_DEFINE_H__ */
/******************************************************************************
End Of File
******************************************************************************/
