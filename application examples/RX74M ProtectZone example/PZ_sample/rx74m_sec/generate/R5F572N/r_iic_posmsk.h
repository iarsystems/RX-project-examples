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

#ifndef R_IIC_POSMSK_H
#define R_IIC_POSMSK_H

/* =========================================================================================================================== */
/* ================                                 Pos/Mask Cluster Section                                  ================ */
/* =========================================================================================================================== */

/** @addtogroup PosMask_clusters
 * @{
 */

/* =========================================================================================================================== */
/* ================                                            SAR                                            ================ */
/* =========================================================================================================================== */

/* ===========================================================  L  =========================================================== */
#define R_IIC0_SAR_L_SVA0_Pos    (0UL)    /*!< SVA0 (Bit 0)                                          */
#define R_IIC0_SAR_L_SVA0_Msk    (0x1UL)  /*!< SVA0 (Bitfield-Mask: 0x01)                            */
#define R_IIC0_SAR_L_SVA_Pos     (1UL)    /*!< SVA (Bit 1)                                           */
#define R_IIC0_SAR_L_SVA_Msk     (0xfeUL) /*!< SVA (Bitfield-Mask: 0x7f)                             */
/* ===========================================================  U  =========================================================== */
#define R_IIC0_SAR_U_FS_Pos      (0UL)    /*!< FS (Bit 0)                                            */
#define R_IIC0_SAR_U_FS_Msk      (0x1UL)  /*!< FS (Bitfield-Mask: 0x01)                              */
#define R_IIC0_SAR_U_SVA_Pos     (1UL)    /*!< SVA (Bit 1)                                           */
#define R_IIC0_SAR_U_SVA_Msk     (0x6UL)  /*!< SVA (Bitfield-Mask: 0x03)                             */

/** @} */ /* End of group PosMask_clusters */

/* =========================================================================================================================== */
/* ================                                Pos/Mask Peripheral Section                                ================ */
/* =========================================================================================================================== */

/** @addtogroup PosMask_peripherals
 * @{
 */

/* =========================================================================================================================== */
/* ================                                          R_IIC0                                           ================ */
/* =========================================================================================================================== */

/* =========================================================  ICCR1  ========================================================= */
#define R_IIC0_ICCR1_ICE_Pos       (7UL)    /*!< ICE (Bit 7)                                           */
#define R_IIC0_ICCR1_ICE_Msk       (0x80UL) /*!< ICE (Bitfield-Mask: 0x01)                             */
#define R_IIC0_ICCR1_IICRST_Pos    (6UL)    /*!< IICRST (Bit 6)                                        */
#define R_IIC0_ICCR1_IICRST_Msk    (0x40UL) /*!< IICRST (Bitfield-Mask: 0x01)                          */
#define R_IIC0_ICCR1_CLO_Pos       (5UL)    /*!< CLO (Bit 5)                                           */
#define R_IIC0_ICCR1_CLO_Msk       (0x20UL) /*!< CLO (Bitfield-Mask: 0x01)                             */
#define R_IIC0_ICCR1_SOWP_Pos      (4UL)    /*!< SOWP (Bit 4)                                          */
#define R_IIC0_ICCR1_SOWP_Msk      (0x10UL) /*!< SOWP (Bitfield-Mask: 0x01)                            */
#define R_IIC0_ICCR1_SCLO_Pos      (3UL)    /*!< SCLO (Bit 3)                                          */
#define R_IIC0_ICCR1_SCLO_Msk      (0x8UL)  /*!< SCLO (Bitfield-Mask: 0x01)                            */
#define R_IIC0_ICCR1_SDAO_Pos      (2UL)    /*!< SDAO (Bit 2)                                          */
#define R_IIC0_ICCR1_SDAO_Msk      (0x4UL)  /*!< SDAO (Bitfield-Mask: 0x01)                            */
#define R_IIC0_ICCR1_SCLI_Pos      (1UL)    /*!< SCLI (Bit 1)                                          */
#define R_IIC0_ICCR1_SCLI_Msk      (0x2UL)  /*!< SCLI (Bitfield-Mask: 0x01)                            */
#define R_IIC0_ICCR1_SDAI_Pos      (0UL)    /*!< SDAI (Bit 0)                                          */
#define R_IIC0_ICCR1_SDAI_Msk      (0x1UL)  /*!< SDAI (Bitfield-Mask: 0x01)                            */
/* =========================================================  ICCR2  ========================================================= */
#define R_IIC0_ICCR2_BBSY_Pos      (7UL)    /*!< BBSY (Bit 7)                                          */
#define R_IIC0_ICCR2_BBSY_Msk      (0x80UL) /*!< BBSY (Bitfield-Mask: 0x01)                            */
#define R_IIC0_ICCR2_MST_Pos       (6UL)    /*!< MST (Bit 6)                                           */
#define R_IIC0_ICCR2_MST_Msk       (0x40UL) /*!< MST (Bitfield-Mask: 0x01)                             */
#define R_IIC0_ICCR2_TRS_Pos       (5UL)    /*!< TRS (Bit 5)                                           */
#define R_IIC0_ICCR2_TRS_Msk       (0x20UL) /*!< TRS (Bitfield-Mask: 0x01)                             */
#define R_IIC0_ICCR2_SP_Pos        (3UL)    /*!< SP (Bit 3)                                            */
#define R_IIC0_ICCR2_SP_Msk        (0x8UL)  /*!< SP (Bitfield-Mask: 0x01)                              */
#define R_IIC0_ICCR2_RS_Pos        (2UL)    /*!< RS (Bit 2)                                            */
#define R_IIC0_ICCR2_RS_Msk        (0x4UL)  /*!< RS (Bitfield-Mask: 0x01)                              */
#define R_IIC0_ICCR2_ST_Pos        (1UL)    /*!< ST (Bit 1)                                            */
#define R_IIC0_ICCR2_ST_Msk        (0x2UL)  /*!< ST (Bitfield-Mask: 0x01)                              */
/* =========================================================  ICMR1  ========================================================= */
#define R_IIC0_ICMR1_MTWP_Pos      (7UL)    /*!< MTWP (Bit 7)                                          */
#define R_IIC0_ICMR1_MTWP_Msk      (0x80UL) /*!< MTWP (Bitfield-Mask: 0x01)                            */
#define R_IIC0_ICMR1_CKS_Pos       (4UL)    /*!< CKS (Bit 4)                                           */
#define R_IIC0_ICMR1_CKS_Msk       (0x70UL) /*!< CKS (Bitfield-Mask: 0x07)                             */
#define R_IIC0_ICMR1_BCWP_Pos      (3UL)    /*!< BCWP (Bit 3)                                          */
#define R_IIC0_ICMR1_BCWP_Msk      (0x8UL)  /*!< BCWP (Bitfield-Mask: 0x01)                            */
#define R_IIC0_ICMR1_BC_Pos        (0UL)    /*!< BC (Bit 0)                                            */
#define R_IIC0_ICMR1_BC_Msk        (0x7UL)  /*!< BC (Bitfield-Mask: 0x07)                              */
/* =========================================================  ICMR2  ========================================================= */
#define R_IIC0_ICMR2_DLCS_Pos      (7UL)    /*!< DLCS (Bit 7)                                          */
#define R_IIC0_ICMR2_DLCS_Msk      (0x80UL) /*!< DLCS (Bitfield-Mask: 0x01)                            */
#define R_IIC0_ICMR2_SDDL_Pos      (4UL)    /*!< SDDL (Bit 4)                                          */
#define R_IIC0_ICMR2_SDDL_Msk      (0x70UL) /*!< SDDL (Bitfield-Mask: 0x07)                            */
#define R_IIC0_ICMR2_TMOH_Pos      (2UL)    /*!< TMOH (Bit 2)                                          */
#define R_IIC0_ICMR2_TMOH_Msk      (0x4UL)  /*!< TMOH (Bitfield-Mask: 0x01)                            */
#define R_IIC0_ICMR2_TMOL_Pos      (1UL)    /*!< TMOL (Bit 1)                                          */
#define R_IIC0_ICMR2_TMOL_Msk      (0x2UL)  /*!< TMOL (Bitfield-Mask: 0x01)                            */
#define R_IIC0_ICMR2_TMOS_Pos      (0UL)    /*!< TMOS (Bit 0)                                          */
#define R_IIC0_ICMR2_TMOS_Msk      (0x1UL)  /*!< TMOS (Bitfield-Mask: 0x01)                            */
/* =========================================================  ICMR3  ========================================================= */
#define R_IIC0_ICMR3_SMBS_Pos      (7UL)    /*!< SMBS (Bit 7)                                          */
#define R_IIC0_ICMR3_SMBS_Msk      (0x80UL) /*!< SMBS (Bitfield-Mask: 0x01)                            */
#define R_IIC0_ICMR3_WAIT_Pos      (6UL)    /*!< WAIT (Bit 6)                                          */
#define R_IIC0_ICMR3_WAIT_Msk      (0x40UL) /*!< WAIT (Bitfield-Mask: 0x01)                            */
#define R_IIC0_ICMR3_RDRFS_Pos     (5UL)    /*!< RDRFS (Bit 5)                                         */
#define R_IIC0_ICMR3_RDRFS_Msk     (0x20UL) /*!< RDRFS (Bitfield-Mask: 0x01)                           */
#define R_IIC0_ICMR3_ACKWP_Pos     (4UL)    /*!< ACKWP (Bit 4)                                         */
#define R_IIC0_ICMR3_ACKWP_Msk     (0x10UL) /*!< ACKWP (Bitfield-Mask: 0x01)                           */
#define R_IIC0_ICMR3_ACKBT_Pos     (3UL)    /*!< ACKBT (Bit 3)                                         */
#define R_IIC0_ICMR3_ACKBT_Msk     (0x8UL)  /*!< ACKBT (Bitfield-Mask: 0x01)                           */
#define R_IIC0_ICMR3_ACKBR_Pos     (2UL)    /*!< ACKBR (Bit 2)                                         */
#define R_IIC0_ICMR3_ACKBR_Msk     (0x4UL)  /*!< ACKBR (Bitfield-Mask: 0x01)                           */
#define R_IIC0_ICMR3_NF_Pos        (0UL)    /*!< NF (Bit 0)                                            */
#define R_IIC0_ICMR3_NF_Msk        (0x3UL)  /*!< NF (Bitfield-Mask: 0x03)                              */
/* =========================================================  ICFER  ========================================================= */
#define R_IIC0_ICFER_FMPE_Pos      (7UL)    /*!< FMPE (Bit 7)                                          */
#define R_IIC0_ICFER_FMPE_Msk      (0x80UL) /*!< FMPE (Bitfield-Mask: 0x01)                            */
#define R_IIC0_ICFER_SCLE_Pos      (6UL)    /*!< SCLE (Bit 6)                                          */
#define R_IIC0_ICFER_SCLE_Msk      (0x40UL) /*!< SCLE (Bitfield-Mask: 0x01)                            */
#define R_IIC0_ICFER_NFE_Pos       (5UL)    /*!< NFE (Bit 5)                                           */
#define R_IIC0_ICFER_NFE_Msk       (0x20UL) /*!< NFE (Bitfield-Mask: 0x01)                             */
#define R_IIC0_ICFER_NACKE_Pos     (4UL)    /*!< NACKE (Bit 4)                                         */
#define R_IIC0_ICFER_NACKE_Msk     (0x10UL) /*!< NACKE (Bitfield-Mask: 0x01)                           */
#define R_IIC0_ICFER_SALE_Pos      (3UL)    /*!< SALE (Bit 3)                                          */
#define R_IIC0_ICFER_SALE_Msk      (0x8UL)  /*!< SALE (Bitfield-Mask: 0x01)                            */
#define R_IIC0_ICFER_NALE_Pos      (2UL)    /*!< NALE (Bit 2)                                          */
#define R_IIC0_ICFER_NALE_Msk      (0x4UL)  /*!< NALE (Bitfield-Mask: 0x01)                            */
#define R_IIC0_ICFER_MALE_Pos      (1UL)    /*!< MALE (Bit 1)                                          */
#define R_IIC0_ICFER_MALE_Msk      (0x2UL)  /*!< MALE (Bitfield-Mask: 0x01)                            */
#define R_IIC0_ICFER_TMOE_Pos      (0UL)    /*!< TMOE (Bit 0)                                          */
#define R_IIC0_ICFER_TMOE_Msk      (0x1UL)  /*!< TMOE (Bitfield-Mask: 0x01)                            */
/* =========================================================  ICSER  ========================================================= */
#define R_IIC0_ICSER_HOAE_Pos      (7UL)    /*!< HOAE (Bit 7)                                          */
#define R_IIC0_ICSER_HOAE_Msk      (0x80UL) /*!< HOAE (Bitfield-Mask: 0x01)                            */
#define R_IIC0_ICSER_DIDE_Pos      (5UL)    /*!< DIDE (Bit 5)                                          */
#define R_IIC0_ICSER_DIDE_Msk      (0x20UL) /*!< DIDE (Bitfield-Mask: 0x01)                            */
#define R_IIC0_ICSER_GCAE_Pos      (3UL)    /*!< GCAE (Bit 3)                                          */
#define R_IIC0_ICSER_GCAE_Msk      (0x8UL)  /*!< GCAE (Bitfield-Mask: 0x01)                            */
#define R_IIC0_ICSER_SAR2E_Pos     (2UL)    /*!< SAR2E (Bit 2)                                         */
#define R_IIC0_ICSER_SAR2E_Msk     (0x4UL)  /*!< SAR2E (Bitfield-Mask: 0x01)                           */
#define R_IIC0_ICSER_SAR1E_Pos     (1UL)    /*!< SAR1E (Bit 1)                                         */
#define R_IIC0_ICSER_SAR1E_Msk     (0x2UL)  /*!< SAR1E (Bitfield-Mask: 0x01)                           */
#define R_IIC0_ICSER_SAR0E_Pos     (0UL)    /*!< SAR0E (Bit 0)                                         */
#define R_IIC0_ICSER_SAR0E_Msk     (0x1UL)  /*!< SAR0E (Bitfield-Mask: 0x01)                           */
/* =========================================================  ICIER  ========================================================= */
#define R_IIC0_ICIER_TIE_Pos       (7UL)    /*!< TIE (Bit 7)                                           */
#define R_IIC0_ICIER_TIE_Msk       (0x80UL) /*!< TIE (Bitfield-Mask: 0x01)                             */
#define R_IIC0_ICIER_TEIE_Pos      (6UL)    /*!< TEIE (Bit 6)                                          */
#define R_IIC0_ICIER_TEIE_Msk      (0x40UL) /*!< TEIE (Bitfield-Mask: 0x01)                            */
#define R_IIC0_ICIER_RIE_Pos       (5UL)    /*!< RIE (Bit 5)                                           */
#define R_IIC0_ICIER_RIE_Msk       (0x20UL) /*!< RIE (Bitfield-Mask: 0x01)                             */
#define R_IIC0_ICIER_NAKIE_Pos     (4UL)    /*!< NAKIE (Bit 4)                                         */
#define R_IIC0_ICIER_NAKIE_Msk     (0x10UL) /*!< NAKIE (Bitfield-Mask: 0x01)                           */
#define R_IIC0_ICIER_SPIE_Pos      (3UL)    /*!< SPIE (Bit 3)                                          */
#define R_IIC0_ICIER_SPIE_Msk      (0x8UL)  /*!< SPIE (Bitfield-Mask: 0x01)                            */
#define R_IIC0_ICIER_STIE_Pos      (2UL)    /*!< STIE (Bit 2)                                          */
#define R_IIC0_ICIER_STIE_Msk      (0x4UL)  /*!< STIE (Bitfield-Mask: 0x01)                            */
#define R_IIC0_ICIER_ALIE_Pos      (1UL)    /*!< ALIE (Bit 1)                                          */
#define R_IIC0_ICIER_ALIE_Msk      (0x2UL)  /*!< ALIE (Bitfield-Mask: 0x01)                            */
#define R_IIC0_ICIER_TMOIE_Pos     (0UL)    /*!< TMOIE (Bit 0)                                         */
#define R_IIC0_ICIER_TMOIE_Msk     (0x1UL)  /*!< TMOIE (Bitfield-Mask: 0x01)                           */
/* =========================================================  ICSR1  ========================================================= */
#define R_IIC0_ICSR1_HOA_Pos       (7UL)    /*!< HOA (Bit 7)                                           */
#define R_IIC0_ICSR1_HOA_Msk       (0x80UL) /*!< HOA (Bitfield-Mask: 0x01)                             */
#define R_IIC0_ICSR1_DID_Pos       (5UL)    /*!< DID (Bit 5)                                           */
#define R_IIC0_ICSR1_DID_Msk       (0x20UL) /*!< DID (Bitfield-Mask: 0x01)                             */
#define R_IIC0_ICSR1_GCA_Pos       (3UL)    /*!< GCA (Bit 3)                                           */
#define R_IIC0_ICSR1_GCA_Msk       (0x8UL)  /*!< GCA (Bitfield-Mask: 0x01)                             */
#define R_IIC0_ICSR1_AAS2_Pos      (2UL)    /*!< AAS2 (Bit 2)                                          */
#define R_IIC0_ICSR1_AAS2_Msk      (0x4UL)  /*!< AAS2 (Bitfield-Mask: 0x01)                            */
#define R_IIC0_ICSR1_AAS1_Pos      (1UL)    /*!< AAS1 (Bit 1)                                          */
#define R_IIC0_ICSR1_AAS1_Msk      (0x2UL)  /*!< AAS1 (Bitfield-Mask: 0x01)                            */
#define R_IIC0_ICSR1_AAS0_Pos      (0UL)    /*!< AAS0 (Bit 0)                                          */
#define R_IIC0_ICSR1_AAS0_Msk      (0x1UL)  /*!< AAS0 (Bitfield-Mask: 0x01)                            */
/* =========================================================  ICSR2  ========================================================= */
#define R_IIC0_ICSR2_TDRE_Pos      (7UL)    /*!< TDRE (Bit 7)                                          */
#define R_IIC0_ICSR2_TDRE_Msk      (0x80UL) /*!< TDRE (Bitfield-Mask: 0x01)                            */
#define R_IIC0_ICSR2_TEND_Pos      (6UL)    /*!< TEND (Bit 6)                                          */
#define R_IIC0_ICSR2_TEND_Msk      (0x40UL) /*!< TEND (Bitfield-Mask: 0x01)                            */
#define R_IIC0_ICSR2_RDRF_Pos      (5UL)    /*!< RDRF (Bit 5)                                          */
#define R_IIC0_ICSR2_RDRF_Msk      (0x20UL) /*!< RDRF (Bitfield-Mask: 0x01)                            */
#define R_IIC0_ICSR2_NACKF_Pos     (4UL)    /*!< NACKF (Bit 4)                                         */
#define R_IIC0_ICSR2_NACKF_Msk     (0x10UL) /*!< NACKF (Bitfield-Mask: 0x01)                           */
#define R_IIC0_ICSR2_STOP_Pos      (3UL)    /*!< STOP (Bit 3)                                          */
#define R_IIC0_ICSR2_STOP_Msk      (0x8UL)  /*!< STOP (Bitfield-Mask: 0x01)                            */
#define R_IIC0_ICSR2_START_Pos     (2UL)    /*!< START (Bit 2)                                         */
#define R_IIC0_ICSR2_START_Msk     (0x4UL)  /*!< START (Bitfield-Mask: 0x01)                           */
#define R_IIC0_ICSR2_AL_Pos        (1UL)    /*!< AL (Bit 1)                                            */
#define R_IIC0_ICSR2_AL_Msk        (0x2UL)  /*!< AL (Bitfield-Mask: 0x01)                              */
#define R_IIC0_ICSR2_TMOF_Pos      (0UL)    /*!< TMOF (Bit 0)                                          */
#define R_IIC0_ICSR2_TMOF_Msk      (0x1UL)  /*!< TMOF (Bitfield-Mask: 0x01)                            */
/* =========================================================  ICBRL  ========================================================= */
#define R_IIC0_ICBRL_BRL_Pos       (0UL)    /*!< BRL (Bit 0)                                           */
#define R_IIC0_ICBRL_BRL_Msk       (0x1fUL) /*!< BRL (Bitfield-Mask: 0x1f)                             */
/* =========================================================  ICBRH  ========================================================= */
#define R_IIC0_ICBRH_BRH_Pos       (0UL)    /*!< BRH (Bit 0)                                           */
#define R_IIC0_ICBRH_BRH_Msk       (0x1fUL) /*!< BRH (Bitfield-Mask: 0x1f)                             */
/* =========================================================  ICDRT  ========================================================= */
#define R_IIC0_ICDRT_ICDRT_Pos     (0UL)    /*!< ICDRT (Bit 0)                                         */
#define R_IIC0_ICDRT_ICDRT_Msk     (0xffUL) /*!< ICDRT (Bitfield-Mask: 0xff)                           */
/* =========================================================  ICDRR  ========================================================= */
#define R_IIC0_ICDRR_ICDRR_Pos     (0UL)    /*!< ICDRR (Bit 0)                                         */
#define R_IIC0_ICDRR_ICDRR_Msk     (0xffUL) /*!< ICDRR (Bitfield-Mask: 0xff)                           */

/** @} */ /* End of group PosMask_peripherals */

#endif /* R_IIC_POSMSK_H */
