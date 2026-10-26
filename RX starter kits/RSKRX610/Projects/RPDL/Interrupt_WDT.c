/* Adapted for use with IAR Embedded Workbench */
/*""FILE COMMENT""*******************************************************
* System Name  : Interrupt program for RX610xV
* File Name    : Interrupt_WDT.c
* Version    : 0.10
* Contents    : Interrupt handler for the Watchdog timer
* Customer    : 
* Model       :
* Order       :
* CPU      : RX600
* Compiler    : RXC
* OS      : Nothing
* Programmer  : 
* Note      : 
************************************************************************
* Copyright,2009 RENESAS TECHNOLOGY CORPORATION
* AND RENESAS SOLUTIONS CORPORATION
************************************************************************
* History    : 2009.11.24
*        : Ver 0.10
*""FILE COMMENT END""**************************************************/

#include "r_pdl_wdt.h"
#include "r_pdl_definitions.h"

/*""FUNC COMMENT""***************************************************
* Module outline : Timer interrupt processing
*-------------------------------------------------------------------
* Include    : r_pdl_wdt_library_RX610xV.h
*-------------------------------------------------------------------
* Declaration  : void Interrupt_WDT(void)
*-------------------------------------------------------------------
* Function    : Interrupt processing function for the WDT
*-------------------------------------------------------------------
* Argument    : Nothing
*-------------------------------------------------------------------
* Return value  : Nothing
*-------------------------------------------------------------------
* Input      :
* Output    : 
*-------------------------------------------------------------------
* Use function  : WDT_callback_func
*-------------------------------------------------------------------
* Notes      :
*-------------------------------------------------------------------
* History    : 2009.11.26
*        : Ver 0.10
*        : 2010.02.02
*        : Ver 0.11
*        : Added callback function checking
*        : 2010.02.22
*        : Ver 0.12
*        : Removed fast interrupt code
*""FUNC COMMENT END""**********************************************/

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_WOVI
__fast_interrupt void Interrupt_WDT(void)
#else
#pragma vector = PDL_INTC_VECTOR_WOVI
__interrupt void Interrupt_WDT(void)
#endif
{
  /* Call the user function */
  if (rpdl_WDT_callback_func != PDL_NO_FUNC)
  {
    rpdl_WDT_callback_func();
  }
}
/* End of file */
