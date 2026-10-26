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

#ifndef R_MRAM_POSMSK_H
#define R_MRAM_POSMSK_H

/* =========================================================================================================================== */
/* ================                                          R_MRMS                                           ================ */
/* =========================================================================================================================== */

/* ========================================================  MRCPFB  ========================================================= */
#define R_MRMS_MRCPFB_MPFBEN_Pos         (0UL)          /*!< MPFBEN (Bit 0)                                        */
#define R_MRMS_MRCPFB_MPFBEN_Msk         (0x1UL)        /*!< MPFBEN (Bitfield-Mask: 0x01)                          */
/* ========================================================  MRCFREQ  ======================================================== */
#define R_MRMS_MRCFREQ_KEY_Pos           (24UL)         /*!< KEY (Bit 24)                                          */
#define R_MRMS_MRCFREQ_KEY_Msk           (0xff000000UL) /*!< KEY (Bitfield-Mask: 0xff)                             */
#define R_MRMS_MRCFREQ_MRCMHZ_Pos        (0UL)          /*!< MRCMHZ (Bit 0)                                        */
#define R_MRMS_MRCFREQ_MRCMHZ_Msk        (0x3ffUL)      /*!< MRCMHZ (Bitfield-Mask: 0x3ff)                         */
/* ========================================================  MREFREQ  ======================================================== */
#define R_MRMS_MREFREQ_KEY_Pos           (24UL)         /*!< KEY (Bit 24)                                          */
#define R_MRMS_MREFREQ_KEY_Msk           (0xff000000UL) /*!< KEY (Bitfield-Mask: 0xff)                             */
#define R_MRMS_MREFREQ_MREMHZ_Pos        (0UL)          /*!< MREMHZ (Bit 0)                                        */
#define R_MRMS_MREFREQ_MREMHZ_Msk        (0xffUL)       /*!< MREMHZ (Bitfield-Mask: 0xff)                          */
/* ========================================================  MRCDECC  ======================================================== */
#define R_MRMS_MRCDECC_KEY_Pos           (8UL)          /*!< KEY (Bit 8)                                           */
#define R_MRMS_MRCDECC_KEY_Msk           (0xff00UL)     /*!< KEY (Bitfield-Mask: 0xff)                             */
#define R_MRMS_MRCDECC_ECCSELC_Pos       (1UL)          /*!< ECCSELC (Bit 1)                                       */
#define R_MRMS_MRCDECC_ECCSELC_Msk       (0x2UL)        /*!< ECCSELC (Bitfield-Mask: 0x01)                         */
#define R_MRMS_MRCDECC_DECDISC_Pos       (0UL)          /*!< DECDISC (Bit 0)                                       */
#define R_MRMS_MRCDECC_DECDISC_Msk       (0x1UL)        /*!< DECDISC (Bitfield-Mask: 0x01)                         */
/* =======================================================  MRCRAEINT  ======================================================= */
#define R_MRMS_MRCRAEINT_INTENBTC_Pos    (1UL)          /*!< INTENBTC (Bit 1)                                      */
#define R_MRMS_MRCRAEINT_INTENBTC_Msk    (0x2UL)        /*!< INTENBTC (Bitfield-Mask: 0x01)                        */
#define R_MRMS_MRCRAEINT_INTENBDC_Pos    (0UL)          /*!< INTENBDC (Bit 0)                                      */
#define R_MRMS_MRCRAEINT_INTENBDC_Msk    (0x1UL)        /*!< INTENBDC (Bitfield-Mask: 0x01)                        */
/* ========================================================  MRCRAES  ======================================================== */
#define R_MRMS_MRCRAES_TEDERRC_Pos       (1UL)          /*!< TEDERRC (Bit 1)                                       */
#define R_MRMS_MRCRAES_TEDERRC_Msk       (0x2UL)        /*!< TEDERRC (Bitfield-Mask: 0x01)                         */
#define R_MRMS_MRCRAES_DECERRC_Pos       (0UL)          /*!< DECERRC (Bit 0)                                       */
#define R_MRMS_MRCRAES_DECERRC_Msk       (0x1UL)        /*!< DECERRC (Bitfield-Mask: 0x01)                         */
/* ========================================================  MRCRTEA  ======================================================== */
#define R_MRMS_MRCRTEA_MRCRTEA_Pos       (5UL)          /*!< MRCRTEA (Bit 5)                                       */
#define R_MRMS_MRCRTEA_MRCRTEA_Msk       (0xffffffe0UL) /*!< MRCRTEA (Bitfield-Mask: 0x7ffffff)                    */
/* ========================================================  MRCRDEA  ======================================================== */
#define R_MRMS_MRCRDEA_MRCRDEA_Pos       (5UL)          /*!< MRCRDEA (Bit 5)                                       */
#define R_MRMS_MRCRDEA_MRCRDEA_Msk       (0xffffffe0UL) /*!< MRCRDEA (Bitfield-Mask: 0x7ffffff)                    */
/* ========================================================  MREDECC  ======================================================== */
#define R_MRMS_MREDECC_KEY_Pos           (8UL)          /*!< KEY (Bit 8)                                           */
#define R_MRMS_MREDECC_KEY_Msk           (0xff00UL)     /*!< KEY (Bitfield-Mask: 0xff)                             */
#define R_MRMS_MREDECC_ECCSELE_Pos       (1UL)          /*!< ECCSELE (Bit 1)                                       */
#define R_MRMS_MREDECC_ECCSELE_Msk       (0x2UL)        /*!< ECCSELE (Bitfield-Mask: 0x01)                         */
#define R_MRMS_MREDECC_DECDISE_Pos       (0UL)          /*!< DECDISE (Bit 0)                                       */
#define R_MRMS_MREDECC_DECDISE_Msk       (0x1UL)        /*!< DECDISE (Bitfield-Mask: 0x01)                         */
/* =======================================================  MRERAEINT  ======================================================= */
#define R_MRMS_MRERAEINT_INTENBTE_Pos    (1UL)          /*!< INTENBTE (Bit 1)                                      */
#define R_MRMS_MRERAEINT_INTENBTE_Msk    (0x2UL)        /*!< INTENBTE (Bitfield-Mask: 0x01)                        */
#define R_MRMS_MRERAEINT_INTENBDE_Pos    (0UL)          /*!< INTENBDE (Bit 0)                                      */
#define R_MRMS_MRERAEINT_INTENBDE_Msk    (0x1UL)        /*!< INTENBDE (Bitfield-Mask: 0x01)                        */
/* ========================================================  MRERAES  ======================================================== */
#define R_MRMS_MRERAES_TEDERRE_Pos       (1UL)          /*!< TEDERRE (Bit 1)                                       */
#define R_MRMS_MRERAES_TEDERRE_Msk       (0x2UL)        /*!< TEDERRE (Bitfield-Mask: 0x01)                         */
#define R_MRMS_MRERAES_DECERRE_Pos       (0UL)          /*!< DECERRE (Bit 0)                                       */
#define R_MRMS_MRERAES_DECERRE_Msk       (0x1UL)        /*!< DECERRE (Bitfield-Mask: 0x01)                         */
/* ========================================================  MRERTEA  ======================================================== */
#define R_MRMS_MRERTEA_MRERTEA_Pos       (4UL)          /*!< MRERTEA (Bit 4)                                       */
#define R_MRMS_MRERTEA_MRERTEA_Msk       (0xfffffff0UL) /*!< MRERTEA (Bitfield-Mask: 0xfffffff)                    */
/* ========================================================  MRERDEA  ======================================================== */
#define R_MRMS_MRERDEA_MRERDEA_Pos       (4UL)          /*!< MRERDEA (Bit 4)                                       */
#define R_MRMS_MRERDEA_MRERDEA_Msk       (0xfffffff0UL) /*!< MRERDEA (Bitfield-Mask: 0xfffffff)                    */
/* =========================================================  MSAR  ========================================================== */
#define R_MRMS_MSAR_MRCPSEQSA_Pos        (15UL)         /*!< MRCPSEQSA (Bit 15)                                    */
#define R_MRMS_MSAR_MRCPSEQSA_Msk        (0x8000UL)     /*!< MRCPSEQSA (Bitfield-Mask: 0x01)                       */
#define R_MRMS_MSAR_MREPSEQSA_Pos        (14UL)         /*!< MREPSEQSA (Bit 14)                                    */
#define R_MRMS_MSAR_MREPSEQSA_Msk        (0x4000UL)     /*!< MREPSEQSA (Bitfield-Mask: 0x01)                       */
#define R_MRMS_MSAR_MRCPSA_Pos           (13UL)         /*!< MRCPSA (Bit 13)                                       */
#define R_MRMS_MSAR_MRCPSA_Msk           (0x2000UL)     /*!< MRCPSA (Bitfield-Mask: 0x01)                          */
#define R_MRMS_MSAR_MRDPSA_Pos           (12UL)         /*!< MRDPSA (Bit 12)                                       */
#define R_MRMS_MSAR_MRDPSA_Msk           (0x1000UL)     /*!< MRDPSA (Bitfield-Mask: 0x01)                          */
#define R_MRMS_MSAR_MACITRSA_Pos         (11UL)         /*!< MACITRSA (Bit 11)                                     */
#define R_MRMS_MSAR_MACITRSA_Msk         (0x800UL)      /*!< MACITRSA (Bitfield-Mask: 0x01)                        */
#define R_MRMS_MSAR_MACICMRSA_Pos        (10UL)         /*!< MACICMRSA (Bit 10)                                    */
#define R_MRMS_MSAR_MACICMRSA_Msk        (0x400UL)      /*!< MACICMRSA (Bitfield-Mask: 0x01)                       */
#define R_MRMS_MSAR_MACICMISA_Pos        (9UL)          /*!< MACICMISA (Bit 9)                                     */
#define R_MRMS_MSAR_MACICMISA_Msk        (0x200UL)      /*!< MACICMISA (Bitfield-Mask: 0x01)                       */
#define R_MRMS_MSAR_MPFBENSA_Pos         (4UL)          /*!< MPFBENSA (Bit 4)                                      */
#define R_MRMS_MSAR_MPFBENSA_Msk         (0x10UL)       /*!< MPFBENSA (Bitfield-Mask: 0x01)                        */
#define R_MRMS_MSAR_MRCFREQSA_Pos        (3UL)          /*!< MRCFREQSA (Bit 3)                                     */
#define R_MRMS_MSAR_MRCFREQSA_Msk        (0x8UL)        /*!< MRCFREQSA (Bitfield-Mask: 0x01)                       */
#define R_MRMS_MSAR_MRCECCSA_Pos         (2UL)          /*!< MRCECCSA (Bit 2)                                      */
#define R_MRMS_MSAR_MRCECCSA_Msk         (0x4UL)        /*!< MRCECCSA (Bitfield-Mask: 0x01)                        */
#define R_MRMS_MSAR_MREFREQSA_Pos        (1UL)          /*!< MREFREQSA (Bit 1)                                     */
#define R_MRMS_MSAR_MREFREQSA_Msk        (0x2UL)        /*!< MREFREQSA (Bitfield-Mask: 0x01)                       */
#define R_MRMS_MSAR_MREECCSA_Pos         (0UL)          /*!< MREECCSA (Bit 0)                                      */
#define R_MRMS_MSAR_MREECCSA_Msk         (0x1UL)        /*!< MREECCSA (Bitfield-Mask: 0x01)                        */
/* =========================================================  MREZS  ========================================================= */
#define R_MRMS_MREZS_WHUKEXE_Pos         (1UL)          /*!< WHUKEXE (Bit 1)                                       */
#define R_MRMS_MREZS_WHUKEXE_Msk         (0x2UL)        /*!< WHUKEXE (Bitfield-Mask: 0x01)                         */
#define R_MRMS_MREZS_WHUKZF_Pos          (0UL)          /*!< WHUKZF (Bit 0)                                        */
#define R_MRMS_MREZS_WHUKZF_Msk          (0x1UL)        /*!< WHUKZF (Bitfield-Mask: 0x01)                          */
/* =========================================================  MREZC  ========================================================= */
#define R_MRMS_MREZC_KEY_Pos             (8UL)          /*!< KEY (Bit 8)                                           */
#define R_MRMS_MREZC_KEY_Msk             (0xff00UL)     /*!< KEY (Bitfield-Mask: 0xff)                             */
#define R_MRMS_MREZC_WHUKZE_Pos          (0UL)          /*!< WHUKZE (Bit 0)                                        */
#define R_MRMS_MREZC_WHUKZE_Msk          (0x7UL)        /*!< WHUKZE (Bitfield-Mask: 0x07)                          */
/* =======================================================  MREASTAT  ======================================================== */
#define R_MRMS_MREASTAT_CMDLK_Pos        (4UL)          /*!< CMDLK (Bit 4)                                         */
#define R_MRMS_MREASTAT_CMDLK_Msk        (0x10UL)       /*!< CMDLK (Bitfield-Mask: 0x01)                           */
#define R_MRMS_MREASTAT_MREAE_Pos        (3UL)          /*!< MREAE (Bit 3)                                         */
#define R_MRMS_MREASTAT_MREAE_Msk        (0x8UL)        /*!< MREAE (Bitfield-Mask: 0x01)                           */
/* =======================================================  MREPAEINT  ======================================================= */
#define R_MRMS_MREPAEINT_CMDLKIE_Pos     (4UL)          /*!< CMDLKIE (Bit 4)                                       */
#define R_MRMS_MREPAEINT_CMDLKIE_Msk     (0x10UL)       /*!< CMDLKIE (Bitfield-Mask: 0x01)                         */
#define R_MRMS_MREPAEINT_MREAEIE_Pos     (3UL)          /*!< MREAEIE (Bit 3)                                       */
#define R_MRMS_MREPAEINT_MREAEIE_Msk     (0x8UL)        /*!< MREAEIE (Bitfield-Mask: 0x01)                         */
/* ========================================================  MRDYIE  ========================================================= */
#define R_MRMS_MRDYIE_MRDYIE_Pos         (0UL)          /*!< MRDYIE (Bit 0)                                        */
#define R_MRMS_MRDYIE_MRDYIE_Msk         (0x1UL)        /*!< MRDYIE (Bitfield-Mask: 0x01)                          */
/* =======================================================  MRESADDR  ======================================================== */
#define R_MRMS_MRESADDR_MSA_Pos          (4UL)          /*!< MSA (Bit 4)                                           */
#define R_MRMS_MRESADDR_MSA_Msk          (0xfffffff0UL) /*!< MSA (Bitfield-Mask: 0xfffffff)                        */
/* =======================================================  MCNTSELR  ======================================================== */
#define R_MRMS_MCNTSELR_CNTSEL_Pos       (0UL)          /*!< CNTSEL (Bit 0)                                        */
#define R_MRMS_MCNTSELR_CNTSEL_Msk       (0x7UL)        /*!< CNTSEL (Bitfield-Mask: 0x07)                          */
/* =======================================================  MCNTDATAR  ======================================================= */
#define R_MRMS_MCNTDATAR_CNTRDAT_Pos     (0UL)          /*!< CNTRDAT (Bit 0)                                       */
#define R_MRMS_MCNTDATAR_CNTRDAT_Msk     (0xffffffffUL) /*!< CNTRDAT (Bitfield-Mask: 0xffffffff)                   */
/* =======================================================  MCTRCNTR  ======================================================== */
#define R_MRMS_MCTRCNTR_KEY_Pos          (8UL)          /*!< KEY (Bit 8)                                           */
#define R_MRMS_MCTRCNTR_KEY_Msk          (0xff00UL)     /*!< KEY (Bitfield-Mask: 0xff)                             */
#define R_MRMS_MCTRCNTR_TRSEL_Pos        (1UL)          /*!< TRSEL (Bit 1)                                         */
#define R_MRMS_MCTRCNTR_TRSEL_Msk        (0x2UL)        /*!< TRSEL (Bitfield-Mask: 0x01)                           */
#define R_MRMS_MCTRCNTR_TRTRG_Pos        (0UL)          /*!< TRTRG (Bit 0)                                         */
#define R_MRMS_MCTRCNTR_TRTRG_Msk        (0x1UL)        /*!< TRTRG (Bitfield-Mask: 0x01)                           */
/* ========================================================  MCTRLSR  ======================================================== */
#define R_MRMS_MCTRLSR_TRLIST_Pos        (0UL)          /*!< TRLIST (Bit 0)                                        */
#define R_MRMS_MCTRLSR_TRLIST_Msk        (0x7UL)        /*!< TRLIST (Bitfield-Mask: 0x07)                          */
/* =======================================================  MCTRADDR  ======================================================== */
#define R_MRMS_MCTRADDR_CTRA_Pos         (0UL)          /*!< CTRA (Bit 0)                                          */
#define R_MRMS_MCTRADDR_CTRA_Msk         (0xffffffffUL) /*!< CTRA (Bitfield-Mask: 0xffffffff)                      */
/* =======================================================  MCTRSTATR  ======================================================= */
#define R_MRMS_MCTRSTATR_TRMD_Pos        (2UL)          /*!< TRMD (Bit 2)                                          */
#define R_MRMS_MCTRSTATR_TRMD_Msk        (0x4UL)        /*!< TRMD (Bitfield-Mask: 0x01)                            */
#define R_MRMS_MCTRSTATR_TRAD_Pos        (1UL)          /*!< TRAD (Bit 1)                                          */
#define R_MRMS_MCTRSTATR_TRAD_Msk        (0x2UL)        /*!< TRAD (Bitfield-Mask: 0x01)                            */
#define R_MRMS_MCTRSTATR_TRBUSY_Pos      (0UL)          /*!< TRBUSY (Bit 0)                                        */
#define R_MRMS_MCTRSTATR_TRBUSY_Msk      (0x1UL)        /*!< TRBUSY (Bitfield-Mask: 0x01)                          */
/* ========================================================  MSTATR  ========================================================= */
#define R_MRMS_MSTATR_ILGCOMERR_Pos      (23UL)         /*!< ILGCOMERR (Bit 23)                                    */
#define R_MRMS_MSTATR_ILGCOMERR_Msk      (0x800000UL)   /*!< ILGCOMERR (Bitfield-Mask: 0x01)                       */
#define R_MRMS_MSTATR_SECERR_Pos         (21UL)         /*!< SECERR (Bit 21)                                       */
#define R_MRMS_MSTATR_SECERR_Msk         (0x200000UL)   /*!< SECERR (Bitfield-Mask: 0x01)                          */
#define R_MRMS_MSTATR_OTERR_Pos          (20UL)         /*!< OTERR (Bit 20)                                        */
#define R_MRMS_MSTATR_OTERR_Msk          (0x100000UL)   /*!< OTERR (Bitfield-Mask: 0x01)                           */
#define R_MRMS_MSTATR_TZFERR_Pos         (19UL)         /*!< TZFERR (Bit 19)                                       */
#define R_MRMS_MSTATR_TZFERR_Msk         (0x80000UL)    /*!< TZFERR (Bitfield-Mask: 0x01)                          */
#define R_MRMS_MSTATR_MRDY_Pos           (15UL)         /*!< MRDY (Bit 15)                                         */
#define R_MRMS_MSTATR_MRDY_Msk           (0x8000UL)     /*!< MRDY (Bitfield-Mask: 0x01)                            */
#define R_MRMS_MSTATR_ILGLERR_Pos        (14UL)         /*!< ILGLERR (Bit 14)                                      */
#define R_MRMS_MSTATR_ILGLERR_Msk        (0x4000UL)     /*!< ILGLERR (Bitfield-Mask: 0x01)                         */
#define R_MRMS_MSTATR_PRGERR_Pos         (12UL)         /*!< PRGERR (Bit 12)                                       */
#define R_MRMS_MSTATR_PRGERR_Msk         (0x1000UL)     /*!< PRGERR (Bitfield-Mask: 0x01)                          */
#define R_MRMS_MSTATR_CFGPRGERR_Pos      (5UL)          /*!< CFGPRGERR (Bit 5)                                     */
#define R_MRMS_MSTATR_CFGPRGERR_Msk      (0x20UL)       /*!< CFGPRGERR (Bitfield-Mask: 0x01)                       */
#define R_MRMS_MSTATR_PROTERR_Pos        (4UL)          /*!< PROTERR (Bit 4)                                       */
#define R_MRMS_MSTATR_PROTERR_Msk        (0x10UL)       /*!< PROTERR (Bitfield-Mask: 0x01)                         */
/* ========================================================  MENTRYR  ======================================================== */
#define R_MRMS_MENTRYR_KEY_Pos           (8UL)          /*!< KEY (Bit 8)                                           */
#define R_MRMS_MENTRYR_KEY_Msk           (0xff00UL)     /*!< KEY (Bitfield-Mask: 0xff)                             */
#define R_MRMS_MENTRYR_MENTRY_Pos        (7UL)          /*!< MENTRY (Bit 7)                                        */
#define R_MRMS_MENTRYR_MENTRY_Msk        (0x80UL)       /*!< MENTRY (Bitfield-Mask: 0x01)                          */
/* =======================================================  MSUINITR  ======================================================== */
#define R_MRMS_MSUINITR_KEY_Pos          (8UL)          /*!< KEY (Bit 8)                                           */
#define R_MRMS_MSUINITR_KEY_Msk          (0xff00UL)     /*!< KEY (Bitfield-Mask: 0xff)                             */
#define R_MRMS_MSUINITR_SUINIT_Pos       (0UL)          /*!< SUINIT (Bit 0)                                        */
#define R_MRMS_MSUINITR_SUINIT_Msk       (0x1UL)        /*!< SUINIT (Bitfield-Mask: 0x01)                          */
/* =========================================================  MCMDR  ========================================================= */
#define R_MRMS_MCMDR_CMDR_Pos            (8UL)          /*!< CMDR (Bit 8)                                          */
#define R_MRMS_MCMDR_CMDR_Msk            (0xff00UL)     /*!< CMDR (Bitfield-Mask: 0xff)                            */
#define R_MRMS_MCMDR_PCMDR_Pos           (0UL)          /*!< PCMDR (Bit 0)                                         */
#define R_MRMS_MCMDR_PCMDR_Msk           (0xffUL)       /*!< PCMDR (Bitfield-Mask: 0xff)                           */
/* =======================================================  MSUASMON  ======================================================== */
#define R_MRMS_MSUASMON_BTFLG_Pos        (31UL)         /*!< BTFLG (Bit 31)                                        */
#define R_MRMS_MSUASMON_BTFLG_Msk        (0x80000000UL) /*!< BTFLG (Bitfield-Mask: 0x01)                           */
#define R_MRMS_MSUASMON_BTSIZE_Pos       (29UL)         /*!< BTSIZE (Bit 29)                                       */
#define R_MRMS_MSUASMON_BTSIZE_Msk       (0x60000000UL) /*!< BTSIZE (Bitfield-Mask: 0x03)                          */
#define R_MRMS_MSUASMON_FSPR_Pos         (15UL)         /*!< FSPR (Bit 15)                                         */
#define R_MRMS_MSUASMON_FSPR_Msk         (0x8000UL)     /*!< FSPR (Bitfield-Mask: 0x01)                            */
#define R_MRMS_MSUASMON_FSPRS_Pos        (14UL)         /*!< FSPRS (Bit 14)                                        */
#define R_MRMS_MSUASMON_FSPRS_Msk        (0x4000UL)     /*!< FSPRS (Bitfield-Mask: 0x01)                           */
/* ========================================================  MSUACR  ========================================================= */
#define R_MRMS_MSUACR_KEY_Pos            (8UL)          /*!< KEY (Bit 8)                                           */
#define R_MRMS_MSUACR_KEY_Msk            (0xff00UL)     /*!< KEY (Bitfield-Mask: 0xff)                             */
#define R_MRMS_MSUACR_SAS_Pos            (0UL)          /*!< SAS (Bit 0)                                           */
#define R_MRMS_MSUACR_SAS_Msk            (0x3UL)        /*!< SAS (Bitfield-Mask: 0x03)                             */
/* =======================================================  MRSTTRST  ======================================================== */
#define R_MRMS_MRSTTRST_RSTTRER_Pos      (0UL)          /*!< RSTTRER (Bit 0)                                       */
#define R_MRMS_MRSTTRST_RSTTRER_Msk      (0x1UL)        /*!< RSTTRER (Bitfield-Mask: 0x01)                         */
/* ========================================================  MRCPC0  ========================================================= */
#define R_MRMS_MRCPC0_KEY_Pos            (8UL)          /*!< KEY (Bit 8)                                           */
#define R_MRMS_MRCPC0_KEY_Msk            (0xff00UL)     /*!< KEY (Bitfield-Mask: 0xff)                             */
#define R_MRMS_MRCPC0_MRCPNEN_Pos        (0UL)          /*!< MRCPNEN (Bit 0)                                       */
#define R_MRMS_MRCPC0_MRCPNEN_Msk        (0x1UL)        /*!< MRCPNEN (Bitfield-Mask: 0x01)                         */
/* ========================================================  MRCPC1  ========================================================= */
#define R_MRMS_MRCPC1_KEY_Pos            (8UL)          /*!< KEY (Bit 8)                                           */
#define R_MRMS_MRCPC1_KEY_Msk            (0xff00UL)     /*!< KEY (Bitfield-Mask: 0xff)                             */
#define R_MRMS_MRCPC1_MRCPSEN_Pos        (0UL)          /*!< MRCPSEN (Bit 0)                                       */
#define R_MRMS_MRCPC1_MRCPSEN_Msk        (0x1UL)        /*!< MRCPSEN (Bitfield-Mask: 0x01)                         */
/* ========================================================  MBPROT0  ======================================================== */
#define R_MRMS_MBPROT0_KEY_Pos           (8UL)          /*!< KEY (Bit 8)                                           */
#define R_MRMS_MBPROT0_KEY_Msk           (0xff00UL)     /*!< KEY (Bitfield-Mask: 0xff)                             */
#define R_MRMS_MBPROT0_BPCN0_Pos         (0UL)          /*!< BPCN0 (Bit 0)                                         */
#define R_MRMS_MBPROT0_BPCN0_Msk         (0x1UL)        /*!< BPCN0 (Bitfield-Mask: 0x01)                           */
/* ========================================================  MBPROT1  ======================================================== */
#define R_MRMS_MBPROT1_KEY_Pos           (8UL)          /*!< KEY (Bit 8)                                           */
#define R_MRMS_MBPROT1_KEY_Msk           (0xff00UL)     /*!< KEY (Bitfield-Mask: 0xff)                             */
#define R_MRMS_MBPROT1_BPCN1_Pos         (0UL)          /*!< BPCN1 (Bit 0)                                         */
#define R_MRMS_MBPROT1_BPCN1_Msk         (0x1UL)        /*!< BPCN1 (Bitfield-Mask: 0x01)                           */
/* =========================================================  MRCPS  ========================================================= */
#define R_MRMS_MRCPS_PRGBSYC_Pos         (7UL)          /*!< PRGBSYC (Bit 7)                                       */
#define R_MRMS_MRCPS_PRGBSYC_Msk         (0x80UL)       /*!< PRGBSYC (Bitfield-Mask: 0x01)                         */
#define R_MRMS_MRCPS_ABUFFULL_Pos        (6UL)          /*!< ABUFFULL (Bit 6)                                      */
#define R_MRMS_MRCPS_ABUFFULL_Msk        (0x40UL)       /*!< ABUFFULL (Bitfield-Mask: 0x01)                        */
#define R_MRMS_MRCPS_ABUFEMP_Pos         (5UL)          /*!< ABUFEMP (Bit 5)                                       */
#define R_MRMS_MRCPS_ABUFEMP_Msk         (0x20UL)       /*!< ABUFEMP (Bitfield-Mask: 0x01)                         */
#define R_MRMS_MRCPS_PRGBSYCB_Pos        (4UL)          /*!< PRGBSYCB (Bit 4)                                      */
#define R_MRMS_MRCPS_PRGBSYCB_Msk        (0x10UL)       /*!< PRGBSYCB (Bitfield-Mask: 0x01)                        */
#define R_MRMS_MRCPS_ABUFFULLB_Pos       (3UL)          /*!< ABUFFULLB (Bit 3)                                     */
#define R_MRMS_MRCPS_ABUFFULLB_Msk       (0x8UL)        /*!< ABUFFULLB (Bitfield-Mask: 0x01)                       */
#define R_MRMS_MRCPS_ABUFEMPB_Pos        (2UL)          /*!< ABUFEMPB (Bit 2)                                      */
#define R_MRMS_MRCPS_ABUFEMPB_Msk        (0x4UL)        /*!< ABUFEMPB (Bitfield-Mask: 0x01)                        */
#define R_MRMS_MRCPS_ECCERRC_Pos         (1UL)          /*!< ECCERRC (Bit 1)                                       */
#define R_MRMS_MRCPS_ECCERRC_Msk         (0x2UL)        /*!< ECCERRC (Bitfield-Mask: 0x01)                         */
#define R_MRMS_MRCPS_PRGERRC_Pos         (0UL)          /*!< PRGERRC (Bit 0)                                       */
#define R_MRMS_MRCPS_PRGERRC_Msk         (0x1UL)        /*!< PRGERRC (Bitfield-Mask: 0x01)                         */
/* =======================================================  MRCPAEINT  ======================================================= */
#define R_MRMS_MRCPAEINT_MRCAEIE_Pos     (7UL)          /*!< MRCAEIE (Bit 7)                                       */
#define R_MRMS_MRCPAEINT_MRCAEIE_Msk     (0x80UL)       /*!< MRCAEIE (Bitfield-Mask: 0x01)                         */
/* ========================================================  MRCPEA  ========================================================= */
#define R_MRMS_MRCPEA_MCPEA_Pos          (5UL)          /*!< MCPEA (Bit 5)                                         */
#define R_MRMS_MRCPEA_MCPEA_Msk          (0xffffffe0UL) /*!< MCPEA (Bitfield-Mask: 0x7ffffff)                      */
/* ========================================================  MRCFLR  ========================================================= */
#define R_MRMS_MRCFLR_KEY_Pos            (8UL)          /*!< KEY (Bit 8)                                           */
#define R_MRMS_MRCFLR_KEY_Msk            (0xff00UL)     /*!< KEY (Bitfield-Mask: 0xff)                             */
#define R_MRMS_MRCFLR_MRCFL_Pos          (0UL)          /*!< MRCFL (Bit 0)                                         */
#define R_MRMS_MRCFLR_MRCFL_Msk          (0x1UL)        /*!< MRCFL (Bitfield-Mask: 0x01)                           */
/* =======================================================  MRCPRICR  ======================================================== */
#define R_MRMS_MRCPRICR_MRCPRRIE_Pos     (7UL)          /*!< MRCPRRIE (Bit 7)                                      */
#define R_MRMS_MRCPRICR_MRCPRRIE_Msk     (0x80UL)       /*!< MRCPRRIE (Bitfield-Mask: 0x01)                        */
#define R_MRMS_MRCPRICR_MRCINTSEL_Pos    (0UL)          /*!< MRCINTSEL (Bit 0)                                     */
#define R_MRMS_MRCPRICR_MRCINTSEL_Msk    (0x1UL)        /*!< MRCINTSEL (Bitfield-Mask: 0x01)                       */
/* =========================================================  MRDPS  ========================================================= */
#define R_MRMS_MRDPS_PRGBSYD_Pos         (7UL)          /*!< PRGBSYD (Bit 7)                                       */
#define R_MRMS_MRDPS_PRGBSYD_Msk         (0x80UL)       /*!< PRGBSYD (Bitfield-Mask: 0x01)                         */
#define R_MRMS_MRDPS_ABUFFULLD_Pos       (6UL)          /*!< ABUFFULLD (Bit 6)                                     */
#define R_MRMS_MRDPS_ABUFFULLD_Msk       (0x40UL)       /*!< ABUFFULLD (Bitfield-Mask: 0x01)                       */
#define R_MRMS_MRDPS_ABUFEMPD_Pos        (5UL)          /*!< ABUFEMPD (Bit 5)                                      */
#define R_MRMS_MRDPS_ABUFEMPD_Msk        (0x20UL)       /*!< ABUFEMPD (Bitfield-Mask: 0x01)                        */
#define R_MRMS_MRDPS_ECCERRD_Pos         (1UL)          /*!< ECCERRD (Bit 1)                                       */
#define R_MRMS_MRDPS_ECCERRD_Msk         (0x2UL)        /*!< ECCERRD (Bitfield-Mask: 0x01)                         */
#define R_MRMS_MRDPS_PRGERRD_Pos         (0UL)          /*!< PRGERRD (Bit 0)                                       */
#define R_MRMS_MRDPS_PRGERRD_Msk         (0x1UL)        /*!< PRGERRD (Bitfield-Mask: 0x01)                         */
/* =======================================================  MRDPAEINT  ======================================================= */
#define R_MRMS_MRDPAEINT_MRDAEIE_Pos     (7UL)          /*!< MRDAEIE (Bit 7)                                       */
#define R_MRMS_MRDPAEINT_MRDAEIE_Msk     (0x80UL)       /*!< MRDAEIE (Bitfield-Mask: 0x01)                         */
/* ========================================================  MRDPEA  ========================================================= */
#define R_MRMS_MRDPEA_MDPEA_Pos          (4UL)          /*!< MDPEA (Bit 4)                                         */
#define R_MRMS_MRDPEA_MDPEA_Msk          (0xfffffff0UL) /*!< MDPEA (Bitfield-Mask: 0xfffffff)                      */
/* ========================================================  MRDFLR  ========================================================= */
#define R_MRMS_MRDFLR_KEY_Pos            (8UL)          /*!< KEY (Bit 8)                                           */
#define R_MRMS_MRDFLR_KEY_Msk            (0xff00UL)     /*!< KEY (Bitfield-Mask: 0xff)                             */
#define R_MRMS_MRDFLR_MRDFL_Pos          (0UL)          /*!< MRDFL (Bit 0)                                         */
#define R_MRMS_MRDFLR_MRDFL_Msk          (0x1UL)        /*!< MRDFL (Bitfield-Mask: 0x01)                           */
/* =======================================================  MRDPRICR  ======================================================== */
#define R_MRMS_MRDPRICR_MRDPRRIE_Pos     (7UL)          /*!< MRDPRRIE (Bit 7)                                      */
#define R_MRMS_MRDPRICR_MRDPRRIE_Msk     (0x80UL)       /*!< MRDPRRIE (Bitfield-Mask: 0x01)                        */
/* =========================================================  MRPC  ========================================================== */
#define R_MRMS_MRPC_MHSPEN_Pos           (0UL)          /*!< MHSPEN (Bit 0)                                        */
#define R_MRMS_MRPC_MHSPEN_Msk           (0x1UL)        /*!< MHSPEN (Bitfield-Mask: 0x01)                          */
/* ========================================================  MREEECC  ======================================================== */
#define R_MRMS_MREEECC_KEY_Pos           (8UL)          /*!< KEY (Bit 8)                                           */
#define R_MRMS_MREEECC_KEY_Msk           (0xff00UL)     /*!< KEY (Bitfield-Mask: 0xff)                             */
#define R_MRMS_MREEECC_ECCBYPE_Pos       (0UL)          /*!< ECCBYPE (Bit 0)                                       */
#define R_MRMS_MREEECC_ECCBYPE_Msk       (0x1UL)        /*!< ECCBYPE (Bitfield-Mask: 0x01)                         */
/* ========================================================  MRCEECC  ======================================================== */
#define R_MRMS_MRCEECC_KEY_Pos           (8UL)          /*!< KEY (Bit 8)                                           */
#define R_MRMS_MRCEECC_KEY_Msk           (0xff00UL)     /*!< KEY (Bitfield-Mask: 0xff)                             */
#define R_MRMS_MRCEECC_ECCBYPC_Pos       (0UL)          /*!< ECCBYPC (Bit 0)                                       */
#define R_MRMS_MRCEECC_ECCBYPC_Msk       (0x1UL)        /*!< ECCBYPC (Bitfield-Mask: 0x01)                         */

#endif /* R_MRAM_POSMSK_H */
