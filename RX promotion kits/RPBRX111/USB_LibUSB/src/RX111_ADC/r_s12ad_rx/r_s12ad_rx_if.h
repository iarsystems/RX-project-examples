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
* File Name    : r_s12ad_rx_if.h
* Description  : 12-bit A/D Converter driver interface file.
*******************************************************************************
* History : DD.MM.YYYY Version Description           
*******************************************************************************/

#ifndef S12AD_IF_H
#define S12AD_IF_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
/* Fixed width integer support. */
//#include <stdint.h>   Not needed for LibUSB package.
//#include <stdbool.h>
/* Used for configuring the S12AD code */
#include "r_s12ad_rx_config.h"


/******************************************************************************
Macro definitions
*******************************************************************************/
/* Version Number of API. */
#define ADC_VERSION_MAJOR       (0)
#define ADC_VERSION_MINOR       (4)


/*****************************************************************************
Typedef definitions
******************************************************************************/

/* ADC_OPEN() ARGUMENT DEFINITIONS */

typedef enum e_adc_mode
{
    ADC_MODE_SS_TEMPERATURE,        // single scan temperature sensor
    ADC_MODE_SS_INT_REF_VOLT,       // single scan internal ref voltage sensor
    ADC_MODE_SS_ONE_CH,             // single scan one channel
    ADC_MODE_SS_ONE_CH_DBLTRIG,     // on even triggers save to ADDBLDR & interrupt
    ADC_MODE_SS_MULTI_CH,           // 1 trigger source, scan multiple channels
    ADC_MODE_SS_MULTI_CH_GROUPED,   // 2 trigger sources, scan multiple channels
    ADC_MODE_SS_MULTI_CH_GROUPED_DBLTRIG_A,
    ADC_MODE_CONT_ONE_CH,           // continuous scan one channel
    ADC_MODE_CONT_MULTI_CH,         // continuous scan multiple channels
    ADC_MODE_MAX
} adc_mode_t;


typedef enum e_adc_trig             // trigger sources
{
    ADC_TRIG_ASYNC_ADTRG0=0,        // asynchronous trigger; not for Group modes
                                    //  nor double trigger modes
                                    // [non-dbltrg group mode: p1038 yes  p1028 no; no is correct]
                                    // [dbltrg non-group mode: p1036 yes & no; no is correct]
    ADC_TRIG_SYNC_TRG0AN=1,
    ADC_TRIG_SYNC_TRG0BN=2,
    ADC_TRIG_SYNC_TRGAN=3,
    ADC_TRIG_SYNC_TRG0EN=4,
    ADC_TRIG_SYNC_TRG0FN=5,
    ADC_TRIG_SYNC_TRG4AN=6,
    ADC_TRIG_SYNC_TRG4BN=7,
    ADC_TRIG_SYNC_TRG4ABN=8,
    ADC_TRIG_SYNC_ELC=9,
    ADC_TRIG_HW_MAX,
    ADC_TRIG_SOFTWARE=16            // software trigger; not for Group modes
                                    //  nor double trigger modes
} adc_trig_t;


typedef enum e_adc_add
{
    ADC_ADD_OFF=0,                  // addition is turned off for chans/sensors
                                    // not allowed for temperature sensor
                                    // [p1018 and 1026 yes, p1030 no; no is correct]
    ADC_ADD_TWO_SAMPLES=1,
    ADC_ADD_THREE_SAMPLES=2,
    ADC_ADD_FOUR_SAMPLES=3,
    ADC_ADD_MAX
} adc_add_t;

typedef enum e_adc_align
{
    ADC_ALIGN_RIGHT = 0x0000,
    ADC_ALIGN_LEFT  = 0x8000
} adc_align_t;

typedef enum e_adc_clear
{
    ADC_NO_CLEAR_AFTER_READ = 0x0000,
    ADC_CLEAR_AFTER_READ    = 0x0020
} adc_clear_t;

typedef enum e_adc_speed
{
    ADC_CONVERT_NORM_SPEED = 0x0000,
    ADC_CONVERT_HI_SPEED   = 0x0400
} adc_speed_t;


typedef struct st_adc_cfg
{
    adc_trig_t      trigger;        // default and Group A trigger source
    uint8_t         priority;       // for S12ADIO int; 1=lo 15=hi 0=off/polled
    adc_add_t       add_cnt;        // must be off for temperature sensor
    adc_align_t     alignment;      // ignored if addition used
    adc_clear_t     clearing;
    adc_speed_t     conv_speed;
    adc_trig_t      groupb_trigger;     // valid only for group modes
    uint8_t         groupb_priority;    // GBADI interrupt priority; 0-15
} adc_cfg_t;


typedef enum e_adc_err      // ADC API error codes
{
    ADC_SUCCESS=0,
    ADC_ERR_AD_LOCKED,      // Open() call is in progress elsewhere
    ADC_ERR_AD_NOT_CLOSED,  // peripheral still running in another mode
    ADC_ERR_MISSING_PTR,    // missing required pointer argument
    ADC_ERR_INVALID_ARG,    // argument is not valid for parameter
    ADC_ERR_ILLEGAL_ARG,    // argument is illegal for mode
    ADC_ERR_SCAN_NOT_DONE   // default, Group A, or Group B scan not done
} adc_err_t;


/* CALLBACK FUNCTION ARGUMENT DEFINITIONS */

typedef enum e_adc_cb_evt           // callback function events
{
    ADC_EVT_SCAN_COMPLETE,          // normal/Group A scan complete
    ADC_EVT_GROUPB_SCAN_COMPLETE    // Group B scan complete
} adc_cb_evt_t;

typedef struct st_adc_cb_args       // callback arguments
{
    adc_cb_evt_t   event;
} adc_cb_args_t;


/* ADC_CONTROL() ARGUMENT DEFINITIONS */

typedef enum e_adc_cmd
{
    ADC_CMD_ENABLE_CHANS,           // enables chans and INT(s) if priority != 0
    ADC_CMD_ENABLE_TEMP_SENSOR,     // enables sensor and INT if priority != 0
    ADC_CMD_ENABLE_VOLT_SENSOR,     // enables sensor and INT if priority != 0
    ADC_CMD_SET_SAMPLE_STATE_CNT,
    ADC_CMD_TRIG_ENABLE,            // ADCSR.TRGE=1 for sync/async trigs
    ADC_CMD_TRIG_DISABLE,           // ADCSR.TRGE=0 for sync/async trigs
    ADC_CMD_SOFTWARE_TRIG_START_SCAN,
    ADC_CMD_INT_DISABLE,            // interrupt disable; ADCSR.ADIE=0
    ADC_CMD_INT_ENABLE,             // interrupt enable;  ADCSR.ADIE=1
    ADC_CMD_INT_DISABLE_GROUPB,     // interrupt disable; ADCSR.GBADIE=0
    ADC_CMD_INT_ENABLE_GROUPB,      // interrupt enable;  ADCSR.GBADIE=1
    ADC_CMD_CHECK_SCAN_DONE,
    ADC_CMD_CHECK_SCAN_DONE_GROUPA,
    ADC_CMD_CHECK_SCAN_DONE_GROUPB,
    ADC_CMD_MAX
} adc_cmd_t;


#define ADC_CH0_MASK    (1<<0)
#define ADC_CH1_MASK    (1<<1)
#define ADC_CH2_MASK    (1<<2)
#define ADC_CH3_MASK    (1<<3)
#define ADC_CH4_MASK    (1<<4)
#define ADC_CH6_MASK    (1<<6)
#define ADC_CH8_MASK    (1<<8)
#define ADC_CH9_MASK    (1<<9)
#define ADC_CH10_MASK   (1<<10)
#define ADC_CH11_MASK   (1<<11)
#define ADC_CH12_MASK   (1<<12)
#define ADC_CH13_MASK   (1<<13)
#define ADC_CH14_MASK   (1<<14)
#define ADC_CH15_MASK   (1<<15)

typedef struct st_adc_ch_cfg        // for ADC_CMD_ENABLE_CHANS
{
    uint16_t        chan_mask;      // channels/bits 0-4, 6, 8-15
    uint16_t        groupb_mask;    // valid for group modes
    uint16_t        add_mask;       // valid if add enabled in Open()
} adc_ch_cfg_t;


typedef enum e_adc_sst_reg          // sample state registers
{
    ADC_SST_CH0=0,
    ADC_SST_CH1,
    ADC_SST_CH2,
    ADC_SST_CH3,
    ADC_SST_CH4,
    ADC_SST_CH6,
    ADC_SST_CH8_TO_15,
    ADC_SST_TEMPERATURE,
    ADC_SST_VOLTAGE,
    ADC_SST_NUM_REGS
} adc_sst_reg_t;

typedef struct st_adc_time          // for ADC_CMD_SET_SAMPLE_STATE_CNT
{
    adc_sst_reg_t   reg_id;
    uint8_t         num_states;     // 7-255; default=20; ch8-15 same
} adc_time_t;


/* ADC_READ() ARGUMENT DEFINITIONS */

typedef enum e_adc_reg
{
    ADC_CH0_REG=0,
    ADC_CH1_REG=1,
    ADC_CH2_REG=2,
    ADC_CH3_REG=3,
    ADC_CH4_REG=4,
    ADC_CH6_REG=6,
    ADC_CH8_REG=8,
    ADC_CH9_REG=9,
    ADC_CH10_REG=10,
    ADC_CH11_REG=11,
    ADC_CH12_REG=12,
    ADC_CH13_REG=13,
    ADC_CH14_REG=14,
    ADC_CH15_REG=15,
    ADC_TEMP_REG=16,
    ADC_VOLT_REG=17,
    ADC_DBLTRIG_REG=18,
    ADC_MAX_NUM_REGS
} adc_reg_t;


/* ADC_READALL() ARGUMENT DEFINITIONS */

typedef struct st_adc_data
{
    uint16_t    chan[16];       // chan[5] and chan[7] are not used
    uint16_t    dbltrig;
} adc_data_t;


/*****************************************************************************
Public Functions
******************************************************************************/
adc_err_t R_ADC_Open(adc_mode_t const   mode,
                     adc_cfg_t * const  p_cfg,
                     void               (* const p_callback)(void *p_args));

adc_err_t R_ADC_Control(adc_cmd_t const cmd,
                        void * const    p_cfg);

adc_err_t R_ADC_Read(adc_reg_t const    reg_id,
                     uint16_t * const   p_data);

adc_err_t R_ADC_ReadAll(adc_data_t * const  p_data);

void R_ADC_Close(void);

uint32_t  R_ADC_GetVersion(void);

                                    
#endif /* S12AD_IF_H */

