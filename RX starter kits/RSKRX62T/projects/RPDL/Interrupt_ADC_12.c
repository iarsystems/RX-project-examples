/* Adapted for use with IAR Embedded Workbench */
/*""FILE COMMENT""*******************************************************
* System Name  : 12-bit ADC Interrupt program for RX62Txx
* File Name   : Interrupt_ADC_12.c
* Version    : 1.01
* Contents    : Interrupt handler for ADC_12
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

#include "r_pdl_adc_12.h"
#include "r_pdl_definitions.h"
#include "r_pdl_user_definitions.h"

/*""FUNC COMMENT""***************************************************
* Module outline: ADC interrupt processing
*-------------------------------------------------------------------
* Declaration  : void Interrupt_ADCn(void)
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
* Use function  : rpdl_ADC_12_callback_func[n]()
*-------------------------------------------------------------------
* Notes      : 
*-------------------------------------------------------------------
* History    : 2011.05.04
*        : Ver 1.01
*        : Release CS3.
*""FUNC COMMENT END""**********************************************/

#if FAST_INTC_VECTOR == VECT_S12AD0_S12ADI0
__fast_interrupt void Interrupt_ADC_12_0(void)
#else
#pragma vector = VECT_S12AD0_S12ADI0
__interrupt void Interrupt_ADC_12_0(void)
#endif
{
  /* Call the user function */
  if (rpdl_ADC_12_callback_func[0] != PDL_NO_FUNC)
  {
    rpdl_ADC_12_callback_func[0]();
  }
}

#if FAST_INTC_VECTOR == VECT_S12AD1_S12ADI1
__fast_interrupt void Interrupt_ADC_12_1(void)
#else
#pragma vector = VECT_S12AD1_S12ADI1
__interrupt void Interrupt_ADC_12_1(void)
#endif
{
  /* Call the user function */
  if (rpdl_ADC_12_callback_func[1] != PDL_NO_FUNC)
  {
    rpdl_ADC_12_callback_func[1]();
  }
}

/*""FUNC COMMENT""***************************************************
* Module outline: Comparator interrupt processing
*-------------------------------------------------------------------
* Declaration  : void Interrupt_ADC_12_Cmp(void)
*-------------------------------------------------------------------
* Function    : Interrupt processing function for comparator
*-------------------------------------------------------------------
* Argument    : Nothing
*-------------------------------------------------------------------
* Return value  : Nothing
*-------------------------------------------------------------------
* Input      : 
* Output    : 
*-------------------------------------------------------------------
* Use function  : rpdl_ADC_12_cmp_callback_func()
*-------------------------------------------------------------------
* Notes      : 
*-------------------------------------------------------------------
* History    : 2011.05.04
*        : Ver 1.01
*        : Release CS3.
*""FUNC COMMENT END""**********************************************/

#if FAST_INTC_VECTOR == VECT_S12AD_CMPI
__fast_interrupt void Interrupt_ADC_12_Cmp(void)
#else
#pragma vector = VECT_S12AD_CMPI
__interrupt void Interrupt_ADC_12_Cmp(void)
#endif
{
  /* Call the user function */
  if (rpdl_ADC_12_cmp_callback_func != PDL_NO_FUNC)
  {
    rpdl_ADC_12_cmp_callback_func();
  }
}

/* End of file */
