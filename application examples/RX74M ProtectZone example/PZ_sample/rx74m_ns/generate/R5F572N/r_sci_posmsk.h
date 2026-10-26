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

#ifndef R_SCI_POSMSK_H
#define R_SCI_POSMSK_H

/* =========================================================================================================================== */
/* ================                                          R_SCI0                                           ================ */
/* =========================================================================================================================== */

/* ==========================================================  RDR  ========================================================== */
#define R_SCI0_RDR_RDR_Pos             (0UL)      /*!< RDR (Bit 0)                                           */
#define R_SCI0_RDR_RDR_Msk             (0xffUL)   /*!< RDR (Bitfield-Mask: 0xff)                             */
/* =========================================================  RDRHL  ========================================================= */
#define R_SCI0_RDRHL_RDRHL_Pos         (0UL)      /*!< RDRHL (Bit 0)                                         */
#define R_SCI0_RDRHL_RDRHL_Msk         (0xffffUL) /*!< RDRHL (Bitfield-Mask: 0xffff)                         */
/* ========================================================  FRDRHL  ========================================================= */
#define R_SCI0_FRDRHL_RDF_Pos          (14UL)     /*!< RDF (Bit 14)                                          */
#define R_SCI0_FRDRHL_RDF_Msk          (0x4000UL) /*!< RDF (Bitfield-Mask: 0x01)                             */
#define R_SCI0_FRDRHL_ORER_Pos         (13UL)     /*!< ORER (Bit 13)                                         */
#define R_SCI0_FRDRHL_ORER_Msk         (0x2000UL) /*!< ORER (Bitfield-Mask: 0x01)                            */
#define R_SCI0_FRDRHL_FER_Pos          (12UL)     /*!< FER (Bit 12)                                          */
#define R_SCI0_FRDRHL_FER_Msk          (0x1000UL) /*!< FER (Bitfield-Mask: 0x01)                             */
#define R_SCI0_FRDRHL_PER_Pos          (11UL)     /*!< PER (Bit 11)                                          */
#define R_SCI0_FRDRHL_PER_Msk          (0x800UL)  /*!< PER (Bitfield-Mask: 0x01)                             */
#define R_SCI0_FRDRHL_DR_Pos           (10UL)     /*!< DR (Bit 10)                                           */
#define R_SCI0_FRDRHL_DR_Msk           (0x400UL)  /*!< DR (Bitfield-Mask: 0x01)                              */
#define R_SCI0_FRDRHL_MPB_Pos          (9UL)      /*!< MPB (Bit 9)                                           */
#define R_SCI0_FRDRHL_MPB_Msk          (0x200UL)  /*!< MPB (Bitfield-Mask: 0x01)                             */
#define R_SCI0_FRDRHL_RDAT_Pos         (0UL)      /*!< RDAT (Bit 0)                                          */
#define R_SCI0_FRDRHL_RDAT_Msk         (0x1ffUL)  /*!< RDAT (Bitfield-Mask: 0x1ff)                           */
/* =========================================================  FRDRH  ========================================================= */
#define R_SCI0_FRDRH_RDF_Pos           (6UL)      /*!< RDF (Bit 6)                                           */
#define R_SCI0_FRDRH_RDF_Msk           (0x40UL)   /*!< RDF (Bitfield-Mask: 0x01)                             */
#define R_SCI0_FRDRH_ORER_Pos          (5UL)      /*!< ORER (Bit 5)                                          */
#define R_SCI0_FRDRH_ORER_Msk          (0x20UL)   /*!< ORER (Bitfield-Mask: 0x01)                            */
#define R_SCI0_FRDRH_FER_Pos           (4UL)      /*!< FER (Bit 4)                                           */
#define R_SCI0_FRDRH_FER_Msk           (0x10UL)   /*!< FER (Bitfield-Mask: 0x01)                             */
#define R_SCI0_FRDRH_PER_Pos           (3UL)      /*!< PER (Bit 3)                                           */
#define R_SCI0_FRDRH_PER_Msk           (0x8UL)    /*!< PER (Bitfield-Mask: 0x01)                             */
#define R_SCI0_FRDRH_DR_Pos            (2UL)      /*!< DR (Bit 2)                                            */
#define R_SCI0_FRDRH_DR_Msk            (0x4UL)    /*!< DR (Bitfield-Mask: 0x01)                              */
#define R_SCI0_FRDRH_MPB_Pos           (1UL)      /*!< MPB (Bit 1)                                           */
#define R_SCI0_FRDRH_MPB_Msk           (0x2UL)    /*!< MPB (Bitfield-Mask: 0x01)                             */
#define R_SCI0_FRDRH_RDATH_Pos         (0UL)      /*!< RDATH (Bit 0)                                         */
#define R_SCI0_FRDRH_RDATH_Msk         (0x1UL)    /*!< RDATH (Bitfield-Mask: 0x01)                           */
/* =========================================================  FRDRL  ========================================================= */
#define R_SCI0_FRDRL_RDATL_Pos         (0UL)      /*!< RDATL (Bit 0)                                         */
#define R_SCI0_FRDRL_RDATL_Msk         (0xffUL)   /*!< RDATL (Bitfield-Mask: 0xff)                           */
/* ==========================================================  TDR  ========================================================== */
#define R_SCI0_TDR_TDR_Pos             (0UL)      /*!< TDR (Bit 0)                                           */
#define R_SCI0_TDR_TDR_Msk             (0xffUL)   /*!< TDR (Bitfield-Mask: 0xff)                             */
/* =========================================================  TDRHL  ========================================================= */
#define R_SCI0_TDRHL_TDRHL_Pos         (0UL)      /*!< TDRHL (Bit 0)                                         */
#define R_SCI0_TDRHL_TDRHL_Msk         (0xffffUL) /*!< TDRHL (Bitfield-Mask: 0xffff)                         */
/* ========================================================  FTDRHL  ========================================================= */
#define R_SCI0_FTDRHL_MPBT_Pos         (9UL)      /*!< MPBT (Bit 9)                                          */
#define R_SCI0_FTDRHL_MPBT_Msk         (0x200UL)  /*!< MPBT (Bitfield-Mask: 0x01)                            */
#define R_SCI0_FTDRHL_TDAT_Pos         (0UL)      /*!< TDAT (Bit 0)                                          */
#define R_SCI0_FTDRHL_TDAT_Msk         (0x1ffUL)  /*!< TDAT (Bitfield-Mask: 0x1ff)                           */
/* =========================================================  FTDRH  ========================================================= */
#define R_SCI0_FTDRH_MPBT_Pos          (1UL)      /*!< MPBT (Bit 1)                                          */
#define R_SCI0_FTDRH_MPBT_Msk          (0x2UL)    /*!< MPBT (Bitfield-Mask: 0x01)                            */
#define R_SCI0_FTDRH_TDATH_Pos         (0UL)      /*!< TDATH (Bit 0)                                         */
#define R_SCI0_FTDRH_TDATH_Msk         (0x1UL)    /*!< TDATH (Bitfield-Mask: 0x01)                           */
/* =========================================================  FTDRL  ========================================================= */
#define R_SCI0_FTDRL_TDATL_Pos         (0UL)      /*!< TDATL (Bit 0)                                         */
#define R_SCI0_FTDRL_TDATL_Msk         (0xffUL)   /*!< TDATL (Bitfield-Mask: 0xff)                           */
/* ==========================================================  SMR  ========================================================== */
#define R_SCI0_SMR_CM_Pos              (7UL)      /*!< CM (Bit 7)                                            */
#define R_SCI0_SMR_CM_Msk              (0x80UL)   /*!< CM (Bitfield-Mask: 0x01)                              */
#define R_SCI0_SMR_CHR_Pos             (6UL)      /*!< CHR (Bit 6)                                           */
#define R_SCI0_SMR_CHR_Msk             (0x40UL)   /*!< CHR (Bitfield-Mask: 0x01)                             */
#define R_SCI0_SMR_PE_Pos              (5UL)      /*!< PE (Bit 5)                                            */
#define R_SCI0_SMR_PE_Msk              (0x20UL)   /*!< PE (Bitfield-Mask: 0x01)                              */
#define R_SCI0_SMR_PM_Pos              (4UL)      /*!< PM (Bit 4)                                            */
#define R_SCI0_SMR_PM_Msk              (0x10UL)   /*!< PM (Bitfield-Mask: 0x01)                              */
#define R_SCI0_SMR_STOP_Pos            (3UL)      /*!< STOP (Bit 3)                                          */
#define R_SCI0_SMR_STOP_Msk            (0x8UL)    /*!< STOP (Bitfield-Mask: 0x01)                            */
#define R_SCI0_SMR_MP_Pos              (2UL)      /*!< MP (Bit 2)                                            */
#define R_SCI0_SMR_MP_Msk              (0x4UL)    /*!< MP (Bitfield-Mask: 0x01)                              */
#define R_SCI0_SMR_CKS_Pos             (0UL)      /*!< CKS (Bit 0)                                           */
#define R_SCI0_SMR_CKS_Msk             (0x3UL)    /*!< CKS (Bitfield-Mask: 0x03)                             */
/* =======================================================  SMR_SMCI  ======================================================== */
#define R_SCI0_SMR_SMCI_GM_Pos         (7UL)      /*!< GM (Bit 7)                                            */
#define R_SCI0_SMR_SMCI_GM_Msk         (0x80UL)   /*!< GM (Bitfield-Mask: 0x01)                              */
#define R_SCI0_SMR_SMCI_BLK_Pos        (6UL)      /*!< BLK (Bit 6)                                           */
#define R_SCI0_SMR_SMCI_BLK_Msk        (0x40UL)   /*!< BLK (Bitfield-Mask: 0x01)                             */
#define R_SCI0_SMR_SMCI_PE_Pos         (5UL)      /*!< PE (Bit 5)                                            */
#define R_SCI0_SMR_SMCI_PE_Msk         (0x20UL)   /*!< PE (Bitfield-Mask: 0x01)                              */
#define R_SCI0_SMR_SMCI_PM_Pos         (4UL)      /*!< PM (Bit 4)                                            */
#define R_SCI0_SMR_SMCI_PM_Msk         (0x10UL)   /*!< PM (Bitfield-Mask: 0x01)                              */
#define R_SCI0_SMR_SMCI_BCP_Pos        (2UL)      /*!< BCP (Bit 2)                                           */
#define R_SCI0_SMR_SMCI_BCP_Msk        (0xcUL)    /*!< BCP (Bitfield-Mask: 0x03)                             */
#define R_SCI0_SMR_SMCI_CKS_Pos        (0UL)      /*!< CKS (Bit 0)                                           */
#define R_SCI0_SMR_SMCI_CKS_Msk        (0x3UL)    /*!< CKS (Bitfield-Mask: 0x03)                             */
/* ==========================================================  SCR  ========================================================== */
#define R_SCI0_SCR_TIE_Pos             (7UL)      /*!< TIE (Bit 7)                                           */
#define R_SCI0_SCR_TIE_Msk             (0x80UL)   /*!< TIE (Bitfield-Mask: 0x01)                             */
#define R_SCI0_SCR_RIE_Pos             (6UL)      /*!< RIE (Bit 6)                                           */
#define R_SCI0_SCR_RIE_Msk             (0x40UL)   /*!< RIE (Bitfield-Mask: 0x01)                             */
#define R_SCI0_SCR_TE_Pos              (5UL)      /*!< TE (Bit 5)                                            */
#define R_SCI0_SCR_TE_Msk              (0x20UL)   /*!< TE (Bitfield-Mask: 0x01)                              */
#define R_SCI0_SCR_RE_Pos              (4UL)      /*!< RE (Bit 4)                                            */
#define R_SCI0_SCR_RE_Msk              (0x10UL)   /*!< RE (Bitfield-Mask: 0x01)                              */
#define R_SCI0_SCR_MPIE_Pos            (3UL)      /*!< MPIE (Bit 3)                                          */
#define R_SCI0_SCR_MPIE_Msk            (0x8UL)    /*!< MPIE (Bitfield-Mask: 0x01)                            */
#define R_SCI0_SCR_TEIE_Pos            (2UL)      /*!< TEIE (Bit 2)                                          */
#define R_SCI0_SCR_TEIE_Msk            (0x4UL)    /*!< TEIE (Bitfield-Mask: 0x01)                            */
#define R_SCI0_SCR_CKE_Pos             (0UL)      /*!< CKE (Bit 0)                                           */
#define R_SCI0_SCR_CKE_Msk             (0x3UL)    /*!< CKE (Bitfield-Mask: 0x03)                             */
/* =======================================================  SCR_SMCI  ======================================================== */
#define R_SCI0_SCR_SMCI_TIE_Pos        (7UL)      /*!< TIE (Bit 7)                                           */
#define R_SCI0_SCR_SMCI_TIE_Msk        (0x80UL)   /*!< TIE (Bitfield-Mask: 0x01)                             */
#define R_SCI0_SCR_SMCI_RIE_Pos        (6UL)      /*!< RIE (Bit 6)                                           */
#define R_SCI0_SCR_SMCI_RIE_Msk        (0x40UL)   /*!< RIE (Bitfield-Mask: 0x01)                             */
#define R_SCI0_SCR_SMCI_TE_Pos         (5UL)      /*!< TE (Bit 5)                                            */
#define R_SCI0_SCR_SMCI_TE_Msk         (0x20UL)   /*!< TE (Bitfield-Mask: 0x01)                              */
#define R_SCI0_SCR_SMCI_RE_Pos         (4UL)      /*!< RE (Bit 4)                                            */
#define R_SCI0_SCR_SMCI_RE_Msk         (0x10UL)   /*!< RE (Bitfield-Mask: 0x01)                              */
#define R_SCI0_SCR_SMCI_MPIE_Pos       (3UL)      /*!< MPIE (Bit 3)                                          */
#define R_SCI0_SCR_SMCI_MPIE_Msk       (0x8UL)    /*!< MPIE (Bitfield-Mask: 0x01)                            */
#define R_SCI0_SCR_SMCI_TEIE_Pos       (2UL)      /*!< TEIE (Bit 2)                                          */
#define R_SCI0_SCR_SMCI_TEIE_Msk       (0x4UL)    /*!< TEIE (Bitfield-Mask: 0x01)                            */
#define R_SCI0_SCR_SMCI_CKE_Pos        (0UL)      /*!< CKE (Bit 0)                                           */
#define R_SCI0_SCR_SMCI_CKE_Msk        (0x3UL)    /*!< CKE (Bitfield-Mask: 0x03)                             */
/* ==========================================================  SSR  ========================================================== */
#define R_SCI0_SSR_TDRE_Pos            (7UL)      /*!< TDRE (Bit 7)                                          */
#define R_SCI0_SSR_TDRE_Msk            (0x80UL)   /*!< TDRE (Bitfield-Mask: 0x01)                            */
#define R_SCI0_SSR_RDRF_Pos            (6UL)      /*!< RDRF (Bit 6)                                          */
#define R_SCI0_SSR_RDRF_Msk            (0x40UL)   /*!< RDRF (Bitfield-Mask: 0x01)                            */
#define R_SCI0_SSR_ORER_Pos            (5UL)      /*!< ORER (Bit 5)                                          */
#define R_SCI0_SSR_ORER_Msk            (0x20UL)   /*!< ORER (Bitfield-Mask: 0x01)                            */
#define R_SCI0_SSR_FER_Pos             (4UL)      /*!< FER (Bit 4)                                           */
#define R_SCI0_SSR_FER_Msk             (0x10UL)   /*!< FER (Bitfield-Mask: 0x01)                             */
#define R_SCI0_SSR_PER_Pos             (3UL)      /*!< PER (Bit 3)                                           */
#define R_SCI0_SSR_PER_Msk             (0x8UL)    /*!< PER (Bitfield-Mask: 0x01)                             */
#define R_SCI0_SSR_TEND_Pos            (2UL)      /*!< TEND (Bit 2)                                          */
#define R_SCI0_SSR_TEND_Msk            (0x4UL)    /*!< TEND (Bitfield-Mask: 0x01)                            */
#define R_SCI0_SSR_MPB_Pos             (1UL)      /*!< MPB (Bit 1)                                           */
#define R_SCI0_SSR_MPB_Msk             (0x2UL)    /*!< MPB (Bitfield-Mask: 0x01)                             */
#define R_SCI0_SSR_MPBT_Pos            (0UL)      /*!< MPBT (Bit 0)                                          */
#define R_SCI0_SSR_MPBT_Msk            (0x1UL)    /*!< MPBT (Bitfield-Mask: 0x01)                            */
/* =======================================================  SSR_FIFO  ======================================================== */
#define R_SCI0_SSR_FIFO_TDFE_Pos       (7UL)      /*!< TDFE (Bit 7)                                          */
#define R_SCI0_SSR_FIFO_TDFE_Msk       (0x80UL)   /*!< TDFE (Bitfield-Mask: 0x01)                            */
#define R_SCI0_SSR_FIFO_RDF_Pos        (6UL)      /*!< RDF (Bit 6)                                           */
#define R_SCI0_SSR_FIFO_RDF_Msk        (0x40UL)   /*!< RDF (Bitfield-Mask: 0x01)                             */
#define R_SCI0_SSR_FIFO_ORER_Pos       (5UL)      /*!< ORER (Bit 5)                                          */
#define R_SCI0_SSR_FIFO_ORER_Msk       (0x20UL)   /*!< ORER (Bitfield-Mask: 0x01)                            */
#define R_SCI0_SSR_FIFO_FER_Pos        (4UL)      /*!< FER (Bit 4)                                           */
#define R_SCI0_SSR_FIFO_FER_Msk        (0x10UL)   /*!< FER (Bitfield-Mask: 0x01)                             */
#define R_SCI0_SSR_FIFO_PER_Pos        (3UL)      /*!< PER (Bit 3)                                           */
#define R_SCI0_SSR_FIFO_PER_Msk        (0x8UL)    /*!< PER (Bitfield-Mask: 0x01)                             */
#define R_SCI0_SSR_FIFO_TEND_Pos       (2UL)      /*!< TEND (Bit 2)                                          */
#define R_SCI0_SSR_FIFO_TEND_Msk       (0x4UL)    /*!< TEND (Bitfield-Mask: 0x01)                            */
#define R_SCI0_SSR_FIFO_DR_Pos         (0UL)      /*!< DR (Bit 0)                                            */
#define R_SCI0_SSR_FIFO_DR_Msk         (0x1UL)    /*!< DR (Bitfield-Mask: 0x01)                              */
/* =======================================================  SSR_SMCI  ======================================================== */
#define R_SCI0_SSR_SMCI_TDRE_Pos       (7UL)      /*!< TDRE (Bit 7)                                          */
#define R_SCI0_SSR_SMCI_TDRE_Msk       (0x80UL)   /*!< TDRE (Bitfield-Mask: 0x01)                            */
#define R_SCI0_SSR_SMCI_RDRF_Pos       (6UL)      /*!< RDRF (Bit 6)                                          */
#define R_SCI0_SSR_SMCI_RDRF_Msk       (0x40UL)   /*!< RDRF (Bitfield-Mask: 0x01)                            */
#define R_SCI0_SSR_SMCI_ORER_Pos       (5UL)      /*!< ORER (Bit 5)                                          */
#define R_SCI0_SSR_SMCI_ORER_Msk       (0x20UL)   /*!< ORER (Bitfield-Mask: 0x01)                            */
#define R_SCI0_SSR_SMCI_ERS_Pos        (4UL)      /*!< ERS (Bit 4)                                           */
#define R_SCI0_SSR_SMCI_ERS_Msk        (0x10UL)   /*!< ERS (Bitfield-Mask: 0x01)                             */
#define R_SCI0_SSR_SMCI_PER_Pos        (3UL)      /*!< PER (Bit 3)                                           */
#define R_SCI0_SSR_SMCI_PER_Msk        (0x8UL)    /*!< PER (Bitfield-Mask: 0x01)                             */
#define R_SCI0_SSR_SMCI_TEND_Pos       (2UL)      /*!< TEND (Bit 2)                                          */
#define R_SCI0_SSR_SMCI_TEND_Msk       (0x4UL)    /*!< TEND (Bitfield-Mask: 0x01)                            */
#define R_SCI0_SSR_SMCI_MPB_Pos        (1UL)      /*!< MPB (Bit 1)                                           */
#define R_SCI0_SSR_SMCI_MPB_Msk        (0x2UL)    /*!< MPB (Bitfield-Mask: 0x01)                             */
#define R_SCI0_SSR_SMCI_MPBT_Pos       (0UL)      /*!< MPBT (Bit 0)                                          */
#define R_SCI0_SSR_SMCI_MPBT_Msk       (0x1UL)    /*!< MPBT (Bitfield-Mask: 0x01)                            */
/* =========================================================  SCMR  ========================================================== */
#define R_SCI0_SCMR_BCP2_Pos           (7UL)      /*!< BCP2 (Bit 7)                                          */
#define R_SCI0_SCMR_BCP2_Msk           (0x80UL)   /*!< BCP2 (Bitfield-Mask: 0x01)                            */
#define R_SCI0_SCMR_CHR1_Pos           (4UL)      /*!< CHR1 (Bit 4)                                          */
#define R_SCI0_SCMR_CHR1_Msk           (0x10UL)   /*!< CHR1 (Bitfield-Mask: 0x01)                            */
#define R_SCI0_SCMR_SDIR_Pos           (3UL)      /*!< SDIR (Bit 3)                                          */
#define R_SCI0_SCMR_SDIR_Msk           (0x8UL)    /*!< SDIR (Bitfield-Mask: 0x01)                            */
#define R_SCI0_SCMR_SINV_Pos           (2UL)      /*!< SINV (Bit 2)                                          */
#define R_SCI0_SCMR_SINV_Msk           (0x4UL)    /*!< SINV (Bitfield-Mask: 0x01)                            */
#define R_SCI0_SCMR_SMIF_Pos           (0UL)      /*!< SMIF (Bit 0)                                          */
#define R_SCI0_SCMR_SMIF_Msk           (0x1UL)    /*!< SMIF (Bitfield-Mask: 0x01)                            */
/* ==========================================================  BRR  ========================================================== */
#define R_SCI0_BRR_BRR_Pos             (0UL)      /*!< BRR (Bit 0)                                           */
#define R_SCI0_BRR_BRR_Msk             (0xffUL)   /*!< BRR (Bitfield-Mask: 0xff)                             */
/* =========================================================  MDDR  ========================================================== */
#define R_SCI0_MDDR_MDDR_Pos           (0UL)      /*!< MDDR (Bit 0)                                          */
#define R_SCI0_MDDR_MDDR_Msk           (0xffUL)   /*!< MDDR (Bitfield-Mask: 0xff)                            */
/* =========================================================  SEMR  ========================================================== */
#define R_SCI0_SEMR_RXDESEL_Pos        (7UL)      /*!< RXDESEL (Bit 7)                                       */
#define R_SCI0_SEMR_RXDESEL_Msk        (0x80UL)   /*!< RXDESEL (Bitfield-Mask: 0x01)                         */
#define R_SCI0_SEMR_BGDM_Pos           (6UL)      /*!< BGDM (Bit 6)                                          */
#define R_SCI0_SEMR_BGDM_Msk           (0x40UL)   /*!< BGDM (Bitfield-Mask: 0x01)                            */
#define R_SCI0_SEMR_NFEN_Pos           (5UL)      /*!< NFEN (Bit 5)                                          */
#define R_SCI0_SEMR_NFEN_Msk           (0x20UL)   /*!< NFEN (Bitfield-Mask: 0x01)                            */
#define R_SCI0_SEMR_ABCS_Pos           (4UL)      /*!< ABCS (Bit 4)                                          */
#define R_SCI0_SEMR_ABCS_Msk           (0x10UL)   /*!< ABCS (Bitfield-Mask: 0x01)                            */
#define R_SCI0_SEMR_ABCSE_Pos          (3UL)      /*!< ABCSE (Bit 3)                                         */
#define R_SCI0_SEMR_ABCSE_Msk          (0x8UL)    /*!< ABCSE (Bitfield-Mask: 0x01)                           */
#define R_SCI0_SEMR_BRME_Pos           (2UL)      /*!< BRME (Bit 2)                                          */
#define R_SCI0_SEMR_BRME_Msk           (0x4UL)    /*!< BRME (Bitfield-Mask: 0x01)                            */
#define R_SCI0_SEMR_ACS0_Pos           (0UL)      /*!< ACS0 (Bit 0)                                          */
#define R_SCI0_SEMR_ACS0_Msk           (0x1UL)    /*!< ACS0 (Bitfield-Mask: 0x01)                            */
/* =========================================================  SNFR  ========================================================== */
#define R_SCI0_SNFR_NFCS_Pos           (0UL)      /*!< NFCS (Bit 0)                                          */
#define R_SCI0_SNFR_NFCS_Msk           (0x7UL)    /*!< NFCS (Bitfield-Mask: 0x07)                            */
/* =========================================================  SIMR1  ========================================================= */
#define R_SCI0_SIMR1_IICDL_Pos         (3UL)      /*!< IICDL (Bit 3)                                         */
#define R_SCI0_SIMR1_IICDL_Msk         (0xf8UL)   /*!< IICDL (Bitfield-Mask: 0x1f)                           */
#define R_SCI0_SIMR1_IICM_Pos          (0UL)      /*!< IICM (Bit 0)                                          */
#define R_SCI0_SIMR1_IICM_Msk          (0x1UL)    /*!< IICM (Bitfield-Mask: 0x01)                            */
/* =========================================================  SIMR2  ========================================================= */
#define R_SCI0_SIMR2_IICACKT_Pos       (5UL)      /*!< IICACKT (Bit 5)                                       */
#define R_SCI0_SIMR2_IICACKT_Msk       (0x20UL)   /*!< IICACKT (Bitfield-Mask: 0x01)                         */
#define R_SCI0_SIMR2_IICCSC_Pos        (1UL)      /*!< IICCSC (Bit 1)                                        */
#define R_SCI0_SIMR2_IICCSC_Msk        (0x2UL)    /*!< IICCSC (Bitfield-Mask: 0x01)                          */
#define R_SCI0_SIMR2_IICINTM_Pos       (0UL)      /*!< IICINTM (Bit 0)                                       */
#define R_SCI0_SIMR2_IICINTM_Msk       (0x1UL)    /*!< IICINTM (Bitfield-Mask: 0x01)                         */
/* =========================================================  SIMR3  ========================================================= */
#define R_SCI0_SIMR3_IICSCLS_Pos       (6UL)      /*!< IICSCLS (Bit 6)                                       */
#define R_SCI0_SIMR3_IICSCLS_Msk       (0xc0UL)   /*!< IICSCLS (Bitfield-Mask: 0x03)                         */
#define R_SCI0_SIMR3_IICSDAS_Pos       (4UL)      /*!< IICSDAS (Bit 4)                                       */
#define R_SCI0_SIMR3_IICSDAS_Msk       (0x30UL)   /*!< IICSDAS (Bitfield-Mask: 0x03)                         */
#define R_SCI0_SIMR3_IICSTIF_Pos       (3UL)      /*!< IICSTIF (Bit 3)                                       */
#define R_SCI0_SIMR3_IICSTIF_Msk       (0x8UL)    /*!< IICSTIF (Bitfield-Mask: 0x01)                         */
#define R_SCI0_SIMR3_IICSTPREQ_Pos     (2UL)      /*!< IICSTPREQ (Bit 2)                                     */
#define R_SCI0_SIMR3_IICSTPREQ_Msk     (0x4UL)    /*!< IICSTPREQ (Bitfield-Mask: 0x01)                       */
#define R_SCI0_SIMR3_IICRSTAREQ_Pos    (1UL)      /*!< IICRSTAREQ (Bit 1)                                    */
#define R_SCI0_SIMR3_IICRSTAREQ_Msk    (0x2UL)    /*!< IICRSTAREQ (Bitfield-Mask: 0x01)                      */
#define R_SCI0_SIMR3_IICSTAREQ_Pos     (0UL)      /*!< IICSTAREQ (Bit 0)                                     */
#define R_SCI0_SIMR3_IICSTAREQ_Msk     (0x1UL)    /*!< IICSTAREQ (Bitfield-Mask: 0x01)                       */
/* =========================================================  SISR  ========================================================== */
#define R_SCI0_SISR_IICACKR_Pos        (0UL)      /*!< IICACKR (Bit 0)                                       */
#define R_SCI0_SISR_IICACKR_Msk        (0x1UL)    /*!< IICACKR (Bitfield-Mask: 0x01)                         */
/* =========================================================  SPMR  ========================================================== */
#define R_SCI0_SPMR_CKPH_Pos           (7UL)      /*!< CKPH (Bit 7)                                          */
#define R_SCI0_SPMR_CKPH_Msk           (0x80UL)   /*!< CKPH (Bitfield-Mask: 0x01)                            */
#define R_SCI0_SPMR_CKPOL_Pos          (6UL)      /*!< CKPOL (Bit 6)                                         */
#define R_SCI0_SPMR_CKPOL_Msk          (0x40UL)   /*!< CKPOL (Bitfield-Mask: 0x01)                           */
#define R_SCI0_SPMR_MFF_Pos            (4UL)      /*!< MFF (Bit 4)                                           */
#define R_SCI0_SPMR_MFF_Msk            (0x10UL)   /*!< MFF (Bitfield-Mask: 0x01)                             */
#define R_SCI0_SPMR_CSTPEN_Pos         (3UL)      /*!< CSTPEN (Bit 3)                                        */
#define R_SCI0_SPMR_CSTPEN_Msk         (0x8UL)    /*!< CSTPEN (Bitfield-Mask: 0x01)                          */
#define R_SCI0_SPMR_MSS_Pos            (2UL)      /*!< MSS (Bit 2)                                           */
#define R_SCI0_SPMR_MSS_Msk            (0x4UL)    /*!< MSS (Bitfield-Mask: 0x01)                             */
#define R_SCI0_SPMR_CTSE_Pos           (1UL)      /*!< CTSE (Bit 1)                                          */
#define R_SCI0_SPMR_CTSE_Msk           (0x2UL)    /*!< CTSE (Bitfield-Mask: 0x01)                            */
#define R_SCI0_SPMR_SSE_Pos            (0UL)      /*!< SSE (Bit 0)                                           */
#define R_SCI0_SPMR_SSE_Msk            (0x1UL)    /*!< SSE (Bitfield-Mask: 0x01)                             */
/* ==========================================================  FCR  ========================================================== */
#define R_SCI0_FCR_RSTRG_Pos           (12UL)     /*!< RSTRG (Bit 12)                                        */
#define R_SCI0_FCR_RSTRG_Msk           (0xf000UL) /*!< RSTRG (Bitfield-Mask: 0x0f)                           */
#define R_SCI0_FCR_RTRG_Pos            (8UL)      /*!< RTRG (Bit 8)                                          */
#define R_SCI0_FCR_RTRG_Msk            (0xf00UL)  /*!< RTRG (Bitfield-Mask: 0x0f)                            */
#define R_SCI0_FCR_TTRG_Pos            (4UL)      /*!< TTRG (Bit 4)                                          */
#define R_SCI0_FCR_TTRG_Msk            (0xf0UL)   /*!< TTRG (Bitfield-Mask: 0x0f)                            */
#define R_SCI0_FCR_DRES_Pos            (3UL)      /*!< DRES (Bit 3)                                          */
#define R_SCI0_FCR_DRES_Msk            (0x8UL)    /*!< DRES (Bitfield-Mask: 0x01)                            */
#define R_SCI0_FCR_TFRST_Pos           (2UL)      /*!< TFRST (Bit 2)                                         */
#define R_SCI0_FCR_TFRST_Msk           (0x4UL)    /*!< TFRST (Bitfield-Mask: 0x01)                           */
#define R_SCI0_FCR_RFRST_Pos           (1UL)      /*!< RFRST (Bit 1)                                         */
#define R_SCI0_FCR_RFRST_Msk           (0x2UL)    /*!< RFRST (Bitfield-Mask: 0x01)                           */
#define R_SCI0_FCR_FM_Pos              (0UL)      /*!< FM (Bit 0)                                            */
#define R_SCI0_FCR_FM_Msk              (0x1UL)    /*!< FM (Bitfield-Mask: 0x01)                              */
/* ==========================================================  FDR  ========================================================== */
#define R_SCI0_FDR_T_Pos               (8UL)      /*!< T (Bit 8)                                             */
#define R_SCI0_FDR_T_Msk               (0x1f00UL) /*!< T (Bitfield-Mask: 0x1f)                               */
#define R_SCI0_FDR_R_Pos               (0UL)      /*!< R (Bit 0)                                             */
#define R_SCI0_FDR_R_Msk               (0x1fUL)   /*!< R (Bitfield-Mask: 0x1f)                               */
/* ==========================================================  LSR  ========================================================== */
#define R_SCI0_LSR_PNUM_Pos            (8UL)      /*!< PNUM (Bit 8)                                          */
#define R_SCI0_LSR_PNUM_Msk            (0x1f00UL) /*!< PNUM (Bitfield-Mask: 0x1f)                            */
#define R_SCI0_LSR_FNUM_Pos            (2UL)      /*!< FNUM (Bit 2)                                          */
#define R_SCI0_LSR_FNUM_Msk            (0x7cUL)   /*!< FNUM (Bitfield-Mask: 0x1f)                            */
#define R_SCI0_LSR_ORER_Pos            (0UL)      /*!< ORER (Bit 0)                                          */
#define R_SCI0_LSR_ORER_Msk            (0x1UL)    /*!< ORER (Bitfield-Mask: 0x01)                            */
/* ==========================================================  CDR  ========================================================== */
#define R_SCI0_CDR_CMPD_Pos            (0UL)      /*!< CMPD (Bit 0)                                          */
#define R_SCI0_CDR_CMPD_Msk            (0x1ffUL)  /*!< CMPD (Bitfield-Mask: 0x1ff)                           */
/* =========================================================  DCCR  ========================================================== */
#define R_SCI0_DCCR_DCME_Pos           (7UL)      /*!< DCME (Bit 7)                                          */
#define R_SCI0_DCCR_DCME_Msk           (0x80UL)   /*!< DCME (Bitfield-Mask: 0x01)                            */
#define R_SCI0_DCCR_IDSEL_Pos          (6UL)      /*!< IDSEL (Bit 6)                                         */
#define R_SCI0_DCCR_IDSEL_Msk          (0x40UL)   /*!< IDSEL (Bitfield-Mask: 0x01)                           */
#define R_SCI0_DCCR_DFER_Pos           (4UL)      /*!< DFER (Bit 4)                                          */
#define R_SCI0_DCCR_DFER_Msk           (0x10UL)   /*!< DFER (Bitfield-Mask: 0x01)                            */
#define R_SCI0_DCCR_DPER_Pos           (3UL)      /*!< DPER (Bit 3)                                          */
#define R_SCI0_DCCR_DPER_Msk           (0x8UL)    /*!< DPER (Bitfield-Mask: 0x01)                            */
#define R_SCI0_DCCR_DCMF_Pos           (0UL)      /*!< DCMF (Bit 0)                                          */
#define R_SCI0_DCCR_DCMF_Msk           (0x1UL)    /*!< DCMF (Bitfield-Mask: 0x01)                            */
/* =========================================================  SPTR  ========================================================== */
#define R_SCI0_SPTR_SPB2IO_Pos         (2UL)      /*!< SPB2IO (Bit 2)                                        */
#define R_SCI0_SPTR_SPB2IO_Msk         (0x4UL)    /*!< SPB2IO (Bitfield-Mask: 0x01)                          */
#define R_SCI0_SPTR_SPB2DT_Pos         (1UL)      /*!< SPB2DT (Bit 1)                                        */
#define R_SCI0_SPTR_SPB2DT_Msk         (0x2UL)    /*!< SPB2DT (Bitfield-Mask: 0x01)                          */
#define R_SCI0_SPTR_RXDMON_Pos         (0UL)      /*!< RXDMON (Bit 0)                                        */
#define R_SCI0_SPTR_RXDMON_Msk         (0x1UL)    /*!< RXDMON (Bitfield-Mask: 0x01)                          */

#endif /* R_SCI_POSMSK_H */
