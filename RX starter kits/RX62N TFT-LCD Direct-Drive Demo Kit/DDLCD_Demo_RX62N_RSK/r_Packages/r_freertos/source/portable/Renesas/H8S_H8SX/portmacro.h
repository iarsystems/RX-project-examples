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


#ifndef PORTMACRO_H
#define PORTMACRO_H

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------
 * Port specific definitions.  
 *
 * The settings in this file configure FreeRTOS correctly for the
 * given hardware and compiler.
 *
 * These settings should not be altered.
 *-----------------------------------------------------------
 */
 
/* Type definitions. */
#define portCHAR    char
#define portFLOAT    float
#define portDOUBLE    double
#define portLONG    long
#define portSHORT    short
#define portSTACK_TYPE  unsigned portSHORT
#define portBASE_TYPE  short

#if( configUSE_16_BIT_TICKS == 1 )
  typedef unsigned portSHORT portTickType;
  #define portMAX_DELAY ( portTickType ) 0xffff
#else
  typedef unsigned portLONG portTickType;
  #define portMAX_DELAY ( portTickType ) 0xffffffff
#endif

/*-----------------------------------------------------------*/
/* Hardware specifics. */
#define portBYTE_ALIGNMENT      2
#define portSTACK_GROWTH      ( -1 )
#define portTICK_RATE_MS      ( 1000. / configTICK_RATE_HZ )    
#define port_mS_to_TICK(time) ((portTickType) (((time) * (long)configTICK_RATE_HZ)/1000L))
#define port_TICK_to_mS(tick) ((portTickType) (((tick) * 1000L)/configTICK_RATE_HZ))

/*-----------------------------------------------------------*/
/* Scheduler support. */

/*-----------------------------------------------------------*/
/* Task utilities. */

void portDISABLE_INTERRUPTS(void);
void portENABLE_INTERRUPTS(void);
void portENTER_CRITICAL(void);
void portEXIT_CRITICAL(void);
void portPendSwitch(void);
#define portYIELD portPendSwitch
#define portSET_INTERRUPT_MASK_FROM_ISR() 0;portDISABLE_INTERRUPTS();
#define portCLEAR_INTERRUPT_MASK_FROM_ISR(x) portENABLE_INTERRUPTS();(void)x;

/* Macros to allow a context switch from within an application ISR. */
static inline void portEXIT_SWITCHING_ISR(portCHAR x)
{
  if(x != 0)
    portPendSwitch();           /* pend context switch ISR */
}

/*-----------------------------------------------------------*/

/* Task function macros as described on the FreeRTOS.org WEB site. */
#define portTASK_FUNCTION_PROTO( vFunction, pvParameters ) void vFunction( void *pvParameters )
#define portTASK_FUNCTION( vFunction, pvParameters ) void vFunction( void *pvParameters )

#ifdef __cplusplus
}
#endif

#endif /* PORTMACRO_H */

