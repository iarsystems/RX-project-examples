/* Adapted for use with IAR Embedded Workbench */
/*""FILE COMMENT""*******************************************************
* System Name  : Interrupt program for RX610xV
* File Name    : Interrupt_INTC.c
* Version    : 0.10
* Contents    : Interrupt handlers for the external interrupts
* Customer    :
* Model      :
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
* History    : 2009.09.08
*        : Ver 0.10
*""FILE COMMENT END""**************************************************/

#include "r_pdl_intc.h"
#include "r_pdl_definitions.h"
#include "intrinsics.h"

void rpdl_ADC_10_Start(void);

/*""FUNC COMMENT""***************************************************
* Module outline: External interrupt processing
*-------------------------------------------------------------------
* Include    : r_pdl_intc_library_RX610xV.h
*-------------------------------------------------------------------
* Declaration  : void Interrupt_XXXX(void)
*-------------------------------------------------------------------
* Function    : Interrupt processing function for External interrupts
*-------------------------------------------------------------------
* Argument    : Nothing
*-------------------------------------------------------------------
* Return value  : Nothing
*------------------------------------------------------------------
* Input      :
* Output    :
*-------------------------------------------------------------------
* Use function  : INTC_callback_func[n]()
*-------------------------------------------------------------------
* Notes       :
*-------------------------------------------------------------------
* History    : 2009.09.25
*        : Ver 0.10
*        : 2009.11.06
*        : Ver 0.11
*        : Added DMAC control for IRQ0 to IRQ3
*        : PDL_NO_FUNC used for no-function pointer
*        : 2009.11.26
*        : Ver 0.12
*        : Modified DMAC control
*        : 2010.02.02
*        : Ver 0.13
*        : Added callback function checking
*        : 2010.02.05
*        : Ver 0.14
*        : Prevented out-of-order completion before __wait_for_interrupt();
*        : 2010.02.08
*        : Ver 0.15
*        : Removed DMAC trigger control
*        : 2010.02.10
*        : Ver 0.16
*        : Added BRK support for the low-power modes
*        : 2010.02.22
*        : Ver 0.17
*        : Removed fast interrupt code
*        : 2010.02.23
*        : Ver 0.18
*        : Updated for the new iodefine.h file.
*""FUNC COMMENT END""**********************************************/

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_IRQ0
__fast_interrupt void Interrupt_IRQ0(void)
#else
#pragma vector = PDL_INTC_VECTOR_IRQ0
__interrupt void Interrupt_IRQ0(void)
#endif
{
  /* Call the user function */
  if (rpdl_INTC_callback_func[PDL_INTC_IRQ0] != PDL_NO_FUNC)
  {
    rpdl_INTC_callback_func[PDL_INTC_IRQ0]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_IRQ1
__fast_interrupt void Interrupt_IRQ1(void)
#else
#pragma vector = PDL_INTC_VECTOR_IRQ1
__interrupt void Interrupt_IRQ1(void)
#endif
{
  /* Call the user function */
  if (rpdl_INTC_callback_func[PDL_INTC_IRQ1] != PDL_NO_FUNC)
  {
    rpdl_INTC_callback_func[PDL_INTC_IRQ1]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_IRQ2
__fast_interrupt void Interrupt_IRQ2(void)
#else
#pragma vector = PDL_INTC_VECTOR_IRQ2
__interrupt void Interrupt_IRQ2(void)
#endif
{
  /* Call the user function */
  if (rpdl_INTC_callback_func[PDL_INTC_IRQ2] != PDL_NO_FUNC)
  {
    rpdl_INTC_callback_func[PDL_INTC_IRQ2]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_IRQ3
__fast_interrupt void Interrupt_IRQ3(void)
#else
#pragma vector = PDL_INTC_VECTOR_IRQ3
__interrupt void Interrupt_IRQ3(void)
#endif
{
  /* Call the user function */
  if (rpdl_INTC_callback_func[PDL_INTC_IRQ3] != PDL_NO_FUNC)
  {
    rpdl_INTC_callback_func[PDL_INTC_IRQ3]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_IRQ4
__fast_interrupt void Interrupt_IRQ4(void)
#else
#pragma vector = PDL_INTC_VECTOR_IRQ4
__interrupt void Interrupt_IRQ4(void)
#endif
{
  /* Call the user function */
  if (rpdl_INTC_callback_func[PDL_INTC_IRQ4] != PDL_NO_FUNC)
  {
    rpdl_INTC_callback_func[PDL_INTC_IRQ4]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_IRQ5
__fast_interrupt void Interrupt_IRQ5(void)
#else
#pragma vector = PDL_INTC_VECTOR_IRQ5
__interrupt void Interrupt_IRQ5(void)
#endif
{
  /* Call the user function */
  if (rpdl_INTC_callback_func[PDL_INTC_IRQ5] != PDL_NO_FUNC)
  {
    rpdl_INTC_callback_func[PDL_INTC_IRQ5]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_IRQ6
__fast_interrupt void Interrupt_IRQ6(void)
#else
#pragma vector = PDL_INTC_VECTOR_IRQ6
__interrupt void Interrupt_IRQ6(void)
#endif
{
  /* Call the user function */
  if (rpdl_INTC_callback_func[PDL_INTC_IRQ6] != PDL_NO_FUNC)
  {
    rpdl_INTC_callback_func[PDL_INTC_IRQ6]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_IRQ7
__fast_interrupt void Interrupt_IRQ7(void)
#else
#pragma vector = PDL_INTC_VECTOR_IRQ7
__interrupt void Interrupt_IRQ7(void)
#endif
{
  /* Call the user function */
  if (rpdl_INTC_callback_func[PDL_INTC_IRQ7] != PDL_NO_FUNC)
  {
    rpdl_INTC_callback_func[PDL_INTC_IRQ7]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_IRQ8
__fast_interrupt void Interrupt_IRQ8(void)
#else
#pragma vector = PDL_INTC_VECTOR_IRQ8
__interrupt void Interrupt_IRQ8(void)
#endif
{
  /* Call the user function */
  if (rpdl_INTC_callback_func[PDL_INTC_IRQ8] != PDL_NO_FUNC)
  {
    rpdl_INTC_callback_func[PDL_INTC_IRQ8]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_IRQ9
__fast_interrupt void Interrupt_IRQ9(void)
#else
#pragma vector = PDL_INTC_VECTOR_IRQ9
__interrupt void Interrupt_IRQ9(void)
#endif
{
  /* Call the user function */
  if (rpdl_INTC_callback_func[PDL_INTC_IRQ9] != PDL_NO_FUNC)
  {
    rpdl_INTC_callback_func[PDL_INTC_IRQ9]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_IRQ10
__fast_interrupt void Interrupt_IRQ10(void)
#else
#pragma vector = PDL_INTC_VECTOR_IRQ10
__interrupt void Interrupt_IRQ10(void)
#endif
{
  /* Call the user function */
  if (rpdl_INTC_callback_func[PDL_INTC_IRQ10] != PDL_NO_FUNC)
  {
    rpdl_INTC_callback_func[PDL_INTC_IRQ10]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_IRQ11
__fast_interrupt void Interrupt_IRQ11(void)
#else
#pragma vector = PDL_INTC_VECTOR_IRQ11
__interrupt void Interrupt_IRQ11(void)
#endif
{
  /* Call the user function */
  if (rpdl_INTC_callback_func[PDL_INTC_IRQ11] != PDL_NO_FUNC)
  {
    rpdl_INTC_callback_func[PDL_INTC_IRQ11]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_IRQ12
__fast_interrupt void Interrupt_IRQ12(void)
#else
#pragma vector = PDL_INTC_VECTOR_IRQ12
__interrupt void Interrupt_IRQ12(void)
#endif
{
  /* Call the user function */
  if (rpdl_INTC_callback_func[PDL_INTC_IRQ12] != PDL_NO_FUNC)
  {
    rpdl_INTC_callback_func[PDL_INTC_IRQ12]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_IRQ13
__fast_interrupt void Interrupt_IRQ13(void)
#else
#pragma vector = PDL_INTC_VECTOR_IRQ13
__interrupt void Interrupt_IRQ13(void)
#endif
{
  /* Call the user function */
  if (rpdl_INTC_callback_func[PDL_INTC_IRQ13] != PDL_NO_FUNC)
  {
    rpdl_INTC_callback_func[PDL_INTC_IRQ13]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_IRQ14
__fast_interrupt void Interrupt_IRQ14(void)
#else
#pragma vector = PDL_INTC_VECTOR_IRQ14
__interrupt void Interrupt_IRQ14(void)
#endif
{
  /* Call the user function */
  if (rpdl_INTC_callback_func[PDL_INTC_IRQ14] != PDL_NO_FUNC)
  {
    rpdl_INTC_callback_func[PDL_INTC_IRQ14]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_IRQ15
__fast_interrupt void Interrupt_IRQ15(void)
#else
#pragma vector = PDL_INTC_VECTOR_IRQ15
__interrupt void Interrupt_IRQ15(void)
#endif
{
  /* Call the user function */
  if (rpdl_INTC_callback_func[PDL_INTC_IRQ15] != PDL_NO_FUNC)
  {
    rpdl_INTC_callback_func[PDL_INTC_IRQ15]();
  }
}

#pragma vector = 0
__interrupt void Interrupt_BRK(void)
{
  uint32_t vector_location;
  uint8_t * saved_ipl_location;
  uint8_t temp;
  switch (rpdl_INTC_brk_command)
  {
    case BRK_START_ADC_10:
      rpdl_ADC_10_Start();
      break;
    case BRK_START_ADC_10_AND_SLEEP:
      rpdl_ADC_10_Start();
      /* Prevent all-module clock stop */
      SYSTEM.MSTPCRA.BIT.ACSE = 0;
      /* Select sleep or all-module clock stop */
      SYSTEM.SBYCR.BIT.SSBY = 0;
      /* Prevent out-of-order exection */
      while (SYSTEM.SBYCR.BIT.SSBY == 1);
      __wait_for_interrupt();
      break;
    case BRK_SLEEP:
      /* Prevent all-module clock stop */
      SYSTEM.MSTPCRA.BIT.ACSE = 0;
      /* Select sleep or all-module clock stop */
      SYSTEM.SBYCR.BIT.SSBY = 0;
      /* Prevent out-of-order exection */
      while (SYSTEM.SBYCR.BIT.SSBY == 1);
      __wait_for_interrupt();
      break;
    case BRK_ALL_MODULE_CLOCK_STOP:
      /* Select sleep or all-module clock stop */
      SYSTEM.SBYCR.BIT.SSBY = 0;
      /* Prevent out-of-order exection */
      while (SYSTEM.SBYCR.BIT.SSBY == 1);
      __wait_for_interrupt();
      break;
    case BRK_STANDBY:
      /* Prevent deep standby mode */
      SYSTEM.DPSBYCR.BIT.DPSBY = 0;
      /* Select standby mode */
      SYSTEM.SBYCR.BIT.SSBY = 1;
      /* Prevent out-of-order exection */
      while (SYSTEM.SBYCR.BIT.SSBY == 0);
      __wait_for_interrupt();
      break;
    case BRK_DEEP_STANDBY:
      /* Select deep standby mode */
      SYSTEM.DPSBYCR.BIT.DPSBY = 1;
      /* Select standby mode */
      SYSTEM.SBYCR.BIT.SSBY = 1;
      /* Prevent out-of-order exection */
      while (SYSTEM.SBYCR.BIT.SSBY == 0);
      __wait_for_interrupt();
      break;
    case BRK_LOAD_FINTV_REGISTER:
      /* Find the start of the maskable interrupt vector table */
      vector_location = __get_interrupt_table();
      /* Move to the location of the address of the selected interrupt */
      vector_location += ICU.FIR.BIT.FVCT;
      /* Store the address of this vector in the FINTV register */
      __set_FINTV_register((__fast_int_f)vector_location);
      break;
    case BRK_WRITE_IPL:
      /* Where was the PSW put? */
      saved_ipl_location = (uint8_t *)rpdl_INTC_saved_isp;
      /* Move to the IPL byte location */
      saved_ipl_location -= 1;
      /* Read the saved IPL */
      temp = *saved_ipl_location;
      /* Clear the IPL bits */
      temp &= (INV_BIT_2 & INV_BIT_1 & INV_BIT_0);
      /* Modify the IPL bits */
      temp |= rpdl_INTC_brk_data8;
      /* Modify the saved IPL */
      *saved_ipl_location = temp;
      break;
    default:
      /* This must never occur */
      break;
  }
  rpdl_INTC_brk_command = BRK_NO_COMMAND;
}

/* Exception (Privileged Instruction) */
__interrupt void __privileged_handler(void)
{
  if (rpdl_INTC_callback_func[PDL_INTC_PRIVILEGED] != PDL_NO_FUNC)
  {
    /* Call the user function */
    rpdl_INTC_callback_func[PDL_INTC_PRIVILEGED]();
  }
}

/* Exception (Undefined Instruction) */
__interrupt void __undefined_handler(void)
{
  if (rpdl_INTC_callback_func[PDL_INTC_UNDEFINED] != PDL_NO_FUNC)
  {
    /* Call the user function */
    rpdl_INTC_callback_func[PDL_INTC_UNDEFINED]();
  }
}

__interrupt void __NMI_handler(void)
{
  if (rpdl_INTC_callback_func[PDL_INTC_NMI] != PDL_NO_FUNC)
  {
    /* Call the user function */
    rpdl_INTC_callback_func[PDL_INTC_NMI]();
  }

  /* The NMI interrupt is used to trap fatal faults, so we do not return */
  while(1);
}

/* End of file */
