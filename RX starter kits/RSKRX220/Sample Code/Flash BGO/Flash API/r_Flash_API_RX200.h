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
/* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.  */
/******************************************************************************
* File Name    : r_Flash_API_RX200.h
* Version      : 1.00
* Device       : R5F52206
* Tool Chain   : RX Family C Compiler 1.2.1.0
* H/W Platform : RSKRX220
* Description  : Flash programming for the RX200 Group
*******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 5.10.2011 1.00    First Release. Evolution from RX600 v2.1 Flash API
*         : 8.15.2012 1.01    Modified R_FlashErase to use a uint16_t argument
*                             instead of uint8_t
*                             Fixed a bug that created 16KB Code Flash blocks instead
*                             of 2K causing only 2K to be erased.
*                             Added check to make sure FCLK is wsithin 4-32 MHz
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
#include "r_Flash_API_RX200_UserConfig.h"

/******************************************************************************
DEFINES
******************************************************************************/
/* Pointer definitions for what should be sent in to R_FlashWrite */
#define FLASH_PTR_TYPE uint32_t
#define   BUF_PTR_TYPE uint32_t

/* This catches to make sure the user selected an MCU group */
#if (RX210) > 1
    #error "ERROR !!! You can only have one RX2xx Device defined \
            at a time in r_Flash_API_RX200_UserConfig.h !!!"
#endif

/* This catches to make sure the user specified a CPU clock */
#if !defined(ICLK_FREQUENCY)
    #error "ERROR !!! You must specify the System Clock Frequency \
          (ICLK_FREQUENCY) in r_Flash_API_RX200_UserConfig.h first !!!";
#endif

/* This catches to make sure the user specified the Flash Clock value*/
#if !defined(FLASH_CLOCK_FREQUENCY)
    #error "ERROR !!! You must specify the clock frequency supplied to the \
           flash (FLASH_CLOCK_FREQUENCY) in r_Flash_API_RX200_UserConfig.h \
           first !!!";
#endif
/* This catches to make sure the user specified a valid value to (Flash Clock)FCLK */
#if (((FLASH_CLOCK_FREQUENCY) > 32) || ((FLASH_CLOCK_FREQUENCY) < 4))
    #error ""ERROR !!! (FLASH_CLOCK_FREQUENCY) in r_Flash_API_RX200_UserConfig.h\
             must be defined to a value between 4 and 32";
#endif
/* This checks to make sure an IPL for the flash ready interrupt was defined */
#if (defined(ROM_BGO) || defined(DATA_FLASH_BGO)) && !defined(FLASH_READY_IPL)
    #error "ERROR !!! You must specify an IPL for the flash ready interrupt
            in r_Flash_API_RX200_UserConfig.h when using BGO.
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
uint8_t R_FlashErase(   uint16_t block );


#if defined(RX210)
/******************************************************************************
* Function Name: R_FlashEraseRange
* Description  : Erases blocks starting at an address and stopping when the
*                number of bytes to erase has been reached.
*                NOTE: This function is currently only for data flash blocks
*                on RX MCUs that have 32 byte erase sectors.
*                NOTE: This function MUST execute from in RAM for 'ROM Area' 
*                programming, but if you are ONLY doing Data Flash programming,
*                this function can reside and execute in Flash.
* Arguments    : start_addr - The address of where to start erasing. Must be on
*                             erase boundary.
*                bytes - The numbers of bytes to erase. Must be 
*                        multiple of erase size.
* Return Value : FLASH_SUCCESS = Operation Successful
*                FLASH_FAILURE = Operation Failed
*                FLASH_ERROR_ALIGNED = Flash address was not on correct boundary
*                FLASH_BUSY = Another flash operation is in progress
*                FLASH_ERROR_BYTES = Number of bytes did not match erase size
*                FLASH_ERROR_ADDRESS = Invalid address, this is only for DF
******************************************************************************/
uint8_t R_FlashEraseRange(uint32_t start_addr, uint32_t bytes);
#endif

/******************************************************************************
* Function Name: R_FlashWrite
* Description  : Writes bytes into flash.
*                NOTE: This function MUST execute from in RAM for 'ROM Area' 
*                programming, but if you are ONLY doing Data Flash programming,
*                this function can reside and execute in Flash.
* Arguments    : flash_addr - Flash address location to write to. This address 
*                             must be on a program boundary (e.g. RX210 has 
*                             2, 8 or 128-byte ROM writes and 2 or 8-byte DF writes).
*                buffer_addr - Address location of data buffer to write 
*                              into flash.
*                bytes - The number of bytes to write. You must always pass a 
*                        multiple of the programming size (e.g. RX210 has 
*                        options for 128, 8 or 2-byte ROM writes and 8 or 2-byte DF writes).

* Return Value : FLASH_SUCCESS = Operation Successful
*                FLASH_FAILURE = Operation Failed
*                FLASH_ERROR_ALIGNED = Flash address was not on correct boundary
*                FLASH_ERROR_BYTES = Number of bytes did not match programming
*                                    size of ROM or DF.
*                FLASH_ERROR_ADDRESS = Invalid address
*                FLASH_ERROR_BOUNDARY = (ROM) Cannot write across flash areas.                                       
*                FLASH_BUSY = Flash is busy with another operation
******************************************************************************/
uint8_t R_FlashWrite(   uint32_t flash_addr,
                        uint32_t buffer_addr,
                        uint16_t bytes
                        );

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
*                NOTE1: This function does not have to execute from in RAM.
*                NOTE2: RX210 support block and 2 byte checks
* Arguments    : address - The address to check if is blank.
*                          If the parameter 'size'=='BLANK_CHECK_ENTIRE_BLOCK', 
*                          this should be set to a defined Data Block Number 
*                          ('BLOCK_DB0', 'BLOCK_DB1', etc...) or an address 
*                          in the data flash block.  Either option will work.
*                          If the parameter 'size'=='BLANK_CHECK_2_BYTE',
*                          this should be set to a 2 byte address boundary.
*                size - This specifies if you are checking an 8-byte location,
*                       2-byte location, or an entire block. You must set this 
*                       to either 'BLANK_CHECK_2_BYTE', 
*                       or 'BLANK_CHECK_ENTIRE_BLOCK'.
* Return Value : FLASH_BLANK = (2 or 8 Byte check or non-BGO) Blank
*                              (Entire Block & BGO) Blank check operation 
*                              started
*                FLASH_NOT_BLANK = Not Blank
*                FLASH_FAILURE = Operation Failed
*                FLASH_BUSY = Another flash operation is in progress
*                FLASH_ERROR_ADDRESS = Invalid address
*                FLASH_ERROR_BYTES = Incorrect 'size' was submitted
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
#if defined(RX210)
    /* 'size' parameter for R_FlashDataAreaBlankCheck */
    #define BLANK_CHECK_2_BYTE       2
#endif
/* 'size' parameter for R_FlashDataAreaBlankCheck */
#define BLANK_CHECK_ENTIRE_BLOCK 1  

/* This flag is used for setting/clearing the I bit in the PSW */
#define I_FLAG  (0x00010000)

/* Below is memory information that is specific to device families (RX210)
  While memory sizes will differ between family members,
   the memory layout should remain the same */
   
/* Do not edit below this line unless you are editting for another RX with */
/* a different memory layout                                               */

#if defined(RX210)

    /*  According to HW Manual the Max Programming Time for 2 or 8 bytes (ROM)
        is 2.5ms.  This is with a FCLK of 50MHz. The calculation below
        calculates the number of ICLK ticks needed for the timeout delay.
        The 12ms number is adjusted linearly depending on the FCLK frequency.
    */
    #define WAIT_MAX_ROM_WRITE_2_8 \
            ((int32_t)(2500 * (50.0/FLASH_CLOCK_FREQUENCY))*ICLK_FREQUENCY)

    /*  According to HW Manual the Max Programming Time for 128 bytes (ROM)
        is 5ms.  This is with a FCLK of 50MHz. The calculation below
        calculates the number of ICLK ticks needed for the timeout delay.
        The 12ms number is adjusted linearly depending on the FCLK frequency.
    */
    #define WAIT_MAX_ROM_WRITE_128 \
            ((int32_t)(5000 * (50.0/FLASH_CLOCK_FREQUENCY))*ICLK_FREQUENCY)
    
    /*  According to HW Manual the Max Programming Time for 2 bytes
        (Data Flash) is 2ms.  This is with a FCLK of 50MHz. The calculation
        below calculates the number of ICLK ticks needed for the timeout delay.
        The 5ms number is adjusted linearly depending on the FCLK frequency.
    */
    #define WAIT_MAX_DF_WRITE \
            ((int32_t)(2000 * (50.0/FLASH_CLOCK_FREQUENCY))*ICLK_FREQUENCY)

    /*  According to HW Manual the Max Blank Check time for 2k bytes
        (Data Flash) is 0.7ms.  This is with a FCLK of 50MHz. The calculation
        below calculates the number of ICLK ticks needed for the timeout delay.
        The 0.7ms number is adjusted linearly depending on the FCLK frequency.
    */
    #define WAIT_MAX_BLANK_CHECK \
            ((int32_t)(700 * (50.0/FLASH_CLOCK_FREQUENCY))*ICLK_FREQUENCY)
            
    /*  According to HW Manual the max timeout value when using the peripheral
        clock notification command is 60us. This is with a FCLK of 50MHz. The 
        calculation below calculates the number of ICLK ticks needed for the 
        timeout delay. The 10us number is adjusted linearly depending on 
        the FCLK frequency.
    */
    #define WAIT_MAX_NOTIFY_PERIPHERAL_CLOCK \
            ((int32_t)(60 * (50.0/FLASH_CLOCK_FREQUENCY))*ICLK_FREQUENCY)           
            
#else
    #error "ERROR !!! Define timeout values for this device \
            in r_Flash_API_RX200.h !!!"
#endif

#if defined(RX210)
    /* FCU-RAM address define */
    /* FCU F/W Store Address */
    #define FCU_PRG_TOP     0xFEFFE000
    /* FCU RAM Address */
    #define FCU_RAM_TOP     0x007F8000
    /* FCU RAM Size */
    #define FCU_RAM_SIZE    0x2000
#else
    #error "ERROR !!! Set memory locations for FCU RAM and ROM \
            in r_Flash_API_RX200.h !!!"
#endif

/* Memory specifics for the RX210 group */
#if defined(RX210)
           
    /* Defines the number of flash areas */
    #define NUM_ROM_AREAS       1
    /* Defines the start program/erase address for the different flash areas */
    #define ROM_AREA_0          (0x00F80000)
   
       
    /* User ROM Block Area           Size: Start Addr -   End Addr      */
    #define BLOCK_0     0       /*   2KB: 0xFFFFC000 - 0xFFFFFFFF       */
    #define BLOCK_1     1       /*   2KB: 0xFFFF8000 - 0xFFFFBFFF       */
    #define BLOCK_2     2       /*   2KB: 0xFFFF4000 - 0xFFFF7FFF       */
    #define BLOCK_3     3       /*   2KB: 0xFFFF0000 - 0xFFFF3FFF       */
    
    #define BLOCK_4     4       /*   2KB: 0xFFFEC000 - 0xFFFEFFFF       */
    #define BLOCK_5     5       /*   2KB: 0xFFFE8000 - 0xFFFEBFFF       */
    #define BLOCK_6     6       /*   2KB: 0xFFFE4000 - 0xFFFE7FFF       */
    #define BLOCK_7     7       /*   2KB: 0xFFFE0000 - 0xFFFE3FFF       */
    
    #define BLOCK_8     8       /*   2KB: 0xFFFDC000 - 0xFFFDFFFF       */
    #define BLOCK_9     9       /*   2KB: 0xFFFD8000 - 0xFFFDBFFF       */
    #define BLOCK_10    10      /*   2KB: 0xFFFD4000 - 0xFFFD7FFF       */
    #define BLOCK_11    11      /*   2KB: 0xFFFD0000 - 0xFFFD3FFF       */
    
    #define BLOCK_12    12      /*   2KB: 0xFFFCC000 - 0xFFFCFFFF       */
    #define BLOCK_13    13      /*   2KB: 0xFFFC8000 - 0xFFFCBFFF       */
    #define BLOCK_14    14      /*   2KB: 0xFFFC4000 - 0xFFFC7FFF       */
    #define BLOCK_15    15      /*   2KB: 0xFFFC0000 - 0xFFFC3FFF       */
    
    #define BLOCK_16    16      /*   2KB: 0xFFFBC000 - 0xFFFBFFFF       */
    #define BLOCK_17    17      /*   2KB: 0xFFFB8000 - 0xFFFBBFFF       */
    #define BLOCK_18    18      /*   2KB: 0xFFFB4000 - 0xFFFB7FFF       */
    #define BLOCK_19    19      /*   2KB: 0xFFFB0000 - 0xFFFB3FFF       */
    
    #define BLOCK_20    20      /*   2KB: 0xFFFAC000 - 0xFFFAFFFF       */
    #define BLOCK_21    21      /*   2KB: 0xFFFA8000 - 0xFFFABFFF       */
    #define BLOCK_22    22      /*   2KB: 0xFFFA4000 - 0xFFFA7FFF       */
    #define BLOCK_23    23      /*   2KB: 0xFFFA0000 - 0xFFFA3FFF       */
    
    #define BLOCK_24    24      /*   2KB: 0xFFF9C000 - 0xFFF9FFFF       */
    #define BLOCK_25    25      /*   2KB: 0xFFF98000 - 0xFFF9BFFF       */
    #define BLOCK_26    26      /*   2KB: 0xFFF94000 - 0xFFF97FFF       */
    #define BLOCK_27    27      /*   2KB: 0xFFF90000 - 0xFFF93FFF       */
    
    #define BLOCK_28    28      /*   2KB: 0xFFF8C000 - 0xFFF8FFFF       */
    #define BLOCK_29    29      /*   2KB: 0xFFF88000 - 0xFFF8BFFF       */
    #define BLOCK_30    30      /*   2KB: 0xFFF84000 - 0xFFF87FFF       */
    #define BLOCK_31    31      /*   2KB: 0xFFF80000 - 0xFFF83FFF       */
    
    #define BLOCK_32    32      
    #define BLOCK_33    33      
    #define BLOCK_34    34      
    #define BLOCK_35    35
    
    #define BLOCK_36    36      
    #define BLOCK_37    37      
    #define BLOCK_38    38      
    #define BLOCK_39    39
    
    #define BLOCK_40    40      
    #define BLOCK_41    41      
    #define BLOCK_42    42      
    #define BLOCK_43    43

    #define BLOCK_44    44      
    #define BLOCK_45    45      
    #define BLOCK_46    46      
    #define BLOCK_47    47
    
    #define BLOCK_48    48      
    #define BLOCK_49    49      
    #define BLOCK_50    50      
    #define BLOCK_51    51
    
    #define BLOCK_52    52      
    #define BLOCK_53    53      
    #define BLOCK_54    54      
    #define BLOCK_55    55
    
    #define BLOCK_56    56      
    #define BLOCK_57    57      
    #define BLOCK_58    58      
    #define BLOCK_59    59
    
    #define BLOCK_60    60      
    #define BLOCK_61    61      
    #define BLOCK_62    62      
    #define BLOCK_63    63
    
    #define BLOCK_64    64      
    #define BLOCK_65    65      
    #define BLOCK_66    66      
    #define BLOCK_67    67
    
    #define BLOCK_68    68      
    #define BLOCK_69    69      
    #define BLOCK_70    70      
    #define BLOCK_71    71
    
    #define BLOCK_72    72      
    #define BLOCK_73    73      
    #define BLOCK_74    74      
    #define BLOCK_75    75
    
    #define BLOCK_76    76      
    #define BLOCK_77    77      
    #define BLOCK_78    78      
    #define BLOCK_79    79
    
    #define BLOCK_80    80      
    #define BLOCK_81    81      
    #define BLOCK_82    82      
    #define BLOCK_83    83
    
    #define BLOCK_84    84      
    #define BLOCK_85    85      
    #define BLOCK_86    86      
    #define BLOCK_87    87
    
    #define BLOCK_88    88      
    #define BLOCK_89    89      
    #define BLOCK_90    90      
    #define BLOCK_91    91
    
    #define BLOCK_92    92      
    #define BLOCK_93    93      
    #define BLOCK_94    94      
    #define BLOCK_95    95
    
    #define BLOCK_96    96      
    #define BLOCK_97    97      
    #define BLOCK_98    98      
    #define BLOCK_99    99
    
    #define BLOCK_100    100        
    #define BLOCK_101    101        
    #define BLOCK_102    102        
    #define BLOCK_103    103
    
    #define BLOCK_104    104        
    #define BLOCK_105    105        
    #define BLOCK_106    106        
    #define BLOCK_107    107
    
    #define BLOCK_108    108        
    #define BLOCK_109    109        
    #define BLOCK_110    110        
    #define BLOCK_111    111
    
    #define BLOCK_112    112        
    #define BLOCK_113    113        
    #define BLOCK_114    114        
    #define BLOCK_115    115
    
    #define BLOCK_116    116        
    #define BLOCK_117    117        
    #define BLOCK_118    118        
    #define BLOCK_119    119
    
    #define BLOCK_120    120        
    #define BLOCK_121    121        
    #define BLOCK_122    122        
    #define BLOCK_123    123
    
    #define BLOCK_124    124        
    #define BLOCK_125    125        
    #define BLOCK_126    126        
    #define BLOCK_127    127
    
    #define BLOCK_128    128        
    #define BLOCK_129    129        
    #define BLOCK_130    130        
    #define BLOCK_131    131
    
    #define BLOCK_132    132        
    #define BLOCK_133    133        
    #define BLOCK_134    134        
    #define BLOCK_135    135
    
    #define BLOCK_136    136        
    #define BLOCK_137    137        
    #define BLOCK_138    138        
    #define BLOCK_139    139
    
    #define BLOCK_140    140        
    #define BLOCK_141    141        
    #define BLOCK_142    142        
    #define BLOCK_143    143
    
    #define BLOCK_144    144        
    #define BLOCK_145    145        
    #define BLOCK_146    146        
    #define BLOCK_147    147
    
    #define BLOCK_148    148        
    #define BLOCK_149    149        
    #define BLOCK_150    150        
    #define BLOCK_151    151
    
    #define BLOCK_152    152        
    #define BLOCK_153    153        
    #define BLOCK_154    154        
    #define BLOCK_155    155
    
    #define BLOCK_156    156        
    #define BLOCK_157    157        
    #define BLOCK_158    158        
    #define BLOCK_159    159
    
    #define BLOCK_160    160        
    #define BLOCK_161    161        
    #define BLOCK_162    162        
    #define BLOCK_163    163
    
    #define BLOCK_164    164        
    #define BLOCK_165    165        
    #define BLOCK_166    166        
    #define BLOCK_167    167
    
    #define BLOCK_168    168        
    #define BLOCK_169    169        
    #define BLOCK_170    170        
    #define BLOCK_171    171
    
    #define BLOCK_172    172        
    #define BLOCK_173    173        
    #define BLOCK_174    174        
    #define BLOCK_175    175
    
    #define BLOCK_176    176        
    #define BLOCK_177    177        
    #define BLOCK_178    178        
    #define BLOCK_179    179
    
    #define BLOCK_180    180        
    #define BLOCK_181    181        
    #define BLOCK_182    182        
    #define BLOCK_183    183
    
    #define BLOCK_184    184        
    #define BLOCK_185    185        
    #define BLOCK_186    186        
    #define BLOCK_187    187
    
    #define BLOCK_188    188        
    #define BLOCK_189    189        
    #define BLOCK_190    190        
    #define BLOCK_191    191
    
    #define BLOCK_192    192        
    #define BLOCK_193    193        
    #define BLOCK_194    194        
    #define BLOCK_195    195
    
    #define BLOCK_196    196        
    #define BLOCK_197    197        
    #define BLOCK_198    198        
    #define BLOCK_199    199
    
    #define BLOCK_200    200        
    #define BLOCK_201    201        
    #define BLOCK_202    202        
    #define BLOCK_203    203
    
    #define BLOCK_204    204        
    #define BLOCK_205    205        
    #define BLOCK_206    206        
    #define BLOCK_207    207
    
    #define BLOCK_208    208        
    #define BLOCK_209    209        
    #define BLOCK_210    210        
    #define BLOCK_211    211        
    
    #define BLOCK_212    212        
    #define BLOCK_213    213        
    #define BLOCK_214    214
    #define BLOCK_215    215        

    #define BLOCK_216    216        
    #define BLOCK_217    217        
    #define BLOCK_218    218
    #define BLOCK_219    219        

    #define BLOCK_220    220        
    #define BLOCK_221    221        
    #define BLOCK_222    222            
    #define BLOCK_223    223        

    #define BLOCK_224    224        
    #define BLOCK_225    225        
    #define BLOCK_226    226
    #define BLOCK_227    227        

    #define BLOCK_228    228        
    #define BLOCK_229    229        
    #define BLOCK_230    230
    #define BLOCK_231    231        

    #define BLOCK_232    232        
    #define BLOCK_233    233        
    #define BLOCK_234    234
    #define BLOCK_235    235        

    #define BLOCK_236    236        
    #define BLOCK_237    237        
    #define BLOCK_238    238
    #define BLOCK_239    239        

    #define BLOCK_240    240        
    #define BLOCK_241    241        
    #define BLOCK_242    242
    #define BLOCK_243    243        

    #define BLOCK_244    244        
    #define BLOCK_245    245        
    #define BLOCK_246    246
    #define BLOCK_247    247        

    #define BLOCK_248    248        
    #define BLOCK_249    249        
    #define BLOCK_250    250
    #define BLOCK_251    251        

    #define BLOCK_252    252        
    #define BLOCK_253    253        
    #define BLOCK_254    254
    #define BLOCK_255    255        



        
    /* NOTE:
       The RX210 actually has 64 x 128 byte blocks instead of the 
       16 x 2Kbyte blocks shown below. These are grouped into 4 blocks to
       make it easier for the user to delete larger sections of the data 
       flash. The user can still delete individual blocks but they will 
       need to use the new flash erase function that takes addresses 
       instead of blocks. */
    
    /* Data Flash Block Area         Size: Start Addr -   End Addr */
    #define BLOCK_DB0    256        /*    2KB: 0x00100000 - 0x001007FF */
    #define BLOCK_DB1    257        /*    2KB: 0x00100800 - 0x00100FFF */
    #define BLOCK_DB2    258        /*    2KB: 0x00101000 - 0x001017FF */
    #define BLOCK_DB3    259        /*    2KB: 0x00101800 - 0x00101FFF */


    /*  Bottom of DF Area */
    #define DF_ADDRESS      0x00100000
    /* Used for getting DF block */
    #define DF_MASK         0xFFFFF800
    /* Used for getting 128 byte boundary in DF block when erasing*/
    #define DF_ERASE_BLOCK_SIZE   0x00000080
    /* This is used to get the boundary of the 'fake' blocks that
       are 2KB. */
    #define DF_BLOCK_SIZE_LARGE     0x00000800
    /* Used for
    /* Defines how many "fake" DF blocks are on this part */
    #define DF_NUM_BLOCKS   4                
    
    #if ROM_SIZE == 131072
        /*  Bottom of User Flash Area */
        #define ROM_PE_ADDR     0x00FE0000  //128KB part
        /* Defines how many ROM blocks are on this part */
        #define ROM_NUM_BLOCKS  64          //128KB part
    #elif ROM_SIZE == 262144
        /*  Bottom of User Flash Area */
        #define ROM_PE_ADDR     0x00FC0000  //256KB part
        /* Defines how many ROM blocks are on this part */
        #define ROM_NUM_BLOCKS  128         //256KB part
    #elif ROM_SIZE == 393216
        /*  Bottom of User Flash Area */
        #define ROM_PE_ADDR     0x00FA0000  //384KB part
        /* Defines how many ROM blocks are on this part */
        #define ROM_NUM_BLOCKS  192         //384KB part
    #elif ROM_SIZE == 524288
        /*  Bottom of User Flash Area */
        #define ROM_PE_ADDR     0x00F80000  //512KB part
        /* Defines how many ROM blocks are on this part */
        #define ROM_NUM_BLOCKS  256         //512KB part
    #endif
    
    /* Programming size for ROM in bytes RX210 support 128, 8 and 2 byte options*/
    #define ROM_PROGRAM_SIZE_LARGE    128
    #define ROM_PROGRAM_SIZE_MEDIUM   8
    #define ROM_PROGRAM_SIZE_SMALL    2
    /* Programming size for data flash in bytes */
    /* NOTE: RX210programs in 2 and 8-byte intervals */
    #define DF_PROGRAM_SIZE_SMALL     2
    #define DF_PROGRAM_SIZE_LARGE     8

    /* Array of flash addresses used for writing */
    #if defined(FLASH_BLOCKS_DECLARE)
    const uint32_t block_addresses[260] = { 
            /* Caution. ID CODE(FFFFFFA0-FFFFFFAF) is excluded. */ 
                        
            0x00FFF800,  /* EB00 */
            0x00FFF000,  /* EB01 */
            0x00FFE800,  /* EB02 */
            0x00FFE000,  /* EB03 */
            0x00FFD800,  /* EB04 */
            0x00FFD000,  /* EB05 */
            0x00FFC800,  /* EB06 */
            0x00FFC000,  /* EB07 */  
            
            0x00FFB800,  /* EB08 */
            0x00FFB000,  /* EB09 */
            0x00FFA800,  /* EB00 */
            0x00FFA000,  /* EB01 */
            0x00FF9800,  /* EB02 */
            0x00FF9000,  /* EB03 */
            0x00FF8800,  /* EB04 */
            0x00FF8000,  /* EB05 */ 
            
            0x00FF7800,  /* EB06 */
            0x00FF7000,  /* EB07 */
            0x00FF6800,  /* EB08 */
            0x00FF6000,  /* EB09 */
            0x00FF5800,  /* EB00 */
            0x00FF5000,  /* EB01 */
            0x00FF4800,  /* EB02 */
            0x00FF4000,  /* EB03 */ 
            
            0x00FF3800,  /* EB04 */
            0x00FF3000,  /* EB05 */
            0x00FF2800,  /* EB06 */
            0x00FF2000,  /* EB07 */
            0x00FF1800,  /* EB08 */
            0x00FF1000,  /* EB09 */
            0x00FF0800,  /* EB00 */
            0x00FF0000,  /* EB01 */ 
            
            0x00FEF800,  /* EB02 */
            0x00FEF000,  /* EB03 */
            0x00FEE800,  /* EB04 */
            0x00FEE000,  /* EB05 */
            0x00FED800,  /* EB06 */
            0x00FED000,  /* EB07 */
            0x00FEC800,  /* EB08 */
            0x00FEC000,  /* EB09 */ 
            
            0x00FEB800,  /* EB00 */
            0x00FEB000,  /* EB01 */
            0x00FEA800,  /* EB02 */
            0x00FEA000,  /* EB03 */
            0x00FE9800,  /* EB04 */
            0x00FE9000,  /* EB05 */
            0x00FE8800,  /* EB06 */
            0x00FE8000,  /* EB07 */ 
            
            0x00FE7800,  /* EB08 */
            0x00FE7000,  /* EB09 */
            0x00FE6800,  /* EB00 */
            0x00FE6000,  /* EB01 */
            0x00FE5800,  /* EB02 */
            0x00FE5000,  /* EB03 */
            0x00FE4800,  /* EB04 */
            0x00FE4000,  /* EB05 */ 
            
            0x00FE3800,  /* EB06 */
            0x00FE3000,  /* EB07 */
            0x00FE2800,  /* EB08 */
            0x00FE2000,  /* EB09 */
            0x00FE1800,  /* EB00 */
            0x00FE1000,  /* EB01 */
            0x00FE0800,  /* EB02 */
            0x00FE0000,  /* EB03 */ 
            
            0x00FDF800,  /* EB04 */
            0x00FDF000,  /* EB05 */
            0x00FDE800,  /* EB06 */
            0x00FDE000,  /* EB07 */
            0x00FDD800,  /* EB08 */
            0x00FDD000,  /* EB09 */
            0x00FDC800,  /* EB00 */
            0x00FDC000,  /* EB01 */ 
            
            0x00FDB800,  /* EB02 */
            0x00FDB000,  /* EB03 */
            0x00FDA800,  /* EB04 */
            0x00FDA000,  /* EB05 */
            0x00FD9800,  /* EB06 */
            0x00FD9000,  /* EB07 */
            0x00FD8800,  /* EB08 */
            0x00FD8000,  /* EB09 */ 
            
            0x00FD7800,  /* EB10 */
            0x00FD7000,  /* EB11 */
            0x00FD6800,  /* EB12 */
            0x00FD6000,  /* EB13 */
            0x00FD5800,  /* EB14 */
            0x00FD5000,  /* EB15 */
            0x00FD4800,  /* EB16 */
            0x00FD4000,  /* EB17 */ 
            
            0x00FD3800,  /* EB18 */
            0x00FD3000,  /* EB19 */
            0x00FD2800,  /* EB10 */
            0x00FD2000,  /* EB11 */
            0x00FD1800,  /* EB12 */
            0x00FD1000,  /* EB13 */
            0x00FD0800,  /* EB14 */
            0x00FD0000,  /* EB15 */ 
            
            0x00FCF800,  /* EB16 */
            0x00FCF000,  /* EB17 */
            0x00FCE800,  /* EB18 */
            0x00FCE000,  /* EB19 */
            0x00FCD800,  /* EB10 */
            0x00FCD000,  /* EB11 */
            0x00FCC800,  /* EB12 */
            0x00FCC000,  /* EB13 */ 
            
            0x00FCB800,  /* EB14 */
            0x00FCB000,  /* EB15 */
            0x00FCA800,  /* EB16 */
            0x00FCA000,  /* EB17 */
            0x00FC9800,  /* EB18 */
            0x00FC9000,  /* EB19 */
            0x00FC8800,  /* EB10 */
            0x00FC8000,  /* EB11 */ 

            0x00FC7800,  /* EB14 */
            0x00FC7000,  /* EB14 */
            0x00FC6800,  /* EB14 */
            0x00FC6000,  /* EB14 */
            0x00FC5800,  /* EB14 */
            0x00FC5000,  /* EB14 */
            0x00FC4800,  /* EB14 */
            0x00FC4000,  /* EB14 */ 
            
            0x00FC3800,  /* EB15 */
            0x00FC3000,  /* EB15 */
            0x00FC2800,  /* EB15 */
            0x00FC2000,  /* EB15 */
            0x00FC1800,  /* EB15 */
            0x00FC1000,  /* EB15 */
            0x00FC0800,  /* EB15 */
            0x00FC0000,  /* EB15 */ 
            
            0x00FBF800,  /* EB16 */
            0x00FBF000,  /* EB16 */
            0x00FBE800,  /* EB16 */
            0x00FBE000,  /* EB16 */
            0x00FBD800,  /* EB16 */
            0x00FBD000,  /* EB16 */
            0x00FBC800,  /* EB16 */
            0x00FBC000,  /* EB16 */ 
            
            0x00FBB800,  /* EB17 */
            0x00FBB000,  /* EB17 */
            0x00FBA800,  /* EB17 */
            0x00FBA000,  /* EB17 */
            0x00FB9800,  /* EB17 */
            0x00FB9000,  /* EB17 */
            0x00FB8800,  /* EB17 */
            0x00FB8000,  /* EB17 */ 
            
            0x00FB7800,  /* EB18 */
            0x00FB7000,  /* EB18 */
            0x00FB6800,  /* EB18 */
            0x00FB6000,  /* EB18 */
            0x00FB5800,  /* EB18 */
            0x00FB5000,  /* EB18 */
            0x00FB4800,  /* EB18 */
            0x00FB4000,  /* EB18 */ 
            
            0x00FB3800,  /* EB19 */
            0x00FB3000,  /* EB19 */
            0x00FB2800,  /* EB19 */
            0x00FB2000,  /* EB19 */
            0x00FB1800,  /* EB19 */
            0x00FB1000,  /* EB19 */
            0x00FB0800,  /* EB19 */
            0x00FB0000,  /* EB19 */ 
            
            0x00FAF800,  /* EB20 */
            0x00FAF000,  /* EB20 */
            0x00FAE800,  /* EB20 */
            0x00FAE000,  /* EB20 */
            0x00FAD800,  /* EB20 */
            0x00FAD000,  /* EB20 */
            0x00FAC800,  /* EB20 */
            0x00FAC000,  /* EB20 */ 
            
            0x00FAB800,  /* EB21 */
            0x00FAB000,  /* EB21 */
            0x00FAA800,  /* EB21 */
            0x00FAA000,  /* EB21 */
            0x00FA9800,  /* EB21 */
            0x00FA9000,  /* EB21 */
            0x00FA8800,  /* EB21 */
            0x00FA8000,  /* EB21 */ 
            
            0x00FA7800,  /* EB22 */
            0x00FA7000,  /* EB22 */
            0x00FA6800,  /* EB22 */
            0x00FA6000,  /* EB22 */
            0x00FA5800,  /* EB22 */
            0x00FA5000,  /* EB22 */
            0x00FA4800,  /* EB22 */
            0x00FA4000,  /* EB22 */
            
            0x00FA3800,  /* EB23 */ 
            0x00FA3000,  /* EB23 */ 
            0x00FA2800,  /* EB23 */ 
            0x00FA2000,  /* EB23 */ 
            0x00FA1800,  /* EB23 */ 
            0x00FA1000,  /* EB23 */ 
            0x00FA0800,  /* EB23 */  
            0x00FA0000,  /* EB23 */ 
            
            0x00F9F800,  /* EB24 */
            0x00F9F000,  /* EB24 */
            0x00F9E800,  /* EB24 */
            0x00F9E000,  /* EB24 */
            0x00F9D800,  /* EB24 */
            0x00F9D000,  /* EB24 */
            0x00F9C800,  /* EB24 */
            0x00F9C000,  /* EB24 */ 
            
            0x00F9B800,  /* EB25 */
            0x00F9B000,  /* EB25 */
            0x00F9A800,  /* EB25 */
            0x00F9A000,  /* EB25 */
            0x00F99800,  /* EB25 */
            0x00F99000,  /* EB25 */
            0x00F98800,  /* EB25 */
            0x00F98000,  /* EB25 */ 
            
            0x00F97800,  /* EB26 */
            0x00F97000,  /* EB26 */
            0x00F96800,  /* EB26 */
            0x00F96000,  /* EB26 */
            0x00F95800,  /* EB26 */
            0x00F95000,  /* EB26 */
            0x00F94800,  /* EB26 */
            0x00F94000,  /* EB26 */
             
            0x00F93800,  /* EB27 */
            0x00F93000,  /* EB27 */
            0x00F92800,  /* EB27 */
            0x00F92000,  /* EB27 */
            0x00F91800,  /* EB27 */
            0x00F91000,  /* EB27 */
            0x00F90800,  /* EB27 */
            0x00F90000,  /* EB27 */ 
            
            0x00F8F800,  /* EB28 */
            0x00F8F000,  /* EB28 */
            0x00F8E800,  /* EB28 */
            0x00F8E000,  /* EB28 */
            0x00F8D800,  /* EB28 */
            0x00F8D000,  /* EB28 */
            0x00F8C800,  /* EB28 */
            0x00F8C000,  /* EB28 */ 
            
            0x00F8B800,  /* EB29 */
            0x00F8B000,  /* EB29 */
            0x00F8A800,  /* EB29 */
            0x00F8A000,  /* EB29 */
            0x00F89800,  /* EB29 */
            0x00F89000,  /* EB29 */
            0x00F88800,  /* EB29 */
            0x00F88000,  /* EB29 */

            0x00F87800,  /* EB30 */ 
            0x00F87000,  /* EB30 */
            0x00F86800,  /* EB30 */ 
            0x00F86000,  /* EB30 */
            0x00F85800,  /* EB30 */ 
            0x00F85000,  /* EB30 */         
            0x00F84800,  /* EB30 */ 
            0x00F84000,  /* EB30 */
            
            0x00F83800,  /* EB31 */
            0x00F83000,  /* EB31 */ 
            0x00F82800,  /* EB31 */
            0x00F82000,  /* EB31 */         
            0x00F81800,  /* EB31 */
            0x00F81000,  /* EB31 */ 
            0x00F80800,  /* EB31 */
            0x00F80000,  /* EB31 */ 
            
            0x00100000,  /* DB00 */ 
            0x00100800,  /* DB01 */ 
            0x00101000,  /* DB02 */ 
            0x00101800};  /* DB03 */ 
             

    #else 
    extern const uint32_t block_addresses[260];
    #endif             
            
    /*  According to HW Manual the Max Erasure Time for a 64kB block is
        around 1152ms.  This is with a FCLK of 50MHz. The calculation below
        calculates the number of ICLK ticks needed for the timeout delay.
        The 1152ms number is adjusted linearly depending on the FCLK frequency.
    */
    #define WAIT_MAX_ERASE \
            ((int32_t)(1152000 * (50.0/FLASH_CLOCK_FREQUENCY))*ICLK_FREQUENCY)  

#else
    #error "!!! Need to define memory specifics for this RX200 family \
            in r_Flash_API_RX200.h !!!"
#endif


#endif /* _FLASH_API_H */