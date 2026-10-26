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
* File Name     : doc.c
* Version       : 1.00
* Device        : R5F5210B
* Tool-Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX210B
* Description   : Defines DOC & SCI operation functions
*******************************************************************************/
/*******************************************************************************
* History       : 14.05.2013  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/
/* Provides standard library function prototypes */
#include "stdlib.h"
/* Provides standard string function prototypes */
#include "string.h"
/* SCI RPDL function definitions */
#include "r_pdl_sci.h"
/* DOC RPDL function definitions */
#include "r_pdl_doc.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* LCD controlling function prototypes & macro definitions */
#include "lcd.h"
/* Declares functions defined in this file */
#include "doc.h"

/*******************************************************************************
* Function Prototypes
*******************************************************************************/
/* SCI receive interrupt callback function prototype declaration */
void CB_SCIReceive_DOC (void);

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Declaration of SCI receive buffer & character variable */
uint8_t gSCI_RXBuffer[5], gSCI_RXChar;
/* Declaration of the command string to clear the terminal screen */
const uint8_t gSCI_ClearScreen[9] = {27, 91, 50, 74, 0, 27, 91, 72, 0};
/* Input reference value */
extern const uint16_t gCompareReference_DOC;
/* Status registers to indicate completion of reception & status of DOC
   operation */
static uint8_t gInputDataReceived=0,gDOCStatus=0;

/*******************************************************************************
* Outline       : Init_DOC
* Description   : This function initialises the SCI channel connected to the
*                 RS232 connector on the RSK, and configures the DOC unit to 
*                 perform comparison of the input data with the reference data. 
*                 The function then displays the result on the terminal window.
* Argument      : none
* Return value  : none
*******************************************************************************/
void Init_DOC(void)
{
    /* Declare error flag */
    bool err = true;

    /* Clear the contents of the 'gSCI_RXBuffer' array */
    if(memset(gSCI_RXBuffer, 0u, sizeof(gSCI_RXBuffer)) == NULL)
    {
        /* Halt in while loop when errors detected */
        while(1);
    }   

    /* Configure the DOC module in comparison mode */
    err &=  R_DOC_Create
            (
                PDL_DOC_COMPARISON_MISMATCH,
                gCompareReference_DOC,
                PDL_NO_FUNC,
                PDL_NO_DATA 
            );

    /* Configure the SCI pins */
    err &=  R_SCI_Set
            (
                0,
                PDL_SCI_PIN_SCI0_TXD0_P20 | PDL_SCI_PIN_SCI0_RXD0_P21
            );
                
    /* Configure the SCI module for UART communication */
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
                &gSCI_RXChar,
                1,
                CB_SCIReceive_DOC,
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
                "Renesas RSKRX210B DOC Sample\r\n\n",
                0,
                PDL_NO_FUNC
            );

    /* Display sample instructions on the terminal */
    err &=  R_SCI_Send
        (
            0,
            PDL_NO_DATA,
            "Please enter input data (up to 4 digits): ",
            0,
            PDL_NO_FUNC
        );  
            
    /* Halt in while loop when RPDL errors detected */
    while(!err);
    
    while(1)
    {
        /* Check if the data comparison is over */
        if(gInputDataReceived != 0)
        {
            /* Clear the flag   */
            gInputDataReceived = 0;
            
            /* Disable reception    */
            err &=  R_SCI_Control 
                    (
                        0,
                        PDL_SCI_STOP_RX
                    );  
            
            /* Check the comparison result  */
            if((gDOCStatus & 0x01) != 0)
            {
                /* Send the comparison result to the terminal   */
                err &=  R_SCI_Send
                        (
                            0,
                            PDL_NO_DATA,
                            "\r\nReference Data = 1000 \r\n"\
                            "The Input data does not match with reference data\
                             \r\n\n",
                            0,
                            PDL_NO_FUNC
                        );                  
            }   
            else
            {
                /* Send the comparison result to the terminal   */
                err &=  R_SCI_Send
                        (
                            0,
                            PDL_NO_DATA,
                            "\r\nReference Data = 1000 \r\n"\
                            "The Input data matches with reference data\
                             \r\n\n",
                            0,
                            PDL_NO_FUNC
                        );                  
            }
            
            /* Display sample instructions on the terminal */
            err &=  R_SCI_Send
                    (
                        0,
                        PDL_NO_DATA,
                        "Please enter input data (up to 4 digits): ",
                        0,
                        PDL_NO_FUNC
                    );      
            
            /* Re-configure the SCI receive interrupt */
            err &=  R_SCI_Receive
                    (
                        0,
                        PDL_NO_DATA,
                        &gSCI_RXChar,
                        1,
                        CB_SCIReceive_DOC,
                        PDL_NO_FUNC
                    );          
            
            /* Halt in while loop when RPDL errors detected */
            while(!err);                
        }   
    }
}
/*******************************************************************************
* End of function Init_DOC
*******************************************************************************/

/*******************************************************************************
* Outline      : CB_SCIReceive_DOC 
* Description  : SCI receive interrupt callback function, which is executed
*                every time a character is received from the terminal. The
*                function writes the received data to the DOC register &
*                stores the comparison result in the variable 'gDOCStatus'. 
*                The input data is then displayed on the debug LCD.
* Argument     : none
* Return value : none
*******************************************************************************/
void CB_SCIReceive_DOC (void)
{
    /* Declare error flag */
    bool err = true;
    /* Counter for receive buffer   */
    static uint8_t receive_cnt;
    /* Local variable to hold the input & reference values */
    uint16_t received_input_value, ref_reg=0;
    /* Declare temporary character string */
    uint8_t lcd_buffer[9] = " IN=WXYZ";
    /* Declare SCI status container variable */
    uint8_t sci_status = 0x00;
    
    /* Check if the character is a number */
    if((gSCI_RXChar >= '0')&&(gSCI_RXChar <= '9'))
    {
        /* Copy the received data into a buffer */
        gSCI_RXBuffer[receive_cnt] = gSCI_RXChar;

        /* Echo back the character */
        err &=  R_SCI_Send
                (
                    0,
                    PDL_NO_DATA,
                    &gSCI_RXBuffer[receive_cnt],
                    0,
                    PDL_NO_FUNC
                );
            
        /* Increment the counter to point the next location */
        receive_cnt++;      
    }
    /* If the 'enter' key is detected, terminate the reception & analyse
       the received data */
    else if (gSCI_RXChar == 0x0D)
    {
        receive_cnt = 4;    
    }
    else
    {
        /* Skip the received character  */
    }
        
    /* If the reception is complete, compare the data using DOC */
    if(receive_cnt >= 4)
    {
        /* Set the flag to indicate that the complete data is received  */
        gInputDataReceived = 1;

        /* Reset the counter to the receive buffer  */
        receive_cnt = 0;

        /* Convert the received data into an integer value */
        received_input_value = (uint16_t)(atoi((char *)gSCI_RXBuffer)); 

        /* Write the input data to DOC input register   */
        err &=  R_DOC_Write
                (
                    &received_input_value,
                    1
                );      
        
        /* Read the status  */
        err &=  R_DOC_Read
                (
                    &gDOCStatus,
                    &ref_reg
                );  
        
        /* Wait until SCI channel is ready */
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

        /* Clear the line 2 of LCD display */
        Display_LCD(LCD_LINE2, " IN=    ");

        /* Copy the data input by the user to the LCD buffer & display
           on the LCD */
        strcpy((char*)&lcd_buffer[4], (char*)gSCI_RXBuffer);
        Display_LCD(LCD_LINE2, lcd_buffer);
        
        /* Clear the contents of the 'gSCI_RXBuffer' array */
        if(memset(gSCI_RXBuffer, 0u, sizeof(gSCI_RXBuffer)) == NULL)
        {
            /* Halt in while loop when errors detected */
            while(1);
        }
    }

    /* Re-configure the SCI receive interrupt */
    err &=  R_SCI_Receive
            (
                0,
                PDL_NO_DATA,
                &gSCI_RXChar,
                1,
                CB_SCIReceive_DOC,
                PDL_NO_FUNC
            );

    /* Halt in while loop when RPDL errors detected */  
    while(!err);
}
/******************************************************************************
* End of function CB_SCIReceive_DOC 
******************************************************************************/
