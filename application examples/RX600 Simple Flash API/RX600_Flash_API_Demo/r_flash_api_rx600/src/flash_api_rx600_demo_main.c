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
* http://www.renesas.com/disclaimer 
*
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.    
*******************************************************************************/
/******************************************************************************
* File Name    : flash_api_rx600_demo_main.c
* Version      : 2.20
* Device       : RX600 Series
* Tool-Chain   : RX Family C Compiler
* H/W Platform : RSKRX62N, RSKRX610, YRDKRX62N, RSKRX630, RSKRX63N, RSKRX62T,
*                YRDKRX63N
* Description  : Demo code for using the Simple Flash API for RX. This code will
*                erase and write to all flash blocks except for the ones that
*                are holding this application.
* Operation    : 1. Compile the sample code.
*                2. Enable "Debug the program re-writing the PROGRAM ROM" and/or
*                   "Debug the program re-writing the DATA FLASH" in the
*                   "Hardware Setup" dialog. If you debug without these
*                   settings, the memory window will not function correctly and
*                   make the program fail depending on what memory addresses
*                   are visible in the memory window.
*                3. Download code to the MCU.
*                   NOTE:
*                   Do not use live update in the Memory window when debugging
*                   this example since that might cause memory access conflicts
*                   prohibiting further FCU commands to be executed.
*                4. Place a breakpoint on the infinite loop at the end of the
*                   main() function. You can also place a breakpoint on the
*                   R_FlashDataAreaAccess() function to stop in between data
*                   flash and ROM operations.
*                5. Click 'Go'.
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
*         : 23.08.2010 1.42    Updated source to raise the coding standard, to
*                              meet GSCE & RSK standards.
*         : 15.02.2011 1.43    Fixed bug in blank check routine when handling
*                              input arguments and moved _Flash_Init() to
*                              _Enter_PE_Mode() function.
*         : 21.04.2011 2.00    Added BGO capabilities for data flash. Made 
*                              some more changes to fit coding standard. Added
*                              ability to do ROM to ROM or DF to DF transfers.
*                              Added the ability to use the lock bit feature
*                              on ROM blocks.  Added BGO capabilities for
*                              ROM operations.
*         : 06.07.2011 2.10    Added support for RX630, RX631, and RX63N.
*                              Also added R_FlashEraseRange() for devices like
*                              RX63x that have finer granularity data flash.
*                              Various bug fixes as well. Example bug fix was
*                              removing DATA_FLASH_OPERATION_PIPL and 
*                              ROM_OPERATION_PIPL #defines since the IPL was
*                              not restored when leaving flash ready interrupt.
*         : 29.11.2011 2.20    Renamed private functions according to new 
*                              Coding Standard. Removed unused 'bytes' argument 
*                              from enter_pe_mode() function. Removed 'far' 
*                              keyword since it is not needed. Fixed where some 
*                              functions were being placed in RAM when this was
*                              not needed. Uses platform.h now instead of 
*                              having multiple iodefine_rxXXX.h's. Added 
*                              __evenaccess directive to FCU accesses. This 
*                              ensures proper bus width accesses. Added
*                              R_FlashCodeCopy() function. When clearing the
*                              FENTRYR register, the register is read back to
*                              ensure its value is 0x0000. Added call to 
*                              exit_pe_mode() when enter_pe_mode() function 
*                              fails to protect against reading ROM in P/E
*                              mode. Added option to use r_bsp package.
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
/* Get memory placement directives */
#include "placement.h"
/* Get board and MCU definitions. */
#include "platform.h"
/* Used for Simple Flash API functions */
#include "r_flash_api_rx600.h"
/* Used for getting flash info for testing. User would typically not need to 
   include this file with their projects. */
#include "r_flash_api_rx600_private.h"

/******************************************************************************
Macro definitions
******************************************************************************/
#define PROG_SIZE       512
#define BUFF_SIZE       512

/* If you want to test DF to DF or ROM to ROM transfers then first make sure
   to enable the feature in r_flash_api_rx600_config.h by uncommenting the
   #define for 'FLASH_TO_FLASH'. */   
   
/* If you want to test DF to DF uncomment this #define */
//#define TEST_DF_TO_DF_TRANSFERS
/* If you want to test ROM to ROM transfers then uncomment this #define */
//#define TEST_ROM_TO_ROM_TRANSFERS

#if (defined(TEST_DF_TO_DF_TRANSFERS) || defined(TEST_ROM_TO_ROM_TRANSFERS)) \
    && !defined(FLASH_TO_FLASH)
ERROR!!! - In order to test flash to flash transfers you must first enable
this feature in r_flash_api_rx600_config.h by uncommenting the define
for FLASH_TO_FLASH.
#endif

/* In your application feel free to enable both ROM to ROM and DF to DF 
   transfers. However, in this sample code there is only one program buffer
   so it cannot be in DF and ROM at the same time. This means that you can
   only test DF to DF or ROM to ROM in one test run. */
#if defined(TEST_DF_TO_DF_TRANSFERS) && defined(TEST_ROM_TO_ROM_TRANSFERS)
ERROR!!! - With this sample code you can only test DF to DF or ROM to ROM
transfers at one time. In your application feel free to use both though.
#endif

#if defined(TEST_DF_TO_DF_TRANSFERS) && defined(USER_BOOT_MODE)
ERROR!!! - Everything ends up in the user boot area so DF to DF transfers
cannot be used. The test program would have to copy the test data from the
user boot area to the data flash in runtime and then perform the DF to DF
transfer.
#endif

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Exported global variables
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
#if defined(TEST_DF_TO_DF_TRANSFERS)
/* If testing DF to DF transfers then put program data in data flash block 0.
   The code which copy the data depends on that it resides in block 0. The data
   will end up in block 0 (first in the data flash) as long as no other data
   buffer which is larger than 'prog_buff' is placed in the '.dataflash'
   section. */
PLACE_IN_DATA_FLASH
#endif
static const uint8_t prog_buff[BUFF_SIZE] = "hello world0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

static uint8_t read_buff[BUFF_SIZE];
static uint32_t address;

/* Loop variable, put here for ease in use when debugging */
volatile uint32_t loop, ret;

#if defined(DATA_FLASH_BGO) || defined(ROM_BGO)
/* Used to tell application whether previous flash operation has finished */
static uint8_t flash_is_ready;
#endif
#if defined(DATA_FLASH_BGO)
/* Holds the outcome of the blank check operation */
static uint8_t blank_check_result;
#endif

void main(void);
static void flash_api_demo(void);

/******************************************************************************
* Function Name: main
* Description  : Program starts here after Reset and initlization. Runs through
*                all functions of the Flash API.
* Arguments    : none
* Return Value : none
******************************************************************************/
void main(void)
{
  __enable_interrupt();
  
  /* Run Flash API tests. */
  flash_api_demo();
  
  while(1)
  {
    /* Infinite Loop */
  }
}

#if defined(ROM_BGO) && !defined(USER_BOOT_MODE)
/* If you are testing ROM BGO then this sample code will need to be in RAM.
   The reason for this is that the API calls will be non-blocking so the 
   code will return to here. If the sample code was in ROM then when the
   API code returned it would try to fetch from ROM and would cause a flash
   access error. */

/* We will also need some RAM space to hold the interrupt vector table */
static unsigned long ram_vector_table[256];
/* If using ROM BGO then this sample code needs to be in RAM */
PLACE_IN_RAM
#endif

/******************************************************************************
* Function Name: flash_api_demo
* Description  : Runs through all functions of the Flash API.
* Arguments    : none
* Return Value : none
******************************************************************************/
static void flash_api_demo(void)
{
  /* Loop counter. */
  uint32_t i;
  
  /* If user boot mode is enabled, the interrupt vector table will be
     located in the user boot area. */
#if defined(ROM_BGO) && !defined(USER_BOOT_MODE)
  /* Will hold INTB */
  unsigned long * flash_vect;
#endif
  
#ifdef ENABLE_ROM_PROGRAMMING
  
#if defined(ROM_BGO) && !defined(USER_BOOT_MODE)
  /* If ROM BGO is used then the variable vector table will need to be 
     relocated to RAM. This code does this. */
  /* Get address of variable vector table in ROM */
  flash_vect = (unsigned long *)__get_interrupt_table();
  
  /* Copy over variable vector table to RAM */
  for(i = 0; i < 256; i++ )
  {
    /* Copy over entry */
    ram_vector_table[i] = *flash_vect;
    
    /* Move pointer */
    flash_vect += 1;
  }
  
  /* Set INTB to ram address */
  __set_interrupt_table((unsigned long)&ram_vector_table[0]);
  
#endif /* ROM_BGO && !USER_BOOT_MODE */
  
#if defined(TEST_DF_TO_DF_TRANSFERS)
  /* If we are testing DF to DF transfers then buffer data will be located
     in the DF. DF therefore needs to be enabled before ROM programming. */
  R_FlashDataAreaAccess(0xFFFF, 0xFFFF);
#endif
  
  /*****************************************************************/
  /*                    Write/Erase User Flash                     */
  /*****************************************************************/
    
  /* Test erasing and flashing ROM 
     'loop' starts at 4 so we don't erase the first 4 blocks
     which is where this program is located by default. */
  for(loop = 4; loop < ROM_NUM_BLOCKS; loop++) 
  {
    
    /* Get address for reading */
    address = (0xFF000000 | g_flash_BlockAddresses[loop]);
    
#if defined(ROM_BGO)
    /* Flash is ready */
    flash_is_ready = false;
#endif
    
#if (!defined(IGNORE_LOCK_BITS)) && (!defined(ROM_BGO))
    /* Lock bit tests should be done with ROM BGO disabled. There is
       no problem with using the two together, it would just have made
       the code below more complex. */
    
    /* Enable lock bit protection (this is default out of reset) */
    ret = R_FlashSetLockBitProtection(true);
    
    /* Should turn lock bit protection on */
    while( ret != FLASH_SUCCESS );
    
    /* Program lock bits */        
    ret = R_FlashProgramLockBit(loop);
    
    /* Should set lock bit successfully */
    while( ret != FLASH_SUCCESS );
    
    /* Read lock bit */
    ret = R_FlashReadLockBit(loop);
    
    /* Should return lock bit is set */
    while( ret != FLASH_LOCK_BIT_SET );
    
    /* Attempt an erase, should fail */
    ret = R_FlashErase(loop);
    
    /* Should return failure due to lock bit being set */
    while( ret != FLASH_FAILURE );        
    
    /* Attempt a write, should fail */
    ret = R_FlashWrite(address, (uint32_t)prog_buff, PROG_SIZE);
    
    /* Should return failure due to lock bit being set */
    while( ret != FLASH_FAILURE );                
    
    /* Disable lock bit protection */
    ret = R_FlashSetLockBitProtection(false);
    
    /* Should turn lock bit protection off */
    while( ret != FLASH_SUCCESS );
    
    /* Attempt erase again */
    ret = R_FlashErase(loop);
    
    /* Should return that erase was successful */
    while( ret != FLASH_SUCCESS );        
    
    /* Read lock bit */
    ret = R_FlashReadLockBit(loop);   
    
    /* Should return lock bit is not set */
    while( ret != FLASH_LOCK_BIT_NOT_SET );                    
    
#else        
    
    /************************/
    /* Erase our Flash area */
    /************************/
    /* NOTE: You can watch the flash memory erasing/writing with the E1/E20
       if you enable "Debug the program re-writing the PROGRAM ROM" 
       in the "Hardware Setup" dialog.
       */
    ret = R_FlashErase(loop); 
    
    /* Check for errors */
    if(ret != FLASH_SUCCESS)
    {
      while(1)
      {
        /* Error in demo. */
      }
    }    
    
#endif /* (!defined(IGNORE_LOCK_BITS)) && (!defined(ROM_BGO)) */
    
#if defined(ROM_BGO)
    /* Wait for flash operation to finish. In a real application we
    could go off and do real work. */
    while( flash_is_ready == false );
#endif
    
    /* Read area after erase */
    for(i = 0; i < sizeof(read_buff); i++) 
    {
      read_buff[i] = *(uint8_t *)(address + i);
    }
    
    /* Verify it was erased */
    for(i = 0; i < sizeof(read_buff); i++) 
    {
      if(read_buff[i] != 0xFF) 
      {
        while(1)
        {
          /* Error in demo. */
        }
      }
    }
    
    /*****************************/
    /* Write our buffer to Flash */
    /*****************************/
    ret = R_FlashWrite(address, (uint32_t)prog_buff, PROG_SIZE);
    
    /* Check for errors */
    if(ret != FLASH_SUCCESS) 
    {
      while(1)
      {
        /* Error in demo. */
      }
    }
    
#if defined(ROM_BGO)
    /* You will notice that here, instead of polling on the variable
       'flash_is_ready' we are polling on the function 
       R_FlashGetStatus() */
    while( R_FlashGetStatus() == FLASH_BUSY );
#endif                        
    
    /* Read back written area */
    for(i = 0; i < sizeof(read_buff); i++) 
    {
      read_buff[i] = *(uint8_t *)(address + i);
    }
    
    /* Verify */
    for(i = 0; i < sizeof(read_buff); i++) 
    {
      if(read_buff[i] != prog_buff[i]) 
      {
        while(1)
        {
          /* Error in demo. */
        }
      }            
    }
  }
  
#endif /* ENABLE_ROM_PROGRAMMING */
    
 
    
  /*****************************************************************/
  /*                    Write/Erase Data Flash                     */
  /*****************************************************************/    
    
  /* After Data Flash Area is enabled, you can access Data Flash Memory */
  R_FlashDataAreaAccess(0xFFFF, 0xFFFF);
  
  /* If testing DF to DF transfers then the first DF block is reserved for
     holding the data */
#ifdef TEST_DF_TO_DF_TRANSFERS
  for(loop = 1; loop < DF_NUM_BLOCKS; loop++)
#else    
  for(loop = 0; loop < DF_NUM_BLOCKS; loop++)
#endif
  {
    /* Get beginning address of DF block */
    address = g_flash_BlockAddresses[BLOCK_DB0 + loop];
    
#if defined(DATA_FLASH_BGO)
    /* Flash is ready */
    flash_is_ready = false;
#endif
    
    /***************************/
    /* Check if block is blank */
    /***************************/
    ret = R_FlashDataAreaBlankCheck(address, BLANK_CHECK_ENTIRE_BLOCK);
    
#if defined(DATA_FLASH_BGO)
    /* Wait for flash operation to finish. In a real application we
       could go off and do real work. */
    while( flash_is_ready == false );
    
    /* Flash is ready */
    flash_is_ready = false;
#endif
    
    /************************/
    /* Erase our Flash area */
    /************************/
    /* NOTE: You can watch the flash memory erasing/writing with the E1/E20
       if you enable "Debug the program re-writing the DATA FLASH" 
       in the "Hardware Setup" dialog.
       */
    ret = R_FlashErase(BLOCK_DB0 + loop);
    
    /* Check for errors */
    if(ret != FLASH_SUCCESS) 
    {
      while(1)
      {
        /* Error in demo. */
      }
    }
    
#if defined(DATA_FLASH_BGO)
    /* Wait for flash operation to finish. In a real application we
       could go off and do real work. */
    while( flash_is_ready == false );
    
    /* Flash is ready */
    flash_is_ready = false;
#endif
    
    /* Check Blank Checking using both methods of input: address and
       block number */
    /* First try with address */           
    ret = R_FlashDataAreaBlankCheck(address, BLANK_CHECK_ENTIRE_BLOCK);
    
#if defined(DATA_FLASH_BGO)
    /* Wait for flash operation to finish. In a real application we
       could go off and do real work. */
    while( flash_is_ready == false );
    
    /* Copy over blank check result */
    ret = blank_check_result;
#endif        
    
    /* Should be FLASH_BLANK since it was just erased */
    if(ret != FLASH_BLANK) 
    {
      while(1)
      {
        /* Error in demo. */
      }
    }
    
#if defined(DATA_FLASH_BGO)
    /* Flash is ready */
    flash_is_ready = false;
#endif        
    
    /* Now use block number */           
    ret = R_FlashDataAreaBlankCheck(BLOCK_DB0 + loop, BLANK_CHECK_ENTIRE_BLOCK);
    
#if defined(DATA_FLASH_BGO)
    /* Wait for flash operation to finish. In a real application we
       could go off and do real work. */
    while( flash_is_ready == false );
    
    /* Copy over blank check result */
    ret = blank_check_result;
#endif        
    
    /* Should be FLASH_BLANK since it was just erased */
    if(ret != FLASH_BLANK) 
    {
      while(1)
      {
        /* Error in demo. */
      }
    }        
    
#if defined(DATA_FLASH_BGO)
    /* Flash is ready */
    flash_is_ready = false;
#endif        
    
    /*****************************/
    /* Write our buffer to Flash */
    /*****************************/
    ret = R_FlashWrite( address, (uint32_t)prog_buff, sizeof(read_buff));
    
    /* Check for errors */
    if(ret != FLASH_SUCCESS) 
    {
      while(1)
      {
        /* Error in demo. */
      }
    }
    
#if defined(DATA_FLASH_BGO)
    /* Wait for flash operation to finish. In a real application we
       could go off and do real work. */
    while( flash_is_ready == false );
#endif        
    
    
    /* Should return error because not multiple of the programming size */
    ret = R_FlashWrite( address, (uint32_t)prog_buff, 15);
    
    /* Check for absence of errors */
    if(ret == FLASH_SUCCESS) 
    {
      while(1)
      {
        /* Error in demo. */
      }
    }
    
#if defined(DATA_FLASH_BGO)
    /* You will notice that here, instead of polling on the variable
       'flash_is_ready' we are polling on the function 
       R_FlashGetStatus() */
    while( R_FlashGetStatus() == FLASH_BUSY );
#endif                
    
    /* Clear ret */
    ret = 0;
    
    /* Read back written area */
    for(i = 0; i < sizeof(read_buff); i++) 
    {
      read_buff[i] = *(uint8_t *)(address + i);
    }
    
    /* Verify */
    for(i = 0; i < sizeof(read_buff); i++) 
    {
      if(read_buff[i] != prog_buff[i]) 
      {
        while(1)
        {
          /* Error in demo. */
        }
      }            
    }                
    
    /******************************************************************/
    /* More Blank Check tests, use debugger to see returned values    */
    /******************************************************************/                
    /* Check Blank Checking using both methods of input: address and
       block number */
    /* First try with address */           
    ret = R_FlashDataAreaBlankCheck(address, BLANK_CHECK_ENTIRE_BLOCK);  
    
#if defined(DATA_FLASH_BGO)
    /* You will notice that here, instead of polling on the variable
       'flash_is_ready' we are polling on the function 
       R_FlashGetStatus() */
    while( R_FlashGetStatus() == FLASH_BUSY );
    
    /* Copy over blank check result */
    ret = blank_check_result;
#endif                
    
    /* Should return FLASH_NOT_BLANK (not blank) */
    if(ret != FLASH_NOT_BLANK) 
    {
      while(1)
      {
        /* Error in demo. */
      }
    }
    
    /* Now use block number */           
    ret = R_FlashDataAreaBlankCheck(BLOCK_DB0 + loop, BLANK_CHECK_ENTIRE_BLOCK);
    
#if defined(DATA_FLASH_BGO)
    /* You will notice that here, instead of polling on the variable
       'flash_is_ready' we are polling on the function 
       R_FlashGetStatus() */
    while( R_FlashGetStatus() == FLASH_BUSY );
    
    /* Copy over blank check result */
    ret = blank_check_result;
#endif                
    
    /* Should return FLASH_NOT_BLANK (not blank) */
    if(ret != FLASH_NOT_BLANK) 
    {
      while(1)
      {
        /* Error in demo. */
      }
    }        
    
    /* Should return FLASH_NOT_BLANK (not blank) */
    /* Perform blank check on newly erased area. Smaller blank check
       does not use BGO so return is the correct result. */        
#if defined(MCU_RX610) || defined(MCU_RX621) || defined(MCU_RX62N) || defined(MCU_RX62T)        
    ret = R_FlashDataAreaBlankCheck(address, BLANK_CHECK_8_BYTE);
#elif defined(MCU_RX630) || defined(MCU_RX631) || defined(MCU_RX63N)
    ret = R_FlashDataAreaBlankCheck(address, BLANK_CHECK_2_BYTE);
#endif        
    
    /* Should return FLASH_NOT_BLANK (blank) */
    if(ret != FLASH_NOT_BLANK) 
    {
      while(1)
      {
        /* Error in demo. */
      }
    }                
    
    /* Should return FLASH_BLANK (blank) */
#if defined(MCU_RX610) || defined(MCU_RX621) || defined(MCU_RX62N) || defined(MCU_RX62T)                
    ret = R_FlashDataAreaBlankCheck(address + sizeof(read_buff), BLANK_CHECK_8_BYTE);
#elif defined(MCU_RX630) || defined(MCU_RX631) || defined(MCU_RX63N)
    ret = R_FlashDataAreaBlankCheck(address + sizeof(read_buff), BLANK_CHECK_2_BYTE);
#endif        
    
    /* Should return FLASH_BLANK (blank) */
    if(ret != FLASH_BLANK) 
    {
      while(1)
      {
        /* Error in demo. */
      }
    }                
    
#if defined(MCU_RX630) || defined(MCU_RX631) || defined(MCU_RX63N)
    /* Test the erasing a range of data flash addresses if supported by MCU */
    ret = R_FlashEraseRange(address, DF_ERASE_BLOCK_SIZE*2);
    
    /* Check for errors */
    if(ret != FLASH_SUCCESS) 
    {
      while(1)
      {
        /* Error in demo. */
      }
    }    
    
#if defined(DATA_FLASH_BGO)
    /* Wait for operation to finish */
    while( R_FlashGetStatus() == FLASH_BUSY );                    
#endif             
    
    /* Perform blank check on newly erased area. Smaller blank check
       does not use BGO so return is the correct result. */
    ret = R_FlashDataAreaBlankCheck(address, BLANK_CHECK_2_BYTE);
    
    /* Should be FLASH_BLANK since it was just erased */
    if(ret != FLASH_BLANK) 
    {
      while(1)
      {
        /* Error in demo. */
      }
    }                                                               
#endif /* defined(MCU_RX630) || defined(MCU_RX631) || defined(MCU_RX63N) */
  } 
  
  /*****************************/
  /* Tests are done            */
  /*****************************/    
}

#if defined(DATA_FLASH_BGO)

/******************************************************************************
* Function Name: FlashBlankCheckDone
* Description  : Callback function for when blank check operation has finished
* Arguments    : result - FLASH_BLANK if block was blank
*                         FLASH_NOT_BLANK if block was not blank
* Return Value : none
******************************************************************************/
#if defined(ROM_BGO) && !defined(USER_BOOT_MODE)
PLACE_IN_RAM
#endif
void FlashBlankCheckDone(uint8_t result)
{
  /* Copy result of blank check */
  blank_check_result = result;
  
  /* Signal flash operation is done */
  flash_is_ready = true;
}
/******************************************************************************
End of function  FlashBlankCheckDone
******************************************************************************/

#endif /* defined(DATA_FLASH_BGO) */

#if defined(DATA_FLASH_BGO) || defined(ROM_BGO)

/******************************************************************************
* Function Name: FlashEraseDone
* Description  : Callback function for when blank check operation has finished
* Arguments    : none
* Return Value : none
******************************************************************************/
#if defined(ROM_BGO) && !defined(USER_BOOT_MODE)
PLACE_IN_RAM
#endif
void FlashEraseDone(void)
{
  /* Signal flash operation is done */
  flash_is_ready = true;
}
/******************************************************************************
End of function  FlashEraseDone
******************************************************************************/

/******************************************************************************
* Function Name: FlashWriteDone
* Description  : Callback function for when blank check operation has finished
* Arguments    : none
* Return Value : none
******************************************************************************/
#if defined(ROM_BGO) && !defined(USER_BOOT_MODE)
PLACE_IN_RAM
#endif
void FlashWriteDone(void)
{
  /* Signal flash operation is done */
  flash_is_ready = true;
}
/******************************************************************************
End of function  FlashWriteDone
******************************************************************************/

/******************************************************************************
* Function Name: FlashError
* Description  : Callback function for when blank check operation has finished
* Arguments    : none
* Return Value : none
******************************************************************************/
#if defined(ROM_BGO) && !defined(USER_BOOT_MODE)
PLACE_IN_RAM
#endif
void FlashError(void)
{
  /* Alert program or user that flash error has occurred */
  while(1)
  {
    /* Error in demo. */
  }
}
/******************************************************************************
End of function  FlashError
******************************************************************************/

#endif /* defined(DATA_FLASH_BGO) || defined(ROM_BGO) */

