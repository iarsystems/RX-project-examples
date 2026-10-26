/* Adapted for use with IAR Embedded Workbench */
/*""FILE COMMENT""*******************************************************
* System Name  : Interrupt program for RX610xV
* File Name    : Interrupt_TPU.c
* Version    : 0.10
* Contents    : Interrupt handlers for the TPU channels
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
* History    : 2009.09.29
*        : Ver 0.10
*""FILE COMMENT END""**************************************************/

#include "r_pdl_tpu.h"
#include "r_pdl_definitions.h"

/*""FUNC COMMENT""***************************************************
* Module outline: Timer interrupt processing
*-------------------------------------------------------------------
* Include    : r_pdl_tpu_library_RX610xV.h
*-------------------------------------------------------------------
* Declaration  : void Interrupt_TPUn_TGInA(void)
*-------------------------------------------------------------------
* Function    : TGInA interrupt processing
*-------------------------------------------------------------------
* Argument    : Nothing
*-------------------------------------------------------------------
* Return value  : Nothing
*-------------------------------------------------------------------
* Input      : 
* Output    : 
*-------------------------------------------------------------------
* Use function  : rpdl_TPU_TGRA_callback_func[n]
*-------------------------------------------------------------------
* Notes      : 
*-------------------------------------------------------------------
* History    : 2009.09.30
*        : Ver 0.10
*        : 2009.10.12
*        : Ver 0.11
*        : Added DMAC control
*        : 2009.11.26
*        : Ver 0.12
*        : Modified DMAC control
*        : 2010.02.02
*        : Ver 0.13
*        : Added callback function checking
*        : 2010.02.08
*        : Ver 0.14
*        : Removed DMAC trigger control
*        : 2010.02.22
*        : Ver 0.15
*        : Removed fast interrupt code
*""FUNC COMMENT END""**********************************************/

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TGI0A
__fast_interrupt void Interrupt_TPU0_TGI0A(void)
#else
#pragma vector = PDL_INTC_VECTOR_TGI0A
__interrupt void Interrupt_TPU0_TGI0A(void)
#endif
{
  /* Call the user function */
  if (rpdl_TPU_TGRA_callback_func[0] != PDL_NO_FUNC)
  {
    rpdl_TPU_TGRA_callback_func[0]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TGI1A
__fast_interrupt void Interrupt_TPU1_TGI1A(void)
#else
#pragma vector = PDL_INTC_VECTOR_TGI1A
__interrupt void Interrupt_TPU1_TGI1A(void)
#endif
{
  /* Call the user function */
  if (rpdl_TPU_TGRA_callback_func[1] != PDL_NO_FUNC)
  {
    rpdl_TPU_TGRA_callback_func[1]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TGI2A
__fast_interrupt void Interrupt_TPU2_TGI2A(void)
#else
#pragma vector = PDL_INTC_VECTOR_TGI2A
__interrupt void Interrupt_TPU2_TGI2A(void)
#endif
{
  /* Call the user function */
  if (rpdl_TPU_TGRA_callback_func[2] != PDL_NO_FUNC)
  {
    rpdl_TPU_TGRA_callback_func[2]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TGI3A
__fast_interrupt void Interrupt_TPU3_TGI3A(void)
#else
#pragma vector = PDL_INTC_VECTOR_TGI3A
__interrupt void Interrupt_TPU3_TGI3A(void)
#endif
{
  /* Call the user function */
  if (rpdl_TPU_TGRA_callback_func[3] != PDL_NO_FUNC)
  {
    rpdl_TPU_TGRA_callback_func[3]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TGI4A
__fast_interrupt void Interrupt_TPU4_TGI4A(void)
#else
#pragma vector = PDL_INTC_VECTOR_TGI4A
__interrupt void Interrupt_TPU4_TGI4A(void)
#endif
{
  /* Call the user function */
  if (rpdl_TPU_TGRA_callback_func[4] != PDL_NO_FUNC)
  {
    rpdl_TPU_TGRA_callback_func[4]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TGI5A
__fast_interrupt void Interrupt_TPU5_TGI5A(void)
#else
#pragma vector = PDL_INTC_VECTOR_TGI5A
__interrupt void Interrupt_TPU5_TGI5A(void)
#endif
{
  /* Call the user function */
  if (rpdl_TPU_TGRA_callback_func[5] != PDL_NO_FUNC)
  {
    rpdl_TPU_TGRA_callback_func[5]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TGI6A
__fast_interrupt void Interrupt_TPU6_TGI6A(void)
#else
#pragma vector = PDL_INTC_VECTOR_TGI6A
__interrupt void Interrupt_TPU6_TGI6A(void)
#endif
{
  /* Call the user function */
  if (rpdl_TPU_TGRA_callback_func[6] != PDL_NO_FUNC)
  {
    rpdl_TPU_TGRA_callback_func[6]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TGI7A
__fast_interrupt void Interrupt_TPU7_TGI7A(void)
#else
#pragma vector = PDL_INTC_VECTOR_TGI7A
__interrupt void Interrupt_TPU7_TGI7A(void)
#endif
{
  /* Call the user function */
  if (rpdl_TPU_TGRA_callback_func[7] != PDL_NO_FUNC)
  {
    rpdl_TPU_TGRA_callback_func[7]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TGI8A
__fast_interrupt void Interrupt_TPU8_TGI8A(void)
#else
#pragma vector = PDL_INTC_VECTOR_TGI8A
__interrupt void Interrupt_TPU8_TGI8A(void)
#endif
{
  /* Call the user function */
  if (rpdl_TPU_TGRA_callback_func[8] != PDL_NO_FUNC)
  {
    rpdl_TPU_TGRA_callback_func[8]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TGI9A
__fast_interrupt void Interrupt_TPU9_TGI9A(void)
#else
#pragma vector = PDL_INTC_VECTOR_TGI9A
__interrupt void Interrupt_TPU9_TGI9A(void)
#endif
{
  /* Call the user function */
  if (rpdl_TPU_TGRA_callback_func[9] != PDL_NO_FUNC)
  {
    rpdl_TPU_TGRA_callback_func[9]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TGI10A
__fast_interrupt void Interrupt_TPU10_TGI10A(void)
#else
#pragma vector = PDL_INTC_VECTOR_TGI10A
__interrupt void Interrupt_TPU10_TGI10A(void)
#endif
{
  /* Call the user function */
  if (rpdl_TPU_TGRA_callback_func[10] != PDL_NO_FUNC)
  {
    rpdl_TPU_TGRA_callback_func[10]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TGI11A
__fast_interrupt void Interrupt_TPU11_TGI11A(void)
#else
#pragma vector = PDL_INTC_VECTOR_TGI11A
__interrupt void Interrupt_TPU11_TGI11A(void)
#endif
{
  /* Call the user function */
  if (rpdl_TPU_TGRA_callback_func[11] != PDL_NO_FUNC)
  {
    rpdl_TPU_TGRA_callback_func[11]();
  }
}

/*""FUNC COMMENT""***************************************************
* Module outline : Timer interrupt processing
*-------------------------------------------------------------------
* Include    : r_pdl_tpu_library_RX610xV.h
*-------------------------------------------------------------------
* Declaration  : void Interrupt_TPUn_TGInB(void)
*-------------------------------------------------------------------
* Function    : TGInB interrupt processing
*-------------------------------------------------------------------
* Argument    : Nothing
*-------------------------------------------------------------------
* Return value  : Nothing
*-------------------------------------------------------------------
* Input     : 
* Output    : 
*-------------------------------------------------------------------
* Use function  : rpdl_TPU_TGRB_callback_func[n]
*-------------------------------------------------------------------
* Notes     : 
*-------------------------------------------------------------------
* History    : 2009.09.30
*        : Ver 0.10
*        : 2010.02.02
*        : Ver 0.11
*        : Added callback function checking
*        : 2010.02.22
*        : Ver 0.12
*        : Removed fast interrupt code
*""FUNC COMMENT END""**********************************************/

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TGI0B
__fast_interrupt void Interrupt_TPU0_TGI0B(void)
#else
#pragma vector = PDL_INTC_VECTOR_TGI0B
__interrupt void Interrupt_TPU0_TGI0B(void)
#endif
{
  /* Call the user function */
  if (rpdl_TPU_TGRB_callback_func[0] != PDL_NO_FUNC)
  {
    rpdl_TPU_TGRB_callback_func[0]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TGI1B
__fast_interrupt void Interrupt_TPU1_TGI1B(void)
#else
#pragma vector = PDL_INTC_VECTOR_TGI1B
__interrupt void Interrupt_TPU1_TGI1B(void)
#endif
{
  /* Call the user function */
  if (rpdl_TPU_TGRB_callback_func[1] != PDL_NO_FUNC)
  {
    rpdl_TPU_TGRB_callback_func[1]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TGI2B
__fast_interrupt void Interrupt_TPU2_TGI2B(void)
#else
#pragma vector = PDL_INTC_VECTOR_TGI2B
__interrupt void Interrupt_TPU2_TGI2B(void)
#endif
{
  /* Call the user function */
  if (rpdl_TPU_TGRB_callback_func[2] != PDL_NO_FUNC)
  {
    rpdl_TPU_TGRB_callback_func[2]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TGI3B
__fast_interrupt void Interrupt_TPU3_TGI3B(void)
#else
#pragma vector = PDL_INTC_VECTOR_TGI3B
__interrupt void Interrupt_TPU3_TGI3B(void)
#endif
{
  /* Call the user function */
  if (rpdl_TPU_TGRB_callback_func[3] != PDL_NO_FUNC)
  {
    rpdl_TPU_TGRB_callback_func[3]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TGI4B
__fast_interrupt void Interrupt_TPU4_TGI4B(void)
#else
#pragma vector = PDL_INTC_VECTOR_TGI4B
__interrupt void Interrupt_TPU4_TGI4B(void)
#endif
{
  /* Call the user function */
  if (rpdl_TPU_TGRB_callback_func[4] != PDL_NO_FUNC)
  {
    rpdl_TPU_TGRB_callback_func[4]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TGI5B
__fast_interrupt void Interrupt_TPU5_TGI5B(void)
#else
#pragma vector = PDL_INTC_VECTOR_TGI5B
__interrupt void Interrupt_TPU5_TGI5B(void)
#endif
{
  /* Call the user function */
  if (rpdl_TPU_TGRB_callback_func[5] != PDL_NO_FUNC)
  {
    rpdl_TPU_TGRB_callback_func[5]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TGI6B
__fast_interrupt void Interrupt_TPU6_TGI6B(void)
#else
#pragma vector = PDL_INTC_VECTOR_TGI6B
__interrupt void Interrupt_TPU6_TGI6B(void)
#endif
{
  /* Call the user function */
  if (rpdl_TPU_TGRB_callback_func[6] != PDL_NO_FUNC)
  {
    rpdl_TPU_TGRB_callback_func[6]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TGI7B
__fast_interrupt void Interrupt_TPU7_TGI7B(void)
#else
#pragma vector = PDL_INTC_VECTOR_TGI7B
__interrupt void Interrupt_TPU7_TGI7B(void)
#endif
{
  /* Call the user function */
  if (rpdl_TPU_TGRB_callback_func[7] != PDL_NO_FUNC)
  {
    rpdl_TPU_TGRB_callback_func[7]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TGI8B
__fast_interrupt void Interrupt_TPU8_TGI8B(void)
#else
#pragma vector = PDL_INTC_VECTOR_TGI8B
__interrupt void Interrupt_TPU8_TGI8B(void)
#endif
{
  /* Call the user function */
  if (rpdl_TPU_TGRB_callback_func[8] != PDL_NO_FUNC)
  {
    rpdl_TPU_TGRB_callback_func[8]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TGI9B
__fast_interrupt void Interrupt_TPU9_TGI9B(void)
#else
#pragma vector = PDL_INTC_VECTOR_TGI9B
__interrupt void Interrupt_TPU9_TGI9B(void)
#endif
{
  /* Call the user function */
  if (rpdl_TPU_TGRB_callback_func[9] != PDL_NO_FUNC)
  {
    rpdl_TPU_TGRB_callback_func[9]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TGI10B
__fast_interrupt void Interrupt_TPU10_TGI10B(void)
#else
#pragma vector = PDL_INTC_VECTOR_TGI10B
__interrupt void Interrupt_TPU10_TGI10B(void)
#endif
{
  /* Call the user function */
  if (rpdl_TPU_TGRB_callback_func[10] != PDL_NO_FUNC)
  {
    rpdl_TPU_TGRB_callback_func[10]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TGI11B
__fast_interrupt void Interrupt_TPU11_TGI11B(void)
#else
#pragma vector = PDL_INTC_VECTOR_TGI11B
__interrupt void Interrupt_TPU11_TGI11B(void)
#endif
{
  /* Call the user function */
  if (rpdl_TPU_TGRB_callback_func[11] != PDL_NO_FUNC)
  {
    rpdl_TPU_TGRB_callback_func[11]();
  }
}

/*""FUNC COMMENT""***************************************************
* Module outline : Timer interrupt processing
*-------------------------------------------------------------------
* Include    : r_pdl_tpu_library_RX610xV.h
*-------------------------------------------------------------------
* Declaration  : void Interrupt_TPUn_TGInC(void)
*-------------------------------------------------------------------
* Function    : TGInC interrupt processing
*-------------------------------------------------------------------
* Argument    : Nothing
*-------------------------------------------------------------------
* Return value  : Nothing
*-------------------------------------------------------------------
* Input     : 
* Output    : 
*-------------------------------------------------------------------
* Use function  : rpdl_TPU_TGRC_callback_func[n]
*-------------------------------------------------------------------
* Notes     : 
*-------------------------------------------------------------------
* History    : 2009.09.30
*        : Ver 0.10
*        : 2010.02.02
*        : Ver 0.11
*        : Added callback function checking
*        : 2010.02.17
*        : Ver 0.12
*        : Removed fast interrupt code
*""FUNC COMMENT END""**********************************************/

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TGI0C
__fast_interrupt void Interrupt_TPU0_TGI0C(void)
#else
#pragma vector = PDL_INTC_VECTOR_TGI0C
__interrupt void Interrupt_TPU0_TGI0C(void)
#endif
{
  /* Call the user function */
  if (rpdl_TPU_TGRC_callback_func[0] != PDL_NO_FUNC)
  {
    rpdl_TPU_TGRC_callback_func[0]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TGI3C
__fast_interrupt void Interrupt_TPU3_TGI3C(void)
#else
#pragma vector = PDL_INTC_VECTOR_TGI3C
__interrupt void Interrupt_TPU3_TGI3C(void)
#endif
{
  /* Call the user function */
  if (rpdl_TPU_TGRC_callback_func[3] != PDL_NO_FUNC)
  {
    rpdl_TPU_TGRC_callback_func[3]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TGI6C
__fast_interrupt void Interrupt_TPU6_TGI6C(void)
#else
#pragma vector = PDL_INTC_VECTOR_TGI6C
__interrupt void Interrupt_TPU6_TGI6C(void)
#endif
{
  /* Call the user function */
  if (rpdl_TPU_TGRC_callback_func[6] != PDL_NO_FUNC)
  {
    rpdl_TPU_TGRC_callback_func[6]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TGI9C
__fast_interrupt void Interrupt_TPU9_TGI9C(void)
#else
#pragma vector = PDL_INTC_VECTOR_TGI9C
__interrupt void Interrupt_TPU9_TGI9C(void)
#endif
{
  /* Call the user function */
  if (rpdl_TPU_TGRC_callback_func[9] != PDL_NO_FUNC)
  {
    rpdl_TPU_TGRC_callback_func[9]();
  }
}

/*""FUNC COMMENT""***************************************************
* Module outline : Timer interrupt processing
*-------------------------------------------------------------------
* Include    : r_pdl_tpu_library_RX610xV.h
*-------------------------------------------------------------------
* Declaration  : void Interrupt_TPUn_TGInD(void)
*-------------------------------------------------------------------
* Function    : TGInD interrupt processing
*-------------------------------------------------------------------
* Argument    : Nothing
*-------------------------------------------------------------------
* Return value  : Nothing
*-------------------------------------------------------------------
* Input     : 
* Output    : 
*-------------------------------------------------------------------
* Use function  : rpdl_TPU_TGRD_callback_func[n]
*-------------------------------------------------------------------
* Notes     : 
*-------------------------------------------------------------------
* History    : 2009.09.30
*        : Ver 0.10
*        : 2010.02.02
*        : Ver 0.11
*        : Added callback function checking
*        : 2010.02.22
*        : Ver 0.12
*        : Removed fast interrupt code
*""FUNC COMMENT END""**********************************************/

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TGI0D
__fast_interrupt void Interrupt_TPU0_TGI0D(void)
#else
#pragma vector = PDL_INTC_VECTOR_TGI0D
__interrupt void Interrupt_TPU0_TGI0D(void)
#endif
{
  /* Call the user function */
  if (rpdl_TPU_TGRD_callback_func[0] != PDL_NO_FUNC)
  {
    rpdl_TPU_TGRD_callback_func[0]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TGI3D
__fast_interrupt void Interrupt_TPU3_TGI3D(void)
#else
#pragma vector = PDL_INTC_VECTOR_TGI3D
__interrupt void Interrupt_TPU3_TGI3D(void)
#endif
{
  /* Call the user function */
  if (rpdl_TPU_TGRD_callback_func[3] != PDL_NO_FUNC)
  {
    rpdl_TPU_TGRD_callback_func[3]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TGI6D
__fast_interrupt void Interrupt_TPU6_TGI6D(void)
#else
#pragma vector = PDL_INTC_VECTOR_TGI6D
__interrupt void Interrupt_TPU6_TGI6D(void)
#endif
{
  /* Call the user function */
  if (rpdl_TPU_TGRD_callback_func[6] != PDL_NO_FUNC)
  {
    rpdl_TPU_TGRD_callback_func[6]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TGI9D
__fast_interrupt void Interrupt_TPU9_TGI9D(void)
#else
#pragma vector = PDL_INTC_VECTOR_TGI9D
__interrupt void Interrupt_TPU9_TGI9D(void)
#endif
{
  /* Call the user function */
  if (rpdl_TPU_TGRD_callback_func[9] != PDL_NO_FUNC)
  {
    rpdl_TPU_TGRD_callback_func[9]();
  }
}

/*""FUNC COMMENT""***************************************************
* Module outline : Timer interrupt processing
*-------------------------------------------------------------------
* Include    : r_pdl_tpu_library_RX610xV.h
*-------------------------------------------------------------------
* Declaration  : void Interrupt_TPUn_TCInV(void)
*-------------------------------------------------------------------
* Function    : TCInV interrupt processing
*-------------------------------------------------------------------
* Argument    : Nothing
*-------------------------------------------------------------------
* Return value  : Nothing
*-------------------------------------------------------------------
* Input     : 
* Output    : 
*-------------------------------------------------------------------
* Use function  : rpdl_TPU_Overflow_callback_func[n]
*-------------------------------------------------------------------
* Notes     : 
*-------------------------------------------------------------------
* History    : 2009.09.30
*        : Ver 0.10
*        : 2010.01.06
*        : Ver 0.11
*        : Corrected TG to TC
*        : 2010.02.02
*        : Ver 0.12
*        : Added callback function checking
*        : 2010.02.17
*        : Ver 0.13
*        : Removed fast interrupt code
*""FUNC COMMENT END""**********************************************/

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TCI0V
__fast_interrupt void Interrupt_TPU0_TCI0V(void)
#else
#pragma vector = PDL_INTC_VECTOR_TCI0V
__interrupt void Interrupt_TPU0_TCI0V(void)
#endif
{
  /* Call the user function */
  if (rpdl_TPU_Overflow_callback_func[0] != PDL_NO_FUNC)
  {
    rpdl_TPU_Overflow_callback_func[0]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TCI1V
__fast_interrupt void Interrupt_TPU1_TCI1V(void)
#else
#pragma vector = PDL_INTC_VECTOR_TCI1V
__interrupt void Interrupt_TPU1_TCI1V(void)
#endif
{
  /* Call the user function */
  if (rpdl_TPU_Overflow_callback_func[1] != PDL_NO_FUNC)
  {
    rpdl_TPU_Overflow_callback_func[1]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TCI2V
__fast_interrupt void Interrupt_TPU2_TCI2V(void)
#else
#pragma vector = PDL_INTC_VECTOR_TCI2V
__interrupt void Interrupt_TPU2_TCI2V(void)
#endif
{
  /* Call the user function */
  if (rpdl_TPU_Overflow_callback_func[2] != PDL_NO_FUNC)
  {
    rpdl_TPU_Overflow_callback_func[2]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TCI3V
__fast_interrupt void Interrupt_TPU3_TCI3V(void)
#else
#pragma vector = PDL_INTC_VECTOR_TCI3V
__interrupt void Interrupt_TPU3_TCI3V(void)
#endif
{
  /* Call the user function */
  if (rpdl_TPU_Overflow_callback_func[3] != PDL_NO_FUNC)
  {
    rpdl_TPU_Overflow_callback_func[3]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TCI4V
__fast_interrupt void Interrupt_TPU4_TCI4V(void)
#else
#pragma vector = PDL_INTC_VECTOR_TCI4V
__interrupt void Interrupt_TPU4_TCI4V(void)
#endif
{
  /* Call the user function */
  if (rpdl_TPU_Overflow_callback_func[4] != PDL_NO_FUNC)
  {
    rpdl_TPU_Overflow_callback_func[4]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TCI5V
__fast_interrupt void Interrupt_TPU5_TCI5V(void)
#else
#pragma vector = PDL_INTC_VECTOR_TCI5V
__interrupt void Interrupt_TPU5_TCI5V(void)
#endif
{
  /* Call the user function */
  if (rpdl_TPU_Overflow_callback_func[5] != PDL_NO_FUNC)
  {
    rpdl_TPU_Overflow_callback_func[5]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TCI6V
__fast_interrupt void Interrupt_TPU6_TCI6V(void)
#else
#pragma vector = PDL_INTC_VECTOR_TCI6V
__interrupt void Interrupt_TPU6_TCI6V(void)
#endif
{
  /* Call the user function */
  if (rpdl_TPU_Overflow_callback_func[6] != PDL_NO_FUNC)
  {
    rpdl_TPU_Overflow_callback_func[6]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TCI7V
__fast_interrupt void Interrupt_TPU7_TCI7V(void)
#else
#pragma vector = PDL_INTC_VECTOR_TCI7V
__interrupt void Interrupt_TPU7_TCI7V(void)
#endif
{
  /* Call the user function */
  if (rpdl_TPU_Overflow_callback_func[7] != PDL_NO_FUNC)
  {
    rpdl_TPU_Overflow_callback_func[7]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TCI8V
__fast_interrupt void Interrupt_TPU8_TCI8V(void)
#else
#pragma vector = PDL_INTC_VECTOR_TCI8V
__interrupt void Interrupt_TPU8_TCI8V(void)
#endif
{
  /* Call the user function */
  if (rpdl_TPU_Overflow_callback_func[8] != PDL_NO_FUNC)
  {
    rpdl_TPU_Overflow_callback_func[8]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TCI9V
__fast_interrupt void Interrupt_TPU9_TCI9V(void)
#else
#pragma vector = PDL_INTC_VECTOR_TCI9V
__interrupt void Interrupt_TPU9_TCI9V(void)
#endif
{
  /* Call the user function */
  if (rpdl_TPU_Overflow_callback_func[9] != PDL_NO_FUNC)
  {
    rpdl_TPU_Overflow_callback_func[9]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TCI10V
__fast_interrupt void Interrupt_TPU10_TCI10V(void)
#else
#pragma vector = PDL_INTC_VECTOR_TCI10V
__interrupt void Interrupt_TPU10_TCI10V(void)
#endif
{
  /* Call the user function */
  if (rpdl_TPU_Overflow_callback_func[10] != PDL_NO_FUNC)
  {
    rpdl_TPU_Overflow_callback_func[10]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TCI11V
__fast_interrupt void Interrupt_TPU11_TCI11V(void)
#else
#pragma vector = PDL_INTC_VECTOR_TCI11V
__interrupt void Interrupt_TPU11_TCI11V(void)
#endif
{
  /* Call the user function */
  if (rpdl_TPU_Overflow_callback_func[11] != PDL_NO_FUNC)
  {
    rpdl_TPU_Overflow_callback_func[11]();
  }
}

/*""FUNC COMMENT""***************************************************
* Module outline : Timer interrupt processing
*-------------------------------------------------------------------
* Include    : r_pdl_tpu_library_RX610xV.h
*-------------------------------------------------------------------
* Declaration  : void Interrupt_TPUn_TCInU(void)
*-------------------------------------------------------------------
* Function    : TCInU interrupt processing
*-------------------------------------------------------------------
* Argument    : Nothing
*-------------------------------------------------------------------
* Return value  : Nothing
*-------------------------------------------------------------------
* Input     : 
* Output    : 
*-------------------------------------------------------------------
* Use function  : rpdl_TPU_Underflow_callback_func[n]
*-------------------------------------------------------------------
* Notes     : 
*-------------------------------------------------------------------
* History    : 2009.09.30
*        : Ver 0.10
*        : 2010.01.06
*        : Ver 0.11
*        : Corrected TG to TC
*        : 2010.02.02
*        : Ver 0.12
*        : Added callback function checking
*        : 2010.02.22
*        : Ver 0.13
*        : Removed fast interrupt code
*""FUNC COMMENT END""**********************************************/

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TCI1U
__fast_interrupt void Interrupt_TPU1_TCI1U(void)
#else
#pragma vector = PDL_INTC_VECTOR_TCI1U
__interrupt void Interrupt_TPU1_TCI1U(void)
#endif
{
  /* Call the user function */
  if (rpdl_TPU_Underflow_callback_func[1] != PDL_NO_FUNC)
  {
    rpdl_TPU_Underflow_callback_func[1]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TCI2U
__fast_interrupt void Interrupt_TPU2_TCI2U(void)
#else
#pragma vector = PDL_INTC_VECTOR_TCI2U
__interrupt void Interrupt_TPU2_TCI2U(void)
#endif
{
  /* Call the user function */
  if (rpdl_TPU_Underflow_callback_func[2] != PDL_NO_FUNC)
  {
    rpdl_TPU_Underflow_callback_func[2]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TCI4U
__fast_interrupt void Interrupt_TPU4_TCI4U(void)
#else
#pragma vector = PDL_INTC_VECTOR_TCI4U
__interrupt void Interrupt_TPU4_TCI4U(void)
#endif
{
  /* Call the user function */
  if (rpdl_TPU_Underflow_callback_func[4] != PDL_NO_FUNC)
  {
    rpdl_TPU_Underflow_callback_func[4]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TCI5U
__fast_interrupt void Interrupt_TPU5_TCI5U(void)
#else
#pragma vector = PDL_INTC_VECTOR_TCI5U
__interrupt void Interrupt_TPU5_TCI5U(void)
#endif
{
  /* Call the user function */
  if (rpdl_TPU_Underflow_callback_func[5] != PDL_NO_FUNC)
  {
    rpdl_TPU_Underflow_callback_func[5]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TCI7U
__fast_interrupt void Interrupt_TPU7_TCI7U(void)
#else
#pragma vector = PDL_INTC_VECTOR_TCI7U
__interrupt void Interrupt_TPU7_TCI7U(void)
#endif
{
  /* Call the user function */
  if (rpdl_TPU_Underflow_callback_func[7] != PDL_NO_FUNC)
  {
    rpdl_TPU_Underflow_callback_func[7]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TCI8U
__fast_interrupt void Interrupt_TPU8_TCI8U(void)
#else
#pragma vector = PDL_INTC_VECTOR_TCI8U
__interrupt void Interrupt_TPU8_TCI8U(void)
#endif
{
  /* Call the user function */
  if (rpdl_TPU_Underflow_callback_func[8] != PDL_NO_FUNC)
  {
    rpdl_TPU_Underflow_callback_func[8]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TCI10U
__fast_interrupt void Interrupt_TPU10_TCI10U(void)
#else
#pragma vector = PDL_INTC_VECTOR_TCI10U
__interrupt void Interrupt_TPU10_TCI10U(void)
#endif
{
  /* Call the user function */
  if (rpdl_TPU_Underflow_callback_func[10] != PDL_NO_FUNC)
  {
    rpdl_TPU_Underflow_callback_func[10]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TCI11U
__fast_interrupt void Interrupt_TPU11_TCI11U(void)
#else
#pragma vector = PDL_INTC_VECTOR_TCI11U
__interrupt void Interrupt_TPU11_TCI11U(void)
#endif
{
  /* Call the user function */
  if (rpdl_TPU_Underflow_callback_func[11] != PDL_NO_FUNC)
  {
    rpdl_TPU_Underflow_callback_func[11]();
  }
}
/* End of file */
