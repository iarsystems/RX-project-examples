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

#ifndef R_SYSTEM_POSMSK_H
#define R_SYSTEM_POSMSK_H

/* =========================================================================================================================== */
/* ================                                          DPSBKR                                           ================ */
/* =========================================================================================================================== */

/* ========================================================  DPSBKR  ========================================================= */
#define R_SYSTEM_DPSBKR_DPSBKR_DPSBKR_Pos    (0UL)    /*!< DPSBKR (Bit 0)                                        */
#define R_SYSTEM_DPSBKR_DPSBKR_DPSBKR_Msk    (0xffUL) /*!< DPSBKR (Bitfield-Mask: 0xff)                          */

/* ================                                         R_SYSTEM                                          ================ */

/* ========================================================  MDMONR  ========================================================= */
#define R_SYSTEM_MDMONR_MD_Pos               (0UL)          /*!< MD (Bit 0)                                            */
#define R_SYSTEM_MDMONR_MD_Msk               (0x1UL)        /*!< MD (Bitfield-Mask: 0x01)                              */
/* ========================================================  SYSCR0  ========================================================= */
#define R_SYSTEM_SYSCR0_ROME_Pos             (0UL)          /*!< ROME (Bit 0)                                          */
#define R_SYSTEM_SYSCR0_ROME_Msk             (0x1UL)        /*!< ROME (Bitfield-Mask: 0x01)                            */
#define R_SYSTEM_SYSCR0_EXBE_Pos             (1UL)          /*!< EXBE (Bit 1)                                          */
#define R_SYSTEM_SYSCR0_EXBE_Msk             (0x2UL)        /*!< EXBE (Bitfield-Mask: 0x01)                            */
#define R_SYSTEM_SYSCR0_KEY_Pos              (8UL)          /*!< KEY (Bit 8)                                           */
#define R_SYSTEM_SYSCR0_KEY_Msk              (0xff00UL)     /*!< KEY (Bitfield-Mask: 0xff)                             */
/* ========================================================  SYSCR1  ========================================================= */
#define R_SYSTEM_SYSCR1_RAME_Pos             (0UL)          /*!< RAME (Bit 0)                                          */
#define R_SYSTEM_SYSCR1_RAME_Msk             (0x1UL)        /*!< RAME (Bitfield-Mask: 0x01)                            */
#define R_SYSTEM_SYSCR1_ECCRAME_Pos          (6UL)          /*!< ECCRAME (Bit 6)                                       */
#define R_SYSTEM_SYSCR1_ECCRAME_Msk          (0x40UL)       /*!< ECCRAME (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_SYSCR1_SBYRAME_Pos          (7UL)          /*!< SBYRAME (Bit 7)                                       */
#define R_SYSTEM_SYSCR1_SBYRAME_Msk          (0x80UL)       /*!< SBYRAME (Bitfield-Mask: 0x01)                         */
/* =========================================================  SBYCR  ========================================================= */
#define R_SYSTEM_SBYCR_OPE_Pos               (14UL)         /*!< OPE (Bit 14)                                          */
#define R_SYSTEM_SBYCR_OPE_Msk               (0x4000UL)     /*!< OPE (Bitfield-Mask: 0x01)                             */
#define R_SYSTEM_SBYCR_SSBY_Pos              (15UL)         /*!< SSBY (Bit 15)                                         */
#define R_SYSTEM_SBYCR_SSBY_Msk              (0x8000UL)     /*!< SSBY (Bitfield-Mask: 0x01)                            */
/* ========================================================  MSTPCRA  ======================================================== */
#define R_SYSTEM_MSTPCRA_MSTPA0_Pos          (0UL)          /*!< MSTPA0 (Bit 0)                                        */
#define R_SYSTEM_MSTPCRA_MSTPA0_Msk          (0x1UL)        /*!< MSTPA0 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_MSTPCRA_MSTPA1_Pos          (1UL)          /*!< MSTPA1 (Bit 1)                                        */
#define R_SYSTEM_MSTPCRA_MSTPA1_Msk          (0x2UL)        /*!< MSTPA1 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_MSTPCRA_MSTPA4_Pos          (4UL)          /*!< MSTPA4 (Bit 4)                                        */
#define R_SYSTEM_MSTPCRA_MSTPA4_Msk          (0x10UL)       /*!< MSTPA4 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_MSTPCRA_MSTPA5_Pos          (5UL)          /*!< MSTPA5 (Bit 5)                                        */
#define R_SYSTEM_MSTPCRA_MSTPA5_Msk          (0x20UL)       /*!< MSTPA5 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_MSTPCRA_MSTPA7_Pos          (7UL)          /*!< MSTPA7 (Bit 7)                                        */
#define R_SYSTEM_MSTPCRA_MSTPA7_Msk          (0x80UL)       /*!< MSTPA7 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_MSTPCRA_MSTPA9_Pos          (9UL)          /*!< MSTPA9 (Bit 9)                                        */
#define R_SYSTEM_MSTPCRA_MSTPA9_Msk          (0x200UL)      /*!< MSTPA9 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_MSTPCRA_MSTPA10_Pos         (10UL)         /*!< MSTPA10 (Bit 10)                                      */
#define R_SYSTEM_MSTPCRA_MSTPA10_Msk         (0x400UL)      /*!< MSTPA10 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_MSTPCRA_MSTPA11_Pos         (11UL)         /*!< MSTPA11 (Bit 11)                                      */
#define R_SYSTEM_MSTPCRA_MSTPA11_Msk         (0x800UL)      /*!< MSTPA11 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_MSTPCRA_MSTPA13_Pos         (13UL)         /*!< MSTPA13 (Bit 13)                                      */
#define R_SYSTEM_MSTPCRA_MSTPA13_Msk         (0x2000UL)     /*!< MSTPA13 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_MSTPCRA_MSTPA14_Pos         (14UL)         /*!< MSTPA14 (Bit 14)                                      */
#define R_SYSTEM_MSTPCRA_MSTPA14_Msk         (0x4000UL)     /*!< MSTPA14 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_MSTPCRA_MSTPA15_Pos         (15UL)         /*!< MSTPA15 (Bit 15)                                      */
#define R_SYSTEM_MSTPCRA_MSTPA15_Msk         (0x8000UL)     /*!< MSTPA15 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_MSTPCRA_MSTPA16_Pos         (16UL)         /*!< MSTPA16 (Bit 16)                                      */
#define R_SYSTEM_MSTPCRA_MSTPA16_Msk         (0x10000UL)    /*!< MSTPA16 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_MSTPCRA_MSTPA17_Pos         (17UL)         /*!< MSTPA17 (Bit 17)                                      */
#define R_SYSTEM_MSTPCRA_MSTPA17_Msk         (0x20000UL)    /*!< MSTPA17 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_MSTPCRA_MSTPA19_Pos         (19UL)         /*!< MSTPA19 (Bit 19)                                      */
#define R_SYSTEM_MSTPCRA_MSTPA19_Msk         (0x80000UL)    /*!< MSTPA19 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_MSTPCRA_MSTPA24_Pos         (24UL)         /*!< MSTPA24 (Bit 24)                                      */
#define R_SYSTEM_MSTPCRA_MSTPA24_Msk         (0x1000000UL)  /*!< MSTPA24 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_MSTPCRA_MSTPA27_Pos         (27UL)         /*!< MSTPA27 (Bit 27)                                      */
#define R_SYSTEM_MSTPCRA_MSTPA27_Msk         (0x8000000UL)  /*!< MSTPA27 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_MSTPCRA_MSTPA28_Pos         (28UL)         /*!< MSTPA28 (Bit 28)                                      */
#define R_SYSTEM_MSTPCRA_MSTPA28_Msk         (0x10000000UL) /*!< MSTPA28 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_MSTPCRA_MSTPA29_Pos         (29UL)         /*!< MSTPA29 (Bit 29)                                      */
#define R_SYSTEM_MSTPCRA_MSTPA29_Msk         (0x20000000UL) /*!< MSTPA29 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_MSTPCRA_ACSE_Pos            (31UL)         /*!< ACSE (Bit 31)                                         */
#define R_SYSTEM_MSTPCRA_ACSE_Msk            (0x80000000UL) /*!< ACSE (Bitfield-Mask: 0x01)                            */
/* ========================================================  MSTPCRB  ======================================================== */
#define R_SYSTEM_MSTPCRB_MSTPB0_Pos          (0UL)          /*!< MSTPB0 (Bit 0)                                        */
#define R_SYSTEM_MSTPCRB_MSTPB0_Msk          (0x1UL)        /*!< MSTPB0 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_MSTPCRB_MSTPB1_Pos          (1UL)          /*!< MSTPB1 (Bit 1)                                        */
#define R_SYSTEM_MSTPCRB_MSTPB1_Msk          (0x2UL)        /*!< MSTPB1 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_MSTPCRB_MSTPB2_Pos          (2UL)          /*!< MSTPB2 (Bit 2)                                        */
#define R_SYSTEM_MSTPCRB_MSTPB2_Msk          (0x4UL)        /*!< MSTPB2 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_MSTPCRB_MSTPB4_Pos          (4UL)          /*!< MSTPB4 (Bit 4)                                        */
#define R_SYSTEM_MSTPCRB_MSTPB4_Msk          (0x10UL)       /*!< MSTPB4 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_MSTPCRB_MSTPB6_Pos          (6UL)          /*!< MSTPB6 (Bit 6)                                        */
#define R_SYSTEM_MSTPCRB_MSTPB6_Msk          (0x40UL)       /*!< MSTPB6 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_MSTPCRB_MSTPB8_Pos          (8UL)          /*!< MSTPB8 (Bit 8)                                        */
#define R_SYSTEM_MSTPCRB_MSTPB8_Msk          (0x100UL)      /*!< MSTPB8 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_MSTPCRB_MSTPB9_Pos          (9UL)          /*!< MSTPB9 (Bit 9)                                        */
#define R_SYSTEM_MSTPCRB_MSTPB9_Msk          (0x200UL)      /*!< MSTPB9 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_MSTPCRB_MSTPB13_Pos         (13UL)         /*!< MSTPB13 (Bit 13)                                      */
#define R_SYSTEM_MSTPCRB_MSTPB13_Msk         (0x2000UL)     /*!< MSTPB13 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_MSTPCRB_MSTPB14_Pos         (14UL)         /*!< MSTPB14 (Bit 14)                                      */
#define R_SYSTEM_MSTPCRB_MSTPB14_Msk         (0x4000UL)     /*!< MSTPB14 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_MSTPCRB_MSTPB15_Pos         (15UL)         /*!< MSTPB15 (Bit 15)                                      */
#define R_SYSTEM_MSTPCRB_MSTPB15_Msk         (0x8000UL)     /*!< MSTPB15 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_MSTPCRB_MSTPB16_Pos         (16UL)         /*!< MSTPB16 (Bit 16)                                      */
#define R_SYSTEM_MSTPCRB_MSTPB16_Msk         (0x10000UL)    /*!< MSTPB16 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_MSTPCRB_MSTPB17_Pos         (17UL)         /*!< MSTPB17 (Bit 17)                                      */
#define R_SYSTEM_MSTPCRB_MSTPB17_Msk         (0x20000UL)    /*!< MSTPB17 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_MSTPCRB_MSTPB19_Pos         (19UL)         /*!< MSTPB19 (Bit 19)                                      */
#define R_SYSTEM_MSTPCRB_MSTPB19_Msk         (0x80000UL)    /*!< MSTPB19 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_MSTPCRB_MSTPB20_Pos         (20UL)         /*!< MSTPB20 (Bit 20)                                      */
#define R_SYSTEM_MSTPCRB_MSTPB20_Msk         (0x100000UL)   /*!< MSTPB20 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_MSTPCRB_MSTPB21_Pos         (21UL)         /*!< MSTPB21 (Bit 21)                                      */
#define R_SYSTEM_MSTPCRB_MSTPB21_Msk         (0x200000UL)   /*!< MSTPB21 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_MSTPCRB_MSTPB22_Pos         (22UL)         /*!< MSTPB22 (Bit 22)                                      */
#define R_SYSTEM_MSTPCRB_MSTPB22_Msk         (0x400000UL)   /*!< MSTPB22 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_MSTPCRB_MSTPB23_Pos         (23UL)         /*!< MSTPB23 (Bit 23)                                      */
#define R_SYSTEM_MSTPCRB_MSTPB23_Msk         (0x800000UL)   /*!< MSTPB23 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_MSTPCRB_MSTPB24_Pos         (24UL)         /*!< MSTPB24 (Bit 24)                                      */
#define R_SYSTEM_MSTPCRB_MSTPB24_Msk         (0x1000000UL)  /*!< MSTPB24 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_MSTPCRB_MSTPB25_Pos         (25UL)         /*!< MSTPB25 (Bit 25)                                      */
#define R_SYSTEM_MSTPCRB_MSTPB25_Msk         (0x2000000UL)  /*!< MSTPB25 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_MSTPCRB_MSTPB26_Pos         (26UL)         /*!< MSTPB26 (Bit 26)                                      */
#define R_SYSTEM_MSTPCRB_MSTPB26_Msk         (0x4000000UL)  /*!< MSTPB26 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_MSTPCRB_MSTPB27_Pos         (27UL)         /*!< MSTPB27 (Bit 27)                                      */
#define R_SYSTEM_MSTPCRB_MSTPB27_Msk         (0x8000000UL)  /*!< MSTPB27 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_MSTPCRB_MSTPB28_Pos         (28UL)         /*!< MSTPB28 (Bit 28)                                      */
#define R_SYSTEM_MSTPCRB_MSTPB28_Msk         (0x10000000UL) /*!< MSTPB28 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_MSTPCRB_MSTPB29_Pos         (29UL)         /*!< MSTPB29 (Bit 29)                                      */
#define R_SYSTEM_MSTPCRB_MSTPB29_Msk         (0x20000000UL) /*!< MSTPB29 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_MSTPCRB_MSTPB30_Pos         (30UL)         /*!< MSTPB30 (Bit 30)                                      */
#define R_SYSTEM_MSTPCRB_MSTPB30_Msk         (0x40000000UL) /*!< MSTPB30 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_MSTPCRB_MSTPB31_Pos         (31UL)         /*!< MSTPB31 (Bit 31)                                      */
#define R_SYSTEM_MSTPCRB_MSTPB31_Msk         (0x80000000UL) /*!< MSTPB31 (Bitfield-Mask: 0x01)                         */
/* ========================================================  MSTPCRC  ======================================================== */
#define R_SYSTEM_MSTPCRC_MSTPC0_Pos          (0UL)          /*!< MSTPC0 (Bit 0)                                        */
#define R_SYSTEM_MSTPCRC_MSTPC0_Msk          (0x1UL)        /*!< MSTPC0 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_MSTPCRC_MSTPC2_Pos          (2UL)          /*!< MSTPC2 (Bit 2)                                        */
#define R_SYSTEM_MSTPCRC_MSTPC2_Msk          (0x4UL)        /*!< MSTPC2 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_MSTPCRC_MSTPC6_Pos          (6UL)          /*!< MSTPC6 (Bit 6)                                        */
#define R_SYSTEM_MSTPCRC_MSTPC6_Msk          (0x40UL)       /*!< MSTPC6 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_MSTPCRC_MSTPC7_Pos          (7UL)          /*!< MSTPC7 (Bit 7)                                        */
#define R_SYSTEM_MSTPCRC_MSTPC7_Msk          (0x80UL)       /*!< MSTPC7 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_MSTPCRC_MSTPC17_Pos         (17UL)         /*!< MSTPC17 (Bit 17)                                      */
#define R_SYSTEM_MSTPCRC_MSTPC17_Msk         (0x20000UL)    /*!< MSTPC17 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_MSTPCRC_MSTPC19_Pos         (19UL)         /*!< MSTPC19 (Bit 19)                                      */
#define R_SYSTEM_MSTPCRC_MSTPC19_Msk         (0x80000UL)    /*!< MSTPC19 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_MSTPCRC_MSTPC22_Pos         (22UL)         /*!< MSTPC22 (Bit 22)                                      */
#define R_SYSTEM_MSTPCRC_MSTPC22_Msk         (0x400000UL)   /*!< MSTPC22 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_MSTPCRC_MSTPC23_Pos         (23UL)         /*!< MSTPC23 (Bit 23)                                      */
#define R_SYSTEM_MSTPCRC_MSTPC23_Msk         (0x800000UL)   /*!< MSTPC23 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_MSTPCRC_MSTPC24_Pos         (24UL)         /*!< MSTPC24 (Bit 24)                                      */
#define R_SYSTEM_MSTPCRC_MSTPC24_Msk         (0x1000000UL)  /*!< MSTPC24 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_MSTPCRC_MSTPC25_Pos         (25UL)         /*!< MSTPC25 (Bit 25)                                      */
#define R_SYSTEM_MSTPCRC_MSTPC25_Msk         (0x2000000UL)  /*!< MSTPC25 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_MSTPCRC_MSTPC26_Pos         (26UL)         /*!< MSTPC26 (Bit 26)                                      */
#define R_SYSTEM_MSTPCRC_MSTPC26_Msk         (0x4000000UL)  /*!< MSTPC26 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_MSTPCRC_MSTPC27_Pos         (27UL)         /*!< MSTPC27 (Bit 27)                                      */
#define R_SYSTEM_MSTPCRC_MSTPC27_Msk         (0x8000000UL)  /*!< MSTPC27 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_MSTPCRC_MSTPC28_Pos         (28UL)         /*!< MSTPC28 (Bit 28)                                      */
#define R_SYSTEM_MSTPCRC_MSTPC28_Msk         (0x10000000UL) /*!< MSTPC28 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_MSTPCRC_MSTPC29_Pos         (29UL)         /*!< MSTPC29 (Bit 29)                                      */
#define R_SYSTEM_MSTPCRC_MSTPC29_Msk         (0x20000000UL) /*!< MSTPC29 (Bitfield-Mask: 0x01)                         */
/* ========================================================  MSTPCRD  ======================================================== */
#define R_SYSTEM_MSTPCRD_MSTPD0_Pos          (0UL)          /*!< MSTPD0 (Bit 0)                                        */
#define R_SYSTEM_MSTPCRD_MSTPD0_Msk          (0x1UL)        /*!< MSTPD0 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_MSTPCRD_MSTPD1_Pos          (1UL)          /*!< MSTPD1 (Bit 1)                                        */
#define R_SYSTEM_MSTPCRD_MSTPD1_Msk          (0x2UL)        /*!< MSTPD1 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_MSTPCRD_MSTPD2_Pos          (2UL)          /*!< MSTPD2 (Bit 2)                                        */
#define R_SYSTEM_MSTPCRD_MSTPD2_Msk          (0x4UL)        /*!< MSTPD2 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_MSTPCRD_MSTPD3_Pos          (3UL)          /*!< MSTPD3 (Bit 3)                                        */
#define R_SYSTEM_MSTPCRD_MSTPD3_Msk          (0x8UL)        /*!< MSTPD3 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_MSTPCRD_MSTPD4_Pos          (4UL)          /*!< MSTPD4 (Bit 4)                                        */
#define R_SYSTEM_MSTPCRD_MSTPD4_Msk          (0x10UL)       /*!< MSTPD4 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_MSTPCRD_MSTPD5_Pos          (5UL)          /*!< MSTPD5 (Bit 5)                                        */
#define R_SYSTEM_MSTPCRD_MSTPD5_Msk          (0x20UL)       /*!< MSTPD5 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_MSTPCRD_MSTPD6_Pos          (6UL)          /*!< MSTPD6 (Bit 6)                                        */
#define R_SYSTEM_MSTPCRD_MSTPD6_Msk          (0x40UL)       /*!< MSTPD6 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_MSTPCRD_MSTPD7_Pos          (7UL)          /*!< MSTPD7 (Bit 7)                                        */
#define R_SYSTEM_MSTPCRD_MSTPD7_Msk          (0x80UL)       /*!< MSTPD7 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_MSTPCRD_MSTPD14_Pos         (14UL)         /*!< MSTPD14 (Bit 14)                                      */
#define R_SYSTEM_MSTPCRD_MSTPD14_Msk         (0x4000UL)     /*!< MSTPD14 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_MSTPCRD_MSTPD15_Pos         (15UL)         /*!< MSTPD15 (Bit 15)                                      */
#define R_SYSTEM_MSTPCRD_MSTPD15_Msk         (0x8000UL)     /*!< MSTPD15 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_MSTPCRD_MSTPD19_Pos         (19UL)         /*!< MSTPD19 (Bit 19)                                      */
#define R_SYSTEM_MSTPCRD_MSTPD19_Msk         (0x80000UL)    /*!< MSTPD19 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_MSTPCRD_MSTPD21_Pos         (21UL)         /*!< MSTPD21 (Bit 21)                                      */
#define R_SYSTEM_MSTPCRD_MSTPD21_Msk         (0x200000UL)   /*!< MSTPD21 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_MSTPCRD_MSTPD27_Pos         (27UL)         /*!< MSTPD27 (Bit 27)                                      */
#define R_SYSTEM_MSTPCRD_MSTPD27_Msk         (0x8000000UL)  /*!< MSTPD27 (Bitfield-Mask: 0x01)                         */
/* =========================================================  SCKCR  ========================================================= */
#define R_SYSTEM_SCKCR_PCKD_Pos              (0UL)          /*!< PCKD (Bit 0)                                          */
#define R_SYSTEM_SCKCR_PCKD_Msk              (0xfUL)        /*!< PCKD (Bitfield-Mask: 0x0f)                            */
#define R_SYSTEM_SCKCR_PCKC_Pos              (4UL)          /*!< PCKC (Bit 4)                                          */
#define R_SYSTEM_SCKCR_PCKC_Msk              (0xf0UL)       /*!< PCKC (Bitfield-Mask: 0x0f)                            */
#define R_SYSTEM_SCKCR_PCKB_Pos              (8UL)          /*!< PCKB (Bit 8)                                          */
#define R_SYSTEM_SCKCR_PCKB_Msk              (0xf00UL)      /*!< PCKB (Bitfield-Mask: 0x0f)                            */
#define R_SYSTEM_SCKCR_PCKA_Pos              (12UL)         /*!< PCKA (Bit 12)                                         */
#define R_SYSTEM_SCKCR_PCKA_Msk              (0xf000UL)     /*!< PCKA (Bitfield-Mask: 0x0f)                            */
#define R_SYSTEM_SCKCR_BCK_Pos               (16UL)         /*!< BCK (Bit 16)                                          */
#define R_SYSTEM_SCKCR_BCK_Msk               (0xf0000UL)    /*!< BCK (Bitfield-Mask: 0x0f)                             */
#define R_SYSTEM_SCKCR_PSTOP0_Pos            (22UL)         /*!< PSTOP0 (Bit 22)                                       */
#define R_SYSTEM_SCKCR_PSTOP0_Msk            (0x400000UL)   /*!< PSTOP0 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_SCKCR_PSTOP1_Pos            (23UL)         /*!< PSTOP1 (Bit 23)                                       */
#define R_SYSTEM_SCKCR_PSTOP1_Msk            (0x800000UL)   /*!< PSTOP1 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_SCKCR_ICK_Pos               (24UL)         /*!< ICK (Bit 24)                                          */
#define R_SYSTEM_SCKCR_ICK_Msk               (0xf000000UL)  /*!< ICK (Bitfield-Mask: 0x0f)                             */
#define R_SYSTEM_SCKCR_FCK_Pos               (28UL)         /*!< FCK (Bit 28)                                          */
#define R_SYSTEM_SCKCR_FCK_Msk               (0xf0000000UL) /*!< FCK (Bitfield-Mask: 0x0f)                             */
/* ========================================================  SCKCR2  ========================================================= */
#define R_SYSTEM_SCKCR2_UCK_Pos              (4UL)          /*!< UCK (Bit 4)                                           */
#define R_SYSTEM_SCKCR2_UCK_Msk              (0xf0UL)       /*!< UCK (Bitfield-Mask: 0x0f)                             */
/* ========================================================  SCKCR3  ========================================================= */
#define R_SYSTEM_SCKCR3_CKSEL_Pos            (8UL)          /*!< CKSEL (Bit 8)                                         */
#define R_SYSTEM_SCKCR3_CKSEL_Msk            (0x700UL)      /*!< CKSEL (Bitfield-Mask: 0x07)                           */
/* =========================================================  PLLCR  ========================================================= */
#define R_SYSTEM_PLLCR_PLIDIV_Pos            (0UL)          /*!< PLIDIV (Bit 0)                                        */
#define R_SYSTEM_PLLCR_PLIDIV_Msk            (0x3UL)        /*!< PLIDIV (Bitfield-Mask: 0x03)                          */
#define R_SYSTEM_PLLCR_PLLSRCSEL_Pos         (4UL)          /*!< PLLSRCSEL (Bit 4)                                     */
#define R_SYSTEM_PLLCR_PLLSRCSEL_Msk         (0x10UL)       /*!< PLLSRCSEL (Bitfield-Mask: 0x01)                       */
#define R_SYSTEM_PLLCR_STC_Pos               (8UL)          /*!< STC (Bit 8)                                           */
#define R_SYSTEM_PLLCR_STC_Msk               (0x3f00UL)     /*!< STC (Bitfield-Mask: 0x3f)                             */
/* ========================================================  PLLCR2  ========================================================= */
#define R_SYSTEM_PLLCR2_PLLEN_Pos            (0UL)          /*!< PLLEN (Bit 0)                                         */
#define R_SYSTEM_PLLCR2_PLLEN_Msk            (0x1UL)        /*!< PLLEN (Bitfield-Mask: 0x01)                           */
/* =========================================================  BCKCR  ========================================================= */
#define R_SYSTEM_BCKCR_BCLKDIV_Pos           (0UL)          /*!< BCLKDIV (Bit 0)                                       */
#define R_SYSTEM_BCKCR_BCLKDIV_Msk           (0x1UL)        /*!< BCLKDIV (Bitfield-Mask: 0x01)                         */
/* ========================================================  MOSCCR  ========================================================= */
#define R_SYSTEM_MOSCCR_MOSTP_Pos            (0UL)          /*!< MOSTP (Bit 0)                                         */
#define R_SYSTEM_MOSCCR_MOSTP_Msk            (0x1UL)        /*!< MOSTP (Bitfield-Mask: 0x01)                           */
/* ========================================================  SOSCCR  ========================================================= */
#define R_SYSTEM_SOSCCR_SOSTP_Pos            (0UL)          /*!< SOSTP (Bit 0)                                         */
#define R_SYSTEM_SOSCCR_SOSTP_Msk            (0x1UL)        /*!< SOSTP (Bitfield-Mask: 0x01)                           */
/* ========================================================  LOCOCR  ========================================================= */
#define R_SYSTEM_LOCOCR_LCSTP_Pos            (0UL)          /*!< LCSTP (Bit 0)                                         */
#define R_SYSTEM_LOCOCR_LCSTP_Msk            (0x1UL)        /*!< LCSTP (Bitfield-Mask: 0x01)                           */
/* ========================================================  ILOCOCR  ======================================================== */
#define R_SYSTEM_ILOCOCR_ILCSTP_Pos          (0UL)          /*!< ILCSTP (Bit 0)                                        */
#define R_SYSTEM_ILOCOCR_ILCSTP_Msk          (0x1UL)        /*!< ILCSTP (Bitfield-Mask: 0x01)                          */
/* ========================================================  HOCOCR  ========================================================= */
#define R_SYSTEM_HOCOCR_HCSTP_Pos            (0UL)          /*!< HCSTP (Bit 0)                                         */
#define R_SYSTEM_HOCOCR_HCSTP_Msk            (0x1UL)        /*!< HCSTP (Bitfield-Mask: 0x01)                           */
/* ========================================================  HOCOCR2  ======================================================== */
#define R_SYSTEM_HOCOCR2_HCFRQ_Pos           (0UL)          /*!< HCFRQ (Bit 0)                                         */
#define R_SYSTEM_HOCOCR2_HCFRQ_Msk           (0x3UL)        /*!< HCFRQ (Bitfield-Mask: 0x03)                           */
/* =======================================================  OSCOVFSR  ======================================================== */
#define R_SYSTEM_OSCOVFSR_MOOVF_Pos          (0UL)          /*!< MOOVF (Bit 0)                                         */
#define R_SYSTEM_OSCOVFSR_MOOVF_Msk          (0x1UL)        /*!< MOOVF (Bitfield-Mask: 0x01)                           */
#define R_SYSTEM_OSCOVFSR_SOOVF_Pos          (1UL)          /*!< SOOVF (Bit 1)                                         */
#define R_SYSTEM_OSCOVFSR_SOOVF_Msk          (0x2UL)        /*!< SOOVF (Bitfield-Mask: 0x01)                           */
#define R_SYSTEM_OSCOVFSR_PLOVF_Pos          (2UL)          /*!< PLOVF (Bit 2)                                         */
#define R_SYSTEM_OSCOVFSR_PLOVF_Msk          (0x4UL)        /*!< PLOVF (Bitfield-Mask: 0x01)                           */
#define R_SYSTEM_OSCOVFSR_HCOVF_Pos          (3UL)          /*!< HCOVF (Bit 3)                                         */
#define R_SYSTEM_OSCOVFSR_HCOVF_Msk          (0x8UL)        /*!< HCOVF (Bitfield-Mask: 0x01)                           */
#define R_SYSTEM_OSCOVFSR_ILCOVF_Pos         (4UL)          /*!< ILCOVF (Bit 4)                                        */
#define R_SYSTEM_OSCOVFSR_ILCOVF_Msk         (0x10UL)       /*!< ILCOVF (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_OSCOVFSR_PPLOVF_Pos         (5UL)          /*!< PPLOVF (Bit 5)                                        */
#define R_SYSTEM_OSCOVFSR_PPLOVF_Msk         (0x20UL)       /*!< PPLOVF (Bitfield-Mask: 0x01)                          */
/* =========================================================  CKOCR  ========================================================= */
#define R_SYSTEM_CKOCR_CKOSEL_Pos            (8UL)          /*!< CKOSEL (Bit 8)                                        */
#define R_SYSTEM_CKOCR_CKOSEL_Msk            (0x700UL)      /*!< CKOSEL (Bitfield-Mask: 0x07)                          */
#define R_SYSTEM_CKOCR_CKODIV_Pos            (12UL)         /*!< CKODIV (Bit 12)                                       */
#define R_SYSTEM_CKOCR_CKODIV_Msk            (0x7000UL)     /*!< CKODIV (Bitfield-Mask: 0x07)                          */
#define R_SYSTEM_CKOCR_CKOSTP_Pos            (15UL)         /*!< CKOSTP (Bit 15)                                       */
#define R_SYSTEM_CKOCR_CKOSTP_Msk            (0x8000UL)     /*!< CKOSTP (Bitfield-Mask: 0x01)                          */
/* ========================================================  OSTDCR  ========================================================= */
#define R_SYSTEM_OSTDCR_OSTDIE_Pos           (0UL)          /*!< OSTDIE (Bit 0)                                        */
#define R_SYSTEM_OSTDCR_OSTDIE_Msk           (0x1UL)        /*!< OSTDIE (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_OSTDCR_OSTDE_Pos            (7UL)          /*!< OSTDE (Bit 7)                                         */
#define R_SYSTEM_OSTDCR_OSTDE_Msk            (0x80UL)       /*!< OSTDE (Bitfield-Mask: 0x01)                           */
/* ========================================================  OSTDSR  ========================================================= */
#define R_SYSTEM_OSTDSR_OSTDF_Pos            (0UL)          /*!< OSTDF (Bit 0)                                         */
#define R_SYSTEM_OSTDSR_OSTDF_Msk            (0x1UL)        /*!< OSTDF (Bitfield-Mask: 0x01)                           */
/* ========================================================  PACKCR  ========================================================= */
#define R_SYSTEM_PACKCR_OUTCKSEL_Pos         (4UL)          /*!< OUTCKSEL (Bit 4)                                      */
#define R_SYSTEM_PACKCR_OUTCKSEL_Msk         (0x10UL)       /*!< OUTCKSEL (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_PACKCR_UPLLSEL_Pos          (12UL)         /*!< UPLLSEL (Bit 12)                                      */
#define R_SYSTEM_PACKCR_UPLLSEL_Msk          (0x1000UL)     /*!< UPLLSEL (Bitfield-Mask: 0x01)                         */
/* ========================================================  PPLLCR  ========================================================= */
#define R_SYSTEM_PPLLCR_PPLIDIV_Pos          (0UL)          /*!< PPLIDIV (Bit 0)                                       */
#define R_SYSTEM_PPLLCR_PPLIDIV_Msk          (0x3UL)        /*!< PPLIDIV (Bitfield-Mask: 0x03)                         */
#define R_SYSTEM_PPLLCR_PPLSTC_Pos           (8UL)          /*!< PPLSTC (Bit 8)                                        */
#define R_SYSTEM_PPLLCR_PPLSTC_Msk           (0x3f00UL)     /*!< PPLSTC (Bitfield-Mask: 0x3f)                          */
/* ========================================================  PPLLCR2  ======================================================== */
#define R_SYSTEM_PPLLCR2_PPLLEN_Pos          (0UL)          /*!< PPLLEN (Bit 0)                                        */
#define R_SYSTEM_PPLLCR2_PPLLEN_Msk          (0x1UL)        /*!< PPLLEN (Bitfield-Mask: 0x01)                          */
/* ========================================================  PPLLCR3  ======================================================== */
#define R_SYSTEM_PPLLCR3_PPLCK_Pos           (0UL)          /*!< PPLCK (Bit 0)                                         */
#define R_SYSTEM_PPLLCR3_PPLCK_Msk           (0xfUL)        /*!< PPLCK (Bitfield-Mask: 0x0f)                           */
/* =========================================================  OPCCR  ========================================================= */
#define R_SYSTEM_OPCCR_OPCM_Pos              (0UL)          /*!< OPCM (Bit 0)                                          */
#define R_SYSTEM_OPCCR_OPCM_Msk              (0x7UL)        /*!< OPCM (Bitfield-Mask: 0x07)                            */
#define R_SYSTEM_OPCCR_OPCMTSF_Pos           (4UL)          /*!< OPCMTSF (Bit 4)                                       */
#define R_SYSTEM_OPCCR_OPCMTSF_Msk           (0x10UL)       /*!< OPCMTSF (Bitfield-Mask: 0x01)                         */
/* ========================================================  RSTCKCR  ======================================================== */
#define R_SYSTEM_RSTCKCR_RSTCKSEL_Pos        (0UL)          /*!< RSTCKSEL (Bit 0)                                      */
#define R_SYSTEM_RSTCKCR_RSTCKSEL_Msk        (0x7UL)        /*!< RSTCKSEL (Bitfield-Mask: 0x07)                        */
#define R_SYSTEM_RSTCKCR_RSTCKEN_Pos         (7UL)          /*!< RSTCKEN (Bit 7)                                       */
#define R_SYSTEM_RSTCKCR_RSTCKEN_Msk         (0x80UL)       /*!< RSTCKEN (Bitfield-Mask: 0x01)                         */
/* =======================================================  MOSCWTCR  ======================================================== */
#define R_SYSTEM_MOSCWTCR_MSTS_Pos           (0UL)          /*!< MSTS (Bit 0)                                          */
#define R_SYSTEM_MOSCWTCR_MSTS_Msk           (0xffUL)       /*!< MSTS (Bitfield-Mask: 0xff)                            */
/* =======================================================  SOSCWTCR  ======================================================== */
#define R_SYSTEM_SOSCWTCR_SSTS_Pos           (0UL)          /*!< SSTS (Bit 0)                                          */
#define R_SYSTEM_SOSCWTCR_SSTS_Msk           (0xffUL)       /*!< SSTS (Bitfield-Mask: 0xff)                            */
/* ========================================================  RSTSR2  ========================================================= */
#define R_SYSTEM_RSTSR2_IWDTRF_Pos           (0UL)          /*!< IWDTRF (Bit 0)                                        */
#define R_SYSTEM_RSTSR2_IWDTRF_Msk           (0x1UL)        /*!< IWDTRF (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_RSTSR2_WDTRF_Pos            (1UL)          /*!< WDTRF (Bit 1)                                         */
#define R_SYSTEM_RSTSR2_WDTRF_Msk            (0x2UL)        /*!< WDTRF (Bitfield-Mask: 0x01)                           */
#define R_SYSTEM_RSTSR2_SWRF_Pos             (2UL)          /*!< SWRF (Bit 2)                                          */
#define R_SYSTEM_RSTSR2_SWRF_Msk             (0x4UL)        /*!< SWRF (Bitfield-Mask: 0x01)                            */
/* =========================================================  SWRR  ========================================================== */
#define R_SYSTEM_SWRR_SWRR_Pos               (0UL)          /*!< SWRR (Bit 0)                                          */
#define R_SYSTEM_SWRR_SWRR_Msk               (0xffffUL)     /*!< SWRR (Bitfield-Mask: 0xffff)                          */
/* ========================================================  LVD1CR1  ======================================================== */
#define R_SYSTEM_LVD1CR1_LVD1IDTSEL_Pos      (0UL)          /*!< LVD1IDTSEL (Bit 0)                                    */
#define R_SYSTEM_LVD1CR1_LVD1IDTSEL_Msk      (0x3UL)        /*!< LVD1IDTSEL (Bitfield-Mask: 0x03)                      */
#define R_SYSTEM_LVD1CR1_LVD1IRQSEL_Pos      (2UL)          /*!< LVD1IRQSEL (Bit 2)                                    */
#define R_SYSTEM_LVD1CR1_LVD1IRQSEL_Msk      (0x4UL)        /*!< LVD1IRQSEL (Bitfield-Mask: 0x01)                      */
/* ========================================================  LVD1SR  ========================================================= */
#define R_SYSTEM_LVD1SR_LVD1DET_Pos          (0UL)          /*!< LVD1DET (Bit 0)                                       */
#define R_SYSTEM_LVD1SR_LVD1DET_Msk          (0x1UL)        /*!< LVD1DET (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_LVD1SR_LVD1MON_Pos          (1UL)          /*!< LVD1MON (Bit 1)                                       */
#define R_SYSTEM_LVD1SR_LVD1MON_Msk          (0x2UL)        /*!< LVD1MON (Bitfield-Mask: 0x01)                         */
/* ========================================================  LVD2CR1  ======================================================== */
#define R_SYSTEM_LVD2CR1_LVD2IDTSEL_Pos      (0UL)          /*!< LVD2IDTSEL (Bit 0)                                    */
#define R_SYSTEM_LVD2CR1_LVD2IDTSEL_Msk      (0x3UL)        /*!< LVD2IDTSEL (Bitfield-Mask: 0x03)                      */
#define R_SYSTEM_LVD2CR1_LVD2IRQSEL_Pos      (2UL)          /*!< LVD2IRQSEL (Bit 2)                                    */
#define R_SYSTEM_LVD2CR1_LVD2IRQSEL_Msk      (0x4UL)        /*!< LVD2IRQSEL (Bitfield-Mask: 0x01)                      */
/* ========================================================  LVD2SR  ========================================================= */
#define R_SYSTEM_LVD2SR_LVD2DET_Pos          (0UL)          /*!< LVD2DET (Bit 0)                                       */
#define R_SYSTEM_LVD2SR_LVD2DET_Msk          (0x1UL)        /*!< LVD2DET (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_LVD2SR_LVD2MON_Pos          (1UL)          /*!< LVD2MON (Bit 1)                                       */
#define R_SYSTEM_LVD2SR_LVD2MON_Msk          (0x2UL)        /*!< LVD2MON (Bitfield-Mask: 0x01)                         */
/* =========================================================  PRCR  ========================================================== */
#define R_SYSTEM_PRCR_PRC0_Pos               (0UL)          /*!< PRC0 (Bit 0)                                          */
#define R_SYSTEM_PRCR_PRC0_Msk               (0x1UL)        /*!< PRC0 (Bitfield-Mask: 0x01)                            */
#define R_SYSTEM_PRCR_PRC1_Pos               (1UL)          /*!< PRC1 (Bit 1)                                          */
#define R_SYSTEM_PRCR_PRC1_Msk               (0x2UL)        /*!< PRC1 (Bitfield-Mask: 0x01)                            */
#define R_SYSTEM_PRCR_PRC3_Pos               (3UL)          /*!< PRC3 (Bit 3)                                          */
#define R_SYSTEM_PRCR_PRC3_Msk               (0x8UL)        /*!< PRC3 (Bitfield-Mask: 0x01)                            */
#define R_SYSTEM_PRCR_PRKEY_Pos              (8UL)          /*!< PRKEY (Bit 8)                                         */
#define R_SYSTEM_PRCR_PRKEY_Msk              (0xff00UL)     /*!< PRKEY (Bitfield-Mask: 0xff)                           */
/* ========================================================  MEMWAIT  ======================================================== */
#define R_SYSTEM_MEMWAIT_MEMWAIT_Pos         (0UL)          /*!< MEMWAIT (Bit 0)                                       */
#define R_SYSTEM_MEMWAIT_MEMWAIT_Msk         (0x1UL)        /*!< MEMWAIT (Bitfield-Mask: 0x01)                         */
/* ========================================================  DPSBYCR  ======================================================== */
#define R_SYSTEM_DPSBYCR_DEEPCUT_Pos         (0UL)          /*!< DEEPCUT (Bit 0)                                       */
#define R_SYSTEM_DPSBYCR_DEEPCUT_Msk         (0x3UL)        /*!< DEEPCUT (Bitfield-Mask: 0x03)                         */
#define R_SYSTEM_DPSBYCR_IOKEEP_Pos          (6UL)          /*!< IOKEEP (Bit 6)                                        */
#define R_SYSTEM_DPSBYCR_IOKEEP_Msk          (0x40UL)       /*!< IOKEEP (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_DPSBYCR_DPSBY_Pos           (7UL)          /*!< DPSBY (Bit 7)                                         */
#define R_SYSTEM_DPSBYCR_DPSBY_Msk           (0x80UL)       /*!< DPSBY (Bitfield-Mask: 0x01)                           */
/* ========================================================  DPSIER0  ======================================================== */
#define R_SYSTEM_DPSIER0_DIRQ0E_Pos          (0UL)          /*!< DIRQ0E (Bit 0)                                        */
#define R_SYSTEM_DPSIER0_DIRQ0E_Msk          (0x1UL)        /*!< DIRQ0E (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_DPSIER0_DIRQ1E_Pos          (1UL)          /*!< DIRQ1E (Bit 1)                                        */
#define R_SYSTEM_DPSIER0_DIRQ1E_Msk          (0x2UL)        /*!< DIRQ1E (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_DPSIER0_DIRQ2E_Pos          (2UL)          /*!< DIRQ2E (Bit 2)                                        */
#define R_SYSTEM_DPSIER0_DIRQ2E_Msk          (0x4UL)        /*!< DIRQ2E (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_DPSIER0_DIRQ3E_Pos          (3UL)          /*!< DIRQ3E (Bit 3)                                        */
#define R_SYSTEM_DPSIER0_DIRQ3E_Msk          (0x8UL)        /*!< DIRQ3E (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_DPSIER0_DIRQ4E_Pos          (4UL)          /*!< DIRQ4E (Bit 4)                                        */
#define R_SYSTEM_DPSIER0_DIRQ4E_Msk          (0x10UL)       /*!< DIRQ4E (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_DPSIER0_DIRQ5E_Pos          (5UL)          /*!< DIRQ5E (Bit 5)                                        */
#define R_SYSTEM_DPSIER0_DIRQ5E_Msk          (0x20UL)       /*!< DIRQ5E (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_DPSIER0_DIRQ6E_Pos          (6UL)          /*!< DIRQ6E (Bit 6)                                        */
#define R_SYSTEM_DPSIER0_DIRQ6E_Msk          (0x40UL)       /*!< DIRQ6E (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_DPSIER0_DIRQ7E_Pos          (7UL)          /*!< DIRQ7E (Bit 7)                                        */
#define R_SYSTEM_DPSIER0_DIRQ7E_Msk          (0x80UL)       /*!< DIRQ7E (Bitfield-Mask: 0x01)                          */
/* ========================================================  DPSIER1  ======================================================== */
#define R_SYSTEM_DPSIER1_DIRQ8E_Pos          (0UL)          /*!< DIRQ8E (Bit 0)                                        */
#define R_SYSTEM_DPSIER1_DIRQ8E_Msk          (0x1UL)        /*!< DIRQ8E (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_DPSIER1_DIRQ9E_Pos          (1UL)          /*!< DIRQ9E (Bit 1)                                        */
#define R_SYSTEM_DPSIER1_DIRQ9E_Msk          (0x2UL)        /*!< DIRQ9E (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_DPSIER1_DIRQ10E_Pos         (2UL)          /*!< DIRQ10E (Bit 2)                                       */
#define R_SYSTEM_DPSIER1_DIRQ10E_Msk         (0x4UL)        /*!< DIRQ10E (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIER1_DIRQ11E_Pos         (3UL)          /*!< DIRQ11E (Bit 3)                                       */
#define R_SYSTEM_DPSIER1_DIRQ11E_Msk         (0x8UL)        /*!< DIRQ11E (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIER1_DIRQ12E_Pos         (4UL)          /*!< DIRQ12E (Bit 4)                                       */
#define R_SYSTEM_DPSIER1_DIRQ12E_Msk         (0x10UL)       /*!< DIRQ12E (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIER1_DIRQ13E_Pos         (5UL)          /*!< DIRQ13E (Bit 5)                                       */
#define R_SYSTEM_DPSIER1_DIRQ13E_Msk         (0x20UL)       /*!< DIRQ13E (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIER1_DIRQ14E_Pos         (6UL)          /*!< DIRQ14E (Bit 6)                                       */
#define R_SYSTEM_DPSIER1_DIRQ14E_Msk         (0x40UL)       /*!< DIRQ14E (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIER1_DIRQ15E_Pos         (7UL)          /*!< DIRQ15E (Bit 7)                                       */
#define R_SYSTEM_DPSIER1_DIRQ15E_Msk         (0x80UL)       /*!< DIRQ15E (Bitfield-Mask: 0x01)                         */
/* ========================================================  DPSIER2  ======================================================== */
#define R_SYSTEM_DPSIER2_DLVD1IE_Pos         (0UL)          /*!< DLVD1IE (Bit 0)                                       */
#define R_SYSTEM_DPSIER2_DLVD1IE_Msk         (0x1UL)        /*!< DLVD1IE (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIER2_DLVD2IE_Pos         (1UL)          /*!< DLVD2IE (Bit 1)                                       */
#define R_SYSTEM_DPSIER2_DLVD2IE_Msk         (0x2UL)        /*!< DLVD2IE (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIER2_DRTCIIE_Pos         (2UL)          /*!< DRTCIIE (Bit 2)                                       */
#define R_SYSTEM_DPSIER2_DRTCIIE_Msk         (0x4UL)        /*!< DRTCIIE (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIER2_DRTCAIE_Pos         (3UL)          /*!< DRTCAIE (Bit 3)                                       */
#define R_SYSTEM_DPSIER2_DRTCAIE_Msk         (0x8UL)        /*!< DRTCAIE (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIER2_DNMIE_Pos           (4UL)          /*!< DNMIE (Bit 4)                                         */
#define R_SYSTEM_DPSIER2_DNMIE_Msk           (0x10UL)       /*!< DNMIE (Bitfield-Mask: 0x01)                           */
#define R_SYSTEM_DPSIER2_DRIICDIE_Pos        (5UL)          /*!< DRIICDIE (Bit 5)                                      */
#define R_SYSTEM_DPSIER2_DRIICDIE_Msk        (0x20UL)       /*!< DRIICDIE (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_DPSIER2_DRIICCIE_Pos        (6UL)          /*!< DRIICCIE (Bit 6)                                      */
#define R_SYSTEM_DPSIER2_DRIICCIE_Msk        (0x40UL)       /*!< DRIICCIE (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_DPSIER2_DUSBIE_Pos          (7UL)          /*!< DUSBIE (Bit 7)                                        */
#define R_SYSTEM_DPSIER2_DUSBIE_Msk          (0x80UL)       /*!< DUSBIE (Bitfield-Mask: 0x01)                          */
/* ========================================================  DPSIER3  ======================================================== */
#define R_SYSTEM_DPSIER3_DCANIE_Pos          (0UL)          /*!< DCANIE (Bit 0)                                        */
#define R_SYSTEM_DPSIER3_DCANIE_Msk          (0x1UL)        /*!< DCANIE (Bitfield-Mask: 0x01)                          */
/* ========================================================  DPSIFR0  ======================================================== */
#define R_SYSTEM_DPSIFR0_DIRQ0F_Pos          (0UL)          /*!< DIRQ0F (Bit 0)                                        */
#define R_SYSTEM_DPSIFR0_DIRQ0F_Msk          (0x1UL)        /*!< DIRQ0F (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_DPSIFR0_DIRQ1F_Pos          (1UL)          /*!< DIRQ1F (Bit 1)                                        */
#define R_SYSTEM_DPSIFR0_DIRQ1F_Msk          (0x2UL)        /*!< DIRQ1F (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_DPSIFR0_DIRQ2F_Pos          (2UL)          /*!< DIRQ2F (Bit 2)                                        */
#define R_SYSTEM_DPSIFR0_DIRQ2F_Msk          (0x4UL)        /*!< DIRQ2F (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_DPSIFR0_DIRQ3F_Pos          (3UL)          /*!< DIRQ3F (Bit 3)                                        */
#define R_SYSTEM_DPSIFR0_DIRQ3F_Msk          (0x8UL)        /*!< DIRQ3F (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_DPSIFR0_DIRQ4F_Pos          (4UL)          /*!< DIRQ4F (Bit 4)                                        */
#define R_SYSTEM_DPSIFR0_DIRQ4F_Msk          (0x10UL)       /*!< DIRQ4F (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_DPSIFR0_DIRQ5F_Pos          (5UL)          /*!< DIRQ5F (Bit 5)                                        */
#define R_SYSTEM_DPSIFR0_DIRQ5F_Msk          (0x20UL)       /*!< DIRQ5F (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_DPSIFR0_DIRQ6F_Pos          (6UL)          /*!< DIRQ6F (Bit 6)                                        */
#define R_SYSTEM_DPSIFR0_DIRQ6F_Msk          (0x40UL)       /*!< DIRQ6F (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_DPSIFR0_DIRQ7F_Pos          (7UL)          /*!< DIRQ7F (Bit 7)                                        */
#define R_SYSTEM_DPSIFR0_DIRQ7F_Msk          (0x80UL)       /*!< DIRQ7F (Bitfield-Mask: 0x01)                          */
/* ========================================================  DPSIFR1  ======================================================== */
#define R_SYSTEM_DPSIFR1_DIRQ8F_Pos          (0UL)          /*!< DIRQ8F (Bit 0)                                        */
#define R_SYSTEM_DPSIFR1_DIRQ8F_Msk          (0x1UL)        /*!< DIRQ8F (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_DPSIFR1_DIRQ9F_Pos          (1UL)          /*!< DIRQ9F (Bit 1)                                        */
#define R_SYSTEM_DPSIFR1_DIRQ9F_Msk          (0x2UL)        /*!< DIRQ9F (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_DPSIFR1_DIRQ10F_Pos         (2UL)          /*!< DIRQ10F (Bit 2)                                       */
#define R_SYSTEM_DPSIFR1_DIRQ10F_Msk         (0x4UL)        /*!< DIRQ10F (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIFR1_DIRQ11F_Pos         (3UL)          /*!< DIRQ11F (Bit 3)                                       */
#define R_SYSTEM_DPSIFR1_DIRQ11F_Msk         (0x8UL)        /*!< DIRQ11F (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIFR1_DIRQ12F_Pos         (4UL)          /*!< DIRQ12F (Bit 4)                                       */
#define R_SYSTEM_DPSIFR1_DIRQ12F_Msk         (0x10UL)       /*!< DIRQ12F (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIFR1_DIRQ13F_Pos         (5UL)          /*!< DIRQ13F (Bit 5)                                       */
#define R_SYSTEM_DPSIFR1_DIRQ13F_Msk         (0x20UL)       /*!< DIRQ13F (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIFR1_DIRQ14F_Pos         (6UL)          /*!< DIRQ14F (Bit 6)                                       */
#define R_SYSTEM_DPSIFR1_DIRQ14F_Msk         (0x40UL)       /*!< DIRQ14F (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIFR1_DIRQ15F_Pos         (7UL)          /*!< DIRQ15F (Bit 7)                                       */
#define R_SYSTEM_DPSIFR1_DIRQ15F_Msk         (0x80UL)       /*!< DIRQ15F (Bitfield-Mask: 0x01)                         */
/* ========================================================  DPSIFR2  ======================================================== */
#define R_SYSTEM_DPSIFR2_DLVD1IF_Pos         (0UL)          /*!< DLVD1IF (Bit 0)                                       */
#define R_SYSTEM_DPSIFR2_DLVD1IF_Msk         (0x1UL)        /*!< DLVD1IF (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIFR2_DLVD2IF_Pos         (1UL)          /*!< DLVD2IF (Bit 1)                                       */
#define R_SYSTEM_DPSIFR2_DLVD2IF_Msk         (0x2UL)        /*!< DLVD2IF (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIFR2_DRTCIIF_Pos         (2UL)          /*!< DRTCIIF (Bit 2)                                       */
#define R_SYSTEM_DPSIFR2_DRTCIIF_Msk         (0x4UL)        /*!< DRTCIIF (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIFR2_DRTCAIF_Pos         (3UL)          /*!< DRTCAIF (Bit 3)                                       */
#define R_SYSTEM_DPSIFR2_DRTCAIF_Msk         (0x8UL)        /*!< DRTCAIF (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIFR2_DNMIF_Pos           (4UL)          /*!< DNMIF (Bit 4)                                         */
#define R_SYSTEM_DPSIFR2_DNMIF_Msk           (0x10UL)       /*!< DNMIF (Bitfield-Mask: 0x01)                           */
#define R_SYSTEM_DPSIFR2_DRIICDIF_Pos        (5UL)          /*!< DRIICDIF (Bit 5)                                      */
#define R_SYSTEM_DPSIFR2_DRIICDIF_Msk        (0x20UL)       /*!< DRIICDIF (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_DPSIFR2_DRIICCIF_Pos        (6UL)          /*!< DRIICCIF (Bit 6)                                      */
#define R_SYSTEM_DPSIFR2_DRIICCIF_Msk        (0x40UL)       /*!< DRIICCIF (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_DPSIFR2_DUSBIF_Pos          (7UL)          /*!< DUSBIF (Bit 7)                                        */
#define R_SYSTEM_DPSIFR2_DUSBIF_Msk          (0x80UL)       /*!< DUSBIF (Bitfield-Mask: 0x01)                          */
/* ========================================================  DPSIFR3  ======================================================== */
#define R_SYSTEM_DPSIFR3_DCANIF_Pos          (0UL)          /*!< DCANIF (Bit 0)                                        */
#define R_SYSTEM_DPSIFR3_DCANIF_Msk          (0x1UL)        /*!< DCANIF (Bitfield-Mask: 0x01)                          */
/* =======================================================  DPSIEGR0  ======================================================== */
#define R_SYSTEM_DPSIEGR0_DIRQ0EG_Pos        (0UL)          /*!< DIRQ0EG (Bit 0)                                       */
#define R_SYSTEM_DPSIEGR0_DIRQ0EG_Msk        (0x1UL)        /*!< DIRQ0EG (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIEGR0_DIRQ1EG_Pos        (1UL)          /*!< DIRQ1EG (Bit 1)                                       */
#define R_SYSTEM_DPSIEGR0_DIRQ1EG_Msk        (0x2UL)        /*!< DIRQ1EG (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIEGR0_DIRQ2EG_Pos        (2UL)          /*!< DIRQ2EG (Bit 2)                                       */
#define R_SYSTEM_DPSIEGR0_DIRQ2EG_Msk        (0x4UL)        /*!< DIRQ2EG (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIEGR0_DIRQ3EG_Pos        (3UL)          /*!< DIRQ3EG (Bit 3)                                       */
#define R_SYSTEM_DPSIEGR0_DIRQ3EG_Msk        (0x8UL)        /*!< DIRQ3EG (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIEGR0_DIRQ4EG_Pos        (4UL)          /*!< DIRQ4EG (Bit 4)                                       */
#define R_SYSTEM_DPSIEGR0_DIRQ4EG_Msk        (0x10UL)       /*!< DIRQ4EG (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIEGR0_DIRQ5EG_Pos        (5UL)          /*!< DIRQ5EG (Bit 5)                                       */
#define R_SYSTEM_DPSIEGR0_DIRQ5EG_Msk        (0x20UL)       /*!< DIRQ5EG (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIEGR0_DIRQ6EG_Pos        (6UL)          /*!< DIRQ6EG (Bit 6)                                       */
#define R_SYSTEM_DPSIEGR0_DIRQ6EG_Msk        (0x40UL)       /*!< DIRQ6EG (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIEGR0_DIRQ7EG_Pos        (7UL)          /*!< DIRQ7EG (Bit 7)                                       */
#define R_SYSTEM_DPSIEGR0_DIRQ7EG_Msk        (0x80UL)       /*!< DIRQ7EG (Bitfield-Mask: 0x01)                         */
/* =======================================================  DPSIEGR1  ======================================================== */
#define R_SYSTEM_DPSIEGR1_DIRQ8EG_Pos        (0UL)          /*!< DIRQ8EG (Bit 0)                                       */
#define R_SYSTEM_DPSIEGR1_DIRQ8EG_Msk        (0x1UL)        /*!< DIRQ8EG (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIEGR1_DIRQ9EG_Pos        (1UL)          /*!< DIRQ9EG (Bit 1)                                       */
#define R_SYSTEM_DPSIEGR1_DIRQ9EG_Msk        (0x2UL)        /*!< DIRQ9EG (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIEGR1_DIRQ10EG_Pos       (2UL)          /*!< DIRQ10EG (Bit 2)                                      */
#define R_SYSTEM_DPSIEGR1_DIRQ10EG_Msk       (0x4UL)        /*!< DIRQ10EG (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_DPSIEGR1_DIRQ11EG_Pos       (3UL)          /*!< DIRQ11EG (Bit 3)                                      */
#define R_SYSTEM_DPSIEGR1_DIRQ11EG_Msk       (0x8UL)        /*!< DIRQ11EG (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_DPSIEGR1_DIRQ12EG_Pos       (4UL)          /*!< DIRQ12EG (Bit 4)                                      */
#define R_SYSTEM_DPSIEGR1_DIRQ12EG_Msk       (0x10UL)       /*!< DIRQ12EG (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_DPSIEGR1_DIRQ13EG_Pos       (5UL)          /*!< DIRQ13EG (Bit 5)                                      */
#define R_SYSTEM_DPSIEGR1_DIRQ13EG_Msk       (0x20UL)       /*!< DIRQ13EG (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_DPSIEGR1_DIRQ14EG_Pos       (6UL)          /*!< DIRQ14EG (Bit 6)                                      */
#define R_SYSTEM_DPSIEGR1_DIRQ14EG_Msk       (0x40UL)       /*!< DIRQ14EG (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_DPSIEGR1_DIRQ15EG_Pos       (7UL)          /*!< DIRQ15EG (Bit 7)                                      */
#define R_SYSTEM_DPSIEGR1_DIRQ15EG_Msk       (0x80UL)       /*!< DIRQ15EG (Bitfield-Mask: 0x01)                        */
/* =======================================================  DPSIEGR2  ======================================================== */
#define R_SYSTEM_DPSIEGR2_DLVD1EG_Pos        (0UL)          /*!< DLVD1EG (Bit 0)                                       */
#define R_SYSTEM_DPSIEGR2_DLVD1EG_Msk        (0x1UL)        /*!< DLVD1EG (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIEGR2_DLVD2EG_Pos        (1UL)          /*!< DLVD2EG (Bit 1)                                       */
#define R_SYSTEM_DPSIEGR2_DLVD2EG_Msk        (0x2UL)        /*!< DLVD2EG (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIEGR2_DNMIEG_Pos         (4UL)          /*!< DNMIEG (Bit 4)                                        */
#define R_SYSTEM_DPSIEGR2_DNMIEG_Msk         (0x10UL)       /*!< DNMIEG (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_DPSIEGR2_DRIICDEG_Pos       (5UL)          /*!< DRIICDEG (Bit 5)                                      */
#define R_SYSTEM_DPSIEGR2_DRIICDEG_Msk       (0x20UL)       /*!< DRIICDEG (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_DPSIEGR2_DRIICCEG_Pos       (6UL)          /*!< DRIICCEG (Bit 6)                                      */
#define R_SYSTEM_DPSIEGR2_DRIICCEG_Msk       (0x40UL)       /*!< DRIICCEG (Bitfield-Mask: 0x01)                        */
/* =======================================================  DPSIEGR3  ======================================================== */
#define R_SYSTEM_DPSIEGR3_DCANIEG_Pos        (0UL)          /*!< DCANIEG (Bit 0)                                       */
#define R_SYSTEM_DPSIEGR3_DCANIEG_Msk        (0x1UL)        /*!< DCANIEG (Bitfield-Mask: 0x01)                         */
/* ========================================================  RSTSR0  ========================================================= */
#define R_SYSTEM_RSTSR0_PORF_Pos             (0UL)          /*!< PORF (Bit 0)                                          */
#define R_SYSTEM_RSTSR0_PORF_Msk             (0x1UL)        /*!< PORF (Bitfield-Mask: 0x01)                            */
#define R_SYSTEM_RSTSR0_LVD0RF_Pos           (1UL)          /*!< LVD0RF (Bit 1)                                        */
#define R_SYSTEM_RSTSR0_LVD0RF_Msk           (0x2UL)        /*!< LVD0RF (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_RSTSR0_LVD1RF_Pos           (2UL)          /*!< LVD1RF (Bit 2)                                        */
#define R_SYSTEM_RSTSR0_LVD1RF_Msk           (0x4UL)        /*!< LVD1RF (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_RSTSR0_LVD2RF_Pos           (3UL)          /*!< LVD2RF (Bit 3)                                        */
#define R_SYSTEM_RSTSR0_LVD2RF_Msk           (0x8UL)        /*!< LVD2RF (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_RSTSR0_DPSRSTF_Pos          (7UL)          /*!< DPSRSTF (Bit 7)                                       */
#define R_SYSTEM_RSTSR0_DPSRSTF_Msk          (0x80UL)       /*!< DPSRSTF (Bitfield-Mask: 0x01)                         */
/* ========================================================  RSTSR1  ========================================================= */
#define R_SYSTEM_RSTSR1_CWSF_Pos             (0UL)          /*!< CWSF (Bit 0)                                          */
#define R_SYSTEM_RSTSR1_CWSF_Msk             (0x1UL)        /*!< CWSF (Bitfield-Mask: 0x01)                            */
/* =========================================================  MOFCR  ========================================================= */
#define R_SYSTEM_MOFCR_MOFXIN_Pos            (0UL)          /*!< MOFXIN (Bit 0)                                        */
#define R_SYSTEM_MOFCR_MOFXIN_Msk            (0x1UL)        /*!< MOFXIN (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_MOFCR_MODRV2_Pos            (4UL)          /*!< MODRV2 (Bit 4)                                        */
#define R_SYSTEM_MOFCR_MODRV2_Msk            (0x30UL)       /*!< MODRV2 (Bitfield-Mask: 0x03)                          */
#define R_SYSTEM_MOFCR_MOSEL_Pos             (6UL)          /*!< MOSEL (Bit 6)                                         */
#define R_SYSTEM_MOFCR_MOSEL_Msk             (0x40UL)       /*!< MOSEL (Bitfield-Mask: 0x01)                           */
/* ========================================================  HOCOPCR  ======================================================== */
#define R_SYSTEM_HOCOPCR_HOCOPCNT_Pos        (0UL)          /*!< HOCOPCNT (Bit 0)                                      */
#define R_SYSTEM_HOCOPCR_HOCOPCNT_Msk        (0x1UL)        /*!< HOCOPCNT (Bitfield-Mask: 0x01)                        */
/* ========================================================  FWEPROR  ======================================================== */
#define R_SYSTEM_FWEPROR_FLWE_Pos            (0UL)          /*!< FLWE (Bit 0)                                          */
#define R_SYSTEM_FWEPROR_FLWE_Msk            (0x3UL)        /*!< FLWE (Bitfield-Mask: 0x03)                            */
/* ========================================================  LVCMPCR  ======================================================== */
#define R_SYSTEM_LVCMPCR_LVD1E_Pos           (5UL)          /*!< LVD1E (Bit 5)                                         */
#define R_SYSTEM_LVCMPCR_LVD1E_Msk           (0x20UL)       /*!< LVD1E (Bitfield-Mask: 0x01)                           */
#define R_SYSTEM_LVCMPCR_LVD2E_Pos           (6UL)          /*!< LVD2E (Bit 6)                                         */
#define R_SYSTEM_LVCMPCR_LVD2E_Msk           (0x40UL)       /*!< LVD2E (Bitfield-Mask: 0x01)                           */
/* ========================================================  LVDLVLR  ======================================================== */
#define R_SYSTEM_LVDLVLR_LVD1LVL_Pos         (0UL)          /*!< LVD1LVL (Bit 0)                                       */
#define R_SYSTEM_LVDLVLR_LVD1LVL_Msk         (0xfUL)        /*!< LVD1LVL (Bitfield-Mask: 0x0f)                         */
#define R_SYSTEM_LVDLVLR_LVD2LVL_Pos         (4UL)          /*!< LVD2LVL (Bit 4)                                       */
#define R_SYSTEM_LVDLVLR_LVD2LVL_Msk         (0xf0UL)       /*!< LVD2LVL (Bitfield-Mask: 0x0f)                         */
/* ========================================================  LVD1CR0  ======================================================== */
#define R_SYSTEM_LVD1CR0_LVD1RIE_Pos         (0UL)          /*!< LVD1RIE (Bit 0)                                       */
#define R_SYSTEM_LVD1CR0_LVD1RIE_Msk         (0x1UL)        /*!< LVD1RIE (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_LVD1CR0_LVD1DFDIS_Pos       (1UL)          /*!< LVD1DFDIS (Bit 1)                                     */
#define R_SYSTEM_LVD1CR0_LVD1DFDIS_Msk       (0x2UL)        /*!< LVD1DFDIS (Bitfield-Mask: 0x01)                       */
#define R_SYSTEM_LVD1CR0_LVD1CMPE_Pos        (2UL)          /*!< LVD1CMPE (Bit 2)                                      */
#define R_SYSTEM_LVD1CR0_LVD1CMPE_Msk        (0x4UL)        /*!< LVD1CMPE (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_LVD1CR0_LVD1FSAMP_Pos       (4UL)          /*!< LVD1FSAMP (Bit 4)                                     */
#define R_SYSTEM_LVD1CR0_LVD1FSAMP_Msk       (0x30UL)       /*!< LVD1FSAMP (Bitfield-Mask: 0x03)                       */
#define R_SYSTEM_LVD1CR0_LVD1RI_Pos          (6UL)          /*!< LVD1RI (Bit 6)                                        */
#define R_SYSTEM_LVD1CR0_LVD1RI_Msk          (0x40UL)       /*!< LVD1RI (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_LVD1CR0_LVD1RN_Pos          (7UL)          /*!< LVD1RN (Bit 7)                                        */
#define R_SYSTEM_LVD1CR0_LVD1RN_Msk          (0x80UL)       /*!< LVD1RN (Bitfield-Mask: 0x01)                          */
/* ========================================================  LVD2CR0  ======================================================== */
#define R_SYSTEM_LVD2CR0_LVD2RIE_Pos         (0UL)          /*!< LVD2RIE (Bit 0)                                       */
#define R_SYSTEM_LVD2CR0_LVD2RIE_Msk         (0x1UL)        /*!< LVD2RIE (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_LVD2CR0_LVD2DFDIS_Pos       (1UL)          /*!< LVD2DFDIS (Bit 1)                                     */
#define R_SYSTEM_LVD2CR0_LVD2DFDIS_Msk       (0x2UL)        /*!< LVD2DFDIS (Bitfield-Mask: 0x01)                       */
#define R_SYSTEM_LVD2CR0_LVD2CMPE_Pos        (2UL)          /*!< LVD2CMPE (Bit 2)                                      */
#define R_SYSTEM_LVD2CR0_LVD2CMPE_Msk        (0x4UL)        /*!< LVD2CMPE (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_LVD2CR0_LVD2FSAMP_Pos       (4UL)          /*!< LVD2FSAMP (Bit 4)                                     */
#define R_SYSTEM_LVD2CR0_LVD2FSAMP_Msk       (0x30UL)       /*!< LVD2FSAMP (Bitfield-Mask: 0x03)                       */
#define R_SYSTEM_LVD2CR0_LVD2RI_Pos          (6UL)          /*!< LVD2RI (Bit 6)                                        */
#define R_SYSTEM_LVD2CR0_LVD2RI_Msk          (0x40UL)       /*!< LVD2RI (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_LVD2CR0_LVD2RN_Pos          (7UL)          /*!< LVD2RN (Bit 7)                                        */
#define R_SYSTEM_LVD2CR0_LVD2RN_Msk          (0x80UL)       /*!< LVD2RN (Bitfield-Mask: 0x01)                          */

#endif /* R_SYSTEM_POSMSK_H */
