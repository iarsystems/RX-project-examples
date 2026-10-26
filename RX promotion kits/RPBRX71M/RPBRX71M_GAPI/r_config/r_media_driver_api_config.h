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
* Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name     : r_media_driver_api_config.h
* Description   : This is the configuration file for the block media driver API.
*                 User configurable options, macro definitions, and enumerations
*                 are declared here.
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 04.01.2014 1.00 First Release
*         : 30.09.2015 1.11    Added RX63N.
***********************************************************************************************************************/

#include "r_media_driver_if.h"

#ifndef R_MEDIA_DRIVER_API_CONFIG_H    /* Multiple inclusion prevention */
#define R_MEDIA_DRIVER_API_CONFIG_H

/*
Media logical unit number enumeration
This enumeration is used to define labels for the logical unit numbers that will
be assigned to each media device. The devices defined here are examples.
This is intended to be a user configurable list, however media drivers distributed
as middleware may predefine certain labels that will need to be used here.
The values assigned in this enumeration will determine the location in the drivers
list that the driver reference will be placed when the driver is registered.
Afterward these values will serve as indices to the list and will be used to
dereference calls to the media driver functions via the API.

This enumeration must always start at 0 and must be sequential to provide proper
array indexing. Use these device labels as the argument for the lun parameter when
making calls to the API.
/* Examples. User adds own devices here */
typedef enum
{
    SPI_FLASH_LUN = 0,  /* device 0: Only RSK63N currently. */
    SDCARD_LUN,         /* device 1: Not implemented for PMSC. */
    RAM_DISK_LUN,       /* device 2: Default PMSC demo, all boards. */
    DATAFLASH_DISK_LUN, /* device 3: Data Flash Disk */
    /* Do not add after this line. */
    MAX_NUM_LUNS        /* Do not change this line. */
} media_lun_t;

#define DUMMY_SECTSIZE               0x0200ul    /* 512 bytes per sector */

#endif

