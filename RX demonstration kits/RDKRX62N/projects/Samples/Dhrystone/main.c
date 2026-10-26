/******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Electronics Corp. and is
* only intended for use with Renesas products.

* No other uses are authorized.

* This software is owned by Renesas Electronics Corp. and is
* protected under the applicable laws, including copyright laws.

* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY,
* INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH
* WARRANTIES ARE EXPRESSLY DISCLAIMED.

* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER
* RENESAS ELECTRONICS CORP. NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR AND DIRECT, INDIRECT, SPECIAL, INCIDENTAL
* OR COSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE,
* EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE
* POSSIBILITIES OF SUCH DAMAGES.

* Renesas reserves the right, without notice, to make changes to this
* software and to discontinue availability of this software.
* By using this software, you agree to the additional terms and
* conditions found by accessing the following link:
* http://www.renesas.com/disclaimer
*******************************************************************************/
/* Copyright (C) 2010. Renesas Electronics Corp., All Rights Reserved         */
/******************************************************************************
* File Name     : main.c
* Version       : 1.0
* Device(s)     : R5F562N8
* Tool-Chain    : RX Family C Compiler
* OS            : None
* H/W Platform  : RDK+RX62N
* Description   : Main program for RX Performance Lab
*
* Operation     : Run the application and watch the Dhrystone score 
*                 on the LCD display.
*******************************************************************************
* History : DD.MMM.YYYY   Version   Description
*         : 10.Jul.2010   1.00      First release
*******************************************************************************/

/******************************************************************************
Include files
******************************************************************************/
#include "platform.h"
#include "machine.h"
#include <iorx62n.h>
#include "lcd.h"
#include <stdio.h>
#include "timer.h"
#include "s12adc.h"
#include "hwsetup.h"
#include <string.h>
#include "dhry.h"
#include "rx_dhry.h"

/******************************************************************************
Global variables from original Dhrystone code
******************************************************************************/

Rec_Pointer     Ptr_Glob, Next_Ptr_Glob;
int             Int_Glob;
Boolean         Bool_Glob;
char            Ch_1_Glob, Ch_2_Glob;
int             Arr_1_Glob [50];
int             Arr_2_Glob [50] [50];

Boolean Func_2 (Str_30  Str_1_Par_Ref, Str_30  Str_2_Par_Ref);

void Proc_7 (One_Fifty Int_1_Par_Va,
             One_Fifty Int_2_Par_Val,
             One_Fifty *Int_Par_Ref);

void Proc_8 (Arr_1_Dim Arr_1_Par_Ref,
             Arr_2_Dim Arr_2_Par_Ref,
             int Int_1_Par_Val,
             int Int_2_Par_Val);

void Proc_6 (Enumeration  Enum_Val_Par, Enumeration *Enum_Ref_Par);

/******************************************************************************
Local function prototypes
******************************************************************************/
void main(void);

extern char     *malloc ();
/* forward declaration necessary since Enumeration may not simply be int */
Enumeration     Func_1 ();

#ifndef REG
Boolean Reg = false;
#define REG
/* REG becomes defined as empty */
/* i.e. no register variables   */
#else
Boolean Reg = true;
#endif

void Proc_1 (REG Rec_Pointer Ptr_Val_Par);
void Proc_2 (One_Fifty   *Int_Par_Ref);
void Proc_3 (Rec_Pointer *Ptr_Ref_Par);
void Proc_4 ();
void Proc_5 ();

/******************************************************************************
Variables for performance measurement
******************************************************************************/
float           Dhrystones_Per_Second;

/* ICLK = 96 MHz */
/* PCLK = 48 MHz*/
//#define ICLK_MHZ 96.0
#define PCLK_MHZ 48.0

uint32_t          pclk_cycles ;       /* Counts number of PCLK cycles that have elapsed */
float           elapsed_seconds ;   /* Number of seconds to run Dhrystone test */
float           DMIPS_per_MHz ;     /* DMIPS/MHz result*/

/******************************************************************************
Local data
******************************************************************************/
#define TRUE    1
#define FALSE   0

/* This buffer is to demonstrate using the DMAC to log data */
#define BUF_SIZE 5000
uint16_t adBuffer[2][BUF_SIZE] ;        /* Ping & pong buffers */

/******************************************************************************
* Function name : main
* Description   : Main program starts here
* Arguments     : None
* Return Value  : Should not return!
******************************************************************************/
void main(void)
{

  One_Fifty       Int_1_Loc;
  REG   One_Fifty Int_2_Loc;
  One_Fifty       Int_3_Loc;
  REG   char      Ch_Index;
  Enumeration     Enum_Loc;
  Str_30          Str_1_Loc;
  Str_30          Str_2_Loc;
  REG   int       Run_Index;
  REG   int       Number_Of_Runs;
  char alcdbuffer[12] ;                 /* Display buffer for LCD */

  HardwareSetup();

  /* Enable Interrupt */
  __enable_interrupt();


  /* Initialize display */
  InitialiseDisplay();
  DisplayString(LCD_LINE1,"Dhrystone");
  DisplayString(LCD_LINE2,"Running ") ;

  /* Setup the 12-bit ADC */
  //S12ADC_Init(&adBuffer[0][0], &adBuffer[1][0], BUF_SIZE) ;

  /* Start recording ADC readings to buffers */
  //S12ADC_Start() ;

  /* Initializations */
  Next_Ptr_Glob = (Rec_Pointer) malloc (sizeof (Rec_Type));
  Ptr_Glob      = (Rec_Pointer) malloc (sizeof (Rec_Type));
  Ptr_Glob->Ptr_Comp                    = Next_Ptr_Glob;
  Ptr_Glob->Discr                       = Ident_1;
  Ptr_Glob->variant.var_1.Enum_Comp     = Ident_3;
  Ptr_Glob->variant.var_1.Int_Comp      = 40;
  strcpy (Ptr_Glob->variant.var_1.Str_Comp, "DHRYSTONE PROGRAM, SOME STRING");
  strcpy (Str_1_Loc, "DHRYSTONE PROGRAM, 1'ST STRING");

  /*
  Was missing in published program. Without this statement,
  Arr_2_Glob [8][7] would have an undefined value.
  Warning: With 16-Bit processors and Number_Of_Runs > 32000,
  overflow may occur for this array element.
  */
  Arr_2_Glob [8][7] = 10;

  /*
  The Dhrystone benchmark should run for over 2 seconds for accuracy.
  An RX running at 96 MHz completes 1,000,000 Dhrystones in 3-4 seconds.
  */
  Number_Of_Runs = 1000000;

  /***************/
  /* Start timer */
  /***************/
  TimerInit() ;

  /* Light up the red LED's */
  PORTD.DR.BYTE = 0x38;
  PORTE.DR.BYTE = 0xFE;

  /* Start channels TPU1 and TPU2 */
  TimerStart() ;

  /******************************************************************************
  Dhrystone loop starts here!
  ******************************************************************************/
  for (Run_Index = 1; Run_Index <= Number_Of_Runs; ++Run_Index)
  {

    Proc_5();
    Proc_4();
    /* Ch_1_Glob == 'A', Ch_2_Glob == 'B', Bool_Glob == true */
    Int_1_Loc = 2;
    Int_2_Loc = 3;
    strcpy (Str_2_Loc, "DHRYSTONE PROGRAM, 2'ND STRING");
    Enum_Loc = Ident_2;
    Bool_Glob = ! Func_2 (Str_1_Loc, Str_2_Loc);
    /* Bool_Glob == 1 */
    while (Int_1_Loc < Int_2_Loc)  /* loop body executed once */
    {
      Int_3_Loc = 5 * Int_1_Loc - Int_2_Loc;
      /* Int_3_Loc == 7 */
      Proc_7 (Int_1_Loc, Int_2_Loc, &Int_3_Loc);
      /* Int_3_Loc == 7 */
      Int_1_Loc += 1;
    } /* while */
    /* Int_1_Loc == 3, Int_2_Loc == 3, Int_3_Loc == 7 */
    Proc_8 (Arr_1_Glob, Arr_2_Glob, Int_1_Loc, Int_3_Loc);
    /* Int_Glob == 5 */
    Proc_1 (Ptr_Glob);
    for (Ch_Index = 'A'; Ch_Index <= Ch_2_Glob; ++Ch_Index)
      /* loop body executed twice */
    {
      if (Enum_Loc == Func_1 (Ch_Index, 'C'))
        /* then, not executed */
      {
        Proc_6 (Ident_1, &Enum_Loc);
        strcpy (Str_2_Loc, "DHRYSTONE PROGRAM, 3'RD STRING");
        Int_2_Loc = Run_Index;
        Int_Glob = Run_Index;
      }
    }
    /* Int_1_Loc == 3, Int_2_Loc == 3, Int_3_Loc == 7 */
    Int_2_Loc = Int_2_Loc * Int_1_Loc;
    Int_1_Loc = Int_2_Loc / Int_3_Loc;
    Int_2_Loc = 7 * (Int_2_Loc - Int_3_Loc) - Int_1_Loc;
    /* Int_1_Loc == 1, Int_2_Loc == 13, Int_3_Loc == 7 */
    Proc_2 (&Int_1_Loc);
    /* Int_1_Loc == 5 */

  } /* loop "for Run_Index" */
  /******************************************************************************
  Dhrystone loop end
  ******************************************************************************/

  /**************/
  /* Stop timer */
  /**************/

  /* Stop TPU1 and TPU2 to freeze end time */
  pclk_cycles = TimerStop() ;

  /* Seconds = number of ticks at 48 MHz PCLK */
  elapsed_seconds = (1.0 / (PCLK_MHZ * 1.0E6)) * (float) pclk_cycles ;

  /* Dhrystone sanity checks */
  if (Int_Glob  != 5  ) goto err ;
  if (Bool_Glob != 1  ) goto err ;
  if (Ch_1_Glob != 'A') goto err ;
  if (Ch_2_Glob != 'B') goto err ;
  if (Arr_1_Glob[8] != 7) goto err ;
  if (Arr_2_Glob[8][7] != (Number_Of_Runs + 10)) goto err ;
  if (Ptr_Glob->Discr != 0) goto err ;
  if (Ptr_Glob->variant.var_1.Enum_Comp !=  2) goto err ;
  if (Ptr_Glob->variant.var_1.Int_Comp  != 17) goto err ;
  if (strcmp("DHRYSTONE PROGRAM, SOME STRING", Ptr_Glob->variant.var_1.Str_Comp) != 0) goto err ;
  if (Next_Ptr_Glob->Discr != 0) goto err ;
  if (Next_Ptr_Glob->variant.var_1.Enum_Comp !=  1) goto err ;
  if (Next_Ptr_Glob->variant.var_1.Int_Comp  != 18) goto err ;
  if (strcmp("DHRYSTONE PROGRAM, SOME STRING", Next_Ptr_Glob->variant.var_1.Str_Comp)) goto err ;
  if (Int_1_Loc !=  5) goto err ;
  if (Int_2_Loc != 13) goto err ;
  if (Int_3_Loc !=  7) goto err ;
  if (Enum_Loc  !=  1) goto err ;
  if (strcmp("DHRYSTONE PROGRAM, 1'ST STRING", Str_1_Loc) != 0) goto err ;
  if (strcmp("DHRYSTONE PROGRAM, 2'ND STRING", Str_2_Loc) != 0) goto err ;

  /* All the sanity checks passed, now calculate Dhrystone values */

  /* Now compute benchmark results (examine these in HEW) */
  Dhrystones_Per_Second = (float) Number_Of_Runs / elapsed_seconds ;
  DMIPS_per_MHz = Dhrystones_Per_Second / (1757.0 * ICLK_MHZ) ;

  /* Light up the green LED's */
  PORTD.DR.BYTE = ~0x38;
  PORTE.DR.BYTE =  0xF1;

  DisplayString(LCD_LINE1, "Dhrystone");
  DisplayString(LCD_LINE2, "        ") ;
  DisplayString(LCD_LINE3, "        ") ;
  DisplayString(LCD_LINE4, "        ") ;
  DisplayString(LCD_LINE5, "        ") ;

  sprintf(alcdbuffer, "%5.4f DMIPs", DMIPS_per_MHz) ;
  DisplayString(LCD_LINE2, alcdbuffer) ;
  DisplayString(LCD_LINE3, "     per MHz") ;

  sprintf(alcdbuffer,"%5.4f secs", elapsed_seconds) ;
  DisplayString(LCD_LINE5, alcdbuffer) ;

  while (1)
  {
  }

  /* If there was an error, an exclamation point is displayed */
err:
  DisplayString(LCD_LINE1, "Dhrystone");
  DisplayString(LCD_LINE2, "ERROR!  ") ;

  while (1)
  {
  }


}

#if 0
/******************************************************************************
* Function name : TimerInit
* Description   : Sets up a 32-bit cascaded timer on TPU1 & TPU2 running @ PCLK
* Arguments     : none
* Return Value  : none
******************************************************************************/
void TimerInit (void)
{
  /*
  Create a free running counter based on PCLK (48 MHz)
  A 32-bit counter is created by cascading TPU channels TPU1 & TPU2
  */

  /* Power up TPU unit 0 */
  MSTP(MTU1) = 0 ;

  /* TPU1 is set to count falling edges of TPU2 overflows */

  /* Timer Control Register (TCR)
  b7:b5   CCLR - TCNT counter cleared by counter clearing for another channel
  b4:b3   CKEG - Count at falling edge
  b2:b0   TPSC - Counts on TPU2.TCNT overflow/underflow
  */
  MTU1.TCR.BYTE = 0x67  ;

  /* Timer Mode Register (TMDR)
  b7      ICSELD - Input capture source is TIOCDn pin
  b6      ICSELB - Input capture source is TIOCBn pin
  b5      BFB - TPUm.TGRB operates normally
  b4      BFA - TPUm.TGRA operates normally
  b3:b0   MD - Normal operation
  */
  MTU1.TMDR.BYTE = 0x00 ;

  /* Timer I/O Control Register (TIOR)
  b7:b4   IOB - Output compare, output disabled
  b3:b0   IOA - Output compare, output disabled
  */
  MTU1.TIOR.BYTE = 0x00 ;

  /* Timer Interrupt Enable Register (TIER)
  b7      TTEG - A/D conversion start request generation disabled
  b6      reserved
  b5      TCIEU - Underflow interrupts disabled
  b4      TCIEV - Overflow interrupts disabled
  b3      TGIED - TGRD interrupt disabled
  b2      TGIEC - TGRC interrupt disabled
  b1      TGIEB - TGRB interrupt disabled
  b0      TGIEA - TGRA interrupt disabled
  */
  MTU1.TIER.BYTE = 0x00 ;

  /* TPU2 is set to count falling edges of PCLK/1 (48 MHz) */

  /* Timer Control Register (TCR)
  b7:b5   CCLR - TCNT counter cleared by counter clearing for another channel
  b4:b3   CKEG - Count at falling edge
  b2:b0   TPSC - Internal clock: counts on PCLK/1
  */
  MTU2.TCR.BYTE = 0x60  ;

  /* Timer Mode Register (TMDR)
  b7      ICSELD - Input capture source is TIOCDn pin
  b6      ICSELB - Input capture source is TIOCBn pin
  b5      BFB - TPUm.TGRB operates normally
  b4      BFA - TPUm.TGRA operates normally
  b3:b0   MD - Normal operation
  */
  MTU2.TMDR.BYTE = 0x00 ;

  /* Timer I/O Control Register (TIOR)
  b7:b4   IOB - Output compare, output disabled
  b3:b0   IOA - Output compare, output disabled
  */
  MTU2.TIOR.BYTE = 0x00 ;

  /* Timer Interrupt Enable Register (TIER)
  b7      TTEG - A/D conversion start request generation disabled
  b6      reserved
  b5      TCIEU - Underflow interrupts disabled
  b4      TCIEV - Overflow interrupts disabled
  b3      TGIED - TGRD interrupt disabled
  b2      TGIEC - TGRC interrupt disabled
  b1      TGIEB - TGRB interrupt disabled
  b0      TGIEA - TGRA interrupt disabled
  */
  MTU2.TIER.BYTE = 0x00 ;

  /* Timer Synchronous Register (TSYRA)
  b7:b6   ---
  b5      SYNC5 - TCNT operates independently
  b4      SYNC4 - TCNT operates independently
  b3      SYNC3 - TCNT operates independently
  b2      SYNC2 - TCNT performs synchronous operation
  b1      SYNC1 - TCNT performs synchronous operation
  b0      SYNC0 - TCNT operates independently
  */
  MTUA.TSYR.BYTE = 0x06 ;   /* Sync TPU1 and TPU 2*/

  /* Clear counter register for both TPU channels synchronously */
  MTU1.TCNT = 0x0000 ;
}
#endif

/******************************************************************************
The functions below are from the original Dhrystone code
******************************************************************************/
void Proc_1 (Ptr_Val_Par)
/******************/

REG Rec_Pointer Ptr_Val_Par;
/* executed once */
{
  REG Rec_Pointer Next_Record = Ptr_Val_Par->Ptr_Comp;
  /* == Ptr_Glob_Next */
  /* Local variable, initialized with Ptr_Val_Par->Ptr_Comp,    */
  /* corresponds to "rename" in Ada, "with" in Pascal           */

  structassign (*Ptr_Val_Par->Ptr_Comp, *Ptr_Glob);
  Ptr_Val_Par->variant.var_1.Int_Comp = 5;
  Next_Record->variant.var_1.Int_Comp
    = Ptr_Val_Par->variant.var_1.Int_Comp;
  Next_Record->Ptr_Comp = Ptr_Val_Par->Ptr_Comp;
  Proc_3 (&Next_Record->Ptr_Comp);
  /* Ptr_Val_Par->Ptr_Comp->Ptr_Comp
  == Ptr_Glob->Ptr_Comp */
  if (Next_Record->Discr == Ident_1)
    /* then, executed */
  {
    Next_Record->variant.var_1.Int_Comp = 6;
    Proc_6 (Ptr_Val_Par->variant.var_1.Enum_Comp,
      &Next_Record->variant.var_1.Enum_Comp);
    Next_Record->Ptr_Comp = Ptr_Glob->Ptr_Comp;
    Proc_7 (Next_Record->variant.var_1.Int_Comp, 10,
      &Next_Record->variant.var_1.Int_Comp);
  }
  else /* not executed */
    structassign (*Ptr_Val_Par, *Ptr_Val_Par->Ptr_Comp);
} /* Proc_1 */


void Proc_2 (Int_Par_Ref)
/******************/
/* executed once */
/* *Int_Par_Ref == 1, becomes 4 */

One_Fifty   *Int_Par_Ref;
{
  One_Fifty  Int_Loc;
  Enumeration   Enum_Loc;

  Int_Loc = *Int_Par_Ref + 10;
  do /* executed once */
  if (Ch_1_Glob == 'A')
    /* then, executed */
  {
    Int_Loc -= 1;
    *Int_Par_Ref = Int_Loc - Int_Glob;
    Enum_Loc = Ident_1;
  } /* if */
  while (Enum_Loc != Ident_1); /* true */
} /* Proc_2 */


void Proc_3 (Ptr_Ref_Par)
/******************/
/* executed once */
/* Ptr_Ref_Par becomes Ptr_Glob */

Rec_Pointer *Ptr_Ref_Par;

{
  if (Ptr_Glob != Null)
    /* then, executed */
    *Ptr_Ref_Par = Ptr_Glob->Ptr_Comp;
  Proc_7 (10, Int_Glob, &Ptr_Glob->variant.var_1.Int_Comp);
} /* Proc_3 */


void Proc_4 () /* without parameters */
/*******/
/* executed once */
{
  Boolean Bool_Loc;

  Bool_Loc = Ch_1_Glob == 'A';
  Bool_Glob = Bool_Loc | Bool_Glob;
  Ch_2_Glob = 'B';
} /* Proc_4 */


void Proc_5 () /* without parameters */
/*******/
/* executed once */
{
  Ch_1_Glob = 'A';
  Bool_Glob = false;
} /* Proc_5 */


/* Procedure for the assignment of structures,          */
/* if the C compiler doesn't support this feature       */
#ifdef  NOSTRUCTASSIGN
memcpy (d, s, l)
register char   *d;
register char   *s;
register int    l;
{
  while (l--) *d++ = *s++;
}
#endif

