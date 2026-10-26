/* Adapted for use with IAR Embedded Workbench */
/*""FILE COMMENT""*************************************************************
* System Name  : Interrupt program for RX62G and RX62T
* File Name    : Interrupt_GPT.c
* Version    : 1.00
* Contents    : Interrupt handlers for the GPT channels
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

#include "r_pdl_gpt.h"
#include "r_pdl_definitions.h"
#include "r_pdl_user_definitions.h"

/*""FUNC COMMENT""*************************************************************
* Module outline : Timer interrupt processing
*------------------------------------------------------------------------------
* Declaration  : void Interrupt_GTCIAn(void)
*------------------------------------------------------------------------------
* Function    : Interrupt processing for A events
*------------------------------------------------------------------------------
* Argument    : Nothing
*------------------------------------------------------------------------------
* Return value  : Nothing
*------------------------------------------------------------------------------
* Input      :
* Output    : 
*------------------------------------------------------------------------------
* Use function  : rpdl_GPT_GTCIA_callback_func[n]
*------------------------------------------------------------------------------
* Notes      :
*------------------------------------------------------------------------------
* History    : 2012.09.11
*        : Ver 1.00
*        : CS release.
*""FUNC COMMENT END""*********************************************************/

#if FAST_INTC_VECTOR == VECT_GPT0_GTCIA0
__fast_interrupt void Interrupt_GTCIA0(void)
#else
#pragma vector = VECT_GPT0_GTCIA0
__interrupt void Interrupt_GTCIA0(void)
#endif
{
  /* Call the user function */
  if (rpdl_GPT_GTCIA_callback_func[0] != PDL_NO_FUNC)
  {
    rpdl_GPT_GTCIA_callback_func[0]();
  }
}

#if FAST_INTC_VECTOR == VECT_GPT1_GTCIA1
__fast_interrupt void Interrupt_GTCIA1(void)
#else
#pragma vector = VECT_GPT1_GTCIA1
__interrupt void Interrupt_GTCIA1(void)
#endif
{
  /* Call the user function */
  if (rpdl_GPT_GTCIA_callback_func[1] != PDL_NO_FUNC)
  {
    rpdl_GPT_GTCIA_callback_func[1]();
  }
}

#if FAST_INTC_VECTOR == VECT_GPT2_GTCIA2
__fast_interrupt void Interrupt_GTCIA2(void)
#else
#pragma vector = VECT_GPT2_GTCIA2
__interrupt void Interrupt_GTCIA2(void)
#endif
{
  /* Call the user function */
  if (rpdl_GPT_GTCIA_callback_func[2] != PDL_NO_FUNC)
  {
    rpdl_GPT_GTCIA_callback_func[2]();
  }
}

#if FAST_INTC_VECTOR == VECT_GPT3_GTCIA3
__fast_interrupt void Interrupt_GTCIA3(void)
#else
#pragma vector = VECT_GPT3_GTCIA3
__interrupt void Interrupt_GTCIA3(void)
#endif
{
  /* Call the user function */
  if (rpdl_GPT_GTCIA_callback_func[3] != PDL_NO_FUNC)
  {
    rpdl_GPT_GTCIA_callback_func[3]();
  }
}

/*""FUNC COMMENT""*************************************************************
* Module outline : Timer interrupt processing
*------------------------------------------------------------------------------
* Declaration  : void Interrupt_GTCIBn(void)
*------------------------------------------------------------------------------
* Function    : Interrupt processing for B events
*------------------------------------------------------------------------------
* Argument    : Nothing
*------------------------------------------------------------------------------
* Return value  : Nothing
*------------------------------------------------------------------------------
* Input      :
* Output    : 
*------------------------------------------------------------------------------
* Use function  : rpdl_GPT_CMICB_callback_func[n]
*------------------------------------------------------------------------------
* Notes      :
*------------------------------------------------------------------------------
* History    : 2012.09.11
*        : Ver 1.00
*        : CS release.
*""FUNC COMMENT END""*********************************************************/

#if FAST_INTC_VECTOR == VECT_GPT0_GTCIB0
__fast_interrupt void Interrupt_GTCIB0(void)
#else
#pragma vector = VECT_GPT0_GTCIB0
__interrupt void Interrupt_GTCIB0(void)
#endif
{
  /* Call the user function */
  if (rpdl_GPT_GTCIB_callback_func[0] != PDL_NO_FUNC)
  {
    rpdl_GPT_GTCIB_callback_func[0]();
  }
}

#if FAST_INTC_VECTOR == VECT_GPT1_GTCIB1
__fast_interrupt void Interrupt_GTCIB1(void)
#else
#pragma vector = VECT_GPT1_GTCIB1
__interrupt void Interrupt_GTCIB1(void)
#endif
{
  /* Call the user function */
  if (rpdl_GPT_GTCIB_callback_func[1] != PDL_NO_FUNC)
  {
    rpdl_GPT_GTCIB_callback_func[1]();
  }
}

#if FAST_INTC_VECTOR == VECT_GPT2_GTCIB2
__fast_interrupt void Interrupt_GTCIB2(void)
#else
#pragma vector = VECT_GPT2_GTCIB2
__interrupt void Interrupt_GTCIB2(void)
#endif
{
  /* Call the user function */
  if (rpdl_GPT_GTCIB_callback_func[2] != PDL_NO_FUNC)
  {
    rpdl_GPT_GTCIB_callback_func[2]();
  }
}

#if FAST_INTC_VECTOR == VECT_GPT3_GTCIB3
__fast_interrupt void Interrupt_GTCIB3(void)
#else
#pragma vector = VECT_GPT3_GTCIB3
__interrupt void Interrupt_GTCIB3(void)
#endif
{
  /* Call the user function */
  if (rpdl_GPT_GTCIB_callback_func[3] != PDL_NO_FUNC)
  {
    rpdl_GPT_GTCIB_callback_func[3]();
  }
}

/*""FUNC COMMENT""*************************************************************
* Module outline : Timer interrupt processing
*------------------------------------------------------------------------------
* Declaration  : void Interrupt_GTCICn(void)
*------------------------------------------------------------------------------
* Function    : Interrupt processing for C / D / dead-time events
*------------------------------------------------------------------------------
* Argument    : Nothing
*------------------------------------------------------------------------------
* Return value  : Nothing
*------------------------------------------------------------------------------
* Input      :
* Output    : 
*------------------------------------------------------------------------------
* Use function  : rpdl_GPT_GTCIC_callback_func[n]
*------------------------------------------------------------------------------
* Notes      :
*------------------------------------------------------------------------------
* History    : 2012.09.11
*        : Ver 1.00
*        : CS release.
*""FUNC COMMENT END""*********************************************************/

#if FAST_INTC_VECTOR == VECT_GPT0_GTCIC0
__fast_interrupt void Interrupt_GTCIC0(void)
#else
#pragma vector = VECT_GPT0_GTCIC0
__interrupt void Interrupt_GTCIC0(void)
#endif
{
  /* Call the user functions */
  if (rpdl_GPT_GTCIC_callback_func[0] != PDL_NO_FUNC)
  {
    rpdl_GPT_GTCIC_callback_func[0]();
  }
}

#if FAST_INTC_VECTOR == VECT_GPT1_GTCIC1
__fast_interrupt void Interrupt_GTCIC1(void)
#else
#pragma vector = VECT_GPT1_GTCIC1
__interrupt void Interrupt_GTCIC1(void)
#endif
{
  /* Call the user functions */
  if (rpdl_GPT_GTCIC_callback_func[1] != PDL_NO_FUNC)
  {
    rpdl_GPT_GTCIC_callback_func[1]();
  }
}

#if FAST_INTC_VECTOR == VECT_GPT2_GTCIC2
__fast_interrupt void Interrupt_GTCIC2(void)
#else
#pragma vector = VECT_GPT2_GTCIC2
__interrupt void Interrupt_GTCIC2(void)
#endif
{
  /* Call the user functions */
  if (rpdl_GPT_GTCIC_callback_func[2] != PDL_NO_FUNC)
  {
    rpdl_GPT_GTCIC_callback_func[2]();
  }
}

#if FAST_INTC_VECTOR == VECT_GPT3_GTCIC3
__fast_interrupt void Interrupt_GTCIC3(void)
#else
#pragma vector = VECT_GPT3_GTCIC3
__interrupt void Interrupt_GTCIC3(void)
#endif
{
  /* Call the user functions */
  if (rpdl_GPT_GTCIC_callback_func[3] != PDL_NO_FUNC)
  {
    rpdl_GPT_GTCIC_callback_func[3]();
  }
}

/*""FUNC COMMENT""*************************************************************
* Module outline : Timer interrupt processing
*------------------------------------------------------------------------------
* Declaration  : void Interrupt_GTCIEn(void)
*------------------------------------------------------------------------------
* Function    : Interrupt processing for E / F events
*------------------------------------------------------------------------------
* Argument    : Nothing
*------------------------------------------------------------------------------
* Return value  : Nothing
*------------------------------------------------------------------------------
* Input      :
* Output    : 
*------------------------------------------------------------------------------
* Use function  : rpdl_GPT_GTCIE_callback_func[n]
*------------------------------------------------------------------------------
* Notes      :
*------------------------------------------------------------------------------
* History    : 2012.09.11
*        : Ver 1.00
*        : CS release.
*""FUNC COMMENT END""*********************************************************/

#if FAST_INTC_VECTOR == VECT_GPT0_GTCIE0
__fast_interrupt void Interrupt_GTCIE0(void)
#else
#pragma vector = VECT_GPT0_GTCIE0
__interrupt void Interrupt_GTCIE0(void)
#endif
{
  /* Call the user functions */
  if (rpdl_GPT_GTCIE_callback_func[0] != PDL_NO_FUNC)
  {
    rpdl_GPT_GTCIE_callback_func[0]();
  }
}

#if FAST_INTC_VECTOR == VECT_GPT1_GTCIE1
__fast_interrupt void Interrupt_GTCIE1(void)
#else
#pragma vector = VECT_GPT1_GTCIE1
__interrupt void Interrupt_GTCIE1(void)
#endif
{
  /* Call the user functions */
  if (rpdl_GPT_GTCIE_callback_func[1] != PDL_NO_FUNC)
  {
    rpdl_GPT_GTCIE_callback_func[1]();
  }
}

#if FAST_INTC_VECTOR == VECT_GPT2_GTCIE2
__fast_interrupt void Interrupt_GTCIE2(void)
#else
#pragma vector = VECT_GPT2_GTCIE2
__interrupt void Interrupt_GTCIE2(void)
#endif
{
  /* Call the user functions */
  if (rpdl_GPT_GTCIE_callback_func[2] != PDL_NO_FUNC)
  {
    rpdl_GPT_GTCIE_callback_func[2]();
  }
}

#if FAST_INTC_VECTOR == VECT_GPT3_GTCIE3
__fast_interrupt void Interrupt_GTCIE3(void)
#else
#pragma vector = VECT_GPT3_GTCIE3
__interrupt void Interrupt_GTCIE3(void)
#endif
{
  /* Call the user functions */
  if (rpdl_GPT_GTCIE_callback_func[3] != PDL_NO_FUNC)
  {
    rpdl_GPT_GTCIE_callback_func[3]();
  }
}

/*""FUNC COMMENT""*************************************************************
* Module outline : Timer interrupt processing
*------------------------------------------------------------------------------
* Declaration  : void Interrupt_GTCIVn(void)
*------------------------------------------------------------------------------
* Function    : Interrupt processing for over / underflow events
*------------------------------------------------------------------------------
* Argument    : Nothing
*------------------------------------------------------------------------------
* Return value  : Nothing
*------------------------------------------------------------------------------
* Input      :
* Output    : 
*------------------------------------------------------------------------------
* Use function  : rpdl_GPT_GTCIV_callback_func[n]
*------------------------------------------------------------------------------
* Notes      :
*------------------------------------------------------------------------------
* History    : 2012.09.11
*        : Ver 1.00
*        : CS release.
*""FUNC COMMENT END""*********************************************************/

#if FAST_INTC_VECTOR == VECT_GPT0_GTCIV0
__fast_interrupt void Interrupt_GTCIV0(void)
#else
#pragma vector = VECT_GPT0_GTCIV0
__interrupt void Interrupt_GTCIV0(void)
#endif
{
  /* Call the user function */
  if (rpdl_GPT_GTCIV_callback_func[0] != PDL_NO_FUNC)
  {
    rpdl_GPT_GTCIV_callback_func[0]();
  }
}

#if FAST_INTC_VECTOR == VECT_GPT1_GTCIV1
__fast_interrupt void Interrupt_GTCIV1(void)
#else
#pragma vector = VECT_GPT1_GTCIV1
__interrupt void Interrupt_GTCIV1(void)
#endif
{
  /* Call the user function */
  if (rpdl_GPT_GTCIV_callback_func[1] != PDL_NO_FUNC)
  {
    rpdl_GPT_GTCIV_callback_func[1]();
  }
}

#if FAST_INTC_VECTOR == VECT_GPT2_GTCIV2
__fast_interrupt void Interrupt_GTCIV2(void)
#else
#pragma vector = VECT_GPT2_GTCIV2
__interrupt void Interrupt_GTCIV2(void)
#endif
{
  /* Call the user function */
  if (rpdl_GPT_GTCIV_callback_func[2] != PDL_NO_FUNC)
  {
    rpdl_GPT_GTCIV_callback_func[2]();
  }
}

#if FAST_INTC_VECTOR == VECT_GPT3_GTCIV3
__fast_interrupt void Interrupt_GTCIV3(void)
#else
#pragma vector = VECT_GPT3_GTCIV3
__interrupt void Interrupt_GTCIV3(void)
#endif
{
  /* Call the user function */
  if (rpdl_GPT_GTCIV_callback_func[3] != PDL_NO_FUNC)
  {
    rpdl_GPT_GTCIV_callback_func[3]();
  }
}

/*""FUNC COMMENT""*************************************************************
* Module outline : Timer interrupt processing
*------------------------------------------------------------------------------
* Declaration  : void Interrupt_LOCO1(void)
*------------------------------------------------------------------------------
* Function    : Interrupt processing for LOCO events
*------------------------------------------------------------------------------
* Argument    : Nothing
*------------------------------------------------------------------------------
* Return value  : Nothing
*------------------------------------------------------------------------------
* Input      :
* Output    : 
*------------------------------------------------------------------------------
* Use function  : rpdl_GPT_LOCO_callback_func
*------------------------------------------------------------------------------
* Notes      :
*------------------------------------------------------------------------------
* History    : 2012.09.11
*        : Ver 1.00
*        : CS release.
*""FUNC COMMENT END""*********************************************************/

#if FAST_INTC_VECTOR == VECT_GPT0_LOCO1
__fast_interrupt void Interrupt_LOCO1(void)
#else
#pragma vector = VECT_GPT0_LOCO1
__interrupt void Interrupt_LOCO1(void)
#endif
{
  /* Call the user function */
  if (rpdl_GPT_LOCO_callback_func != PDL_NO_FUNC)
  {
    rpdl_GPT_LOCO_callback_func();
  }
}

/* End of file */
