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
* http://www.renesas.com/disclaimer *
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.    
*******************************************************************************/
/*******************************************************************************
* File Name     : S12ADC.C
* Version       : 1.0
* Description   : Simple driver for the S12ADC (12-bit A/D converter)
*                 This implementation sets up S12ADC in continuous-scan mode.
*                 It scans AN002 only. 
*                 It generates an interrupt request on conversion completion.
*                 Each scan operation is triggered by an interrupt routed from the 
*                 CMT. 
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY     Version     Description
*         : 22.09.2011     1.00        First release
*           23.01.2012                 CS revisions
*******************************************************************************/

/*******************************************************************************
Includes <System Includes>, "Project Includes"
*******************************************************************************/
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <iorx63n.h>
#include <intrinsics.h>
#include "S12ADC.h"

/******************************************************************************
Local function declarations and local global variables
******************************************************************************/

/*******************************************************************************
* Function name :  S12ADC_init
* Description  : Sets up S12ADC in continuous scan mode.
*                The potentiometer on the YRDK RX63N board is connected to AN002.
*                Interrupt requests are generated on conversion complete.
* Argument     : none
* Return value : none
*******************************************************************************/
void S12ADC_init (void)
{
  SYSTEM.PRCR.WORD = 0xA50B; /* Protect off */
  
  /* Power up the S12ADC */
  MSTP(S12AD) = 0;
  
  SYSTEM.PRCR.WORD = 0xA500; /* Protect on  */
  
  /* ADCSR: A/D Control Register  
  b7    ADST     0 a/d conversion start, Stop a scan conversion process
  b6    ADCS     1 Scan mode select, Continuous scan mode
  b5    Reserved 0 This bit is always read as 0. The write value should always be 0.
  b4    ADIE     0 Disables conversion complete IRQ to ICU
  b3:b2 CKS      0 A/D conversion clock select = PCLK/8
  b1    TRGE     0 Disables conversion to start w/ trigger
  b0    EXTRG    0 Trigger select, Scan conversion start by a timer source or software
  */
  S12AD.ADCSR.BYTE = 0x40;
  
  /* ADANS0: A/D Channel Select Register 0
  b15:b0  ANS0: Selects analog inputs of the channels AN000 to AN015 that are subjected to A/D conversion
  */
  S12AD.ADANS0.WORD = 0x0004; /* Read AN002, which is connected to the potentiometer */
  
  /* ADANS1: A/D Channel Select Register 1
  b15:b5  Reserved: These bits are always read as 0. The write value should always be 0.
  b4:b0   ANS1:     Selects analog inputs of the channels AN016 to AN020 that are subjected to A/D conversion
  */
  S12AD.ADANS1.WORD = 0x0000;
  
  /* ADADS0: A/D-converted Value Addition Mode Select Register 0
  b15:b0  ADS0: A/D-Converted Value Addition Channel Select for AN000 to AN015.
  */
  S12AD.ADADS0.WORD = 0x0000;
  
  /* ADADS1: A/D-converted Value Addition Mode Select Register 1
  b15:b5  Reserved: These bits are always read as 0. The write value should always be 0.
  b4:b0   ADS1: A/D-Converted Value Addition Channel Select for AN016 to AN020.
  */
  S12AD.ADADS1.WORD = 0x0000;
  
  /* ADADC: A/D-Converted Value Addition Count Select Register
  b1:b0   ADC: 00 = 1 time conversion (same as normal conversion)
  */
  S12AD.ADADC.BYTE = 0x00;   /* 1-time conversion */
  
  /* ADCER: A/D Control Extended Register
  b15     ADRFMT:0  Right align the data in the result registers
  b5      ACE:0 Disables automatic clearing of ADDRn after it is read
  */
  S12AD.ADCER.WORD = 0x0000;   /* Right align data, automatic clearing off. */
  
  /* ADSTRGR: A/D Start Triggger Select Register
  b7:b4   Reserved. Always read/write 0.
  b3:b0   ADSTRS: 1, Trigger on compare match A from MTU2A TRG1N TRG0AN_0
  */
  // S12AD.ADSTRGR.BYTE = 0x01; /* Triggered by MTU2A TRG1N TRG0AN_0. */
  S12AD.ADSTRGR.BYTE = 0x00;   
} /* End of function S12ADC_init() */


/*******************************************************************************
* Function name: S12ADC_start
* Description  : Starts the ADC converter.  It will run continuously.
* Argument     : none
* Return value : none
*******************************************************************************/
void S12ADC_start (void)
{
  /* Set ADC interrupt priority in ICU and then enable interrupt */
  IPR(S12AD, S12ADI0) = 2;
  IEN(S12AD, S12ADI0) = 1;
  
  /* Start the A/D converter */
  S12AD.ADCSR.BIT.ADST = 1;
} /* End of function S12ADC_start() */


/*******************************************************************************
* Function name: S12ADC_conversion_complete 
* Description  : Checks to see if the conversion is complete.   
* Argument     : none
* Return value : true -
*                    conversion is complete 
*                false -
*                    conversion not complete 
*
* Notes        : Only works for single scan mode ADC operation.
*******************************************************************************/
bool S12ADC_conversion_complete (void)
{
  /* The ADST returns to zero when the conversion is complete  */
  return ((bool) S12AD.ADCSR.BIT.ADST == 0);
} /* End of function S12ADC_conversion_complete() */


/*******************************************************************************
* Function name: S12ADC_read
* Description  : Reads the results register for channel AN002 of the S12ADC
* Argument     : none
* Return value : adc_result - 
*                   The ADC conversion result value.
*******************************************************************************/
uint16_t S12ADC_read (void)
{
  uint16_t adc_result;
  
  do
  {
    IEN(S12AD, S12ADI0) = 0;    /* Temporarily disable interrupts. */
  } while (IEN(S12AD, S12ADI0));  /* Loop in case of pipelining latency */
  
  adc_result = S12AD.ADDR2;        /* Read the result register for AN002 */
  IEN(S12AD, S12ADI0) = 1;        /* Re-enable interrupts. */
  
  return adc_result;    
} /* End of function S12ADC_read() */


/******************************************************************************
* Function name : S12ADC_Isr
* Description   : ADC conversion complete interrupt service routine 
* Arguments     : none
* Return Value  : none
******************************************************************************/
#pragma vector = VECT_S12AD_S12ADI0
__interrupt void S12ADC_isr (void)
{
  /* Interrupts are being routed to other peripheral(e.g.:DTC or DMAC).
     Add appropriate code here otherwise. */
  __no_operation();
} /* End of S12ADC_isr */

/*******************************************************************************
End of file s12adc.c
*******************************************************************************/


