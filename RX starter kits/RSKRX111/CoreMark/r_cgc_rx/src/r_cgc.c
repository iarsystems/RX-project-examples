/********************************************************************************************************************
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
********************************************************************************************************************/
/*******************************************************************************************************************
* File Name	: r_example.c
* Description  : This module solves all the world's problems
********************************************************************************************************************/
/*******************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 03.01.2013 1.00    First Release
********************************************************************************************************************/

/********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
********************************************************************************************************************/
/* Includes board and MCU related header files. */
#include "platform.h"
/* Public interface header file for this package. */
#include "r_cgc_rx_if.h"
/* Configuration for this package. */
#include "r_cgc_config.h"
/* Private header file for this package. */
#include "r_cgc.h"
/* include definition for NULL*/
#include <stddef.h>

/*********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/

/*********************************************************************************************************************
 Typedef definitions
 *********************************************************************************************************************/

/*********************************************************************************************************************
 Private global variables and functions
 *********************************************************************************************************************/
// clock_frequency is used by R_CGC_ClockHzGet(). Do not change the order of the array entries
static long clock_frequency[5]= {BSP_LOCO_HZ,
		                         BSP_HOCO_HZ,
		                         BSP_CFG_XTAL_HZ,
		                         BSP_SUB_CLOCK_HZ,
								 0xFF};
static bool loco_stab_flag = true;    // local flag used to maintain stabilization state of LOCO
static bool subosc_stab_flag = true;  // local flag used to maintain stabilization state of SubOSC
//static void update_loco_stab(void);    // local function used as callback to LOCO stabilization delay routine
//static void update_subosc_stab(void);  // local function used as callback to SubOSC stabilization delay routine
/**********************************************************************************************************************
 * Function Name: R_CGC_Start
 * Description  : This function will start the specified clock irrespective of whether it is currently active or not.
 * Arguments    : clock_source -
 *                    Which clock source to initialize.
 *                *clock_config-
 *                     pointer to a structure that contains the dividers or multipliers to be used when configuring
 *                     the PLL.
 * Return Value : CGC_SUCCESS -
 *                    Clock initialized successfully.
 *                CGC_ERR_ILL_PARAM -
 *                    Invalid argument used.
 *                CGC_ERR_CLOCK_ACTIVE
 *                    Active clock source specified for modification. This applies specifically to the
 *                    PLL dividers/multipliers which cannot be modified if the PLL is active. It has to
 *                    be stopped first before modification.
 *********************************************************************************************************************/

cgc_err_t R_CGC_ClockStart(cgc_clocks_t clock_source,
		cgc_clock_config_t* pClock_config)
{
	uint32_t temp;  //used to check if PLL divider/multiplier values are within acceptable MHz range
#if (CGC_CFG_PARAM_CHECKING_ENABLE == 1)
	if ((clock_source > CGC_PLL) || (clock_source < CGC_LOCO))
	{
		return CGC_ERR_ILL_PARAM;
	}
	if (clock_source == CGC_PLL)
	{
		if (pClock_config == NULL)
		{
			return CGC_ERR_NULL_PTR;
		}
		if (SYSTEM.SCKCR3.WORD == CGC_PLL) // if current clock is PLL, divider/multiplier cannot be modified
		{
			return CGC_ERR_CLOCK_ACTIVE;
		}
		if ((pClock_config->divider > PLL_DIV_4) || (pClock_config->divider < PLL_DIV_1))
		{
			return CGC_ERR_ILL_PARAM;
		}

		if ((pClock_config->multiplier > PLL_MUL_8)	|| (pClock_config->multiplier < PLL_MUL_6))
		{
			return CGC_ERR_ILL_PARAM;
		}
		switch (pClock_config->divider) //check that output of divider is between 4 and 8 MHz
		{
		case PLL_DIV_1:
			temp = BSP_CFG_XTAL_HZ/1;
			if ((temp > MAX_PLL_DIV_OP) || (temp < MIN_PLL_DIV_OP))
			{
				return CGC_ERR_ILL_PARAM;
			}
		break;
		case PLL_DIV_2:
			temp = BSP_CFG_XTAL_HZ/2;
			if ((temp > MAX_PLL_DIV_OP) || (temp < MIN_PLL_DIV_OP))
			{
				return CGC_ERR_ILL_PARAM;
			}
		break;
		case PLL_DIV_4:
			temp = BSP_CFG_XTAL_HZ/4;
			if ((temp > MAX_PLL_DIV_OP) || (temp < MIN_PLL_DIV_OP))
			{
				return CGC_ERR_ILL_PARAM;
			}
		break;
		default:
			return CGC_ERR_ILL_PARAM;
		}

		switch (pClock_config->multiplier) //check that output of multiplier is between 32 and 48Mhz
		{
		case PLL_MUL_6:
			temp = temp*6;
			if ((temp > MAX_PLL_MUL_OP) || (temp < MIN_PLL_MUL_OP))
			{
				return CGC_ERR_ILL_PARAM;
			}
		break;
		case PLL_MUL_8:
			temp = temp*8;
			if ((temp > MAX_PLL_MUL_OP) || (temp < MIN_PLL_MUL_OP))
			{
				return CGC_ERR_ILL_PARAM;
			}
		break;
		default:        return CGC_ERR_ILL_PARAM;

		}
	}
#endif //CGC_CFG_PARAM_CHECKING_ENABLE
	SYSTEM.PRCR.WORD = 0xA503; // unlock clock control registers
	if (clock_source == CGC_LOCO)
	{
		if (SYSTEM.LOCOCR.BYTE == CLOCK_ACTIVE)
		{
			return CGC_ERR_CLOCK_ACTIVE;
		}
		else
		{
			//TODO: check to make sure that at least 5 LOCO cycles have passed after stopping LOCO before restarting it
			SYSTEM.LOCOCR.BYTE = CLOCK_ACTIVE; // Start LOCO Oscillation
		}
	}
	if (clock_source == CGC_HOCO)
	{
		if (SYSTEM.HOCOCR.BYTE == CLOCK_ACTIVE)
		{
			return CGC_ERR_CLOCK_ACTIVE;
		}
		else
		{
#if (!CGC_CFG_HOCO_DEBUG)
			if (SYSTEM.OSCOVFSR.BIT.HCOVF == OVERFLOW_OCCURRED) // HCOVF has to be 0 before HOCO can be started
			{
				return CGC_ERR_NOT_STABILIZED;
			}
#endif
			SYSTEM.HOCOCR.BYTE = CLOCK_ACTIVE; // Start HOCO Oscillation
		}
	}
	if (clock_source == CGC_MAIN_OSC)
	{
		if (SYSTEM.MOSCCR.BYTE == CLOCK_ACTIVE)
		{
			return CGC_ERR_CLOCK_ACTIVE;
		}
		else
		{
			if (SYSTEM.OSCOVFSR.BIT.MOOVF == OVERFLOW_OCCURRED) // MOOVF has to be 0 before Main OCO can be started
			{
				return CGC_ERR_NOT_STABILIZED;
			}
			SYSTEM.MOSCCR.BYTE = CLOCK_ACTIVE; // Start Main OCO Oscillation
		}
	}
	if (clock_source == CGC_SUBCLOCK)
	{
		if (SYSTEM.SOSCCR.BYTE == CLOCK_ACTIVE)
		{
			return CGC_ERR_CLOCK_ACTIVE;
		}
		else
		{
	//TODO: Check to make sure at least 5 SUBOSC cycles have passed since it was turned off before turning it back on
			while (SYSTEM.SOSCCR.BYTE == 0x01) // Confirm that the register has been written to
			{
				SYSTEM.SOSCCR.BYTE = CLOCK_ACTIVE; // Start Sub Clock Oscillation
			}
		}
	}
	if (clock_source == CGC_PLL)
	{
		if (SYSTEM.MOSCCR.BYTE != CLOCK_ACTIVE)    // if Main OSC is not active, PLL cannot be turned ON
		{
			return CGC_ERR_MAIN_OCO_INACTIVE;
		}
		else
		{
			if (SYSTEM.PLLCR2.BYTE == CLOCK_ACTIVE) // if PLL is already running, divider/multiplers cannot be modified
			{
				return CGC_ERR_CLOCK_ACTIVE;
			}
			else
			{
				if (SYSTEM.OSCOVFSR.BIT.PLOVF == OVERFLOW_OCCURRED)// PLOVF has to be 0 before PLL can be started
				{
					return CGC_ERR_NOT_STABILIZED;
				}
				SYSTEM.PLLCR.WORD = ((pClock_config->divider) | (pClock_config->multiplier << 8));
				SYSTEM.PLLCR2.BYTE = CLOCK_ACTIVE; // Start PLL Oscillation
			}
		}
	}
	SYSTEM.PRCR.WORD = 0xA500; // lock clock control registers
	return CGC_SUCCESS;
}
/*********************************************************************************************************************
 * Function Name: R_CGC_ClockStop
 * Description  : This function will stop the specified clock if it is active and not configured as the system clock
 * Arguments    : clock_source -
 *                    Which clock source to stop.

 * Return Value : CGC_SUCCESS -
 *                    Clock stopped successfully.
 *                CGC_ERR_CLOCK_ACTIVE
 *                    Current System clock source specified for stopping. This is not allowed.
 *                CGC_ERR_OSC_DET_ENABLED-
 *                    LOCO cannot be stopped if OScillation stop detection is enabled
 *                CGC_ERR_ILL_PARAM -
 *                    Invalid argument used.
 *********************************************************************************************************************/

cgc_err_t R_CGC_ClockStop(cgc_clocks_t clock_source)
{
#if (CGC_CFG_PARAM_CHECKING_ENABLE == 1)
	if ((clock_source > CGC_PLL) || (clock_source < CGC_LOCO))
	{
		return CGC_ERR_ILL_PARAM;
	}
#endif//CGC_CFG_PARAM_CHECKING_ENABLE
	if (clock_source == SYSTEM.SCKCR3.BIT.CKSEL) // The currently active system clock source cannot be stopped
	{
		return CGC_ERR_CLOCK_ACTIVE;
	}

	SYSTEM.PRCR.WORD = 0xA503; // unlock clock control registers

	if (clock_source == CGC_LOCO)
	{
		if (SYSTEM.LOCOCR.BYTE == CLOCK_ACTIVE)
		{
#if CGC_CFG_OSC_STOP_DET_USED
			if(SYSTEM.OSTDCR.BIT.OSTDE == OSCILLATION_STOP_ENABLED)
			{
				SYSTEM.PRCR.WORD = 0xA500;       // lock clock control registers
				return CGC_ERR_OSC_STOP_DET_ENABLED;  // LOCO cannot be stopped if OSC Stop Detect is enabled
			}
#endif
			//TODO: Check that LOCO is stable before stopping
			SYSTEM.LOCOCR.BYTE = CLOCK_INACTIVE; // Stop LOCO Oscillating
		}
	}
	else if (clock_source == CGC_HOCO)
	{
		if (SYSTEM.HOCOCR.BYTE == CLOCK_ACTIVE)
		{
			while (SYSTEM.OSCOVFSR.BIT.HCOVF == OVERFLOW_NOT_OCCURRED)
			{
				// If HOCO is active, HCOVF has to be 1 before it can be stopped
			}

			SYSTEM.HOCOCR.BYTE = CLOCK_INACTIVE; // Stop HOCO Oscillation
		}
	}
	else if (clock_source == CGC_MAIN_OSC)
	{
		if (SYSTEM.SCKCR3.BIT.CKSEL == CGC_PLL) // Main OCO cannot be stopped if PLL is the current system clock
		{
			return CGC_ERR_CLOCK_ACTIVE;
		}
		if (SYSTEM.MOSCCR.BYTE == CLOCK_ACTIVE)
		{
			while (SYSTEM.OSCOVFSR.BIT.MOOVF == OVERFLOW_NOT_OCCURRED)
			{
				// If Main OCO is active, MOOVF has to be 1 before it can be stopped
			}
			SYSTEM.MOSCCR.BYTE = CLOCK_INACTIVE; // Stop Main OCO Oscillation
		}
	}
	else if (clock_source == CGC_SUBCLOCK)
	{
		if (SYSTEM.SOSCCR.BYTE == CLOCK_ACTIVE)
		{
			//TODO: Check that SubOSC is stable before stopping
			SYSTEM.SOSCCR.BYTE = CLOCK_INACTIVE; // Stop Sub Clock Oscillation
			while (SYSTEM.SOSCCR.BYTE != CLOCK_INACTIVE)
			{
				// wait until that the register has been written to
			}
		}
	}
	else if (clock_source == CGC_PLL)
	{
		if (SYSTEM.PLLCR2.BYTE == CLOCK_ACTIVE)
		{
			while (SYSTEM.OSCOVFSR.BIT.PLOVF == OVERFLOW_NOT_OCCURRED)
			{
				// If PLL is active,  an overflow has to have occurred (PLOVF=1) before it can be stopped
			}
			SYSTEM.PLLCR2.BYTE = CLOCK_INACTIVE; // Stop PLL Oscillation
		}
	}
	else
	{
		//nothing here
	}
	SYSTEM.PRCR.WORD = 0xA500; // lock clock control registers
	return CGC_SUCCESS;
}
/*********************************************************************************************************************
 * Function Name: R_CGC_SystemClockSet
 * Description  : This function set the specified clock as the system clock and configure the internal dividers for
 *                ICLK,PCLKB,PCLKD and FCLK. If the specified clock source is active but has not stabilized yet, then
 *                the function will block until the specified clock has stabilized.
 *                Maximum stabilization wait times:
 *                HOCO    :    40 usec
 *                LOCO    :    4  usec
 *                Main OSC:    65536 Main OSC Cycles
 *                SubClock:    2  sec
 *                THIS FUNCTION DOES NOT CHECK TO SEE IF THE OPERATING MODE SUPPORTS THE SPECIFIED CLOCK SOURCE
 *                AND DIVIDER VALUES. SETTING A CLOCK SOURCE AND DVIDER OUTSIDE THE RANGE SUPPORTED BY THE
 *                CURRENT OPERATING MODE WILL RESULT IN UNDEFINED OPERATION.
 * Arguments    : cgc_clocks_t clock_source -
 *                    Clock source to set as system clock
 *                cgc_system_clock_config_t* pClock_config
 *                    Pointer to the clock dividers configuration passed by the caller
 *
 * Return Value : CGC_SUCCESS -
 *                CGC_ERR_CLOCK_INACTIVE
 *                    The specified clock source is inactive.
 *                CGC_ERR_NULL_PTR -
 *                    the pclock_config parameter is NULL
 *                CGC_ERR_ILL_PARAM -
 *                    Invalid argument used.
 *                    ICLK is not set as the fastest clock.
 *********************************************************************************************************************/
cgc_err_t  R_CGC_SystemClockSet(cgc_clocks_t clock_source, cgc_system_clock_config_t* pclock_config)
{
	cgc_err_t err;
	uint32_t sckcr_temp;
#if (CGC_CFG_PARAM_CHECKING_ENABLE == 1)
	if (pclock_config == NULL)
	{
		return CGC_ERR_NULL_PTR;
	}
	if ((clock_source > CGC_PLL) || (clock_source < CGC_LOCO))
	{
		return CGC_ERR_ILL_PARAM;
	}
	if ((pclock_config->fclk < pclock_config->iclk) ||
		(pclock_config->pclkb < pclock_config->iclk) ||
		(pclock_config->pclkd < pclock_config->iclk))
	{
		return CGC_ERR_ILL_PARAM;
	}
#endif//CGC_CFG_PARAM_CHECKING_ENABLE

	if (clock_source != SYSTEM.SCKCR3.BIT.CKSEL) //if clock_source is not the current system clock, check stabilization
	{
		err = R_CGC_ClockCheck(clock_source); // Check if the clock is stabilized/active/valid
		if ((err == CGC_ERR_CLOCK_INACTIVE) || (err == CGC_ERR_ILL_PARAM))
		{
			return err;  // If clock is inactive or an invalid clock source is chosen return with error code
		}
		else
		{
			while(R_CGC_ClockCheck(clock_source)!=CGC_ERR_STABILIZED)
			{
			  // if clock is active, then block till stabilized.
			}
		}
	}
	/* if clock_source is the current system clock or clock_source has stabilized, modify the clock registers*/
	SYSTEM.PRCR.WORD = 0xA503; // unlock clock control registers
	SYSTEM.SCKCR3.BIT.CKSEL = clock_source; //set the system clock
	sckcr_temp = ((uint32_t)pclock_config->pclkd |
			     (uint32_t)pclock_config->pclkb << 8 |
			     (uint32_t)pclock_config->iclk  << 24|
			     (uint32_t)pclock_config->fclk  << 28);
	SYSTEM.SCKCR.LONG = sckcr_temp;         // set the internal dividers
	while (SYSTEM.SCKCR.LONG != sckcr_temp)
	{
		// make sure write to sckcr is successful
	}
	SYSTEM.PRCR.WORD = 0xA500; // lock clock control registers
	return CGC_SUCCESS;
}
/*********************************************************************************************************************
 * Function Name: R_CGC_ClockHzGet
 * Description  : This function will return the requested internal clock frequency in Hz
 * Arguments    : cgc_system_clocks_t clock -
 *                    Specifies the internal clock whose frequency will be returned in hz .

 * Return Value : uint32_t frequency
 * 					clock  frequency in hz
 *
 *********************************************************************************************************************/
uint32_t R_CGC_ClockHzGet(cgc_system_clocks_t clock)
{
#if (CGC_CFG_PARAM_CHECKING_ENABLE == 1)
	if ((clock > CGC_FCLK) || (clock < CGC_PCLKD))
	{
		return CGC_ERR_ILL_PARAM;
	}
#endif//CGC_CFG_PARAM_CHECKING_ENABLE
    if (SYSTEM.SCKCR3.BIT.CKSEL == CGC_PLL)
    	// calculate the output of the PLL circuit and enter into the clock_frequency[5] array
    {
    	if (SYSTEM.PLLCR.BIT.STC == PLL_MUL_6)
    	{
    		clock_frequency[CGC_PLL] = (BSP_CFG_XTAL_HZ>>SYSTEM.PLLCR.BIT.PLIDIV)*6;
    	}
    	else  if (SYSTEM.PLLCR.BIT.STC == PLL_MUL_8)
    	{
    		clock_frequency[CGC_PLL] = (BSP_CFG_XTAL_HZ>>SYSTEM.PLLCR.BIT.PLIDIV)*8;
    	}
    }
	if (clock == CGC_PCLKD)
	{
		return (clock_frequency[SYSTEM.SCKCR3.BIT.CKSEL]/(uint32_t)(1 << SYSTEM.SCKCR.BIT.PCKD));
	}
	else if (clock == CGC_PCLKB)
	{
		return (clock_frequency[SYSTEM.SCKCR3.BIT.CKSEL]/(uint32_t)(1 << SYSTEM.SCKCR.BIT.PCKB));
	}
	else if (clock == CGC_ICLK)
	{
		return (clock_frequency[SYSTEM.SCKCR3.BIT.CKSEL]/(uint32_t)(1 << SYSTEM.SCKCR.BIT.ICK));
	}
	else if (clock == CGC_FCLK)
	{
		return (clock_frequency[SYSTEM.SCKCR3.BIT.CKSEL]/(uint32_t)(1 << SYSTEM.SCKCR.BIT.FCK));
	}
	else
	{
		return CGC_ERR_ILL_PARAM; //should never get here
	}

}
/*********************************************************************************************************************
 * Function Name: R_CGC_ClockCheck
 * Description  : This function will check the specified clock for stabilization by looking at the state of the
 *                OSCOVFSR register.
 * Arguments    : cgc_clocks_t clock_source -
 *                    Which clock source to check for stabilization.

 * Return Value : CLOCK_ERR_STABILIZED -
 *                    Clock stabilized
 *                CGC_ERR_NOT_STABILIZED
 *                    Clock not stabilized.
 *                CGC_ERR_CLOCK_INACTIVE
 *                    Clock not turned on
 *                CGC_ERR_ILL_PARAM
 *                    Illegal parameter passed
 *********************************************************************************************************************/
cgc_err_t R_CGC_ClockCheck(cgc_clocks_t clock_source)
{
#if (CGC_CFG_PARAM_CHECKING_ENABLE == 1)
	if ((clock_source > CGC_PLL) || (clock_source < CGC_LOCO))
	{
		return CGC_ERR_ILL_PARAM;
	}
#endif//CGC_CFG_PARAM_CHECKING_ENABLE
	if (clock_source == CGC_LOCO)
	{
		if (SYSTEM.LOCOCR.BYTE == CLOCK_ACTIVE)
		{
			// TODO: Implement a way to check for LOCO stabilization
			if (true == loco_stab_flag)
			{
				return CGC_ERR_STABILIZED;
			}
			else
			{
				return CGC_ERR_NOT_STABILIZED;
			}
		}
		else
		{
			return CGC_ERR_CLOCK_INACTIVE;
		}
	}
	else if (clock_source == CGC_HOCO)
	{
		if (SYSTEM.HOCOCR.BYTE == CLOCK_ACTIVE)                   // Check to see if HOCO is already running
		{
			if (SYSTEM.OSCOVFSR.BIT.HCOVF == OVERFLOW_OCCURRED)   // If it is running, check if it has stabilized
			{
				return CGC_ERR_STABILIZED;
			}
			else
			{
				return CGC_ERR_NOT_STABILIZED;
			}
		}
		else
		{
			return CGC_ERR_CLOCK_INACTIVE;
		}
	}
	else if (clock_source == CGC_MAIN_OSC)
	{
		if (SYSTEM.MOSCCR.BYTE == CLOCK_ACTIVE)                    // Check to see if Main OCO is already running
		{
			if (SYSTEM.OSCOVFSR.BIT.MOOVF == OVERFLOW_OCCURRED)   // If it is running, check if it has stabilized
			{
				return CGC_ERR_STABILIZED;
			}
			else
			{
				return CGC_ERR_NOT_STABILIZED;
			}
		}
		else
		{
			return CGC_ERR_CLOCK_INACTIVE;
		}
	}
	else if (clock_source == CGC_SUBCLOCK)
	{
		if (SYSTEM.SOSCCR.BYTE == CLOCK_ACTIVE)
		{
			//TODO: Implement a way to check for subOSC stabilization
			//Wait for tSUBOSCWT time for stabilization
			if (true == subosc_stab_flag)
			{
				return CGC_ERR_STABILIZED;
			}
			else
			{
				return CGC_ERR_NOT_STABILIZED;
			}
		}
		else
		{
			return CGC_ERR_CLOCK_INACTIVE;
		}

	}
	else if (clock_source == CGC_PLL)
	{
		if (SYSTEM.PLLCR2.BYTE == CLOCK_ACTIVE)                   // Check to see if PLL is already running
		{
			if (SYSTEM.OSCOVFSR.BIT.PLOVF == OVERFLOW_OCCURRED)  // If it is running, check if it has stabilized
			{
				return CGC_ERR_STABILIZED;
			}
			else
			{
				return CGC_ERR_NOT_STABILIZED;
			}
		}
		else
		{
			return CGC_ERR_CLOCK_INACTIVE;
		}
	}
	else
	{
		return CGC_ERR_ILL_PARAM; // should never get here
	}
}

/*********************************************************************************************************************
 * Function Name: R_CGC_ClockOutputConfig
 * Description  : This function will set the specified clock (after divider) as output on the CLKOUT pin.
 *                Pin configuration by the MPC registers is required and not handled in this module.
 * Arguments    : cgc_clocks_t clock_source -
 *                    Which clock source to set as clock out.
 *                cgc_clockout_dividers_t clockout_divider
 *                    Specify the divider to be used for the output
 *                bool enable_now
 *                    Enable the output at the end of this function

 * Return Value : CGC_SUCCESS -
 *                    Successful in setting the CKOSTP register according to parameters passed
 *                CGC_ERR_ILL_PARAM
 *                		Illegal parameter passed
 *********************************************************************************************************************/

cgc_err_t R_CGC_ClockOutputConfig(cgc_clocks_t clock_source, cgc_clockout_dividers_t clockout_divider, bool enable_now)
{
#if (CGC_CFG_PARAM_CHECKING_ENABLE == 1)
	if ((clock_source > CGC_PLL) || (clock_source < CGC_LOCO))
	{
		return CGC_ERR_ILL_PARAM;
	}
	if ((clockout_divider > CLOCKOUT_DIV_16) || (clockout_divider < CLOCKOUT_DIV_1))
	{
		return CGC_ERR_ILL_PARAM;
	}
#endif//CGC_CFG_PARAM_CHECKING_ENABLE
	SYSTEM.PRCR.WORD = 0xA503; // unlock clock control registers
	SYSTEM.CKOCR.BIT.CKOSTP = CLOCK_INACTIVE; // disable clock output
	SYSTEM.CKOCR.WORD = (uint16_t)((clock_source <<8) | (clockout_divider<<12) | (CLOCK_INACTIVE << 15));
	//TODO: Check for max clock that can be output on CLKOUT. This is TBD in the manual.
	SYSTEM.PRCR.WORD = 0xA500; // lock clock control registers

	if(enable_now == true)
	{
		R_CGC_ClockOutputEnable(true);
	}

	return CGC_SUCCESS;
}
/*********************************************************************************************************************
 * Function Name: R_CGC_ClockOutputEnable
 * Description  : This function will enable or disable the output on the CLKOUT pin. The clock source has to be
 *                pre-configured via R_CGC_ClockOutputConfig(). Pin configuration by the MPC registers is required
 *                and not handled in this module.
 * Arguments    : bool enable_now
 *                    Enable/disable the clock output

 * Return Value : CGC_SUCCESS -
 *
 *********************************************************************************************************************/
cgc_err_t R_CGC_ClockOutputEnable(bool enable)
{
	SYSTEM.PRCR.WORD = 0xA503; // unlock clock control registers
	if (enable == true)
	{
		SYSTEM.CKOCR.BIT.CKOSTP = CLOCK_ACTIVE;   // enable clock output
	}
	else
	{
		SYSTEM.CKOCR.BIT.CKOSTP = CLOCK_INACTIVE; // disable clock output
	}
    SYSTEM.PRCR.WORD = 0xA500; // lock clock control registers

	return CGC_SUCCESS;
}


#if CGC_CFG_OSC_STOP_DET_USED
/**********************************************************************************************************************
 * Function Name: R_CGC_OscStopDetect
 * Description  : This function will enable or disable the oscillation stop detection for the main clock.
 *                The MCU will automatically switch the system clock to LOCO when a stop is detected if Main Clock is
 *                the system clock.
 *                If the system clock is the PLL, then the clock source will not be changed and the PLL free running
 *                frequency will be the system clock frequency.
 * Arguments    : bool enable
 *                    Enable/disable Oscillation Stop Detection

 * Return Value : CGC_SUCCESS -
 *				  CGC_ERR_OSC_STOP_DETECTED-
 *				  The Oscillation stop detect status flag is set. Under this condition it is not possible to disable the
 *				  Oscillation stop detection function.
 *				  CGC_ERR_NULL_PTR -
 *				  Null pointer passed for callback function.
 **********************************************************************************************************************/
cgc_err_t R_CGC_OscStopDetect(void (*pcallback)(void* pdata), bool enable)
{
	SYSTEM.PRCR.WORD = 0xA503; // unlock clock control registers
	if (true == enable)
	{
#if (CGC_CFG_PARAM_CHECKING_ENABLE == 1)
		if (pcallback == NULL)
		{
			SYSTEM.PRCR.WORD = 0xA500; // lock clock control registers
			return CGC_ERR_NULL_PTR;
		}
		//TODO: Assign NMI callback here
		SYSTEM.OSTDCR.BYTE = 0x81;     // enable Oscillation Stop Detect function and the interrupt.
#endif
	}
	else if (false == enable)
	{
		if (SYSTEM.OSTDSR.BYTE == OSCILLATION_STOP_DETECTED)
		{
			SYSTEM.PRCR.WORD = 0xA500; // lock clock control registers
			return CGC_ERR_OSC_STOP_DETECTED;
		}
		SYSTEM.OSTDCR.BYTE = 0x00;     // disable OScillation Stop Detect function and the interrupt.
	}

    SYSTEM.PRCR.WORD = 0xA500; // lock clock control registers

	return CGC_SUCCESS;
}

/**********************************************************************************************************************
 * Function Name: R_CGC_OscStopStatusClear
 * Description  : This function will clear the Oscillation Stop Detection Status register. This register is not cleared
 * 				  automatically if the stopped clock is restarted. This function blocks for about 3 ICLK cycles until
 * 				  the status register is cleared.
 *
 * Arguments    : None

 * Return Value : CGC_SUCCESS -
 *				  CGC_ERR_OSC_STOP_CLOCK_ACTIVE-
 *				  The Oscillation Detect Status flag cannot be cleared if the Main OCO or PLL is set as the
 *				  system clock. Change the system clock before attempting to clear this bit.
 **********************************************************************************************************************/
cgc_err_t R_CGC_OscStopStatusClear()
{

	if (SYSTEM.OSTDSR.BYTE == OSCILLATION_STOP_DETECTED)
	{
		if ((SYSTEM.SCKCR3.BIT.CKSEL == CGC_PLL) || (SYSTEM.SCKCR3.BIT.CKSEL == CGC_MAIN_OSC))
		{
			return CGC_ERR_OSC_STOP_CLOCK_ACTIVE;
		}
		SYSTEM.PRCR.WORD = 0xA503;                           // unlock clock control registers
		SYSTEM.OSTDSR.BIT.OSTDF = OSCILLATION_STOP_CLEARED;  // clear the status register
		SYSTEM.PRCR.WORD = 0xA500;                           // lock clock control registers
		while (SYSTEM.OSTDSR.BIT.OSTDF != OSCILLATION_STOP_CLEARED)
		{
			//This will take at least three ICLK cycles
		}
	}
   	return CGC_SUCCESS;
}
#endif

/*****************************************************************************
* Function Name: R_CGC_GetVersion
* Description  : Returns the version of this module. The version number is
*                encoded such that the top two bytes are the major version
*                number and the bottom two bytes are the minor version number.
* Arguments    : none
* Return Value : version number
******************************************************************************/
#if defined(__RENESAS__)
#pragma inline(R_CGC_GetVersion)
#endif
uint32_t  R_CGC_GetVersion(void)
{
    uint32_t version;

    version = (CGC_RX_VERSION_MAJOR << 16) | CGC_RX_VERSION_MINOR;

    return version;
}
