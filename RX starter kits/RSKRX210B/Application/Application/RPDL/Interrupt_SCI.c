/*""FILE COMMENT""*************************************************************
* System Name	: Interrupt program for RX210
* File Name		: Interrupt_SCI.c
* Version		: 2.00
* Contents		: Interrupt handlers for all SCI channels
* Customer		:
* Model			:
* Order			:
* CPU			: RX
* Compiler		: RXC
* OS			: Nothing
* Programmer	:
* Note			:
*******************************************************************************
* Copyright, 2013. Renesas Electronics Corporation and Renesas Solutions Corp.
*******************************************************************************
* History		: 2013.05.14
*				: Ver 2.00
*				: CS Release
*""FILE COMMENT END""*********************************************************/

#include "r_pdl_sci.h"
#include "r_pdl_definitions.h"
#include "r_pdl_user_definitions.h"

/* This file uses some SCI utility functions */
extern bool rpdl_SCI_SetTxPinGeneralIO(const uint8_t channel,
									   const bool bGeneralIO);
extern void rpdl_SCI_ModifyTx_IER(const uint8_t, const uint8_t);
extern void rpdl_SCI_ModifyTxEnd_IER(const uint8_t, const uint8_t);
extern void R_SCI_IIC_Handler_RXI(const uint8_t channel);
extern void R_SCI_IIC_Handler_TXI(const uint8_t channel);

/* Common interrupt processing for channels 0 to 11.
Channel 12 is handled separately */
static void Interrupt_SCIx_ERIx(const uint8_t channel,
						volatile struct st_sci0 __sfr * const pSCIx);
static void Interrupt_SCIx_RXIx(const uint8_t channel,
						volatile struct st_sci0 __sfr * const pSCIx);
static void Interrupt_SCIx_TXIx(const uint8_t channel,
						volatile struct st_sci0 __sfr * const pSCIx);
static void Interrupt_SCIx_TEIx(const uint8_t channel,
						volatile struct st_sci0 __sfr * const pSCIx);


/*""FUNC COMMENT""*************************************************************
* Module outline: SCIn receive data error interrupt processing
*------------------------------------------------------------------------------
* Declaration	: void Interrupt_SCIn_ERIn(void)
*------------------------------------------------------------------------------
* Function		:
*------------------------------------------------------------------------------
* Argument		: Nothing
*------------------------------------------------------------------------------
* Return value	: Nothing
*------------------------------------------------------------------------------
* Output		: SSR for that SCI channel
*------------------------------------------------------------------------------
* Use function	: rpdl_SCI_RX_Error_callback_func
*------------------------------------------------------------------------------
* Notes			:
*------------------------------------------------------------------------------
* History		: 2013.05.14
*				: Ver 2.00
*				: CS Release
*""FUNC COMMENT END""*********************************************************/

#if FAST_INTC_VECTOR == VECT_SCI0_ERI0
__fast_interrupt  void Interrupt_SCI0_ERI0(void)
#else
#pragma vector=VECT_SCI0_ERI0
__interrupt  void Interrupt_SCI0_ERI0(void)
#endif
{
	Interrupt_SCIx_ERIx(0, &SCI0);
}

#if FAST_INTC_VECTOR == VECT_SCI1_ERI1
__fast_interrupt  void Interrupt_SCI1_ERI1(void)
#else
#pragma vector=VECT_SCI1_ERI1
__interrupt  void Interrupt_SCI1_ERI1(void)
#endif
{
	Interrupt_SCIx_ERIx(1, &SCI1);
}

#if FAST_INTC_VECTOR == VECT_SCI2_ERI2
__fast_interrupt  void Interrupt_SCI2_ERI2(void)
#else
#pragma vector=VECT_SCI2_ERI2
__interrupt  void Interrupt_SCI2_ERI2(void)
#endif
{
	Interrupt_SCIx_ERIx(2, &SCI2);
}

#if FAST_INTC_VECTOR == VECT_SCI3_ERI3
__fast_interrupt  void Interrupt_SCI3_ERI3(void)
#else
#pragma vector=VECT_SCI3_ERI3
__interrupt  void Interrupt_SCI3_ERI3(void)
#endif
{
	Interrupt_SCIx_ERIx(3, &SCI3);
}

#if FAST_INTC_VECTOR == VECT_SCI4_ERI4
__fast_interrupt  void Interrupt_SCI4_ERI4(void)
#else
#pragma vector=VECT_SCI4_ERI4
__interrupt  void Interrupt_SCI4_ERI4(void)
#endif
{
	Interrupt_SCIx_ERIx(4, &SCI4);
}

#if FAST_INTC_VECTOR == VECT_SCI5_ERI5
__fast_interrupt  void Interrupt_SCI5_ERI5(void)
#else
#pragma vector=VECT_SCI5_ERI5
__interrupt  void Interrupt_SCI5_ERI5(void)
#endif
{
	Interrupt_SCIx_ERIx(5, &SCI5);
}

#if FAST_INTC_VECTOR == VECT_SCI6_ERI6
__fast_interrupt  void Interrupt_SCI6_ERI6(void)
#else
#pragma vector=VECT_SCI6_ERI6
__interrupt  void Interrupt_SCI6_ERI6(void)
#endif
{
	Interrupt_SCIx_ERIx(6, &SCI6);
}

#if FAST_INTC_VECTOR == VECT_SCI7_ERI7
__fast_interrupt  void Interrupt_SCI7_ERI7(void)
#else
#pragma vector=VECT_SCI7_ERI7
__interrupt  void Interrupt_SCI7_ERI7(void)
#endif
{
	Interrupt_SCIx_ERIx(7, &SCI7);
}

#if FAST_INTC_VECTOR == VECT_SCI8_ERI8
__fast_interrupt  void Interrupt_SCI8_ERI8(void)
#else
#pragma vector=VECT_SCI8_ERI8
__interrupt  void Interrupt_SCI8_ERI8(void)
#endif
{
	Interrupt_SCIx_ERIx(8, &SCI8);
}

#if FAST_INTC_VECTOR == VECT_SCI9_ERI9
__fast_interrupt  void Interrupt_SCI9_ERI9(void)
#else
#pragma vector=VECT_SCI9_ERI9
__interrupt  void Interrupt_SCI9_ERI9(void)
#endif
{
	Interrupt_SCIx_ERIx(9, &SCI9);
}

#if FAST_INTC_VECTOR == VECT_SCI10_ERI10
__fast_interrupt  void Interrupt_SCI10_ERI10(void)
#else
#pragma vector=VECT_SCI10_ERI10
__interrupt  void Interrupt_SCI10_ERI10(void)
#endif
{
	Interrupt_SCIx_ERIx(10, &SCI10);
}

#if FAST_INTC_VECTOR == VECT_SCI11_ERI11
__fast_interrupt  void Interrupt_SCI11_ERI11(void)
#else
#pragma vector=VECT_SCI11_ERI11
__interrupt  void Interrupt_SCI11_ERI11(void)
#endif
{
	Interrupt_SCIx_ERIx(11, &SCI11);
}

/* This is a common function used by channels 0 to 11 to process the
receive data error interrupt.
NOTE: Channel 12 is handled separately. */
static void Interrupt_SCIx_ERIx(const uint8_t channel,
						volatile struct st_sci0 __sfr * const pSCIx)
{
	/* Will the user handle the errors? */
	if (rpdl_SCI_GlobalData(channel)->RX_Error_callback_func != PDL_NO_FUNC)
	{
		/* Call the error handler */
		rpdl_SCI_GlobalData(channel)->RX_Error_callback_func();
	}
	else
	{
		/* Clear the SSR error flags;
		Preserve MPBT(b0) and write 1 to reserved bits (b6 and b7)*/
		pSCIx->SSR.BYTE = (uint8_t)((BIT_7 | BIT_6) |
						 (pSCIx->SSR.BYTE & (uint8_t)BIT_0));
	}
}

#if FAST_INTC_VECTOR == VECT_SCI12_ERI12
__fast_interrupt  void Interrupt_SCI12_ERI12(void)
#else
#pragma vector=VECT_SCI12_ERI12
__interrupt  void Interrupt_SCI12_ERI12(void)
#endif
{
	/* Will the user handle the errors? */
	if (rpdl_SCI_GlobalData(12)->RX_Error_callback_func != PDL_NO_FUNC)
	{
		/* Call the error handler */
		rpdl_SCI_GlobalData(12)->RX_Error_callback_func();
	}
	else
	{
		/* Clear the SSR error flags;
		Preserve MPBT(b0) and write 1 to reserved bits (b6 and b7)*/
		SCI12.SSR.BYTE = (uint8_t)((BIT_7 | BIT_6) |
						 (SCI12.SSR.BYTE & (uint8_t)BIT_0));
	}
}

/*""FUNC COMMENT""*************************************************************
* Module outline: SCIn receive data interrupt processing
*------------------------------------------------------------------------------
* Declaration	: void Interrupt_SCIn_RXIn(void)
*------------------------------------------------------------------------------
* Function		:
*------------------------------------------------------------------------------
* Argument		: Nothing
*------------------------------------------------------------------------------
* Return value	: Nothing
*------------------------------------------------------------------------------
* Output		:
*------------------------------------------------------------------------------
* Use function	: rpdl_SCI_RX_End_callback_func
*------------------------------------------------------------------------------
* Notes			:
*------------------------------------------------------------------------------
* History		: 2013.05.14
*				: Ver 2.00
*				: CS Release
*""FUNC COMMENT END""*********************************************************/
#if FAST_INTC_VECTOR == VECT_SCI0_RXI0
__fast_interrupt  void Interrupt_SCI0_RXI0(void)
#else
#pragma vector=VECT_SCI0_RXI0
__interrupt  void Interrupt_SCI0_RXI0(void)
#endif
{
	Interrupt_SCIx_RXIx(0, &SCI0);
}

#if FAST_INTC_VECTOR == VECT_SCI1_RXI1
__fast_interrupt  void Interrupt_SCI1_RXI1(void)
#else
#pragma vector=VECT_SCI1_RXI1
__interrupt  void Interrupt_SCI1_RXI1(void)
#endif
{
	Interrupt_SCIx_RXIx(1, &SCI1);
}

#if FAST_INTC_VECTOR == VECT_SCI2_RXI2
__fast_interrupt  void Interrupt_SCI2_RXI2(void)
#else
#pragma vector=VECT_SCI2_RXI2
__interrupt  void Interrupt_SCI2_RXI2(void)
#endif
{
	Interrupt_SCIx_RXIx(2, &SCI2);
}

#if FAST_INTC_VECTOR == VECT_SCI3_RXI3
__fast_interrupt  void Interrupt_SCI3_RXI3(void)
#else
#pragma vector=VECT_SCI3_RXI3
__interrupt  void Interrupt_SCI3_RXI3(void)
#endif
{
	Interrupt_SCIx_RXIx(3, &SCI3);
}

#if FAST_INTC_VECTOR == VECT_SCI4_RXI4
__fast_interrupt  void Interrupt_SCI4_RXI4(void)
#else
#pragma vector=VECT_SCI4_RXI4
__interrupt  void Interrupt_SCI4_RXI4(void)
#endif
{
	Interrupt_SCIx_RXIx(4, &SCI4);
}

#if FAST_INTC_VECTOR == VECT_SCI5_RXI5
__fast_interrupt  void Interrupt_SCI5_RXI5(void)
#else
#pragma vector=VECT_SCI5_RXI5
__interrupt  void Interrupt_SCI5_RXI5(void)
#endif
{
	Interrupt_SCIx_RXIx(5, &SCI5);
}

#if FAST_INTC_VECTOR == VECT_SCI6_RXI6
__fast_interrupt  void Interrupt_SCI6_RXI6(void)
#else
#pragma vector=VECT_SCI6_RXI6
__interrupt  void Interrupt_SCI6_RXI6(void)
#endif
{
	Interrupt_SCIx_RXIx(6, &SCI6);
}

#if FAST_INTC_VECTOR == VECT_SCI7_RXI7
__fast_interrupt  void Interrupt_SCI7_RXI7(void)
#else
#pragma vector=VECT_SCI7_RXI7
__interrupt  void Interrupt_SCI7_RXI7(void)
#endif
{
	Interrupt_SCIx_RXIx(7, &SCI7);
}

#if FAST_INTC_VECTOR == VECT_SCI8_RXI8
__fast_interrupt  void Interrupt_SCI8_RXI8(void)
#else
#pragma vector=VECT_SCI8_RXI8
__interrupt  void Interrupt_SCI8_RXI8(void)
#endif
{
	Interrupt_SCIx_RXIx(8, &SCI8);
}

#if FAST_INTC_VECTOR == VECT_SCI9_RXI9
__fast_interrupt  void Interrupt_SCI9_RXI9(void)
#else
#pragma vector=VECT_SCI9_RXI9
__interrupt  void Interrupt_SCI9_RXI9(void)
#endif
{
	Interrupt_SCIx_RXIx(9, &SCI9);
}

#if FAST_INTC_VECTOR == VECT_SCI10_RXI10
__fast_interrupt  void Interrupt_SCI10_RXI10(void)
#else
#pragma vector=VECT_SCI10_RXI10
__interrupt  void Interrupt_SCI10_RXI10(void)
#endif
{
	Interrupt_SCIx_RXIx(10, &SCI10);
}

#if FAST_INTC_VECTOR == VECT_SCI11_RXI11
__fast_interrupt  void Interrupt_SCI11_RXI11(void)
#else
#pragma vector=VECT_SCI11_RXI11
__interrupt  void Interrupt_SCI11_RXI11(void)
#endif
{
	Interrupt_SCIx_RXIx(11, &SCI11);
}

/* This is a common function used by channels 0 to 11 to process the
receive data interrupt.
NOTE: Channel 12 is handled separately. */
static void Interrupt_SCIx_RXIx(const uint8_t channel,
						volatile struct st_sci0 __sfr * const pSCIx)
{
	/*If SCI is in IIC mode (SIMR1.IICM == 1)
	then handle this interrupt separately*/
	if(pSCIx->SIMR1.BIT.IICM == 1)
	{
		R_SCI_IIC_Handler_RXI(channel);
		return;
	}

	/* Ok to process the data? */
	if (rpdl_SCI_GlobalData(channel)->rx_using_irq == true)
	{
		/* check if ID reception in Multiprocessor mode */
		if (rpdl_SCI_GlobalData(channel)->MP.MP_mode == RPDL_MP_MODE_EXPECTING_ID)
		{
			/* check if ID cycle ? */
			if (pSCIx->SSR.BIT.MPB == 1)
			{
				uint8_t id;

				/* Read the ID */
				id = pSCIx->RDR;

				/* ID matching ? */
				if (id != rpdl_SCI_GlobalData(channel)->MP.MP_rx_stationID)
				{
					/* ID does not match */
					/* MPIE = 1 */
					pSCIx->SCR.BIT.MPIE = 1;
				}
				else
				{
					/* ID matches */
					/* Disable interrupt request generation, and try to disable reception */
					pSCIx->SCR.BYTE &= (uint8_t)(INV_BIT_6 & INV_BIT_4);

					/*Set flag back from RPDL_MP_MODE_EXPECTING_ID to RPDL_MP_MODE_ENABLED*/
					rpdl_SCI_GlobalData(channel)->MP.MP_mode = RPDL_MP_MODE_ENABLED;

					/* Notify the user */
					if (rpdl_SCI_GlobalData(channel)->RX_End_callback_func != PDL_NO_FUNC)
					{
						rpdl_SCI_GlobalData(channel)->RX_End_callback_func();
					}
				}

				/* Exit ISR */
				return;
			}
		}

		/* Read and store the character */
		*rpdl_SCI_GlobalData(channel)->rx_string_pointer = pSCIx->RDR;

		/* Increment the character counter */
		rpdl_SCI_GlobalData(channel)->rx_counter++;

		/* More characters expected? */
		if (rpdl_SCI_GlobalData(channel)->rx_counter < rpdl_SCI_GlobalData(channel)->rx_threshold)
		{
			/* Move to the next location in the buffer */
			rpdl_SCI_GlobalData(channel)->rx_string_pointer++;
		}
		else
		{
			/*Rx has finished.*/

			/* If continuous receive mode is selected, data will be stored to the top of receive buffer
			after callback function is triggered. */
			if (rpdl_SCI_GlobalData(channel)->rx_continuous == true)
			{
				rpdl_SCI_GlobalData(channel)->rx_string_pointer = rpdl_SCI_GlobalData(channel)->rx_string_start;
				rpdl_SCI_GlobalData(channel)->rx_counter = 0;
			}

			/*----------------------------------------------------------------------
			The HW Manual explains a delay is required if in Clock Synchronous mode
			(SMR.CM = 1) (including SPI mode) and using the delayed clock phase
			select (SPMR.CKPH = 1). This delay prevents the final clock pulse from
			being truncated when SCR.RE is cleared.
			USER: If this delay in an ISR is unacceptable in your system
			then you will need to provide an alternative solution that executes
			the remainder of this ISR code after an appropriate delay.
			NOTE: It is not recommended to use nested interrupts as a solution.
			----------------------------------------------------------------------*/
			if((pSCIx->SPMR.BIT.CKPH == 1) && (pSCIx->SMR.BIT.CM == 1))
			{
				/* Generate a half-bit period delay.*/
				rpdl_wait_time(0.5 / (double)rpdl_SCI_GlobalData(channel)->baud_rate);
			}

			/*If in sync mode (SMR.CM==1) then can't clear SCR.TE and SCR.RE independantly.
			Hence if TE bit is enabled but Tx is not pending (SCR.TIE==0 && SCR.TEIE==0)
			then also clear TE*/
			if((pSCIx->SMR.BIT.CM == 1) &&
			   ((pSCIx->SCR.BYTE & (BIT_5 | BIT_2 | BIT_7)) == BIT_5))
			{
				/*Tx has finished, so disable TE,TIE,TEIE as well as RE and RIE*/
				pSCIx->SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5 & INV_BIT_2 &
				 							INV_BIT_6 & INV_BIT_4);
			}
			else
			{
				/* In continous receive mode, do not disable receive interrupt */
				if (rpdl_SCI_GlobalData(channel)->rx_continuous == false)
				{
					/* Disable interrupt request generation, and try to disable reception */
					pSCIx->SCR.BYTE &= (uint8_t)(INV_BIT_6 & INV_BIT_4);
				}
			}

			/* Async MP mode ? */
			if (((pSCIx->SMR.BIT.MP) != 0) &&
				(rpdl_SCI_GlobalData(channel)->MP.MP_mode != RPDL_MP_MODE_DISABLED))
			{
				/* Set MPIE = 1 (multiprocessor mode reception) */
				pSCIx->SCR.BIT.MPIE = 1;
			}

			/* Notify the user */
			if (rpdl_SCI_GlobalData(channel)->RX_End_callback_func != PDL_NO_FUNC)
			{
				rpdl_SCI_GlobalData(channel)->RX_End_callback_func();
			}
		}
	}
	/* Either the DMAC or DTC has passed on the interrupt */
	else
	{
		/* Call the callback function */
		if (rpdl_SCI_GlobalData(channel)->RX_End_callback_func != PDL_NO_FUNC)
		{
			rpdl_SCI_GlobalData(channel)->RX_End_callback_func();
		}
	}
}

#if FAST_INTC_VECTOR == VECT_SCI12_RXI12
__fast_interrupt  void Interrupt_SCI12_RXI12(void)
#else
#pragma vector=VECT_SCI12_RXI12
__interrupt  void Interrupt_SCI12_RXI12(void)
#endif
{
	/*If SCI is in IIC mode (SIMR1.IICM == 1)
	then handle this interrupt separately*/
	if(SCI12.SIMR1.BIT.IICM == 1)
	{
		R_SCI_IIC_Handler_RXI(12);
		return;
	}

	/* Ok to process the data? */
	if (rpdl_SCI_GlobalData(12)->rx_using_irq == true)
	{
		/* check if ID reception in Multiprocessor mode */
		if (rpdl_SCI_GlobalData(12)->MP.MP_mode == RPDL_MP_MODE_EXPECTING_ID)
		{
			/* check if ID cycle ? */
			if (SCI12.SSR.BIT.MPB == 1)
			{
				uint8_t id;

				/* Read the ID */
				id = SCI12.RDR;

				/* ID matching ? */
				if (id != rpdl_SCI_GlobalData(12)->MP.MP_rx_stationID)
				{
					/* ID does not match */
					/* MPIE = 1 */
					SCI12.SCR.BIT.MPIE = 1;
				}
				else
				{
					/* ID matches */
					/* Disable interrupt request generation, and try to disable reception */
					SCI12.SCR.BYTE &= (uint8_t)(INV_BIT_6 & INV_BIT_4);

					/*Set flag back from RPDL_MP_MODE_EXPECTING_ID to RPDL_MP_MODE_ENABLED*/
					rpdl_SCI_GlobalData(12)->MP.MP_mode = RPDL_MP_MODE_ENABLED;

					/* Notify the user */
					if (rpdl_SCI_GlobalData(12)->RX_End_callback_func != PDL_NO_FUNC)
					{
						rpdl_SCI_GlobalData(12)->RX_End_callback_func();
					}
				}

				/* Exit ISR */
				return;
			}
		}

		/* Read and store the character */
		*rpdl_SCI_GlobalData(12)->rx_string_pointer = SCI12.RDR;

		/* Increment the character counter */
		rpdl_SCI_GlobalData(12)->rx_counter++;

		/* More characters expected? */
		if (rpdl_SCI_GlobalData(12)->rx_counter < rpdl_SCI_GlobalData(12)->rx_threshold)
		{
			/* Move to the next location in the buffer */
			rpdl_SCI_GlobalData(12)->rx_string_pointer++;
		}
		else
		{
			/*Rx has finished.*/
			/* If continuous receive mode is selected, data will be stored to the top of receive buffer
			after callback function is triggered. */
			if (rpdl_SCI_GlobalData(12)->rx_continuous == true)
			{
				rpdl_SCI_GlobalData(12)->rx_string_pointer = rpdl_SCI_GlobalData(12)->rx_string_start;
				rpdl_SCI_GlobalData(12)->rx_counter = 0;
			}

			/*----------------------------------------------------------------------
			The HW Manual explains a delay is required if in Clock Synchronous mode
			(SMR.CM = 1) (including SPI mode) and using the delayed clock phase
			select (SPMR.CKPH = 1). This delay prevents the final clock pulse from
			being truncated when SCR.RE is cleared.
			USER: If this delay in an ISR is unacceptable in your system
			then you will need to provide an alternative solution that executes
			the remainder of this ISR code after an appropriate delay.
			NOTE: It is not recommended to use nested interrupts as a solution.
			----------------------------------------------------------------------*/
			if((SCI12.SPMR.BIT.CKPH == 1) && (SCI12.SMR.BIT.CM == 1))
			{
				/* Generate a half-bit period delay.*/
				rpdl_wait_time(0.5 / (double)rpdl_SCI_GlobalData(12)->baud_rate);
			}

			/*If in sync mode (SMR.CM==1) then can't clear SCR.TE and SCR.RE independantly.
			Hence if TE bit is enabled but Tx is not pending (SCR.TIE==0 && SCR.TEIE==0)
			then also clear TE*/
			if((SCI12.SMR.BIT.CM == 1) &&
			   ((SCI12.SCR.BYTE & (BIT_5 | BIT_2 | BIT_7)) == BIT_5))

			{
				/*Tx has finished, so disable TE,TIE,TEIE as well as RE and RIE*/
				SCI12.SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5 & INV_BIT_2 &
				 							INV_BIT_6 & INV_BIT_4);
			}
			else
			{
				/* In continous receive mode, do not disable receive interrupt */
				if (rpdl_SCI_GlobalData(12)->rx_continuous == false)
				{
					/* Disable interrupt request generation, and try to disable reception */
					SCI12.SCR.BYTE &= (uint8_t)(INV_BIT_6 & INV_BIT_4);
				}
			}

			/* Async MP mode ? */
			if (((SCI12.SMR.BIT.MP) != 0) &&
				(rpdl_SCI_GlobalData(12)->MP.MP_mode != RPDL_MP_MODE_DISABLED))
			{
				/* Set MPIE = 1 (multiprocessor mode reception) */
				SCI12.SCR.BIT.MPIE = 1;
			}

			/* Notify the user */
			if (rpdl_SCI_GlobalData(12)->RX_End_callback_func != PDL_NO_FUNC)
			{
				rpdl_SCI_GlobalData(12)->RX_End_callback_func();
			}
		}
	}
	/* Either the DMAC or DTC has passed on the interrupt */
	else
	{
		/* Call the callback function */
		if (rpdl_SCI_GlobalData(12)->RX_End_callback_func != PDL_NO_FUNC)
		{
			rpdl_SCI_GlobalData(12)->RX_End_callback_func();
		}
	}
}

/*""FUNC COMMENT""*************************************************************
* Module outline: SCIn transmit data interrupt processing
*------------------------------------------------------------------------------
* Declaration	: void Interrupt_SCIn_TXIn(void)
*------------------------------------------------------------------------------
* Function		:
*------------------------------------------------------------------------------
* Argument		: Nothing
*------------------------------------------------------------------------------
* Return value	: Nothing
*------------------------------------------------------------------------------
* Output		: TDR for that SCI channel
*------------------------------------------------------------------------------
* Use function	:
*------------------------------------------------------------------------------
* Notes			:
*------------------------------------------------------------------------------
* History		: 2013.05.14
*				: Ver 2.00
*				: CS Release
*""FUNC COMMENT END""*********************************************************/

#if FAST_INTC_VECTOR == VECT_SCI0_TXI0
__fast_interrupt  void Interrupt_SCI0_TXI0(void)
#else
#pragma vector=VECT_SCI0_TXI0
__interrupt  void Interrupt_SCI0_TXI0(void)
#endif
{
	Interrupt_SCIx_TXIx(0, &SCI0);
}

#if FAST_INTC_VECTOR == VECT_SCI1_TXI1
__fast_interrupt  void Interrupt_SCI1_TXI1(void)
#else
#pragma vector=VECT_SCI1_TXI1
__interrupt  void Interrupt_SCI1_TXI1(void)
#endif
{
	Interrupt_SCIx_TXIx(1, &SCI1);
}

#if FAST_INTC_VECTOR == VECT_SCI2_TXI2
__fast_interrupt  void Interrupt_SCI2_TXI2(void)
#else
#pragma vector=VECT_SCI2_TXI2
__interrupt  void Interrupt_SCI2_TXI2(void)
#endif
{
	Interrupt_SCIx_TXIx(2, &SCI2);
}

#if FAST_INTC_VECTOR == VECT_SCI3_TXI3
__fast_interrupt  void Interrupt_SCI3_TXI3(void)
#else
#pragma vector=VECT_SCI3_TXI3
__interrupt  void Interrupt_SCI3_TXI3(void)
#endif
{
	Interrupt_SCIx_TXIx(3, &SCI3);
}

#if FAST_INTC_VECTOR == VECT_SCI4_TXI4
__fast_interrupt  void Interrupt_SCI4_TXI4(void)
#else
#pragma vector=VECT_SCI4_TXI4
__interrupt  void Interrupt_SCI4_TXI4(void)
#endif
{
	Interrupt_SCIx_TXIx(4, &SCI4);
}

#if FAST_INTC_VECTOR == VECT_SCI5_TXI5
__fast_interrupt  void Interrupt_SCI5_TXI5(void)
#else
#pragma vector=VECT_SCI5_TXI5
__interrupt  void Interrupt_SCI5_TXI5(void)
#endif
{
	Interrupt_SCIx_TXIx(5, &SCI5);
}

#if FAST_INTC_VECTOR == VECT_SCI6_TXI6
__fast_interrupt  void Interrupt_SCI6_TXI6(void)
#else
#pragma vector=VECT_SCI6_TXI6
__interrupt  void Interrupt_SCI6_TXI6(void)
#endif
{
	Interrupt_SCIx_TXIx(6, &SCI6);
}

#if FAST_INTC_VECTOR == VECT_SCI7_TXI7
__fast_interrupt  void Interrupt_SCI7_TXI7(void)
#else
#pragma vector=VECT_SCI7_TXI7
__interrupt  void Interrupt_SCI7_TXI7(void)
#endif
{
	Interrupt_SCIx_TXIx(7, &SCI7);
}

#if FAST_INTC_VECTOR == VECT_SCI8_TXI8
__fast_interrupt  void Interrupt_SCI8_TXI8(void)
#else
#pragma vector=VECT_SCI8_TXI8
__interrupt  void Interrupt_SCI8_TXI8(void)
#endif
{
	Interrupt_SCIx_TXIx(8, &SCI8);
}

#if FAST_INTC_VECTOR == VECT_SCI9_TXI9
__fast_interrupt  void Interrupt_SCI9_TXI9(void)
#else
#pragma vector=VECT_SCI9_TXI9
__interrupt  void Interrupt_SCI9_TXI9(void)
#endif
{
	Interrupt_SCIx_TXIx(9, &SCI9);
}

#if FAST_INTC_VECTOR == VECT_SCI10_TXI10
__fast_interrupt  void Interrupt_SCI10_TXI10(void)
#else
#pragma vector=VECT_SCI10_TXI10
__interrupt  void Interrupt_SCI10_TXI10(void)
#endif
{
	Interrupt_SCIx_TXIx(10, &SCI10);
}

#if FAST_INTC_VECTOR == VECT_SCI11_TXI11
__fast_interrupt  void Interrupt_SCI11_TXI11(void)
#else
#pragma vector=VECT_SCI11_TXI11
__interrupt  void Interrupt_SCI11_TXI11(void)
#endif
{
	Interrupt_SCIx_TXIx(11, &SCI11);
}

/* This is a common function used by channels 0 to 11 to process the
transmit data interrupt.
NOTE: Channel 12 is handled separately. */
static void Interrupt_SCIx_TXIx(const uint8_t channel,
						volatile struct st_sci0 __sfr * const pSCIx)
{
	/*If SCI is in IIC mode (SIMR1.IICM == 1)
	then handle this interrupt separately*/
	if(pSCIx->SIMR1.BIT.IICM == 1)
	{
		R_SCI_IIC_Handler_TXI(channel);
		return;
	}

	/* Ok to process the string? */
	if (rpdl_SCI_GlobalData(channel)->tx_using_irq == true)
	{
		/* Another character to be sent? */
		if (
		/* Sending a string and next character is not a Null? */
		((rpdl_SCI_GlobalData(channel)->tx_threshold == 0) && (*rpdl_SCI_GlobalData(channel)->tx_string_pointer != NULL))
		||
		/* Not reached the byte total? */
		(rpdl_SCI_GlobalData(channel)->tx_counter < rpdl_SCI_GlobalData(channel)->tx_threshold)
		)
		{
			/* Send the character */
			pSCIx->TDR = *rpdl_SCI_GlobalData(channel)->tx_string_pointer;
			/* Perform SSR.TEND clearing operation after writing to TDR:
			(Dummy read of SSR plus dummy operation on read data.) */
			if (pSCIx->SSR.BIT.TEND == 1)
			{
				nop();
			}

			/* Increment the pointer */
			rpdl_SCI_GlobalData(channel)->tx_string_pointer++;

			/* Increment the counter */
			rpdl_SCI_GlobalData(channel)->tx_counter++;

			/*If have just written last byte out (end of string or count reached)*/
			if(((rpdl_SCI_GlobalData(channel)->tx_threshold == 0) &&
				(*rpdl_SCI_GlobalData(channel)->tx_string_pointer == NULL)) ||
				(rpdl_SCI_GlobalData(channel)->tx_counter == rpdl_SCI_GlobalData(channel)->tx_threshold))
			{
				/*Unless in smart card mode then disable this interrupt
				and enable the Transmit End Interrupt*/
				if(pSCIx->SCMR.BIT.SMIF == 0)
				{
					/* Disable TXI interrupt request generation */
					pSCIx->SCR.BIT.TIE = 0;
					rpdl_SCI_ModifyTx_IER(channel, 0);

					/*Enable Transmit End Interrupt */
					pSCIx->SCR.BIT.TEIE = 1;
					rpdl_SCI_ModifyTxEnd_IER(channel, 1);
				}
			}
		}
		else
		{
			/* Disable interrupt request generation */
			pSCIx->SCR.BIT.TIE = 0;

			/* Smart card mode? */
			if (pSCIx->SCMR.BIT.SMIF == 1)
			{
				/* Disable transmission and interrupt request generation */
				pSCIx->SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5);

				/* Call the callback function */
				if (rpdl_SCI_GlobalData(channel)->TX_End_callback_func != PDL_NO_FUNC)
				{
					rpdl_SCI_GlobalData(channel)->TX_End_callback_func();
				}
			}
		}
	}
	/* Either the DMAC or DTC has passed on the interrupt */
	else
	{
		/* Call the callback function */
		if (rpdl_SCI_GlobalData(channel)->TX_End_callback_func != PDL_NO_FUNC)
		{
			rpdl_SCI_GlobalData(channel)->TX_End_callback_func();
		}
	}
}

#if FAST_INTC_VECTOR == VECT_SCI12_TXI12
__fast_interrupt  void Interrupt_SCI12_TXI12(void)
#else
#pragma vector=VECT_SCI12_TXI12
__interrupt  void Interrupt_SCI12_TXI12(void)
#endif
{
	/*If SCI is in IIC mode (SIMR1.IICM == 1)
	then handle this interrupt separately*/
	if(SCI12.SIMR1.BIT.IICM == 1)
	{
		R_SCI_IIC_Handler_TXI(12);
		return;
	}

	/* Ok to process the string? */
	if (rpdl_SCI_GlobalData(12)->tx_using_irq == true)
	{
		/* Another character to be sent? */
		if (
		/* Sending a string and next character is not a Null? */
		((rpdl_SCI_GlobalData(12)->tx_threshold == 0) && (*rpdl_SCI_GlobalData(12)->tx_string_pointer != NULL))
		||
		/* Not reached the byte total? */
		(rpdl_SCI_GlobalData(12)->tx_counter < rpdl_SCI_GlobalData(12)->tx_threshold)
		)
		{
			/* Send the character */
			SCI12.TDR = *rpdl_SCI_GlobalData(12)->tx_string_pointer;
			/* Perform SSR.TEND clearing operation after writing to TDR:
			(Dummy read of SSR plus dummy operation on read data.) */
			if (SCI12.SSR.BIT.TEND == 1)
			{
				nop();
			}

			/* Increment the pointer */
			rpdl_SCI_GlobalData(12)->tx_string_pointer++;

			/* Increment the counter */
			rpdl_SCI_GlobalData(12)->tx_counter++;

			/*If have just written last byte out (end of string or count reached)*/
			if(((rpdl_SCI_GlobalData(12)->tx_threshold == 0) &&
				(*rpdl_SCI_GlobalData(12)->tx_string_pointer == NULL)) ||
				(rpdl_SCI_GlobalData(12)->tx_counter == rpdl_SCI_GlobalData(12)->tx_threshold))
			{
				/*Unless in smart card mode then disable this interrupt
				and enable the Transmit End Interrupt*/
				if(SCI12.SCMR.BIT.SMIF == 0)
				{
					/* Disable TXI interrupt request generation */
					SCI12.SCR.BIT.TIE = 0;
					rpdl_SCI_ModifyTx_IER(12, 0);

					/*Enable Transmit End Interrupt */
					SCI12.SCR.BIT.TEIE = 1;
					rpdl_SCI_ModifyTxEnd_IER(12, 1);
				}
			}
		}
		else
		{
			/* Disable interrupt request generation */
			SCI12.SCR.BIT.TIE = 0;

			/* Smart card mode? */
			if (SCI12.SCMR.BIT.SMIF == 1)
			{
				/* Disable transmission and interrupt request generation */
				SCI12.SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5);

				/* Call the callback function */
				if (rpdl_SCI_GlobalData(12)->TX_End_callback_func != PDL_NO_FUNC)
				{
					rpdl_SCI_GlobalData(12)->TX_End_callback_func();
				}
			}
		}
	}
	/* Either the DMAC or DTC has passed on the interrupt */
	else
	{
		/* Call the callback function */
		if (rpdl_SCI_GlobalData(12)->TX_End_callback_func != PDL_NO_FUNC)
		{
			rpdl_SCI_GlobalData(12)->TX_End_callback_func();
		}
	}
}

/*""FUNC COMMENT""*************************************************************
* Module outline: SCIn transmit end interrupt processing
*------------------------------------------------------------------------------
* Declaration	: void Interrupt_SCIn_TEIn(void)
*------------------------------------------------------------------------------
* Function		:
*------------------------------------------------------------------------------
* Argument		: Nothing
*------------------------------------------------------------------------------
* Return value	: Nothing
*------------------------------------------------------------------------------
* Output		: SCR for that channel
*------------------------------------------------------------------------------
* Use function	: rpdl_SCI_TX_End_callback_func()
*------------------------------------------------------------------------------
* Notes			:
*------------------------------------------------------------------------------
* History		: 2013.05.14
*				: Ver 2.00
*				: CS Release
*""FUNC COMMENT END""*********************************************************/
#if FAST_INTC_VECTOR == VECT_SCI0_TEI0
__fast_interrupt  void Interrupt_SCI0_TEI0(void)
#else
#pragma vector=VECT_SCI0_TEI0
__interrupt  void Interrupt_SCI0_TEI0(void)
#endif
{
	Interrupt_SCIx_TEIx(0, &SCI0);
}

#if FAST_INTC_VECTOR == VECT_SCI1_TEI1
__fast_interrupt  void Interrupt_SCI1_TEI1(void)
#else
#pragma vector=VECT_SCI1_TEI1
__interrupt  void Interrupt_SCI1_TEI1(void)
#endif
{
	Interrupt_SCIx_TEIx(1, &SCI1);
}

#if FAST_INTC_VECTOR == VECT_SCI2_TEI2
__fast_interrupt  void Interrupt_SCI2_TEI2(void)
#else
#pragma vector=VECT_SCI2_TEI2
__interrupt  void Interrupt_SCI2_TEI2(void)
#endif
{
	Interrupt_SCIx_TEIx(2, &SCI2);
}

#if FAST_INTC_VECTOR == VECT_SCI3_TEI3
__fast_interrupt  void Interrupt_SCI3_TEI3(void)
#else
#pragma vector=VECT_SCI3_TEI3
__interrupt  void Interrupt_SCI3_TEI3(void)
#endif
{
	Interrupt_SCIx_TEIx(3, &SCI3);
}

#if FAST_INTC_VECTOR == VECT_SCI4_TEI4
__fast_interrupt  void Interrupt_SCI4_TEI4(void)
#else
#pragma vector=VECT_SCI4_TEI4
__interrupt  void Interrupt_SCI4_TEI4(void)
#endif
{
	Interrupt_SCIx_TEIx(4, &SCI4);
}

#if FAST_INTC_VECTOR == VECT_SCI5_TEI5
__fast_interrupt  void Interrupt_SCI5_TEI5(void)
#else
#pragma vector=VECT_SCI5_TEI5
__interrupt  void Interrupt_SCI5_TEI5(void)
#endif
{
	Interrupt_SCIx_TEIx(5, &SCI5);
}

#if FAST_INTC_VECTOR == VECT_SCI6_TEI6
__fast_interrupt  void Interrupt_SCI6_TEI6(void)
#else
#pragma vector=VECT_SCI6_TEI6
__interrupt  void Interrupt_SCI6_TEI6(void)
#endif
{
	Interrupt_SCIx_TEIx(6, &SCI6);
}

#if FAST_INTC_VECTOR == VECT_SCI7_TEI7
__fast_interrupt  void Interrupt_SCI7_TEI7(void)
#else
#pragma vector=VECT_SCI7_TEI7
__interrupt  void Interrupt_SCI7_TEI7(void)
#endif
{
	Interrupt_SCIx_TEIx(7, &SCI7);
}

#if FAST_INTC_VECTOR == VECT_SCI8_TEI8
__fast_interrupt  void Interrupt_SCI8_TEI8(void)
#else
#pragma vector=VECT_SCI8_TEI8
__interrupt  void Interrupt_SCI8_TEI8(void)
#endif
{
	Interrupt_SCIx_TEIx(8, &SCI8);
}

#if FAST_INTC_VECTOR == VECT_SCI9_TEI9
__fast_interrupt  void Interrupt_SCI9_TEI9(void)
#else
#pragma vector=VECT_SCI9_TEI9
__interrupt  void Interrupt_SCI9_TEI9(void)
#endif
{
	Interrupt_SCIx_TEIx(9, &SCI9);
}

#if FAST_INTC_VECTOR == VECT_SCI10_TEI10
__fast_interrupt  void Interrupt_SCI10_TEI10(void)
#else
#pragma vector=VECT_SCI10_TEI10
__interrupt  void Interrupt_SCI10_TEI10(void)
#endif
{
	Interrupt_SCIx_TEIx(10, &SCI10);
}

#if FAST_INTC_VECTOR == VECT_SCI11_TEI11
__fast_interrupt  void Interrupt_SCI11_TEI11(void)
#else
#pragma vector=VECT_SCI11_TEI11
__interrupt  void Interrupt_SCI11_TEI11(void)
#endif
{
	Interrupt_SCIx_TEIx(11, &SCI11);
}

/* This is a common function used by channels 0 to 11 to process the
transmit end interrupt.
NOTE: Channel 12 is handled separately. */
static void Interrupt_SCIx_TEIx(const uint8_t channel,
						volatile struct st_sci0 __sfr * const pSCIx)
{
	/* If the channel is operating in synchronous mode, transmit only and
	   with an an external clock, the TXD pin may need to be held active
	   for longer (up to half a bit period) to avoid violating the data hold
	   time for the receiving device.  If a delay is required then for example
	   use rpdl_wait_time (X / rpdl_SCI_GlobalData(<channel>)->baud_rate)
	   here, where X = the portion of a bit period required.
	   So rpdl_wait_time(0.5 / rpdl_SCI_GlobalData(channel)->baud_rate);
	   creates a delay of half a bit period.
	   The function call is omitted by default to avoid an unnecessary delay. */

	/*Tx has finished.*/

	/*If in async mode (SMIF == 0 and CM == 0): To allow control of the
	Tx pin when idle, configure it as a general IO pin.
	NOTE: This must be done before disabling SCR.TE as when TE is cleared the
	pin becomes Hi-z.*/
	if( (pSCIx->SMR.BIT.CM == 0) && (pSCIx->SCMR.BIT.SMIF == 0) )
	{
		rpdl_SCI_SetTxPinGeneralIO(channel, true);
	}

	/* If in sync mode (SMR.CM==1) then can't clear SCR.TE and SCR.RE independantly.
	Hence if RE bit is enabled but Rx is not pending (SCR.RIE==0) also clear RE*/
	if((pSCIx->SMR.BIT.CM == 1) &&
	   ((pSCIx->SCR.BYTE & (BIT_4 | BIT_6)) == BIT_4))

	{
		/*Rx has finished, so disable RE,RIE as well as TE,TIE,TEIE*/
		pSCIx->SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5 & INV_BIT_2 &
		 							INV_BIT_6 & INV_BIT_4);
	}
	else
	{
		/* Disable interrupt request generation, and try to disable transmission */
		pSCIx->SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5 & INV_BIT_2);
	}

	/* Call the callback function */
	if (rpdl_SCI_GlobalData(channel)->TX_End_callback_func != PDL_NO_FUNC)
	{
		rpdl_SCI_GlobalData(channel)->TX_End_callback_func();
	}
}

#if FAST_INTC_VECTOR == VECT_SCI12_TEI12
__fast_interrupt  void Interrupt_SCI12_TEI12(void)
#else
#pragma vector=VECT_SCI12_TEI12
__interrupt  void Interrupt_SCI12_TEI12(void)
#endif
{
	/* If the channel is operating in synchronous mode, transmit only and
	   with an an external clock, the TXD pin may need to be held active
	   for longer (up to half a bit period) to avoid violating the data hold
	   time for the receiving device.  If a delay is required then for example
	   use rpdl_wait_time (X / rpdl_SCI_GlobalData(<channel>)->baud_rate)
	   here, where X = the portion of a bit period required.
	   So rpdl_wait_time(0.5 / rpdl_SCI_GlobalData(12)->baud_rate);
	   creates a delay of half a bit period.
	   The function call is omitted by default to avoid an unnecessary delay. */

	/*Tx has finished.*/

	/*If in async mode (SMIF == 0 and CM == 0): To allow control of the
	Tx pin when idle, configure it as a general IO pin.
	NOTE: This must be done before disabling SCR.TE as when TE is cleared the
	pin becomes Hi-z.*/
	if( (SCI12.SMR.BIT.CM == 0) && (SCI12.SCMR.BIT.SMIF == 0) )
	{
		rpdl_SCI_SetTxPinGeneralIO(12, true);
	}

	/*If in sync mode (SMR.CM==1) then can't clear SCR.TE and SCR.RE independantly.
	Hence if RE bit is enabled but Rx is not pending (SCR.RIE==0) also clear RE*/
	if((SCI12.SMR.BIT.CM == 1) &&
	   ((SCI12.SCR.BYTE & (BIT_4 | BIT_6)) == BIT_4))

	{
		/*Rx has finished, so disable RE,RIE as well as TE,TIE,TEIE*/
		SCI12.SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5 & INV_BIT_2 &
		 							INV_BIT_6 & INV_BIT_4);
	}
	else
	{
		/* Disable interrupt request generation, and try to disable transmission */
		SCI12.SCR.BYTE &= (uint8_t)(INV_BIT_7 & INV_BIT_5 & INV_BIT_2);
	}

	/* Call the callback function */
	if (rpdl_SCI_GlobalData(12)->TX_End_callback_func != PDL_NO_FUNC)
	{
		rpdl_SCI_GlobalData(12)->TX_End_callback_func();
	}
}

/* End of file */
