/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No 
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all 
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM 
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES 
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS 
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of 
* this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer 
*
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.    
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : lowlvl_debug_console.c
* Description  : Functions to support stream I/O to the E1 virtual Console
* Version      : 1.0
* Device(s)    : R5F51115AxFM
* Tool-Chain   : CCRX
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 26.10.2011 1.00     First Release
***********************************************************************************************************************/
/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "lowlvl_debug_console.h"
typedef struct st_dbg
{
    uint32_t   TX_DATA;     // Debug Virtual Console TX data
    char       wk1[12];     // spacer
    uint32_t   RX_DATA;     // Debug Virtual Console RX data
    char       wk2[44];     // spacer
    uint32_t   DBGSTAT;     // Debug Virtual Console Status    
} st_dbg_t;
#define E1_DBG_PORT (*(volatile st_dbg_t     __evenaccess *)0x84080)

#define TXFL0EN     0x00000100          // debug tx flow control bit
#define RXFL0EN     0x00001000          // debug RX flow control bit


/******************************************************************************
* Function Name: charput
* Description  : Outputs a character on a serial port
* Arguments    : character to output
* Return Value : none
******************************************************************************/
void charput (const uint8_t output_char)
{
    /* Wait for transmit buffer to be empty */
    while (0 != (E1_DBG_PORT.DBGSTAT & TXFL0EN))
    {
        /* Wait */
    }

    /* Write the character out */
    E1_DBG_PORT.TX_DATA = (uint32_t)output_char;
}
/******************************************************************************
End of function  charput
******************************************************************************/

/******************************************************************************
* Function Name: charget
* Description  : Gets a character from the debug port register
* Arguments    : address of character buffer to store result
* Return Value : 0 if no character received, 1 if received
******************************************************************************/
uint8_t charget (uint8_t * const destchar)
{   
    uint8_t char_rxd = 0;

    /* Wait for rx buffer buffer to be ready */
    if (0 != (E1_DBG_PORT.DBGSTAT & RXFL0EN))
    {
        *destchar = (uint8_t)E1_DBG_PORT.RX_DATA;
        char_rxd = 1;
    }

    /* Read data, send back up */
    return char_rxd;
}
/******************************************************************************
End of function charget
******************************************************************************/

/******************************************************************************
* Function Name: charwait
* Description  : Gets a character from the debug port register
* Arguments    : address of character buffer to store result
* Return Value : 0 if no character received, 1 if received
******************************************************************************/
uint8_t charwait (void)
{
    uint8_t ch;

    while (0 == charget(&ch))
    {
        /* Wait */
    }

    return ch;

}
/******************************************************************************
End of function charwait
******************************************************************************/


