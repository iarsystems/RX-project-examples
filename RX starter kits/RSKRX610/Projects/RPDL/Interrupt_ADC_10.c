/* Adapted for use with IAR Embedded Workbench */
/*""FILE COMMENT""*******************************************************
* System Name  : Interrupt program for RX610xV
* File Name   : Interrupt_ADC_10.c
* Version    : 0.10
* Contents    : Interrupt handlers for the ADC units
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
* History    : 2009.09.21
*        : Ver 0.10
*""FILE COMMENT END""**************************************************/

#include "r_pdl_adc_10.h"
#include "r_pdl_definitions.h"

/*""FUNC COMMENT""***************************************************
* Module outline: ADC interrupt processing
*-------------------------------------------------------------------
* Include    : r_pdl_adc_10_library_RX610xV.h
*-------------------------------------------------------------------
* Declaration  : void InterruptADCn(void)
*-------------------------------------------------------------------
* Function    : Interrupt processing function for ADC unit n
*-------------------------------------------------------------------
* Argument    : Nothing
*-------------------------------------------------------------------
* Return value  : Nothing
*-------------------------------------------------------------------
* Input      : 
* Output    : 
*-------------------------------------------------------------------
* Use function  : ADC_callback_func[n]()
*-------------------------------------------------------------------
* Notes      : 
*-------------------------------------------------------------------
* History    : 2009.09.25
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

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_ADI0
__fast_interrupt void Interrupt_ADC0(void)
#else
#pragma vector = PDL_INTC_VECTOR_ADI0
__interrupt void Interrupt_ADC0(void)
#endif
{
  /* Call the user function */
  if (rpdl_ADC_10_callback_func[0] != PDL_NO_FUNC)
  {
    rpdl_ADC_10_callback_func[0]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_ADI1
__fast_interrupt void Interrupt_ADC1(void)
#else
#pragma vector = PDL_INTC_VECTOR_ADI1
__interrupt void Interrupt_ADC1(void)
#endif
{
  /* Call the user function */
  if (rpdl_ADC_10_callback_func[1] != PDL_NO_FUNC)
  {
    rpdl_ADC_10_callback_func[1]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_ADI2
__fast_interrupt void Interrupt_ADC2(void)
#else
#pragma vector = PDL_INTC_VECTOR_ADI2
__interrupt void Interrupt_ADC2(void)
#endif
{
  /* Call the user function */
  if (rpdl_ADC_10_callback_func[2] != PDL_NO_FUNC)
  {
    rpdl_ADC_10_callback_func[2]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_ADI3
__fast_interrupt void Interrupt_ADC3(void)
#else
#pragma vector = PDL_INTC_VECTOR_ADI3
__interrupt void Interrupt_ADC3(void)
#endif
{
  /* Call the user function */
  if (rpdl_ADC_10_callback_func[3] != PDL_NO_FUNC)
  {
    rpdl_ADC_10_callback_func[3]();
  }
}
/* End of file */
