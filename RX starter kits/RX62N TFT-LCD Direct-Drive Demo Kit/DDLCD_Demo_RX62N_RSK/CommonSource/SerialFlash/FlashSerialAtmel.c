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

/*********************************************************************************                                                                                   *
FILE NAME      FlashSerial.c
DESCRIPTION:  This code acts as an interface to an external serial flash device.
The specific device tested is an Atmel 26F081A. This module relies upon having a
lower level interface to the MCU specific spi port. The functionality of this module
is independent of target MCU and only depends on the protocol of the Serial Flash.

Copyright   : 2007 Renesas Electronics Corporation.
All Rights Reserved
**********************************************************************************/
/**********************************************************************************
Revision History
DD.MM.YYYY OSO-UID Description
05.16.2007 RTA-JMB First Release
**********************************************************************************/
/**********************************************************************************
System Includes
***********************************************************************************/


/**********************************************************************************
User Includes
***********************************************************************************/
#include "FlashSerial.h"
#include "spi.h"

/* ************************* Global variables ********************************/

/*****************************************************************************
Name:    WriteEnable
Arguments: none
Returns: none
Description: Performs a write enable command
*****************************************************************************/
static void WriteEnable (void)
{
  /* send write enable command */
  const unsigned char command = 0x6;

  /* Initialize peripheral for SPI */
  SPI_open();

  SPI_send_receive(&command, NULL, 1 );

  /* close peripheral for SPI */
  SPI_close();
}

/*****************************************************************************
Name:    WriteProtect
Arguments: None
Returns:  None
Description: Sets the Status Register Write Disable (SRWD) bit.
             Setting this bit locks the status register from being rewritten
       if the Write Protect pin is high.  If the pin is low, then the
       status register can be changed.
             You must set/clear the Block Protect Bits at this time too.

*****************************************************************************/
static void WriteProtect (void)
{
  unsigned char command[2];

  /* send write enable command */
  WriteEnable();

  /* Initialize peripheral for SPI */
  SPI_open();

  /*  This section writes data.  The first character is the write command */
  command[0] = 0x1;
  command[1] = 0x85;
  SPI_send_receive(command, NULL, 2 );

  /* close peripheral for SPI */
  SPI_close();
}

/*****************************************************************************
Name:    WriteUnprotect
Arguments: None
Returns:  None
Description: Clears the Status Register Write Disable (SRWD) bit, if the Write
             Protect pin is high.
             Clearing this bit unlocks the status register for rewriting.
       The Write Protect pin must be high to clear the SRWD bit.
             You can set/clear the Block Protect Bits at this time too.

*****************************************************************************/
static void WriteUnprotect (void)
{
  unsigned char command[2];

  /* send write enable command */
  WriteEnable();

  /* Initialize peripheral for SPI */
  SPI_open();

  /*  This section writes data.  The first character is the write command */
  command[0] = 0x1;
  command[1] = 0x0;
  SPI_send_receive(command, NULL, 2 );

  /* close peripheral for SPI */
  SPI_close();
}

/*****************************************************************************
Name:    FlashEraseSector
Arguments: Address: address of sector to erase
            size: 0=4K, 1=32K, 2=64K, 3 = chip
Returns: Success/Failure: 0=success, not 0= failure
Description: Performs a 4k erase at address
*****************************************************************************/
static const unsigned char erase_commands[]={0x20, 0x52, 0xD8, 0xC7};
unsigned char FlashErase (const unsigned long address, const unsigned char size)
{
  unsigned char command[4];

  /* allow memory to be modified */
  WriteUnprotect();

  /* send write enable command */
  WriteEnable();

  /* Initialize peripheral for SPI */
  SPI_open();

  /* 4k erase command */
  command[0] = erase_commands[size];
  command[1] = (unsigned char)(address >> 16);
  command[2] = (unsigned char)(address >>  8);
  command[3] = (unsigned char)(address >>  0);
  SPI_send_receive(command, NULL, 4 );

  /* close peripheral for SPI */
  SPI_close();

  /* check status to make sure previous operation completed */
  while ((FlashReadStatus() & FLASH_BSY) == FLASH_BSY)
    ;
    
  /* protect memory from modification */
  WriteProtect();

  return(0);
}

/*****************************************************************************
Name:    FlashWriteData
Arguments: address: Target address to write to.
            data: Location to retrieve data to write.
            size: Amount of data to write.
Returns: Success/Failure: 0=success, not 0= failure
Description: Writes data to the external flash (up to 256 bytes)
*****************************************************************************/
unsigned char FlashWriteData (unsigned long address, unsigned char const *data, unsigned long size)
{
  unsigned char command[4];

  /* allow memory to be modified */
  WriteUnprotect();

  while(size > 0)
  {
    unsigned long transfer_size;
    if((address & 0xFF) != 0)
      transfer_size = 0x100 - (address & 0xFF);
    else
      transfer_size = 0x100;
    
    if(size < transfer_size)
      transfer_size = size;
    
    /* send write enable command */
    WriteEnable();

    /* Initialize peripheral for SPI */
    SPI_open();

    /*  This section writes data.  The first character is the write command */
    command[0] = 0x2;
    command[1] = (unsigned char)(address >> 16);
    command[2] = (unsigned char)(address >>  8);
    command[3] = (unsigned char)(address >>  0);
    SPI_send_receive(command, NULL, 4 );

    /* write data buffer to the flash */
    SPI_send_receive(data,NULL, transfer_size );

    /* close peripheral for SPI */
    SPI_close();
    
    /* update address & size values */
    size -= transfer_size;
    address += transfer_size;
    data += transfer_size;
    
    /* check status to make sure previous operation completed */
    while ((FlashReadStatus() & FLASH_BSY) == FLASH_BSY)
      ;
  }
  
  /* protect memory from modification */
  WriteProtect();

  return(0);
}

/*****************************************************************************
Name:    FlashReadData
Arguments: address: Target address to read from.
            data: Location to place read data.
            size: Amount of data to read.
Returns: Success/Failure: 0=success, not 0= failure
Description: Performs a read of the external flash to specified buffer.
*****************************************************************************/
unsigned char FlashReadData (const unsigned long address, unsigned char *data, const unsigned long size)
{
  unsigned char command[4];

  /* Initialize peripheral for SPI */
  SPI_open();

  /*  This section reads back data.  The first character is the read command */
  command[0] = 0x3;
  command[1] = (unsigned char)(address >> 16);
  command[2] = (unsigned char)(address >>  8);
  command[3] = (unsigned char)(address >>  0);
  SPI_send_receive(command, NULL, 4 );

  SPI_send_receive( NULL, data, size );

  /* close peripheral for SPI */
  SPI_close();
  return(0);
}

/*****************************************************************************
Name:    FlashReadStatus
Arguments: None
Returns:  status response
Description: Reads flash status register and returns.
*****************************************************************************/
unsigned char FlashReadStatus (void)
{
  unsigned char command;

  /* Initialize peripheral for SPI */
  SPI_open();

  command = 0x05;
  SPI_send_receive( &command, NULL, 1);
  SPI_send_receive( NULL, &command, 1);

  /* close peripheral for SPI */
  SPI_close();

  return(command);
}

