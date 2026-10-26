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
* File Name    : srec.h
* Version      : 1.00
* Device(s)    : R5F565N9AxFB
* Tool-Chain   : CCRX
* H/W Platform : RSK+RX65N
* Description  : Decode S-Record format data into flash
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 30.06.2016 1.00     First Release
***********************************************************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef SREC_H_
#define SREC_H_

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
typedef enum
{
    SREC_NONE,
    SREC_HEADER,
    SREC_16BIT_ADDR,
    SREC_24BIT_ADDR,
    SREC_32BIT_ADDR,
    SREC_RECORD_COUNT,
    SREC_RECORD_START32,
    SREC_RECORD_START24,
    SREC_RECORD_START16 /*, SREC_DATA_RECORD */
} srec_type_t;

/***********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/
uint8_t is_file_srec (char byte);
uint8_t get_srec_type (void);
uint8_t import_srec_file (const uint8_t byte);
char *get_srec_data (void);
uint8_t get_srec_num_data_bytes (void);
uint32_t get_srec_address (void);
uint32_t get_srec_start_address (void);
uint8_t is_srec_file_error (void);
char *get_srec_error_text (void);

#endif /* SREC_H_ */
