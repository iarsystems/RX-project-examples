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
/* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.   */
/*******************************************************************************
* File Name     : crc.c
* Version       : 1.00
* Device        : R5F5210B
* Tool-Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX210B
* Description   : Defines CRC generation & SCI functions
*******************************************************************************/
/*******************************************************************************
* History       : 14.05.2013  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/
/* SCI RPDL function definitions */
#include "r_pdl_sci.h"
/* CRC RPDL function definitions */
#include "r_pdl_crc.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* Declares functions defined in this file */
#include "crc.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Declaration of SCI transmit and receive buffers */
volatile uint8_t gSCI_RXBuffer = 0x0000;
/* Declaration of the command string to clear the terminal screen */
const uint8_t gSCI_ClearScreen[9] = {27, 91, 50, 74, 0, 27, 91, 72, 0};

/*******************************************************************************
* Function Prototypes
*******************************************************************************/
/* CRC checksum calculate function prototype declaration */
static uint16_t Calculate_CRC(uint8_t);
/* 16 bit integer to character string function prototype declaration */
static void uint16_ToString(uint8_t *, uint8_t, uint16_t);
/* SCI receive interrupt callback function prototype declaration */
void CB_SCIReceive_CRC(void);

/*******************************************************************************
* Outline       : Init_CRC
* Description   : This function initialises the SCI channel connected to the
*                 RSK RS232 connector, and configures the CRC unit to perform
*                 ANSI standard 16bit checksum calculations. The function then 
*                 displays instructions in the terminal window.
* Argument      : none
* Return value  : none
*******************************************************************************/
void Init_CRC(void)
{
    /* Declare error flag */
    bool err = true;
    
    /* Initialise the CRC calculator module to calculate CRC16 checksum */
    err &=  R_CRC_Create
            (
                PDL_CRC_POLY_CRC_16|PDL_CRC_LSB_FIRST
            );

    /* Configure the SCI pins */
    err &=  R_SCI_Set
            (
                0,
                PDL_SCI_PIN_SCI0_TXD0_P20 | PDL_SCI_PIN_SCI0_RXD0_P21
            );
            
    /* Configure the SCI in the UART mode */
    err &=  R_SCI_Create
            (
                0,
                PDL_SCI_ASYNC | PDL_SCI_8N1,
                19200, 
                6
            );

    /* Configure the SCI receive interrupt */
    err &=  R_SCI_Receive
            (
                0,
                PDL_NO_DATA,
                &gSCI_RXBuffer,
                1,
                CB_SCIReceive_CRC,
                PDL_NO_FUNC
            );

    /* Clear the terminal screen */
    err &=  R_SCI_Send
            (
                0,
                PDL_NO_DATA,
                gSCI_ClearScreen,
                0,
                PDL_NO_FUNC
            );
            
    /* Display terminal header message */
    err &=  R_SCI_Send
            (
                0,
                PDL_NO_DATA,
                "Renesas RSKRX210B CRC Sample\r\n\n",
                0,
                PDL_NO_FUNC
            );

    /* Display sample instructions on the terminal */
    err &=  R_SCI_Send
            (
                0,
                PDL_NO_DATA,
                "Enter any key, and the checksum will be displayed below \
                \r\n\n",
                0,
                PDL_NO_FUNC
            );
                
    /* Halt in while loop when RPDL errors detected */
    while(!err);
}
/*******************************************************************************
* End of function Init_CRC
*******************************************************************************/

/*******************************************************************************
* Outline      : Calculate_CRC
* Description  : Function takes an 8 bit input, writes it to the CRC unit,
*                fetches the CRC result and returns the value. The CRC checksum
*                method used will depend on how the CRC unit is configured.
* Argument     : uint8_t input_char - The 8bit input character.
* Return value : uint16_t output_char - The 16bit checksum output.
*******************************************************************************/
uint16_t Calculate_CRC(uint8_t input_char)
{
    /* Declare error flag */
    bool err = true;
    
    /* Declare temporary result container variable */
    uint16_t output_char = 0x0000;
    
    /* Write the input character to the CRC registers */
    err &=  R_CRC_Write
            (   
                input_char
            );

    /* Fetch the result from the CRC unit, and store it in output_char */   
    err &=  R_CRC_Read
            (
                PDL_CRC_CLEAR_RESULT,
                &output_char
            );

    /* Halt in while loop when RPDL errors detected */  
    while(!err);
    
    /* Return the CRC result */
    return output_char;
}
/*******************************************************************************
* End of function Calculate_CRC
*******************************************************************************/

/*******************************************************************************
* Outline      : CB_SCIReceive_CRC
* Description  : SCI receive interrupt callback function, which is executed
*                every time a character is received from the terminal. The
*                function uses the Calculate_CRC function to fetch the checksum
*                for the received file, then converts the checksum into a 
*                character string and displays both values onto the terminal.
*                If the SCI channel is busy, the function waits until it is free
*                to transmit.
* Argument     : none
* Return value : none
*******************************************************************************/
void CB_SCIReceive_CRC(void)
{
    /* Declare error flag */
    bool err = true;
    
    /* Declare temporary checksum receive character container variables */
    uint16_t checksum;
        
    /* Declare SCI status container variable */
    uint8_t sci_status = 0x00, received_char;
    
    /* Declare SCI transmission buffer string */
    uint8_t tx_buffer[] = "\r Character: X (0xYYYY) - Checksum: 0xZZZZ\0";  
    
    /* Replace X in the transmit buffer string with the received character */
    tx_buffer[13] = gSCI_RXBuffer;
    
    /* Copy the received data */
    received_char = gSCI_RXBuffer;
    
    /* Convert the received character into a hex number, and insert into 
       transmit buffer string to replace YYYY */
    uint16_ToString(tx_buffer, 18, (uint16_t)received_char);    
    
    /* Calculate the CRC checksum for the character received */
    checksum = Calculate_CRC(gSCI_RXBuffer);
    
    /* Convert the checksum into a hex number, and insert into transmit
       buffer string to replace YYYY */
    uint16_ToString(tx_buffer, 38, checksum);
    
    /* Wait until SCI channel is no longer busy */
    while(!(sci_status & 0x04))
    {
        /* Fetch SCI channel 0 status */
        err &=  R_SCI_GetStatus
                (
                    0,
                    &sci_status,
                    PDL_NO_PTR,
                    PDL_NO_PTR,
                    PDL_NO_PTR
                );
    
        /* Halt in while loop when RPDL errors detected */  
        while(!err);    
    }
        
    /* Send the SCI transmission buffer to the terminal */
    err &=  R_SCI_Send
            (
                0,
                PDL_NO_DATA,
                tx_buffer,
                0,
                PDL_NO_FUNC
            );

    /* Re-configure the SCI receive interrupt */
    err &=  R_SCI_Receive
            (
                0,
                PDL_NO_DATA,
                &gSCI_RXBuffer,
                1,
                CB_SCIReceive_CRC,
                PDL_NO_DATA
            );

    /* Halt in while loop when RPDL errors detected */  
    while(!err);
}
/******************************************************************************
* End of function CB_SCIReceive_CRC
******************************************************************************/

/******************************************************************************
* Outline      : uint16_ToString
* Description  : Function converts a 16 bit integer into a character string, 
*                inserts it into the array via the pointer passed at execution.
* Argument     : * output_string : Pointer to uint8_t array that will hold 
*                                  character string.
*                            pos : uint8_t number, element number to begin 
*                                  inserting the character string from (offset).
*                   input_number : 16 bit integer to convert into a string.
* Return value : none
* Note         : No input validation is used, so output data can overflow the
*                array passed.
*******************************************************************************/
void uint16_ToString(uint8_t * output_string,uint8_t pos, uint16_t input_number)
{
    /* Declare temporary character storage variable, and bit_shift variable */  
    uint8_t a, bit_shift = 12u;
    
    /* Declare 16bit mask variable */
    uint16_t mask = 0xF000;
    
    /* Loop through until each hex digit is converted to an ASCII character */
    while(bit_shift < 30u)
    {
        /* Mask and shift the hex digit, and store in temporary variable, a */ 
        a = (uint8_t)((input_number & mask) >> bit_shift);
        
        /* Convert the hex digit into an ASCII character, and store in output
           string */
        output_string[pos] = (uint8_t)((a < 0x0A) ? (a + 0x30) : (a + 0x37));
        
        /* Shift the bit mask 4 bits to the right, to convert the next digit */
        mask = (uint16_t) (mask >> 4u);
        
        /* Decrement the bit_shift counter by 4 (bits in a each digit) */
        bit_shift -= 4u;
        
        /* Increment the output string location */
        pos++;
    }
}
/*******************************************************************************
* End of function uint16_t_ToString
*******************************************************************************/
