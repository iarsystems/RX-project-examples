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
* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.    
*******************************************************************************/
/*******************************************************************************
* File Name    : r_fl_comm_uart.c
* Version      : 3.00
* Description  : Has functions for communicating with Host.  This file will
*                change depending on what communications medium is used to 
*                communicate with the host. This file uses asynchronous serial
*                communications.
******************************************************************************/  
/******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 05.04.2010 1.00    First Release
*         : 22.03.2011 2.00    First Release for YRDK
*         : 27.02.2012 3.00    Renamed to 'r_fl_comm_uart.c' to clearly label
*                              this as a UART implementation. Removed 
*                              'rx_bytes' argument from fl_com_receive() 
*                              function because it was not being used. Code 
*                              now uses r_sci_async_rx package.
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
/* Fixed-width types support. */
#include <stdint.h>
/* bool support. */
#include <stdbool.h>
/* Use the r_sci_async_rx package for UART communications. */
#include "r_sci_async_1ch_rx_if.h"

/******************************************************************************
Typedef definitions
******************************************************************************/
typedef struct 
{
    /* Where to store data */
    uint8_t *   receive_buffer;
    /* Has reception been requested? */
    bool        receive_ready;
    /* Where in buffer to receive */
    uint32_t    receive_index;
    /* How many bytes received */
    uint32_t    received_bytes;
} uart_rx_info_t;

/******************************************************************************
Private global variables and functions
******************************************************************************/
/* Holds info about data we have received. */
static uart_rx_info_t uart_rx_info = { 0, false, 0, 0 };

/******************************************************************************
* Function Name: fl_com_receive
* Description  : Sets up communications medium for receiving next data.
* Arguments    : rx_buffer - 
*                    Where to place receive data
*                rx_index - 
*                    Index in receive buffer to start at
* Return value : none
******************************************************************************/
void fl_com_receive(uint8_t * rx_buffer, uint32_t rx_index)
{
    /* Setup SCI2 to receive bytes into receive buffer starting
       at location 'rx_index'. */
    uart_rx_info.receive_buffer = rx_buffer;   
    uart_rx_info.receive_index = rx_index;

    /* Receive is ready. */
    uart_rx_info.receive_ready = true;
    
    /* Init bytes received to zero */
    uart_rx_info.received_bytes = 0;
}
/******************************************************************************
End of function fl_com_receive
******************************************************************************/

/******************************************************************************
* Function Name: fl_com_bytes_received
* Description  : Returns how many bytes have been received.
* Arguments    : none
* Return value : Number of bytes that have been received
******************************************************************************/
uint32_t fl_com_bytes_received(void)
{                                       
    return uart_rx_info.received_bytes;                                          
}
/******************************************************************************
End of function fl_com_bytes_received
******************************************************************************/

/******************************************************************************
* Function Name: fl_com_transmit
* Description  : Transmit data over communications medium
* Arguments    : tx_buffer - 
*                    Buffer where transmit data is located
*                tx_index - 
*                    How many bytes to transmit
* Return value : none
******************************************************************************/
void fl_com_transmit(uint8_t *tx_buffer, uint32_t tx_bytes)
{
    /* Tranmist data. */
    R_SCIA_Send(tx_buffer, tx_bytes);
}
/******************************************************************************
End of function fl_com_transmit
******************************************************************************/

/******************************************************************************
* Function Name: fl_com_init
* Description  : Sets up communications medium for receiving data from host.
* Arguments    : none
* Return value : none
******************************************************************************/
void fl_com_init(void)
{   
    /* Initialize UART channel. */
    R_SCIA_Init();
}
/******************************************************************************
End of function fl_com_init
******************************************************************************/

/******************************************************************************
* Function Name: fl_com_send_status
* Description  : Returns whether the last transmission is complete or not.
* Arguments    : none
* Return value : true -
*                    Transfer completed.
*                false -
*                    Transfer has not completed. 
******************************************************************************/
bool fl_com_send_status(void)
{   
    /* Get status of transmission. */
    return R_SCIA_SendStatus();
}
/******************************************************************************
End of function fl_com_init
******************************************************************************/

/******************************************************************************
* Function Name: my_sci_callback
* Description  : This is a callback function from the UART driver. Whenever
*                a byte is received, this function will be called with the
*                received byte as an input argument. 
*                NOTE: This function is called from an ISR so keep this short!
* Arguments    : rx_data -
*                    Data received.
* Return value : none
******************************************************************************/
void my_sci_callback(uint8_t data)
{    
    /* Only read data if we are expecting data. */
    if (uart_rx_info.receive_ready == true)
    {    
        /* Read data */
        uart_rx_info.receive_buffer[uart_rx_info.receive_index] = data; 
        
        /* Increase index & bytes received */
        uart_rx_info.receive_index++;
        uart_rx_info.received_bytes++;
    }
    else
    {
        /* Doing nothing. You could alert the application that 
           unexpected data was received if you wanted. */
    }
}
/******************************************************************************
End of function my_sci_callback
******************************************************************************/


