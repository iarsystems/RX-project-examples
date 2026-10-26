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
* Copyright (C) 2015, 2016 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_s12ad.h
* Version      : Applilet4 for RX24T V1.00.01.02 [07 Mar 2016]
* Device(s)    : R5F524TAAxFP
* Tool-Chain   : IAR Systems ICCRX
* Description  : This file implements device driver for S12AD module.
* Creation Date: 2016-07-08
***********************************************************************************************************************/
#ifndef S12AD_H
#define S12AD_H

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/
/*
    A/D control register (ADCSR)
*/
/* Group B scan end interrupt enable (GBADIE) */
#define _0000_AD_GBADI_DISABLE               (0x0000U) /* Disables GBADI interrupt generation upon group B scan
                                                          completion */
#define _0040_AD_GBADI_ENABLE                (0x0040U) /* Enables GBADI interrupt generation upon group B scan
                                                          completion */
/* Double trigger mode select (DBLE) */
#define _0000_AD_DBLTRIGGER_DISABLE          (0x0000U) /* Disable double trigger mode */
#define _0080_AD_DBLTRIGGER_ENABLE           (0x0080U) /* Enable double trigger mode */
/* Trigger select (EXTRG) */
#define _0000_AD_SYNC_TRIGGER                (0x0000U) /* A/D conversion started by snychronous trigger */
#define _0100_AD_ASYNC_TRIGGER               (0x0100U) /* A/D conversion started by asynchronous trigger */
/* Trigger start enable (TRGE) */
#define _0000_AD_SYNCASYNCTRG_DISABLE        (0x0000U) /* A/D conversion synchronous or asynchronous trigger disable */
#define _0200_AD_SYNCASYNCTRG_ENABLE         (0x0200U) /* A/D conversion synchronous or asynchronous trigger enable */
/* Scan end interrupt enable (ADIE) */
#define _0000_AD_SCAN_END_INTERRUPT_DISABLE  (0x0000U) /* Disable S12ADI interrupt generation upon scan completion */
#define _1000_AD_SCAN_END_INTERRUPT_ENABLE   (0x1000U) /* Enable S12ADI interrupt generation upon scan completion */
/* Scan mode select (ADCS) */
#define _0000_AD_SINGLE_SCAN_MODE            (0x0000U) /* Single scan mode */
#define _2000_AD_GROUP_SCAN_MODE             (0x2000U) /* Group scan mode */
#define _4000_AD_CONTINUOUS_SCAN_MODE        (0x4000U) /* Continuous scan mode */
/* A/D conversion start (ADST) */
#define _0000_AD_CONVERSION_STOP             (0x0000U) /* Stop A/D conversion */
#define _8000_AD_CONVERSION_START            (0x8000U) /* Start A/D conversion */

/*
    A/D converted value addition count select register (ADADC)
*/
/* Addition Count Select ADC[2:0] */
#define _00_AD_1_TIME_CONVERSION             (0x00U) /* 1-time conversion */
#define _01_AD_2_TIME_CONVERSION             (0x01U) /* 2-time conversion */
#define _02_AD_3_TIME_CONVERSION             (0x02U) /* 3-time conversion */
#define _03_AD_4_TIME_CONVERSION             (0x03U) /* 4-time conversion */
#define _05_AD_16_TIME_CONVERSION            (0x05U) /* 16-time conversion */
/* Average Mode Enable bit (AVEE) */
#define _00_AD_ADDITION_MODE                 (0x00U) /* Addition mode */
#define _80_AD_AVERAGE_MODE                  (0x80U) /* Average mode */

/*
    A/D control extended register (ADCER)
*/
/* Automatic clearing enable (ACE) */
#define _0000_AD_AUTO_CLEARING_DISABLE       (0x0000U) /* Disable auto clearing */
#define _0020_AD_AUTO_CLEARING_ENABLE        (0x0020U) /* Enable auto clearing */
/* A/D Self-diagnosis selection (DIAGVAL) */
#define _0000_AD_SELFTDIAGST_DISABLE         (0x0000U) /* Disable self-diagnosis */
#define _0100_AD_SELFTDIAGST_AVCC0_0         (0x0100U) /* Self-diagnosis using a voltage of 0V */
#define _0200_AD_SELFTDIAGST_AVCC0_HALF      (0x0200U) /* Self-diagnosis using a voltage of AVCC0_1/2*/
#define _0300_AD_SELFTDIAGST_AVCC0           (0x0300U) /* Self-diagnosis using a voltage of AVCC0_1*/
#define _0100_AD_SELFTDIAGST_AVCC1_0         (0x0100U) /* Self-diagnosis using a voltage of 0V */
#define _0200_AD_SELFTDIAGST_AVCC1_HALF      (0x0200U) /* Self-diagnosis using a voltage of AVCC1_1/2*/
#define _0300_AD_SELFTDIAGST_AVCC1           (0x0300U) /* Self-diagnosis using a voltage of AVCC1_1*/
#define _0100_AD_SELFTDIAGST_AVCC2_0         (0x0100U) /* Self-diagnosis using a voltage of 0V */
#define _0200_AD_SELFTDIAGST_AVCC2_HALF      (0x0200U) /* Self-diagnosis using a voltage of AVCC2_1/2*/
#define _0300_AD_SELFTDIAGST_AVCC2           (0x0300U) /* Self-diagnosis using a voltage of AVCC2_1*/
/* A/D Self-diagnostic mode selection (DIAGLD) */
#define _0000_AD_SELFTDIAGST_ROTATION        (0x0000U) /* Rotation mode for self-diagnosis voltage */
#define _0400_AD_SELFTDIAGST_FIX             (0x0400U) /* Fixed mode for self-diagnosis voltage */
/* A/D Self-diagnostic enable (DIAGM) */
#define _0000_AD_SELFTDIAGST_DISABLE         (0x0000U) /* 12bit self-diagnosis disable */
#define _0800_AD_SELFTDIAGST_ENABLE          (0x0800U) /* 12bit self-diagnosis enable */
/* A/D data register format selection (ADRFMT) */
#define _0000_AD_RIGHT_ALIGNMENT             (0x0000U) /* Right-alignment for data register format */
#define _8000_AD_LEFT_ALIGNMENT              (0x8000U) /* Left-alignment for data register format */

/*
    A/D start trigger select register (ADSTRGR)
*/
/* A/D conversion start trigger select for group B (TRSB) */
#define _0001_AD_TRSB_TRGA0N                 (0x0001U) /* Compare match with or input capture to MTU0.TGRA */
#define _0002_AD_TRSB_TRGA1N                 (0x0002U) /* Compare match with or input capture to MTU1.TGRA */
#define _0003_AD_TRSB_TRGA2N                 (0x0003U) /* Compare match with or input capture to MTU2.TGRA */
#define _0004_AD_TRSB_TRGA3N                 (0x0004U) /* Compare match with or input capture to MTU3.TGRA */
#define _0005_AD_TRSB_TRGA4N                 (0x0005U) /* Compare match with or input capture to MTU4.TGRA, or an
                                                          underflow of MTU4.TCNT (in the trough) in complementary
                                                          PWM mode */
#define _0006_AD_TRSB_TRGA6N                 (0x0006U) /* Compare match with or input capture to MTU6.TGRA */
#define _0007_AD_TRSB_TRGA7N                 (0x0007U) /* Compare match with or input capture to MTU7.TGRA, or an
                                                          underflow of MTU7.TCNT (in the trough) in complementary
                                                          PWM mode */
#define _0008_AD_TRSB_TRG0N                  (0x0008U) /* Compare match with MTU0.TGRE */
#define _0009_AD_TRSB_TRG4AN                 (0x0009U) /* Compare match between MTU4.TADCORA and MTU4.TCNT */
#define _000A_AD_TRSB_TRG4BN                 (0x000AU) /* Compare match between MTU4.TADCORB and MTU4.TCNT */
#define _000B_AD_TRSB_TRG4AN_TRG4BN          (0x000BU) /* Compare match between MTU4.TADCORA and MTU4.TCNT, or
                                                          between MTU4.TADCORB and MTU4.TCNT */
#define _000C_AD_TRSB_TRG4ABN                (0x000CU) /* Compare match between MTU4.TADCORA and MTU4.TCNT, and
                                                          between MTU4.TADCORB and MTU4.TCNT (when interrupt skipping
                                                          function 2 is used) */
#define _000D_AD_TRSB_TRG7AN                 (0x000DU) /* Compare match between MTU7.TADCORA and MTU7.TCNT */
#define _000E_AD_TRSB_TRG7BN                 (0x000EU) /* Compare match between MTU7.TADCORB and MTU7.TCNT */
#define _000F_AD_TRSB_TRG7AN_TRG7BN          (0x000FU) /* Compare match between MTU7.TADCORA and MTU7.TCNT, or
                                                          between MTU7.TADCORB and MTU7.TCNT */
#define _0010_AD_TRSB_TRG7ABN                (0x0010U) /* Compare match between MTU7.TADCORA and MTU7.TCNT, and
                                                          between MTU7.TADCORB and MTU7.TCNT (when interrupt skipping
                                                          function 2 is used) */
#define _0013_AD_TRSB_TRGA9N                 (0x0013U) /* Compare match with or input capture to MTU9.TGRA */
#define _0014_AD_TRSB_TRG9N                  (0x0014U) /* Compare match with MTU9.TGRE */
#define _0019_AD_TRSB_TRGA0N_TRG0N           (0x0019U) /* Compare match with or input capture to MTU0.TGRA, or
                                                          compare matach with MTU0.TRGE */
#define _001A_AD_TRSB_TRGA9N_TRG9N           (0x001AU) /* Compare match with or input capture to MTU9.TGRA, or
                                                          compare matach with MTU9.TRGE */
#define _001B_AD_TRSB_TRGA0N_TRGA9N          (0x001BU) /* Compare match with or input capture to MTU0.TGRA, or 
                                                          compare match with or input capture to MTU9.TRGA */
#define _001C_AD_TRSB_TRG0N_TRG9N            (0x001CU) /* Compare match with MTU0.TGRE or MTU9.TRGE */
#define _001D_AD_TRSB_TMTRG0AN0              (0x001DU) /* Compare match between TMR0.TCORA0 and TMR0.TCNT0 */
#define _001E_AD_TRSB_TMTRG0AN1              (0x001EU) /* Compare match between TMR2.TCORA0 and TMR2.TCNT0 */
#define _001F_AD_TRSB_TMTRG0AN2              (0x001FU) /* Compare match between TMR4.TCORA0 and TMR4.TCNT0 */
#define _0020_AD_TRSB_TMTRG0AN3              (0x0020U) /* Compare match between TMR6.TCORA0 and TMR6.TCNT0 */
#define _0021_AD_TRSB_TRG9AEN                (0x0021U) /* Compare match with or input capture to MTU9.TGRA, and 
                                                          compare match with MTU9.TGRE.*/
#define _0022_AD_TRSB_TRG0AEN                (0x0022U) /* Compare match with or input capture to MTU0.TGRA, and 
                                                          compare match with MTU0.TGRE.*/
#define _0023_AD_TRSB_TRGA09N                (0x0023U) /* Compare match with or input capture to MTU0.TGRA, and 
                                                          compare match with or input capture to MTU9.TGRA.*/
#define _0024_AD_TRSB_TRG09N                 (0x0024U) /* Compare match with MTU0.TGRE, and compare match with MTU9.TGRE.*/
/* A/D conversion start trigger select for group A (TRSA) */
#define _0000_AD_TRSA_ADTRG                  (0x0000U) /* Input pin for the trigger */
#define _0100_AD_TRSA_TRGA0N                 (0x0100U) /* Compare match with or input capture to MTU0.TGRA */
#define _0200_AD_TRSA_TRGA1N                 (0x0200U) /* Compare match with or input capture to MTU1.TGRA */
#define _0300_AD_TRSA_TRGA2N                 (0x0300U) /* Compare match with or input capture to MTU2.TGRA */
#define _0400_AD_TRSA_TRGA3N                 (0x0400U) /* Compare match with or input capture to MTU3.TGRA */
#define _0500_AD_TRSA_TRGA4N                 (0x0500U) /* Compare match with or input capture to MTU4.TGRA, or an
                                                          underflow of MTU4.TCNT (in the trough) in complementary
                                                          PWM mode */
#define _0600_AD_TRSA_TRGA6N                 (0x0600U) /* Compare match with or input capture to MTU6.TGRA */
#define _0700_AD_TRSA_TRGA7N                 (0x0700U) /* Compare match with or input capture to MTU7.TGRA, or an
                                                          underflow of MTU7.TCNT (in the trough) in complementary
                                                          PWM mode */
#define _0800_AD_TRSA_TRG0N                  (0x0800U) /* Compare match with MTU0.TGRE */
#define _0900_AD_TRSA_TRG4AN                 (0x0900U) /* Compare match between MTU4.TADCORA and MTU4.TCNT */
#define _0A00_AD_TRSA_TRG4BN                 (0x0A00U) /* Compare match between MTU4.TADCORB and MTU4.TCNT */
#define _0B00_AD_TRSA_TRG4AN_TRG4BN          (0x0B00U) /* Compare match between MTU4.TADCORA and MTU4.TCNT, or
                                                          between MTU4.TADCORB and MTU4.TCNT */
#define _0C00_AD_TRSA_TRG4ABN                (0x0C00U) /* Compare match between MTU4.TADCORA and MTU4.TCNT, and
                                                          between MTU4.TADCORB and MTU4.TCNT (when interrupt skipping
                                                          function 2 is used) */
#define _0D00_AD_TRSA_TRG7AN                 (0x0D00U) /* Compare match between MTU7.TADCORA and MTU7.TCNT */
#define _0E00_AD_TRSA_TRG7BN                 (0x0E00U) /* Compare match between MTU7.TADCORB and MTU7.TCNT */
#define _0F00_AD_TRSA_TRG7AN_TRG7BN          (0x0F00U) /* Compare match between MTU7.TADCORA and MTU7.TCNT, or
                                                          between MTU7.TADCORB and MTU7.TCNT */
#define _1000_AD_TRSA_TRG7ABN                (0x1000U) /* Compare match between MTU7.TADCORA and MTU7.TCNT, and
                                                          between MTU7.TADCORB and MTU7.TCNT (when interrupt skipping
                                                          function 2 is used) */
#define _1300_AD_TRSA_TRGA9N                 (0x1300U) /* Compare match with or input capture to MTU9.TGRA */
#define _1400_AD_TRSA_TRG9N                  (0x1400U) /* Compare match with MTU9.TGRE */
#define _1900_AD_TRSA_TRGA0N_TRG0N           (0x1900U) /* Compare match with or input capture to MTU0.TGRA, or
                                                          compare matach with MTU0.TGRE */
#define _1A00_AD_TRSA_TRGA9N_TRG9N           (0x1A00U) /* Compare match with or input capture to MTU9.TGRA, or
                                                          compare matach with MTU9.TGRE */
#define _1B00_AD_TRSA_TRGA0N_TRGA9N          (0x1B00U) /* Compare match with or input capture to MTU0.TGRA, or MTU9.TRGA */
#define _1C00_AD_TRSA_TRG0N_TRG9N            (0x1C00U) /* Compare match with MTU0.TGRE or MTU9.TGRE */
#define _1D00_AD_TRSA_TMTRG0AN0              (0x1D00U) /* Compare match between TMR0.TCORA0 and TMR0.TCNT0 */
#define _1E00_AD_TRSA_TMTRG0AN1              (0x1E00U) /* Compare match between TMR2.TCORA0 and TMR2.TCNT0 */
#define _1F00_AD_TRSA_TMTRG0AN2              (0x1F00U) /* Compare match between TMR4.TCORA0 and TMR4.TCNT0 */
#define _2000_AD_TRSA_TMTRG0AN3              (0x2000U) /* Compare match between TMR6.TCORA0 and TMR6.TCNT0 */
#define _2100_AD_TRSA_TRG9AEN                (0x2100U) /* Compare match with or input capture to MTU9.TGRA, and 
                                                          compare match with MTU9.TGRE.*/
#define _2200_AD_TRSA_TRG0AEN                (0x2200U) /* Compare match with or input capture to MTU0.TGRA, and 
                                                          compare match with MTU0.TGRE.*/
#define _2300_AD_TRSA_TRGA09N                (0x2300U) /* Compare match with or input capture to MTU0.TGRA, and MTU9.TGRA */
#define _2400_AD_TRSA_TRG09N                 (0x2400U) /* Compare match with MTU0.TGRE, and MTU9.TGRE*/

/*
    A/D converted extended input control register (ADEXICR)
*/
/* Internal reference voltage A/D conversion value addition mode select (OCSAD) */
#define _0000_AD_IREF_ADDITION_DISABLE       (0x0000U) /* Internal ref. voltage A/D converted value addition/average 
                                                          mode disabled */
#define _0002_AD_IREF_ADDITION_ENABLE        (0x0002U) /* Internal ref. voltage A/D converted value addition/average
                                                           mode enabled */
/* Internal reference voltage A/D conversion select (OCSA) */
#define _0000_AD_IREF_GROUPA_DISABLE         (0x0000U) /* A/D conversion of internal reference voltage is disabled in 
                                                          group A  */
#define _0200_AD_IREF_GROUPA_ENABLE          (0x0200U) /* A/D conversion of internal reference voltage is enabled in 
                                                          group A  */

/*
    A/D Group C Trigger Select Register (ADGCTRGR)
*/
/* Group C A/D Conversion Start Trigger Select (TRSC) */
#define _01_AD_TRSC_TRGA0N                   (0x01U) /* Compare match with or input capture to MTU0.TGRA */
#define _02_AD_TRSC_TRGA1N                   (0x02U) /* Compare match with or input capture to MTU1.TGRA */
#define _03_AD_TRSC_TRGA2N                   (0x03U) /* Compare match with or input capture to MTU2.TGRA */
#define _04_AD_TRSC_TRGA3N                   (0x04U) /* Compare match with or input capture to MTU3.TGRA */
#define _05_AD_TRSC_TRGA4N                   (0x05U) /* Compare match with or input capture to MTU4.TGRA, or an
                                                        underflow of MTU4.TCNT (in the trough) in complementary
                                                        PWM mode */
#define _06_AD_TRSC_TRGA6N                   (0x06U) /* Compare match with or input capture to MTU6.TGRA */
#define _07_AD_TRSC_TRGA7N                   (0x07U) /* Compare match with or input capture to MTU7.TGRA, or an
                                                        underflow of MTU7.TCNT (in the trough) in complementary
                                                        PWM mode */
#define _08_AD_TRSC_TRG0N                    (0x08U) /* Compare match with MTU0.TGRE */
#define _09_AD_TRSC_TRG4AN                   (0x09U) /* Compare match between MTU4.TADCORA and MTU4.TCNT */
#define _0A_AD_TRSC_TRG4BN                   (0x0AU) /* Compare match between MTU4.TADCORB and MTU4.TCNT */
#define _0B_AD_TRSC_TRG4AN_TRG4BN            (0x0BU) /* Compare match between MTU4.TADCORA and MTU4.TCNT, or
                                                        between MTU4.TADCORB and MTU4.TCNT */
#define _0C_AD_TRSC_TRG4ABN                  (0x0CU) /* Compare match between MTU4.TADCORA and MTU4.TCNT, and
                                                        between MTU4.TADCORB and MTU4.TCNT (when interrupt skipping
                                                        function 2 is used) */
#define _0D_AD_TRSC_TRG7AN                   (0x0DU) /* Compare match between MTU7.TADCORA and MTU7.TCNT */
#define _0E_AD_TRSC_TRG7BN                   (0x0EU) /* Compare match between MTU7.TADCORB and MTU7.TCNT */
#define _0F_AD_TRSC_TRG7AN_TRG7BN            (0x0FU) /* Compare match between MTU7.TADCORA and MTU7.TCNT, or
                                                        between MTU7.TADCORB and MTU7.TCNT */
#define _10_AD_TRSC_TRG7ABN                  (0x10U) /* Compare match between MTU7.TADCORA and MTU7.TCNT, and
                                                        between MTU7.TADCORB and MTU7.TCNT (when interrupt skipping
                                                        function 2 is used) */
#define _13_AD_TRSC_TRGA9N                   (0x13U) /* Compare match with or input capture to MTU9.TGRA */
#define _14_AD_TRSC_TRG9N                    (0x14U) /* Compare match with MTU9.TGRE */
#define _19_AD_TRSC_TRGA0N_TRG0N             (0x19U) /* Compare match with or input capture to MTU0.TGRA, or
                                                        compare matach with MTU0.TRGE */
#define _1A_AD_TRSC_TRGA9N_TRG9N             (0x1AU) /* Compare match with or input capture to MTU9.TGRA, or
                                                        compare matach with MTU9.TRGE */
#define _1B_AD_TRSC_TRGA0N_TRGA9N            (0x1BU) /* Compare match with or input capture to MTU0.TGRA, or 
                                                        compare match with or input capture to MTU9.TRGA */
#define _1C_AD_TRSC_TRG0N_TRG9N              (0x1CU) /* Compare match with MTU0.TGRE or MTU9.TRGE */
#define _1D_AD_TRSC_TMTRG0AN0                (0x1DU) /* Compare match between TMR0.TCORA0 and TMR0.TCNT0 */
#define _1E_AD_TRSC_TMTRG0AN1                (0x1EU) /* Compare match between TMR2.TCORA0 and TMR2.TCNT0 */
#define _1F_AD_TRSC_TMTRG0AN2                (0x1FU) /* Compare match between TMR4.TCORA0 and TMR4.TCNT0 */
#define _20_AD_TRSC_TMTRG0AN3                (0x20U) /* Compare match between TMR6.TCORA0 and TMR6.TCNT0 */
#define _21_AD_TRSC_TRG9AEN                  (0x21U) /* Compare match with or input capture to MTU9.TGRA, and 
                                                        compare match with MTU9.TGRE.*/
#define _22_AD_TRSC_TRG0AEN                  (0x22U) /* Compare match with or input capture to MTU0.TGRA, and 
                                                          compare match with MTU0.TGRE.*/
#define _23_AD_TRSC_TRGA09N                  (0x23U) /* Compare match with or input capture to MTU0.TGRA, and 
                                                        compare match with or input capture to MTU9.TGRA.*/
#define _24_AD_TRSC_TRG09N                   (0x24U) /* Compare match with MTU0.TGRE, and compare match with MTU9.TGRE.*/

/* Group C Scan End Interrupt Enable (GCADIE) */
#define _00_AD_GCADI_DISABLE                 (0x00U) /* Disables GCADI interrupt generation after completion of group C scan */
#define _40_AD_GCADI_ENABLE                  (0x40U) /* Enables GCADI interrupt generation after completion of group C scan */

/* Group C A/D Conversion Operation Enable (GRCE) */
#define _00_AD_GROUPC_DISABLE                (0x00U) /* Disables A/D conversion operation for group C */
#define _80_AD_GROUPC_ENABLE                 (0x80U) /* Enables A/D conversion operation for group C */

/*
    A/D Group Scan Priority Control Register (ADGSPCR)
*/
/* Group Priority Control Setting (PGS) */
#define _0000_AD_GRPPRIORITY_DISABLE         (0x0000U) /* Operation is without group priority control */
#define _0001_AD_GRPPRIORITY_ENABLE          (0x0001U) /* Operation is with group priority control */
/* Low-Priority Group Restart Setting (GBRSCN) */
#define _0000_AD_GRPRESTART_DISABLE          (0x0000U) /* Scanning for the group is not restarted after discontinued due to group priority control */
#define _0002_AD_GRPRESTART_ENABLE           (0x0002U) /* Scanning for the group is restarted after discontinued due to group priority control */
/* Restart Channel Select (LGRRS) */
#define _0000_AD_RESTART_START_CHANNEL       (0x0000U) /* Scanning is restarted from the scan start channel */
#define _4000_AD_RESTART_INCOMPLETED_CHANNEL (0x4000U) /* Scanning is restarted from the channel on which A/D conversion is not completed */
/* Single Cycle Scan Continuous Start (GBRP) */
#define _0000_AD_GRPSCSCS_DISABLE            (0x0000U) /* Single scan is not continuously activated */
#define _8000_AD_GRPSCSCS_ENABLE             (0x8000U) /* Single scan for the lowest-priority group is continuously activated */

/*
    A/D Programmable Gain Amplifier Control Register (ADPGACR)
*/
/* PGA P000 Amplifier Pass-Through Enable (P000SEL1) */
#define _9999_AD_AMPLIPASSS_DIS_AN000        (0x9999U) /* Disable pass through the amplifier in the PGA channel AN000 */
#define _999B_AD_AMPLIPASSS_EN_AN000         (0x999BU) /* Enable pass through the amplifier in the PGA channel AN000 */
/* PGA P000 Amplifier Enable (P000ENAMP) */
#define _9999_AD_AMPLI_DISABLE_AN000         (0x9999U) /* Not use the amplifier in the PGA channel AN000 */
#define _999D_AD_AMPLI_ENABLE_AN000          (0x999DU) /* Uses the amplifier in the PGA channel AN000 */
/* PGA P100 Amplifier Pass-Through Enable (P100SEL1) */
#define _9999_AD_AMPLIPASSS_DIS_AN100        (0x9999U) /* Disable pass through the amplifier in the PGA channel AN100 */
#define _999B_AD_AMPLIPASSS_EN_AN100         (0x999BU) /* Enable pass through the amplifier in the PGA channel AN100 */
/* PGA P100 Amplifier Enable (P100ENAMP) */
#define _9999_AD_AMPLI_DISABLE_AN100         (0x9999U) /* Not use the amplifier in the PGA channel AN100 */
#define _999D_AD_AMPLI_ENABLE_AN100          (0x999DU) /* Uses the amplifier in the PGA channel AN100 */
/* PGA P101 Amplifier Pass-Through Enable (P101SEL1) */
#define _9999_AD_AMPLIPASSS_DIS_AN101        (0x9999U) /* Disable pass through the amplifier in the PGA channel AN101 */
#define _99B9_AD_AMPLIPASSS_EN_AN101         (0x99B9U) /* Enable pass through the amplifier in the PGA channel AN101 */
/* PGA P101 Amplifier Enable (P101ENAMP) */
#define _9999_AD_AMPLI_DISABLE_AN101         (0x9999U) /* Not use the amplifier in the PGA channel AN101 */
#define _99D9_AD_AMPLI_ENABLE_AN101          (0x99D9U) /* Uses the amplifier in the PGA channel AN101 */
/* PGA P102 Amplifier Pass-Through Enable (P102SEL1) */
#define _9999_AD_AMPLIPASSS_DIS_AN102        (0x9999U) /* Disable pass through the amplifier in the PGA channel AN102 */
#define _9B99_AD_AMPLIPASSS_EN_AN102         (0x9B99U) /* Enable pass through the amplifier in the PGA channel AN102 */
/* PGA P102 Amplifier Enable (P102ENAMP) */
#define _9999_AD_AMPLI_DISABLE_AN102         (0x9999U) /* Not use the amplifier in the PGA channel AN102 */
#define _9D99_AD_AMPLI_ENABLE_AN102          (0x9D99U) /* Uses the amplifier in the PGA channel AN102 */

/*
    A/D Programmable Gain Amplifier Gain Setting Register 0 (ADPGAGS0)
*/
/* PGA P000/P100 Gain Setting P000GAIN[3:0]/P100GAIN[3:0]/ */
#define _0000_AD_GAIN_AMPLIFIER              (0x0000U) /* The gain of Programmable gain amplifier P000/P100 : x 2.000 */
#define _0001_AD_GAIN_AMPLIFIER              (0x0001U) /* The gain of Programmable gain amplifier P000/P100 : x 2.500 */
#define _0004_AD_GAIN_AMPLIFIER              (0x0004U) /* The gain of Programmable gain amplifier P000/P100 : x 3.077 */
#define _0006_AD_GAIN_AMPLIFIER              (0x0006U) /* The gain of Programmable gain amplifier P000/P100 : x 3.636 */
#define _0007_AD_GAIN_AMPLIFIER              (0x0007U) /* The gain of Programmable gain amplifier P000/P100 : x 4.000 */
#define _0008_AD_GAIN_AMPLIFIER              (0x0008U) /* The gain of Programmable gain amplifier P000/P100 : x 4.444 */
/* PGA P101 Gain Setting P101GAIN[3:0] */
#define _0000_AD_GAIN_AMPLIFIER              (0x0000U) /* The gain of Programmable gain amplifier P101 : x 2.000 */
#define _0010_AD_GAIN_AMPLIFIER              (0x0010U) /* The gain of Programmable gain amplifier P101 : x 2.500 */
#define _0040_AD_GAIN_AMPLIFIER              (0x0040U) /* The gain of Programmable gain amplifier P101 : x 3.077 */
#define _0060_AD_GAIN_AMPLIFIER              (0x0060U) /* The gain of Programmable gain amplifier P101 : x 3.636 */
#define _0070_AD_GAIN_AMPLIFIER              (0x0070U) /* The gain of Programmable gain amplifier P101 : x 4.000 */
#define _0080_AD_GAIN_AMPLIFIER              (0x0080U) /* The gain of Programmable gain amplifier P101 : x 4.444 */
/* PGA P102 Gain Setting P102GAIN[3:0] */
#define _0000_AD_GAIN_AMPLIFIER              (0x0000U) /* The gain of Programmable gain amplifier P102 : x 2.000 */
#define _0100_AD_GAIN_AMPLIFIER              (0x0100U) /* The gain of Programmable gain amplifier P102 : x 2.500 */
#define _0400_AD_GAIN_AMPLIFIER              (0x0400U) /* The gain of Programmable gain amplifier P102 : x 3.077 */
#define _0600_AD_GAIN_AMPLIFIER              (0x0600U) /* The gain of Programmable gain amplifier P102 : x 3.636 */
#define _0700_AD_GAIN_AMPLIFIER              (0x0700U) /* The gain of Programmable gain amplifier P102 : x 4.000 */
#define _0800_AD_GAIN_AMPLIFIER              (0x0800U) /* The gain of Programmable gain amplifier P102 : x 4.444 */

/*
    Interrupt Source Priority Register n (IPRn)
*/
/* Interrupt Priority Level Select (IPR[3:0]) */
#define _00_AD_PRIORITY_LEVEL0               (0x00U) /* Level 0 (interrupt disabled) */
#define _01_AD_PRIORITY_LEVEL1               (0x01U) /* Level 1 */
#define _02_AD_PRIORITY_LEVEL2               (0x02U) /* Level 2 */
#define _03_AD_PRIORITY_LEVEL3               (0x03U) /* Level 3 */
#define _04_AD_PRIORITY_LEVEL4               (0x04U) /* Level 4 */
#define _05_AD_PRIORITY_LEVEL5               (0x05U) /* Level 5 */
#define _06_AD_PRIORITY_LEVEL6               (0x06U) /* Level 6 */
#define _07_AD_PRIORITY_LEVEL7               (0x07U) /* Level 7 */
#define _08_AD_PRIORITY_LEVEL8               (0x08U) /* Level 8 */
#define _09_AD_PRIORITY_LEVEL9               (0x09U) /* Level 9 */
#define _0A_AD_PRIORITY_LEVEL10              (0x0AU) /* Level 10 */
#define _0B_AD_PRIORITY_LEVEL11              (0x0BU) /* Level 11 */
#define _0C_AD_PRIORITY_LEVEL12              (0x0CU) /* Level 12 */
#define _0D_AD_PRIORITY_LEVEL13              (0x0DU) /* Level 13 */
#define _0E_AD_PRIORITY_LEVEL14              (0x0EU) /* Level 14 */
#define _0F_AD_PRIORITY_LEVEL15              (0x0FU) /* Level 15 (highest) */

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define _0001_AD_CHANNEL_SELECT_A0           (0x0001U)
#define _0000_AD_CHANNEL_SELECT_A1           (0x0000U)
#define _0000_AD_ADDAVG_CHANNEL_SELECT0      (0x0000U)
#define _0000_AD_ADDAVG_CHANNEL_SELECT1      (0x0000U)
#define _00_AD_DISCONECT_SETTING             (0x00U)
#define _93_AD_SAMPLING_STATE_0              (0x93U)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
typedef enum
{
    ADCHANNEL0, ADCHANNEL1, ADCHANNEL2, ADCHANNEL3, ADCHANNEL4, ADCHANNEL5, ADCHANNEL6,
    ADCHANNEL7, ADCHANNEL8, ADCHANNEL9, ADCHANNEL10, ADCHANNEL11, ADCHANNEL16, ADSELFDIAGNOSIS,
    ADINTERREFVOLT,ADDATADUPLICATION, ADDATADUPLICATIONA, ADDATADUPLICATIONB
} ad_channel_t;

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void R_S12AD0_Create(void);
void R_S12AD0_Start(void);
void R_S12AD0_Stop(void);
void R_S12AD0_Get_ValueResult(ad_channel_t channel, uint16_t * const buffer);

/* Start user code for function. Do not edit comment generated here */

/* Flag indicates when A/D conversion is complete */

extern volatile uint8_t g_adc_complete;

/* Functions for starting and stopping software triggered A/D conversion */
void R_S12AD0_SWTriggerStart(void);
void R_S12AD0_SWTriggerStop(void);

/* End user code. Do not edit comment generated here */
#endif