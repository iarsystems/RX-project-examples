/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIESREGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_s12ad_user.c
* Version      : Applilet4 for RX113 V1.01.00.02 [31 Jul 2014]
* Device(s)    : R5F51138AxFP
* Tool-Chain   : IAR Systems ICCRX
* Description  : This file implements device driver for S12AD module.
* Creation Date: 2015-01-20
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_s12ad.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */


#define TSCDRH (*(volatile uint8_t*)(0x007FC0AD))	/* Define for the Temp calibration data High register */
#define TSCDRL (*(volatile uint8_t*)(0x007FC0AC))	/* Define for the Temp calibration data Low register */

#define ADC_FULLRANGE ((float)(0xFFF))				/* Number of conversion bits of ADC */
#define ADC_VREF ((float)(3.325))					/* Reference voltage of ADC */

/* ADC global flag - used to indicate ADC Interrupt from the ISR */
volatile uint8_t g_adc_flag = FALSE;

/* End user code. Do not edit comment generated here */



/***********************************************************************************************************************
* Function Name: r_s12ad_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = VECT_S12AD_S12ADI0
#if FAST_INTERRUPT_VECTOR == VECT_S12AD_S12ADI0
__fast_interrupt static void r_s12ad_interrupt(void)
#else
__interrupt static void r_s12ad_interrupt(void)
#endif
{
    /* Start user code. Do not edit comment generated here */

	/* Set the global flag to indicate an ADC conversion is complete */
	 g_adc_flag = TRUE;

    /* End user code. Do not edit comment generated here */
}


/* Start user code for adding. Do not edit comment generated here */

/************************************************************************************
* Function Name: r_s12ad_temperature_conversion
* Description  : This function converts the raw ADC Temperature Sensor reading in to
* 			   : Degree C. Please refer to section Temperature Sensor (TEMPSA) in the
* 			   : User's Manual: hardware for further details.
* Arguments    : uint16_t adc_result - Temperature Sensor channel ADC reading
*              : float* temperature - pointer to the temperature store variable.
* Return Value : None
*************************************************************************************/
void r_s12ad_temperature_conversion(uint16_t adc_result, float *temperature)
{
	float 	temperature_volt;	/* ADC counts to voltage converted value */
	float	v1;
	uint16_t adc_temp_cal88;	/* Variable to assemble the high and low cal. data */


	/* Read the calibration from the on-chip storage location */
	adc_temp_cal88 = ((TSCDRH << 8) + TSCDRL);

	/* Calculate the relative voltage from the ADC conversion */
	temperature_volt = ((float)(ADC_VREF / ADC_FULLRANGE) * (float)adc_result);

	v1 = ((float)3.3 * ((float)adc_temp_cal88 / (float)4096));

	(*temperature) =( ( (temperature_volt - v1) / (-0.00365F) ) + 88);
}

/***********************************************************************************
* End of function r_s12ad_temperature_conversion
*************************************************************************************/


/************************************************************************************
* Function Name: R_S12AD_GetTemperature
* Description  : This function sets up and performs a read from the RX113
* 			   : Temperature Sensor
* Arguments    : float* temperature - pointer to the temperature store variable.
* Return Value : None
*************************************************************************************/
void R_S12AD_GetTemperature(float* temperature)
{
	uint16_t adc_temp_result;

		R_S12AD_Create_Temperature();			/* Create an temperature ADC channel for the temperature*/

		/* Start an ADC Conversion */
		/* Note: From the data sheet it is critical to ignore the first conversion result */
		R_S12AD_Start();

		while (FALSE == g_adc_flag)
		{
			/* Loop while the ADC is sampling */
		}

		/* Reset the ADC Global flag */
			g_adc_flag = FALSE;


		/* Take a second sample that will now be valid */
		/* Start an ADC Conversion */
			R_S12AD_Start();

			while (FALSE == g_adc_flag)
			{
				/* Loop while the ADC is sampling */
			}


		/* Reset the ADC Global flag */
			g_adc_flag = FALSE;


		/* Fetch the ADC Temperature Conversion Result */
		R_S12AD_Get_ValueResult(ADTEMPSENSOR, &adc_temp_result);

		/* Convert the raw ADC result in to Degree C temperature */
		r_s12ad_temperature_conversion(adc_temp_result, temperature);
}

/***********************************************************************************
* End of function R_S12AD_GetTemperature
*************************************************************************************/

/* End user code. Do not edit comment generated here */
