/* Adapted for use with IAR Embedded Workbench */
/*""FILE COMMENT""*******************************************************
* System Name  : Interrupt program for RX610xV
* File Name    : Interrupt_BSC.c
* Version    : 0.10
* Contents    : Interrupt handler for the BSC
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
* History    : 2009.09.23
*        : Ver 0.10
*""FILE COMMENT END""**************************************************/

#include "r_pdl_bsc.h"
#include "r_pdl_definitions.h"

/*""FUNC COMMENT""***************************************************
* Module outline : BSC interrupt processing
*-------------------------------------------------------------------
* Include    : r_pdl_bsc_library_RX610xV.h
*-------------------------------------------------------------------
* Declaration  : void InterruptBSC(void)
*-------------------------------------------------------------------
* Function    : Interrupt processing function for the BSC
*-------------------------------------------------------------------
* Argument    : Nothing
*-------------------------------------------------------------------
* Return value  : Nothing
*-------------------------------------------------------------------
* Input      :
* Output    : 
*-------------------------------------------------------------------
* Use function  : rpdl_BSC_callback_func
*-------------------------------------------------------------------
* Notes       :
*-------------------------------------------------------------------
* History    : 2009.09.25
*        : Ver 0.10
*        : 2010.02.02
*        : Ver 0.11
*        : Added callback function checking
*        : 2010.02.08
*        : Ver 0.14
*        : Removed DMAC trigger control
*        : 2010.02.22
*        : Ver 0.15
*        : Removed fast interrupt code
*""FUNC COMMENT END""**********************************************/

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_BUSERR
__fast_interrupt void Interrupt_BSC(void)
#else
#pragma vector = PDL_INTC_VECTOR_BUSERR
__interrupt void Interrupt_BSC(void)
#endif
{
  /* Call the user function */
  if (rpdl_BSC_callback_func != PDL_NO_FUNC)
  {
    rpdl_BSC_callback_func();
  }
}
/* End of file */
