/*
 * lowinit.c
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

#include <iorx63n.h>
#include <intrinsics.h>
#include "hwsetup.h"

__intrinsic
int __low_level_init ( void )
{
  
  /* Switch to high-speed operation */
  operating_frequency_set();
  
  /* Configure the MCU and board hardware */
  hardware_setup();
  
  /* Enable the bus error interrupt to catch accesses to illegal/reserved areas of memory */
  /* The ISR for this interrupt can be found in vecttbl.c in the function "bus_error_isr" */
  /* Clear any pending interrupts */
  IR(BSC,BUSERR) = 0;
  /* Make this the highest priority interrupt (adjust as necessary for your application */
  IPR(BSC,BUSERR) = 0x0F; 
  /* Enable the interrupt in the ICU*/
  IEN(BSC,BUSERR) = 1; 
  /* Enable illegal address interrupt in the BSC */
  BSC.BEREN.BIT.IGAEN = 1;
    
  /*==================================*/
  /* Choose if segment initialization */
  /* should be done or not.           */
  /* Return: 0 to omit seg_init       */
  /*         1 to run seg_init        */
  /*==================================*/
  return (1);
}
#pragma language=default
