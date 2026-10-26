/* Adapted for use with IAR Embedded Workbench */
/*""FILE COMMENT""*******************************************************
* System Name  : Interrupt program for RX210xx
* File Name    : Interrupt_not_RPDL.c
* Version    : 0.20
* Contents    : Interrupt handlers for peripherals outside the scope
*        : of RPDL
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

#include "r_pdl_common_defs_RX210xx.h"
#include "r_pdl_user_definitions.h"

#if FAST_INTC_VECTOR == VECT_FCU_FIFERR
__fast_interrupt void Interrupt_FIFERR(void)
#else
#pragma vector = VECT_FCU_FIFERR
__interrupt void Interrupt_FIFERR(void)
#endif
{
  /* The user can put their own code here */
  nop();
}

#if FAST_INTC_VECTOR == VECT_FCU_FRDYI
__fast_interrupt void Interrupt_FRDYI(void)
#else
#pragma vector = VECT_FCU_FRDYI
__interrupt void Interrupt_FRDYI(void)
#endif
{
  /* The user can put their own code here */
  nop();
}
/* End of file */
