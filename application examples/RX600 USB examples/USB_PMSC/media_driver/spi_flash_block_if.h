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
* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.    
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : spi_flash_block_if.h
* Description  : Header file for spi flash block type 'disk media' driver.
************************************************************************************************************************
* History : DD.MM.YYYY  Version     Description
*           23.1.2013   3.00( PMSC) For PMSC, added SPI flash.
***********************************************************************************************************************/

#ifndef SPIFLASH_DISK_H 
#define SPIFLASH_DISK_H

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Fixed width integers. */
#include <stdint.h>

/* SPI-flash disk configuration. */
#include "r_media_driver_api.h"
#include "r_usb_usrconfig.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* Media size */
#define     SPIFLASH_DISK_MEDIASIZE     (2ul * 0x400ul * 0x400ul) /* 2 MB. */
//#define     SPIFLASH_DISK_MEDIASIZE     (1ul * 0x100ul * 0x200ul) /* smaller for testing. */

/* Sector (block) size restriction: 
Windows 7 or later must be used as Win7 supports 4 kB block size.
The spi-flash (U4 on the RSK63N) needs 4 kB as sixe since this is the erase size.
Code would become too complex using 512 sector size. See also
   http://support.microsoft.com/kb/2510009
   http://www.seagate.com/tech-insights/advanced-format-4k-sector-hard-drives-master-ti */
#define     SPIFLASH_DISK_SECTSIZE      (0x1000ul) 
#define     SPIFLASH_DISK_TOTALSECT     (SPIFLASH_DISK_MEDIASIZE / SPIFLASH_DISK_SECTSIZE)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
* Exported global variables and functions (to be accessed by other files)
************************************************************************************************************************/

/*****************************************************************************
*               SPI FLASH STORAGE EXTERNAL DRIVER INTERFACE  
* Driver functions encapsulated as structure members of g_SpiFlashMediaDriver
/******************************************************************************/
extern struct media_driver_s  g_SpiFlashMediaDriver;

#endif /* SPIFLASH_DISK_H */
