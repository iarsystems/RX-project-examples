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
* Device        : R5F563TE
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX63T144
* Description   : Defines CRC functions used in this sample.
*******************************************************************************/
/*******************************************************************************
* History       : 05.12.2013  Ver. 1.00 First Release
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

/* Defines R5F563TE registers */
#include <iorx63t.h>

/* Defines LCD functions used in this file */
#include "lcd.h"

/*******************************************************************************
* Function Name : Init_CRC
* Description   : This function initialises the CRC peripheral.
* Argument      : none
* Return value  : none
*******************************************************************************/
void Init_CRC (void)
{
    /* Disable register protection */
    SYSTEM.PRCR.WORD = 0xA503u;

    /* Enable clock to the CRC module */
    MSTP_CRC = 0;

    /* Set the CRC module to perform 8bit CRC calculation:
        X^8 + X^2 + X + 1 */
    CRC.CRCCR.BIT.GPS = 0x1u;

    /* Re-enable register protection */
    SYSTEM.PRCR.BIT.PRKEY = 0xA5u;
    SYSTEM.PRCR.WORD &= 0xFF00u;
}
/*******************************************************************************
* End of function Init_CRC
*******************************************************************************/

/*******************************************************************************
* Function Name : Calculate_CRC
* Description   : This function calculates the CRC checksum of a 16bit integer
*                 input, and returns the result.
* Argument      : uint8_t : Data input to perform CRC calculation on
* Return value  : uint16_t : Checksum result from CRC calculation
*******************************************************************************/
uint16_t Calculate_CRC (const uint8_t data_input)
{
    /* Clear the CRC data input register */
    CRC.CRCCR.BIT.DORCLR = 1;

    /* Write the input data to the CRC data input register */
    CRC.CRCDIR = data_input;

    /* Fetch the CRC calculation, and return value */
    return CRC.CRCDOR;
}
/*******************************************************************************
* End of function Calculate_CRC
*******************************************************************************/

