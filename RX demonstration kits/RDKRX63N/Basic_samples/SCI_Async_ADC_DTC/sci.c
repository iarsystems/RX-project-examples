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
#include "mtu.h"


/******************************************************************************
    Variables
******************************************************************************/
struct _Tx Tx;
struct _Rx Rx;


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

  /* Ensure SCR is all 0 before configuration                           */
  SCI2.SCR.BYTE = 0;

  /* Setting of Transmission/Reception Format
     ASYNC Mode, 
     8bit data, 
     No parity bit, 
     1 stop bit, 
     P/1 clock                                                          */
  SCI2.SMR.BYTE = 0;

  /* Setup I/O pins based upon Tx and Rx request                        */
  /* Setup I/O for TX pin                                               */
  if(TXState == TX_ENABLE)  
  {
    /* NOTE: Modify the MPC value before setting the PMR bit            */
    MPC.P50PFS.BIT.PSEL = 10;
    PORT5.PMR.BIT.B0 = 1;  
  }
  
  /* Setup I/O for RX pin                                               */
  if(RXState == RX_ENABLE)  
  {
    /* NOTE: Modify the MPC value before setting the PMR bit            */
    MPC.P52PFS.BIT.PSEL = 10;
    PORT5.PMR.BIT.B2 = 1;  
  }
  
  /* Setting of LSB First                                               */
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
  
  /* Wait for SCI Setting (1 bit cycle)                                 */
  for(i=0 ; i< (( PCLKB_FREQUENCY / SelectedBaud ) +1 ); i++);
  
  /* Clear all status flags.                                            */
  SCI2.SSR.BYTE = 0x00;           
  
  if(RXState == RX_ENABLE)
  {
    IEN(SCI2,RXI2) = 1;         /* SCI2 RXI2 Enable bit                    */
    IPR(SCI2,RXI2) = 0x02;      /* SCI2 RXI2 Priority                      */  
    
    IR(SCI2,RXI2) = 0;          /* clear IR bits, just in case             */
    
    SCI2.SCR.BIT.RE = 1;        /* Enable RX Pin                           */
    SCI2.SCR.BIT.RIE = 1;       /* Enable RX Interrupt                     */
  }
  
  if(TXState == TX_ENABLE)
  {
    IEN(SCI2,TXI2) = 1;         /* SCI2 TXI2 Enable bit                    */
    IEN(SCI2,TEI2) = 1;         /* SCI2 TEI2 Enable bit                    */
    
    IPR(SCI2,TXI2) = 2;         /* TXI2 & TEI2 share the same priority bit */
    
    IR(SCI2,TXI2) = 0;          /* clear IR bits, just in case             */
    IR(SCI2,TEI2) = 0;          /* clear IR bits, just in case             */    
  }          
}


/******************************************************************************
Function Name:  WriteString
Description:    Transmits data via the SCI2 port
Parameters:     pointer to a string
Return value:   none
******************************************************************************/
void WriteString( char *str )
{
  unsigned char temp;
  
  while( Tx.Busy == 1);
  
  strcpy( (char*)&Tx.Data[0], str );  /* Copy the data to a local copy    */
  Tx.Length = strlen( str );          /* calculate the number of bytes to */
                                      /* transmit.                        */
  Tx.Index = 0;
  Tx.Busy = 1;                        /* We are now transmitting          */
                                      /* so set as busy                   */  
  
  /* Read the current SCR value to see if RIE, RE, MPIE, CKE bits are set */
  temp = SCI2.SCR.BYTE & 0x53 ;
  
  /* Set the TIE & TE bits simultaneously and maintain the                */
  /* values of RIE, RE, MPIE and CKE[1:0]                                 */
  /* The simultaneously setting will generate the TXI interrupt.          */
  /* Loading of data into the TDR is done in the TXI ISR                  */
  SCI2.SCR.BYTE = 0xa0 | temp; 
}

/******************************************************************************
Function Name:  WriteByte
Description:    Transmits data via the SCI2 port
Parameters:     pointer to a string
Return value:   none
******************************************************************************/
void WriteByte( char data )
{
  unsigned char temp;
  
  while( Tx.Busy == 1);
  
  Tx.Data[0] = data;                  /* Copy the data to a local copy    */
  Tx.Length = 1;                      /* Number of bytes to transmit      */                              
  Tx.Index = 0;
  Tx.Busy = 1;
  
  /* Read the current SCR value to see if RIE, RE, MPIE, CKE bits are set */
  temp = SCI2.SCR.BYTE & 0x53 ;
  
  /* Set the TIE & TE bits simultaneously and maintain the                */
  /* values of RIE, RE, MPIE and CKE[1:0]                                 */
  /* The simultaneously setting will generate the TXI interrupt.          */
  /* Loading of data into the TDR is done in the TXI ISR                  */
  SCI2.SCR.BYTE = 0xa0 | temp; 
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
  if( Tx.Index != Tx.Length )
  {
    SCI2.TDR = Tx.Data[ Tx.Index ];
    while(SCI2.SSR.BIT.TEND == 1);
    Tx.Index++;        
  }
  else
  {
    /* else enable the TEI interrupt which will be taken */
    SCI2.SCR.BYTE |= 0x04;
  }
}

/******************************************************************************
Function Name:  Transmission End Interrupt
Description:    When data has been clocked out of the Transmit Shift Register
Parameters:     none
Return value:   none
******************************************************************************/
#pragma vector = 222
__interrupt void Excep_SCI2_TEI2(void)
{
  /* If we are here then all data has been transmitted, so            */
  /* clear the TE, TIE, TEIE bits.                                    */
  /* They will be set by the next transmission                        */
  SCI2.SCR.BYTE &= ~0xa4;
  
  Tx.Busy = 0;
}


/******************************************************************************
Function Name:  Reception Interrupt
Description:    When data has been been received by the RDR
Parameters:     none
Return value:   none
******************************************************************************/
#pragma vector = 220
__interrupt void Excep_SCI2_RXI2(void)
{
  unsigned char Buffer;
  
  Buffer = SCI2.RDR;
  
  if( (Buffer == 'g') || (Buffer == 'G') )
  {
    StartMTU0();
  }
  else if( (Buffer == 's') || (Buffer == 'S') )
  {
    StopMTU0();
  }
  else
  {
  }
}
