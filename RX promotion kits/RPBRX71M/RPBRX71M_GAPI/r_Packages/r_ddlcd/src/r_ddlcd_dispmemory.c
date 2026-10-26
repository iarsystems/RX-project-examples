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
/**********************************************************************************
 FILE NAME    : r_ddlcd_dispmemory.c

 DESCRIPTION  : Display Memory Manager.
 The Display memory is located in internal SDRAM. If accessed by other
 tasks at the same time as the DMA channel is transferrig data to the
 panel the image being displayed will be disrupted.

 This task coordinates the access to the display memory, only allowing
 registered tasks to access the memory while in the v-blank period.

 Copyright   : 2016 Renesas Electronics America
 Copyright   : 2016 Renesas Electronics Corporation.
 All Rights Reserved
 /**********************************************************************************/

#include "r_ddlcd_public.h"
#include "r_ddlcd_private.h"
#include "r_FreeRTOS.h"
#include "r_ddlcd_dispmemory.h"

static void DispMemoryMonitor (void * pvParameters);      // The task of DispMemory Monitor
static xTaskHandle DMACHnd; /* RTOS_USAGE */

/*****************************************************************************/
//
// FUNCTION    :DispMemoryMonitorInit
// PARAMETERS  :init
// RETURNS     :void
//
// DESCRIPTION : Initialize external memory system
//
//
/*****************************************************************************/
void R_DDLCD_DispMemoryInit (void)
{
    // Create DispMemoryMonitor task
    (void) xTaskCreate(DispMemoryMonitor, "a:DMAC", configMINIMAL_STACK_SIZE, NULL, DD_DISPMEMORY_MANAGER_PRIORITY,
            &DMACHnd); /* RTOS_USAGE */
}

/*****************************************************************************/
//
//  FUNCTION    TASK_DD_isr()
//  PARAMETERS  : None (ISR)
// 
// RETURNS     : None
//        
// DESCRIPTION :This task calls the scheduler to wake the task that blocks other
//              tasks that are accessing RAM. If RAM access tasks are not blocked,
//              the CPU will stall during DMA block transfers.
//              The IRQ source is a TMR channel that is configured to interrupt once/frame
//              at the end of the vertical blanking time.
//
/*****************************************************************************/
#pragma vector=DD_TMR_ICU(VECT,DOTCLK)
__interrupt __nested void R_DDLCD_DispMemory_isr (void)
{
    // Clear interupt and request to interupt
    DD_TMR_ICU(IEN,DOTCLK)= 0;
    (void)xTaskResumeFromISR( DMACHnd ); /* RTOS_USAGE */
    portYIELD_FROM_ISR( pdTRUE ); /* RTOS_USAGE */
}

/*****************************************************************************/
//
// FUNCTION    :DispMemoryMonitor
// PARAMETERS  :(void *)pvParameters
// RETURNS     :
//
// DESCRIPTION :
//
//
/*****************************************************************************/
static xTaskHandle DispMemoryList[DD_DISPMEMORY_MANAGER_MAX_TASKS]; /* RTOS_USAGE */

static void DispMemoryMonitor (void *pvParameters)
{
    uint8_t index;

    // Initialize list of tasks to suspend on data transfer
    for (index = 0; index < DD_DISPMEMORY_MANAGER_MAX_TASKS; index++)
    {
        DispMemoryList[index] = NULL;
    }

    for (;;)
    {
        if (0 != DDLCD_BusActive)
        {
            // Suspend tasks currently using external memory
            for (index = 0; index < DD_DISPMEMORY_MANAGER_MAX_TASKS; index++)
            {
                if (NULL != DispMemoryList[index])
                    vTaskSuspend(DispMemoryList[index]); /* RTOS_USAGE */
            }
        }
        else
        {
            /* Resume tasks currently using Display Memory */
            for (index = 0; index < DD_DISPMEMORY_MANAGER_MAX_TASKS; index++)
            {
                if (NULL != DispMemoryList[index])
                {
                    vTaskResume(DispMemoryList[index]); /* RTOS_USAGE */
                }
            }
        }
        vTaskSuspend( NULL); // Suspend ourselves. /* RTOS_USAGE */
    }
}

/*****************************************************************************/
//
// FUNCTION    :R_DDLCD_DispMemoryAcquire
// PARAMETERS  :xTaskHandle handle
// RETURNS     :
//
// DESCRIPTION :Lets the system know that this task is currently using the external memory.
//              The idea being that when the ExDMA block transfer is started, that we suspend
//              this task until the next Vertical Blanking period.
//
/*****************************************************************************/
void R_DDLCD_DispMemoryAcquire (xTaskHandle handle)
{
    uint8_t index;
    _Bool return_flag = false;

    for (;;)
    {
        /* Search for an open slot */
        for (index = 0; index < DD_DISPMEMORY_MANAGER_MAX_TASKS; index++)
        {
            taskENTER_CRITICAL()
            ; /* RTOS_USAGE */
            if (NULL == DispMemoryList[index])
            {
                // Success, store and return
                DispMemoryList[index] = handle;
                return_flag = true;
            }
            taskEXIT_CRITICAL()
            ; /* RTOS_USAGE */
            if (return_flag)
            {
                // If the bus is already busy, we must manually suspend ourselves,
                // The DispMemory Monitor task will resume us.
                if (0 != DDLCD_BusActive)
                    vTaskSuspend( NULL); /* RTOS_USAGE */
                return;
            }
        }
        // No open slot found...we must wait
        taskYIELD()
        ; /* RTOS_USAGE */
    }
}

/*****************************************************************************/
//
// FUNCTION    :R_DDLCD_DispMemoryRelease
// PARAMETERS  :xTaskHandle handle
// RETURNS     :
//
// DESCRIPTION :Let's the system know that this task has completed using the external memory.
//
/*****************************************************************************/
void R_DDLCD_DispMemoryRelease (xTaskHandle handle)
{
    uint8_t index;

    // Search for our slot and set to NULL
    for (index = 0; index < DD_DISPMEMORY_MANAGER_MAX_TASKS; index++)
    {
        if (DispMemoryList[index] == handle)
            DispMemoryList[index] = NULL;
    }
}

