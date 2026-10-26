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
              No Serial Flash device is fitted to the RPB RX71M so this file
              acts as just stub functions - returning error is appropriate.

Copyright   : 2016 Renesas Electronics Corporation.
All Rights Reserved
**********************************************************************************/
/**********************************************************************************
Revision History
DD.MM.YYYY OSO-UID Description
08.04.2016 First Release
**********************************************************************************/
/**********************************************************************************
System Includes
***********************************************************************************/


/**********************************************************************************
User Includes
***********************************************************************************/
#include "platform.h"
#include "Global.h"

/*******************************************************************************
Name:    FlashReset
Arguments: None
Returns:  status response
Description: Reads flash status register and returns.
*******************************************************************************/
void FlashReset (void)
{
	/* This is a stub function */
}
/*******************************************************************************
End of function FlashReset
*******************************************************************************/


/*****************************************************************************
Name:      FlashWriteData
Arguments: address: Target address to write to.
            data: Location to retrieve data to write.
            size: Amount of data to write.
Returns: Success/Failure: 0=completed
Description: Writes data to the external flash (up to 256 bytes)
             This is a stub function and always returns an error code.
*****************************************************************************/
unsigned char FlashWriteData (unsigned long address, unsigned char const * data, unsigned long size)
{
	return(0xFF);       /* Return an error - no serial flash fitted */
}
/*******************************************************************************
End of function FlashWriteData
*******************************************************************************/



/*******************************************************************************
Name:    FlashReadData
Arguments: address: Target address to read from.
            data: Location to place read data.
            size: Amount of data to read.
Returns: Success/Failure: 0=success, not 0= failure
Description: Performs a read of the external flash to specified buffer.
             This is a stub function and always returns an error code.
*******************************************************************************/
unsigned char FlashReadData (const unsigned long address, unsigned char * data, const unsigned long size)
{
    return(0xFF);       /* Return an error - no serial flash fitted */
}
/*******************************************************************************
End of function FlashReadData
*******************************************************************************/


/*****************************************************************************
Name:    FlashErase
Arguments: Address: address of sector to erase
            size: 0=4K, 1=32K, 2=64K, 3 = chip
Returns: Success/Failure: 0=success, not 0= failure
Description: Performs a 4k erase at address
             This is a stub function and always returns an error code.
*****************************************************************************/
unsigned char FlashErase (const unsigned long address, const unsigned char size)
{
    return(0xFF);       /* Return an error - no serial flash fitted */
}
/*******************************************************************************
End of function FlashErase
*******************************************************************************/

