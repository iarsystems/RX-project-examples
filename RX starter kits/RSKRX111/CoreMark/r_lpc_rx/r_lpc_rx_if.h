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
* File Name	   : r_lpc_rx_if.h
* Description  : The LPC module configures the MCU for the different operating and low power modes.
*                This file is the LPC module's interface header file and should be included by the application that
*                intends to use the API.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version Description           
*         : 10.03.2013 1.00    First Release
***********************************************************************************************************************/

#ifndef LPC_INTERFACE_HEADER_FILE
#define LPC_INTERFACE_HEADER_FILE

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Fixed width integer support. */
#include <stdint.h>
/* bool support */
#include <stdbool.h>
/* Used for configuring the Low Power Configuration module */
#include "r_lpc_config.h"
/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* Version Number of API. */
#define LPC_RX_VERSION_MAJOR           (1)
#define LPC_RX_VERSION_MINOR           (00)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/*  LPC API Error codes */
typedef enum _lpc_err{
	LPC_SUCCESS,
	LPC_ERR_INVALID_DIVIDER,      // inactive clock specified as system clock
	LPC_ERR_NULL_PTR,             // Null pointer passed as argument
	LPC_ERR_OSC_STOP_ENABLED,     // Software Standby Mode cannot be entered if Oscillation Stop is enabled
	LPC_ERR_CLOCK_EXCEEDED,       // If clock speed exceeds the limit of current operating mode
	LPC_ERR_ILL_MAIN_CLK_FREQ,    // If Main Oscillator is more than 8 MHz and is chosen as the Sleep return clock
	LPC_ERR_ILL_CLOCK_SOURCE,     // Return Clock Switch is only supported if clock on entering Sleep is SubOSC
	LPC_ERR_ILL_PARAM             // illegal argument not specified in enum
}lpc_err_t;

typedef enum _lpc_low_power_mode{
	LP_SLEEP,
	LP_ALL_MODULE_STOP,
	LP_DEEP_SLEEP,
	LP_SW_STANDBY,
	LP_INVALID_MODE
}lpc_low_power_mode_t;

typedef enum _lpc_operating_mode{
	OP_HIGH_SPEED   = 0x00,
	OP_MIDDLE_SPEED = 0x02,
	OP_LOW_SPEED    = 0x06,
	OP_INVALID_MODE
}lpc_operating_mode_t;
/* LPC Sleep Mode Return Clock Switching Sources*/
typedef enum _lpc_clock_switch{
	LPC_LOCO     = 0x00,
	LPC_HOCO     = 0x01,
	LPC_MAIN_OSC = 0x02,
}lpc_clock_switch_t;

/***********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/
uint32_t  R_LPC_GetVersion (void);
lpc_err_t R_LPC_ConfigureLowPowerMode(lpc_low_power_mode_t e_mode);
lpc_err_t R_LPC_ConfigureOperatingMode(lpc_operating_mode_t e_mode);
lpc_err_t R_LPC_LowPowerActivate(void (*pcallback)(void* pdata));
lpc_err_t R_LPC_ReturnClockSwitch(lpc_clock_switch_t e_clock_source, bool enable);
/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/


#endif/* LPC_INTERFACE_HEADER_FILE */


