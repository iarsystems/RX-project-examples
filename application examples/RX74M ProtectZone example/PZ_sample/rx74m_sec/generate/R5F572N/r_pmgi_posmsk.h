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

#ifndef R_PMGI_POSMSK_H
#define R_PMGI_POSMSK_H

/* =========================================================  PMGCR  ========================================================= */
#define R_PMGI0_PMGCR_PSMCT_Pos    (20UL)         /*!< PSMCT (Bit 20)                                        */
#define R_PMGI0_PMGCR_PSMCT_Msk    (0x700000UL)   /*!< PSMCT (Bitfield-Mask: 0x07)                           */
#define R_PMGI0_PMGCR_PSMHT_Pos    (16UL)         /*!< PSMHT (Bit 16)                                        */
#define R_PMGI0_PMGCR_PSMHT_Msk    (0x70000UL)    /*!< PSMHT (Bitfield-Mask: 0x07)                           */
#define R_PMGI0_PMGCR_PSMDP_Pos    (15UL)         /*!< PSMDP (Bit 15)                                        */
#define R_PMGI0_PMGCR_PSMDP_Msk    (0x8000UL)     /*!< PSMDP (Bitfield-Mask: 0x01)                           */
#define R_PMGI0_PMGCR_PSMCS_Pos    (8UL)          /*!< PSMCS (Bit 8)                                         */
#define R_PMGI0_PMGCR_PSMCS_Msk    (0x3f00UL)     /*!< PSMCS (Bitfield-Mask: 0x3f)                           */
/* =========================================================  PSMR  ========================================================== */
#define R_PMGI0_PSMR_PRD_Pos       (16UL)         /*!< PRD (Bit 16)                                          */
#define R_PMGI0_PSMR_PRD_Msk       (0xffff0000UL) /*!< PRD (Bitfield-Mask: 0xffff)                           */
#define R_PMGI0_PSMR_PRA_Pos       (8UL)          /*!< PRA (Bit 8)                                           */
#define R_PMGI0_PSMR_PRA_Msk       (0x1f00UL)     /*!< PRA (Bitfield-Mask: 0x1f)                             */
#define R_PMGI0_PSMR_PDA_Pos       (3UL)          /*!< PDA (Bit 3)                                           */
#define R_PMGI0_PSMR_PDA_Msk       (0xf8UL)       /*!< PDA (Bitfield-Mask: 0x1f)                             */
#define R_PMGI0_PSMR_PSMAD_Pos     (1UL)          /*!< PSMAD (Bit 1)                                         */
#define R_PMGI0_PSMR_PSMAD_Msk     (0x2UL)        /*!< PSMAD (Bitfield-Mask: 0x01)                           */
#define R_PMGI0_PSMR_PSME_Pos      (0UL)          /*!< PSME (Bit 0)                                          */
#define R_PMGI0_PSMR_PSME_Msk      (0x1UL)        /*!< PSME (Bitfield-Mask: 0x01)                            */

#endif /* R_PMGI_POSMSK_H */
