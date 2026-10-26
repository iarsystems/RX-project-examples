/* Adapted for use with IAR Embedded Workbench */
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
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.    
*******************************************************************************/
/******************************************************************************
* File Name     : flash_demo_functions.c
* Version       : 1.0
* Device(s)     : R5F562N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.1
* OS            : None
* H/W Platform  : YRDKRX63N
* Description   : Demonstrates use of the Micron PCM flash device on the
*                 YRDKRX63N. Uses flash device driver library provided by
*                 Numonyx.
*
*******************************************************************************/ 
/*******************************************************************************
* History : DD.MM.YYYY     Version     Description
*         : 14.09.2011     1.0         First release 
*           19.01.2012                 CS revisions
*******************************************************************************/

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdint.h>
#include <stdbool.h>

#include "flash_demo_functions.h"
#include "P5Q.h"            /* Numonxy P5Q flash device definitions. */
#include "data_file.h"      /* Sample file data. */


/*******************************************************************************
* Macro definitions
*******************************************************************************/
#define FLASH_SECTOR             7  /* Will write and read this sector. */
#define FLASH_SECTOR_ADDR 0x0E0000  /* Sector address base. Must correspond to FLASH_SECTOR */


/*******************************************************************************
* Local global variables
*******************************************************************************/
uint8_t g_Buff[512] = {0};          /* Buffer for read back and compare. */

ParameterType   parameter_block;    /* Contains parameters used by P5Q driver */
ParameterType   *p_param_block = &parameter_block;


/*******************************************************************************
* Function Name: PQ5_flash_verify
* Description  : Routine to validate RDKRX62N SPI attached flash device. 
* Argument     : none
* Return value : bool true,  OK
*                     false, failed to verify the device
*******************************************************************************/
bool PQ5_flash_verify()
{
  uint32_t status;
  
  status = Flash(ReadManufacturerIdentification, p_param_block);
  
  if (Flash_Success != status)
  {
    return false;
  }
  
  status = Flash(ReadDeviceIdentification, p_param_block);
  
  if (Flash_Success != status)
  {
    return false;
  }
  
  return true;
} /* End of function PQ5_flash_verify() */


/*******************************************************************************
* Function Name: PQ5_flash_write_read_demo
* Description  : Demonstrate programming the Micron PQ5 SPI attached flash device. 
* Argument     : none
* Return value : bool true, OK
*                     false, failed to verify the device
*******************************************************************************/
bool PQ5_flash_write_read_demo()
{
  bool        err = true;
  int32_t     byte_count;
  uint16_t    data_count;    
  
  /*  Clear the section of flash to write to. */
  parameter_block.SectorErase.ustSectorNr = FLASH_SECTOR;
  if (Flash_Success != Flash(SectorErase, p_param_block))
  {
    err = false;
  }
  
  /*  write "filedata" out to the flash device ( 64 bytes at a time ) */
  /*  Clear the counter before counting the no. of bytes to write     */
  byte_count = 0;
  
  /* Repeat the write process until required no. of bytes are written */
  while ((byte_count < (sizeof(filedata) - 1)) && (err == true))
  {
    /* update the P5Q parameter block with pertinent flash info */
    parameter_block.PageProgram.udAddr                = FLASH_SECTOR_ADDR + byte_count;
    parameter_block.PageProgram.udNrOfElementsInArray = FLASH_PAGE_SIZE; 
    parameter_block.PageProgram.pArray                = (void *)(filedata + byte_count);
    
    if (Flash_Success != Flash(WriteEnable, p_param_block))
    {
      err = false;
    }
    
    if (Flash_Success != Flash(PageProgram, p_param_block))
    {
      err = false;
    }
    
    if (Flash_Success != Flash(WriteDisable, p_param_block))
    {
      err = false;
    }
    
    /* Update the counter with the total no. of bytes read from the file */
    byte_count += parameter_block.PageProgram.udNrOfElementsInArray;    
  }
  
  /* Read the data back 512 bytes at at time and compare to databuffer */
  /* Clear the counter before counting the number of bytes read.       */
  byte_count = 0;
  
  /* Repeat the read process until required no. of bytes are read */
  while ((byte_count < (sizeof(filedata) - 1)) && (err == true))
  {
    /* Read the data 512 bytes at a time. */
    parameter_block.PageProgram.udAddr                = FLASH_SECTOR_ADDR + byte_count;    
    parameter_block.PageProgram.udNrOfElementsInArray = 512;
    parameter_block.PageProgram.pArray                = (void *)g_Buff;
    
    if (Flash_Success != Flash( Read, p_param_block))
    {
      err = false;
    }
    else
    {
      /* Compare the contents of the file with the ROM data.    */
      for (data_count = 0; data_count < sizeof(g_Buff); data_count++)
      {
        if (g_Buff[data_count] != filedata[byte_count + data_count])
        {
          err = false;
        }
      }
    }
    
    /* Update the counter with the total no. of bytes read from the file. */
    byte_count += parameter_block.PageProgram.udNrOfElementsInArray;
  }
  
  return err;
} /* End of function PQ5_flash_write_read_demo() */

/*******************************************************************************
* End of file: flash_demo_functions.c
*******************************************************************************/
