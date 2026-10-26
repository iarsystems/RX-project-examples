/* Adapted for use with IAR Embedded Workbench */
/*""FILE COMMENT""*******************************************************
* System Name  : Interrupt program for RX610xV
* File Name    : Interrupt_CMT.c
* Version    : 0.10
* Contents    : Interrupt handlers for the CMT channels
* Customer    : 
* Model       :
* Order       :
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

#include "r_pdl_cmt.h"
#include "r_pdl_definitions.h"

/*""FUNC COMMENT""***************************************************
* Module outline : Timer interrupt processing
*-------------------------------------------------------------------
* Include    : r_pdl_cmt_library_RX610xV.h
*-------------------------------------------------------------------
* Declaration  : void Interrupt_CMTn(void)
*-------------------------------------------------------------------
* Function    : Interrupt processing function for CMT channel n
*-------------------------------------------------------------------
* Argument    : Nothing
*-------------------------------------------------------------------
* Return value  : Nothing
*-------------------------------------------------------------------
* Input      :
* Output    : 
*-------------------------------------------------------------------
* Use function  : CMT_callback_func[n]
*-------------------------------------------------------------------
* Notes      :
*-------------------------------------------------------------------
* History    : 2009.09.25
*        : Ver 0.10
*        : 2009.10.08
*        : Ver 0.11
*        : Added DMAC control
*        : 2009.11.24
*        : Ver 0.12
*        : Removed clearing of the global flag (not required)
*        : 2009.11.26
*        : Ver 0.13
*        : Modified DMAC control
*        : 2010.02.02
*        : Ver 0.14
*        : Added callback function checking
*        : 2010.02.08
*        : Ver 0.14
*        : Removed DMAC trigger control
*        : 2010.02.22
*        : Ver 0.15
*        : Removed fast interrupt code
*        : 2010.02.23
*        : Ver 0.16
*        : Updated for the new iodefine.h file.
*""FUNC COMMENT END""**********************************************/

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_CMT0
__fast_interrupt void Interrupt_CMT0(void)
#else
#pragma vector = PDL_INTC_VECTOR_CMT0
__interrupt void Interrupt_CMT0(void)
#endif
{
  if (rpdl_CMT_one_shot[0] != false)
  {
    /* Stop the channel 0 clock input */
    CMT.CMSTR0.BIT.STR0 = 0;

    /* Clear the interrupt request flag (the timer may have set it again) */
    ICU.IR[PDL_INTC_VECTOR_CMT0].BIT.IR = 0;
  }

  /* Call the user function */
  if (rpdl_CMT_callback_func[0] != PDL_NO_FUNC)
  {
    rpdl_CMT_callback_func[0]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_CMT1
__fast_interrupt void Interrupt_CMT1(void)
#else
#pragma vector = PDL_INTC_VECTOR_CMT1
__interrupt void Interrupt_CMT1(void)
#endif
{
  if (rpdl_CMT_one_shot[1] != false)
  {
    /* Stop the channel 0 clock input */
    CMT.CMSTR0.BIT.STR1 = 0;

    /* Clear the interrupt request flag (the timer may have set it again) */
    ICU.IR[PDL_INTC_VECTOR_CMT1].BIT.IR = 0;
  }

  /* Call the user function */
  if (rpdl_CMT_callback_func[1] != PDL_NO_FUNC)
  {
    rpdl_CMT_callback_func[1]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_CMT2
__fast_interrupt void Interrupt_CMT2(void)
#else
#pragma vector = PDL_INTC_VECTOR_CMT2
__interrupt void Interrupt_CMT2(void)
#endif
{
  if (rpdl_CMT_one_shot[2] != false)
  {
    /* Stop the channel 0 clock input */
    CMT.CMSTR1.BIT.STR2 = 0;

    /* Clear the interrupt request flag (the timer may have set it again) */
    ICU.IR[PDL_INTC_VECTOR_CMT2].BIT.IR = 0;
  }

  /* Call the user function */
  if (rpdl_CMT_callback_func[2] != PDL_NO_FUNC)
  {
    rpdl_CMT_callback_func[2]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_CMT3
__fast_interrupt void Interrupt_CMT3(void)
#else
#pragma vector = PDL_INTC_VECTOR_CMT3
__interrupt void Interrupt_CMT3(void)
#endif
{
  if (rpdl_CMT_one_shot[3] != false)
  {
    /* Stop the channel 0 clock input */
    CMT.CMSTR1.BIT.STR3 = 0;

    /* Clear the interrupt request flag (the timer may have set it again) */
    ICU.IR[PDL_INTC_VECTOR_CMT3].BIT.IR = 0;
  }

  /* Call the user function */
  if (rpdl_CMT_callback_func[3] != PDL_NO_FUNC)
  {
    rpdl_CMT_callback_func[3]();
  }
}
/* End of file */
