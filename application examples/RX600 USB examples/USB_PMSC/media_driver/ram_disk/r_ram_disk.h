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
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_ram_disk.h
* Description  : Header file for RAM-disk media driver.
************************************************************************************************************************
* History : DD.MM.YYYY Version      Description
*           23.1.2013  3.00( PMSC)  For PMSC added block media API.
***********************************************************************************************************************/

#ifndef RAM_DISK_H
#define RAM_DISK_H

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Fixed width integers. */
#include <stdint.h>

/* RAM disk configuration */
#include "r_usb_usrconfig.h"
#include "r_media_driver_api.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* Media size */

#ifdef USB_SDRAM_USE_PP
 #define     RAMDISK_MEDIASIZE          (8ul * 1024ul * 1024ul) /*  8 MB. */
 //#error "8 MB"
#else
 #define     RAMDISK_MEDIASIZE          (32ul * 1024ul)         /*  32 kB. */
 //#error "32 kB"
#endif      /* USB_SDRAM_USE_PP */

#define RAMDISK_SECTSIZE               0x0200ul    /* 512 bytes per sector */
#define RAMDISK_TOTALSECT              RAMDISK_MEDIASIZE / RAMDISK_SECTSIZE

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
* Exported global variables and functions (to be accessed by other files)
************************************************************************************************************************/
extern struct media_driver_s g_RamMediaDriver;

extern uint8_t ram_disk_boot_sector[];

#endif /* RAM_DISK_H */
