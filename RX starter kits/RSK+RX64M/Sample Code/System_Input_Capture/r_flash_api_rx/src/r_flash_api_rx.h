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
* File Name    : r_flash_api_rx.h
* Description  : This is a private header file used internally by the FLASH API module. It should not be modified or
* 				 included by the user in their application.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version Description           
*         : 12.04.2014 1.00    First Release
***********************************************************************************************************************/

#ifndef RX_FLASH_API_PRIVATE_HEADER_FILE
#define RX_FLASH_API_PRIVATE_HEADER_FILE

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* Tests to make sure the user configured the code correctly. */
/*
#if !defined(FLASH_MACRO) 
    #error "One of the required configuration options is not defined in r_flash_config.h!"
#endif
*/
#define USER_LOCKING_TYPE bsp_lock_t

/*FACI Commands*/
#define FLASH_FACI_CMD_PROGRAM              0xE8
#define FLASH_FACI_CMD_PROGRAM_CF           0x80
#define FLASH_FACI_CMD_PROGRAM_DF           0x02
#define FLASH_FACI_CMD_BLOCK_ERASE          0x20
#define FLASH_FACI_CMD_PE_SUSPEND           0xB0
#define FLASH_FACI_CMD_PE_RESUME            0xD0
#define FLASH_FACI_CMD_STATUS_CLEAR         0x50
#define FLASH_FACI_CMD_FORCED_STOP          0xB3
#define FLASH_FACI_CMD_BLANK_CHECK          0x71
#define FLASH_FACI_CMD_CONFIG_SET_1         0x40
#define FLASH_FACI_CMD_CONFIG_SET_2         0x08
#define FLASH_FACI_CMD_LOCK_BIT_PGM         0x77
#define FLASH_FACI_CMD_LOCK_BIT_READ        0x71
#define FLASH_FACI_CMD_FINAL                0xD0


/* Define the clock frequency supplied to the FCU. On the RX64x it is the FCLK. */
#define FLASH_CLOCK_HZ BSP_FCLK_HZ

/*  According to HW Manual the Max Programming Time for 256 bytes (ROM)
    is 6ms.  This is with a FCLK of 60MHz. The calculation below
    calculates the number of ICLK ticks needed for the timeout delay.
    The 6ms number is adjusted linearly depending on the FCLK frequency.
*/
#define WAIT_MAX_ROM_WRITE \
        ((int32_t)(6000 * (60.0/(FLASH_CLOCK_HZ/1000000)))*(BSP_ICLK_HZ/1000000))

/*  According to HW Manual the Max Programming Time for 4 bytes
    (Data Flash) is 1.7ms.  This is with a FCLK of 60MHz. The calculation
    below calculates the number of ICLK ticks needed for the timeout delay.
    The 1.7ms number is adjusted linearly depending on the FCLK frequency.
*/
#define WAIT_MAX_DF_WRITE \
        ((int32_t)(1700 * (60.0/(FLASH_CLOCK_HZ/1000000)))*(BSP_ICLK_HZ/1000000))

/*  According to HW Manual the Max Blank Check time for 4 bytes
    (Data Flash) is 30 usec.  This is with a FCLK of 60MHz. The calculation
    below calculates the number of ICLK ticks needed for the timeout delay.
    The 30 usec number is adjusted linearly depending on the FCLK frequency.
*/
#define WAIT_MAX_BLANK_CHECK \
        ((int32_t)(30 * (60.0/(FLASH_CLOCK_HZ/1000000)))*(BSP_ICLK_HZ/1000000))

/*  According to HW Manual the max timeout value when using the peripheral
    clock notification command is 60us. This is with a FCLK of 50MHz. The
    calculation below calculates the number of ICLK ticks needed for the
    timeout delay. The 60us number is adjusted linearly depending on
    the FCLK frequency.
*/
#define WAIT_MAX_NOTIFY_FCU_CLOCK \
        ((int32_t)(60 * (50.0/(FLASH_CLOCK_HZ/1000000)))*(BSP_ICLK_HZ/1000000))

/*  According to HW Manual the Max Erasure Time for a 32KB block is
    around 580ms.  This is with a FCLK of 60MHz. The calculation below
    calculates the number of ICLK ticks needed for the timeout delay.
    The 580ms number is adjusted linearly depending on the FCLK frequency.
*/
#define WAIT_MAX_ERASE_CF_32K \
        ((int32_t)(580000 * (60.0/(FLASH_CLOCK_HZ/1000000)))*(BSP_ICLK_HZ/1000000))

/*  According to HW Manual the Max Erasure Time for a 8KB block is
    around 145ms.  This is with a FCLK of 60MHz. The calculation below
    calculates the number of ICLK ticks needed for the timeout delay.
    The 145ms number is adjusted linearly depending on the FCLK frequency.
*/
#define WAIT_MAX_ERASE_CF_8K \
        ((int32_t)(145000 * (60.0/(FLASH_CLOCK_HZ/1000000)))*(BSP_ICLK_HZ/1000000))

/*  According to HW Manual the Max Erasure Time for a 1KB Data Flash block is
    around 40ms.  This is with a FCLK of 60MHz. The calculation below
    calculates the number of ICLK ticks needed for the timeout delay.
    The 40ms number is adjusted linearly depending on the FCLK frequency.
*/
#define WAIT_MAX_ERASE_DF \
        ((int32_t)(40000 * (60.0/(FLASH_CLOCK_HZ/1000000)))*(BSP_ICLK_HZ/1000000))
/******************************************************************************
Error checking
******************************************************************************/
/* FCLK must be between 4MHz and 60MHz.*/
//#if (FLASH_CLOCK_HZ > 60000000) || (FLASH_CLOCK_HZ < 4000000)
//    #error "ERROR - Flash API - FCLK on RX64M must be between 4MHz and 60MHz!"
//#endif

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
typedef __evenaccess volatile uint8_t  * FCU_BYTE_PTR;
typedef __evenaccess volatile uint16_t  * FCU_WORD_PTR;

typedef enum
{
    FLASH_UNINITIALIZED = 0,
    FLASH_INITIALIZATION,
    FLASH_READY,
    FLASH_WRITING,
    FLASH_ERASING,
    FLASH_BLANKCHECK,
    FLASH_BUSY,
} flash_states_t;

typedef enum _current
{
 FLASH_CUR_INVALID,
 FLASH_CUR_FCU_INIT,
 FLASH_CUR_IDLE,
 FLASH_CUR_LOCKBIT_SET,
 FLASH_CUR_CF_ERASE,
 FLASH_CUR_CF_WRITE,
 FLASH_CUR_CF_BGO_WRITE,
 FLASH_CUR_CF_BGO_ERASE,
 FLASH_CUR_DF_ERASE,
 FLASH_CUR_DF_BGO_ERASE,
 FLASH_CUR_DF_WRITE,
 FLASH_CUR_DF_BLANKCHECK,
 FLASH_CUR_DF_BGO_WRITE,
 FLASH_CUR_DF_BGO_BLANKCHECK,
}flash_cur_t;

typedef struct _current_parameters
{
 uint32_t src_addr;
 uint32_t dest_addr;
 uint32_t total_count;
 uint32_t current_count;
 flash_cur_t current_operation;
 bool lockbits_ignore;
 bool bgo_enabled_df;
 bool bgo_enabled_cf;
 uint16_t min_pgm_size;
 uint32_t wait_cnt;
}current_param_t;

/***********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/
extern current_param_t g_current_parameters;            // Current operation parameters
extern flash_int_cb_args_t g_flash_int_ready_cb_args;   // Callback argument structure for flash READY interrupt
extern flash_int_cb_args_t g_flash_int_error_cb_args;   // Callback argument structure for flash ERROR interrupt

/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/
flash_err_t flash_init();
flash_err_t flash_clock_config();
flash_err_t flash_fcuram_codecopy();
flash_err_t flash_reset();
flash_err_t flash_stop();
flash_err_t flash_suspend();
flash_err_t flash_resume();
flash_err_t flash_interrupt_config(bool state, void *pCallback);
flash_err_t flash_lockbit_write(flash_block_address_t block_address, uint32_t num_blocks);
flash_err_t flash_lockbit_read(flash_block_address_t block_address, flash_res_t *lock_state_result);
flash_err_t flash_lockbit_ignore(bool state);
flash_err_t flash_pe_mode_enter(flash_type_t flash_type);
flash_err_t flash_pe_mode_exit();
flash_err_t flash_erase(uint32_t block_address, uint32_t block_count);
flash_err_t flash_write(uint32_t *src_start_address, uint32_t * dest_start_address, uint32_t *num_bytes);
flash_err_t flash_blankcheck(uint32_t start_address, uint32_t num_bytes, flash_res_t *blank_check_result);
flash_err_t flash_lock_state (flash_states_t new_state);
flash_err_t flash_clock_set (uint32_t fclk_frequency);
flash_err_t flash_get_status (void);
void flash_release_state (void);
void flash_codecopy (void);
extern void (*flash_ready_isr_handler)(void *);          // Function pointer for Flash Ready ISR
extern void (*flash_error_isr_handler)(void *);          // Function pointer for Flash Error ISR
#endif /* RX_FLASH_API_PRIVATE_HEADER_FILE */
