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
 * File Name    : r_tfat_dataflash_media.c
 * Version      : 1.00
 * Device(s)    : Renesas
 * Tool-Chain   : RXC
 * OS           : None
 * H/W Platform : RPB
 * Description  : Functions and stubs to allow the r_tfat_rx file system
 *                to use Data Flash as a FAT drive.
 *
 *                Functionality is limited to read only.
  *******************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 08.04.2016 1.00 First Release
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
#include "r_flash_rx_if.h"
#include "r_tfat_lib.h"


/******************************************************************************
 Macro Definitions
 ******************************************************************************/
#define DATAFLASH_BASE (FLASH_DF_BLOCK_0) 	/* Defines the start address of the flash drive */

/******************************************************************************
 Enumerated Types
 ******************************************************************************/

/******************************************************************************
 Typedef definitions
 ******************************************************************************/


/******************************************************************************
 Private global variables and functions
 ******************************************************************************/

/******************************************************************************
 Function Prototypes
 ******************************************************************************/

/******************************************************************************
 Constant Data
 ******************************************************************************/

/******************************************************************************
 Public Functions
 ******************************************************************************/


/*****************************************************************************
 Function Name: R_tfat_disk_initialize
 Description:   This function should consist of the code to initialize the disk drive.
 Parameters:    pdrv - Specifies the initialize drive number.
 Return value:  DSTATUS - always TFAT_RES_OK
 *****************************************************************************/
DSTATUS R_tfat_disk_initialize (uint8_t pdrv)
{
	DSTATUS retval = TFAT_RES_OK;

	/* There is no additional initialisation required */
	return(retval);
}
/*****************************************************************************
 End of function  R_tfat_disk_initialize
 ******************************************************************************/


/*****************************************************************************
 Function Name: R_tfat_disk_read
 Description:   This function consists of the code to read data from the disk drive
 Parameters:    Drive - Specifies the physical drive number
                Buffer - Pointer to the read buffer to store the read data.
                SectorNumber - Specifies the start sector number in logical block
                               address (LBA)
                SectorCount - Specifies number of sectors to read
 Return value:  DRESULT - TFAT_RES_OK or
                          TFAT_RES_PARERR if invalid drive.
 *****************************************************************************/
DRESULT R_tfat_disk_read (uint8_t Drive, uint8_t *Buffer, uint32_t SectorNumber, uint8_t SectorCount)
{
	DRESULT retval = TFAT_RES_OK;
    uint8_t count;

	if (0 != Drive )
	{
		/* Verify drive is 0 - always should be */
		retval = TFAT_RES_PARERR;
	}
	else
	{
	    for (count = 0; count < SectorCount; count++)
	    {
	        /* Copy 1 block from specified flash disk block address to read_buffer. */
	        memcpy(Buffer,
	               (void const *)(DATAFLASH_BASE + ((SectorNumber + count) * FLASHDISK_SECTSIZE)),
		       FLASHDISK_SECTSIZE);

	        if (SectorCount > 1) /* More than 1 block is to be read. */
	        {
	            /* Advance read_buffer write position pointer. */
	        	Buffer += FLASHDISK_SECTSIZE;
	        }
	    }
	}

return(retval);
}
/*****************************************************************************
 End of function  R_tfat_disk_read
 ******************************************************************************/

/*****************************************************************************
 Function Name: R_tfat_disk_write
 Description:   This function consists of the code to write data from the disk drive
                Note: Functionality is limited to Read Only so this is only
                a stub function that returns a write protected error.

 Parameters:    Drive - Specifies the physical drive number
                Buffer - Pointer to the data to be written
                SectorNumber - Specifies the start sector number in logical block
                               address (LBA)
                SectorCount - Specifies number of sectors to write
 Return value:  DRESULT - TFAT_RES_WRPRT - Always return a write protected error
 *****************************************************************************/
DRESULT R_tfat_disk_write (uint8_t Drive, const uint8_t *Buffer, uint32_t SectorNumber, uint8_t SectorCount)
{
    DRESULT retval = TFAT_RES_OK;
    flash_err_t ret;
    flash_block_address_t start_address;
    uint32_t num_blocks;

    if (0 != Drive )
    {
        /* Verify drive is 0 - always should be */
        retval = TFAT_RES_PARERR;
    }
    else
    {
            start_address = (uint32_t)(DATAFLASH_BASE + (SectorNumber * FLASHDISK_SECTSIZE));
            num_blocks = (SectorCount * FLASHDISK_SECTSIZE) / FLASHDISK_DATA_FLASH_BLOCK;

            ret = R_FLASH_Erase(start_address, num_blocks);
            if (FLASH_SUCCESS != ret)
            {
                /* Flash Error */
                retval = TFAT_RES_ERROR;
            }
            else
            {
                ret = R_FLASH_Write( (uint32_t)Buffer, (uint32_t)start_address, (SectorCount * FLASHDISK_SECTSIZE));
                if (FLASH_SUCCESS != ret)
                {
                    /* Flash Error */
                    retval = TFAT_RES_ERROR;
                }
            }
    }

	return(retval);
}
/*****************************************************************************
 End of function  R_tfat_disk_write
 ******************************************************************************/

/*****************************************************************************
 Function Name: R_tfat_disk_ioctl
 Description:   Control device dependent features.
                None required for the Data Flash drive so return a parameter
                error if called.
 Parameters:    Drive - Specifies the physical drive number
                Buffer- Specifies the command code
 Return value:  DRESULT - TFAT_RES_PARERR Always return a parameter error
 *****************************************************************************/
DRESULT R_tfat_disk_ioctl (uint8_t Drive, uint8_t Command, void *Buffer)
{
	DRESULT retval = TFAT_RES_PARERR;

	/* Extract from the r_tfat manual */
	/* Users who do not plan to use R_tfat_f_sync function in their applications can skip the
	 * implementation for this particular driver interface function. */

	/* Return a Parameter error */

	return(retval);
}
/*****************************************************************************
 End of function  R_tfat_disk_ioctl
 ******************************************************************************/


/*****************************************************************************
 Function Name: R_tfat_disk_status
 Description:   This function should consist of the code that checks the disk
                and returns the current disk status.
                Data Flash is always accessible so there is no checking needed
                however functionality is limited to read only so will
                always indicate the drive is write protected.
 Parameters:    Drive - Specifies the physical drive number
 Return value:  DSTATUS TFAT_STA_PROTECT Always indicate the media is write
                protected.
 *****************************************************************************/
DSTATUS R_tfat_disk_status (uint8_t Drive)
{
	DSTATUS retval = 0x00;

	/* As we are using an internal memory for the drive there is no 
	 * checking needed here. Just return a not problem
  	 * response */

		return(retval);
}
/*****************************************************************************
 End of function  R_tfat_disk_status
 ******************************************************************************/


/*****************************************************************************
 Function Name: R_tfat_get_fattime
 Description:   Get current time.
                As no writing is supported this always returns a fixed time
                and date of 10:00:00 1st April 2016
 Parameters:    None
 Return value:  uint32 - encoded: 10:00:00 1st April 2016
 *****************************************************************************/
uint32_t R_tfat_get_fattime (void)
{
 uint32_t time = 0x48815000;	/* 10:00:00 1st April 2016 */

 return(time);
}
/*****************************************************************************
 End of function  R_tfat_get_fattime
 ******************************************************************************/



/******************************************************************************
 Private Functions
 ******************************************************************************/


/******************************************************************************
 End  Of File
 ******************************************************************************/

