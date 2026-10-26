/*""FILE COMMENT""*******************************************************
* System Name	: Interrupt program for RX610xx
* File Name		: Interrupt_BSC.c
* Version		: 0.50
* Contents		: Interrupt handler for the BSC
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
* History		: 2009.09.23
*				: Ver 0.10
*				: 2010.03.12
*				: Ver 0.11
*				: Changed the header file inclusion
*				: 2010.04.06
*				: Ver 0.50
*				: ES-level release
*""FILE COMMENT END""**************************************************/

#include "r_pdl_bsc.h"
#include "r_pdl_bsc_RX62Nxx.h"
#include "r_pdl_user_definitions.h"

/*""FUNC COMMENT""***************************************************
* Module outline : BSC interrupt processing
*-------------------------------------------------------------------
* Declaration	: void Interrupt_BSC(void)
*-------------------------------------------------------------------
* Function		: Interrupt processing function for the BSC
*-------------------------------------------------------------------
* Argument		: Nothing
*-------------------------------------------------------------------
* Return value	: Nothing
*-------------------------------------------------------------------
* Input			:
* Output		: 
*-------------------------------------------------------------------
* Use function	: rpdl_BSC_callback_func
*-------------------------------------------------------------------
* Notes		 	:
*-------------------------------------------------------------------
* History		: 2009.09.25
*				: Ver 0.10
*				: 2010.02.02
*				: Ver 0.11
*				: Added callback function checking
*				: 2010.02.08
*				: Ver 0.12
*				: Removed DMAC trigger control
*				: 2010.02.22
*				: Ver 0.13
*				: Removed fast interrupt code
*				: 2010.03.22
*				: Ver 0.14
*				: Updated for the v2.2 iodefine.h file.
* 				: 2010.04.23
*				: Ver 0.15
*				: Updated for the v3.0 iodefine.h file.
*""FUNC COMMENT END""**********************************************/

#if FAST_INTC_VECTOR == VECT_BSC_BUSERR
__fast_interrupt void Interrupt_BSC(void)
#else
#pragma vector = VECT_BSC_BUSERR
__interrupt void Interrupt_BSC(void)
#endif
{
	/* Call the user function */
	//if (rpdl_BSC_callback_func != PDL_NO_FUNC)
	{
	//	rpdl_BSC_callback_func();
	}
}
/* End of file */
