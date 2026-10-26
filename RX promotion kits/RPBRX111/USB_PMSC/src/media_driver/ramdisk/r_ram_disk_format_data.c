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
* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.    
*******************************************************************************/
/*******************************************************************************
* File Name     : r_ram_disk_format_data.c
* Version       : 1.0
* Device        : Renesas Generic MCU.
* Tool Chain    : HEW
* H/W Platform  : RSK 
* Description   : Contains initialized memory that provides a FAT file system 
*                 format to the ram-disk.
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY Version      Description
*           23.1.2013  3.00( PMSC)  For PMSC added block media API.
*******************************************************************************/

/******************************************************************************
Section definitions
******************************************************************************/
#ifdef R8CUSB
#pragma section program P_smpl
#pragma section bss B_smpl
#pragma section data R_smpl
#endif

#ifdef RL78USB
#endif

#ifdef RL78USB_SECTION
#pragma section @@CODE   P_SMP
#pragma section @@BASE   T_SMP
#pragma section @@DATA   B_SMP
#endif

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include "r_usb_ctypedef.h"             /* Type define */
#include "r_usb_ckernelid.h"            /* Kernel ID definition */
#include "r_usb_api.h"                  /* USB API public function header */

#include "r_usb_usrconfig.h"
#include "r_usb_api.h"
#include "r_usb_cmsc_define.h"      /* USB  Mass Storage Class Header */
#include "r_ram_disk.h"


/******************************************************************************
Constant macro definitions
******************************************************************************/
#define RAMDISK_MEDIATYPE              0xF8u   /* Fixed media */
//#define   RAMDISK_MEDIATYPE              0xF0u   /* Removable media */

#define RAMDISK_SIGNATURE              0xAA55u

#define RAMDISK_CLSTSIZE               0x01u
#define RAMDISK_FATNUM                 0x02u

/*
    If the number of data areas of clusters is smaller
    than that of value 4085(4096-11), it is FAT12.
    If the number of data areas of clusters is smaller
    than that of value 65525(65536-11), it is FAT16.
    Otherwise it is FAT32.
*/

#if RAMDISK_TOTALSECT < 4096
        #define RAMDISK_FATLENGTH      341ul   /* FAT12 */
#else
    #if RAMDISK_TOTALSECT < 65536
        #define RAMDISK_FATLENGTH      256ul   /* FAT16 */
    #else
        #define RAMDISK_FATLENGTH      128ul   /* FAT32 */
    #endif
#endif

#define RAMDISK_FATSIZE   (((RAMDISK_TOTALSECT - 8) / RAMDISK_FATLENGTH) + 1)

#define RAMDISK_ROOTTOP   (((RAMDISK_FATSIZE  * RAMDISK_FATNUM + 1) / 8 + 1 ) * 8)
#define USB_MEDIA_FATTOP  (RAMDISK_ROOTTOP - (RAMDISK_FATSIZE * RAMDISK_FATNUM))


/******************************************************************************
Public global variables
******************************************************************************/
uint8_t ram_disk_boot_sector[62] =
{
/* Master boor recorder (FAT16) */
    /* 0x00:JMP code (PBR) */
    0xEB,0x3C,0x90,
    /* 0x03:OEM name */
    'M','S','D','O','S','5','.','0',
    /* 0x0B:Sector size */
    ((uint8_t)(RAMDISK_SECTSIZE)      & 0xfful),
    ((uint8_t)(RAMDISK_SECTSIZE >> 8) & 0xfful),
    /* 0x0D:Cluster size */
    RAMDISK_CLSTSIZE,
    /* 0x0E:reserved (FAT entry) */
    0x02,0x00,
    /* 0x10:FAT number */
    RAMDISK_FATNUM,
    /* 0x11:ROOT entry address */
    0x00, 0x02,
    /* 0x13:Total sector(16bit) */
    0x2C, 0x00,
    /* 0x15:Media type */
    RAMDISK_MEDIATYPE,
    /* 0x16:FAT size */
    ((uint8_t)(RAMDISK_FATSIZE)      & 0xfful),
    ((uint8_t)(RAMDISK_FATSIZE >> 8) & 0xfful),
    /* 0x18:Track sector */
    0x00,0x00,
    /* 0x1A:Head number */
    0x00,0x00,
    /* 0x1C:Offset sector */
    0x00,0x00,0x00,0x00,
    /* 0x20:Total sector(32bit) */
    0x00,0x00,0x00,0x00,
    /* 0x24:Drive number */
    0x00,
    /* 0x25:reserved */
    0x00,
    /* 0x26:Boot signature */
    0x00,
    /* 0x27:Volume serial number */
    0x00,0x00,0x00,0x00,
    /* 0x2B:Volume label */
    'N','O',' ','N','A','M','E',' ',' ',' ',' ',
    /* 0x36:File system type */
#if  RAMDISK_FATLENGTH == 341
    'F','A','T','1','2',' ',' ',' ',
#endif
#if  RAMDISK_FATLENGTH == 256
    'F','A','T','1','6',' ',' ',' ',
#endif
};


const uint8_t usb_gpmsc_TableFat[8*2] =
{
#if  RAMDISK_FATLENGTH == 341
    RAMDISK_MEDIATYPE,0xFF,0xFF,0x00, 0x00,0x00,0x00,0x00,  /* FAT-ID   */
#endif
#if  RAMDISK_FATLENGTH == 256
    RAMDISK_MEDIATYPE,0xFF,0xFF,0xFF, 0x00,0x00,0x00,0x00,  /* FAT-ID   */
#endif
    0x00,0x00,0x00,0x00,             0x00,0x00,0x00,0x00,

};


const uint8_t usb_gpmsc_RootDir[8*4] =
{
    'S','A','M','P','L','E',' ',' ',
    0x20,0x20,0x20,0x08,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

/*******************************************************************************
End Of File
*******************************************************************************/
