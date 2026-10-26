/******************************************************************************
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
 * http://www.renesas.com/disclaimer
 *******************************************************************************
 * Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.    */
/******************************************************************************
 * File Name    : r_usb_dataflash_disk_format.c
 * Version      : 1.00
 * Device(s)    : Renesas
 * Tool-Chain   : RXC
 * OS           : None
 * H/W Platform : RPB
 * Description  : Implements top level of API compliant block media driver for
*                 'FLASH-disk'.
 *
 *******************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 14.04.2016 1.00 First Release
 ******************************************************************************/

/******************************************************************************
 WARNING!  IN ACCORDANCE WITH THE USER LICENCE THIS CODE MUST NOT BE CONVEYED
 OR REDISTRIBUTED IN COMBINATION WITH ANY SOFTWARE LICENSED UNDER TERMS THE
 SAME AS OR SIMILAR TO THE GNU GENERAL PUBLIC LICENCE
 ******************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include <string.h>
#include "r_usb_dataflash_disk.h"
#include "r_media_driver_if.h"
#include "r_usb_atapi_driver_config.h"
#include "r_flash_rx_if.h"

/******************************************************************************
 Macro Definitions
 ******************************************************************************/
#define DATAFLASH_BASE (FLASH_DF_BLOCK_0)   /* Defines the start address of the flash drive */

/******************************************************************************
 Enumerated Types
 ******************************************************************************/

/******************************************************************************
 Typedef definitions
 ******************************************************************************/


/******************************************************************************
 Private global variables and functions
 ******************************************************************************/
static uint32_t flash_get_block_size(void);
static uint32_t flash_get_num_blocks(void);

/***** Driver functions *******************************************************/
static media_ret_t flash_disk_read(uint8_t lun,
                                  uint8_t* read_buffer,
                                  uint32_t sector,
                                  uint8_t sector_count);

static media_ret_t flash_disk_write(uint8_t lun,
                                   uint8_t* write_buffer,
                                   uint32_t sector,
                                   uint8_t sector_count);

static media_ret_t flash_disk_open(uint8_t lun);
static media_ret_t flash_disk_close(uint8_t lun);
static media_ret_t flash_disk_ioctl(uint8_t lun,
                                   uint8_t command_code,
                                   uint32_t* data_buffer);

/* Structure in which file system driver function pointers are exported */
struct media_driver_s g_dataflash_media_driver =
{
    DATAFLASH_DISK_LUN,
    &flash_disk_open,
    &flash_disk_close,
    &flash_disk_read,
    &flash_disk_write,
    &flash_disk_ioctl
};

/******************************************************************************
 Function Prototypes
 ******************************************************************************/

/******************************************************************************
 Constant Data
 ******************************************************************************/

/******************************************************************************
 Public Functions
 ******************************************************************************/
/******************************************************************************
Function Name   : usb_pmsc_flashdisk_section_init
Description     : Initialize section for USB F/W
Arguments       : none
Return value    : none
******************************************************************************/
void usb_pmsc_flashdisk_section_init(void)
{
 /* Do nothing */
}
/*****************************************************************************
 End of function  usb_pmsc_flashdisk_section_init
 ******************************************************************************/


/******************************************************************************
 Private Functions
 ******************************************************************************/

/*******************************************************************************
* Function Name: flash_get_block_size
* Description  :  
* Argument     : none
* Return value : Block size in bytes.
*******************************************************************************/
static uint32_t flash_get_block_size(void)
{      
    return FLASHDISK_SECTSIZE;
}
/*****************************************************************************
 End of function  flash_get_block_size
 ******************************************************************************/

/*******************************************************************************
* Function Name: flash_get_num_blocks
* Description  : gets the number of logical blocks in the flash-disk
* Argument     : none
* Return value : Number of blocks.
*******************************************************************************/
static uint32_t flash_get_num_blocks(void)
{
    return FLASHDISK_TOTALSECT;
}
/*****************************************************************************
 End of function  flash_get_num_blocks
 ******************************************************************************/

/*******************************************************************************
* Function Name: flash_disk_read
* Description  :  
* Argument     :     (uint8_t lun, the logical unit (device number) to read from
*                    uint8_t* read_buffer,     destination buffer for read data
*                    uint32_t sector,         the starting sector number
*                    uint8_t sector_count)    number of sectors to be read
*
* Return value :     R_RES_OK  = 0     Successful
*                    R_RES_ERROR = 1  R/W Error
* Note:    This function does not check for destination buffer overrun.
*          Make absolutely sure that enough memory has been allocated to accept 
*          the data that will be read. This is especially important when the 
*          sector count is greater than 1, because typically only 1 sector is 
*          read at a time.
*******************************************************************************/
static media_ret_t flash_disk_read(uint8_t lun,
                                  uint8_t* read_buffer, 
                                  uint32_t sector, 
                                  uint8_t sector_count)
{
    (void)lun;  /* Acknowledge standard API parameter not used */
    uint8_t count;
    
    for (count = 0; count < sector_count; count++)
    {
        /* Copy 1 block from specified flash disk block address to read_buffer. */
        memcpy((void*)read_buffer,
               (const void*)(DATAFLASH_BASE + ((sector + count) * FLASHDISK_SECTSIZE)),
		FLASHDISK_SECTSIZE);
               
        if (sector_count > 1) /* More than 1 block is to be read. */
        {
            /* Advance read_buffer write position pointer. */
            read_buffer += FLASHDISK_SECTSIZE;
        }
    }

    return MEDIA_RET_OK;
}
/*****************************************************************************
 End of function  flash_disk_read
 ******************************************************************************/


/*******************************************************************************
* Function Name: flash_disk_write
* Description  :  
* Argument     :    uint8_t lun,  the logical unit (device number) to write to
*                    const uint8_t* write_buffer,  source for data to be written from 
*                    uint32_t sector,         the starting sector number
*                    uint8_t sector_count     number of sectors to be written
*                     
* Return value :     R_RES_OK  = 0     Successful
*                    R_RES_ERROR = 1  R/W Error
*******************************************************************************/
static media_ret_t flash_disk_write(uint8_t lun,
                                   uint8_t* write_buffer, 
                                   uint32_t sector, 
                                   uint8_t sector_count)
{
    (void)lun;  /* Acknowledge standard API parameter not used */
    media_ret_t retval = MEDIA_RET_OK;
	flash_err_t ret;
	flash_block_address_t start_address;
	uint32_t num_blocks;

	start_address = (uint32_t)(DATAFLASH_BASE + (sector * FLASHDISK_SECTSIZE));
	num_blocks = (sector_count * FLASHDISK_SECTSIZE) / FLASHDISK_DATA_FLASH_BLOCK;

    ret = R_FLASH_Erase(start_address, num_blocks);

    if (FLASH_SUCCESS != ret)
    {
        /* Flash Error */
    	retval = MEDIA_RET_RWERR;
    }
    else
    {
        ret = R_FLASH_Write( (uint32_t)write_buffer, (uint32_t)start_address, (uint32_t)(sector_count * FLASHDISK_SECTSIZE));
        if (FLASH_SUCCESS != ret)
        {
            /* Flash Error */
            retval = MEDIA_RET_RWERR;
        }
    }

    return retval;
}
/*****************************************************************************
 End of function  flash_disk_write
 ******************************************************************************/


/*******************************************************************************
* Function Name: flash_disk_open
* Description  :  
* Argument     : (uint8_t lun)
* Return value :     R_RES_OK  = 0     Successful
*
*******************************************************************************/ 
static media_ret_t flash_disk_open(uint8_t lun)
{
    (void)lun;  /* Acknowledge standard API parameter not used */
    flash_err_t ret;

	ret = R_FLASH_Open();

	if(FLASH_SUCCESS != ret)
	{
		while(1)
		{
			/* Flash open failed */
		}
	}

    return     MEDIA_RET_OK;
}
/*****************************************************************************
 End of function  flash_disk_open
 ******************************************************************************/


/*******************************************************************************
* Function Name: flash_disk_close
* Description  :  
* Argument     : (uint8_t lun)
* Return value :     R_RES_OK  = 0     Successful
*
*******************************************************************************/ 
static media_ret_t flash_disk_close(uint8_t lun)
{
    (void)lun;  /* Acknowledge standard API parameter not used */

    /* Nothing required here */
    return MEDIA_RET_OK;
}
/*****************************************************************************
 End of function  flash_disk_close
 ******************************************************************************/

/*******************************************************************************
* Function Name: flash_disk_ioctl
* Description  :  
* Argument     : (uint8_t lun, uint8_t command_code, uint32_t* data_buffer)
* Return value :     R_RES_OK  = 0     Successful
*
*******************************************************************************/      
static media_ret_t flash_disk_ioctl(uint8_t lun,
                                   uint8_t command_code, 
                                   uint32_t* data_buffer)
{
    (void)lun;  /* Acknowledge standard API parameter not used */

    switch (command_code) {
        
        case MEDIA_IOCTL_GET_NUM_BLOCKS:
            *data_buffer = (uint32_t)flash_get_num_blocks();
        break;
        
        case MEDIA_IOCTL_GET_SECTOR_SIZE:
        case MEDIA_IOCTL_GET_BLOCK_SIZE:   
             *data_buffer = (uint32_t)flash_get_block_size();
        break;
        
        case MEDIA_IOCTL_SYNC:
        break;

        default:
            return MEDIA_RET_PARERR;

    }    
    return MEDIA_RET_OK;
}
/*****************************************************************************
 End of function  flash_disk_ioctl
 ******************************************************************************/


/******************************************************************************
 End  Of File
 ******************************************************************************/



