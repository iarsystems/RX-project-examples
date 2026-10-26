/******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Electronics Corp. and is only 
* intended for use with Renesas products. No other uses are authorized.

* This software is owned by Renesas Electronics Corp. and is protected under 
* all applicable laws, including copyright laws.

* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES 
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, 
* INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
* PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY 
* DISCLAIMED.

* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORP. NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES 
* FOR ANY REASON RELATED TO THE THIS SOFTWARE, EVEN IF RENESAS OR ITS 
* AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

* Renesas reserves the right, without notice, to make changes to this 
* software and to discontinue the availability of this software.  
* By using this software, you agree to the additional terms and 
* conditions found by accessing the following link:
* http://www.renesas.com/disclaimer
******************************************************************************/
/***********************************************************************/
/*                                                                     */
/*  FILE        :Resources.c                                           */
/*  DATE        :Sep 12, 2015                                          */
/*  DESCRIPTION :External resource handler                             */        
/*                                                                     */
/*  CPU TYPE    :RX64M/RX71M                                           */
/*                                                                     */
/***********************************************************************/
/************************************************************************
Revision History
DD.MM.YYYY OSO-UID Description
12.09.2015 REE - Ported from RX63N GAPI Demo
************************************************************************/
#include "Global.h"
#include "LCD_demo.h"
#include "FlashSerialStubs.h" /* There is no serial flash device fitted on the
                                 RPB RX71M. Calling stub functions */

#define ACCESS_SIZE 256

/**********************************************************************************
Function Name:   ResourceVerify
Description:  Confirm the proper format of a resource file.
Arguments:   ResourceAddress: Location of resource file to test.
Returns:     address of the resource
             0 = Resource NG
***********************************************************************************/
File_Table_type const * ResourceVerify(uint8_t const *ResourceAddress)
{
    File_Table_type const *pReturn = (void *)ResourceAddress;

    /* verify header format */
    if(FileFind(pReturn, "BFS_Header") != ResourceAddress)
    {
        return(NULL);
    }

    {
        /* verify CRC */
        uint32_t size = (((uint32_t)(pReturn[0].size[0]) << 0)  |
                         ((uint32_t)(pReturn[0].size[1]) << 8)  |
                         ((uint32_t)(pReturn[0].size[2]) <<16)  |
                         ((uint32_t)(pReturn[0].size[3]) <<24));

        if(0 != crcFast((void *)ResourceAddress, size))
        {
          return(NULL);
        }
    }

    return(pReturn);
}
/*******************************************************************************
End of function ResourceVerify
*******************************************************************************/

#ifdef EXTERNAL_RESOURCE_FUNCTIONS
/* These functions may be used when resources need to be loaded in to RAM from another
 * storage medium. Not used in the RPB RX71M implementation and provided for
 * information only.
 */

/**********************************************************************************
Function Name:   ResourceLoad
Description:  Load user resources from external device to RAM.
Arguments:   FlashAddress: Location in flash to read from.
              RAMAddress: Location in RAM to put data.
Returns:     address of the resource
             0 = Resource NG
***********************************************************************************/
File_Table_type const * ResourceLoad(uint32_t FlashAddress, uint8_t *RAMAddress, ResourceCallback_t callback)
{
  static const char BFS_Name[] = "BFS_Header";

  File_Table_type BFS_Header;
  int16_t index;

#ifndef SST25V16F
    /* Reset the whole flash */
    FlashReset();
#endif

  /* read in header from serial flash */
  if(0 != FlashReadData(FlashAddress, (unsigned char *)&BFS_Header, sizeof(BFS_Header)))
  {
    return(NULL);
  }
  
  /* basic check of header */
  for (index = 0; 0 != BFS_Name[index]; index++)
  {
    if(BFS_Name[index] != BFS_Header.name[index])
    {
      return(NULL);
    }
  }
  
  {
    /* read in the resource file to RAM */
    uint8_t *pRAM = RAMAddress;
    uint32_t pROM = FlashAddress;
    uint32_t size = (((uint32_t)(BFS_Header.size[0]) << 0)  | 
                 ((uint32_t)(BFS_Header.size[1]) << 8)  |
                 ((uint32_t)(BFS_Header.size[2]) <<16)  |
                 ((uint32_t)(BFS_Header.size[3]) <<24));

    while(pROM < (FlashAddress + size))
    {
      uint32_t read_size = (FlashAddress+size)-pROM;
      if(read_size > ACCESS_SIZE)
      {
        read_size=ACCESS_SIZE;
      }

      if(0 != FlashReadData(pROM, pRAM, read_size))
      {
        return(NULL);
      }

      pROM += ACCESS_SIZE;
      pRAM += ACCESS_SIZE;

      if(NULL != callback)
      {
        callback(RESOURCE_READING, pROM - FlashAddress, size);
      }
    }
  }
  
  {
    /* verify the RAM image */
    File_Table_type const *pResource = ResourceVerify(RAMAddress);
    if (NULL != callback)
    {
      callback(RESOURCE_VERIFYING, (uint32_t)pResource, 0);
    }

    return(pResource);
  }
}
/*******************************************************************************
End of function ResourceLoad
*******************************************************************************/

/**********************************************************************************
Function Name:   ResourceStore
Description:  Store user resources from RAM to external device.
  Emulator tool or user must set ResourceWrite to non-zero in order to send out data.
Arguments:   FlashAddress: Location in flash to write to.
              RAMAddress: Location in RAM to get data from.
Returns: Success/Failure: 0=success, not 0= failure
***********************************************************************************/
int16_t ResourceStore(uint32_t FlashAddress, uint8_t const * RAMAddress, ResourceCallback_t callback)
{
  uint32_t pROM;
  uint32_t size;

#ifndef SST25V16F
    /* Reset the whole flash */
    FlashReset();
#endif

  /* verify data prior to writing */
  
  File_Table_type const *pResource = ResourceVerify(RAMAddress);
  if (NULL != callback)
  {
      callback(RESOURCE_VERIFYING, (uint32_t) pResource, 0);
  }
    
  if (NULL == pResource)
  {
    return(-1);
  }

  /* read the RAM header */
  pROM = FlashAddress;
  size = (((uint32_t)(pResource->size[0]) << 0)  | 
          ((uint32_t)(pResource->size[1]) << 8)  |
          ((uint32_t)(pResource->size[2]) <<16)  |
          ((uint32_t)(pResource->size[3]) <<24));
               
  /* erase flash blocks that we'll be writing */
  while(pROM < (FlashAddress + size))
  {
    if(0 != FlashErase ( pROM, 2))
    {
      return(-1);
    }

    pROM += 0x10000;
    
    if(NULL != callback)
    {
      callback(RESOURCE_ERASING, pROM - FlashAddress, size);
    }
  }
  
  /* write the data to flash */
  pROM = FlashAddress;
  while(pROM < (FlashAddress + size))
  {
    uint32_t read_size = (FlashAddress + size)-pROM;
    if(read_size > ACCESS_SIZE)
    {
      read_size=ACCESS_SIZE;
    }
      
    if(0 != FlashWriteData(pROM, RAMAddress, read_size))
    {
      return(-1);
    }

    pROM += ACCESS_SIZE; 
    RAMAddress+=ACCESS_SIZE;
    
    if(NULL != callback)
    {
      callback(RESOURCE_WRITING, pROM - FlashAddress, size);
    }
  }

  return(0);
}
/*******************************************************************************
End of function ResourceStore
*******************************************************************************/


/**********************************************************************************
Function Name: CheckFlashHeader
Description:   Checks for valid data in the flash and RAM.
Arguments:     none.
Returns:       address of the resource
***********************************************************************************/
File_Table_type const *  CheckFlashHeader(void)
{
  const char BFS_Name[] = "BFS_Header";
  File_Table_type BFS_Header;
  int16_t index;

  /* Image resources are all stored in SDRAM at address 0x080C2650 */
  char * pram_header = (char *)0x080C2650;

  /* read in header from serial flash */
  if(0 != FlashReadData(0, (unsigned char *)&BFS_Header.name, sizeof(BFS_Name)))
  {
    return(NULL); //turn on an LED to indicate that no header has been found in the flash?
  }

  /* basic check of header read from flash */
  for (index = 0; 0 != BFS_Name[index]; index++)
  {
    if(BFS_Name[index] != BFS_Header.name[index])
    {
      return(NULL);
    }
  }

  /* basic check of header read from flash */
  for (index = 0; 0 != BFS_Name[index]; index++, pram_header++)
  {
    if((*pram_header) != BFS_Header.name[index])
    {
      return(NULL);
    }
  }

  pram_header = (char *)0x080C2650;

  /* A header was found, indicating valid images
   * have been copied from the external flash */
  return (pram_header);
}
/*******************************************************************************
End of function CheckFlashHeader
*******************************************************************************/

#endif //EXTERNAL_RESOURCE_FUNCTIONS
