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

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "hwsetup.h"
/* Defines machine level functions used in this file */
#include    <machine.h>
/* Defines standard variable types used in this file */
#include    <stdbool.h>
#include    <stdint.h>
#include    "r_bsp_config.h"

/***********************************************************************************************************************
* Function name: operating_frequency_set
* Description  : Configures the clock settings for each of the device clocks
* Arguments    : none
* Return value : none
***********************************************************************************************************************/
static void operating_frequency_set (void)
{
    /* Used for constructing value to write to SCKCR register. */
    uint32_t temp_clock = 0;

    /* 
    DEFAULT SETTINGS:
    Clock Description              Frequency
    ----------------------------------------
    Input Clock Frequency..............12MHz
    Internal Clock Frequency...........96MHz
    Peripheral Clock Frequency.........48MHz
    External Bus Clock Frequency.......12MHz */
    
    /* SCKCR - System Clock Control Register 
    b27:b23 ICK[3:0] = 0, system clock = EXTAL * 8 = 96 MHz
    b23     PSTOP1 = 1, BCLK pin output disabled
    b22     PSTOP0 = 1, SDCLK pin output disabled
    b19:b16 BCK[3:0] = 3, BCLK set to lowest multiplier (x1)to reduce power
    b11:b8  PCK[3:0] = 1, PCK = EXTAL * 4 = 48 MHz
    */

    /* Figure out setting for ICK bits. */
#if   BSP_CFG_ICK_MUL == 8
    /* Do nothing since ICK bits should be 0. */
#elif BSP_CFG_ICK_MUL == 4
    temp_clock |= 0x01000000;
#elif BSP_CFG_ICK_MUL == 2
    temp_clock |= 0x02000000;
#elif BSP_CFG_ICK_MUL == 1
    temp_clock |= 0x03000000;
#else
    #error "Error! Invalid setting for BSP_CFG_ICK_MUL in r_bsp_config.h"
#endif

    /* Figure out setting for BCK bits. */
#if   BSP_CFG_BCK_MUL == 8
    /* Do nothing since BCK bits should be 0. */
#elif BSP_CFG_BCK_MUL == 4
    temp_clock |= 0x00010000;
#elif BSP_CFG_BCK_MUL == 2
    temp_clock |= 0x00020000;
#elif BSP_CFG_BCK_MUL == 1
    temp_clock |= 0x00030000;
#else
    #error "Error! Invalid setting for BSP_CFG_BCK_MUL in r_bsp_config.h"
#endif

    /* Figure out setting for BCK bits. */
#if   BSP_CFG_PCK_MUL == 8
    /* Do nothing since BCK bits should be 0. */
#elif BSP_CFG_PCK_MUL == 4
    temp_clock |= 0x00000100;
#elif BSP_CFG_PCK_MUL == 2
    temp_clock |= 0x00000200;
#elif BSP_CFG_PCK_MUL == 1
    temp_clock |= 0x00000300;
#else
    #error "Error! Invalid setting for BSP_CFG_PCK_MUL in r_bsp_config.h"
#endif

    /* Configure PSTOP1 bit for BCLK output. */
#if   BSP_CFG_BCLK_OUTPUT == 0    
    /* Set PSTOP1 bit */
    temp_clock |= 0x00800000;
#elif BSP_CFG_BCLK_OUTPUT == 1
    /* Clear PSTOP1 bit */
    temp_clock &= ~0x00800000;
#elif BSP_CFG_BCLK_OUTPUT == 2
    /* Clear PSTOP1 bit */
    temp_clock &= ~0x00800000;
    /* Set BCLK divider bit */
    SYSTEM.BCKCR.BIT.BCLKDIV = 1;
#else
    #error "Error! Invalid setting for BSP_CFG_BCLK_OUTPUT in r_bsp_config.h"
#endif

    /* Configure PSTOP0 bit for SDCLK output. */
#if   BSP_CFG_SDCLK_OUTPUT == 0    
    /* Set PSTOP0 bit */
    temp_clock |= 0x00400000;
#elif BSP_CFG_SDCLK_OUTPUT == 1
    /* Clear PSTOP0 bit */
    temp_clock &= ~0x00400000;
#else
    #error "Error! Invalid setting for BSP_CFG_SDCLK_OUTPUT in r_bsp_config.h"
#endif

    SYSTEM.SCKCR.LONG = temp_clock;
}

__intrinsic
int __low_level_init ( void )
{
  
#ifdef BSP_CFG_NMI_ISR_CALLBACK
  /* Enable NMI interrupt if callback is configured in r_bsp_config.h */
  ICU.NMIER.BIT.NMIEN = 1;
#endif
  
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
