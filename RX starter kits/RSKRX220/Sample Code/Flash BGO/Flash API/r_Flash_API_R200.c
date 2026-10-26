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
* File Name	   : r_Flash_API_RX200.c
* Version	   : 1.01
* Device 	   : RX200 Series
* Tool Chain   : RX Family C Compiler
* H/W Platform : RSKRX220
* Description  : Flash programming for the RX200 Group
*******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 5.10.2011 1.00    First Release. Evolution from RX600 v2.1 Flash API
*		  : 8.15.2012 1.01	  Modified R_FlashErase to use a uint16_t argument
*							  instead of uint8_t
*							  Removed check for BLANK_CHECK_8_BYTE in R_FlashDataAreaBlankCheck()
******************************************************************************/
/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
/* Intrinsic functions of MCU */
#include <machine.h>

/* Allocate flash block array here */
#define FLASH_BLOCKS_DECLARE

/* Function prototypes and device specific info needed for Flash API */
#include "r_Flash_API_RX200.h"

/* Device specific SFR definitions */
#if defined(RX210)
#include <iorx220.h>
#else
#error "!!! You must specify your device in Flash_API_RX200.h first !!!"
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
#define	WAIT_tRESW (35*ICLK_FREQUENCY)
/*  The number of ICLK ticks needed for 10us delay are calculated below */
#define	WAIT_t10USEC (10*ICLK_FREQUENCY)

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
/* Semaphore for making sure flash routines do not stomp on each other */
static signed long flash_semaphore = 0;
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
static uint8_t temp_array[ROM_PROGRAM_SIZE_LARGE];
/* Used for telling whether a DF to DF or ROM to ROM operation is on-going */
static uint8_t flash_to_flash_op;
#endif

#if defined(DATA_FLASH_BGO) || defined(ROM_BGO)
/* How many bytes are left to program */
static uint32_t bgo_bytes;
/* Where we are programming or erasing */
static uint32_t  bgo_flash_addr;
/* Where the data is that we are programming */
static uint32_t  bgo_buffer_addr;
/* The programming size to be used*/
static uint32_t  bgo_programming_size;
#endif

/* Flash intialisation function prototype */
static uint8_t _Flash_Init(void);
/* Enter PE mode function prototype */
static uint8_t _Enter_PE_Mode( uint32_t flash_addr, uint16_t bytes );
/* Exit PE mode function prototype */
static void _Exit_PE_Mode(void);
/* ROM write function prototype */
static uint8_t _ROM_Write(uint32_t address, uint32_t data,uint32_t programming_size);
/* Data flash write function prototype */
static uint8_t _DataFlash_Write(uint32_t address, uint32_t data, uint8_t size);
/* Data flash status clear function prototype */
static void _DataFlash_Status_Clr(void);
/* Notify peripheral clock function prototype */
static uint8_t _Notify_Peripheral_Clock(volatile uint8_t * flash_addr);
/* FCU reset function prototype */
static void    _Flash_Reset(void);
/* Used to grab flash state */
static uint8_t _FlashGrabState(enum Flash_States new_state);
/* Used to release flash state */
static void _FlashReleaseState(void);
/* Used to issue an erase command to the FCU */
static uint8_t _Flash_Erase_Command(volatile uint8_t * const erase_addr);

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


    /* Disable FCU interrupts in FCU block */
	FLASH.FAEINT.BIT.ROMAEIE = 0;
	FLASH.FAEINT.BIT.CMDLKIE = 0;
	FLASH.FAEINT.BIT.DFLAEIE = 0;
	FLASH.FAEINT.BIT.DFLRPEIE = 0;
	FLASH.FAEINT.BIT.DFLWPEIE = 0;

    /* Disable FCU interrupts in ICU */

    /* Disable flash interface error (FIFERR) */
    IPR(FCU, FIFERR) = 0;
    IEN(FCU, FIFERR) = 0;

    #if defined(DATA_FLASH_BGO) || defined(ROM_BGO)
    /* Enable flash ready interrupt (FRDYI) */
    /* Make sure IPL is high enough that interrupt will trigger */
    IPR(FCU, FRDYI) = FLASH_READY_IPL;
    IEN(FCU, FRDYI) = 1;
    #else
    /* Disable flash ready interrupt (FRDYI) */
    IPR(FCU, FRDYI) = 0;
    IEN(FCU, FRDYI) = 0;
    #endif

	/* Return no errors */
	return FLASH_SUCCESS;
}
/******************************************************************************
End of function  _Flash_Init
******************************************************************************/

/******************************************************************************
* Function Name: _DataFlash_Status_Clr
* Description  : Clear the status of the Data Flash operation.
*	             NOTE: This function does not have to execute from in RAM.
* Arguments    : none
* Return Value : none
******************************************************************************/
static void _DataFlash_Status_Clr(void)
{
	/* Declare temporaty pointer */
    volatile uint8_t *ptrb;

    /* Set pointer to Data Flash to issue a FCU command if needed */
    ptrb = (volatile uint8_t *)(DF_ADDRESS);

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
* Description  : Write either bytes to Data Flash area.
*	             NOTE: This function does not have to execute from in RAM.
* Arguments    : address - The address (in the Data Flash programming area)
*                          to write the data to
*                data - The address of the data to write
*                size - The size of the data to write. Must be set to
*                       either DF_PROGRAM_SIZE_LARGE or DF_PROGRAM_SIZE_SMALL.
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
#if defined(DF_PROGRAM_SIZE_LARGE)
    if((DF_PROGRAM_SIZE_LARGE == size) || (DF_PROGRAM_SIZE_SMALL == size))
#else
    if(DF_PROGRAM_SIZE_SMALL == size)
#endif
	{
		/* Perform bit shift since 2 bytes are written at a time */
        size = size >> 1;

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
*                NOTE1: This function does not have to execute from in RAM.
*                NOTE2: RX210 support block and 2 byte checks
* Arguments    : address - The address to check if is blank.
*                          If the parameter 'size'=='BLANK_CHECK_ENTIRE_BLOCK',
*                          this should be set to a defined Data Block Number
*                          ('BLOCK_DB0', 'BLOCK_DB1', etc...) or an address
*                          in the data flash block.  Either option will work.
*                          If the parameter 'size'=='BLANK_CHECK_2_BYTE',
*                          this should be set to a 2 byte address boundary.
*                size - This specifies if you are checking a 2-byte location,
*						or an entire block. You must set this
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
uint8_t R_FlashDataAreaBlankCheck(uint32_t address, uint8_t size)
{
	/* Declare data flash pointer */
    volatile uint8_t *ptrb;

	/* Declare result container variable */
	uint8_t result;

    /* Declare wait counter variable */
    int32_t wait_cnt;

    /* Check to make sure address is valid. */
    if( ((address > (DF_NUM_BLOCKS+BLOCK_DB0)) && (address < DF_ADDRESS))
        || (address > DF_ADDRESS + DF_SIZE) )
    {
        /* Address is not a valid DF address or block number */
        return FLASH_ERROR_ADDRESS;
    }

    /* Check to make sure 'size' parameter is valid */
    if( (size != BLANK_CHECK_ENTIRE_BLOCK) &&
#if defined(BLANK_CHECK_2_BYTE)
        (size != BLANK_CHECK_2_BYTE)
#endif
      )
    {
        /* 'size' parameter is not valid. */
        return FLASH_ERROR_BYTES;
    }

    /* Attempt to grab state */
    if( _FlashGrabState(FLASH_BLANKCHECK) != FLASH_SUCCESS )
    {
        /* Another operation is already in progress */
        return FLASH_BUSY;
    }

    /* Set current FCU mode to Data Flash PE Mode */
	current_mode = FLD_PE_MODE;

    /* Enter Data Flash PE mode in the FCU */
	if( _Enter_PE_Mode( address, 0 ) != FLASH_SUCCESS)
	{
        /* Release state */
        _FlashReleaseState();

		/* Return FLASH_FAILURE, operation failure */
		return FLASH_FAILURE;
	}

	/* Set  bit FRDMD (bit 4) in FMODR to 1 */
	FLASH.FMODR.BIT.FRDMD = 1;

#if defined(RX210)
	/* Check if 2 byte size has been passed */
    if(size == BLANK_CHECK_2_BYTE)
	{
        #ifdef DATA_FLASH_BGO
        /* Disable FCU ready interrupt for 2 byte checks */
        FLASH.FRDYIE.BIT.FRDYIE = 0;
        #endif

        /* Set data flash pointer to beginning of the memory block */
		ptrb = (volatile uint8_t *)(address & DF_MASK);

		/* Check if the next 2 bytes are blank
		   Bits BCADR to the address of the 2-byte location to check.
		   Set bit BCSIZE in EEPBCCNT to 0. */
        FLASH.DFLBCCNT.WORD = (uint16_t)(address & (DF_BLOCK_SIZE_LARGE-2));
    }
#endif
 	/* Check entire data block */
	else
	{
        /* Check to see if user sent in a data block number or the
           address.  The function description tells the user to send
           in the block number but in some early examples the address
           was used. To make this work with both versions we detect which
           is sent in below and make it work either way. */
        if( address < (DF_NUM_BLOCKS+BLOCK_DB0) )
        {
            /* A data block number was sent in */
            ptrb = (volatile uint8_t *)block_addresses[address];
        }
        else
        {
            /* Any address in the erasure block */
		    ptrb = (volatile uint8_t *)address;
        }

		/* Check if the entire block is blank
		   Set bit BCSIZE in EEPBCCNT to 1. */
		FLASH.DFLBCCNT.BIT.BCSIZE = 1;
    }

    /* Send commands to FCU */
    *ptrb = 0x71;
    *ptrb = 0xD0;

#ifdef DATA_FLASH_BGO
    /* Only using BGO on block blank checks */
    if( size == BLANK_CHECK_ENTIRE_BLOCK )
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
	if(FLASH.FSTATR0.BIT.ILGLERR == 1)	/* Check 'ILGERR' bit */
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
	result = FLASH.DFLBCSTAT.BIT.BCST;

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
#if  defined(RX210)

    /* Set Read access for the Data Flash blocks DB0-DB7 */
	FLASH.DFLRE0.WORD = 0x2D00 | (read_en_mask & 0x00FF);

	/* Set Erase/Program access for the Data Flash blocks DB0-DB7 */
	FLASH.DFLWE0.WORD = 0x1E00 |  (write_en_mask & 0x00FF);

#else

    #error "!!! You must specify your device in r_Flash_API_RX2xx.h first !!!"

#endif
}
/******************************************************************************
End of function  R_FlashDataAreaAccess
******************************************************************************/

#ifdef ENABLE_ROM_PROGRAMMING
#pragma section="FRAM"
#endif

/******************************************************************************
* Function Name: _ROM_Write
* Description  : Write bytes to ROM Area Flash.
*                NOTE: This function MUST execute from in RAM.
* Arguments    : address - ROM address of where to write to
*                data - Pointer to the data to write
*				 programming_size - 2,8 or 128 byte options for the RX210
* Return Value : FLASH_SUCCESS = Operation Successful
*                FLASH_FAILURE = Operation Failed
******************************************************************************/
static uint8_t _ROM_Write(uint32_t address, uint32_t data, uint32_t programming_size)
{
	/* Declare iteration loop count variable */
	uint8_t i;
	/* Declare wait counter variable */
    int32_t wait_cnt;
    /* Number of bytes to write */
   // uint32_t size = ROM_PROGRAM_SIZE;
	  uint32_t size = programming_size;

    /* Writes are done 16-bit at a time, scale 'size' argument */
    size = programming_size >> 1;

	/* Write the FCU Program command */
    *(volatile uint8_t *)address = 0xE8;
    *(volatile uint8_t *)address = size;

    /* Write 'size' bytes into flash, 16-bits at a time */
	for(i = 0; i < size; i++)
	{
		/* Copy data from source address to destination ROM */
        *(volatile uint16_t *)address = *(uint16_t *)data;

		/* Increment destination address by 2 bytes */
		data += 2;
    }

    /* Write the final FCU command for programming */
	*(volatile uint8_t *)address = 0xD0;

#ifdef ROM_BGO
    /* Return, rest of programming will be done in interrupt */
    return FLASH_SUCCESS;
#endif

	if(programming_size==128)
	{
		/* Set timeout wait counter value */
    	wait_cnt = WAIT_MAX_ROM_WRITE_128;
	}
	else if((programming_size==2)||(programming_size==8))
	{
		/* Set timeout wait counter value */
    	wait_cnt = WAIT_MAX_ROM_WRITE_2_8;
	}


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
* Arguments    : flash_addr - The programming/erasure address
*                bytes - The number of bytes you are writing (if you are
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
		if( _Flash_Init() != FLASH_SUCCESS )
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

#if defined(RX210)
               /* There is only 1 flash area */
        FLASH.FENTRYR.WORD = 0xAA01;
#endif

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
	if(		(FLASH.FSTATR0.BIT.ILGLERR == 1)
		||	(FLASH.FSTATR0.BIT.ERSERR  == 1)
		||	(FLASH.FSTATR0.BIT.PRGERR  == 1)
		||	(FLASH.FSTATR1.BIT.FCUERR  == 1))
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

        /* Inform FCU of flash clock speed, check if operation is succesful */
        if(_Notify_Peripheral_Clock((volatile uint8_t *)flash_addr)!=0)
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

        /* No need to notify FCU of clock supplied to flash again */
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
	int32_t	wait_cnt;

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
	if(		(FLASH.FSTATR0.BIT.ILGLERR == 1)
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
            /* If there is more than one area then we need to check and make
			   sure we are sending the status register clear command
			   to the correct section. */
#if defined(RX210)
			/* There is only one area */
            /* Set address pointer to ROM area */
            pAddr = (volatile uint8_t *)(ROM_AREA_0);

#endif

            /* Send status clear command to FCU */
			*pAddr = 0x50;
		}
		else
		{
			/* Set address pointer to data flash area */
			pAddr = (volatile uint8_t *)DF_ADDRESS;

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
uint8_t R_FlashErase(uint16_t block)
{
	/* Declare address pointer */
	uint32_t pAddr;
	/* Declare erase operation result container variable */
	uint8_t result = FLASH_SUCCESS;


    /* Attempt to grab state */
    if( _FlashGrabState(FLASH_ERASING) != FLASH_SUCCESS )
    {
        /* Another operation is already in progress */
        return FLASH_BUSY;
    }

    /* Erase Command Address */
	pAddr = block_addresses[ block ];

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
	if( _Enter_PE_Mode(pAddr, 0) != FLASH_SUCCESS)
	{
        /* Release state */
        _FlashReleaseState();

		/* Return FLASH_FAILURE, operation failure */
		return FLASH_FAILURE;
	}

#ifdef  IGNORE_LOCK_BITS
	/* Cancel the ROM Protect feature
	   NOTE: If you are using block locking feature to protect your data,
	   then comment out 'IGNORE_LOCK_BITS' in Flash_API_RX200_UserConfig.h */
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


#if defined(RX210)
    /* NOTE:
       The RX210 actually has 64 x 128 byte blocks instead of the
       16 x 2Kbyte blocks shown below. These are grouped into 4 blocks to
       make it easier for the user to delete larger sections of the data
       flash at once. The user can still delete individual blocks
       but they will need to use the new flash erase function that takes
       addresses instead of blocks. For reference, the 2KB blocks defined
       are called 'fake' blocks. */
    if( FLD_PE_MODE == current_mode ) {

        /* Calculate how many bytes to erase */
        int32_t bytes_to_erase = block_addresses[BLOCK_DB1] -
                                 block_addresses[BLOCK_DB0];

        /* Erase real data flash blocks until the 'fake' block is erased .*/
        while(0 < bytes_to_erase) {

            /* Send FCU command to erase block */
            result = _Flash_Erase_Command((volatile uint8_t *)pAddr);

            /* Advance pointer to next block */
            pAddr += DF_ERASE_BLOCK_SIZE;

            /* Subtract off bytes erased */
            bytes_to_erase -= DF_ERASE_BLOCK_SIZE;

    #if defined(DATA_FLASH_BGO)
            /* Set global variables so that erase can continue in ISR. */
            bgo_flash_addr = pAddr;
            bgo_bytes = bytes_to_erase;

            /* Return, check result and continue erasure later in ISR */
            return FLASH_SUCCESS;
    #endif
        }
    }
    else
    {
        /* This is a ROM block erase */
        /* Send FCU command to erase block */
        result = _Flash_Erase_Command((volatile uint8_t *)pAddr);
    }
#else
    /* Send FCU command to erase block */
    result = _Flash_Erase_Command((volatile uint8_t *)pAddr);
#endif

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

	/* Leave Program/Erase Mode */
 	_Exit_PE_Mode();

    /* Release state */
    _FlashReleaseState();

	/* Return erase result */
	return result;
}
/******************************************************************************
End of function  R_FlashErase
******************************************************************************/

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
uint8_t R_FlashEraseRange(uint32_t start_addr, uint32_t bytes)
{
	/* Declare erase operation result container variable */
	uint8_t result = FLASH_SUCCESS;

    /* Take off upper byte since for programming/erase addresses for ROM are
		the same as read addresses except upper byte is masked off to 0's.
		Data Flash addresses are not affected. */
    start_addr &= 0x00FFFFFF;

	/* Confirm this is for data flash */
	if( start_addr >= DF_ADDRESS && start_addr < (DF_ADDRESS + DF_SIZE) )
	{
		/* Check if the number of bytes were passed is a multiple of the
           programming size for the data flash */
		if( bytes & (DF_ERASE_BLOCK_SIZE-1) )
		{
			/* Return number of bytes not a multiple of the programming size */
			return FLASH_ERROR_BYTES;
		}
		/* Check for an address on an erase boundary. */
		if( start_addr & (DF_ERASE_BLOCK_SIZE-1) )
		{
			/* Return address not on a programming boundary */
			return FLASH_ERROR_ALIGNED;
		}
	}
    else
    {
        /* This function is only for data flash blocks. */
        return FLASH_ERROR_ADDRESS;
    }

    /* Attempt to grab state */
    if( _FlashGrabState(FLASH_ERASING) != FLASH_SUCCESS )
    {
        /* Another operation is already in progress */
        return FLASH_BUSY;
    }

    /* Set current FCU mode to data flash PE */
    current_mode = FLD_PE_MODE;

    /* Enter ROM PE mode, check if operation successful */
	if( _Enter_PE_Mode(start_addr, 0) != FLASH_SUCCESS)
	{
        /* Release state */
        _FlashReleaseState();

		/* Return FLASH_FAILURE, operation failure */
		return FLASH_FAILURE;
	}

#ifdef  IGNORE_LOCK_BITS
	/* Cancel the ROM Protect feature
	   NOTE: If you are using block locking feature to protect your data,
	   then comment out 'IGNORE_LOCK_BITS' in Flash_API_RX200_UserConfig.h */
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

    /* Erase real data flash blocks until the 'fake' block is erased .*/
    while(0 < bytes) {

        /* Send FCU command to erase block */
        result = _Flash_Erase_Command((volatile uint8_t *)start_addr);

        /* Advance pointer to next block */
        start_addr += DF_ERASE_BLOCK_SIZE;

        /* Subtract off bytes erased */
        bytes -= DF_ERASE_BLOCK_SIZE;

#if defined(DATA_FLASH_BGO)
        /* Set global variables so that erase can continue in ISR. */
        bgo_flash_addr = start_addr;
        bgo_bytes = bytes;

        /* Return, check result and continue erasure later in ISR */
        return FLASH_SUCCESS;
#endif
    }

	/* Leave Program/Erase Mode */
 	_Exit_PE_Mode();

    /* Release state */
    _FlashReleaseState();

	/* Return erase result */
	return result;
}
/******************************************************************************
End of function  R_FlashEraseRange
******************************************************************************/
#endif /* defined(RX210) */

/******************************************************************************
* Function Name: _Flash_Erase_Command
* Description  : Issues the FCU command to erase a flash block
*                NOTE: This function MUST execute from in RAM for 'ROM Area'
*                programming, but if you are ONLY doing Data Flash programming,
*                this function can reside and execute in Flash.
* Arguments    : erase_addr = An address in the block to be erased
* Return Value : FLASH_SUCCESS = Operation Successful
*                FLASH_FAILURE = Operation Failed
******************************************************************************/
static uint8_t _Flash_Erase_Command(volatile uint8_t * const erase_addr)
{
	/* Declare timer wait count variable */
    int32_t wait_cnt;
	/* Declare erase operation result container variable */
	uint8_t result = FLASH_SUCCESS;

    /* Send the FCU Command */
    *erase_addr = 0x20;
    *erase_addr = 0xD0;

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

	/* Return erase result */
	return result;
}
/******************************************************************************
End of function  _Flash_Erase_Command
******************************************************************************/

/******************************************************************************
* Function Name: R_FlashWrite
* Description  : Writes bytes into flash.
*                NOTE: This function MUST execute from in RAM for 'ROM Area'
*                programming, but if you are ONLY doing Data Flash programming,
*                this function can reside and execute in Flash.
* Arguments    : flash_addr - Flash address location to write to. This address
*                             must be on a program boundary.
*                buffer_addr - Address location of data buffer to write
*                              into flash.
*                bytes - The number of bytes to write. You must always pass a
*                        multiple of the programming size (e.g. RX210 has
*                        options for 128, 8 or 2-byte ROM writes and 8 or 2-byte DF writes).
*				programming_size - The programming size. RX210 has  support for
*								128, 8 or 2-byte ROM writes and 8 or 2-byte DF writes
* Return Value : FLASH_SUCCESS = Operation Successful
*                FLASH_FAILURE = Operation Failed
*                FLASH_ERROR_ALIGNED = Flash address was not on correct boundary
*                FLASH_ERROR_BYTES = Number of bytes did not match programming
*                                    size of ROM or DF.
*                FLASH_ERROR_ADDRESS = Invalid address
*                FLASH_ERROR_BOUNDARY = (ROM) Cannot write across flash areas.
*                FLASH_BUSY = Flash is busy with another operation
******************************************************************************/
uint8_t R_FlashWrite ( uint32_t flash_addr,
                       uint32_t buffer_addr,
                       uint16_t bytes
					    )
{
    /* Declare result container and number of bytes to write variables */
	uint8_t result = FLASH_SUCCESS;
    uint8_t num_byte_to_write;
	uint32_t programming_size;
#ifdef FLASH_TO_FLASH
    /* Local variable when using FLASH_TO_FLASH */
    uint16_t i;
#endif

    /* Take off upper byte since for programming/erase addresses for ROM are
		the same as read addresses except upper byte is masked off to 0's.
		Data Flash addresses are not affected. */
    flash_addr &= 0x00FFFFFF;

	/* ROM area or Data Flash area - Check for DF first */
	if( flash_addr >= DF_ADDRESS && (flash_addr < DF_ADDRESS + DF_SIZE) )
	{
		/* Check if the number of bytes were passed is a multiple of the
           programming size for the data flash */
		if( bytes & (DF_PROGRAM_SIZE_SMALL-1) )
		{
			/* Return number of bytes not a multiple of the programming size */
			return FLASH_ERROR_BYTES;
		}
		/* Check for an address on a programming boundary. */
		if( flash_addr & (DF_PROGRAM_SIZE_SMALL-1) )
		{
			/* Return address not on a programming boundary */
			return FLASH_ERROR_ALIGNED;
		}
	}
    /* Check for ROM area */
	else if(	(flash_addr >= ROM_PE_ADDR)
			&&	(flash_addr < ROM_PE_ADDR + ROM_SIZE) )
	{
		/* Check if the number of bytes were passed is a multiple of the
           programming size for ROM; if it is a multiple of ROM_PROGRAM_SIZE_SMALL,
		   then it is a multiple of the other sizes as well */
		if( bytes & (ROM_PROGRAM_SIZE_SMALL-1) )
		{
			/* Return number of bytes not a multiple of the programming size */
			return FLASH_ERROR_BYTES;
		}

		/* Check for an address on the smallest programming boundary. in case of the RX210 this is 2 bytes */
		if( flash_addr & (ROM_PROGRAM_SIZE_SMALL-1))
		{
			/* Return address not on a ROM programming byte boundary */
			return FLASH_ERROR_ALIGNED;
		}
		/*Determine what programming granularity to use based
		on the number of bytes to be programmed, The RX210 supports
		2, 8 and 128 bytes */
		if( (bytes >= ROM_PROGRAM_SIZE_LARGE) &&
                ((flash_addr & (ROM_PROGRAM_SIZE_LARGE-1)) == 0x00))
		{
			programming_size=ROM_PROGRAM_SIZE_LARGE;
		}
		else if( (bytes >= ROM_PROGRAM_SIZE_MEDIUM) &&
                ((flash_addr & (ROM_PROGRAM_SIZE_MEDIUM-1)) == 0x00))
		{
			programming_size=ROM_PROGRAM_SIZE_MEDIUM;
		}
		else
		{
			programming_size=ROM_PROGRAM_SIZE_SMALL;
		}

	}
	else
    {
		/* Return invalid flash address */
        return FLASH_ERROR_ADDRESS;
    }

    /* Attempt to grab state */
    if( _FlashGrabState(FLASH_WRITING) != FLASH_SUCCESS )
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
    if( (buffer_addr >= ROM_PE_ADDR) ||
        ((buffer_addr >= DF_ADDRESS) && (buffer_addr < (DF_ADDRESS + DF_SIZE))))
    {
        /* Performing ROM to ROM, DF to ROM, ROM to DF, or DF to DF tranbsfer */
        if(current_mode == ROM_PE_MODE) {

            /* Performing ROM to ROM or DF to ROM */

            /* Copy over first programming 'page' */
            for(i = 0; i < programming_size; i++)
            {
                /* Copy over each byte */
                temp_array[i] = *((uint8_t *)(buffer_addr+i));
            }

            /* Check size */
            if( bytes == programming_size )
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
            /* Performing DF to DF or ROM to DF transfer */

            /* Check if address is on a data flash programming boundary, and
               there is at least 1 'page' remaining to write. */
            /* Not all MCUs have two programming sizes for the data flash. If a
               MCU only has 1 then DF_PROGRAM_SIZE_SMALL is defined and not
               DF_PROGRAM_SIZE_LARGE. */
    #if defined(DF_PROGRAM_SIZE_LARGE)
            if( (bytes >= DF_PROGRAM_SIZE_LARGE) &&
                ((flash_addr & (DF_PROGRAM_SIZE_LARGE-1)) == 0x00))
            {
                /* Use large write size since it's more time efficient */
                num_byte_to_write = DF_PROGRAM_SIZE_LARGE;
            }
            else
            {
                /* Cannot use large program size, use small */
                num_byte_to_write = DF_PROGRAM_SIZE_SMALL;
            }
    #else
            /* Set the next data transfer size to be DF_PROGRAM_SIZE_SMALL */
            num_byte_to_write = DF_PROGRAM_SIZE_SMALL;
    #endif

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
    }
    else
    {
        /* No need to buffer */
        flash_to_flash_op = 0;
    }

#endif /* FLASH_TO_FLASH */

	/* Enter PE mode, check if operation is successful */
	if( _Enter_PE_Mode( flash_addr, bytes ) != FLASH_SUCCESS)
	{
        /* Release state */
        _FlashReleaseState();

		/* Return operation failure */
		return FLASH_FAILURE;
	}

    #ifdef  IGNORE_LOCK_BITS
	/* Cancel the ROM Protect feature
	   NOTE: If you are using block locking feature to protect your data,
	   then comment out 'IGNORE_LOCK_BITS' in Flash_API_RX200_UserConfig.h */
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
            /* Check if address is on a data flash programming boundary, and
               there is at least 1 'page' remaining to write. */
            /* Not all MCUs have two programming sizes for the data flash. If a
               MCU only has 1 then DF_PROGRAM_SIZE_SMALL is defined and not
               DF_PROGRAM_SIZE_LARGE. */
#if defined(DF_PROGRAM_SIZE_LARGE)
    		if( (bytes >= DF_PROGRAM_SIZE_LARGE) &&
                ((flash_addr & (DF_PROGRAM_SIZE_LARGE-1)) == 0x00))
            {
                /* Use large write size since it's more time efficient */
                num_byte_to_write = DF_PROGRAM_SIZE_LARGE;
            }
            else
            {
                /* Cannot use large program size, use small */
                num_byte_to_write = DF_PROGRAM_SIZE_SMALL;
            }
#else
            /* Set the next data transfer size to be DF_PROGRAM_SIZE_SMALL */
    		num_byte_to_write = DF_PROGRAM_SIZE_SMALL;
#endif

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

                /* What's the maximum number of bytes we can program next
                   iteration? */
        #if defined(DF_PROGRAM_SIZE_LARGE)
        		if( (bytes >= DF_PROGRAM_SIZE_LARGE) &&
                    ((flash_addr & (DF_PROGRAM_SIZE_LARGE-1)) == 0))
        		{
                    /* Use large write size since it's more time efficient */
                    num_byte_to_write = DF_PROGRAM_SIZE_LARGE;
        		}
        		else
        		{
                    /* Cannot use large program size, use small */
        			num_byte_to_write = DF_PROGRAM_SIZE_SMALL;
        		}
        #else
                /* Set the next data transfer size to be DF_PROGRAM_SIZE_SMALL*/
        		num_byte_to_write = DF_PROGRAM_SIZE_SMALL;
        #endif

                /* Copy over up to next bytes */
                for(i = 0; i < num_byte_to_write; i++)
                {
                    /* Copy over each byte */
                    temp_array[i] = *((uint8_t *)(buffer_addr+i));
                }

            	/* Re-enter PE mode, check if operation is successful */
            	if( _Enter_PE_Mode( flash_addr, bytes ) != FLASH_SUCCESS)
            	{
                    /* Release state */
                    _FlashReleaseState();

            		/* Return operation failure */
            		return FLASH_FAILURE;
            	}
            }
    #endif /* FLASH_TO_FLASH */
#endif  /* DATA_FLASH_BGO */
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
            bgo_flash_addr = flash_addr + programming_size;
            bgo_buffer_addr = buffer_addr + programming_size;
            bgo_bytes = bytes - programming_size;
			bgo_programming_size=programming_size;

#endif

			/* Call the Programming function */
#ifdef FLASH_TO_FLASH
            if( flash_to_flash_op == 1 )
            {
                /* Use RAM array */
                result = _ROM_Write(flash_addr, (uint32_t)&temp_array[0],programming_size);
            }
            else
            {
                /* Go as usual */
                result = _ROM_Write(flash_addr, buffer_addr,programming_size);
            }
#else
            result = _ROM_Write(flash_addr, buffer_addr,programming_size);
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
			flash_addr += programming_size;
			buffer_addr += programming_size;

			/* Decrement the number of bytes remaining by the size of the last
			   flash write */
			bytes -= programming_size;

		if( (bytes >= ROM_PROGRAM_SIZE_LARGE) &&
                ((flash_addr & (ROM_PROGRAM_SIZE_LARGE-1)) == 0x00))
		{
			programming_size=ROM_PROGRAM_SIZE_LARGE;
		}
		else if( (bytes >= ROM_PROGRAM_SIZE_MEDIUM) &&
                ((flash_addr & (ROM_PROGRAM_SIZE_MEDIUM-1)) == 0x00))
		{
			programming_size=ROM_PROGRAM_SIZE_MEDIUM;
		}
		else
		{
			programming_size=ROM_PROGRAM_SIZE_SMALL;
		}

    #ifdef FLASH_TO_FLASH
            /* Check to see if we need to buffer more data */
            if( (bytes > 0) &&
                (flash_to_flash_op == 1) )
            {
                /* We must leave PE mode to transfer next buffer to RAM */
                _Exit_PE_Mode();

                /* Copy over next ROM_PROGRAM_SIZE bytes */
                for(i = 0; i < programming_size; i++)
                {
                    /* Copy over each byte */
                    temp_array[i] = *((uint8_t *)(buffer_addr+i));
                }

            	/* Re-enter PE mode, check if operation is successful */
            	if( _Enter_PE_Mode( flash_addr, bytes ) != FLASH_SUCCESS)
            	{
                    /* Release state */
                    _FlashReleaseState();

            		/* Return operation failure */
            		return FLASH_FAILURE;
            	}
            }
    #endif /* FLASH_TO_FLASH */
#endif /* ROM_BGO */
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
* Description  : Notifies FCU or clock supplied to flash unit
*                NOTE: This function MUST execute from in RAM for 'ROM Area'
*                programming, but if you are ONLY doing Data Flash programming,
*                this function can reside and execute in Flash.
* Arguments    : flash_addr - Flash address you will be erasing or writing to
* Return Value : FLASH_SUCCESS = Operation Successful
*                FLASH_FAILURE = Operation Failed
******************************************************************************/
static uint8_t _Notify_Peripheral_Clock(volatile uint8_t * flash_addr)
{
	/* Declare wait counter variable */
	int32_t wait_cnt;

	/* Notify Peripheral Clock(PCK) */
    /* Set frequency of PCK */
	FLASH.PCKAR.WORD = FLASH_CLOCK_FREQUENCY;

	/* Execute Peripheral Clock Notification Commands */
    *flash_addr = 0xE9;
	*flash_addr = 0x03;
	*(volatile uint16_t __data24 *)flash_addr = 0x0F0F;
	*(volatile uint16_t __data24 *)flash_addr = 0x0F0F;
	*(volatile uint16_t __data24 *)flash_addr = 0x0F0F;
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
	int32_t	wait_cnt;

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
#pragma vector=VECT(FCU, FRDYI)
__interrupt  void FlashReadyInterrupt(void)
{
    /* Local variables */
    uint32_t num_byte_to_write;
    uint8_t ret;
#ifdef FLASH_TO_FLASH
    uint32_t i;
#endif

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
#if defined(RX210)
        /* If we are erasing data flash then we need to see if all requested
           blocks are erased. */
        if( FLD_PE_MODE == current_mode )
        {
            /* Check to see if there are more bytes to erase. */
            if( 0 < bgo_bytes ) {

                /* Send FCU command to erase block */
                ret = _Flash_Erase_Command((volatile uint8_t *)bgo_flash_addr);

                /* Advance pointer to next block */
                bgo_flash_addr += DF_ERASE_BLOCK_SIZE;

                /* Subtract off bytes erased */
                bgo_bytes -= DF_ERASE_BLOCK_SIZE;

                /* Only continue if last command was successful */
                if( ret == FLASH_SUCCESS )
                {
                    /* Exit ISR */
                    return;
                }
            }
        }
#endif

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
                /* Check if address is on a data flash programming boundary, and
                   there is at least 1 'page' remaining to write. */
                /* Not all MCUs have two programming sizes for the data flash.
                   If a MCU only has 1 then DF_PROGRAM_SIZE_SMALL is defined and
                   not DF_PROGRAM_SIZE_LARGE. */
#if defined(DF_PROGRAM_SIZE_LARGE)
        		if( (bgo_bytes >= DF_PROGRAM_SIZE_LARGE) &&
                    ((bgo_flash_addr & (DF_PROGRAM_SIZE_LARGE-1)) == 0x00))
                {
                    /* Use large write size since it's more time efficient */
                    num_byte_to_write = DF_PROGRAM_SIZE_LARGE;
                }
                else
                {
                    /* Cannot use large program size, use small */
                    num_byte_to_write = DF_PROGRAM_SIZE_SMALL;
                }
#else
                /* Set the next data transfer size to be DF_PROGRAM_SIZE_SMALL*/
        		num_byte_to_write = DF_PROGRAM_SIZE_SMALL;
#endif
            }
            else
            {
                /* Writing ROM, always write ROM_PROGRAM_SIZE bytes */
                num_byte_to_write = bgo_programming_size;
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
        			ret = _DataFlash_Write( bgo_flash_addr,
                                            (uint32_t)&temp_array[0],
    			                            num_byte_to_write);
                }
                else
                {
        			ret = _DataFlash_Write( bgo_flash_addr,
                                            bgo_buffer_addr,
    			                            num_byte_to_write);
                }
#else
    			ret = _DataFlash_Write( bgo_flash_addr,
                                        bgo_buffer_addr,
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
                    ret = _ROM_Write(bgo_flash_addr, (uint32_t)&temp_array[0],bgo_programming_size);
                }
                else
                {
                    /* Go as usual */
                    ret = _ROM_Write(bgo_flash_addr, bgo_buffer_addr,bgo_programming_size);
                }
#else
                ret = _ROM_Write(bgo_flash_addr, bgo_buffer_addr,bgo_programming_size);
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

			if( (bgo_bytes >= ROM_PROGRAM_SIZE_LARGE) &&
                ((bgo_flash_addr & (ROM_PROGRAM_SIZE_LARGE-1)) == 0x00))
			{
				bgo_programming_size=ROM_PROGRAM_SIZE_LARGE;
			}
			else if( (bgo_bytes >= ROM_PROGRAM_SIZE_MEDIUM) &&
            	    ((bgo_flash_addr & (ROM_PROGRAM_SIZE_MEDIUM-1)) == 0x00))
			{
				bgo_programming_size=ROM_PROGRAM_SIZE_MEDIUM;
			}
			else
			{
				bgo_programming_size=ROM_PROGRAM_SIZE_SMALL;
			}

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
    if( _FlashGrabState(FLASH_LOCK_BIT) != FLASH_SUCCESS )
    {
        /* Another operation is already in progress */
        return FLASH_BUSY;
    }

    /* Flash Command Address */
	pAddr = (volatile uint8_t *)(block_addresses[ block ]);

    /* Set FCU to ROM PE mode */
    current_mode = ROM_PE_MODE;

    /* Enter ROM PE mode, check if operation successful */
	if( _Enter_PE_Mode((uint32_t)pAddr, 0) != FLASH_SUCCESS)
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
    wait_cnt = WAIT_MAX_ROM_WRITE_128;

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
    if( _FlashGrabState(FLASH_LOCK_BIT) != FLASH_SUCCESS )
    {
        /* Another operation is already in progress */
        return FLASH_BUSY;
    }

    /* Flash Command Address */
	pAddr = (volatile uint8_t *)(block_addresses[ block ]);

    /* Set FCU to ROM PE mode */
    current_mode = ROM_PE_MODE;

    /* Enter ROM PE mode, check if operation successful */
	if( _Enter_PE_Mode((uint32_t)pAddr, 0) != FLASH_SUCCESS)
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
    if( _FlashGrabState(FLASH_LOCK_BIT) != FLASH_SUCCESS )
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

