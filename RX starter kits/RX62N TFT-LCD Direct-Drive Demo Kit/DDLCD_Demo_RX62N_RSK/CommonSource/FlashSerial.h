/******************************************************************************
* DISCLAIMER
* Please refer to http://www.renesas.com/disclaimer
******************************************************************************/
/* Status Register Bit Definitions */
#define   FLASH_BSY    0x01
#define   FLASH_RDY    0x01
#define   FLASH_WEL    0x02
#define   FLASH_SWP    0x0C
#define   FLASH_WPP    0x10
#define   FLASH_EPE    0x20
#define   FLASH_SPM    0x40
#define   FLASH_SPRL   0x80

/*****************************************************************************
Name:    FlashEraseSector
Arguments: Address: address of sector to erase
size: 0=4K, 1=32K, 2=64K, 3 = chip
Returns: Success/Failure: 0=success, not 0= failure
Description: Performs a erase or requested size at address
*****************************************************************************/
unsigned char FlashErase (const unsigned long address, const unsigned char size);

/*****************************************************************************
Name:    ReadData
Arguments: address: Target address to read from.
            data: Location to place read data.
            size: Amount of data to read.
Returns: Success/Failure: 0=success, not 0= failure
Description: Performs a read of the external flash to specified buffer.
*****************************************************************************/
unsigned char FlashReadData (const unsigned long address, unsigned char *data, const unsigned long size);

/*****************************************************************************
Name:    WriteData
Arguments: address: Target address to write to.
            data: Location to retrieve data to write.
            size: Amount of data to write.
Returns: Success/Failure: 0=success, not 0= failure
Description: Writes data to the external flash
*****************************************************************************/
unsigned char FlashWriteData (unsigned long address, unsigned char const *data, unsigned long size);

/*****************************************************************************
Name:    ReadStatus
Arguments: None
Returns:  status response
Description: Reads flash status register and returns.
*****************************************************************************/
unsigned char FlashReadStatus (void);
