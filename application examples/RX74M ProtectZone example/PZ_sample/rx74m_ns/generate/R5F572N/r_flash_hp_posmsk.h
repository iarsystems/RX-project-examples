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

#ifndef R_FLASH_HP_POSMSK_H
#define R_FLASH_HP_POSMSK_H

/* =========================================================================================================================== */
/* ================                                       R_FACI_HP_CMD                                       ================ */
/* =========================================================================================================================== */

/* ======================================================  FACI_CMD16  ======================================================= */
/* =======================================================  FACI_CMD8  ======================================================= */

/* =========================================================================================================================== */
/* ================                                         R_FACI_HP                                         ================ */
/* =========================================================================================================================== */

/* ========================================================  FASTAT  ========================================================= */
#define R_FACI_HP_FASTAT_CFAE_Pos         (7UL)          /*!< CFAE (Bit 7)                                          */
#define R_FACI_HP_FASTAT_CFAE_Msk         (0x80UL)       /*!< CFAE (Bitfield-Mask: 0x01)                            */
#define R_FACI_HP_FASTAT_CMDLK_Pos        (4UL)          /*!< CMDLK (Bit 4)                                         */
#define R_FACI_HP_FASTAT_CMDLK_Msk        (0x10UL)       /*!< CMDLK (Bitfield-Mask: 0x01)                           */
#define R_FACI_HP_FASTAT_DFAE_Pos         (3UL)          /*!< DFAE (Bit 3)                                          */
#define R_FACI_HP_FASTAT_DFAE_Msk         (0x8UL)        /*!< DFAE (Bitfield-Mask: 0x01)                            */
#define R_FACI_HP_FASTAT_ECRCT_Pos        (0UL)          /*!< ECRCT (Bit 0)                                         */
#define R_FACI_HP_FASTAT_ECRCT_Msk        (0x1UL)        /*!< ECRCT (Bitfield-Mask: 0x01)                           */
/* ========================================================  FAEINT  ========================================================= */
#define R_FACI_HP_FAEINT_CFAEIE_Pos       (7UL)          /*!< CFAEIE (Bit 7)                                        */
#define R_FACI_HP_FAEINT_CFAEIE_Msk       (0x80UL)       /*!< CFAEIE (Bitfield-Mask: 0x01)                          */
#define R_FACI_HP_FAEINT_CMDLKIE_Pos      (4UL)          /*!< CMDLKIE (Bit 4)                                       */
#define R_FACI_HP_FAEINT_CMDLKIE_Msk      (0x10UL)       /*!< CMDLKIE (Bitfield-Mask: 0x01)                         */
#define R_FACI_HP_FAEINT_DFAEIE_Pos       (3UL)          /*!< DFAEIE (Bit 3)                                        */
#define R_FACI_HP_FAEINT_DFAEIE_Msk       (0x8UL)        /*!< DFAEIE (Bitfield-Mask: 0x01)                          */
#define R_FACI_HP_FAEINT_ECRCTIE_Pos      (0UL)          /*!< ECRCTIE (Bit 0)                                       */
#define R_FACI_HP_FAEINT_ECRCTIE_Msk      (0x1UL)        /*!< ECRCTIE (Bitfield-Mask: 0x01)                         */
/* ========================================================  FRDYIE  ========================================================= */
#define R_FACI_HP_FRDYIE_FRDYIE_Pos       (0UL)          /*!< FRDYIE (Bit 0)                                        */
#define R_FACI_HP_FRDYIE_FRDYIE_Msk       (0x1UL)        /*!< FRDYIE (Bitfield-Mask: 0x01)                          */
/* ========================================================  FSADDR  ========================================================= */
#define R_FACI_HP_FSADDR_FSA_Pos          (0UL)          /*!< FSA (Bit 0)                                           */
#define R_FACI_HP_FSADDR_FSA_Msk          (0xffffffffUL) /*!< FSA (Bitfield-Mask: 0xffffffff)                       */
/* ========================================================  FEADDR  ========================================================= */
#define R_FACI_HP_FEADDR_FEA_Pos          (0UL)          /*!< FEA (Bit 0)                                           */
#define R_FACI_HP_FEADDR_FEA_Msk          (0xffffffffUL) /*!< FEA (Bitfield-Mask: 0xffffffff)                       */
/* ========================================================  FMEPROT  ======================================================== */
#define R_FACI_HP_FMEPROT_KEY_Pos         (8UL)          /*!< KEY (Bit 8)                                           */
#define R_FACI_HP_FMEPROT_KEY_Msk         (0xff00UL)     /*!< KEY (Bitfield-Mask: 0xff)                             */
#define R_FACI_HP_FMEPROT_CEPROT_Pos      (0UL)          /*!< CEPROT (Bit 0)                                        */
#define R_FACI_HP_FMEPROT_CEPROT_Msk      (0x1UL)        /*!< CEPROT (Bitfield-Mask: 0x01)                          */
/* ========================================================  FBPROT0  ======================================================== */
#define R_FACI_HP_FBPROT0_KEY_Pos         (8UL)          /*!< KEY (Bit 8)                                           */
#define R_FACI_HP_FBPROT0_KEY_Msk         (0xff00UL)     /*!< KEY (Bitfield-Mask: 0xff)                             */
#define R_FACI_HP_FBPROT0_BPCN0_Pos       (0UL)          /*!< BPCN0 (Bit 0)                                         */
#define R_FACI_HP_FBPROT0_BPCN0_Msk       (0x1UL)        /*!< BPCN0 (Bitfield-Mask: 0x01)                           */
/* ========================================================  FBPROT1  ======================================================== */
#define R_FACI_HP_FBPROT1_KEY_Pos         (8UL)          /*!< KEY (Bit 8)                                           */
#define R_FACI_HP_FBPROT1_KEY_Msk         (0xff00UL)     /*!< KEY (Bitfield-Mask: 0xff)                             */
#define R_FACI_HP_FBPROT1_BPCN1_Pos       (0UL)          /*!< BPCN1 (Bit 0)                                         */
#define R_FACI_HP_FBPROT1_BPCN1_Msk       (0x1UL)        /*!< BPCN1 (Bitfield-Mask: 0x01)                           */
/* ========================================================  FSTATR  ========================================================= */
#define R_FACI_HP_FSTATR_ILGCOMERR_Pos    (23UL)         /*!< ILGCOMERR (Bit 23)                                    */
#define R_FACI_HP_FSTATR_ILGCOMERR_Msk    (0x800000UL)   /*!< ILGCOMERR (Bitfield-Mask: 0x01)                       */
#define R_FACI_HP_FSTATR_FESETERR_Pos     (22UL)         /*!< FESETERR (Bit 22)                                     */
#define R_FACI_HP_FSTATR_FESETERR_Msk     (0x400000UL)   /*!< FESETERR (Bitfield-Mask: 0x01)                        */
#define R_FACI_HP_FSTATR_SECERR_Pos       (21UL)         /*!< SECERR (Bit 21)                                       */
#define R_FACI_HP_FSTATR_SECERR_Msk       (0x200000UL)   /*!< SECERR (Bitfield-Mask: 0x01)                          */
#define R_FACI_HP_FSTATR_OTERR_Pos        (20UL)         /*!< OTERR (Bit 20)                                        */
#define R_FACI_HP_FSTATR_OTERR_Msk        (0x100000UL)   /*!< OTERR (Bitfield-Mask: 0x01)                           */
#define R_FACI_HP_FSTATR_FRDY_Pos         (15UL)         /*!< FRDY (Bit 15)                                         */
#define R_FACI_HP_FSTATR_FRDY_Msk         (0x8000UL)     /*!< FRDY (Bitfield-Mask: 0x01)                            */
#define R_FACI_HP_FSTATR_ILGLERR_Pos      (14UL)         /*!< ILGLERR (Bit 14)                                      */
#define R_FACI_HP_FSTATR_ILGLERR_Msk      (0x4000UL)     /*!< ILGLERR (Bitfield-Mask: 0x01)                         */
#define R_FACI_HP_FSTATR_ERSERR_Pos       (13UL)         /*!< ERSERR (Bit 13)                                       */
#define R_FACI_HP_FSTATR_ERSERR_Msk       (0x2000UL)     /*!< ERSERR (Bitfield-Mask: 0x01)                          */
#define R_FACI_HP_FSTATR_PRGERR_Pos       (12UL)         /*!< PRGERR (Bit 12)                                       */
#define R_FACI_HP_FSTATR_PRGERR_Msk       (0x1000UL)     /*!< PRGERR (Bitfield-Mask: 0x01)                          */
#define R_FACI_HP_FSTATR_SUSRDY_Pos       (11UL)         /*!< SUSRDY (Bit 11)                                       */
#define R_FACI_HP_FSTATR_SUSRDY_Msk       (0x800UL)      /*!< SUSRDY (Bitfield-Mask: 0x01)                          */
#define R_FACI_HP_FSTATR_DBFULL_Pos       (10UL)         /*!< DBFULL (Bit 10)                                       */
#define R_FACI_HP_FSTATR_DBFULL_Msk       (0x400UL)      /*!< DBFULL (Bitfield-Mask: 0x01)                          */
#define R_FACI_HP_FSTATR_ERSSPD_Pos       (9UL)          /*!< ERSSPD (Bit 9)                                        */
#define R_FACI_HP_FSTATR_ERSSPD_Msk       (0x200UL)      /*!< ERSSPD (Bitfield-Mask: 0x01)                          */
#define R_FACI_HP_FSTATR_PRGSPD_Pos       (8UL)          /*!< PRGSPD (Bit 8)                                        */
#define R_FACI_HP_FSTATR_PRGSPD_Msk       (0x100UL)      /*!< PRGSPD (Bitfield-Mask: 0x01)                          */
#define R_FACI_HP_FSTATR_FLWEERR_Pos      (6UL)          /*!< FLWEERR (Bit 6)                                       */
#define R_FACI_HP_FSTATR_FLWEERR_Msk      (0x40UL)       /*!< FLWEERR (Bitfield-Mask: 0x01)                         */
/* ========================================================  FENTRYR  ======================================================== */
#define R_FACI_HP_FENTRYR_KEY_Pos         (8UL)          /*!< KEY (Bit 8)                                           */
#define R_FACI_HP_FENTRYR_KEY_Msk         (0xff00UL)     /*!< KEY (Bitfield-Mask: 0xff)                             */
#define R_FACI_HP_FENTRYR_FENTRYD_Pos     (7UL)          /*!< FENTRYD (Bit 7)                                       */
#define R_FACI_HP_FENTRYR_FENTRYD_Msk     (0x80UL)       /*!< FENTRYD (Bitfield-Mask: 0x01)                         */
#define R_FACI_HP_FENTRYR_FENTRYC_Pos     (0UL)          /*!< FENTRYC (Bit 0)                                       */
#define R_FACI_HP_FENTRYR_FENTRYC_Msk     (0x1UL)        /*!< FENTRYC (Bitfield-Mask: 0x01)                         */
/* =======================================================  FSUINITR  ======================================================== */
#define R_FACI_HP_FSUINITR_KEY_Pos        (8UL)          /*!< KEY (Bit 8)                                           */
#define R_FACI_HP_FSUINITR_KEY_Msk        (0xff00UL)     /*!< KEY (Bitfield-Mask: 0xff)                             */
#define R_FACI_HP_FSUINITR_SUINIT_Pos     (0UL)          /*!< SUINIT (Bit 0)                                        */
#define R_FACI_HP_FSUINITR_SUINIT_Msk     (0x1UL)        /*!< SUINIT (Bitfield-Mask: 0x01)                          */
/* =========================================================  FCMDR  ========================================================= */
#define R_FACI_HP_FCMDR_CMDR_Pos          (8UL)          /*!< CMDR (Bit 8)                                          */
#define R_FACI_HP_FCMDR_CMDR_Msk          (0xff00UL)     /*!< CMDR (Bitfield-Mask: 0xff)                            */
#define R_FACI_HP_FCMDR_PCMDR_Pos         (0UL)          /*!< PCMDR (Bit 0)                                         */
#define R_FACI_HP_FCMDR_PCMDR_Msk         (0xffUL)       /*!< PCMDR (Bitfield-Mask: 0xff)                           */
/* ========================================================  FPESTAT  ======================================================== */
#define R_FACI_HP_FPESTAT_PEERRST_Pos     (0UL)          /*!< PEERRST (Bit 0)                                       */
#define R_FACI_HP_FPESTAT_PEERRST_Msk     (0xffUL)       /*!< PEERRST (Bitfield-Mask: 0xff)                         */
/* ========================================================  FBCCNT  ========================================================= */
#define R_FACI_HP_FBCCNT_BCDIR_Pos        (0UL)          /*!< BCDIR (Bit 0)                                         */
#define R_FACI_HP_FBCCNT_BCDIR_Msk        (0x1UL)        /*!< BCDIR (Bitfield-Mask: 0x01)                           */
/* ========================================================  FBCSTAT  ======================================================== */
#define R_FACI_HP_FBCSTAT_BCST_Pos        (0UL)          /*!< BCST (Bit 0)                                          */
#define R_FACI_HP_FBCSTAT_BCST_Msk        (0x1UL)        /*!< BCST (Bitfield-Mask: 0x01)                            */
/* ========================================================  FPSADDR  ======================================================== */
#define R_FACI_HP_FPSADDR_PSADR_Pos       (0UL)          /*!< PSADR (Bit 0)                                         */
#define R_FACI_HP_FPSADDR_PSADR_Msk       (0x7ffffUL)    /*!< PSADR (Bitfield-Mask: 0x7ffff)                        */
/* ========================================================  FAWMON  ========================================================= */
#define R_FACI_HP_FAWMON_BTFLG_Pos        (31UL)         /*!< BTFLG (Bit 31)                                        */
#define R_FACI_HP_FAWMON_BTFLG_Msk        (0x80000000UL) /*!< BTFLG (Bitfield-Mask: 0x01)                           */
#define R_FACI_HP_FAWMON_FAWE_Pos         (16UL)         /*!< FAWE (Bit 16)                                         */
#define R_FACI_HP_FAWMON_FAWE_Msk         (0x7ff0000UL)  /*!< FAWE (Bitfield-Mask: 0x7ff)                           */
#define R_FACI_HP_FAWMON_FSPR_Pos         (15UL)         /*!< FSPR (Bit 15)                                         */
#define R_FACI_HP_FAWMON_FSPR_Msk         (0x8000UL)     /*!< FSPR (Bitfield-Mask: 0x01)                            */
#define R_FACI_HP_FAWMON_FAWS_Pos         (0UL)          /*!< FAWS (Bit 0)                                          */
#define R_FACI_HP_FAWMON_FAWS_Msk         (0x7ffUL)      /*!< FAWS (Bitfield-Mask: 0x7ff)                           */
/* =========================================================  FCPSR  ========================================================= */
#define R_FACI_HP_FCPSR_ESUSPMD_Pos       (0UL)          /*!< ESUSPMD (Bit 0)                                       */
#define R_FACI_HP_FCPSR_ESUSPMD_Msk       (0x1UL)        /*!< ESUSPMD (Bitfield-Mask: 0x01)                         */
/* ========================================================  FPCKAR  ========================================================= */
#define R_FACI_HP_FPCKAR_KEY_Pos          (8UL)          /*!< KEY (Bit 8)                                           */
#define R_FACI_HP_FPCKAR_KEY_Msk          (0xff00UL)     /*!< KEY (Bitfield-Mask: 0xff)                             */
#define R_FACI_HP_FPCKAR_PCKA_Pos         (0UL)          /*!< PCKA (Bit 0)                                          */
#define R_FACI_HP_FPCKAR_PCKA_Msk         (0xffUL)       /*!< PCKA (Bitfield-Mask: 0xff)                            */
/* ========================================================  FSUACR  ========================================================= */
#define R_FACI_HP_FSUACR_KEY_Pos          (8UL)          /*!< KEY (Bit 8)                                           */
#define R_FACI_HP_FSUACR_KEY_Msk          (0xff00UL)     /*!< KEY (Bitfield-Mask: 0xff)                             */
#define R_FACI_HP_FSUACR_SAS_Pos          (0UL)          /*!< SAS (Bit 0)                                           */
#define R_FACI_HP_FSUACR_SAS_Msk          (0x3UL)        /*!< SAS (Bitfield-Mask: 0x03)                             */

#endif /* R_FLASH_HP_POSMSK_H */
