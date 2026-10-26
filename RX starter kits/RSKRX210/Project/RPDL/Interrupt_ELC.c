/* Adapted for use with IAR Embedded Workbench */
/*""FILE COMMENT""*******************************************************
* System Name  : Interrupt program for RX210xx
* File Name    : Interrupt_ELC.c
* Version    : 0.20
* Contents    : Interrupt handlers for the ELC.
* Customer    : 
* Model       :
* Order       :
* CPU      : RX
* Compiler    : RXC
* OS      : Nothing
* Programmer  : 
* Note      : 
************************************************************************
* Copyright, 2011. Renesas Electronics Corporation
* and Renesas Solutions Corporation
************************************************************************
* History    : 2011.08.12
*        : Ver 0.20
*        : WS2 release.
*""FILE COMMENT END""**************************************************/

#include "r_pdl_elc.h"
#include "r_pdl_definitions.h"
#include "r_pdl_user_definitions.h"

/*""FUNC COMMENT""***************************************************
* Module outline : ELC interrupt processing
*-------------------------------------------------------------------
* Declaration  : void Interrupt_ELSR1xI(void)
*-------------------------------------------------------------------
* Function    : Interrupt processing function for ELC interrupts
*          ELSR18I and ELSR19I
*-------------------------------------------------------------------
* Argument    : Nothing
*-------------------------------------------------------------------
* Return value  : Nothing
*-------------------------------------------------------------------
* Input      :
* Output    : 
*-------------------------------------------------------------------
* Use function  : 
*-------------------------------------------------------------------
* Notes      :
*-------------------------------------------------------------------
* History    : 2011.08.12
*        : Ver 0.20
*        : WS2 release.
*""FILE COMMENT END""**************************************************/

#if FAST_INTC_VECTOR == VECT_ELC_ELSR18I
__fast_interrupt void Interrupt_ELSR18I(void)
#else
#pragma vector = VECT_ELC_ELSR18I
__interrupt void Interrupt_ELSR18I(void)
#endif
{
  /* Call the user function */
  if(rpdl_ELC_callback_func[0] != PDL_NO_FUNC)
  {
    rpdl_ELC_callback_func[0]();
  }
}

#if FAST_INTC_VECTOR == VECT_ELC_ELSR19I
__fast_interrupt void Interrupt_ELSR19I(void)
#else
#pragma vector = VECT_ELC_ELSR19I
__interrupt void Interrupt_ELSR19I(void)
#endif
{
  /* Call the user function */
  if(rpdl_ELC_callback_func[1] != PDL_NO_FUNC)
  {
    rpdl_ELC_callback_func[1]();
  }
}
/* End of file */
