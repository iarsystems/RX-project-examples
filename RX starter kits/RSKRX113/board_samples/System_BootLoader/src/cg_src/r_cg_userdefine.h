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
* Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_userdefine.h
* Version      : Applilet4 for RX113 V1.01.00.02 [31 Jul 2014]
* Device(s)    : R5F51138AxFP
* Tool-Chain   : IAR Systems ICCRX
* Description  : This file includes user definition.
* Creation Date: 2015-01-21
***********************************************************************************************************************/
#ifndef _USER_DEF_H
#define _USER_DEF_H

/***********************************************************************************************************************
User definitions
***********************************************************************************************************************/
#define FAST_INTERRUPT_VECTOR 0

/* Start user code for function. Do not edit comment generated here */

/* general definitions */
#define TRUE            (1)
#define FALSE           (0)
#define UNDECIDED       (2)
#ifndef  NULL
#define NULL         (0)
#endif /* define of NULL */

/* clock frequency defines */
#define CLOCK_FREQUENCY_MHZ (24)
#define CLOCK_PERIOD_NS     ((uint32_t)(1000 / CLOCK_FREQUENCY_MHZ))
#define CLOCK_COUNT_US      (1000 / (uint32_t)CLOCK_PERIOD_NS)
#define CLOCK_COUNT_MS      ((uint32_t)(1000 * CLOCK_COUNT_US))

/* RSK board hardware based definitions */
#define LED_BLINK_COUNT_HALF_SEC ((500 * CLOCK_COUNT_MS) / 20)
#define LED_BLINK_COUNT_100_MSEC ((100 * CLOCK_COUNT_MS) / 20)

/* timings related to switch presses */
#define SWITCH_PRESS_MEASURE_TIME_MS    (100)
#define SWITCH_PRESS_DELAY_COUNTS       (SWITCH_PRESS_MEASURE_TIME_MS * CLOCK_COUNT_MS / 20)

/* defines used to process received lines of code */
#define RECORD_NEXT                     (0)   /* ignore line and wait for the next one */
#define RECORD_FLASH                    (1)   /* store record in flash */
#define RECORD_FINISHED                 (2)   /* last record of file */
#define RECORD_HEADER                   (3)   /* header record */

/* defines to signify what file format is in use */
#define FILE_FORMAT_UNKNOWN             (0)
#define FILE_FORMAT_SREC                (1)
#define FILE_FORMAT_HEX                 (2)

/* End user code. Do not edit comment generated here */
#endif