/* Adapted for use with IAR Embedded Workbench */

/*******************************************************************************
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.    
*******************************************************************************/
/******************************************************************************
* File Name     : adc_mtu_timer.h
* Version       : 1.0
* Device(s)     : R5F562N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.1
* OS            : None
* H/W Platform  : YRDKRX62N
* Description   : Provides declarations for functions defined in adc_mtu_timer.c
* Limitations   : None
*******************************************************************************
* History : DD.MM.YYYY   Version    Description
*         : 21.09.2011   1.00       First release
*******************************************************************************/
#ifndef ADC_TIMER_H      // Multiple inclusion prevention macro
#define ADC_TIMER_H

#include <stdint.h>

/*******************************************************************************
* Exported global functions (accessible by other files)
*******************************************************************************/
void adc_mtu_init(uint16_t sample_rate);
void adc_mtu_start(void);
void adc_mtu_stop(void);

#endif // End of multiple inclusion prevention macro
