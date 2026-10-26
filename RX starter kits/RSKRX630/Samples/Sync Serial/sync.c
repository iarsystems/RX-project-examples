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
/* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.   */
/*******************************************************************************
* File Name    : sync.c
* Version     : 1.00
* Device     : R5F5630E
* Tool-Chain   : Renesas RX Toolchain 1.2.0.0
* H/W Platform  : RSKRX630
* Description   : Defines sync serial functions used in this sample.
*******************************************************************************/
/*******************************************************************************
* History     : 23 Jan. 2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* System Includes
*******************************************************************************/
/* Following header file provides standard integer type definitions. */
#include <stdint.h>
/* Following header file provides string type definitions. */
#include <string.h>
/* Following header file provides Boolen type definitions */
#include <stdbool.h>

/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/
/* Defines RX630 LEDs and switches */
#include "rskrx630def.h"
/* Defines RX630 port registers */
#include "iorx630.h"
/* Defines LCD functions used in this file */
#include "lcd.h"
/* Provides declarations of functions defined in this file */
#include "sync.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Declaration of the SCI5 transmit buffer */
uint8_t g_SCI5_TXBuffer[] = "SCI5 to SCI12 Loopback Test";
/* Declaration of the SCI12 transmit buffer */
uint8_t g_SCI12_TXBuffer[] = "SCI12 to SCI5 Loopback Test";
/* Declaration of SCI5 receive buffer */
uint8_t g_SCI5_RXBuffer[28];
/* Declaration of SCI12 receive buffer */
uint8_t g_SCI12_RXBuffer[28];
/* Declaration of loopback test success flag */
volatile bool gTestPass = true;
/* Declare a SCI5 data receive counter */
uint8_t g_SCI5_RX_Counter = 0;
/* Declare a SCI12 data receive counter */
uint8_t g_SCI12_RX_Counter = 0;


/*******************************************************************************
* Outline      : Init_sync
* Description  : This function reserves and clears 28 bytes of memory for 
*         SCI5 and SCI12's receive buffers.
* Argument     : none
* Return value : none
*******************************************************************************/
void Init_Sync(void)
{
  /* Clear the SCI5 and SCI12 receive buffers */
  memset(g_SCI5_RXBuffer, 0x00, 28);
  memset(g_SCI12_RXBuffer, 0x00, 28);
}
/*******************************************************************************
* End of function Init_Sync
*******************************************************************************/

/*******************************************************************************
* Outline      : SCI5toSCI12Transfer_Sync
* Description  : This function configures SCI5 and SCI12's peripheral pins, as 
*         well as enabling reception interrupts. SCI5 is set as the 
*         transmit channel, supplying the transfer clock, and SCI12 as 
*         the receive channel. The function disables SCI5 reception and 
*         SCI12 transmission and finally sends the contents of the SCI5 
*         transmit buffer.
* Argument     : none
* Return value : none
*******************************************************************************/
void SCI5toSCI12Transfer_Sync(void)
{  
  /* Declare a transmit count variable */
  uint8_t i = 0;

  /* Configure RXD5, TXD5 and SCK5 as input, output and output, respectively */
  PORTA.PDR.BYTE = 0x12;
  /* Configure RXD12, TXD12 and SCK12 as input, output and input, respectively */
  PORTE.PDR.BYTE = 0xF2;
  /* Configure Port A pins 1, 2 and 4 for peripheral use */
  PORTA.PMR.BYTE |= 0x16;
  /* Configure Port E pins 0, 1 and 2 for peripheral use */
  PORTE.PMR.BYTE |= 0x07;
  
  /* Enable write to PFSWE bit */
  MPC.PWPR.BYTE = 0x00;
  /* Enable write to PFSWE bit and disable write protection to PFS registers */
  MPC.PWPR.BYTE = 0x40;
  
  /* Configure pin 1 of port A as SCK5 */
  MPC.PA1PFS.BIT.PSEL = 0xA;
  /* Configure pin 2 of port A as RXD5 */
  MPC.PA2PFS.BIT.PSEL = 0xA;
  /* Configure pin 4 of port A as TXD5 */
  MPC.PA4PFS.BIT.PSEL = 0xA;
  
  /* Configure pin 0 of port E as SCK12 */
  MPC.PE0PFS.BIT.PSEL = 0xC;
  /* Configure pin 2 of port E as RXD12 */
  MPC.PE2PFS.BIT.PSEL = 0xC;
  /* Configure pin 1 of port E as TXD12 */
  MPC.PE1PFS.BIT.PSEL = 0xC;
  /* Enable write protection to PFS registers */
  MPC.PWPR.BYTE = 0x80;
  
  /* Protection off */
  SYSTEM.PRCR.WORD = 0xA503;
  /* Cancel the SCI5 and SCI12 module clock stop modes */
  MSTP_SCI5 = 0x0;
  MSTP_SCI12 = 0x0;    
  /* Protection on */
  SYSTEM.PRCR.WORD = 0xA500;

  /* Set SCI5 RXI5 interrupt priority level to 7 */
  IPR(SCI5, RXI5) = 0x7;
  /* Enable SCI5 RXI5 interrupts */
  IEN(SCI5, RXI5) = 0x1;
  /* Clear SCI5 RXI5 IR flag */
  IR(SCI5, RXI5) = 0x0;
  
  /* Set SCI12 RXI12 interrupt priority level to 7 */
  IPR(SCI12, RXI12) = 0x7;
  /* Enable SCI12 RXI12 interrupts */
  IEN(SCI12, RXI12) = 0x1;
  /* Clear SCI12 RXI12 IR flag */
  IR(SCI12, RXI12) = 0x0;
  
  /* Initialise SCI5 */
  /* Select on-chip baudrate generator and output clock on SCK5 pin */
  SCI5.SCR.BIT.CKE = 0x1;
  /* Enable serial interface mode */
  SCI5.SIMR1.BIT.IICM = 0x0;
  /* No clock phase delay used */
  SCI5.SPMR.BIT.CKPH = 0x0;
  /* Select clock source frequency as PCLK/16=3MHz,
     Select clock synchronous mode */
  SCI5.SMR.BYTE = 0x82;
  /* Select serial communication interface mode */
  SCI5.SCMR.BYTE = 0x72;
  /* Set the baudrate to 19200 bps */
  SCI5.BRR = 0x26;
  /* Enable SCI5 transmission */
  SCI5.SCR.BYTE |= 0x20;
  
  /* Initialise SCI12 */
  /* Clock is to be supplied externally */
  SCI12.SCR.BIT.CKE = 0x2;
  /* Ensure to disable Simple IIC mode */
  SCI12.SIMR1.BIT.IICM = 0x0;
  /* No clock phase delay */
  SCI12.SPMR.BIT.CKPH = 0x0;
  /* Select clock source frequency as PCLK/16=3MHz
     Select clock synchronous mode
  */
  SCI12.SMR.BYTE = 0x82;
  /* Select the serial communication interface mode */
  SCI12.SCMR.BYTE = 0x72;
  /* Enable SCI12 reception, as well as reception interrupts */
  SCI12.SCR.BYTE |= 0x50;  
            
  /* Transmit data string from SCI5 to SCI12 */      
  for(i=0; g_SCI5_TXBuffer[i] != NULL; i++)
  {
    /* Write data to the transmit buffer */
    SCI5.TDR = g_SCI5_TXBuffer[i];
    /* Ensure last transmission completed succssfully */
    while(SCI5.SSR.BIT.TEND == 0);
  }
}
/*******************************************************************************
* End of function SCI5toSCI12Transfer_Sync
*******************************************************************************/

/*******************************************************************************
* Outline      : SCI12toSCI5Transfer_Sync
* Description  : This function disables SCI5 and SCI12 transmission and 
*         reception. It then configures SCI12 as the transmit channel, 
*         supplying the transfer clock, and SCI5 as the receive channel. 
*         The function then enables SCI12 reception and SCI5 transmission.
*         The contents of the SCI12 transmit buffer are then sent to SCI5.
* Argument     : none
* Return value : none
*******************************************************************************/
void SCI12toSCI5Transfer_Sync(void)
{  
  /* Declare a count variable */
  uint8_t i = 0;
    
  /* Disable SCI5 transmission and reception, 
     as well as their respective interrupts */
  SCI5.SCR.BYTE &= 0x0F;
  /* Disable SCI12 transmission and reception, 
     as well as their respective interrupts */
  SCI12.SCR.BYTE &= 0x0F;
  
  /* Select on-chip baudrate generator and output clock on SCK12 pin */
  SCI12.SCR.BIT.CKE = 0x1;
  /* Set the baudrate to 9600 bps */
  SCI12.BRR = 0x04;
  /* SCI12 clock is to be supplied externally */
  SCI5.SCR.BIT.CKE = 0x2;
  
  /* Enable SCI5 reception and receive interrupts */
  SCI5.SCR.BYTE |= 0x50;
  /* Enable SCI12 transmissions */
  SCI12.SCR.BYTE |= 0x20;
      
  /* Transmit data string from SCI12 to SCI5 */      
  for(i=0; g_SCI12_TXBuffer[i] != NULL; i++)
  {
    /* Write data to the transmit buffer */
    SCI12.TDR = g_SCI12_TXBuffer[i];
    /* Ensure last transmission completed succssfully */
    while(SCI12.SSR.BIT.TEND == 0);
  }
}
/*******************************************************************************
* End of function SCI12toSCI5Transfer_Sync
*******************************************************************************/

/*******************************************************************************
* Outline      : Transfer_End
* Description  : This function is executed after all transfers are completed 
*         on both channels. The function compares the received data
*         against the sent data, and sets the gTestPass flag accordingly.
*         The function then displays either "Success" or "Failure" on the
*         debug LCD, depending on the status of the gTestPass flag.
* Argument     : none
* Return value : none
*******************************************************************************/
void Transfer_End(void)
{
  /* Check if recevied SCI5 data matches sent SCI12 data */
  if(strcmp((char*)g_SCI5_RXBuffer, (char*)g_SCI12_TXBuffer))
  {
    /* Sent and received data does not match, set test pass flag to false */
    gTestPass = false;
  }
  else
  {
    /* Sent and received data matches, set test pass flag to true */
    gTestPass &= true;
  }
  
  /* Check if received SCI12 data matches sent SCI5 data */
  if(strcmp((char*)g_SCI12_RXBuffer, (char*)g_SCI5_TXBuffer))
  {
    /* Sent and received data does not match, set test pass flag to false */
    gTestPass = false;
  }
  /* Sent and received data match */
  else
  {
    /* Sent and received data matches, set test pass flag to true */
    gTestPass &= true;
  }
  
  /* Check the status of the test pass flag */
  if(gTestPass == true)
  {
    /* Display success on debug LCD */
    Display_LCD(LCD_LINE2, "Success ");
  }
  /* Test pass flag false */
  else
  {
    /* Display failure on debug LCD */
    Display_LCD(LCD_LINE2, "Failure ");
  }
}
/*******************************************************************************
* End of function Transfer_End
*******************************************************************************/

/*******************************************************************************
* Outline      : Excep_SCI5_RXI5
* Description  : This interrupt handler is called on reception of new data 
*         received by SCI5. The function stores the data in the 
*         g_SCI5_RXBuffer array location pointed by g_SCI5_RX_Counter; 
*         which is incremented by 1.
* Argument     : none
* Return value : none
*******************************************************************************/
#pragma vector=VECT_SCI5_RXI5
__interrupt void Excep_SCI5_RXI5(void)
{
  /* Read the received byte */
  g_SCI5_RXBuffer[g_SCI5_RX_Counter++] = SCI5.RDR;
}
/*******************************************************************************
* End of function Excep_SCI5_RXI5
*******************************************************************************/

/*******************************************************************************
* Outline      : Excep_SCI12_RXI12
* Description  : This interrupt handler is called on reception of new data 
*         received by SCI12. The function stores the data in the 
*         g_SCI12_RXBuffer array location pointed by g_SCI12_RX_Counter; 
*         which is incremented by 1. 
* Argument     : none
* Return value : none
*******************************************************************************/
#pragma vector=VECT_SCI12_RXI12
__interrupt void Excep_SCI12_RXI12(void)
{
  /* Read the received byte */
  g_SCI12_RXBuffer[g_SCI12_RX_Counter++] = SCI12.RDR;
}
/*******************************************************************************
* End of function Excep_SCI12_RXI12
*******************************************************************************/
