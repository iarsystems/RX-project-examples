/* Adapted for use with IAR Embedded Workbench */
/*""FILE COMMENT""*************************************************************
* System Name  : Interrupt program for RX62G and RX62T
* File Name    : Interrupt_not_RPDL.c
* Version    : 1.00
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
*******************************************************************************
* Copyright, 2012. Renesas Electronics Corporation and Renesas Solutions Corp.
*******************************************************************************
* History    : 2012.09.11
*        : Ver 1.00
*        : CS release.
*""FILE COMMENT END""*********************************************************/

#include "r_pdl_common_defs_RX62Gxx.h"
#include "r_pdl_user_definitions.h"

#if FAST_INTC_VECTOR == VECT_FCUIF_FIFERR
__fast_interrupt void Interrupt_FIFERR(void)
#else
#pragma vector = VECT_FCUIF_FIFERR
__interrupt void Interrupt_FIFERR(void)
#endif
{
  /* The user can put their own code here */
  nop();
}

#if FAST_INTC_VECTOR == VECT_FCUIF_FRDYI
__fast_interrupt void Interrupt_FRDYI(void)
#else
#pragma vector = VECT_FCUIF_FRDYI
__interrupt void Interrupt_FRDYI(void)
#endif
{
  /* The user can put their own code here */
  nop();
}

#if FAST_INTC_VECTOR == VECT_CAN0_ERS0
__fast_interrupt void Interrupt_ERS0(void)
#else
#pragma vector = VECT_CAN0_ERS0
__interrupt void Interrupt_ERS0(void)
#endif
{
  /* The user can put their own code here */
  nop();
}

#if FAST_INTC_VECTOR == VECT_CAN0_RXF0
__fast_interrupt void Interrupt_RXF0(void)
#else
#pragma vector = VECT_CAN0_RXF0
__interrupt void Interrupt_RXF0(void)
#endif
{
  /* The user can put their own code here */
  nop();
}

#if FAST_INTC_VECTOR == VECT_CAN0_TXF0
__fast_interrupt void Interrupt_TXF0(void)
#else
#pragma vector = VECT_CAN0_TXF0
__interrupt void Interrupt_TXF0(void)
#endif
{
  /* The user can put their own code here */
  nop();
}

#if FAST_INTC_VECTOR == VECT_CAN0_RXM0
__fast_interrupt void Interrupt_RXM0(void)
#else
#pragma vector = VECT_CAN0_RXM0
__interrupt void Interrupt_RXM0(void)
#endif
{
  /* The user can put their own code here */
  nop();
}

#if FAST_INTC_VECTOR == VECT_CAN0_TXM0
__fast_interrupt void Interrupt_TXM0(void)
#else
#pragma vector = VECT_CAN0_TXM0
__interrupt void Interrupt_TXM0(void)
#endif
{
  /* The user can put their own code here */
  nop();
}

/* End of file */
