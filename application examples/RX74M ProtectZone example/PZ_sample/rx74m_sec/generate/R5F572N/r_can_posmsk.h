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

#ifndef R_CAN_POSMSK_H
#define R_CAN_POSMSK_H

/** @addtogroup Device_Peripheral_peripheralAddr
 * @{
 */

/** @} */ /* End of group Device_Peripheral_peripheralAddr */

/* =========================================================================================================================== */
/* ================                                  Peripheral declaration                                   ================ */
/* =========================================================================================================================== */

/** @addtogroup Device_Peripheral_declaration
 * @{
 */

/** @} */ /* End of group Device_Peripheral_declaration */

/* =========================================================================================================================== */
/* ================                                            MB                                             ================ */
/* =========================================================================================================================== */

/* ==========================================================  ID  =========================================================== */
#define R_CAN0_MB_ID_EID_Pos    (0UL)          /*!< EID (Bit 0)                                           */
#define R_CAN0_MB_ID_EID_Msk    (0x3ffffUL)    /*!< EID (Bitfield-Mask: 0x3ffff)                          */
#define R_CAN0_MB_ID_SID_Pos    (18UL)         /*!< SID (Bit 18)                                          */
#define R_CAN0_MB_ID_SID_Msk    (0x1ffc0000UL) /*!< SID (Bitfield-Mask: 0x7ff)                            */
#define R_CAN0_MB_ID_RTR_Pos    (30UL)         /*!< RTR (Bit 30)                                          */
#define R_CAN0_MB_ID_RTR_Msk    (0x40000000UL) /*!< RTR (Bitfield-Mask: 0x01)                             */
#define R_CAN0_MB_ID_IDE_Pos    (31UL)         /*!< IDE (Bit 31)                                          */
#define R_CAN0_MB_ID_IDE_Msk    (0x80000000UL) /*!< IDE (Bitfield-Mask: 0x01)                             */
/* ==========================================================  DL  =========================================================== */
#define R_CAN0_MB_DL_DLC_Pos    (0UL)          /*!< DLC (Bit 0)                                           */
#define R_CAN0_MB_DL_DLC_Msk    (0xfUL)        /*!< DLC (Bitfield-Mask: 0x0f)                             */
/* ===========================================================  D  =========================================================== */
#define R_CAN0_MB_D_DATA_Pos    (0UL)          /*!< DATA (Bit 0)                                          */
#define R_CAN0_MB_D_DATA_Msk    (0xffUL)       /*!< DATA (Bitfield-Mask: 0xff)                            */
/* ==========================================================  TS  =========================================================== */
#define R_CAN0_MB_TS_TSL_Pos    (0UL)          /*!< TSL (Bit 0)                                           */
#define R_CAN0_MB_TS_TSL_Msk    (0xffUL)       /*!< TSL (Bitfield-Mask: 0xff)                             */
#define R_CAN0_MB_TS_TSH_Pos    (8UL)          /*!< TSH (Bit 8)                                           */
#define R_CAN0_MB_TS_TSH_Msk    (0xff00UL)     /*!< TSH (Bitfield-Mask: 0xff)                             */

/** @} */ /* End of group PosMask_clusters */

/* =========================================================================================================================== */
/* ================                                Pos/Mask Peripheral Section                                ================ */
/* =========================================================================================================================== */

/** @addtogroup PosMask_peripherals
 * @{
 */

/* =========================================================================================================================== */
/* ================                                          R_CAN0                                           ================ */
/* =========================================================================================================================== */

/* ==========================================================  MKR  ========================================================== */
#define R_CAN0_MKR_SID_Pos              (18UL)         /*!< SID (Bit 18)                                          */
#define R_CAN0_MKR_SID_Msk              (0x1ffc0000UL) /*!< SID (Bitfield-Mask: 0x7ff)                            */
#define R_CAN0_MKR_EID_Pos              (0UL)          /*!< EID (Bit 0)                                           */
#define R_CAN0_MKR_EID_Msk              (0x3ffffUL)    /*!< EID (Bitfield-Mask: 0x3ffff)                          */
/* =========================================================  FIDCR  ========================================================= */
#define R_CAN0_FIDCR_IDE_Pos            (31UL)         /*!< IDE (Bit 31)                                          */
#define R_CAN0_FIDCR_IDE_Msk            (0x80000000UL) /*!< IDE (Bitfield-Mask: 0x01)                             */
#define R_CAN0_FIDCR_RTR_Pos            (30UL)         /*!< RTR (Bit 30)                                          */
#define R_CAN0_FIDCR_RTR_Msk            (0x40000000UL) /*!< RTR (Bitfield-Mask: 0x01)                             */
#define R_CAN0_FIDCR_SID_Pos            (18UL)         /*!< SID (Bit 18)                                          */
#define R_CAN0_FIDCR_SID_Msk            (0x1ffc0000UL) /*!< SID (Bitfield-Mask: 0x7ff)                            */
#define R_CAN0_FIDCR_EID_Pos            (0UL)          /*!< EID (Bit 0)                                           */
#define R_CAN0_FIDCR_EID_Msk            (0x3ffffUL)    /*!< EID (Bitfield-Mask: 0x3ffff)                          */
/* ========================================================  MKIVLR  ========================================================= */
#define R_CAN0_MKIVLR_MB31_Pos          (31UL)         /*!< MB31 (Bit 31)                                         */
#define R_CAN0_MKIVLR_MB31_Msk          (0x80000000UL) /*!< MB31 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MKIVLR_MB30_Pos          (30UL)         /*!< MB30 (Bit 30)                                         */
#define R_CAN0_MKIVLR_MB30_Msk          (0x40000000UL) /*!< MB30 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MKIVLR_MB29_Pos          (29UL)         /*!< MB29 (Bit 29)                                         */
#define R_CAN0_MKIVLR_MB29_Msk          (0x20000000UL) /*!< MB29 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MKIVLR_MB28_Pos          (28UL)         /*!< MB28 (Bit 28)                                         */
#define R_CAN0_MKIVLR_MB28_Msk          (0x10000000UL) /*!< MB28 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MKIVLR_MB27_Pos          (27UL)         /*!< MB27 (Bit 27)                                         */
#define R_CAN0_MKIVLR_MB27_Msk          (0x8000000UL)  /*!< MB27 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MKIVLR_MB26_Pos          (26UL)         /*!< MB26 (Bit 26)                                         */
#define R_CAN0_MKIVLR_MB26_Msk          (0x4000000UL)  /*!< MB26 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MKIVLR_MB25_Pos          (25UL)         /*!< MB25 (Bit 25)                                         */
#define R_CAN0_MKIVLR_MB25_Msk          (0x2000000UL)  /*!< MB25 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MKIVLR_MB24_Pos          (24UL)         /*!< MB24 (Bit 24)                                         */
#define R_CAN0_MKIVLR_MB24_Msk          (0x1000000UL)  /*!< MB24 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MKIVLR_MB23_Pos          (23UL)         /*!< MB23 (Bit 23)                                         */
#define R_CAN0_MKIVLR_MB23_Msk          (0x800000UL)   /*!< MB23 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MKIVLR_MB22_Pos          (22UL)         /*!< MB22 (Bit 22)                                         */
#define R_CAN0_MKIVLR_MB22_Msk          (0x400000UL)   /*!< MB22 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MKIVLR_MB21_Pos          (21UL)         /*!< MB21 (Bit 21)                                         */
#define R_CAN0_MKIVLR_MB21_Msk          (0x200000UL)   /*!< MB21 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MKIVLR_MB20_Pos          (20UL)         /*!< MB20 (Bit 20)                                         */
#define R_CAN0_MKIVLR_MB20_Msk          (0x100000UL)   /*!< MB20 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MKIVLR_MB19_Pos          (19UL)         /*!< MB19 (Bit 19)                                         */
#define R_CAN0_MKIVLR_MB19_Msk          (0x80000UL)    /*!< MB19 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MKIVLR_MB18_Pos          (18UL)         /*!< MB18 (Bit 18)                                         */
#define R_CAN0_MKIVLR_MB18_Msk          (0x40000UL)    /*!< MB18 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MKIVLR_MB17_Pos          (17UL)         /*!< MB17 (Bit 17)                                         */
#define R_CAN0_MKIVLR_MB17_Msk          (0x20000UL)    /*!< MB17 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MKIVLR_MB16_Pos          (16UL)         /*!< MB16 (Bit 16)                                         */
#define R_CAN0_MKIVLR_MB16_Msk          (0x10000UL)    /*!< MB16 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MKIVLR_MB15_Pos          (15UL)         /*!< MB15 (Bit 15)                                         */
#define R_CAN0_MKIVLR_MB15_Msk          (0x8000UL)     /*!< MB15 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MKIVLR_MB14_Pos          (14UL)         /*!< MB14 (Bit 14)                                         */
#define R_CAN0_MKIVLR_MB14_Msk          (0x4000UL)     /*!< MB14 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MKIVLR_MB13_Pos          (13UL)         /*!< MB13 (Bit 13)                                         */
#define R_CAN0_MKIVLR_MB13_Msk          (0x2000UL)     /*!< MB13 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MKIVLR_MB12_Pos          (12UL)         /*!< MB12 (Bit 12)                                         */
#define R_CAN0_MKIVLR_MB12_Msk          (0x1000UL)     /*!< MB12 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MKIVLR_MB11_Pos          (11UL)         /*!< MB11 (Bit 11)                                         */
#define R_CAN0_MKIVLR_MB11_Msk          (0x800UL)      /*!< MB11 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MKIVLR_MB10_Pos          (10UL)         /*!< MB10 (Bit 10)                                         */
#define R_CAN0_MKIVLR_MB10_Msk          (0x400UL)      /*!< MB10 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MKIVLR_MB9_Pos           (9UL)          /*!< MB9 (Bit 9)                                           */
#define R_CAN0_MKIVLR_MB9_Msk           (0x200UL)      /*!< MB9 (Bitfield-Mask: 0x01)                             */
#define R_CAN0_MKIVLR_MB8_Pos           (8UL)          /*!< MB8 (Bit 8)                                           */
#define R_CAN0_MKIVLR_MB8_Msk           (0x100UL)      /*!< MB8 (Bitfield-Mask: 0x01)                             */
#define R_CAN0_MKIVLR_MB7_Pos           (7UL)          /*!< MB7 (Bit 7)                                           */
#define R_CAN0_MKIVLR_MB7_Msk           (0x80UL)       /*!< MB7 (Bitfield-Mask: 0x01)                             */
#define R_CAN0_MKIVLR_MB6_Pos           (6UL)          /*!< MB6 (Bit 6)                                           */
#define R_CAN0_MKIVLR_MB6_Msk           (0x40UL)       /*!< MB6 (Bitfield-Mask: 0x01)                             */
#define R_CAN0_MKIVLR_MB5_Pos           (5UL)          /*!< MB5 (Bit 5)                                           */
#define R_CAN0_MKIVLR_MB5_Msk           (0x20UL)       /*!< MB5 (Bitfield-Mask: 0x01)                             */
#define R_CAN0_MKIVLR_MB4_Pos           (4UL)          /*!< MB4 (Bit 4)                                           */
#define R_CAN0_MKIVLR_MB4_Msk           (0x10UL)       /*!< MB4 (Bitfield-Mask: 0x01)                             */
#define R_CAN0_MKIVLR_MB3_Pos           (3UL)          /*!< MB3 (Bit 3)                                           */
#define R_CAN0_MKIVLR_MB3_Msk           (0x8UL)        /*!< MB3 (Bitfield-Mask: 0x01)                             */
#define R_CAN0_MKIVLR_MB2_Pos           (2UL)          /*!< MB2 (Bit 2)                                           */
#define R_CAN0_MKIVLR_MB2_Msk           (0x4UL)        /*!< MB2 (Bitfield-Mask: 0x01)                             */
#define R_CAN0_MKIVLR_MB1_Pos           (1UL)          /*!< MB1 (Bit 1)                                           */
#define R_CAN0_MKIVLR_MB1_Msk           (0x2UL)        /*!< MB1 (Bitfield-Mask: 0x01)                             */
#define R_CAN0_MKIVLR_MB0_Pos           (0UL)          /*!< MB0 (Bit 0)                                           */
#define R_CAN0_MKIVLR_MB0_Msk           (0x1UL)        /*!< MB0 (Bitfield-Mask: 0x01)                             */
/* =========================================================  MIER  ========================================================== */
#define R_CAN0_MIER_MB31_Pos            (31UL)         /*!< MB31 (Bit 31)                                         */
#define R_CAN0_MIER_MB31_Msk            (0x80000000UL) /*!< MB31 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MIER_MB30_Pos            (30UL)         /*!< MB30 (Bit 30)                                         */
#define R_CAN0_MIER_MB30_Msk            (0x40000000UL) /*!< MB30 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MIER_MB29_Pos            (29UL)         /*!< MB29 (Bit 29)                                         */
#define R_CAN0_MIER_MB29_Msk            (0x20000000UL) /*!< MB29 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MIER_MB28_Pos            (28UL)         /*!< MB28 (Bit 28)                                         */
#define R_CAN0_MIER_MB28_Msk            (0x10000000UL) /*!< MB28 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MIER_MB27_Pos            (27UL)         /*!< MB27 (Bit 27)                                         */
#define R_CAN0_MIER_MB27_Msk            (0x8000000UL)  /*!< MB27 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MIER_MB26_Pos            (26UL)         /*!< MB26 (Bit 26)                                         */
#define R_CAN0_MIER_MB26_Msk            (0x4000000UL)  /*!< MB26 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MIER_MB25_Pos            (25UL)         /*!< MB25 (Bit 25)                                         */
#define R_CAN0_MIER_MB25_Msk            (0x2000000UL)  /*!< MB25 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MIER_MB24_Pos            (24UL)         /*!< MB24 (Bit 24)                                         */
#define R_CAN0_MIER_MB24_Msk            (0x1000000UL)  /*!< MB24 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MIER_MB23_Pos            (23UL)         /*!< MB23 (Bit 23)                                         */
#define R_CAN0_MIER_MB23_Msk            (0x800000UL)   /*!< MB23 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MIER_MB22_Pos            (22UL)         /*!< MB22 (Bit 22)                                         */
#define R_CAN0_MIER_MB22_Msk            (0x400000UL)   /*!< MB22 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MIER_MB21_Pos            (21UL)         /*!< MB21 (Bit 21)                                         */
#define R_CAN0_MIER_MB21_Msk            (0x200000UL)   /*!< MB21 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MIER_MB20_Pos            (20UL)         /*!< MB20 (Bit 20)                                         */
#define R_CAN0_MIER_MB20_Msk            (0x100000UL)   /*!< MB20 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MIER_MB19_Pos            (19UL)         /*!< MB19 (Bit 19)                                         */
#define R_CAN0_MIER_MB19_Msk            (0x80000UL)    /*!< MB19 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MIER_MB18_Pos            (18UL)         /*!< MB18 (Bit 18)                                         */
#define R_CAN0_MIER_MB18_Msk            (0x40000UL)    /*!< MB18 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MIER_MB17_Pos            (17UL)         /*!< MB17 (Bit 17)                                         */
#define R_CAN0_MIER_MB17_Msk            (0x20000UL)    /*!< MB17 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MIER_MB16_Pos            (16UL)         /*!< MB16 (Bit 16)                                         */
#define R_CAN0_MIER_MB16_Msk            (0x10000UL)    /*!< MB16 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MIER_MB15_Pos            (15UL)         /*!< MB15 (Bit 15)                                         */
#define R_CAN0_MIER_MB15_Msk            (0x8000UL)     /*!< MB15 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MIER_MB14_Pos            (14UL)         /*!< MB14 (Bit 14)                                         */
#define R_CAN0_MIER_MB14_Msk            (0x4000UL)     /*!< MB14 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MIER_MB13_Pos            (13UL)         /*!< MB13 (Bit 13)                                         */
#define R_CAN0_MIER_MB13_Msk            (0x2000UL)     /*!< MB13 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MIER_MB12_Pos            (12UL)         /*!< MB12 (Bit 12)                                         */
#define R_CAN0_MIER_MB12_Msk            (0x1000UL)     /*!< MB12 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MIER_MB11_Pos            (11UL)         /*!< MB11 (Bit 11)                                         */
#define R_CAN0_MIER_MB11_Msk            (0x800UL)      /*!< MB11 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MIER_MB10_Pos            (10UL)         /*!< MB10 (Bit 10)                                         */
#define R_CAN0_MIER_MB10_Msk            (0x400UL)      /*!< MB10 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MIER_MB9_Pos             (9UL)          /*!< MB9 (Bit 9)                                           */
#define R_CAN0_MIER_MB9_Msk             (0x200UL)      /*!< MB9 (Bitfield-Mask: 0x01)                             */
#define R_CAN0_MIER_MB8_Pos             (8UL)          /*!< MB8 (Bit 8)                                           */
#define R_CAN0_MIER_MB8_Msk             (0x100UL)      /*!< MB8 (Bitfield-Mask: 0x01)                             */
#define R_CAN0_MIER_MB7_Pos             (7UL)          /*!< MB7 (Bit 7)                                           */
#define R_CAN0_MIER_MB7_Msk             (0x80UL)       /*!< MB7 (Bitfield-Mask: 0x01)                             */
#define R_CAN0_MIER_MB6_Pos             (6UL)          /*!< MB6 (Bit 6)                                           */
#define R_CAN0_MIER_MB6_Msk             (0x40UL)       /*!< MB6 (Bitfield-Mask: 0x01)                             */
#define R_CAN0_MIER_MB5_Pos             (5UL)          /*!< MB5 (Bit 5)                                           */
#define R_CAN0_MIER_MB5_Msk             (0x20UL)       /*!< MB5 (Bitfield-Mask: 0x01)                             */
#define R_CAN0_MIER_MB4_Pos             (4UL)          /*!< MB4 (Bit 4)                                           */
#define R_CAN0_MIER_MB4_Msk             (0x10UL)       /*!< MB4 (Bitfield-Mask: 0x01)                             */
#define R_CAN0_MIER_MB3_Pos             (3UL)          /*!< MB3 (Bit 3)                                           */
#define R_CAN0_MIER_MB3_Msk             (0x8UL)        /*!< MB3 (Bitfield-Mask: 0x01)                             */
#define R_CAN0_MIER_MB2_Pos             (2UL)          /*!< MB2 (Bit 2)                                           */
#define R_CAN0_MIER_MB2_Msk             (0x4UL)        /*!< MB2 (Bitfield-Mask: 0x01)                             */
#define R_CAN0_MIER_MB1_Pos             (1UL)          /*!< MB1 (Bit 1)                                           */
#define R_CAN0_MIER_MB1_Msk             (0x2UL)        /*!< MB1 (Bitfield-Mask: 0x01)                             */
#define R_CAN0_MIER_MB0_Pos             (0UL)          /*!< MB0 (Bit 0)                                           */
#define R_CAN0_MIER_MB0_Msk             (0x1UL)        /*!< MB0 (Bitfield-Mask: 0x01)                             */
/* =======================================================  MIER_FIFO  ======================================================= */
#define R_CAN0_MIER_FIFO_MB29_Pos       (29UL)         /*!< MB29 (Bit 29)                                         */
#define R_CAN0_MIER_FIFO_MB29_Msk       (0x20000000UL) /*!< MB29 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MIER_FIFO_MB28_Pos       (28UL)         /*!< MB28 (Bit 28)                                         */
#define R_CAN0_MIER_FIFO_MB28_Msk       (0x10000000UL) /*!< MB28 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MIER_FIFO_MB25_Pos       (25UL)         /*!< MB25 (Bit 25)                                         */
#define R_CAN0_MIER_FIFO_MB25_Msk       (0x2000000UL)  /*!< MB25 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MIER_FIFO_MB24_Pos       (24UL)         /*!< MB24 (Bit 24)                                         */
#define R_CAN0_MIER_FIFO_MB24_Msk       (0x1000000UL)  /*!< MB24 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MIER_FIFO_MB23_Pos       (23UL)         /*!< MB23 (Bit 23)                                         */
#define R_CAN0_MIER_FIFO_MB23_Msk       (0x800000UL)   /*!< MB23 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MIER_FIFO_MB22_Pos       (22UL)         /*!< MB22 (Bit 22)                                         */
#define R_CAN0_MIER_FIFO_MB22_Msk       (0x400000UL)   /*!< MB22 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MIER_FIFO_MB21_Pos       (21UL)         /*!< MB21 (Bit 21)                                         */
#define R_CAN0_MIER_FIFO_MB21_Msk       (0x200000UL)   /*!< MB21 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MIER_FIFO_MB20_Pos       (20UL)         /*!< MB20 (Bit 20)                                         */
#define R_CAN0_MIER_FIFO_MB20_Msk       (0x100000UL)   /*!< MB20 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MIER_FIFO_MB19_Pos       (19UL)         /*!< MB19 (Bit 19)                                         */
#define R_CAN0_MIER_FIFO_MB19_Msk       (0x80000UL)    /*!< MB19 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MIER_FIFO_MB18_Pos       (18UL)         /*!< MB18 (Bit 18)                                         */
#define R_CAN0_MIER_FIFO_MB18_Msk       (0x40000UL)    /*!< MB18 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MIER_FIFO_MB17_Pos       (17UL)         /*!< MB17 (Bit 17)                                         */
#define R_CAN0_MIER_FIFO_MB17_Msk       (0x20000UL)    /*!< MB17 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MIER_FIFO_MB16_Pos       (16UL)         /*!< MB16 (Bit 16)                                         */
#define R_CAN0_MIER_FIFO_MB16_Msk       (0x10000UL)    /*!< MB16 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MIER_FIFO_MB15_Pos       (15UL)         /*!< MB15 (Bit 15)                                         */
#define R_CAN0_MIER_FIFO_MB15_Msk       (0x8000UL)     /*!< MB15 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MIER_FIFO_MB14_Pos       (14UL)         /*!< MB14 (Bit 14)                                         */
#define R_CAN0_MIER_FIFO_MB14_Msk       (0x4000UL)     /*!< MB14 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MIER_FIFO_MB13_Pos       (13UL)         /*!< MB13 (Bit 13)                                         */
#define R_CAN0_MIER_FIFO_MB13_Msk       (0x2000UL)     /*!< MB13 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MIER_FIFO_MB12_Pos       (12UL)         /*!< MB12 (Bit 12)                                         */
#define R_CAN0_MIER_FIFO_MB12_Msk       (0x1000UL)     /*!< MB12 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MIER_FIFO_MB11_Pos       (11UL)         /*!< MB11 (Bit 11)                                         */
#define R_CAN0_MIER_FIFO_MB11_Msk       (0x800UL)      /*!< MB11 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MIER_FIFO_MB10_Pos       (10UL)         /*!< MB10 (Bit 10)                                         */
#define R_CAN0_MIER_FIFO_MB10_Msk       (0x400UL)      /*!< MB10 (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MIER_FIFO_MB9_Pos        (9UL)          /*!< MB9 (Bit 9)                                           */
#define R_CAN0_MIER_FIFO_MB9_Msk        (0x200UL)      /*!< MB9 (Bitfield-Mask: 0x01)                             */
#define R_CAN0_MIER_FIFO_MB8_Pos        (8UL)          /*!< MB8 (Bit 8)                                           */
#define R_CAN0_MIER_FIFO_MB8_Msk        (0x100UL)      /*!< MB8 (Bitfield-Mask: 0x01)                             */
#define R_CAN0_MIER_FIFO_MB7_Pos        (7UL)          /*!< MB7 (Bit 7)                                           */
#define R_CAN0_MIER_FIFO_MB7_Msk        (0x80UL)       /*!< MB7 (Bitfield-Mask: 0x01)                             */
#define R_CAN0_MIER_FIFO_MB6_Pos        (6UL)          /*!< MB6 (Bit 6)                                           */
#define R_CAN0_MIER_FIFO_MB6_Msk        (0x40UL)       /*!< MB6 (Bitfield-Mask: 0x01)                             */
#define R_CAN0_MIER_FIFO_MB5_Pos        (5UL)          /*!< MB5 (Bit 5)                                           */
#define R_CAN0_MIER_FIFO_MB5_Msk        (0x20UL)       /*!< MB5 (Bitfield-Mask: 0x01)                             */
#define R_CAN0_MIER_FIFO_MB4_Pos        (4UL)          /*!< MB4 (Bit 4)                                           */
#define R_CAN0_MIER_FIFO_MB4_Msk        (0x10UL)       /*!< MB4 (Bitfield-Mask: 0x01)                             */
#define R_CAN0_MIER_FIFO_MB3_Pos        (3UL)          /*!< MB3 (Bit 3)                                           */
#define R_CAN0_MIER_FIFO_MB3_Msk        (0x8UL)        /*!< MB3 (Bitfield-Mask: 0x01)                             */
#define R_CAN0_MIER_FIFO_MB2_Pos        (2UL)          /*!< MB2 (Bit 2)                                           */
#define R_CAN0_MIER_FIFO_MB2_Msk        (0x4UL)        /*!< MB2 (Bitfield-Mask: 0x01)                             */
#define R_CAN0_MIER_FIFO_MB1_Pos        (1UL)          /*!< MB1 (Bit 1)                                           */
#define R_CAN0_MIER_FIFO_MB1_Msk        (0x2UL)        /*!< MB1 (Bitfield-Mask: 0x01)                             */
#define R_CAN0_MIER_FIFO_MB0_Pos        (0UL)          /*!< MB0 (Bit 0)                                           */
#define R_CAN0_MIER_FIFO_MB0_Msk        (0x1UL)        /*!< MB0 (Bitfield-Mask: 0x01)                             */
/* ========================================================  MCTL_TX  ======================================================== */
#define R_CAN0_MCTL_TX_TRMREQ_Pos       (7UL)          /*!< TRMREQ (Bit 7)                                        */
#define R_CAN0_MCTL_TX_TRMREQ_Msk       (0x80UL)       /*!< TRMREQ (Bitfield-Mask: 0x01)                          */
#define R_CAN0_MCTL_TX_RECREQ_Pos       (6UL)          /*!< RECREQ (Bit 6)                                        */
#define R_CAN0_MCTL_TX_RECREQ_Msk       (0x40UL)       /*!< RECREQ (Bitfield-Mask: 0x01)                          */
#define R_CAN0_MCTL_TX_ONESHOT_Pos      (4UL)          /*!< ONESHOT (Bit 4)                                       */
#define R_CAN0_MCTL_TX_ONESHOT_Msk      (0x10UL)       /*!< ONESHOT (Bitfield-Mask: 0x01)                         */
#define R_CAN0_MCTL_TX_TRMABT_Pos       (2UL)          /*!< TRMABT (Bit 2)                                        */
#define R_CAN0_MCTL_TX_TRMABT_Msk       (0x4UL)        /*!< TRMABT (Bitfield-Mask: 0x01)                          */
#define R_CAN0_MCTL_TX_TRMACTIVE_Pos    (1UL)          /*!< TRMACTIVE (Bit 1)                                     */
#define R_CAN0_MCTL_TX_TRMACTIVE_Msk    (0x2UL)        /*!< TRMACTIVE (Bitfield-Mask: 0x01)                       */
#define R_CAN0_MCTL_TX_SENTDATA_Pos     (0UL)          /*!< SENTDATA (Bit 0)                                      */
#define R_CAN0_MCTL_TX_SENTDATA_Msk     (0x1UL)        /*!< SENTDATA (Bitfield-Mask: 0x01)                        */
/* ========================================================  MCTL_RX  ======================================================== */
#define R_CAN0_MCTL_RX_TRMREQ_Pos       (7UL)          /*!< TRMREQ (Bit 7)                                        */
#define R_CAN0_MCTL_RX_TRMREQ_Msk       (0x80UL)       /*!< TRMREQ (Bitfield-Mask: 0x01)                          */
#define R_CAN0_MCTL_RX_RECREQ_Pos       (6UL)          /*!< RECREQ (Bit 6)                                        */
#define R_CAN0_MCTL_RX_RECREQ_Msk       (0x40UL)       /*!< RECREQ (Bitfield-Mask: 0x01)                          */
#define R_CAN0_MCTL_RX_ONESHOT_Pos      (4UL)          /*!< ONESHOT (Bit 4)                                       */
#define R_CAN0_MCTL_RX_ONESHOT_Msk      (0x10UL)       /*!< ONESHOT (Bitfield-Mask: 0x01)                         */
#define R_CAN0_MCTL_RX_MSGLOST_Pos      (2UL)          /*!< MSGLOST (Bit 2)                                       */
#define R_CAN0_MCTL_RX_MSGLOST_Msk      (0x4UL)        /*!< MSGLOST (Bitfield-Mask: 0x01)                         */
#define R_CAN0_MCTL_RX_INVALDATA_Pos    (1UL)          /*!< INVALDATA (Bit 1)                                     */
#define R_CAN0_MCTL_RX_INVALDATA_Msk    (0x2UL)        /*!< INVALDATA (Bitfield-Mask: 0x01)                       */
#define R_CAN0_MCTL_RX_NEWDATA_Pos      (0UL)          /*!< NEWDATA (Bit 0)                                       */
#define R_CAN0_MCTL_RX_NEWDATA_Msk      (0x1UL)        /*!< NEWDATA (Bitfield-Mask: 0x01)                         */
/* =========================================================  CTLR  ========================================================== */
#define R_CAN0_CTLR_RBOC_Pos            (13UL)         /*!< RBOC (Bit 13)                                         */
#define R_CAN0_CTLR_RBOC_Msk            (0x2000UL)     /*!< RBOC (Bitfield-Mask: 0x01)                            */
#define R_CAN0_CTLR_BOM_Pos             (11UL)         /*!< BOM (Bit 11)                                          */
#define R_CAN0_CTLR_BOM_Msk             (0x1800UL)     /*!< BOM (Bitfield-Mask: 0x03)                             */
#define R_CAN0_CTLR_SLPM_Pos            (10UL)         /*!< SLPM (Bit 10)                                         */
#define R_CAN0_CTLR_SLPM_Msk            (0x400UL)      /*!< SLPM (Bitfield-Mask: 0x01)                            */
#define R_CAN0_CTLR_CANM_Pos            (8UL)          /*!< CANM (Bit 8)                                          */
#define R_CAN0_CTLR_CANM_Msk            (0x300UL)      /*!< CANM (Bitfield-Mask: 0x03)                            */
#define R_CAN0_CTLR_TSPS_Pos            (6UL)          /*!< TSPS (Bit 6)                                          */
#define R_CAN0_CTLR_TSPS_Msk            (0xc0UL)       /*!< TSPS (Bitfield-Mask: 0x03)                            */
#define R_CAN0_CTLR_TSRC_Pos            (5UL)          /*!< TSRC (Bit 5)                                          */
#define R_CAN0_CTLR_TSRC_Msk            (0x20UL)       /*!< TSRC (Bitfield-Mask: 0x01)                            */
#define R_CAN0_CTLR_TPM_Pos             (4UL)          /*!< TPM (Bit 4)                                           */
#define R_CAN0_CTLR_TPM_Msk             (0x10UL)       /*!< TPM (Bitfield-Mask: 0x01)                             */
#define R_CAN0_CTLR_MLM_Pos             (3UL)          /*!< MLM (Bit 3)                                           */
#define R_CAN0_CTLR_MLM_Msk             (0x8UL)        /*!< MLM (Bitfield-Mask: 0x01)                             */
#define R_CAN0_CTLR_IDFM_Pos            (1UL)          /*!< IDFM (Bit 1)                                          */
#define R_CAN0_CTLR_IDFM_Msk            (0x6UL)        /*!< IDFM (Bitfield-Mask: 0x03)                            */
#define R_CAN0_CTLR_MBM_Pos             (0UL)          /*!< MBM (Bit 0)                                           */
#define R_CAN0_CTLR_MBM_Msk             (0x1UL)        /*!< MBM (Bitfield-Mask: 0x01)                             */
/* ==========================================================  STR  ========================================================== */
#define R_CAN0_STR_RECST_Pos            (14UL)         /*!< RECST (Bit 14)                                        */
#define R_CAN0_STR_RECST_Msk            (0x4000UL)     /*!< RECST (Bitfield-Mask: 0x01)                           */
#define R_CAN0_STR_TRMST_Pos            (13UL)         /*!< TRMST (Bit 13)                                        */
#define R_CAN0_STR_TRMST_Msk            (0x2000UL)     /*!< TRMST (Bitfield-Mask: 0x01)                           */
#define R_CAN0_STR_BOST_Pos             (12UL)         /*!< BOST (Bit 12)                                         */
#define R_CAN0_STR_BOST_Msk             (0x1000UL)     /*!< BOST (Bitfield-Mask: 0x01)                            */
#define R_CAN0_STR_EPST_Pos             (11UL)         /*!< EPST (Bit 11)                                         */
#define R_CAN0_STR_EPST_Msk             (0x800UL)      /*!< EPST (Bitfield-Mask: 0x01)                            */
#define R_CAN0_STR_SLPST_Pos            (10UL)         /*!< SLPST (Bit 10)                                        */
#define R_CAN0_STR_SLPST_Msk            (0x400UL)      /*!< SLPST (Bitfield-Mask: 0x01)                           */
#define R_CAN0_STR_HLTST_Pos            (9UL)          /*!< HLTST (Bit 9)                                         */
#define R_CAN0_STR_HLTST_Msk            (0x200UL)      /*!< HLTST (Bitfield-Mask: 0x01)                           */
#define R_CAN0_STR_RSTST_Pos            (8UL)          /*!< RSTST (Bit 8)                                         */
#define R_CAN0_STR_RSTST_Msk            (0x100UL)      /*!< RSTST (Bitfield-Mask: 0x01)                           */
#define R_CAN0_STR_EST_Pos              (7UL)          /*!< EST (Bit 7)                                           */
#define R_CAN0_STR_EST_Msk              (0x80UL)       /*!< EST (Bitfield-Mask: 0x01)                             */
#define R_CAN0_STR_TABST_Pos            (6UL)          /*!< TABST (Bit 6)                                         */
#define R_CAN0_STR_TABST_Msk            (0x40UL)       /*!< TABST (Bitfield-Mask: 0x01)                           */
#define R_CAN0_STR_FMLST_Pos            (5UL)          /*!< FMLST (Bit 5)                                         */
#define R_CAN0_STR_FMLST_Msk            (0x20UL)       /*!< FMLST (Bitfield-Mask: 0x01)                           */
#define R_CAN0_STR_NMLST_Pos            (4UL)          /*!< NMLST (Bit 4)                                         */
#define R_CAN0_STR_NMLST_Msk            (0x10UL)       /*!< NMLST (Bitfield-Mask: 0x01)                           */
#define R_CAN0_STR_TFST_Pos             (3UL)          /*!< TFST (Bit 3)                                          */
#define R_CAN0_STR_TFST_Msk             (0x8UL)        /*!< TFST (Bitfield-Mask: 0x01)                            */
#define R_CAN0_STR_RFST_Pos             (2UL)          /*!< RFST (Bit 2)                                          */
#define R_CAN0_STR_RFST_Msk             (0x4UL)        /*!< RFST (Bitfield-Mask: 0x01)                            */
#define R_CAN0_STR_SDST_Pos             (1UL)          /*!< SDST (Bit 1)                                          */
#define R_CAN0_STR_SDST_Msk             (0x2UL)        /*!< SDST (Bitfield-Mask: 0x01)                            */
#define R_CAN0_STR_NDST_Pos             (0UL)          /*!< NDST (Bit 0)                                          */
#define R_CAN0_STR_NDST_Msk             (0x1UL)        /*!< NDST (Bitfield-Mask: 0x01)                            */
/* ==========================================================  BCR  ========================================================== */
#define R_CAN0_BCR_TSEG1_Pos            (28UL)         /*!< TSEG1 (Bit 28)                                        */
#define R_CAN0_BCR_TSEG1_Msk            (0xf0000000UL) /*!< TSEG1 (Bitfield-Mask: 0x0f)                           */
#define R_CAN0_BCR_BRP_Pos              (16UL)         /*!< BRP (Bit 16)                                          */
#define R_CAN0_BCR_BRP_Msk              (0x3ff0000UL)  /*!< BRP (Bitfield-Mask: 0x3ff)                            */
#define R_CAN0_BCR_SJW_Pos              (12UL)         /*!< SJW (Bit 12)                                          */
#define R_CAN0_BCR_SJW_Msk              (0x3000UL)     /*!< SJW (Bitfield-Mask: 0x03)                             */
#define R_CAN0_BCR_TSEG2_Pos            (8UL)          /*!< TSEG2 (Bit 8)                                         */
#define R_CAN0_BCR_TSEG2_Msk            (0x700UL)      /*!< TSEG2 (Bitfield-Mask: 0x07)                           */
#define R_CAN0_BCR_CCLKS_Pos            (0UL)          /*!< CCLKS (Bit 0)                                         */
#define R_CAN0_BCR_CCLKS_Msk            (0x1UL)        /*!< CCLKS (Bitfield-Mask: 0x01)                           */
/* =========================================================  RFCR  ========================================================== */
#define R_CAN0_RFCR_RFEST_Pos           (7UL)          /*!< RFEST (Bit 7)                                         */
#define R_CAN0_RFCR_RFEST_Msk           (0x80UL)       /*!< RFEST (Bitfield-Mask: 0x01)                           */
#define R_CAN0_RFCR_RFWST_Pos           (6UL)          /*!< RFWST (Bit 6)                                         */
#define R_CAN0_RFCR_RFWST_Msk           (0x40UL)       /*!< RFWST (Bitfield-Mask: 0x01)                           */
#define R_CAN0_RFCR_RFFST_Pos           (5UL)          /*!< RFFST (Bit 5)                                         */
#define R_CAN0_RFCR_RFFST_Msk           (0x20UL)       /*!< RFFST (Bitfield-Mask: 0x01)                           */
#define R_CAN0_RFCR_RFMLF_Pos           (4UL)          /*!< RFMLF (Bit 4)                                         */
#define R_CAN0_RFCR_RFMLF_Msk           (0x10UL)       /*!< RFMLF (Bitfield-Mask: 0x01)                           */
#define R_CAN0_RFCR_RFUST_Pos           (1UL)          /*!< RFUST (Bit 1)                                         */
#define R_CAN0_RFCR_RFUST_Msk           (0xeUL)        /*!< RFUST (Bitfield-Mask: 0x07)                           */
#define R_CAN0_RFCR_RFE_Pos             (0UL)          /*!< RFE (Bit 0)                                           */
#define R_CAN0_RFCR_RFE_Msk             (0x1UL)        /*!< RFE (Bitfield-Mask: 0x01)                             */
/* =========================================================  RFPCR  ========================================================= */
#define R_CAN0_RFPCR_RFPCR_Pos          (0UL)          /*!< RFPCR (Bit 0)                                         */
#define R_CAN0_RFPCR_RFPCR_Msk          (0xffUL)       /*!< RFPCR (Bitfield-Mask: 0xff)                           */
/* =========================================================  TFCR  ========================================================== */
#define R_CAN0_TFCR_TFEST_Pos           (7UL)          /*!< TFEST (Bit 7)                                         */
#define R_CAN0_TFCR_TFEST_Msk           (0x80UL)       /*!< TFEST (Bitfield-Mask: 0x01)                           */
#define R_CAN0_TFCR_TFFST_Pos           (6UL)          /*!< TFFST (Bit 6)                                         */
#define R_CAN0_TFCR_TFFST_Msk           (0x40UL)       /*!< TFFST (Bitfield-Mask: 0x01)                           */
#define R_CAN0_TFCR_TFUST_Pos           (1UL)          /*!< TFUST (Bit 1)                                         */
#define R_CAN0_TFCR_TFUST_Msk           (0xeUL)        /*!< TFUST (Bitfield-Mask: 0x07)                           */
#define R_CAN0_TFCR_TFE_Pos             (0UL)          /*!< TFE (Bit 0)                                           */
#define R_CAN0_TFCR_TFE_Msk             (0x1UL)        /*!< TFE (Bitfield-Mask: 0x01)                             */
/* =========================================================  TFPCR  ========================================================= */
#define R_CAN0_TFPCR_TFPCR_Pos          (0UL)          /*!< TFPCR (Bit 0)                                         */
#define R_CAN0_TFPCR_TFPCR_Msk          (0xffUL)       /*!< TFPCR (Bitfield-Mask: 0xff)                           */
/* =========================================================  EIER  ========================================================== */
#define R_CAN0_EIER_BLIE_Pos            (7UL)          /*!< BLIE (Bit 7)                                          */
#define R_CAN0_EIER_BLIE_Msk            (0x80UL)       /*!< BLIE (Bitfield-Mask: 0x01)                            */
#define R_CAN0_EIER_OLIE_Pos            (6UL)          /*!< OLIE (Bit 6)                                          */
#define R_CAN0_EIER_OLIE_Msk            (0x40UL)       /*!< OLIE (Bitfield-Mask: 0x01)                            */
#define R_CAN0_EIER_ORIE_Pos            (5UL)          /*!< ORIE (Bit 5)                                          */
#define R_CAN0_EIER_ORIE_Msk            (0x20UL)       /*!< ORIE (Bitfield-Mask: 0x01)                            */
#define R_CAN0_EIER_BORIE_Pos           (4UL)          /*!< BORIE (Bit 4)                                         */
#define R_CAN0_EIER_BORIE_Msk           (0x10UL)       /*!< BORIE (Bitfield-Mask: 0x01)                           */
#define R_CAN0_EIER_BOEIE_Pos           (3UL)          /*!< BOEIE (Bit 3)                                         */
#define R_CAN0_EIER_BOEIE_Msk           (0x8UL)        /*!< BOEIE (Bitfield-Mask: 0x01)                           */
#define R_CAN0_EIER_EPIE_Pos            (2UL)          /*!< EPIE (Bit 2)                                          */
#define R_CAN0_EIER_EPIE_Msk            (0x4UL)        /*!< EPIE (Bitfield-Mask: 0x01)                            */
#define R_CAN0_EIER_EWIE_Pos            (1UL)          /*!< EWIE (Bit 1)                                          */
#define R_CAN0_EIER_EWIE_Msk            (0x2UL)        /*!< EWIE (Bitfield-Mask: 0x01)                            */
#define R_CAN0_EIER_BEIE_Pos            (0UL)          /*!< BEIE (Bit 0)                                          */
#define R_CAN0_EIER_BEIE_Msk            (0x1UL)        /*!< BEIE (Bitfield-Mask: 0x01)                            */
/* =========================================================  EIFR  ========================================================== */
#define R_CAN0_EIFR_BLIF_Pos            (7UL)          /*!< BLIF (Bit 7)                                          */
#define R_CAN0_EIFR_BLIF_Msk            (0x80UL)       /*!< BLIF (Bitfield-Mask: 0x01)                            */
#define R_CAN0_EIFR_OLIF_Pos            (6UL)          /*!< OLIF (Bit 6)                                          */
#define R_CAN0_EIFR_OLIF_Msk            (0x40UL)       /*!< OLIF (Bitfield-Mask: 0x01)                            */
#define R_CAN0_EIFR_ORIF_Pos            (5UL)          /*!< ORIF (Bit 5)                                          */
#define R_CAN0_EIFR_ORIF_Msk            (0x20UL)       /*!< ORIF (Bitfield-Mask: 0x01)                            */
#define R_CAN0_EIFR_BORIF_Pos           (4UL)          /*!< BORIF (Bit 4)                                         */
#define R_CAN0_EIFR_BORIF_Msk           (0x10UL)       /*!< BORIF (Bitfield-Mask: 0x01)                           */
#define R_CAN0_EIFR_BOEIF_Pos           (3UL)          /*!< BOEIF (Bit 3)                                         */
#define R_CAN0_EIFR_BOEIF_Msk           (0x8UL)        /*!< BOEIF (Bitfield-Mask: 0x01)                           */
#define R_CAN0_EIFR_EPIF_Pos            (2UL)          /*!< EPIF (Bit 2)                                          */
#define R_CAN0_EIFR_EPIF_Msk            (0x4UL)        /*!< EPIF (Bitfield-Mask: 0x01)                            */
#define R_CAN0_EIFR_EWIF_Pos            (1UL)          /*!< EWIF (Bit 1)                                          */
#define R_CAN0_EIFR_EWIF_Msk            (0x2UL)        /*!< EWIF (Bitfield-Mask: 0x01)                            */
#define R_CAN0_EIFR_BEIF_Pos            (0UL)          /*!< BEIF (Bit 0)                                          */
#define R_CAN0_EIFR_BEIF_Msk            (0x1UL)        /*!< BEIF (Bitfield-Mask: 0x01)                            */
/* =========================================================  RECR  ========================================================== */
#define R_CAN0_RECR_RECR_Pos            (0UL)          /*!< RECR (Bit 0)                                          */
#define R_CAN0_RECR_RECR_Msk            (0xffUL)       /*!< RECR (Bitfield-Mask: 0xff)                            */
/* =========================================================  TECR  ========================================================== */
#define R_CAN0_TECR_TECR_Pos            (0UL)          /*!< TECR (Bit 0)                                          */
#define R_CAN0_TECR_TECR_Msk            (0xffUL)       /*!< TECR (Bitfield-Mask: 0xff)                            */
/* =========================================================  ECSR  ========================================================== */
#define R_CAN0_ECSR_EDPM_Pos            (7UL)          /*!< EDPM (Bit 7)                                          */
#define R_CAN0_ECSR_EDPM_Msk            (0x80UL)       /*!< EDPM (Bitfield-Mask: 0x01)                            */
#define R_CAN0_ECSR_ADEF_Pos            (6UL)          /*!< ADEF (Bit 6)                                          */
#define R_CAN0_ECSR_ADEF_Msk            (0x40UL)       /*!< ADEF (Bitfield-Mask: 0x01)                            */
#define R_CAN0_ECSR_BE0F_Pos            (5UL)          /*!< BE0F (Bit 5)                                          */
#define R_CAN0_ECSR_BE0F_Msk            (0x20UL)       /*!< BE0F (Bitfield-Mask: 0x01)                            */
#define R_CAN0_ECSR_BE1F_Pos            (4UL)          /*!< BE1F (Bit 4)                                          */
#define R_CAN0_ECSR_BE1F_Msk            (0x10UL)       /*!< BE1F (Bitfield-Mask: 0x01)                            */
#define R_CAN0_ECSR_CEF_Pos             (3UL)          /*!< CEF (Bit 3)                                           */
#define R_CAN0_ECSR_CEF_Msk             (0x8UL)        /*!< CEF (Bitfield-Mask: 0x01)                             */
#define R_CAN0_ECSR_AEF_Pos             (2UL)          /*!< AEF (Bit 2)                                           */
#define R_CAN0_ECSR_AEF_Msk             (0x4UL)        /*!< AEF (Bitfield-Mask: 0x01)                             */
#define R_CAN0_ECSR_FEF_Pos             (1UL)          /*!< FEF (Bit 1)                                           */
#define R_CAN0_ECSR_FEF_Msk             (0x2UL)        /*!< FEF (Bitfield-Mask: 0x01)                             */
#define R_CAN0_ECSR_SEF_Pos             (0UL)          /*!< SEF (Bit 0)                                           */
#define R_CAN0_ECSR_SEF_Msk             (0x1UL)        /*!< SEF (Bitfield-Mask: 0x01)                             */
/* =========================================================  CSSR  ========================================================== */
#define R_CAN0_CSSR_CSSR_Pos            (0UL)          /*!< CSSR (Bit 0)                                          */
#define R_CAN0_CSSR_CSSR_Msk            (0xffUL)       /*!< CSSR (Bitfield-Mask: 0xff)                            */
/* =========================================================  MSSR  ========================================================== */
#define R_CAN0_MSSR_SEST_Pos            (7UL)          /*!< SEST (Bit 7)                                          */
#define R_CAN0_MSSR_SEST_Msk            (0x80UL)       /*!< SEST (Bitfield-Mask: 0x01)                            */
#define R_CAN0_MSSR_MBNST_Pos           (0UL)          /*!< MBNST (Bit 0)                                         */
#define R_CAN0_MSSR_MBNST_Msk           (0x1fUL)       /*!< MBNST (Bitfield-Mask: 0x1f)                           */
/* =========================================================  MSMR  ========================================================== */
#define R_CAN0_MSMR_MBSM_Pos            (0UL)          /*!< MBSM (Bit 0)                                          */
#define R_CAN0_MSMR_MBSM_Msk            (0x3UL)        /*!< MBSM (Bitfield-Mask: 0x03)                            */
/* ==========================================================  TSR  ========================================================== */
#define R_CAN0_TSR_TSR_Pos              (0UL)          /*!< TSR (Bit 0)                                           */
#define R_CAN0_TSR_TSR_Msk              (0xffffUL)     /*!< TSR (Bitfield-Mask: 0xffff)                           */
/* =========================================================  AFSR  ========================================================== */
#define R_CAN0_AFSR_AFSR_Pos            (0UL)          /*!< AFSR (Bit 0)                                          */
#define R_CAN0_AFSR_AFSR_Msk            (0xffffUL)     /*!< AFSR (Bitfield-Mask: 0xffff)                          */
/* ==========================================================  TCR  ========================================================== */
#define R_CAN0_TCR_TSTM_Pos             (1UL)          /*!< TSTM (Bit 1)                                          */
#define R_CAN0_TCR_TSTM_Msk             (0x6UL)        /*!< TSTM (Bitfield-Mask: 0x03)                            */
#define R_CAN0_TCR_TSTE_Pos             (0UL)          /*!< TSTE (Bit 0)                                          */
#define R_CAN0_TCR_TSTE_Msk             (0x1UL)        /*!< TSTE (Bitfield-Mask: 0x01)                            */

#endif /* R_CAN_POSMSK_H */
