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
* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name	   : r_example.h
* Description  : This module solves all the world's problems. This is a private header file which means that it should
*                only be used internally to the module. The user should not have to include this header file. You could
*                put internal global functions or error checking here.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version Description           
*         : 03.01.2013 1.00    First Release
***********************************************************************************************************************/

#ifndef CGC_PRIVATE_HEADER_FILE
#define CGC_PRIVATE_HEADER_FILE

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Fixed width integer support. */
#include <stdint.h>
/* bool support */
#include <stdbool.h>

#include "r_cgc_rx_if.h"
/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* Tests to make sure the user configured the code correctly. */

#define CLOCK_INACTIVE    0x01
#define CLOCK_ACTIVE      0x00

#define OVERFLOW_OCCURRED     0x01
#define OVERFLOW_NOT_OCCURRED 0x00

#define OSCILLATION_STOP_DETECTED  1
#define OSCILLATION_STOP_NOT_DETECTED  0
#define OSCILLATION_STOP_CLEARED 0

#define OSCILLATION_STOP_ENABLED  1
#define OSCILLATION_STOP_DISABLED 0

#define OSCILLATION_STOP_INT_ENABLED  1
#define OSCILLATION_STOP_INT_DISABLED 0

#define MAX_PLL_DIV_OP 8000000   /* This is the maximum output in Hz from the PLL divider*/
#define MIN_PLL_DIV_OP 4000000   /* This is the minimum output in Hz from the PLL divider*/

#define MAX_PLL_MUL_OP 48000000  /* This is the maximum output in Hz from the PLL multiplier*/
#define MIN_PLL_MUL_OP 32000000  /* This is the minimum output in Hz from the PLL multiplier*/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
enum _HOCO_Frequency
{
	HOCO_32MHZ = 0x00,
	HOCO_32_864MHz = 0x01,
	HOCO_40MHz = 0x02,
	HOCO_50MHz = 0x03,
	HOCO_INVALID
};

/* Internal structure used to keep track of current configuration */
typedef struct _cgc_current_system_param{
	uint32_t pclkd_hz;
	uint32_t pclkb_hz;
	uint32_t iclk_hz;
	uint32_t fclk_hz;
	uint8_t  clock_source;
}cgc_current_system_param_t;

/***********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/
#endif /* CGC_PRIVATE_HEADER_FILE */
