/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. This 
* software is owned by Renesas Electronics Corporation and is protected under 
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING 
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT 
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR 
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE 
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software 
* and to discontinue the availability of this software.  By using this software, 
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_main.c
* Version      : Applilet4 for RX111 V1.00.00.05 [06 Jun 2013]
* Device(s)    : R5F51115AxFM
* Tool-Chain   : IAR Systems icc78k0r
* Description  : This file implements main function.
* Creation Date: 2013-08-16
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_icu.h"
#include "r_cg_cmt.h"
#include "r_cg_sci.h"
#include "r_cg_crc.h"
/* Start user code for include. Do not edit comment generated here */
#include <string.h>
#include "lcd.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */

/* Declaration of the command string to clear the terminal screen */
const char cmd_clr_scr[] = {27, 91, 50, 74, 0, 27, 91, 72, 0,};

uint8_t tx_buffer[] = "\r Character: X (0xYYYY) - Checksum: 0xZZZZ\0";
uint16_t checksum;
uint8_t  input;

static void text_write (const char * const msg_string);
static void uint16_to_string (uint8_t * const output_string, uint8_t pos, uint16_t const input_number);

/* End user code. Do not edit comment generated here */


/* Set option bytes */
#pragma location = "OPTBYTE"
__root const uint32_t OFS0_location = 0xFFFFFFFFUL;
#pragma location = "OPTBYTE"
__root const uint32_t OFS1_location = 0xFFFFFFFFUL;


void R_MAIN_UserInit(void);
/***********************************************************************************************************************
* Function Name: main
* Description  : This function implements main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void main(void)
{
    R_MAIN_UserInit();
    /* Start user code. Do not edit comment generated here */


    /* Displays the application name on the debug LCD */
    Clear_Display();
    DisplayCenter(0,  (uint8_t *)"CRC");
    DisplayCenter(1, (uint8_t *)"Sample");
    
    /* Set the CRC module for LSB first */
    R_CRC_SetCRC16(CRC_LSB);
    
    /* enable uart reception and transmission */
    startSCI1();
    initSCI1Receive();
    
    /* Clear the text on terminal window */
    text_write(cmd_clr_scr);

    /* Display splash screen on terminal window */
    text_write("\r");
    text_write("Renesas RX111 CRC Sample \r\n");
    text_write("Enter any printable key, and the checksum will be displayed below\r\n\n");

    
    while (1U)
    {
        /* Wait here until a character is received */
        while (g_sci1_data_available == 0)
        {
            /* Wait */
        }

        g_sci1_data_available = 0;

         /* Read the character */
        input = gp_sci1_rx_buf[0];

        /* Load data for CRC calculation */
        R_CRC_Input_Data(input);

        /* read the result */
        R_CRC_Get_Result(&checksum);
        
        /* Replace X in the transmit buffer string with the received character */
        tx_buffer[13] = input;

        /* Convert the received character into a hex number, and insert into transmit buffer string to replace YYYY */
        uint16_to_string(tx_buffer, 18, (uint16_t)input);  
        
        /* Convert the checksum into a hex number, and insert into transmit buffer string to replace YYYY */
        uint16_to_string(tx_buffer, 38, checksum);
        
        /* Display the result */    
        text_write((const char *) tx_buffer);    
    }
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: R_MAIN_UserInit
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_MAIN_UserInit(void)
{
    /* Start user code. Do not edit comment generated here */
    /* Initialise the debug LCD */
    Init_LCD();
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/*******************************************************************************
* Function Name : text_write
* Description   : Transmits null-terminated string.
* Argument      : (char*) msg_string - null terminated string
* Return value  : None
*******************************************************************************/
static void text_write (const char * const msg_string)
{
    while (sci1_tx_is_busy())
    {
    }
    sendSCI1((uint8_t *)msg_string, (uint16_t)strlen(msg_string));
}
/*******************************************************************************
* End of function text_write
*******************************************************************************/


/***********************************************************************************************************************
* Function Name : uint16_to_string
* Description   : Function converts a 16 bit integer into a character string, 
*                  inserts it into the array via the pointer passed at execution.
* Argument      : * output_string : Pointer to uint8_t array that will hold 
*                                   character string.
*                             pos : uint8_t number, element number to begin 
*                                   inserting the character string from (offset).
*                    input_number : 16 bit integer to convert into a string.
* Return value  : none
* Note            : No input validation is used, so output data can overflow the
*                  array passed.
***********************************************************************************************************************/
static void uint16_to_string (uint8_t * const output_string, uint8_t pos, uint16_t const input_number)
{
    /* Declare temporary character storage variable, and bit_shift variable */    
    uint8_t     a;
    uint8_t     bit_shift = 12u;
    
    /* Declare 16bit mask variable */
    uint16_t    mask = 0xF000;
    
    /* Loop through until each hex digit is converted to an ASCII character */
    while (bit_shift < 30u)
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
* End of function uint16_to_string
*******************************************************************************/

/* End user code. Do not edit comment generated here */
