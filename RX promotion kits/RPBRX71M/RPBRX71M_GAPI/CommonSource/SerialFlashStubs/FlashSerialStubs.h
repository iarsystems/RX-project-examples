/******************************************************************************
* DISCLAIMER
* Please refer to http://www.renesas.com/disclaimer
******************************************************************************/
/*********************************************************************************                                                                                   *
FILE NAME     FlashSerial.h
DESCRIPTION:  This code acts as an interface to an external serial flash device.
              No Serial Flash device is fitted to the RPB RX71M so this file
              acts as just stub functions - returning error is appropriate.

Copyright   : 2016 Renesas Electronics Corporation.
All Rights Reserved
**********************************************************************************/


/*******************************************************************************
Name:    FlashReset
Arguments: None
Returns:  status response
Description: Reads flash status register and returns.
*******************************************************************************/
void FlashReset (void);


/*****************************************************************************
Name:      FlashWriteData
Arguments: address: Target address to write to.
            data: Location to retrieve data to write.
            size: Amount of data to write.
Returns: Success/Failure: 0=completed
Description: Writes data to the external flash (up to 256 bytes)
             This is a stub function and always returns an error code.
*****************************************************************************/
unsigned char FlashWriteData (unsigned long address, unsigned char const * data, unsigned long size);

/*******************************************************************************
Name:    FlashReadData
Arguments: address: Target address to read from.
            data: Location to place read data.
            size: Amount of data to read.
Returns: Success/Failure: 0=success, not 0= failure
Description: Performs a read of the external flash to specified buffer.
             This is a stub function and always returns an error code.
*******************************************************************************/
unsigned char FlashReadData (const unsigned long address, unsigned char * data, const unsigned long size);

/*****************************************************************************
Name:    FlashErase
Arguments: Address: address of sector to erase
            size: 0=4K, 1=32K, 2=64K, 3 = chip
Returns: Success/Failure: 0=success, not 0= failure
Description: Performs a 4k erase at address
             This is a stub function and always returns an error code.
*****************************************************************************/
unsigned char FlashErase (const unsigned long address, const unsigned char size);
