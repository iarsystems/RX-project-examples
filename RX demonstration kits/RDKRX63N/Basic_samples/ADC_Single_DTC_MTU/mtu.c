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
 * mtu.c
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
#include "yrdkrx63n.h"
#include "cgc.h"


/******************************************************************************
   Function     : InitMTU3
   Description   : Initialises MTU2_3 to generate a 50ms interrupt.
                  The interrupt is generated on a TGRA Compare Match
                  The compare match value is calculated by the macro: 
                  ( 50 * (PCLKB_FREQUENCY / (64L * 1000L) ) ) - 1
                  where 50 = 50ms
                        PCLKB_FREQUENCY is defined in cgc.h
                        64 = the MTU clock divide value, set in TCR register
                        1000 = because we want ms
                        - 1 as the counter starts to count from 0
   Argument    : none
   Return value : none
******************************************************************************/
void InitMTU3(void)
{
  /* Take TPU Unit 0 out of Module Stop Mode                               */
  MSTP(MTU3) = 0;
  
  /* MTU3 setting for LED timimg                                           */
  MTU3.TCR.BYTE = 0x23;       /* Clear TCNT when TGRA match, P/64 phi      */
  MTU3.TMDR.BYTE = 0;         /* Normal mode                               */
  
  MTU3.TCNT = 0;
  MTU3.TGRA = ( 50 * (PCLKB_FREQUENCY / (64L * 1000L) ) ) - 1;
  
  /* Enable Interrupt                                                      */
  MTU3.TIER.BYTE = 0x01;
  
  /* Set up the Interrupt Control Unit for MTU3 TGRA                       */
  IPR(MTU3,TGIA3) = 0x01;     /* Interrupt Priority Register               */
                              /*   0: lowest (interrupt will not be taken
                                   7: highest                              */
  
  IEN(MTU3,TGIA3) = 1;        /* Interrupt Request Enable Register         */
  IR(MTU3,TGIA3) = 0;         /* Ensure that the flag is set to 0,         */
                              /* otherwise the interrupt will not be       */ 
                              /* accepted                                  */
}


/******************************************************************************
   Function     : StartMTU3
   Description   : Sets the TSTR bit to start the counter running
   Argument    : none
   Return value : none
******************************************************************************/
void StartMTU3(void)
{
  MTU.TSTR.BIT.CST3 = 1;      
}


/******************************************************************************
   Function     : StopMTU3
   Description   : Clears the TSTR bit to stop the counter running
   Argument    : none
   Return value : none
******************************************************************************/
void StopMTU3(void)
{
  MTU.TSTR.BIT.CST3 = 0;      
}


/******************************************************************************
   Function     : Excep_TPU9_TGIA9
   Description   : MTU3 TGIA ISR
   Argument    : none
   Return value : none
******************************************************************************/
#pragma vector = 152
__interrupt void Excep_TPU9_TGIA9(void)
{
  static unsigned long Count=0;
  
  Count++;
  
  if(Count == 10)
  {
    LED4 = LED_OFF;
  }
  
  if(Count == 20)
  {
    LED5 = LED_OFF;
  }
  
  if(Count == 30)    
  {
    LED6 = LED_OFF;
  }
  
  if(Count == 40)
  {
    LED7 = LED_OFF;
  }
  
  if(Count == 50)
  {
    LED8 = LED_OFF;
  }
  
  if(Count == 60)    
  {
    LED9 = LED_OFF;
  }  
  
  if(Count == 70)    
  {
    LED10 = LED_OFF;
  }
  
  if(Count == 80)    
  {
    LED11 = LED_OFF;
  }
  
  if(Count == 90)    
  {
    LED12 = LED_OFF;
  }
  
  if(Count == 100)    
  {
    LED13 = LED_OFF;
  }
  
  if(Count == 110)    
  {
    LED14 = LED_OFF;
  }
  
  if(Count == 120)
  {
    LED4 = LED_ON;
  }
  
  if(Count == 130)
  {
    LED5 = LED_ON;
  }
  
  if(Count == 140)    
  {
    LED6 = LED_ON;
  }
  
  if(Count == 150)
  {
    LED7 = LED_ON;
  }
  
  if(Count == 160)
  {
    LED8 = LED_ON;
  }
  
  if(Count == 170)    
  {
    LED9 = LED_ON;
  }  
  
  if(Count == 180)    
  {
    LED10 = LED_ON;
  }
  
  if(Count == 190)    
  {
    LED11 = LED_ON;
  }
  
  if(Count == 200)    
  {
    LED12 = LED_ON;
  }
  
  if(Count == 210)    
  {
    LED13 = LED_ON;
  }
  
  if(Count == 220)    
  {
    LED14 = LED_ON;
    Count = 0;
  }
  
  LED15 = 1 - LED15;
}

/******************************************************************************
   Function     : InitMTU0
   Description   : Initialises MTU2_0 to generate a 5ms interrupt.
                  The interrupt is generated on a TGRA Compare Match
                  The compare match value is calculated by the macro:
                  ( 5 * (PCLKB_FREQUENCY / (64L * 1000L) ) ) - 1
                  where 5 = 5ms
                        PCLKB_FREQUENCY is defined in cgc.h
                        64 = the MTU clock divide value, set in TCR register
                        1000 = because we want ms
                        - 1 as the counter starts to count from 0
   Argument    : none
   Return value : none
******************************************************************************/
void InitMTU0(void)
{
  /* Take TPU Unit 0 out of Module Stop Mode                              */
  MSTP(MTU0) = 0;
  
  
  /* MTU0 setting for ADC Trigger by Compare Match                        */
  MTU0.TCR.BYTE = 0x23;        /* Clear TCNT when TGRA match, P/64 phi    */
  MTU0.TMDR.BYTE = 0;          /* Normal mode                             */
  
  MTU0.TCNT = 0;
  MTU0.TGRA = ( 5 * (PCLKB_FREQUENCY / (64L * 1000L) ) ) - 1;
  MTU0.TIER.BIT.TTGE = 1;
}

/******************************************************************************
   Function     : StartMTU0
   Description   : Sets the TSTR bit to start the counter running
   Argument    : none
   Return value : none
******************************************************************************/
void StartMTU0(void)
{
  MTU.TSTR.BIT.CST0 = 1;
}

/******************************************************************************
   Function     : StopMTU0
   Description   : Clears the TSTR bit to stop the counter running
   Argument    : none
   Return value : none
******************************************************************************/
void StopMTU0(void)
{
  MTU.TSTR.BIT.CST0 = 0;
}
