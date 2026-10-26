/* Adapted for use with IAR Embedded Workbench */
/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. This 
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*******************************************************************************/
/*******************************************************************************
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.    */ 
/*******************************************************************************
* File Name    : lvd.c
* Version     : 2.00
* Device     : R5F562TAADFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX62T
* Description   : Defines LVD functions       
*******************************************************************************/
/*******************************************************************************
* History     : 15.10.2010 Ver. 1.00 First Release
              : 21.05.2011 Ver. 2.00 Second Release (Updated to RPDL CS3)
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
/* Defines registers RX62T device registers */
#include <iorx62t.h>
/* IO Port RPDL function defintions */
#include "r_pdl_io_port.h"
/* CGC RPDL function defintions */
#include "r_pdl_cgc.h"
/* CMT RPDL function definitions */
#include "r_pdl_cmt.h"
/* LPC RPDL function definitions */
#include "r_pdl_lpc.h"
/* LVD RPDL function definitions */
#include "r_pdl_lvd.h"
/* INTC RPDL function definitions */
#include "r_pdl_intc.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"

/* Defines switch functions and variables used in this file */
#include "switch.h"
/* Following header file provides function prototypes for LCD controlling
   functions & macro defines */
#include "lcd.h"
/* Provides declarations of functions defined in this file */
#include "lvd.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Global variable used to signal the need for LEDs flashing synchronisation */
uint8_t gLEDsync_flg = 0x0;

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
/* Timer period interrupt callback function prototype delcaration */
void CB_CMTimer(void);
/* Low voltage detect interrupt callback function prototype delcaration */
void CB_LVD1(void);

/******************************************************************************
* Outline        : Init_LVD
* Description    : This function initialises the voltage detect circuitry to 
*          use the threshold set by VDET1. It also creates a periodic 
*          timer called every 500ms to toggle the LEDs.
* Argument    : none
* Return value  : none
******************************************************************************/
void Init_LVD(void)
{
  /* Declare error flag */
  bool err = true;
  
  /* Generate an NMI when Vdet1 is reached */
  err &= R_LVD_Control
          (
            PDL_LVD_VDET2_DISABLE_VDET1_INTERRUPT
          );

  /* Configure the NMI interrupt */
  err &= R_INTC_CreateExtInterrupt
          (
            PDL_INTC_NMI,
            PDL_INTC_FALLING | PDL_INTC_LVD_ENABLE,
            CB_LVD1,
            PDL_NO_DATA
          );
  
  /* Use the compare match timer to toggle LEDs every 500ms */
  err &= R_CMT_Create
          (
             2, 
             PDL_CMT_PERIOD, 
             500E-3, 
             CB_CMTimer, 
             7
          );
            
  /* Halt in while loop when RPDL errors detected  */
  while(!err);
}
/*******************************************************************************
* End of function Init_LVD
*******************************************************************************/

/*******************************************************************************
* Outline      : CB_CMTimer
* Description  : Timer period interrupt callback function. This function 
*         toggles the LEDs
* Argument     : none
* Return value : none
*******************************************************************************/
void CB_CMTimer(void)
{
  /* Declare error flag */
  bool err = true;

  /* Check if LEDs are in sync */
  if(gLEDsync_flg == 0x1)
  {
    /* Toggle user LEDs LED0-LED2 */
    err &=  R_IO_PORT_Modify
        (
          PDL_IO_PORT_7_1 | PDL_IO_PORT_7_2 | PDL_IO_PORT_7_3,
          PDL_IO_PORT_XOR,
          1
        );
    
    /* Reset the synchronisation flag to indicate 
         that the user LEDs are synchronised */
    gLEDsync_flg = 0x0;
  }

  /* Toggle user LEDs LED0-LED2 */
  err &=  R_IO_PORT_Modify
        (
          PDL_IO_PORT_7_1 | PDL_IO_PORT_7_2 | PDL_IO_PORT_7_3,
          PDL_IO_PORT_XOR,
          1
        );

  /* Toggle user LED LED3 */    
  err &=  R_IO_PORT_Modify
        (
          PDL_IO_PORT_3_3,
          PDL_IO_PORT_XOR,
          1
        );      
          
  /* Halt in while loop when RPDL errors detected */  
  while(!err);    
}
/*******************************************************************************
* End of function CB_CMTimer
*******************************************************************************/

/*******************************************************************************
* Outline      : CB_LVD1
* Description  : Non maskable interrupt callback function. This function simply 
*         clears the the LVD interrupt flag.
*         toggles the LEDs
* Argument     : none
* Return value : none
*******************************************************************************/
void CB_LVD1(void)
{
  /* Declare error flag */
  bool err = true;
  
  /* Stop the timer */
  err &= R_CMT_Control
        (
          2,
          PDL_CMT_STOP,
          500E-3
        );
  
  /* Wait until the power supply increases above LVD1's detection level */
  while(SYSTEM.RSTSR.BIT.LVD1F == 0x1)
  {
    /* Clear interrupt flag */
    SYSTEM.RSTSR.BIT.LVD1F = 0x0;
    
    /* Turn off LED0-LED2 */
    err &= R_IO_PORT_Write
          (
            PDL_IO_PORT_7_1|PDL_IO_PORT_7_2|PDL_IO_PORT_7_3,
            1
          );
  
    /* Turn on LED3 */
    err &= R_IO_PORT_Write
          (
            PDL_IO_PORT_3_3,
            0
          );

    /* Set the synchronisation flag to indicate 
         that the user LEDs are no longer synchronised */
    gLEDsync_flg = 0x1;
  }
  
  /* Re-start the timer */
  err &= R_CMT_Control
          (
            2,
            PDL_CMT_START,
            500E-3
          );
      
  /* Halt in while loop when RPDL errors detected */  
  while(!err);    
}
/*******************************************************************************
* End of function CB_LVD1
*******************************************************************************/
