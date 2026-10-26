/* Adapted for use with IAR Embedded Workbench */
/******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized.

* This software is owned by Renesas Electronics Corporation and is protected under 
* all applicable laws, including copyright laws.

* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES 
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, 
* INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
* PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY 
* DISCLAIMED.

* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES 
* FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS 
* AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

* Renesas reserves the right, without notice, to make changes to this 
* software and to discontinue the availability of this software.  
* By using this software, you agree to the additional terms and 
* conditions found by accessing the following link:
* http://www.renesas.com/disclaimer
******************************************************************************/
/* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.  */
/******************************************************************************
* File Name     : Flash_API_RX600.h
* Version     : 2.0
* Device      : RX600 Series
* Tool Chain   : IAR Embedded Workbench
* H/W Platform : RSKRX62N, RSKRX610, YRDKRX62N
* Description  : Flash programming for the RX600 Group
*******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 21.12.2009 1.00    First Release
*         : 13.01.2010 1.10    Made function names and variables RAPI compliant
*         : 11.02.2010 1.20    Fixed other RAPI issues and fixed I flag issue
*         : 29.04.2010 1.30    Added support for RX621/N Group. Moved most
*                              device specific data to header file.
*         : 26.05.2010 1.40    Added support for RX62T Group
*         : 28.07.2010 1.41    Fixed bug when performing a blank check on an
*                              entire data flash block.  Also declared 
*                              functions not in the API as 'static'.
*      : 23.08.2010 1.42    Updated source to raise the coding standard, to
*                 meet GSCE & RSK standards.
*         : 15.02.2011 1.43    Fixed bug in blank check routine when handling
*                              input arguments and moved _Flash_Init() to
*                              _Enter_PE_Mode() function.
*         : 21.04.2011 2.00    Added BGO capabilities for data flash. Made 
*                              some more changes to fit coding standard. Added
*                              ability to do ROM to ROM or DF to DF transfers.
*                              Added the ability to use the lock bit feature
*                              on ROM blocks.  Added BGO capabilities for
*                              ROM operations.
******************************************************************************/

#ifndef _FLASH_API_H
#define _FLASH_API_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
/* Defines standard typedefs used in this file */
#include <stdint.h>
/* Used for boolean definitions */
#include <stdbool.h>
/* User specific options for Flash API */
#include "r_Flash_API_RX600_UserConfig.h"

/******************************************************************************
DEFINES
******************************************************************************/
/* Pointer definitions for what should be sent in to R_FlashWrite */
#define FLASH_PTR_TYPE uint32_t
#define   BUF_PTR_TYPE uint32_t

/* This catches to make sure the user selected an MCU group */
#if (RX610 + RX62N + RX621 + RX62T) > 1
    #error "ERROR !!! You can only have one RX6xx Device defined \
      at a time in Flash_API_RX600.h !!!"
#endif

/* This catches to make sure the user specified a CPU clock */
#if !defined(ICLK_FREQUENCY)
  #error "ERROR !!! You must specify the System Clock Frequency \
      (ICLK_FREQUENCY) in Flash_API_RX6xx.h first !!!";
#endif

/* This catches to make sure the user specified a peripheral clock */
#if !defined(PCLK_FREQUENCY)
  #error "ERROR !!! You must specify the Peripheral Clock Frequency \
      (PCLK_FREQUENCY) in Flash_API_RX6xx.h first !!!";
#endif

/******************************************************************************
Function Return Values
******************************************************************************/
/* Operation was successful */
#define FLASH_SUCCESS           (0x00)
/* Flash area checked was blank, making this 0x00 as well to keep existing
   code checking compatability */
#define FLASH_BLANK             (0x00)
/* The address that was supplied was not on aligned correctly for ROM or DF */
#define FLASH_ERROR_ALIGNED     (0x01)
/* Flash area checked was not blank, making this 0x01 as well to keep existing
   code checking compatability */
#define FLASH_NOT_BLANK         (0x01)
/* The number of bytes supplied to write was incorrect */
#define FLASH_ERROR_BYTES       (0x02)
/* The address provided is not a valid ROM or DF address */
#define FLASH_ERROR_ADDRESS     (0x03)
/* Writes cannot cross the 1MB boundary on some parts */
#define FLASH_ERROR_BOUNDARY    (0x04)
/* Flash is busy with another operation */
#define FLASH_BUSY              (0x05)
/* Operation failed */
#define FLASH_FAILURE           (0x06)
/* Lock bit was set for the block in question */
#define FLASH_LOCK_BIT_SET      (0x07)
/* Lock bit was not set for the block in question */
#define FLASH_LOCK_BIT_NOT_SET  (0x08)

/******************************************************************************
Function Prototypes
******************************************************************************/

/******************************************************************************
* Function Name: R_FlashErase
* Description  : Erases an entire flash block.
*                NOTE: This function MUST execute from in RAM for 'ROM Area' 
*                programming, but if you are ONLY doing Data Flash programming,
*                this function can reside and execute in Flash.
* Arguments    : block - The block number to erase (BLOCK_0, BLOCK_1, etc...)
* Return Value : FLASH_SUCCESS = Operation Successful
*                FLASH_FAILURE = Operation Failed
*                FLASH_BUSY = Another flash operation is in progress
******************************************************************************/
uint8_t R_FlashErase(   uint8_t block );

/******************************************************************************
* Function Name: R_FlashWrite
* Description  : Writes bytes into flash.
*                NOTE: This function MUST execute from in RAM for 'ROM Area' 
*                programming, but if you are ONLY doing Data Flash programming,
*                this function can reside and execute in Flash.
* Arguments    : flash_addr - Flash address location to write to. This address 
*                             must be on a 256 byte  boundary for ROM or 8-byte
*                             boundary for data flash.
*                buffer_addr - Address location of data buffer to write 
*                              into flash.
*                bytes - The number of bytes to write. You must always pass a 
*                        multiple of 256 bytes for programming ROM and 8 bytes
*                        for programming data flash.
* Return Value : FLASH_SUCCESS = Operation Successful
*                FLASH_FAILURE = Operation Failed
*                FLASH_ERROR_ALIGNED = (ROM) Flash address not on a 256 byte 
*                                      boundary
*                                      (Data Flash) Flash address not on 8 byte 
*                                       boundary
*                FLASH_ERROR_BYTES = (ROM) Number of bytes is not divisible 
*                                    by 256
*                                    (Data Flash) Number of bytes not divisible 
*                                    by 8
*                FLASH_ERROR_ADDRESS = Invalid address
*                FLASH_ERROR_BOUNDARY = (ROM) Cannot write over 1MB boundary at 
*                                       0xFFF00000
*                FLASH_BUSY = Flash is busy with another operation
******************************************************************************/
uint8_t R_FlashWrite(  uint32_t flash_addr,
            uint32_t buffer_addr,
            uint16_t bytes );

/******************************************************************************
* Function Name: R_FlashProgramLockBit
* Description  : Programs the lock bit for a specified ROM erasure block. If
*                the lock bit for a block is set and lock bit protection is 
*                enabled then that block cannot be programmed/erased.
* Arguments    : block - Which ROM erasure block to set the lock bit for
* Return Value : FLASH_SUCCESS = Operation Successful
*                FLASH_FAILURE = Operation Failed
*                FLASH_BUSY = Another flash operation is in progress
******************************************************************************/
uint8_t R_FlashProgramLockBit(uint8_t block);

/******************************************************************************
* Function Name: R_FlashReadLockBit
* Description  : Reads and returns the lock bit status for a ROM block
* Arguments    : block - Which ROM erasure block to read the lock bit of
* Return Value : FLASH_LOCK_BIT_SET = Lock bit was set
*                FLASH_LOCK_BIT_NOT_SET = Lock bit was not set
*                FLASH_FAILURE = Operation Failed
*                FLASH_BUSY = Another flash operation is in progress
******************************************************************************/
uint8_t R_FlashReadLockBit(uint8_t block);

/******************************************************************************
* Function Name: R_FlashSetLockBitProtection
* Description  : Enables or disables lock bit protection.
* Arguments    : lock_bit - Boolean, whether to enable or disable lock bit 
*                           protection (true = enabled, false = disabled)
* Return Value : FLASH_SUCCESS = Operation Successful
*                FLASH_BUSY = Another flash operation is in progress
******************************************************************************/
uint8_t R_FlashSetLockBitProtection(uint8_t lock_bit);
                        
/******************************************************************************
* Function Name: R_FlashGetStatus
* Description  : Returns the current state of the flash
* Arguments    : none
* Return Value : FLASH_SUCCESS = Flash is ready to use
*                FLASH_BUSY = Flash is busy with another operation
******************************************************************************/
uint8_t R_FlashGetStatus(void);                        

/* Data Flash Only Functions */

/******************************************************************************
* Function Name: R_FlashDataAreaAccess
* Description  : This function is used to allow read and program permissions 
*                to the Data Flash areas. NOTE: This function does not have to 
*                execute from in RAM.
* Arguments    : read_en_mask - Bitmasked value. Bits 0-3 represents each Data 
*                               Blocks 0-3 (respectively).
*                               '0'=no Read access. 
*                               '1'=Allows Read by CPU
*                write_en_mask - Bitmasked value. Bits 0-3 represents each Data 
*                               Blocks 0-3 (respectively).
*                               '0'=no Erase/Write access. 
*                               '1'=Allows Erase/Write by FCU
* Return Value : none
******************************************************************************/
void R_FlashDataAreaAccess(uint16_t read_en_mask, uint16_t write_en_mask);

/******************************************************************************
* Function Name: R_FlashDataAreaBlankCheck
* Description  : Performs a blank check on a specified data flash block
*                NOTE: This function does not have to execute from in RAM.
* Arguments    : address - The address to check if is blank.
*                          If the parameter 'size'=='BLANK_CHECK_8_BYTE', 
*                          this should be set to an 8-byte address boundary.
*                          If the parameter 'size'=='BLANK_CHECK_ENTIRE_BLOCK', 
*                          this should be set to a defined Data Block Number 
*                          ('BLOCK_DB0', 'BLOCK_DB1', etc...) or an address 
*                          in the data flash block.  Either option will work.
*                size - This specifies if you are checking an 8-byte location, 
*                       or an entire block. You must set this to either 
*                       'BLANK_CHECK_8_BYTE' or 'BLANK_CHECK_ENTIRE_BLOCK'.
* Return Value : FLASH_BLANK = (8 Byte check or non-BGO) Blank
*                              (Entire Block & BGO) Blank check operation 
*                              started
*                FLASH_NOT_BLANK = Not Blank
*                FLASH_FAILURE = Operation Failed
*                FLASH_BUSY = Another flash operation is in progress
******************************************************************************/
uint8_t R_FlashDataAreaBlankCheck(uint32_t address, uint8_t size);

/* These functions are only used when BGO (non-blocking) mode is enabled */
#if defined(DATA_FLASH_BGO) || defined(ROM_BGO)
/* Callback function to call when flash erase is finished */
void FlashEraseDone(void);
/* Callback function to call when flash write is finished */
void FlashWriteDone(void);
/* Function to take care of flash errors */
void FlashError(void);
/* Callback function to call when flash blank check is finished. 'result' 
   argument is 0 if block was blank and 1 if it was not */ 
void FlashBlankCheckDone(uint8_t result);
#endif


/******************************************************************************
* Definitions - Use 'Configuration Settings' above to select these.
******************************************************************************/
/* 'size' parameter for R_FlashDataAreaBlankCheck */
#define BLANK_CHECK_8_BYTE       0  
/* 'size' parameter for R_FlashDataAreaBlankCheck */
#define BLANK_CHECK_ENTIRE_BLOCK 1  

/* This flag is used for setting/clearing the I bit in the PSW */
#define I_FLAG  (0x00010000)

/* Below is memory information that is specific to device families (RX610,
   RX62N, etc...).  While memory sizes will differ between family members,
   the memory layout should remain the same */
   
/* Do not edit below this line unless you are editting for another RX with */
/* a different memory layout                                               */

#if defined(RX610) || defined(RX621) || defined(RX62N) || defined(RX62T)
    /*  According to HW Manual the Max Programming Time for 256 bytes (ROM)
    is 12ms.  This is with a PCLK of 50MHz. The calculation below
    calculates the number of ICLK ticks needed for the timeout delay.
    The 12ms number is adjusted linearly depending on the PCLK frequency.
  */
    #define  WAIT_MAX_ROM_WRITE \
      ((int32_t)(12000 * (50.0/PCLK_FREQUENCY))*ICLK_FREQUENCY)

    /*  According to HW Manual the Max Programming Time for 128 bytes
    (Data Flash) is 5ms.  This is with a PCLK of 50MHz. The calculation
    below calculates the number of ICLK ticks needed for the timeout delay.
    The 5ms number is adjusted linearly depending on the PCLK frequency.
  */
    #define  WAIT_MAX_DF_WRITE \
      ((int32_t)(5000 * (50.0/PCLK_FREQUENCY))*ICLK_FREQUENCY)

    /*  According to HW Manual the Max Blank Check time for 2k bytes
    (Data Flash) is 0.7ms.  This is with a PCLK of 50MHz. The calculation
    below calculates the number of ICLK ticks needed for the timeout delay.
    The 0.7ms number is adjusted linearly depending on the PCLK frequency.
  */
    #define  WAIT_MAX_BLANK_CHECK \
      ((int32_t)(700 * (50.0/PCLK_FREQUENCY))*ICLK_FREQUENCY)
      
  /*  According to HW Manual the max timeout value when using the peripheral
      clock notification command is 60us. This is with a PCLK of 50MHz. The 
    calculation below calculates the number of ICLK ticks needed for the 
    timeout delay. The 10us number is adjusted linearly depending on 
    the PCLK frequency.
  */
    #define  WAIT_MAX_NOTIFY_PERIPHERAL_CLOCK \
      ((int32_t)(60 * (50.0/PCLK_FREQUENCY))*ICLK_FREQUENCY)      
            
#else
    #error "ERROR !!! Define timeout values for this device \
      in Flash_API_RX600.h !!!"
#endif

#if defined(RX610) || defined(RX621) || defined(RX62N) || defined(RX62T)
    /* FCU-RAM address define */
    /* FCU F/W Store Address */
    #define FCU_PRG_TOP    0xFEFFE000
    /* FCU RAM Address */
    #define FCU_RAM_TOP    0x007F8000
    /* FCU RAM Size */
    #define FCU_RAM_SIZE  0x2000
#else
    #error "ERROR !!! Set memory locations for FCU RAM and ROM \
      in Flash_API_RX600.h !!!"
#endif

/* Memory specifics for the RX610 group */
#if defined(RX610)

    /* User ROM Block Area           Size: Start Addr -   End Addr */
    #define BLOCK_0     0    /*    8KB: 0xFFFFE000 - 0xFFFFFFFF */
    #define BLOCK_1     1    /*    8KB: 0xFFFFC000 - 0xFFFFDFFF */
    #define BLOCK_2     2    /*    8KB: 0xFFFFA000 - 0xFFFFBFFF */
    #define BLOCK_3     3    /*    8KB: 0xFFFF8000 - 0xFFFF9FFF */
    #define BLOCK_4     4    /*    8KB: 0xFFFF6000 - 0xFFFF7FFF */
    #define BLOCK_5     5    /*    8KB: 0xFFFF4000 - 0xFFFF5FFF */
    #define BLOCK_6     6    /*    8KB: 0xFFFF2000 - 0xFFFF3FFF */
    #define BLOCK_7     7    /*    8KB: 0xFFFF0000 - 0xFFFF1FFF */
    #define BLOCK_8     8    /*   64KB: 0xFFFE0000 - 0xFFFEFFFF */
    #define BLOCK_9     9    /*   64KB: 0xFFFD0000 - 0xFFFDFFFF */
    #define BLOCK_10    10    /*   64KB: 0xFFFC0000 - 0xFFFCFFFF */
    #define BLOCK_11    11    /*   64KB: 0xFFFB0000 - 0xFFFBFFFF */
    #define BLOCK_12    12    /*   64KB: 0xFFFA0000 - 0xFFFAFFFF */
    #define BLOCK_13    13    /*   64KB: 0xFFF90000 - 0xFFF9FFFF */
    #define BLOCK_14    14    /*   64KB: 0xFFF80000 - 0xFFF8FFFF */
    #define BLOCK_15    15    /*   64KB: 0xFFF70000 - 0xFFF7FFFF */
    #define BLOCK_16    16    /*   64KB: 0xFFF60000 - 0xFFF6FFFF */
    #define BLOCK_17    17    /*  128KB: 0xFFF40000 - 0xFFF5FFFF */
    #define BLOCK_18    18    /*  128KB: 0xFFF20000 - 0xFFF3FFFF */
    #define BLOCK_19    19    /*  128KB: 0xFFF00000 - 0xFFF1FFFF */
    #define BLOCK_20    20      /*  128KB: 0xFFEE0000 - 0xFFEFFFFF */
    #define BLOCK_21    21      /*  128KB: 0xFFEC0000 - 0xFFEDFFFF */
    #define BLOCK_22    22      /*  128KB: 0xFFEA0000 - 0xFFEBFFFF */
    #define BLOCK_23    23      /*  128KB: 0xFFE80000 - 0xFFE9FFFF */
    #define BLOCK_24    24      /*  128KB: 0xFFE60000 - 0xFFE7FFFF */
    #define BLOCK_25    25      /*  128KB: 0xFFE40000 - 0xFFE5FFFF */
    #define BLOCK_26    26      /*  128KB: 0xFFE20000 - 0xFFE3FFFF */
    #define BLOCK_27    27      /*  128KB: 0xFFE00000 - 0xFFE1FFFF */

    /* Data Flash Block Area         Size: Start Addr -   End Addr */
    #define BLOCK_DB0   28      /*    8KB: 0x00100000 - 0x00101FFF */
    #define BLOCK_DB1   29      /*    8KB: 0x00102000 - 0x00103FFF */
    #define BLOCK_DB2   30      /*    8KB: 0x00104000 - 0x00105FFF */
    #define BLOCK_DB3   31      /*    8KB: 0x00106000 - 0x00107FFF */

    /*  Bottom of DF Area */
    #define DF_ADDRESS      0x00100000
    /* Used for getting DF block */
    #define DF_MASK         0xFFFF6000
    /* Used for getting 8 byte boundary in DF block when
       doing blank checking */
    #define DF_BLOCK_SIZE   0x00002000
    /* Defines how many DF blocks are on this part */
    #define DF_NUM_BLOCKS   4
    /* Defines how many ROM blocks are on this part */
    #define ROM_NUM_BLOCKS  28
    /*  Bottom of User Flash Area */
    #define ROM_PE_ADDR     0x00E00000                                                  

    /* Array of flash addresses used for writing */
    #define DEFINE_ADDR_LOOKUP const uint32_t block_addresses[32] = { \
            /* Caution. ID CODE(FFFFFFA0-FFFFFFAF) is excluded. */ \
      0x00FFE000,  /* EB00 */ \
            0x00FFC000,  /* EB01 */ \
            0x00FFA000,  /* EB02 */ \
            0x00FF8000,  /* EB03 */ \
            0x00FF6000,  /* EB04 */ \
            0x00FF4000,  /* EB05 */ \
            0x00FF2000,  /* EB06 */ \
            0x00FF0000,  /* EB07 */ \
            0x00FE0000,  /* EB08 */ \
            0x00FD0000,  /* EB09 */ \
            0x00FC0000,  /* EB10 */ \
            0x00FB0000,  /* EB11 */ \
            0x00FA0000,  /* EB12 */ \
            0x00F90000,  /* EB13 */ \
            0x00F80000,  /* EB14 */ \
            0x00F70000,  /* EB15 */ \
            0x00F60000,  /* EB16 */ \
            0x00F40000,  /* EB17 */ \
            0x00F20000,  /* EB18 */ \
            0x00F00000,  /* EB19 */ \
            0x00EE0000,  /* EB20 */ \
            0x00EC0000,  /* EB21 */ \
            0x00EA0000,  /* EB22 */ \
            0x00E80000,  /* EB23 */ \
            0x00E60000,  /* EB24 */ \
            0x00E40000,  /* EB25 */ \
            0x00E20000,  /* EB26 */ \
            0x00E00000,  /* EB27 */ \
            0x00100000,  /* DB00 */ \
            0x00102000,  /* DB01 */ \
            0x00104000,  /* DB02 */ \
            0x00106000   /* DB03 */ \
    };
   

    /* The max User ROM size on the RX610 is 2MB */
    #define ROM_SIZE            2097152
    /* The Data Flash size on the RX610 is 32kB */
    #define DF_SIZE             32768

    /*  According to HW Manual the Max Erasure Time for a 128kB block
    is 1750ms.  This is with a PCLK of 50MHz. The calculation below
    calculates the number of ICLK ticks needed for the timeout delay.
    The 1750ms number is adjusted linearly depending on the PCLK frequency.
  */
    #define WAIT_MAX_ERASE \
      ((int32_t)(1750000 * (50.0/PCLK_FREQUENCY))*ICLK_FREQUENCY)   

/* Memory specifics for the RX621/N group */
#elif defined(RX62N) || defined(RX621)

    /* User ROM Block Area           Size: Start Addr -   End Addr */
    #define BLOCK_0     0    /*    4KB: 0xFFFFF000 - 0xFFFFFFFF */
    #define BLOCK_1     1    /*    4KB: 0xFFFFE000 - 0xFFFFEFFF */
    #define BLOCK_2     2    /*    4KB: 0xFFFFD000 - 0xFFFFDFFF */
    #define BLOCK_3     3    /*    4KB: 0xFFFFC000 - 0xFFFFCFFF */
    #define BLOCK_4     4    /*    4KB: 0xFFFFB000 - 0xFFFFBFFF */
    #define BLOCK_5     5    /*    4KB: 0xFFFFA000 - 0xFFFFAFFF */
    #define BLOCK_6     6    /*    4KB: 0xFFFF9000 - 0xFFFF9FFF */
    #define BLOCK_7     7    /*    4KB: 0xFFFF8000 - 0xFFFF8FFF */
    #define BLOCK_8     8    /*   16KB: 0xFFFF4000 - 0xFFFF7FFF */
    #define BLOCK_9     9    /*   16KB: 0xFFFF0000 - 0xFFFF3FFF */
    #define BLOCK_10    10    /*   16KB: 0xFFFEC000 - 0xFFFEFFFF */
    #define BLOCK_11    11    /*   16KB: 0xFFFE8000 - 0xFFFEBFFF */
    #define BLOCK_12    12    /*   16KB: 0xFFFE4000 - 0xFFFE7FFF */
    #define BLOCK_13    13    /*   16KB: 0xFFFE0000 - 0xFFFE3FFF */
    #define BLOCK_14    14    /*   16KB: 0xFFFDC000 - 0xFFFDFFFF */
    #define BLOCK_15    15    /*   16KB: 0xFFFD8000 - 0xFFFDBFFF */
    #define BLOCK_16    16    /*   16KB: 0xFFFD4000 - 0xFFFD7FFF */
    #define BLOCK_17    17    /*   16KB: 0xFFFD0000 - 0xFFFD3FFF */
    #define BLOCK_18    18    /*   16KB: 0xFFFCC000 - 0xFFFCFFFF */
    #define BLOCK_19    19    /*   16KB: 0xFFFC8000 - 0xFFFCBFFF */
    #define BLOCK_20    20    /*   16KB: 0xFFFC4000 - 0xFFFC7FFF */
    #define BLOCK_21    21    /*   16KB: 0xFFFC0000 - 0xFFFC3FFF */
    #define BLOCK_22    22    /*   16KB: 0xFFFBC000 - 0xFFFBFFFF */
    #define BLOCK_23    23    /*   16KB: 0xFFFB8000 - 0xFFFBBFFF */
    #define BLOCK_24    24    /*   16KB: 0xFFFB4000 - 0xFFFB7FFF */
    #define BLOCK_25    25    /*   16KB: 0xFFFB0000 - 0xFFFB3FFF */
    #define BLOCK_26    26    /*   16KB: 0xFFFAC000 - 0xFFFAFFFF */
    #define BLOCK_27    27    /*   16KB: 0xFFFA8000 - 0xFFFABFFF */
    #define BLOCK_28    28    /*   16KB: 0xFFFA4000 - 0xFFFA7FFF */
    #define BLOCK_29    29    /*   16KB: 0xFFFA0000 - 0xFFFA3FFF */
    #define BLOCK_30    30    /*   16KB: 0xFFF9C000 - 0xFFF9FFFF */
    #define BLOCK_31    31    /*   16KB: 0xFFF98000 - 0xFFF9BFFF */
    #define BLOCK_32    32    /*   16KB: 0xFFF94000 - 0xFFF97FFF */
    #define BLOCK_33    33    /*   16KB: 0xFFF90000 - 0xFFF93FFF */
    #define BLOCK_34    34    /*   16KB: 0xFFF8C000 - 0xFFF8FFFF */
    #define BLOCK_35    35    /*   16KB: 0xFFF88000 - 0xFFF8BFFF */
    #define BLOCK_36    36    /*   16KB: 0xFFF84000 - 0xFFF87FFF */
    #define BLOCK_37    37    /*   16KB: 0xFFF80000 - 0xFFF83FFF */

    /* Data Flash Block Area         Size: Start Addr -   End Addr */
    #define BLOCK_DB0    38      /*    2KB: 0x00100000 - 0x001007FF */
    #define BLOCK_DB1    39      /*    2KB: 0x00100800 - 0x00100FFF */
    #define BLOCK_DB2    40      /*    2KB: 0x00101000 - 0x001017FF */
    #define BLOCK_DB3    41      /*    2KB: 0x00101800 - 0x00101FFF */
    #define BLOCK_DB4    42      /*    2KB: 0x00102000 - 0x001027FF */
    #define BLOCK_DB5    43      /*    2KB: 0x00102800 - 0x00102FFF */
    #define BLOCK_DB6    44      /*    2KB: 0x00103000 - 0x001037FF */
    #define BLOCK_DB7    45      /*    2KB: 0x00103800 - 0x00103FFF */
    #define BLOCK_DB8    46      /*    2KB: 0x00104000 - 0x001047FF */
    #define BLOCK_DB9    47      /*    2KB: 0x00104800 - 0x00104FFF */
    #define BLOCK_DB10   48      /*    2KB: 0x00105000 - 0x001057FF */
    #define BLOCK_DB11   49      /*    2KB: 0x00105800 - 0x00105FFF */
    #define BLOCK_DB12   50      /*    2KB: 0x00106000 - 0x001067FF */
    #define BLOCK_DB13   51      /*    2KB: 0x00106800 - 0x00106FFF */
    #define BLOCK_DB14   52      /*    2KB: 0x00107000 - 0x001077FF */
    #define BLOCK_DB15   53      /*    2KB: 0x00107800 - 0x00107FFF */

    /*  Bottom of DF Area */
    #define DF_ADDRESS      0x00100000
    /* Used for getting DF block */
    #define DF_MASK         0xFFFFF800
    /* Used for getting 8 byte boundary in DF block when
       doing blank checking */
    #define DF_BLOCK_SIZE   0x00000800
    /* Defines how many DF blocks are on this part */
    #define DF_NUM_BLOCKS   16
    /* Defines how many ROM blocks are on this part */
    #define ROM_NUM_BLOCKS  38
    /*  Bottom of User Flash Area */
    #define ROM_PE_ADDR     0x00F80000                                                  

    /* Array of flash addresses used for writing */
    #define DEFINE_ADDR_LOOKUP const uint32_t block_addresses[54] = { \
            /* Caution. ID CODE(FFFFFFA0-FFFFFFAF) is excluded. */ \
      0x00FFF000,  /* EB00 */ \
            0x00FFE000,  /* EB01 */ \
            0x00FFD000,  /* EB02 */ \
            0x00FFC000,  /* EB03 */ \
            0x00FFB000,  /* EB04 */ \
            0x00FFA000,  /* EB05 */ \
            0x00FF9000,  /* EB06 */ \
            0x00FF8000,  /* EB07 */ \
            0x00FF4000,  /* EB08 */ \
            0x00FF0000,  /* EB09 */ \
            0x00FEC000,  /* EB10 */ \
            0x00FE8000,  /* EB11 */ \
            0x00FE4000,  /* EB12 */ \
            0x00FE0000,  /* EB13 */ \
            0x00FDC000,  /* EB14 */ \
            0x00FD8000,  /* EB15 */ \
            0x00FD4000,  /* EB16 */ \
            0x00FD0000,  /* EB17 */ \
            0x00FCC000,  /* EB18 */ \
            0x00FC8000,  /* EB19 */ \
            0x00FC4000,  /* EB20 */ \
            0x00FC0000,  /* EB21 */ \
            0x00FBC000,  /* EB22 */ \
            0x00FB8000,  /* EB23 */ \
            0x00FB4000,  /* EB24 */ \
            0x00FB0000,  /* EB25 */ \
            0x00FAC000,  /* EB26 */ \
            0x00FA8000,  /* EB27 */ \
            0x00FA4000,  /* EB28 */ \
            0x00FA0000,  /* EB29 */ \
            0x00F9C000,  /* EB30 */ \
            0x00F98000,  /* EB31 */ \
            0x00F94000,  /* EB32 */ \
            0x00F90000,  /* EB33 */ \
            0x00F8C000,  /* EB34 */ \
            0x00F88000,  /* EB35 */ \
            0x00F84000,  /* EB36 */ \
            0x00F80000,  /* EB37 */ \
            0x00100000,  /* DB00 */ \
            0x00100800,  /* DB01 */ \
            0x00101000,  /* DB02 */ \
            0x00101800,  /* DB03 */ \
            0x00102000,  /* DB04 */ \
            0x00102800,  /* DB05 */ \
            0x00103000,  /* DB06 */ \
            0x00103800,  /* DB07 */ \
            0x00104000,  /* DB08 */ \
            0x00104800,  /* DB09 */ \
            0x00105000,  /* DB10 */ \
            0x00105800,  /* DB11 */ \
            0x00106000,  /* DB12 */ \
            0x00106800,  /* DB13 */ \
            0x00107000,  /* DB14 */ \
            0x00107800}; /* DB15 */       

    /* The max User ROM size on the RX621/N is 512KB */
    #define ROM_SIZE            524288
    /* The Data Flash size on the RX621/N is 32KB */
    #define DF_SIZE             32768

    /*  According to HW Manual the Max Erasure Time for a 16kB block is
    around 300ms.  This is with a PCLK of 50MHz. The calculation below
    calculates the number of ICLK ticks needed for the timeout delay.
    The 300ms number is adjusted linearly depending on the PCLK frequency.
  */
    #define WAIT_MAX_ERASE \
      ((int32_t)(300000 * (50.0/PCLK_FREQUENCY))*ICLK_FREQUENCY)   
    
/* Memory specifics for the RX62T group */
#elif defined(RX62T)

    /* User ROM Block Area           Size: Start Addr -   End Addr */
    #define BLOCK_0     0    /*    4KB: 0xFFFFF000 - 0xFFFFFFFF */
    #define BLOCK_1     1    /*    4KB: 0xFFFFE000 - 0xFFFFEFFF */
    #define BLOCK_2     2    /*    4KB: 0xFFFFD000 - 0xFFFFDFFF */
    #define BLOCK_3     3    /*    4KB: 0xFFFFC000 - 0xFFFFCFFF */
    #define BLOCK_4     4    /*    4KB: 0xFFFFB000 - 0xFFFFBFFF */
    #define BLOCK_5     5    /*    4KB: 0xFFFFA000 - 0xFFFFAFFF */
    #define BLOCK_6     6    /*    4KB: 0xFFFF9000 - 0xFFFF9FFF */
    #define BLOCK_7     7    /*    4KB: 0xFFFF8000 - 0xFFFF8FFF */
    #define BLOCK_8     8    /*   16KB: 0xFFFF4000 - 0xFFFF7FFF */
    #define BLOCK_9     9    /*   16KB: 0xFFFF0000 - 0xFFFF3FFF */
    #define BLOCK_10    10    /*   16KB: 0xFFFEC000 - 0xFFFEFFFF */
    #define BLOCK_11    11    /*   16KB: 0xFFFE8000 - 0xFFFEBFFF */
    #define BLOCK_12    12    /*   16KB: 0xFFFE4000 - 0xFFFE7FFF */
    #define BLOCK_13    13    /*   16KB: 0xFFFE0000 - 0xFFFE3FFF */
    #define BLOCK_14    14    /*   16KB: 0xFFFDC000 - 0xFFFDFFFF */
    #define BLOCK_15    15    /*   16KB: 0xFFFD8000 - 0xFFFDBFFF */
    #define BLOCK_16    16    /*   16KB: 0xFFFD4000 - 0xFFFD7FFF */
    #define BLOCK_17    17    /*   16KB: 0xFFFD0000 - 0xFFFD3FFF */
    #define BLOCK_18    18    /*   16KB: 0xFFFCC000 - 0xFFFCFFFF */
    #define BLOCK_19    19    /*   16KB: 0xFFFC8000 - 0xFFFCBFFF */
    #define BLOCK_20    20    /*   16KB: 0xFFFC4000 - 0xFFFC7FFF */
    #define BLOCK_21    21    /*   16KB: 0xFFFC0000 - 0xFFFC3FFF */

    /* Data Flash Block Area         Size: Start Addr -   End Addr */
    #define BLOCK_DB0    22      /*    2KB: 0x00100000 - 0x001007FF */
    #define BLOCK_DB1    23      /*    2KB: 0x00100800 - 0x00100FFF */
    #define BLOCK_DB2    24      /*    2KB: 0x00101000 - 0x001017FF */
    #define BLOCK_DB3    25      /*    2KB: 0x00101800 - 0x00101FFF */
    #define BLOCK_DB4    26      /*    2KB: 0x00102000 - 0x001027FF */
    #define BLOCK_DB5    27      /*    2KB: 0x00102800 - 0x00102FFF */
    #define BLOCK_DB6    28     /*    2KB: 0x00103000 - 0x001037FF */
    #define BLOCK_DB7    29      /*    2KB: 0x00103800 - 0x00103FFF */
    #define BLOCK_DB8    30      /*    2KB: 0x00104000 - 0x001047FF */
    #define BLOCK_DB9    31      /*    2KB: 0x00104800 - 0x00104FFF */
    #define BLOCK_DB10   32      /*    2KB: 0x00105000 - 0x001057FF */
    #define BLOCK_DB11   33      /*    2KB: 0x00105800 - 0x00105FFF */
    #define BLOCK_DB12   34      /*    2KB: 0x00106000 - 0x001067FF */
    #define BLOCK_DB13   35      /*    2KB: 0x00106800 - 0x00106FFF */
    #define BLOCK_DB14   36      /*    2KB: 0x00107000 - 0x001077FF */
    #define BLOCK_DB15   37      /*    2KB: 0x00107800 - 0x00107FFF */

    /*  Bottom of DF Area */
    #define DF_ADDRESS      0x00100000
    /* Used for getting DF block */
    #define DF_MASK         0xFFFFF800
    /* Used for getting 8 byte boundary in DF block when
       doing blank checking */
    #define DF_BLOCK_SIZE   0x00000800
    /* Defines how many DF blocks are on this part */
    #define DF_NUM_BLOCKS   16
    /* Defines how many ROM blocks are on this part */
    #define ROM_NUM_BLOCKS  22
    /*  Bottom of User Flash Area */
    #define ROM_PE_ADDR     0x00FC0000                                                  

    /* Array of flash addresses used for writing */
    #define DEFINE_ADDR_LOOKUP const uint32_t block_addresses[54] = { \
            /* Caution. ID CODE(FFFFFFA0-FFFFFFAF) is excluded. */ \
      0x00FFF000,  /* EB00 */ \
            0x00FFE000,  /* EB01 */ \
            0x00FFD000,  /* EB02 */ \
            0x00FFC000,  /* EB03 */ \
            0x00FFB000,  /* EB04 */ \
            0x00FFA000,  /* EB05 */ \
            0x00FF9000,  /* EB06 */ \
            0x00FF8000,  /* EB07 */ \
            0x00FF4000,  /* EB08 */ \
            0x00FF0000,  /* EB09 */ \
            0x00FEC000,  /* EB10 */ \
            0x00FE8000,  /* EB11 */ \
            0x00FE4000,  /* EB12 */ \
            0x00FE0000,  /* EB13 */ \
            0x00FDC000,  /* EB14 */ \
            0x00FD8000,  /* EB15 */ \
            0x00FD4000,  /* EB16 */ \
            0x00FD0000,  /* EB17 */ \
            0x00FCC000,  /* EB18 */ \
            0x00FC8000,  /* EB19 */ \
            0x00FC4000,  /* EB20 */ \
            0x00FC0000,  /* EB21 */ \
            0x00100000,  /* DB00 */ \
            0x00100800,  /* DB01 */ \
            0x00101000,  /* DB02 */ \
            0x00101800,  /* DB03 */ \
            0x00102000,  /* DB04 */ \
            0x00102800,  /* DB05 */ \
            0x00103000,  /* DB06 */ \
            0x00103800,  /* DB07 */ \
            0x00104000,  /* DB08 */ \
            0x00104800,  /* DB09 */ \
            0x00105000,  /* DB10 */ \
            0x00105800,  /* DB11 */ \
            0x00106000,  /* DB12 */ \
            0x00106800,  /* DB13 */ \
            0x00107000,  /* DB14 */ \
            0x00107800}; /* DB15 */       

    /* The max User ROM size on the RX62T is 256KB */
    #define ROM_SIZE            262144
    /* The Data Flash size on the RX62T is 32KB man (8KB min) */
    #define DF_SIZE             32768

    /*  According to HW Manual the Max Erasure Time for a 16kB block is
    around 300ms.  This is with a PCLK of 50MHz. The calculation below
    calculates the number of ICLK ticks needed for the timeout delay.
    The 300ms number is adjusted linearly depending on the PCLK frequency.
  */
    #define WAIT_MAX_ERASE \
      ((int32_t)(300000 * (50.0/PCLK_FREQUENCY))*ICLK_FREQUENCY)   

#else
    #error "!!! Need to define memory specifics for this RX600 family \
      in Flash_API_RX600.h !!!"
#endif


#endif
