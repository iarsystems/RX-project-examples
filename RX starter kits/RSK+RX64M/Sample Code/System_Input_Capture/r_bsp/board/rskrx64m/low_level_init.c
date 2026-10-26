/*
 * low_level_init.c
 *
 * This module contains the function '__low_level_init', a function
 * that is called before the 'main' function of the program.  Normally
 * low-level initializations - such as setting the prefered interrupt
 * level or setting the watchdog - can be performed here.
 *
 * Note that this function is called before the data segments are
 * initialized, this means that this function can't rely on the
 * values of global or static variables.
 *
 * When this function returns zero, the startup code will inhibit the
 * initialization of the data segments.  The result is faster startup,
 * the drawback is that neither global nor static data will be
 * initialized.
 *
 *
 * Copyright 2009-2010 IAR Systems AB.
 *
 *          
 */

#pragma language=extended

#include "intrinsics.h"
#include "platform.h"
void hardware_setup(void);
void operating_frequency_set(void);
__intrinsic
int __low_level_init ( void )
{
  /*==================================*/
  /*  Init I/O which must be enabled  */
  /*  before segment initializations  */
  /*==================================*/

  /*==================================*/
  /* Choose if segment initialization */
  /* should be done or not.           */
  /* Return: 0 to omit seg_init       */
  /*         1 to run seg_init        */
  /*==================================*/
  /* Switch to high-speed operation */
  operating_frequency_set();





  /* Enable the bus error interrupt to catch accesses to illegal/reserved areas of memory */
  R_BSP_InterruptControl(BSP_INT_SRC_BUS_ERROR, BSP_INT_CMD_INTERRUPT_ENABLE, FIT_NO_PTR);
  return (1);
}
#pragma language=default



