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
* File Name    : r_usb_pmsc_apl.h
* Description  : USB Phelipheral MSC Code
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 04.01.2014 1.00 First Release
*         : 30.01.2015 1.01    Added RX71M.
*         : 30.09.2015 1.11    Added RX63N.
***********************************************************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <r_usb_dataflash_disk.h>

#include <stdlib.h>                 /* PMSC Sample use */
#include <stdio.h>                  /* PMSC Sample use */

/* Access to peripherals and board defines. */
#include "platform.h"
#include "r_usb_basic_if.h"
#include "r_usb_pmsc_if.h"
#include "r_media_driver_if.h"
#include "r_usb_atapi_driver_config.h"
#include "r_usb_pmsc_config.h"

/******************************************************************************
Constant macro definitions
******************************************************************************/


/*****************************************************************************
Function
******************************************************************************/
/* Functions */
void usb_main( void );
void usb_msc_driver(void);
void usb_driver_init( void );
void pmsc_registration(USB_UTR_t *ptr);
void usb_mcu_init(void);
void usb_board_init( void );
void media_driver_init( void );
void msc_configured(USB_UTR_t *ptr, uint16_t data1, uint16_t data2);
void msc_detach(USB_UTR_t *ptr, uint16_t data1, uint16_t data2);
void msc_suspended(USB_UTR_t *ptr, uint16_t data1, uint16_t data2);

#if (USB_CPU_LPW_PP == USB_LPWR_USE_PP)
void msc_low_power_control( void );
#endif  /* (USB_CPU_LPW_PP == USB_LPWR_USE_PP) */

/******************************************************************************
External variables and functions
******************************************************************************/
extern uint16_t         *usb_gpmsc_EpPtr[];
extern uint8_t          usb_gpmsc_DeviceDescriptor[];
extern uint8_t          *usb_gpmsc_ConPtr[];
extern uint8_t          *usb_gpmsc_StrPtr[];
extern uint16_t         usb_gpmsc_EpTbl1[];

extern uint8_t          usb_gpmsc_QualifierDescriptor[];
extern uint8_t          *usb_gpmsc_ConPtrOther[];
extern uint8_t          usb_gpmsc_HsConfigration[];
extern uint8_t          usb_gpmsc_FsConfigration[];

extern void usb_pstd_usbdriver_start( USB_UTR_t *ptr );
extern void R_usb_pmsc_driver_start( USB_UTR_t *ptr );
extern void usb_cpu_LcdDisp(uint8_t position, uint8_t *string);
extern void usb_pmsc_Task(USB_VP_INT_t stacd);

/******************************************************************************
End  Of File
******************************************************************************/
