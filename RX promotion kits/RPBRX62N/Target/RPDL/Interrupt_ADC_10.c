/*""FILE COMMENT""*******************************************************
* System Name	: Interrupt program for RX62Nxx
* File Name 	: Interrupt_ADC_10.c
* Version		: 0.10
* Contents		: Interrupt handlers for the ADC units
* Customer		: 
* Model			: 
* Order			: 
* CPU			: RX600
* Compiler		: RXC
* OS			: Nothing
* Programmer	: 
* Note			: 
************************************************************************
* Copyright,2010 RENESAS ELECTRONICS CORPORATION
* AND RENESAS SOLUTIONS CORPORATION
************************************************************************
* History		: 2010.04.20
*				: Ver 0.10
*""FILE COMMENT END""**************************************************/

#include "r_pdl_adc_10.h"
#include "r_pdl_adc_10_RX62Nxx.h"
#include "r_pdl_user_definitions.h"

/*""FUNC COMMENT""***************************************************
* Module outline: ADC interrupt processing
*-------------------------------------------------------------------
* Declaration	: void Interrupt_ADCn(void)
*-------------------------------------------------------------------
* Function		: Interrupt processing function for ADC unit n
*-------------------------------------------------------------------
* Argument		: Nothing
*-------------------------------------------------------------------
* Return value	: Nothing
*-------------------------------------------------------------------
* Input			: 
* Output		: 
*-------------------------------------------------------------------
* Use function	: ADC_callback_func[n]()
*-------------------------------------------------------------------
* Notes			: 
*-------------------------------------------------------------------
* History		: 2010.04.20
*				: Ver 0.10
*""FUNC COMMENT END""**********************************************/

#if FAST_INTC_VECTOR == VECT_AD0_ADI0
__fast_interrupt void Interrupt_ADC0(void)
#else
#pragma vector = VECT_AD0_ADI0
__interrupt void Interrupt_ADC0(void)
#endif
{
	/* Call the user function */
	if (rpdl_ADC_10_callback_func[0] != PDL_NO_FUNC)
	{
		rpdl_ADC_10_callback_func[0]();
	}
}

#if FAST_INTC_VECTOR == VECT_AD1_ADI1
__fast_interrupt void Interrupt_ADC1(void)
#else
#pragma vector = VECT_AD1_ADI1
__interrupt void Interrupt_ADC1(void)
#endif
{
	/* Call the user function */
	if (rpdl_ADC_10_callback_func[1] != PDL_NO_FUNC)
	{
		rpdl_ADC_10_callback_func[1]();
	}
}

/* End of file */
