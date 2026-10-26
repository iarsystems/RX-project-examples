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
* File Name     : doc.c
* Version       : 1.00
* Device        : R5F52206
* Tool-Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX220
* Description   : Defines DOC & SCI operation functions
*******************************************************************************/
/*******************************************************************************
* History       : 14.12.2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* System Includes 
*******************************************************************************/

/* Following header file provides standard library functions */
#include <stdlib.h>

/* Following header file provides standard integer type definitions. */
#include <stdint.h>

/* Following header file provides string type definitions. */
#include <string.h>


/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/

/* Defines RX220 port registers */
#include <iorx220.h>

/* Defines RX220 LEDs and switches */
#include "rskrx220def.h"

/* LCD controlling function prototypes & macro definitions */
#include "lcd.h"

/* Declares functions defined in this file */
#include "doc.h"

/* SCI function prototypes */
#include "async.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/

/* Declaration of SCI receive buffer */
char gSCI_RXBuffer[5];

/* Input reference value */
extern const uint16_t g_compare_reference_doc;


/*******************************************************************************
* Function Name: init_doc
* Description  : This function initialises the SCI channel connected to the
*                RS232 connector on the RSK, and configures the DOC unit to 
*                perform comparison of the input data with the reference data. 
*                The function then displays the result on the terminal window.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void init_doc (void)
{
    /* Disable write protection of the system registers */
    SYSTEM.PRCR.WORD = PROTECTOFF;
    
    /* Cancel the DOC module clock stop mode */
    MSTP_DOC = 0;
    
    /* Disable write protection of the system registers */
    SYSTEM.PRCR.WORD = PROTECTON;
    
    /* Configure the DOC module in comparison mode */
    DOC.DOCR.BIT.OMS = 0;
    
    /* Set DOC condition to mismatch */
    DOC.DOCR.BIT.DCSEL = 0;
    
    /* Initialise the SCI interface */
    sci_init(cb_sci_receive_doc);
    
    /* Write the reference value to the DOC data register */
    DOC.DODSR = g_compare_reference_doc;
    
    /* Print a welcome message and instructions on the terminal */
    sci_transmit("Renesas RSKRX220 DOC Sample\r\n\n");
    sci_transmit("Please enter input data (up to 4 digits): ");
}
/*******************************************************************************
* End of function init_doc
*******************************************************************************/

/*******************************************************************************
* Function Name: cb_sci_receive_doc 
* Description  : SCI receive interrupt callback function, which is executed
*                every time a character is received from the terminal. The
*                function writes the received data to the DOC register &
*                stores the comparison result in the variable 'gDOCStatus'. 
*                The input data is then displayed on the debug LCD.
* Arguments    : (uint8_t)character - Character received from terminal.
* Return Value : none
*******************************************************************************/
void cb_sci_receive_doc (const char character)
{
    /* Counter for receive buffer */
    static uint8_t receive_cnt;
    
    /* Declare temporary character string */
    char lcd_buffer[9] = " IN=WXYZ";
        
    /* Check if the character is a number */
    if ((character >= '0') && (character <= '9'))
    {
        /* Copy the received data into a buffer */
        gSCI_RXBuffer[receive_cnt] = character;

        /* Echo back the character */
        sci_transmit(&gSCI_RXBuffer[receive_cnt]);

        /* Increment the counter to point the next location */
        receive_cnt++;      
    }
    
    /* If the 'enter' key is detected, terminate the reception & analyse
       the received data */
    else if (0x0D == character)
    {
        receive_cnt = RECEIVE_TOTAL;    
    }
    else
    {
        /* Skip the received character  */
    }
        
    /* If the reception is complete, compare the data using DOC */
    if (receive_cnt >= RECEIVE_TOTAL)
    {
        /* Reset the counter to the receive buffer  */
        receive_cnt = 0;

        /* Convert the received data into an integer value */
        const uint16_t received_input_value = 
            (uint16_t)(atoi((char *)gSCI_RXBuffer)); 

        /* Write the input data to DOC input register */
        DOC.DODIR = received_input_value;
        
        /* Check if the DOC result is a mismatch */
        if (DOC.DOCR.BIT.DOPCF)
        {
            /* Write a message on the terminal */
            sci_transmit("\r\nReference Data = 1000 \r\n"\
                "The Input data does not match with reference data\r\n\n");
                
            /* Clear the DOC result flag */
            DOC.DOCR.BIT.DOPCFCL = 1;
        }
        else
        {
            /* Result is a match - write a message to the terminal */
            sci_transmit("\r\nReference Data = 1000 \r\n"\
                "The Input data matches with reference data\r\n\n");
        }
        
        /* Reprint instructions on terminal */
        sci_transmit("Please enter input data (up to 4 digits): ");
        
        /* Clear the line 2 of LCD display */
        Display_LCD(LCD_LINE2, " IN=    ");

        /* Copy the data input by the user to the LCD buffer & display
           on the LCD */
        strcpy(&lcd_buffer[4], gSCI_RXBuffer);
        Display_LCD(LCD_LINE2, lcd_buffer);
        
        /* Clear the contents of the 'gSCI_RXBuffer' array */
        if (NULL == memset(gSCI_RXBuffer, 0u, sizeof(gSCI_RXBuffer)))
        {
            /* Halt in while loop when errors detected */
            while (1)
            {
            }
        }
    }
}
/******************************************************************************
* End of function cb_sci_receive_doc 
******************************************************************************/

