/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. This 
* software is owned by Renesas Electronics Corporation and is protected under 
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING 
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT 
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR 
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE 
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software 
* and to discontinue the availability of this software.  By using this software, 
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_cac.h
* Version      : Applilet4 for RX111 V1.00.00.05 [06 Jun 2013]
* Device(s)    : R5F51115AxFM
* Tool-Chain   : IAR Systems icc78k0r
* Description  : This file implements device driver for CAC module.
* Creation Date: 2013-08-16
***********************************************************************************************************************/
#ifndef CAC_H
#define CAC_H

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/
/* 
    CAC Control Register 0 (CACR0)
*/
/* Clock Frequency Measurement Enable (CFME) */
#define _00_CAC_DISABLE                     (0x00U) /* Clock frequency measurement is disabled */
#define _01_CAC_ENABLE                      (0x01U) /* Clock frequency measurement is enabled */

/*
    CAC Control Register 1 (CACR1)
*/
/* CACREF Pin Input Enable (CACREFE) */
#define _00_CAC_CACREF_PIN_DISABLE          (0x00U) /* CACREF pin input is disabled */
#define _01_CAC_CACREF_PIN_ENABLE           (0x01U) /* CACREF pin input is enabled */
/* Frequency Measurement Clock Select (FMCS[2:0]) */
#define _00_CAC_OUTPUT_MEASURE_MAIN_OSC     (0x00U) /* Output clock of main clock oscillator */
#define _02_CAC_OUTPUT_MEASURE_SUB_OSC      (0x02U) /* Output clock of sub-clock oscillator */
#define _04_CAC_OUTPUT_MEASURE_HOCO         (0x04U) /* Output clock of high-speed on-chip oscillator */
#define _06_CAC_OUTPUT_MEASURE_LOCO         (0x06U) /* Output clock of low-speed on-chip oscillator */
#define _08_CAC_OUTPUT_MEASURE_IWDTCLK      (0x08U) /* Output clock of IWDT-dedicated on-chip oscillator */
#define _0A_CAC_OUTPUT_MEASURE_PCLKB        (0x0AU) /* Peripheral module clock (PCLKB) */
/* Timer Count Clock Source Select (TCSS[1:0]) */
#define _00_CAC_TIMER_CLOCK_SOURCE_1        (0x00U) /* No division */
#define _10_CAC_TIMER_CLOCK_SOURCE_4        (0x10U) /* x 1/4 clock */
#define _20_CAC_TIMER_CLOCK_SOURCE_8        (0x20U) /* x 1/8 clock */
#define _30_CAC_TIMER_CLOCK_SOURCE_32       (0x30U) /* x 1/32 clock */
/* Valid Edge Select (EDGES[1:0]) */
#define _00_CAC_VALID_EDGE_RISING           (0x00U) /* Rising edge */
#define _40_CAC_VALID_EDGE_FALLING          (0x40U) /* Falling edge */
#define _80_CAC_VALID_EDGE_BOTH             (0x80U) /* Both rising and falling edges */

/*
    CAC Control Register 2 (CACR2)
*/
/* Reference Signal Select (RPS) */
#define _00_CAC_REF_SIGNAL_EXTERNAL         (0x00U) /* CACREF pin input */
#define _01_CAC_REF_SIGNAL_INTERNAL         (0x01U) /* Internally generated signal */
/* Reference Signal Generation Clock Select (RSCS[2:0]) */
#define _00_CAC_REF_CLOCK_MAIN_OSC          (0x00U) /* Output clock of main clock oscillator */
#define _02_CAC_REF_CLOCK_SUB_OSC           (0x02U) /* Output clock of sub-clock oscillator */
#define _04_CAC_REF_CLOCK_HOCO              (0x04U) /* Output clock of high-speed on-chip oscillator */
#define _06_CAC_REF_CLOCK_LOCO              (0x06U) /* Output clock of low-speed on-chip oscillator */
#define _08_CAC_REF_CLOCK_IWDTCLK           (0x08U) /* Output clock of IWDT-dedicated on-chip oscillator */
#define _0A_CAC_REF_CLOCK_PCLKB             (0x0AU) /* Peripheral module clock (PCLKB) */
/* Reference Signal Generation Clock Frequency Division Ratio Select (RCDS[1:0]) */
#define _00_CAC_REF_CLOCK_SOURCE_32         (0x00U) /* x 1/32 clock */
#define _10_CAC_REF_CLOCK_SOURCE_128        (0x10U) /* x 1/128 clock */
#define _20_CAC_REF_CLOCK_SOURCE_1024       (0x20U) /* x 1/1024 clock */
#define _30_CAC_REF_CLOCK_SOURCE_8192       (0x30U) /* x 1/8192 clock */
/* Digital Filter Selection (DFS[1:0]) */
#define _00_CAC_DIGITAL_FILTER_DISABLED     (0x00U) /* Digital filtering is disabled */
#define _40_CAC_DIGITAL_FILTER_DIV_1        (0x40U) /* Digital filter sampling is at frequency measuring clock / 1 */
#define _80_CAC_DIGITAL_FILTER_DIV_4        (0x80U) /* Digital filter sampling is at frequency measuring clock / 4 */
#define _C0_CAC_DIGITAL_FILTER_DIV_16       (0xC0U) /* Digital filter sampling is at frequency measuring clock / 16 */

/*
    CAC Interrupt Control Register (CAICR)
*/
/* Frequency Error Interrupt Enable (FERRIE) */
#define _00_CAC_FERRIE_INTERRUPT_DISABLE    (0x00U) /* Frequency error interrupt is disabled */
#define _01_CAC_FERRIE_INTERRUPT_ENABLE     (0x01U) /* Frequency error interrupt is enabled */
/* Measurement End Interrupt Enable (MENDIE) */
#define _00_CAC_MENDIE_INTERRUPT_DISABLE    (0x00U) /* Measurement end interrupt is disabled */
#define _02_CAC_MENDIE_INTERRUPT_ENABLE     (0x02U) /* Measurement end interrupt is enabled */
/* Overflow Interrupt Enable (OVFIE) */
#define _00_CAC_OVFIE_INTERRUPT_DISABLE     (0x00U) /* Overflow interrupt is disabled */
#define _04_CAC_OVFIE_INTERRUPT_ENABLE      (0x04U) /* Overflow interrupt is enabled */

/*
    Interrupt Source Priority Register n (IPRn)
*/
/* Interrupt Priority Level Select (IPR[3:0]) */
#define _00_CAC_PRIORITY_LEVEL0             (0x00U) /* Level 0 (interrupt disabled) */
#define _01_CAC_PRIORITY_LEVEL1             (0x01U) /* Level 1 */
#define _02_CAC_PRIORITY_LEVEL2             (0x02U) /* Level 2 */
#define _03_CAC_PRIORITY_LEVEL3             (0x03U) /* Level 3 */
#define _04_CAC_PRIORITY_LEVEL4             (0x04U) /* Level 4 */
#define _05_CAC_PRIORITY_LEVEL5             (0x05U) /* Level 5 */
#define _06_CAC_PRIORITY_LEVEL6             (0x06U) /* Level 6 */
#define _07_CAC_PRIORITY_LEVEL7             (0x07U) /* Level 7 */
#define _08_CAC_PRIORITY_LEVEL8             (0x08U) /* Level 8 */
#define _09_CAC_PRIORITY_LEVEL9             (0x09U) /* Level 9 */
#define _0A_CAC_PRIORITY_LEVEL10            (0x0AU) /* Level 10 */
#define _0B_CAC_PRIORITY_LEVEL11            (0x0BU) /* Level 11 */
#define _0C_CAC_PRIORITY_LEVEL12            (0x0CU) /* Level 12 */
#define _0D_CAC_PRIORITY_LEVEL13            (0x0DU) /* Level 13 */
#define _0E_CAC_PRIORITY_LEVEL14            (0x0EU) /* Level 14 */
#define _0F_CAC_PRIORITY_LEVEL15            (0x0FU) /* Level 15 (highest) */


/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define _0204_CAC_UPPER_LIMIT               (0x0204U)
#define _01D2_CAC_LOWER_LIMIT               (0x01D2U)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void R_CAC_Create(void);
void R_CAC_Start(void);
void R_CAC_Stop(void);

/* Start user code for function. Do not edit comment generated here */
uint32_t Frequency_Value(void);
uint32_t Upper_Frequency_Limit(void);
uint32_t Lower_Frequency_Limit(void);
/* End user code. Do not edit comment generated here */
#endif