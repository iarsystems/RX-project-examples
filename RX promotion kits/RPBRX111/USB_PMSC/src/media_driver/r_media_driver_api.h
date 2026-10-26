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
* File Name    : r_media_driver_api.h
* Version      : 2.00
* Description  : This is the header file for r_media_driver_api.c 
*                Public functions prototypes, type definitions, and other 
*                public data structure for the block media driver API are 
*                declared here.
*******************************************************************************/
/*******************************************************************************
* History    : DD.MM.YYYY Version Description
*            : 09.11.2012 1.00         First Release
*            : 30.11.2012 2.00
*******************************************************************************/

#ifndef __R_MEDIA_DRIVER_API_H__    /* Multiple inclusion prevention */
#define __R_MEDIA_DRIVER_API_H__

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include "r_usb_ctypedef.h"                /* Type define */
#include "r_usb_ckernelid.h"            /* Kernel ID definition */
//#include "r_usb_usrconfig.h"
#include "r_usb_cdefusbip.h"            /* USB-FW Library Header */
#include "r_usb_usbip.h"                /* Definition of the USB register & bit define */
#include "r_usb_api.h"                    /* USB API public function header */

#include "r_media_driver_api_config.h"

/*******************************************************************************
Macro definitions
*******************************************************************************/
#define STA_NOINIT        0x01    /* Drive not initialized */
#define STA_NODISK        0x02    /* No medium in the drive */
#define STA_PROTECT       0x04    /* Write protected */

/*******************************************************************************
Typedef definitions
*******************************************************************************/
typedef unsigned char    BOOL_t;

#define    TRUE     1
#define    FALSE    0



/* Return values for functions */
typedef enum
{ 
    MEDIA_RET_OK = 0,       /* 0: Successful */
    MEDIA_RET_RWERR,        /* 1: Read/Write Error */
    MEDIA_RET_WRPRT,        /* 2: Write Protected */
    MEDIA_RET_NOTRDY,       /* 3: Not Ready */
    MEDIA_RET_PARERR,       /* 4: Invalid Parameter */
    MEDIA_RET_OP_FAIL,      /*  Operation failed. */
    MEDIA_RET_DEV_OPEN,     /*  The device is already open. */
    MEDIA_RET_DEV_NO_INIT,  /*  Device not initialized. */
    /* For expansion, add only after this line. */
} media_ret_t;

/* 
Definition of media driver function pointer types
The media driver API interfaces to the specific media device driver functions
through an abstraction layer. Therefore each of the API functions has a
corresponding type-defined  type that consists of a pointer to a function with a
matching parameter list.
*/
typedef media_ret_t(*media_open_t)   (void);
typedef media_ret_t(*media_close_t)  (void);
typedef media_ret_t(*media_read_t)   (uint8_t *, uint32_t, uint8_t);
typedef media_ret_t(*media_write_t)  (uint8_t *, uint32_t, uint8_t);
typedef media_ret_t(*media_ioctl_t)  (uint8_t, uint32_t *);

/* Media driver Data Structure */
typedef struct media_driver_s
{
    media_open_t     pf_media_open;  /* Pointer to driver open function. */  
    media_close_t    pf_media_close; /* Pointer to driver close function. */
    media_read_t     pf_media_read;  /* Pointer to driver read function. */
    media_write_t    pf_media_write; /* Pointer to driver write function. */
    media_ioctl_t    pf_media_ctrl;  /* Pointer to driver control function. */
} media_driver_t;

typedef enum
{
    /* Generic ioctl command (defined for compatibility with FatFs) */
    MEDIA_IOCTL_SYNC              = 0,  /* Flush media write cache. */
    MEDIA_IOCTL_GET_NUM_BLOCKS    = 1,  /* For use un calculating media size. */
    MEDIA_IOCTL_GET_SECTOR_SIZE   = 2,  /* Get sector size (for multiple sector size (_MAX_SS >= 1024)) */
    MEDIA_IOCTL_GET_BLOCK_SIZE    = 3,  /* Get number of byes per block. */
    MEDIA_IOCTL_CTRL_ERASE_SECTOR = 4,  /* Force erased a sector group (for only _USE_ERASE) */
    /* Add more IOCTL commands starting below here. */
} ioctl_cmd_t;

/*******************************************************************************
External public global variables
*******************************************************************************/
extern media_driver_t  *g_MediaDriverList;

/*******************************************************************************
Public Function Prototypes
*******************************************************************************/
/* The R_MEDIA_Initialize() function initializes data structures and variables 
   that are used by the target media device to support its operation for the 
   first time. 
   */
BOOL_t          R_MEDIA_Initialize(struct media_driver_s *p_media_driver);


/* The R_MEDIA_Open() function initializes the hardware registers for 
   peripherals used by the media driver and leaves the media device ready for 
   communications.
   */
media_ret_t     R_MEDIA_Open(void);

/* The R_MEDIA_Open() function initializes the hardware registers for peripherals
   used by the media driver and leaves the media device ready for communications.
   */
media_ret_t     R_MEDIA_Close(void);

/* The R_MEDIA_Read() function reads one or more blocks of data from the selected 
   media device and places it into a buffer provided by the caller.
   */
media_ret_t     R_MEDIA_Read(uint8_t* p_rbuffer,       /* Pointer to read data buffer */
                             uint32_t start_block,     /* Start block number          */
                             uint8_t  block_count);    /* Number of blocks to read.   */

/* The R_MEDIA_Write() function writes one or more blocks of data to the selected media 
   device from a source buffer provided by the caller.
   */
media_ret_t     R_MEDIA_Write(uint8_t *p_wbuffer,      /* Pointer to write data buffer */
                              uint32_t start_block,    /* Start block number           */
                              uint8_t  block_count);   /* Number of blocks to write.   */

/* The R_MEDIA_Ioctl() function provides a generalized means to pass special command 
   and control instructions to the media driver, and for the driver to return information.
   */
media_ret_t     R_MEDIA_Ioctl(ioctl_cmd_t ioctl_cmd,   /* Control command code     */
                              void *ioctl_data);       /* Data transfer buffer     */


#endif /* __R_MEDIA_DRIVER_API_H__ */

/*****************************************************************************
End Of File
******************************************************************************/
