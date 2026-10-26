/* Adapted for use with IAR Embedded Workbench */
/**************** STFL-I based Serial Flash Memory Driver **********************

   Filename:    P5Q.c
   Description: Library routines for the P5Q Flash Memories

   Version:    1.0
   Date:        
   Authors:    Numonyx S.r.l. Arzano (Napoli)

   THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH
   CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS A
   RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT OR
   CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT OF SUCH
   SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION CONTAINED HEREIN
   IN CONNECTION WITH THEIR PRODUCTS.
********************************************************************************
 
********************************************************************************

   This source file provides library C code for   flash devices.

   The following functions are available in this library(some memories may only support
   a subset of the list, refer to the specific product datasheet for details):

      Flash(WriteEnable, 0)                              to disable Write Protect in the Flash memory
      Flash(WriteDisable, 0)                             to enable Write Protect in the Flash memory
      Flash(ReadDeviceIdentification, ParameterType)       to get the Device Identification from the device
      Flash(ReadManufacturerIdentification, ParameterType)  (if available in the memory)    to get the manufacturer Identification from the device
      Flash(ReadStatusRegister, ParameterType)             to get the value in the Status Register from the device
      Flash(WriteStatusRegister, ParameterType)            to set the value in the Status Register from the device

      Flash(Read, ParameterType)                            to read from the Flash device
      Flash(FastRead, ParameterType)                        to read from the Flash device in a faster way
      Flash(DualOutputFastRead, ParameterType)              to output from the Flash device in a faster way by two output pins
      Flash(QuadOutputFastRead, ParameterType)              to output from the Flash device in a faster way by four output pins

      Flash(SectorErase, ParameterType)                     to erase a whole sector
      Flash(BulkErase, ParameterType)                       to erase the whole memory

      Flash(PageProgram, ParameterType)                     to program up to 64 bytes, writes only 0s and uses 1s as data masks
      Flash(PageProgramBitAlterable, ParameterType)         to program up to 64 bytes , writes both 0s and 1s
      Flash(PageProgramOnErasedBlock, ParameterType)        to program up to 64 bytes, writes on all 1s (preSET Write)

      Flash(DualInputProgram, ParameterType)                to program up to 64 bytes, using two input pin at the same time,
                                                                       writes only 0s and uses 1s as data masks, using two input pin at the same time,
      Flash(DualInputProgramBitAlterable, ParameterType)    to program up to 64 bytes , using two input pin at the same time,
                                                                       writes both 0s and 1s
      Flash(DualInputProgramOnErasedBlock, ParameterType)   to program up to 64 bytes, using two input pin at the same time,
                                                                       writes on all 1s (preSET Write)

      Flash(QuadInputProgram, ParameterType)                to program up to 64 bytes, using four input pin at the same time,
                                                                       writes only 0s and uses 1s as data masks, using four input pin at the same time,
      Flash(QuadInputProgramBitAlterable, ParameterType)    to program up to 64 bytes , using four input pin at the same time,
                                                                       writes both 0s and 1s
      Flash(QuadInputProgramOnErasedBlock, ParameterType)   to program up to 64 bytes, using four input pin at the same time,
                                                                       writes on all 1s (preSET Write)


      FlashErrorStr()                                       to return an error description (define VERBOSE)

   Note that data Bytes will be referred to as elements throughout the document unless otherwise specified.

   For further information consult the related Datasheets and Application Note.
   The Application Note gives information about how to modify this code for
   a specific application.

   The hardware specific functions which may need to be modified by the user are:

      FlashWrite() used to write an element (uCPUBusType) to the Flash memory
      FlashRead()  used to read an element (uCPUBusType) from the Flash memory
      FlashTimeOut() to return after the function has timed out

   A list of the error conditions can be found at the end of the header file.

*******************************************************************************/

/**********************************************************************************
System Includes
***********************************************************************************/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "P5Q.h"     /* Header file with global prototypes */
#include "Serialize.h"   /* Header file with SPI master abstract prototypes */
#include "yrdkrx62n_rspi_api.h"

static const NMX_uint32 BlockOffset[] =
{
  0x000000,  /* Start offset of block 0  */
  0x020000,  /* Start offset of block 1  */
  0x040000,  /* Start offset of block 2  */
  0x060000,  /* Start offset of block 3  */
  0x080000,  /* Start offset of block 4  */
  0x0A0000,  /* Start offset of block 5  */
  0x0C0000,  /* Start offset of block 6  */
  0x0E0000,  /* Start offset of block 7  */
  0x100000,  /* Start offset of block 8  */
  0x120000,  /* Start offset of block 9  */
  0x140000,  /* Start offset of block 10 */
  0x160000,  /* Start offset of block 11 */
  0x180000,  /* Start offset of block 12 */
  0x1A0000,  /* Start offset of block 13 */
  0x1C0000,  /* Start offset of block 14 */
  0x1E0000,  /* Start offset of block 15 */
  0x200000,  /* Start offset of block 16 */
  0x220000,  /* Start offset of block 17 */
  0x240000,  /* Start offset of block 18 */
  0x260000,  /* Start offset of block 19 */
  0x280000,  /* Start offset of block 20 */
  0x2A0000,  /* Start offset of block 21 */
  0x2C0000,  /* Start offset of block 22 */
  0x2E0000,  /* Start offset of block 23 */
  0x300000,  /* Start offset of block 24 */
  0x320000,  /* Start offset of block 25 */
  0x340000,  /* Start offset of block 26 */
  0x360000,  /* Start offset of block 27 */
  0x380000,  /* Start offset of block 28 */
  0x3A0000,  /* Start offset of block 29 */
  0x3C0000,  /* Start offset of block 30 */
  0x3E0000,  /* Start offset of block 31 */
  0x400000,  /* Start offset of block 32 */
  0x420000,  /* Start offset of block 33 */
  0x440000,  /* Start offset of block 34 */
  0x460000,  /* Start offset of block 35 */
  0x480000,  /* Start offset of block 36 */
  0x4A0000,  /* Start offset of block 37 */
  0x4C0000,  /* Start offset of block 38 */
  0x4E0000,  /* Start offset of block 39 */
  0x500000,  /* Start offset of block 40 */
  0x520000,  /* Start offset of block 41 */
  0x540000,  /* Start offset of block 42 */
  0x560000,  /* Start offset of block 43 */
  0x580000,  /* Start offset of block 44 */
  0x5A0000,  /* Start offset of block 45 */
  0x5C0000,  /* Start offset of block 46 */
  0x5E0000,  /* Start offset of block 47 */
  0x600000,  /* Start offset of block 48 */
  0x620000,  /* Start offset of block 49 */
  0x640000,  /* Start offset of block 50 */
  0x660000,  /* Start offset of block 51 */
  0x680000,  /* Start offset of block 52 */
  0x6A0000,  /* Start offset of block 53 */
  0x6C0000,  /* Start offset of block 54 */
  0x6E0000,  /* Start offset of block 55 */
  0x700000,  /* Start offset of block 56 */
  0x720000,  /* Start offset of block 57 */
  0x740000,  /* Start offset of block 58 */
  0x760000,  /* Start offset of block 59 */
  0x780000,  /* Start offset of block 60 */
  0x7A0000,  /* Start offset of block 61 */
  0x7C0000,  /* Start offset of block 62 */
  0x7E0000,  /* Start offset of block 63 */
  0x800000,  /* Start offset of block 64 */
  0x820000,  /* Start offset of block 65 */
  0x840000,  /* Start offset of block 66 */
  0x860000,  /* Start offset of block 67 */
  0x880000,  /* Start offset of block 68 */
  0x8A0000,  /* Start offset of block 69 */
  0x8C0000,  /* Start offset of block 70 */
  0x8E0000,  /* Start offset of block 71 */
  0x900000,  /* Start offset of block 72 */
  0x920000,  /* Start offset of block 73 */
  0x940000,  /* Start offset of block 74 */
  0x960000,  /* Start offset of block 75 */
  0x980000,  /* Start offset of block 76 */
  0x9A0000,  /* Start offset of block 77 */
  0x9C0000,  /* Start offset of block 78 */
  0x9E0000,  /* Start offset of block 79 */
  0xA00000,  /* Start offset of block 80 */
  0xA20000,  /* Start offset of block 81 */
  0xA40000,  /* Start offset of block 82 */
  0xA60000,  /* Start offset of block 83 */
  0xA80000,  /* Start offset of block 84 */
  0xAA0000,  /* Start offset of block 85 */
  0xAC0000,  /* Start offset of block 86 */
  0xAE0000,  /* Start offset of block 87 */
  0xB00000,  /* Start offset of block 88 */
  0xB20000,  /* Start offset of block 89 */
  0xB40000,  /* Start offset of block 90 */
  0xB60000,  /* Start offset of block 91 */
  0xB80000,  /* Start offset of block 92 */
  0xBA0000,  /* Start offset of block 93 */
  0xBC0000,  /* Start offset of block 94 */
  0xBE0000,  /* Start offset of block 95 */
  0xC00000,  /* Start offset of block 96 */
  0xC20000,  /* Start offset of block 97 */
  0xC40000,  /* Start offset of block 98 */
  0xC60000,  /* Start offset of block 99 */
  0xC80000,  /* Start offset of block 100 */
  0xCA0000,  /* Start offset of block 101 */
  0xCC0000,  /* Start offset of block 102 */
  0xCE0000,  /* Start offset of block 103 */
  0xD00000,  /* Start offset of block 104 */
  0xD20000,  /* Start offset of block 105 */
  0xD40000,  /* Start offset of block 106 */
  0xD60000,  /* Start offset of block 107 */
  0xD80000,  /* Start offset of block 108 */
  0xDA0000,  /* Start offset of block 109 */
  0xDC0000,  /* Start offset of block 110 */
  0xDE0000,  /* Start offset of block 111 */
  0xE00000,  /* Start offset of block 112 */
  0xE20000,  /* Start offset of block 113 */
  0xE40000,  /* Start offset of block 114 */
  0xE60000,  /* Start offset of block 115 */
  0xE80000,  /* Start offset of block 116 */
  0xEA0000,  /* Start offset of block 117 */
  0xEC0000,  /* Start offset of block 118 */
  0xEE0000,  /* Start offset of block 119 */
  0xF00000,  /* Start offset of block 120 */
  0xF20000,  /* Start offset of block 121 */
  0xF40000,  /* Start offset of block 122 */
  0xF60000,  /* Start offset of block 123 */
  0xF80000,  /* Start offset of block 124 */
  0xFA0000,  /* Start offset of block 125 */
  0xFC0000,  /* Start offset of block 126 */
  0xFE0000   /* Start offset of block 127 */
}; /* EndArray BlockOffset[] */


ReturnType WAIT_TILL_Instruction_EXECUTION_COMPLETE(NMX_sint16 second)
{
  FlashTimeOut(0);
  while(IsFlashBusy())
  {
    if(Flash_OperationTimeOut == FlashTimeOut(second))
      return  Flash_OperationTimeOut;
  }
  return Flash_Success;
}


/******************************************************************************
    Global variables: none
*******************************************************************************/

/*******************************************************************************
Function:     ReturnType Flash( InstructionType insInstruction, ParameterType *fp )
Arguments:    insInstruction is an enum which contains all the available Instructions
    of the SW driver.
              fp is a (union) parameter struct for all Flash Instruction parameters
Return Value: The function returns the following conditions:

   Flash_AddressInvalid,
   Flash_MemoryOverflow,
   Flash_PageEraseFailed,
   Flash_PageNrInvalid,
   Flash_SectorNrInvalid,
   Flash_FunctionNotSupported,
   Flash_NoInformationAvailable,
   Flash_OperationOngoing,
   Flash_OperationTimeOut,
   Flash_ProgramFailed,
   Flash_SpecificError,
   Flash_SectorProtected,
   Flash_SectorUnprotected,
   Flash_SectorProtectFailed,
   Flash_SectorUnprotectFailed,
   Flash_SectorLocked,
   Flash_SectorUnlocked,
   Flash_SectorLockDownFailed,
   Flash_Success,
   Flash_WrongType

Description:  This function is used to access all functions provided with the
   current Flash device.

Pseudo Code:
   Step 1: Select the right action using the insInstruction parameter
   Step 2: Execute the Flash memory Function
   Step 3: Return the Error Code
*******************************************************************************/
ReturnType Flash( InstructionType insInstruction, ParameterType *fp )
{
  ReturnType rRetVal;
  NMX_uint8  ucStatusRegister;
  NMX_uint16 ucDeviceIdentification;
  NMX_uint8  ucManufacturerIdentification;
  
  switch (insInstruction) {
  case WriteEnable:
    rRetVal = FlashWriteEnable( );
    break;
    
  case WriteDisable:
    rRetVal = FlashWriteDisable( );
    break;
    
  case ReadDeviceIdentification:
    rRetVal = FlashReadDeviceIdentification(&ucDeviceIdentification);
    (*fp).ReadDeviceIdentification.ucDeviceIdentification = ucDeviceIdentification;
    break;
    
  case ReadManufacturerIdentification:
    rRetVal = FlashReadManufacturerIdentification(&ucManufacturerIdentification);
    (*fp).ReadManufacturerIdentification.ucManufacturerIdentification = ucManufacturerIdentification;
    break;
    
  case ReadStatusRegister:
    rRetVal = FlashReadStatusRegister(&ucStatusRegister);
    (*fp).ReadStatusRegister.ucStatusRegister = ucStatusRegister;
    break;
    
  case WriteStatusRegister:
    ucStatusRegister = (*fp).WriteStatusRegister.ucStatusRegister;
    rRetVal = FlashWriteStatusRegister(ucStatusRegister);
    break;
    
  case Read:
    rRetVal = FlashRead( (*fp).Read.udAddr,
                        (*fp).Read.pArray,
                        (*fp).Read.udNrOfElementsToRead
                          );
    break;
#if 0 // following cases not supported on RDK+62N board
  case FastRead:
    rRetVal = FlashFastRead( (*fp).Read.udAddr,
                            (*fp).Read.pArray,
                            (*fp).Read.udNrOfElementsToRead
                              );
    break;
    
  case DualOutputFastRead:
    rRetVal = FlashDualOutputFastRead( (*fp).Read.udAddr,
                                      (*fp).Read.pArray,
                                      (*fp).Read.udNrOfElementsToRead
                                        );
    break;
    
  case QuadOutputFastRead:
    rRetVal = FlashQuadOutputFastRead( (*fp).Read.udAddr,
                                      (*fp).Read.pArray,
                                      (*fp).Read.udNrOfElementsToRead
                                        );
    break;
#endif     
  case PageProgram:
    rRetVal = FlashPageProgram( (*fp).PageProgram.udAddr,
                               (*fp).PageProgram.pArray,
                               (*fp).PageProgram.udNrOfElementsInArray
                                 );
    break;
    
  case PageProgramBitAlterable:
    rRetVal = FlashProgramBitAlterable( (*fp).PageProgram.udAddr,
                                       (*fp).PageProgram.pArray,
                                       (*fp).PageProgram.udNrOfElementsInArray
                                         );
    break;
    
  case PageProgramOnErasedBlock:
    rRetVal = FlashProgramOnErasedBlock( (*fp).PageProgram.udAddr,
                                        (*fp).PageProgram.pArray,
                                        (*fp).PageProgram.udNrOfElementsInArray
                                          );
    break;
#if 0 // following cases not supported on RDK+62N board
  case DualInputProgram:
    rRetVal = FlashDualInputProgram( (*fp).PageProgram.udAddr,
                                    (*fp).PageProgram.pArray,
                                    (*fp).PageProgram.udNrOfElementsInArray
                                      );
    break;
    
  case DualInputProgramBitAlterable:
    rRetVal = FlashDualInputProgramBitAlterable( (*fp).PageProgram.udAddr,
                                                (*fp).PageProgram.pArray,
                                                (*fp).PageProgram.udNrOfElementsInArray
                                                  );
    break;
    
  case DualInputProgramOnErasedBlock:
    rRetVal = FlashDualInputProgramOnErasedBlock( (*fp).PageProgram.udAddr,
                                                 (*fp).PageProgram.pArray,
                                                 (*fp).PageProgram.udNrOfElementsInArray
                                                   );
    break;
    
  case QuadInputProgram:
    rRetVal = FlashQuadInputProgram( (*fp).PageProgram.udAddr,
                                    (*fp).PageProgram.pArray,
                                    (*fp).PageProgram.udNrOfElementsInArray
                                      );
    break;
    
  case QuadInputProgramBitAlterable:
    rRetVal = FlashQuadInputProgramBitAlterable( (*fp).PageProgram.udAddr,
                                                (*fp).PageProgram.pArray,
                                                (*fp).PageProgram.udNrOfElementsInArray
                                                  );
    break;
    
  case QuadInputProgramOnErasedBlock:
    rRetVal = FlashQuadInputProgramOnErasedBlock( (*fp).PageProgram.udAddr,
                                                 (*fp).PageProgram.pArray,
                                                 (*fp).PageProgram.udNrOfElementsInArray
                                                   );
    break;
#endif                        
  case SectorErase:
    rRetVal = FlashSectorErase((*fp).SectorErase.ustSectorNr );
    break;
    
  case BulkErase:
    rRetVal = FlashBulkErase( );
    break;
    
  default:
    rRetVal = Flash_FunctionNotSupported;
    break;
    
  } /* EndSwitch */
  return rRetVal;
} /* EndFunction Flash */

/*******************************************************************************
Function:     ReturnType Program( InstructionType insInstruction, ParameterType *fp )
Arguments:    insInstruction is an enum which contains all the available Instructions
    of the SW driver.
              fp is a (union) parameter struct for all Flash Instruction parameters
Return Value: The function returns the following conditions:

   Flash_AddressInvalid,
   Flash_MemoryOverflow,
   Flash_PageEraseFailed,
   Flash_PageNrInvalid,
   .
   Flash_SectorNrInvalid,
   Flash_FunctionNotSupported,
   Flash_NoInformationAvailable,
   Flash_OperationOngoing,
   Flash_OperationTimeOut,
   Flash_ProgramFailed,
   Flash_SpecificError,
   Flash_SectorProtected,
   Flash_SectorUnprotected,
   Flash_SectorProtectFailed,
   Flash_SectorUnprotectFailed,
   Flash_SectorLocked,
   Flash_SectorUnlocked,
   Flash_SectorLockDownFailed,
   Flash_Success,
   Flash_WrongType

Description:  This function is used to access all functions provided with the
   current Flash device.

Pseudo Code:
   Step 1: Select the right action using the insInstruction parameter
   Step 2: Execute the Flash memory Function
   Step 3: Return the Error Code
*******************************************************************************/
ReturnType DataProgram( InstructionType insInstruction, ParameterType *fp )
{
  ReturnType  rRetVal;
  //    NMX_uint8  ucStatusRegister;
  //    NMX_uint16 ucDeviceIdentification;
  //    NMX_uint8  ucManufacturerIdentification;
  
  NMX_uint8   insCode;
  
  
  switch (insInstruction)
  {
  case PageProgram:
    insCode = SPI_FLASH_INS_PP;
    break;
    
  case PageProgramBitAlterable:
    insCode = SPI_FLASH_INS_PPBA;
    break;
    
  case PageProgramOnErasedBlock:
    insCode = SPI_FLASH_INS_PPPW;
    break;
    
  case DualInputProgram:
    insCode = SPI_FLASH_INS_DIPP;
    break;
    
  case DualInputProgramBitAlterable:
    insCode = SPI_FLASH_INS_DIBA;
    break;
    
  case DualInputProgramOnErasedBlock:
    insCode = SPI_FLASH_INS_DIPW;
    break;
    
  case QuadInputProgram:
    insCode = SPI_FLASH_INS_QIPP;
    break;
    
  case QuadInputProgramBitAlterable:
    insCode = SPI_FLASH_INS_QIBA;
    break;
    
  case QuadInputProgramOnErasedBlock:
    insCode = SPI_FLASH_INS_QIPW;
    break;
    
  default:
    return Flash_FunctionNotSupported;
    break;
    
  } /* EndSwitch */
  
  rRetVal = FlashDataProgram( (*fp).PageProgram.udAddr,
                             (*fp).PageProgram.pArray,
                             (*fp).PageProgram.udNrOfElementsInArray,
                             insCode
                               );
  
  return rRetVal;
} /* EndFunction Flash */


/*******************************************************************************
Function:     FlashWriteEnable( void )
Arguments:    void

Return Value:
   Flash_Success

Description:  This function sets the Write Enable Latch(WEL)
              by sending a WREN Instruction.

Pseudo Code:
   Step 1: Initialize the data (i.e. Instruction) packet to be sent serially
   Step 2: Send the packet serially
*******************************************************************************/
ReturnType  FlashWriteEnable( void )
{
  NMX_uint8 cWREN = SPI_FLASH_INS_WREN;
  NMX_uint8 ucSR;
  
  // Step 2: Send the packet serially
  YRDKRX62N_RSPI_Select  (FLASH_SELECTED);  
  YRDKRX62N_RSPI_Write   (&cWREN, 1 );
  YRDKRX62N_RSPI_Deselect(FLASH_SELECTED);
  
  // Step 3: Read the Status Register.
  do {
    FlashReadStatusRegister(&ucSR);
  } while(~ucSR & SPI_SR1_WEL);
  
  return Flash_Success;
}

/*******************************************************************************
Function:     FlashWriteDisable( void )
Arguments:    void

Return Value:
   Flash_Success

Description:  This function resets the Write Enable Latch(WEL)
              by sending a WRDI Instruction.

Pseudo Code:
   Step 1: Initialize the data (i.e. Instruction) packet to be sent serially
   Step 2: Send the packet serially
*******************************************************************************/
ReturnType  FlashWriteDisable( void )
{
  NMX_uint8  cWRDI = SPI_FLASH_INS_WRDI;
  NMX_uint8  ucSR;
  
  // Step 2: Send the packet serially
  YRDKRX62N_RSPI_Select  (FLASH_SELECTED);  
  YRDKRX62N_RSPI_Write   (&cWRDI, 1 );
  YRDKRX62N_RSPI_Deselect(FLASH_SELECTED);
  
  // Step 3: Read the Status Register.
  do {
    FlashReadStatusRegister(&ucSR);
  } while(ucSR & SPI_SR1_WEL);
  
  return Flash_Success;
}


/*******************************************************************************
Function:     FlashReadDeviceIdentification( NMX_uint16 *uwpDeviceIdentification)
Arguments:    uwpDeviceIdentificaiton, 16-bit buffer to hold the DeviceIdentification read from the
              memory, with memory type residing in the higher 8 bits, and
              memory capacity in the lower ones.

Return Value:
   Flash_Success
   Flash_WrongType(if USE_JEDEC_STANDARD_TWO_BYTE_SIGNATURE defined)

Description:  This function returns the Device Identification (memory type + memory capacity)
              by sending an SPI_FLASH_INS_RDID Instruction.
              After retrieving the Device Identificaiton, the routine checks if the device is
              an expected device(defined by EXPECTED_DEVICE). If not,
              Flash_WrongType is returned.
              the returned 16-bit word comprises memory type(higher 8 bits) and memory capacity
              (lower 8 bits).

Pseudo Code:
   Step 1: Initialize the data (i.e. Instruction) packet to be sent serially
   Step 2: Send the packet serially
   Step 3: Device Identification is returned
*******************************************************************************/
ReturnType  FlashReadDeviceIdentification( NMX_uint16 *uwpDeviceIdentification)
{
  NMX_uint8  cRDID = SPI_FLASH_INS_RDID;
  NMX_uint8  pIdentification[3];
  
  // Step 2: Send the packet serially
  YRDKRX62N_RSPI_Select  (FLASH_SELECTED);  
  YRDKRX62N_RSPI_Write   (&cRDID, 1 );
  YRDKRX62N_RSPI_Read    (&pIdentification[0], 3 );
  YRDKRX62N_RSPI_Deselect(FLASH_SELECTED);
  
  // Step 3: Device Identification is returned ( memory type + memory capacity )
  *uwpDeviceIdentification   = pIdentification[1];
  *uwpDeviceIdentification <<= 8;
  *uwpDeviceIdentification  |= pIdentification[2];
  
  if(EXPECTED_DEVICE == *uwpDeviceIdentification)
    return Flash_Success;
  else
    return Flash_WrongType;
}


/*******************************************************************************
Function:     FlashReadManufacturerIdentification( NMX_uint8 *ucpManufactureIdentification) ----ok
Arguments:    ucpManufacturerIdentification, 8-bit buffer to hold the manufacturer identification
              being read from the memory

Return Value:
   Flash_WrongType: if any value other than MANUFACTURER_ST(0x20) is returned
   Flash_Success : if MANUFACTURER_ST(0x20) is correctly returned

Description:  This function returns the Manufacturer Identification(0x20) by sending an
              SPI_FLASH_INS_RDID Instruction.
              After retrieving the Manufacturer Identification, the routine checks if the device is
              an ST memory product. If not, Flash_WrongType is returned.

Note: The availability of this function should be checked in the appropriate datasheet
  for each memory.

Pseudo Code:
   Step 1: Initialize the data (i.e. Instruction) packet to be sent serially
   Step 2: Send the packet serially
   Step 3: get the Manufacturer Identification
*******************************************************************************/
ReturnType  FlashReadManufacturerIdentification( NMX_uint8 *ucpManufacturerIdentification)
{
  NMX_uint8  cRDID = SPI_FLASH_INS_RDID;
  NMX_uint8  pIdentification[3];
  
  // Step 2: Send the packet serially  
  YRDKRX62N_RSPI_Select  (FLASH_SELECTED);  
  YRDKRX62N_RSPI_Write   (&cRDID, 1);
  YRDKRX62N_RSPI_Read    (&pIdentification[0], 3);
  YRDKRX62N_RSPI_Deselect(FLASH_SELECTED);
  
  // Step 3: get the Manufacturer Identification
  *ucpManufacturerIdentification = pIdentification[0];
  if (MANUFACTURER_NMX == *ucpManufacturerIdentification)
  {
    return Flash_Success;
  }
  else
  {
    return Flash_WrongType;
  }
}


/*******************************************************************************
Function:     FlashReadStatusRegister( NMX_uint8 *ucpStatusRegister)    ----ok
Arguments:    ucpStatusRegister, 8-bit buffer to hold the Status Register value read
              from the memory

Return Value:
   Flash_Success

Description:  This function reads the Status Register by sending an
               SPI_FLASH_INS_RDSR Instruction.

Pseudo Code:
   Step 1: Initialize the data (i.e. Instruction) packet to be sent serially
   Step 2: Send the packet serially, get the Status Register content

*******************************************************************************/
ReturnType  FlashReadStatusRegister( NMX_uint8 *ucpStatusRegister)
{
  NMX_uint8  cRDSR = SPI_FLASH_INS_RDSR;
  
  // Step 2: Send the packet serially, get the Status Register content
  YRDKRX62N_RSPI_Select(FLASH_SELECTED);  
  YRDKRX62N_RSPI_Write(&cRDSR, 1 );
  YRDKRX62N_RSPI_Read( ucpStatusRegister, 1 );
  YRDKRX62N_RSPI_Deselect(FLASH_SELECTED);
  
  return Flash_Success;
}


/*******************************************************************************
Function:     FlashWriteStatusRegister( NMX_uint8 ucStatusRegister)
Arguments:    ucStatusRegister, an 8-bit new value to be written to the Status Register

Return Value:
   Flash_Success

Description:  This function modifies the Status Register by sending an
              SPI_FLASH_INS_WRSR Instruction.
              The Write Status Register (WRSR) Instruction has no effect
              on b6, b5, b1(WEL) and b0(WIP) of the Status Register.b6 and b5 are
              always read as 0.

Pseudo Code:
   Step 1: Disable Write protection
   Step 2: Initialize the data (i.e. Instruction & value) packet to be sent serially
   Step 3: Send the packet serially
   Step 4: Wait until the operation completes or a timeout occurs.
*******************************************************************************/
ReturnType  FlashWriteStatusRegister( NMX_uint8 ucStatusRegister)
{
  NMX_uint8  pIns_Val[2];
  
  // Step 1: Disable Write protection
  FlashWriteEnable();
  
  // Step 2: Initialize the data (i.e. Instruction & value) packet to be sent serially
  pIns_Val[0] = SPI_FLASH_INS_WRSR;
  pIns_Val[1] = ucStatusRegister;
  
  // Step 3: Send the packet serially
  YRDKRX62N_RSPI_Select(FLASH_SELECTED);  
  YRDKRX62N_RSPI_Write(&pIns_Val[0], 2 );
  YRDKRX62N_RSPI_Deselect(FLASH_SELECTED);
  
  // Step 4: Wait until the operation completes or a timeout occurs.
  return WAIT_TILL_Instruction_EXECUTION_COMPLETE(1);
}


/*******************************************************************************
Function:     FlashRead( NMX_uint32 udAddr, NMX_uint8 *ucpElements, NMX_uint32 udNrOfElementsToRead)
Arguments:    udAddr, start address to read from
              ucpElements, buffer to hold the elements to be returned
              udNrOfElementsToRead, number of elements to be returned, counted in bytes.

Return Value:
   Flash_AddressInvalid
   Flash_Success

Description:  This function reads the Flash memory by sending an
              SPI_FLASH_INS_READ Instruction.
              by design, the whole Flash memory space can be read with one READ Instruction
              by incrementing the start address and rolling to 0x0 automatically,
              that is, this function is across pages and sectors.

Pseudo Code:
   Step 1: Validate address input
   Step 2: Initialize the data (i.e. Instruction) packet to be sent serially
   Step 3: Send the packet serially, and fill the buffer with the data being returned
*******************************************************************************/
ReturnType  FlashRead( uAddrType udAddr, NMX_uint8 *ucpElements, NMX_uint32 udNrOfElementsToRead)
{
  NMX_uint8  pIns_Addr[4];
  
  // Step 1: Validate address input
  if(!(udAddr <  FLASH_SIZE)) 
    return Flash_AddressInvalid;
  
  // Step 2: Initialize the data (i.e. Instruction) packet to be sent serially
  pIns_Addr[0]              = SPI_FLASH_INS_READ;
  pIns_Addr[1]              = udAddr >> 16;
  pIns_Addr[2]              = udAddr >>  8;
  pIns_Addr[3]              = udAddr;
  
  // Step 3: Send the packet serially, and fill the buffer with the data being returned
  YRDKRX62N_RSPI_Select(FLASH_SELECTED);  
  YRDKRX62N_RSPI_Write(&pIns_Addr[0], 4 );
  YRDKRX62N_RSPI_Read(ucpElements, udNrOfElementsToRead );
  YRDKRX62N_RSPI_Deselect(FLASH_SELECTED);
  
  return Flash_Success;
}

#if 0
/*******************************************************************************
Function:     FlashFastRead( NMX_uint32 udAddr, NMX_uint8 *ucpElements, NMX_uint32 udNrOfElementsToRead)
Arguments:    udAddr, start address to read from
              ucpElements, buffer to hold the elements to be returned
              udNrOfElementsToRead, number of elements to be returned, counted in bytes.

Return Value:
   Flash_AddressInvalid
   Flash_Success

Description:  This function reads the Flash memory by sending an
              SPI_FLASH_INS_FAST_READ Instruction.
              by design, the whole Flash memory space can be read with one FAST_READ Instruction
              by incrementing the start address and rolling to 0x0 automatically,
              that is, this function is across pages and sectors.

Pseudo Code:
   Step 1: Validate address input
   Step 2: Initialize the data (i.e. Instruction) packet to be sent serially
   Step 3: Send the packet serially, and fill the buffer with the data being returned
*******************************************************************************/
ReturnType  FlashFastRead( uAddrType udAddr, NMX_uint8 *ucpElements, NMX_uint32 udNrOfElementsToRead)
{
  NMX_uint8  pIns_Addr[4];
  
  // Step 1: Validate address input
  if(!(udAddr <  FLASH_SIZE)) 
    return Flash_AddressInvalid;
  
  // Step 2: Initialize the data (i.e. Instruction) packet to be sent serially
  pIns_Addr[0]              = SPI_FLASH_INS_FAST_READ;
  pIns_Addr[1]              = udAddr >> 16;
  pIns_Addr[2]              = udAddr >>  8;
  pIns_Addr[3]              = udAddr;
  
  // Step 3: Send the packet serially, and fill the buffer with the data being returned
  YRDKRX62N_RSPI_Select(FLASH_SELECTED);  
  YRDKRX62N_RSPI_Write(&pIns_Addr[0], 4 );
  YRDKRX62N_RSPI_Read(ucpElements, udNrOfElementsToRead );
  YRDKRX62N_RSPI_Deselect(FLASH_SELECTED);
  
  return Flash_Success;
}

/*******************************************************************************
Function:     FlashDualOutputFastRead( NMX_uint32 udAddr, NMX_uint8 *ucpElements, NMX_uint32 udNrOfElementsToRead)
Arguments:    udAddr, start address to read from
              ucpElements, buffer to hold the elements to be returned
              udNrOfElementsToRead, number of elements to be returned, counted in bytes.

Return Value:
   Flash_AddressInvalid
   Flash_Success

Description:  This function reads the Flash memory by sending an
              SPI_FLASH_INS_DOFR Instruction.
              by design, the whole Flash memory space can be read with one DOFR Instruction
              with two output pins(pinDQ0 and pinDQ1) instead of only one
              by incrementing the start address and rolling to 0x0 automatically,
              that is, this function is across pages and sectors.

Pseudo Code:
   Step 1: Validate address input
   Step 2: Initialize the data (i.e. Instruction) packet to be sent serially
   Step 3: Send the packet serially, and fill the buffer with the data being returned
*******************************************************************************/
ReturnType  FlashDualOutputFastRead(uAddrType udAddr, NMX_uint8 * ucpElements, NMX_uint32 udNrOfElementsToRead)
{
  NMX_uint8  pIns_Addr[5];
  
  // Step 1: Validate address input
  if(!(udAddr <  FLASH_SIZE)) 
    return Flash_AddressInvalid;
  
  // Step 2: Initialize the data (i.e. Instruction) packet to be sent serially
  pIns_Addr[0]              = SPI_FLASH_INS_DOFR;
  pIns_Addr[1]              = udAddr >> 16;
  pIns_Addr[2]              = udAddr >>  8;
  pIns_Addr[3]              = udAddr;
  pIns_Addr[4]              = SPI_FLASH_INS_DUMMY;
  
  // Step 3: Send the packet serially, and fill the buffer with the data being returned
  YRDKRX62N_RSPI_Select(FLASH_SELECTED);  
  YRDKRX62N_RSPI_Write(&pIns_Addr[0], 5 );
  YRDKRX62N_RSPI_Read(ucpElements, udNrOfElementsToRead );
  YRDKRX62N_RSPI_Deselect(FLASH_SELECTED);
  
  return Flash_Success;
}

/*******************************************************************************
Function:     FlashQuadOutputFastRead( NMX_uint32 udAddr, NMX_uint8 *ucpElements, NMX_uint32 udNrOfElementsToRead)
Arguments:    udAddr, start address to read from
              ucpElements, buffer to hold the elements to be returned
              udNrOfElementsToRead, number of elements to be returned, counted in bytes.

Return Value:
   Flash_AddressInvalid
   Flash_Success

Description:  This function reads the Flash memory by sending an
              SPI_FLASH_INS_QOFR Instruction.
              by design, the whole Flash memory space can be read with one DOFR Instruction
              with two output pins(pinDQ0 and pinDQ1) instead of only one
              by incrementing the start address and rolling to 0x0 automatically,
              that is, this function is across pages and sectors.

Pseudo Code:
   Step 1: Validate address input
   Step 2: Initialize the data (i.e. Instruction) packet to be sent serially
   Step 3: Send the packet serially, and fill the buffer with the data being returned
*******************************************************************************/
ReturnType  FlashQuadOutputFastRead(uAddrType udAddr, NMX_uint8 * ucpElements, NMX_uint32 udNrOfElementsToRead)
{
  NMX_uint8  pIns_Addr[5];
  
  // Step 1: Validate address input
  if(!(udAddr <  FLASH_SIZE)) 
    return Flash_AddressInvalid;
  
  // Step 2: Initialize the data (i.e. Instruction) packet to be sent serially
  pIns_Addr[0]              = SPI_FLASH_INS_QOFR;
  pIns_Addr[1]              = udAddr >> 16;
  pIns_Addr[2]              = udAddr >>  8;
  pIns_Addr[3]              = udAddr;
  pIns_Addr[4]              = SPI_FLASH_INS_DUMMY;
  
  // Step 3: Send the packet serially, and fill the buffer with the data being returned
  YRDKRX62N_RSPI_Select(FLASH_SELECTED);  
  YRDKRX62N_RSPI_Write(&pIns_Addr[0], 5 );
  YRDKRX62N_RSPI_Read(ucpElements, udNrOfElementsToRead );
  YRDKRX62N_RSPI_Deselect(FLASH_SELECTED);
  
  return Flash_Success;
}
#endif 

/*******************************************************************************
Function:     FlashPageProgram( NMX_uint32 udAddr, NMX_uint8 *pArray, NMX_uint32 udNrOfElementsInArray)
Arguments:    udAddr, start address to write to
              pArray, buffer to hold the elements to be programmed
              udNrOfElementsInArray, number of elements to be programmed, counted in bytes

Return Value:
   Flash_AddressInvalid
   Flash_OperationOngoing
   Flash_OperationTimeOut
   Flash_Success

Description:  This function writes a maximum of 64 bytes of data into the memory by sending an
              SPI_FLASH_INS_PP Instruction.
              by design, the PP Instruction is effective WITHIN ONE page,i.e. 0xXX00 - 0xXXff.
              when 0xXXff is reached, the address rolls over to 0xXX00 automatically.
Note:
              This function does not check whether the target memory area is in a Software
              Protection Mode(SPM) or Hardware Protection Mode(HPM), in which case the PP
              Instruction will be ignored.
              The function assumes that the target memory area has previously been unprotected at both
              the hardware and software levels.
              To unprotect the memory, please call FlashWriteStatusRegister(NMX_uint8 ucStatusRegister),
              and refer to the datasheet for the setup of a proper ucStatusRegister value.
Pseudo Code:
   Step 1: Validate address input
   Step 2: Check whether any previous Write, Program or Erase cycle is on going
   Step 3: Disable Write protection (the Flash memory will automatically enable it again after
           the execution of the Instruction)
   Step 4: Initialize the data (Instruction & address only) packet to be sent serially
   Step 5: Send the packet (Instruction & address only) serially
   Step 6: Initialize the data (data to be programmed) packet to be sent serially
   Step 7: Send the packet (data to be programmed) serially
   Step 8: Wait until the operation completes or a timeout occurs.
*******************************************************************************/
ReturnType  FlashGenProgram( uAddrType udAddr, NMX_uint8 *pArray , NMX_uint32 udNrOfElementsInArray, NMX_uint8 ubSpiInstruction)
{
  NMX_uint8  pIns_Addr[4];
  
  // Step 1: Validate address input
  if(!(udAddr <  FLASH_SIZE)) 
    return Flash_AddressInvalid;
  
  // Step 2: Check whether any previous Write, Program or Erase cycle is on-going
  if(IsFlashBusy()) 
    return Flash_OperationOngoing;
  
  // Step 3: Disable Write protection
  FlashWriteEnable();
  
  // Step 4: Initialize the data (Instruction & address only) packet to be sent serially
  pIns_Addr[0]              = ubSpiInstruction; //SPI_FLASH_INS_PP;
  pIns_Addr[1]              = udAddr >> 16;
  pIns_Addr[2]              = udAddr >>  8;
  pIns_Addr[3]              = udAddr;
  
  // Step 5: Send the packet (Instruction & address only) serially
  YRDKRX62N_RSPI_Select(FLASH_SELECTED);  
  YRDKRX62N_RSPI_Write(&pIns_Addr[0], 4 );
  
  // Step 6: Initialize the data (data to be programmed) packet to be sent serially
  
  
  // Step 7: Send the packet (data to be programmed) serially
  YRDKRX62N_RSPI_Write(&pArray[0], udNrOfElementsInArray );
  YRDKRX62N_RSPI_Deselect(FLASH_SELECTED);  
  
  // Step 8: Wait until the operation completes or a timeout occurs.
  return WAIT_TILL_Instruction_EXECUTION_COMPLETE(1);
}


/*******************************************************************************
Function:     ReturnType FlashSectorErase( uSectorType uscSectorNr )
Arguments:    uSectorType is the number of the Sector to be erased.

Return Values:
   Flash_SectorNrInvalid
   Flash_OperationOngoing
   Flash_OperationTimeOut
   
   Flash_Success

Description:  This function erases the Sector specified in uscSectorNr by sending an
              SPI_FLASH_INS_SE Instruction.
              The function checks that the sector number is within the valid range
              before issuing the erase Instruction. Once erase has completed the status
              Flash_Success is returned.
Note:
              This function does not check whether the target memory area is in a Software
              Protection Mode(SPM) or Hardware Protection Mode(HPM), in which case the PP
              Instruction will be ignored.
              The function assumes that the target memory area has previously been unprotected at both
              the hardware and software levels.
              To unprotect the memory, please call FlashWriteStatusRegister(NMX_uint8 ucStatusRegister),
              and refer to the datasheet to set a proper ucStatusRegister value.

Pseudo Code:
   Step 1: Validate the sector number input
   Step 2: Check whether any previous Write, Program or Erase cycle is on going
   Step 3: Disable Write protection (the Flash memory will automatically enable it
           again after the execution of the Instruction)
   Step 4: Initialize the data (Instruction & address) packet to be sent serially
   Step 5: Send the packet (Instruction & address) serially
   Step 6: Wait until the operation completes or a timeout occurs.
*******************************************************************************/
ReturnType  FlashSectorErase( uSectorType uscSectorNr )
{
  NMX_uint8  pIns_Addr[4];
  uAddrType SectorAddr;
  
  // Step 1: Validate the sector number input
  if(!(uscSectorNr < FLASH_SECTOR_COUNT)) 
    return Flash_SectorNrInvalid;
  
  SectorAddr = BlockOffset[uscSectorNr];
  
  // Step 2: Check whether any previous Write, Program or Erase cycle is on going
  if(IsFlashBusy()) 
    return Flash_OperationOngoing;
  
  // Step 3: Disable Write protection
  FlashWriteEnable();
  
  // Step 4: Initialize the data (Instruction & address) packet to be sent serially
  pIns_Addr[0]              = SPI_FLASH_INS_SE;
  pIns_Addr[1]              = SectorAddr >> 16;
  pIns_Addr[2]              = SectorAddr >>  8;
  pIns_Addr[3]              = SectorAddr;
  
  // Step 5: Send the packet (Instruction & address) serially
  YRDKRX62N_RSPI_Select(FLASH_SELECTED);  
  YRDKRX62N_RSPI_Write(&pIns_Addr[0], 4 );
  YRDKRX62N_RSPI_Deselect(FLASH_SELECTED);
  
  // Step 6: Wait until the operation completes or a timeout occurs.
  return WAIT_TILL_Instruction_EXECUTION_COMPLETE(SE_TIMEOUT);
}

/*******************************************************************************
Function:     ReturnType FlashBulkErase( void )
Arguments:    none

Return Values:
   Flash_OperationOngoing
   Flash_OperationTimeOut
   Flash_Success

Description:  This function erases the whole Flash memory by sending an
              SPI_FLASH_INS_BE Instruction.
Note:
              This function does not check whether the target memory area (or part of it)
        is in a Software Protection Mode(SPM) or Hardware Protection Mode(HPM),
        in which case the PP Instruction will be ignored.
              The function assumes that the target memory area has previously been unprotected at both
              the hardware and software levels.
              To unprotect the memory, please call FlashWriteStatusRegister(NMX_uint8 ucStatusRegister),
              and refer to the datasheet to set a proper ucStatusRegister value.

Pseudo Code:
   Step 1: Check whether any previous Write, Program or Erase cycle is on going
   Step 2: Disable the Write protection (the Flash memory will automatically enable it
           again after the execution of the Instruction)
   Step 3: Initialize the data (Instruction & address) packet to be sent serially
   Step 4: Send the packet (Instruction & address) serially
   Step 5: Wait until the operation completes or a timeout occurs.
*******************************************************************************/
ReturnType  FlashBulkErase( void )
{
  NMX_uint8  cBE = SPI_FLASH_INS_BE;
  
  // Step 1: Check whether any previous Write, Program or Erase cycle is on going
  if(IsFlashBusy()) 
    return Flash_OperationOngoing;
  
  // Step 2: Disable Write protection
  FlashWriteEnable();
  
  // Step 3: Initialize the data(Instruction & address) packet to be sent serially
  
  // Step 4: Send the packet(Instruction & address) serially
  YRDKRX62N_RSPI_Select(FLASH_SELECTED);  
  YRDKRX62N_RSPI_Write(&cBE, 1 );
  YRDKRX62N_RSPI_Deselect(FLASH_SELECTED);
  
  // Step 5: Wait until the operation completes or a timeout occurs.
  return WAIT_TILL_Instruction_EXECUTION_COMPLETE(BE_TIMEOUT);
}

/*******************************************************************************
Function:     IsFlashBusy( )
Arguments:    none

Return Value:
   TRUE
   FALSE

Description:  This function checks the Write In Progress (WIP) bit to determine whether
              the Flash memory is busy with a Write, Program or Erase cycle.

Pseudo Code:
   Step 1: Read the Status Register.
   Step 2: Check the WIP bit.
*******************************************************************************/
BOOL IsFlashBusy()
{
  NMX_uint8 ucSR;
  
  // Step 1: Read the Status Register.
  FlashReadStatusRegister(&ucSR);
  
  // Step 2: Check the WIP bit.
  if(ucSR & SPI_FLASH_WIP)
    return TRUE;
  else
    return FALSE;
}

/*******************************************************************************
Function:     IsFlashWELBusy( )
Arguments:    none

Return Value:
   TRUE
   FALSE

Description:  This function checks the Write Enable bit to determine whether
              the Flash memory is busy with a Write Enable or Write Disable Op.

Pseudo Code:
   Step 1: Read the Status Register.
   Step 2: Check the WEL bit.
*******************************************************************************/
BOOL IsFlashWELBusy()
{
  NMX_uint8 ucSR;
  
  // Step 1: Read the Status Register.
  FlashReadStatusRegister(&ucSR);
  
  // Step 2: Check the WEL bit.
  if(ucSR & SPI_FLASH_WEL)
    return TRUE;
  else
    return FALSE;
}

ReturnType FlashDataProgram( uAddrType udAddr, NMX_uint8 *pArray , NMX_uint16 udNrOfElementsInArray, NMX_uint8 ubSpiInstruction)
{
  ReturnType retValue = Flash_Success;
  NMX_uint16 dataOffset;
  
  // Enabling the Write
  retValue = FlashWriteEnable();
  if (retValue != Flash_Success)
    return retValue;
  
  // Computing the starting alignment, i.e. the distance from the 64 bytes boundary
  dataOffset = (FLASH_PAGE_SIZE - (udAddr & FLASH_ADDRESS_MASK) ) & FLASH_ADDRESS_MASK;
  
  if (dataOffset > 0)
    retValue = FlashGenProgram(udAddr, pArray, dataOffset, ubSpiInstruction); 
  
  
  for ( ; (dataOffset+FLASH_PAGE_SIZE) < udNrOfElementsInArray; dataOffset += FLASH_PAGE_SIZE)
  {
    retValue = FlashGenProgram(udAddr + dataOffset, pArray + dataOffset, FLASH_PAGE_SIZE, ubSpiInstruction); 
  }
  
  if (udNrOfElementsInArray > dataOffset)
    retValue = FlashGenProgram(udAddr + dataOffset, pArray + dataOffset, (udNrOfElementsInArray - dataOffset), ubSpiInstruction);     
  
  return retValue;
}

#ifdef VERBOSE
/*******************************************************************************
Function:     FlashErrorStr( ReturnType rErrNum );
Arguments:    rErrNum is the error number returned from other Flash memory Routines

Return Value: A pointer to a string with the error message

Description:  This function is used to generate a text string describing the
   error from the Flash memory. Call with the return value from other Flash memory routines.

Pseudo Code:
   Step 1: Return the correct string.
*******************************************************************************/
NMX_sint8 *FlashErrorStr( ReturnType rErrNum )
{
  switch(rErrNum)
  {
  case Flash_AddressInvalid:
    return "Flash - Address is out of Range";
  case Flash_MemoryOverflow:
    return "Flash - Memory Overflows";
  case Flash_PageEraseFailed:
    return "Flash - Page Erase failed";
  case Flash_PageNrInvalid:
    return "Flash - Page Number is out of Range";
  case Flash_SectorNrInvalid:
    return "Flash - Sector Number is out of Range";
  case Flash_FunctionNotSupported:
    return "Flash - Function not supported";
  case Flash_NoInformationAvailable:
    return "Flash - No Additional Information Available";
  case Flash_OperationOngoing:
    return "Flash - Operation ongoing";
  case Flash_OperationTimeOut:
    return "Flash - Operation TimeOut";
  case Flash_ProgramFailed:
    return "Flash - Program failed";
  case Flash_Success:
    return "Flash - Success";
  case Flash_WrongType:
    return "Flash - Wrong Type";
  default:
    return "Flash - Undefined Error Value";
  } /* EndSwitch */
} /* EndFunction FlashErrorString */
#endif /* VERBOSE Definition */


/*******************************************************************************
Function:     FlashTimeOut(NMX_uint32 udSeconds)
Arguments:    udSeconds holds the number of seconds before TimeOut occurs

Return Value:
   Flash_OperationTimeOut
   Flash_OperationOngoing

Example:   FlashTimeOut(0)  // Initializes the Timer

           While(1) {
              ...
              If (FlashTimeOut(5) == Flash_OperationTimeOut) break;
              // The loop is executed for 5 Seconds before the operation is aborted
           } EndWhile

*******************************************************************************/
#ifdef TIME_H_EXISTS
/*-----------------------------------------------------------------------------
Description:   This function provides a timeout for Flash polling actions or
   other operations which would otherwise never return.
   The Routine uses the function clock() inside ANSI C library "time.h".
-----------------------------------------------------------------------------*/
ReturnType FlashTimeOut(NMX_uint32 udSeconds)
{
  static clock_t clkReset,clkCount;
  
  if (udSeconds == 0) { /* Set Timeout to 0 */
    clkReset=clock();
  } /* EndIf */
  
  clkCount = clock() - clkReset;
  
  if (clkCount<(CLOCKS_PER_SEC*(clock_t)udSeconds))
    return Flash_OperationOngoing;
  else
    return Flash_OperationTimeOut;
}/* EndFunction FlashTimeOut */

#else

/*-----------------------------------------------------------------------------
Description:   This function provides a timeout for Flash polling actions or
   other operations which would otherwise never return.
   The Routine uses COUNT_FOR_A_SECOND which is considered to be a loop that
   counts for one second. It needs to be adapted to the target Hardware.
-----------------------------------------------------------------------------*/
ReturnType FlashTimeOut(NMX_uint32 udSeconds)
{
  static NMX_uint32 udCounter = 0;
  if (udSeconds == 0) { /* Set Timeout to 0 */
    udCounter = 0;
  } /* EndIf */
  
  if (udCounter == (udSeconds * COUNT_FOR_A_SECOND)) {
    udCounter = 0;
    return Flash_OperationTimeOut;
  } else {
    udCounter++;
    return Flash_OperationOngoing;
  } /* Endif */

} /* EndFunction FlashTimeOut */

#endif 

/* End of file */
                                                                                                                                                                                                                                                                                                                                                                                                                                                          
