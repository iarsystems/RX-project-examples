/* Adapted for use with IAR Embedded Workbench */
/*
 * Copyright (c) 2006, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 *
 */

/*******************************************************************************
* Copyright (C) 2010 Renesas Electronics Corporation. All rights reserved.    */ 
/*******************************************************************************
* File Name    : mt.c
* Version     : 1.00
* Device     : R5F562N8
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSK+RX62N
* Description   : High level Multi Tasking Framework:
*                 definitions of tasks, initialisation
*                 of tasks, start/suspend/exit tasks,
* Author        : Renesas
* Comments      : This code was taken from the Contiki
*                 operating system, and changed to
*                 provide a simple task scheduler for
*                 this project. Any remaining Contiki
*                 related comments can be ignored.
******************************************************************************/
/******************************************************************************
* History     : 17.08.2010 Ver. 1.00 First Release
******************************************************************************/

#include <stdlib.h>

#include "mt.h"
#include "iodefine.h"
#include "rskrx62ndef.h"
#include "clock-arch.h"

static struct mt_thread *g_pmtCurtask = NULL;
static struct mt_thread *g_pmtLast    = NULL;
static struct mt_thread *g_pmtFirst   = NULL;
static int               g_iNumtasks  = 0;

/*--------------------------------------------------------------------------*/
void
mt_init(void)
{
   /* There's always a main process, in our case the
      uIP main/uip_main. No need to initialise this,
    the very first timer tick will fill it in. */
   static struct mt_thread maintask;

   g_pmtCurtask    = &maintask;
   g_pmtLast       = &maintask;
   g_pmtFirst      = &maintask;
   g_pmtLast->next =  g_pmtFirst;
   g_iNumtasks     =  1;
   
   mtarch_init(&maintask.thread);
}
/*--------------------------------------------------------------------------*/
void
mt_start(struct mt_thread *_pmtThread, void (* _fieFunction)(void *), void *_pvData)
{
  /* Call the architecture dependant function to set up the processor
     stack with the correct parameters. */
  mtarch_start(&_pmtThread->thread, _fieFunction, _pvData);

  _pmtThread->state = MT_STATE_SUSPENDED;
  
  /* Make sure we're not interrupted by the task tick */
  disable_tasktimertick();
  _pmtThread->next = g_pmtFirst;
  g_pmtLast->next  = _pmtThread;
  g_pmtLast        = _pmtThread;
  g_iNumtasks++;
  enable_tasktimertick();
}
/*--------------------------------------------------------------------------*/
void
mt_yield(void)
{
  if (g_pmtCurtask != NULL)
  {
    /* Disable MT timer, as we access shared globals: */
    /* the list of tasks.                             */
    disable_tasktimertick();
    /* Set the state to ready */
    g_pmtCurtask->state = MT_STATE_READY;
    /* Call the scheduler (timer tick interrupt)      */
    /* mtarch_yield will re-enable timer interrupt.   */
    mtarch_yield();
  }
}

/*--------------------------------------------------------------------------*/
void
mt_suspend(struct mt_thread *_pmtThread)
{
  if ((_pmtThread != NULL)&&(g_pmtCurtask != NULL))
  {
    /* Disable MT timer, as we access shared globals: */
    /* the list of tasks.                             */
    disable_tasktimertick();
    if (_pmtThread == g_pmtCurtask)
    {
    /* If a task suspends itself, we need to yield  */
    /* after updating the tasks's status.           */
      _pmtThread->state = MT_STATE_SUSPENDED;
      /* Call the scheduler (timer tick interrupt)    */
      /* mtarch_yield will re-enable timer interrupt. */
      mtarch_yield();
    }
    else
    {
    /* If a task suspends another task, there's no  */
    /* need to yield, because the other task isn't  */
    /* running. We do need to re-enable interrupts. */
      _pmtThread->state = MT_STATE_SUSPENDED;
      enable_tasktimertick();
    }
  }
}

/*--------------------------------------------------------------------------*/
void
mt_resume(struct mt_thread *_pmtThread)
{
  if ((_pmtThread != NULL)&&(g_pmtCurtask != NULL))
  {
    /* Disable MT timer, as we access shared globals: */
    /* the list of tasks.                             */
    disable_tasktimertick();
    /* Set the state to ready */
    if (_pmtThread != g_pmtCurtask)
    {
      _pmtThread->state = MT_STATE_READY;
    }
    enable_tasktimertick();
  }
}

/*--------------------------------------------------------------------------*/
void
mt_exit(void)
{
  /* Defensive programming. g_pmtCurtask should never be NULL, as long */
  /* as the linked list of processes (tasks) is properly linked.      */
  if (g_pmtCurtask != NULL)
  {
    /* Make sure we're not interrupted by the task tick */
    disable_tasktimertick();
    g_pmtCurtask->state = MT_STATE_EXITED;
    enable_tasktimertick();
  
  mt_yield();

    /* Defensive programming: we should never reach this */  
    while (1) {}
  }
}

/*--------------------------------------------------------------------------*/
void
mt_swap_tasks(struct mtarch_thread **_pmtarRunning)
{
   if ((g_pmtCurtask != NULL)&&(g_pmtCurtask->next != NULL))
   {
     int i=0;
     
     /* Flag this task as ready */
     g_pmtCurtask->state = MT_STATE_READY;
     /* Start looking for a new task */
     g_pmtCurtask = g_pmtCurtask->next;
     while ((g_pmtCurtask        != NULL          )&&
            (g_pmtCurtask->state != MT_STATE_READY)&&
        (i<g_iNumtasks                      )  )
     {
       if (g_pmtCurtask->next == NULL) while (1) {}
       g_pmtCurtask = g_pmtCurtask->next;
       i++;
     }
     /* Defensive programming. g_pmtCurtask should never be NULL, as long */
     /* as the linked list of processes (tasks) is properly linked.      */
     if (g_pmtCurtask != NULL)
     {
        g_pmtCurtask->state =  MT_STATE_RUNNING;
       *_pmtarRunning       = &g_pmtCurtask->thread;
     }
   }
}
/*--------------------------------------------------------------------------*/
