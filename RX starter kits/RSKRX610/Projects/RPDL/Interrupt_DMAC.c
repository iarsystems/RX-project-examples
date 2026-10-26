/* Adapted for use with IAR Embedded Workbench */
/*""FILE COMMENT""*******************************************************
* System Name  : Interrupt program for RX610xV
* File Name    : Interrupt_DMAC.c
* Version    : 0.10
* Contents    : Interrupt handlers for the CMT channels
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

#include "r_pdl_dmac.h"
#include "r_pdl_definitions.h"

/*""FUNC COMMENT""***************************************************
* Module outline : DMAC interrupt processing
*-------------------------------------------------------------------
* Include    : r_pdl_dmac_library_RX610xV.h
*-------------------------------------------------------------------
* Declaration  : void Interrupt_DMACn(void)
*-------------------------------------------------------------------
* Function    : Interrupt processing function for DMAC channel n
*-------------------------------------------------------------------
* Argument    : Nothing
*-------------------------------------------------------------------
* Return value  : Nothing
*-------------------------------------------------------------------
* Input      : 
* Output    : 
*-------------------------------------------------------------------
* Use function  : DMAC_callback_func[n]
*-------------------------------------------------------------------
* Notes      : 
*-------------------------------------------------------------------
* History    : 2009.10.12
*        : Ver 0.10
*        : 2009.11.26
*        : Ver 0.11
*        : Modified ISELR control
*        : 2009.12.14
*        : Ver 0.12
*        : Changed DMEDET flag clearing to byte access
*        : 2010.02.02
*        : Ver 0.13
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

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_DMTEND0
__fast_interrupt void Interrupt_DMAC0(void)
#else
#pragma vector = PDL_INTC_VECTOR_DMTEND0
__interrupt void Interrupt_DMAC0(void)
#endif
{
  /* Clear the Transfer End Detect flag */
  DMAC.DMEDET.BYTE = BIT_0;

  /* Call the user function */
  if (rpdl_DMAC_callback_func[0] != PDL_NO_FUNC)
  {
    rpdl_DMAC_callback_func[0]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_DMTEND1
__fast_interrupt void Interrupt_DMAC1(void)
#else
#pragma vector = PDL_INTC_VECTOR_DMTEND1
__interrupt void Interrupt_DMAC1(void)
#endif
{
  /* Clear the Transfer End Detect flag */
  DMAC.DMEDET.BYTE = BIT_1;

  /* Call the user function */
  if (rpdl_DMAC_callback_func[1] != PDL_NO_FUNC)
  {
    rpdl_DMAC_callback_func[1]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_DMTEND2
__fast_interrupt void Interrupt_DMAC2(void)
#else
#pragma vector = PDL_INTC_VECTOR_DMTEND2
__interrupt void Interrupt_DMAC2(void)
#endif
{
  /* Clear the Transfer End Detect flag */
  DMAC.DMEDET.BYTE = BIT_2;

  /* Call the user function */
  if (rpdl_DMAC_callback_func[2] != PDL_NO_FUNC)
  {
    rpdl_DMAC_callback_func[2]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_DMTEND3
__fast_interrupt void Interrupt_DMAC3(void)
#else
#pragma vector = PDL_INTC_VECTOR_DMTEND3
__interrupt void Interrupt_DMAC3(void)
#endif
{
  /* Clear the Transfer End Detect flag */
  DMAC.DMEDET.BYTE = BIT_3;

  /* Call the user function */
  if (rpdl_DMAC_callback_func[3] != PDL_NO_FUNC)
  {
    rpdl_DMAC_callback_func[3]();
  }
}
/* End of file */
