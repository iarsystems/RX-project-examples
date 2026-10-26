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
* File Name     : r_usb_atapi_driver_config.h
* Version       : 1.0
* Device(s)     : Any
* Tool-Chain    : C/C++ compiler package for RX family V.1.00 Release 00
* H/W Platform  : Any
* Description   : This is the configuration file for the USB atapi transport 
*                 subtype driver.  
*                 User configurable options, macro definitions, and enumerations
*                 are declared here.
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY  Version Description
*           26.11.2012  1.0     First Release
*           23.1.2013   1.0     For PMSC, added SPI flash.
*******************************************************************************/

#ifndef R_USB_ATAPI_DRIVER_CONFIG_H    /* Multiple inclusion prevention */
#define R_USB_ATAPI_DRIVER_CONFIG_H

#include "r_ram_disk.h"
#include "spi_flash_block_if.h"

/* The ATAPI transport subtype driver requires that a storage media device 
   driver be assigned to it so that protocol level storage commands can be 
   directed to a physical storage medium. The ATAPI module communicates with 
   the assigned media device driver through a generalized API. The media driver
   assignment is made here with the macro ATAPI_MEDIA_DEVICE_DRIVER, and the 
   logical unit number that corresponds to the assigned media driver is defined 
   here with the macro ATAPI_MEDIA_LUN. */

/* UNCOMMENT ONE STORAGE MEDIA */
#define USE_RAM         1
//#define USE_SPI_FLASH   1     /* Only RSK63N currently. */

#ifdef USE_RAM
    //Use if using RAM (default).
    /* If you want to use external SDRAM, remember to uncomment USB_SDRAM_USE_PP in r_usb_usrconfig.h. */
    #define ATAPI_MEDIA_DEVICE_DRIVER   g_RamMediaDriver    // Suppies the media driver member functoins to use.
    #define ATAPI_MEDIA_LUN             RAM_DISK_LUN        // The logical unit number of the media driver
    /* Storage sector size */
    #define USB_ATAPI_BLOCK_UNIT        RAMDISK_SECTSIZE
#else //Use serial flash. Only RSK63N.
    #define ATAPI_MEDIA_DEVICE_DRIVER   g_SpiFlashMediaDriver // Suppies the media driver member functoins to use.
    #define ATAPI_MEDIA_LUN             SPI_FLASH_LUN       // The logical unit number of the media driver
    #define USB_ATAPI_BLOCK_UNIT        SPIFLASH_DISK_SECTSIZE
#endif
#endif //ifndef R_USB_ATAPI_DRIVER_CONFIG_H