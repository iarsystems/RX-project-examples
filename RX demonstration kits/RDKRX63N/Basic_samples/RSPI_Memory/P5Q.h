/* Adapted for use with IAR Embedded Workbench */
/***************** Header File for STFL-I based Serial Flash Memory Driver *****

   Filename:    
   Description: Header file for 
                Also consult the C file for more details.

                Please note that some necessary changes are made in favor of the
                SPI-specific communication property which slightly differs from
                the STFL-I Specification designed for parallel NOR Flash memories. The major
                differences from the SPECIFICATION OF THE STFL-I SOFTWARE DRIVER INTERFACE
            are the following:
                - Flash Configuration Selection is not used.
                - BASE_ADDR is not used.
                - InstructionType enumuerations are re-formulated to use SPI Flash instructions.
                - CONFIGURATION CONSTANTS are fixed, with #define ins(A) not used.

   Version:    1.0
   Date:        Sept. 2009
   Authors:    Numonyx S.r.l. Arzano (Napoli)

   THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH
   CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS A
   RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT OR
   CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT OF SUCH
   SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION CONTAINED HEREIN
   IN CONNECTION WITH THEIR PRODUCTS.
********************************************************************************

   Version History.
   Ver.   Date         Comments
   1.0   Sept. 2009  
   
*******************************************************************************/


/*************** User Change Area *******************************************

   The purpose of this section is to show how the SW Drivers can be customized
   according to the requirements of the hardware and Flash memory configurations.
   It is possible to choose the Flash memory start address, the CPU Bit depth, the number of Flash
   chips, the hardware configuration and performance data (TimeOut Info).

   The options are listed and explained below:

   ********* Data Types *********
   The source code defines hardware independent datatypes assuming that the
   compiler implements the numerical types as

   unsigned char    8 bits (defined as NMX_uint8)
   char             8 bits (defined as NMX_sint8)
   unsigned int     16 bits (defined as NMX_uint16)
   int              16 bits (defined as NMX_sint16)
   unsigned long    32 bits (defined as NMX_uint32)
   long             32 bits (defined as NMX_sint32)

   In case the compiler does not support the currently used numerical types,
   they can be easily changed just once here in the user area of the headerfile.
   The data types are consequently referenced in the source code as (u)NMX_sint8,
   (u)NMX_sint16 and (u)NMX_sint32. No other data types like 'CHAR','SHORT','INT','LONG'
   are directly used in the code.


   ********* Flash Type *********
   This driver supports the following Serial Flash memory Types


   P5Q        32Mb Serial Flash Memory                  #define USE_P5Q


   ********* Flash and Board Configuration *********
   The driver also supports different configurations of the Flash chips
   on the board. In each configuration a new data Type called
   'uCPUBusType' is defined to match the current CPU data bus width.
   This data type is then used for all accesses to the memory.

   Because SPI interface communications are controlled by the
   SPI master, which, in turn, is accessed by the CPU as an 8-bit data
   buffer, the configuration is fixed for all cases.

   ********* TimeOut *********
   There are timeouts implemented in the loops of the code, in order
   to enable a timeout detection for operations that would otherwise never terminate.
   There are two possibilities:

   1) The ANSI Library functions declared in 'time.h' exist

      If the current compiler supports 'time.h' the define statement
      TIME_H_EXISTS should be activated. This makes sure that
      the performance of the current evaluation HW does not change
      the timeout settings.

   2) or they are not available (COUNT_FOR_A_SECOND)

      If the current compiler does not support 'time.h', the define
      statement cannot be used. In this case the COUNT_FOR_A_SECOND
      value has to be defined so as to create a one-second delay.
      For example, if 100000 repetitions of a loop are
      needed to give a time delay of one second, then
      COUNT_FOR_A_SECOND should have the value 100000.

      Note: This delay is HW (Performance) dependent and therefore needs
      to be updated with every new HW.

      This driver has been tested with a certain configuration and other
      target platforms may have other performance data, therefore, the
      value may have to be changed.

      It is up to the user to implement this value to prevent the code
      from timing out too early and allow correct completion of the device
      operations.


   ********* Additional Routines *********
   The drivers also provide a subroutine which displays the full
   error message instead of just an error number.

   The define statement VERBOSE activates additional Routines.
   Currently it activates the FlashErrorStr() function

   No further changes should be necessary.

*****************************************************************************/

#ifndef __P5Q__H__
#define __P5Q__H__

#define DRIVER_VERSION_MAJOR 0
#define DRIVER_VERSION_MINOR 1


typedef unsigned char  NMX_uint8; /* All HW dependent Basic Data Types */
typedef  signed        char  NMX_sint8; /* (M) All HW dependent Basic Data Types */
typedef unsigned int   NMX_uint16;
typedef          int   NMX_sint16;
typedef unsigned long  NMX_uint32;
typedef          long  NMX_sint32;

/* With SYNCHRONOUS_IO defined, each function that sends an Instruction(e.g. PE)
   shall not return until the Flash memory finishes executing the Instruction
   or a pre-set timeout limit is reached. the pre-set timeout value is in
   accordance with the datasheet of each memory.

   To achieve Send-n-Forget feature, comment out this #define*/
//#define SYNCHRONOUS_IO
#define NO_DEEP_POWER_DOWN_SUPPORT

#define USE_P5Q

/* Possible Values: USE_P5Q
                    */


/*#define TIME_H_EXISTS*/  /* set this macro if C-library "time.h" is supported */
/* Possible Values: TIME_H_EXISTS
                    - no define - TIME_H_EXISTS */


#ifndef TIME_H_EXISTS
  #define COUNT_FOR_A_SECOND 0xFFFFFF         /* Timer Usage */
#endif


#define VERBOSE /* Activates additional Routines */
/* Currently the Error String Definition */

/********************** End of User Change Area *****************************/

/*******************************************************************************
Device Constants
*******************************************************************************/

#define MANUFACTURER_NMX (0x20)                /* Numonyx Manufacturer Identification is 0x18 */
#define MEMORYTYPE_P5Q (0xDA)                /* JEDEC Memory Type for P5Q Identification is 0xDA */
#define ANY_ADDR (0x0)                        /* Any address offset within the Flash Memory will do */

#ifdef USE_P5Q                                /* The USE_P5Q device */
   #define EXPECTED_DEVICE       (0xDA18)        /* Device Identification for the USE_M25P32 */
   #define FLASH_SIZE         (0x1000000)         /* Total device size in Bytes (128Mbit) */
   #define FLASH_PAGE_COUNT     (0x40000)         /* Total device size in Pages */
   #define FLASH_SECTOR_COUNT     (0x0080)          /* Total device size in Sectors */
   #define FLASH_PAGE_SIZE       (0x0040)            /* Page Size in Bytes*/
   #define FLASH_ADDRESS_MASK    (0x003F)            /* Page Size in Bytes*/
   #define FLASH_SECTOR_SIZE     (0x400)             /* Sector Size in Bytes*/
   #define FLASH_WRITE_BUFFER_SIZE  (0x0040)        /* Write Buffer = 64 bytes */
   #define FLASH_MWA 1                            /* Minimum Write Access */
   #define SE_TIMEOUT (3)                        /* Timeout in seconds suggested for Sector Erase Operation*/
   #define BE_TIMEOUT (80)                        /* Timeout in seconds suggested for Bulk Erase Operation*/

#endif /* USE_P5Q */


/*******************************************************************************
     DERIVED DATATYPES
*******************************************************************************/
/******** InstructionsCode ********/
#define SPI_FLASH_INS_DUMMY 0xAA                  /* dummy byte*/
enum
{
  //Instruction set
  SPI_FLASH_INS_WREN        = 0x06,          /* write enable*/
  SPI_FLASH_INS_WRDI        = 0x04,          /* write disable*/
  SPI_FLASH_INS_RDID        = 0x9F,          /* Read Identification 20 bytes*/
  /*SPI_FLASH_INS_RDID1         = 0x9E,        / * Read Identification 3 bytes*/
  SPI_FLASH_INS_RDSR        = 0x05,          /* read status register*/
  SPI_FLASH_INS_WRSR        = 0x01,          /* write status register*/

  SPI_FLASH_INS_READ        = 0x03,          /* read data bytes*/
  SPI_FLASH_INS_FAST_READ   = 0x0B,          /* read data bytes at higher speed*/
  SPI_FLASH_INS_DOFR        = 0x3B,              /*Dual output Fast Read*/
  SPI_FLASH_INS_QOFR        = 0x6B,              /*Quad output Fast Read*/

  SPI_FLASH_INS_PP          = 0x02,          /* page program*/
  SPI_FLASH_INS_PPBA        = 0x22,          /* page program bit alterable*/
  SPI_FLASH_INS_PPPW        = 0xD1,          /* page program PreSET Write*/

  SPI_FLASH_INS_DIPP        = 0xA2,          /* dual input page program*/
  SPI_FLASH_INS_DIBA        = 0xD3,          /* dual input page program bit alterable*/
  SPI_FLASH_INS_DIPW        = 0xD5,          /* dual input page program PreSET Write*/

  SPI_FLASH_INS_QIPP        = 0xA2,          /* quad input page program*/
  SPI_FLASH_INS_QIBA        = 0xD3,          /* quad input page program bit alterable*/
  SPI_FLASH_INS_QIPW        = 0xD5,          /* quad input page program PreSET Write*/

  SPI_FLASH_INS_SE          = 0xD8,          /* sector erase*/
    SPI_FLASH_INS_BE           = 0xC7,          /* bulk erase*/

};

/******** InstructionsType ********/

typedef enum {
   WriteEnable,
   WriteDisable,

   ReadDeviceIdentification,
   ReadManufacturerIdentification,

   ReadStatusRegister,
   WriteStatusRegister,

   Read,
   FastRead,
   DualOutputFastRead,
   QuadOutputFastRead,

   PageProgram,
   PageProgramBitAlterable,
   PageProgramOnErasedBlock,

   DualInputProgram,
   DualInputProgramBitAlterable,
   DualInputProgramOnErasedBlock,

   QuadInputProgram,
   QuadInputProgramBitAlterable,
   QuadInputProgramOnErasedBlock,

   SectorErase,
   BulkErase,

} InstructionType;

/******** ReturnType ********/

typedef enum {
   Flash_AddressInvalid,
   Flash_MemoryOverflow,
   Flash_PageEraseFailed,
   Flash_PageNrInvalid,
   Flash_SubSectorNrInvalid,
   Flash_SectorNrInvalid,
   Flash_FunctionNotSupported,
   Flash_NoInformationAvailable,
   Flash_OperationOngoing,
   Flash_OperationTimeOut,
   Flash_ProgramFailed,
   Flash_SectorProtected,
   Flash_SectorUnprotected,
   Flash_SectorProtectFailed,
   Flash_SectorUnprotectFailed,
   Flash_SectorLocked,
   Flash_SectorUnlocked,
   Flash_SectorLockDownFailed,
   Flash_WrongType,
   Flash_Success
} ReturnType;

/******** SectorType ********/

typedef NMX_uint8 uSectorType;

/******** SubSectorType ********/

typedef NMX_uint16 uSubSectorType;

/******** PageType ********/

typedef NMX_uint16 uPageType;

/******** AddrType ********/

typedef NMX_uint32 uAddrType;

/******** ParameterType ********/

typedef union {

    /**** WriteEnable has no parameters ****/

    /**** WriteDisable has no parameters ****/

    /**** ReadDeviceIdentification Parameters ****/
    struct {
      NMX_uint16 ucDeviceIdentification;
    } ReadDeviceIdentification;

    /**** ReadManufacturerIdentification Parameters ****/
    struct {
      NMX_uint8 ucManufacturerIdentification;
    } ReadManufacturerIdentification;

    /**** ReadStatusRegister Parameters ****/
    struct {
      NMX_uint8 ucStatusRegister;
    } ReadStatusRegister;

    /**** WriteStatusRegister Parameters ****/
    struct {
      NMX_uint8 ucStatusRegister;
    } WriteStatusRegister;

    /**** Read Parameters ****/
    struct {
      uAddrType udAddr; 
      NMX_uint32 udNrOfElementsToRead;
      void *pArray;
    } Read;

    /**** PageProgram Parameters ****/
    struct {
      uAddrType udAddr;
      NMX_uint32 udNrOfElementsInArray;
      void *pArray;
    } PageProgram;

    /**** SectorErase Parameters ****/
    struct {
      uSectorType ustSectorNr;
    } SectorErase;

    /***** BulkErase has no parameters ****/

    /**** Clear  has no parameters ****/


} ParameterType;

/******************************************************************************
    Standard functions
*******************************************************************************/
//#define 

/******************************************************************************
    Standard functions
*******************************************************************************/
  ReturnType  Flash( InstructionType insInstruction, ParameterType *fp );
  ReturnType  FlashWriteEnable( void );
  ReturnType  FlashWriteDisable( void );
  ReturnType  FlashReadDeviceIdentification( NMX_uint16 *uwpDeviceIdentification);
  ReturnType  FlashReadManufacturerIdentification( NMX_uint8 *ucpManufacturerIdentification);
  ReturnType  FlashReadStatusRegister( NMX_uint8 *ucpStatusRegister);
  ReturnType  FlashWriteStatusRegister( NMX_uint8 ucStatusRegister);

  ReturnType  FlashRead( uAddrType udAddr, NMX_uint8 *ucpElements, NMX_uint32 udNrOfElementsToRead);
  ReturnType  FlashFastRead( uAddrType udAddr, NMX_uint8 *ucpElements, NMX_uint32 udNrOfElementsToRead);
  ReturnType  FlashDualOutputFastRead( uAddrType udAddr, NMX_uint8 *ucpElements, NMX_uint32 udNrOfElementsToRead);
  ReturnType  FlashQuadOutputFastRead( uAddrType udAddr, NMX_uint8 *ucpElements, NMX_uint32 udNrOfElementsToRead);
  ReturnType  FlashGenProgram( NMX_uint32 udAddr, NMX_uint8 *pArray , NMX_uint32 udNrOfElementsInArray, NMX_uint8 ubSpiInstruction);

  #define  FlashPageProgram(p1, p2, p3) (FlashGenProgram (p1, p2, p3, SPI_FLASH_INS_PP))
  #define  FlashProgramBitAlterable(p1, p2, p3) (FlashGenProgram (p1, p2, p3, SPI_FLASH_INS_PPBA))
  #define  FlashProgramOnErasedBlock(p1, p2, p3) (FlashGenProgram (p1, p2, p3, SPI_FLASH_INS_PPPW))

  #define  FlashDualInputProgram(p1, p2, p3) (FlashGenProgram (p1, p2, p3, SPI_FLASH_INS_DIPP))
  #define  FlashDualInputProgramBitAlterable(p1, p2, p3) (FlashGenProgram (p1, p2, p3, SPI_FLASH_INS_DIBA))
  #define  FlashDualInputProgramOnErasedBlock(p1, p2, p3) (FlashGenProgram (p1, p2, p3, SPI_FLASH_INS_DIPW))

  #define  FlashQuadInputProgram(p1, p2, p3) (FlashGenProgram (p1, p2, p3, SPI_FLASH_INS_QIPP))
  #define  FlashQuadInputProgramBitAlterable(p1, p2, p3) (FlashGenProgram (p1, p2, p3, SPI_FLASH_INS_QIBA))
  #define  FlashQuadInputProgramOnErasedBlock(p1, p2, p3) (FlashGenProgram (p1, p2, p3, SPI_FLASH_INS_QIPW))

  ReturnType FlashDataProgram( uAddrType udAddr, NMX_uint8 *pArray , NMX_uint16 udNrOfElementsInArray, NMX_uint8 ubSpiInstruction);

  ReturnType  FlashSectorErase( uSectorType uscSectorNr );
  ReturnType  FlashBulkErase( void );


/******************************************************************************
    Utility functions
*******************************************************************************/
#ifdef VERBOSE
   NMX_sint8 *FlashErrorStr( ReturnType rErrNum );
#endif

  ReturnType  FlashTimeOut( NMX_uint32 udSeconds );

/*******************************************************************************
List of Errors and Return values, Explanations and Help.
********************************************************************************

Error Name:   Flash_AddressInvalid
Description:  The address given is out of the range of the Flash device.
Solution:     Check whether the address is in the valid range of the
              Flash device.
********************************************************************************

Error Name:   Flash_PageEraseFailed
Description:  The Page erase Instruction did not complete successfully.
Solution:     Try to erase the Page again. If this fails once more, the device
              may be faulty and need to be replaced.
********************************************************************************

Error Name:   Flash_PageNrInvalid
Note:         The Flash memory is not at fault.
Description:  A Page has been selected (Parameter), which is not
              within the valid range. Valid Page numbers are from 0 to
              FLASH_PAGE_COUNT - 1.
Solution:     Check that the Page number given is in the valid range.
********************************************************************************

Error Name:   Flash_SectorNrInvalid
Note:         The Flash memory is not at fault.
Description:  A Sector has been selected (Parameter), which is not
              within the valid range. Valid Page numbers are from 0 to
              FLASH_SECTOR_COUNT - 1.
Solution:     Check that the Sector number given is in the valid range.
********************************************************************************

Return Name:  Flash_FunctionNotSupported
Description:  The user has attempted to make use of a functionality not
              available on this Fash device (and thus not provided by the
              software drivers).
Solution:     This may happen after changing Flash SW Drivers in existing
              environments. For example an application tries to use a
              functionality which is no longer provided with the new device.
********************************************************************************

Return Name:  Flash_NoInformationAvailable
Description:  The system cannot give any additional information about the error.
Solution:     None
********************************************************************************

Error Name:   Flash_OperationOngoing
Description:  This message is one of two messages that are given by the TimeOut
              subroutine. It means that the ongoing Flash operation is still within
              the defined time frame.
********************************************************************************

Error Name:   Flash_OperationTimeOut
Description:  The Program/Erase Controller algorithm could not finish an
              operation successfully. It should have set bit 7 of the Status
              Register from 0 to 1, but that did not happen within a predetermined
              time. The program execution was therefore cancelled by a
              timeout. This may be because the device is damaged.
Solution:     Try the previous Instruction again. If it fails a second time then it
              is likely that the device will need to be replaced.
********************************************************************************

Error Name:   Flash_ProgramFailed
Description:  The value that should be programmed has not been written correctly
              to the Flash memory.
Solutions:    Make sure that the Page which is supposed to receive the value
              was erased successfully before programming. Try to erase the Page and
              to program the value again. If it fails again then the device may
              be faulty.
********************************************************************************

Error Name:   Flash_WrongType
Description:  This message appears if the Manufacture and Device Identifications read from
              the current Flash device do not match the expected identifier
              codes. This means that the source code is not explicitely written for
              the currently used Flash chip. It may work, but the operation cannot be
              guaranteed.
Solutions:    Use a different Flash chip with the target hardware or contact
              STMicroelectronics for a different source code library.
********************************************************************************

Return Name:  Flash_Success
Description:  This value indicates that the Flash memory Instruction was executed
              correctly.
********************************************************************************/

/******************************************************************************
    External variable declaration
*******************************************************************************/

// none in this version of the release

/*******************************************************************************
Flash Status Register Definitions (see Datasheet)
*******************************************************************************/
enum
{
  SPI_FLASH_SRWD    = 0x80,        /* Status Register Write Protect */
  SPI_FLASH_BP3    = 0x40,        /* Block Protect Bit3 */
  SPI_FLASH_TB      = 0x20,              /* Top/Bottom bit */
  SPI_FLASH_BP2    = 0x10,        /* Block Protect Bit2 */
  SPI_FLASH_BP1    = 0x08,        /* Block Protect Bit1 */
  SPI_FLASH_BP0    = 0x04,        /* Block Protect Bit0 */
  SPI_FLASH_WEL    = 0x02,        /* Write Enable Latch */
  SPI_FLASH_WIP    = 0x01        /* write/Program/Erase in progress bit */
};


/*******************************************************************************
Specific Function Prototypes
*******************************************************************************/
typedef unsigned char BOOL;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

BOOL IsFlashBusy(void);
BOOL IsFlashWELBusy(void);


/*******************************************************************************
List of Specific Errors and Return values, Explanations and Help.
*******************************************************************************

// none in this version of the release
********************************************************************************/

#endif /* __P5Q__H__  */
/* In order to avoid a repeated usage of the header file */

/*******************************************************************************
     End of file
********************************************************************************/

