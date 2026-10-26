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
* File Name    : r_ram_disk.c
* Version      : 2.00
* Description  : Implements top level of API compliant block media driver for 
*                'RAM'.
*******************************************************************************/
/*******************************************************************************
* History    : DD.MM.YYYY Version Description
*            : 04.07.2013 1.00         First Release
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
******************************************************************************/
#include <string.h>
#include "r_ram_disk.h"
#if DEBUG_CONSOLE
    #include "r_usb_usrconfig.h"
    #include "string.h"
    #include <stdio.h>
#endif

#define USB_PMSC_SIGNATURE              0xAA55u
#define RAMDISK_FORMAT_FAT              0x1

/*******************************************************************************
* Private function declarations
*******************************************************************************/
uint32_t ramdisk_get_block_size(void);
uint32_t ramdisk_get_num_blocks(void);
void     ramdisk_disk_format (void);


/***** Driver functions *******************************************************/
media_ret_t ramdisk_disk_open(void);
media_ret_t ramdisk_disk_close(void);
media_ret_t ramdisk_disk_read(uint8_t *read_buffer, uint32_t sector, uint8_t sector_count);
media_ret_t ramdisk_disk_write(uint8_t *write_buffer, uint32_t sector, uint8_t sector_count);
media_ret_t ramdisk_disk_ioctl(uint8_t command_code, uint32_t *data_buffer);


/*******************************************************************************
* Private global variables
*******************************************************************************/
/* to store the filesystem driver function pointers */
struct media_driver_s    g_RamdiskMediaDriver = 
{
    &ramdisk_disk_open,
    &ramdisk_disk_close,
    &ramdisk_disk_read,
    &ramdisk_disk_write,
    &ramdisk_disk_ioctl
};

/* RAM storage area */
static uint8_t g_ram_data[RAMDISK_MEDIASIZE];

extern uint8_t              ram_disk_boot_sector[62];
extern const uint8_t        usb_gpmsc_TableFat[8*2];
extern const uint8_t        usb_gpmsc_RootDir[8*4];

/*******************************************************************************
* Function Name: ramdisk_disk_format
* Description  : Write the FAT data into the ramdisk
* Argument     : none
* Return value : none
*******************************************************************************/
void ramdisk_disk_format (void)
{
    /* Erase disk */
    memset(g_ram_data, 0, RAMDISK_MEDIASIZE);

#ifdef RAMDISK_FORMAT_FAT
    /* LBA = 0 (PBR) */
    memcpy(g_ram_data, ram_disk_boot_sector, 62);

    g_ram_data[510] = (uint8_t)((USB_PMSC_SIGNATURE)      & 0x00FF);
    g_ram_data[511] = (uint8_t)((USB_PMSC_SIGNATURE >> 8) & 0x00FF);

    /* LBA = 2 (FAT1) */
    memcpy(g_ram_data + (2 * RAMDISK_SECTSIZE), usb_gpmsc_TableFat, 16);

    /* LBA = 3 (FAT1) */
    memcpy(g_ram_data + (3 * RAMDISK_SECTSIZE), usb_gpmsc_TableFat, 16);

    /* LBA = 4 (FAT1) */
    memcpy(g_ram_data + (4 * RAMDISK_SECTSIZE), usb_gpmsc_RootDir, 32);
#endif
}
/*******************************************************************************
* End of function ramdisk_disk_format
*******************************************************************************/


/*******************************************************************************
* Function Name: ramdisk_get_block_size.
* Description  :  
* Argument     : none
* Return value : Block size in bytes.
*******************************************************************************/
uint32_t ramdisk_get_block_size(void)
{
    return RAMDISK_SECTSIZE;
}
/*******************************************************************************
* End of function ramdisk_get_block_size
*******************************************************************************/


/*******************************************************************************
* Function Name: ramdisk_get_num_blocks
* Description  : gets the number of logical blocks in the ram-disk
* Argument     : none
* Return value : Number of blocks.
*******************************************************************************/
uint32_t ramdisk_get_num_blocks(void)
{
    return RAMDISK_TOTALSECT;
}
/*******************************************************************************
* End of function ramdisk_get_num_blocks
*******************************************************************************/


/*******************************************************************************
* Function Name: ramdisk_disk_read
* Description  :  
* Argument     :     uint8_t  *read_buffer   : destination buffer for read data
*                    uint32_t sector         : the starting sector number
*                    uint8_t sector_count    : number of sectors to be read
*
* Return value :     R_RES_OK  = 0     Successful
*                    R_RES_ERROR = 1  R/W Error
* Note:    This function does not check for destination buffer overrun.
*          Make absolutely sure that enough memory has been allocated to accept 
*          the data that will be read. This is especially important when the 
*          sector count is greater than 1, because typically only 1 sector is 
*          read at a time.
*******************************************************************************/
media_ret_t ramdisk_disk_read(uint8_t *read_buffer, uint32_t sector, uint8_t sector_count)
{
    /* Identify the start byte and number of bytes to read */
    uint32_t start_byte = sector * RAMDISK_SECTSIZE;
    uint32_t byte_count = sector_count * RAMDISK_SECTSIZE;

    /* Check that the read won't run off the end of the disk */
    if ((start_byte + byte_count) < RAMDISK_MEDIASIZE)
    {
        /* Read the data to the buffer */
        memcpy(read_buffer, g_ram_data + start_byte, byte_count);

#if DEBUG_CONSOLE
        printf("Read 0x%X%X%X%X%X%X%X... from sector 0x%X", read_buffer[0],
                read_buffer[1], read_buffer[2], read_buffer[3], read_buffer[4],
                read_buffer[5], read_buffer[6], sector + sector_count);

        printf(" (address 0x%X)\n", (sector + sector_count) * RAMDISK_MEDIASIZE);
#endif

        return MEDIA_RET_OK;
    }
    else
    {
#if DEBUG_CONSOLE
        printf("Invalid read ran over end of disk");
#endif
        return MEDIA_RET_RWERR;
    }
}
/*******************************************************************************
* End of function ram_disk_read
*******************************************************************************/

/*******************************************************************************
* Function Name: ramdisk_disk_write
* Description  :  
* Argument     :  uint8_t* write_buffer   : source for data to be written from
*                 uint32_t sector         : the starting sector number
*                 uint8_t sector_count    : number of sectors to be written
*                     
* Return value :     R_RES_OK  = 0     Successful
*                    R_RES_ERROR = 1  R/W Error
*******************************************************************************/
media_ret_t ramdisk_disk_write(uint8_t *write_buffer, uint32_t sector, uint8_t sector_count)
{
    /* Identify the start byte and number of bytes to write */
    uint32_t start_byte = sector * RAMDISK_SECTSIZE;
    uint32_t byte_count = sector_count * RAMDISK_SECTSIZE;

    /* Check that the write won't run off the end of the disk */
    if ((start_byte + byte_count) < RAMDISK_MEDIASIZE)
    {
        /* Write the data to the disk */
        memcpy(g_ram_data + start_byte, write_buffer, byte_count);

#if DEBUG_CONSOLE
        printf("Wrote 0x%X%X%X%X%X%X%X... to sector 0x%X", write_buffer[0],
                write_buffer[1], write_buffer[2], write_buffer[3], write_buffer[4],
                write_buffer[5], write_buffer[6], sector + sector_count);

        printf(" (address 0x%X)\n", (sector + sector_count) * RAMDISK_MEDIASIZE);
#endif

        return MEDIA_RET_OK;
    }
    else
    {
#if DEBUG_CONSOLE
        printf("Invalid write ran over end of disk");
#endif
        return MEDIA_RET_RWERR;
    }
}
/*******************************************************************************
* End of function ram_disk_write
*******************************************************************************/


/*******************************************************************************
* Function Name: ramdisk_disk_open
* Description  :  
* Argument     :  none
* Return value :     R_RES_OK  = 0     Successful
*
*******************************************************************************/
media_ret_t ramdisk_disk_open(void)
{
    return MEDIA_RET_OK;
}
/*******************************************************************************
* End of function ram_disk_opend
*******************************************************************************/

/*******************************************************************************
* Function Name: ramdisk_disk_close
* Description  :  
* Argument     :  none
* Return value :     R_RES_OK  = 0     Successful
*
*******************************************************************************/
media_ret_t ramdisk_disk_close(void)
{
    return MEDIA_RET_OK;
}
/*******************************************************************************
* End of function ram_disk_close
*******************************************************************************/


/*******************************************************************************
* Function Name: ramdisk_disk_ioctl
* Description  :  
* Argument     : uint8_t command_code   :
*                uint32_t *data_buffer  :
* Return value :     R_RES_OK  = 0     Successful
*
*******************************************************************************/
media_ret_t ramdisk_disk_ioctl(uint8_t command_code, uint32_t *data_buffer)
{
    switch (command_code)
    {
        case MEDIA_IOCTL_GET_NUM_BLOCKS:
            *data_buffer = ramdisk_get_num_blocks();
        break;
        
        case MEDIA_IOCTL_GET_SECTOR_SIZE:
        case MEDIA_IOCTL_GET_BLOCK_SIZE:
             *data_buffer = ramdisk_get_block_size();
        break;
        
        case MEDIA_IOCTL_SYNC:
            /* TODO: Flush write cache if necessary */
        break;
        default:
            return MEDIA_RET_PARERR;
        break;
    }

    return MEDIA_RET_OK;
}
/*******************************************************************************
* End of function ram_disk_ioctl
*******************************************************************************/

/*******************************************************************************
End Of File
*******************************************************************************/
