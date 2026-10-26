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
*******************************************************************************
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.    */ 
/*******************************************************************************
* File Name    : async.c 
* Version      : 2.00
* Device       : R5F562N8
* Tool Chain   : IAR Embedded Workbench
* H/W Platform : RSK+RX62N
* Description  : Defines async serial functions used in this sample.
*******************************************************************************/
/*******************************************************************************
* History    : 03.08.2010 Ver. 1.00 First Release
*          18.04.2011 Ver. 2.00 Second Release (Updated to RPDL CS4)
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
/* SCI RPDL function definitions */
#include "r_pdl_sci.h"
/* CMT RPDL function definitions */
#include "r_pdl_cmt.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* Provides declarations of functions defined in this file */
#include "async.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Declaration of SCI transmit and receive buffers */
uint8_t gSCI_RXBuffer, gSCI_TXBuffer[2] = {0x2F, 0x0};
/* Declaration of the SCI status flag global variable */
bool gSCI_Flag = true;
/* Declaration of the command string to clear the terminal screen */
const uint8_t gSCI_ClearScreen[9] = {27, 91, 50, 74, 0, 27, 91, 72, 0};

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
/* Declaration of SCI transmit function prototype */
void Transmit_Async(void);

/*******************************************************************************
* Outline      : Init_Async
* Description  : This function initialises the SCI channel connected to the RSK+
*         RS232 connector. The channel is configured for transmission and
*         reception, and instructions are sent to the terminal. A compare
*         match timer is also configured, to produce an ISR every 100ms
*         that is used to time the SCI data transmission.
* Argument     : none
* Return value : none
*******************************************************************************/
void Init_Async(void)
{
  /* Declare error flag */
  bool err = true;
    
  /* Configure the pin selection of SCI channel */
  err &=  R_SCI_Set
      (
        PDL_SCI_PIN_SCI2_A
      );
      
  /* Configure the RS232 port */
  err &=  R_SCI_Create
      (
        2,
        PDL_SCI_ASYNC | PDL_SCI_8N1,
        19200,
        5
      );
  
  /* Configure the SCI receive interrupt */
  err &=  R_SCI_Receive
      (
        2,
        PDL_NO_DATA,
        &gSCI_RXBuffer,
        1,
        CB_SCIReceive,
        PDL_NO_FUNC
      );
    
  /* Clear the terminal screen */
  err &=   R_SCI_Send
      (
        2,
        PDL_NO_DATA,
        gSCI_ClearScreen,
        0,
        PDL_NO_FUNC
      );
          
  /* Display terminal header message */
  err &=  R_SCI_Send
      (
        2,
        PDL_NO_DATA,
        "Renesas RSK+RX62N Async_Serial Sample\r\n\n",
        0,
        PDL_NO_FUNC
      );
  
  /* Display sample instructions on the terminal */
  err &=  R_SCI_Send
      (
        2,
        PDL_NO_DATA,
        "Press z to stop transmission and any key to resume\r\n\n",
        0,
        PDL_NO_FUNC
      );
    
  /* Configure CMT timer for SCI transmission intervals */
  err &=  R_CMT_Create
      (
        3,
        PDL_CMT_PERIOD,
        100E-3,
        CB_CMTTimer,
        3
      );
     
  /* Halt in while loop when RPDL errors detected */  
  while(!err);
}
/*******************************************************************************
* End of function Init_Async
*******************************************************************************/

/*******************************************************************************
* Outline      : Transmit_Async
* Description  : This function increments the ASCII number character in the SCI
*         transmit buffer, and sends it to the terminal. If the ASCII
*         value goes out of the range of the cardinal numbers; it loops
*         the value back to 0, and inserts a new line.
* Argument     : none
* Return value : none
*******************************************************************************/
void Transmit_Async(void)
{
  /* Declare error flag */
  bool err = true;
  
  /* Check SCI status flag */
  if(gSCI_Flag)
  {
    /* Increment number in transmit buffer */
    gSCI_TXBuffer[0]++;
    
    /* Check if hex value of transmit buffer is greater than 0x39 (9) */
    if(gSCI_TXBuffer[0] > 0x39)
    {
      /* Insert a new line and return the cursor left */
      err &=  R_SCI_Send
          (
            2,
            PDL_NO_DATA,
            "\r\n",
            0,
            PDL_NO_FUNC
          );
          
      /* Reset the ASCII characer in the transmit buffer back to 0 */
      gSCI_TXBuffer[0] = 0x30;
    }
    
    /* Send the transmit buffer to the terminal */
    err &=  R_SCI_Send
        (
          2,
          PDL_NO_DATA,
          gSCI_TXBuffer,
          0,
          PDL_NO_FUNC
        );
  }
  
  /* Halt in while loop when RPDL errors detected */  
  while(!err);
}
/*******************************************************************************
* End of function Transmit_Async
*******************************************************************************/

/*******************************************************************************
* Outline      : CB_CMTTimer
* Description  : This callback function is executed when a compare match occurs
*         in the CMT timer (every 100ms). The function checks if a SCI
*         transmission is in progress before executing the Transmit_Async
*         function. If the SCI channel is busy, it postpones the trans-
*         mission untill the next CMT period.
* Argument     : none
* Return value : none
*******************************************************************************/
void CB_CMTTimer(void)
{
  /* Declare error flag */
  bool err = true;
  
  /* Declare SCI status container variable */
  uint8_t sci_status = 0x00;
  
  /* Check the status of SCI channel 2, and insert results into sci_status */
  err &=  R_SCI_GetStatus
      (
        2,
        &sci_status,
        PDL_NO_PTR,
        PDL_NO_PTR,
        PDL_NO_PTR
      );
  
  /* Check if SCI transmission is in progress */
  if(sci_status & 0x04)
  {
    /* SCI transmit not in progress, execute transmit function */
    Transmit_Async();
  }
  
  /* Halt in while loop when RPDL errors detected */  
  while(!err);
}
/*******************************************************************************
* End of function CB_CMTTimer
*******************************************************************************/

/*******************************************************************************
* Outline      : CB_SCIReceive
* Description  : This callback function is executed every time a character is
*         received from the SCI, channel 2. The function checks if the
*         character was 'z', and pauses SCI transmission by setting the 
*         SCI status flag (gSCI_Flag) to 0 untill another character is
*         received.
* Argument     : none
* Return value : none
*******************************************************************************/
void CB_SCIReceive(void)
{
  /* Declare error flag */
  bool err = true;
  
  /* Check if last received character was 'z' */
  if(gSCI_RXBuffer == 'z')
  {
    /* Set SCI status flag to 0 */
    gSCI_Flag = false;
  }
  
  /* Check if the SCI status flag is 0, and the last received character
     was not 'z' */
  else if(!gSCI_Flag)
  {
    /* Set SCI status flag to 1 */
    gSCI_Flag = true;
  }
  
  /* Reconfigure the SCI receive interrupt */
  err &=  R_SCI_Receive
      (
        2,
        PDL_NO_DATA,
        &gSCI_RXBuffer,
        1,
        CB_SCIReceive,
        PDL_NO_DATA
      );
      
  /* Halt in while loop when RPDL errors detected */  
  while(!err);
}
/*******************************************************************************
* End of function CB_SCIReceive
*******************************************************************************/
