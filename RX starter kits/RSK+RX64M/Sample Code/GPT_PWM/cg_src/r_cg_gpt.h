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
* Copyright (C) 2013, 2014 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_gpt.h
* Version      : Applilet4 for RX64M V1.00.00.03 [28 Feb 2014]
* Device(s)    : R5F564MLCxFC
* Tool-Chain   : IAR Systems ICCRX
* Description  : This file implements device driver for GPT module.
* Creation Date: 28.03.2014
***********************************************************************************************************************/
#ifndef GPT_H
#define GPT_H

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/
/*
    Noise Filter Control Register (NFCR)
*/
/* Noise Filter 0A Enable (NFA0EN) */
#define _0001_GPT_GTIOC0A_NOISE_FILTER_EN           (0x0001U) /* Noise filter enable */
/* Noise Filter 0B Enable (NFB0EN) */
#define _0002_GPT_GTIOC0B_NOISE_FILTER_EN           (0x0002U) /* Noise filter enable */
/* Noise Filter 1A Enable (NFA1EN) */
#define _0004_GPT_GTIOC1A_NOISE_FILTER_EN           (0x0004U) /* Noise filter enable */
/* Noise Filter 1B Enable (NFB1EN) */
#define _0008_GPT_GTIOC1B_NOISE_FILTER_EN           (0x0008U) /* Noise filter enable */
/* Noise Filter 2A Enable (NFA2EN) */
#define _0010_GPT_GTIOC2A_NOISE_FILTER_EN           (0x0010U) /* Noise filter enable */
/* Noise Filter 2B Enable (NFB2EN) */
#define _0020_GPT_GTIOC2B_NOISE_FILTER_EN           (0x0020U) /* Noise filter enable */
/* Noise Filter 3A Enable (NFA3EN) */
#define _0040_GPT_GTIOC3A_NOISE_FILTER_EN           (0x0040U) /* Noise filter enable */
/* Noise Filter 3B Enable (NFB3EN) */
#define _0080_GPT_GTIOC3B_NOISE_FILTER_EN           (0x0080U) /* Noise filter enable */
/* GPT0 Noise Filter Sampling Clock Select (NFCS0[1:0]) */
#define _0000_GPT0_NOISE_FILTER_PCLKA_1             (0x0000U) /* Noise filter clock PCLKA/1 */
#define _0100_GPT0_NOISE_FILTER_PCLKA_4             (0x0100U) /* Noise filter clock PCLKA/4 */
#define _0200_GPT0_NOISE_FILTER_PCLKA_32            (0x0200U) /* Noise filter clock PCLKA/32 */
#define _0300_GPT0_NOISE_FILTER_CNT_SRC             (0x0300U) /* Noise filter use count clock source */
/* GPT1 Noise Filter Sampling Clock Select (NFCS1[1:0]) */
#define _0000_GPT1_NOISE_FILTER_PCLKA_1             (0x0000U) /* Noise filter clock PCLKA/1 */
#define _0400_GPT1_NOISE_FILTER_PCLKA_4             (0x0400U) /* Noise filter clock PCLKA/4 */
#define _0800_GPT1_NOISE_FILTER_PCLKA_32            (0x0800U) /* Noise filter clock PCLKA/32 */
#define _0C00_GPT1_NOISE_FILTER_CNT_SRC             (0x0C00U) /* Noise filter use count clock source */
/* GPT2 Noise Filter Sampling Clock Select (NFCS2[1:0]) */
#define _0000_GPT2_NOISE_FILTER_PCLKA_1             (0x0000U) /* Noise filter clock PCLKA/1 */
#define _1000_GPT2_NOISE_FILTER_PCLKA_4             (0x1000U) /* Noise filter clock PCLKA/4 */
#define _2000_GPT2_NOISE_FILTER_PCLKA_32            (0x2000U) /* Noise filter clock PCLKA/32 */
#define _3000_GPT2_NOISE_FILTER_CNT_SRC             (0x3000U) /* Noise filter use count clock source */
/* GPT3 Noise Filter Sampling Clock Select (NFCS3[1:0]) */
#define _0000_GPT3_NOISE_FILTER_PCLKA_1             (0x0000U) /* Noise filter clock PCLKA/1 */
#define _4000_GPT3_NOISE_FILTER_PCLKA_4             (0x4000U) /* Noise filter clock PCLKA/4 */
#define _8000_GPT3_NOISE_FILTER_PCLKA_32            (0x8000U) /* Noise filter clock PCLKA/32 */
#define _C000_GPT3_NOISE_FILTER_CNT_SRC             (0xC000U) /* Noise filter use count clock source */

/*
    General PWM Timer Hardware Source Start Control Register (GTHSCR)
*/
/* GPTn.GTCNT Hardware Source Count Start (CSHWn[1:0]) */
#define _00_GPT_HW_START_DISABLE                    (0x00U) /* Counting is not started by hardware */
#define _01_GPT_HW_START_RISING                     (0x01U) /* Counting is started at rising edge */
#define _02_GPT_HW_START_FALLING                    (0x02U) /* Counting is started at falling edge */
#define _03_GPT_HW_START_BOTH                       (0x03U) /* Counting is started at both edges */
/* GPTn.GTCTN Hardware Source Count Stop (CPHWn[1:0]) */
#define _00_GPT_HW_STOP_DISABLE                     (0x00U) /* Counting is not started by hardware */
#define _01_GPT_HW_STOP_RISING                      (0x01U) /* Counting is started at rising edge */
#define _02_GPT_HW_STOP_FALLING                     (0x02U) /* Counting is started at falling edge */
#define _03_GPT_HW_STOP_BOTH                        (0x03U) /* Counting is started at both edges */

/*
    General PWM Timer Hardware Source Clear Control Register (GTHCCR)
*/
/* GPTn.GTCNT Hardware Source Counter Clear (CCHWn[1:0]) */
#define _00_GPT_HW_CLEAR_DISABLE                    (0x00U) /* Counter is not cleared by hardware */
#define _01_GPT_HW_CLEAR_RISING                     (0x01U) /* Counter is cleared at rising edge */
#define _02_GPT_HW_CLEAR_FALLING                    (0x02U) /* Counter is cleared at falling edge */
#define _03_GPT_HW_CLEAR_BOTH                       (0x03U) /* Counter is cleared at both edges */

/*
    General PWM Timer Hardware Start Source Select Register (GTHSSR)
*/
/* GPTn.GTCNT Hardware Counter Start Source Select (CSHSLn[3:0]) */
#define _00_GPT_HW_START_SRC_NONE                   (0x00U) /* No hardware start source */
#define _08_GPT_HW_START_SRC_GTIOC3A_PIN            (0x08U) /* GTIOC3A pin input */
#define _09_GPT_HW_START_SRC_GTIOC3B_PIN            (0x09U) /* GTIOC3B pin input */
#define _0A_GPT_HW_START_SRC_GTIOC3A_COMP           (0x0AU) /* GTIOC3A internal output (output compare) */
#define _0B_GPT_HW_START_SRC_GTIOC3B_COMP           (0x0BU) /* GTIOC3B internal output (output compare) */
#define _0C_GPT_HW_START_SRC_GTETRG_PIN             (0x0CU) /* GTETRG pin input */

/*
    General PWM Timer Hardware Stop/Clear Source Select Register (GTHPSR)
*/
/* GPTn.GTCNT Hardware Counter Stop/Clear Source Select (CSHPLn[3:0]) */
#define _00_GPT_HW_STPCLR_SRC_NONE                  (0x00U) /* No hardware stop/clear source */
#define _08_GPT_HW_STPCLR_SRC_GTIOC3A_PIN           (0x08U) /* GTIOC3A pin input */
#define _09_GPT_HW_STPCLR_SRC_GTIOC3B_PIN           (0x09U) /* GTIOC3B pin input */
#define _0A_GPT_HW_STPCLR_SRC_GTIOC3A_COMP          (0x0AU) /* GTIOC3A internal output (output compare) */
#define _0B_GPT_HW_STPCLR_SRC_GTIOC3B_COMP          (0x0BU) /* GTIOC3B internal output (output compare) */
#define _0C_GPT_HW_STPCLR_SRC_GTETRG_PIN            (0x0CU) /* GTETRG pin input */

/*
    General PWM Timer Sync Register (GTSYNC)
*/
/* GTPn.GTCNT Counter Synchronized-Clear Source Select (SYNCn[1:0]) */
#define _00_GPT_SYNC_CLEAR_SRC_GPT0                 (0x00U) /* Cleared by GPT0 clearing source */
#define _01_GPT_SYNC_CLEAR_SRC_GPT1                 (0x01U) /* Cleared by GPT1 clearing source */
#define _02_GPT_SYNC_CLEAR_SRC_GPT2                 (0x02U) /* Cleared by GPT2 clearing source */
#define _03_GPT_SYNC_CLEAR_SRC_GPT3                 (0x03U) /* Cleared by GPT3 clearing source */

/*
    General PWM Timer External Trigger Input Interrupt Register (GTETINT)
*/
/* External Trigger Rising Input Interrupt Request Enable (ETIPEN) */
#define _0000_GPT_GTGIP_DISABLE                     (0x0000U) /* GTGIP interrupt is disabled */
#define _0001_GPT_GTGIP_ENABLE                      (0x0001U) /* GTGIP interrupt is enabled */
/* External Trigger Falling Input Interrupt Request Enable (ETINEN) */
#define _0000_GPT_GTGIN_DISABLE                     (0x0000U) /* GTGIN interrupt is disabled */
#define _0002_GPT_GTGIN_ENABLE                      (0x0002U) /* GTGIN interrupt is enabled */
/* GTETRG Noise Filter Sampling Clock Select (GTENFCS[1:0]) */
#define _0000_GPT_GTETRG_NOISE_FILTER_PCLKA1        (0x0000U) /* PCLKA/1 */
#define _2000_GPT_GTETRG_NOISE_FILTER_PCLKA2        (0x2000U) /* PCLKA/2 */
#define _4000_GPT_GTETRG_NOISE_FILTER_PCLKA4        (0x4000U) /* PCLKA/4 */
#define _6000_GPT_GTETRG_NOISE_FILTER_PCLKA32       (0x6000U) /* PCLKA/32 */
/* GTETRG Noise Filter Enable (GTETRGEN) */
#define _0000_GPT_GTETRG_NOISE_FILTER_DISABLE       (0x0000U) /* GTETRG noise filter disable */
#define _8000_GPT_GTETRG_NOISE_FILTER_ENABLE        (0x8000U) /* GTETRG noise filter enable */
/* Default write value for reserved bits */
#define _0300_GPT_GTETINT_DEFAULT                   (0x0300U) /* Default write value */

/*
    General PWM Timer Buffer Operation Disable Register (GTBDR)
*/
/* GPTn.GTCCR Buffer Operation Disable (BDn[0:0]) */
#define _00_GPT_GTCCR_BUF_OP_ENABLE                 (0x00U) /* Buffer operation is enabled */
#define _01_GPT_GTCCR_BUF_OP_DISABLE                (0x01U) /* Buffer operation is disabled */
/* GPTn.GTPR Buffer Operation Disable (BDn[1:1]) */
#define _00_GPT_GTPR_BUF_OP_ENABLE                  (0x00U) /* Buffer operation is enabled */
#define _02_GPT_GTPR_BUF_OP_DISABLE                 (0x02U) /* Buffer operation is disabled */
/* GPTn.GTADTR Buffer Operation Disable (BDn[2:2]) */
#define _00_GPT_GTADTR_BUF_OP_ENABLE                (0x00U) /* Buffer operation is enabled */
#define _04_GPT_GTADTR_BUF_OP_DISABLE               (0x04U) /* Buffer operation is disabled */
/* GPTn.GTDV Buffer Operation Disable (BDn[3:3]) */
#define _00_GPT_GTDV_BUF_OP_ENABLE                  (0x00U) /* Buffer operation is enabled */
#define _08_GPT_GTDV_BUF_OP_DISABLE                 (0x08U) /* Buffer operation is disabled */

/*
    General PWM Timer I/O Control Register (GTIOR)
*/
/* GTIOCnA Pin Function Select (GTIOA[1:0]) */
#define _0000_GPTn_GTIOCA_COMP_MATCH_RETAIN         (0x0000U) /* Output retained at compare match */
#define _0001_GPTn_GTIOCA_COMP_MATCH_LOW            (0x0001U) /* Output low at compare match */
#define _0002_GPTn_GTIOCA_COMP_MATCH_HIGH           (0x0002U) /* Output high at compare match */
#define _0003_GPTn_GTIOCA_COMP_MATCH_TOGGLE         (0x0003U) /* Output toggled at compare match */
#define _0000_GPTn_GTIOCA_INPUT_CAPTURE_RISING      (0x0000U) /* Input capture at rising edge */
#define _0001_GPTn_GTIOCA_INPUT_CAPTURE_FALLING     (0x0001U) /* Input capture at falling edge */
#define _0002_GPTn_GTIOCA_INPUT_CAPTURE_BOTH        (0x0002U) /* Input capture at both edges */
/* GTIOCnA Pin Function Select (GTIOA[3:2]) */
#define _0000_GPTn_GTIOCA_CYCLE_END_RETAIN          (0x0000U) /* Output retained at cycle end */
#define _0004_GPTn_GTIOCA_CYCLE_END_LOW             (0x0004U) /* Output low at cycle end */
#define _0008_GPTn_GTIOCA_CYCLE_END_HIGH            (0x0008U) /* Output hight at cycle end */
#define _000C_GPTn_GTIOCA_CYCLE_END_TOGGLE          (0x000CU) /* Output toggled at cycle end */
/* GTIOCnA Pin Function Select (GTIOA[4:4]) */
#define _0000_GPTn_GTIOCA_INIT_OUT_LOW              (0x0000U) /* Initial output is low */
#define _0010_GPTn_GTIOCA_INIT_OUT_HIGH             (0x0010U) /* Initial output is high */
/* GTIOCnA Pin Function Select (GTIOA[5:5]) */
#define _0000_GPTn_GTCCRA_COMP_MATCH                (0x0000U) /* GTCCRA function is compare match */
#define _0020_GPTn_GTCCRA_INPUT_CAPTURE             (0x0020U) /* GTCCRA function is input capture */
/* Output Value at GTIOCnA Pin Count Stop (OADFLT) */
#define _0000_GPTn_GTIOCA_OUTPUT_ON_STOP_0          (0x0000U) /* Outputs low when counting is stopped */
#define _0040_GPTn_GTIOCA_OUTPUT_ON_STOP_1          (0x0040U) /* Outputs high when counting is stopped */
/* Output Retain at GTIOCnA Pin Count Start/Stop (OAHLD) */
#define _0000_GPTn_GTIOCA_OUTPUT_ON_START_SET       (0x0000U) /* Output at start/stop based on setting */
#define _0080_GPTn_GTIOCA_OUTPUT_ON_START_RETAIN    (0x0080U) /* Output at start/stop is retained */
/* GTIOCnB Pin Function Select (GTIOB[1:0]) */
#define _0000_GPTn_GTIOCB_COMP_MATCH_RETAIN         (0x0000U) /* Output retained at compare match */
#define _0100_GPTn_GTIOCB_COMP_MATCH_LOW            (0x0100U) /* Output low at compare match */
#define _0200_GPTn_GTIOCB_COMP_MATCH_HIGH           (0x0200U) /* Output high at compare match */
#define _0300_GPTn_GTIOCB_COMP_MATCH_TOGGLE         (0x0300U) /* Output toggled at compare match */
#define _0000_GPTn_GTIOCB_INPUT_CAPTURE_RISING      (0x0000U) /* Input capture at rising edge */
#define _0100_GPTn_GTIOCB_INPUT_CAPTURE_FALLING     (0x0100U) /* Input capture at falling edge */
#define _0200_GPTn_GTIOCB_INPUT_CAPTURE_BOTH        (0x0200U) /* Input capture at both edges */
/* GTIOCnB Pin Function Select (GTIOB[3:2]) */
#define _0000_GPTn_GTIOCB_CYCLE_END_RETAIN          (0x0000U) /* Output retained at cycle end */
#define _0400_GPTn_GTIOCB_CYCLE_END_LOW             (0x0400U) /* Output low at cycle end */
#define _0800_GPTn_GTIOCB_CYCLE_END_HIGH            (0x0800U) /* Output hight at cycle end */
#define _0C00_GPTn_GTIOCB_CYCLE_END_TOGGLE          (0x0C00U) /* Output toggled at cycle end */
/* GTIOCnB Pin Function Select (GTIOB[4:4]) */
#define _0000_GPTn_GTIOCB_INIT_OUT_LOW              (0x0000U) /* Initial output is low */
#define _1000_GPTn_GTIOCB_INIT_OUT_HIGH             (0x1000U) /* Initial output is high */
/* GTIOCnB Pin Function Select (GTIOB[5:5]) */
#define _0000_GPTn_GTCCRB_COMP_MATCH                (0x0000U) /* GTCCRB function is compare match */
#define _2000_GPTn_GTCCRB_INPUT_CAPTURE             (0x2000U) /* GTCCRB function is input capture */
/* Output Value at GTIOCnB Pin Count Stop (OBDFLT) */    
#define _0000_GPTn_GTIOCB_OUTPUT_ON_STOP_0          (0x0000U) /* Outputs low when counting is stopped */
#define _4000_GPTn_GTIOCB_OUTPUT_ON_STOP_1          (0x4000U) /* Outputs high when counting is stopped */
/* Output Retain at GTIOCnB Pin Count Start/Stop (OBHLD) */
#define _0000_GPTn_GTIOCB_OUTPUT_ON_START_SET       (0x0000U) /* Output at start/stop based on setting */
#define _8000_GPTn_GTIOCB_OUTPUT_ON_START_RETAIN    (0x8000U) /* Output at start/stop is retained */

/*
    General PWM Timer Interrupt Output Setting Register (GTINTAD)
*/
/* GTCCRA Compare Match/Input Capture Interrupt Enable (GTINTA) */
#define _0000_GPTn_GTCIA_INTERRUPT_DISABLE          (0x0000U) /* Interrupt request is disabled */
#define _0001_GPTn_GTCIA_INTERRUPT_ENABLE           (0x0001U) /* Interrupt request is enabled */
/* GTCCRB Compare Match/Input Capture Interrupt Enable (GTINTB) */
#define _0000_GPTn_GTCIB_INTERRUPT_DISABLE          (0x0000U) /* Interrupt request is disabled */
#define _0002_GPTn_GTCIB_INTERRUPT_ENABLE           (0x0002U) /* Interrupt request is enabled */
/* GTCCRC Compare Match/Input Capture Interrupt Enable (GTINTC) */
#define _0000_GPTn_GTCIC_INTERRUPT_DISABLE          (0x0000U) /* Interrupt request is disabled */
#define _0004_GPTn_GTCIC_INTERRUPT_ENABLE           (0x0004U) /* Interrupt request is enabled */
/* GTCCRD Compare Match/Input Capture Interrupt Enable (GTINTD) */
#define _0000_GPTn_GTCID_INTERRUPT_DISABLE          (0x0000U) /* Interrupt request is disabled */
#define _0008_GPTn_GTCID_INTERRUPT_ENABLE           (0x0008U) /* Interrupt request is enabled */
/* GTCCRE Compare Match/Input Capture Interrupt Enable (GTINTE) */
#define _0000_GPTn_GTCIE_INTERRUPT_DISABLE          (0x0000U) /* Interrupt request is disabled */
#define _0010_GPTn_GTCIE_INTERRUPT_ENABLE           (0x0010U) /* Interrupt request is enabled */
/* GTCCRF Compare Match/Input Capture Interrupt Enable (GTINTF) */
#define _0000_GPTn_GTCIF_INTERRUPT_DISABLE          (0x0000U) /* Interrupt request is disabled */
#define _0020_GPTn_GTCIF_INTERRUPT_ENABLE           (0x0020U) /* Interrupt request is enabled */
/* GTPR Compare Match Interrupt Enable (GTINTPR[0:0]) */
#define _0000_GPTn_GTCIV_INTERRUPT_DISABLE          (0x0000U) /* Interrupt request is disabled */
#define _0040_GPTn_GTCIV_INTERRUPT_ENABLE           (0x0040U) /* Interrupt request is enabled */
/* GTPR Compare Match Interrupt Enable (GTINTPR[1:1]) */
#define _0000_GPTn_GTCIU_INTERRUPT_DISABLE          (0x0000U) /* Interrupt request is disabled */
#define _0080_GPTn_GTCIU_INTERRUPT_ENABLE           (0x0080U) /* Interrupt request is enabled */
/* Dead Time Error Interrupt Enable (EINT) */
#define _0000_GPTn_GTDE_INTERRUPT_DISABLE           (0x0000U) /* Interrupt request is disabled */
#define _0800_GPTn_GTDE_INTERRUPT_ENABLE            (0x0800U) /* Interrupt request is enabled */
/* GTADTRA Compare Match (Up-Counting) A/D Converter Start Request Enable (ADTRAUEN) */
#define _0000_GPTn_ADTRA_UPCOUNTING_DISABLE         (0x0000U) /* A/D start request is disabled */
#define _1000_GPTn_ADTRA_UPCOUNTING_ENABLE          (0x1000U) /* A/D start request is enabled */
/* GTADTRA Compare Match (Down-Counting) A/D Converter Start Request Enable (ADTRADEN) */
#define _0000_GPTn_ADTRA_DOWNCOUNTING_DISABLE       (0x0000U) /* A/D start request is disabled */
#define _2000_GPTn_ADTRA_DOWNCOUNTING_ENABLE        (0x2000U) /* A/D start request is enabled */
/* GTADTRB Compare Match (Up-Counting) A/D Converter Start Request Enable (ADTRBUEN) */
#define _0000_GPTn_ADTRB_UPCOUNTING_DISABLE         (0x0000U) /* A/D start request is disabled */
#define _4000_GPTn_ADTRB_UPCOUNTING_ENABLE          (0x4000U) /* A/D start request is enabled */
/* GTADTRB Compare Match (Down-Counting) A/D Converter Start Request Enable (ADTRBDEN) */
#define _0000_GPTn_ADTRB_DOWNCOUNTING_DISABLE       (0x0000U) /* A/D start request is disabled */
#define _8000_GPTn_ADTRB_DOWNCOUNTING_ENABLE        (0x8000U) /* A/D start request is enabled */

/*
    General PWM Timer Control Register (GTCR)
*/
/* Mode Select (MD[2:0]) */
#define _0000_GPTn_MODE_SAW_WAVE_PWM                (0x0000U) /* Saw-wave PWM mode */
#define _0001_GPTn_MODE_SAW_WAVE_ONESHOT            (0x0001U) /* Saw-wave one-shot mode */
#define _0004_GPTn_MODE_TRIANGLE_WAVE_PWM_1         (0x0004U) /* Triangle-wave PWM mode 1 */
#define _0005_GPTn_MODE_TRIANGLE_WAVE_PWM_2         (0x0005U) /* Triangle-wave PWM mode 2 */
#define _0006_GPTn_MODE_TRIANGLE_WAVE_PWM_3         (0x0006U) /* Triangle-wave PWM mode 3 */
/* Timer Prescaler Select (TPCS[1:0]) */
#define _00_GPTn_COUNT_CLK_PCLKA_1                  (0x00U) /* PCLKA */
#define _01_GPTn_COUNT_CLK_PCLKA_2                  (0x01U) /* PCLKA/2 */
#define _02_GPTn_COUNT_CLK_PCLKA_4                  (0x02U) /* PCLKA/4 */
#define _03_GPTn_COUNT_CLK_PCLKA_8                  (0x03U) /* PCLKA/8 */
/* Counter Clear Source Select (CCLR[1:0]) */
#define _0000_GPTn_COUNTER_CLEAR_NONE               (0x0000U) /* No counter clear source */
#define _1000_GPTn_COUNTER_CLEAR_GTCCRA             (0x1000U) /* Cleared by GTCCRA input capture */
#define _2000_GPTn_COUNTER_CLEAR_GTCCRB             (0x2000U) /* Cleared by GTCCRB input capture */
#define _3000_GPTn_COUNTER_CLEAR_SYNC               (0x3000U) /* Synchronized clearing */

/*
    General PWM Timer Buffer Enable Register (GTBER)
*/
/* GTCCRA Buffer Operation (CCRA[1:0]) */
#define _0000_GPTn_GTCCRA_BUF_NONE                  (0x0000U) /* Buffer operation is not performed */
#define _0001_GPTn_GTCCRA_BUF_SINGLE                (0x0001U) /* Single buffer operation */
#define _0002_GPTn_GTCCRA_BUF_DOUBLE                (0x0002U) /* Double buffer operation */
/* GTCCRB Buffer Operation (CCRB[1:0]) */
#define _0000_GPTn_GTCCRB_BUF_NONE                  (0x0000U) /* Buffer operation is not performed */
#define _0004_GPTn_GTCCRB_BUF_SINGLE                (0x0004U) /* Single buffer operation */
#define _0008_GPTn_GTCCRB_BUF_DOUBLE                (0x0008U) /* Double buffer operation */
/* GTPR Buffer Operation (PR[1:0]) */
#define _0000_GPTn_GTPR_BUF_NONE                    (0x0000U) /* Buffer operation is not performed */
#define _0010_GPTn_GTPR_BUF_SINGLE                  (0x0010U) /* Single buffer operation */
#define _0020_GPTn_GTPR_BUF_DOUBLE                  (0x0020U) /* Double buffer operation */
/* GTCCRA and GTCCRB Forcible Buffer Operation (CCRSWT) */
#define _0000_GPTn_GTCCR_BUF_TRANS_NORMAL           (0x0000U) /* Not forcibly perform buffer transfer */
#define _0040_GPTn_GTCCR_BUF_TRANS_FORCE            (0x0040U) /* Forcibly perform buffer transfer */
/* GTADTRA Buffer Transfer Timing Select ADTTA[1:0] */
#define _0000_GPTn_GTADTRA_BUF_TRANS_NONE           (0x0000U) /* No transfer */
#define _0100_GPTn_GTADTRA_BUF_TRANS_OVRF_UDRF      (0x0100U) /* Transfer at overflow & underflow */
#define _0100_GPTn_GTADTRA_BUF_TRANS_CREST          (0x0100U) /* Transfer at crest */
#define _0200_GPTn_GTADTRA_BUF_TRANS_TROUGH         (0x0200U) /* Transfer at trough */
#define _0300_GPTn_GTADTRA_BUF_TRANS_BOTH           (0x0300U) /* Transfer at both crest & trough */
/* GTADTRA Double Buffer Operation (ADTDA) */
#define _0000_GPTn_GTADTRA_BUF_SINGLE               (0x0000U) /* Single buffer operation */
#define _0400_GPTn_GTADTRA_BUF_DOUBLE               (0x0400U) /* Double buffer operation */
/* GTADTRB Buffer Transfer Timing Select ADTTB[1:0] */
#define _0000_GPTn_GTADTRB_BUF_TRANS_NONE           (0x0000U) /* No transfer */
#define _1000_GPTn_GTADTRB_BUF_TRANS_OVRF_UDRF      (0x1000U) /* Transfer at overflow & underflow */
#define _1000_GPTn_GTADTRB_BUF_TRANS_CREST          (0x1000U) /* Transfer at crest */
#define _2000_GPTn_GTADTRB_BUF_TRANS_TROUGH         (0x2000U) /* Transfer at trough */
#define _3000_GPTn_GTADTRB_BUF_TRANS_BOTH           (0x3000U) /* Transfer at both crest & trough */
/* GTADTRB Double Buffer Operation (ADTDB) */
#define _0000_GPTn_GTADTRB_BUF_SINGLE               (0x0000U) /* Single buffer operation */
#define _4000_GPTn_GTADTRB_BUF_DOUBLE               (0x4000U) /* Double buffer operation */

/*
    General PWM Timer Count Direction Register (GTUDC)
*/
/* Count Direction Setting (UD) */
#define _0000_GPTn_COUNT_DOWN                       (0x0000U) /* GTCNT counts down */
#define _0001_GPTn_COUNT_UP                         (0x0001U) /* GTCNT counts up */
/* Forcible Count Direction Setting (UDF) */
#define _0000_GPTn_COUNT_DIR_NORMAL                 (0x0000U) /* Not forcibly set */
#define _0002_GPTn_COUNT_DIR_FORCE                  (0x0002U) /* Forcibly set */

/*
    General PWM Timer Interrupt and A/D Converter Start Request Skipping Setting Register (GTITC)
*/
/* GTCCRA Compare Match/Input Capture Interrupt Link (ITLA) */
#define _0000_GPTn_GTCIA_INTERRUPT_SKIP_DISABLE     (0x0000U) /* No skipping */
#define _0001_GPTn_GTCIA_INTERRUPT_SKIP_ENABLE      (0x0001U) /* Linked with GTCIV/GTCIU skipping */
/* GTCCRB Compare Match/Input Capture Interrupt Link (ITLB) */
#define _0000_GPTn_GTCIB_INTERRUPT_SKIP_DISABLE     (0x0000U) /* No skipping */
#define _0002_GPTn_GTCIB_INTERRUPT_SKIP_ENABLE      (0x0002U) /* Linked with GTCIV/GTCIU skipping */
/* GTCCRC Compare Match/Input Capture Interrupt Link (ITLC) */
#define _0000_GPTn_GTCIC_INTERRUPT_SKIP_DISABLE     (0x0000U) /* No skipping */
#define _0004_GPTn_GTCIC_INTERRUPT_SKIP_ENABLE      (0x0004U) /* Linked with GTCIV/GTCIU skipping */
/* GTCCRD Compare Match/Input Capture Interrupt Link (ITLD) */
#define _0000_GPTn_GTCID_INTERRUPT_SKIP_DISABLE     (0x0000U) /* No skipping */
#define _0008_GPTn_GTCID_INTERRUPT_SKIP_ENABLE      (0x0008U) /* Linked with GTCIV/GTCIU skipping */
/* GTCCRE Compare Match/Input Capture Interrupt Link (ITLE) */
#define _0000_GPTn_GTCIE_INTERRUPT_SKIP_DISABLE     (0x0000U) /* No skipping */
#define _0010_GPTn_GTCIE_INTERRUPT_SKIP_ENABLE      (0x0010U) /* Linked with GTCIV/GTCIU skipping */
/* GTCCRF Compare Match/Input Capture Interrupt Link (ITLF) */
#define _0000_GPTn_GTCIF_INTERRUPT_SKIP_DISABLE     (0x0000U) /* No skipping */
#define _0020_GPTn_GTCIF_INTERRUPT_SKIP_ENABLE      (0x0020U) /* Linked with GTCIV/GTCIU skipping */
/* GTCIV/GTCIU Interrupt Skipping Function Select (IVTC[1:0]) */
#define _0000_GPTn_GTCI_SKIPPING_DISABLE            (0x0000U) /* Skipping is not performed */
#define _0040_GPTn_GTCI_SKIPPING_OVRF_UDRF          (0x0040U) /* Skipping at overflow & underflow */
#define _0040_GPTn_GTCI_SKIPPING_CREST              (0x0040U) /* Skipping at crest */
#define _0080_GPTn_GTCI_SKIPPING_TROUGH             (0x0080U) /* Skipping at trough */
#define _00C0_GPTn_GTCI_SKIPPING_BOTH               (0x00C0U) /* Skipping at both crest & trough */
/* GTCIV/GTCIU Interrupt Skipping Count Select (IVTT[2:0]) */
#define _0000_GPTn_GTCI_SKIPPING_COUNT_0            (0x0000U) /* Skipping is not performed */
#define _0100_GPTn_GTCI_SKIPPING_COUNT_1            (0x0100U) /* Skipping count of 1 */
#define _0200_GPTn_GTCI_SKIPPING_COUNT_2            (0x0200U) /* Skipping count of 2 */
#define _0300_GPTn_GTCI_SKIPPING_COUNT_3            (0x0300U) /* Skipping count of 3 */
#define _0400_GPTn_GTCI_SKIPPING_COUNT_4            (0x0400U) /* Skipping count of 4 */
#define _0500_GPTn_GTCI_SKIPPING_COUNT_5            (0x0500U) /* Skipping count of 5 */
#define _0600_GPTn_GTCI_SKIPPING_COUNT_6            (0x0600U) /* Skipping count of 6 */
#define _0700_GPTn_GTCI_SKIPPING_COUNT_7            (0x0700U) /* Skipping count of 7 */
/* GTADTRA A/D Converter Start Request Link (ADTAL) */
#define _0000_GPTn_ADTRA_REQ_SKIP_DISABLE           (0x0000U) /* No skipping */
#define _1000_GPTn_ADTRA_REQ_SKIP_ENABLE            (0x1000U) /* Linked with GTCIV/GTCIU skipping */
/* GTADTRB A/D Converter Start Request Link (ADTBL) */
#define _0000_GPTn_ADTRB_REQ_SKIP_DISABLE           (0x0000U) /* No skipping */
#define _4000_GPTn_ADTRB_REQ_SKIP_ENABLE            (0x4000U) /* Linked with GTCIV/GTCIU skipping */

/*
    General PWM Timer Output Negate Control Register (GTONCR)
*/
/* GTIOCnA Pin Negate Control Enable (NEA) */
#define _0000_GPTn_GTCIOA_NEGATE_DISABLE            (0x0000U) /* Negate is disabled */
#define _0001_GPTn_GTCIOA_NEGATE_ENABLE             (0x0001U) /* Negate is enabled */
/* GTIOCnB Pin Negate Control Enable (NEB) */
#define _0000_GPTn_GTCIOB_NEGATE_DISABLE            (0x0000U) /* Negate is disabled */
#define _0002_GPTn_GTCIOB_NEGATE_ENABLE             (0x0002U) /* Negate is enabled */
/* GTIOCnA Pin Negate Value Setting (NVA) */
#define _0000_GPTn_GTCIOA_NEGATE_OUT_0              (0x0000U) /* Negate value is 0 */
#define _0004_GPTn_GTCIOA_NEGATE_OUT_1              (0x0004U) /* Negate value is 1 */
/* GTIOCnB Pin Negate Value Setting (NVB) */
#define _0000_GPTn_GTCIOB_NEGATE_OUT_0              (0x0000U) /* Negate value is 0 */
#define _0008_GPTn_GTCIOB_NEGATE_OUT_1              (0x0008U) /* Negate value is 1 */
/* GTIOC Output Negate Source Select (NFS[3:0]) */
#define _0000_GPTn_GTCIO_NEGATE_SRC_NONE            (0x0000U) /* No negate source */
#define _0070_GPTn_GTCIO_NEGATE_SRC_GTETRG          (0x0070U) /* GTETRG pin input */
#define _0080_GPTn_GTCIO_NEGATE_SRC_SW              (0x0080U) /* Software control with SWN bit */
/* Negate Source Polarity Select (NFV) */
#define _0000_GPTn_NEGATE_POLARITY_0                (0x0000U) /* Negate when source is 0 */
#define _0100_GPTn_NEGATE_POLARITY_1                (0x0100U) /* Negate when source is 1 */
/* GTIOCnA Pin Output Enable (OAE) */
#define _0000_GPTn_GTCIOCA_OUTPUT_DISABLE           (0x0000U) /* No pin output */
#define _4000_GPTn_GTCIOCA_OUTPUT_ENABLE            (0x4000U) /* Pin output */
/* GTIOCnB Pin Output Enable (OBE) */
#define _0000_GPTn_GTCIOCB_OUTPUT_DISABLE           (0x0000U) /* No pin output */
#define _8000_GPTn_GTCIOCB_OUTPUT_ENABLE            (0x8000U) /* Pin output */

/*
    General PWM Timer Dead Time Control Register (GTDTCR)
*/
/* Negative-Phase Waveform Setting (TDE) */
#define _0000_GPTn_DEAD_TIME_DISABLE                (0x0000U) /* GTCCRB is set without dead time */
#define _0001_GPTn_DEAD_TIME_ENABLE                 (0x0001U) /* GTCCRB is auto-set with dead time */
/* GTDVU Buffer Operation Enable (TDBUE) */
#define _0000_GPTn_GTDVU_BUF_DISABLE                (0x0000U) /* GTDVU buffer operation is disabled */
#define _0010_GPTn_GTDVU_BUF_ENABLE                 (0x0010U) /* GTDVU buffer operation is enabled */
/* GTDVD Buffer Operation Enable (TDBDE) */
#define _0000_GPTn_GTDVD_BUF_DISABLE                (0x0000U) /* GTDVD buffer operation is disabled */
#define _0020_GPTn_GTDVD_BUF_ENABLE                 (0x0020U) /* GTDVD buffer operation is enabled */
/* GTDVD Setting (TDFER) */
#define _0000_GPTn_GTDVD_VALUE_SEPARATE             (0x0000U) /* GTDVU and GTDVD are set separately */
#define _0100_GPTn_GTDVD_VALUE_AUTO                 (0x0100U) /* GTDVU value is auto-set to GTDVD */

/*
    Interrupt Source Priority Register n (IPRn)
*/
/* Interrupt Priority Level Select (IPR[3:0]) */
#define _00_GPT_PRIORITY_LEVEL0                     (0x00U) /* Level 0 (interrupt disabled) */
#define _01_GPT_PRIORITY_LEVEL1                     (0x01U) /* Level 1 */
#define _02_GPT_PRIORITY_LEVEL2                     (0x02U) /* Level 2 */
#define _03_GPT_PRIORITY_LEVEL3                     (0x03U) /* Level 3 */
#define _04_GPT_PRIORITY_LEVEL4                     (0x04U) /* Level 4 */
#define _05_GPT_PRIORITY_LEVEL5                     (0x05U) /* Level 5 */
#define _06_GPT_PRIORITY_LEVEL6                     (0x06U) /* Level 6 */
#define _07_GPT_PRIORITY_LEVEL7                     (0x07U) /* Level 7 */
#define _08_GPT_PRIORITY_LEVEL8                     (0x08U) /* Level 8 */
#define _09_GPT_PRIORITY_LEVEL9                     (0x09U) /* Level 9 */
#define _0A_GPT_PRIORITY_LEVEL10                    (0x0AU) /* Level 10 */
#define _0B_GPT_PRIORITY_LEVEL11                    (0x0BU) /* Level 11 */
#define _0C_GPT_PRIORITY_LEVEL12                    (0x0CU) /* Level 12 */
#define _0D_GPT_PRIORITY_LEVEL13                    (0x0DU) /* Level 13 */
#define _0E_GPT_PRIORITY_LEVEL14                    (0x0EU) /* Level 14 */
#define _0F_GPT_PRIORITY_LEVEL15                    (0x0FU) /* Level 15 (highest) */

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define _FFF0_GPT0_CYCLE_VALUE                      (0xFFF0U) /* GPT0 cycle register value */
#define _0000_GPT0_INIT_COUNT                       (0x0000U) /* GPT0 initial counter value */
#define _4E20_GPT0_COMP_MATCH_A_VALUE               (0x4E20U) /* GPT0 compare match A value */
#define _9C40_GPT0_COMP_MATCH_B_VALUE               (0x9C40U) /* GPT0 compare match B value */
#define _0064_GPT0_COMP_MATCH_C_VALUE               (0x0064U) /* GPT0 compare match C value */
#define _0064_GPT0_COMP_MATCH_D_VALUE               (0x0064U) /* GPT0 compare match D value */
#define _0064_GPT0_COMP_MATCH_E_VALUE               (0x0064U) /* GPT0 compare match E value */
#define _0064_GPT0_COMP_MATCH_F_VALUE               (0x0064U) /* GPT0 compare match F value */

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void R_GPT_Create(void);
void R_GPT0_Start(void);
void R_GPT0_Stop(void);

/* Start user code for function. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#endif