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

#ifndef R_PORT_POSMSK_H
#define R_PORT_POSMSK_H

/* ================                                       R_PORT                                       ================ */

/* ==========================================================  PDR  ========================================================== */
#define R_PORT_PDR_B7_Pos        (7UL)    /*!< B7 (Bit 7)                                            */
#define R_PORT_PDR_B7_Msk        (0x80UL) /*!< B7 (Bitfield-Mask: 0x01)                              */
#define R_PORT_PDR_B6_Pos        (6UL)    /*!< B6 (Bit 6)                                            */
#define R_PORT_PDR_B6_Msk        (0x40UL) /*!< B6 (Bitfield-Mask: 0x01)                              */
#define R_PORT_PDR_B5_Pos        (5UL)    /*!< B5 (Bit 5)                                            */
#define R_PORT_PDR_B5_Msk        (0x20UL) /*!< B5 (Bitfield-Mask: 0x01)                              */
#define R_PORT_PDR_B4_Pos        (4UL)    /*!< B4 (Bit 4)                                            */
#define R_PORT_PDR_B4_Msk        (0x10UL) /*!< B4 (Bitfield-Mask: 0x01)                              */
#define R_PORT_PDR_B3_Pos        (3UL)    /*!< B3 (Bit 3)                                            */
#define R_PORT_PDR_B3_Msk        (0x8UL)  /*!< B3 (Bitfield-Mask: 0x01)                              */
#define R_PORT_PDR_B2_Pos        (2UL)    /*!< B2 (Bit 2)                                            */
#define R_PORT_PDR_B2_Msk        (0x4UL)  /*!< B2 (Bitfield-Mask: 0x01)                              */
#define R_PORT_PDR_B1_Pos        (1UL)    /*!< B1 (Bit 1)                                            */
#define R_PORT_PDR_B1_Msk        (0x2UL)  /*!< B1 (Bitfield-Mask: 0x01)                              */
#define R_PORT_PDR_B0_Pos        (0UL)    /*!< B0 (Bit 0)                                            */
#define R_PORT_PDR_B0_Msk        (0x1UL)  /*!< B0 (Bitfield-Mask: 0x01)                              */
/* =========================================================  PODR  ========================================================== */
#define R_PORT_PODR_B7_Pos       (7UL)    /*!< B7 (Bit 7)                                            */
#define R_PORT_PODR_B7_Msk       (0x80UL) /*!< B7 (Bitfield-Mask: 0x01)                              */
#define R_PORT_PODR_B6_Pos       (6UL)    /*!< B6 (Bit 6)                                            */
#define R_PORT_PODR_B6_Msk       (0x40UL) /*!< B6 (Bitfield-Mask: 0x01)                              */
#define R_PORT_PODR_B5_Pos       (5UL)    /*!< B5 (Bit 5)                                            */
#define R_PORT_PODR_B5_Msk       (0x20UL) /*!< B5 (Bitfield-Mask: 0x01)                              */
#define R_PORT_PODR_B4_Pos       (4UL)    /*!< B4 (Bit 4)                                            */
#define R_PORT_PODR_B4_Msk       (0x10UL) /*!< B4 (Bitfield-Mask: 0x01)                              */
#define R_PORT_PODR_B3_Pos       (3UL)    /*!< B3 (Bit 3)                                            */
#define R_PORT_PODR_B3_Msk       (0x8UL)  /*!< B3 (Bitfield-Mask: 0x01)                              */
#define R_PORT_PODR_B2_Pos       (2UL)    /*!< B2 (Bit 2)                                            */
#define R_PORT_PODR_B2_Msk       (0x4UL)  /*!< B2 (Bitfield-Mask: 0x01)                              */
#define R_PORT_PODR_B1_Pos       (1UL)    /*!< B1 (Bit 1)                                            */
#define R_PORT_PODR_B1_Msk       (0x2UL)  /*!< B1 (Bitfield-Mask: 0x01)                              */
#define R_PORT_PODR_B0_Pos       (0UL)    /*!< B0 (Bit 0)                                            */
#define R_PORT_PODR_B0_Msk       (0x1UL)  /*!< B0 (Bitfield-Mask: 0x01)                              */
/* =========================================================  PIDR  ========================================================== */
#define R_PORT_PIDR_B7_Pos       (7UL)    /*!< B7 (Bit 7)                                            */
#define R_PORT_PIDR_B7_Msk       (0x80UL) /*!< B7 (Bitfield-Mask: 0x01)                              */
#define R_PORT_PIDR_B6_Pos       (6UL)    /*!< B6 (Bit 6)                                            */
#define R_PORT_PIDR_B6_Msk       (0x40UL) /*!< B6 (Bitfield-Mask: 0x01)                              */
#define R_PORT_PIDR_B5_Pos       (5UL)    /*!< B5 (Bit 5)                                            */
#define R_PORT_PIDR_B5_Msk       (0x20UL) /*!< B5 (Bitfield-Mask: 0x01)                              */
#define R_PORT_PIDR_B4_Pos       (4UL)    /*!< B4 (Bit 4)                                            */
#define R_PORT_PIDR_B4_Msk       (0x10UL) /*!< B4 (Bitfield-Mask: 0x01)                              */
#define R_PORT_PIDR_B3_Pos       (3UL)    /*!< B3 (Bit 3)                                            */
#define R_PORT_PIDR_B3_Msk       (0x8UL)  /*!< B3 (Bitfield-Mask: 0x01)                              */
#define R_PORT_PIDR_B2_Pos       (2UL)    /*!< B2 (Bit 2)                                            */
#define R_PORT_PIDR_B2_Msk       (0x4UL)  /*!< B2 (Bitfield-Mask: 0x01)                              */
#define R_PORT_PIDR_B1_Pos       (1UL)    /*!< B1 (Bit 1)                                            */
#define R_PORT_PIDR_B1_Msk       (0x2UL)  /*!< B1 (Bitfield-Mask: 0x01)                              */
#define R_PORT_PIDR_B0_Pos       (0UL)    /*!< B0 (Bit 0)                                            */
#define R_PORT_PIDR_B0_Msk       (0x1UL)  /*!< B0 (Bitfield-Mask: 0x01)                              */
/* ==========================================================  PMR  ========================================================== */
#define R_PORT_PMR_B7_Pos        (7UL)    /*!< B7 (Bit 7)                                            */
#define R_PORT_PMR_B7_Msk        (0x80UL) /*!< B7 (Bitfield-Mask: 0x01)                              */
#define R_PORT_PMR_B6_Pos        (6UL)    /*!< B6 (Bit 6)                                            */
#define R_PORT_PMR_B6_Msk        (0x40UL) /*!< B6 (Bitfield-Mask: 0x01)                              */
#define R_PORT_PMR_B5_Pos        (5UL)    /*!< B5 (Bit 5)                                            */
#define R_PORT_PMR_B5_Msk        (0x20UL) /*!< B5 (Bitfield-Mask: 0x01)                              */
#define R_PORT_PMR_B4_Pos        (4UL)    /*!< B4 (Bit 4)                                            */
#define R_PORT_PMR_B4_Msk        (0x10UL) /*!< B4 (Bitfield-Mask: 0x01)                              */
#define R_PORT_PMR_B3_Pos        (3UL)    /*!< B3 (Bit 3)                                            */
#define R_PORT_PMR_B3_Msk        (0x8UL)  /*!< B3 (Bitfield-Mask: 0x01)                              */
#define R_PORT_PMR_B2_Pos        (2UL)    /*!< B2 (Bit 2)                                            */
#define R_PORT_PMR_B2_Msk        (0x4UL)  /*!< B2 (Bitfield-Mask: 0x01)                              */
#define R_PORT_PMR_B1_Pos        (1UL)    /*!< B1 (Bit 1)                                            */
#define R_PORT_PMR_B1_Msk        (0x2UL)  /*!< B1 (Bitfield-Mask: 0x01)                              */
#define R_PORT_PMR_B0_Pos        (0UL)    /*!< B0 (Bit 0)                                            */
#define R_PORT_PMR_B0_Msk        (0x1UL)  /*!< B0 (Bitfield-Mask: 0x01)                              */
/* ==========================================================  PCR  ========================================================== */
#define R_PORT_PCR_B7_Pos        (7UL)    /*!< B7 (Bit 7)                                            */
#define R_PORT_PCR_B7_Msk        (0x80UL) /*!< B7 (Bitfield-Mask: 0x01)                              */
#define R_PORT_PCR_B6_Pos        (6UL)    /*!< B6 (Bit 6)                                            */
#define R_PORT_PCR_B6_Msk        (0x40UL) /*!< B6 (Bitfield-Mask: 0x01)                              */
#define R_PORT_PCR_B5_Pos        (5UL)    /*!< B5 (Bit 5)                                            */
#define R_PORT_PCR_B5_Msk        (0x20UL) /*!< B5 (Bitfield-Mask: 0x01)                              */
#define R_PORT_PCR_B4_Pos        (4UL)    /*!< B4 (Bit 4)                                            */
#define R_PORT_PCR_B4_Msk        (0x10UL) /*!< B4 (Bitfield-Mask: 0x01)                              */
#define R_PORT_PCR_B3_Pos        (3UL)    /*!< B3 (Bit 3)                                            */
#define R_PORT_PCR_B3_Msk        (0x8UL)  /*!< B3 (Bitfield-Mask: 0x01)                              */
#define R_PORT_PCR_B2_Pos        (2UL)    /*!< B2 (Bit 2)                                            */
#define R_PORT_PCR_B2_Msk        (0x4UL)  /*!< B2 (Bitfield-Mask: 0x01)                              */
#define R_PORT_PCR_B1_Pos        (1UL)    /*!< B1 (Bit 1)                                            */
#define R_PORT_PCR_B1_Msk        (0x2UL)  /*!< B1 (Bitfield-Mask: 0x01)                              */
#define R_PORT_PCR_B0_Pos        (0UL)    /*!< B0 (Bit 0)                                            */
#define R_PORT_PCR_B0_Msk        (0x1UL)  /*!< B0 (Bitfield-Mask: 0x01)                              */
/* =========================================================  DSCR  ========================================================== */
#define R_PORT_DSCR_B7_Pos       (7UL)    /*!< B7 (Bit 7)                                            */
#define R_PORT_DSCR_B7_Msk       (0x80UL) /*!< B7 (Bitfield-Mask: 0x01)                              */
#define R_PORT_DSCR_B6_Pos       (6UL)    /*!< B6 (Bit 6)                                            */
#define R_PORT_DSCR_B6_Msk       (0x40UL) /*!< B6 (Bitfield-Mask: 0x01)                              */
#define R_PORT_DSCR_B5_Pos       (5UL)    /*!< B5 (Bit 5)                                            */
#define R_PORT_DSCR_B5_Msk       (0x20UL) /*!< B5 (Bitfield-Mask: 0x01)                              */
#define R_PORT_DSCR_B4_Pos       (4UL)    /*!< B4 (Bit 4)                                            */
#define R_PORT_DSCR_B4_Msk       (0x10UL) /*!< B4 (Bitfield-Mask: 0x01)                              */
#define R_PORT_DSCR_B3_Pos       (3UL)    /*!< B3 (Bit 3)                                            */
#define R_PORT_DSCR_B3_Msk       (0x8UL)  /*!< B3 (Bitfield-Mask: 0x01)                              */
#define R_PORT_DSCR_B2_Pos       (2UL)    /*!< B2 (Bit 2)                                            */
#define R_PORT_DSCR_B2_Msk       (0x4UL)  /*!< B2 (Bitfield-Mask: 0x01)                              */
#define R_PORT_DSCR_B1_Pos       (1UL)    /*!< B1 (Bit 1)                                            */
#define R_PORT_DSCR_B1_Msk       (0x2UL)  /*!< B1 (Bitfield-Mask: 0x01)                              */
#define R_PORT_DSCR_B0_Pos       (0UL)    /*!< B0 (Bit 0)                                            */
#define R_PORT_DSCR_B0_Msk       (0x1UL)  /*!< B0 (Bitfield-Mask: 0x01)                              */
/* ========================================================  DSCR_ty  ======================================================== */
#define R_PORT_DSCR_ty_B7_Pos    (7UL)    /*!< B7 (Bit 7)                                            */
#define R_PORT_DSCR_ty_B7_Msk    (0x80UL) /*!< B7 (Bitfield-Mask: 0x01)                              */
#define R_PORT_DSCR_ty_B6_Pos    (6UL)    /*!< B6 (Bit 6)                                            */
#define R_PORT_DSCR_ty_B6_Msk    (0x40UL) /*!< B6 (Bitfield-Mask: 0x01)                              */
#define R_PORT_DSCR_ty_B5_Pos    (5UL)    /*!< B5 (Bit 5)                                            */
#define R_PORT_DSCR_ty_B5_Msk    (0x20UL) /*!< B5 (Bitfield-Mask: 0x01)                              */
#define R_PORT_DSCR_ty_B4_Pos    (4UL)    /*!< B4 (Bit 4)                                            */
#define R_PORT_DSCR_ty_B4_Msk    (0x10UL) /*!< B4 (Bitfield-Mask: 0x01)                              */
#define R_PORT_DSCR_ty_B3_Pos    (3UL)    /*!< B3 (Bit 3)                                            */
#define R_PORT_DSCR_ty_B3_Msk    (0x8UL)  /*!< B3 (Bitfield-Mask: 0x01)                              */
#define R_PORT_DSCR_ty_B2_Pos    (2UL)    /*!< B2 (Bit 2)                                            */
#define R_PORT_DSCR_ty_B2_Msk    (0x4UL)  /*!< B2 (Bitfield-Mask: 0x01)                              */
#define R_PORT_DSCR_ty_B1_Pos    (1UL)    /*!< B1 (Bit 1)                                            */
#define R_PORT_DSCR_ty_B1_Msk    (0x2UL)  /*!< B1 (Bitfield-Mask: 0x01)                              */
#define R_PORT_DSCR_ty_B0_Pos    (0UL)    /*!< B0 (Bit 0)                                            */
#define R_PORT_DSCR_ty_B0_Msk    (0x1UL)  /*!< B0 (Bitfield-Mask: 0x01)                                                         */

#endif /* R_PORT_POSMSK_H */
