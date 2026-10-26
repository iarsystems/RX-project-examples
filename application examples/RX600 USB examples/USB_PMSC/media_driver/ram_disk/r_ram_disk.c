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
* File Name     : r_ram_disk.c
* Version       : 1.0
* Device        : Renesas Generic MCU.
* Tool Chain    : HEW
* H/W Platform  : RDK 
* Description   : Implements top level of API compliant block media driver for 
*                 'RAM-disk'. 
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY Version      Description
*           23.1.2013  3.00( PMSC)  For PMSC added block media API.
*******************************************************************************/

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <string.h>
#include "r_ram_disk.h"

/******************************************************************************
Section    <Section Difinition> , "Project Sections"
******************************************************************************/
#pragma section="_pmsc"


/*******************************************************************************
* Private function declarations
*******************************************************************************/
static uint32_t ram_get_block_size(void);
static uint32_t ram_get_num_blocks();

/***** Driver functions *******************************************************/
static media_ret_t ram_disk_open(uint8_t);        /* Drive number (LUN) */
static media_ret_t ram_disk_close(uint8_t);        /* Drive number (LUN) */                                     
static media_ret_t ram_disk_read(uint8_t, uint8_t*, uint32_t, uint8_t);  /* LUN, read buffer, sector, sector count */                    
static media_ret_t ram_disk_write(uint8_t, uint8_t*, uint32_t, uint8_t); /* LUN, write buffer, sector, sector count */             
static media_ret_t ram_disk_ioctl(uint8_t, uint8_t, uint32_t*);          /* LUN, command code, data buffer */

/*******************************************************************************
* Private global variables
*******************************************************************************/
/* to store the filesystem driver function pointers */
struct media_driver_s g_RamMediaDriver = 
{
    RAM_DISK_LUN,        
    &ram_disk_open,   
    &ram_disk_close,  
    &ram_disk_read,
    &ram_disk_write,
    &ram_disk_ioctl
};



#ifdef USE_FAT_FS
static FATFS g_ram_fatfs; /* local instance of FAT file system structure */
#endif


/*******************************************************************************
* Function Name: ram_get_block_size. 
* Description  :  
* Argument     : none
* Return value : Block size in bytes.
*******************************************************************************/
static uint32_t ram_get_block_size(void)
{      
    return RAMDISK_SECTSIZE; 
} /* end ram_get_block_size */

/*******************************************************************************
* Function Name: ram_get_num_blocks
* Description  : gets the number of logical blocks in the ram-disk
* Argument     : none
* Return value : Number of blocks.
*******************************************************************************/
static uint32_t ram_get_num_blocks()
{
    return RAMDISK_TOTALSECT;
} /* end ram_get_num_blocks */


/*******************************************************************************
* Function Name: ram_disk_read
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
static media_ret_t ram_disk_read(uint8_t lun, 
                                  uint8_t* read_buffer, 
                                  uint32_t sector, 
                                  uint8_t sector_count)
{
    uint8_t count;
    
    for (count = 0; count < sector_count; count++)
    {
        /* Copy 1 block from specified ram disk block address to read_buffer. */        
        memcpy(read_buffer, 
               ram_disk_boot_sector + ((sector + count) * RAMDISK_SECTSIZE), 
               RAMDISK_SECTSIZE);
               
        if (sector_count > 1) /* More than 1 block is to be read. */
        {
            /* Advance read_buffer write position pointer. */
            read_buffer += RAMDISK_SECTSIZE; 
        }
    }

    return MEDIA_RET_OK;
} /* end ram_disk_read */


/*******************************************************************************
* Function Name: ram_disk_write
* Description  :  
* Argument     :    uint8_t lun,  the logical unit (device number) to write to
*                    const uint8_t* write_buffer,  source for data to be written from 
*                    uint32_t sector,         the starting sector number
*                    uint8_t sector_count     number of sectors to be written
*                     
* Return value :     R_RES_OK  = 0     Successful
*                    R_RES_ERROR = 1  R/W Error
*******************************************************************************/
static media_ret_t ram_disk_write(uint8_t lun, 
                                   uint8_t* write_buffer, 
                                   uint32_t sector, 
                                   uint8_t sector_count)
{
    uint8_t count;
    
    for ( count = 0; count < sector_count; count++)
    {
        /* Copy 1 block from write_buffer to appropriate block address in ram disk. */
        memcpy(ram_disk_boot_sector + ((sector + count) * RAMDISK_SECTSIZE), 
               write_buffer, 
               RAMDISK_SECTSIZE);
               
        if (sector_count > 1) /* More than 1 block is to be written. */
        {
            /* Advance write_buffer read position pointer. */
            write_buffer += RAMDISK_SECTSIZE; 
        }    
    }

    return MEDIA_RET_OK;
} /* end ram_disk_write */


/*******************************************************************************
* Function Name: ram_disk_open
* Description  :  
* Argument     : (uint8_t lun)
* Return value :     R_RES_OK  = 0     Successful
*
*******************************************************************************/ 
media_ret_t ram_disk_open(uint8_t lun)
{
    // TODO: actually open the device if possible    
    return     MEDIA_RET_OK;
} /* end ram_disk_initialize */

/*******************************************************************************
* Function Name: ram_disk_close
* Description  :  
* Argument     : (uint8_t lun)
* Return value :     R_RES_OK  = 0     Successful
*
*******************************************************************************/ 
media_ret_t ram_disk_close(uint8_t lun)
{
    // TODO: actually close the device if possible
    return MEDIA_RET_OK;
} /* end ram_disk_initialize */

/*******************************************************************************
* Function Name: ram_disk_ioctl
* Description  :  
* Argument     : (uint8_t lun, uint8_t command_code, uint32_t* data_buffer)
* Return value :     R_RES_OK  = 0     Successful
*
*******************************************************************************/      
static media_ret_t ram_disk_ioctl(uint8_t lun, 
                                   uint8_t command_code, 
                                   uint32_t* data_buffer)
{
    switch (command_code) {
        
        case MEDIA_IOCTL_GET_NUM_BLOCKS:
            *data_buffer = (uint32_t)ram_get_num_blocks();
        break;
        
        case MEDIA_IOCTL_GET_SECTOR_SIZE:
        case MEDIA_IOCTL_GET_BLOCK_SIZE:   
             *data_buffer = (uint32_t)ram_get_block_size();     
        break;
        
        case MEDIA_IOCTL_SYNC:
            /* TODO: Flush write cache if necessary */
        break;
        default:
            return MEDIA_RET_PARERR;
        break;
    }    
    return MEDIA_RET_OK;
} /* end ram_disk_ioctl */

