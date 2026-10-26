/* Adapted for use with IAR Embedded Workbench */
/*""FILE COMMENT""*************************************************************
* System Name  : Interrupt program for RX62G and RX62T
* File Name    : Interrupt_LIN.c
* Version    : 1.00
* Contents    : Interrupt handlers for all LIN channels
* Customer    : 
* Model      : 
* Order      : 
* CPU      : RX
* Compiler    : RXC
* OS      : Nothing
* Programmer  : 
* Note      : 
*******************************************************************************
* Copyright, 2012. Renesas Electronics Corporation and Renesas Solutions Corp.
*******************************************************************************
* History    : 2012.09.11
*        : Ver 1.00
*        : CS release.
*""FILE COMMENT END""*********************************************************/

#include "r_pdl_lin.h"
#include "r_pdl_definitions.h"
#include "r_pdl_user_definitions.h"

/*""FUNC COMMENT""*************************************************************
* Module outline: LINn interrupt processing
*------------------------------------------------------------------------------
* Declaration  : void Interrupt_LINn_LINn(void)
*------------------------------------------------------------------------------
* Function    : 
*------------------------------------------------------------------------------
* Argument    : Nothing
*------------------------------------------------------------------------------
* Return value  : Nothing
*------------------------------------------------------------------------------
* Output    : None
*------------------------------------------------------------------------------
* Use function  : rpdl_LIN_callback_func
*------------------------------------------------------------------------------
* Notes      : 
*------------------------------------------------------------------------------
* History    : 2012.09.11
*        : Ver 1.00
*        : CS release.
*""FUNC COMMENT END""*********************************************************/

#if FAST_INTC_VECTOR == VECT_LIN0_LIN0
__fast_interrupt void Interrupt_LIN0_LIN0(void)
#else
#pragma vector = VECT_LIN0_LIN0
__interrupt void Interrupt_LIN0_LIN0(void)
#endif
{
  /* Notify the user */
  if (rpdl_LIN_callback_func != PDL_NO_FUNC)
  {
    /* Notify the user */
    rpdl_LIN_callback_func();
  }
}

/* End of file */
