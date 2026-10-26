/***********************************************************************************************************************
 * Copyright [2020-2022] Renesas Electronics Corporation and/or its affiliates.  All Rights Reserved.
 *
 * This software and documentation are supplied by Renesas Electronics Corporation and/or its affiliates and may only
 * be used with products of Renesas Electronics Corp. and its affiliates ("Renesas").  No other uses are authorized.
 * Renesas products are sold pursuant to Renesas terms and conditions of sale.  Purchasers are solely responsible for
 * the selection and use of Renesas products and Renesas assumes no liability.  No license, express or implied, to any
 * intellectual property right is granted by Renesas.  This software is protected under all applicable laws, including
 * copyright laws. Renesas reserves the right to change or discontinue this software and/or this documentation.
 * THE SOFTWARE AND DOCUMENTATION IS DELIVERED TO YOU "AS IS," AND RENESAS MAKES NO REPRESENTATIONS OR WARRANTIES, AND
 * TO THE FULLEST EXTENT PERMISSIBLE UNDER APPLICABLE LAW, DISCLAIMS ALL WARRANTIES, WHETHER EXPLICITLY OR IMPLICITLY,
 * INCLUDING WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT, WITH RESPECT TO THE
 * SOFTWARE OR DOCUMENTATION.  RENESAS SHALL HAVE NO LIABILITY ARISING OUT OF ANY SECURITY VULNERABILITY OR BREACH.
 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT WILL RENESAS BE LIABLE TO YOU IN CONNECTION WITH THE SOFTWARE OR
 * DOCUMENTATION (OR ANY PERSON OR ENTITY CLAIMING RIGHTS DERIVED FROM YOU) FOR ANY LOSS, DAMAGES, OR CLAIMS WHATSOEVER,
 * INCLUDING, WITHOUT LIMITATION, ANY DIRECT, CONSEQUENTIAL, SPECIAL, INDIRECT, PUNITIVE, OR INCIDENTAL DAMAGES; ANY
 * LOST PROFITS, OTHER ECONOMIC DAMAGE, PROPERTY DAMAGE, OR PERSONAL INJURY; AND EVEN IF RENESAS HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH LOSS, DAMAGES, CLAIMS OR COSTS.
 **********************************************************************************************************************/

#ifndef R_ICU_POSMSK_H
#define R_ICU_POSMSK_H

/* =========================================================================================================================== */
/* ================                                            IR                                             ================ */
/* =========================================================================================================================== */

/* ==========================================================  IR  =========================================================== */
#define R_ICU_IR_IR_IR_Pos    (0UL)    /*!< IR (Bit 0)                                            */
#define R_ICU_IR_IR_IR_Msk    (0x1UL)  /*!< IR (Bitfield-Mask: 0x01)                              */

/* =========================================================================================================================== */
/* ================                                           DTCER                                           ================ */
/* =========================================================================================================================== */

/* =========================================================  DTCER  ========================================================= */
#define R_ICU_DTCER_DTCER_DTCE_Pos    (0UL)   /*!< DTCE (Bit 0)                                          */
#define R_ICU_DTCER_DTCER_DTCE_Msk    (0x1UL) /*!< DTCE (Bitfield-Mask: 0x01)                            */

/* =========================================================================================================================== */
/* ================                                            IER                                            ================ */
/* =========================================================================================================================== */

/* ==========================================================  IER  ========================================================== */
#define R_ICU_IER_IER_IEN0_Pos    (0UL)    /*!< IEN0 (Bit 0)                                          */
#define R_ICU_IER_IER_IEN0_Msk    (0x1UL)  /*!< IEN0 (Bitfield-Mask: 0x01)                            */
#define R_ICU_IER_IER_IEN1_Pos    (1UL)    /*!< IEN1 (Bit 1)                                          */
#define R_ICU_IER_IER_IEN1_Msk    (0x2UL)  /*!< IEN1 (Bitfield-Mask: 0x01)                            */
#define R_ICU_IER_IER_IEN2_Pos    (2UL)    /*!< IEN2 (Bit 2)                                          */
#define R_ICU_IER_IER_IEN2_Msk    (0x4UL)  /*!< IEN2 (Bitfield-Mask: 0x01)                            */
#define R_ICU_IER_IER_IEN3_Pos    (3UL)    /*!< IEN3 (Bit 3)                                          */
#define R_ICU_IER_IER_IEN3_Msk    (0x8UL)  /*!< IEN3 (Bitfield-Mask: 0x01)                            */
#define R_ICU_IER_IER_IEN4_Pos    (4UL)    /*!< IEN4 (Bit 4)                                          */
#define R_ICU_IER_IER_IEN4_Msk    (0x10UL) /*!< IEN4 (Bitfield-Mask: 0x01)                            */
#define R_ICU_IER_IER_IEN5_Pos    (5UL)    /*!< IEN5 (Bit 5)                                          */
#define R_ICU_IER_IER_IEN5_Msk    (0x20UL) /*!< IEN5 (Bitfield-Mask: 0x01)                            */
#define R_ICU_IER_IER_IEN6_Pos    (6UL)    /*!< IEN6 (Bit 6)                                          */
#define R_ICU_IER_IER_IEN6_Msk    (0x40UL) /*!< IEN6 (Bitfield-Mask: 0x01)                            */
#define R_ICU_IER_IER_IEN7_Pos    (7UL)    /*!< IEN7 (Bit 7)                                          */
#define R_ICU_IER_IER_IEN7_Msk    (0x80UL) /*!< IEN7 (Bitfield-Mask: 0x01)                            */

/* =========================================================================================================================== */
/* ================                                            IPR                                            ================ */
/* =========================================================================================================================== */

/* ==========================================================  IPR  ========================================================== */
#define R_ICU_IPR_IPR_IPR_Pos    (0UL)   /*!< IPR (Bit 0)                                           */
#define R_ICU_IPR_IPR_IPR_Msk    (0xfUL) /*!< IPR (Bitfield-Mask: 0x0f)                             */

/* =========================================================================================================================== */
/* ================                                           DMRSR                                           ================ */
/* =========================================================================================================================== */

/* =========================================================  DMRSR  ========================================================= */
#define R_ICU_DMRSR_DMRSR_DMRSR_Pos    (0UL)    /*!< DMRSR (Bit 0)                                         */
#define R_ICU_DMRSR_DMRSR_DMRSR_Msk    (0xffUL) /*!< DMRSR (Bitfield-Mask: 0xff)                           */

/* =========================================================================================================================== */
/* ================                                           IRQCR                                           ================ */
/* =========================================================================================================================== */

/* =========================================================  IRQCR  ========================================================= */
#define R_ICU_IRQCR_IRQCR_IRQMD_Pos    (2UL)   /*!< IRQMD (Bit 2)                                         */
#define R_ICU_IRQCR_IRQCR_IRQMD_Msk    (0xcUL) /*!< IRQMD (Bitfield-Mask: 0x03)                           */

/* =========================================================================================================================== */
/* ================                                          IRQFLTE                                          ================ */
/* =========================================================================================================================== */

/* ========================================================  IRQFLTE  ======================================================== */
#define R_ICU_IRQFLTE_IRQFLTE_FLTEN0_Pos    (0UL)    /*!< FLTEN0 (Bit 0)                                        */
#define R_ICU_IRQFLTE_IRQFLTE_FLTEN0_Msk    (0x1UL)  /*!< FLTEN0 (Bitfield-Mask: 0x01)                          */
#define R_ICU_IRQFLTE_IRQFLTE_FLTEN1_Pos    (1UL)    /*!< FLTEN1 (Bit 1)                                        */
#define R_ICU_IRQFLTE_IRQFLTE_FLTEN1_Msk    (0x2UL)  /*!< FLTEN1 (Bitfield-Mask: 0x01)                          */
#define R_ICU_IRQFLTE_IRQFLTE_FLTEN2_Pos    (2UL)    /*!< FLTEN2 (Bit 2)                                        */
#define R_ICU_IRQFLTE_IRQFLTE_FLTEN2_Msk    (0x4UL)  /*!< FLTEN2 (Bitfield-Mask: 0x01)                          */
#define R_ICU_IRQFLTE_IRQFLTE_FLTEN3_Pos    (3UL)    /*!< FLTEN3 (Bit 3)                                        */
#define R_ICU_IRQFLTE_IRQFLTE_FLTEN3_Msk    (0x8UL)  /*!< FLTEN3 (Bitfield-Mask: 0x01)                          */
#define R_ICU_IRQFLTE_IRQFLTE_FLTEN4_Pos    (4UL)    /*!< FLTEN4 (Bit 4)                                        */
#define R_ICU_IRQFLTE_IRQFLTE_FLTEN4_Msk    (0x10UL) /*!< FLTEN4 (Bitfield-Mask: 0x01)                          */
#define R_ICU_IRQFLTE_IRQFLTE_FLTEN5_Pos    (5UL)    /*!< FLTEN5 (Bit 5)                                        */
#define R_ICU_IRQFLTE_IRQFLTE_FLTEN5_Msk    (0x20UL) /*!< FLTEN5 (Bitfield-Mask: 0x01)                          */
#define R_ICU_IRQFLTE_IRQFLTE_FLTEN6_Pos    (6UL)    /*!< FLTEN6 (Bit 6)                                        */
#define R_ICU_IRQFLTE_IRQFLTE_FLTEN6_Msk    (0x40UL) /*!< FLTEN6 (Bitfield-Mask: 0x01)                          */
#define R_ICU_IRQFLTE_IRQFLTE_FLTEN7_Pos    (7UL)    /*!< FLTEN7 (Bit 7)                                        */
#define R_ICU_IRQFLTE_IRQFLTE_FLTEN7_Msk    (0x80UL) /*!< FLTEN7 (Bitfield-Mask: 0x01)                          */

/* =========================================================================================================================== */
/* ================                                          IRQFLTC                                          ================ */
/* =========================================================================================================================== */

/* ========================================================  IRQFLTC  ======================================================== */
#define R_ICU_IRQFLTC_IRQFLTC_FCLKSEL0_Pos    (0UL)      /*!< FCLKSEL0 (Bit 0)                                      */
#define R_ICU_IRQFLTC_IRQFLTC_FCLKSEL0_Msk    (0x3UL)    /*!< FCLKSEL0 (Bitfield-Mask: 0x03)                        */
#define R_ICU_IRQFLTC_IRQFLTC_FCLKSEL1_Pos    (2UL)      /*!< FCLKSEL1 (Bit 2)                                      */
#define R_ICU_IRQFLTC_IRQFLTC_FCLKSEL1_Msk    (0xcUL)    /*!< FCLKSEL1 (Bitfield-Mask: 0x03)                        */
#define R_ICU_IRQFLTC_IRQFLTC_FCLKSEL2_Pos    (4UL)      /*!< FCLKSEL2 (Bit 4)                                      */
#define R_ICU_IRQFLTC_IRQFLTC_FCLKSEL2_Msk    (0x30UL)   /*!< FCLKSEL2 (Bitfield-Mask: 0x03)                        */
#define R_ICU_IRQFLTC_IRQFLTC_FCLKSEL3_Pos    (6UL)      /*!< FCLKSEL3 (Bit 6)                                      */
#define R_ICU_IRQFLTC_IRQFLTC_FCLKSEL3_Msk    (0xc0UL)   /*!< FCLKSEL3 (Bitfield-Mask: 0x03)                        */
#define R_ICU_IRQFLTC_IRQFLTC_FCLKSEL4_Pos    (8UL)      /*!< FCLKSEL4 (Bit 8)                                      */
#define R_ICU_IRQFLTC_IRQFLTC_FCLKSEL4_Msk    (0x300UL)  /*!< FCLKSEL4 (Bitfield-Mask: 0x03)                        */
#define R_ICU_IRQFLTC_IRQFLTC_FCLKSEL5_Pos    (10UL)     /*!< FCLKSEL5 (Bit 10)                                     */
#define R_ICU_IRQFLTC_IRQFLTC_FCLKSEL5_Msk    (0xc00UL)  /*!< FCLKSEL5 (Bitfield-Mask: 0x03)                        */
#define R_ICU_IRQFLTC_IRQFLTC_FCLKSEL6_Pos    (12UL)     /*!< FCLKSEL6 (Bit 12)                                     */
#define R_ICU_IRQFLTC_IRQFLTC_FCLKSEL6_Msk    (0x3000UL) /*!< FCLKSEL6 (Bitfield-Mask: 0x03)                        */
#define R_ICU_IRQFLTC_IRQFLTC_FCLKSEL7_Pos    (14UL)     /*!< FCLKSEL7 (Bit 14)                                     */
#define R_ICU_IRQFLTC_IRQFLTC_FCLKSEL7_Msk    (0xc000UL) /*!< FCLKSEL7 (Bitfield-Mask: 0x03)                        */

/* =========================================================================================================================== */
/* ================                                           PIBR                                            ================ */
/* =========================================================================================================================== */

/* =========================================================  PIBR  ========================================================== */
#define R_ICU_PIBR_PIBR_PIR0_Pos    (0UL)    /*!< PIR0 (Bit 0)                                          */
#define R_ICU_PIBR_PIBR_PIR0_Msk    (0x1UL)  /*!< PIR0 (Bitfield-Mask: 0x01)                            */
#define R_ICU_PIBR_PIBR_PIR1_Pos    (1UL)    /*!< PIR1 (Bit 1)                                          */
#define R_ICU_PIBR_PIBR_PIR1_Msk    (0x2UL)  /*!< PIR1 (Bitfield-Mask: 0x01)                            */
#define R_ICU_PIBR_PIBR_PIR2_Pos    (2UL)    /*!< PIR2 (Bit 2)                                          */
#define R_ICU_PIBR_PIBR_PIR2_Msk    (0x4UL)  /*!< PIR2 (Bitfield-Mask: 0x01)                            */
#define R_ICU_PIBR_PIBR_PIR3_Pos    (3UL)    /*!< PIR3 (Bit 3)                                          */
#define R_ICU_PIBR_PIBR_PIR3_Msk    (0x8UL)  /*!< PIR3 (Bitfield-Mask: 0x01)                            */
#define R_ICU_PIBR_PIBR_PIR4_Pos    (4UL)    /*!< PIR4 (Bit 4)                                          */
#define R_ICU_PIBR_PIBR_PIR4_Msk    (0x10UL) /*!< PIR4 (Bitfield-Mask: 0x01)                            */
#define R_ICU_PIBR_PIBR_PIR5_Pos    (5UL)    /*!< PIR5 (Bit 5)                                          */
#define R_ICU_PIBR_PIBR_PIR5_Msk    (0x20UL) /*!< PIR5 (Bitfield-Mask: 0x01)                            */
#define R_ICU_PIBR_PIBR_PIR6_Pos    (6UL)    /*!< PIR6 (Bit 6)                                          */
#define R_ICU_PIBR_PIBR_PIR6_Msk    (0x40UL) /*!< PIR6 (Bitfield-Mask: 0x01)                            */
#define R_ICU_PIBR_PIBR_PIR7_Pos    (7UL)    /*!< PIR7 (Bit 7)                                          */
#define R_ICU_PIBR_PIBR_PIR7_Msk    (0x80UL) /*!< PIR7 (Bitfield-Mask: 0x01)                            */

/* =========================================================================================================================== */
/* ================                                          SLIBXR                                           ================ */
/* =========================================================================================================================== */

/* ========================================================  SLIBXR  ========================================================= */
#define R_ICU_SLIBXR_SLIBXR_SLIBXR_Pos    (0UL)    /*!< SLIBXR (Bit 0)                                        */
#define R_ICU_SLIBXR_SLIBXR_SLIBXR_Msk    (0xffUL) /*!< SLIBXR (Bitfield-Mask: 0xff)                          */

/* =========================================================================================================================== */
/* ================                                           SLIBR                                           ================ */
/* =========================================================================================================================== */

/* =========================================================  SLIBR  ========================================================= */
#define R_ICU_SLIBR_SLIBR_SLIBR_Pos    (0UL)    /*!< SLIBR (Bit 0)                                         */
#define R_ICU_SLIBR_SLIBR_SLIBR_Msk    (0xffUL) /*!< SLIBR (Bitfield-Mask: 0xff)                           */

/* =========================================================================================================================== */
/* ================                                           PIAR                                            ================ */
/* =========================================================================================================================== */

/* =========================================================  PIAR  ========================================================== */
#define R_ICU_PIAR_PIAR_PIR0_Pos    (0UL)    /*!< PIR0 (Bit 0)                                          */
#define R_ICU_PIAR_PIAR_PIR0_Msk    (0x1UL)  /*!< PIR0 (Bitfield-Mask: 0x01)                            */
#define R_ICU_PIAR_PIAR_PIR1_Pos    (1UL)    /*!< PIR1 (Bit 1)                                          */
#define R_ICU_PIAR_PIAR_PIR1_Msk    (0x2UL)  /*!< PIR1 (Bitfield-Mask: 0x01)                            */
#define R_ICU_PIAR_PIAR_PIR2_Pos    (2UL)    /*!< PIR2 (Bit 2)                                          */
#define R_ICU_PIAR_PIAR_PIR2_Msk    (0x4UL)  /*!< PIR2 (Bitfield-Mask: 0x01)                            */
#define R_ICU_PIAR_PIAR_PIR3_Pos    (3UL)    /*!< PIR3 (Bit 3)                                          */
#define R_ICU_PIAR_PIAR_PIR3_Msk    (0x8UL)  /*!< PIR3 (Bitfield-Mask: 0x01)                            */
#define R_ICU_PIAR_PIAR_PIR4_Pos    (4UL)    /*!< PIR4 (Bit 4)                                          */
#define R_ICU_PIAR_PIAR_PIR4_Msk    (0x10UL) /*!< PIR4 (Bitfield-Mask: 0x01)                            */
#define R_ICU_PIAR_PIAR_PIR5_Pos    (5UL)    /*!< PIR5 (Bit 5)                                          */
#define R_ICU_PIAR_PIAR_PIR5_Msk    (0x20UL) /*!< PIR5 (Bitfield-Mask: 0x01)                            */
#define R_ICU_PIAR_PIAR_PIR6_Pos    (6UL)    /*!< PIR6 (Bit 6)                                          */
#define R_ICU_PIAR_PIAR_PIR6_Msk    (0x40UL) /*!< PIR6 (Bitfield-Mask: 0x01)                            */
#define R_ICU_PIAR_PIAR_PIR7_Pos    (7UL)    /*!< PIR7 (Bit 7)                                          */
#define R_ICU_PIAR_PIAR_PIR7_Msk    (0x80UL) /*!< PIR7 (Bitfield-Mask: 0x01)                            */

/* =========================================================================================================================== */
/* ================                                           SLIAR                                           ================ */
/* =========================================================================================================================== */

/* =========================================================  SLIAR  ========================================================= */
#define R_ICU_SLIAR_SLIAR_SLIAR_Pos    (0UL)    /*!< SLIAR (Bit 0)                                         */
#define R_ICU_SLIAR_SLIAR_SLIAR_Msk    (0xffUL) /*!< SLIAR (Bitfield-Mask: 0xff)                           */

/* =========================================================================================================================== */
/* ================                                           R_ICU                                           ================ */
/* =========================================================================================================================== */

/* ========================================================  SWINTR  ========================================================= */
#define R_ICU_SWINTR_SWINT_Pos         (0UL)          /*!< SWINT (Bit 0)                                         */
#define R_ICU_SWINTR_SWINT_Msk         (0x1UL)        /*!< SWINT (Bitfield-Mask: 0x01)                           */
/* ========================================================  SWINT2R  ======================================================== */
#define R_ICU_SWINT2R_SWINT2_Pos       (0UL)          /*!< SWINT2 (Bit 0)                                        */
#define R_ICU_SWINT2R_SWINT2_Msk       (0x1UL)        /*!< SWINT2 (Bitfield-Mask: 0x01)                          */
/* ==========================================================  FIR  ========================================================== */
#define R_ICU_FIR_FVCT_Pos             (0UL)          /*!< FVCT (Bit 0)                                          */
#define R_ICU_FIR_FVCT_Msk             (0xffUL)       /*!< FVCT (Bitfield-Mask: 0xff)                            */
#define R_ICU_FIR_FIEN_Pos             (15UL)         /*!< FIEN (Bit 15)                                         */
#define R_ICU_FIR_FIEN_Msk             (0x8000UL)     /*!< FIEN (Bitfield-Mask: 0x01)                            */
/* =========================================================  NMISR  ========================================================= */
#define R_ICU_NMISR_NMIST_Pos          (0UL)          /*!< NMIST (Bit 0)                                         */
#define R_ICU_NMISR_NMIST_Msk          (0x1UL)        /*!< NMIST (Bitfield-Mask: 0x01)                           */
#define R_ICU_NMISR_OSTST_Pos          (1UL)          /*!< OSTST (Bit 1)                                         */
#define R_ICU_NMISR_OSTST_Msk          (0x2UL)        /*!< OSTST (Bitfield-Mask: 0x01)                           */
#define R_ICU_NMISR_WDTST_Pos          (2UL)          /*!< WDTST (Bit 2)                                         */
#define R_ICU_NMISR_WDTST_Msk          (0x4UL)        /*!< WDTST (Bitfield-Mask: 0x01)                           */
#define R_ICU_NMISR_IWDTST_Pos         (3UL)          /*!< IWDTST (Bit 3)                                        */
#define R_ICU_NMISR_IWDTST_Msk         (0x8UL)        /*!< IWDTST (Bitfield-Mask: 0x01)                          */
#define R_ICU_NMISR_LVD1ST_Pos         (4UL)          /*!< LVD1ST (Bit 4)                                        */
#define R_ICU_NMISR_LVD1ST_Msk         (0x10UL)       /*!< LVD1ST (Bitfield-Mask: 0x01)                          */
#define R_ICU_NMISR_LVD2ST_Pos         (5UL)          /*!< LVD2ST (Bit 5)                                        */
#define R_ICU_NMISR_LVD2ST_Msk         (0x20UL)       /*!< LVD2ST (Bitfield-Mask: 0x01)                          */
#define R_ICU_NMISR_EXNMIST_Pos        (6UL)          /*!< EXNMIST (Bit 6)                                       */
#define R_ICU_NMISR_EXNMIST_Msk        (0x40UL)       /*!< EXNMIST (Bitfield-Mask: 0x01)                         */
/* =========================================================  NMIER  ========================================================= */
#define R_ICU_NMIER_NMIEN_Pos          (0UL)          /*!< NMIEN (Bit 0)                                         */
#define R_ICU_NMIER_NMIEN_Msk          (0x1UL)        /*!< NMIEN (Bitfield-Mask: 0x01)                           */
#define R_ICU_NMIER_OSTEN_Pos          (1UL)          /*!< OSTEN (Bit 1)                                         */
#define R_ICU_NMIER_OSTEN_Msk          (0x2UL)        /*!< OSTEN (Bitfield-Mask: 0x01)                           */
#define R_ICU_NMIER_WDTEN_Pos          (2UL)          /*!< WDTEN (Bit 2)                                         */
#define R_ICU_NMIER_WDTEN_Msk          (0x4UL)        /*!< WDTEN (Bitfield-Mask: 0x01)                           */
#define R_ICU_NMIER_IWDTEN_Pos         (3UL)          /*!< IWDTEN (Bit 3)                                        */
#define R_ICU_NMIER_IWDTEN_Msk         (0x8UL)        /*!< IWDTEN (Bitfield-Mask: 0x01)                          */
#define R_ICU_NMIER_LVD1EN_Pos         (4UL)          /*!< LVD1EN (Bit 4)                                        */
#define R_ICU_NMIER_LVD1EN_Msk         (0x10UL)       /*!< LVD1EN (Bitfield-Mask: 0x01)                          */
#define R_ICU_NMIER_LVD2EN_Pos         (5UL)          /*!< LVD2EN (Bit 5)                                        */
#define R_ICU_NMIER_LVD2EN_Msk         (0x20UL)       /*!< LVD2EN (Bitfield-Mask: 0x01)                          */
#define R_ICU_NMIER_EXNMIEN_Pos        (6UL)          /*!< EXNMIEN (Bit 6)                                       */
#define R_ICU_NMIER_EXNMIEN_Msk        (0x40UL)       /*!< EXNMIEN (Bitfield-Mask: 0x01)                         */
/* ========================================================  NMICLR  ========================================================= */
#define R_ICU_NMICLR_NMICLR_Pos        (0UL)          /*!< NMICLR (Bit 0)                                        */
#define R_ICU_NMICLR_NMICLR_Msk        (0x1UL)        /*!< NMICLR (Bitfield-Mask: 0x01)                          */
#define R_ICU_NMICLR_OSTCLR_Pos        (1UL)          /*!< OSTCLR (Bit 1)                                        */
#define R_ICU_NMICLR_OSTCLR_Msk        (0x2UL)        /*!< OSTCLR (Bitfield-Mask: 0x01)                          */
#define R_ICU_NMICLR_WDTCLR_Pos        (2UL)          /*!< WDTCLR (Bit 2)                                        */
#define R_ICU_NMICLR_WDTCLR_Msk        (0x4UL)        /*!< WDTCLR (Bitfield-Mask: 0x01)                          */
#define R_ICU_NMICLR_IWDTCLR_Pos       (3UL)          /*!< IWDTCLR (Bit 3)                                       */
#define R_ICU_NMICLR_IWDTCLR_Msk       (0x8UL)        /*!< IWDTCLR (Bitfield-Mask: 0x01)                         */
#define R_ICU_NMICLR_LVD1CLR_Pos       (4UL)          /*!< LVD1CLR (Bit 4)                                       */
#define R_ICU_NMICLR_LVD1CLR_Msk       (0x10UL)       /*!< LVD1CLR (Bitfield-Mask: 0x01)                         */
#define R_ICU_NMICLR_LVD2CLR_Pos       (5UL)          /*!< LVD2CLR (Bit 5)                                       */
#define R_ICU_NMICLR_LVD2CLR_Msk       (0x20UL)       /*!< LVD2CLR (Bitfield-Mask: 0x01)                         */
/* =========================================================  NMICR  ========================================================= */
#define R_ICU_NMICR_NMIMD_Pos          (3UL)          /*!< NMIMD (Bit 3)                                         */
#define R_ICU_NMICR_NMIMD_Msk          (0x8UL)        /*!< NMIMD (Bitfield-Mask: 0x01)                           */
/* ========================================================  EXNMISR  ======================================================== */
#define R_ICU_EXNMISR_RAMST_Pos        (0UL)          /*!< RAMST (Bit 0)                                         */
#define R_ICU_EXNMISR_RAMST_Msk        (0x1UL)        /*!< RAMST (Bitfield-Mask: 0x01)                           */
#define R_ICU_EXNMISR_DPFPUST_Pos      (1UL)          /*!< DPFPUST (Bit 1)                                       */
#define R_ICU_EXNMISR_DPFPUST_Msk      (0x2UL)        /*!< DPFPUST (Bitfield-Mask: 0x01)                         */
/* ========================================================  EXNMIER  ======================================================== */
#define R_ICU_EXNMIER_RAMEN_Pos        (0UL)          /*!< RAMEN (Bit 0)                                         */
#define R_ICU_EXNMIER_RAMEN_Msk        (0x1UL)        /*!< RAMEN (Bitfield-Mask: 0x01)                           */
#define R_ICU_EXNMIER_DPFPUEN_Pos      (1UL)          /*!< DPFPUEN (Bit 1)                                       */
#define R_ICU_EXNMIER_DPFPUEN_Msk      (0x2UL)        /*!< DPFPUEN (Bitfield-Mask: 0x01)                         */
/* =======================================================  EXNMICLR  ======================================================== */
#define R_ICU_EXNMICLR_DPFPUCLR_Pos    (1UL)          /*!< DPFPUCLR (Bit 1)                                      */
#define R_ICU_EXNMICLR_DPFPUCLR_Msk    (0x2UL)        /*!< DPFPUCLR (Bitfield-Mask: 0x01)                        */
/* ========================================================  NMIFLTE  ======================================================== */
#define R_ICU_NMIFLTE_NFLTEN_Pos       (0UL)          /*!< NFLTEN (Bit 0)                                        */
#define R_ICU_NMIFLTE_NFLTEN_Msk       (0x1UL)        /*!< NFLTEN (Bitfield-Mask: 0x01)                          */
/* ========================================================  NMIFLTC  ======================================================== */
#define R_ICU_NMIFLTC_NFCLKSEL_Pos     (0UL)          /*!< NFCLKSEL (Bit 0)                                      */
#define R_ICU_NMIFLTC_NFCLKSEL_Msk     (0x3UL)        /*!< NFCLKSEL (Bitfield-Mask: 0x03)                        */
/* ========================================================  GRPIE0  ========================================================= */
#define R_ICU_GRPIE0_IS0_Pos           (0UL)          /*!< IS0 (Bit 0)                                           */
#define R_ICU_GRPIE0_IS0_Msk           (0x1UL)        /*!< IS0 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPIE0_IS1_Pos           (1UL)          /*!< IS1 (Bit 1)                                           */
#define R_ICU_GRPIE0_IS1_Msk           (0x2UL)        /*!< IS1 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPIE0_IS2_Pos           (2UL)          /*!< IS2 (Bit 2)                                           */
#define R_ICU_GRPIE0_IS2_Msk           (0x4UL)        /*!< IS2 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPIE0_IS3_Pos           (3UL)          /*!< IS3 (Bit 3)                                           */
#define R_ICU_GRPIE0_IS3_Msk           (0x8UL)        /*!< IS3 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPIE0_IS4_Pos           (4UL)          /*!< IS4 (Bit 4)                                           */
#define R_ICU_GRPIE0_IS4_Msk           (0x10UL)       /*!< IS4 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPIE0_IS5_Pos           (5UL)          /*!< IS5 (Bit 5)                                           */
#define R_ICU_GRPIE0_IS5_Msk           (0x20UL)       /*!< IS5 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPIE0_IS6_Pos           (6UL)          /*!< IS6 (Bit 6)                                           */
#define R_ICU_GRPIE0_IS6_Msk           (0x40UL)       /*!< IS6 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPIE0_IS7_Pos           (7UL)          /*!< IS7 (Bit 7)                                           */
#define R_ICU_GRPIE0_IS7_Msk           (0x80UL)       /*!< IS7 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPIE0_IS8_Pos           (8UL)          /*!< IS8 (Bit 8)                                           */
#define R_ICU_GRPIE0_IS8_Msk           (0x100UL)      /*!< IS8 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPIE0_IS9_Pos           (9UL)          /*!< IS9 (Bit 9)                                           */
#define R_ICU_GRPIE0_IS9_Msk           (0x200UL)      /*!< IS9 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPIE0_IS10_Pos          (10UL)         /*!< IS10 (Bit 10)                                         */
#define R_ICU_GRPIE0_IS10_Msk          (0x400UL)      /*!< IS10 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPIE0_IS11_Pos          (11UL)         /*!< IS11 (Bit 11)                                         */
#define R_ICU_GRPIE0_IS11_Msk          (0x800UL)      /*!< IS11 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPIE0_IS12_Pos          (12UL)         /*!< IS12 (Bit 12)                                         */
#define R_ICU_GRPIE0_IS12_Msk          (0x1000UL)     /*!< IS12 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPIE0_IS13_Pos          (13UL)         /*!< IS13 (Bit 13)                                         */
#define R_ICU_GRPIE0_IS13_Msk          (0x2000UL)     /*!< IS13 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPIE0_IS14_Pos          (14UL)         /*!< IS14 (Bit 14)                                         */
#define R_ICU_GRPIE0_IS14_Msk          (0x4000UL)     /*!< IS14 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPIE0_IS15_Pos          (15UL)         /*!< IS15 (Bit 15)                                         */
#define R_ICU_GRPIE0_IS15_Msk          (0x8000UL)     /*!< IS15 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPIE0_IS16_Pos          (16UL)         /*!< IS16 (Bit 16)                                         */
#define R_ICU_GRPIE0_IS16_Msk          (0x10000UL)    /*!< IS16 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPIE0_IS17_Pos          (17UL)         /*!< IS17 (Bit 17)                                         */
#define R_ICU_GRPIE0_IS17_Msk          (0x20000UL)    /*!< IS17 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPIE0_IS18_Pos          (18UL)         /*!< IS18 (Bit 18)                                         */
#define R_ICU_GRPIE0_IS18_Msk          (0x40000UL)    /*!< IS18 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPIE0_IS19_Pos          (19UL)         /*!< IS19 (Bit 19)                                         */
#define R_ICU_GRPIE0_IS19_Msk          (0x80000UL)    /*!< IS19 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPIE0_IS20_Pos          (20UL)         /*!< IS20 (Bit 20)                                         */
#define R_ICU_GRPIE0_IS20_Msk          (0x100000UL)   /*!< IS20 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPIE0_IS21_Pos          (21UL)         /*!< IS21 (Bit 21)                                         */
#define R_ICU_GRPIE0_IS21_Msk          (0x200000UL)   /*!< IS21 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPIE0_IS22_Pos          (22UL)         /*!< IS22 (Bit 22)                                         */
#define R_ICU_GRPIE0_IS22_Msk          (0x400000UL)   /*!< IS22 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPIE0_IS23_Pos          (23UL)         /*!< IS23 (Bit 23)                                         */
#define R_ICU_GRPIE0_IS23_Msk          (0x800000UL)   /*!< IS23 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPIE0_IS24_Pos          (24UL)         /*!< IS24 (Bit 24)                                         */
#define R_ICU_GRPIE0_IS24_Msk          (0x1000000UL)  /*!< IS24 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPIE0_IS25_Pos          (25UL)         /*!< IS25 (Bit 25)                                         */
#define R_ICU_GRPIE0_IS25_Msk          (0x2000000UL)  /*!< IS25 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPIE0_IS26_Pos          (26UL)         /*!< IS26 (Bit 26)                                         */
#define R_ICU_GRPIE0_IS26_Msk          (0x4000000UL)  /*!< IS26 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPIE0_IS27_Pos          (27UL)         /*!< IS27 (Bit 27)                                         */
#define R_ICU_GRPIE0_IS27_Msk          (0x8000000UL)  /*!< IS27 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPIE0_IS28_Pos          (28UL)         /*!< IS28 (Bit 28)                                         */
#define R_ICU_GRPIE0_IS28_Msk          (0x10000000UL) /*!< IS28 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPIE0_IS29_Pos          (29UL)         /*!< IS29 (Bit 29)                                         */
#define R_ICU_GRPIE0_IS29_Msk          (0x20000000UL) /*!< IS29 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPIE0_IS30_Pos          (30UL)         /*!< IS30 (Bit 30)                                         */
#define R_ICU_GRPIE0_IS30_Msk          (0x40000000UL) /*!< IS30 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPIE0_IS31_Pos          (31UL)         /*!< IS31 (Bit 31)                                         */
#define R_ICU_GRPIE0_IS31_Msk          (0x80000000UL) /*!< IS31 (Bitfield-Mask: 0x01)                            */
/* ========================================================  GENIE0  ========================================================= */
#define R_ICU_GENIE0_EN0_Pos           (0UL)          /*!< EN0 (Bit 0)                                           */
#define R_ICU_GENIE0_EN0_Msk           (0x1UL)        /*!< EN0 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENIE0_EN1_Pos           (1UL)          /*!< EN1 (Bit 1)                                           */
#define R_ICU_GENIE0_EN1_Msk           (0x2UL)        /*!< EN1 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENIE0_EN2_Pos           (2UL)          /*!< EN2 (Bit 2)                                           */
#define R_ICU_GENIE0_EN2_Msk           (0x4UL)        /*!< EN2 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENIE0_EN3_Pos           (3UL)          /*!< EN3 (Bit 3)                                           */
#define R_ICU_GENIE0_EN3_Msk           (0x8UL)        /*!< EN3 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENIE0_EN4_Pos           (4UL)          /*!< EN4 (Bit 4)                                           */
#define R_ICU_GENIE0_EN4_Msk           (0x10UL)       /*!< EN4 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENIE0_EN5_Pos           (5UL)          /*!< EN5 (Bit 5)                                           */
#define R_ICU_GENIE0_EN5_Msk           (0x20UL)       /*!< EN5 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENIE0_EN6_Pos           (6UL)          /*!< EN6 (Bit 6)                                           */
#define R_ICU_GENIE0_EN6_Msk           (0x40UL)       /*!< EN6 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENIE0_EN7_Pos           (7UL)          /*!< EN7 (Bit 7)                                           */
#define R_ICU_GENIE0_EN7_Msk           (0x80UL)       /*!< EN7 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENIE0_EN8_Pos           (8UL)          /*!< EN8 (Bit 8)                                           */
#define R_ICU_GENIE0_EN8_Msk           (0x100UL)      /*!< EN8 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENIE0_EN9_Pos           (9UL)          /*!< EN9 (Bit 9)                                           */
#define R_ICU_GENIE0_EN9_Msk           (0x200UL)      /*!< EN9 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENIE0_EN10_Pos          (10UL)         /*!< EN10 (Bit 10)                                         */
#define R_ICU_GENIE0_EN10_Msk          (0x400UL)      /*!< EN10 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENIE0_EN11_Pos          (11UL)         /*!< EN11 (Bit 11)                                         */
#define R_ICU_GENIE0_EN11_Msk          (0x800UL)      /*!< EN11 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENIE0_EN12_Pos          (12UL)         /*!< EN12 (Bit 12)                                         */
#define R_ICU_GENIE0_EN12_Msk          (0x1000UL)     /*!< EN12 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENIE0_EN13_Pos          (13UL)         /*!< EN13 (Bit 13)                                         */
#define R_ICU_GENIE0_EN13_Msk          (0x2000UL)     /*!< EN13 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENIE0_EN14_Pos          (14UL)         /*!< EN14 (Bit 14)                                         */
#define R_ICU_GENIE0_EN14_Msk          (0x4000UL)     /*!< EN14 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENIE0_EN15_Pos          (15UL)         /*!< EN15 (Bit 15)                                         */
#define R_ICU_GENIE0_EN15_Msk          (0x8000UL)     /*!< EN15 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENIE0_EN16_Pos          (16UL)         /*!< EN16 (Bit 16)                                         */
#define R_ICU_GENIE0_EN16_Msk          (0x10000UL)    /*!< EN16 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENIE0_EN17_Pos          (17UL)         /*!< EN17 (Bit 17)                                         */
#define R_ICU_GENIE0_EN17_Msk          (0x20000UL)    /*!< EN17 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENIE0_EN18_Pos          (18UL)         /*!< EN18 (Bit 18)                                         */
#define R_ICU_GENIE0_EN18_Msk          (0x40000UL)    /*!< EN18 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENIE0_EN19_Pos          (19UL)         /*!< EN19 (Bit 19)                                         */
#define R_ICU_GENIE0_EN19_Msk          (0x80000UL)    /*!< EN19 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENIE0_EN20_Pos          (20UL)         /*!< EN20 (Bit 20)                                         */
#define R_ICU_GENIE0_EN20_Msk          (0x100000UL)   /*!< EN20 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENIE0_EN21_Pos          (21UL)         /*!< EN21 (Bit 21)                                         */
#define R_ICU_GENIE0_EN21_Msk          (0x200000UL)   /*!< EN21 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENIE0_EN22_Pos          (22UL)         /*!< EN22 (Bit 22)                                         */
#define R_ICU_GENIE0_EN22_Msk          (0x400000UL)   /*!< EN22 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENIE0_EN23_Pos          (23UL)         /*!< EN23 (Bit 23)                                         */
#define R_ICU_GENIE0_EN23_Msk          (0x800000UL)   /*!< EN23 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENIE0_EN24_Pos          (24UL)         /*!< EN24 (Bit 24)                                         */
#define R_ICU_GENIE0_EN24_Msk          (0x1000000UL)  /*!< EN24 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENIE0_EN25_Pos          (25UL)         /*!< EN25 (Bit 25)                                         */
#define R_ICU_GENIE0_EN25_Msk          (0x2000000UL)  /*!< EN25 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENIE0_EN26_Pos          (26UL)         /*!< EN26 (Bit 26)                                         */
#define R_ICU_GENIE0_EN26_Msk          (0x4000000UL)  /*!< EN26 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENIE0_EN27_Pos          (27UL)         /*!< EN27 (Bit 27)                                         */
#define R_ICU_GENIE0_EN27_Msk          (0x8000000UL)  /*!< EN27 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENIE0_EN28_Pos          (28UL)         /*!< EN28 (Bit 28)                                         */
#define R_ICU_GENIE0_EN28_Msk          (0x10000000UL) /*!< EN28 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENIE0_EN29_Pos          (29UL)         /*!< EN29 (Bit 29)                                         */
#define R_ICU_GENIE0_EN29_Msk          (0x20000000UL) /*!< EN29 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENIE0_EN30_Pos          (30UL)         /*!< EN30 (Bit 30)                                         */
#define R_ICU_GENIE0_EN30_Msk          (0x40000000UL) /*!< EN30 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENIE0_EN31_Pos          (31UL)         /*!< EN31 (Bit 31)                                         */
#define R_ICU_GENIE0_EN31_Msk          (0x80000000UL) /*!< EN31 (Bitfield-Mask: 0x01)                            */
/* ========================================================  GCRIE0  ========================================================= */
#define R_ICU_GCRIE0_CLR0_Pos          (0UL)          /*!< CLR0 (Bit 0)                                          */
#define R_ICU_GCRIE0_CLR0_Msk          (0x1UL)        /*!< CLR0 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GCRIE0_CLR1_Pos          (1UL)          /*!< CLR1 (Bit 1)                                          */
#define R_ICU_GCRIE0_CLR1_Msk          (0x2UL)        /*!< CLR1 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GCRIE0_CLR2_Pos          (2UL)          /*!< CLR2 (Bit 2)                                          */
#define R_ICU_GCRIE0_CLR2_Msk          (0x4UL)        /*!< CLR2 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GCRIE0_CLR3_Pos          (3UL)          /*!< CLR3 (Bit 3)                                          */
#define R_ICU_GCRIE0_CLR3_Msk          (0x8UL)        /*!< CLR3 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GCRIE0_CLR4_Pos          (4UL)          /*!< CLR4 (Bit 4)                                          */
#define R_ICU_GCRIE0_CLR4_Msk          (0x10UL)       /*!< CLR4 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GCRIE0_CLR5_Pos          (5UL)          /*!< CLR5 (Bit 5)                                          */
#define R_ICU_GCRIE0_CLR5_Msk          (0x20UL)       /*!< CLR5 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GCRIE0_CLR6_Pos          (6UL)          /*!< CLR6 (Bit 6)                                          */
#define R_ICU_GCRIE0_CLR6_Msk          (0x40UL)       /*!< CLR6 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GCRIE0_CLR7_Pos          (7UL)          /*!< CLR7 (Bit 7)                                          */
#define R_ICU_GCRIE0_CLR7_Msk          (0x80UL)       /*!< CLR7 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GCRIE0_CLR8_Pos          (8UL)          /*!< CLR8 (Bit 8)                                          */
#define R_ICU_GCRIE0_CLR8_Msk          (0x100UL)      /*!< CLR8 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GCRIE0_CLR9_Pos          (9UL)          /*!< CLR9 (Bit 9)                                          */
#define R_ICU_GCRIE0_CLR9_Msk          (0x200UL)      /*!< CLR9 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GCRIE0_CLR10_Pos         (10UL)         /*!< CLR10 (Bit 10)                                        */
#define R_ICU_GCRIE0_CLR10_Msk         (0x400UL)      /*!< CLR10 (Bitfield-Mask: 0x01)                           */
#define R_ICU_GCRIE0_CLR11_Pos         (11UL)         /*!< CLR11 (Bit 11)                                        */
#define R_ICU_GCRIE0_CLR11_Msk         (0x800UL)      /*!< CLR11 (Bitfield-Mask: 0x01)                           */
#define R_ICU_GCRIE0_CLR12_Pos         (12UL)         /*!< CLR12 (Bit 12)                                        */
#define R_ICU_GCRIE0_CLR12_Msk         (0x1000UL)     /*!< CLR12 (Bitfield-Mask: 0x01)                           */
#define R_ICU_GCRIE0_CLR13_Pos         (13UL)         /*!< CLR13 (Bit 13)                                        */
#define R_ICU_GCRIE0_CLR13_Msk         (0x2000UL)     /*!< CLR13 (Bitfield-Mask: 0x01)                           */
#define R_ICU_GCRIE0_CLR14_Pos         (14UL)         /*!< CLR14 (Bit 14)                                        */
#define R_ICU_GCRIE0_CLR14_Msk         (0x4000UL)     /*!< CLR14 (Bitfield-Mask: 0x01)                           */
#define R_ICU_GCRIE0_CLR15_Pos         (15UL)         /*!< CLR15 (Bit 15)                                        */
#define R_ICU_GCRIE0_CLR15_Msk         (0x8000UL)     /*!< CLR15 (Bitfield-Mask: 0x01)                           */
#define R_ICU_GCRIE0_CLR16_Pos         (16UL)         /*!< CLR16 (Bit 16)                                        */
#define R_ICU_GCRIE0_CLR16_Msk         (0x10000UL)    /*!< CLR16 (Bitfield-Mask: 0x01)                           */
#define R_ICU_GCRIE0_CLR17_Pos         (17UL)         /*!< CLR17 (Bit 17)                                        */
#define R_ICU_GCRIE0_CLR17_Msk         (0x20000UL)    /*!< CLR17 (Bitfield-Mask: 0x01)                           */
#define R_ICU_GCRIE0_CLR18_Pos         (18UL)         /*!< CLR18 (Bit 18)                                        */
#define R_ICU_GCRIE0_CLR18_Msk         (0x40000UL)    /*!< CLR18 (Bitfield-Mask: 0x01)                           */
#define R_ICU_GCRIE0_CLR19_Pos         (19UL)         /*!< CLR19 (Bit 19)                                        */
#define R_ICU_GCRIE0_CLR19_Msk         (0x80000UL)    /*!< CLR19 (Bitfield-Mask: 0x01)                           */
#define R_ICU_GCRIE0_CLR20_Pos         (20UL)         /*!< CLR20 (Bit 20)                                        */
#define R_ICU_GCRIE0_CLR20_Msk         (0x100000UL)   /*!< CLR20 (Bitfield-Mask: 0x01)                           */
#define R_ICU_GCRIE0_CLR21_Pos         (21UL)         /*!< CLR21 (Bit 21)                                        */
#define R_ICU_GCRIE0_CLR21_Msk         (0x200000UL)   /*!< CLR21 (Bitfield-Mask: 0x01)                           */
#define R_ICU_GCRIE0_CLR22_Pos         (22UL)         /*!< CLR22 (Bit 22)                                        */
#define R_ICU_GCRIE0_CLR22_Msk         (0x400000UL)   /*!< CLR22 (Bitfield-Mask: 0x01)                           */
#define R_ICU_GCRIE0_CLR23_Pos         (23UL)         /*!< CLR23 (Bit 23)                                        */
#define R_ICU_GCRIE0_CLR23_Msk         (0x800000UL)   /*!< CLR23 (Bitfield-Mask: 0x01)                           */
#define R_ICU_GCRIE0_CLR24_Pos         (24UL)         /*!< CLR24 (Bit 24)                                        */
#define R_ICU_GCRIE0_CLR24_Msk         (0x1000000UL)  /*!< CLR24 (Bitfield-Mask: 0x01)                           */
#define R_ICU_GCRIE0_CLR25_Pos         (25UL)         /*!< CLR25 (Bit 25)                                        */
#define R_ICU_GCRIE0_CLR25_Msk         (0x2000000UL)  /*!< CLR25 (Bitfield-Mask: 0x01)                           */
#define R_ICU_GCRIE0_CLR26_Pos         (26UL)         /*!< CLR26 (Bit 26)                                        */
#define R_ICU_GCRIE0_CLR26_Msk         (0x4000000UL)  /*!< CLR26 (Bitfield-Mask: 0x01)                           */
#define R_ICU_GCRIE0_CLR27_Pos         (27UL)         /*!< CLR27 (Bit 27)                                        */
#define R_ICU_GCRIE0_CLR27_Msk         (0x8000000UL)  /*!< CLR27 (Bitfield-Mask: 0x01)                           */
#define R_ICU_GCRIE0_CLR28_Pos         (28UL)         /*!< CLR28 (Bit 28)                                        */
#define R_ICU_GCRIE0_CLR28_Msk         (0x10000000UL) /*!< CLR28 (Bitfield-Mask: 0x01)                           */
#define R_ICU_GCRIE0_CLR29_Pos         (29UL)         /*!< CLR29 (Bit 29)                                        */
#define R_ICU_GCRIE0_CLR29_Msk         (0x20000000UL) /*!< CLR29 (Bitfield-Mask: 0x01)                           */
#define R_ICU_GCRIE0_CLR30_Pos         (30UL)         /*!< CLR30 (Bit 30)                                        */
#define R_ICU_GCRIE0_CLR30_Msk         (0x40000000UL) /*!< CLR30 (Bitfield-Mask: 0x01)                           */
#define R_ICU_GCRIE0_CLR31_Pos         (31UL)         /*!< CLR31 (Bit 31)                                        */
#define R_ICU_GCRIE0_CLR31_Msk         (0x80000000UL) /*!< CLR31 (Bitfield-Mask: 0x01)                           */
/* ========================================================  GRPBE0  ========================================================= */
#define R_ICU_GRPBE0_IS0_Pos           (0UL)          /*!< IS0 (Bit 0)                                           */
#define R_ICU_GRPBE0_IS0_Msk           (0x1UL)        /*!< IS0 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPBE0_IS1_Pos           (1UL)          /*!< IS1 (Bit 1)                                           */
#define R_ICU_GRPBE0_IS1_Msk           (0x2UL)        /*!< IS1 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPBE0_IS2_Pos           (2UL)          /*!< IS2 (Bit 2)                                           */
#define R_ICU_GRPBE0_IS2_Msk           (0x4UL)        /*!< IS2 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPBE0_IS3_Pos           (3UL)          /*!< IS3 (Bit 3)                                           */
#define R_ICU_GRPBE0_IS3_Msk           (0x8UL)        /*!< IS3 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPBE0_IS4_Pos           (4UL)          /*!< IS4 (Bit 4)                                           */
#define R_ICU_GRPBE0_IS4_Msk           (0x10UL)       /*!< IS4 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPBE0_IS5_Pos           (5UL)          /*!< IS5 (Bit 5)                                           */
#define R_ICU_GRPBE0_IS5_Msk           (0x20UL)       /*!< IS5 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPBE0_IS6_Pos           (6UL)          /*!< IS6 (Bit 6)                                           */
#define R_ICU_GRPBE0_IS6_Msk           (0x40UL)       /*!< IS6 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPBE0_IS7_Pos           (7UL)          /*!< IS7 (Bit 7)                                           */
#define R_ICU_GRPBE0_IS7_Msk           (0x80UL)       /*!< IS7 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPBE0_IS8_Pos           (8UL)          /*!< IS8 (Bit 8)                                           */
#define R_ICU_GRPBE0_IS8_Msk           (0x100UL)      /*!< IS8 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPBE0_IS9_Pos           (9UL)          /*!< IS9 (Bit 9)                                           */
#define R_ICU_GRPBE0_IS9_Msk           (0x200UL)      /*!< IS9 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPBE0_IS10_Pos          (10UL)         /*!< IS10 (Bit 10)                                         */
#define R_ICU_GRPBE0_IS10_Msk          (0x400UL)      /*!< IS10 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBE0_IS11_Pos          (11UL)         /*!< IS11 (Bit 11)                                         */
#define R_ICU_GRPBE0_IS11_Msk          (0x800UL)      /*!< IS11 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBE0_IS12_Pos          (12UL)         /*!< IS12 (Bit 12)                                         */
#define R_ICU_GRPBE0_IS12_Msk          (0x1000UL)     /*!< IS12 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBE0_IS13_Pos          (13UL)         /*!< IS13 (Bit 13)                                         */
#define R_ICU_GRPBE0_IS13_Msk          (0x2000UL)     /*!< IS13 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBE0_IS14_Pos          (14UL)         /*!< IS14 (Bit 14)                                         */
#define R_ICU_GRPBE0_IS14_Msk          (0x4000UL)     /*!< IS14 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBE0_IS15_Pos          (15UL)         /*!< IS15 (Bit 15)                                         */
#define R_ICU_GRPBE0_IS15_Msk          (0x8000UL)     /*!< IS15 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBE0_IS16_Pos          (16UL)         /*!< IS16 (Bit 16)                                         */
#define R_ICU_GRPBE0_IS16_Msk          (0x10000UL)    /*!< IS16 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBE0_IS17_Pos          (17UL)         /*!< IS17 (Bit 17)                                         */
#define R_ICU_GRPBE0_IS17_Msk          (0x20000UL)    /*!< IS17 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBE0_IS18_Pos          (18UL)         /*!< IS18 (Bit 18)                                         */
#define R_ICU_GRPBE0_IS18_Msk          (0x40000UL)    /*!< IS18 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBE0_IS19_Pos          (19UL)         /*!< IS19 (Bit 19)                                         */
#define R_ICU_GRPBE0_IS19_Msk          (0x80000UL)    /*!< IS19 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBE0_IS20_Pos          (20UL)         /*!< IS20 (Bit 20)                                         */
#define R_ICU_GRPBE0_IS20_Msk          (0x100000UL)   /*!< IS20 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBE0_IS21_Pos          (21UL)         /*!< IS21 (Bit 21)                                         */
#define R_ICU_GRPBE0_IS21_Msk          (0x200000UL)   /*!< IS21 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBE0_IS22_Pos          (22UL)         /*!< IS22 (Bit 22)                                         */
#define R_ICU_GRPBE0_IS22_Msk          (0x400000UL)   /*!< IS22 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBE0_IS23_Pos          (23UL)         /*!< IS23 (Bit 23)                                         */
#define R_ICU_GRPBE0_IS23_Msk          (0x800000UL)   /*!< IS23 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBE0_IS24_Pos          (24UL)         /*!< IS24 (Bit 24)                                         */
#define R_ICU_GRPBE0_IS24_Msk          (0x1000000UL)  /*!< IS24 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBE0_IS25_Pos          (25UL)         /*!< IS25 (Bit 25)                                         */
#define R_ICU_GRPBE0_IS25_Msk          (0x2000000UL)  /*!< IS25 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBE0_IS26_Pos          (26UL)         /*!< IS26 (Bit 26)                                         */
#define R_ICU_GRPBE0_IS26_Msk          (0x4000000UL)  /*!< IS26 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBE0_IS27_Pos          (27UL)         /*!< IS27 (Bit 27)                                         */
#define R_ICU_GRPBE0_IS27_Msk          (0x8000000UL)  /*!< IS27 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBE0_IS28_Pos          (28UL)         /*!< IS28 (Bit 28)                                         */
#define R_ICU_GRPBE0_IS28_Msk          (0x10000000UL) /*!< IS28 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBE0_IS29_Pos          (29UL)         /*!< IS29 (Bit 29)                                         */
#define R_ICU_GRPBE0_IS29_Msk          (0x20000000UL) /*!< IS29 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBE0_IS30_Pos          (30UL)         /*!< IS30 (Bit 30)                                         */
#define R_ICU_GRPBE0_IS30_Msk          (0x40000000UL) /*!< IS30 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBE0_IS31_Pos          (31UL)         /*!< IS31 (Bit 31)                                         */
#define R_ICU_GRPBE0_IS31_Msk          (0x80000000UL) /*!< IS31 (Bitfield-Mask: 0x01)                            */
/* ========================================================  GRPBL0  ========================================================= */
#define R_ICU_GRPBL0_IS0_Pos           (0UL)          /*!< IS0 (Bit 0)                                           */
#define R_ICU_GRPBL0_IS0_Msk           (0x1UL)        /*!< IS0 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPBL0_IS1_Pos           (1UL)          /*!< IS1 (Bit 1)                                           */
#define R_ICU_GRPBL0_IS1_Msk           (0x2UL)        /*!< IS1 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPBL0_IS2_Pos           (2UL)          /*!< IS2 (Bit 2)                                           */
#define R_ICU_GRPBL0_IS2_Msk           (0x4UL)        /*!< IS2 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPBL0_IS3_Pos           (3UL)          /*!< IS3 (Bit 3)                                           */
#define R_ICU_GRPBL0_IS3_Msk           (0x8UL)        /*!< IS3 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPBL0_IS4_Pos           (4UL)          /*!< IS4 (Bit 4)                                           */
#define R_ICU_GRPBL0_IS4_Msk           (0x10UL)       /*!< IS4 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPBL0_IS5_Pos           (5UL)          /*!< IS5 (Bit 5)                                           */
#define R_ICU_GRPBL0_IS5_Msk           (0x20UL)       /*!< IS5 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPBL0_IS6_Pos           (6UL)          /*!< IS6 (Bit 6)                                           */
#define R_ICU_GRPBL0_IS6_Msk           (0x40UL)       /*!< IS6 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPBL0_IS7_Pos           (7UL)          /*!< IS7 (Bit 7)                                           */
#define R_ICU_GRPBL0_IS7_Msk           (0x80UL)       /*!< IS7 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPBL0_IS8_Pos           (8UL)          /*!< IS8 (Bit 8)                                           */
#define R_ICU_GRPBL0_IS8_Msk           (0x100UL)      /*!< IS8 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPBL0_IS9_Pos           (9UL)          /*!< IS9 (Bit 9)                                           */
#define R_ICU_GRPBL0_IS9_Msk           (0x200UL)      /*!< IS9 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPBL0_IS10_Pos          (10UL)         /*!< IS10 (Bit 10)                                         */
#define R_ICU_GRPBL0_IS10_Msk          (0x400UL)      /*!< IS10 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL0_IS11_Pos          (11UL)         /*!< IS11 (Bit 11)                                         */
#define R_ICU_GRPBL0_IS11_Msk          (0x800UL)      /*!< IS11 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL0_IS12_Pos          (12UL)         /*!< IS12 (Bit 12)                                         */
#define R_ICU_GRPBL0_IS12_Msk          (0x1000UL)     /*!< IS12 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL0_IS13_Pos          (13UL)         /*!< IS13 (Bit 13)                                         */
#define R_ICU_GRPBL0_IS13_Msk          (0x2000UL)     /*!< IS13 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL0_IS14_Pos          (14UL)         /*!< IS14 (Bit 14)                                         */
#define R_ICU_GRPBL0_IS14_Msk          (0x4000UL)     /*!< IS14 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL0_IS15_Pos          (15UL)         /*!< IS15 (Bit 15)                                         */
#define R_ICU_GRPBL0_IS15_Msk          (0x8000UL)     /*!< IS15 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL0_IS16_Pos          (16UL)         /*!< IS16 (Bit 16)                                         */
#define R_ICU_GRPBL0_IS16_Msk          (0x10000UL)    /*!< IS16 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL0_IS17_Pos          (17UL)         /*!< IS17 (Bit 17)                                         */
#define R_ICU_GRPBL0_IS17_Msk          (0x20000UL)    /*!< IS17 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL0_IS18_Pos          (18UL)         /*!< IS18 (Bit 18)                                         */
#define R_ICU_GRPBL0_IS18_Msk          (0x40000UL)    /*!< IS18 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL0_IS19_Pos          (19UL)         /*!< IS19 (Bit 19)                                         */
#define R_ICU_GRPBL0_IS19_Msk          (0x80000UL)    /*!< IS19 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL0_IS20_Pos          (20UL)         /*!< IS20 (Bit 20)                                         */
#define R_ICU_GRPBL0_IS20_Msk          (0x100000UL)   /*!< IS20 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL0_IS21_Pos          (21UL)         /*!< IS21 (Bit 21)                                         */
#define R_ICU_GRPBL0_IS21_Msk          (0x200000UL)   /*!< IS21 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL0_IS22_Pos          (22UL)         /*!< IS22 (Bit 22)                                         */
#define R_ICU_GRPBL0_IS22_Msk          (0x400000UL)   /*!< IS22 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL0_IS23_Pos          (23UL)         /*!< IS23 (Bit 23)                                         */
#define R_ICU_GRPBL0_IS23_Msk          (0x800000UL)   /*!< IS23 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL0_IS24_Pos          (24UL)         /*!< IS24 (Bit 24)                                         */
#define R_ICU_GRPBL0_IS24_Msk          (0x1000000UL)  /*!< IS24 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL0_IS25_Pos          (25UL)         /*!< IS25 (Bit 25)                                         */
#define R_ICU_GRPBL0_IS25_Msk          (0x2000000UL)  /*!< IS25 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL0_IS26_Pos          (26UL)         /*!< IS26 (Bit 26)                                         */
#define R_ICU_GRPBL0_IS26_Msk          (0x4000000UL)  /*!< IS26 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL0_IS27_Pos          (27UL)         /*!< IS27 (Bit 27)                                         */
#define R_ICU_GRPBL0_IS27_Msk          (0x8000000UL)  /*!< IS27 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL0_IS28_Pos          (28UL)         /*!< IS28 (Bit 28)                                         */
#define R_ICU_GRPBL0_IS28_Msk          (0x10000000UL) /*!< IS28 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL0_IS29_Pos          (29UL)         /*!< IS29 (Bit 29)                                         */
#define R_ICU_GRPBL0_IS29_Msk          (0x20000000UL) /*!< IS29 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL0_IS30_Pos          (30UL)         /*!< IS30 (Bit 30)                                         */
#define R_ICU_GRPBL0_IS30_Msk          (0x40000000UL) /*!< IS30 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL0_IS31_Pos          (31UL)         /*!< IS31 (Bit 31)                                         */
#define R_ICU_GRPBL0_IS31_Msk          (0x80000000UL) /*!< IS31 (Bitfield-Mask: 0x01)                            */
/* ========================================================  GRPBL1  ========================================================= */
#define R_ICU_GRPBL1_IS0_Pos           (0UL)          /*!< IS0 (Bit 0)                                           */
#define R_ICU_GRPBL1_IS0_Msk           (0x1UL)        /*!< IS0 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPBL1_IS1_Pos           (1UL)          /*!< IS1 (Bit 1)                                           */
#define R_ICU_GRPBL1_IS1_Msk           (0x2UL)        /*!< IS1 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPBL1_IS2_Pos           (2UL)          /*!< IS2 (Bit 2)                                           */
#define R_ICU_GRPBL1_IS2_Msk           (0x4UL)        /*!< IS2 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPBL1_IS3_Pos           (3UL)          /*!< IS3 (Bit 3)                                           */
#define R_ICU_GRPBL1_IS3_Msk           (0x8UL)        /*!< IS3 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPBL1_IS4_Pos           (4UL)          /*!< IS4 (Bit 4)                                           */
#define R_ICU_GRPBL1_IS4_Msk           (0x10UL)       /*!< IS4 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPBL1_IS5_Pos           (5UL)          /*!< IS5 (Bit 5)                                           */
#define R_ICU_GRPBL1_IS5_Msk           (0x20UL)       /*!< IS5 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPBL1_IS6_Pos           (6UL)          /*!< IS6 (Bit 6)                                           */
#define R_ICU_GRPBL1_IS6_Msk           (0x40UL)       /*!< IS6 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPBL1_IS7_Pos           (7UL)          /*!< IS7 (Bit 7)                                           */
#define R_ICU_GRPBL1_IS7_Msk           (0x80UL)       /*!< IS7 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPBL1_IS8_Pos           (8UL)          /*!< IS8 (Bit 8)                                           */
#define R_ICU_GRPBL1_IS8_Msk           (0x100UL)      /*!< IS8 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPBL1_IS9_Pos           (9UL)          /*!< IS9 (Bit 9)                                           */
#define R_ICU_GRPBL1_IS9_Msk           (0x200UL)      /*!< IS9 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPBL1_IS10_Pos          (10UL)         /*!< IS10 (Bit 10)                                         */
#define R_ICU_GRPBL1_IS10_Msk          (0x400UL)      /*!< IS10 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL1_IS11_Pos          (11UL)         /*!< IS11 (Bit 11)                                         */
#define R_ICU_GRPBL1_IS11_Msk          (0x800UL)      /*!< IS11 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL1_IS12_Pos          (12UL)         /*!< IS12 (Bit 12)                                         */
#define R_ICU_GRPBL1_IS12_Msk          (0x1000UL)     /*!< IS12 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL1_IS13_Pos          (13UL)         /*!< IS13 (Bit 13)                                         */
#define R_ICU_GRPBL1_IS13_Msk          (0x2000UL)     /*!< IS13 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL1_IS14_Pos          (14UL)         /*!< IS14 (Bit 14)                                         */
#define R_ICU_GRPBL1_IS14_Msk          (0x4000UL)     /*!< IS14 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL1_IS15_Pos          (15UL)         /*!< IS15 (Bit 15)                                         */
#define R_ICU_GRPBL1_IS15_Msk          (0x8000UL)     /*!< IS15 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL1_IS16_Pos          (16UL)         /*!< IS16 (Bit 16)                                         */
#define R_ICU_GRPBL1_IS16_Msk          (0x10000UL)    /*!< IS16 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL1_IS17_Pos          (17UL)         /*!< IS17 (Bit 17)                                         */
#define R_ICU_GRPBL1_IS17_Msk          (0x20000UL)    /*!< IS17 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL1_IS18_Pos          (18UL)         /*!< IS18 (Bit 18)                                         */
#define R_ICU_GRPBL1_IS18_Msk          (0x40000UL)    /*!< IS18 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL1_IS19_Pos          (19UL)         /*!< IS19 (Bit 19)                                         */
#define R_ICU_GRPBL1_IS19_Msk          (0x80000UL)    /*!< IS19 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL1_IS20_Pos          (20UL)         /*!< IS20 (Bit 20)                                         */
#define R_ICU_GRPBL1_IS20_Msk          (0x100000UL)   /*!< IS20 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL1_IS21_Pos          (21UL)         /*!< IS21 (Bit 21)                                         */
#define R_ICU_GRPBL1_IS21_Msk          (0x200000UL)   /*!< IS21 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL1_IS22_Pos          (22UL)         /*!< IS22 (Bit 22)                                         */
#define R_ICU_GRPBL1_IS22_Msk          (0x400000UL)   /*!< IS22 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL1_IS23_Pos          (23UL)         /*!< IS23 (Bit 23)                                         */
#define R_ICU_GRPBL1_IS23_Msk          (0x800000UL)   /*!< IS23 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL1_IS24_Pos          (24UL)         /*!< IS24 (Bit 24)                                         */
#define R_ICU_GRPBL1_IS24_Msk          (0x1000000UL)  /*!< IS24 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL1_IS25_Pos          (25UL)         /*!< IS25 (Bit 25)                                         */
#define R_ICU_GRPBL1_IS25_Msk          (0x2000000UL)  /*!< IS25 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL1_IS26_Pos          (26UL)         /*!< IS26 (Bit 26)                                         */
#define R_ICU_GRPBL1_IS26_Msk          (0x4000000UL)  /*!< IS26 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL1_IS27_Pos          (27UL)         /*!< IS27 (Bit 27)                                         */
#define R_ICU_GRPBL1_IS27_Msk          (0x8000000UL)  /*!< IS27 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL1_IS28_Pos          (28UL)         /*!< IS28 (Bit 28)                                         */
#define R_ICU_GRPBL1_IS28_Msk          (0x10000000UL) /*!< IS28 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL1_IS29_Pos          (29UL)         /*!< IS29 (Bit 29)                                         */
#define R_ICU_GRPBL1_IS29_Msk          (0x20000000UL) /*!< IS29 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL1_IS30_Pos          (30UL)         /*!< IS30 (Bit 30)                                         */
#define R_ICU_GRPBL1_IS30_Msk          (0x40000000UL) /*!< IS30 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL1_IS31_Pos          (31UL)         /*!< IS31 (Bit 31)                                         */
#define R_ICU_GRPBL1_IS31_Msk          (0x80000000UL) /*!< IS31 (Bitfield-Mask: 0x01)                            */
/* ========================================================  GRPBL2  ========================================================= */
#define R_ICU_GRPBL2_IS0_Pos           (0UL)          /*!< IS0 (Bit 0)                                           */
#define R_ICU_GRPBL2_IS0_Msk           (0x1UL)        /*!< IS0 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPBL2_IS1_Pos           (1UL)          /*!< IS1 (Bit 1)                                           */
#define R_ICU_GRPBL2_IS1_Msk           (0x2UL)        /*!< IS1 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPBL2_IS2_Pos           (2UL)          /*!< IS2 (Bit 2)                                           */
#define R_ICU_GRPBL2_IS2_Msk           (0x4UL)        /*!< IS2 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPBL2_IS3_Pos           (3UL)          /*!< IS3 (Bit 3)                                           */
#define R_ICU_GRPBL2_IS3_Msk           (0x8UL)        /*!< IS3 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPBL2_IS4_Pos           (4UL)          /*!< IS4 (Bit 4)                                           */
#define R_ICU_GRPBL2_IS4_Msk           (0x10UL)       /*!< IS4 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPBL2_IS5_Pos           (5UL)          /*!< IS5 (Bit 5)                                           */
#define R_ICU_GRPBL2_IS5_Msk           (0x20UL)       /*!< IS5 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPBL2_IS6_Pos           (6UL)          /*!< IS6 (Bit 6)                                           */
#define R_ICU_GRPBL2_IS6_Msk           (0x40UL)       /*!< IS6 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPBL2_IS7_Pos           (7UL)          /*!< IS7 (Bit 7)                                           */
#define R_ICU_GRPBL2_IS7_Msk           (0x80UL)       /*!< IS7 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPBL2_IS8_Pos           (8UL)          /*!< IS8 (Bit 8)                                           */
#define R_ICU_GRPBL2_IS8_Msk           (0x100UL)      /*!< IS8 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPBL2_IS9_Pos           (9UL)          /*!< IS9 (Bit 9)                                           */
#define R_ICU_GRPBL2_IS9_Msk           (0x200UL)      /*!< IS9 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPBL2_IS10_Pos          (10UL)         /*!< IS10 (Bit 10)                                         */
#define R_ICU_GRPBL2_IS10_Msk          (0x400UL)      /*!< IS10 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL2_IS11_Pos          (11UL)         /*!< IS11 (Bit 11)                                         */
#define R_ICU_GRPBL2_IS11_Msk          (0x800UL)      /*!< IS11 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL2_IS12_Pos          (12UL)         /*!< IS12 (Bit 12)                                         */
#define R_ICU_GRPBL2_IS12_Msk          (0x1000UL)     /*!< IS12 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL2_IS13_Pos          (13UL)         /*!< IS13 (Bit 13)                                         */
#define R_ICU_GRPBL2_IS13_Msk          (0x2000UL)     /*!< IS13 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL2_IS14_Pos          (14UL)         /*!< IS14 (Bit 14)                                         */
#define R_ICU_GRPBL2_IS14_Msk          (0x4000UL)     /*!< IS14 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL2_IS15_Pos          (15UL)         /*!< IS15 (Bit 15)                                         */
#define R_ICU_GRPBL2_IS15_Msk          (0x8000UL)     /*!< IS15 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL2_IS16_Pos          (16UL)         /*!< IS16 (Bit 16)                                         */
#define R_ICU_GRPBL2_IS16_Msk          (0x10000UL)    /*!< IS16 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL2_IS17_Pos          (17UL)         /*!< IS17 (Bit 17)                                         */
#define R_ICU_GRPBL2_IS17_Msk          (0x20000UL)    /*!< IS17 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL2_IS18_Pos          (18UL)         /*!< IS18 (Bit 18)                                         */
#define R_ICU_GRPBL2_IS18_Msk          (0x40000UL)    /*!< IS18 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL2_IS19_Pos          (19UL)         /*!< IS19 (Bit 19)                                         */
#define R_ICU_GRPBL2_IS19_Msk          (0x80000UL)    /*!< IS19 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL2_IS20_Pos          (20UL)         /*!< IS20 (Bit 20)                                         */
#define R_ICU_GRPBL2_IS20_Msk          (0x100000UL)   /*!< IS20 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL2_IS21_Pos          (21UL)         /*!< IS21 (Bit 21)                                         */
#define R_ICU_GRPBL2_IS21_Msk          (0x200000UL)   /*!< IS21 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL2_IS22_Pos          (22UL)         /*!< IS22 (Bit 22)                                         */
#define R_ICU_GRPBL2_IS22_Msk          (0x400000UL)   /*!< IS22 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL2_IS23_Pos          (23UL)         /*!< IS23 (Bit 23)                                         */
#define R_ICU_GRPBL2_IS23_Msk          (0x800000UL)   /*!< IS23 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL2_IS24_Pos          (24UL)         /*!< IS24 (Bit 24)                                         */
#define R_ICU_GRPBL2_IS24_Msk          (0x1000000UL)  /*!< IS24 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL2_IS25_Pos          (25UL)         /*!< IS25 (Bit 25)                                         */
#define R_ICU_GRPBL2_IS25_Msk          (0x2000000UL)  /*!< IS25 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL2_IS26_Pos          (26UL)         /*!< IS26 (Bit 26)                                         */
#define R_ICU_GRPBL2_IS26_Msk          (0x4000000UL)  /*!< IS26 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL2_IS27_Pos          (27UL)         /*!< IS27 (Bit 27)                                         */
#define R_ICU_GRPBL2_IS27_Msk          (0x8000000UL)  /*!< IS27 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL2_IS28_Pos          (28UL)         /*!< IS28 (Bit 28)                                         */
#define R_ICU_GRPBL2_IS28_Msk          (0x10000000UL) /*!< IS28 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL2_IS29_Pos          (29UL)         /*!< IS29 (Bit 29)                                         */
#define R_ICU_GRPBL2_IS29_Msk          (0x20000000UL) /*!< IS29 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL2_IS30_Pos          (30UL)         /*!< IS30 (Bit 30)                                         */
#define R_ICU_GRPBL2_IS30_Msk          (0x40000000UL) /*!< IS30 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPBL2_IS31_Pos          (31UL)         /*!< IS31 (Bit 31)                                         */
#define R_ICU_GRPBL2_IS31_Msk          (0x80000000UL) /*!< IS31 (Bitfield-Mask: 0x01)                            */
/* ========================================================  GENBE0  ========================================================= */
#define R_ICU_GENBE0_EN0_Pos           (0UL)          /*!< EN0 (Bit 0)                                           */
#define R_ICU_GENBE0_EN0_Msk           (0x1UL)        /*!< EN0 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENBE0_EN1_Pos           (1UL)          /*!< EN1 (Bit 1)                                           */
#define R_ICU_GENBE0_EN1_Msk           (0x2UL)        /*!< EN1 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENBE0_EN2_Pos           (2UL)          /*!< EN2 (Bit 2)                                           */
#define R_ICU_GENBE0_EN2_Msk           (0x4UL)        /*!< EN2 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENBE0_EN3_Pos           (3UL)          /*!< EN3 (Bit 3)                                           */
#define R_ICU_GENBE0_EN3_Msk           (0x8UL)        /*!< EN3 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENBE0_EN4_Pos           (4UL)          /*!< EN4 (Bit 4)                                           */
#define R_ICU_GENBE0_EN4_Msk           (0x10UL)       /*!< EN4 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENBE0_EN5_Pos           (5UL)          /*!< EN5 (Bit 5)                                           */
#define R_ICU_GENBE0_EN5_Msk           (0x20UL)       /*!< EN5 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENBE0_EN6_Pos           (6UL)          /*!< EN6 (Bit 6)                                           */
#define R_ICU_GENBE0_EN6_Msk           (0x40UL)       /*!< EN6 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENBE0_EN7_Pos           (7UL)          /*!< EN7 (Bit 7)                                           */
#define R_ICU_GENBE0_EN7_Msk           (0x80UL)       /*!< EN7 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENBE0_EN8_Pos           (8UL)          /*!< EN8 (Bit 8)                                           */
#define R_ICU_GENBE0_EN8_Msk           (0x100UL)      /*!< EN8 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENBE0_EN9_Pos           (9UL)          /*!< EN9 (Bit 9)                                           */
#define R_ICU_GENBE0_EN9_Msk           (0x200UL)      /*!< EN9 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENBE0_EN10_Pos          (10UL)         /*!< EN10 (Bit 10)                                         */
#define R_ICU_GENBE0_EN10_Msk          (0x400UL)      /*!< EN10 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBE0_EN11_Pos          (11UL)         /*!< EN11 (Bit 11)                                         */
#define R_ICU_GENBE0_EN11_Msk          (0x800UL)      /*!< EN11 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBE0_EN12_Pos          (12UL)         /*!< EN12 (Bit 12)                                         */
#define R_ICU_GENBE0_EN12_Msk          (0x1000UL)     /*!< EN12 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBE0_EN13_Pos          (13UL)         /*!< EN13 (Bit 13)                                         */
#define R_ICU_GENBE0_EN13_Msk          (0x2000UL)     /*!< EN13 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBE0_EN14_Pos          (14UL)         /*!< EN14 (Bit 14)                                         */
#define R_ICU_GENBE0_EN14_Msk          (0x4000UL)     /*!< EN14 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBE0_EN15_Pos          (15UL)         /*!< EN15 (Bit 15)                                         */
#define R_ICU_GENBE0_EN15_Msk          (0x8000UL)     /*!< EN15 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBE0_EN16_Pos          (16UL)         /*!< EN16 (Bit 16)                                         */
#define R_ICU_GENBE0_EN16_Msk          (0x10000UL)    /*!< EN16 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBE0_EN17_Pos          (17UL)         /*!< EN17 (Bit 17)                                         */
#define R_ICU_GENBE0_EN17_Msk          (0x20000UL)    /*!< EN17 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBE0_EN18_Pos          (18UL)         /*!< EN18 (Bit 18)                                         */
#define R_ICU_GENBE0_EN18_Msk          (0x40000UL)    /*!< EN18 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBE0_EN19_Pos          (19UL)         /*!< EN19 (Bit 19)                                         */
#define R_ICU_GENBE0_EN19_Msk          (0x80000UL)    /*!< EN19 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBE0_EN20_Pos          (20UL)         /*!< EN20 (Bit 20)                                         */
#define R_ICU_GENBE0_EN20_Msk          (0x100000UL)   /*!< EN20 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBE0_EN21_Pos          (21UL)         /*!< EN21 (Bit 21)                                         */
#define R_ICU_GENBE0_EN21_Msk          (0x200000UL)   /*!< EN21 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBE0_EN22_Pos          (22UL)         /*!< EN22 (Bit 22)                                         */
#define R_ICU_GENBE0_EN22_Msk          (0x400000UL)   /*!< EN22 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBE0_EN23_Pos          (23UL)         /*!< EN23 (Bit 23)                                         */
#define R_ICU_GENBE0_EN23_Msk          (0x800000UL)   /*!< EN23 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBE0_EN24_Pos          (24UL)         /*!< EN24 (Bit 24)                                         */
#define R_ICU_GENBE0_EN24_Msk          (0x1000000UL)  /*!< EN24 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBE0_EN25_Pos          (25UL)         /*!< EN25 (Bit 25)                                         */
#define R_ICU_GENBE0_EN25_Msk          (0x2000000UL)  /*!< EN25 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBE0_EN26_Pos          (26UL)         /*!< EN26 (Bit 26)                                         */
#define R_ICU_GENBE0_EN26_Msk          (0x4000000UL)  /*!< EN26 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBE0_EN27_Pos          (27UL)         /*!< EN27 (Bit 27)                                         */
#define R_ICU_GENBE0_EN27_Msk          (0x8000000UL)  /*!< EN27 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBE0_EN28_Pos          (28UL)         /*!< EN28 (Bit 28)                                         */
#define R_ICU_GENBE0_EN28_Msk          (0x10000000UL) /*!< EN28 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBE0_EN29_Pos          (29UL)         /*!< EN29 (Bit 29)                                         */
#define R_ICU_GENBE0_EN29_Msk          (0x20000000UL) /*!< EN29 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBE0_EN30_Pos          (30UL)         /*!< EN30 (Bit 30)                                         */
#define R_ICU_GENBE0_EN30_Msk          (0x40000000UL) /*!< EN30 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBE0_EN31_Pos          (31UL)         /*!< EN31 (Bit 31)                                         */
#define R_ICU_GENBE0_EN31_Msk          (0x80000000UL) /*!< EN31 (Bitfield-Mask: 0x01)                            */
/* ========================================================  GENBL0  ========================================================= */
#define R_ICU_GENBL0_EN0_Pos           (0UL)          /*!< EN0 (Bit 0)                                           */
#define R_ICU_GENBL0_EN0_Msk           (0x1UL)        /*!< EN0 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENBL0_EN1_Pos           (1UL)          /*!< EN1 (Bit 1)                                           */
#define R_ICU_GENBL0_EN1_Msk           (0x2UL)        /*!< EN1 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENBL0_EN2_Pos           (2UL)          /*!< EN2 (Bit 2)                                           */
#define R_ICU_GENBL0_EN2_Msk           (0x4UL)        /*!< EN2 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENBL0_EN3_Pos           (3UL)          /*!< EN3 (Bit 3)                                           */
#define R_ICU_GENBL0_EN3_Msk           (0x8UL)        /*!< EN3 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENBL0_EN4_Pos           (4UL)          /*!< EN4 (Bit 4)                                           */
#define R_ICU_GENBL0_EN4_Msk           (0x10UL)       /*!< EN4 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENBL0_EN5_Pos           (5UL)          /*!< EN5 (Bit 5)                                           */
#define R_ICU_GENBL0_EN5_Msk           (0x20UL)       /*!< EN5 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENBL0_EN6_Pos           (6UL)          /*!< EN6 (Bit 6)                                           */
#define R_ICU_GENBL0_EN6_Msk           (0x40UL)       /*!< EN6 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENBL0_EN7_Pos           (7UL)          /*!< EN7 (Bit 7)                                           */
#define R_ICU_GENBL0_EN7_Msk           (0x80UL)       /*!< EN7 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENBL0_EN8_Pos           (8UL)          /*!< EN8 (Bit 8)                                           */
#define R_ICU_GENBL0_EN8_Msk           (0x100UL)      /*!< EN8 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENBL0_EN9_Pos           (9UL)          /*!< EN9 (Bit 9)                                           */
#define R_ICU_GENBL0_EN9_Msk           (0x200UL)      /*!< EN9 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENBL0_EN10_Pos          (10UL)         /*!< EN10 (Bit 10)                                         */
#define R_ICU_GENBL0_EN10_Msk          (0x400UL)      /*!< EN10 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL0_EN11_Pos          (11UL)         /*!< EN11 (Bit 11)                                         */
#define R_ICU_GENBL0_EN11_Msk          (0x800UL)      /*!< EN11 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL0_EN12_Pos          (12UL)         /*!< EN12 (Bit 12)                                         */
#define R_ICU_GENBL0_EN12_Msk          (0x1000UL)     /*!< EN12 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL0_EN13_Pos          (13UL)         /*!< EN13 (Bit 13)                                         */
#define R_ICU_GENBL0_EN13_Msk          (0x2000UL)     /*!< EN13 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL0_EN14_Pos          (14UL)         /*!< EN14 (Bit 14)                                         */
#define R_ICU_GENBL0_EN14_Msk          (0x4000UL)     /*!< EN14 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL0_EN15_Pos          (15UL)         /*!< EN15 (Bit 15)                                         */
#define R_ICU_GENBL0_EN15_Msk          (0x8000UL)     /*!< EN15 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL0_EN16_Pos          (16UL)         /*!< EN16 (Bit 16)                                         */
#define R_ICU_GENBL0_EN16_Msk          (0x10000UL)    /*!< EN16 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL0_EN17_Pos          (17UL)         /*!< EN17 (Bit 17)                                         */
#define R_ICU_GENBL0_EN17_Msk          (0x20000UL)    /*!< EN17 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL0_EN18_Pos          (18UL)         /*!< EN18 (Bit 18)                                         */
#define R_ICU_GENBL0_EN18_Msk          (0x40000UL)    /*!< EN18 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL0_EN19_Pos          (19UL)         /*!< EN19 (Bit 19)                                         */
#define R_ICU_GENBL0_EN19_Msk          (0x80000UL)    /*!< EN19 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL0_EN20_Pos          (20UL)         /*!< EN20 (Bit 20)                                         */
#define R_ICU_GENBL0_EN20_Msk          (0x100000UL)   /*!< EN20 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL0_EN21_Pos          (21UL)         /*!< EN21 (Bit 21)                                         */
#define R_ICU_GENBL0_EN21_Msk          (0x200000UL)   /*!< EN21 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL0_EN22_Pos          (22UL)         /*!< EN22 (Bit 22)                                         */
#define R_ICU_GENBL0_EN22_Msk          (0x400000UL)   /*!< EN22 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL0_EN23_Pos          (23UL)         /*!< EN23 (Bit 23)                                         */
#define R_ICU_GENBL0_EN23_Msk          (0x800000UL)   /*!< EN23 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL0_EN24_Pos          (24UL)         /*!< EN24 (Bit 24)                                         */
#define R_ICU_GENBL0_EN24_Msk          (0x1000000UL)  /*!< EN24 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL0_EN25_Pos          (25UL)         /*!< EN25 (Bit 25)                                         */
#define R_ICU_GENBL0_EN25_Msk          (0x2000000UL)  /*!< EN25 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL0_EN26_Pos          (26UL)         /*!< EN26 (Bit 26)                                         */
#define R_ICU_GENBL0_EN26_Msk          (0x4000000UL)  /*!< EN26 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL0_EN27_Pos          (27UL)         /*!< EN27 (Bit 27)                                         */
#define R_ICU_GENBL0_EN27_Msk          (0x8000000UL)  /*!< EN27 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL0_EN28_Pos          (28UL)         /*!< EN28 (Bit 28)                                         */
#define R_ICU_GENBL0_EN28_Msk          (0x10000000UL) /*!< EN28 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL0_EN29_Pos          (29UL)         /*!< EN29 (Bit 29)                                         */
#define R_ICU_GENBL0_EN29_Msk          (0x20000000UL) /*!< EN29 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL0_EN30_Pos          (30UL)         /*!< EN30 (Bit 30)                                         */
#define R_ICU_GENBL0_EN30_Msk          (0x40000000UL) /*!< EN30 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL0_EN31_Pos          (31UL)         /*!< EN31 (Bit 31)                                         */
#define R_ICU_GENBL0_EN31_Msk          (0x80000000UL) /*!< EN31 (Bitfield-Mask: 0x01)                            */
/* ========================================================  GENBL1  ========================================================= */
#define R_ICU_GENBL1_EN0_Pos           (0UL)          /*!< EN0 (Bit 0)                                           */
#define R_ICU_GENBL1_EN0_Msk           (0x1UL)        /*!< EN0 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENBL1_EN1_Pos           (1UL)          /*!< EN1 (Bit 1)                                           */
#define R_ICU_GENBL1_EN1_Msk           (0x2UL)        /*!< EN1 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENBL1_EN2_Pos           (2UL)          /*!< EN2 (Bit 2)                                           */
#define R_ICU_GENBL1_EN2_Msk           (0x4UL)        /*!< EN2 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENBL1_EN3_Pos           (3UL)          /*!< EN3 (Bit 3)                                           */
#define R_ICU_GENBL1_EN3_Msk           (0x8UL)        /*!< EN3 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENBL1_EN4_Pos           (4UL)          /*!< EN4 (Bit 4)                                           */
#define R_ICU_GENBL1_EN4_Msk           (0x10UL)       /*!< EN4 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENBL1_EN5_Pos           (5UL)          /*!< EN5 (Bit 5)                                           */
#define R_ICU_GENBL1_EN5_Msk           (0x20UL)       /*!< EN5 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENBL1_EN6_Pos           (6UL)          /*!< EN6 (Bit 6)                                           */
#define R_ICU_GENBL1_EN6_Msk           (0x40UL)       /*!< EN6 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENBL1_EN7_Pos           (7UL)          /*!< EN7 (Bit 7)                                           */
#define R_ICU_GENBL1_EN7_Msk           (0x80UL)       /*!< EN7 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENBL1_EN8_Pos           (8UL)          /*!< EN8 (Bit 8)                                           */
#define R_ICU_GENBL1_EN8_Msk           (0x100UL)      /*!< EN8 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENBL1_EN9_Pos           (9UL)          /*!< EN9 (Bit 9)                                           */
#define R_ICU_GENBL1_EN9_Msk           (0x200UL)      /*!< EN9 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENBL1_EN10_Pos          (10UL)         /*!< EN10 (Bit 10)                                         */
#define R_ICU_GENBL1_EN10_Msk          (0x400UL)      /*!< EN10 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL1_EN11_Pos          (11UL)         /*!< EN11 (Bit 11)                                         */
#define R_ICU_GENBL1_EN11_Msk          (0x800UL)      /*!< EN11 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL1_EN12_Pos          (12UL)         /*!< EN12 (Bit 12)                                         */
#define R_ICU_GENBL1_EN12_Msk          (0x1000UL)     /*!< EN12 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL1_EN13_Pos          (13UL)         /*!< EN13 (Bit 13)                                         */
#define R_ICU_GENBL1_EN13_Msk          (0x2000UL)     /*!< EN13 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL1_EN14_Pos          (14UL)         /*!< EN14 (Bit 14)                                         */
#define R_ICU_GENBL1_EN14_Msk          (0x4000UL)     /*!< EN14 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL1_EN15_Pos          (15UL)         /*!< EN15 (Bit 15)                                         */
#define R_ICU_GENBL1_EN15_Msk          (0x8000UL)     /*!< EN15 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL1_EN16_Pos          (16UL)         /*!< EN16 (Bit 16)                                         */
#define R_ICU_GENBL1_EN16_Msk          (0x10000UL)    /*!< EN16 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL1_EN17_Pos          (17UL)         /*!< EN17 (Bit 17)                                         */
#define R_ICU_GENBL1_EN17_Msk          (0x20000UL)    /*!< EN17 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL1_EN18_Pos          (18UL)         /*!< EN18 (Bit 18)                                         */
#define R_ICU_GENBL1_EN18_Msk          (0x40000UL)    /*!< EN18 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL1_EN19_Pos          (19UL)         /*!< EN19 (Bit 19)                                         */
#define R_ICU_GENBL1_EN19_Msk          (0x80000UL)    /*!< EN19 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL1_EN20_Pos          (20UL)         /*!< EN20 (Bit 20)                                         */
#define R_ICU_GENBL1_EN20_Msk          (0x100000UL)   /*!< EN20 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL1_EN21_Pos          (21UL)         /*!< EN21 (Bit 21)                                         */
#define R_ICU_GENBL1_EN21_Msk          (0x200000UL)   /*!< EN21 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL1_EN22_Pos          (22UL)         /*!< EN22 (Bit 22)                                         */
#define R_ICU_GENBL1_EN22_Msk          (0x400000UL)   /*!< EN22 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL1_EN23_Pos          (23UL)         /*!< EN23 (Bit 23)                                         */
#define R_ICU_GENBL1_EN23_Msk          (0x800000UL)   /*!< EN23 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL1_EN24_Pos          (24UL)         /*!< EN24 (Bit 24)                                         */
#define R_ICU_GENBL1_EN24_Msk          (0x1000000UL)  /*!< EN24 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL1_EN25_Pos          (25UL)         /*!< EN25 (Bit 25)                                         */
#define R_ICU_GENBL1_EN25_Msk          (0x2000000UL)  /*!< EN25 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL1_EN26_Pos          (26UL)         /*!< EN26 (Bit 26)                                         */
#define R_ICU_GENBL1_EN26_Msk          (0x4000000UL)  /*!< EN26 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL1_EN27_Pos          (27UL)         /*!< EN27 (Bit 27)                                         */
#define R_ICU_GENBL1_EN27_Msk          (0x8000000UL)  /*!< EN27 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL1_EN28_Pos          (28UL)         /*!< EN28 (Bit 28)                                         */
#define R_ICU_GENBL1_EN28_Msk          (0x10000000UL) /*!< EN28 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL1_EN29_Pos          (29UL)         /*!< EN29 (Bit 29)                                         */
#define R_ICU_GENBL1_EN29_Msk          (0x20000000UL) /*!< EN29 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL1_EN30_Pos          (30UL)         /*!< EN30 (Bit 30)                                         */
#define R_ICU_GENBL1_EN30_Msk          (0x40000000UL) /*!< EN30 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL1_EN31_Pos          (31UL)         /*!< EN31 (Bit 31)                                         */
#define R_ICU_GENBL1_EN31_Msk          (0x80000000UL) /*!< EN31 (Bitfield-Mask: 0x01)                            */
/* ========================================================  GENBL2  ========================================================= */
#define R_ICU_GENBL2_EN0_Pos           (0UL)          /*!< EN0 (Bit 0)                                           */
#define R_ICU_GENBL2_EN0_Msk           (0x1UL)        /*!< EN0 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENBL2_EN1_Pos           (1UL)          /*!< EN1 (Bit 1)                                           */
#define R_ICU_GENBL2_EN1_Msk           (0x2UL)        /*!< EN1 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENBL2_EN2_Pos           (2UL)          /*!< EN2 (Bit 2)                                           */
#define R_ICU_GENBL2_EN2_Msk           (0x4UL)        /*!< EN2 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENBL2_EN3_Pos           (3UL)          /*!< EN3 (Bit 3)                                           */
#define R_ICU_GENBL2_EN3_Msk           (0x8UL)        /*!< EN3 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENBL2_EN4_Pos           (4UL)          /*!< EN4 (Bit 4)                                           */
#define R_ICU_GENBL2_EN4_Msk           (0x10UL)       /*!< EN4 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENBL2_EN5_Pos           (5UL)          /*!< EN5 (Bit 5)                                           */
#define R_ICU_GENBL2_EN5_Msk           (0x20UL)       /*!< EN5 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENBL2_EN6_Pos           (6UL)          /*!< EN6 (Bit 6)                                           */
#define R_ICU_GENBL2_EN6_Msk           (0x40UL)       /*!< EN6 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENBL2_EN7_Pos           (7UL)          /*!< EN7 (Bit 7)                                           */
#define R_ICU_GENBL2_EN7_Msk           (0x80UL)       /*!< EN7 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENBL2_EN8_Pos           (8UL)          /*!< EN8 (Bit 8)                                           */
#define R_ICU_GENBL2_EN8_Msk           (0x100UL)      /*!< EN8 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENBL2_EN9_Pos           (9UL)          /*!< EN9 (Bit 9)                                           */
#define R_ICU_GENBL2_EN9_Msk           (0x200UL)      /*!< EN9 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENBL2_EN10_Pos          (10UL)         /*!< EN10 (Bit 10)                                         */
#define R_ICU_GENBL2_EN10_Msk          (0x400UL)      /*!< EN10 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL2_EN11_Pos          (11UL)         /*!< EN11 (Bit 11)                                         */
#define R_ICU_GENBL2_EN11_Msk          (0x800UL)      /*!< EN11 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL2_EN12_Pos          (12UL)         /*!< EN12 (Bit 12)                                         */
#define R_ICU_GENBL2_EN12_Msk          (0x1000UL)     /*!< EN12 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL2_EN13_Pos          (13UL)         /*!< EN13 (Bit 13)                                         */
#define R_ICU_GENBL2_EN13_Msk          (0x2000UL)     /*!< EN13 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL2_EN14_Pos          (14UL)         /*!< EN14 (Bit 14)                                         */
#define R_ICU_GENBL2_EN14_Msk          (0x4000UL)     /*!< EN14 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL2_EN15_Pos          (15UL)         /*!< EN15 (Bit 15)                                         */
#define R_ICU_GENBL2_EN15_Msk          (0x8000UL)     /*!< EN15 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL2_EN16_Pos          (16UL)         /*!< EN16 (Bit 16)                                         */
#define R_ICU_GENBL2_EN16_Msk          (0x10000UL)    /*!< EN16 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL2_EN17_Pos          (17UL)         /*!< EN17 (Bit 17)                                         */
#define R_ICU_GENBL2_EN17_Msk          (0x20000UL)    /*!< EN17 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL2_EN18_Pos          (18UL)         /*!< EN18 (Bit 18)                                         */
#define R_ICU_GENBL2_EN18_Msk          (0x40000UL)    /*!< EN18 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL2_EN19_Pos          (19UL)         /*!< EN19 (Bit 19)                                         */
#define R_ICU_GENBL2_EN19_Msk          (0x80000UL)    /*!< EN19 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL2_EN20_Pos          (20UL)         /*!< EN20 (Bit 20)                                         */
#define R_ICU_GENBL2_EN20_Msk          (0x100000UL)   /*!< EN20 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL2_EN21_Pos          (21UL)         /*!< EN21 (Bit 21)                                         */
#define R_ICU_GENBL2_EN21_Msk          (0x200000UL)   /*!< EN21 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL2_EN22_Pos          (22UL)         /*!< EN22 (Bit 22)                                         */
#define R_ICU_GENBL2_EN22_Msk          (0x400000UL)   /*!< EN22 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL2_EN23_Pos          (23UL)         /*!< EN23 (Bit 23)                                         */
#define R_ICU_GENBL2_EN23_Msk          (0x800000UL)   /*!< EN23 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL2_EN24_Pos          (24UL)         /*!< EN24 (Bit 24)                                         */
#define R_ICU_GENBL2_EN24_Msk          (0x1000000UL)  /*!< EN24 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL2_EN25_Pos          (25UL)         /*!< EN25 (Bit 25)                                         */
#define R_ICU_GENBL2_EN25_Msk          (0x2000000UL)  /*!< EN25 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL2_EN26_Pos          (26UL)         /*!< EN26 (Bit 26)                                         */
#define R_ICU_GENBL2_EN26_Msk          (0x4000000UL)  /*!< EN26 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL2_EN27_Pos          (27UL)         /*!< EN27 (Bit 27)                                         */
#define R_ICU_GENBL2_EN27_Msk          (0x8000000UL)  /*!< EN27 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL2_EN28_Pos          (28UL)         /*!< EN28 (Bit 28)                                         */
#define R_ICU_GENBL2_EN28_Msk          (0x10000000UL) /*!< EN28 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL2_EN29_Pos          (29UL)         /*!< EN29 (Bit 29)                                         */
#define R_ICU_GENBL2_EN29_Msk          (0x20000000UL) /*!< EN29 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL2_EN30_Pos          (30UL)         /*!< EN30 (Bit 30)                                         */
#define R_ICU_GENBL2_EN30_Msk          (0x40000000UL) /*!< EN30 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENBL2_EN31_Pos          (31UL)         /*!< EN31 (Bit 31)                                         */
#define R_ICU_GENBL2_EN31_Msk          (0x80000000UL) /*!< EN31 (Bitfield-Mask: 0x01)                            */
/* ========================================================  GCRBE0  ========================================================= */
#define R_ICU_GCRBE0_CLR0_Pos          (0UL)          /*!< CLR0 (Bit 0)                                          */
#define R_ICU_GCRBE0_CLR0_Msk          (0x1UL)        /*!< CLR0 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GCRBE0_CLR1_Pos          (1UL)          /*!< CLR1 (Bit 1)                                          */
#define R_ICU_GCRBE0_CLR1_Msk          (0x2UL)        /*!< CLR1 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GCRBE0_CLR2_Pos          (2UL)          /*!< CLR2 (Bit 2)                                          */
#define R_ICU_GCRBE0_CLR2_Msk          (0x4UL)        /*!< CLR2 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GCRBE0_CLR3_Pos          (3UL)          /*!< CLR3 (Bit 3)                                          */
#define R_ICU_GCRBE0_CLR3_Msk          (0x8UL)        /*!< CLR3 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GCRBE0_CLR4_Pos          (4UL)          /*!< CLR4 (Bit 4)                                          */
#define R_ICU_GCRBE0_CLR4_Msk          (0x10UL)       /*!< CLR4 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GCRBE0_CLR5_Pos          (5UL)          /*!< CLR5 (Bit 5)                                          */
#define R_ICU_GCRBE0_CLR5_Msk          (0x20UL)       /*!< CLR5 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GCRBE0_CLR6_Pos          (6UL)          /*!< CLR6 (Bit 6)                                          */
#define R_ICU_GCRBE0_CLR6_Msk          (0x40UL)       /*!< CLR6 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GCRBE0_CLR7_Pos          (7UL)          /*!< CLR7 (Bit 7)                                          */
#define R_ICU_GCRBE0_CLR7_Msk          (0x80UL)       /*!< CLR7 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GCRBE0_CLR8_Pos          (8UL)          /*!< CLR8 (Bit 8)                                          */
#define R_ICU_GCRBE0_CLR8_Msk          (0x100UL)      /*!< CLR8 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GCRBE0_CLR9_Pos          (9UL)          /*!< CLR9 (Bit 9)                                          */
#define R_ICU_GCRBE0_CLR9_Msk          (0x200UL)      /*!< CLR9 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GCRBE0_CLR10_Pos         (10UL)         /*!< CLR10 (Bit 10)                                        */
#define R_ICU_GCRBE0_CLR10_Msk         (0x400UL)      /*!< CLR10 (Bitfield-Mask: 0x01)                           */
#define R_ICU_GCRBE0_CLR11_Pos         (11UL)         /*!< CLR11 (Bit 11)                                        */
#define R_ICU_GCRBE0_CLR11_Msk         (0x800UL)      /*!< CLR11 (Bitfield-Mask: 0x01)                           */
#define R_ICU_GCRBE0_CLR12_Pos         (12UL)         /*!< CLR12 (Bit 12)                                        */
#define R_ICU_GCRBE0_CLR12_Msk         (0x1000UL)     /*!< CLR12 (Bitfield-Mask: 0x01)                           */
#define R_ICU_GCRBE0_CLR13_Pos         (13UL)         /*!< CLR13 (Bit 13)                                        */
#define R_ICU_GCRBE0_CLR13_Msk         (0x2000UL)     /*!< CLR13 (Bitfield-Mask: 0x01)                           */
#define R_ICU_GCRBE0_CLR14_Pos         (14UL)         /*!< CLR14 (Bit 14)                                        */
#define R_ICU_GCRBE0_CLR14_Msk         (0x4000UL)     /*!< CLR14 (Bitfield-Mask: 0x01)                           */
#define R_ICU_GCRBE0_CLR15_Pos         (15UL)         /*!< CLR15 (Bit 15)                                        */
#define R_ICU_GCRBE0_CLR15_Msk         (0x8000UL)     /*!< CLR15 (Bitfield-Mask: 0x01)                           */
#define R_ICU_GCRBE0_CLR16_Pos         (16UL)         /*!< CLR16 (Bit 16)                                        */
#define R_ICU_GCRBE0_CLR16_Msk         (0x10000UL)    /*!< CLR16 (Bitfield-Mask: 0x01)                           */
#define R_ICU_GCRBE0_CLR17_Pos         (17UL)         /*!< CLR17 (Bit 17)                                        */
#define R_ICU_GCRBE0_CLR17_Msk         (0x20000UL)    /*!< CLR17 (Bitfield-Mask: 0x01)                           */
#define R_ICU_GCRBE0_CLR18_Pos         (18UL)         /*!< CLR18 (Bit 18)                                        */
#define R_ICU_GCRBE0_CLR18_Msk         (0x40000UL)    /*!< CLR18 (Bitfield-Mask: 0x01)                           */
#define R_ICU_GCRBE0_CLR19_Pos         (19UL)         /*!< CLR19 (Bit 19)                                        */
#define R_ICU_GCRBE0_CLR19_Msk         (0x80000UL)    /*!< CLR19 (Bitfield-Mask: 0x01)                           */
#define R_ICU_GCRBE0_CLR20_Pos         (20UL)         /*!< CLR20 (Bit 20)                                        */
#define R_ICU_GCRBE0_CLR20_Msk         (0x100000UL)   /*!< CLR20 (Bitfield-Mask: 0x01)                           */
#define R_ICU_GCRBE0_CLR21_Pos         (21UL)         /*!< CLR21 (Bit 21)                                        */
#define R_ICU_GCRBE0_CLR21_Msk         (0x200000UL)   /*!< CLR21 (Bitfield-Mask: 0x01)                           */
#define R_ICU_GCRBE0_CLR22_Pos         (22UL)         /*!< CLR22 (Bit 22)                                        */
#define R_ICU_GCRBE0_CLR22_Msk         (0x400000UL)   /*!< CLR22 (Bitfield-Mask: 0x01)                           */
#define R_ICU_GCRBE0_CLR23_Pos         (23UL)         /*!< CLR23 (Bit 23)                                        */
#define R_ICU_GCRBE0_CLR23_Msk         (0x800000UL)   /*!< CLR23 (Bitfield-Mask: 0x01)                           */
#define R_ICU_GCRBE0_CLR24_Pos         (24UL)         /*!< CLR24 (Bit 24)                                        */
#define R_ICU_GCRBE0_CLR24_Msk         (0x1000000UL)  /*!< CLR24 (Bitfield-Mask: 0x01)                           */
#define R_ICU_GCRBE0_CLR25_Pos         (25UL)         /*!< CLR25 (Bit 25)                                        */
#define R_ICU_GCRBE0_CLR25_Msk         (0x2000000UL)  /*!< CLR25 (Bitfield-Mask: 0x01)                           */
#define R_ICU_GCRBE0_CLR26_Pos         (26UL)         /*!< CLR26 (Bit 26)                                        */
#define R_ICU_GCRBE0_CLR26_Msk         (0x4000000UL)  /*!< CLR26 (Bitfield-Mask: 0x01)                           */
#define R_ICU_GCRBE0_CLR27_Pos         (27UL)         /*!< CLR27 (Bit 27)                                        */
#define R_ICU_GCRBE0_CLR27_Msk         (0x8000000UL)  /*!< CLR27 (Bitfield-Mask: 0x01)                           */
#define R_ICU_GCRBE0_CLR28_Pos         (28UL)         /*!< CLR28 (Bit 28)                                        */
#define R_ICU_GCRBE0_CLR28_Msk         (0x10000000UL) /*!< CLR28 (Bitfield-Mask: 0x01)                           */
#define R_ICU_GCRBE0_CLR29_Pos         (29UL)         /*!< CLR29 (Bit 29)                                        */
#define R_ICU_GCRBE0_CLR29_Msk         (0x20000000UL) /*!< CLR29 (Bitfield-Mask: 0x01)                           */
#define R_ICU_GCRBE0_CLR30_Pos         (30UL)         /*!< CLR30 (Bit 30)                                        */
#define R_ICU_GCRBE0_CLR30_Msk         (0x40000000UL) /*!< CLR30 (Bitfield-Mask: 0x01)                           */
#define R_ICU_GCRBE0_CLR31_Pos         (31UL)         /*!< CLR31 (Bit 31)                                        */
#define R_ICU_GCRBE0_CLR31_Msk         (0x80000000UL) /*!< CLR31 (Bitfield-Mask: 0x01)                           */
/* ========================================================  GRPAL0  ========================================================= */
#define R_ICU_GRPAL0_IS0_Pos           (0UL)          /*!< IS0 (Bit 0)                                           */
#define R_ICU_GRPAL0_IS0_Msk           (0x1UL)        /*!< IS0 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPAL0_IS1_Pos           (1UL)          /*!< IS1 (Bit 1)                                           */
#define R_ICU_GRPAL0_IS1_Msk           (0x2UL)        /*!< IS1 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPAL0_IS2_Pos           (2UL)          /*!< IS2 (Bit 2)                                           */
#define R_ICU_GRPAL0_IS2_Msk           (0x4UL)        /*!< IS2 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPAL0_IS3_Pos           (3UL)          /*!< IS3 (Bit 3)                                           */
#define R_ICU_GRPAL0_IS3_Msk           (0x8UL)        /*!< IS3 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPAL0_IS4_Pos           (4UL)          /*!< IS4 (Bit 4)                                           */
#define R_ICU_GRPAL0_IS4_Msk           (0x10UL)       /*!< IS4 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPAL0_IS5_Pos           (5UL)          /*!< IS5 (Bit 5)                                           */
#define R_ICU_GRPAL0_IS5_Msk           (0x20UL)       /*!< IS5 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPAL0_IS6_Pos           (6UL)          /*!< IS6 (Bit 6)                                           */
#define R_ICU_GRPAL0_IS6_Msk           (0x40UL)       /*!< IS6 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPAL0_IS7_Pos           (7UL)          /*!< IS7 (Bit 7)                                           */
#define R_ICU_GRPAL0_IS7_Msk           (0x80UL)       /*!< IS7 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPAL0_IS8_Pos           (8UL)          /*!< IS8 (Bit 8)                                           */
#define R_ICU_GRPAL0_IS8_Msk           (0x100UL)      /*!< IS8 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPAL0_IS9_Pos           (9UL)          /*!< IS9 (Bit 9)                                           */
#define R_ICU_GRPAL0_IS9_Msk           (0x200UL)      /*!< IS9 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPAL0_IS10_Pos          (10UL)         /*!< IS10 (Bit 10)                                         */
#define R_ICU_GRPAL0_IS10_Msk          (0x400UL)      /*!< IS10 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPAL0_IS11_Pos          (11UL)         /*!< IS11 (Bit 11)                                         */
#define R_ICU_GRPAL0_IS11_Msk          (0x800UL)      /*!< IS11 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPAL0_IS12_Pos          (12UL)         /*!< IS12 (Bit 12)                                         */
#define R_ICU_GRPAL0_IS12_Msk          (0x1000UL)     /*!< IS12 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPAL0_IS13_Pos          (13UL)         /*!< IS13 (Bit 13)                                         */
#define R_ICU_GRPAL0_IS13_Msk          (0x2000UL)     /*!< IS13 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPAL0_IS14_Pos          (14UL)         /*!< IS14 (Bit 14)                                         */
#define R_ICU_GRPAL0_IS14_Msk          (0x4000UL)     /*!< IS14 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPAL0_IS15_Pos          (15UL)         /*!< IS15 (Bit 15)                                         */
#define R_ICU_GRPAL0_IS15_Msk          (0x8000UL)     /*!< IS15 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPAL0_IS16_Pos          (16UL)         /*!< IS16 (Bit 16)                                         */
#define R_ICU_GRPAL0_IS16_Msk          (0x10000UL)    /*!< IS16 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPAL0_IS17_Pos          (17UL)         /*!< IS17 (Bit 17)                                         */
#define R_ICU_GRPAL0_IS17_Msk          (0x20000UL)    /*!< IS17 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPAL0_IS18_Pos          (18UL)         /*!< IS18 (Bit 18)                                         */
#define R_ICU_GRPAL0_IS18_Msk          (0x40000UL)    /*!< IS18 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPAL0_IS19_Pos          (19UL)         /*!< IS19 (Bit 19)                                         */
#define R_ICU_GRPAL0_IS19_Msk          (0x80000UL)    /*!< IS19 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPAL0_IS20_Pos          (20UL)         /*!< IS20 (Bit 20)                                         */
#define R_ICU_GRPAL0_IS20_Msk          (0x100000UL)   /*!< IS20 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPAL0_IS21_Pos          (21UL)         /*!< IS21 (Bit 21)                                         */
#define R_ICU_GRPAL0_IS21_Msk          (0x200000UL)   /*!< IS21 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPAL0_IS22_Pos          (22UL)         /*!< IS22 (Bit 22)                                         */
#define R_ICU_GRPAL0_IS22_Msk          (0x400000UL)   /*!< IS22 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPAL0_IS23_Pos          (23UL)         /*!< IS23 (Bit 23)                                         */
#define R_ICU_GRPAL0_IS23_Msk          (0x800000UL)   /*!< IS23 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPAL0_IS24_Pos          (24UL)         /*!< IS24 (Bit 24)                                         */
#define R_ICU_GRPAL0_IS24_Msk          (0x1000000UL)  /*!< IS24 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPAL0_IS25_Pos          (25UL)         /*!< IS25 (Bit 25)                                         */
#define R_ICU_GRPAL0_IS25_Msk          (0x2000000UL)  /*!< IS25 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPAL0_IS26_Pos          (26UL)         /*!< IS26 (Bit 26)                                         */
#define R_ICU_GRPAL0_IS26_Msk          (0x4000000UL)  /*!< IS26 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPAL0_IS27_Pos          (27UL)         /*!< IS27 (Bit 27)                                         */
#define R_ICU_GRPAL0_IS27_Msk          (0x8000000UL)  /*!< IS27 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPAL0_IS28_Pos          (28UL)         /*!< IS28 (Bit 28)                                         */
#define R_ICU_GRPAL0_IS28_Msk          (0x10000000UL) /*!< IS28 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPAL0_IS29_Pos          (29UL)         /*!< IS29 (Bit 29)                                         */
#define R_ICU_GRPAL0_IS29_Msk          (0x20000000UL) /*!< IS29 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPAL0_IS30_Pos          (30UL)         /*!< IS30 (Bit 30)                                         */
#define R_ICU_GRPAL0_IS30_Msk          (0x40000000UL) /*!< IS30 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPAL0_IS31_Pos          (31UL)         /*!< IS31 (Bit 31)                                         */
#define R_ICU_GRPAL0_IS31_Msk          (0x80000000UL) /*!< IS31 (Bitfield-Mask: 0x01)                            */
/* ========================================================  GRPAL1  ========================================================= */
#define R_ICU_GRPAL1_IS0_Pos           (0UL)          /*!< IS0 (Bit 0)                                           */
#define R_ICU_GRPAL1_IS0_Msk           (0x1UL)        /*!< IS0 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPAL1_IS1_Pos           (1UL)          /*!< IS1 (Bit 1)                                           */
#define R_ICU_GRPAL1_IS1_Msk           (0x2UL)        /*!< IS1 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPAL1_IS2_Pos           (2UL)          /*!< IS2 (Bit 2)                                           */
#define R_ICU_GRPAL1_IS2_Msk           (0x4UL)        /*!< IS2 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPAL1_IS3_Pos           (3UL)          /*!< IS3 (Bit 3)                                           */
#define R_ICU_GRPAL1_IS3_Msk           (0x8UL)        /*!< IS3 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPAL1_IS4_Pos           (4UL)          /*!< IS4 (Bit 4)                                           */
#define R_ICU_GRPAL1_IS4_Msk           (0x10UL)       /*!< IS4 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPAL1_IS5_Pos           (5UL)          /*!< IS5 (Bit 5)                                           */
#define R_ICU_GRPAL1_IS5_Msk           (0x20UL)       /*!< IS5 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPAL1_IS6_Pos           (6UL)          /*!< IS6 (Bit 6)                                           */
#define R_ICU_GRPAL1_IS6_Msk           (0x40UL)       /*!< IS6 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPAL1_IS7_Pos           (7UL)          /*!< IS7 (Bit 7)                                           */
#define R_ICU_GRPAL1_IS7_Msk           (0x80UL)       /*!< IS7 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPAL1_IS8_Pos           (8UL)          /*!< IS8 (Bit 8)                                           */
#define R_ICU_GRPAL1_IS8_Msk           (0x100UL)      /*!< IS8 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPAL1_IS9_Pos           (9UL)          /*!< IS9 (Bit 9)                                           */
#define R_ICU_GRPAL1_IS9_Msk           (0x200UL)      /*!< IS9 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GRPAL1_IS10_Pos          (10UL)         /*!< IS10 (Bit 10)                                         */
#define R_ICU_GRPAL1_IS10_Msk          (0x400UL)      /*!< IS10 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPAL1_IS11_Pos          (11UL)         /*!< IS11 (Bit 11)                                         */
#define R_ICU_GRPAL1_IS11_Msk          (0x800UL)      /*!< IS11 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPAL1_IS12_Pos          (12UL)         /*!< IS12 (Bit 12)                                         */
#define R_ICU_GRPAL1_IS12_Msk          (0x1000UL)     /*!< IS12 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPAL1_IS13_Pos          (13UL)         /*!< IS13 (Bit 13)                                         */
#define R_ICU_GRPAL1_IS13_Msk          (0x2000UL)     /*!< IS13 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPAL1_IS14_Pos          (14UL)         /*!< IS14 (Bit 14)                                         */
#define R_ICU_GRPAL1_IS14_Msk          (0x4000UL)     /*!< IS14 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPAL1_IS15_Pos          (15UL)         /*!< IS15 (Bit 15)                                         */
#define R_ICU_GRPAL1_IS15_Msk          (0x8000UL)     /*!< IS15 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPAL1_IS16_Pos          (16UL)         /*!< IS16 (Bit 16)                                         */
#define R_ICU_GRPAL1_IS16_Msk          (0x10000UL)    /*!< IS16 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPAL1_IS17_Pos          (17UL)         /*!< IS17 (Bit 17)                                         */
#define R_ICU_GRPAL1_IS17_Msk          (0x20000UL)    /*!< IS17 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPAL1_IS18_Pos          (18UL)         /*!< IS18 (Bit 18)                                         */
#define R_ICU_GRPAL1_IS18_Msk          (0x40000UL)    /*!< IS18 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPAL1_IS19_Pos          (19UL)         /*!< IS19 (Bit 19)                                         */
#define R_ICU_GRPAL1_IS19_Msk          (0x80000UL)    /*!< IS19 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPAL1_IS20_Pos          (20UL)         /*!< IS20 (Bit 20)                                         */
#define R_ICU_GRPAL1_IS20_Msk          (0x100000UL)   /*!< IS20 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPAL1_IS21_Pos          (21UL)         /*!< IS21 (Bit 21)                                         */
#define R_ICU_GRPAL1_IS21_Msk          (0x200000UL)   /*!< IS21 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPAL1_IS22_Pos          (22UL)         /*!< IS22 (Bit 22)                                         */
#define R_ICU_GRPAL1_IS22_Msk          (0x400000UL)   /*!< IS22 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPAL1_IS23_Pos          (23UL)         /*!< IS23 (Bit 23)                                         */
#define R_ICU_GRPAL1_IS23_Msk          (0x800000UL)   /*!< IS23 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPAL1_IS24_Pos          (24UL)         /*!< IS24 (Bit 24)                                         */
#define R_ICU_GRPAL1_IS24_Msk          (0x1000000UL)  /*!< IS24 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPAL1_IS25_Pos          (25UL)         /*!< IS25 (Bit 25)                                         */
#define R_ICU_GRPAL1_IS25_Msk          (0x2000000UL)  /*!< IS25 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPAL1_IS26_Pos          (26UL)         /*!< IS26 (Bit 26)                                         */
#define R_ICU_GRPAL1_IS26_Msk          (0x4000000UL)  /*!< IS26 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPAL1_IS27_Pos          (27UL)         /*!< IS27 (Bit 27)                                         */
#define R_ICU_GRPAL1_IS27_Msk          (0x8000000UL)  /*!< IS27 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPAL1_IS28_Pos          (28UL)         /*!< IS28 (Bit 28)                                         */
#define R_ICU_GRPAL1_IS28_Msk          (0x10000000UL) /*!< IS28 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPAL1_IS29_Pos          (29UL)         /*!< IS29 (Bit 29)                                         */
#define R_ICU_GRPAL1_IS29_Msk          (0x20000000UL) /*!< IS29 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPAL1_IS30_Pos          (30UL)         /*!< IS30 (Bit 30)                                         */
#define R_ICU_GRPAL1_IS30_Msk          (0x40000000UL) /*!< IS30 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GRPAL1_IS31_Pos          (31UL)         /*!< IS31 (Bit 31)                                         */
#define R_ICU_GRPAL1_IS31_Msk          (0x80000000UL) /*!< IS31 (Bitfield-Mask: 0x01)                            */
/* ========================================================  GENAL0  ========================================================= */
#define R_ICU_GENAL0_EN0_Pos           (0UL)          /*!< EN0 (Bit 0)                                           */
#define R_ICU_GENAL0_EN0_Msk           (0x1UL)        /*!< EN0 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENAL0_EN1_Pos           (1UL)          /*!< EN1 (Bit 1)                                           */
#define R_ICU_GENAL0_EN1_Msk           (0x2UL)        /*!< EN1 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENAL0_EN2_Pos           (2UL)          /*!< EN2 (Bit 2)                                           */
#define R_ICU_GENAL0_EN2_Msk           (0x4UL)        /*!< EN2 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENAL0_EN3_Pos           (3UL)          /*!< EN3 (Bit 3)                                           */
#define R_ICU_GENAL0_EN3_Msk           (0x8UL)        /*!< EN3 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENAL0_EN4_Pos           (4UL)          /*!< EN4 (Bit 4)                                           */
#define R_ICU_GENAL0_EN4_Msk           (0x10UL)       /*!< EN4 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENAL0_EN5_Pos           (5UL)          /*!< EN5 (Bit 5)                                           */
#define R_ICU_GENAL0_EN5_Msk           (0x20UL)       /*!< EN5 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENAL0_EN6_Pos           (6UL)          /*!< EN6 (Bit 6)                                           */
#define R_ICU_GENAL0_EN6_Msk           (0x40UL)       /*!< EN6 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENAL0_EN7_Pos           (7UL)          /*!< EN7 (Bit 7)                                           */
#define R_ICU_GENAL0_EN7_Msk           (0x80UL)       /*!< EN7 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENAL0_EN8_Pos           (8UL)          /*!< EN8 (Bit 8)                                           */
#define R_ICU_GENAL0_EN8_Msk           (0x100UL)      /*!< EN8 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENAL0_EN9_Pos           (9UL)          /*!< EN9 (Bit 9)                                           */
#define R_ICU_GENAL0_EN9_Msk           (0x200UL)      /*!< EN9 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENAL0_EN10_Pos          (10UL)         /*!< EN10 (Bit 10)                                         */
#define R_ICU_GENAL0_EN10_Msk          (0x400UL)      /*!< EN10 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENAL0_EN11_Pos          (11UL)         /*!< EN11 (Bit 11)                                         */
#define R_ICU_GENAL0_EN11_Msk          (0x800UL)      /*!< EN11 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENAL0_EN12_Pos          (12UL)         /*!< EN12 (Bit 12)                                         */
#define R_ICU_GENAL0_EN12_Msk          (0x1000UL)     /*!< EN12 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENAL0_EN13_Pos          (13UL)         /*!< EN13 (Bit 13)                                         */
#define R_ICU_GENAL0_EN13_Msk          (0x2000UL)     /*!< EN13 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENAL0_EN14_Pos          (14UL)         /*!< EN14 (Bit 14)                                         */
#define R_ICU_GENAL0_EN14_Msk          (0x4000UL)     /*!< EN14 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENAL0_EN15_Pos          (15UL)         /*!< EN15 (Bit 15)                                         */
#define R_ICU_GENAL0_EN15_Msk          (0x8000UL)     /*!< EN15 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENAL0_EN16_Pos          (16UL)         /*!< EN16 (Bit 16)                                         */
#define R_ICU_GENAL0_EN16_Msk          (0x10000UL)    /*!< EN16 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENAL0_EN17_Pos          (17UL)         /*!< EN17 (Bit 17)                                         */
#define R_ICU_GENAL0_EN17_Msk          (0x20000UL)    /*!< EN17 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENAL0_EN18_Pos          (18UL)         /*!< EN18 (Bit 18)                                         */
#define R_ICU_GENAL0_EN18_Msk          (0x40000UL)    /*!< EN18 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENAL0_EN19_Pos          (19UL)         /*!< EN19 (Bit 19)                                         */
#define R_ICU_GENAL0_EN19_Msk          (0x80000UL)    /*!< EN19 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENAL0_EN20_Pos          (20UL)         /*!< EN20 (Bit 20)                                         */
#define R_ICU_GENAL0_EN20_Msk          (0x100000UL)   /*!< EN20 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENAL0_EN21_Pos          (21UL)         /*!< EN21 (Bit 21)                                         */
#define R_ICU_GENAL0_EN21_Msk          (0x200000UL)   /*!< EN21 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENAL0_EN22_Pos          (22UL)         /*!< EN22 (Bit 22)                                         */
#define R_ICU_GENAL0_EN22_Msk          (0x400000UL)   /*!< EN22 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENAL0_EN23_Pos          (23UL)         /*!< EN23 (Bit 23)                                         */
#define R_ICU_GENAL0_EN23_Msk          (0x800000UL)   /*!< EN23 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENAL0_EN24_Pos          (24UL)         /*!< EN24 (Bit 24)                                         */
#define R_ICU_GENAL0_EN24_Msk          (0x1000000UL)  /*!< EN24 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENAL0_EN25_Pos          (25UL)         /*!< EN25 (Bit 25)                                         */
#define R_ICU_GENAL0_EN25_Msk          (0x2000000UL)  /*!< EN25 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENAL0_EN26_Pos          (26UL)         /*!< EN26 (Bit 26)                                         */
#define R_ICU_GENAL0_EN26_Msk          (0x4000000UL)  /*!< EN26 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENAL0_EN27_Pos          (27UL)         /*!< EN27 (Bit 27)                                         */
#define R_ICU_GENAL0_EN27_Msk          (0x8000000UL)  /*!< EN27 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENAL0_EN28_Pos          (28UL)         /*!< EN28 (Bit 28)                                         */
#define R_ICU_GENAL0_EN28_Msk          (0x10000000UL) /*!< EN28 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENAL0_EN29_Pos          (29UL)         /*!< EN29 (Bit 29)                                         */
#define R_ICU_GENAL0_EN29_Msk          (0x20000000UL) /*!< EN29 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENAL0_EN30_Pos          (30UL)         /*!< EN30 (Bit 30)                                         */
#define R_ICU_GENAL0_EN30_Msk          (0x40000000UL) /*!< EN30 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENAL0_EN31_Pos          (31UL)         /*!< EN31 (Bit 31)                                         */
#define R_ICU_GENAL0_EN31_Msk          (0x80000000UL) /*!< EN31 (Bitfield-Mask: 0x01)                            */
/* ========================================================  GENAL1  ========================================================= */
#define R_ICU_GENAL1_EN0_Pos           (0UL)          /*!< EN0 (Bit 0)                                           */
#define R_ICU_GENAL1_EN0_Msk           (0x1UL)        /*!< EN0 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENAL1_EN1_Pos           (1UL)          /*!< EN1 (Bit 1)                                           */
#define R_ICU_GENAL1_EN1_Msk           (0x2UL)        /*!< EN1 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENAL1_EN2_Pos           (2UL)          /*!< EN2 (Bit 2)                                           */
#define R_ICU_GENAL1_EN2_Msk           (0x4UL)        /*!< EN2 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENAL1_EN3_Pos           (3UL)          /*!< EN3 (Bit 3)                                           */
#define R_ICU_GENAL1_EN3_Msk           (0x8UL)        /*!< EN3 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENAL1_EN4_Pos           (4UL)          /*!< EN4 (Bit 4)                                           */
#define R_ICU_GENAL1_EN4_Msk           (0x10UL)       /*!< EN4 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENAL1_EN5_Pos           (5UL)          /*!< EN5 (Bit 5)                                           */
#define R_ICU_GENAL1_EN5_Msk           (0x20UL)       /*!< EN5 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENAL1_EN6_Pos           (6UL)          /*!< EN6 (Bit 6)                                           */
#define R_ICU_GENAL1_EN6_Msk           (0x40UL)       /*!< EN6 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENAL1_EN7_Pos           (7UL)          /*!< EN7 (Bit 7)                                           */
#define R_ICU_GENAL1_EN7_Msk           (0x80UL)       /*!< EN7 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENAL1_EN8_Pos           (8UL)          /*!< EN8 (Bit 8)                                           */
#define R_ICU_GENAL1_EN8_Msk           (0x100UL)      /*!< EN8 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENAL1_EN9_Pos           (9UL)          /*!< EN9 (Bit 9)                                           */
#define R_ICU_GENAL1_EN9_Msk           (0x200UL)      /*!< EN9 (Bitfield-Mask: 0x01)                             */
#define R_ICU_GENAL1_EN10_Pos          (10UL)         /*!< EN10 (Bit 10)                                         */
#define R_ICU_GENAL1_EN10_Msk          (0x400UL)      /*!< EN10 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENAL1_EN11_Pos          (11UL)         /*!< EN11 (Bit 11)                                         */
#define R_ICU_GENAL1_EN11_Msk          (0x800UL)      /*!< EN11 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENAL1_EN12_Pos          (12UL)         /*!< EN12 (Bit 12)                                         */
#define R_ICU_GENAL1_EN12_Msk          (0x1000UL)     /*!< EN12 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENAL1_EN13_Pos          (13UL)         /*!< EN13 (Bit 13)                                         */
#define R_ICU_GENAL1_EN13_Msk          (0x2000UL)     /*!< EN13 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENAL1_EN14_Pos          (14UL)         /*!< EN14 (Bit 14)                                         */
#define R_ICU_GENAL1_EN14_Msk          (0x4000UL)     /*!< EN14 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENAL1_EN15_Pos          (15UL)         /*!< EN15 (Bit 15)                                         */
#define R_ICU_GENAL1_EN15_Msk          (0x8000UL)     /*!< EN15 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENAL1_EN16_Pos          (16UL)         /*!< EN16 (Bit 16)                                         */
#define R_ICU_GENAL1_EN16_Msk          (0x10000UL)    /*!< EN16 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENAL1_EN17_Pos          (17UL)         /*!< EN17 (Bit 17)                                         */
#define R_ICU_GENAL1_EN17_Msk          (0x20000UL)    /*!< EN17 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENAL1_EN18_Pos          (18UL)         /*!< EN18 (Bit 18)                                         */
#define R_ICU_GENAL1_EN18_Msk          (0x40000UL)    /*!< EN18 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENAL1_EN19_Pos          (19UL)         /*!< EN19 (Bit 19)                                         */
#define R_ICU_GENAL1_EN19_Msk          (0x80000UL)    /*!< EN19 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENAL1_EN20_Pos          (20UL)         /*!< EN20 (Bit 20)                                         */
#define R_ICU_GENAL1_EN20_Msk          (0x100000UL)   /*!< EN20 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENAL1_EN21_Pos          (21UL)         /*!< EN21 (Bit 21)                                         */
#define R_ICU_GENAL1_EN21_Msk          (0x200000UL)   /*!< EN21 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENAL1_EN22_Pos          (22UL)         /*!< EN22 (Bit 22)                                         */
#define R_ICU_GENAL1_EN22_Msk          (0x400000UL)   /*!< EN22 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENAL1_EN23_Pos          (23UL)         /*!< EN23 (Bit 23)                                         */
#define R_ICU_GENAL1_EN23_Msk          (0x800000UL)   /*!< EN23 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENAL1_EN24_Pos          (24UL)         /*!< EN24 (Bit 24)                                         */
#define R_ICU_GENAL1_EN24_Msk          (0x1000000UL)  /*!< EN24 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENAL1_EN25_Pos          (25UL)         /*!< EN25 (Bit 25)                                         */
#define R_ICU_GENAL1_EN25_Msk          (0x2000000UL)  /*!< EN25 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENAL1_EN26_Pos          (26UL)         /*!< EN26 (Bit 26)                                         */
#define R_ICU_GENAL1_EN26_Msk          (0x4000000UL)  /*!< EN26 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENAL1_EN27_Pos          (27UL)         /*!< EN27 (Bit 27)                                         */
#define R_ICU_GENAL1_EN27_Msk          (0x8000000UL)  /*!< EN27 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENAL1_EN28_Pos          (28UL)         /*!< EN28 (Bit 28)                                         */
#define R_ICU_GENAL1_EN28_Msk          (0x10000000UL) /*!< EN28 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENAL1_EN29_Pos          (29UL)         /*!< EN29 (Bit 29)                                         */
#define R_ICU_GENAL1_EN29_Msk          (0x20000000UL) /*!< EN29 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENAL1_EN30_Pos          (30UL)         /*!< EN30 (Bit 30)                                         */
#define R_ICU_GENAL1_EN30_Msk          (0x40000000UL) /*!< EN30 (Bitfield-Mask: 0x01)                            */
#define R_ICU_GENAL1_EN31_Pos          (31UL)         /*!< EN31 (Bit 31)                                         */
#define R_ICU_GENAL1_EN31_Msk          (0x80000000UL) /*!< EN31 (Bitfield-Mask: 0x01)                            */
/* =========================================================  PIARC  ========================================================= */
#define R_ICU_PIARC_PIR0_Pos           (0UL)          /*!< PIR0 (Bit 0)                                          */
#define R_ICU_PIARC_PIR0_Msk           (0x1UL)        /*!< PIR0 (Bitfield-Mask: 0x01)                            */
#define R_ICU_PIARC_PIR1_Pos           (1UL)          /*!< PIR1 (Bit 1)                                          */
#define R_ICU_PIARC_PIR1_Msk           (0x2UL)        /*!< PIR1 (Bitfield-Mask: 0x01)                            */
#define R_ICU_PIARC_PIR2_Pos           (2UL)          /*!< PIR2 (Bit 2)                                          */
#define R_ICU_PIARC_PIR2_Msk           (0x4UL)        /*!< PIR2 (Bitfield-Mask: 0x01)                            */
#define R_ICU_PIARC_PIR3_Pos           (3UL)          /*!< PIR3 (Bit 3)                                          */
#define R_ICU_PIARC_PIR3_Msk           (0x8UL)        /*!< PIR3 (Bitfield-Mask: 0x01)                            */
#define R_ICU_PIARC_PIR4_Pos           (4UL)          /*!< PIR4 (Bit 4)                                          */
#define R_ICU_PIARC_PIR4_Msk           (0x10UL)       /*!< PIR4 (Bitfield-Mask: 0x01)                            */
#define R_ICU_PIARC_PIR5_Pos           (5UL)          /*!< PIR5 (Bit 5)                                          */
#define R_ICU_PIARC_PIR5_Msk           (0x20UL)       /*!< PIR5 (Bitfield-Mask: 0x01)                            */
#define R_ICU_PIARC_PIR6_Pos           (6UL)          /*!< PIR6 (Bit 6)                                          */
#define R_ICU_PIARC_PIR6_Msk           (0x40UL)       /*!< PIR6 (Bitfield-Mask: 0x01)                            */
#define R_ICU_PIARC_PIR7_Pos           (7UL)          /*!< PIR7 (Bit 7)                                          */
#define R_ICU_PIARC_PIR7_Msk           (0x80UL)       /*!< PIR7 (Bitfield-Mask: 0x01)                            */
/* ========================================================  SLIPRCR  ======================================================== */
#define R_ICU_SLIPRCR_WPRC_Pos         (0UL)          /*!< WPRC (Bit 0)                                          */
#define R_ICU_SLIPRCR_WPRC_Msk         (0x1UL)        /*!< WPRC (Bitfield-Mask: 0x01)                            */
/* ========================================================  SELEXDR  ======================================================== */
#define R_ICU_SELEXDR_SELEXD0_Pos      (0UL)          /*!< SELEXD0 (Bit 0)                                       */
#define R_ICU_SELEXDR_SELEXD0_Msk      (0x1UL)        /*!< SELEXD0 (Bitfield-Mask: 0x01)                         */
#define R_ICU_SELEXDR_SELEXD1_Pos      (1UL)          /*!< SELEXD1 (Bit 1)                                       */
#define R_ICU_SELEXDR_SELEXD1_Msk      (0x2UL)        /*!< SELEXD1 (Bitfield-Mask: 0x01)                         */

#endif /* R_ICU_POSMSK_H */
