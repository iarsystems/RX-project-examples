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
* File Name     : Flash_API_RX600.c
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

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
/* Intrinsic functions of MCU */
#include <machine.h>

/* Function prototypes and device specific info needed for Flash API */
#include "r_Flash_API_RX600.h"

/* Device specific SFR definitions */
#if defined(RX610)
#include "iodefine_RX610.h"
#elif defined(RX62N)
#include "iodefine_RX62N.h"
#elif defined(RX621)
#include "iodefine_RX62N.h"
#elif defined(RX62T)
#include <iorx62t.h>
#else
#error "!!! You must specify your device in Flash_API_RX600.h first !!!"
#endif

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/
/* Define read mode macro */
#define READ_MODE 0
/* Define ROM PE mode macro */
#define ROM_PE_MODE 1
/* Define data flash PE mode macro */
#define FLD_PE_MODE 2
/*  The number of ICLK ticks needed for 35us delay are calculated below */
#define  WAIT_tRESW (35*ICLK_FREQUENCY)              
/*  The number of ICLK ticks needed for 10us delay are calculated below */
#define  WAIT_t10USEC (10*ICLK_FREQUENCY)    
/* This macro was defined in the Flash_API_RXxxx.h file and will 
   create an array of flash addresses called block_addresses.  It is
   used for getting the write addresses of flash blocks */
DEFINE_ADDR_LOOKUP

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
/* Signals whether FCU firmware has been transferred to the FCU RAM 
   0 : No, 1 : Yes */
static uint8_t fcu_transfer_complete = 0;
/* Valid values are 'READ_MODE','ROM_PE_MODE' or 'FLD_PE_MODE' */
static uint8_t current_mode;       
/* Stores whether the peripheral clock notification command has 
   been executed */
static uint8_t fcu_pclk_command = 0;   
/* Holds IPL of processor before flash operation */
static uint8_t flash_pipl;
/* Semaphore for making sure flash routines do not stomp on each other */
signed long flash_semaphore = 0;
/* States for flash operations */
static enum Flash_States{ FLASH_READY, 
                          FLASH_ERASING, 
                          FLASH_WRITING, 
                          FLASH_BLANKCHECK, 
                          FLASH_LOCK_BIT  } flash_state;

#ifndef IGNORE_LOCK_BITS                          
/* Determines whether lock bit protection is used when programming/erasing */
static uint8_t lock_bit_protection = true;                          
#endif

/* Only allocate these arrays if needed. */
#ifdef FLASH_TO_FLASH   
/* Used for holding data when DF to DF write is peformed */
static uint8_t temp_array[256];
/* Used for telling whether a DF to DF or ROM to ROM operation is on-going */
static uint8_t flash_to_flash_op;
#endif

#if defined(DATA_FLASH_BGO) || defined(ROM_BGO)
/* How many bytes are left to program */
static uint32_t bgo_bytes;
/* Where we are programming to */
static uint32_t  bgo_flash_addr;
/* Where the data is that we are programming */
static uint32_t  bgo_buffer_addr;
#endif

/* Flash intialisation function prototype */
static uint8_t _Flash_Init(void);
/* Enter PE mode function prototype */
static uint8_t _Enter_PE_Mode( uint32_t flash_addr, uint16_t bytes );
/* Exit PE mode function prototype */
static void _Exit_PE_Mode(void);
/* ROM write function prototype */
static uint8_t _ROM_Write(uint32_t address, uint32_t data);
/* Data flash write function prototype */
static uint8_t _DataFlash_Write(uint32_t address, uint32_t data, uint8_t size);
/* Data flash status clear function prototype */
static void _DataFlash_Status_Clr(void);
/* Notify peripheral clock function prototype */
static uint8_t _Notify_Peripheral_Clock(uint8_t * flash_addr);
/* FCU reset function prototype */
static void    _Flash_Reset(void);
/* Used to grab flash state */
static uint8_t _FlashGrabState(enum Flash_States new_state);
/* Used to release flash state */
static void _FlashReleaseState(void);

    
/******************************************************************************
* Function Name: _Flash_Init
* Description  : Initializes the FCU peripheral block.
*                NOTE: This function does not have to execute from in RAM.
* Arguments    : none
* Return Value : FLASH_SUCCESS = Operation Successful
*                FLASH_FAILURE = Operation Failed
******************************************************************************/
static uint8_t _Flash_Init(void)
{
  /* Declare source and destination pointers */
  uint32_t * src, * dst;
  
  /* Declare iteration counter variable */
  uint16_t i;
    
    /* Disable FCU interrupts in FCU block */
  FLASH.FAEINT.BIT.ROMAEIE = 0;
  FLASH.FAEINT.BIT.CMDLKIE = 0;
  FLASH.FAEINT.BIT.DFLAEIE = 0;
  FLASH.FAEINT.BIT.DFLRPEIE = 0;
  FLASH.FAEINT.BIT.DFLWPEIE = 0;
  
    /* Disable FCU interrupts in ICU */
    
    /* Disable flash interface error (FIFERR) */
//    IPR(FCU, FIFERR) = 0;
ICU.IPR[0x1].BIT.IPR = 0;
//    IEN(FCU, FIFERR) = 0;
ICU.IER[0x2].BIT.IEN5 = 0;
        
    #if defined(DATA_FLASH_BGO) || defined(ROM_BGO)
    /* Enable flash ready interrupt (FRDYI) */    
    /* Make sure IPL is high enough that interrupt will trigger */
//    IPR(FCU, FRDYI) = max(ROM_OPERATION_PIPL, DATA_FLASH_OPERATION_PIPL) + 1;
//    IEN(FCU, FRDYI) = 1;
ICU.IPR[0x2].BIT.IPR = max(ROM_OPERATION_PIPL, DATA_FLASH_OPERATION_PIPL) + 1;
ICU.IER[0x2].BIT.IEN7 = 1;
    #else        
    /* Disable flash ready interrupt (FRDYI) */
//    IPR(FCU, FRDYI) = 0;
ICU.IPR[0x2].BIT.IPR = 0;
//    IEN(FCU, FRDYI) = 0;
ICU.IER[0x2].BIT.IEN7 = 0;
    #endif    

  /* Transfer Firmware to the FCU RAM. To use FCU commands, the FCU firmware
      must be stored in the FCU RAM. */
  
  /* Before writing data to the FCU RAM, clear FENTRYR to stop the FCU. */
  if(FLASH.FENTRYR.WORD != 0x0000)
  {
    /* Disable the FCU from accepting commands - Clear both the
       FENTRY0(ROM) and FENTRYD(Data Flash) bits to 0 */
        FLASH.FENTRYR.WORD = 0xAA00;
    }
  
  /* Enable the FCU RAM */
  FLASH.FCURAME.WORD = 0xC401;
    
  /* Copies the FCU firmware to the FCU RAM.
     Source: H'FEFFE000 to H'FF00000 (FCU firmware area)
     Destination: H'007F8000 to H'007FA000 (FCU RAM area) */
  /* Set source pointer */
  src = (uint32_t *)FCU_PRG_TOP;
  
  /* Set destination pointer */
  dst = (uint32_t *)FCU_RAM_TOP;
  
  /* Iterate for loop to copy the FCU firmware */
  for( i=0; i<(FCU_RAM_SIZE/4); i++)
  {
    /* Copy data from the source to the destination pointer */
    *dst = *src;
    
    /* Increment the source and destination pointers */
    src++;
    dst++;
  }

  /* FCU firmware transfer complete, set the flag to 1 */
  fcu_transfer_complete = 1;

  /* Return no errors */
  return FLASH_SUCCESS;
}
/******************************************************************************
End of function  _Flash_Init
******************************************************************************/

/******************************************************************************
* Function Name: _DataFlash_Status_Clr
* Description  : Clear the status of the Data Flash operation.
*               NOTE: This function does not have to execute from in RAM.
* Arguments    : none
* Return Value : none
******************************************************************************/
static void _DataFlash_Status_Clr(void)
{
  /* Declare temporaty pointer */
    volatile uint8_t *ptrb;

    /* Set pointer to Data Flash to issue a FCU command if needed */
    ptrb = (uint8_t *)(DF_ADDRESS);

    /* Check to see if an error has occurred with the FCU.  If set, then
       issue a status clear command to bring the FCU out of the 
       command-locked state */
  if(FLASH.FSTATR0.BIT.ILGLERR == 1)  
    {
        /* FASTAT must be set to 0x10 before the status clear command
           can be successfully issued  */
    if(FLASH.FASTAT.BYTE != 0x10)
        {
            /* Set the FASTAT register to 0x10 so that a status clear
               command can be issued */
      FLASH.FASTAT.BYTE = 0x10;
    }
  }

    /* Issue a status clear command to the FCU */
    *ptrb = 0x50;
}
/******************************************************************************
End of function  _DataFlash_Status_Clr
******************************************************************************/


/******************************************************************************
* Function Name: _DataFlash_Write
* Description  : Write either 8-bytes of 128-bytes to Data Flash area.
*               NOTE: This function does not have to execute from in RAM.
* Arguments    : address - The address (in the Data Flash programming area) 
*                          to write the data to  
*                data - The address of the data to write
*                size - The size of the data to write. Must be set to 
*                        either '8' or '128'
* Return Value : FLASH_SUCCESS = Operation Successful
*                FLASH_FAILURE = Operation Failed
******************************************************************************/
static uint8_t _DataFlash_Write(uint32_t address, uint32_t data, uint8_t size)
{
  /* Declare wait counter variable */
    int32_t wait_cnt;
  
  /* Define loop iteration count variable */
    uint8_t n = 0;
    
    /* Check data size is valid */
    if((size == 128) || (size == 8))
  {
    /* Perform bit shift */
        size = (uint8_t)(size >> 1);
    
    /* Send command to data flash area */
        *(volatile uint8_t *)DF_ADDRESS = 0xE8;

        /* Specify data transfer size to data flash area */
        *(volatile uint8_t *)DF_ADDRESS = size;   
    
    /* Iterate through the number of data bytes */
    while(n++ < size)
    {
      /* Copy data from source address to destination area */
            *(volatile uint16_t *)address = *(uint16_t *)data;
    
      /* Increment data address by two bytes */
      data += 2;
        }

      /* Write the final FCU command for programming */
    *(volatile uint8_t *)(DF_ADDRESS) = 0xD0;
        
        #ifdef DATA_FLASH_BGO
        /* Program is ongoing, return */
        return FLASH_SUCCESS;
        #endif

        /* Set the wait counter with timeout value */
        wait_cnt = WAIT_MAX_DF_WRITE;

    /* Check if FCU has completed its last operation */
      while(FLASH.FSTATR0.BIT.FRDY == 0)
        {  
      /* Decrement the wait counter */
        wait_cnt--;
    
      /* Check if the wait counter has reached zero */
        if(wait_cnt == 0)
            {            
                /* Maximum time for writing a block has passed, 
                   operation failed, reset FCU */
          _Flash_Reset();
        
        /* Return FLASH_FAILURE, operation failure */
          return FLASH_FAILURE;
        }
      }

        /* Check for illegal command or programming errors */
      if((FLASH.FSTATR0.BIT.ILGLERR == 1) || (FLASH.FSTATR0.BIT.PRGERR == 1))
        {        
      /* Return FLASH_FAILURE, operation failure */
        return FLASH_FAILURE;
      }                
    }
   /* Data size is invalid */
    else
  {
        /*Return FLASH_FAILURE, operation failure */
        return FLASH_FAILURE;
    }
 
   /* Return FLASH_SUCCESS, operation success */
  return FLASH_SUCCESS;
}
/******************************************************************************
End of function  _DataFlash_Write
******************************************************************************/

/* If ROM_BGO is enabled then the following 2 functions also need to be in
   RAM in the case that the user calls them during a ROM operation. */
#ifdef ROM_BGO
#pragma section FRAM
#endif

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
uint8_t R_FlashDataAreaBlankCheck(uint32_t address, uint8_t size)
{
  /* Declare data flash pointer */
    volatile uint8_t *ptrb;
  
  /* Declare result container variable */
  uint8_t result;
    
    /* Declare wait counter variable */
    int32_t wait_cnt;
        
    /* Attempt to grab state */
    if( _FlashGrabState(FLASH_BLANKCHECK) != 0 )
    {
        /* Another operation is already in progress */
        return FLASH_BUSY;
    }
    
    /* Set current FCU mode to Data Flash PE Mode */
  current_mode = FLD_PE_MODE;

    /* Enter Data Flash PE mode in the FCU */
  if( _Enter_PE_Mode( address, 0 ) != 0)
  {
        /* Release state */
        _FlashReleaseState();
    
    /* Return FLASH_FAILURE, operation failure */
    return FLASH_FAILURE;
  }

  /* Set  bit FRDMD (bit 4) in FMODR to 1 */
  FLASH.FMODR.BIT.FRDMD = 1;

  /* Check if 8 byte size has been passed */
    if(size == BLANK_CHECK_8_BYTE)
  {
        #ifdef DATA_FLASH_BGO
        /* Disable FCU ready interrupt for 8 byte checks */
        FLASH.FRDYIE.BIT.FRDYIE = 0;
        #endif        
        
        /* Set data flash pointer to beginning of the memory block */
    ptrb = (uint8_t *)(address & DF_MASK);  
    
    /* Check if the next 8 bytes are blank
       Bits BCADR to the address of the 8-byte location
       to check (only bits A3-A12).
       Set bit BCSIZE in EEPBCCNT to 0. */
        FLASH.DFLBCCNT.WORD = (uint16_t)(address & (DF_BLOCK_SIZE-8));
    }
   /* Check entire data block */
  else
  {
        /* Check to see if user sent in a data block number or the 
           address.  The function description tells the user to send
           in the block number but in some early examples the address
           was used. To make this work with both versions we detect which
           is sent in below and make it work either way. */
        if( address < (DF_NUM_BLOCKS+ROM_NUM_BLOCKS) )
        {
            /* A data block number was sent in */
            ptrb = (uint8_t *)block_addresses[address];
        }           
        else if( (address >= DF_ADDRESS) ||
                 (address < DF_ADDRESS + DF_SIZE) )
        {
            /* Any address in the erasure block */
        ptrb = (uint8_t *)address;
        }
        else
        {
            /* Invalid 'address' argument input */
            return FLASH_FAILURE;
        }
    
    /* Check if the entire block is blank
       Set bit BCSIZE in EEPBCCNT to 1.
       Bits BCADR are not used, set to '0'. */
    FLASH.DFLBCCNT.WORD = 0x0001;
    }

    /* Send commands to FCU */
    *ptrb = 0x71;
    *ptrb = 0xD0;
    
    #ifdef DATA_FLASH_BGO
    /* Only using BGO on block blank checks */
    if( size != BLANK_CHECK_8_BYTE )
    {
        /* Return, check result later in ISR */
        return FLASH_BLANK;
    }
    #endif    

    /* Set timeout wait counter value */
    wait_cnt = WAIT_MAX_BLANK_CHECK;

    /* Wait until FCU operation finishes, or a timeout occurs */
    while(FLASH.FSTATR0.BIT.FRDY == 0)
    {
    /* Decrement the wait counter */
      wait_cnt--;

    /* Check if the wait counter has reached zero */
      if(wait_cnt == 0)
        {    
            /* Maximum timeout duration for writing to ROM has elapsed - 
               assume operation failure and reset the FCU */
        _Flash_Reset();

      /* Return FLASH_FAILURE, operation failure */
        return FLASH_FAILURE;
      }
    }

  /* Reset the FRDMD bit back to 0 */
  FLASH.FMODR.BIT.FRDMD = 0x00;

  /* Check if the 'ILGERR' was set during the command */
  if(FLASH.FSTATR0.BIT.ILGLERR == 1)  /* Check 'ILGERR' bit */
  {
    /* Take the FCU out of PE mode */
    _Exit_PE_Mode( );
    
        /* Release state */
        _FlashReleaseState();
    
    /* Return FLASH_FAILURE, operation failure */
    return FLASH_FAILURE;
    }

  /* (Read the 'BCST' bit (bit 0) in the 'DFLBCSTAT' register
       0=blank, 1=not blank */
  result = (uint8_t)(FLASH.DFLBCSTAT.BIT.BCST);  

  /* Take the FCU out of PE mode */
  _Exit_PE_Mode( );
  
    /* Release state */
    _FlashReleaseState();
  
  /* Return (Not Blank/Blank), operation successful */
    if( result == 0 ) {
        /* Block was blank */
        return FLASH_BLANK;
    }
    else
    {
        /* Block was not blank */
        return FLASH_NOT_BLANK;
    }    
}
/******************************************************************************
End of function  R_FlashDataAreaBlankCheck
******************************************************************************/

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
void R_FlashDataAreaAccess(uint16_t read_en_mask, uint16_t write_en_mask)
{
    #if   defined(RX610)
    
    /* Set Read access for the Data Flash blocks */
  FLASH.DFLRE.WORD = 0x2D00 | (read_en_mask & 0x00FF);

  /* Set Erase/Program access for the Data Flash blocks */
  FLASH.DFLWE.WORD = 0x1E00 | (write_en_mask & 0x00FF);
        
    #elif defined(RX621) || defined(RX62T) || defined(RX62N)
  
    /* Set Read access for the Data Flash blocks DB0-DB7 */
  FLASH.DFLRE0.WORD = (uint16_t)(0x2D00 | (read_en_mask & 0x00FF));
    
    /* Set Read access for the Data Flash blocks DB8-DB15 */
  FLASH.DFLRE1.WORD = (uint16_t)(0xD200 | ((read_en_mask >> 8) & 0x00FF));

  /* Set Erase/Program access for the Data Flash blocks DB0-DB7 */
  FLASH.DFLWE0.WORD = (uint16_t)(0x1E00 |  (write_en_mask & 0x00FF));

    /* Set Erase/Program access for the Data Flash blocks DB8-DB15 */
  FLASH.DFLWE1.WORD = (uint16_t)(0xE100 |  ((write_en_mask >> 8) & 0x00FF));
    
    #else    
        
    #error "!!! You must specify your device in Flash_API_RX6xx.h first !!!"
    
    #endif
}

/******************************************************************************
End of function  R_FlashDataAreaAccess
******************************************************************************/

/******************************************************************************
* Function Name: _ROM_Write
* Description  : Write 256 bytes to ROM Area Flash.
*                NOTE: This function MUST execute from in RAM.
* Arguments    : none
* Return Value : FLASH_SUCCESS = Operation Successful
*                FLASH_FAILURE = Operation Failed
******************************************************************************/
static uint8_t _ROM_Write(uint32_t address, uint32_t data)
{
  /* Declare iteration loop count variable */
  uint8_t i;
  
  /* Declare wait counter variable */
    int32_t wait_cnt;

  /* Write the FCU Program command */
    *(volatile uint8_t *)address = 0xE8;  
    *(volatile uint8_t *)address = 0x80;

    /* Write 256 bytes into flash, 16-bits at a time */
  for(i = 0; i < 128; i++)
  {
    /* Copy data from source address to destination ROM */
        *(volatile uint16_t *)address = *(uint16_t *)data;
    
    /* Increment destination address by 2 bytes */
    data += 2;
    }

    /* Write the final FCU command for programming */
  *(volatile uint8_t *)address = 0xD0;

    /* Set timeout wait counter value */
    wait_cnt = WAIT_MAX_ROM_WRITE;

    /* Wait until FCU operation finishes, or a timeout occurs */
    while(FLASH.FSTATR0.BIT.FRDY == 0)
    {
    /* Decrement the wait counter */
      wait_cnt--;

    /* Check if the wait counter has reached zero */
      if(wait_cnt == 0)
        {    
            /* Maximum timeout duration for writing to ROM has elapsed - 
               assume operation failure and reset the FCU */
        _Flash_Reset();

      /* Return FLASH_FAILURE, operation failure */
        return FLASH_FAILURE;
      }
    }

    /* Check for illegal command or programming errors */
    if((FLASH.FSTATR0.BIT.ILGLERR == 1) || (FLASH.FSTATR0.BIT.PRGERR  == 1)) 
    {        
    /* Return FLASH_FAILURE, operation failure */
      return FLASH_FAILURE;
    }                
    
  /* Return FLASH_SUCCESS, operation successful */
    return FLASH_SUCCESS;
}
/******************************************************************************
End of function  _ROM_Write
******************************************************************************/

/******************************************************************************
* Function Name: _Enter_PE_Mode
* Description  : Puts the FCU into program/erase mode.
*                NOTE: This function MUST execute from in RAM for 'ROM Area' 
*                programming, but if you are ONLY doing Data Flash programming,
*                this function can reside and execute in Flash.
* Arguments    : bytes - The number of bytes you are writing (if you are 
*                        writing).  
* Return Value : FLASH_SUCCESS = Operation Successful
*                FLASH_FAILURE = Operation Failed
******************************************************************************/
static uint8_t _Enter_PE_Mode( uint32_t flash_addr, uint16_t bytes )
{
    /* If FCU firmware has already been transferred to FCU RAM,
       no need to do it again */
  if( !fcu_transfer_complete )
  {
    /* Initialise the FCU, and store operation resilt in result variable */        
    /* Check if FCU initialisation was successful */
    if( _Flash_Init() != 0x00 )
    {
      /* FCU initialisiation failed - return operation failure */
      return FLASH_FAILURE;
    }
  }
    
  /* FENTRYR must be 0x0000 before bit FENTRY0 or FENTRYD can be set to 1 */
    FLASH.FENTRYR.WORD = 0xAA00;

  /* Execute null operations to allow FENTRYR register to settle */
    nop();
    nop();

    /* Save current processor IPL */
    flash_pipl = get_ipl();

  /* Check if FCU mode is set to ROM PE */
  if( current_mode == ROM_PE_MODE) 
    {
        #if defined(ROM_BGO)
        /* Don't enable flash interrupts if programming lock bits */
    if( flash_state != FLASH_LOCK_BIT )
    {    
          /* Re-enable the FRDYI interrupt */
          FLASH.FRDYIE.BIT.FRDYIE = 1;     
    }
        #else
        /* Disable the FRDYI interrupt */
        FLASH.FRDYIE.BIT.FRDYIE = 0;            
        #endif                       
                                    
        /* Set the processor IPL so that interrupts that access ROM will 
           not occur during ROM program/erase operations. */
        set_ipl(ROM_OPERATION_PIPL);    
                          
        /* Check which partition of flash this address is in */
        if(flash_addr >= 0x00F00000)
        {
            /* Enter ROM PE mode for addresses 0xFFF00000 - 0xFFFFFFFF */
            FLASH.FENTRYR.WORD = 0xAA01;
        } 
        else
        {            
            /* Enter ROM PE mode for addresses 0xFFE00000 - 0xFFEFFFFF */
            FLASH.FENTRYR.WORD = 0xAA02;
        }
                
    }
  /* Check if FCU mode is set to data flash PE */
  else if( current_mode == FLD_PE_MODE )
  {        
        #if defined(DATA_FLASH_BGO)
        /* Re-enable the FRDYI interrupt */
        FLASH.FRDYIE.BIT.FRDYIE = 1; 
        #else
        /* Disable the FRDYI interrupt */
        FLASH.FRDYIE.BIT.FRDYIE = 0; 
        #endif                               
        
        /* Save current processor IPL */
        flash_pipl = get_ipl();
        
        /* Set the processor IPL so that interrupts that access DF will 
           not occur during DF program/erase operations. */
        set_ipl(DATA_FLASH_OPERATION_PIPL);                                   
        
        /* Set FENTRYD bit(Bit 7) and FKEY (B8-15 = 0xAA) */
    FLASH.FENTRYR.WORD = 0xAA80;  

    /*  First clear the FCU's status before doing Data Flash programming.
        This is to clear out any previous errors that may have occured.
        For example, if you attempt to read the Data Flash area 
        before you make it readable using R_FlashDataAreaAccess(). */
    _DataFlash_Status_Clr();
        
    }
   /* Catch-all for invalid FCU mode */
   else
  {
    /* Invalid value of 'current_mode' */
    return FLASH_FAILURE;
  }
    
    /* Enable Write/Erase of ROM/Data Flash */
    FLASH.FWEPROR.BYTE = 0x01;

  /* Check for FCU error */
  if(    (FLASH.FSTATR0.BIT.ILGLERR == 1)
    ||  (FLASH.FSTATR0.BIT.ERSERR  == 1)
    ||  (FLASH.FSTATR0.BIT.PRGERR  == 1)
    ||  (FLASH.FSTATR1.BIT.FCUERR  == 1)) 
  {
        /* Return FLASH_FAILURE, operation failure */
    return FLASH_FAILURE;   
  }
    
    /* Check to see if peripheral clock notification command is needed */
    if( fcu_pclk_command == 0 )
    {
        /* Disable FCU interrupts, so interrupt will not trigger after
           peripheral clock notification command */
      FLASH.FRDYIE.BIT.FRDYIE = 0;
        
        /* Inform FCU of peripheral clock speed, check if operation is\
           succesful */
        if(_Notify_Peripheral_Clock((uint8_t *)flash_addr)!=0)
        {                
        /* Return FLASH_FAILURE, operation failure */
        return FLASH_FAILURE;
        }
        
        /* Only re-enable flash ready interrupt if using BGO */
        #ifdef DATA_FLASH_BGO
        if( current_mode == FLD_PE_MODE )
        {
            /* Re-enable the FRDYI interrupt */
          FLASH.FRDYIE.BIT.FRDYIE = 1;        
        }       
        #endif
        #ifdef ROM_BGO
        if( current_mode == ROM_PE_MODE )
        {
          /* Don't enable flash interrupts if programming lock bits */
      if( flash_state != FLASH_LOCK_BIT )
      {    
            /* Re-enable the FRDYI interrupt */
            FLASH.FRDYIE.BIT.FRDYIE = 1;     
      }                   
        }
        #endif
        
        /* No need to notify FCU of PCLK again */
        fcu_pclk_command = 1;
    }    

  /* Return FLASH_SUCCESS, operation successful */
  return FLASH_SUCCESS;
}
/******************************************************************************
End of function _Enter_PE_Mode
******************************************************************************/

/******************************************************************************
* Function Name: _Exit_PE_Mode
* Description  : Takes the FCU out of program/erase mode.
*                NOTE: This function MUST execute from in RAM for 'ROM Area' 
*                programming, but if you are ONLY doing Data Flash 
*                programming, this function can reside and execute in Flash.
* Arguments    : none
* Return Value : none
******************************************************************************/
static void _Exit_PE_Mode( void )
{
  /* Declare wait timer count variable */
  int32_t  wait_cnt;
    
  /* Declare address pointer */
  volatile uint8_t *pAddr;

  /* Set wait timer count duration */   
  wait_cnt = WAIT_MAX_ERASE;

  /* Iterate while loop whilst FCU operation is in progress */
  while(FLASH.FSTATR0.BIT.FRDY == 0)
    {
    /* Decrement wait timer count variable */
        wait_cnt--;              

    /* Check if wait timer count value has reached zero */
    if(wait_cnt == 0)
        {
      /* Timeout duration has elapsed, assuming operation failure and
         resetting the FCU */
            _Flash_Reset();      
      
      /* Break from the while loop prematurely */
      break;
    }
  }

    /* Check FSTATR0 and execute a status register clear command if needed */
  if(    (FLASH.FSTATR0.BIT.ILGLERR == 1) 
    ||  (FLASH.FSTATR0.BIT.ERSERR  == 1) 
    ||  (FLASH.FSTATR0.BIT.PRGERR  == 1)) 
    {

    /* Clear ILGLERR */
        if(FLASH.FSTATR0.BIT.ILGLERR == 1)
        {
            /* FASTAT must be set to 0x10 before the status clear command
               can be successfully issued  */
      if(FLASH.FASTAT.BYTE != 0x10)
            {
                /* Set the FASTAT register to 0x10 so that a status clear
                    command can be issued */
        FLASH.FASTAT.BYTE = 0x10;
      }
    }
    
        /* Issue a status register clear command to clear all error bits */
    if( current_mode == ROM_PE_MODE )
    {
      /* If ROM is greater than 1MB then we need to check and make
         sure we are sending the status register clear command
         to the correct section. */
      if( ROM_SIZE > 1048576 )
      {
        /* See which ROM area is being accessed */               
        if( FLASH.FENTRYR.BIT.FENTRY0 == 1 )
        {
          /* ROM within 1MB of 'top' of flash is being used so
             use an address within that area. */
          pAddr = (uint8_t *)(ROM_PE_ADDR + (ROM_SIZE/2));
        }        
        else
        {       
          /* Set address pointer to ROM area */
          pAddr = (uint8_t *)ROM_PE_ADDR;
        }
      }
      else
      {
        /* ROM size is less than or equal to 1MB which means there is only 
           one section. */
        /* Set address pointer to ROM area */
        pAddr = (uint8_t *)ROM_PE_ADDR;
      }

            /* Send status clear command to FCU */
      *pAddr = 0x50;
    }
    else
    {
      /* Set address pointer to data flash area */
      pAddr = (uint8_t *)DF_ADDRESS;
      
            /* Send status clear command to FCU */
      *pAddr = 0x50;
    }
  }

  /* Enter ROM Read mode */
    FLASH.FENTRYR.WORD = 0xAA00;    

    /* Disable the FRDYI interrupt */
    FLASH.FRDYIE.BIT.FRDYIE = 0;      
    
    /* Flash write/erase disabled */
  FLASH.FWEPROR.BYTE = 0x02;

    /* Restore processor IPL */
    set_ipl(flash_pipl);        

}
/******************************************************************************
End of function  _Exit_PE_Mode
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
uint8_t R_FlashErase(uint8_t block)
{
  /* Declare address pointer */
  volatile uint8_t *pAddr;
  
  /* Declare erase operation result container variable */
  uint8_t result = FLASH_SUCCESS;
  
  /* Declare timer wait count variable */
    int32_t wait_cnt;
    
    /* Attempt to grab state */
    if( _FlashGrabState(FLASH_ERASING) != 0 )
    {
        /* Another operation is already in progress */
        return FLASH_BUSY;
    }
    
    /* Erase Command Address */
  pAddr = (volatile uint8_t *)(block_addresses[ block ]);

     /* Do we want to erase a Data Flash block or ROM block? */
  if( block >= BLOCK_DB0 )
  {
    /* Set current FCU mode to data flash PE */
    current_mode = FLD_PE_MODE;
  }
  else
  {
    /* Set current FCU mode to ROM PE */
    current_mode = ROM_PE_MODE;
  }

    /* Enter ROM PE mode, check if operation successful */
  if( _Enter_PE_Mode((uint32_t)pAddr, 0) != 0)        
  {
        /* Release state */
        _FlashReleaseState();
    
    /* Return FLASH_FAILURE, operation failure */
    return FLASH_FAILURE;
  }

    #ifdef  IGNORE_LOCK_BITS
  /* Cancel the ROM Protect feature 
     NOTE: If you are using block locking feature to protect your data,
     then comment out 'IGNORE_LOCK_BITS' in Flash_API_RX600_UserConfig.h */
  FLASH.FPROTR.WORD = 0x5501;
    #else
    /* Only disable lock bit protection if user has specified to 
       do so earlier */
    if( lock_bit_protection == false )
    {
        /* Cancel the ROM Protect feature */
      FLASH.FPROTR.WORD = 0x5501;    
    }       
    #endif

    /* Send the FCU Command */
    *pAddr = 0x20;
    *pAddr = 0xD0;

    #if defined(ROM_BGO)
    if( current_mode == ROM_PE_MODE )
    {
        /* Return, check result later in ISR */
        return FLASH_SUCCESS;
    }
    #endif    
    
    #if defined(DATA_FLASH_BGO)
    if( current_mode == FLD_PE_MODE )
    {
        /* Return, check result later in ISR */
        return FLASH_SUCCESS;
    }
    #endif    

    /* Set the wait counter timeout duration */
  wait_cnt = WAIT_MAX_ERASE;

  /* Wait while FCU operation is in progress */
  while(FLASH.FSTATR0.BIT.FRDY == 0)
    {
    /* Decrement the wait counter */
    wait_cnt--;
    
    /* Check if the wait counter has reached zero */
    if(wait_cnt == 0){

            /* Maximum timeout duration for erasing a ROM block has 
               elapsed, assuming operation failure - reset the FCU */
      _Flash_Reset();

      /* Return FLASH_FAILURE, operation failure */
      return FLASH_FAILURE;
    }
  }

  /* Check if erase operation was successful by checking 
     bit 'ERSERR' (bit5) and 'ILGLERR' (bit 6) of register 'FSTATR0' */
  /* Check FCU error */
  if((FLASH.FSTATR0.BIT.ILGLERR == 1) || (FLASH.FSTATR0.BIT.ERSERR  == 1)) 
    {
        result = FLASH_FAILURE;
    }
 
  /* Leave Program/Erase Mode */
   _Exit_PE_Mode();

    /* Release state */
    _FlashReleaseState();

  /* Return erase result, operation succesful */
  return result;
}
/******************************************************************************
End of function  R_FlashErase
******************************************************************************/

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
uint8_t R_FlashWrite ( uint32_t flash_addr, uint32_t buffer_addr,
                       uint16_t bytes )
{
    /* Local variable */
    uint16_t i;  
    /* Declare result container and number of bytes to write variables */
  uint8_t result = FLASH_SUCCESS, num_byte_to_write;    
        
    /* Take off upper byte since for programming/erase addresses for ROM are 
    the same as read addresses except upper byte is masked off to 0's.
    Data Flash addresses are not affected. */
    flash_addr &= 0x00FFFFFF;

  /* ROM area or Data Flash area - Check for DF first */  
  if( flash_addr >= DF_ADDRESS && flash_addr < DF_ADDRESS + DF_SIZE )
  {
    /* Check if the number of bytes were passed is a multiple of 8 */
    if( bytes & 0x07 )
    {
      /* Return number of bytes not a multiple of 8 */
      return FLASH_ERROR_BYTES;
    }
    /* Check for an address on a 8 byte page boundary. */
    if( (uint16_t)flash_addr & 0x07)
    {
      /* Return address not on an 8 byte boundary */
      return FLASH_ERROR_ALIGNED;
    }
  }
    /* Check for ROM area */
  else if(  (flash_addr >= ROM_PE_ADDR)
      &&  (flash_addr < ROM_PE_ADDR + ROM_SIZE) )
  {
    /* Check if the number of bytes were passed is a multiple of 256 */
    if( bytes & 0xFF )
    {
      /* Return number of bytes not a multiple of 256 */
      return FLASH_ERROR_BYTES;
    }

    /* Check for an address on a 256 byte page */
    if( (uint16_t)flash_addr & 0xFF)
    {
      /* Return address not on a 256 byte boundary */
      return FLASH_ERROR_ALIGNED;  
    }
        
        /* Check for passing over 1MB boundary at 0xFFF00000 */
        if(    (flash_addr < (0x00F00000))
      &&  ((flash_addr + bytes) > 0x00F00000) )
        {
      /* Return cannot write across a 1MB boundary */
            return FLASH_ERROR_BOUNDARY;
        } 
  }
  else 
    {
    /* Return invalid flash address */
        return FLASH_ERROR_ADDRESS;
    }
    
    /* Attempt to grab state */
    if( _FlashGrabState(FLASH_WRITING) != 0 )
    {
        /* Another operation is already in progress */
        return FLASH_BUSY;
    }
    
  /* Do we want to program a DF area or ROM area? */
  if( flash_addr < block_addresses[ROM_NUM_BLOCKS-1] )
  {
    /* Set current FCU mode to data flash PE */
    current_mode = FLD_PE_MODE;
  }
  else
  {
    /* Set FCU to ROM PE mode */
    current_mode = ROM_PE_MODE;
  }     
    
#ifdef FLASH_TO_FLASH    
    /* Are we doing a ROM to ROM or DF to DF transfer? */
    if( (buffer_addr >= ROM_PE_ADDR) &&
        (current_mode == ROM_PE_MODE) )
    {
        /* Performing ROM to ROM transfer */
        
        /* Copy over first 256 bytes */
        for(i = 0; i < 256; i++)
        {
            /* Copy over each byte */
            temp_array[i] = *((uint8_t *)(buffer_addr+i));             
        }

        /* Check size */
        if( bytes == 256 )
        {        
            /* Since we were able to put the entire buffer in RAM we can
               just change the buffer pointer and go on as usual. */
            flash_to_flash_op = 0;
            
            /* Set new buffer address */
            buffer_addr = (uint32_t)&temp_array[0];
        }        
        else
        {
            /* Set FLASH_TO_FLASH flag */
            flash_to_flash_op = 1;   
        }
    }
    else if( (buffer_addr < ROM_PE_ADDR) &&
             (buffer_addr >= DF_ADDRESS) &&
             (current_mode == FLD_PE_MODE) )
    {
        /* Performing DF to DF transfer */

        /* Check if address is on a 128 byte boundary, and there are
       at least 128 bytes remaining to write */
    if( (bytes >= 128) && (((uint8_t)flash_addr & 0x7F) == 0x00))
    {
      /* Set the next data transfer size to be 128 */
      num_byte_to_write = 128;
    }
    else
    {  /* Set the next data transfer size to be 8 */
      num_byte_to_write = 8;
    }      
                       
        /* Copy over first buffer */
        for(i = 0; i < num_byte_to_write; i++)
        {
            /* Copy over each byte */
            temp_array[i] = *((uint8_t *)(buffer_addr+i));             
        }

        /* Check size */
        if( bytes == num_byte_to_write )
        {        
            /* Since we were able to put the entire buffer in RAM we can
               just change the buffer pointer and go on as usual. */
            flash_to_flash_op = 0;
            
            /* Set new buffer address */
            buffer_addr = (uint32_t)&temp_array[0];
        }        
        else
        {
            /* Set FLASH_TO_FLASH flag */
            flash_to_flash_op = 1;   
        }        
    }
    else
    {
        /* No need to buffer */
        flash_to_flash_op = 0;
    }
    
           
#endif    

  /* Enter PE mode, check if operation is successful */
  if( _Enter_PE_Mode( flash_addr, bytes ) != 0)
  {
        /* Release state */
        _FlashReleaseState();
            
    /* Return operation failure */
    return FLASH_FAILURE;
  }
    
    #ifdef  IGNORE_LOCK_BITS
  /* Cancel the ROM Protect feature 
     NOTE: If you are using block locking feature to protect your data,
     then comment out 'IGNORE_LOCK_BITS' in Flash_API_RX600_UserConfig.h */
  FLASH.FPROTR.WORD = 0x5501;
    #else
    /* Only disable lock bit protection if user has specified to 
       do so earlier */
    if( lock_bit_protection == false )
    {
        /* Cancel the ROM Protect feature */
      FLASH.FPROTR.WORD = 0x5501;    
    }       
    #endif
    
    
  /* Check if FCU mode is set to data flash PE */
  if( current_mode == FLD_PE_MODE )
  {
    /* Iterate while there are still bytes remaining to write */
        while( bytes )
    {
      /* Check if address is on a 128 byte boundary, and there are
         at least 128 bytes remaining to write */
      if( (bytes >= 128) && (((uint8_t)flash_addr & 0x7F) == 0x00))
      {
        /* Set the next data transfer size to be 128 */
        num_byte_to_write = 128;
      }
      else
      {  /* Set the next data transfer size to be 8 */
        num_byte_to_write = 8;
      }

            #ifdef DATA_FLASH_BGO
            /* At this point we are going to set some global variables which
               will continue on with write after these bytes are written. 
               I set these values before the actual first write because I was 
               getting an error when the flash ready interrupt was being called
               before these values were set. */        
            bgo_flash_addr = flash_addr + num_byte_to_write;               
            bgo_buffer_addr = buffer_addr + num_byte_to_write;
            bgo_bytes = bytes - num_byte_to_write;
            
            #endif

      /* Call the Programming function, store the operation status in the
         container variable, result */
            #ifdef FLASH_TO_FLASH      
            if( flash_to_flash_op == 1 )
            {
          result = _DataFlash_Write( flash_addr, (uint32_t)&temp_array[0],
                                     num_byte_to_write);                
            }
            else
            {
          result = _DataFlash_Write( flash_addr, buffer_addr,
                                     num_byte_to_write);                
            }
            #else                           
      result = _DataFlash_Write( flash_addr, buffer_addr,
                                 num_byte_to_write);
            #endif                                       

      /* Check the container variable result for errors */
      if( result != FLASH_SUCCESS )
      {
        /* Data flash write error detected, break from flash write
           while loop prematurely */
        break;
      }

            #ifdef DATA_FLASH_BGO            
            /* Return FLASH_SUCCESS, rest of programming will be done 
               in interrupt */
            return FLASH_SUCCESS;
            #else

      /* Increment the flash address and the buffer address by the size
         of the transfer thats just completed */
      flash_addr += num_byte_to_write;
      buffer_addr += num_byte_to_write;
      
      /* Decrement the number of bytes remaining by the size of the last
         flash write */
      bytes -= num_byte_to_write;
                        
            #ifdef FLASH_TO_FLASH
            /* Check to see if we need to buffer more data */
            if( (bytes > 0) &&
                (flash_to_flash_op == 1) )
            {
                /* We must leave PE mode to transfer next buffer to RAM */
                _Exit_PE_Mode();                
                
                /* Copy over up to next 256 bytes */
                /* Copy over next 256 bytes */
                for(i = 0; i < min(bytes, 256); i++)
                {
                    /* Copy over each byte */
                    temp_array[i] = *((uint8_t *)(buffer_addr+i));             
                }

              /* Re-enter PE mode, check if operation is successful */
              if( _Enter_PE_Mode( flash_addr, bytes ) != 0)
              {
                    /* Release state */
                    _FlashReleaseState();
    
                /* Return operation failure */
                return FLASH_FAILURE;
              }                
            }
            #endif   
            #endif           
    }
  }
  /* Catch-all - FCU mode must be ROM PE */
  else
  {
    /* Iterate loop while there are still bytes remaining to write */
    while( bytes )
    {         
            #ifdef ROM_BGO
            /* At this point we are going to set some global variables which
               will continue on with write after these bytes are written. 
               I set these values before the actual first write because I was 
               getting an error when the flash ready interrupt was being called
               before these values were set. */        
            bgo_flash_addr = flash_addr + 256;               
            bgo_buffer_addr = buffer_addr + 256;
            bgo_bytes = bytes - 256;
            
            #endif
            
      /* Call the Programming function */
            #ifdef FLASH_TO_FLASH      
            if( flash_to_flash_op == 1 )
            {
                /* Use RAM array */
                result = _ROM_Write(flash_addr, (uint32_t)&temp_array[0]);
            }
            else
            {          
                /* Go as usual */
                result = _ROM_Write(flash_addr, buffer_addr);
            }
            #else            
            result = _ROM_Write(flash_addr, buffer_addr);
            #endif

      /* Check for errors */
      if( result != FLASH_SUCCESS )
      {
                /* Stop programming */
        break;  
      }            

            #ifdef ROM_BGO            
            /* Return, rest of programming will be done in interrupt */
            return FLASH_SUCCESS;
            #else

      /* Increment the flash address and the buffer address by the size
         of the transfer thats just completed */
      flash_addr += 256;
      buffer_addr += 256;
      
      /* Decrement the number of bytes remaining by the size of the last
         flash write */    
      bytes -= 256;
                                    
            #ifdef FLASH_TO_FLASH
            /* Check to see if we need to buffer more data */
            if( (bytes > 0) &&
                (flash_to_flash_op == 1) )
            {
                /* We must leave PE mode to transfer next buffer to RAM */
                _Exit_PE_Mode();                
                
                /* Copy over NEXT 256 bytes */
                for(i = 0; i < 256; i++)
                {
                    /* Copy over each byte */
                    temp_array[i] = *((uint8_t *)(buffer_addr+i));             
                }                

              /* Re-enter PE mode, check if operation is successful */
              if( _Enter_PE_Mode( flash_addr, bytes ) != 0)
              {
                    /* Release state */
                    _FlashReleaseState();
    
                /* Return operation failure */
                return FLASH_FAILURE;
              }                
            }
            #endif   
            #endif         
    }
  }

  /* Leave Program/Erase Mode */
  _Exit_PE_Mode();
    
    /* Release state */
    _FlashReleaseState();

  /* Return flash programming result */
  return result;
}
/******************************************************************************
End of function  R_FlashWrite
******************************************************************************/

/******************************************************************************
* Function Name: _Notify_Peripheral_Clock
* Description  : Notifies FCU or Peripheral Clock (PCLK)
*                NOTE: This function MUST execute from in RAM for 'ROM Area' 
*                programming, but if you are ONLY doing Data Flash programming,
*                this function can reside and execute in Flash.
* Arguments    : flash_addr - Flash address you will be erasing or writing to
* Return Value : FLASH_SUCCESS = Operation Successful
*                FLASH_FAILURE = Operation Failed
******************************************************************************/
static uint8_t _Notify_Peripheral_Clock(uint8_t * flash_addr)
{
  /* Declare wait counter variable */
  int32_t wait_cnt;

  /* Notify Peripheral Clock(PCK) */
    /* Set frequency of PCK */
  FLASH.PCKAR.WORD = PCLK_FREQUENCY;        

  /* Execute Peripheral Clock Notification Commands */
    *flash_addr = 0xE9;      
  *flash_addr = 0x03;
  *(uint16_t *)flash_addr = 0x0F0F;
  *(uint16_t *)flash_addr = 0x0F0F;
  *(uint16_t *)flash_addr = 0x0F0F;
  *flash_addr = 0xD0;

  /* Set timeout wait duration */
  wait_cnt = WAIT_MAX_NOTIFY_PERIPHERAL_CLOCK;

    /* Check FRDY */
  while(FLASH.FSTATR0.BIT.FRDY == 0)
    {
    /* Decrement wait counter */
        wait_cnt--;                
    
    /* Check if wait counter has reached zero */
    if(wait_cnt == 0)
        {
      /* Timeout duration elapsed, assuming operation failure - Reset
         FCU */
            _Flash_Reset();                

      /* Return FLASH_FAILURE, operation failure  */
      return FLASH_FAILURE;
    }
  }

    /* Check ILGLERR */
  if(FLASH.FSTATR0.BIT.ILGLERR == 1)
    {
        /* Return FLASH_FAILURE, operation failure*/
    return FLASH_FAILURE;
  }

    /* Return FLASH_SUCCESS, operation success */
  return FLASH_SUCCESS;
}
/******************************************************************************
End of function _Notify_Peripheral_Clock
******************************************************************************/

/******************************************************************************
* Function Name: _Flash_Reset
* Description  : Reset the FCU.
*                NOTE: This function MUST execute from in RAM for 'ROM Area' 
*                programming, but if you are ONLY doing Data Flash 
*                programming, this function can reside and execute in Flash.
* Arguments    : none
* Return Value : none
******************************************************************************/
static void _Flash_Reset(void)
{
  /* Declare wait counter variable */
  int32_t  wait_cnt;

    /* Reset the FCU */
  FLASH.FRESETR.WORD = 0xCC01;

    /* Give FCU time to reset */
  wait_cnt = WAIT_tRESW;

  /* Wait until the timer reaches zero */
  while(wait_cnt != 0)
    {
    /* Decrement the timer count each iteration */
    wait_cnt--;
  }

    /* Release state */
    _FlashReleaseState();

    /* FCU is not reset anymore */
  FLASH.FRESETR.WORD = 0xCC00;
    
  /* Enter ROM Read mode */
    FLASH.FENTRYR.WORD = 0xAA00;        
    
    /* Flash write/erase disabled */
  FLASH.FWEPROR.BYTE = 0x02;    
}
/******************************************************************************
End of function  _Flash_Reset
******************************************************************************/

#if defined(DATA_FLASH_BGO) || defined(ROM_BGO)
/******************************************************************************
* Function Name: FlashReadyInterrupt
* Description  : ISR that is called when FCU is done with flash operation
* Arguments    : none
* Return Value : none
******************************************************************************/
#pragma interrupt FlashReadyInterrupt(vect=VECT(FCU, FRDYI))
void FlashReadyInterrupt(void)
{
    /* Local variables */    
    uint32_t num_byte_to_write;
    uint32_t i;
    uint8_t ret;
    
    /* Check for any errors */
    if( (FLASH.FSTATR0.BIT.ILGLERR == 1) || 
        (FLASH.FSTATR0.BIT.ERSERR  == 1) ||
        (FLASH.FSTATR0.BIT.PRGERR == 1) )
    {
        /* Leave Program/Erase Mode and clear any error flags */
       _Exit_PE_Mode();          
        
        /* Release flash state */
        _FlashReleaseState();
  
    /* Operation failure, use callback function to alert user */
    FlashError();
        
        /* Exit ISR */
        return;
    }
    
    /* Check state and see if anything else needs to be done */
    if( flash_state == FLASH_ERASING )
    {
        /* Erase is done */ 
      /* Leave Program/Erase Mode */
       _Exit_PE_Mode();

        /* Release flash state */
        _FlashReleaseState();
        
        /* Flash operation finished callback function */
        FlashEraseDone(); 
    }
    #ifdef DATA_FLASH_BGO
    else if( flash_state == FLASH_BLANKCHECK )
    {
        /* Blank check finished, check result */
        
      /* Reset the FRDMD bit back to 0 */
      FLASH.FMODR.BIT.FRDMD = 0x00;

      /* (Read the 'BCST' bit (bit 0) in the 'DFLBCSTAT' register
           0=blank, 1=not blank */
      ret = FLASH.DFLBCSTAT.BIT.BCST;  

      /* Take the FCU out of PE mode */
      _Exit_PE_Mode( );
  
        /* Release flash state */
        _FlashReleaseState();
        
        /* Return result, 0 means blank, 1 means not blank */
        if(ret == 0) {
            FlashBlankCheckDone(FLASH_BLANK);        
        }
        else
        {
            FlashBlankCheckDone(FLASH_NOT_BLANK);        
        }
    }
    #endif
    else if( flash_state == FLASH_WRITING )
    {        
    /* Iterate while there are still bytes remaining to write */
        if( bgo_bytes > 0 )
    {
            /* Are we writing DF or ROM? */
            if( current_mode == FLD_PE_MODE )
            {
                /* Writing DF */
          /* Check if address is on a 128 byte boundary, and there are
             at least 128 bytes remaining to write */
          if( (bgo_bytes >= 128) && (((uint8_t)bgo_flash_addr & 0x7F) == 0x00))
          {
            /* Set the next data transfer size to be 128 */
            num_byte_to_write = 128;
          }
          else
          {  /* Set the next data transfer size to be 8 */
            num_byte_to_write = 8;
          }
            }
            else
            {
                /* Writing ROM, always write 256 bytes */
                num_byte_to_write = 256;
            }

            #ifdef FLASH_TO_FLASH
            /* Check to see if we need to buffer more data */
            if( flash_to_flash_op == 1 )
            {
                /* We must leave PE mode to transfer next buffer to RAM */
                _Exit_PE_Mode();                
                
                /* Copy over next bytes to write */
                for(i = 0; i < num_byte_to_write; i++)
                {
                    /* Copy over each byte */
                    temp_array[i] = *((uint8_t *)(bgo_buffer_addr+i));             
                }

              /* Re-enter PE mode, check if operation is successful */
              if( _Enter_PE_Mode( bgo_flash_addr, bgo_bytes ) != FLASH_SUCCESS)
              {
                    /* Leave Program/Erase Mode and clear any error flags */
                   _Exit_PE_Mode();          
        
                    /* Release flash state */
                    _FlashReleaseState();
  
                /* Operation failure, use callback function to alert user */
                FlashError();
        
                    /* Exit ISR */
                    return;
              }                
            }
            #endif                          

            /* Are we writing DF or ROM? */
            if( current_mode == FLD_PE_MODE )
            {
                /* Writing DF */
          /* Call the Programming function again for next bytes */
                #ifdef FLASH_TO_FLASH      
                if( flash_to_flash_op == 1 )
                {
              ret = _DataFlash_Write( bgo_flash_addr, (uint32_t)&temp_array[0],
                                         num_byte_to_write);                
                }
                else
                {
              ret = _DataFlash_Write( bgo_flash_addr, bgo_buffer_addr,
                                         num_byte_to_write);                
                }
                #else                           
          ret = _DataFlash_Write( bgo_flash_addr, bgo_buffer_addr,
                                     num_byte_to_write);
                #endif     
            }                 
            else
            {
                /* Writing ROM */
          /* Call the Programming function */
                #ifdef FLASH_TO_FLASH      
                if( flash_to_flash_op == 1 )
                {
                    /* Use RAM array */
                    ret = _ROM_Write(bgo_flash_addr, (uint32_t)&temp_array[0]);
                }
                else
                {          
                    /* Go as usual */
                    ret = _ROM_Write(bgo_flash_addr, bgo_buffer_addr);
                }
                #else            
                ret = _ROM_Write(bgo_flash_addr, bgo_buffer_addr);
                #endif                
            }                 

      /* Check the result for errors */
      if( ret != FLASH_SUCCESS )
      {
                /* Error detected during programming, stop and return */
                /* Leave Program/Erase Mode and clear any error flags */
               _Exit_PE_Mode();          
        
                /* Release flash state */
                _FlashReleaseState();
  
            /* Operation failure, use callback function to alert user */
            FlashError();
        
                /* Exit ISR */
                return;
      }

      /* Increment the flash address and the buffer address by the size
         of the transfer thats just completed */
      bgo_flash_addr += num_byte_to_write;
      bgo_buffer_addr += num_byte_to_write;
      
      /* Decrement the number of bytes remaining by the size of the last
         flash write */
      bgo_bytes -= num_byte_to_write;  
                                  
        }
        else
        {
          /* Leave Program/Erase Mode */
          _Exit_PE_Mode();

            /* Release flash state */
            _FlashReleaseState();
            
            /* Flash operation finished callback function */
            FlashWriteDone(); 
        }
        
    }
    else
    {
        /* Error, should never get here */
        while(1);
    }
}
/******************************************************************************
End of function  FlashReadyInterrupt
******************************************************************************/

#endif

/******************************************************************************
* Function Name: R_FlashGetStatus
* Description  : Returns the current state of the flash
* Arguments    : none
* Return Value : FLASH_SUCCESS = Flash is ready to use
*                FLASH_BUSY = Flash is busy with another operation
******************************************************************************/
uint8_t R_FlashGetStatus(void)
{

    /* Return flash status */
    if( flash_state == FLASH_READY )
    {
        return FLASH_SUCCESS;
    }
    else
    {
        return FLASH_BUSY;
    }

}
/******************************************************************************
End of function  R_FlashGetStatus
******************************************************************************/

/******************************************************************************
* Function Name: _FlashGrabState
* Description  : Attempt to grab the flash state to perform an operation
* Arguments    : Which state to attempt to transition to
* Return Value : FLASH_SUCCESS = State was grabbed
*                FLASH_BUSY = Flash is busy with another operation
******************************************************************************/
static uint8_t _FlashGrabState(enum Flash_States new_state)
{
    /* Variable used in trying to grab semaphore. Using the xchg instruction 
       makes this atomic */
    signed long semaphore = 1;
    
    /* Try to grab semaphore to change state */
    xchg(&semaphore, &flash_semaphore);
    
    /* Check to see if semaphore was successfully taken */
    if( semaphore == 0 )
    {
        /* Semaphore grabbed, we can change state */
        flash_state = new_state;
        
        /* Return success */
        return FLASH_SUCCESS;
    }
    else
    {
        /* Another operation is on-going */
        return FLASH_BUSY;
    }
}
/******************************************************************************
End of function  _FlashGrabState
******************************************************************************/

/******************************************************************************
* Function Name: _FlashReleaseState
* Description  : Release state so another flash operation can take place
* Arguments    : none
* Return Value : none
******************************************************************************/
static void _FlashReleaseState(void)
{
  /* Set current FCU mode to READ */
  current_mode = READ_MODE;            
    
    /* Done with programming */
    flash_state = FLASH_READY; 

    /* Release hold on semaphore */
    flash_semaphore = 0;
}
/******************************************************************************
End of function  _FlashReleaseState
******************************************************************************/

#ifndef  IGNORE_LOCK_BITS

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
uint8_t R_FlashProgramLockBit(uint8_t block)
{
  /* Declare address pointer */
  volatile uint8_t *pAddr;
  
  /* Declare operation result container variable */
  uint8_t result = FLASH_SUCCESS;
  
  /* Declare timer wait count variable */
    int32_t wait_cnt;
    
    /* Attempt to grab state */
    if( _FlashGrabState(FLASH_LOCK_BIT) != 0 )
    {
        /* Another operation is already in progress */
        return FLASH_BUSY;
    }
    
    /* Flash Command Address */
  pAddr = (volatile uint8_t *)(block_addresses[ block ]);
    
    /* Set FCU to ROM PE mode */
    current_mode = ROM_PE_MODE;    
    
    /* Enter ROM PE mode, check if operation successful */
  if( _Enter_PE_Mode((uint32_t)pAddr, 0) != 0)        
  {
        /* Release state */
        _FlashReleaseState();
    
    /* Return FLASH_FAILURE, operation failure */
    return FLASH_FAILURE;
  }
    
    /* Send the FCU Command */
    *pAddr = 0x77;
    *pAddr = 0xD0;

    /* Set timeout wait counter value */
    wait_cnt = WAIT_MAX_ROM_WRITE;

    /* Wait until FCU operation finishes, or a timeout occurs */
    while(FLASH.FSTATR0.BIT.FRDY == 0)
    {
    /* Decrement the wait counter */
      wait_cnt--;

    /* Check if the wait counter has reached zero */
      if(wait_cnt == 0)
        {    
            /* Maximum timeout duration for writing to ROM has elapsed - 
               assume operation failure and reset the FCU */
        _Flash_Reset();

      /* Return FLASH_FAILURE, operation failure */
        result = FLASH_FAILURE;
      }
    }

    /* Check for illegal command or programming errors */
    if((FLASH.FSTATR0.BIT.ILGLERR == 1) || (FLASH.FSTATR0.BIT.PRGERR  == 1)) 
    {        
    /* Return FLASH_FAILURE, operation failure */
      result = FLASH_FAILURE;
    }      
    
  /* Leave Program/Erase Mode */
  _Exit_PE_Mode();
    
    /* Release state */
    _FlashReleaseState();              
    
  /* Return result of lock bit programming */
    return result;
}
/******************************************************************************
End of function  R_FlashProgramLockBit
******************************************************************************/

/******************************************************************************
* Function Name: R_FlashReadLockBit
* Description  : Reads and returns the lock bit status for a ROM block
* Arguments    : block - Which ROM erasure block to read the lock bit of
* Return Value : FLASH_LOCK_BIT_SET = Lock bit was set
*                FLASH_LOCK_BIT_NOT_SET = Lock bit was not set
*                FLASH_FAILURE = Operation Failed
*                FLASH_BUSY = Another flash operation is in progress
******************************************************************************/
uint8_t R_FlashReadLockBit(uint8_t block)
{
  /* Declare address pointer */
  volatile uint8_t *pAddr;
  
  /* Declare operation result container variable */
  uint8_t result = FLASH_SUCCESS;
      
    /* Holds outcome of lock-bit read */
    uint8_t lock_bit;
    
    /* Attempt to grab state */;
    if( _FlashGrabState(FLASH_LOCK_BIT) != 0 )
    {
        /* Another operation is already in progress */
        return FLASH_BUSY;
    }
    
    /* Flash Command Address */
  pAddr = (volatile uint8_t *)(block_addresses[ block ]);
    
    /* Set FCU to ROM PE mode */
    current_mode = ROM_PE_MODE;

    /* Enter ROM PE mode, check if operation successful */
  if( _Enter_PE_Mode((uint32_t)pAddr, 0) != 0)        
  {
        /* Release state */
        _FlashReleaseState();
    
    /* Return FLASH_FAILURE, operation failure */
    return FLASH_FAILURE;
  }
    
    /* Switch to ROM Lock-Bit Read Mode */
    *pAddr = 0x71;
    
    /* Check for illegal command */
    if(FLASH.FSTATR0.BIT.ILGLERR == 1) 
    {        
    /* Return FLASH_FAILURE, operation failure */
      result = FLASH_FAILURE;        
    }      
    else
    {       
        /* Read lock bit value */
        lock_bit = *pAddr;
        
        /* Set return value */
        if(lock_bit != 0x00)
        {
            /* Lock bit is not set */
            result = FLASH_LOCK_BIT_NOT_SET;
        }
        else
        {
            /* Lock bit is set */
            result = FLASH_LOCK_BIT_SET;
        }
    }
              
  /* Leave Program/Erase Mode */
  _Exit_PE_Mode();
    
    /* Release state */
    _FlashReleaseState();   
        
  /* Return result of lock bit programming */
    return result;
}
/******************************************************************************
End of function  R_FlashReadLockBit
******************************************************************************/

/******************************************************************************
* Function Name: R_FlashSetLockBitProtection
* Description  : Enables or disables lock bit protection.
* Arguments    : lock_bit - Boolean, whether to enable or disable lock bit 
*                           protection (true = enabled, false = disabled)
* Return Value : FLASH_SUCCESS = Operation Successful
*                FLASH_BUSY = Another flash operation is in progress
******************************************************************************/
uint8_t R_FlashSetLockBitProtection(uint8_t lock_bit)
{        
    /* Attempt to grab state */;
    if( _FlashGrabState(FLASH_LOCK_BIT) != 0 )
    {
        /* Another operation is already in progress */
        return FLASH_BUSY;
    }    
    
    /* Enable or disable? */
    lock_bit_protection = lock_bit;
        
    /* Release state */
    _FlashReleaseState();

    /* Return success */
    return FLASH_SUCCESS;
}
/******************************************************************************
End of function  R_FlashSetLockBitProtection
******************************************************************************/

#endif /* IGNORE_LOCK_BITS */

