/**********************************************************************************************************************
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
**********************************************************************************************************************/
/**********************************************************************************************************************
* File Name	   : r_cgc_rx_if.h
* Description  : This module solves all the world's problems. This file is this module's interface header file. This 
*                file should contain everything the user will need to interface to your module. This could include 
*                public API functions, typedef structures, macros, etc...
**********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version Description           
*         : 03.01.2013 1.00    First Release
**********************************************************************************************************************/

#ifndef CGC_INTERFACE_HEADER_FILE
#define CGC_INTERFACE_HEADER_FILE

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Fixed width integer support. */
#include <stdint.h>
/* bool support */
#include <stdbool.h>
/* Used for configuring the CGC */
#include "r_cgc_config.h"
/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* Version Number of API. */
#define CGC_RX_VERSION_MAJOR           (1)
#define CGC_RX_VERSION_MINOR           (00)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/


typedef enum _cgc_clocks{        // Available system clock sources
	CGC_LOCO         = 0x00,
	CGC_HOCO         = 0x01,
	CGC_MAIN_OSC     = 0x02,
	CGC_SUBCLOCK     = 0x03,
	CGC_PLL          = 0x04
}cgc_clocks_t;

/*  CGC API Error codes */
typedef enum _cgc_err{
	CGC_SUCCESS,
	CGC_ERR_CLOCK_INACTIVE,          // inactive clock specified as system clock
	CGC_ERR_CLOCK_ACTIVE,            // active clock source cannot be modified without stopping first.
	CGC_ERR_STABILIZED,              // Clock has stabilized after its been turned on/off
	CGC_ERR_NOT_STABILIZED,          // clock has not stabilized after its been turned on/off
	CGC_ERR_MAIN_OCO_INACTIVE,       // PLL initialization attempted when main OCO is turned off
	CGC_ERR_NULL_PTR,                // Null pointer passed as argument
	CGC_ERR_OSC_STOP_DET_ENABLED,    // Illegal attempt to stop LOCO when Oscillation stop is enabled
	CGC_ERR_OSC_STOP_DETECTED,       // The Oscillation stop detection status flag is set
	CGC_ERR_OSC_STOP_CLOCK_ACTIVE,   // Attempt to clear Oscillation Stop Detect Status with PLL/MAIN_OSC active
	CGC_ERR_ILL_PARAM                // Unsupported argument used
}cgc_err_t;

typedef struct _cgc_clock_config{
	uint8_t divider;
	uint8_t multiplier;
}cgc_clock_config_t;

enum _cgc_pll_divider{
	PLL_DIV_1 = 0x00,
	PLL_DIV_2 = 0x01,
	PLL_DIV_4 = 0x02
};

enum _cgc_pll_multiplier{
	PLL_MUL_6  = 0x0B,
	PLL_MUL_8  = 0x0F
};

/* System clocks*/
typedef enum _cgc_system_clocks{
	CGC_PCLKD,
	CGC_PCLKB,
	CGC_ICLK,
	CGC_FCLK
}cgc_system_clocks_t;
/* System Clock Dividers*/
typedef enum _cgc_system_clock_dividers{
	SYS_CLOCK_DIV_1  = 0x00,
	SYS_CLOCK_DIV_2  = 0x01,
	SYS_CLOCK_DIV_4  = 0x02,
	SYS_CLOCK_DIV_8  = 0x03,
	SYS_CLOCK_DIV_16 = 0x04,
	SYS_CLOCK_DIV_32 = 0x05,
	SYS_CLOCK_DIV_64 = 0x06
}cgc_system_clock_dividers_t;

typedef struct _cgc_system_clock_config{
	uint8_t pclkd;
	uint8_t pclkb;
	uint8_t iclk;
	uint8_t fclk;
}cgc_system_clock_config_t;

typedef enum _cgc_clockout_dividers{
	CLOCKOUT_DIV_1 = 0x00,
	CLOCKOUT_DIV_2 = 0x01,
	CLOCKOUT_DIV_4 = 0x02,
	CLOCKOUT_DIV_8 = 0x03,
	CLOCKOUT_DIV_16= 0x04
}cgc_clockout_dividers_t;
/***********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/
uint32_t    R_CGC_GetVersion (void);
cgc_err_t   R_CGC_ClockStart(cgc_clocks_t clock_source, cgc_clock_config_t* pclock_config);
cgc_err_t   R_CGC_ClockStop(cgc_clocks_t clock_source);
cgc_err_t   R_CGC_SystemClockSet(cgc_clocks_t set_clock_source, cgc_system_clock_config_t* pSet_clock_config);
uint32_t    R_CGC_ClockHzGet(cgc_system_clocks_t clock);
cgc_err_t   R_CGC_ClockCheck(cgc_clocks_t clock_source);
cgc_err_t   R_CGC_ClockOutputConfig(cgc_clocks_t clock_source, cgc_clockout_dividers_t clockout_divider, bool enable_now);
cgc_err_t   R_CGC_ClockOutputEnable(bool enable);
cgc_err_t   R_CGC_OscStopDetect(void (*pcallback)(void* pdata), bool enable);
cgc_err_t   R_CGC_OscStopStatusClear();

#endif /* CGC_INTERFACE_HEADER_FILE */


