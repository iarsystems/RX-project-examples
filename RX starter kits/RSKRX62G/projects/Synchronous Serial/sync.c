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
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.    */ 
/*******************************************************************************
* File Name     : sync.c
* Version       : 1.00
* Device        : R5F562GAADFP
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX62G
* Description   : Defines sync serial functions used in this sample.
*******************************************************************************/
/*******************************************************************************
* History       : 06.09.2012 Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
/* Defines standard string functions used in this file  */
#include <string.h>
/* SCI RPDL function definitions */
#include "r_pdl_sci.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* Defines LCD functions used in this file */
#include "lcd.h"
/* Provides declarations of functions defined in this file */
#include "sync.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Declaration of the SCI0 transmit buffer */
uint8_t gSCI0_TXBuffer[] = "SCI0 to SCI2 Loopback Test";
/* Declaration of the SCI2 transmit buffer */
uint8_t gSCI2_TXBuffer[] = "SCI2 to SCI0 Loopback Test";
/* Declaration of SCI0 and SCI2 receive buffers */
uint8_t gSCI0_RXBuffer[26], gSCI2_RXBuffer[26];
/* Declaration of loopback test success flag */
volatile bool gTestPass = true;

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
/* SCI channel 0 receive ISR callback function prototype declaration */
void CB_SCI2Receive(void);
/* SCI channel 0 receive ISR callback function prototype declaration */
void CB_SCI0Receive(void);

/*******************************************************************************
* Outline      : Init_sync
* Description  : This function reserves and clears 26 bytes of memory for 
*                SCI0 and SCI2's receive buffers.
* Argument     : none
* Return value : none
*******************************************************************************/
void Init_Sync(void)
{
    /* Clear the SCI0 and SCI2 receive buffers */
    memset(gSCI0_RXBuffer, 0x00, 26);
    memset(gSCI2_RXBuffer, 0x00, 26);
}
/*******************************************************************************
* End of function Init_Sync
*******************************************************************************/

/*******************************************************************************
* Outline      : SCI0toSCI2Transfer_Sync
* Description  : This function configures SCI0 as the transmit channel, and SCI2
*                as the receive channel. The function then configures a callback
*                interrupt for SCI2 reception, and finally sends the contents of
*                the SCI0 transmit buffer.
* Argument     : none
* Return value : none
*******************************************************************************/
void SCI0toSCI2Transfer_Sync(void)
{   
    /* Declare error flag */
    bool err = true;
    
    /* Configure the applicable SCI pins */
    err &= R_SCI_Set
    (
        PDL_SCI_PIN_SCI2_A
    );
    
    /* Configure SCI channel 0 to operate in sync mode, and to output a clock
       and disable the RX pin */
    err &=  R_SCI_Create
            (
                0,
                PDL_SCI_SYNC|PDL_SCI_CLK_INT_OUT|PDL_SCI_BCP_128|PDL_SCI_RX_DISCONNECTED,
                BIT_31 | 9600,
                4
            );
                
    /* Configure SCI channel 2 to operate in sync mode, and to take an external
       clock and disable the TX pin*/
    err &=  R_SCI_Create
            (
                2,
                PDL_SCI_SYNC|PDL_SCI_CLK_EXT|PDL_SCI_BCP_128|PDL_SCI_TX_DISCONNECTED,
                BIT_31 | 9600,
                5
            );  
            
    /* Configure the SCI channel 2 receive interrupt */
    err &=  R_SCI_Receive
            (
                2,
                PDL_NO_DATA,
                gSCI2_RXBuffer,
                26,
                CB_SCI2Receive,
                PDL_NO_FUNC
            );
            
    /* Transmit data string from SCI0 to SCI2 */
    err &=  R_SCI_Send
            (
                0,
                PDL_NO_DATA,
                gSCI0_TXBuffer,
                0,
                PDL_NO_FUNC
            );
            
    /* Halt in while loop when RPDL errors detected */  
    while(!err);
}
/*******************************************************************************
* End of function SCI0toSCI2Transfer_Sync
*******************************************************************************/

/*******************************************************************************
* Outline      : SCI2toSCI0Transfer_Sync
* Description  : This function configures SCI2 as the transmit channel, and SCI0
*                as the receive channel. The function then configures a callback
*                interrupt for SCI0 reception, and finally sends the contents of
*                the SCI2 transmit buffer.
* Argument     : none
* Return value : none
*******************************************************************************/
void SCI2toSCI0Transfer_Sync(void)
{   
    /* Declare error flag */
    bool err = true;
    
    /* Configure the SCI2 pins */
    err &= R_SCI_Set
            (
                PDL_SCI_PIN_SCI2_A
            );

    /* Configure SCI channel 0 to operate in sync mode, and to take an external
       clock and disable the TX pin */
    err &=  R_SCI_Create
            (
                0,
                PDL_SCI_SYNC|PDL_SCI_CLK_EXT|PDL_SCI_BCP_128|PDL_SCI_TX_DISCONNECTED,
                BIT_31 | 9600,
                4
            );  
                
    /* Configure SCI channel 2 to operate in sync mode, and to output an external
       clock and disable the RX pin */
    err &=  R_SCI_Create
            (
                2,
                PDL_SCI_SYNC|PDL_SCI_CLK_INT_OUT|PDL_SCI_BCP_128|PDL_SCI_RX_DISCONNECTED,
                BIT_31 | 9600,
                5
            );  
            
    /* Configure the SCI channel 0 receive interrupt */
    err &=  R_SCI_Receive
            (
                0,
                PDL_NO_DATA,
                gSCI0_RXBuffer,
                26,
                CB_SCI0Receive,
                PDL_NO_FUNC
            );      
    
    /* Transmit data string from SCI2 to SCI0 */
    err &=  R_SCI_Send
            (
                2,
                PDL_NO_DATA,
                gSCI2_TXBuffer,
                0,
                PDL_NO_FUNC
            );
            
    /* Halt in while loop when RPDL errors detected */  
    while(!err);
}
/*******************************************************************************
* End of function SCI2toSCI0Transfer_Sync
*******************************************************************************/

/*******************************************************************************
* Outline      : CB_SCI0Receive
* Description  : This callback function is executed when reception is complete 
*                on the SCI0 channel. The function compares the received data
*                against the sent data, and sets the gTestPass flag accordingly.
* Argument     : none
* Return value : none
*******************************************************************************/
void CB_SCI0Receive(void)
{
    /* Check if recevied data matches sent data */
    if(strcmp((char*)gSCI0_RXBuffer, (char*)gSCI2_TXBuffer))
    {
        /* Sent and received data does not match, set test pass flag to false */
        gTestPass = false;
    }
    else
    {
        /* Sent and received data matches, set test pass flag to true */
        gTestPass &= true;
    }
}
/*******************************************************************************
* End of function CB_SCI0Receive
*******************************************************************************/

/*******************************************************************************
* Outline      : CB_SCI0Receive
* Description  : This callback function is executed when reception is complete 
*                on the SCI0 channel. The function compares the received data
*                against the sent data, and sets the gTestPass flag accordingly.
*                The function then displays either "Success" or "Failure" on the
*                debug LCD, depending on the status of the gTestPass flag.
* Argument     : none
* Return value : none
*******************************************************************************/
void CB_SCI2Receive(void)
{
    /* Check if received data matches sent data */
    if(strcmp((char*)gSCI2_RXBuffer, (char*)gSCI0_TXBuffer))
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
    if(gTestPass)
    {
        /* Display success on debug LCD */
        DisplayLCD(LCD_LINE2, "Success ");
    }
    /* Test pass flag false */
    else
    {
        /* Display failure on debug LCD */
        DisplayLCD(LCD_LINE2, "Failure ");
    }
}
/*******************************************************************************
* End of function CB_SCI2Receive
*******************************************************************************/
