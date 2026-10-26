/*""FILE COMMENT""*************************************************************
* System Name	: Interrupt program for RX210xx
* File Name		: Interrupt_TPU.c
* Version		: 2.00
* Contents		: Interrupt handlers for the TPU channels
* Customer		:
* Model			:
* Order			:
* CPU			: RX
* Compiler		: RXC
* OS			: Nothing
* Programmer	:
* Note			: Interrupts shared between TPU and the MTU2 are
*				:  handled in a file called Interrupt_MTU2_TPU.c.
*******************************************************************************
* Copyright, 2013. Renesas Electronics Corporation and Renesas Solutions Corp.
*******************************************************************************
* History		: 2013.05.14
*				: Ver 2.00
*				: CS Release
*""FILE COMMENT END""*********************************************************/

#include "r_pdl_tpu.h"
#include "r_pdl_definitions.h"
#include "r_pdl_user_definitions.h"

/*""FUNC COMMENT""*************************************************************
* Module outline: Timer interrupt processing
*------------------------------------------------------------------------------
* Declaration	: void Interrupt_TPUn_TGInA(void)
*------------------------------------------------------------------------------
* Function		: TGInA interrupt processing
*------------------------------------------------------------------------------
* Argument		: Nothing
*------------------------------------------------------------------------------
* Return value	: Nothing
*------------------------------------------------------------------------------
* Input			:
* Output		:
*------------------------------------------------------------------------------
* Use function	: rpdl_TPU_TGRA_callback_func[n]
*------------------------------------------------------------------------------
* Notes			:
*------------------------------------------------------------------------------
* History		: 2013.05.14
*				: Ver 2.00
*				: CS Release
*""FUNC COMMENT END""*********************************************************/

#if FAST_INTC_VECTOR == VECT_TPU0_TGI0A
__fast_interrupt  void Interrupt_TPU0_TGI0A(void)
#else
#pragma vector=VECT_TPU0_TGI0A
__interrupt  void Interrupt_TPU0_TGI0A(void)
#endif
{
	/* Call the user function */
	if (rpdl_TPU_TGRA_callback_func[0] != PDL_NO_FUNC)
	{
		rpdl_TPU_TGRA_callback_func[0]();
	}
}

#if FAST_INTC_VECTOR == VECT_TPU1_TGI1A
__fast_interrupt  void Interrupt_TPU1_TGI1A(void)
#else
#pragma vector=VECT_TPU1_TGI1A
__interrupt  void Interrupt_TPU1_TGI1A(void)
#endif
{
	/* Call the user function */
	if (rpdl_TPU_TGRA_callback_func[1] != PDL_NO_FUNC)
	{
		rpdl_TPU_TGRA_callback_func[1]();
	}
}

#if FAST_INTC_VECTOR == VECT_TPU2_TGI2A
__fast_interrupt  void Interrupt_TPU2_TGI2A(void)
#else
#pragma vector=VECT_TPU2_TGI2A
__interrupt  void Interrupt_TPU2_TGI2A(void)
#endif
{
	/* Call the user function */
	if (rpdl_TPU_TGRA_callback_func[2] != PDL_NO_FUNC)
	{
		rpdl_TPU_TGRA_callback_func[2]();
	}
}

#if FAST_INTC_VECTOR == VECT_TPU3_TGI3A
__fast_interrupt  void Interrupt_TPU3_TGI3A(void)
#else
#pragma vector=VECT_TPU3_TGI3A
__interrupt  void Interrupt_TPU3_TGI3A(void)
#endif
{
	/* Call the user function */
	if (rpdl_TPU_TGRA_callback_func[3] != PDL_NO_FUNC)
	{
		rpdl_TPU_TGRA_callback_func[3]();
	}
}

#if FAST_INTC_VECTOR == VECT_TPU4_TGI4A
__fast_interrupt  void Interrupt_TPU4_TGI4A(void)
#else
#pragma vector=VECT_TPU4_TGI4A
__interrupt  void Interrupt_TPU4_TGI4A(void)
#endif
{
	/* Call the user function */
	if (rpdl_TPU_TGRA_callback_func[4] != PDL_NO_FUNC)
	{
		rpdl_TPU_TGRA_callback_func[4]();
	}
}

#if FAST_INTC_VECTOR == VECT_TPU5_TGI5A
__fast_interrupt  void Interrupt_TPU5_TGI5A(void)
#else
#pragma vector=VECT_TPU5_TGI5A
__interrupt  void Interrupt_TPU5_TGI5A(void)
#endif
{
	/* Call the user function */
	if (rpdl_TPU_TGRA_callback_func[5] != PDL_NO_FUNC)
	{
		rpdl_TPU_TGRA_callback_func[5]();
	}
}

/*""FUNC COMMENT""*************************************************************
* Module outline : Timer interrupt processing
*------------------------------------------------------------------------------
* Declaration	: void Interrupt_TPUn_TGInB(void)
*------------------------------------------------------------------------------
* Function		: TGInB interrupt processing
*------------------------------------------------------------------------------
* Argument		: Nothing
*------------------------------------------------------------------------------
* Return value	: Nothing
*------------------------------------------------------------------------------
* Input		 :
* Output		:
*------------------------------------------------------------------------------
* Use function	: rpdl_TPU_TGRB_callback_func[n]
*------------------------------------------------------------------------------
* Notes		 :
*------------------------------------------------------------------------------
* History		: 2013.05.14
*				: Ver 2.00
*				: CS Release
*""FUNC COMMENT END""*********************************************************/

#if FAST_INTC_VECTOR == VECT_TPU0_TGI0B
__fast_interrupt  void Interrupt_TPU0_TGI0B(void)
#else
#pragma vector=VECT_TPU0_TGI0B
__interrupt  void Interrupt_TPU0_TGI0B(void)
#endif
{
	/* Call the user function */
	if (rpdl_TPU_TGRB_callback_func[0] != PDL_NO_FUNC)
	{
		rpdl_TPU_TGRB_callback_func[0]();
	}
}

#if FAST_INTC_VECTOR == VECT_TPU1_TGI1B
__fast_interrupt  void Interrupt_TPU1_TGI1B(void)
#else
#pragma vector=VECT_TPU1_TGI1B
__interrupt  void Interrupt_TPU1_TGI1B(void)
#endif
{
	/* Call the user function */
	if (rpdl_TPU_TGRB_callback_func[1] != PDL_NO_FUNC)
	{
		rpdl_TPU_TGRB_callback_func[1]();
	}
}

#if FAST_INTC_VECTOR == VECT_TPU2_TGI2B
__fast_interrupt  void Interrupt_TPU2_TGI2B(void)
#else
#pragma vector=VECT_TPU2_TGI2B
__interrupt  void Interrupt_TPU2_TGI2B(void)
#endif
{
	/* Call the user function */
	if (rpdl_TPU_TGRB_callback_func[2] != PDL_NO_FUNC)
	{
		rpdl_TPU_TGRB_callback_func[2]();
	}
}

#if FAST_INTC_VECTOR == VECT_TPU3_TGI3B
__fast_interrupt  void Interrupt_TPU3_TGI3B(void)
#else
#pragma vector=VECT_TPU3_TGI3B
__interrupt  void Interrupt_TPU3_TGI3B(void)
#endif
{
	/* Call the user function */
	if (rpdl_TPU_TGRB_callback_func[3] != PDL_NO_FUNC)
	{
		rpdl_TPU_TGRB_callback_func[3]();
	}
}

#if FAST_INTC_VECTOR == VECT_TPU4_TGI4B
__fast_interrupt  void Interrupt_TPU4_TGI4B(void)
#else
#pragma vector=VECT_TPU4_TGI4B
__interrupt  void Interrupt_TPU4_TGI4B(void)
#endif
{
	/* Call the user function */
	if (rpdl_TPU_TGRB_callback_func[4] != PDL_NO_FUNC)
	{
		rpdl_TPU_TGRB_callback_func[4]();
	}
}

#if FAST_INTC_VECTOR == VECT_TPU5_TGI5B
__fast_interrupt  void Interrupt_TPU5_TGI5B(void)
#else
#pragma vector=VECT_TPU5_TGI5B
__interrupt  void Interrupt_TPU5_TGI5B(void)
#endif
{
	/* Call the user function */
	if (rpdl_TPU_TGRB_callback_func[5] != PDL_NO_FUNC)
	{
		rpdl_TPU_TGRB_callback_func[5]();
	}
}

/*""FUNC COMMENT""*************************************************************
* Module outline : Timer interrupt processing
*------------------------------------------------------------------------------
* Declaration	: void Interrupt_TPUn_TGInC(void)
*------------------------------------------------------------------------------
* Function		: TGInC interrupt processing
*------------------------------------------------------------------------------
* Argument		: Nothing
*------------------------------------------------------------------------------
* Return value	: Nothing
*------------------------------------------------------------------------------
* Input		 :
* Output		:
*------------------------------------------------------------------------------
* Use function	: rpdl_TPU_TGRC_callback_func[n]
*------------------------------------------------------------------------------
* Notes		 :
*------------------------------------------------------------------------------
* History		: 2013.05.14
*				: Ver 2.00
*				: CS Release
*""FUNC COMMENT END""*********************************************************/

#if FAST_INTC_VECTOR == VECT_TPU0_TGI0C
__fast_interrupt  void Interrupt_TPU0_TGI0C(void)
#else
#pragma vector=VECT_TPU0_TGI0C
__interrupt  void Interrupt_TPU0_TGI0C(void)
#endif
{
	/* Call the user function */
	if (rpdl_TPU_TGRC_callback_func[0] != PDL_NO_FUNC)
	{
		rpdl_TPU_TGRC_callback_func[0]();
	}
}

#if FAST_INTC_VECTOR == VECT_TPU3_TGI3C
__fast_interrupt  void Interrupt_TPU3_TGI3C(void)
#else
#pragma vector=VECT_TPU3_TGI3C
__interrupt  void Interrupt_TPU3_TGI3C(void)
#endif
{
	/* Call the user function */
	if (rpdl_TPU_TGRC_callback_func[3] != PDL_NO_FUNC)
	{
		rpdl_TPU_TGRC_callback_func[3]();
	}
}


/*""FUNC COMMENT""*************************************************************
* Module outline : Timer interrupt processing
*------------------------------------------------------------------------------
* Declaration	: void Interrupt_TPUn_TGInD(void)
*------------------------------------------------------------------------------
* Function		: TGInD interrupt processing
*------------------------------------------------------------------------------
* Argument		: Nothing
*------------------------------------------------------------------------------
* Return value	: Nothing
*------------------------------------------------------------------------------
* Input		 :
* Output		:
*------------------------------------------------------------------------------
* Use function	: rpdl_TPU_TGRD_callback_func[n]
*------------------------------------------------------------------------------
* Notes		 :
*------------------------------------------------------------------------------
* History		: 2013.05.14
*				: Ver 2.00
*				: CS Release
*""FUNC COMMENT END""*********************************************************/

#if FAST_INTC_VECTOR == VECT_TPU0_TGI0D
__fast_interrupt  void Interrupt_TPU0_TGI0D(void)
#else
#pragma vector=VECT_TPU0_TGI0D
__interrupt  void Interrupt_TPU0_TGI0D(void)
#endif
{
	/* Call the user function */
	if (rpdl_TPU_TGRD_callback_func[0] != PDL_NO_FUNC)
	{
		rpdl_TPU_TGRD_callback_func[0]();
	}
}

#if FAST_INTC_VECTOR == VECT_TPU3_TGI3D
__fast_interrupt  void Interrupt_TPU3_TGI3D(void)
#else
#pragma vector=VECT_TPU3_TGI3D
__interrupt  void Interrupt_TPU3_TGI3D(void)
#endif
{
	/* Call the user function */
	if (rpdl_TPU_TGRD_callback_func[3] != PDL_NO_FUNC)
	{
		rpdl_TPU_TGRD_callback_func[3]();
	}
}

/*""FUNC COMMENT""*************************************************************
* Module outline : Timer interrupt processing
*------------------------------------------------------------------------------
* Declaration	: void Interrupt_TPUn_TGInV(void)
*------------------------------------------------------------------------------
* Function		: TGInV interrupt processing
*------------------------------------------------------------------------------
* Argument		: Nothing
*------------------------------------------------------------------------------
* Return value	: Nothing
*------------------------------------------------------------------------------
* Input		 :
* Output		:
*------------------------------------------------------------------------------
* Use function	: rpdl_TPU_Overflow_callback_func[n]
*------------------------------------------------------------------------------
* Notes		 :
*------------------------------------------------------------------------------
* History		: 2013.05.14
*				: Ver 2.00
*				: CS Release
*""FUNC COMMENT END""*********************************************************/

#if FAST_INTC_VECTOR == VECT_TPU0_TCI0V
__fast_interrupt  void Interrupt_TPU0_TCI0V(void)
#else
#pragma vector=VECT_TPU0_TCI0V
__interrupt  void Interrupt_TPU0_TCI0V(void)
#endif
{
	/* Call the user function */
	if (rpdl_TPU_Overflow_callback_func[0] != PDL_NO_FUNC)
	{
		rpdl_TPU_Overflow_callback_func[0]();
	}
}

#if FAST_INTC_VECTOR == VECT_TPU1_TCI1V
__fast_interrupt  void Interrupt_TPU1_TCI1V(void)
#else
#pragma vector=VECT_TPU1_TCI1V
__interrupt  void Interrupt_TPU1_TCI1V(void)
#endif
{
	/* Call the user function */
	if (rpdl_TPU_Overflow_callback_func[1] != PDL_NO_FUNC)
	{
		rpdl_TPU_Overflow_callback_func[1]();
	}
}

#if FAST_INTC_VECTOR == VECT_TPU2_TCI2V
__fast_interrupt  void Interrupt_TPU2_TCI2V(void)
#else
#pragma vector=VECT_TPU2_TCI2V
__interrupt  void Interrupt_TPU2_TCI2V(void)
#endif
{
	/* Call the user function */
	if (rpdl_TPU_Overflow_callback_func[2] != PDL_NO_FUNC)
	{
		rpdl_TPU_Overflow_callback_func[2]();
	}
}

#if FAST_INTC_VECTOR == VECT_TPU3_TCI3V
__fast_interrupt  void Interrupt_TPU3_TCI3V(void)
#else
#pragma vector=VECT_TPU3_TCI3V
__interrupt  void Interrupt_TPU3_TCI3V(void)
#endif
{
	/* Call the user function */
	if (rpdl_TPU_Overflow_callback_func[3] != PDL_NO_FUNC)
	{
		rpdl_TPU_Overflow_callback_func[3]();
	}
}

#if FAST_INTC_VECTOR == VECT_TPU4_TCI4V
__fast_interrupt  void Interrupt_TPU4_TCI4V(void)
#else
#pragma vector=VECT_TPU4_TCI4V
__interrupt  void Interrupt_TPU4_TCI4V(void)
#endif
{
	/* Call the user function */
	if (rpdl_TPU_Overflow_callback_func[4] != PDL_NO_FUNC)
	{
		rpdl_TPU_Overflow_callback_func[4]();
	}
}

#if FAST_INTC_VECTOR == VECT_TPU5_TCI5V
__fast_interrupt  void Interrupt_TPU5_TCI5V(void)
#else
#pragma vector=VECT_TPU5_TCI5V
__interrupt  void Interrupt_TPU5_TCI5V(void)
#endif
{
	/* Call the user function */
	if (rpdl_TPU_Overflow_callback_func[5] != PDL_NO_FUNC)
	{
		rpdl_TPU_Overflow_callback_func[5]();
	}
}

/*""FUNC COMMENT""*************************************************************
* Module outline : Timer interrupt processing
*------------------------------------------------------------------------------
* Declaration	: void Interrupt_TPUn_TGInU(void)
*------------------------------------------------------------------------------
* Function		: TGInU interrupt processing
*------------------------------------------------------------------------------
* Argument		: Nothing
*------------------------------------------------------------------------------
* Return value	: Nothing
*------------------------------------------------------------------------------
* Input		 :
* Output		:
*------------------------------------------------------------------------------
* Use function	: rpdl_TPU_Underflow_callback_func[n]
*------------------------------------------------------------------------------
* Notes		 :
*------------------------------------------------------------------------------
* History		: 2013.05.14
*				: Ver 2.00
*				: CS Release
*""FUNC COMMENT END""*********************************************************/

#if FAST_INTC_VECTOR == VECT_TPU1_TCI1U
__fast_interrupt  void Interrupt_TPU1_TCI1U(void)
#else
#pragma vector=VECT_TPU1_TCI1U
__interrupt  void Interrupt_TPU1_TCI1U(void)
#endif
{
	/* Call the user function */
	if (rpdl_TPU_Underflow_callback_func[1] != PDL_NO_FUNC)
	{
		rpdl_TPU_Underflow_callback_func[1]();
	}
}

#if FAST_INTC_VECTOR == VECT_TPU2_TCI2U
__fast_interrupt  void Interrupt_TPU2_TCI2U(void)
#else
#pragma vector=VECT_TPU2_TCI2U
__interrupt  void Interrupt_TPU2_TCI2U(void)
#endif
{
	/* Call the user function */
	if (rpdl_TPU_Underflow_callback_func[2] != PDL_NO_FUNC)
	{
		rpdl_TPU_Underflow_callback_func[2]();
	}
}


#if FAST_INTC_VECTOR == VECT_TPU4_TCI4U
__fast_interrupt  void Interrupt_TPU4_TCI4U(void)
#else
#pragma vector=VECT_TPU4_TCI4U
__interrupt  void Interrupt_TPU4_TCI4U(void)
#endif
{
	/* Call the user function */
	if (rpdl_TPU_Underflow_callback_func[4] != PDL_NO_FUNC)
	{
		rpdl_TPU_Underflow_callback_func[4]();
	}
}

#if FAST_INTC_VECTOR == VECT_TPU5_TCI5U
__fast_interrupt  void Interrupt_TPU5_TCI5U(void)
#else
#pragma vector=VECT_TPU5_TCI5U
__interrupt  void Interrupt_TPU5_TCI5U(void)
#endif
{
	/* Call the user function */
	if (rpdl_TPU_Underflow_callback_func[5] != PDL_NO_FUNC)
	{
		rpdl_TPU_Underflow_callback_func[5]();
	}
}

/* End of file */
