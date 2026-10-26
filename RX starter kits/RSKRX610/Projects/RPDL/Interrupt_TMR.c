/* Adapted for use with IAR Embedded Workbench */
/*""FILE COMMENT""*******************************************************
* System Name  : Interrupt program for RX610xV
* File Name    : Interrupt_TMR.c
* Version    : 0.10
* Contents    : Interrupt handlers for the Timer units
* Customer    : 
* Model      : 
* Order      : 
* CPU      : RX600
* Compiler    : RXC
* OS      : Nothing
* Programmer  : 
* Note      : 
************************************************************************
* Copyright,2009 RENESAS TECHNOLOGY CORPORATION
* AND RENESAS SOLUTIONS CORPORATION
************************************************************************
* History    : 2009.09.08
*        : Ver 0.10
*""FILE COMMENT END""**************************************************/

#include "r_pdl_tmr.h"
#include "r_pdl_definitions.h"

/*""FUNC COMMENT""***************************************************
* Module outline : Timer interrupt processing
*-------------------------------------------------------------------
* Include    : rapi_tmr_library_RX610xV.h
*-------------------------------------------------------------------
* Declaration  : void InterruptTMRn_XXX(void)
*-------------------------------------------------------------------
* Function    : Interrupt processing function for Timer TMR channel n
*-------------------------------------------------------------------
* Argument    : Nothing
*-------------------------------------------------------------------
* Return value  : Nothing
*-------------------------------------------------------------------
* Input      : 
* Output    : 
*-------------------------------------------------------------------
* Use function  : R_TmrOverflowFunc[n](), R_TmrCMAFunc[n]() or
*        : R_TmrCMBFunc[n]() as appropriate.
*-------------------------------------------------------------------
* Notes      : 
*-------------------------------------------------------------------
* History    : 2009.09.25
*        : Ver 0.10
*        : 2010.01.14
*        : Ver 0.11
*        : Added a callback check to the CMA and CMB handlers
*        : (because there is only one IPR for each channel).
*        : 2010.02.02
*        : Ver 0.12
*        : Added callback function checking to the OV handlers
*        : 2010.02.22
*        : Ver 0.13
*        : Removed fast interrupt code
*        : 2010.02.23
*        : Ver 0.14
*        : Updated for the new iodefine.h file.
*""FUNC COMMENT END""**********************************************/

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_CMIA0
__fast_interrupt void InterruptTMR0_CMA(void)
#else
#pragma vector = PDL_INTC_VECTOR_CMIA0
__interrupt void InterruptTMR0_CMA(void)
#endif
{
  /* Call the user function? */
  if (rpdl_TMR_CMA_callback_func[0] != PDL_NO_FUNC)
  {
    /* Call the user function */
    rpdl_TMR_CMA_callback_func[0]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_CMIB0
__fast_interrupt void InterruptTMR0_CMB(void)
#else
#pragma vector = PDL_INTC_VECTOR_CMIB0
__interrupt void InterruptTMR0_CMB(void)
#endif
{
  if (rpdl_TMR_one_shot[0] != false)
  {
    /* Channel mode? */
    if (rpdl_TMR_one_shot[0] == CHANNEL_MODE)
    {
      /* Stop the channel 0 clock input */
      TMR0.TCCR.BYTE = 0x00u;
    }
    /* Unit mode? */
    else
    {
      /* Stop the channel 1 clock input */
      TMR1.TCCR.BYTE = 0x00u;
    }

    rpdl_TMR_one_shot[0] = false;

    /* Clear the interrupt request flag (the timer may have set it again) */
    ICU.IR[PDL_INTC_VECTOR_CMIB0].BIT.IR = 0;
  }

  /* Call the user function? */
  if (rpdl_TMR_CMB_callback_func[0] != PDL_NO_FUNC)
  {
    rpdl_TMR_CMB_callback_func[0]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_OVI0
__fast_interrupt void InterruptTMR0_OV(void)
#else
#pragma vector = PDL_INTC_VECTOR_OVI0
__interrupt void InterruptTMR0_OV(void)
#endif
{
  /* Call the user function */
  if (rpdl_TMR_Overflow_callback_func[0] != PDL_NO_FUNC)
  {
    rpdl_TMR_Overflow_callback_func[0]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_CMIA1
__fast_interrupt void InterruptTMR1_CMA(void)
#else
#pragma vector = PDL_INTC_VECTOR_CMIA1
__interrupt void InterruptTMR1_CMA(void)
#endif
{
  /* Call the user function? */
  if (rpdl_TMR_CMA_callback_func[1] != PDL_NO_FUNC)
  {
    /* Call the user function */
    rpdl_TMR_CMA_callback_func[1]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_CMIB1
__fast_interrupt void InterruptTMR1_CMB(void)
#else
#pragma vector = PDL_INTC_VECTOR_CMIB1
__interrupt void InterruptTMR1_CMB(void)
#endif
{
  if (rpdl_TMR_one_shot[1] != false)
  {
    /* Stop the channel 1 clock input */
    TMR1.TCCR.BYTE = 0x00u;

    rpdl_TMR_one_shot[1] = false;

    /* Clear the interrupt request flag (the timer may have set it again) */
    ICU.IR[PDL_INTC_VECTOR_CMIB1].BIT.IR = 0u;
  }

  /* Call the user function? */
  if (rpdl_TMR_CMB_callback_func[1] != PDL_NO_FUNC)
  {
    rpdl_TMR_CMB_callback_func[1]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_OVI1
__fast_interrupt void InterruptTMR1_OV(void)
#else
#pragma vector = PDL_INTC_VECTOR_OVI1
__interrupt void InterruptTMR1_OV(void)
#endif
{
  /* Call the user function */
  if (rpdl_TMR_Overflow_callback_func[1] != PDL_NO_FUNC)
  {
    rpdl_TMR_Overflow_callback_func[1]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_CMIA2
__fast_interrupt void InterruptTMR2_CMA(void)
#else
#pragma vector = PDL_INTC_VECTOR_CMIA2
__interrupt void InterruptTMR2_CMA(void)
#endif
{
  /* Call the user function? */
  if (rpdl_TMR_CMA_callback_func[2] != PDL_NO_FUNC)
  {
    /* Call the user function */
    rpdl_TMR_CMA_callback_func[2]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_CMIB2
__fast_interrupt void InterruptTMR2_CMB(void)
#else
#pragma vector = PDL_INTC_VECTOR_CMIB2
__interrupt void InterruptTMR2_CMB(void)
#endif
{
  if (rpdl_TMR_one_shot[2] != false)
  {
    /* Channel mode? */
    if (rpdl_TMR_one_shot[2] == CHANNEL_MODE)
    {
      /* Stop the channel 2 clock input */
      TMR2.TCCR.BYTE = 0x00u;
    }
    /* Unit mode? */
    else
    {
      /* Stop the channel 3 clock input */
      TMR3.TCCR.BYTE = 0x00u;
    }

    rpdl_TMR_one_shot[2] = false;

    /* Clear the interrupt request flag (the timer may have set it again) */
    ICU.IR[PDL_INTC_VECTOR_CMIB2].BIT.IR = 0;
  }

  /* Call the user function? */
  if (rpdl_TMR_CMB_callback_func[2] != PDL_NO_FUNC)
  {
    rpdl_TMR_CMB_callback_func[2]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_OVI2
__fast_interrupt void InterruptTMR2_OV(void)
#else
#pragma vector = PDL_INTC_VECTOR_OVI2
__interrupt void InterruptTMR2_OV(void)
#endif
{
  /* Call the user function */
  if (rpdl_TMR_Overflow_callback_func[2] != PDL_NO_FUNC)
  {
    rpdl_TMR_Overflow_callback_func[2]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_CMIA3
__fast_interrupt void InterruptTMR3_CMA(void)
#else
#pragma vector = PDL_INTC_VECTOR_CMIA3
__interrupt void InterruptTMR3_CMA(void)
#endif
{
  /* Call the user function? */
  if (rpdl_TMR_CMA_callback_func[3] != PDL_NO_FUNC)
  {
    /* Call the user function */
    rpdl_TMR_CMA_callback_func[3]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_CMIB3
__fast_interrupt void InterruptTMR3_CMB(void)
#else
#pragma vector = PDL_INTC_VECTOR_CMIB3
__interrupt void InterruptTMR3_CMB(void)
#endif
{
  if (rpdl_TMR_one_shot[3] != false)
  {
    /* Stop the channel 1 clock input */
    TMR3.TCCR.BYTE = 0x00u;

    rpdl_TMR_one_shot[3] = false;

    /* Clear the interrupt request flag (the timer may have set it again) */
    ICU.IR[PDL_INTC_VECTOR_CMIB3].BIT.IR = 0;
  }

  /* Call the user function? */
  if (rpdl_TMR_CMB_callback_func[3] != PDL_NO_FUNC)
  {
    rpdl_TMR_CMB_callback_func[3]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_OVI3
__fast_interrupt void InterruptTMR3_OV(void)
#else
#pragma vector = PDL_INTC_VECTOR_OVI3
__interrupt void InterruptTMR3_OV(void)
#endif
{
  /* Call the user function */
  if (rpdl_TMR_Overflow_callback_func[3] != PDL_NO_FUNC)
  {
    rpdl_TMR_Overflow_callback_func[3]();
  }
}
/* End of file */
