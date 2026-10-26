/* Adapted for use with IAR Embedded Workbench */
/*
 * Copyright (c) 2006, Technical University of Munich
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
 * \file AVR specific implementation of multithreading architecture
 *
 * \author Adam Dunkels <adam@sics.se>
 * \author Simon Barner <barner@in.tum.de>
 *
 * @(#)$Id$
 */

/*******************************************************************************
* Copyright (C) 2010 Renesas Electronics Corporation. All rights reserved.    */ 
/*******************************************************************************
* File Name    : mtarch.c
* Version     : 1.00
* Device     : R5F562N8
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSK+RX62N
* Description   : Low level Multi Tasking
*                 architecture dependent functions
*                 to support the High Level Multi
*                 Tasking Framework in mt.c
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

#include <stdio.h>

#include "rskrx62ndef.h"
#include "mt.h"
#include "clock-arch.h"

/* Indexes into mtarch_thread->stack array */
#define IDX_USP     0
#define IDX_R1      1
#define IDX_R2      2
#define IDX_R3      3
#define IDX_R4      4
#define IDX_R5      5
#define IDX_R6      6
#define IDX_R7      7
#define IDX_R8      8
#define IDX_R9      9
#define IDX_R10    10
#define IDX_R11    11
#define IDX_R12    12
#define IDX_R13    13
#define IDX_R14    14
#define IDX_R15    15
#define IDX_PC     16
#define IDX_PSW    17
#define IDX_IER    18
#define NUMBER_OF_IDX_REGISTER 19

/* Forward definitions of function prototypes */
static void  mtarch_timer_zaptimeslice(void);
static void  mtarch_timer_init(void);

static struct mtarch_thread *g_mtRunning = NULL;

/*--------------------------------------------------------------------------*/
void mtarch_init(struct mtarch_thread *t)
{
  g_mtRunning = t;
   
  mtarch_timer_init();
}

/*--------------------------------------------------------------------------*/
static void mtarch_wrapper(void)
{
  /* Call thread function with argument */
  ((void (*)(void *))g_mtRunning->function)((void*)g_mtRunning->data);
  /* After completing the user task, fall through to: */
  mt_exit();
}

/*--------------------------------------------------------------------------*/
void mtarch_start(struct mtarch_thread *t, void (*function)(void *), void *data)
{
  int i;

  for(i = 0; i < MTARCH_STACKSIZE; ++i) {
    t->stack[i] = i;
  }

  t->sp = &t->stack[MTARCH_STACKSIZE - 1];

  *t->sp = (unsigned int)mt_exit;
  --t->sp;

  *t->sp = (unsigned int)mtarch_wrapper;
  --t->sp;

  /* Fill in our saved registers block. At start, only    */
  /* USP, initial PSW, and saved return address (PC) are  */
  /* interesting.                                         */
  t->stack[IDX_USP] = (unsigned int)t->sp;
  for (i=1; i<16; i++) t->stack[i] = 0;             /* R1-R15  */
  t->stack[IDX_PC]  = (unsigned int)mtarch_wrapper;
  t->stack[IDX_PSW] = 0x30000;  
  t->stack[IDX_IER] = 1;
  
  /* Store function and argument (used in mtarch_wrapper) */
  t->data     = data;
  t->function = function;
}

/*--------------------------------------------------------------------------*/

/* From the RX62N manual: */

/* 10.5.6 Interrupts

   1. The value of the processor status word (PSW) is saved on the stack (ISP) or,
      for the fast interrupt, in the backup PSW register (BPSW).
   2. The processor mode select bit (PM), the stack pointer select bit (U), and 
      the interrupt enable bit (I) in the PSW register are cleared to 0.
   3. If the interrupt was generated during the execution of an RMPA, SCMPU, SMOVB,
      SMOVF, SMOVU, SSTR, SUNTIL, or SWHILE instruction, the value of the program 
    counter (PC) for that instruction is saved. 
    For other instructions, the PC value of the next instruction is saved. 
    Saving of the PC is in the backup PC (BPC) for fast interrupts.
   4. The processor interrupt priority level bits (IPL[3:0]) in the PSW register 
      indicate the interrupt priority level of the interrupt.
   5. The address of the processing routine for an interrupt source other than the 
      fast interrupt is fetched from the relocatable vector table. For the fast
    interrupt, the address is fetched from the fast interrupt vector register (FINTV).
   6. The PC is set to the fetched address and processing branches to the start of
      the exception handler.
    
   10.6 Return from Exception Processing Routines
   
   Executing the instructions listed in table 10.3 at the end of the corresponding
   exception processing routines restores the values of the program counter (PC)
   and processor status word (PSW) that were saved on the stack or in the control
   registers (BPC and BPSW) immediately before the exception processing sequence.
   
   Table 10.3 Return from Exception Processing Routines
   
   Exception                            Instruction for Return
   Undefined instruction exception      RTE
   Privileged instruction exception     RTE
   Floating-point exceptions            RTE
   Reset                                Return is impossible
   Non-maskable interrupt               Return is impossible
   Interrupts Fast interrupt            RTFI
              Other than the above      RTE
   Unconditional trap                   RTE
   
*/


#if DBGLED
/* Used to create a flashing LED1 */
static unsigned char g_led1_onoff = 0;
#endif

/* To tell who 'owns' the LEDs */
extern int g_use_by_LED_control;

/* To read CMT1 interrupt enable bit */
static volatile int g_iIntBit  = 1;

#include "r_pdl_cmt.h"

#if FAST_INTC_VECTOR == VECT_CMT1_CMI1
__fast_interrupt void Interrupt_CMT12(void)
#else
#pragma vector = VECT_CMT1_CMI1
__interrupt void Interrupt_CMT12(void)
#endif
{
/* Save all registers */
  asm("PUSHM       R1-R15");\
  asm("PUSHC       USP");
/* Send stack pointer as parameter to Interrupt_CMT12_C */
/* The stack contain all save registers */
  asm("MOV.L       R0,R1");  
  asm("BSR         _Interrupt_CMT12_C");
/* Restore all register */
  asm("POPC        USP");  
  asm("POPM        R1-R15");
}

void Interrupt_CMT12_C(unsigned int *allsavedregs)
{
   unsigned int i;
   
   /* Store registers form stack into struct */
   for (i = 0;i < NUMBER_OF_IDX_REGISTER;i++)
     g_mtRunning->stack[i] = allsavedregs[i];

#if DBGLED
   /* If LED1 not used by web app, show activity */
   if (!g_use_by_LED_control)
   {
      /* Activity LED blinking */
      g_led1_onoff ^= 1;

      if( g_led1_onoff == 0)
      {
         LED1 = LED_OFF;
      }
      else
      {
         LED1 = LED_ON;
      }
   }
#endif

   /* Save the current state of the Multi Tasking */
   /* timer interrupt enable bit.                 */
   g_mtRunning->stack[IDX_IER] = IEN(CMT1, CMI1);
   
   /* Higher level function that selects */
   /* another task (if any) to run       */
   mt_swap_tasks(&g_mtRunning);

   /* This code is here to fix a nasty problem.      */
   /* When trying to disable timer interrupts, there */
   /* is a short period after accessing the IER bit  */
   /* where interrupts are not enabled yet. If the   */
   /* interrupt occurs just in this period, we're in */
   /* trouble: the task has just disabled interrupts */
   /* yet we're in the task switching interrupt      */
   /* routine... We deal with such situations by     */
   /* saving the IER bit, and restoring it when the  */
   /* task gets rescheduled again. It needs to be    */
   /* restored, because the original task may be     */
   /* trying to disable the IER bit, so we don't     */
   /* want to reschedule the task with IER enabled.  */
   do
   {
     IEN(CMT1, CMI1) = g_mtRunning->stack[IDX_IER];
     g_iIntBit = IEN(CMT1, CMI1);
   } while (g_iIntBit != g_mtRunning->stack[IDX_IER]);

   /* Restore struct registers to stack  */
   for (i = 0;i < NUMBER_OF_IDX_REGISTER;i++)
     allsavedregs[i] = g_mtRunning->stack[i];
}

/*--------------------------------------------------------------------------*/
void mtarch_yield(void)
{
  /* Can't do just a taskswitch here, as     */
  /* we're not on the ISP (interrupt) stack  */
  /* Instead we simply generate an interrupt */
  mtarch_timer_zaptimeslice();
}

/*--------------------------------------------------------------------------*/
int mtarch_stack_usage(struct mt_thread *t)
{
  uint16_t i;
  for(i = 0; i < MTARCH_STACKSIZE; ++i) {
    if(t->thread.stack[i] != i) {
      break;
    }
  }
  return MTARCH_STACKSIZE - i;
}

/**
 * MT Framework timer tick initialization
 *
 * This function initializes a CMT1 timer interrupt with 100ms tick. 
 *
 */
static void mtarch_timer_init(void)
{

    /** 
    * Compare Match Timer Unit 1 is used
    * generate 100 ms (10 Hz) system tick.  
    */
    
    /* Count stop */
    CMT.CMSTR0.BIT.STR1 = 0x0;   /* 0: CMT1.CMCNT count is stopped */       
    /* Pclock/512 */
    CMT1.CMCR.WORD = 0x0043;             
    /* Timer counter clear */
    CMT1.CMCNT = 0x0000;         
    /* 100ms */
    CMT1.CMCOR = (PCLK/CMT0_CLK_SELECT) / CLOCK_CONF_SECOND;         
    /* Count start */
    CMT.CMSTR0.BIT.STR1 = 0x1;   /* 1: CMT1.CMCNT count is started */        

    /* Interrupt priority level = 1 */  
    /* Enable ICU for CMT1 interrupts */
    IPR(CMT1, CMI1) = 1;
    IEN(CMT1, CMI1) = 1;
}

/* rej09b0552_rx62nhm_rev.0.50_with_rev_history.pdf */
/* From section 5.1 of the RX62N manual: we need to */
/* be careful accessing external registers such as  */
/* the interrupt enable registers:                  */
/*                                                  */
/* <quote>                                          */
/* When writing to an I/O register, the CPU starts  */
/* executing the subsequent instruction before      */
/* completing I/O register write. This may cause    */
/* the subsequent instruction to be executed before */
/* the post-update I/O register value is reflected  */
/* on the operation.                                */
/* As described in the following examples, special  */
/* care is required for the cases in which the      */
/* subsequent instruction must be executed after    */
/* the post-update I/O register value is actually   */
/* reflected.                                       */
/* [Examples of cases requiring special care]       */
/* • The subsequent instruction must be executed    */
/* while an interrupt request is disabled with the  */
/* IENj bit in IERm of the ICU (interrupt request   */
/* enable bit) cleared to 0.                        */
/* In the above cases, after writing to an I/O      */
/* register, wait until the write operation is      */
/* completed using the following procedure and then */
/* execute the subsequent instruction.              */
/* (a) Write to an I/O register.                    */
/* (b) Read the value from the I/O register to a    */
/* general register.                                */
/* (c) Execute the operation using the value read.  */
/* (d) Execute the subsequent instruction.          */
/* </quote>                                         */
/*                                                  */
/* Therefore, in our case, we read a IEN register   */
/* back after having written to it, to avoid this   */
/* problem.                                         */

/* Disable task switching timer tick */
void disable_tasktimertick(void)
{
  volatile int iBit=0;

  do
  {
      IEN(CMT1, CMI1) = 0;
    /* Beware: timer interrupt may occur here! */
    iBit = IEN(CMT1, CMI1);
  } while (iBit!=0);
}

/* Disable task switching timer tick */
void enable_tasktimertick(void)
{
  volatile int iBit=0;

  do
  {
      IEN(CMT1, CMI1) = 1;
    /* Beware: timer interrupt may occur here! */
    iBit = IEN(CMT1, CMI1);
  } while (iBit!=1);
}

/**
 * mtarch_timer_zaptimeslice
 *
 * This function sets the counter to CMCOR, thus 
 * generating an immediate timer interrupt. 
 *
 */

static void mtarch_timer_zaptimeslice(void)
{
  volatile int iCount=0, i=0;
  
  enable_tasktimertick();

    /* Timer counter clear */
    CMT1.CMCNT = (PCLK/CMT0_CLK_SELECT) / CLOCK_CONF_SECOND;
  iCount = CMT1.CMCNT;
  
  /* Timer interrupt will happen here, yielding */
  /* to another task.                           */
  for (i=0; i<32; i++)
  {
    iCount++;
  }
  
  /* When this task gets scheduled again, it will */
  /* start in the partially completed loop above. */
}

