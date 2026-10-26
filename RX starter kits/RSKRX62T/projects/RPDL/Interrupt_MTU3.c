/* Adapted for use with IAR Embedded Workbench */
/*""FILE COMMENT""*******************************************************
* System Name  : Interrupt program for RX62Txx
* File Name    : Interrupt_MTU3.c
* Version    : 1.01
* Contents    : Interrupt handlers for the MTU channels
* Customer    : 
* Model      : 
* Order      : 
* CPU      : RX
* Compiler    : RXC
* OS      : Nothing
* Programmer  : 
* Note      : 
************************************************************************
* Copyright, 2011. Renesas Electronics Corporation
* and Renesas Solutions Corporation
************************************************************************
* History    : 2011.05.04
*        : Ver 1.01
*        : Release CS3.
*""FILE COMMENT END""**************************************************/

#include "r_pdl_mtu3.h"
#include "r_pdl_definitions.h"
#include "r_pdl_user_definitions.h"

/*""FUNC COMMENT""***************************************************
* Module outline: Timer interrupt processing
*-------------------------------------------------------------------
* Declaration  : void Interrupt_MTUn_TGIA0(void)
*-------------------------------------------------------------------
* Function    : TGIAn interrupt processing
*-------------------------------------------------------------------
* Argument    : Nothing
*-------------------------------------------------------------------
* Return value  : Nothing
*-------------------------------------------------------------------
* Input      : 
* Output    : 
*-------------------------------------------------------------------
* Use function  : rpdl_MTU3_func1_callback_func[n]
*-------------------------------------------------------------------
* Notes      : 
*-------------------------------------------------------------------
* History    : 2011.05.04
*        : Ver 1.01
*        : Release CS3.
*""FUNC COMMENT END""**********************************************/

#if FAST_INTC_VECTOR == VECT_MTU0_TGIA0
__fast_interrupt void Interrupt_MTU0_TGIA0(void)
#else
#pragma vector = VECT_MTU0_TGIA0
__interrupt void Interrupt_MTU0_TGIA0(void)
#endif
{
  /* Call the user function */
  if (rpdl_MTU3_func1_callback_func[0] != PDL_NO_FUNC)
  {
    rpdl_MTU3_func1_callback_func[0]();
  }
}

#if FAST_INTC_VECTOR == VECT_MTU1_TGIA1
__fast_interrupt void Interrupt_MTU1_TGIA1(void)
#else
#pragma vector = VECT_MTU1_TGIA1
__interrupt void Interrupt_MTU1_TGIA1(void)
#endif
{
  /* Call the user function */
  if (rpdl_MTU3_func1_callback_func[1] != PDL_NO_FUNC)
  {
    rpdl_MTU3_func1_callback_func[1]();
  }
}

#if FAST_INTC_VECTOR == VECT_MTU2_TGIA2
__fast_interrupt void Interrupt_MTU2_TGIA2(void)
#else
#pragma vector = VECT_MTU2_TGIA2
__interrupt void Interrupt_MTU2_TGIA2(void)
#endif
{
  /* Call the user function */
  if (rpdl_MTU3_func1_callback_func[2] != PDL_NO_FUNC)
  {
    rpdl_MTU3_func1_callback_func[2]();
  }
}

#if FAST_INTC_VECTOR == VECT_MTU3_TGIA3
__fast_interrupt void Interrupt_MTU3_TGIA3(void)
#else
#pragma vector = VECT_MTU3_TGIA3
__interrupt void Interrupt_MTU3_TGIA3(void)
#endif
{
  /* Call the user function */
  if (rpdl_MTU3_func1_callback_func[3] != PDL_NO_FUNC)
  {
    rpdl_MTU3_func1_callback_func[3]();
  }
}

#if FAST_INTC_VECTOR == VECT_MTU4_TGIA4
__fast_interrupt void Interrupt_MTU4_TGIA4(void)
#else
#pragma vector = VECT_MTU4_TGIA4
__interrupt void Interrupt_MTU4_TGIA4(void)
#endif
{
  /* Call the user function */
  if (rpdl_MTU3_func1_callback_func[4] != PDL_NO_FUNC)
  {
    rpdl_MTU3_func1_callback_func[4]();
  }
}

#if FAST_INTC_VECTOR == VECT_MTU6_TGIA6
__fast_interrupt void Interrupt_MTU6_TGIA6(void)
#else
#pragma vector = VECT_MTU6_TGIA6
__interrupt void Interrupt_MTU6_TGIA6(void)
#endif
{
  /* Call the user function */
  if (rpdl_MTU3_func1_callback_func[6] != PDL_NO_FUNC)
  {
    rpdl_MTU3_func1_callback_func[6]();
  }
}

#if FAST_INTC_VECTOR == VECT_MTU7_TGIA7
__fast_interrupt void Interrupt_MTU7_TGIA7(void)
#else
#pragma vector = VECT_MTU7_TGIA7
__interrupt void Interrupt_MTU7_TGIA7(void)
#endif
{
  /* Call the user function */
  if (rpdl_MTU3_func1_callback_func[7] != PDL_NO_FUNC)
  {
    rpdl_MTU3_func1_callback_func[7]();
  }
}

/*""FUNC COMMENT""***************************************************
* Module outline: Timer interrupt processing
*-------------------------------------------------------------------
* Declaration  : void Interrupt_MTUn_TGIBn(void)
*-------------------------------------------------------------------
* Function    : TGIBn interrupt processing
*-------------------------------------------------------------------
* Argument    : Nothing
*-------------------------------------------------------------------
* Return value  : Nothing
*-------------------------------------------------------------------
* Input      : 
* Output    : 
*-------------------------------------------------------------------
* Use function  : rpdl_MTU3_func2_callback_func[n]
*-------------------------------------------------------------------
* Notes      : 
*-------------------------------------------------------------------
* History    : 2011.05.04
*        : Ver 1.01
*        : Release CS3.
*""FUNC COMMENT END""**********************************************/

#if FAST_INTC_VECTOR == VECT_MTU0_TGIB0
__fast_interrupt void Interrupt_MTU0_TGIB0(void)
#else
#pragma vector = VECT_MTU0_TGIB0
__interrupt void Interrupt_MTU0_TGIB0(void)
#endif
{
  /* Call the user function */
  if (rpdl_MTU3_func2_callback_func[0] != PDL_NO_FUNC)
  {
    rpdl_MTU3_func2_callback_func[0]();
  }
}

#if FAST_INTC_VECTOR == VECT_MTU1_TGIB1
__fast_interrupt void Interrupt_MTU1_TGIB1(void)
#else
#pragma vector = VECT_MTU1_TGIB1
__interrupt void Interrupt_MTU1_TGIB1(void)
#endif
{
  /* Call the user function */
  if (rpdl_MTU3_func2_callback_func[1] != PDL_NO_FUNC)
  {
    rpdl_MTU3_func2_callback_func[1]();
  }
}

#if FAST_INTC_VECTOR == VECT_MTU2_TGIB2
__fast_interrupt void Interrupt_MTU2_TGIB2(void)
#else
#pragma vector = VECT_MTU2_TGIB2
__interrupt void Interrupt_MTU2_TGIB2(void)
#endif
{
  /* Call the user function */
  if (rpdl_MTU3_func2_callback_func[2] != PDL_NO_FUNC)
  {
    rpdl_MTU3_func2_callback_func[2]();
  }
}

#if FAST_INTC_VECTOR == VECT_MTU3_TGIB3
__fast_interrupt void Interrupt_MTU3_TGIB3(void)
#else
#pragma vector = VECT_MTU3_TGIB3
__interrupt void Interrupt_MTU3_TGIB3(void)
#endif
{
  /* Call the user function */
  if (rpdl_MTU3_func2_callback_func[3] != PDL_NO_FUNC)
  {
    rpdl_MTU3_func2_callback_func[3]();
  }
}

#if FAST_INTC_VECTOR == VECT_MTU4_TGIB4
__fast_interrupt void Interrupt_MTU4_TGIB4(void)
#else
#pragma vector = VECT_MTU4_TGIB4
__interrupt void Interrupt_MTU4_TGIB4(void)
#endif
{
  /* Call the user function */
  if (rpdl_MTU3_func2_callback_func[4] != PDL_NO_FUNC)
  {
    rpdl_MTU3_func2_callback_func[4]();
  }
}

#if FAST_INTC_VECTOR == VECT_MTU6_TGIB6
__fast_interrupt void Interrupt_MTU6_TGIB6(void)
#else
#pragma vector = VECT_MTU6_TGIB6
__interrupt void Interrupt_MTU6_TGIB6(void)
#endif
{
  /* Call the user function */
  if (rpdl_MTU3_func2_callback_func[6] != PDL_NO_FUNC)
  {
    rpdl_MTU3_func2_callback_func[6]();
  }
}

#if FAST_INTC_VECTOR == VECT_MTU7_TGIB7
__fast_interrupt void Interrupt_MTU7_TGIB7(void)
#else
#pragma vector = VECT_MTU7_TGIB7
__interrupt void Interrupt_MTU7_TGIB7(void)
#endif
{
  /* Call the user function */
  if (rpdl_MTU3_func2_callback_func[7] != PDL_NO_FUNC)
  {
    rpdl_MTU3_func2_callback_func[7]();
  }
}

/*""FUNC COMMENT""***************************************************
* Module outline: Timer interrupt processing
*-------------------------------------------------------------------
* Declaration  : void Interrupt_MTUn_TGICn(void)
*-------------------------------------------------------------------
* Function    : TGICn interrupt processing
*-------------------------------------------------------------------
* Argument    : Nothing
*-------------------------------------------------------------------
* Return value  : Nothing
*-------------------------------------------------------------------
* Input      : 
* Output    : 
*-------------------------------------------------------------------
* Use function  : rpdl_MTU3_func3_callback_func[n]
*-------------------------------------------------------------------
* Notes      : 
*-------------------------------------------------------------------
* History    : 2011.05.04
*        : Ver 1.01
*        : Release CS3.
*""FUNC COMMENT END""**********************************************/

#if FAST_INTC_VECTOR == VECT_MTU0_TGIC0
__fast_interrupt void Interrupt_MTU0_TGIC0(void)
#else
#pragma vector = VECT_MTU0_TGIC0
__interrupt void Interrupt_MTU0_TGIC0(void)
#endif
{
  /* Call the user function */
  if (rpdl_MTU3_func3_callback_func[0] != PDL_NO_FUNC)
  {
    rpdl_MTU3_func3_callback_func[0]();
  }
}

#if FAST_INTC_VECTOR == VECT_MTU3_TGIC3
__fast_interrupt void Interrupt_MTU3_TGIC3(void)
#else
#pragma vector = VECT_MTU3_TGIC3
__interrupt void Interrupt_MTU3_TGIC3(void)
#endif
{
  /* Call the user function */
  if (rpdl_MTU3_func3_callback_func[3] != PDL_NO_FUNC)
  {
    rpdl_MTU3_func3_callback_func[3]();
  }
}

#if FAST_INTC_VECTOR == VECT_MTU4_TGIC4
__fast_interrupt void Interrupt_MTU4_TGIC4(void)
#else
#pragma vector = VECT_MTU4_TGIC4
__interrupt void Interrupt_MTU4_TGIC4(void)
#endif
{
  /* Call the user function */
  if (rpdl_MTU3_func3_callback_func[4] != PDL_NO_FUNC)
  {
    rpdl_MTU3_func3_callback_func[4]();
  }
}

#if FAST_INTC_VECTOR == VECT_MTU6_TGIC6
__fast_interrupt void Interrupt_MTU6_TGIC6(void)
#else
#pragma vector = VECT_MTU6_TGIC6
__interrupt void Interrupt_MTU6_TGIC6(void)
#endif
{
  /* Call the user function */
  if (rpdl_MTU3_func3_callback_func[6] != PDL_NO_FUNC)
  {
    rpdl_MTU3_func3_callback_func[6]();
  }
}

#if FAST_INTC_VECTOR == VECT_MTU7_TGIC7
__fast_interrupt void Interrupt_MTU7_TGIC7(void)
#else
#pragma vector = VECT_MTU7_TGIC7
__interrupt void Interrupt_MTU7_TGIC7(void)
#endif
{
  /* Call the user function */
  if (rpdl_MTU3_func3_callback_func[7] != PDL_NO_FUNC)
  {
    rpdl_MTU3_func3_callback_func[7]();
  }
}

/*""FUNC COMMENT""***************************************************
* Module outline: Timer interrupt processing
*-------------------------------------------------------------------
* Declaration  : void Interrupt_MTUn_TGIDn(void)
*-------------------------------------------------------------------
* Function    : TGIDn interrupt processing
*-------------------------------------------------------------------
* Argument    : Nothing
*-------------------------------------------------------------------
* Return value  : Nothing
*-------------------------------------------------------------------
* Input      : 
* Output    : 
*-------------------------------------------------------------------
* Use function  : rpdl_MTU3_func4_callback_func[n]
*-------------------------------------------------------------------
* Notes      : 
*-------------------------------------------------------------------
* History    : 2011.05.04
*        : Ver 1.01
*        : Release CS3.
*""FUNC COMMENT END""**********************************************/

#if FAST_INTC_VECTOR == VECT_MTU0_TGID0
__fast_interrupt void Interrupt_MTU0_TGID0(void)
#else
#pragma vector = VECT_MTU0_TGID0
__interrupt void Interrupt_MTU0_TGID0(void)
#endif
{
  /* Call the user function */
  if (rpdl_MTU3_func4_callback_func[0] != PDL_NO_FUNC)
  {
    rpdl_MTU3_func4_callback_func[0]();
  }
}

#if FAST_INTC_VECTOR == VECT_MTU3_TGID3
__fast_interrupt void Interrupt_MTU3_TGID3(void)
#else
#pragma vector = VECT_MTU3_TGID3
__interrupt void Interrupt_MTU3_TGID3(void)
#endif
{
  /* Call the user function */
  if (rpdl_MTU3_func4_callback_func[3] != PDL_NO_FUNC)
  {
    rpdl_MTU3_func4_callback_func[3]();
  }
}

#if FAST_INTC_VECTOR == VECT_MTU4_TGID4
__fast_interrupt void Interrupt_MTU4_TGID4(void)
#else
#pragma vector = VECT_MTU4_TGID4
__interrupt void Interrupt_MTU4_TGID4(void)
#endif
{
  /* Call the user function */
  if (rpdl_MTU3_func4_callback_func[4] != PDL_NO_FUNC)
  {
    rpdl_MTU3_func4_callback_func[4]();
  }
}

#if FAST_INTC_VECTOR == VECT_MTU6_TGID6
__fast_interrupt void Interrupt_MTU6_TGID6(void)
#else
#pragma vector = VECT_MTU6_TGID6
__interrupt void Interrupt_MTU6_TGID6(void)
#endif
{
  /* Call the user function */
  if (rpdl_MTU3_func4_callback_func[6] != PDL_NO_FUNC)
  {
    rpdl_MTU3_func4_callback_func[6]();
  }
}

#if FAST_INTC_VECTOR == VECT_MTU7_TGID7
__fast_interrupt void Interrupt_MTU7_TGID7(void)
#else
#pragma vector = VECT_MTU7_TGID7
__interrupt void Interrupt_MTU7_TGID7(void)
#endif
{
  /* Call the user function */
  if (rpdl_MTU3_func4_callback_func[7] != PDL_NO_FUNC)
  {
    rpdl_MTU3_func4_callback_func[7]();
  }
}

/*""FUNC COMMENT""***************************************************
* Module outline: Timer interrupt processing
*-------------------------------------------------------------------
* Declaration  : void Interrupt_MTUn_TGIEn(void)
*-------------------------------------------------------------------
* Function    : TGIEn interrupt processing
*-------------------------------------------------------------------
* Argument    : Nothing
*-------------------------------------------------------------------
* Return value  : Nothing
*-------------------------------------------------------------------
* Input      : 
* Output    : 
*-------------------------------------------------------------------
* Use function  : rpdl_MTU3_TGRE_callback_func
*-------------------------------------------------------------------
* Notes      : 
*-------------------------------------------------------------------
* History    : 2011.05.04
*        : Ver 1.01
*        : Release CS3.
*""FUNC COMMENT END""**********************************************/

#if FAST_INTC_VECTOR == VECT_MTU0_TGIE0
__fast_interrupt void Interrupt_MTU0_TGIE0(void)
#else
#pragma vector = VECT_MTU0_TGIE0
__interrupt void Interrupt_MTU0_TGIE0(void)
#endif
{
  /* Call the user function */
  if (rpdl_MTU3_TGRE_callback_func != PDL_NO_FUNC)
  {
    rpdl_MTU3_TGRE_callback_func();
  }
}

/*""FUNC COMMENT""***************************************************
* Module outline: Timer interrupt processing
*-------------------------------------------------------------------
* Declaration  : void Interrupt_MTUn_TGIFn(void)
*-------------------------------------------------------------------
* Function    : TGIFn interrupt processing
*-------------------------------------------------------------------
* Argument    : Nothing
*-------------------------------------------------------------------
* Return value  : Nothing
*-------------------------------------------------------------------
* Input      : 
* Output    : 
*-------------------------------------------------------------------
* Use function  : rpdl_MTU3_TGRF_callback_func
*-------------------------------------------------------------------
* Notes      : 
*-------------------------------------------------------------------
* History    : 2011.05.04
*        : Ver 1.01
*        : Release CS3.
*""FUNC COMMENT END""**********************************************/

#if FAST_INTC_VECTOR == VECT_MTU0_TGIF0
__fast_interrupt void Interrupt_MTU0_TGIF0(void)
#else
#pragma vector = VECT_MTU0_TGIF0
__interrupt void Interrupt_MTU0_TGIF0(void)
#endif
{
  /* Call the user function */
  if (rpdl_MTU3_TGRF_callback_func != PDL_NO_FUNC)
  {
    rpdl_MTU3_TGRF_callback_func();
  }
}

/*""FUNC COMMENT""***************************************************
* Module outline: Timer interrupt processing
*-------------------------------------------------------------------
* Declaration  : void Interrupt_MTUn_TCIVn(void)
*-------------------------------------------------------------------
* Function    : TCIVn interrupt processing
*-------------------------------------------------------------------
* Argument    : Nothing
*-------------------------------------------------------------------
* Return value  : Nothing
*-------------------------------------------------------------------
* Input      : 
* Output    : 
*-------------------------------------------------------------------
* Use function  : rpdl_MTU3_Overflow_callback_func[n]
*-------------------------------------------------------------------
* Notes      : 
*-------------------------------------------------------------------
* History    : 2011.05.04
*        : Ver 1.01
*        : Release CS3.
*""FUNC COMMENT END""**********************************************/

#if FAST_INTC_VECTOR == VECT_MTU0_TCIV0
__fast_interrupt void Interrupt_MTU0_TCIV0(void)
#else
#pragma vector = VECT_MTU0_TCIV0
__interrupt void Interrupt_MTU0_TCIV0(void)
#endif
{
  /* Call the user function */
  if (rpdl_MTU3_Overflow_callback_func[0] != PDL_NO_FUNC)
  {
    rpdl_MTU3_Overflow_callback_func[0]();
  }
}

#if FAST_INTC_VECTOR == VECT_MTU1_TCIV1
__fast_interrupt void Interrupt_MTU1_TCIV1(void)
#else
#pragma vector = VECT_MTU1_TCIV1
__interrupt void Interrupt_MTU1_TCIV1(void)
#endif
{
  /* Call the user function */
  if (rpdl_MTU3_Overflow_callback_func[1] != PDL_NO_FUNC)
  {
    rpdl_MTU3_Overflow_callback_func[1]();
  }
}

#if FAST_INTC_VECTOR == VECT_MTU2_TCIV2
__fast_interrupt void Interrupt_MTU2_TCIV2(void)
#else
#pragma vector = VECT_MTU2_TCIV2
__interrupt void Interrupt_MTU2_TCIV2(void)
#endif
{
  /* Call the user function */
  if (rpdl_MTU3_Overflow_callback_func[2] != PDL_NO_FUNC)
  {
    rpdl_MTU3_Overflow_callback_func[2]();
  }
}

#if FAST_INTC_VECTOR == VECT_MTU3_TCIV3
__fast_interrupt void Interrupt_MTU3_TCIV3(void)
#else
#pragma vector = VECT_MTU3_TCIV3
__interrupt void Interrupt_MTU3_TCIV3(void)
#endif
{
  /* Call the user function */
  if (rpdl_MTU3_Overflow_callback_func[3] != PDL_NO_FUNC)
  {
    rpdl_MTU3_Overflow_callback_func[3]();
  }
}

#if FAST_INTC_VECTOR == VECT_MTU4_TCIV4
__fast_interrupt void Interrupt_MTU4_TCIV4(void)
#else
#pragma vector = VECT_MTU4_TCIV4
__interrupt void Interrupt_MTU4_TCIV4(void)
#endif
{
  /* Call the user function */
  if (rpdl_MTU3_Overflow_callback_func[4] != PDL_NO_FUNC)
  {
    rpdl_MTU3_Overflow_callback_func[4]();
  }
}

#if FAST_INTC_VECTOR == VECT_MTU6_TCIV6
__fast_interrupt void Interrupt_MTU6_TCIV6(void)
#else
#pragma vector = VECT_MTU6_TCIV6
__interrupt void Interrupt_MTU6_TCIV6(void)
#endif
{
  /* Call the user function */
  if (rpdl_MTU3_Overflow_callback_func[6] != PDL_NO_FUNC)
  {
    rpdl_MTU3_Overflow_callback_func[6]();
  }
}

#if FAST_INTC_VECTOR == VECT_MTU7_TCIV7
__fast_interrupt void Interrupt_MTU7_TCIV7(void)
#else
#pragma vector = VECT_MTU7_TCIV7
__interrupt void Interrupt_MTU7_TCIV7(void)
#endif
{
  /* Call the user function */
  if (rpdl_MTU3_Overflow_callback_func[7] != PDL_NO_FUNC)
  {
    rpdl_MTU3_Overflow_callback_func[7]();
  }
}

/*""FUNC COMMENT""***************************************************
* Module outline: Timer interrupt processing
*-------------------------------------------------------------------
* Declaration  : void Interrupt_MTUn_TCIUn(void)
*-------------------------------------------------------------------
* Function    : TCIUn interrupt processing
*-------------------------------------------------------------------
* Argument    : Nothing
*-------------------------------------------------------------------
* Return value  : Nothing
*-------------------------------------------------------------------
* Input      : 
* Output    : 
*-------------------------------------------------------------------
* Use function  : rpdl_MTU3_Underflow_callback_func[n]
*-------------------------------------------------------------------
* Notes      : 
*-------------------------------------------------------------------
* History    : 2011.05.04
*        : Ver 1.01
*        : Release CS3.
*""FUNC COMMENT END""**********************************************/

#if FAST_INTC_VECTOR == VECT_MTU1_TCIU1
__fast_interrupt void Interrupt_MTU1_TCIU1(void)
#else
#pragma vector = VECT_MTU1_TCIU1
__interrupt void Interrupt_MTU1_TCIU1(void)
#endif
{
  /* Call the user function */
  if (rpdl_MTU3_Underflow_callback_func[1] != PDL_NO_FUNC)
  {
    rpdl_MTU3_Underflow_callback_func[1]();
  }
}

#if FAST_INTC_VECTOR == VECT_MTU2_TCIU2
__fast_interrupt void Interrupt_MTU2_TCIU2(void)
#else
#pragma vector = VECT_MTU2_TCIU2
__interrupt void Interrupt_MTU2_TCIU2(void)
#endif
{
  /* Call the user function */
  if (rpdl_MTU3_Underflow_callback_func[2] != PDL_NO_FUNC)
  {
    rpdl_MTU3_Underflow_callback_func[2]();
  }
}

/*""FUNC COMMENT""***************************************************
* Module outline: Timer interrupt processing
*-------------------------------------------------------------------
* Declaration  : void Interrupt_MTUn_TGIUn(void)
*-------------------------------------------------------------------
* Function    : TGIUn interrupt processing
*-------------------------------------------------------------------
* Argument    : Nothing
*-------------------------------------------------------------------
* Return value  : Nothing
*-------------------------------------------------------------------
* Input      : 
* Output    : 
*-------------------------------------------------------------------
* Use function  : rpdl_MTU3_func1_callback_func[n]
*-------------------------------------------------------------------
* Notes      : 
*-------------------------------------------------------------------
* History    : 2011.05.04
*        : Ver 1.01
*        : Release CS3.
*""FUNC COMMENT END""**********************************************/

#if FAST_INTC_VECTOR == VECT_MTU5_TGIU5
__fast_interrupt void Interrupt_MTU5_TGIU5(void)
#else
#pragma vector = VECT_MTU5_TGIU5
__interrupt void Interrupt_MTU5_TGIU5(void)
#endif
{
  /* Call the user function */
  if (rpdl_MTU3_func1_callback_func[5] != PDL_NO_FUNC)
  {
    rpdl_MTU3_func1_callback_func[5]();
  }
}

/*""FUNC COMMENT""***************************************************
* Module outline: Timer interrupt processing
*-------------------------------------------------------------------
* Declaration  : void Interrupt_MTUn_TGIVn(void)
*-------------------------------------------------------------------
* Function    : TGIVn interrupt processing
*-------------------------------------------------------------------
* Argument    : Nothing
*-------------------------------------------------------------------
* Return value  : Nothing
*-------------------------------------------------------------------
* Input      : 
* Output    : 
*-------------------------------------------------------------------
* Use function  : rpdl_MTU3_func2_callback_func[n]
*-------------------------------------------------------------------
* Notes      : 
*-------------------------------------------------------------------
* History    : 2011.05.04
*        : Ver 1.01
*        : Release CS3.
*""FUNC COMMENT END""**********************************************/

#if FAST_INTC_VECTOR == VECT_MTU5_TGIV5
__fast_interrupt void Interrupt_MTU5_TGIV5(void)
#else
#pragma vector = VECT_MTU5_TGIV5
__interrupt void Interrupt_MTU5_TGIV5(void)
#endif
{
  /* Call the user function */
  if (rpdl_MTU3_func2_callback_func[5] != PDL_NO_FUNC)
  {
    rpdl_MTU3_func2_callback_func[5]();
  }
}

/*""FUNC COMMENT""***************************************************
* Module outline: Timer interrupt processing
*-------------------------------------------------------------------
* Declaration  : void Interrupt_MTUn_TGIWn(void)
*-------------------------------------------------------------------
* Function    : TGIWn interrupt processing
*-------------------------------------------------------------------
* Argument    : Nothing
*-------------------------------------------------------------------
* Return value  : Nothing
*-------------------------------------------------------------------
* Input      : 
* Output    : 
*-------------------------------------------------------------------
* Use function  : rpdl_MTU3_func3_callback_func[n]
*-------------------------------------------------------------------
* Notes      : 
*-------------------------------------------------------------------
* History    : 2011.05.04
*        : Ver 1.01
*        : Release CS3.
*""FUNC COMMENT END""**********************************************/

#if FAST_INTC_VECTOR == VECT_MTU5_TGIW5
__fast_interrupt void Interrupt_MTU5_TGIW5(void)
#else
#pragma vector = VECT_MTU5_TGIW5
__interrupt void Interrupt_MTU5_TGIW5(void)
#endif
{
  /* Call the user function */
  if (rpdl_MTU3_func3_callback_func[5] != PDL_NO_FUNC)
  {
    rpdl_MTU3_func3_callback_func[5]();
  }
}

/* End of file */
