/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
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
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_userdefine.h
* Version      : 1.0.0
* Device(s)    : R5F572TKCxFB
* Description  : User header file for code generation.
* Creation Date: 2019-05-21
***********************************************************************************************************************/

#ifndef CG_USER_DEF_H
#define CG_USER_DEF_H

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/
/* Start user code for register. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* Start user code for macro define. Do not edit comment generated here */

/* General definitions */
#define TRUE            (1)
#define FALSE           (0)
#define UNDECIDED       (2)
#ifndef  NULL
#define NULL         (0)
#endif /* Define of NULL */

/* Clock frequency defines */
#define CLOCK_FREQUENCY_MHZ (80)
#define CLOCK_PERIOD_NS     ((uint32_t)(1000 / CLOCK_FREQUENCY_MHZ))
#define CLOCK_COUNT_US      (1000 / (uint32_t)CLOCK_PERIOD_NS)
#define CLOCK_COUNT_MS      ((uint32_t)(1000 * CLOCK_COUNT_US))

/* RSK board hardware based definitions */
#define LED_BLINK_COUNT_HALF_SEC ((500 * CLOCK_COUNT_MS) / 20)
#define LED_BLINK_COUNT_100_MSEC ((100 * CLOCK_COUNT_MS) / 20)

/* Timings related to switch presses */
#define SWITCH_PRESS_MEASURE_TIME_MS    (100)
#define SWITCH_PRESS_DELAY_COUNTS       (SWITCH_PRESS_MEASURE_TIME_MS * CLOCK_COUNT_MS / 20)

/* Defines used to process received lines of code */
#define RECORD_NEXT                     (0)   /* Ignore line and wait for the next one */
#define RECORD_FLASH                    (1)   /* Store record in flash */
#define RECORD_FINISHED                 (2)   /* Last record of file */
#define RECORD_HEADER                   (3)   /* Header record */

/* Defines to signify what file format is in use */
#define FILE_FORMAT_UNKNOWN             (0)
#define FILE_FORMAT_SREC                (1)
#define FILE_FORMAT_HEX                 (2)

/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
/* Start user code for type define. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
/* Start user code for function. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#endif

