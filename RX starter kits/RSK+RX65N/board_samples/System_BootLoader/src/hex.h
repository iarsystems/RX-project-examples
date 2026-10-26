/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIESREGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : Hex.h
* Version      : 1.00
* Device(s)    : R5F565N9AxFB
* Tool-Chain   : CCRX
* H/W Platform : RSK+RX65N
* Description  : Header and defines for Hex manipulation routines.
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 30.06.2016 1.00     First Release
***********************************************************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef HEX_H_
#define HEX_H_

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
typedef enum
{
    HEX_NONE,
    HEX_DATA,
    HEX_FILE_END,
    HEX_EXTENDED_SEG_ADDR,
    HEX_START_SEGMENT_ADDRESS,
    HEX_EXTENDED_LINEAR_ADDR,
    HEX_START_LINEAR_ADDR
} hex_type_t;

/***********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/
uint8_t is_file_hex (char byte);
uint8_t get_hex_type (void);
uint8_t import_hex_file (const uint8_t byte);
char *get_hex_data (void);
uint8_t get_hex_num_data_bytes (void);
uint32_t get_hex_address (void);
uint32_t get_hex_start_address (void);
uint8_t is_hex_file_error (void);
char *get_hex_error_text (void);

#endif /* HEX_H_ */
