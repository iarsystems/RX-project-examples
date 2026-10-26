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

#ifndef R_GLCDC_POSMSK_H
#define R_GLCDC_POSMSK_H

/* =========================================================================================================================== */
/* ================                                            BG                                             ================ */
/* =========================================================================================================================== */

/* ==========================================================  EN  =========================================================== */
#define R_GLCDC_BG_EN_SWRST_Pos     (16UL)        /*!< SWRST (Bit 16)                                        */
#define R_GLCDC_BG_EN_SWRST_Msk     (0x10000UL)   /*!< SWRST (Bitfield-Mask: 0x01)                           */
#define R_GLCDC_BG_EN_VEN_Pos       (8UL)         /*!< VEN (Bit 8)                                           */
#define R_GLCDC_BG_EN_VEN_Msk       (0x100UL)     /*!< VEN (Bitfield-Mask: 0x01)                             */
#define R_GLCDC_BG_EN_EN_Pos        (0UL)         /*!< EN (Bit 0)                                            */
#define R_GLCDC_BG_EN_EN_Msk        (0x1UL)       /*!< EN (Bitfield-Mask: 0x01)                              */
/* =========================================================  PERI  ========================================================== */
#define R_GLCDC_BG_PERI_FV_Pos      (16UL)        /*!< FV (Bit 16)                                           */
#define R_GLCDC_BG_PERI_FV_Msk      (0x7ff0000UL) /*!< FV (Bitfield-Mask: 0x7ff)                             */
#define R_GLCDC_BG_PERI_FH_Pos      (0UL)         /*!< FH (Bit 0)                                            */
#define R_GLCDC_BG_PERI_FH_Msk      (0x7ffUL)     /*!< FH (Bitfield-Mask: 0x7ff)                             */
/* =========================================================  SYNC  ========================================================== */
#define R_GLCDC_BG_SYNC_VP_Pos      (16UL)        /*!< VP (Bit 16)                                           */
#define R_GLCDC_BG_SYNC_VP_Msk      (0xf0000UL)   /*!< VP (Bitfield-Mask: 0x0f)                              */
#define R_GLCDC_BG_SYNC_HP_Pos      (0UL)         /*!< HP (Bit 0)                                            */
#define R_GLCDC_BG_SYNC_HP_Msk      (0xfUL)       /*!< HP (Bitfield-Mask: 0x0f)                              */
/* =========================================================  VSIZE  ========================================================= */
#define R_GLCDC_BG_VSIZE_VP_Pos     (16UL)        /*!< VP (Bit 16)                                           */
#define R_GLCDC_BG_VSIZE_VP_Msk     (0x7ff0000UL) /*!< VP (Bitfield-Mask: 0x7ff)                             */
#define R_GLCDC_BG_VSIZE_VW_Pos     (0UL)         /*!< VW (Bit 0)                                            */
#define R_GLCDC_BG_VSIZE_VW_Msk     (0x7ffUL)     /*!< VW (Bitfield-Mask: 0x7ff)                             */
/* =========================================================  HSIZE  ========================================================= */
#define R_GLCDC_BG_HSIZE_HP_Pos     (16UL)        /*!< HP (Bit 16)                                           */
#define R_GLCDC_BG_HSIZE_HP_Msk     (0x7ff0000UL) /*!< HP (Bitfield-Mask: 0x7ff)                             */
#define R_GLCDC_BG_HSIZE_HW_Pos     (0UL)         /*!< HW (Bit 0)                                            */
#define R_GLCDC_BG_HSIZE_HW_Msk     (0x7ffUL)     /*!< HW (Bitfield-Mask: 0x7ff)                             */
/* ==========================================================  BGC  ========================================================== */
#define R_GLCDC_BG_BGC_R_Pos        (16UL)        /*!< R (Bit 16)                                            */
#define R_GLCDC_BG_BGC_R_Msk        (0xff0000UL)  /*!< R (Bitfield-Mask: 0xff)                               */
#define R_GLCDC_BG_BGC_G_Pos        (8UL)         /*!< G (Bit 8)                                             */
#define R_GLCDC_BG_BGC_G_Msk        (0xff00UL)    /*!< G (Bitfield-Mask: 0xff)                               */
#define R_GLCDC_BG_BGC_B_Pos        (0UL)         /*!< B (Bit 0)                                             */
#define R_GLCDC_BG_BGC_B_Msk        (0xffUL)      /*!< B (Bitfield-Mask: 0xff)                               */
/* ==========================================================  MON  ========================================================== */
#define R_GLCDC_BG_MON_SWRST_Pos    (16UL)        /*!< SWRST (Bit 16)                                        */
#define R_GLCDC_BG_MON_SWRST_Msk    (0x10000UL)   /*!< SWRST (Bitfield-Mask: 0x01)                           */
#define R_GLCDC_BG_MON_VEN_Pos      (8UL)         /*!< VEN (Bit 8)                                           */
#define R_GLCDC_BG_MON_VEN_Msk      (0x100UL)     /*!< VEN (Bitfield-Mask: 0x01)                             */
#define R_GLCDC_BG_MON_EN_Pos       (0UL)         /*!< EN (Bit 0)                                            */
#define R_GLCDC_BG_MON_EN_Msk       (0x1UL)       /*!< EN (Bitfield-Mask: 0x01)                              */

/* =========================================================================================================================== */
/* ================                                            GR                                             ================ */
/* =========================================================================================================================== */

/* ==========================================================  VEN  ========================================================== */
#define R_GLCDC_GR_VEN_PVEN_Pos         (0UL)          /*!< PVEN (Bit 0)                                          */
#define R_GLCDC_GR_VEN_PVEN_Msk         (0x1UL)        /*!< PVEN (Bitfield-Mask: 0x01)                            */
/* =========================================================  FLMRD  ========================================================= */
#define R_GLCDC_GR_FLMRD_RENB_Pos       (0UL)          /*!< RENB (Bit 0)                                          */
#define R_GLCDC_GR_FLMRD_RENB_Msk       (0x1UL)        /*!< RENB (Bitfield-Mask: 0x01)                            */
/* =========================================================  FLM1  ========================================================== */
#define R_GLCDC_GR_FLM1_BSTMD_Pos       (0UL)          /*!< BSTMD (Bit 0)                                         */
#define R_GLCDC_GR_FLM1_BSTMD_Msk       (0x3UL)        /*!< BSTMD (Bitfield-Mask: 0x03)                           */
/* =========================================================  FLM2  ========================================================== */
#define R_GLCDC_GR_FLM2_BASE_Pos        (0UL)          /*!< BASE (Bit 0)                                          */
#define R_GLCDC_GR_FLM2_BASE_Msk        (0xffffffffUL) /*!< BASE (Bitfield-Mask: 0xffffffff)                      */
/* =========================================================  FLM3  ========================================================== */
#define R_GLCDC_GR_FLM3_LNOFF_Pos       (16UL)         /*!< LNOFF (Bit 16)                                        */
#define R_GLCDC_GR_FLM3_LNOFF_Msk       (0xffff0000UL) /*!< LNOFF (Bitfield-Mask: 0xffff)                         */
/* =========================================================  FLM5  ========================================================== */
#define R_GLCDC_GR_FLM5_LNNUM_Pos       (16UL)         /*!< LNNUM (Bit 16)                                        */
#define R_GLCDC_GR_FLM5_LNNUM_Msk       (0x7ff0000UL)  /*!< LNNUM (Bitfield-Mask: 0x7ff)                          */
#define R_GLCDC_GR_FLM5_DATANUM_Pos     (0UL)          /*!< DATANUM (Bit 0)                                       */
#define R_GLCDC_GR_FLM5_DATANUM_Msk     (0xffffUL)     /*!< DATANUM (Bitfield-Mask: 0xffff)                       */
/* =========================================================  FLM6  ========================================================== */
#define R_GLCDC_GR_FLM6_FORMAT_Pos      (28UL)         /*!< FORMAT (Bit 28)                                       */
#define R_GLCDC_GR_FLM6_FORMAT_Msk      (0x70000000UL) /*!< FORMAT (Bitfield-Mask: 0x07)                          */
/* ==========================================================  AB1  ========================================================== */
#define R_GLCDC_GR_AB1_ARCON_Pos        (12UL)         /*!< ARCON (Bit 12)                                        */
#define R_GLCDC_GR_AB1_ARCON_Msk        (0x1000UL)     /*!< ARCON (Bitfield-Mask: 0x01)                           */
#define R_GLCDC_GR_AB1_ARCDISPON_Pos    (8UL)          /*!< ARCDISPON (Bit 8)                                     */
#define R_GLCDC_GR_AB1_ARCDISPON_Msk    (0x100UL)      /*!< ARCDISPON (Bitfield-Mask: 0x01)                       */
#define R_GLCDC_GR_AB1_GRCDISPON_Pos    (4UL)          /*!< GRCDISPON (Bit 4)                                     */
#define R_GLCDC_GR_AB1_GRCDISPON_Msk    (0x10UL)       /*!< GRCDISPON (Bitfield-Mask: 0x01)                       */
#define R_GLCDC_GR_AB1_DISPSEL_Pos      (0UL)          /*!< DISPSEL (Bit 0)                                       */
#define R_GLCDC_GR_AB1_DISPSEL_Msk      (0x3UL)        /*!< DISPSEL (Bitfield-Mask: 0x03)                         */
/* ==========================================================  AB2  ========================================================== */
#define R_GLCDC_GR_AB2_GRCVS_Pos        (16UL)         /*!< GRCVS (Bit 16)                                        */
#define R_GLCDC_GR_AB2_GRCVS_Msk        (0x7ff0000UL)  /*!< GRCVS (Bitfield-Mask: 0x7ff)                          */
#define R_GLCDC_GR_AB2_GRCVW_Pos        (0UL)          /*!< GRCVW (Bit 0)                                         */
#define R_GLCDC_GR_AB2_GRCVW_Msk        (0x7ffUL)      /*!< GRCVW (Bitfield-Mask: 0x7ff)                          */
/* ==========================================================  AB3  ========================================================== */
#define R_GLCDC_GR_AB3_GRCHS_Pos        (16UL)         /*!< GRCHS (Bit 16)                                        */
#define R_GLCDC_GR_AB3_GRCHS_Msk        (0x7ff0000UL)  /*!< GRCHS (Bitfield-Mask: 0x7ff)                          */
#define R_GLCDC_GR_AB3_GRCHW_Pos        (0UL)          /*!< GRCHW (Bit 0)                                         */
#define R_GLCDC_GR_AB3_GRCHW_Msk        (0x7ffUL)      /*!< GRCHW (Bitfield-Mask: 0x7fe) Set even value           */
/* ==========================================================  AB4  ========================================================== */
#define R_GLCDC_GR_AB4_ARCVS_Pos        (16UL)         /*!< ARCVS (Bit 16)                                        */
#define R_GLCDC_GR_AB4_ARCVS_Msk        (0x7ff0000UL)  /*!< ARCVS (Bitfield-Mask: 0x7ff)                          */
#define R_GLCDC_GR_AB4_ARCVW_Pos        (0UL)          /*!< ARCVW (Bit 0)                                         */
#define R_GLCDC_GR_AB4_ARCVW_Msk        (0x7ffUL)      /*!< ARCVW (Bitfield-Mask: 0x7ff)                          */
/* ==========================================================  AB5  ========================================================== */
#define R_GLCDC_GR_AB5_ARCHS_Pos        (16UL)         /*!< ARCHS (Bit 16)                                        */
#define R_GLCDC_GR_AB5_ARCHS_Msk        (0x7ff0000UL)  /*!< ARCHS (Bitfield-Mask: 0x7ff)                          */
#define R_GLCDC_GR_AB5_ARCHW_Pos        (0UL)          /*!< ARCHW (Bit 0)                                         */
#define R_GLCDC_GR_AB5_ARCHW_Msk        (0x7ffUL)      /*!< ARCHW (Bitfield-Mask: 0x7ff)                          */
/* ==========================================================  AB6  ========================================================== */
#define R_GLCDC_GR_AB6_ARCCOEF_Pos      (16UL)         /*!< ARCCOEF (Bit 16)                                      */
#define R_GLCDC_GR_AB6_ARCCOEF_Msk      (0x1ff0000UL)  /*!< ARCCOEF (Bitfield-Mask: 0x1ff)                        */
#define R_GLCDC_GR_AB6_ARCRATE_Pos      (0UL)          /*!< ARCRATE (Bit 0)                                       */
#define R_GLCDC_GR_AB6_ARCRATE_Msk      (0xffUL)       /*!< ARCRATE (Bitfield-Mask: 0xff)                         */
/* ==========================================================  AB7  ========================================================== */
#define R_GLCDC_GR_AB7_ARCDEF_Pos       (16UL)         /*!< ARCDEF (Bit 16)                                       */
#define R_GLCDC_GR_AB7_ARCDEF_Msk       (0xff0000UL)   /*!< ARCDEF (Bitfield-Mask: 0xff)                          */
#define R_GLCDC_GR_AB7_CKON_Pos         (0UL)          /*!< CKON (Bit 0)                                          */
#define R_GLCDC_GR_AB7_CKON_Msk         (0x1UL)        /*!< CKON (Bitfield-Mask: 0x01)                            */
/* ==========================================================  AB8  ========================================================== */
#define R_GLCDC_GR_AB8_CKKG_Pos         (16UL)         /*!< CKKG (Bit 16)                                         */
#define R_GLCDC_GR_AB8_CKKG_Msk         (0xff0000UL)   /*!< CKKG (Bitfield-Mask: 0xff)                            */
#define R_GLCDC_GR_AB8_CKKB_Pos         (8UL)          /*!< CKKB (Bit 8)                                          */
#define R_GLCDC_GR_AB8_CKKB_Msk         (0xff00UL)     /*!< CKKB (Bitfield-Mask: 0xff)                            */
#define R_GLCDC_GR_AB8_CKKR_Pos         (0UL)          /*!< CKKR (Bit 0)                                          */
#define R_GLCDC_GR_AB8_CKKR_Msk         (0xffUL)       /*!< CKKR (Bitfield-Mask: 0xff)                            */
/* ==========================================================  AB9  ========================================================== */
#define R_GLCDC_GR_AB9_CKA_Pos          (24UL)         /*!< CKA (Bit 24)                                          */
#define R_GLCDC_GR_AB9_CKA_Msk          (0xff000000UL) /*!< CKA (Bitfield-Mask: 0xff)                             */
#define R_GLCDC_GR_AB9_CKG_Pos          (16UL)         /*!< CKG (Bit 16)                                          */
#define R_GLCDC_GR_AB9_CKG_Msk          (0xff0000UL)   /*!< CKG (Bitfield-Mask: 0xff)                             */
#define R_GLCDC_GR_AB9_CKB_Pos          (8UL)          /*!< CKB (Bit 8)                                           */
#define R_GLCDC_GR_AB9_CKB_Msk          (0xff00UL)     /*!< CKB (Bitfield-Mask: 0xff)                             */
#define R_GLCDC_GR_AB9_CKR_Pos          (0UL)          /*!< CKR (Bit 0)                                           */
#define R_GLCDC_GR_AB9_CKR_Msk          (0xffUL)       /*!< CKR (Bitfield-Mask: 0xff)                             */
/* =========================================================  BASE  ========================================================== */
#define R_GLCDC_GR_BASE_G_Pos           (16UL)         /*!< G (Bit 16)                                            */
#define R_GLCDC_GR_BASE_G_Msk           (0xff0000UL)   /*!< G (Bitfield-Mask: 0xff)                               */
#define R_GLCDC_GR_BASE_B_Pos           (8UL)          /*!< B (Bit 8)                                             */
#define R_GLCDC_GR_BASE_B_Msk           (0xff00UL)     /*!< B (Bitfield-Mask: 0xff)                               */
#define R_GLCDC_GR_BASE_R_Pos           (0UL)          /*!< R (Bit 0)                                             */
#define R_GLCDC_GR_BASE_R_Msk           (0xffUL)       /*!< R (Bitfield-Mask: 0xff)                               */
/* ========================================================  CLUTINT  ======================================================== */
#define R_GLCDC_GR_CLUTINT_SEL_Pos      (16UL)         /*!< SEL (Bit 16)                                          */
#define R_GLCDC_GR_CLUTINT_SEL_Msk      (0x10000UL)    /*!< SEL (Bitfield-Mask: 0x01)                             */
#define R_GLCDC_GR_CLUTINT_LINE_Pos     (0UL)          /*!< LINE (Bit 0)                                          */
#define R_GLCDC_GR_CLUTINT_LINE_Msk     (0x7ffUL)      /*!< LINE (Bitfield-Mask: 0x7ff)                           */
/* ==========================================================  MON  ========================================================== */
#define R_GLCDC_GR_MON_UNDFLST_Pos      (16UL)         /*!< UNDFLST (Bit 16)                                      */
#define R_GLCDC_GR_MON_UNDFLST_Msk      (0x10000UL)    /*!< UNDFLST (Bitfield-Mask: 0x01)                         */
#define R_GLCDC_GR_MON_ARCST_Pos        (0UL)          /*!< ARCST (Bit 0)                                         */
#define R_GLCDC_GR_MON_ARCST_Msk        (0x1UL)        /*!< ARCST (Bitfield-Mask: 0x01)                           */

/* =========================================================================================================================== */
/* ================                                            GAM                                            ================ */
/* =========================================================================================================================== */

/* =========================================================  LATCH  ========================================================= */
#define R_GLCDC_GAM_LATCH_VEN_Pos       (0UL)     /*!< VEN (Bit 0)                                           */
#define R_GLCDC_GAM_LATCH_VEN_Msk       (0x1UL)   /*!< VEN (Bitfield-Mask: 0x01)                             */
/* ========================================================  GAM_SW  ========================================================= */
#define R_GLCDC_GAM_GAM_SW_GAMON_Pos    (0UL)     /*!< GAMON (Bit 0)                                         */
#define R_GLCDC_GAM_GAM_SW_GAMON_Msk    (0x1UL)   /*!< GAMON (Bitfield-Mask: 0x01)                           */
/* ==========================================================  LUT  ========================================================== */
#define R_GLCDC_GAM_LUT___Pos           (0UL)     /*!< _ (Bit 0)                                             */
#define R_GLCDC_GAM_LUT___Msk           (0x7ffUL) /*!< _ (Bitfield-Mask: 0x7ff)                              */
/* =========================================================  AREA  ========================================================== */
#define R_GLCDC_GAM_AREA___Pos          (0UL)     /*!< _ (Bit 0)                                             */
#define R_GLCDC_GAM_AREA___Msk          (0x3ffUL) /*!< _ (Bitfield-Mask: 0x3ff)                              */

/* =========================================================================================================================== */
/* ================                                            OUT                                            ================ */
/* =========================================================================================================================== */

/* ========================================================  VLATCH  ========================================================= */
#define R_GLCDC_OUT_VLATCH_VEN_Pos            (0UL)          /*!< VEN (Bit 0)                                           */
#define R_GLCDC_OUT_VLATCH_VEN_Msk            (0x1UL)        /*!< VEN (Bitfield-Mask: 0x01)                             */
/* ==========================================================  SET  ========================================================== */
#define R_GLCDC_OUT_SET_ENDIANON_Pos          (28UL)         /*!< ENDIANON (Bit 28)                                     */
#define R_GLCDC_OUT_SET_ENDIANON_Msk          (0x10000000UL) /*!< ENDIANON (Bitfield-Mask: 0x01)                        */
#define R_GLCDC_OUT_SET_SWAPON_Pos            (24UL)         /*!< SWAPON (Bit 24)                                       */
#define R_GLCDC_OUT_SET_SWAPON_Msk            (0x1000000UL)  /*!< SWAPON (Bitfield-Mask: 0x01)                          */
#define R_GLCDC_OUT_SET_FORMAT_Pos            (12UL)         /*!< FORMAT (Bit 12)                                       */
#define R_GLCDC_OUT_SET_FORMAT_Msk            (0x3000UL)     /*!< FORMAT (Bitfield-Mask: 0x03)                          */
#define R_GLCDC_OUT_SET_FRQSEL_Pos            (8UL)          /*!< FRQSEL (Bit 8)                                        */
#define R_GLCDC_OUT_SET_FRQSEL_Msk            (0x300UL)      /*!< FRQSEL (Bitfield-Mask: 0x03)                          */
#define R_GLCDC_OUT_SET_DIRSEL_Pos            (4UL)          /*!< DIRSEL (Bit 4)                                        */
#define R_GLCDC_OUT_SET_DIRSEL_Msk            (0x10UL)       /*!< DIRSEL (Bitfield-Mask: 0x01)                          */
#define R_GLCDC_OUT_SET_PHASE_Pos             (0UL)          /*!< PHASE (Bit 0)                                         */
#define R_GLCDC_OUT_SET_PHASE_Msk             (0x3UL)        /*!< PHASE (Bitfield-Mask: 0x03)                           */
/* ========================================================  BRIGHT1  ======================================================== */
#define R_GLCDC_OUT_BRIGHT1_BRTG_Pos          (0UL)          /*!< BRTG (Bit 0)                                          */
#define R_GLCDC_OUT_BRIGHT1_BRTG_Msk          (0x3ffUL)      /*!< BRTG (Bitfield-Mask: 0x3ff)                           */
/* ========================================================  BRIGHT2  ======================================================== */
#define R_GLCDC_OUT_BRIGHT2_BRTB_Pos          (16UL)         /*!< BRTB (Bit 16)                                         */
#define R_GLCDC_OUT_BRIGHT2_BRTB_Msk          (0x3ff0000UL)  /*!< BRTB (Bitfield-Mask: 0x3ff)                           */
#define R_GLCDC_OUT_BRIGHT2_BRTR_Pos          (0UL)          /*!< BRTR (Bit 0)                                          */
#define R_GLCDC_OUT_BRIGHT2_BRTR_Msk          (0x3ffUL)      /*!< BRTR (Bitfield-Mask: 0x3ff)                           */
/* =======================================================  CONTRAST  ======================================================== */
#define R_GLCDC_OUT_CONTRAST_CONTG_Pos        (16UL)         /*!< CONTG (Bit 16)                                        */
#define R_GLCDC_OUT_CONTRAST_CONTG_Msk        (0xff0000UL)   /*!< CONTG (Bitfield-Mask: 0xff)                           */
#define R_GLCDC_OUT_CONTRAST_CONTB_Pos        (8UL)          /*!< CONTB (Bit 8)                                         */
#define R_GLCDC_OUT_CONTRAST_CONTB_Msk        (0xff00UL)     /*!< CONTB (Bitfield-Mask: 0xff)                           */
#define R_GLCDC_OUT_CONTRAST_CONTR_Pos        (0UL)          /*!< CONTR (Bit 0)                                         */
#define R_GLCDC_OUT_CONTRAST_CONTR_Msk        (0xffUL)       /*!< CONTR (Bitfield-Mask: 0xff)                           */
/* =========================================================  PDTHA  ========================================================= */
#define R_GLCDC_OUT_PDTHA_SEL_Pos             (20UL)         /*!< SEL (Bit 20)                                          */
#define R_GLCDC_OUT_PDTHA_SEL_Msk             (0x300000UL)   /*!< SEL (Bitfield-Mask: 0x03)                             */
#define R_GLCDC_OUT_PDTHA_FORM_Pos            (16UL)         /*!< FORM (Bit 16)                                         */
#define R_GLCDC_OUT_PDTHA_FORM_Msk            (0x30000UL)    /*!< FORM (Bitfield-Mask: 0x03)                            */
#define R_GLCDC_OUT_PDTHA_PA_Pos              (12UL)         /*!< PA (Bit 12)                                           */
#define R_GLCDC_OUT_PDTHA_PA_Msk              (0x3000UL)     /*!< PA (Bitfield-Mask: 0x03)                              */
#define R_GLCDC_OUT_PDTHA_PB_Pos              (8UL)          /*!< PB (Bit 8)                                            */
#define R_GLCDC_OUT_PDTHA_PB_Msk              (0x300UL)      /*!< PB (Bitfield-Mask: 0x03)                              */
#define R_GLCDC_OUT_PDTHA_PC_Pos              (4UL)          /*!< PC (Bit 4)                                            */
#define R_GLCDC_OUT_PDTHA_PC_Msk              (0x30UL)       /*!< PC (Bitfield-Mask: 0x03)                              */
#define R_GLCDC_OUT_PDTHA_PD_Pos              (0UL)          /*!< PD (Bit 0)                                            */
#define R_GLCDC_OUT_PDTHA_PD_Msk              (0x3UL)        /*!< PD (Bitfield-Mask: 0x03)                              */
/* =======================================================  CLKPHASE  ======================================================== */
#define R_GLCDC_OUT_CLKPHASE_FRONTGAM_Pos     (12UL)         /*!< FRONTGAM (Bit 12)                                     */
#define R_GLCDC_OUT_CLKPHASE_FRONTGAM_Msk     (0x1000UL)     /*!< FRONTGAM (Bitfield-Mask: 0x01)                        */
#define R_GLCDC_OUT_CLKPHASE_LCDEDGE_Pos      (8UL)          /*!< LCDEDGE (Bit 8)                                       */
#define R_GLCDC_OUT_CLKPHASE_LCDEDGE_Msk      (0x100UL)      /*!< LCDEDGE (Bitfield-Mask: 0x01)                         */
#define R_GLCDC_OUT_CLKPHASE_TCON0EDGE_Pos    (6UL)          /*!< TCON0EDGE (Bit 6)                                     */
#define R_GLCDC_OUT_CLKPHASE_TCON0EDGE_Msk    (0x40UL)       /*!< TCON0EDGE (Bitfield-Mask: 0x01)                       */
#define R_GLCDC_OUT_CLKPHASE_TCON1EDGE_Pos    (5UL)          /*!< TCON1EDGE (Bit 5)                                     */
#define R_GLCDC_OUT_CLKPHASE_TCON1EDGE_Msk    (0x20UL)       /*!< TCON1EDGE (Bitfield-Mask: 0x01)                       */
#define R_GLCDC_OUT_CLKPHASE_TCON2EDGE_Pos    (4UL)          /*!< TCON2EDGE (Bit 4)                                     */
#define R_GLCDC_OUT_CLKPHASE_TCON2EDGE_Msk    (0x10UL)       /*!< TCON2EDGE (Bitfield-Mask: 0x01)                       */
#define R_GLCDC_OUT_CLKPHASE_TCON3EDGE_Pos    (3UL)          /*!< TCON3EDGE (Bit 3)                                     */
#define R_GLCDC_OUT_CLKPHASE_TCON3EDGE_Msk    (0x8UL)        /*!< TCON3EDGE (Bitfield-Mask: 0x01)                       */

/* =========================================================================================================================== */
/* ================                                           TCON                                            ================ */
/* =========================================================================================================================== */

/* ==========================================================  TIM  ========================================================== */
#define R_GLCDC_TCON_TIM_HALF_Pos       (16UL)        /*!< HALF (Bit 16)                                         */
#define R_GLCDC_TCON_TIM_HALF_Msk       (0x7ff0000UL) /*!< HALF (Bitfield-Mask: 0x7ff)                           */
#define R_GLCDC_TCON_TIM_OFFSET_Pos     (0UL)         /*!< OFFSET (Bit 0)                                        */
#define R_GLCDC_TCON_TIM_OFFSET_Msk     (0x7ffUL)     /*!< OFFSET (Bitfield-Mask: 0x7ff)                         */
/* =========================================================  STVA1  ========================================================= */
#define R_GLCDC_TCON_STVA1_VS_Pos       (16UL)        /*!< VS (Bit 16)                                           */
#define R_GLCDC_TCON_STVA1_VS_Msk       (0x7ff0000UL) /*!< VS (Bitfield-Mask: 0x7ff)                             */
#define R_GLCDC_TCON_STVA1_VW_Pos       (0UL)         /*!< VW (Bit 0)                                            */
#define R_GLCDC_TCON_STVA1_VW_Msk       (0x7ffUL)     /*!< VW (Bitfield-Mask: 0x7ff)                             */
/* =========================================================  STVB1  ========================================================= */
#define R_GLCDC_TCON_STVB1_VS_Pos       (16UL)        /*!< VS (Bit 16)                                           */
#define R_GLCDC_TCON_STVB1_VS_Msk       (0x7ff0000UL) /*!< VS (Bitfield-Mask: 0x7ff)                             */
#define R_GLCDC_TCON_STVB1_VW_Pos       (0UL)         /*!< VW (Bit 0)                                            */
#define R_GLCDC_TCON_STVB1_VW_Msk       (0x7ffUL)     /*!< VW (Bitfield-Mask: 0x7ff)                             */
/* =========================================================  STVA2  ========================================================= */
#define R_GLCDC_TCON_STVA2_INV_Pos      (4UL)         /*!< INV (Bit 4)                                           */
#define R_GLCDC_TCON_STVA2_INV_Msk      (0x10UL)      /*!< INV (Bitfield-Mask: 0x01)                             */
#define R_GLCDC_TCON_STVA2_SEL_Pos      (0UL)         /*!< SEL (Bit 0)                                           */
#define R_GLCDC_TCON_STVA2_SEL_Msk      (0x7UL)       /*!< SEL (Bitfield-Mask: 0x07)                             */
/* =========================================================  STVB2  ========================================================= */
#define R_GLCDC_TCON_STVB2_INV_Pos      (4UL)         /*!< INV (Bit 4)                                           */
#define R_GLCDC_TCON_STVB2_INV_Msk      (0x10UL)      /*!< INV (Bitfield-Mask: 0x01)                             */
#define R_GLCDC_TCON_STVB2_SEL_Pos      (0UL)         /*!< SEL (Bit 0)                                           */
#define R_GLCDC_TCON_STVB2_SEL_Msk      (0x7UL)       /*!< SEL (Bitfield-Mask: 0x07)                             */
/* =========================================================  STHA1  ========================================================= */
#define R_GLCDC_TCON_STHA1_HS_Pos       (16UL)        /*!< HS (Bit 16)                                           */
#define R_GLCDC_TCON_STHA1_HS_Msk       (0x7ff0000UL) /*!< HS (Bitfield-Mask: 0x7ff)                             */
#define R_GLCDC_TCON_STHA1_HW_Pos       (0UL)         /*!< HW (Bit 0)                                            */
#define R_GLCDC_TCON_STHA1_HW_Msk       (0x7ffUL)     /*!< HW (Bitfield-Mask: 0x7ff)                             */
/* =========================================================  STHB1  ========================================================= */
#define R_GLCDC_TCON_STHB1_HS_Pos       (16UL)        /*!< HS (Bit 16)                                           */
#define R_GLCDC_TCON_STHB1_HS_Msk       (0x7ff0000UL) /*!< HS (Bitfield-Mask: 0x7ff)                             */
#define R_GLCDC_TCON_STHB1_HW_Pos       (0UL)         /*!< HW (Bit 0)                                            */
#define R_GLCDC_TCON_STHB1_HW_Msk       (0x7ffUL)     /*!< HW (Bitfield-Mask: 0x7ff)                             */
/* =========================================================  STHA2  ========================================================= */
#define R_GLCDC_TCON_STHA2_HSSEL_Pos    (8UL)         /*!< HSSEL (Bit 8)                                         */
#define R_GLCDC_TCON_STHA2_HSSEL_Msk    (0x100UL)     /*!< HSSEL (Bitfield-Mask: 0x01)                           */
#define R_GLCDC_TCON_STHA2_INV_Pos      (4UL)         /*!< INV (Bit 4)                                           */
#define R_GLCDC_TCON_STHA2_INV_Msk      (0x10UL)      /*!< INV (Bitfield-Mask: 0x01)                             */
#define R_GLCDC_TCON_STHA2_SEL_Pos      (0UL)         /*!< SEL (Bit 0)                                           */
#define R_GLCDC_TCON_STHA2_SEL_Msk      (0x7UL)       /*!< SEL (Bitfield-Mask: 0x07)                             */
/* =========================================================  STHB2  ========================================================= */
#define R_GLCDC_TCON_STHB2_HSSEL_Pos    (8UL)         /*!< HSSEL (Bit 8)                                         */
#define R_GLCDC_TCON_STHB2_HSSEL_Msk    (0x100UL)     /*!< HSSEL (Bitfield-Mask: 0x01)                           */
#define R_GLCDC_TCON_STHB2_INV_Pos      (4UL)         /*!< INV (Bit 4)                                           */
#define R_GLCDC_TCON_STHB2_INV_Msk      (0x10UL)      /*!< INV (Bitfield-Mask: 0x01)                             */
#define R_GLCDC_TCON_STHB2_SEL_Pos      (0UL)         /*!< SEL (Bit 0)                                           */
#define R_GLCDC_TCON_STHB2_SEL_Msk      (0x7UL)       /*!< SEL (Bitfield-Mask: 0x07)                             */
/* ==========================================================  DE  =========================================================== */
#define R_GLCDC_TCON_DE_INV_Pos         (0UL)         /*!< INV (Bit 0)                                           */
#define R_GLCDC_TCON_DE_INV_Msk         (0x1UL)       /*!< INV (Bitfield-Mask: 0x01)                             */

/* =========================================================================================================================== */
/* ================                                          SYSCNT                                           ================ */
/* =========================================================================================================================== */

/* ========================================================  DTCTEN  ========================================================= */
#define R_GLCDC_SYSCNT_DTCTEN_L2UNDFDTC_Pos     (2UL)          /*!< L2UNDFDTC (Bit 2)                                     */
#define R_GLCDC_SYSCNT_DTCTEN_L2UNDFDTC_Msk     (0x4UL)        /*!< L2UNDFDTC (Bitfield-Mask: 0x01)                       */
#define R_GLCDC_SYSCNT_DTCTEN_L1UNDFDTC_Pos     (1UL)          /*!< L1UNDFDTC (Bit 1)                                     */
#define R_GLCDC_SYSCNT_DTCTEN_L1UNDFDTC_Msk     (0x2UL)        /*!< L1UNDFDTC (Bitfield-Mask: 0x01)                       */
#define R_GLCDC_SYSCNT_DTCTEN_VPOSDTC_Pos       (0UL)          /*!< VPOSDTC (Bit 0)                                       */
#define R_GLCDC_SYSCNT_DTCTEN_VPOSDTC_Msk       (0x1UL)        /*!< VPOSDTC (Bitfield-Mask: 0x01)                         */
/* =========================================================  INTEN  ========================================================= */
#define R_GLCDC_SYSCNT_INTEN_L2UNDFINTEN_Pos    (2UL)          /*!< L2UNDFINTEN (Bit 2)                                   */
#define R_GLCDC_SYSCNT_INTEN_L2UNDFINTEN_Msk    (0x4UL)        /*!< L2UNDFINTEN (Bitfield-Mask: 0x01)                     */
#define R_GLCDC_SYSCNT_INTEN_L1UNDFINTEN_Pos    (1UL)          /*!< L1UNDFINTEN (Bit 1)                                   */
#define R_GLCDC_SYSCNT_INTEN_L1UNDFINTEN_Msk    (0x2UL)        /*!< L1UNDFINTEN (Bitfield-Mask: 0x01)                     */
#define R_GLCDC_SYSCNT_INTEN_VPOSINTEN_Pos      (0UL)          /*!< VPOSINTEN (Bit 0)                                     */
#define R_GLCDC_SYSCNT_INTEN_VPOSINTEN_Msk      (0x1UL)        /*!< VPOSINTEN (Bitfield-Mask: 0x01)                       */
/* =========================================================  STCLR  ========================================================= */
#define R_GLCDC_SYSCNT_STCLR_L2UNDFCLR_Pos      (2UL)          /*!< L2UNDFCLR (Bit 2)                                     */
#define R_GLCDC_SYSCNT_STCLR_L2UNDFCLR_Msk      (0x4UL)        /*!< L2UNDFCLR (Bitfield-Mask: 0x01)                       */
#define R_GLCDC_SYSCNT_STCLR_L1UNDFCLR_Pos      (1UL)          /*!< L1UNDFCLR (Bit 1)                                     */
#define R_GLCDC_SYSCNT_STCLR_L1UNDFCLR_Msk      (0x2UL)        /*!< L1UNDFCLR (Bitfield-Mask: 0x01)                       */
#define R_GLCDC_SYSCNT_STCLR_VPOSCLR_Pos        (0UL)          /*!< VPOSCLR (Bit 0)                                       */
#define R_GLCDC_SYSCNT_STCLR_VPOSCLR_Msk        (0x1UL)        /*!< VPOSCLR (Bitfield-Mask: 0x01)                         */
/* =========================================================  STMON  ========================================================= */
#define R_GLCDC_SYSCNT_STMON_L2UNDF_Pos         (2UL)          /*!< L2UNDF (Bit 2)                                        */
#define R_GLCDC_SYSCNT_STMON_L2UNDF_Msk         (0x4UL)        /*!< L2UNDF (Bitfield-Mask: 0x01)                          */
#define R_GLCDC_SYSCNT_STMON_L1UNDF_Pos         (1UL)          /*!< L1UNDF (Bit 1)                                        */
#define R_GLCDC_SYSCNT_STMON_L1UNDF_Msk         (0x2UL)        /*!< L1UNDF (Bitfield-Mask: 0x01)                          */
#define R_GLCDC_SYSCNT_STMON_VPOS_Pos           (0UL)          /*!< VPOS (Bit 0)                                          */
#define R_GLCDC_SYSCNT_STMON_VPOS_Msk           (0x1UL)        /*!< VPOS (Bitfield-Mask: 0x01)                            */
/* =======================================================  PANEL_CLK  ======================================================= */
#define R_GLCDC_SYSCNT_PANEL_CLK_VER_Pos        (16UL)         /*!< VER (Bit 16)                                          */
#define R_GLCDC_SYSCNT_PANEL_CLK_VER_Msk        (0xffff0000UL) /*!< VER (Bitfield-Mask: 0xffff)                           */
#define R_GLCDC_SYSCNT_PANEL_CLK_PIXSEL_Pos     (12UL)         /*!< PIXSEL (Bit 12)                                       */
#define R_GLCDC_SYSCNT_PANEL_CLK_PIXSEL_Msk     (0x1000UL)     /*!< PIXSEL (Bitfield-Mask: 0x01)                          */
#define R_GLCDC_SYSCNT_PANEL_CLK_CLKSEL_Pos     (8UL)          /*!< CLKSEL (Bit 8)                                        */
#define R_GLCDC_SYSCNT_PANEL_CLK_CLKSEL_Msk     (0x100UL)      /*!< CLKSEL (Bitfield-Mask: 0x01)                          */
#define R_GLCDC_SYSCNT_PANEL_CLK_CLKEN_Pos      (6UL)          /*!< CLKEN (Bit 6)                                         */
#define R_GLCDC_SYSCNT_PANEL_CLK_CLKEN_Msk      (0x40UL)       /*!< CLKEN (Bitfield-Mask: 0x01)                           */
#define R_GLCDC_SYSCNT_PANEL_CLK_DCDR_Pos       (0UL)          /*!< DCDR (Bit 0)                                          */
#define R_GLCDC_SYSCNT_PANEL_CLK_DCDR_Msk       (0x3fUL)       /*!< DCDR (Bitfield-Mask: 0x3f)                            */

#endif /* R_GLCDC_POSMSK_H */
