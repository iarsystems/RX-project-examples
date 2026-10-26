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
 * sci.c
 *
 *  Created on: 01 Oct 2011
 *      Author: RJW
 *              Reneses Electronics Europe Ltd
 */


/******************************************************************************
    System Includes                                                           
******************************************************************************/
#include  <string.h>
#include <iorx63n.h>


/******************************************************************************
    User Includes                                                             
******************************************************************************/
#include "sci.h"
#include "cgc.h"
#include "dtc.h"


/******************************************************************************
    Variables
******************************************************************************/
struct _Tx Tx;
struct _Rx Rx;
extern struct st_dtc_xfer_full __evenaccess sciTXI_dtc_table;

/******************************************************************************
Function Name:  Init_SCI2
Description:    Initialises the SCI2 peripheral
Parameters:     none
Return value:   none
******************************************************************************/
void InitSCI2( unsigned long SelectedBaud, unsigned char TXState,  unsigned char RXState)
{
  unsigned long i;

  /* Enable SCI2 Module */
  MSTP(SCI2) = 0;

  /* Ensure SCR is all 0 before configuration                                */
  SCI2.SCR.BYTE = 0;

  /* Setting of Transmission/Reception Format
     ASYNC Mode, 
     8bit data, 
     No parity bit, 
     1 stop bit, 
     P/1 clock                                                               */
  SCI2.SMR.BYTE = 0;

  /* Setup I/O pins based upon Tx and Rx request                             */
  /* Setup I/O for TX pin                                                    */
  if(TXState == TX_ENABLE)  
  {
    /* NOTE: Modify the MPC value before setting the PMR bit                 */
    MPC.P50PFS.BIT.PSEL = 10;
    PORT5.PMR.BIT.B0 = 1;  
  }
  
  /* Setup I/O for RX pin                                                    */
  if(RXState == RX_ENABLE)  
  {
    /* NOTE: Modify the MPC value before setting the PMR bit                 */
    MPC.P52PFS.BIT.PSEL = 10;
    PORT5.PMR.BIT.B2 = 1;  
  }

  /* Setting of LSB First */
  SCI2.SCMR.BIT.SDIR = 0;

/* Setting of Bit Rate
   The equation to calculate the BRR value (N) for the stated value of
   Baud rate (B) is:

              Pclk
   N =  -----------------  - 1
        64 * 2^(2n-1) * B
        
    Here, n = 0  (CKS1 , CKS0 settings)
    
    Therefore:        

              Pclk
   N =  -----------------  - 1
             32 * B

                                                                             */

  SCI2.BRR = ( PCLKB_FREQUENCY / (32 * SelectedBaud)) - 1;

  /* Wait for SCI Setting (1 bit cycle)                                      */
  for(i=0 ; i< (( PCLKB_FREQUENCY / SelectedBaud ) +1 ); i++);

  /* Clear all status flags.                                                 */
  SCI2.SSR.BYTE = 0x00;           

  if(RXState == RX_ENABLE)
  {
    IEN(SCI2,RXI2) = 1;      /* SCI2 RXI2                                    */
    IPR(SCI2,RXI2) = 0x02;    
    IR(SCI2,RXI2) = 0;  
    SCI2.SCR.BIT.RIE = 1;    /* Transmit Data Empty Interrupt Enable         */
    SCI2.SCR.BIT.RE = 1;     /* Enable RX Pin                                */
        
    /* Configure DTC to take RXI2 interrupt */
    DTCE(SCI2,RXI2) = 1;              
  }
  
  if(TXState == TX_ENABLE)
  {
    IEN(SCI2,TXI2) = 1;  
    IPR(SCI2,TXI2) = 2;      /* SCI2 TXI2                                    */            
    IR(SCI2,TXI2) = 0;  
    
    /* Configuring DTC to take TXI2 interrupt is done as part of the         */
    /* WriteStringDTC() function (actually in ResetTXI2_ISR_DTC()).          */
  }    
}


/******************************************************************************
Function Name:  WriteStringDTC
Description:    Transmits data via the SCI2 port
Parameters:     pointer to a string
Return value:   none
******************************************************************************/
void WriteStringDTC( char *str )
{
  volatile unsigned char temp_scr;
  /* Setup the number of bytes we want to transmit         */
  /* In this exampe we are sending the data we have just   */
  /* received into the Rx.Data buffer                      */
  ResetTXI2_ISR_DTC();
  
  /* For demo purposes, overwrite the first 2 chars to     */
  /* be transmitted with a newline, carriage return to     */
  /* make the transmitted data easier to view in           */
  /* terminal window                                       */
  Rx.Data[0] = '\n';
  Rx.Data[1] = '\r';
  
  temp_scr = SCI2.SCR.BYTE & 0x53;
  SCI2.SCR.BYTE = 0xa0 | temp_scr; 
}    

/******************************************************************************
Function Name:  Transmit Data Empty Interrupt
Description:    When TDR is empty, ISR is called
Parameters:     none
Return value:   none
******************************************************************************/
#pragma vector = 221
__interrupt void Excep_SCI2_TXI2(void)
{
  SCI2.SCR.BYTE &= ~0xa0;

  SCI2.SSR.BIT.TEND = 0;    /* Clear Transmit End Flag */
}
