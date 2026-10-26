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
* Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_usb_dataflash_disk.h
* Description  : Header file for Data Flash-disk media driver.
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 02.03.2016 1.00 First Release
***********************************************************************************************************************/

#ifndef FLASH_DISK_H
#define FLASH_DISK_H

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Fixed width integers. */
#include <stdint.h>

/* Flash disk configuration */
#include "r_usb_basic_config.h"


/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* Media size */
#define FLASHDISK_MEDIASIZE             (64ul * 1024ul)     /*  64k - full size of data flash */
#define FLASHDISK_SECTSIZE              (512ul)             /* 512 bytes per sector - 8 x Data Flash unit of programming */
#define FLASHDISK_TOTALSECT             (FLASHDISK_MEDIASIZE / FLASHDISK_SECTSIZE)
#define FLASHDISK_DATA_FLASH_BLOCK		(64)	           /* Number of bytes per data flash block */
/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
* Exported global variables and functions (to be accessed by other files)
************************************************************************************************************************/
extern struct media_driver_s g_dataflash_media_driver;
extern uint8_t flash_disk_boot_sector[];

/******************************************************************************
Function Name   : usb_pmsc_flashdisk_section_init
Description     : Initialize section for USB F/W
Arguments       : none
Return value    : none
******************************************************************************/
void usb_pmsc_flashdisk_section_init(void);

#endif /* FLASH_DISK_H */
