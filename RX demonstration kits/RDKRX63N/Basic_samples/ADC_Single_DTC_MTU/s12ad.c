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


/*
 * s12ac.c
 *
 *  Created on: 01 Oct 2011
 *      Author: RJW
 *              Reneses Electronics Europe Ltd
 */
 
/******************************************************************************
    System Includes                                                           
******************************************************************************/
#include <iorx63n.h>


/******************************************************************************
    User Includes                                                             
******************************************************************************/ 
#include "s12ad.h"


/******************************************************************************
   Function     : Init_S12AD_SingleMode
   Description   : Initialises the 12-bit ADC for Single-cycle scan mode, 
                  Channel AN002 is to be converted
                  The End of Conversion Interrupt is Enabled, but the 
                  DTC Enable bit is set so that the DTC takes the interrupt                   
   Argument    : none
   Return value : none
******************************************************************************/
void InitS12ADSingleModeMTU(void)
{
  MSTP(S12AD) = 0;            /* Take out of module stop mode                */
  
  S12AD.ADCSR.BIT.ADCS = 0;   /* Conversion Mode
                                   0: Single-cycle scan mode
                                   1: Continuous scan mode                   */
  
  S12AD.ADCSR.BIT.CKS = 3;    /* Conversion Clock Select
                                   0: PCLK/8
                                   1: PCLK/4
                                   2: PCLK/2
                                   3: PCLK                                   */
  
  S12AD.ADCSR.BIT.EXTRG = 0;  /* A/D conversion is started by the 
                                 synchronous trigger (MTU, TPU, or TMR)      */
  
  S12AD.ADCSR.BIT.TRGE = 1;   /* Enables A/D conversion to be started by 
                                 a synchronous or asynchronous trigger       */
  
  S12AD.ADSTRGR.BIT.ADSTRS = 1;  
  /* 1 =  IC/CM from MTU0 Synchronous trigger                                */                
  
  S12AD.ADCER.BIT.ADRFMT = 0; /* ADDRn Format Select Register
                                   0: A/D data is padded at the LSB end.
                                   1: A/D data is padded at the MSB end.     */
  
  S12AD.ADANS0.BIT.ANS0 = 4;  /* Select AN002                                */
  
  S12AD.ADCSR.BIT.ADIE = 1;   /* Interrupt enabled                           */
  
  /* Set up the Interrupt Control Unit for AD ADI                            */
  IPR(S12AD,S12ADI0) = 3;    /* Interrupt Priority Register                 */
                              /*    0: lowest (interrupt will not be taken
                                   15: highest                               */
  
  IEN(S12AD,S12ADI0) = 1;    /* Interrupt Request Enable Register           */
  IR(S12AD,S12ADI0) = 0;     /* Ensure that the flag is set to 0, otherwise
                                 the interrupt will not be accepted          */
  
  /* Setup the DTC to take the S12AD ISR                                     */
  DTCE(S12AD,S12ADI0) = 1;
  
  
  MPC.P42PFS.BIT.ASEL = 1;    /* Port4.2 is an analogue pin                  */
  
  PORT4.PMR.BIT.B2 = 1;       /* Port4.2 is NOT a general I/O pin            */
}

void Start_S12AD(void)
{
  S12AD.ADCSR.BIT.ADST = 1;
}

void Stopt_S12AD(void)
{
  S12AD.ADCSR.BIT.ADST = 0;
}
