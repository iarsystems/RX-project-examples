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
* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/******************************************************************************
* File Name    : r_s12ad_rx.h
* Description  : Definitions and structures used internally by the driver.
*******************************************************************************/
/******************************************************************************
* History : DD.MM.YYYY Version Description           
*******************************************************************************/

#ifndef S12AD_PRIVATE_H
#define S12AD_PRIVATE_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
/* Fixed width integer support. */
#include <stdint.h>
/* bool support */
//#include <stdbool.h>

#include "platform.h"
#include "r_s12ad_rx_if.h"

/******************************************************************************
Macro definitions
*******************************************************************************/
//TODO: BSP NEEDS TO ADD EQUATES FOR PACKAGE PIN COUNT.
// IF ADD NEW PACKAGE, BETTER FOR ALL MODULES IF CHANGE IN JUST ONE PLACE.
// 4/4/13: Knew it. 36-pin being dropped and 40-pin being added

#if (defined(BSP_PACKAGE_LFQFP64) || defined(BSP_PACKAGE_LQFP64) || defined(BSP_PACKAGE_TFLGA64))
// valid: 1111 1111 0101 1111; invalid 0000 0000 1010 0000
#define ADC_INVALID_CH_MASK     (0x00A0)
#elif (defined(BSP_PACKAGE_LFQFP48) || defined(BSP_PACKAGE_VQFN48))
// valid: 1001 1111 0100 0111; invalid 0110 0000 1011 1000
#define ADC_INVALID_CH_MASK     (0x60B8)
#elif (defined(BSP_PACKAGE_HWQFN36) || defined(BSP_PACKAGE_WFLGA36) || defined(BSP_PACKAGE_SSOP36))
// valid: 0001 1111 0000 0110; invalid 1110 0000 1111 1001
#define ADC_INVALID_CH_MASK     (0xE0F9)
#else
    #error "ERROR - BSP_CFG_MCU_PART_PACKAGE - Unknown package chosen in r_bsp_config.h"
#endif

/******************************************************************************
Typedef definitions
*******************************************************************************/
typedef struct st_adc_ctrl          // ADC Control Block
{
    adc_mode_t      mode;           // operational mode
    bool            opened;
    void          (*callback)(void *p_args);
} adc_ctrl_t;


/* ADCSR register ADCS field */
typedef enum e_adc_adcs
{
    ADC_ADCS_SINGLE_SCAN=0,
    ADC_ADCS_GROUP_SCAN=1,
    ADC_ADCS_CONT_SCAN=2,
    ADC_ADCS_MAX
} adc_adcs_t;

/******************************************************************************
Exported global variables
*******************************************************************************/

/******************************************************************************
Exported global functions (to be accessed by other files)
*******************************************************************************/


#endif
