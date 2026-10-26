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
* File Name     : async.c
* Version       : 1.00
* Device        : R5F563NB
* Tool-Chain    : Renesas RX Standard 1.2.0.0
* H/W Platform  : RSK+RX63N
* Description   : Defines SCI functions used in this sample.
*******************************************************************************/
/*******************************************************************************
* History       : 13 Aug. 2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* System Includes 
*******************************************************************************/
/* Defines standard boolean variable types used in this file */
#include <stdbool.h>
/* Following header file provides standard integer type definitions. */
#include <stdint.h>

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/
/* Defines port registers */
#include "iorx63n.h"
/* Defines RX63N LEDs and switches */
#include "rskrx63ndef.h"
/* Declares functions defined in this file */
#include "async.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Declaration of the SCI status flag global variable */
volatile bool gSCI_Flag = true;
/* Declare a receive buffer variable */
volatile uint8_t gSCI_RXBuffer;
/* Declaration of the command string to clear the terminal screen */
const uint8_t cmd_clr_scr[] = {27, 91, 50, 74, 0, 27, 91, 72, 0};

/*******************************************************************************
* Function Prototypes
*******************************************************************************/
/* Init_Timer function prototype */
static void InitTimer_Async(void);
/* Transmit_Async function prototype */
static void Transmit_Async(void);
/* Text_Write function prptotype */
static void Text_Write(const uint8_t* msg_string);

/*******************************************************************************
* Outline      : Init_Async
* Description  : This function initialises the SCI channel connected to the 
*                 RS232 connector on the RSK. The channel is configured for 
*                 transmission and reception, and instructions are sent to the
*                 terminal.
* Argument     : none
* Return value : none
*******************************************************************************/
void Init_Async(void)
{        
    /* Protection off */
    SYSTEM.PRCR.WORD = 0xA503u;
    
    /* Cancel the SCI0 module clock stop mode */
    MSTP_SCI0 = 0u;
    
    /* Protection on */
    SYSTEM.PRCR.WORD = 0xA500u;
    
    /* Configure SCI0's TXD0, RXD0 and SCK0 pins for peripheral functions */
    PORT2.PMR.BYTE = 0x07u;
    
    /* PWPR.PFSWE write protect off */
    MPC.PWPR.BYTE = 0x00u;    
    
    /* PFS register write protect off */
    MPC.PWPR.BYTE = 0x40u;    
    
    /* Configure SCI0's communication pins */
    /* Set Port2's pin 0 function to TXD0 */
    MPC.P20PFS.BYTE = 0x0Au; 
    
    /* Set Port2's pin 1 function to RXD0 */
    MPC.P21PFS.BYTE = 0x0Au; 
    
    /* Set Port2's pin 2 function to SCK0 */
    MPC.P22PFS.BYTE = 0x0Au; 
    
    /* Turn on PFS register write protect */
    MPC.PWPR.BYTE = 0x80u;    
    
    /* Cancel SCI0 module stop mode */
    SYSTEM.MSTPCRB.LONG = 0x7FFFFFFFu;
    
    /* Disable All bits and CKE0,1 = 00 */
    SCI0.SCR.BYTE = 0x00u;                                  

    /* Set output port TXD0(P20) */
    PORT2.PDR.BIT.B0 = 1u;
    
    /* Set Receive port RXD0(P21)*/
    PORT2.PDR.BIT.B1 = 1u;

    /* disable All bits and CKE0,1 = 00 once more */
    SCI0.SCR.BYTE = 0x00u;                                  

    /* CKS[0-1] = 00: PCLK 0 div        
       1Sop 8bits non-parity     */
    SCI0.SMR.BYTE = 0x00u;   
                                                              
    /* SMIF = 0: Serial  */
    SCI0.SCMR.BYTE = 0xf2u;                                   

    /* PCLK = 48MHz, Set baudrate to 19200 bps 
       77 = 48MHz / (64*0.5*19200)-1 */
    SCI0.BRR = 77u;                                     
                              
    /* Allow a small delay */
    for (uint16_t i = 0u; i < 20u; i++ )
    {
    }                                 

    /* Clear bits ORER, PER and FER */
    SCI0.SSR.BYTE &= 0xC7u;                                      
        
    /* Set SCI0 RXI0 interrupt priority level to 7 */    
    IPR(SCI0,RXI0) = 0x7u;
    /* Enable SCI0 RXI0 interrupts */
    IEN(SCI0,RXI0) = 0x1u;
    /* Clear SCI0 RXI0 interrupt flag */
    IR(SCI0,RXI0) = 0x0u;

    /* Set bits TIE, RIE, RE and TE. Enable receive interrupts */
    SCI0.SCR.BYTE = 0xF0u;  
    
    /* Clear the text on terminal window */
    Text_Write(cmd_clr_scr);                                  

    /* Display splash screen on terminal window */
    Text_Write("Renesas RX63N Async Serial \n\r");
                
    /* Inform user on how to stop transmission */                        
    Text_Write("Press 'z' to stop and any key to resume\r\n\n");
    
    /* Initialise CMT2 */
    InitTimer_Async();
}
/*******************************************************************************
* End of function Init_Async
*******************************************************************************/

/*******************************************************************************
* Outline      : InitTimer_Async
* Description  : This function initialises the CMT channel 2.
* Argument     : none
* Return value : none
*******************************************************************************/
static void InitTimer_Async(void)
{
    /* Protection off */
    SYSTEM.PRCR.WORD = 0xA503u;
    
    /* Cancel the CMT2 module clock stop mode */
    MSTP_CMT2 = 0u;
    
    /* Protection on */
    SYSTEM.PRCR.WORD = 0xA500u;    
    
    /* Set CMT2 interrupt priority level to 5 */    
    IPR(CMT2,CMI2) = 0x5u;
    
    /* Enable CMT2 interrupts */
    IEN(CMT2,CMI2)      = 0x1u;
    CMT2.CMCR.BIT.CMIE  = 0x1u;
    
    /* Clear CMT2 interrupt flag */
    IR(CMT2,CMI2) = 0x0u;
    
    /* Select the PCLK clock division as PCLK/128 = 375KHz */ 
    CMT2.CMCR.BIT.CKS = 0x2u;
        
    /* Set compare match value maximum */
    CMT2.CMCOR = 0xFFFFu;
    
    /* Start timer running */
    CMT.CMSTR1.BIT.STR2 = 0x1u;    
}
/*******************************************************************************
* End of function InitTimer_Async
*******************************************************************************/

/*******************************************************************************
* Outline       : Transmit_Async 
* Description   : Sends incrementing numbers across the UART link to the 
*                  terminal window.
* Argument      : none
* Return value  : none
*******************************************************************************/
static void Transmit_Async(void)
{    
    /* Declare static local variable */
    static uint8_t tx_buffer[2] = "0";
    
    /* Check SCI status flag */
    if(gSCI_Flag)
    {
        /* Turn on LED0 */
        LED0 = LED_ON;    
               
        /* Check if hex value of transmit buffer is greater than 0x39 (9) */
        if(tx_buffer[0] > 0x39u)
        {
            /* Insert a new line and return the cursor left */
            Text_Write("\r\n");
                    
            /* Reset the ASCII character in the transmit buffer back to 0 */
            tx_buffer[0] = 0x30u;
        }
        
        /* Send the transmit buffer to the terminal */
        Text_Write(tx_buffer);
        
        /* Increment number in transmit buffer */
        tx_buffer[0]++;
                
        /* Turn off LED1 */
        LED1 = LED_OFF;        
    }
}
/********************************************************************************
* End of function Transmit_Async
********************************************************************************/

/*******************************************************************************
* Outline        : Text_Write
* Description    : Transmits null-terminated string.
* Argument       : (uint8_t*) msg_string - null terminated string
* Return value   : None
*******************************************************************************/
static void Text_Write(const uint8_t* msg_string)
{
    /* This loop reads the text string and puts it into the UART's 
       transmit buffer */
    for (volatile uint8_t i = 0u; msg_string[i]; i++)
    {        
        /* Check that the last transmission was completed */
        while(0u == (SCI0.SSR.BYTE & 0x80u));
        
        /* Transmit the next character */
        SCI0.TDR = msg_string[i];
    }
}
/*******************************************************************************
* End of Function Text_Write
*******************************************************************************/

/*******************************************************************************
* Outline       : Excep_SCI0_RXI0
* Description   : SCI0 receive interrupt handler, called when a character is 
*                  received from the terminal program. Sets rx_flag if character 
*                  is 'z' entered.
* Argument      : none
* Return value  : none
*******************************************************************************/
#pragma vector=VECT_SCI0_RXI0
__interrupt void Excep_SCI0_RXI0(void)
{    
    /* Store the received character */
    gSCI_RXBuffer = SCI0.RDR;
    
    /* Check if last received character was 'z' */
    if(gSCI_RXBuffer == 'z')
    {
        /* Set SCI status flag to 0 */
        gSCI_Flag = false;
        
        /* Turn off LED0 */
        LED0 = LED_OFF;
            
        /* Turn on LED1 */
        LED1 = LED_ON;
    }
    
    /* Check if the SCI status flag is 0, and the last received character
       was not 'z' */
    else if(!gSCI_Flag)
    {
        /* Set SCI status flag to 1 */
        gSCI_Flag = true;
    }
}
/*******************************************************************************
* End of function Excep_SCI0_RXI0
*******************************************************************************/

/*******************************************************************************
* Outline        : Excep_CMTU1_CMT2
* Description    : CMT0 interrupt handler. This function calls the Transmit_Async 
*                  function and increments the gDelay_Counter variable before 
*                  exiting.
* Argument       : none    
* Return value   : none
*******************************************************************************/
#pragma vector=VECT_CMT2_CMI2
__interrupt void Excep_CMTU1_CMT2(void)
{    
    /* Transmit a byte */
    Transmit_Async();
}
/*******************************************************************************
* End of function Excep_CMTU1_CMT2
*******************************************************************************/
