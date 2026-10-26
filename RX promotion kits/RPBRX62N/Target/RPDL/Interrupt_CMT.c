/*""FILE COMMENT""*******************************************************
* System Name	: Interrupt program for RX62Nxx
* File Name		: Interrupt_CMT.c
* Version		: 0.10
* Contents		: Interrupt handlers for the CMT channels
* Customer		: 
* Model		 	:
* Order		 	:
* CPU			: RX600
* Compiler		: RXC
* OS			: Nothing
* Programmer	: 
* Note			: 
************************************************************************
* Copyright,2010 RENESAS ELECTRONICS CORPORATION
* AND RENESAS SOLUTIONS CORPORATION
************************************************************************
* History		: 2010.04.28
*				: Ver 0.10
*				: initial creation
*""FILE COMMENT END""**************************************************/

#include "r_pdl_cmt.h"
#include "r_pdl_cmt_RX62Nxx.h"
#include "r_pdl_user_definitions.h"

/*""FUNC COMMENT""***************************************************
* Module outline : Timer interrupt processing
*-------------------------------------------------------------------
* Declaration	: void Interrupt_CMTn(void)
*-------------------------------------------------------------------
* Function		: Interrupt processing function for CMT channel n
*-------------------------------------------------------------------
* Argument		: Nothing
*-------------------------------------------------------------------
* Return value	: Nothing
*-------------------------------------------------------------------
* Input			:
* Output		: 
*-------------------------------------------------------------------
* Use function	: CMT_callback_func[n]
*-------------------------------------------------------------------
* Notes			:
*-------------------------------------------------------------------
* History		: 2010.04.28
*				: Ver 0.10
*				: initial creation
*""FUNC COMMENT END""**********************************************/

/* From clock-arch.c */
void int_cmt0_isr(void);

#if FAST_INTC_VECTOR == VECT_CMT0_CMI0
__fast_interrupt void Interrupt_CMT0(void)
#else
#pragma vector = VECT_CMT0_CMI0
__interrupt void Interrupt_CMT0(void)
#endif
{
	// As used in the uIP demo */
	int_cmt0_isr();
}

// #pragma interrupt Interrupt_CMT1(vect=VECT_CMT1_CMI1)
// Implemented in mtarch.c

#if FAST_INTC_VECTOR == VECT_CMT2_CMI2
__fast_interrupt void Interrupt_CMT2(void)
#else
#pragma vector = VECT_CMT2_CMI2
__interrupt void Interrupt_CMT2(void)
#endif
{
	if (rpdl_CMT_one_shot[2] != false)
	{
		/* Stop the channel 0 clock input */
		CMT.CMSTR1.BIT.STR2 = 0;

		/* Clear the interrupt request flag (the timer may have set it again) */
		ICU.IR[IR_CMT2_CMI2].BIT.IR = 0;
	}

	/* Call the user function */
	if (rpdl_CMT_callback_func[2] != PDL_NO_FUNC)
	{
		rpdl_CMT_callback_func[2]();
	}
}

#if FAST_INTC_VECTOR == VECT_CMT3_CMI3
__fast_interrupt void Interrupt_CMT3(void)
#else
#pragma vector = VECT_CMT3_CMI3
__interrupt void Interrupt_CMT3(void)
#endif
{
	if (rpdl_CMT_one_shot[3] != false)
	{
		/* Stop the channel 0 clock input */
		CMT.CMSTR1.BIT.STR3 = 0;

		/* Clear the interrupt request flag (the timer may have set it again) */
		ICU.IR[IR_CMT3_CMI3].BIT.IR = 0;
	}

	/* Call the user function */
	if (rpdl_CMT_callback_func[3] != PDL_NO_FUNC)
	{
		rpdl_CMT_callback_func[3]();
	}
}
/* End of file */
