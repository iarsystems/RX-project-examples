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
* File Name     : lin_master.c
* Version       : 1.00
* Device        : R5F562GAADFP
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX62G
* Description   : This file contains functions used to initialise and 
                  handle LIN communications. 
*******************************************************************************/
/*******************************************************************************
* History       : 06.09.2012 Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
/* CMT RPDL function definitions */
#include "r_pdl_cmt.h"
/* CGC RPDL function definitions */
#include "r_pdl_cgc.h"
/* IO Port RPDL function definitions */
#include "r_pdl_io_port.h"
/* LIN RPDL function defintions */
#include "r_pdl_lin.h"
/* ADC RPDL function defintions */
#include "r_pdl_adc_10.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* Switch handler function definitions */
#include "switch.h"
/* Following header file provides function prototypes for LCD controlling
   functions & macro defines */
#include "lcd.h"
/* Contains delcarations for the functions defined in this file */
#include "lin_master.h"


/******************************************************************************
* System Definitions & Global Variables
******************************************************************************/ 
/* Declare storage variable for received data */
uint8_t gRx_Buffer[] = "=H\'WXYZ \0";
/* Declare temporary character string */
uint8_t lcd_buffer[] = "=H\'WXYZ \0";
/* Declare variable to store the A/D value during LIN self test */
uint16_t gADC_Result = 0x0;


/* Declarate variable used for updates of data reception statuses */
volatile bool transfer_in_progress = true;


/******************************************************************************
* Outline       : Init_LIN
* Description   : This function configures the LIN module as a master with a 
*                 callback function CB_ReadStatus. The CMT is also configured 
*                 to generate a 100us delay to allow the LIN module to settle 
*                 after initialisation. The swicth press callback function is 
*                 configured differently for the two different LIn operation 
*                 modes. For normal master mode communication with an external 
*                 LIN device, the CB_FrameTransmit function is set as callback 
*                 function on each switch release detection. Callback functions 
*                 CB_Self_Test and LIN_Self_Test are configured for switch press 
*                 and switch release, respectively.
* Argument      : none
* Return value  : none
******************************************************************************/
void Init_LIN(void)
{

    /* Declare error flag */
    bool err = true;

    /* Wake up the LIN transceiver */
    err &= R_IO_PORT_Write
    (
        PDL_IO_PORT_A_2,
        1
    );

    /* Set the IO to an output */
    err &= R_IO_PORT_Set
    (
        PDL_IO_PORT_A_2,
        PDL_IO_PORT_OUTPUT
    );

    /* Configure LIN channel 0 and select operation mode */
    err &= R_LIN_Create
                (
                    0,
                    PDL_LIN_ERROR_ALL_ENABLE,
                    PDL_LIN_TB_LOW_13 | PDL_LIN_TB_HIGH_1,
                    PDL_LIN_IBHR_SPACE_4 | PDL_LIN_IB_SPACE_3 | \
                    PDL_LIN_WAKE_SPEC_2 | PDL_LIN_WAKE_LENGTH_1,
                    19200,
                    CB_ReadStatus,
                    10
                );

    /* Allow 100us for the LIN bus to settle */
    err &= R_CMT_CreateOneShot
                (
                    2,
                    PDL_CMT_PERIOD,
                    100E-6,
                    PDL_NO_FUNC,
                    0
                );

#ifndef LIN_LOOPBACK_MODE
    /* Configure switch callback function */
    SetSwitchReleaseCallback(CB_FrameTransmit);
#else       
    /* Configure switch callback function */
    SetSwitchPressCallback(CB_Self_Test);
        
    /* Configure switch callback function */
    SetSwitchReleaseCallback(LIN_Self_Test);
#endif

    /* Halt in while loop when RPDL errors detected */
    while(!err);
}
/*****************************************************************************
* End of function main_lin_master
*****************************************************************************/


/*****************************************************************************
* Outline       : CB_ReadStatus
* Description   : This function is called on detection of received data or 
*                 errors. The function checks the status to confirm whether a 
*                 reception was successful or not. If successful, the 
*                 transfer_in_progress indicator is changed to false to  
*                 indicate that the transfer was completed and displays the 
*                 received data on the debug LCD.
* Argument      : none
* Return value  : none
*****************************************************************************/
void CB_ReadStatus(void)
{
    /* Declare error flag */
    bool err = true;
            
    /* Display the contents of the local string 
       lcd_buffer if reception was successful */
    if(transfer_in_progress == false)
    {
        /* Read the received 8 bytes */
        err &= R_LIN_Read
                    (
                        0,
                        gRx_Buffer,
                        8
                    );
        
            /* Copy the first two bytes of the receive buffer array */
            gADC_Result = (uint16_t)((gRx_Buffer[0] << 8) | gRx_Buffer[1]);
            
            /* Convert the 2byte hex into a string */
            Convert_16BitNumber_ToString(gADC_Result);      
        
        DisplayLCD(LCD_LINE2, lcd_buffer);  
    }

    /* Halt in while loop when RPDL errors detected */
    while(!err);
}
/******************************************************************************
* End of function CB_ReadStatus
******************************************************************************/

/******************************************************************************
* Outline       : CB_FrameTransmit
* Description   : This function is called on SW3 switch release. It uses the 
*                 CMT module to create a delay for the LIN bus to settle, 
*                 configures the LIN module for reception of 8 data bytes before 
*                 transmitting a LIN header.
* Argument      : none
* Return value  : none
******************************************************************************/
void CB_FrameTransmit(void)
{
    /* Declare error flag */
    bool err = true;
    
    /* Declare and iitialise variable to store the LIN statuses */
    uint16_t LIN_Status = 0x0;
    
    /* Check if switch pressed was SW3 */
    if(gSwitchFlag & SWITCHPRESS_3)
    {

        /* Allow 100us for the LIN bus to settle */
        err &= R_CMT_CreateOneShot
                            (
                                2,
                                PDL_CMT_PERIOD,
                                100E-6,
                                PDL_NO_FUNC,
                                0
                            );

        /* Configure the module for reception of  
           8 bytes from slave device with ID 0x11 */
        err &= R_LIN_Transfer
                        (
                            0,
                            PDL_LIN_RX_DATA | \
                            PDL_LIN_PARITY_CALCULATE | PDL_LIN_CHECKSUM_CLASSIC,
                            0x11,
                            PDL_NO_PTR,
                            8
                        );
        
        /* Allow 100us for the LIN bus to settle */
        err &= R_CMT_CreateOneShot
                            (
                                2,
                                PDL_CMT_PERIOD,
                                500E-6,
                                PDL_NO_FUNC,
                                0
                            );
                            
        /* Read the status of channel 0 */
        err &= R_LIN_GetStatus
                        (
                            0,
                            &LIN_Status,
                            PDL_NO_PTR
                        );

        /* Check for errors and if the reception was completed */
        if ((LIN_Status & 0x00000023) == 0x3u)
        {
            /* Set receive-complete flag to false */
            transfer_in_progress = false;
        }
        else
        {
            /* Inform user of error detection */
            DisplayLCD(LCD_LINE2, "Rx Fail!");
        }       
    }   
        
    /* Halt in while loop when RPDL errors detected */
    while(!err);
}
/*******************************************************************************
* End of function CB_FrameTransmit
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
******************************************************************************/
void uint16_ToString(uint8_t *output_string, uint8_t pos, uint16_t input_number)
{
    /* Declare temporary character storage variable, and bit_shift variable */  
    uint8_t a = 0x00, bit_shift = 12u;
    
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
        mask = (uint16_t)(mask >> 4u);
        
        /* Decrement the bit_shift counter by 4 (bits in a each digit) */
        bit_shift -= (uint8_t)(4u);
        
        /* Increment the output string location */
        pos++;
    }
}
/******************************************************************************
* End of function uint16_ToString
******************************************************************************/

/* LIN Self test */
#ifdef LIN_LOOPBACK_MODE

/******************************************************************************
* Outline       : LIN_Self_Test
* Description   : This function is called when SW3 is released. It verifies that 
*                 SW3 was released then proceeds to start the A/D conversion and 
*                 reads the result. The result is converted to a data string 
*                 format which is inverted before being loaded into the LIN data 
*                 registers for transmission. The string is transmitted and read 
*                 back internally. The debug LCD is called to display the string.
* Argument      : none
* Return value  : none
******************************************************************************/
void LIN_Self_Test(void)
{
    /* Declare error flag */
    bool err = true;

    /* Check if switch pressed was SW3 */
    if(gSwitchFlag & SWITCHPRESS_3)
    {
        /* Trigger AD conversion */
        err &=  R_ADC_10_Control
                (
                    PDL_ADC_10_0_ON
                );
        
        /* Poll ADC unit until conversion is complete and store result in the
           global variable usADC_Result */  
        err &=  R_ADC_10_Read
                (
                    0,
                    &gADC_Result
                );
        
        /* Convert ADC result into a character string, and store in the local
           string lcd_buffer */     
        uint16_ToString(lcd_buffer, 3, gADC_Result);

        /* Reload the transmit data */
        LIN0.LDB1 = (uint8_t)(~lcd_buffer[0]);
        LIN0.LDB2 = (uint8_t)(~lcd_buffer[1]);
        LIN0.LDB3 = (uint8_t)(~lcd_buffer[2]);
        LIN0.LDB4 = (uint8_t)(~lcd_buffer[3]);
        LIN0.LDB5 = (uint8_t)(~lcd_buffer[4]);
        LIN0.LDB6 = (uint8_t)(~lcd_buffer[5]);
        LIN0.LDB7 = (uint8_t)(~lcd_buffer[6]);
        LIN0.LDB8 = (uint8_t)(~lcd_buffer[7]);

        /* Set the flag to true */
        transfer_in_progress = true;

        /* Read 8 bytes from ID 15 */
        err &= R_LIN_Transfer
                        (
                            0,
                            PDL_LIN_RX_DATA | \
                            PDL_LIN_PARITY_CALCULATE | PDL_LIN_CHECKSUM_CLASSIC,
                            0x31,
                            PDL_NO_PTR,
                            8
                        );

        /* Read 8 bytes */
        err &= R_LIN_Read
                        (
                            0,
                            gRx_Buffer,
                            8
                        );
        
        /* Display the contents of the local string lcd_buffer */
        DisplayLCD(LCD_LINE2, lcd_buffer);  
    }
    
    

    /* Halt in while loop when RPDL errors detected */
    while(!err);
}
/******************************************************************************
* End of function LIN_Self_Test
******************************************************************************/

/******************************************************************************
* Outline       : CB_Self_Test
* Description   : Configures the 10-bit A/D converter for single shot 
*                 operations on ADC channel 0 and switch callback function.
* Argument      : none
* Return value  : none
******************************************************************************/
void CB_Self_Test(void)
{
    /* Declare error flag */
    bool err = true;

    /* Select self-test operation mode */
        err &= R_LIN_Control
                    (
                        0,
                        PDL_LIN_MODE_SELFTEST,
                        PDL_NO_DATA
                    );

    /* Configure the ADC unit */
    err &=  R_ADC_10_Create
            (
                0,
                PDL_ADC_10_MODE_SINGLE | PDL_ADC_10_CHANNELS_OPTION_0,
                PDL_ADC_10_TRIGGER_SOFTWARE,
                50E6,
                4E-6,
                PDL_NO_FUNC,
                0
            );

    /* Halt in while loop when RPDL errors detected */
    while(!err);
}
/******************************************************************************
* End of function CB_Self_Test
******************************************************************************/

/* End pre-processor if statement */
#endif 

/******************************************************************************
* Outline       : Convert_16BitNumber_ToString
* Description   : Converts 16-bit number to the hex string 
* Argument      : (uint8_t) string - pointer to output buffer
* Return value  : none
******************************************************************************/
void Convert_16BitNumber_ToString (uint16_t string)
{
    uint8_t a;
    
    lcd_buffer[0] = '=';
    lcd_buffer[1] = 'H';
    lcd_buffer[2] = '\'';
    a = (uint8_t)((string & 0xF000u)>> 12);
    lcd_buffer[3] = (uint8_t)((a < 0x0A) ? (a+0x30):(a+0x37));
    a = (uint8_t)((string & 0x0F00)>> 8);
    lcd_buffer[4] = (uint8_t)((a < 0x0A) ? (a+0x30):(a+0x37));
    a = (uint8_t)((string & 0x00F0)>> 4);
    lcd_buffer[5] = (uint8_t)((a < 0x0A) ? (a+0x30):(a+0x37));
    a = (uint8_t)(string & 0x000F);
    lcd_buffer[6] = (uint8_t)((a < 0x0A) ? (a+0x30):(a+0x37));
    /*  Add spaces */
    lcd_buffer[7] = ' ';
}
/******************************************************************************
* End of function Convert_16BitNumber_ToString
******************************************************************************/


