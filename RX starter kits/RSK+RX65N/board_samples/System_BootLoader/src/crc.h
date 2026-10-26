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
* File Name    : crc.h
* Version      : 1.00
* Device(s)    : R5F565N9AxFB
* Tool-Chain   : CCRX
* H/W Platform : RSK+RX65N
* Description  : CRC checksum calculations on application flash block
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 30.06.2016 1.00     First Release
***********************************************************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef CRC_H_
#define CRC_H_

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* This is the address of the 'RESET' vector in the user's fixed vector table */
#define APP_VECTOR_START       (0XFFFFFF70U)

/* This is the last address of the user's fixed vector table, used for checking jump address*/
#define APP_VECTOR_END         (0xFFFFFF48U)

/* Start of the user application code */
/* Ensure this is the very start of a flash block*/
/* By default this is the top of flash */
#define APP_START              (0xFFFFC000)

/* End of application area */
/* CRC calculation is performed for all addresses APP_START to APP_END */
/* Ensure this is the last address of a flash block, including the fixed vectors */
/* but not including the calculated checksum */
#define APP_END                (0xFFFFFF7B)

/* Location in application code space that contains stored checksum */
/* must be a multiple of 4 to allow the flash self-prog library to work */
#define APP_CRC_CHECKSUM_PTR   ((uint32_t *)(0xFFFFFF7C))

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/
uint8_t is_application_crc_ok (const uint16_t calculatedChecksum);
uint16_t read_app_crc_value (void);
void write_app_crc_value (uint16_t value);
uint16_t calc_app_crc_value (void);
uint8_t is_valid_address (const uint32_t checkAddress);

#endif /* CRC_H_ */
