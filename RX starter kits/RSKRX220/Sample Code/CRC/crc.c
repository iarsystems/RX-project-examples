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
* File Name     : crc.c
* Version       : 1.00
* Device        : R5F52206
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX220
* Description   : Defines CRC functions used in this sample.
*******************************************************************************/
/*******************************************************************************
* History       : 14.12.2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* System Includes 
*******************************************************************************/

/* Defines standard C99 integers used in this file */
#include <stdint.h>

/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/

/* Declares functions defined in this file */
#include "crc.h"

/* Defines RX220 registers */
#include <iorx220.h>

/* Defines LCD functions used in this file */
#include "lcd.h"

/* Defines RX220 LEDs and switches */
#include "rskrx220def.h"

/*******************************************************************************
* Function Name: init_crc
* Description  : This function initialises the CRC peripheral.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void init_crc (void)
{
    /* Disable register protection */
    SYSTEM.PRCR.WORD = PROTECTOFF;  
    
    /* Enable clock to the CRC module */
    MSTP_CRC = 0x0;
    
    /* Set the CRC module to perform 8bit CRC calculation:
       X^8 + X^2 + X + 1 */
    CRC.CRCCR.BIT.GPS = 0x1u;
    
    /* Re-enable register protection */
    SYSTEM.PRCR.WORD = PROTECTON;  
}
/*******************************************************************************
* End of function init_crc
*******************************************************************************/

/*******************************************************************************
* Function Name: calculate_crc
* Description  : This function calculates the CRC checksum of a 16bit integer
*                input, and returns the result.
* Arguments    : (const uint16_t)data_input - Data input to perform CRC 
*                                             calculation on
* Return Value : uint16_t                   - Checksum result from CRC 
*                                             calculation
*******************************************************************************/
uint16_t calculate_crc (const uint16_t data_input)
{
    /* Clear the CRC data input register */
    CRC.CRCCR.BIT.DORCLR = 0x1u;
    
    /* Write the input data to the CRC data input register */
    CRC.CRCDIR = (unsigned char)data_input;
    
    /* Fetch the CRC calculation, and return value */
    return CRC.CRCDOR;
}
/*******************************************************************************
* End of function calculate_crc
*******************************************************************************/

