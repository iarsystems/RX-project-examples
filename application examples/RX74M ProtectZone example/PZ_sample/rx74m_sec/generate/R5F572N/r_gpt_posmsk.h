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

#ifndef R_GPT_POSMSK_H
#define R_GPT_POSMSK_H

/* =========================================================================================================================== */
/* ================                                           GPTW                                            ================ */
/* =========================================================================================================================== */

/* =========================================================  GTWP  ========================================================== */
#define R_GPT0_GTWP_PRKEY_Pos             (8UL)          /*!< PRKEY (Bit 8)                                         */
#define R_GPT0_GTWP_PRKEY_Msk             (0xff00UL)     /*!< PRKEY (Bitfield-Mask: 0xff)                           */
#define R_GPT0_GTWP_CMNWP_Pos             (4UL)          /*!< CMNWP (Bit 4)                                         */
#define R_GPT0_GTWP_CMNWP_Msk             (0x10UL)       /*!< CMNWP (Bitfield-Mask: 0x01)                           */
#define R_GPT0_GTWP_CLRWP_Pos             (3UL)          /*!< CLRWP (Bit 3)                                         */
#define R_GPT0_GTWP_CLRWP_Msk             (0x8UL)        /*!< CLRWP (Bitfield-Mask: 0x01)                           */
#define R_GPT0_GTWP_STPWP_Pos             (2UL)          /*!< STPWP (Bit 2)                                         */
#define R_GPT0_GTWP_STPWP_Msk             (0x4UL)        /*!< STPWP (Bitfield-Mask: 0x01)                           */
#define R_GPT0_GTWP_STRWP_Pos             (1UL)          /*!< STRWP (Bit 1)                                         */
#define R_GPT0_GTWP_STRWP_Msk             (0x2UL)        /*!< STRWP (Bitfield-Mask: 0x01)                           */
#define R_GPT0_GTWP_WP_Pos                (0UL)          /*!< WP (Bit 0)                                            */
#define R_GPT0_GTWP_WP_Msk                (0x1UL)        /*!< WP (Bitfield-Mask: 0x01)                              */
/* =========================================================  GTSTR  ========================================================= */
#define R_GPT0_GTSTR_CSTRT3_Pos           (3UL)          /*!< CSTRT3 (Bit 3)                                        */
#define R_GPT0_GTSTR_CSTRT3_Msk           (0x8UL)        /*!< CSTRT3 (Bitfield-Mask: 0x01)                          */
#define R_GPT0_GTSTR_CSTRT2_Pos           (2UL)          /*!< CSTRT2 (Bit 2)                                        */
#define R_GPT0_GTSTR_CSTRT2_Msk           (0x4UL)        /*!< CSTRT2 (Bitfield-Mask: 0x01)                          */
#define R_GPT0_GTSTR_CSTRT1_Pos           (1UL)          /*!< CSTRT1 (Bit 1)                                        */
#define R_GPT0_GTSTR_CSTRT1_Msk           (0x2UL)        /*!< CSTRT1 (Bitfield-Mask: 0x01)                          */
#define R_GPT0_GTSTR_CSTRT0_Pos           (0UL)          /*!< CSTRT0 (Bit 0)                                        */
#define R_GPT0_GTSTR_CSTRT0_Msk           (0x1UL)        /*!< CSTRT0 (Bitfield-Mask: 0x01)                          */
/* =========================================================  GTSTP  ========================================================= */
#define R_GPT0_GTSTP_CSTOP3_Pos           (3UL)          /*!< CSTOP3 (Bit 3)                                        */
#define R_GPT0_GTSTP_CSTOP3_Msk           (0x8UL)        /*!< CSTOP3 (Bitfield-Mask: 0x01)                          */
#define R_GPT0_GTSTP_CSTOP2_Pos           (2UL)          /*!< CSTOP2 (Bit 2)                                        */
#define R_GPT0_GTSTP_CSTOP2_Msk           (0x4UL)        /*!< CSTOP2 (Bitfield-Mask: 0x01)                          */
#define R_GPT0_GTSTP_CSTOP1_Pos           (1UL)          /*!< CSTOP1 (Bit 1)                                        */
#define R_GPT0_GTSTP_CSTOP1_Msk           (0x2UL)        /*!< CSTOP1 (Bitfield-Mask: 0x01)                          */
#define R_GPT0_GTSTP_CSTOP0_Pos           (0UL)          /*!< CSTOP0 (Bit 0)                                        */
#define R_GPT0_GTSTP_CSTOP0_Msk           (0x1UL)        /*!< CSTOP0 (Bitfield-Mask: 0x01)                          */
/* =========================================================  GTCLR  ========================================================= */
#define R_GPT0_GTCLR_CCLR3_Pos            (3UL)          /*!< CCLR3 (Bit 3)                                         */
#define R_GPT0_GTCLR_CCLR3_Msk            (0x8UL)        /*!< CCLR3 (Bitfield-Mask: 0x01)                           */
#define R_GPT0_GTCLR_CCLR2_Pos            (2UL)          /*!< CCLR2 (Bit 2)                                         */
#define R_GPT0_GTCLR_CCLR2_Msk            (0x4UL)        /*!< CCLR2 (Bitfield-Mask: 0x01)                           */
#define R_GPT0_GTCLR_CCLR1_Pos            (1UL)          /*!< CCLR1 (Bit 1)                                         */
#define R_GPT0_GTCLR_CCLR1_Msk            (0x2UL)        /*!< CCLR1 (Bitfield-Mask: 0x01)                           */
#define R_GPT0_GTCLR_CCLR0_Pos            (0UL)          /*!< CCLR0 (Bit 0)                                         */
#define R_GPT0_GTCLR_CCLR0_Msk            (0x1UL)        /*!< CCLR0 (Bitfield-Mask: 0x01)                           */
/* =========================================================  GTSSR  ========================================================= */
#define R_GPT0_GTSSR_CSTRT_Pos            (31UL)         /*!< CSTRT (Bit 31)                                        */
#define R_GPT0_GTSSR_CSTRT_Msk            (0x80000000UL) /*!< CSTRT (Bitfield-Mask: 0x01)                           */
#define R_GPT0_GTSSR_SSELC_Pos            (16UL)         /*!< SSELC (Bit 16)                                        */
#define R_GPT0_GTSSR_SSELC_Msk            (0x10000UL)    /*!< SSELC (Bitfield-Mask: 0x01)                           */
#define R_GPT0_GTSSR_SSCBFAH_Pos          (15UL)         /*!< SSCBFAH (Bit 15)                                      */
#define R_GPT0_GTSSR_SSCBFAH_Msk          (0x8000UL)     /*!< SSCBFAH (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTSSR_SSCBFAL_Pos          (14UL)         /*!< SSCBFAL (Bit 14)                                      */
#define R_GPT0_GTSSR_SSCBFAL_Msk          (0x4000UL)     /*!< SSCBFAL (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTSSR_SSCBRAH_Pos          (13UL)         /*!< SSCBRAH (Bit 13)                                      */
#define R_GPT0_GTSSR_SSCBRAH_Msk          (0x2000UL)     /*!< SSCBRAH (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTSSR_SSCBRAL_Pos          (12UL)         /*!< SSCBRAL (Bit 12)                                      */
#define R_GPT0_GTSSR_SSCBRAL_Msk          (0x1000UL)     /*!< SSCBRAL (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTSSR_SSCAFBH_Pos          (11UL)         /*!< SSCAFBH (Bit 11)                                      */
#define R_GPT0_GTSSR_SSCAFBH_Msk          (0x800UL)      /*!< SSCAFBH (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTSSR_SSCAFBL_Pos          (10UL)         /*!< SSCAFBL (Bit 10)                                      */
#define R_GPT0_GTSSR_SSCAFBL_Msk          (0x400UL)      /*!< SSCAFBL (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTSSR_SSCARBH_Pos          (9UL)          /*!< SSCARBH (Bit 9)                                       */
#define R_GPT0_GTSSR_SSCARBH_Msk          (0x200UL)      /*!< SSCARBH (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTSSR_SSCARBL_Pos          (8UL)          /*!< SSCARBL (Bit 8)                                       */
#define R_GPT0_GTSSR_SSCARBL_Msk          (0x100UL)      /*!< SSCARBL (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTSSR_SSGTRGF_Pos          (1UL)          /*!< SSGTRGF (Bit 1)                                       */
#define R_GPT0_GTSSR_SSGTRGF_Msk          (0x2UL)        /*!< SSGTRGF (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTSSR_SSGTRGR_Pos          (0UL)          /*!< SSGTRGR (Bit 0)                                       */
#define R_GPT0_GTSSR_SSGTRGR_Msk          (0x1UL)        /*!< SSGTRGR (Bitfield-Mask: 0x01)                         */
/* =========================================================  GTPSR  ========================================================= */
#define R_GPT0_GTPSR_CSTOP_Pos            (31UL)         /*!< CSTOP (Bit 31)                                        */
#define R_GPT0_GTPSR_CSTOP_Msk            (0x80000000UL) /*!< CSTOP (Bitfield-Mask: 0x01)                           */
#define R_GPT0_GTPSR_PSELC_Pos            (16UL)         /*!< PSELC (Bit 16)                                        */
#define R_GPT0_GTPSR_PSELC_Msk            (0x10000UL)    /*!< PSELC (Bitfield-Mask: 0x01)                           */
#define R_GPT0_GTPSR_PSCBFAH_Pos          (15UL)         /*!< PSCBFAH (Bit 15)                                      */
#define R_GPT0_GTPSR_PSCBFAH_Msk          (0x8000UL)     /*!< PSCBFAH (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTPSR_PSCBFAL_Pos          (14UL)         /*!< PSCBFAL (Bit 14)                                      */
#define R_GPT0_GTPSR_PSCBFAL_Msk          (0x4000UL)     /*!< PSCBFAL (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTPSR_PSCBRAH_Pos          (13UL)         /*!< PSCBRAH (Bit 13)                                      */
#define R_GPT0_GTPSR_PSCBRAH_Msk          (0x2000UL)     /*!< PSCBRAH (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTPSR_PSCBRAL_Pos          (12UL)         /*!< PSCBRAL (Bit 12)                                      */
#define R_GPT0_GTPSR_PSCBRAL_Msk          (0x1000UL)     /*!< PSCBRAL (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTPSR_PSCAFBH_Pos          (11UL)         /*!< PSCAFBH (Bit 11)                                      */
#define R_GPT0_GTPSR_PSCAFBH_Msk          (0x800UL)      /*!< PSCAFBH (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTPSR_PSCAFBL_Pos          (10UL)         /*!< PSCAFBL (Bit 10)                                      */
#define R_GPT0_GTPSR_PSCAFBL_Msk          (0x400UL)      /*!< PSCAFBL (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTPSR_PSCARBH_Pos          (9UL)          /*!< PSCARBH (Bit 9)                                       */
#define R_GPT0_GTPSR_PSCARBH_Msk          (0x200UL)      /*!< PSCARBH (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTPSR_PSCARBL_Pos          (8UL)          /*!< PSCARBL (Bit 8)                                       */
#define R_GPT0_GTPSR_PSCARBL_Msk          (0x100UL)      /*!< PSCARBL (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTPSR_PSGTRGF_Pos          (1UL)          /*!< PSGTRGF (Bit 1)                                       */
#define R_GPT0_GTPSR_PSGTRGF_Msk          (0x2UL)        /*!< PSGTRGF (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTPSR_PSGTRGR_Pos          (0UL)          /*!< PSGTRGR (Bit 0)                                       */
#define R_GPT0_GTPSR_PSGTRGR_Msk          (0x1UL)        /*!< PSGTRGR (Bitfield-Mask: 0x01)                         */
/* =========================================================  GTCSR  ========================================================= */
#define R_GPT0_GTCSR_CCLR_Pos             (31UL)         /*!< CCLR (Bit 31)                                         */
#define R_GPT0_GTCSR_CCLR_Msk             (0x80000000UL) /*!< CCLR (Bitfield-Mask: 0x01)                            */
#define R_GPT0_GTCSR_CP1CCE_Pos           (27UL)         /*!< CP1CCE (Bit 27)                                       */
#define R_GPT0_GTCSR_CP1CCE_Msk           (0x8000000UL)  /*!< CP1CCE (Bitfield-Mask: 0x01)                          */
#define R_GPT0_GTCSR_CSCMSC_Pos           (24UL)         /*!< CSCMSC (Bit 24)                                       */
#define R_GPT0_GTCSR_CSCMSC_Msk           (0x7000000UL)  /*!< CSCMSC (Bitfield-Mask: 0x07)                          */
#define R_GPT0_GTCSR_CSELC_Pos            (16UL)         /*!< CSELC (Bit 16)                                        */
#define R_GPT0_GTCSR_CSELC_Msk            (0x10000UL)    /*!< CSELC (Bitfield-Mask: 0x01)                           */
#define R_GPT0_GTCSR_CSCBFAH_Pos          (15UL)         /*!< CSCBFAH (Bit 15)                                      */
#define R_GPT0_GTCSR_CSCBFAH_Msk          (0x8000UL)     /*!< CSCBFAH (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTCSR_CSCBFAL_Pos          (14UL)         /*!< CSCBFAL (Bit 14)                                      */
#define R_GPT0_GTCSR_CSCBFAL_Msk          (0x4000UL)     /*!< CSCBFAL (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTCSR_CSCBRAH_Pos          (13UL)         /*!< CSCBRAH (Bit 13)                                      */
#define R_GPT0_GTCSR_CSCBRAH_Msk          (0x2000UL)     /*!< CSCBRAH (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTCSR_CSCBRAL_Pos          (12UL)         /*!< CSCBRAL (Bit 12)                                      */
#define R_GPT0_GTCSR_CSCBRAL_Msk          (0x1000UL)     /*!< CSCBRAL (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTCSR_CSCAFBH_Pos          (11UL)         /*!< CSCAFBH (Bit 11)                                      */
#define R_GPT0_GTCSR_CSCAFBH_Msk          (0x800UL)      /*!< CSCAFBH (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTCSR_CSCAFBL_Pos          (10UL)         /*!< CSCAFBL (Bit 10)                                      */
#define R_GPT0_GTCSR_CSCAFBL_Msk          (0x400UL)      /*!< CSCAFBL (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTCSR_CSCARBH_Pos          (9UL)          /*!< CSCARBH (Bit 9)                                       */
#define R_GPT0_GTCSR_CSCARBH_Msk          (0x200UL)      /*!< CSCARBH (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTCSR_CSCARBL_Pos          (8UL)          /*!< CSCARBL (Bit 8)                                       */
#define R_GPT0_GTCSR_CSCARBL_Msk          (0x100UL)      /*!< CSCARBL (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTCSR_CSGTRGF_Pos          (1UL)          /*!< CSGTRGF (Bit 1)                                       */
#define R_GPT0_GTCSR_CSGTRGF_Msk          (0x2UL)        /*!< CSGTRGF (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTCSR_CSGTRGR_Pos          (0UL)          /*!< CSGTRGR (Bit 0)                                       */
#define R_GPT0_GTCSR_CSGTRGR_Msk          (0x1UL)        /*!< CSGTRGR (Bitfield-Mask: 0x01)                         */
/* ========================================================  GTUPSR  ========================================================= */
#define R_GPT0_GTUPSR_USILVL_Pos          (24UL)         /*!< USILVL (Bit 24)                                       */
#define R_GPT0_GTUPSR_USILVL_Msk          (0xf000000UL)  /*!< USILVL (Bitfield-Mask: 0x0f)                          */
#define R_GPT0_GTUPSR_USELC_Pos           (16UL)         /*!< USELC (Bit 16)                                        */
#define R_GPT0_GTUPSR_USELC_Msk           (0x10000UL)    /*!< USELC (Bitfield-Mask: 0x01)                           */
#define R_GPT0_GTUPSR_USCBFAH_Pos         (15UL)         /*!< USCBFAH (Bit 15)                                      */
#define R_GPT0_GTUPSR_USCBFAH_Msk         (0x8000UL)     /*!< USCBFAH (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTUPSR_USCBFAL_Pos         (14UL)         /*!< USCBFAL (Bit 14)                                      */
#define R_GPT0_GTUPSR_USCBFAL_Msk         (0x4000UL)     /*!< USCBFAL (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTUPSR_USCBRAH_Pos         (13UL)         /*!< USCBRAH (Bit 13)                                      */
#define R_GPT0_GTUPSR_USCBRAH_Msk         (0x2000UL)     /*!< USCBRAH (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTUPSR_USCBRAL_Pos         (12UL)         /*!< USCBRAL (Bit 12)                                      */
#define R_GPT0_GTUPSR_USCBRAL_Msk         (0x1000UL)     /*!< USCBRAL (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTUPSR_USCAFBH_Pos         (11UL)         /*!< USCAFBH (Bit 11)                                      */
#define R_GPT0_GTUPSR_USCAFBH_Msk         (0x800UL)      /*!< USCAFBH (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTUPSR_USCAFBL_Pos         (10UL)         /*!< USCAFBL (Bit 10)                                      */
#define R_GPT0_GTUPSR_USCAFBL_Msk         (0x400UL)      /*!< USCAFBL (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTUPSR_USCARBH_Pos         (9UL)          /*!< USCARBH (Bit 9)                                       */
#define R_GPT0_GTUPSR_USCARBH_Msk         (0x200UL)      /*!< USCARBH (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTUPSR_USCARBL_Pos         (8UL)          /*!< USCARBL (Bit 8)                                       */
#define R_GPT0_GTUPSR_USCARBL_Msk         (0x100UL)      /*!< USCARBL (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTUPSR_USGTRGF_Pos         (1UL)          /*!< USGTRGF (Bit 1)                                       */
#define R_GPT0_GTUPSR_USGTRGF_Msk         (0x2UL)        /*!< USGTRGF (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTUPSR_USGTRGR_Pos         (0UL)          /*!< USGTRGR (Bit 0)                                       */
#define R_GPT0_GTUPSR_USGTRGR_Msk         (0x1UL)        /*!< USGTRGR (Bitfield-Mask: 0x01)                         */
/* ========================================================  GTDNSR  ========================================================= */
#define R_GPT0_GTDNSR_DSILVL_Pos          (24UL)         /*!< DSILVL (Bit 24)                                       */
#define R_GPT0_GTDNSR_DSILVL_Msk          (0xf000000UL)  /*!< DSILVL (Bitfield-Mask: 0x0f)                          */
#define R_GPT0_GTDNSR_DSELC_Pos           (16UL)         /*!< DSELC (Bit 16)                                        */
#define R_GPT0_GTDNSR_DSELC_Msk           (0x10000UL)    /*!< DSELC (Bitfield-Mask: 0x01)                           */
#define R_GPT0_GTDNSR_DSCBFAH_Pos         (15UL)         /*!< DSCBFAH (Bit 15)                                      */
#define R_GPT0_GTDNSR_DSCBFAH_Msk         (0x8000UL)     /*!< DSCBFAH (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTDNSR_DSCBFAL_Pos         (14UL)         /*!< DSCBFAL (Bit 14)                                      */
#define R_GPT0_GTDNSR_DSCBFAL_Msk         (0x4000UL)     /*!< DSCBFAL (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTDNSR_DSCBRAH_Pos         (13UL)         /*!< DSCBRAH (Bit 13)                                      */
#define R_GPT0_GTDNSR_DSCBRAH_Msk         (0x2000UL)     /*!< DSCBRAH (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTDNSR_DSCBRAL_Pos         (12UL)         /*!< DSCBRAL (Bit 12)                                      */
#define R_GPT0_GTDNSR_DSCBRAL_Msk         (0x1000UL)     /*!< DSCBRAL (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTDNSR_DSCAFBH_Pos         (11UL)         /*!< DSCAFBH (Bit 11)                                      */
#define R_GPT0_GTDNSR_DSCAFBH_Msk         (0x800UL)      /*!< DSCAFBH (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTDNSR_DSCAFBL_Pos         (10UL)         /*!< DSCAFBL (Bit 10)                                      */
#define R_GPT0_GTDNSR_DSCAFBL_Msk         (0x400UL)      /*!< DSCAFBL (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTDNSR_DSCARBH_Pos         (9UL)          /*!< DSCARBH (Bit 9)                                       */
#define R_GPT0_GTDNSR_DSCARBH_Msk         (0x200UL)      /*!< DSCARBH (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTDNSR_DSCARBL_Pos         (8UL)          /*!< DSCARBL (Bit 8)                                       */
#define R_GPT0_GTDNSR_DSCARBL_Msk         (0x100UL)      /*!< DSCARBL (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTDNSR_DSGTRGF_Pos         (1UL)          /*!< DSGTRGF (Bit 1)                                       */
#define R_GPT0_GTDNSR_DSGTRGF_Msk         (0x2UL)        /*!< DSGTRGF (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTDNSR_DSGTRGR_Pos         (0UL)          /*!< DSGTRGR (Bit 0)                                       */
#define R_GPT0_GTDNSR_DSGTRGR_Msk         (0x1UL)        /*!< DSGTRGR (Bitfield-Mask: 0x01)                         */
/* ========================================================  GTICASR  ======================================================== */
#define R_GPT0_GTICASR_ASELC_Pos          (16UL)         /*!< ASELC (Bit 16)                                        */
#define R_GPT0_GTICASR_ASELC_Msk          (0x10000UL)    /*!< ASELC (Bitfield-Mask: 0x01)                           */
#define R_GPT0_GTICASR_ASCBFAH_Pos        (15UL)         /*!< ASCBFAH (Bit 15)                                      */
#define R_GPT0_GTICASR_ASCBFAH_Msk        (0x8000UL)     /*!< ASCBFAH (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTICASR_ASCBFAL_Pos        (14UL)         /*!< ASCBFAL (Bit 14)                                      */
#define R_GPT0_GTICASR_ASCBFAL_Msk        (0x4000UL)     /*!< ASCBFAL (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTICASR_ASCBRAH_Pos        (13UL)         /*!< ASCBRAH (Bit 13)                                      */
#define R_GPT0_GTICASR_ASCBRAH_Msk        (0x2000UL)     /*!< ASCBRAH (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTICASR_ASCBRAL_Pos        (12UL)         /*!< ASCBRAL (Bit 12)                                      */
#define R_GPT0_GTICASR_ASCBRAL_Msk        (0x1000UL)     /*!< ASCBRAL (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTICASR_ASCAFBH_Pos        (11UL)         /*!< ASCAFBH (Bit 11)                                      */
#define R_GPT0_GTICASR_ASCAFBH_Msk        (0x800UL)      /*!< ASCAFBH (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTICASR_ASCAFBL_Pos        (10UL)         /*!< ASCAFBL (Bit 10)                                      */
#define R_GPT0_GTICASR_ASCAFBL_Msk        (0x400UL)      /*!< ASCAFBL (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTICASR_ASCARBH_Pos        (9UL)          /*!< ASCARBH (Bit 9)                                       */
#define R_GPT0_GTICASR_ASCARBH_Msk        (0x200UL)      /*!< ASCARBH (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTICASR_ASCARBL_Pos        (8UL)          /*!< ASCARBL (Bit 8)                                       */
#define R_GPT0_GTICASR_ASCARBL_Msk        (0x100UL)      /*!< ASCARBL (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTICASR_ASGTRGF_Pos        (1UL)          /*!< ASGTRGF (Bit 1)                                       */
#define R_GPT0_GTICASR_ASGTRGF_Msk        (0x2UL)        /*!< ASGTRGF (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTICASR_ASGTRGR_Pos        (0UL)          /*!< ASGTRGR (Bit 0)                                       */
#define R_GPT0_GTICASR_ASGTRGR_Msk        (0x1UL)        /*!< ASGTRGR (Bitfield-Mask: 0x01)                         */
/* ========================================================  GTICBSR  ======================================================== */
#define R_GPT0_GTICBSR_BSELC_Pos          (16UL)         /*!< BSELC (Bit 16)                                        */
#define R_GPT0_GTICBSR_BSELC_Msk          (0x10000UL)    /*!< BSELC (Bitfield-Mask: 0x01)                           */
#define R_GPT0_GTICBSR_BSCBFAH_Pos        (15UL)         /*!< BSCBFAH (Bit 15)                                      */
#define R_GPT0_GTICBSR_BSCBFAH_Msk        (0x8000UL)     /*!< BSCBFAH (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTICBSR_BSCBFAL_Pos        (14UL)         /*!< BSCBFAL (Bit 14)                                      */
#define R_GPT0_GTICBSR_BSCBFAL_Msk        (0x4000UL)     /*!< BSCBFAL (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTICBSR_BSCBRAH_Pos        (13UL)         /*!< BSCBRAH (Bit 13)                                      */
#define R_GPT0_GTICBSR_BSCBRAH_Msk        (0x2000UL)     /*!< BSCBRAH (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTICBSR_BSCBRAL_Pos        (12UL)         /*!< BSCBRAL (Bit 12)                                      */
#define R_GPT0_GTICBSR_BSCBRAL_Msk        (0x1000UL)     /*!< BSCBRAL (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTICBSR_BSCAFBH_Pos        (11UL)         /*!< BSCAFBH (Bit 11)                                      */
#define R_GPT0_GTICBSR_BSCAFBH_Msk        (0x800UL)      /*!< BSCAFBH (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTICBSR_BSCAFBL_Pos        (10UL)         /*!< BSCAFBL (Bit 10)                                      */
#define R_GPT0_GTICBSR_BSCAFBL_Msk        (0x400UL)      /*!< BSCAFBL (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTICBSR_BSCARBH_Pos        (9UL)          /*!< BSCARBH (Bit 9)                                       */
#define R_GPT0_GTICBSR_BSCARBH_Msk        (0x200UL)      /*!< BSCARBH (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTICBSR_BSCARBL_Pos        (8UL)          /*!< BSCARBL (Bit 8)                                       */
#define R_GPT0_GTICBSR_BSCARBL_Msk        (0x100UL)      /*!< BSCARBL (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTICBSR_BSGTRGF_Pos        (1UL)          /*!< BSGTRGF (Bit 1)                                       */
#define R_GPT0_GTICBSR_BSGTRGF_Msk        (0x2UL)        /*!< BSGTRGF (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTICBSR_BSGTRGR_Pos        (0UL)          /*!< BSGTRGR (Bit 0)                                       */
#define R_GPT0_GTICBSR_BSGTRGR_Msk        (0x1UL)        /*!< BSGTRGR (Bitfield-Mask: 0x01)                         */
/* =========================================================  GTCR  ========================================================== */
#define R_GPT0_GTCR_TPCS_Pos              (23UL)         /*!< TPCS (Bit 23)                                         */
#define R_GPT0_GTCR_TPCS_Msk              (0x7800000UL)  /*!< TPCS (Bitfield-Mask: 0x0f)                            */
#define R_GPT0_GTCR_MD_Pos                (16UL)         /*!< MD (Bit 16)                                           */
#define R_GPT0_GTCR_MD_Msk                (0x70000UL)    /*!< MD (Bitfield-Mask: 0x07)                              */
#define R_GPT0_GTCR_ICDS_Pos              (8UL)          /*!< ICDS (Bit 8)                                          */
#define R_GPT0_GTCR_ICDS_Msk              (0x100UL)      /*!< ICDS (Bitfield-Mask: 0x01)                            */
#define R_GPT0_GTCR_CST_Pos               (0UL)          /*!< CST (Bit 0)                                           */
#define R_GPT0_GTCR_CST_Msk               (0x1UL)        /*!< CST (Bitfield-Mask: 0x01)                             */
/* =======================================================  GTUDDTYC  ======================================================== */
#define R_GPT0_GTUDDTYC_OBDTYR_Pos        (27UL)         /*!< OBDTYR (Bit 27)                                       */
#define R_GPT0_GTUDDTYC_OBDTYR_Msk        (0x8000000UL)  /*!< OBDTYR (Bitfield-Mask: 0x01)                          */
#define R_GPT0_GTUDDTYC_OBDTYF_Pos        (26UL)         /*!< OBDTYF (Bit 26)                                       */
#define R_GPT0_GTUDDTYC_OBDTYF_Msk        (0x4000000UL)  /*!< OBDTYF (Bitfield-Mask: 0x01)                          */
#define R_GPT0_GTUDDTYC_OBDTY_Pos         (24UL)         /*!< OBDTY (Bit 24)                                        */
#define R_GPT0_GTUDDTYC_OBDTY_Msk         (0x3000000UL)  /*!< OBDTY (Bitfield-Mask: 0x03)                           */
#define R_GPT0_GTUDDTYC_OADTYR_Pos        (19UL)         /*!< OADTYR (Bit 19)                                       */
#define R_GPT0_GTUDDTYC_OADTYR_Msk        (0x80000UL)    /*!< OADTYR (Bitfield-Mask: 0x01)                          */
#define R_GPT0_GTUDDTYC_OADTYF_Pos        (18UL)         /*!< OADTYF (Bit 18)                                       */
#define R_GPT0_GTUDDTYC_OADTYF_Msk        (0x40000UL)    /*!< OADTYF (Bitfield-Mask: 0x01)                          */
#define R_GPT0_GTUDDTYC_OADTY_Pos         (16UL)         /*!< OADTY (Bit 16)                                        */
#define R_GPT0_GTUDDTYC_OADTY_Msk         (0x30000UL)    /*!< OADTY (Bitfield-Mask: 0x03)                           */
#define R_GPT0_GTUDDTYC_UDF_Pos           (1UL)          /*!< UDF (Bit 1)                                           */
#define R_GPT0_GTUDDTYC_UDF_Msk           (0x2UL)        /*!< UDF (Bitfield-Mask: 0x01)                             */
#define R_GPT0_GTUDDTYC_UD_Pos            (0UL)          /*!< UD (Bit 0)                                            */
#define R_GPT0_GTUDDTYC_UD_Msk            (0x1UL)        /*!< UD (Bitfield-Mask: 0x01)                              */
/* =========================================================  GTIOR  ========================================================= */
#define R_GPT0_GTIOR_NFCSB_Pos            (30UL)         /*!< NFCSB (Bit 30)                                        */
#define R_GPT0_GTIOR_NFCSB_Msk            (0xc0000000UL) /*!< NFCSB (Bitfield-Mask: 0x03)                           */
#define R_GPT0_GTIOR_NFBEN_Pos            (29UL)         /*!< NFBEN (Bit 29)                                        */
#define R_GPT0_GTIOR_NFBEN_Msk            (0x20000000UL) /*!< NFBEN (Bitfield-Mask: 0x01)                           */
#define R_GPT0_GTIOR_OBDF_Pos             (25UL)         /*!< OBDF (Bit 25)                                         */
#define R_GPT0_GTIOR_OBDF_Msk             (0x6000000UL)  /*!< OBDF (Bitfield-Mask: 0x03)                            */
#define R_GPT0_GTIOR_OBE_Pos              (24UL)         /*!< OBE (Bit 24)                                          */
#define R_GPT0_GTIOR_OBE_Msk              (0x1000000UL)  /*!< OBE (Bitfield-Mask: 0x01)                             */
#define R_GPT0_GTIOR_OBHLD_Pos            (23UL)         /*!< OBHLD (Bit 23)                                        */
#define R_GPT0_GTIOR_OBHLD_Msk            (0x800000UL)   /*!< OBHLD (Bitfield-Mask: 0x01)                           */
#define R_GPT0_GTIOR_OBDFLT_Pos           (22UL)         /*!< OBDFLT (Bit 22)                                       */
#define R_GPT0_GTIOR_OBDFLT_Msk           (0x400000UL)   /*!< OBDFLT (Bitfield-Mask: 0x01)                          */
#define R_GPT0_GTIOR_GTIOB_Pos            (16UL)         /*!< GTIOB (Bit 16)                                        */
#define R_GPT0_GTIOR_GTIOB_Msk            (0x1f0000UL)   /*!< GTIOB (Bitfield-Mask: 0x1f)                           */
#define R_GPT0_GTIOR_NFCSA_Pos            (14UL)         /*!< NFCSA (Bit 14)                                        */
#define R_GPT0_GTIOR_NFCSA_Msk            (0xc000UL)     /*!< NFCSA (Bitfield-Mask: 0x03)                           */
#define R_GPT0_GTIOR_NFAEN_Pos            (13UL)         /*!< NFAEN (Bit 13)                                        */
#define R_GPT0_GTIOR_NFAEN_Msk            (0x2000UL)     /*!< NFAEN (Bitfield-Mask: 0x01)                           */
#define R_GPT0_GTIOR_OADF_Pos             (9UL)          /*!< OADF (Bit 9)                                          */
#define R_GPT0_GTIOR_OADF_Msk             (0x600UL)      /*!< OADF (Bitfield-Mask: 0x03)                            */
#define R_GPT0_GTIOR_OAE_Pos              (8UL)          /*!< OAE (Bit 8)                                           */
#define R_GPT0_GTIOR_OAE_Msk              (0x100UL)      /*!< OAE (Bitfield-Mask: 0x01)                             */
#define R_GPT0_GTIOR_OAHLD_Pos            (7UL)          /*!< OAHLD (Bit 7)                                         */
#define R_GPT0_GTIOR_OAHLD_Msk            (0x80UL)       /*!< OAHLD (Bitfield-Mask: 0x01)                           */
#define R_GPT0_GTIOR_OADFLT_Pos           (6UL)          /*!< OADFLT (Bit 6)                                        */
#define R_GPT0_GTIOR_OADFLT_Msk           (0x40UL)       /*!< OADFLT (Bitfield-Mask: 0x01)                          */
#define R_GPT0_GTIOR_GTIOA_Pos            (0UL)          /*!< GTIOA (Bit 0)                                         */
#define R_GPT0_GTIOR_GTIOA_Msk            (0x1fUL)       /*!< GTIOA (Bitfield-Mask: 0x1f)                           */
/* ========================================================  GTINTAD  ======================================================== */
#define R_GPT0_GTINTAD_GRPABL_Pos         (30UL)         /*!< GRPABL (Bit 30)                                       */
#define R_GPT0_GTINTAD_GRPABL_Msk         (0x40000000UL) /*!< GRPABL (Bitfield-Mask: 0x01)                          */
#define R_GPT0_GTINTAD_GRPABH_Pos         (29UL)         /*!< GRPABH (Bit 29)                                       */
#define R_GPT0_GTINTAD_GRPABH_Msk         (0x20000000UL) /*!< GRPABH (Bitfield-Mask: 0x01)                          */
#define R_GPT0_GTINTAD_GRPDTE_Pos         (28UL)         /*!< GRPDTE (Bit 28)                                       */
#define R_GPT0_GTINTAD_GRPDTE_Msk         (0x10000000UL) /*!< GRPDTE (Bitfield-Mask: 0x01)                          */
#define R_GPT0_GTINTAD_GRP_Pos            (24UL)         /*!< GRP (Bit 24)                                          */
#define R_GPT0_GTINTAD_GRP_Msk            (0x3000000UL)  /*!< GRP (Bitfield-Mask: 0x03)                             */
#define R_GPT0_GTINTAD_ADTRBDEN_Pos       (19UL)         /*!< ADTRBDEN (Bit 19)                                     */
#define R_GPT0_GTINTAD_ADTRBDEN_Msk       (0x80000UL)    /*!< ADTRBDEN (Bitfield-Mask: 0x01)                        */
#define R_GPT0_GTINTAD_ADTRBUEN_Pos       (18UL)         /*!< ADTRBUEN (Bit 18)                                     */
#define R_GPT0_GTINTAD_ADTRBUEN_Msk       (0x40000UL)    /*!< ADTRBUEN (Bitfield-Mask: 0x01)                        */
#define R_GPT0_GTINTAD_ADTRADEN_Pos       (17UL)         /*!< ADTRADEN (Bit 17)                                     */
#define R_GPT0_GTINTAD_ADTRADEN_Msk       (0x20000UL)    /*!< ADTRADEN (Bitfield-Mask: 0x01)                        */
#define R_GPT0_GTINTAD_ADTRAUEN_Pos       (16UL)         /*!< ADTRAUEN (Bit 16)                                     */
#define R_GPT0_GTINTAD_ADTRAUEN_Msk       (0x10000UL)    /*!< ADTRAUEN (Bitfield-Mask: 0x01)                        */
#define R_GPT0_GTINTAD_GTINTPR_Pos        (6UL)          /*!< GTINTPR (Bit 6)                                       */
#define R_GPT0_GTINTAD_GTINTPR_Msk        (0xc0UL)       /*!< GTINTPR (Bitfield-Mask: 0x03)                         */
#define R_GPT0_GTINTAD_GTINTF_Pos         (5UL)          /*!< GTINTF (Bit 5)                                        */
#define R_GPT0_GTINTAD_GTINTF_Msk         (0x20UL)       /*!< GTINTF (Bitfield-Mask: 0x01)                          */
#define R_GPT0_GTINTAD_GTINTE_Pos         (4UL)          /*!< GTINTE (Bit 4)                                        */
#define R_GPT0_GTINTAD_GTINTE_Msk         (0x10UL)       /*!< GTINTE (Bitfield-Mask: 0x01)                          */
#define R_GPT0_GTINTAD_GTINTD_Pos         (3UL)          /*!< GTINTD (Bit 3)                                        */
#define R_GPT0_GTINTAD_GTINTD_Msk         (0x8UL)        /*!< GTINTD (Bitfield-Mask: 0x01)                          */
#define R_GPT0_GTINTAD_GTINTC_Pos         (2UL)          /*!< GTINTC (Bit 2)                                        */
#define R_GPT0_GTINTAD_GTINTC_Msk         (0x4UL)        /*!< GTINTC (Bitfield-Mask: 0x01)                          */
#define R_GPT0_GTINTAD_GTINTB_Pos         (1UL)          /*!< GTINTB (Bit 1)                                        */
#define R_GPT0_GTINTAD_GTINTB_Msk         (0x2UL)        /*!< GTINTB (Bitfield-Mask: 0x01)                          */
#define R_GPT0_GTINTAD_GTINTA_Pos         (0UL)          /*!< GTINTA (Bit 0)                                        */
#define R_GPT0_GTINTAD_GTINTA_Msk         (0x1UL)        /*!< GTINTA (Bitfield-Mask: 0x01)                          */
/* =========================================================  GTST  ========================================================== */
#define R_GPT0_GTST_OABLF_Pos             (30UL)         /*!< OABLF (Bit 30)                                        */
#define R_GPT0_GTST_OABLF_Msk             (0x40000000UL) /*!< OABLF (Bitfield-Mask: 0x01)                           */
#define R_GPT0_GTST_OABHF_Pos             (29UL)         /*!< OABHF (Bit 29)                                        */
#define R_GPT0_GTST_OABHF_Msk             (0x20000000UL) /*!< OABHF (Bitfield-Mask: 0x01)                           */
#define R_GPT0_GTST_DTEF_Pos              (28UL)         /*!< DTEF (Bit 28)                                         */
#define R_GPT0_GTST_DTEF_Msk              (0x10000000UL) /*!< DTEF (Bitfield-Mask: 0x01)                            */
#define R_GPT0_GTST_ODF_Pos               (24UL)         /*!< ODF (Bit 24)                                          */
#define R_GPT0_GTST_ODF_Msk               (0x1000000UL)  /*!< ODF (Bitfield-Mask: 0x01)                             */
#define R_GPT0_GTST_ADTRBDF_Pos           (19UL)         /*!< ADTRBDF (Bit 19)                                      */
#define R_GPT0_GTST_ADTRBDF_Msk           (0x80000UL)    /*!< ADTRBDF (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTST_ADTRBUF_Pos           (18UL)         /*!< ADTRBUF (Bit 18)                                      */
#define R_GPT0_GTST_ADTRBUF_Msk           (0x40000UL)    /*!< ADTRBUF (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTST_ADTRADF_Pos           (17UL)         /*!< ADTRADF (Bit 17)                                      */
#define R_GPT0_GTST_ADTRADF_Msk           (0x20000UL)    /*!< ADTRADF (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTST_ADTRAUF_Pos           (16UL)         /*!< ADTRAUF (Bit 16)                                      */
#define R_GPT0_GTST_ADTRAUF_Msk           (0x10000UL)    /*!< ADTRAUF (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTST_TUCF_Pos              (15UL)         /*!< TUCF (Bit 15)                                         */
#define R_GPT0_GTST_TUCF_Msk              (0x8000UL)     /*!< TUCF (Bitfield-Mask: 0x01)                            */
#define R_GPT0_GTST_ITCNT_Pos             (8UL)          /*!< ITCNT (Bit 8)                                         */
#define R_GPT0_GTST_ITCNT_Msk             (0x700UL)      /*!< ITCNT (Bitfield-Mask: 0x07)                           */
/* =========================================================  GTBER  ========================================================= */
#define R_GPT0_GTBER_ADTDB_Pos            (30UL)         /*!< ADTDB (Bit 30)                                        */
#define R_GPT0_GTBER_ADTDB_Msk            (0x40000000UL) /*!< ADTDB (Bitfield-Mask: 0x01)                           */
#define R_GPT0_GTBER_ADTTB_Pos            (28UL)         /*!< ADTTB (Bit 28)                                        */
#define R_GPT0_GTBER_ADTTB_Msk            (0x30000000UL) /*!< ADTTB (Bitfield-Mask: 0x03)                           */
#define R_GPT0_GTBER_ADTDA_Pos            (26UL)         /*!< ADTDA (Bit 26)                                        */
#define R_GPT0_GTBER_ADTDA_Msk            (0x4000000UL)  /*!< ADTDA (Bitfield-Mask: 0x01)                           */
#define R_GPT0_GTBER_ADTTA_Pos            (24UL)         /*!< ADTTA (Bit 24)                                        */
#define R_GPT0_GTBER_ADTTA_Msk            (0x3000000UL)  /*!< ADTTA (Bitfield-Mask: 0x03)                           */
#define R_GPT0_GTBER_CCRSWT_Pos           (22UL)         /*!< CCRSWT (Bit 22)                                       */
#define R_GPT0_GTBER_CCRSWT_Msk           (0x400000UL)   /*!< CCRSWT (Bitfield-Mask: 0x01)                          */
#define R_GPT0_GTBER_PR_Pos               (20UL)         /*!< PR (Bit 20)                                           */
#define R_GPT0_GTBER_PR_Msk               (0x300000UL)   /*!< PR (Bitfield-Mask: 0x03)                              */
#define R_GPT0_GTBER_CCRB_Pos             (18UL)         /*!< CCRB (Bit 18)                                         */
#define R_GPT0_GTBER_CCRB_Msk             (0xc0000UL)    /*!< CCRB (Bitfield-Mask: 0x03)                            */
#define R_GPT0_GTBER_CCRA_Pos             (16UL)         /*!< CCRA (Bit 16)                                         */
#define R_GPT0_GTBER_CCRA_Msk             (0x30000UL)    /*!< CCRA (Bitfield-Mask: 0x03)                            */
#define R_GPT0_GTBER_DBRTECB_Pos          (10UL)         /*!< DBRTECB (Bit 10)                                      */
#define R_GPT0_GTBER_DBRTECB_Msk          (0x400UL)      /*!< DBRTECB (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTBER_DBRTECA_Pos          (8UL)          /*!< DBRTECA (Bit 8)                                       */
#define R_GPT0_GTBER_DBRTECA_Msk          (0x100UL)      /*!< DBRTECA (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTBER_BD3_Pos              (3UL)          /*!< BD3 (Bit 3)                                           */
#define R_GPT0_GTBER_BD3_Msk              (0x8UL)        /*!< BD3 (Bitfield-Mask: 0x01)                             */
#define R_GPT0_GTBER_BD2_Pos              (2UL)          /*!< BD2 (Bit 2)                                           */
#define R_GPT0_GTBER_BD2_Msk              (0x4UL)        /*!< BD2 (Bitfield-Mask: 0x01)                             */
#define R_GPT0_GTBER_BD1_Pos              (1UL)          /*!< BD1 (Bit 1)                                           */
#define R_GPT0_GTBER_BD1_Msk              (0x2UL)        /*!< BD1 (Bitfield-Mask: 0x01)                             */
#define R_GPT0_GTBER_BD0_Pos              (0UL)          /*!< BD0 (Bit 0)                                           */
#define R_GPT0_GTBER_BD0_Msk              (0x1UL)        /*!< BD0 (Bitfield-Mask: 0x01)                             */
/* =========================================================  GTITC  ========================================================= */
#define R_GPT0_GTITC_ADTBL_Pos            (14UL)         /*!< ADTBL (Bit 14)                                        */
#define R_GPT0_GTITC_ADTBL_Msk            (0x4000UL)     /*!< ADTBL (Bitfield-Mask: 0x01)                           */
#define R_GPT0_GTITC_ADTAL_Pos            (12UL)         /*!< ADTAL (Bit 12)                                        */
#define R_GPT0_GTITC_ADTAL_Msk            (0x1000UL)     /*!< ADTAL (Bitfield-Mask: 0x01)                           */
#define R_GPT0_GTITC_IVTT_Pos             (8UL)          /*!< IVTT (Bit 8)                                          */
#define R_GPT0_GTITC_IVTT_Msk             (0x700UL)      /*!< IVTT (Bitfield-Mask: 0x07)                            */
#define R_GPT0_GTITC_IVTC_Pos             (6UL)          /*!< IVTC (Bit 6)                                          */
#define R_GPT0_GTITC_IVTC_Msk             (0xc0UL)       /*!< IVTC (Bitfield-Mask: 0x03)                            */
#define R_GPT0_GTITC_ITLF_Pos             (5UL)          /*!< ITLF (Bit 5)                                          */
#define R_GPT0_GTITC_ITLF_Msk             (0x20UL)       /*!< ITLF (Bitfield-Mask: 0x01)                            */
#define R_GPT0_GTITC_ITLE_Pos             (4UL)          /*!< ITLE (Bit 4)                                          */
#define R_GPT0_GTITC_ITLE_Msk             (0x10UL)       /*!< ITLE (Bitfield-Mask: 0x01)                            */
#define R_GPT0_GTITC_ITLD_Pos             (3UL)          /*!< ITLD (Bit 3)                                          */
#define R_GPT0_GTITC_ITLD_Msk             (0x8UL)        /*!< ITLD (Bitfield-Mask: 0x01)                            */
#define R_GPT0_GTITC_ITLC_Pos             (2UL)          /*!< ITLC (Bit 2)                                          */
#define R_GPT0_GTITC_ITLC_Msk             (0x4UL)        /*!< ITLC (Bitfield-Mask: 0x01)                            */
#define R_GPT0_GTITC_ITLB_Pos             (1UL)          /*!< ITLB (Bit 1)                                          */
#define R_GPT0_GTITC_ITLB_Msk             (0x2UL)        /*!< ITLB (Bitfield-Mask: 0x01)                            */
#define R_GPT0_GTITC_ITLA_Pos             (0UL)          /*!< ITLA (Bit 0)                                          */
#define R_GPT0_GTITC_ITLA_Msk             (0x1UL)        /*!< ITLA (Bitfield-Mask: 0x01)                            */
/* =========================================================  GTCNT  ========================================================= */
#define R_GPT0_GTCNT_GTCNT_Pos            (0UL)          /*!< GTCNT (Bit 0)                                         */
#define R_GPT0_GTCNT_GTCNT_Msk            (0xffffffffUL) /*!< GTCNT (Bitfield-Mask: 0xffffffff)                     */
/* =========================================================  GTCCR  ========================================================= */
#define R_GPT0_GTCCR_GTCCR_Pos            (0UL)          /*!< GTCCR (Bit 0)                                         */
#define R_GPT0_GTCCR_GTCCR_Msk            (0xffffffffUL) /*!< GTCCR (Bitfield-Mask: 0xffffffff)                     */
/* =========================================================  GTPR  ========================================================== */
#define R_GPT0_GTPR_GTPR_Pos              (0UL)          /*!< GTPR (Bit 0)                                          */
#define R_GPT0_GTPR_GTPR_Msk              (0xffffffffUL) /*!< GTPR (Bitfield-Mask: 0xffffffff)                      */
/* =========================================================  GTPBR  ========================================================= */
#define R_GPT0_GTPBR_GTPBR_Pos            (0UL)          /*!< GTPBR (Bit 0)                                         */
#define R_GPT0_GTPBR_GTPBR_Msk            (0xffffffffUL) /*!< GTPBR (Bitfield-Mask: 0xffffffff)                     */
/* ========================================================  GTPDBR  ========================================================= */
#define R_GPT0_GTPDBR_GTPDBR_Pos          (0UL)          /*!< GTPDBR (Bit 0)                                        */
#define R_GPT0_GTPDBR_GTPDBR_Msk          (0xffffffffUL) /*!< GTPDBR (Bitfield-Mask: 0xffffffff)                    */
/* ========================================================  GTADTRA  ======================================================== */
#define R_GPT0_GTADTRA_GTADTRA_Pos        (0UL)          /*!< GTADTRA (Bit 0)                                       */
#define R_GPT0_GTADTRA_GTADTRA_Msk        (0xffffffffUL) /*!< GTADTRA (Bitfield-Mask: 0xffffffff)                   */
/* ========================================================  GTADTRB  ======================================================== */
#define R_GPT0_GTADTRB_GTADTRB_Pos        (0UL)          /*!< GTADTRB (Bit 0)                                       */
#define R_GPT0_GTADTRB_GTADTRB_Msk        (0xffffffffUL) /*!< GTADTRB (Bitfield-Mask: 0xffffffff)                   */
/* =======================================================  GTADTBRA  ======================================================== */
#define R_GPT0_GTADTBRA_GTADTBRA_Pos      (0UL)          /*!< GTADTBRA (Bit 0)                                      */
#define R_GPT0_GTADTBRA_GTADTBRA_Msk      (0xffffffffUL) /*!< GTADTBRA (Bitfield-Mask: 0xffffffff)                  */
/* =======================================================  GTADTBRB  ======================================================== */
#define R_GPT0_GTADTBRB_GTADTBRB_Pos      (0UL)          /*!< GTADTBRB (Bit 0)                                      */
#define R_GPT0_GTADTBRB_GTADTBRB_Msk      (0xffffffffUL) /*!< GTADTBRB (Bitfield-Mask: 0xffffffff)                  */
/* =======================================================  GTADTDBRA  ======================================================= */
#define R_GPT0_GTADTDBRA_GTADTDBRA_Pos    (0UL)          /*!< GTADTDBRA (Bit 0)                                     */
#define R_GPT0_GTADTDBRA_GTADTDBRA_Msk    (0xffffffffUL) /*!< GTADTDBRA (Bitfield-Mask: 0xffffffff)                 */
/* =======================================================  GTADTDBRB  ======================================================= */
#define R_GPT0_GTADTDBRB_GTADTDBRB_Pos    (0UL)          /*!< GTADTDBRB (Bit 0)                                     */
#define R_GPT0_GTADTDBRB_GTADTDBRB_Msk    (0xffffffffUL) /*!< GTADTDBRB (Bitfield-Mask: 0xffffffff)                 */
/* ========================================================  GTDTCR  ========================================================= */
#define R_GPT0_GTDTCR_TDFER_Pos           (8UL)          /*!< TDFER (Bit 8)                                         */
#define R_GPT0_GTDTCR_TDFER_Msk           (0x100UL)      /*!< TDFER (Bitfield-Mask: 0x01)                           */
#define R_GPT0_GTDTCR_TDBDE_Pos           (5UL)          /*!< TDBDE (Bit 5)                                         */
#define R_GPT0_GTDTCR_TDBDE_Msk           (0x20UL)       /*!< TDBDE (Bitfield-Mask: 0x01)                           */
#define R_GPT0_GTDTCR_TDBUE_Pos           (4UL)          /*!< TDBUE (Bit 4)                                         */
#define R_GPT0_GTDTCR_TDBUE_Msk           (0x10UL)       /*!< TDBUE (Bitfield-Mask: 0x01)                           */
/* =========================================================  GTDVU  ========================================================= */
#define R_GPT0_GTDVU_GTDVU_Pos            (0UL)          /*!< GTDVU (Bit 0)                                         */
#define R_GPT0_GTDVU_GTDVU_Msk            (0xffffffffUL) /*!< GTDVU (Bitfield-Mask: 0xffffffff)                     */
/* =========================================================  GTDVD  ========================================================= */
#define R_GPT0_GTDVD_GTDVD_Pos            (0UL)          /*!< GTDVD (Bit 0)                                         */
#define R_GPT0_GTDVD_GTDVD_Msk            (0xffffffffUL) /*!< GTDVD (Bitfield-Mask: 0xffffffff)                     */
/* =========================================================  GTDBU  ========================================================= */
#define R_GPT0_GTDBU_GTDBU_Pos            (0UL)          /*!< GTDBU (Bit 0)                                         */
#define R_GPT0_GTDBU_GTDBU_Msk            (0xffffffffUL) /*!< GTDBU (Bitfield-Mask: 0xffffffff)                     */
/* =========================================================  GTDBD  ========================================================= */
#define R_GPT0_GTDBD_GTDBD_Pos            (0UL)          /*!< GTDBD (Bit 0)                                         */
#define R_GPT0_GTDBD_GTDBD_Msk            (0xffffffffUL) /*!< GTDBD (Bitfield-Mask: 0xffffffff)                     */
/* =========================================================  GTSOS  ========================================================= */
#define R_GPT0_GTSOS_SOS_Pos              (0UL)          /*!< SOS (Bit 0)                                           */
#define R_GPT0_GTSOS_SOS_Msk              (0x3UL)        /*!< SOS (Bitfield-Mask: 0x03)                             */
/* ========================================================  GTSOTR  ========================================================= */
#define R_GPT0_GTSOTR_SOTR_Pos            (0UL)          /*!< SOTR (Bit 0)                                          */
#define R_GPT0_GTSOTR_SOTR_Msk            (0x1UL)        /*!< SOTR (Bitfield-Mask: 0x01)                            */
/* ========================================================  GTADSMR  ======================================================== */
#define R_GPT0_GTADSMR_ADSMEN1_Pos        (24UL)         /*!< ADSMEN1 (Bit 24)                                      */
#define R_GPT0_GTADSMR_ADSMEN1_Msk        (0x1000000UL)  /*!< ADSMEN1 (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTADSMR_ADSMS1_Pos         (16UL)         /*!< ADSMS1 (Bit 16)                                       */
#define R_GPT0_GTADSMR_ADSMS1_Msk         (0x30000UL)    /*!< ADSMS1 (Bitfield-Mask: 0x03)                          */
#define R_GPT0_GTADSMR_ADSMEN0_Pos        (8UL)          /*!< ADSMEN0 (Bit 8)                                       */
#define R_GPT0_GTADSMR_ADSMEN0_Msk        (0x100UL)      /*!< ADSMEN0 (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTADSMR_ADSMS0_Pos         (0UL)          /*!< ADSMS0 (Bit 0)                                        */
#define R_GPT0_GTADSMR_ADSMS0_Msk         (0x3UL)        /*!< ADSMS0 (Bitfield-Mask: 0x03)                          */
/* ========================================================  GTEITC  ========================================================= */
#define R_GPT0_GTEITC_EITCNT2_Pos         (28UL)         /*!< EITCNT2 (Bit 28)                                      */
#define R_GPT0_GTEITC_EITCNT2_Msk         (0xf0000000UL) /*!< EITCNT2 (Bitfield-Mask: 0x0f)                         */
#define R_GPT0_GTEITC_EITCNT2IV_Pos       (24UL)         /*!< EITCNT2IV (Bit 24)                                    */
#define R_GPT0_GTEITC_EITCNT2IV_Msk       (0xf000000UL)  /*!< EITCNT2IV (Bitfield-Mask: 0x0f)                       */
#define R_GPT0_GTEITC_EIVTT2_Pos          (20UL)         /*!< EIVTT2 (Bit 20)                                       */
#define R_GPT0_GTEITC_EIVTT2_Msk          (0xf00000UL)   /*!< EIVTT2 (Bitfield-Mask: 0x0f)                          */
#define R_GPT0_GTEITC_EIVTC2_Pos          (16UL)         /*!< EIVTC2 (Bit 16)                                       */
#define R_GPT0_GTEITC_EIVTC2_Msk          (0x30000UL)    /*!< EIVTC2 (Bitfield-Mask: 0x03)                          */
#define R_GPT0_GTEITC_EITCNT1_Pos         (12UL)         /*!< EITCNT1 (Bit 12)                                      */
#define R_GPT0_GTEITC_EITCNT1_Msk         (0xf000UL)     /*!< EITCNT1 (Bitfield-Mask: 0x0f)                         */
#define R_GPT0_GTEITC_EIVTT1_Pos          (4UL)          /*!< EIVTT1 (Bit 4)                                        */
#define R_GPT0_GTEITC_EIVTT1_Msk          (0xf0UL)       /*!< EIVTT1 (Bitfield-Mask: 0x0f)                          */
#define R_GPT0_GTEITC_EIVTC1_Pos          (0UL)          /*!< EIVTC1 (Bit 0)                                        */
#define R_GPT0_GTEITC_EIVTC1_Msk          (0x3UL)        /*!< EIVTC1 (Bitfield-Mask: 0x03)                          */
/* =======================================================  GTEITLI1  ======================================================== */
#define R_GPT0_GTEITLI1_EIVTT1_Pos        (28UL)         /*!< EIVTT1 (Bit 28)                                       */
#define R_GPT0_GTEITLI1_EIVTT1_Msk        (0x70000000UL) /*!< EIVTT1 (Bitfield-Mask: 0x07)                          */
#define R_GPT0_GTEITLI1_EIVTC1_Pos        (24UL)         /*!< EIVTC1 (Bit 24)                                       */
#define R_GPT0_GTEITLI1_EIVTC1_Msk        (0x7000000UL)  /*!< EIVTC1 (Bitfield-Mask: 0x07)                          */
/* =======================================================  GTEITLI2  ======================================================== */
#define R_GPT0_GTEITLI2_EADTBL_Pos        (4UL)          /*!< EADTBL (Bit 4)                                        */
#define R_GPT0_GTEITLI2_EADTBL_Msk        (0x70UL)       /*!< EADTBL (Bitfield-Mask: 0x07)                          */
#define R_GPT0_GTEITLI2_EADTAL_Pos        (0UL)          /*!< EADTAL (Bit 0)                                        */
#define R_GPT0_GTEITLI2_EADTAL_Msk        (0x7UL)        /*!< EADTAL (Bitfield-Mask: 0x07)                          */
/* ========================================================  GTEITLB  ======================================================== */
#define R_GPT0_GTEITLB_EBTLDVD_Pos        (28UL)         /*!< EBTLDVD (Bit 28)                                      */
#define R_GPT0_GTEITLB_EBTLDVD_Msk        (0x70000000UL) /*!< EBTLDVD (Bitfield-Mask: 0x07)                         */
#define R_GPT0_GTEITLB_EBTLDVU_Pos        (24UL)         /*!< EBTLDVU (Bit 24)                                      */
#define R_GPT0_GTEITLB_EBTLDVU_Msk        (0x7000000UL)  /*!< EBTLDVU (Bitfield-Mask: 0x07)                         */
#define R_GPT0_GTEITLB_EBTLADB_Pos        (20UL)         /*!< EBTLADB (Bit 20)                                      */
#define R_GPT0_GTEITLB_EBTLADB_Msk        (0x700000UL)   /*!< EBTLADB (Bitfield-Mask: 0x07)                         */
#define R_GPT0_GTEITLB_EBTLADA_Pos        (16UL)         /*!< EBTLADA (Bit 16)                                      */
#define R_GPT0_GTEITLB_EBTLADA_Msk        (0x70000UL)    /*!< EBTLADA (Bitfield-Mask: 0x07)                         */
#define R_GPT0_GTEITLB_EBTLPR_Pos         (8UL)          /*!< EBTLPR (Bit 8)                                        */
#define R_GPT0_GTEITLB_EBTLPR_Msk         (0x700UL)      /*!< EBTLPR (Bitfield-Mask: 0x07)                          */
#define R_GPT0_GTEITLB_EBTLCB_Pos         (4UL)          /*!< EBTLCB (Bit 4)                                        */
#define R_GPT0_GTEITLB_EBTLCB_Msk         (0x70UL)       /*!< EBTLCB (Bitfield-Mask: 0x07)                          */
#define R_GPT0_GTEITLB_EBTLCA_Pos         (0UL)          /*!< EBTLCA (Bit 0)                                        */
#define R_GPT0_GTEITLB_EBTLCA_Msk         (0x7UL)        /*!< EBTLCA (Bitfield-Mask: 0x07)                          */
/* ========================================================  GTSECSR  ======================================================== */
#define R_GPT0_GTSECSR_SECSEL3_Pos        (3UL)          /*!< SECSEL3 (Bit 3)                                       */
#define R_GPT0_GTSECSR_SECSEL3_Msk        (0x8UL)        /*!< SECSEL3 (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTSECSR_SECSEL2_Pos        (2UL)          /*!< SECSEL2 (Bit 2)                                       */
#define R_GPT0_GTSECSR_SECSEL2_Msk        (0x4UL)        /*!< SECSEL2 (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTSECSR_SECSEL1_Pos        (1UL)          /*!< SECSEL1 (Bit 1)                                       */
#define R_GPT0_GTSECSR_SECSEL1_Msk        (0x2UL)        /*!< SECSEL1 (Bitfield-Mask: 0x01)                         */
#define R_GPT0_GTSECSR_SECSEL0_Pos        (0UL)          /*!< SECSEL0 (Bit 0)                                       */
#define R_GPT0_GTSECSR_SECSEL0_Msk        (0x1UL)        /*!< SECSEL0 (Bitfield-Mask: 0x01)                         */
/* ========================================================  GTSECR  ========================================================= */
#define R_GPT0_GTSECR_SBDDD_Pos           (11UL)         /*!< SBDDD (Bit 11)                                        */
#define R_GPT0_GTSECR_SBDDD_Msk           (0x800UL)      /*!< SBDDD (Bitfield-Mask: 0x01)                           */
#define R_GPT0_GTSECR_SBDAD_Pos           (10UL)         /*!< SBDAD (Bit 10)                                        */
#define R_GPT0_GTSECR_SBDAD_Msk           (0x400UL)      /*!< SBDAD (Bitfield-Mask: 0x01)                           */
#define R_GPT0_GTSECR_SBDPD_Pos           (9UL)          /*!< SBDPD (Bit 9)                                         */
#define R_GPT0_GTSECR_SBDPD_Msk           (0x200UL)      /*!< SBDPD (Bitfield-Mask: 0x01)                           */
#define R_GPT0_GTSECR_SBDCD_Pos           (8UL)          /*!< SBDCD (Bit 8)                                         */
#define R_GPT0_GTSECR_SBDCD_Msk           (0x100UL)      /*!< SBDCD (Bitfield-Mask: 0x01)                           */
#define R_GPT0_GTSECR_SBDDE_Pos           (3UL)          /*!< SBDDE (Bit 3)                                         */
#define R_GPT0_GTSECR_SBDDE_Msk           (0x8UL)        /*!< SBDDE (Bitfield-Mask: 0x01)                           */
#define R_GPT0_GTSECR_SBDAE_Pos           (2UL)          /*!< SBDAE (Bit 2)                                         */
#define R_GPT0_GTSECR_SBDAE_Msk           (0x4UL)        /*!< SBDAE (Bitfield-Mask: 0x01)                           */
#define R_GPT0_GTSECR_SBDCE_Pos           (0UL)          /*!< SBDCE (Bit 0)                                         */
#define R_GPT0_GTSECR_SBDCE_Msk           (0x1UL)        /*!< SBDCE (Bitfield-Mask: 0x01)                           */

#endif /* R_GPT_POSMSK_H */
