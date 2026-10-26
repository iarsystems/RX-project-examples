/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No 
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all 
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM 
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES 
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS 
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of 
* this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer 
*
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.    
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_flash_api_rx.c
* Description  : This module implements the FLASH API
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version Description           
*         : 12.04.2014 1.00    First Release
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Includes board and MCU related header files. */
#include "platform.h"
/* Public interface header file for this package. */
#include "r_flash_api_rx_if.h"
/* Configuration for this package. */
#include "r_flash_api_rx_config.h"
/* Private header file for this package. */
#include "r_flash_api_rx.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/



/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/
/* State variable for the Flash API. */
flash_states_t g_flash_state = FLASH_UNINITIALIZED;

flash_int_cb_args_t g_flash_int_ready_cb_args;   // Callback argument structure for flash READY interrupt
flash_int_cb_args_t g_flash_int_error_cb_args;   // Callback argument structure for flash ERROR interrupt

/* Internal functions. */
void (*flash_ready_isr_handler)(void *);          // Function pointer for Flash Ready ISR
void (*flash_error_isr_handler)(void *);          // Function pointer for Flash Error ISR

/*Structure that holds the parameters for current operations*/
current_param_t g_current_parameters = {
                                         0,     // Source Address
                                         0,     // Destination Address
                                         0,     // Total Count
                                         0,     // Current Count
                                         FLASH_CUR_INVALID, // Current Operation
                                         true,  // Lockbits Ignored
                                         false, // DF BGO Disabled
                                         false, // CF BGO Disabled
                                         0,     // Minimum Program Size
                                         0      // Wait Count for current operation
                                        };
/***********************************************************************************************************************
* Function Name: R_FLASH_Open
* Description  : Function will initialize the flash peripheral.
* Arguments    : void
* Return Value : FLASH_SUCCESS -
*                    API initialized successfully.
*                FLASH_ERR_BUSY
*                    API has already been initialized and another operation is ongoing.
*                FLASH_ERR_FAILURE
*                    Initialization failed.
*                    A RESET was performed on the FCU to rectify any internal errors
***********************************************************************************************************************/
flash_err_t R_FLASH_Open(void)
{
	flash_err_t err;
#if (FLASH_CFG_PARAM_CHECKING_ENABLE == 1)

#endif
	/* Allow Initialization if not initialized or
	 * if no operation is ongoing and re-initialization is desired */
	if ((FLASH_UNINITIALIZED == g_flash_state) || (FLASH_READY == g_flash_state))
	{
		if (FLASH_SUCCESS != flash_lock_state(FLASH_INITIALIZATION))
			{
				/* API already initialized */
				return FLASH_ERR_BUSY;
			}
	}

	/*Initialize the FCU*/
	err = flash_init();
	if (FLASH_SUCCESS != err)
	{
		return err;
	}

	g_flash_int_ready_cb_args.event = FLASH_INT_EVENT_INITIALIZED;

	/*Interrupts are enabled by default on this MCU.
	 * Disable Interrupts*/
	flash_interrupt_config(false, NULL);
	/* Release state so other operations can be performed. */
	flash_release_state();
	return FLASH_SUCCESS;
}

/*All the functions below need to be placed in RAM if Code Flash programming is to be supported*/
#pragma location = "CODE_IN_RAM"
/***********************************************************************************************************************
* Function Name: R_FLASH_Write
* Description  : Function will write to the specified Code or Data Flash memory area.
*      The
*      The function will not perform a blank check before proceeding with
*      the write operation.
* Arguments    : uint32_t src_address -
*                 Source buffer address.
*                uint32_t dest_address -
*                 Destination address.
*                uint32_t num_bytes
*                 Number of bytes to be written
* Return Value : FLASH_SUCCESS -
*                    Write completed successfully; successfully initialized in case of BGO mode.
*                FLASH_ERR_BYTES -
*                    Number of bytes exceeds max range or is 0 or is not a valid multiple of the minimum programming
*                    size for the specified flash
*                FLASH_ERR_ADRRESS -
*                    src/dest address is an invalid Code/Data Flash block start address or
*                    address is not aligned with the minimum programming size for the Code/Data Flash
*                    For Code Flash programming the src address HAS to be a RAM address since ROM cannot be accessed
*                    during Code Flash programming
*                FLASH_ERR_BUSY -
*                    Flash peripheral is busy with another operation
*                FLASH_ERR_CMD_LOCKED -
*                    The FCU entered a command locked state and cannot process the operation.
*                    A RESET was performed on the FCU to recover from this state.
*                FLASH_ERR_LOCKBIT_SET
*                    Operation failed because the lockbit was set.
*                    A RESET was performed on the FCU to recover from this state.
*                FLASH_ERR_FAILURE
*                	 Code Flash Write operation attempted in BGO mode. This is temporarily not supported
*                    Operation failed for some other reason; RESET was performed on the FCU to recover from this state.
***********************************************************************************************************************/
flash_err_t R_FLASH_Write(uint32_t src_address, uint32_t dest_address, uint32_t num_bytes)
{
	flash_err_t err;

	/*Grab the current flash state*/
	if (FLASH_SUCCESS != flash_lock_state(FLASH_WRITING))
	{
		/* API not initialized or busy with another operation*/
		return FLASH_ERR_BUSY;
	}

#if (FLASH_CFG_PARAM_CHECKING_ENABLE == 1)
	if (num_bytes == 0)
	{
		return FLASH_ERR_BYTES;
	}

#endif

	/*Check address validity and that it is on a Code Flash programming boundary*/
	if (
			((dest_address <= (uint32_t)FLASH_CF_BLOCK_0) && (dest_address > (uint32_t)FLASH_CF_BLOCK_INVALID))
			&&
			(!(dest_address & (FLASH_MIN_PGM_SIZE_CF-1))) // Ensure start address is on 256 (for CF) byte boundary
	)
	{

		/*Check if there is enough space in the destination, and that num_bytes is a multiple of programming size*/
		if ((num_bytes + (dest_address) > (uint32_t)FLASH_CF_BLOCK_0) || (num_bytes & (FLASH_MIN_PGM_SIZE_CF-1)))
		{
			err = FLASH_ERR_BYTES;
		}
		else
		{
			if (g_current_parameters.bgo_enabled_cf == true)
			{
				//TODO: CF BGO not supported currently
				flash_release_state();
				return FLASH_ERR_FAILURE;
				//g_current_parameters.current_operation = FLASH_CUR_CF_BGO_WRITE;
			}
			else
			{
				g_current_parameters.current_operation = FLASH_CUR_CF_WRITE;
				g_current_parameters.wait_cnt = WAIT_MAX_ROM_WRITE;
			}
			g_current_parameters.min_pgm_size = (FLASH_MIN_PGM_SIZE_CF >> 1);
			/*There are no parameter errors. Switch to the CF PE mode*/
			err = flash_pe_mode_enter(FLASH_TYPE_CODE_FLASH);
		}

	}
	/*Check address validity and that it is on a Data Flash programming boundary*/
	else if (((dest_address >= FLASH_DF_BLOCK_0) && (dest_address < FLASH_DF_BLOCK_INVALID ))
			&&
			(!(dest_address & (FLASH_MIN_PGM_SIZE_DF-1))))
	{
		/*Check if there is enough space in the destination, and that num_bytes is a multiple of programming size*/
		if ((num_bytes + (dest_address) >= FLASH_DF_BLOCK_INVALID) || (num_bytes & (FLASH_MIN_PGM_SIZE_DF-1)))
		{
			err = FLASH_ERR_BYTES;
		}
		else
		{
			if (g_current_parameters.bgo_enabled_df == true)
			{
				g_current_parameters.current_operation = FLASH_CUR_DF_BGO_WRITE;
			}
			else
			{
				g_current_parameters.current_operation = FLASH_CUR_DF_WRITE;
			}
			g_current_parameters.wait_cnt = WAIT_MAX_DF_WRITE;
			g_current_parameters.min_pgm_size = (FLASH_MIN_PGM_SIZE_DF >> 1);
			/*There are no parameter errors. Switch to the DF PE mode*/
			err = flash_pe_mode_enter(FLASH_TYPE_DATA_FLASH);
		}
	}
	else
	{
		err = FLASH_ERR_ADRRESS;
	}
	/*If parameters passed are not valid or there was an error entering PE mode,
	 * then release the state and return the error condition*/
	if (FLASH_SUCCESS != err)
	{
		flash_release_state();
		return err;
	}

	/*Write the data*/
	err = flash_write(&src_address, &dest_address, &num_bytes);
	if (FLASH_SUCCESS != err)
	{
		/*If there is an error, then reset the FCU: This will clear error flags and exit the P/E mode*/
		flash_reset();
		/*Release the lock*/
		flash_release_state();
		return err;
	}

	/*If in non-BGO mode, then current operation is completed.
	 * Exit from PE mode and return status*/
	if ((g_current_parameters.current_operation == FLASH_CUR_CF_WRITE)
			||
			(g_current_parameters.current_operation == FLASH_CUR_DF_WRITE))
	{
		/*Return to read mode*/
		err = flash_pe_mode_exit();
		if (FLASH_SUCCESS != err)
		{
			/*Reset the FCU: This will stop any existing processes and exit PE mode*/
			flash_reset();
		}
		/*Release lock and reset the state to idle*/
		flash_release_state();
	}

	/*If BGO is enabled, then
	 *return result of started Write process*/
	return err;
}

#pragma location = "CODE_IN_RAM"
/***********************************************************************************************************************
* Function Name: R_FLASH_Erase
* Description  : Function will erase the specified Code or Data Flash blocks.
* Arguments    : uint32_t block_start_address -
*                 Start address of the first block. Actual address or entry from "flash_block_address_t" enum can be used
*                uint32_t num_blocks -
*                 Number of blocks to erase.
* Return Value : FLASH_SUCCESS -
*                    Erase completed successfully; successfully initialized in case of BGO mode.
*                FLASH_ERR_BLOCKS -
*                    Number of blocks exceeds max range or is 0
*                FLASH_ERR_ADRRESS -
*                    Start address is an invalid Code/Data Flash block start address
*                FLASH_ERR_BUSY -
*                    Flash peripheral is busy with another operation
*                FLASH_ERR_CMD_LOCKED
*                    The FCU entered a command locked state and cannot process the operation.The function performed a
*                    RESET on the peripheral to recover from this state.
*                FLASH_ERR_FAILURE
*                	 Code Flash Erase operation attempted in BGO mode. This is temporarily not supported
***********************************************************************************************************************/
flash_err_t R_FLASH_Erase(flash_block_address_t block_start_address, uint32_t num_blocks)
{
	flash_err_t err;
	/*Grab the current flash state*/
	if (FLASH_SUCCESS != flash_lock_state(FLASH_ERASING))
	{
		/* API not initialized or busy with another operation*/
		return FLASH_ERR_BUSY;
	}
#if (FLASH_CFG_PARAM_CHECKING_ENABLE == 1)

#endif
	/*Check address and num_blocks validity and switch to Code Flash or Data Flash P/E mode*/
	if ((block_start_address <= FLASH_CF_BLOCK_0) && (block_start_address > FLASH_CF_BLOCK_INVALID))
	{
		if ((num_blocks >= FLASH_NUM_BLOCKS_CF) || (num_blocks <= 0))
		{
			err = FLASH_ERR_BLOCKS;
		}
		else
		{ /*No errors in parameters. Enter Code Flash PE mode*/
			err = flash_pe_mode_enter(FLASH_TYPE_CODE_FLASH);
			if (g_current_parameters.bgo_enabled_cf == true)
			{
				//TODO: CF BGO not supported currently
				flash_release_state();
				return FLASH_ERR_FAILURE;
				//g_current_parameters.current_operation = FLASH_CUR_CF_BGO_ERASE;
			}
			else
			{
				g_current_parameters.current_operation = FLASH_CUR_CF_ERASE;
			}
		}
	}
	else if ((block_start_address >= FLASH_DF_BLOCK_0) && (block_start_address < FLASH_DF_BLOCK_INVALID ))
	{
		if ((num_blocks >= FLASH_NUM_BLOCKS_DF) || (num_blocks <= 0))
		{
			err = FLASH_ERR_BLOCKS;
		}
		else
		{ /*No errors in parameters. Enter Data Flash PE mode*/
			err = flash_pe_mode_enter(FLASH_TYPE_DATA_FLASH);
			if (g_current_parameters.bgo_enabled_df == true)
			{
				g_current_parameters.current_operation = FLASH_CUR_DF_BGO_ERASE;
			}
			else
			{
				g_current_parameters.current_operation = FLASH_CUR_DF_ERASE;
			}
		}
	}
	else
	{
		err = FLASH_ERR_ADRRESS;
	}

	if (FLASH_SUCCESS != err)
	{
		flash_release_state();
		return err;
	}

	/*Lock Bit protection not checked here
	 * User Should configure it before calling function*/

	/*Erase the Blocks*/
	err = flash_erase((uint32_t)block_start_address, num_blocks);
	if (FLASH_SUCCESS != err)
	{
		/*If there is an error, then reset the FCU: This will clear error flags and exit the P/E mode*/
		flash_reset();
		flash_release_state();
		return err;
	}

	/*If in non-BGO mode, then current operation is completed.
	 * Exit from PE mode and return status*/
	if ((g_current_parameters.current_operation == FLASH_CUR_CF_ERASE)
			||
			(g_current_parameters.current_operation == FLASH_CUR_DF_ERASE))
	{
		/*Return to read mode*/
		err = flash_pe_mode_exit();
		if (FLASH_SUCCESS != err)
		{
			/*Reset the FCU: This will stop any existing processes and exit PE mode*/
			flash_reset();
		}
		/*Release lock and reset the state to Idle*/
		flash_release_state();
	}
	/*If BGO is enabled, then return result of started Erase process*/
	return err;
}

/***********************************************************************************************************************
* Function Name: R_FLASH_BlankCheck
* Description  : Function will perform a blank check on the specified Data Flash area.
*                The function will incrementally check the area from the start address onward.
*                The minimum number of bytes is 4 and max is 65536 with increments in 4 byte units.
* Arguments    : uint32_t address -
*                 Start address to perform blank check. Actual address or entry from "flash_block_address_t" enum can be used
*                uint32_t num_blocks -
*                 Number of bytes to perform blank check operation for.
*                flash_res_t *result
*                 Returns the result of the blank check operation. This field is valid only in non-BGO mode
*                 operation
* Return Value : FLASH_SUCCESS
*                 Operation was completed successfully
*                FLASH_ERR_BYTES -
*                    Number of bytes exceeds max range (1-65535) or is not a multiple of 4
*                FLASH_ERR_ADRRESS -
*                    Start address is an invalid Data Flash Address
*                FLASH_ERR_LOCKED -
*                    The FCU is in a command locked state and cannot process the operation.The function performed a
*                    RESET on the peripheral to recover from this state.
***********************************************************************************************************************/
flash_err_t R_FLASH_BlankCheck(uint32_t address, uint32_t num_bytes, flash_res_t *result)
{
	flash_err_t err = FLASH_SUCCESS;

	/*Grab the current flash state*/
	if (FLASH_SUCCESS != flash_lock_state(FLASH_BLANKCHECK))
	{
		/* API not initialized or busy with another operation*/
		return FLASH_ERR_BUSY;
	}

#if (FLASH_CFG_PARAM_CHECKING_ENABLE == 1)
	if (((address >= FLASH_DF_BLOCK_0) && (address < FLASH_DF_BLOCK_INVALID )))
	{
		/*Check if the range is valid, num_bytes is a multiple of 4, is larger than 0 and less than 65536*/
		if (
				((num_bytes + address) >= FLASH_DF_BLOCK_INVALID) ||
				(num_bytes & (FLASH_MIN_PGM_SIZE_DF-1)) ||
				(num_bytes == 0) ||
				(num_bytes > 65536)
		)
		{
			err = FLASH_ERR_BYTES;
		}
	}
	else
	{
		err = FLASH_ERR_ADRRESS;

	}
#endif

	if (FLASH_SUCCESS != err)
	{
		flash_release_state();
		return err;
	}

	if (g_current_parameters.bgo_enabled_df == true)
	{
		g_current_parameters.current_operation = FLASH_CUR_DF_BGO_BLANKCHECK;
	}
	else
	{
		g_current_parameters.current_operation = FLASH_CUR_DF_BLANKCHECK;
	}

	/*WAIT_MAX_BLANK_CHECK specifies the wait time for a 4 byte blank check,
	 * num_bytes is divided by 4 and then multiplied to calculate the wait time for the entire operation*/
	g_current_parameters.wait_cnt = (WAIT_MAX_BLANK_CHECK * (num_bytes >> 2));

	err = flash_pe_mode_enter(FLASH_TYPE_DATA_FLASH);
	/*If there is an error, then reset the FCU: This will clear error flags and exit the P/E mode*/
	if (FLASH_SUCCESS != err)
	{
		flash_reset();
		flash_release_state();
		return err;
	}

	/*Initialize the result*/
	*result = FLASH_RES_INVALID;

	/*Start the Erase operation*/
	err = flash_blankcheck(address, num_bytes, result);
	if (g_current_parameters.bgo_enabled_df == false)
	{
		/*If the return condition of the blank-check operation is not FLASH_SUCCESS then  reset the FCU*/
		if (FLASH_SUCCESS != err)
		{
			flash_reset();
			flash_release_state();
			return err;
		}
		/*Return to read mode*/
		err = flash_pe_mode_exit();
		/*Reset the state to Idle*/
		g_current_parameters.current_operation = FLASH_CUR_IDLE;
		/*If the exit from PE mode is not successful, return the failure code,
		 * else return the result of the blank_check*/
		if (FLASH_SUCCESS != err)
		{
			flash_reset();
		}
		flash_release_state();
	}
	/*If BGO is enabled, then
	 *return result of started BlankCheck process*/
	return err;

}
/***********************************************************************************************************************
* Function Name: R_FLASH_Lockbit_Set
* Description  : Function will set the lockbit for the specified number of code flash blocks.
* Arguments    : flash_block_address_t block_address -
*                 Starting address of code flash block to set the lockbit for.
*                 Element from "flash_block_address_t" enum must be used
*                uint32_t num_blocks -
*                 Number of bytes to set the lockbit for.
*                flash_res_t *result
*                 Returns the result of the blank check operation. This field is valid only in non-BGO mode
*                 operation
* Return Value : FLASH_ERR_BLANK -
*                    Specified Data Flash area is blank
*                FLASH_ERR_BYTES -
*                    Number of bytes exceeds max range (1-65535) or is not a multiple of 4
*                FLASH_ERR_ADRRESS -
*                    Start address is an invalid Data Flash Address
*                FLASH_ERR_LOCKED -
*                    The FCU is in a command locked state and cannot process the operation.The function performed a
*                    RESET on the peripheral to recover from this state.
*                FLASH_ERR_FAILURE
*                	 LockBit operation attempted in BGO mode. This is temporarily not supported
***********************************************************************************************************************/
flash_err_t R_FLASH_Lockbit_Set(flash_block_address_t block_address, uint32_t num_blocks)
{
	flash_err_t err;

	if (((block_address > FLASH_CF_BLOCK_0) && (block_address <= FLASH_CF_BLOCK_INVALID )))
	{
		return FLASH_ERR_ADRRESS;
	}

	if (g_current_parameters.bgo_enabled_cf == true)
	{
		//TODO: CF BGO not supported currently
		return FLASH_ERR_FAILURE;
	}
	/*Transition to CF P/E mode to issue LOCKBIT READ command*/
	if (FLASH_SUCCESS != flash_pe_mode_enter(FLASH_TYPE_CODE_FLASH))
	{
		flash_reset();
		return FLASH_ERR_FAILURE;
	}

	/*Set the current operation*/
	g_current_parameters.current_operation = FLASH_CUR_LOCKBIT_SET;

	/*Start the LockBit Set operation*/
	err = flash_lockbit_write(block_address, num_blocks);
	if (FLASH_SUCCESS != err)
	{
		/*Reset the FCU: This will stop any existing processes and exit PE mode*/
		flash_reset();
	}
	/*Current operation is completed. Exit from PE mode and return status*/
	/*Return to read mode*/
	err = flash_pe_mode_exit();
	if (FLASH_SUCCESS != err)
	{
		/*Reset the FCU: This will stop any existing processes and exit PE mode*/
		flash_reset();
	}
	/*Release lock*/
	flash_release_state();

	return err;

}
/***********************************************************************************************************************
* Function Name: R_FLASH_Control
* Description  : This function supports additional configuration operations.
*      The supported commands are listed in the flash_cmd_t enum.
* Arguments    : flash_cmd_t cmd -
*                 command.
*                void *pcfg -
*                 Pointer to configuration. This argument can be NULL for
*                 commands that do not require a configuration.
*
*                Command                                |   Argument
*                FLASH_CMD_RESET------------------------| NULL
*                FLASH_CMD_STATUS_GET-------------------| NULL
*                FLASH_CMD_LOCKBIT_WRITE----------------| flash_lockbit_config_t
*                FLASH_CMD_LOCKBIT_READ-----------------| flash_lockbit_config_t
*                FLASH_CMD_LOCKBIT_ENABLE---------------| NULL
*                FLASH_CMD_LOCKBIT_DISABLE--------------| NULL
*                FLASH_CMD_BGO_ENABLE_DF----------------| void (*pcallback)(void *)
*                FLASH_CMD_BGO_DISABLE_DF---------------| NULL
*                FLASH_CMD_BGO_ENABLE_CF----------------| NULL
*                FLASH_CMD_BGO_DISABLE_CF---------------| void (*pcallback)(void *)
* Return Value : FLASH_SUCCESS -
*                    Operation completed successfully.
*                FLASH_ERR_BYTES -
*                    Number of blocks exceeds max range
*                FLASH_ERR_ADRRESS -
*                    Address is an invalid Code/Data Flash block start address
*                FLASH_ERR_NULL_PTR -
*                    Pointer was NULL for a command that expects a configuration structure
*                FLASH_ERR_BUSY -
*                    Flash peripheral is busy with another operation
*                FLASH_ERR_LOCKED -
*                    The FCU was in a command locked state and has been reset
***********************************************************************************************************************/
flash_err_t R_FLASH_Control(flash_cmd_t cmd, void *pcfg)
{
	flash_err_t err = FLASH_SUCCESS;
	flash_lockbit_config_t *lockbit_cfg;

	/*If the command is to reset the FCU, then no attempt is made to grab the lock
	 * before executing the reset since the assumption is that the RESET command
	 * is used to terminate any existing operation*/
	switch (cmd)
	{
	case FLASH_CMD_RESET:
		err = flash_reset();
		return err;
	}


	/*Grab the current flash state*/
	if (FLASH_SUCCESS != flash_lock_state(FLASH_READY))
	{
		/* API not initialized or busy with another operation*/
		return FLASH_ERR_BUSY;
	}
	switch (cmd)
	{
	case FLASH_CMD_LOCKBIT_ENABLE:
		err = flash_lockbit_ignore(false);
		break;
	case FLASH_CMD_LOCKBIT_DISABLE:
		err = flash_lockbit_ignore(true);
		break;
	case (FLASH_CMD_BGO_DISABLE_CF):
		err = flash_interrupt_config(false, pcfg);
		/*Set the flag indicating that BGO is enabled*/
		g_current_parameters.bgo_enabled_cf = false;
		break;
	case (FLASH_CMD_BGO_ENABLE_CF):
		err = flash_interrupt_config(true, pcfg);
		/*Set the flag indicating that BGO is enabled*/
		g_current_parameters.bgo_enabled_cf = true;
		break;
	case (FLASH_CMD_BGO_DISABLE_DF):
		err = flash_interrupt_config(false, pcfg);
	    /*Set the flag indicating that BGO is enabled*/
		g_current_parameters.bgo_enabled_df = false;
		break;
	case (FLASH_CMD_BGO_ENABLE_DF):
		err = flash_interrupt_config(true, pcfg);
        /*Set the flag indicating that BGO is enabled*/
		g_current_parameters.bgo_enabled_df = true;
		break;
	case FLASH_CMD_STATUS_GET:
		err = flash_get_status();
		break;
	case FLASH_CMD_LOCKBIT_READ:
		lockbit_cfg = pcfg;
		err = flash_lockbit_read(lockbit_cfg->block_start_address, &(lockbit_cfg->result));
		break;
	case FLASH_CMD_LOCKBIT_WRITE:
		lockbit_cfg = pcfg;
		err = R_FLASH_Lockbit_Set(lockbit_cfg->block_start_address, lockbit_cfg->num_blocks);
		break;
	default:
		err = FLASH_ERR_PARAM;
	}
	flash_release_state();
	return err;
}

/***********************************************************************************************************************
* Function Name: R_FLASH_GetVersion
* Description  : Returns the current version of this module. The version number is encoded where the top 2 bytes are the
*                major version number and the bottom 2 bytes are the minor version number. For example, Version 4.25
*                would be returned as 0x00040019.
* Arguments    : none
* Return Value : Version of this module.
***********************************************************************************************************************/
uint32_t R_FLASH_GetVersion (void)
{
    /* These version macros are defined in r_flash_if.h. */
    return ((((uint32_t)FLASH_RX_VERSION_MAJOR) << 16) | (uint32_t)FLASH_RX_VERSION_MINOR);
}


/******************************************************************************
* Function Name: flash_lock_state
* Description  : Attempt to grab the flash state to perform an operation
*                NOTE1: This function does not have to execute from in RAM. It
*                       must be in RAM though if FLASH_API_RX_CFG_ROM_BGO is
*                       enabled and this function is called during a ROM P/E
*                       operation.
* Arguments    : new_state -
*                    Which state to attempt to transition to
* Return Value : FLASH_SUCCESS -
*                    State was grabbed
*                FLASH_ERR_BUSY -
*                    Flash is busy with another operation
******************************************************************************/
flash_err_t flash_lock_state (flash_states_t new_state)
{
    /* Check to see if lock was successfully taken */
    if (R_BSP_HardwareLock(BSP_LOCK_FLASH) == true)
    {
        /* Lock taken, we can change state */
        g_flash_state = new_state;

        /* Return success */
        return FLASH_SUCCESS;
    }
    else
    {
        /* Another operation is on-going */
        return FLASH_ERR_BUSY;
    }
}
/******************************************************************************
End of function  flash_lock_state
******************************************************************************/
/***********************************************************************************************************
* Function Name: flash_get_status
* Description  : Returns the current state of the flash
*                NOTE1: This function does not have to execute from in RAM. It must be in RAM though if
*                CF BGO is enabled and this function is called during a ROM P/E operation.
* Arguments    : none
* Return Value : FLASH_SUCCESS -
*                    Flash is ready to use
*                FLASH_ERR_BUSY -
*                    Flash is busy with another operation or is uninitialized
***********************************************************************************************************/
flash_err_t flash_get_status (void)
{
    /* Return flash status */
    if( g_flash_state == FLASH_READY )
    {
        return FLASH_SUCCESS;
    }
    else
    {
        return FLASH_ERR_BUSY;
    }
}
/******************************************************************************
* Function Name: flash_release_state
* Description  : Release state so another flash operation can take place
*                NOTE1: This function does not have to execute from in RAM. It
*                       must be in RAM though if FLASH_API_RX_CFG_ROM_BGO is
*                       enabled and this function is called during a ROM P/E
*                       operation.
* Arguments    : none
* Return Value : none
******************************************************************************/
void flash_release_state (void)
{

    /* Done with current operation */
    g_flash_state = FLASH_READY;

    /* Release hold on lock */
    R_BSP_HardwareUnlock(BSP_LOCK_FLASH);
}
/******************************************************************************
End of function  flash_release_state
******************************************************************************/
