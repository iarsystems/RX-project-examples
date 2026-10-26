/*""FILE COMMENT""*******************************************************
* System Name	: Interrupt program for RX62N
* File Name		: Interrupt_DMAC.c
* Version		: 0.10
* Contents		: Interrupt handlers for the DMAC channels
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
* History		: 2010.04.22
*				: Ver 0.10
*""FILE COMMENT END""**************************************************/

#include "r_pdl_dmac.h"
#include "r_pdl_dmac_RX62Nxx.h"
#include "r_pdl_user_definitions.h"

/*""FUNC COMMENT""***************************************************
* Module outline : DMAC interrupt processing
*-------------------------------------------------------------------
* Declaration	: void Interrupt_DMACn(void)
*-------------------------------------------------------------------
* Function		: Interrupt processing function for DMAC channel n
*-------------------------------------------------------------------
* Argument		: Nothing
*-------------------------------------------------------------------
* Return value	: Nothing
*-------------------------------------------------------------------
* Input			: 
* Output		: 
*-------------------------------------------------------------------
* Use function	: DMAC_callback_func[n]
*-------------------------------------------------------------------
* Notes			: 
*-------------------------------------------------------------------
* History		: 2010.04.27
*				: Ver 0.10
*""FUNC COMMENT END""**********************************************/

#if FAST_INTC_VECTOR == VECT_DMACA_DMAC0I
__fast_interrupt void Interrupt_DMAC0(void)
#else
#pragma vector = VECT_DMACA_DMAC0I
__interrupt void Interrupt_DMAC0(void)
#endif
{
	/* Call the user function */
	if (rpdl_DMAC_callback_func[0] != PDL_NO_FUNC)
	{
		rpdl_DMAC_callback_func[0]();
	}
}

#if FAST_INTC_VECTOR == VECT_DMACA_DMAC1I
__fast_interrupt void Interrupt_DMAC1(void)
#else
#pragma vector = VECT_DMACA_DMAC1I
__interrupt void Interrupt_DMAC1(void)
#endif
{
	/* Call the user function */
	if (rpdl_DMAC_callback_func[1] != PDL_NO_FUNC)
	{
		rpdl_DMAC_callback_func[1]();
	}
}

#if FAST_INTC_VECTOR == VECT_DMACA_DMAC2I
__fast_interrupt void Interrupt_DMAC2(void)
#else
#pragma vector = VECT_DMACA_DMAC2I
__interrupt void Interrupt_DMAC2(void)
#endif
{
	/* Call the user function */
	if (rpdl_DMAC_callback_func[2] != PDL_NO_FUNC)
	{
		rpdl_DMAC_callback_func[2]();
	}
}

#if FAST_INTC_VECTOR == VECT_DMACA_DMAC3I
__fast_interrupt void Interrupt_DMAC3(void)
#else
#pragma vector = VECT_DMACA_DMAC3I
__interrupt void Interrupt_DMAC3(void)
#endif
{
	/* Call the user function */
	if (rpdl_DMAC_callback_func[3] != PDL_NO_FUNC)
	{
		rpdl_DMAC_callback_func[3]();
	}
}
/* End of file */
