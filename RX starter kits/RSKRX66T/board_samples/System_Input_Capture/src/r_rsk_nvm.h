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
* Copyright (C) 2016, 2017 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_rsk_nvm.h
* Version      : 1.00
* Device(s)    : R5F566TEAxFP
* Tool-Chain   : CCRX
* H/W Platform : RSKRX66T
* Description  : This file contains functions and definitions used in the r_rsk_nvm.c file.
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 30.06.2018 1.00     First Release
***********************************************************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef R_RSK_NVM_H
#define R_RSK_NVM_H

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* NVM records contain a 16-bit signature and are 16 bytes long as follows */
/* bytes 0-1      bytes 3-8        byte 9   bytes 10-13 bytes 14-15 */
/* 0xa5b6      timer counter value  padding   temp. float  ad value   */
#define NVM_SIGNATURE (0xa5b6)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
typedef struct
{
    uint16_t                    signature;
    uint16_t                    adc_value;
    double                      temperature;
    uint32_t                    adc_getcount;
    uint32_t                    dummy;
} st_nvm_record_t;

/***********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/
uint8_t R_NVM_Init(void);
uint8_t R_NVM_Store(const double temp, const uint16_t adc);
uint16_t R_NVM_GetRecordCount(void);
uint8_t R_NVM_GetRecord(const uint16_t record_no, st_nvm_record_t * const p_nvm_rec);
uint8_t R_NVM_Erase(void);
uint8_t R_NVM_SleepRecover (void);

#endif /* R_RSK_NVM_H */
