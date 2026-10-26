/******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Electronics Corp. and is only 
* intended for use with Renesas products. No other uses are authorized.

* This software is owned by Renesas Electronics Corp. and is protected under 
* all applicable laws, including copyright laws.

* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES 
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, 
* INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
* PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY 
* DISCLAIMED.

* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORP. NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES 
* FOR ANY REASON RELATED TO THE THIS SOFTWARE, EVEN IF RENESAS OR ITS 
* AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

* Renesas reserves the right, without notice, to make changes to this 
* software and to discontinue the availability of this software.  
* By using this software, you agree to the additional terms and 
* conditions found by accessing the following link:
* http://www.renesas.com/disclaimer
******************************************************************************/
/*
  FreeRTOS.org V4.7.1 - Copyright (C) 2003-2008 Richard Barry.

  This file is part of the FreeRTOS.org distribution.

  FreeRTOS.org is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  FreeRTOS.org is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with FreeRTOS.org; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  A special exception to the GPL can be applied should you wish to distribute
  a combined work that includes FreeRTOS.org, without being obliged to provide
  the source code for any proprietary components.  See the licensing section 
  of http://www.FreeRTOS.org for full details of how and when the exception
  can be applied.

  ***************************************************************************

  Please ensure to read the configuration and relevant port sections of the 
  online documentation.

  +++ http://www.FreeRTOS.org +++
  Documentation, latest information, license and contact details.  

  +++ http://www.SafeRTOS.com +++
  A version that is certified for use in safety critical systems.

  +++ http://www.OpenRTOS.com +++
  Commercial support, development, porting, licensing and training services.

  ***************************************************************************
*/

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
extern void vTaskSwitchContext( void );
extern void *pxCurrentTCB;

/*-----------------------------------------------------------*/
/* Insert MCU/PCB specific defintions. */
#include "hwsetup.h"
#include "FreeRTOSPort.h"

/*-----------------------------------------------------------
 * Implementation of functions defined in portable.h for the H8S port.
 *----------------------------------------------------------*/
/*-----------------------------------------------------------*/

/* When the task starts interrupts should be enabled. */
#define portINITIAL_CCR      ( ( portSTACK_TYPE ) 0x00 )
#define portINITIAL_EXR      ( ( portSTACK_TYPE ) 0x00 )

/*
 * Setup TPU channel one for the RTOS tick at the requested frequency.
 */
static void prvSetupTimerInterrupt( void );

/*-----------------------------------------------------------*/

/* 
 * See header file for description. 

  word order of stack:
  sp[ 0]    e5  - extra for __asm
  sp[ 1]    r5  - extra for __asm
  sp[ 2]    e6
  sp[ 3]    r6
  sp[ 4]    e5
  sp[ 5]    r5
  sp[ 6]    e4
  sp[ 7]    r4
  sp[ 8]    e3
  sp[ 9]    r3
  sp[10]    e2
  sp[11]    r2
  sp[12]    e1
  sp[13]    r1
  sp[14]    e0
  sp[15]    r0
  sp[16]        exr/exr  - only with prioritized interrupts
  sp[17][16]    ccr/pc[hi]
  sp[18][17]    pc[low]

  All the general purpose registers - with the parameters being passed
  in ER0.  The parameter order must match that used by the compiler when the
  "saveall" function attribute is used.
*/
portSTACK_TYPE *pxPortInitialiseStack( portSTACK_TYPE *pxTopOfStack, pdTASK_CODE pxCode, void *pvParameters )
{
  /* This requires an even address. */
  pxTopOfStack = (portSTACK_TYPE *)((unsigned portLONG)pxTopOfStack & ~0x01UL);

  /* create the frame */
  pxTopOfStack -= 19;

  pxTopOfStack[14] = ( portSTACK_TYPE ) (((unsigned portLONG)pvParameters >> 16) & 0xFFFF );
  pxTopOfStack[15] = ( portSTACK_TYPE ) (((unsigned portLONG)pvParameters >>  0) & 0xFFFF );

  pxTopOfStack[16] = ((portSTACK_TYPE)portINITIAL_EXR << 8);
  pxTopOfStack[17] = ( portSTACK_TYPE ) (((unsigned portLONG)pxCode >> 16) & 0x00FF ) + ((portSTACK_TYPE)portINITIAL_CCR << 8);
  pxTopOfStack[18] = ( portSTACK_TYPE ) (((unsigned portLONG)pxCode >>  0) & 0xFFFF );

  return pxTopOfStack;
}
/*-----------------------------------------------------------*/

portBASE_TYPE xPortStartScheduler( void )
{

  /* Setup the hardware to generate the tick. */
  prvSetupTimerInterrupt();
  /* Setup the hardware to allow pended context switch */
  portConfigPendSwitch();
  /* Restore the context of the first task that is going to run.  This
  mirrors the function epilogue code generated by the compiler when the
  "saveall" function attribute is used. */
  __asm 
  {        MOV.L    @pxCurrentTCB, ER5
          MOV.L     @ER5, ER7
          MOV.L     @ER7+, ER5
          LDM.L     @SP+, (ER4-ER6)
          LDM.L     @SP+, (ER0-ER3)
          RTE
    }
  ( void ) pxCurrentTCB;

  /* Should not get here. */
  return pdTRUE;
}
/*-----------------------------------------------------------*/

void vPortEndScheduler( void )
{
  /* It is unlikely that the h8 port will get stopped. */
}

/*-----------------------------------------------------------*/
/*-----------------------------------------------------------*/
/* Function to initiate ISR_PendSwitch.
 *   This function is used to start process of a context switch
 */
void portPendSwitch(void)
{
  portSetPendSwitch();
}

/*
 * The ISR used by context switch requests. This is called as a lowest level pended interrupt.
 * This is pended by a call to portPendSwitch() defined in "FreeRTOSPort.h".
 * The resource used for this IRQ should be configured with consideration for user target (timer, irq input, etc).
 * The requirement is that the portPendSwitch() should IMMEDIATELY pend the IRQ.
 * The "saveall" function attribute is used so the context is saved by the compiler prologue.  All
 * we have to do is save the stack pointer.
 */
 
/* PEND_SWITCH_VECT is an MCU specific macro should be defined in "FreeRTOSPort.h" */
 __interrupt(vect=PEND_SWITCH_VECT) __regsave void ISR_PendSwitch( void )
{
  set_imask_exr(configMAX_SYSCALL_INTERRUPT_PRIORITY); /* prior IPL will be restored on exit */
  portClearPendSwitch();
  {
    __asm {
      /* push MAC and SBR?? */
      MOV.L  @pxCurrentTCB, ER5
      MOV.L  ER7, @ER5
    }
    vTaskSwitchContext();
    __asm {
      MOV.L  @pxCurrentTCB, ER5
      MOV.L  @ER5, ER7
    }
  }
}

/* 
 * The interrupt handler installed for the RTOS tick depends on whether the 
 * preemptive or cooperative scheduler is being used. 
 * If preemption is specified, the timer tick will also cause a context switch via portPendSwitch.
 */

/* TMR_VECT is an MCU specific macro should be defined in "FreeRTOSPort.h" */
__interrupt(vect=TMR_VECT) void vTickISR( void )
{
  /* Clear the interrupt. */
  portClearTimerFlag();      /* this MCU specific macro should be defined in "FreeRTOSPort.h" */
  set_imask_exr(configMAX_SYSCALL_INTERRUPT_PRIORITY); /* prior IPL will be restored on exit */
  vTaskIncrementTick();
#if( configUSE_PREEMPTION == 1 )
  portPendSwitch();
#endif
}

/*-----------------------------------------------------------*/

/*
 * Setup timer to generate a tick interrupt.
 */
static void prvSetupTimerInterrupt( void )
{
  /* Configure timer */
  portConfigTimer();           /* this MCU specific macro should be defined in "FreeRTOSPort.h" */
 
}

/*-----------------------------------------------------------*/
/* Task blocking utilities. */
static portCHAR portEXR;
static portBASE_TYPE uxCriticalNesting;

void portDISABLE_INTERRUPTS(void)
{
  /* because tempEXR is local, even if IRQ occurs after get_imask_exr and before set_imask_exr
  this code may be re-entered, and correctly unwound by subsequent RTE */
  portCHAR tempEXR = get_imask_exr(); 
  set_imask_exr(configMAX_SYSCALL_INTERRUPT_PRIORITY);
  if(tempEXR < configMAX_SYSCALL_INTERRUPT_PRIORITY)
  {
    portEXR=tempEXR;
  }
}

void portENABLE_INTERRUPTS(void)
{
  set_imask_exr(portEXR);
}

/* Save the EXR then disable interrupts. */
void portENTER_CRITICAL(void)
{
  portDISABLE_INTERRUPTS(); /* this call won't cause problems if already disabled */
  uxCriticalNesting++;      /* counter modification R/M/W after disable */
}  

/* Restore the EXR to set the interrupt masking back to its previous state. */
void portEXIT_CRITICAL(void)
{
  if (--uxCriticalNesting <= 0)
  {
    portENABLE_INTERRUPTS();
    uxCriticalNesting = 0;
  }
}

