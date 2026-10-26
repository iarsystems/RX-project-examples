/* Adapted for use with IAR Embedded Workbench */
/*""FILE COMMENT""*******************************************************
* System Name  : 12-bit ADC Interrupt program for RX210xx
* File Name   : Interrupt_ADC_12.c
* Version    : 0.20
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
* History    : 2011.08.12
*        : Ver 0.20
*        : WS2 release.
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
* History    : 2011.08.12
*        : Ver 0.20
*        : WS2 release.
*""FUNC COMMENT END""**********************************************/

#if FAST_INTC_VECTOR == VECT_S12AD_S12ADI0
__fast_interrupt void Interrupt_ADC_12_0(void)
#else
#pragma vector = VECT_S12AD_S12ADI0
__interrupt void Interrupt_ADC_12_0(void)
#endif
{
  /* Call the user function */
  if (rpdl_ADC_12_callback_func[0] != PDL_NO_FUNC)
  {
    rpdl_ADC_12_callback_func[0]();
  }
}

#if FAST_INTC_VECTOR == VECT_S12AD_GBADI
__fast_interrupt void Interrupt_ADC_12_0_b(void)
#else
#pragma vector = VECT_S12AD_GBADI
__interrupt void Interrupt_ADC_12_0_b(void)
#endif
{
  /* Call the user function */
  if (rpdl_ADC_12_gb_callback_func[0] != PDL_NO_FUNC)
  {
    rpdl_ADC_12_gb_callback_func[0]();
  }
}
/* End of file */
