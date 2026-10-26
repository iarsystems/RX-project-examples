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
/*********************************************************************************************************************
* File Name	   : r_lpc_rx.c
* Description  :
**********************************************************************************************************************/
/*********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 10.03.2013 1.00    First Release
**********************************************************************************************************************/

/*********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
**********************************************************************************************************************/
/* Includes board and MCU related header files. */
#include "platform.h"
/* Public interface header file for this package. */
#include "r_lpc_rx_if.h"
/* Configuration for this package. */
#include "r_lpc_config.h"
/* Private header file for this package. */
#include "r_lpc_rx.h"
/* CGC Module header file . */
#include "r_cgc_rx_if.h"
/* include definition for NULL*/
#include <stddef.h>
/* include definition for clrpsw_i() */
#include <machine.h>
/********************************************************************************************************************
Macro definitions
*********************************************************************************************************************/

/********************************************************************************************************************
Typedef definitions
*********************************************************************************************************************/

/********************************************************************************************************************
Private global variables and functions
*********************************************************************************************************************/

/*********************************************************************************************************************
 * Function Name: R_LPC_ConfigureLowPowerMode
 * Description  : This function will configure the MCU for the specified Low Power Mode. IN the Low Power Mode,
 *                the CPU is turned OFF and peripherals may be on or off depending on the mode. The supported
 *                low power modes are listed in the enum lpc_low_power_t
 * Arguments    : e_mode - Low Power Mode
 *                    LP_SLEEP:
 *                    LP_ALL_MODULE_STOP: This is not a hardware mode on the RX111. If this argument is passed,
 *                                        then the  program turns off all the peripherals on the MCU except RAM.
 *                    LP_DEEP_SLEEP:
 *                    LP_SOFTWARE_STANDBY:
 *
 * Return Value : LPC_SUCCESS -
 *                LPC_ERR_ILL_PARAM -
 *                    Invalid argument used.
 *                LPC_ERR_OSC_STOP_ENABLED -
 *                    The Software Standby Mode cannot be activated if oscillation stop detection is enabled.
 *                    The function will configure the low power register for SW Standby Mode but will
 *                    return this error code instead of LPC_SUCCESS
 ********************************************************************************************************************/
lpc_err_t R_LPC_ConfigureLowPowerMode(lpc_low_power_mode_t e_mode)
{
#if (LPC_CFG_PARAM_CHECKING_ENABLE == 1)
	if ((e_mode >= LP_INVALID_MODE) || (e_mode < LP_SLEEP))
	{
		return LPC_ERR_ILL_PARAM;
	}
#endif//LPC_CFG_PARAM_CHECKING_ENABLE

	SYSTEM.PRCR.WORD = 0xA502;            // Remove protection for modifying low power registers

	if(e_mode == LP_SLEEP)
	{
		SYSTEM.SBYCR.BIT.SSBY =	0;        // Move to Sleep on wait();DSLPE also needs to be cleared
		SYSTEM.MSTPCRC.BIT.DSLPE = 0;       // Move to Sleep on wait()
	}
	else if (e_mode == LP_ALL_MODULE_STOP)
	{
		DTC.DTCST.BIT.DTCST = 0;          // Clear the DTCST bit before disabling the DTC module
		SYSTEM.MSTPCRA.LONG = 0xFFFFFFFF;  // Turn off all peripheral modules
		SYSTEM.MSTPCRB.LONG = 0xFFFFFFFF;
		SYSTEM.MSTPCRC.LONG = 0xFFFFFFFE;  // Leave RAM0 on
	}
	else if (e_mode == LP_DEEP_SLEEP)
	{
		SYSTEM.SBYCR.BIT.SSBY =	 0;         // Move to DeepSleep on wait(). DSLPE also needs to be set
		SYSTEM.MSTPCRC.BIT.DSLPE = 1;       // Move to DeepSleep on wait()
	}
	else if (e_mode == LP_SW_STANDBY)
	{
		SYSTEM.SBYCR.BIT.SSBY =	 1;         // Move to Sw Stby on wait()
		if (SYSTEM.OSTDCR.BIT.OSTDE == OSC_STOP_ENABLED)
		{
			SYSTEM.PRCR.WORD = 0xA500;      // Set protection for modifying low power registers
			return LPC_ERR_OSC_STOP_ENABLED;// Cannot enter SW_Standby if Oscillation Stop is enabled
		}
	}
	else
	{
		//nothing here
	}
	SYSTEM.PRCR.WORD = 0xA500;             // Set protection for modifying low power registers
	return LPC_SUCCESS;
}

/********************************************************************************************************************
 * Function Name: R_LPC_ConfigureOperatingMode
 * Description  : This function will configure the MCU for the specified operating mode. In any of the operating
 *                 modes, the CPU is running, but the maximum clock frequencies are limited based on the mode.
 *                 The supported mode are listed under in the lpc_operating_mode_t enum. The modes with the
 *                 lower operating frequencies use regulator switches to reduce current draw. It is imperative
 *                 that when switching to a higher clock frequency, the mode is changed first to support that
 *                 frequency.
 * Arguments    : e_mode -
 *					Operating Mode
 *
 * Return Value : LPC_SUCCESS -
 * 				  LPC_ERR_CLOCK_EXCEEDED -
 * 				  If the current system clocks dividers are higher than those supported by the e_mode operating mode.
 * 				  If the PLL is the system clock when transition to Medium Speed is attempted.
 * 				  If PLL, Main Clock, HOCO or LOCO are active when transition to Low Speed is attempted.
 *                LPC_ERR_ILL_PARAM -
 *                    Invalid argument used.
 ********************************************************************************************************************/
lpc_err_t R_LPC_ConfigureOperatingMode(lpc_operating_mode_t e_mode)
{
#if (LPC_CFG_PARAM_CHECKING_ENABLE == 1)
	if ((e_mode >= OP_INVALID_MODE) || (e_mode < OP_HIGH_SPEED))
	{
		return LPC_ERR_ILL_PARAM;
	}

#endif//LPC_CFG_PARAM_CHECKING_ENABLE


	SYSTEM.PRCR.WORD = 0xA502;            // Remove protection for modifying low power registers
	if(e_mode == OP_HIGH_SPEED)
	{
		while(SYSTEM.SOPCCR.BIT.SOPCMTSF == TRANSITION_ONGOING)
		{
			//do not modify SOPCCR if a transition is ongoing
		}
		SYSTEM.SOPCCR.BIT.SOPCM = 0x0;   // Set to High Speed/Medium Speed Mode
		while(SYSTEM.OPCCR.BIT.OPCMTSF == TRANSITION_ONGOING)
		{
			//do not modify OPCCR if a transition is ongoing
		}
		SYSTEM.OPCCR.BIT.OPCM = 0x0;     // Set to High Speed Mode
	}
	else if (e_mode == OP_MIDDLE_SPEED)
	{

		if((R_CGC_ClockHzGet(CGC_ICLK)> MAX_MIDDLE_SPEED_FREQ) || // Check if clock speeds are within mode limits
		   (SYSTEM.SCKCR3.BIT.CKSEL == CGC_PLL))	 // PLL cannot be used as the system clock in Medium Speed mode
		{
			SYSTEM.PRCR.WORD = 0xA500;           // Set protection for modifying low power registers
			return LPC_ERR_CLOCK_EXCEEDED;
		}
		while(SYSTEM.SOPCCR.BIT.SOPCMTSF == TRANSITION_ONGOING)
		{
			//do not modify SOPCCR if a transition is ongoing
		}
		SYSTEM.SOPCCR.BIT.SOPCM = 0x0;   // Set to High Speed/Medium Speed Mode
		while(SYSTEM.OPCCR.BIT.OPCMTSF == TRANSITION_ONGOING)
		{
			//do not modify OPCCR if a transition is ongoing
		}
		SYSTEM.OPCCR.BIT.OPCM = 0x2;     // Set to Medium Speed Mode
	}
	else if (e_mode == OP_LOW_SPEED)
	{
		if((R_CGC_ClockHzGet(CGC_ICLK)> MAX_LOW_SPEED_FREQ)|| // Check if clock speeds are within mode limits
		   (SYSTEM.LOCOCR.BYTE == LPC_CLOCK_ACTIVE)||         // Check if LOCO is running
		   (SYSTEM.HOCOCR.BYTE == LPC_CLOCK_ACTIVE)||         // Check if HOCO is running
		   (SYSTEM.MOSCCR.BYTE == LPC_CLOCK_ACTIVE)||         // Check if Main Osc is running
		   (SYSTEM.PLLCR2.BYTE == LPC_CLOCK_ACTIVE))          // Check if PLL  is running
		{
			SYSTEM.PRCR.WORD = 0xA500;           // Set protection for modifying low power registers
			return LPC_ERR_CLOCK_EXCEEDED;
		}
		while(SYSTEM.SOPCCR.BIT.SOPCMTSF == TRANSITION_ONGOING)
		{
			//do not modify SOPCCR if a transition is ongoing
		}
		SYSTEM.SOPCCR.BIT.SOPCM = 0x1;   // Set to Low Speed Mode
	}
	else
	{
		//nothing here
	}
	SYSTEM.PRCR.WORD = 0xA500;           // Set protection for modifying low power registers
	return LPC_SUCCESS;


}
/*********************************************************************************************************************
 * Function Name: R_LPC_LowPowerActivate
 * Description  : This function will activate the pre-configured low power mode. The wait() function re-enables
 * 				  interrupts.
 * Arguments    : void (*pcallback)(void* pdata)-
 * 				  Call back function to configure any un-configured interrupt that can be used to wake up from the low
 * 				  power mode. This can be a NULL if the interrupt(s) are already configured.
 *
 * Return Value : LPC_SUCCESS -
 *                LPC_ERR_ILL_PARAM -
 *                    Invalid argument used.
 ********************************************************************************************************************/
lpc_err_t R_LPC_LowPowerActivate(void (*pcallback)(void* pdata))
{
#if (LPC_CFG_PARAM_CHECKING_ENABLE == 1)

#endif//LPC_CFG_PARAM_CHECKING_ENABLE

   // Clear the I bit to disable interrupts
#if defined(__RENESAS__)
    clrpsw_i();
#endif

#if defined(__IAR_SYSTEMS_ICC__)
    __disable_interrupt();
#endif

	if (pcallback != NULL)
	{
		pcallback(NULL);             // Call callback to configure wake-up interrupt source
	}
	else
	{
		                             // Read last written IO register to make sure it is complete
	}

	wait();                          // Enter Low Power mode

	return LPC_SUCCESS;
}

/*********************************************************************************************************************
 * Function Name: R_LPC_ReturnClockSwitch
 * Description  : This function will configure the return clock switching parameter that allows the clock source to
 *                be switched on returning from Sleep Mode to either the HOCO, LOCO or Main Clock. The following
 *                items have to be followed to allow for Return Clock Switching:
 *                -When entering Sleep Mode, the system clock should be the SubOSC and the operating mode should be
 *                 Low Speed. On exiting sleep, the operating mode will return to whatever the mode was before
 *                 Low Speed was selected as the operating mode.
 *                -If the Main OSC is chosen as the Sleep Return clock source, make sure it does not exceed 8 MHz
 * Arguments    : lpc_clock_switch_t e_clock_source -
 *                  Specify the lock source to switch to.
 *                bool enable
 *                  Enable/disable the Return clock Source switching function.
 *
 * Return Value : LPC_SUCCESS -
 *                LPC_ERR_CLOCK_INACTIVE
 *                    The specified clock source is inactive.
 *                LPC_ERR_ILL_CLOCK_SOURCE
 *                    The Sleep Mode Return Clock Switch is only supported if the clock source at the time of
 *                    entry to sleep mode is the SubClock. The function will configure the MCU for Return Clock
 *                    Switching, but will return this error code.
 *********************************************************************************************************************/
lpc_err_t R_LPC_ReturnClockSwitch(lpc_clock_switch_t e_clock_source, bool enable)
{
#if (LPC_CFG_PARAM_CHECKING_ENABLE == 1)
	if (e_clock_source == LPC_MAIN_OSC)
	{
		if (BSP_CFG_XTAL_HZ > CLOCK_SWITCH_MAIN_OSC_MAX_FREQ)
		{
			return LPC_ERR_ILL_MAIN_CLK_FREQ;
		}
	}
#endif//LPC_CFG_PARAM_CHECKING_ENABLE
	SYSTEM.PRCR.WORD = 0xA502;            // Remove protection for modifying low power registers
	if (true == enable)
	{
		SYSTEM.RSTCKCR.BYTE = (uint8_t)(CLOCK_SWITCH_ENABLE | e_clock_source);
	}
	else
	{
		SYSTEM.RSTCKCR.BYTE = CLOCK_SWITCH_DISABLE;
	}
	SYSTEM.PRCR.WORD = 0xA500;           // Set protection for modifying low power registers

	if(SYSTEM.SCKCR3.WORD != SUB_CLOCK)
	{
		return LPC_ERR_ILL_CLOCK_SOURCE;
	}


	return LPC_SUCCESS;
}
/*****************************************************************************
* Function Name: R_LPC_GetVersion
* Description  : Returns the version of this module. The version number is
*                encoded such that the top two bytes are the major version
*                number and the bottom two bytes are the minor version number.
* Arguments    : none
* Return Value : version number
******************************************************************************/
#if defined(__RENESAS__)
#pragma inline(R_LPC_GetVersion)
#endif
uint32_t  R_LPC_GetVersion(void)
{
    uint32_t version;

    version = (LPC_RX_VERSION_MAJOR << 16) | LPC_RX_VERSION_MINOR;

    return version;
}
