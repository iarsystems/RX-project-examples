/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No 
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all 
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM 
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES 
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS 
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of 
* this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer 
*
* Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.    
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_rsk_nvm.h
* Version      : 1.00
* Device(s)    : R5F523T5AxFM
* Tool-Chain   : CCRX
* H/W Platform : RSKRX23T
* Description  : This file contains functions and definitions used in the nvm.c file.
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 24.08.2015 1.00     First Release
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#ifndef R_RSK_NVM_H
#define R_RSK_NVM_H


/* NVM records contain a 16-bit signature and are 16 bytes long as follows */
/* bytes 0-1      bytes 3-8        byte 9   bytes 10-13 bytes 14-15 */
/* 0xa5b6      rtc_counter_value  padding   temp. float  ad value   */
#define NVM_SIGNATURE (0xa5b6)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
#pragma pack(1)
typedef struct
{
    uint16_t                    signature;
    uint32_t                    adc_getcount;
    uint32_t                    dummy;
    double                      temperature;
    uint16_t                    adc_value;
} nvm_record_t;
#pragma pack()
/***********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/
uint8_t R_NVM_Init(void);
uint8_t R_NVM_Store(const uint16_t adc);
uint16_t R_NVM_GetRecordCount(void);
uint8_t R_NVM_GetRecord(const uint16_t record_no, nvm_record_t * const p_nvm_rec);
uint8_t R_NVM_Erase(void);
uint8_t R_NVM_SleepRecover (void);

#endif
