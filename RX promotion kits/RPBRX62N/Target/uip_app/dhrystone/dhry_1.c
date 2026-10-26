/* Adapted for use with IAR Embedded Workbench */
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
* File Name     : dhry_1.c
* Version       : 1.0
* Device(s)     : R5F56108VNFP
* Tool-Chain    : RX Family C Compiler
* OS            : None
* H/W Platform  : RX-Stick
* Description   : Dhrystone benchmark program adapted from Reinhold P. Weiker's
*                 version 2.1 dated May 25, 1998.
* Operation     : Download and run the program.  When the Dhrystone is complete,
*                 the display shows "OK" or "!" indicating that the benchmark
*                 ran properly or that an error was detected.  Press the "Stop"
*                 button in the debugger and add the variables:
*
*                     Dhrystones_Per_Second
*                     DMIPS_per_MHz
*
*                 to the watch window to see the benchmark results.  Results
*                 will be roughly 1.66 DMIPS/Second.  Run the test multiple times.
*
*                 Note that while the Dhrystone is running, the DMAC is running
*                 in the background continuously refreshing the LED display.
*                 Unique peripherals like the RX's DMAC, DTC, EXDMAC and EDMAC
*                 allow complex data transfers to occur between peripherals and
*                 memory with no impact on performance.  The Dhrystone numbers
*                 of competing MCU's would be negatively impacted by attempting
*                 to refresh the display during the benchmark.
* Limitations   : None
*******************************************************************************
* History : DD.MM.YYYY  Version Description
*         : 12.05.2010  1.00    First release
*******************************************************************************/

/******************************************************************************
 Include files
******************************************************************************/
#include <string.h>
#include "dhry.h"
#include "hwsetup.h"
#include <stdlib.h>
#include <machine.h>

#include "clock-arch.h"

clock_time_t clock_time(void);

/******************************************************************************
 Global variables from original Dhrystone code
******************************************************************************/

Rec_Pointer     Ptr_Glob, Next_Ptr_Glob;
int             Int_Glob;
Boolean         Bool_Glob;
char            Ch_1_Glob, Ch_2_Glob;
int             Arr_1_Glob [50];
int             Arr_2_Glob [50] [50];

/******************************************************************************
 Function prototypes
******************************************************************************/
#ifndef REG
Boolean Reg = false;
#define REG
/* REG becomes defined as empty */
/* i.e. no register variables   */
#else
Boolean Reg = true;
#endif

void Proc_1 (Rec_Pointer );
void Proc_2 (One_Fifty   *);
void Proc_3 (Rec_Pointer *);
void Proc_4 (void);
void Proc_5 (void);
extern void Proc_6 (Enumeration , Enumeration *);
extern void Proc_7 (One_Fifty , One_Fifty, One_Fifty *);
extern void Proc_8 (Arr_1_Dim, Arr_2_Dim, int, int );
extern Enumeration     Func_1 (Capital_Letter,Capital_Letter );
extern Boolean         Func_2 (Str_30, Str_30);


/******************************************************************************
 Variables for performance measurement
******************************************************************************/
float           Dhrystones_Per_Second;

/* ICLK = 96 MHz */
/* PCLK = 48 MHz*/
#define ICLK_MHZ 96.0
#define PCLK_MHZ 48.0

uint32_t        pclk_cycles ;       /* Counts number of PCLK cycles that have elapsed */
float           elapsed_seconds ;   /* Number of seconds to run Dhrystone test */
float           DMIPS_per_MHz ;     /* DMIPS/MHz result*/

/******************************************************************************
 Bitmap images for the LED display
 1 bit per LED: 0 = LED on, 1 = off
 Each row on the display is mapped to a 16-bit value: LSB on the left
 Top row is row 0.
******************************************************************************/
const unsigned short rx_image[SCR_HGHT] =           // "RX" image
{
  0x0E60, 0x0E40, 0x040C, 0x209C, 0x318C, 0x31C0, 0x20E0, 0x0444, 0x0E0C, 0x0E1C
};

const unsigned short ok_image[SCR_HGHT] =           // "OK" image
{
  0x0CC1, 0x0CC1, 0x249C, 0x309C, 0x389C, 0x389C, 0x309C, 0x249C, 0x0CC1, 0x0CC1
};

const unsigned short error_image[SCR_HGHT] =           // Error image - exclamation point
{
  0x3f3f, 0x3e1f, 0x3e1f, 0x3e1f, 0x3e1f, 0x3f3f, 0x3f3f, 0x3fff, 0x3f3f, 0x3f3f
};

/******************************************************************************
* Function name : init_dry
* Description   : initializes global variables used
*                 to communicate dhrystone results
* Arguments     : none
* Return Value  : none
******************************************************************************/
char g_szDhrystone_msg[128];   /* For the dhrystone result web page */

void init_dry(void)
{
   sprintf(g_szDhrystone_msg, "[0] Running Dhrystone... wait...");
}

/******************************************************************************
* Function name : run_dry
* Description   : dhrystone benchmark main entry
* Arguments     : none
* Return Value  : none
******************************************************************************/
void run_dry (char *msg, int n)
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
  clock_time_t    start, end;
  int             errcode=0;
  static int      invocation=0;

  /* Initializations */
  Next_Ptr_Glob = (Rec_Pointer) malloc (sizeof (Rec_Type));
  if (Next_Ptr_Glob == NULL)
  {
    sprintf(msg, "Dhrystone: internal error. Out of memory (%d).", sizeof (Rec_Type));
  return;
  }

  Ptr_Glob = (Rec_Pointer) malloc (sizeof (Rec_Type));
  if (Ptr_Glob == NULL)
  {
  free(Next_Ptr_Glob);
    sprintf(msg, "Dhrystone: internal error. Out of memory (%d).", sizeof (Rec_Type));
  return;
  }

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
  start = clock_time();

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

  end = clock_time();

  /* Seconds = number of ticks at 48 MHz PCLK */
  elapsed_seconds = ((float)(end - start)) / (float)CLOCK_CONF_SECOND;  /* 10 Hz clock */

  /* Free aloocated space: we rerun Dhrystone and if not freed */
  /* at some invocation later in the web session we'll run out */
  /* of space                                                  */
  free(Ptr_Glob);
  free(Next_Ptr_Glob);

  /* Dhrystone sanity checks */

  /* Variable errcode is added for debug purposes in     */
  /* the unlikely case of Dhrystone throwing up errors.  */
  errcode=1;
  if (Int_Glob != 5) goto err ;
  errcode=2;
  if (Bool_Glob != 1) goto err ;
  errcode=3;
  if (Ch_1_Glob != 'A') goto err ;
  errcode=4;
  if (Ch_2_Glob != 'B') goto err ;
  errcode=5;
  if (Arr_1_Glob[8] != 7) goto err ;
  errcode=6;
  if (Arr_2_Glob[8][7] != (Number_Of_Runs + 10)) goto err ;
  errcode=7;
  if (Ptr_Glob->Discr != 0) goto err ;
  errcode=8;
  if (Ptr_Glob->variant.var_1.Enum_Comp != 2) goto err ;
  errcode=9;
  if (Ptr_Glob->variant.var_1.Int_Comp != 17 ) goto err ;
  errcode=10;
  if (strcmp("DHRYSTONE PROGRAM, SOME STRING", Ptr_Glob->variant.var_1.Str_Comp) != 0) goto err ;
  errcode=11;
  if (Next_Ptr_Glob->Discr != 0) goto err ;
  errcode=12;
  if (Next_Ptr_Glob->variant.var_1.Enum_Comp != 1) goto err ;
  errcode=13;
  if (Next_Ptr_Glob->variant.var_1.Int_Comp != 18) goto err ;
  errcode=14;
  if (strcmp("DHRYSTONE PROGRAM, SOME STRING", Next_Ptr_Glob->variant.var_1.Str_Comp)) goto err ;
  errcode=15;
  if (Int_1_Loc != 5) goto err ;
  errcode=16;
  if (Int_2_Loc != 13) goto err ;
  errcode=17;
  if (Int_3_Loc != 7) goto err ;
  errcode=18;
  if (Enum_Loc != 1) goto err ;
  errcode=19;
  if (strcmp("DHRYSTONE PROGRAM, 1'ST STRING", Str_1_Loc) != 0) goto err ;
  errcode=20;
  if (strcmp("DHRYSTONE PROGRAM, 2'ND STRING", Str_2_Loc) != 0) goto err ;

  errcode=0;

  /* All the sanity checks passed, now calculate Dhrystone values */

  /* Now compute benchmark results (examine these in EW) */
  Dhrystones_Per_Second = (float) Number_Of_Runs / elapsed_seconds ;
  DMIPS_per_MHz = Dhrystones_Per_Second / (1757.0 * ICLK_MHZ) ;

  /* Don't loop forever, return to web server     */
  /* Format a string to tell webserver the result */
  sprintf(msg, "[#%d] Dhrystone: %.0f, DMIPS per Mhz %f", ++invocation, Dhrystones_Per_Second, DMIPS_per_MHz);
  return;

  /* If there was an error, an exclamation point is displayed */
err:

  /* Don't loop forever, return to web server     */
  /* Format a string to tell webserver the result */
  sprintf(msg, "Dhrystone: internal error. Calculations don't return the expected result [%d].", errcode);
  return;
}

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

