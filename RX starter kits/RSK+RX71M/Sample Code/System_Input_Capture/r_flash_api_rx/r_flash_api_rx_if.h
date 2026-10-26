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
* Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_flash_api_rx_if.h
* Description  : This module provides the interface file to be included by the user when using the FLASH API
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 12.04.2014 1.00    First Release
***********************************************************************************************************************/

#ifndef FLASH_INTERFACE_HEADER_FILE
#define FLASH_INTERFACE_HEADER_FILE

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* Version Number of API. */
#define FLASH_RX_VERSION_MAJOR           (1)
#define FLASH_RX_VERSION_MINOR           (00)

/* Private header file for this MCU
 * Includes MCU specific definitions and API declaration*/
#ifdef BSP_MCU_RX71M
#include "./targets/rx71x/r_flash_api_rx71m.h"
#else
#error "MCU specified in r_bsp_config.h is not supported by this API"
#endif
/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/* Flash API error codes */
typedef enum _flash_err
{
    FLASH_SUCCESS = 0,
    FLASH_ERR_BUSY,             // Peripheral Busy
    FLASH_ERR_CMD_LOCKED,       // Peripheral in command locked state
    FLASH_ERR_LOCKBIT_SET,      // Pgm/Erase error due to lock bit.
    FLASH_ERR_FAILURE,          // Operation failure.
                                // Programming or erasing error due to something other than lock bit
    FLASH_ERR_BYTES,            // Invalid number of bytes passed
    FLASH_ERR_ADRRESS,          // Invalid address or address not on a programming boundary
    FLASH_ERR_BLOCKS,           // The "number of blocks" argument is invalid
    FLASH_ERR_PARAM,            // Illegal parameter
    FLASH_ERR_NULL_PTR,         // received null ptr; missing required argument
    FLASH_ERR_TIMEOUT,          // Timeout Condition
} flash_err_t;

/*Result type for certain operations*/
typedef enum _flash_res
{
    FLASH_RES_INVALID,                    // Invalid condition
	FLASH_RES_LOCKBIT_STATE_PROTECTED,    // Result for FLASH_CMD_LOCKBIT_READ
	FLASH_RES_LOCKBIT_STATE_NON_PROTECTED,// Result for FLASH_CMD_LOCKBIT_READ
    FLASH_RES_BLANK,                      // Return status for Blank Check Function
    FLASH_RES_NOT_BLANK                   // Return status for Blank Check Function
}flash_res_t;

/*Event type returned by the ISR callback when used in BGO mode*/
typedef enum _flash_interrupt_event
{
	FLASH_INT_EVENT_INITIALIZED,
	FLASH_INT_EVENT_ERASE_COMPLETE,
	FLASH_INT_EVENT_WRITE_COMPLETE,
	FLASH_INT_EVENT_BLANK,
	FLASH_INT_EVENT_NOT_BLANK,
	FLASH_INT_EVENT_ERR_DF_ACCESS,
	FLASH_INT_EVENT_ERR_CF_ACCESS,
	FLASH_INT_EVENT_ERR_CMD_LOCKED,
	FLASH_INT_EVENT_ERR_LOCKBIT_SET,
	FLASH_INT_EVENT_ERR_FAILURE
}flash_interrupt_event_t;

typedef enum _flash_type
{
	FLASH_TYPE_CODE_FLASH = 0,
	FLASH_TYPE_DATA_FLASH,
	FLASH_TYPE_INVALID
}flash_type_t;

/*Commands supported by the R_FLASH_Control function*/
typedef enum _flash_cmd
{
	FLASH_CMD_RESET,
	FLASH_CMD_STATUS_GET,
	FLASH_CMD_LOCKBIT_WRITE,
	FLASH_CMD_LOCKBIT_READ,
	FLASH_CMD_LOCKBIT_ENABLE,
	FLASH_CMD_LOCKBIT_DISABLE,
	FLASH_CMD_BGO_ENABLE_DF,
	FLASH_CMD_BGO_DISABLE_DF,
	FLASH_CMD_BGO_ENABLE_CF,
	FLASH_CMD_BGO_DISABLE_CF,
	FLASH_CMD_CONFIG_OPTION_AND_SECURITY,
	FLASH_CMD_CONFIG_CLOCK,
}flash_cmd_t;

/* Config struct to be used with R_Flash_Control for FLASH_CMD_LOCKBIT_WRITE and FLASH_CMD_LOCKBIT_READ command*/
typedef struct _flash_lockbit_config
{
	flash_block_address_t block_start_address;
	flash_res_t result;						//Populated by API when using FLASH_CMD_LOCKBIT_READ
    uint32_t num_blocks;                    //Required only for FLASH_CMD_LOCKBIT_WRITE
}flash_lockbit_config_t;

/* Config struct to be used with R_Flash_Control for FLASH_CMD_BGO_ENABLE_DF command*/
typedef struct _flash_interrupt_config
{
	void (*pcallback)(void *);
    uint8_t int_priority;
}flash_interrupt_config_t;

/*Event type for ISR callback*/
typedef struct
{
	flash_interrupt_event_t event;         //Which Flash event caused this interrupt
} flash_int_cb_args_t;

typedef union _flash_control_arg
{
	flash_lockbit_config_t lockbit_cmd;
	flash_interrupt_config_t bgo_cmd;
}flash_control_arg_t;
/***********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/
flash_err_t R_FLASH_Open(void);
flash_err_t R_FLASH_Write(uint32_t src_address, uint32_t dest_address, uint32_t num_bytes);
flash_err_t R_FLASH_Erase(flash_block_address_t block_start_address, uint32_t num_blocks);
flash_err_t R_FLASH_BlankCheck(uint32_t address, uint32_t num_bytes, flash_res_t *blank_check_result);
flash_err_t R_FLASH_Control(flash_cmd_t cmd, void *pcfg);
uint32_t R_FLASH_GetVersion (void);

#endif /* FLASH_INTERFACE_HEADER_FILE */


