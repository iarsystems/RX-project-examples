/*******************************************************************************
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
* Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/

/*******************************************************************************
* File Name     : r_rsk_utility.h
* Version       : 1.00
* Device(s)     : R5F51138AxFP
* Tool-Chain    : IAR Systems ICCRX
* Description   : Contains functions for converting an integer to decimal or
*                 hexadecimal string.
* Creation Date : 30/10/2014
*******************************************************************************/


/*******************************************************************************
User Includes (Project level includes)
*******************************************************************************/

/* Multi inclusion prevention macro */
#ifndef R_RSK_UTILITY_H
#define R_RSK_UTILITY_H

/*******************************************************************************
Function Prototypes
*******************************************************************************/
/* Integer to binary coded string function prototype */
void uint16_to_bcd_string (char * const output_string, uint8_t pos,
                                    uint16_t number);
                                    
/* Integer to ASCII string function prototype */
void uint16_to_string (char * const output_string, uint8_t pos, 
 const uint16_t input_number);

#endif

