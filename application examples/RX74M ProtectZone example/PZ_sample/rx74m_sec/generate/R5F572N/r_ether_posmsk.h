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

#ifndef R_ETHER_POSMSK_H
#define R_ETHER_POSMSK_H

/* =========================================================================================================================== */

/* ================                                         R_ETHERC0                                         ================ */
/* =========================================================================================================================== */

/* =========================================================  ECMR  ========================================================== */
#define R_ETHERC0_ECMR_TPC_Pos          (20UL)         /*!< TPC (Bit 20)                                          */
#define R_ETHERC0_ECMR_TPC_Msk          (0x100000UL)   /*!< TPC (Bitfield-Mask: 0x01)                             */
#define R_ETHERC0_ECMR_ZPF_Pos          (19UL)         /*!< ZPF (Bit 19)                                          */
#define R_ETHERC0_ECMR_ZPF_Msk          (0x80000UL)    /*!< ZPF (Bitfield-Mask: 0x01)                             */
#define R_ETHERC0_ECMR_PFR_Pos          (18UL)         /*!< PFR (Bit 18)                                          */
#define R_ETHERC0_ECMR_PFR_Msk          (0x40000UL)    /*!< PFR (Bitfield-Mask: 0x01)                             */
#define R_ETHERC0_ECMR_RXF_Pos          (17UL)         /*!< RXF (Bit 17)                                          */
#define R_ETHERC0_ECMR_RXF_Msk          (0x20000UL)    /*!< RXF (Bitfield-Mask: 0x01)                             */
#define R_ETHERC0_ECMR_TXF_Pos          (16UL)         /*!< TXF (Bit 16)                                          */
#define R_ETHERC0_ECMR_TXF_Msk          (0x10000UL)    /*!< TXF (Bitfield-Mask: 0x01)                             */
#define R_ETHERC0_ECMR_PRCEF_Pos        (12UL)         /*!< PRCEF (Bit 12)                                        */
#define R_ETHERC0_ECMR_PRCEF_Msk        (0x1000UL)     /*!< PRCEF (Bitfield-Mask: 0x01)                           */
#define R_ETHERC0_ECMR_MPDE_Pos         (9UL)          /*!< MPDE (Bit 9)                                          */
#define R_ETHERC0_ECMR_MPDE_Msk         (0x200UL)      /*!< MPDE (Bitfield-Mask: 0x01)                            */
#define R_ETHERC0_ECMR_RE_Pos           (6UL)          /*!< RE (Bit 6)                                            */
#define R_ETHERC0_ECMR_RE_Msk           (0x40UL)       /*!< RE (Bitfield-Mask: 0x01)                              */
#define R_ETHERC0_ECMR_TE_Pos           (5UL)          /*!< TE (Bit 5)                                            */
#define R_ETHERC0_ECMR_TE_Msk           (0x20UL)       /*!< TE (Bitfield-Mask: 0x01)                              */
#define R_ETHERC0_ECMR_ILB_Pos          (3UL)          /*!< ILB (Bit 3)                                           */
#define R_ETHERC0_ECMR_ILB_Msk          (0x8UL)        /*!< ILB (Bitfield-Mask: 0x01)                             */
#define R_ETHERC0_ECMR_RTM_Pos          (2UL)          /*!< RTM (Bit 2)                                           */
#define R_ETHERC0_ECMR_RTM_Msk          (0x4UL)        /*!< RTM (Bitfield-Mask: 0x01)                             */
#define R_ETHERC0_ECMR_DM_Pos           (1UL)          /*!< DM (Bit 1)                                            */
#define R_ETHERC0_ECMR_DM_Msk           (0x2UL)        /*!< DM (Bitfield-Mask: 0x01)                              */
#define R_ETHERC0_ECMR_PRM_Pos          (0UL)          /*!< PRM (Bit 0)                                           */
#define R_ETHERC0_ECMR_PRM_Msk          (0x1UL)        /*!< PRM (Bitfield-Mask: 0x01)                             */
/* =========================================================  RFLR  ========================================================== */
#define R_ETHERC0_RFLR_RFL_Pos          (0UL)          /*!< RFL (Bit 0)                                           */
#define R_ETHERC0_RFLR_RFL_Msk          (0xfffUL)      /*!< RFL (Bitfield-Mask: 0xfff)                            */
/* =========================================================  ECSR  ========================================================== */
#define R_ETHERC0_ECSR_BFR_Pos          (5UL)          /*!< BFR (Bit 5)                                           */
#define R_ETHERC0_ECSR_BFR_Msk          (0x20UL)       /*!< BFR (Bitfield-Mask: 0x01)                             */
#define R_ETHERC0_ECSR_PSRTO_Pos        (4UL)          /*!< PSRTO (Bit 4)                                         */
#define R_ETHERC0_ECSR_PSRTO_Msk        (0x10UL)       /*!< PSRTO (Bitfield-Mask: 0x01)                           */
#define R_ETHERC0_ECSR_LCHNG_Pos        (2UL)          /*!< LCHNG (Bit 2)                                         */
#define R_ETHERC0_ECSR_LCHNG_Msk        (0x4UL)        /*!< LCHNG (Bitfield-Mask: 0x01)                           */
#define R_ETHERC0_ECSR_MPD_Pos          (1UL)          /*!< MPD (Bit 1)                                           */
#define R_ETHERC0_ECSR_MPD_Msk          (0x2UL)        /*!< MPD (Bitfield-Mask: 0x01)                             */
#define R_ETHERC0_ECSR_ICD_Pos          (0UL)          /*!< ICD (Bit 0)                                           */
#define R_ETHERC0_ECSR_ICD_Msk          (0x1UL)        /*!< ICD (Bitfield-Mask: 0x01)                             */
/* ========================================================  ECSIPR  ========================================================= */
#define R_ETHERC0_ECSIPR_BFSIPR_Pos     (5UL)          /*!< BFSIPR (Bit 5)                                        */
#define R_ETHERC0_ECSIPR_BFSIPR_Msk     (0x20UL)       /*!< BFSIPR (Bitfield-Mask: 0x01)                          */
#define R_ETHERC0_ECSIPR_PSRTOIP_Pos    (4UL)          /*!< PSRTOIP (Bit 4)                                       */
#define R_ETHERC0_ECSIPR_PSRTOIP_Msk    (0x10UL)       /*!< PSRTOIP (Bitfield-Mask: 0x01)                         */
#define R_ETHERC0_ECSIPR_LCHNGIP_Pos    (2UL)          /*!< LCHNGIP (Bit 2)                                       */
#define R_ETHERC0_ECSIPR_LCHNGIP_Msk    (0x4UL)        /*!< LCHNGIP (Bitfield-Mask: 0x01)                         */
#define R_ETHERC0_ECSIPR_MPDIP_Pos      (1UL)          /*!< MPDIP (Bit 1)                                         */
#define R_ETHERC0_ECSIPR_MPDIP_Msk      (0x2UL)        /*!< MPDIP (Bitfield-Mask: 0x01)                           */
#define R_ETHERC0_ECSIPR_ICDIP_Pos      (0UL)          /*!< ICDIP (Bit 0)                                         */
#define R_ETHERC0_ECSIPR_ICDIP_Msk      (0x1UL)        /*!< ICDIP (Bitfield-Mask: 0x01)                           */
/* ==========================================================  PIR  ========================================================== */
#define R_ETHERC0_PIR_MDI_Pos           (3UL)          /*!< MDI (Bit 3)                                           */
#define R_ETHERC0_PIR_MDI_Msk           (0x8UL)        /*!< MDI (Bitfield-Mask: 0x01)                             */
#define R_ETHERC0_PIR_MDO_Pos           (2UL)          /*!< MDO (Bit 2)                                           */
#define R_ETHERC0_PIR_MDO_Msk           (0x4UL)        /*!< MDO (Bitfield-Mask: 0x01)                             */
#define R_ETHERC0_PIR_MMD_Pos           (1UL)          /*!< MMD (Bit 1)                                           */
#define R_ETHERC0_PIR_MMD_Msk           (0x2UL)        /*!< MMD (Bitfield-Mask: 0x01)                             */
#define R_ETHERC0_PIR_MDC_Pos           (0UL)          /*!< MDC (Bit 0)                                           */
#define R_ETHERC0_PIR_MDC_Msk           (0x1UL)        /*!< MDC (Bitfield-Mask: 0x01)                             */
/* ==========================================================  PSR  ========================================================== */
#define R_ETHERC0_PSR_LMON_Pos          (0UL)          /*!< LMON (Bit 0)                                          */
#define R_ETHERC0_PSR_LMON_Msk          (0x1UL)        /*!< LMON (Bitfield-Mask: 0x01)                            */
/* =========================================================  RDMLR  ========================================================= */
#define R_ETHERC0_RDMLR_RMD_Pos         (0UL)          /*!< RMD (Bit 0)                                           */
#define R_ETHERC0_RDMLR_RMD_Msk         (0xfffffUL)    /*!< RMD (Bitfield-Mask: 0xfffff)                          */
/* =========================================================  IPGR  ========================================================== */
#define R_ETHERC0_IPGR_IPG_Pos          (0UL)          /*!< IPG (Bit 0)                                           */
#define R_ETHERC0_IPGR_IPG_Msk          (0x1fUL)       /*!< IPG (Bitfield-Mask: 0x1f)                             */
/* ==========================================================  APR  ========================================================== */
#define R_ETHERC0_APR_AP_Pos            (0UL)          /*!< AP (Bit 0)                                            */
#define R_ETHERC0_APR_AP_Msk            (0xffffUL)     /*!< AP (Bitfield-Mask: 0xffff)                            */
/* ==========================================================  MPR  ========================================================== */
#define R_ETHERC0_MPR_MP_Pos            (0UL)          /*!< MP (Bit 0)                                            */
#define R_ETHERC0_MPR_MP_Msk            (0xffffUL)     /*!< MP (Bitfield-Mask: 0xffff)                            */
/* =========================================================  RFCF  ========================================================== */
#define R_ETHERC0_RFCF_RPAUSE_Pos       (0UL)          /*!< RPAUSE (Bit 0)                                        */
#define R_ETHERC0_RFCF_RPAUSE_Msk       (0xffUL)       /*!< RPAUSE (Bitfield-Mask: 0xff)                          */
/* ========================================================  TPAUSER  ======================================================== */
#define R_ETHERC0_TPAUSER_TPAUSE_Pos    (0UL)          /*!< TPAUSE (Bit 0)                                        */
#define R_ETHERC0_TPAUSER_TPAUSE_Msk    (0xffffUL)     /*!< TPAUSE (Bitfield-Mask: 0xffff)                        */
/* =======================================================  TPAUSECR  ======================================================== */
/* =========================================================  BCFRR  ========================================================= */
#define R_ETHERC0_BCFRR_BCF_Pos         (0UL)          /*!< BCF (Bit 0)                                           */
#define R_ETHERC0_BCFRR_BCF_Msk         (0xffffUL)     /*!< BCF (Bitfield-Mask: 0xffff)                           */
/* =========================================================  MAHR  ========================================================== */
#define R_ETHERC0_MAHR_MAHR_Pos         (0UL)          /*!< MAHR (Bit 0)                                          */
#define R_ETHERC0_MAHR_MAHR_Msk         (0xffffffffUL) /*!< MAHR (Bitfield-Mask: 0xffffffff)                      */
/* =========================================================  MALR  ========================================================== */
#define R_ETHERC0_MALR_MALR_Pos         (0UL)          /*!< MALR (Bit 0)                                          */
#define R_ETHERC0_MALR_MALR_Msk         (0xffffUL)     /*!< MALR (Bitfield-Mask: 0xffff)                          */
/* =========================================================  TROCR  ========================================================= */
#define R_ETHERC0_TROCR_TROCR_Pos       (0UL)          /*!< TROCR (Bit 0)                                         */
#define R_ETHERC0_TROCR_TROCR_Msk       (0xffffffffUL) /*!< TROCR (Bitfield-Mask: 0xffffffff)                     */
/* =========================================================  CDCR  ========================================================== */
/* =========================================================  LCCR  ========================================================== */
#define R_ETHERC0_LCCR_LCCR_Pos         (0UL)          /*!< LCCR (Bit 0)                                          */
#define R_ETHERC0_LCCR_LCCR_Msk         (0xffffffffUL) /*!< LCCR (Bitfield-Mask: 0xffffffff)                      */
/* =========================================================  CNDCR  ========================================================= */
#define R_ETHERC0_CNDCR_CNDCR_Pos       (0UL)          /*!< CNDCR (Bit 0)                                         */
#define R_ETHERC0_CNDCR_CNDCR_Msk       (0xffffffffUL) /*!< CNDCR (Bitfield-Mask: 0xffffffff)                     */
/* =========================================================  CEFCR  ========================================================= */
#define R_ETHERC0_CEFCR_CEFCR_Pos       (0UL)          /*!< CEFCR (Bit 0)                                         */
#define R_ETHERC0_CEFCR_CEFCR_Msk       (0xffffffffUL) /*!< CEFCR (Bitfield-Mask: 0xffffffff)                     */
/* =========================================================  FRECR  ========================================================= */
#define R_ETHERC0_FRECR_FRECR_Pos       (0UL)          /*!< FRECR (Bit 0)                                         */
#define R_ETHERC0_FRECR_FRECR_Msk       (0xffffffffUL) /*!< FRECR (Bitfield-Mask: 0xffffffff)                     */
/* ========================================================  TSFRCR  ========================================================= */
#define R_ETHERC0_TSFRCR_TSFRCR_Pos     (0UL)          /*!< TSFRCR (Bit 0)                                        */
#define R_ETHERC0_TSFRCR_TSFRCR_Msk     (0xffffffffUL) /*!< TSFRCR (Bitfield-Mask: 0xffffffff)                    */
/* ========================================================  TLFRCR  ========================================================= */
#define R_ETHERC0_TLFRCR_TLFRCR_Pos     (0UL)          /*!< TLFRCR (Bit 0)                                        */
#define R_ETHERC0_TLFRCR_TLFRCR_Msk     (0xffffffffUL) /*!< TLFRCR (Bitfield-Mask: 0xffffffff)                    */
/* =========================================================  RFCR  ========================================================== */
#define R_ETHERC0_RFCR_RFCR_Pos         (0UL)          /*!< RFCR (Bit 0)                                          */
#define R_ETHERC0_RFCR_RFCR_Msk         (0xffffffffUL) /*!< RFCR (Bitfield-Mask: 0xffffffff)                      */
/* =========================================================  MAFCR  ========================================================= */
#define R_ETHERC0_MAFCR_MAFCR_Pos       (0UL)          /*!< MAFCR (Bit 0)                                         */
#define R_ETHERC0_MAFCR_MAFCR_Msk       (0xffffffffUL) /*!< MAFCR (Bitfield-Mask: 0xffffffff)                     */

/* =========================================================================================================================== */
/* ================                                      R_ETHERC_EDMAC                                       ================ */
/* =========================================================================================================================== */

/* =========================================================  EDMR  ========================================================== */
#define R_ETHERC_EDMAC_EDMR_DE_Pos           (6UL)          /*!< DE (Bit 6)                                            */
#define R_ETHERC_EDMAC_EDMR_DE_Msk           (0x40UL)       /*!< DE (Bitfield-Mask: 0x01)                              */
#define R_ETHERC_EDMAC_EDMR_DL_Pos           (4UL)          /*!< DL (Bit 4)                                            */
#define R_ETHERC_EDMAC_EDMR_DL_Msk           (0x30UL)       /*!< DL (Bitfield-Mask: 0x03)                              */
#define R_ETHERC_EDMAC_EDMR_SWR_Pos          (0UL)          /*!< SWR (Bit 0)                                           */
#define R_ETHERC_EDMAC_EDMR_SWR_Msk          (0x1UL)        /*!< SWR (Bitfield-Mask: 0x01)                             */
/* =========================================================  EDTRR  ========================================================= */
#define R_ETHERC_EDMAC_EDTRR_TR_Pos          (0UL)          /*!< TR (Bit 0)                                            */
#define R_ETHERC_EDMAC_EDTRR_TR_Msk          (0x1UL)        /*!< TR (Bitfield-Mask: 0x01)                              */
/* =========================================================  EDRRR  ========================================================= */
#define R_ETHERC_EDMAC_EDRRR_RR_Pos          (0UL)          /*!< RR (Bit 0)                                            */
#define R_ETHERC_EDMAC_EDRRR_RR_Msk          (0x1UL)        /*!< RR (Bitfield-Mask: 0x01)                              */
/* =========================================================  TDLAR  ========================================================= */
#define R_ETHERC_EDMAC_TDLAR_TDLAR_Pos       (0UL)          /*!< TDLAR (Bit 0)                                         */
#define R_ETHERC_EDMAC_TDLAR_TDLAR_Msk       (0xffffffffUL) /*!< TDLAR (Bitfield-Mask: 0xffffffff)                     */
/* =========================================================  RDLAR  ========================================================= */
#define R_ETHERC_EDMAC_RDLAR_RDLAR_Pos       (0UL)          /*!< RDLAR (Bit 0)                                         */
#define R_ETHERC_EDMAC_RDLAR_RDLAR_Msk       (0xffffffffUL) /*!< RDLAR (Bitfield-Mask: 0xffffffff)                     */
/* =========================================================  EESR  ========================================================== */
#define R_ETHERC_EDMAC_EESR_TWB_Pos          (30UL)         /*!< TWB (Bit 30)                                          */
#define R_ETHERC_EDMAC_EESR_TWB_Msk          (0x40000000UL) /*!< TWB (Bitfield-Mask: 0x01)                             */
#define R_ETHERC_EDMAC_EESR_TABT_Pos         (26UL)         /*!< TABT (Bit 26)                                         */
#define R_ETHERC_EDMAC_EESR_TABT_Msk         (0x4000000UL)  /*!< TABT (Bitfield-Mask: 0x01)                            */
#define R_ETHERC_EDMAC_EESR_RABT_Pos         (25UL)         /*!< RABT (Bit 25)                                         */
#define R_ETHERC_EDMAC_EESR_RABT_Msk         (0x2000000UL)  /*!< RABT (Bitfield-Mask: 0x01)                            */
#define R_ETHERC_EDMAC_EESR_RFCOF_Pos        (24UL)         /*!< RFCOF (Bit 24)                                        */
#define R_ETHERC_EDMAC_EESR_RFCOF_Msk        (0x1000000UL)  /*!< RFCOF (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EDMAC_EESR_ADE_Pos          (23UL)         /*!< ADE (Bit 23)                                          */
#define R_ETHERC_EDMAC_EESR_ADE_Msk          (0x800000UL)   /*!< ADE (Bitfield-Mask: 0x01)                             */
#define R_ETHERC_EDMAC_EESR_ECI_Pos          (22UL)         /*!< ECI (Bit 22)                                          */
#define R_ETHERC_EDMAC_EESR_ECI_Msk          (0x400000UL)   /*!< ECI (Bitfield-Mask: 0x01)                             */
#define R_ETHERC_EDMAC_EESR_TC_Pos           (21UL)         /*!< TC (Bit 21)                                           */
#define R_ETHERC_EDMAC_EESR_TC_Msk           (0x200000UL)   /*!< TC (Bitfield-Mask: 0x01)                              */
#define R_ETHERC_EDMAC_EESR_TDE_Pos          (20UL)         /*!< TDE (Bit 20)                                          */
#define R_ETHERC_EDMAC_EESR_TDE_Msk          (0x100000UL)   /*!< TDE (Bitfield-Mask: 0x01)                             */
#define R_ETHERC_EDMAC_EESR_TFUF_Pos         (19UL)         /*!< TFUF (Bit 19)                                         */
#define R_ETHERC_EDMAC_EESR_TFUF_Msk         (0x80000UL)    /*!< TFUF (Bitfield-Mask: 0x01)                            */
#define R_ETHERC_EDMAC_EESR_FR_Pos           (18UL)         /*!< FR (Bit 18)                                           */
#define R_ETHERC_EDMAC_EESR_FR_Msk           (0x40000UL)    /*!< FR (Bitfield-Mask: 0x01)                              */
#define R_ETHERC_EDMAC_EESR_RDE_Pos          (17UL)         /*!< RDE (Bit 17)                                          */
#define R_ETHERC_EDMAC_EESR_RDE_Msk          (0x20000UL)    /*!< RDE (Bitfield-Mask: 0x01)                             */
#define R_ETHERC_EDMAC_EESR_RFOF_Pos         (16UL)         /*!< RFOF (Bit 16)                                         */
#define R_ETHERC_EDMAC_EESR_RFOF_Msk         (0x10000UL)    /*!< RFOF (Bitfield-Mask: 0x01)                            */
#define R_ETHERC_EDMAC_EESR_CND_Pos          (11UL)         /*!< CND (Bit 11)                                          */
#define R_ETHERC_EDMAC_EESR_CND_Msk          (0x800UL)      /*!< CND (Bitfield-Mask: 0x01)                             */
#define R_ETHERC_EDMAC_EESR_DLC_Pos          (10UL)         /*!< DLC (Bit 10)                                          */
#define R_ETHERC_EDMAC_EESR_DLC_Msk          (0x400UL)      /*!< DLC (Bitfield-Mask: 0x01)                             */
#define R_ETHERC_EDMAC_EESR_CD_Pos           (9UL)          /*!< CD (Bit 9)                                            */
#define R_ETHERC_EDMAC_EESR_CD_Msk           (0x200UL)      /*!< CD (Bitfield-Mask: 0x01)                              */
#define R_ETHERC_EDMAC_EESR_TRO_Pos          (8UL)          /*!< TRO (Bit 8)                                           */
#define R_ETHERC_EDMAC_EESR_TRO_Msk          (0x100UL)      /*!< TRO (Bitfield-Mask: 0x01)                             */
#define R_ETHERC_EDMAC_EESR_RMAF_Pos         (7UL)          /*!< RMAF (Bit 7)                                          */
#define R_ETHERC_EDMAC_EESR_RMAF_Msk         (0x80UL)       /*!< RMAF (Bitfield-Mask: 0x01)                            */
#define R_ETHERC_EDMAC_EESR_RRF_Pos          (4UL)          /*!< RRF (Bit 4)                                           */
#define R_ETHERC_EDMAC_EESR_RRF_Msk          (0x10UL)       /*!< RRF (Bitfield-Mask: 0x01)                             */
#define R_ETHERC_EDMAC_EESR_RTLF_Pos         (3UL)          /*!< RTLF (Bit 3)                                          */
#define R_ETHERC_EDMAC_EESR_RTLF_Msk         (0x8UL)        /*!< RTLF (Bitfield-Mask: 0x01)                            */
#define R_ETHERC_EDMAC_EESR_RTSF_Pos         (2UL)          /*!< RTSF (Bit 2)                                          */
#define R_ETHERC_EDMAC_EESR_RTSF_Msk         (0x4UL)        /*!< RTSF (Bitfield-Mask: 0x01)                            */
#define R_ETHERC_EDMAC_EESR_PRE_Pos          (1UL)          /*!< PRE (Bit 1)                                           */
#define R_ETHERC_EDMAC_EESR_PRE_Msk          (0x2UL)        /*!< PRE (Bitfield-Mask: 0x01)                             */
#define R_ETHERC_EDMAC_EESR_CERF_Pos         (0UL)          /*!< CERF (Bit 0)                                          */
#define R_ETHERC_EDMAC_EESR_CERF_Msk         (0x1UL)        /*!< CERF (Bitfield-Mask: 0x01)                            */
/* ========================================================  EESIPR  ========================================================= */
#define R_ETHERC_EDMAC_EESIPR_TWBIP_Pos      (30UL)         /*!< TWBIP (Bit 30)                                        */
#define R_ETHERC_EDMAC_EESIPR_TWBIP_Msk      (0x40000000UL) /*!< TWBIP (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EDMAC_EESIPR_TABTIP_Pos     (26UL)         /*!< TABTIP (Bit 26)                                       */
#define R_ETHERC_EDMAC_EESIPR_TABTIP_Msk     (0x4000000UL)  /*!< TABTIP (Bitfield-Mask: 0x01)                          */
#define R_ETHERC_EDMAC_EESIPR_RABTIP_Pos     (25UL)         /*!< RABTIP (Bit 25)                                       */
#define R_ETHERC_EDMAC_EESIPR_RABTIP_Msk     (0x2000000UL)  /*!< RABTIP (Bitfield-Mask: 0x01)                          */
#define R_ETHERC_EDMAC_EESIPR_RFCOFIP_Pos    (24UL)         /*!< RFCOFIP (Bit 24)                                      */
#define R_ETHERC_EDMAC_EESIPR_RFCOFIP_Msk    (0x1000000UL)  /*!< RFCOFIP (Bitfield-Mask: 0x01)                         */
#define R_ETHERC_EDMAC_EESIPR_ADEIP_Pos      (23UL)         /*!< ADEIP (Bit 23)                                        */
#define R_ETHERC_EDMAC_EESIPR_ADEIP_Msk      (0x800000UL)   /*!< ADEIP (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EDMAC_EESIPR_ECIIP_Pos      (22UL)         /*!< ECIIP (Bit 22)                                        */
#define R_ETHERC_EDMAC_EESIPR_ECIIP_Msk      (0x400000UL)   /*!< ECIIP (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EDMAC_EESIPR_TCIP_Pos       (21UL)         /*!< TCIP (Bit 21)                                         */
#define R_ETHERC_EDMAC_EESIPR_TCIP_Msk       (0x200000UL)   /*!< TCIP (Bitfield-Mask: 0x01)                            */
#define R_ETHERC_EDMAC_EESIPR_TDEIP_Pos      (20UL)         /*!< TDEIP (Bit 20)                                        */
#define R_ETHERC_EDMAC_EESIPR_TDEIP_Msk      (0x100000UL)   /*!< TDEIP (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EDMAC_EESIPR_TFUFIP_Pos     (19UL)         /*!< TFUFIP (Bit 19)                                       */
#define R_ETHERC_EDMAC_EESIPR_TFUFIP_Msk     (0x80000UL)    /*!< TFUFIP (Bitfield-Mask: 0x01)                          */
#define R_ETHERC_EDMAC_EESIPR_FRIP_Pos       (18UL)         /*!< FRIP (Bit 18)                                         */
#define R_ETHERC_EDMAC_EESIPR_FRIP_Msk       (0x40000UL)    /*!< FRIP (Bitfield-Mask: 0x01)                            */
#define R_ETHERC_EDMAC_EESIPR_RDEIP_Pos      (17UL)         /*!< RDEIP (Bit 17)                                        */
#define R_ETHERC_EDMAC_EESIPR_RDEIP_Msk      (0x20000UL)    /*!< RDEIP (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EDMAC_EESIPR_RFOFIP_Pos     (16UL)         /*!< RFOFIP (Bit 16)                                       */
#define R_ETHERC_EDMAC_EESIPR_RFOFIP_Msk     (0x10000UL)    /*!< RFOFIP (Bitfield-Mask: 0x01)                          */
#define R_ETHERC_EDMAC_EESIPR_CNDIP_Pos      (11UL)         /*!< CNDIP (Bit 11)                                        */
#define R_ETHERC_EDMAC_EESIPR_CNDIP_Msk      (0x800UL)      /*!< CNDIP (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EDMAC_EESIPR_DLCIP_Pos      (10UL)         /*!< DLCIP (Bit 10)                                        */
#define R_ETHERC_EDMAC_EESIPR_DLCIP_Msk      (0x400UL)      /*!< DLCIP (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EDMAC_EESIPR_CDIP_Pos       (9UL)          /*!< CDIP (Bit 9)                                          */
#define R_ETHERC_EDMAC_EESIPR_CDIP_Msk       (0x200UL)      /*!< CDIP (Bitfield-Mask: 0x01)                            */
#define R_ETHERC_EDMAC_EESIPR_TROIP_Pos      (8UL)          /*!< TROIP (Bit 8)                                         */
#define R_ETHERC_EDMAC_EESIPR_TROIP_Msk      (0x100UL)      /*!< TROIP (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EDMAC_EESIPR_RMAFIP_Pos     (7UL)          /*!< RMAFIP (Bit 7)                                        */
#define R_ETHERC_EDMAC_EESIPR_RMAFIP_Msk     (0x80UL)       /*!< RMAFIP (Bitfield-Mask: 0x01)                          */
#define R_ETHERC_EDMAC_EESIPR_RRFIP_Pos      (4UL)          /*!< RRFIP (Bit 4)                                         */
#define R_ETHERC_EDMAC_EESIPR_RRFIP_Msk      (0x10UL)       /*!< RRFIP (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EDMAC_EESIPR_RTLFIP_Pos     (3UL)          /*!< RTLFIP (Bit 3)                                        */
#define R_ETHERC_EDMAC_EESIPR_RTLFIP_Msk     (0x8UL)        /*!< RTLFIP (Bitfield-Mask: 0x01)                          */
#define R_ETHERC_EDMAC_EESIPR_RTSFIP_Pos     (2UL)          /*!< RTSFIP (Bit 2)                                        */
#define R_ETHERC_EDMAC_EESIPR_RTSFIP_Msk     (0x4UL)        /*!< RTSFIP (Bitfield-Mask: 0x01)                          */
#define R_ETHERC_EDMAC_EESIPR_PREIP_Pos      (1UL)          /*!< PREIP (Bit 1)                                         */
#define R_ETHERC_EDMAC_EESIPR_PREIP_Msk      (0x2UL)        /*!< PREIP (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EDMAC_EESIPR_CERFIP_Pos     (0UL)          /*!< CERFIP (Bit 0)                                        */
#define R_ETHERC_EDMAC_EESIPR_CERFIP_Msk     (0x1UL)        /*!< CERFIP (Bitfield-Mask: 0x01)                          */
/* ========================================================  TRSCER  ========================================================= */
#define R_ETHERC_EDMAC_TRSCER_RMAFCE_Pos     (7UL)          /*!< RMAFCE (Bit 7)                                        */
#define R_ETHERC_EDMAC_TRSCER_RMAFCE_Msk     (0x80UL)       /*!< RMAFCE (Bitfield-Mask: 0x01)                          */
#define R_ETHERC_EDMAC_TRSCER_RRFCE_Pos      (4UL)          /*!< RRFCE (Bit 4)                                         */
#define R_ETHERC_EDMAC_TRSCER_RRFCE_Msk      (0x10UL)       /*!< RRFCE (Bitfield-Mask: 0x01)                           */
/* =========================================================  RMFCR  ========================================================= */
#define R_ETHERC_EDMAC_RMFCR_MFC_Pos         (0UL)          /*!< MFC (Bit 0)                                           */
#define R_ETHERC_EDMAC_RMFCR_MFC_Msk         (0xffffUL)     /*!< MFC (Bitfield-Mask: 0xffff)                           */
/* =========================================================  TFTR  ========================================================== */
#define R_ETHERC_EDMAC_TFTR_TFT_Pos          (0UL)          /*!< TFT (Bit 0)                                           */
#define R_ETHERC_EDMAC_TFTR_TFT_Msk          (0x7ffUL)      /*!< TFT (Bitfield-Mask: 0x7ff)                            */
/* ==========================================================  FDR  ========================================================== */
#define R_ETHERC_EDMAC_FDR_TFD_Pos           (8UL)          /*!< TFD (Bit 8)                                           */
#define R_ETHERC_EDMAC_FDR_TFD_Msk           (0x1f00UL)     /*!< TFD (Bitfield-Mask: 0x1f)                             */
#define R_ETHERC_EDMAC_FDR_RFD_Pos           (0UL)          /*!< RFD (Bit 0)                                           */
#define R_ETHERC_EDMAC_FDR_RFD_Msk           (0x1fUL)       /*!< RFD (Bitfield-Mask: 0x1f)                             */
/* =========================================================  RMCR  ========================================================== */
#define R_ETHERC_EDMAC_RMCR_RNR_Pos          (0UL)          /*!< RNR (Bit 0)                                           */
#define R_ETHERC_EDMAC_RMCR_RNR_Msk          (0x1UL)        /*!< RNR (Bitfield-Mask: 0x01)                             */
/* =========================================================  TFUCR  ========================================================= */
#define R_ETHERC_EDMAC_TFUCR_UNDER_Pos       (0UL)          /*!< UNDER (Bit 0)                                         */
#define R_ETHERC_EDMAC_TFUCR_UNDER_Msk       (0xffffUL)     /*!< UNDER (Bitfield-Mask: 0xffff)                         */
/* =========================================================  RFOCR  ========================================================= */
#define R_ETHERC_EDMAC_RFOCR_OVER_Pos        (0UL)          /*!< OVER (Bit 0)                                          */
#define R_ETHERC_EDMAC_RFOCR_OVER_Msk        (0xffffUL)     /*!< OVER (Bitfield-Mask: 0xffff)                          */
/* =========================================================  IOSR  ========================================================== */
#define R_ETHERC_EDMAC_IOSR_ELB_Pos          (0UL)          /*!< ELB (Bit 0)                                           */
#define R_ETHERC_EDMAC_IOSR_ELB_Msk          (0x1UL)        /*!< ELB (Bitfield-Mask: 0x01)                             */
/* =========================================================  FCFTR  ========================================================= */
#define R_ETHERC_EDMAC_FCFTR_RFFO_Pos        (16UL)         /*!< RFFO (Bit 16)                                         */
#define R_ETHERC_EDMAC_FCFTR_RFFO_Msk        (0x70000UL)    /*!< RFFO (Bitfield-Mask: 0x07)                            */
#define R_ETHERC_EDMAC_FCFTR_RFDO_Pos        (0UL)          /*!< RFDO (Bit 0)                                          */
#define R_ETHERC_EDMAC_FCFTR_RFDO_Msk        (0x7UL)        /*!< RFDO (Bitfield-Mask: 0x07)                            */
/* ========================================================  RPADIR  ========================================================= */
#define R_ETHERC_EDMAC_RPADIR_PADS_Pos       (16UL)         /*!< PADS (Bit 16)                                         */
#define R_ETHERC_EDMAC_RPADIR_PADS_Msk       (0x30000UL)    /*!< PADS (Bitfield-Mask: 0x03)                            */
#define R_ETHERC_EDMAC_RPADIR_PADR_Pos       (0UL)          /*!< PADR (Bit 0)                                          */
#define R_ETHERC_EDMAC_RPADIR_PADR_Msk       (0x3fUL)       /*!< PADR (Bitfield-Mask: 0x3f)                            */
/* =========================================================  TRIMD  ========================================================= */
#define R_ETHERC_EDMAC_TRIMD_TIM_Pos         (4UL)          /*!< TIM (Bit 4)                                           */
#define R_ETHERC_EDMAC_TRIMD_TIM_Msk         (0x10UL)       /*!< TIM (Bitfield-Mask: 0x01)                             */
#define R_ETHERC_EDMAC_TRIMD_TIS_Pos         (0UL)          /*!< TIS (Bit 0)                                           */
#define R_ETHERC_EDMAC_TRIMD_TIS_Msk         (0x1UL)        /*!< TIS (Bitfield-Mask: 0x01)                             */
/* =========================================================  RBWAR  ========================================================= */
#define R_ETHERC_EDMAC_RBWAR_RBWAR_Pos       (0UL)          /*!< RBWAR (Bit 0)                                         */
#define R_ETHERC_EDMAC_RBWAR_RBWAR_Msk       (0xffffffffUL) /*!< RBWAR (Bitfield-Mask: 0xffffffff)                     */
/* =========================================================  RDFAR  ========================================================= */
#define R_ETHERC_EDMAC_RDFAR_RDFAR_Pos       (0UL)          /*!< RDFAR (Bit 0)                                         */
#define R_ETHERC_EDMAC_RDFAR_RDFAR_Msk       (0xffffffffUL) /*!< RDFAR (Bitfield-Mask: 0xffffffff)                     */
/* =========================================================  TBRAR  ========================================================= */
#define R_ETHERC_EDMAC_TBRAR_TBRAR_Pos       (0UL)          /*!< TBRAR (Bit 0)                                         */
#define R_ETHERC_EDMAC_TBRAR_TBRAR_Msk       (0xffffffffUL) /*!< TBRAR (Bitfield-Mask: 0xffffffff)                     */
/* =========================================================  TDFAR  ========================================================= */
#define R_ETHERC_EDMAC_TDFAR_TDFAR_Pos       (0UL)          /*!< TDFAR (Bit 0)                                         */
#define R_ETHERC_EDMAC_TDFAR_TDFAR_Msk       (0xffffffffUL) /*!< TDFAR (Bitfield-Mask: 0xffffffff)                     */

/* =========================================================================================================================== */
/* ================                                      R_ETHERC_EPTPC                                       ================ */
/* =========================================================================================================================== */

/* =========================================================  SYSR  ========================================================== */
#define R_ETHERC_EPTPC_SYSR_GENDN_Pos             (17UL)         /*!< GENDN (Bit 17)                                        */
#define R_ETHERC_EPTPC_SYSR_GENDN_Msk             (0x20000UL)    /*!< GENDN (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_SYSR_RESDN_Pos             (16UL)         /*!< RESDN (Bit 16)                                        */
#define R_ETHERC_EPTPC_SYSR_RESDN_Msk             (0x10000UL)    /*!< RESDN (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_SYSR_INFABT_Pos            (14UL)         /*!< INFABT (Bit 14)                                       */
#define R_ETHERC_EPTPC_SYSR_INFABT_Msk            (0x4000UL)     /*!< INFABT (Bitfield-Mask: 0x01)                          */
#define R_ETHERC_EPTPC_SYSR_RECLP_Pos             (12UL)         /*!< RECLP (Bit 12)                                        */
#define R_ETHERC_EPTPC_SYSR_RECLP_Msk             (0x1000UL)     /*!< RECLP (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_SYSR_DRQOVR_Pos            (6UL)          /*!< DRQOVR (Bit 6)                                        */
#define R_ETHERC_EPTPC_SYSR_DRQOVR_Msk            (0x40UL)       /*!< DRQOVR (Bitfield-Mask: 0x01)                          */
#define R_ETHERC_EPTPC_SYSR_INTDEV_Pos            (5UL)          /*!< INTDEV (Bit 5)                                        */
#define R_ETHERC_EPTPC_SYSR_INTDEV_Msk            (0x20UL)       /*!< INTDEV (Bitfield-Mask: 0x01)                          */
#define R_ETHERC_EPTPC_SYSR_DRPTO_Pos             (4UL)          /*!< DRPTO (Bit 4)                                         */
#define R_ETHERC_EPTPC_SYSR_DRPTO_Msk             (0x10UL)       /*!< DRPTO (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_SYSR_MPDUD_Pos             (2UL)          /*!< MPDUD (Bit 2)                                         */
#define R_ETHERC_EPTPC_SYSR_MPDUD_Msk             (0x4UL)        /*!< MPDUD (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_SYSR_INTCHG_Pos            (1UL)          /*!< INTCHG (Bit 1)                                        */
#define R_ETHERC_EPTPC_SYSR_INTCHG_Msk            (0x2UL)        /*!< INTCHG (Bitfield-Mask: 0x01)                          */
#define R_ETHERC_EPTPC_SYSR_OFMUD_Pos             (0UL)          /*!< OFMUD (Bit 0)                                         */
#define R_ETHERC_EPTPC_SYSR_OFMUD_Msk             (0x1UL)        /*!< OFMUD (Bitfield-Mask: 0x01)                           */
/* =========================================================  SYIPR  ========================================================= */
#define R_ETHERC_EPTPC_SYIPR_GENDN_Pos            (17UL)         /*!< GENDN (Bit 17)                                        */
#define R_ETHERC_EPTPC_SYIPR_GENDN_Msk            (0x20000UL)    /*!< GENDN (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_SYIPR_RESDN_Pos            (16UL)         /*!< RESDN (Bit 16)                                        */
#define R_ETHERC_EPTPC_SYIPR_RESDN_Msk            (0x10000UL)    /*!< RESDN (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_SYIPR_INFABT_Pos           (14UL)         /*!< INFABT (Bit 14)                                       */
#define R_ETHERC_EPTPC_SYIPR_INFABT_Msk           (0x4000UL)     /*!< INFABT (Bitfield-Mask: 0x01)                          */
#define R_ETHERC_EPTPC_SYIPR_RECLP_Pos            (12UL)         /*!< RECLP (Bit 12)                                        */
#define R_ETHERC_EPTPC_SYIPR_RECLP_Msk            (0x1000UL)     /*!< RECLP (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_SYIPR_DRQOVR_Pos           (6UL)          /*!< DRQOVR (Bit 6)                                        */
#define R_ETHERC_EPTPC_SYIPR_DRQOVR_Msk           (0x40UL)       /*!< DRQOVR (Bitfield-Mask: 0x01)                          */
#define R_ETHERC_EPTPC_SYIPR_INTDEV_Pos           (5UL)          /*!< INTDEV (Bit 5)                                        */
#define R_ETHERC_EPTPC_SYIPR_INTDEV_Msk           (0x20UL)       /*!< INTDEV (Bitfield-Mask: 0x01)                          */
#define R_ETHERC_EPTPC_SYIPR_DRPTO_Pos            (4UL)          /*!< DRPTO (Bit 4)                                         */
#define R_ETHERC_EPTPC_SYIPR_DRPTO_Msk            (0x10UL)       /*!< DRPTO (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_SYIPR_MPDUD_Pos            (2UL)          /*!< MPDUD (Bit 2)                                         */
#define R_ETHERC_EPTPC_SYIPR_MPDUD_Msk            (0x4UL)        /*!< MPDUD (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_SYIPR_INTCHG_Pos           (1UL)          /*!< INTCHG (Bit 1)                                        */
#define R_ETHERC_EPTPC_SYIPR_INTCHG_Msk           (0x2UL)        /*!< INTCHG (Bitfield-Mask: 0x01)                          */
#define R_ETHERC_EPTPC_SYIPR_OFMUD_Pos            (0UL)          /*!< OFMUD (Bit 0)                                         */
#define R_ETHERC_EPTPC_SYIPR_OFMUD_Msk            (0x1UL)        /*!< OFMUD (Bitfield-Mask: 0x01)                           */
/* ========================================================  SYMACRU  ======================================================== */
#define R_ETHERC_EPTPC_SYMACRU_SYMACRU_Pos        (0UL)          /*!< SYMACRU (Bit 0)                                       */
#define R_ETHERC_EPTPC_SYMACRU_SYMACRU_Msk        (0xffffffUL)   /*!< SYMACRU (Bitfield-Mask: 0xffffff)                     */
/* ========================================================  SYMACRL  ======================================================== */
#define R_ETHERC_EPTPC_SYMACRL_SYMACRL_Pos        (0UL)          /*!< SYMACRL (Bit 0)                                       */
#define R_ETHERC_EPTPC_SYMACRL_SYMACRL_Msk        (0xffffffUL)   /*!< SYMACRL (Bitfield-Mask: 0xffffff)                     */
/* =======================================================  SYLLCCTLR  ======================================================= */
#define R_ETHERC_EPTPC_SYLLCCTLR_CTL_Pos          (0UL)          /*!< CTL (Bit 0)                                           */
#define R_ETHERC_EPTPC_SYLLCCTLR_CTL_Msk          (0xffUL)       /*!< CTL (Bitfield-Mask: 0xff)                             */
/* =======================================================  SYIPADDRR  ======================================================= */
#define R_ETHERC_EPTPC_SYIPADDRR_SYIPADDRR_Pos    (0UL)          /*!< SYIPADDRR (Bit 0)                                     */
#define R_ETHERC_EPTPC_SYIPADDRR_SYIPADDRR_Msk    (0xffffffffUL) /*!< SYIPADDRR (Bitfield-Mask: 0xffffffff)                 */
/* ========================================================  SYSPVRR  ======================================================== */
#define R_ETHERC_EPTPC_SYSPVRR_TRSP_Pos           (4UL)          /*!< TRSP (Bit 4)                                          */
#define R_ETHERC_EPTPC_SYSPVRR_TRSP_Msk           (0xf0UL)       /*!< TRSP (Bitfield-Mask: 0x0f)                            */
#define R_ETHERC_EPTPC_SYSPVRR_VER_Pos            (0UL)          /*!< VER (Bit 0)                                           */
#define R_ETHERC_EPTPC_SYSPVRR_VER_Msk            (0xfUL)        /*!< VER (Bitfield-Mask: 0x0f)                             */
/* ========================================================  SYDOMR  ========================================================= */
#define R_ETHERC_EPTPC_SYDOMR_DNUM_Pos            (0UL)          /*!< DNUM (Bit 0)                                          */
#define R_ETHERC_EPTPC_SYDOMR_DNUM_Msk            (0xffUL)       /*!< DNUM (Bitfield-Mask: 0xff)                            */
/* =========================================================  ANFR  ========================================================== */
#define R_ETHERC_EPTPC_ANFR_FLAG14_Pos            (14UL)         /*!< FLAG14 (Bit 14)                                       */
#define R_ETHERC_EPTPC_ANFR_FLAG14_Msk            (0x4000UL)     /*!< FLAG14 (Bitfield-Mask: 0x01)                          */
#define R_ETHERC_EPTPC_ANFR_FLAG13_Pos            (13UL)         /*!< FLAG13 (Bit 13)                                       */
#define R_ETHERC_EPTPC_ANFR_FLAG13_Msk            (0x2000UL)     /*!< FLAG13 (Bitfield-Mask: 0x01)                          */
#define R_ETHERC_EPTPC_ANFR_FLAG10_Pos            (10UL)         /*!< FLAG10 (Bit 10)                                       */
#define R_ETHERC_EPTPC_ANFR_FLAG10_Msk            (0x400UL)      /*!< FLAG10 (Bitfield-Mask: 0x01)                          */
#define R_ETHERC_EPTPC_ANFR_FLAG8_Pos             (8UL)          /*!< FLAG8 (Bit 8)                                         */
#define R_ETHERC_EPTPC_ANFR_FLAG8_Msk             (0x100UL)      /*!< FLAG8 (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_ANFR_FLAG5_Pos             (5UL)          /*!< FLAG5 (Bit 5)                                         */
#define R_ETHERC_EPTPC_ANFR_FLAG5_Msk             (0x20UL)       /*!< FLAG5 (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_ANFR_FLAG4_Pos             (4UL)          /*!< FLAG4 (Bit 4)                                         */
#define R_ETHERC_EPTPC_ANFR_FLAG4_Msk             (0x10UL)       /*!< FLAG4 (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_ANFR_FLAG3_Pos             (3UL)          /*!< FLAG3 (Bit 3)                                         */
#define R_ETHERC_EPTPC_ANFR_FLAG3_Msk             (0x8UL)        /*!< FLAG3 (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_ANFR_FLAG2_Pos             (2UL)          /*!< FLAG2 (Bit 2)                                         */
#define R_ETHERC_EPTPC_ANFR_FLAG2_Msk             (0x4UL)        /*!< FLAG2 (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_ANFR_FLAG1_Pos             (1UL)          /*!< FLAG1 (Bit 1)                                         */
#define R_ETHERC_EPTPC_ANFR_FLAG1_Msk             (0x2UL)        /*!< FLAG1 (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_ANFR_FLAG0_Pos             (0UL)          /*!< FLAG0 (Bit 0)                                         */
#define R_ETHERC_EPTPC_ANFR_FLAG0_Msk             (0x1UL)        /*!< FLAG0 (Bitfield-Mask: 0x01)                           */
/* =========================================================  SYNFR  ========================================================= */
#define R_ETHERC_EPTPC_SYNFR_FLAG14_Pos           (14UL)         /*!< FLAG14 (Bit 14)                                       */
#define R_ETHERC_EPTPC_SYNFR_FLAG14_Msk           (0x4000UL)     /*!< FLAG14 (Bitfield-Mask: 0x01)                          */
#define R_ETHERC_EPTPC_SYNFR_FLAG13_Pos           (13UL)         /*!< FLAG13 (Bit 13)                                       */
#define R_ETHERC_EPTPC_SYNFR_FLAG13_Msk           (0x2000UL)     /*!< FLAG13 (Bitfield-Mask: 0x01)                          */
#define R_ETHERC_EPTPC_SYNFR_FLAG10_Pos           (10UL)         /*!< FLAG10 (Bit 10)                                       */
#define R_ETHERC_EPTPC_SYNFR_FLAG10_Msk           (0x400UL)      /*!< FLAG10 (Bitfield-Mask: 0x01)                          */
#define R_ETHERC_EPTPC_SYNFR_FLAG9_Pos            (9UL)          /*!< FLAG9 (Bit 9)                                         */
#define R_ETHERC_EPTPC_SYNFR_FLAG9_Msk            (0x200UL)      /*!< FLAG9 (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_SYNFR_FLAG8_Pos            (8UL)          /*!< FLAG8 (Bit 8)                                         */
#define R_ETHERC_EPTPC_SYNFR_FLAG8_Msk            (0x100UL)      /*!< FLAG8 (Bitfield-Mask: 0x01)                           */
/* ========================================================  DYRQFR  ========================================================= */
#define R_ETHERC_EPTPC_DYRQFR_FLAG14_Pos          (14UL)         /*!< FLAG14 (Bit 14)                                       */
#define R_ETHERC_EPTPC_DYRQFR_FLAG14_Msk          (0x4000UL)     /*!< FLAG14 (Bitfield-Mask: 0x01)                          */
#define R_ETHERC_EPTPC_DYRQFR_FLAG13_Pos          (13UL)         /*!< FLAG13 (Bit 13)                                       */
#define R_ETHERC_EPTPC_DYRQFR_FLAG13_Msk          (0x2000UL)     /*!< FLAG13 (Bitfield-Mask: 0x01)                          */
#define R_ETHERC_EPTPC_DYRQFR_FLAG10_Pos          (10UL)         /*!< FLAG10 (Bit 10)                                       */
#define R_ETHERC_EPTPC_DYRQFR_FLAG10_Msk          (0x400UL)      /*!< FLAG10 (Bitfield-Mask: 0x01)                          */
/* ========================================================  DYRPFR  ========================================================= */
#define R_ETHERC_EPTPC_DYRPFR_FLAG14_Pos          (14UL)         /*!< FLAG14 (Bit 14)                                       */
#define R_ETHERC_EPTPC_DYRPFR_FLAG14_Msk          (0x4000UL)     /*!< FLAG14 (Bitfield-Mask: 0x01)                          */
#define R_ETHERC_EPTPC_DYRPFR_FLAG13_Pos          (13UL)         /*!< FLAG13 (Bit 13)                                       */
#define R_ETHERC_EPTPC_DYRPFR_FLAG13_Msk          (0x2000UL)     /*!< FLAG13 (Bitfield-Mask: 0x01)                          */
#define R_ETHERC_EPTPC_DYRPFR_FLAG10_Pos          (10UL)         /*!< FLAG10 (Bit 10)                                       */
#define R_ETHERC_EPTPC_DYRPFR_FLAG10_Msk          (0x400UL)      /*!< FLAG10 (Bitfield-Mask: 0x01)                          */
#define R_ETHERC_EPTPC_DYRPFR_FLAG9_Pos           (9UL)          /*!< FLAG9 (Bit 9)                                         */
#define R_ETHERC_EPTPC_DYRPFR_FLAG9_Msk           (0x200UL)      /*!< FLAG9 (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_DYRPFR_FLAG8_Pos           (8UL)          /*!< FLAG8 (Bit 8)                                         */
#define R_ETHERC_EPTPC_DYRPFR_FLAG8_Msk           (0x100UL)      /*!< FLAG8 (Bitfield-Mask: 0x01)                           */
/* ========================================================  SYCIDRU  ======================================================== */
#define R_ETHERC_EPTPC_SYCIDRU_SYCIDRU_Pos        (0UL)          /*!< SYCIDRU (Bit 0)                                       */
#define R_ETHERC_EPTPC_SYCIDRU_SYCIDRU_Msk        (0xffffffffUL) /*!< SYCIDRU (Bitfield-Mask: 0xffffffff)                   */
/* ========================================================  SYCIDRL  ======================================================== */
#define R_ETHERC_EPTPC_SYCIDRL_SYCIDRL_Pos        (0UL)          /*!< SYCIDRL (Bit 0)                                       */
#define R_ETHERC_EPTPC_SYCIDRL_SYCIDRL_Msk        (0xffffffffUL) /*!< SYCIDRL (Bitfield-Mask: 0xffffffff)                   */
/* ========================================================  SYPNUMR  ======================================================== */
#define R_ETHERC_EPTPC_SYPNUMR_PNUM_Pos           (0UL)          /*!< PNUM (Bit 0)                                          */
#define R_ETHERC_EPTPC_SYPNUMR_PNUM_Msk           (0xffffUL)     /*!< PNUM (Bitfield-Mask: 0xffff)                          */
/* ========================================================  SYRVLDR  ======================================================== */
#define R_ETHERC_EPTPC_SYRVLDR_ANUP_Pos           (2UL)          /*!< ANUP (Bit 2)                                          */
#define R_ETHERC_EPTPC_SYRVLDR_ANUP_Msk           (0x4UL)        /*!< ANUP (Bitfield-Mask: 0x01)                            */
#define R_ETHERC_EPTPC_SYRVLDR_STUP_Pos           (1UL)          /*!< STUP (Bit 1)                                          */
#define R_ETHERC_EPTPC_SYRVLDR_STUP_Msk           (0x2UL)        /*!< STUP (Bitfield-Mask: 0x01)                            */
#define R_ETHERC_EPTPC_SYRVLDR_BMUP_Pos           (0UL)          /*!< BMUP (Bit 0)                                          */
#define R_ETHERC_EPTPC_SYRVLDR_BMUP_Msk           (0x1UL)        /*!< BMUP (Bitfield-Mask: 0x01)                            */
/* ========================================================  SYRFL1R  ======================================================== */
#define R_ETHERC_EPTPC_SYRFL1R_PDFUP2_Pos         (30UL)         /*!< PDFUP2 (Bit 30)                                       */
#define R_ETHERC_EPTPC_SYRFL1R_PDFUP2_Msk         (0x40000000UL) /*!< PDFUP2 (Bitfield-Mask: 0x01)                          */
#define R_ETHERC_EPTPC_SYRFL1R_PDFUP1_Pos         (29UL)         /*!< PDFUP1 (Bit 29)                                       */
#define R_ETHERC_EPTPC_SYRFL1R_PDFUP1_Msk         (0x20000000UL) /*!< PDFUP1 (Bitfield-Mask: 0x01)                          */
#define R_ETHERC_EPTPC_SYRFL1R_PDFUP0_Pos         (28UL)         /*!< PDFUP0 (Bit 28)                                       */
#define R_ETHERC_EPTPC_SYRFL1R_PDFUP0_Msk         (0x10000000UL) /*!< PDFUP0 (Bitfield-Mask: 0x01)                          */
#define R_ETHERC_EPTPC_SYRFL1R_PDRP2_Pos          (26UL)         /*!< PDRP2 (Bit 26)                                        */
#define R_ETHERC_EPTPC_SYRFL1R_PDRP2_Msk          (0x4000000UL)  /*!< PDRP2 (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_SYRFL1R_PDRP1_Pos          (25UL)         /*!< PDRP1 (Bit 25)                                        */
#define R_ETHERC_EPTPC_SYRFL1R_PDRP1_Msk          (0x2000000UL)  /*!< PDRP1 (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_SYRFL1R_PDRP0_Pos          (24UL)         /*!< PDRP0 (Bit 24)                                        */
#define R_ETHERC_EPTPC_SYRFL1R_PDRP0_Msk          (0x1000000UL)  /*!< PDRP0 (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_SYRFL1R_PDRQ2_Pos          (22UL)         /*!< PDRQ2 (Bit 22)                                        */
#define R_ETHERC_EPTPC_SYRFL1R_PDRQ2_Msk          (0x400000UL)   /*!< PDRQ2 (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_SYRFL1R_PDRQ1_Pos          (21UL)         /*!< PDRQ1 (Bit 21)                                        */
#define R_ETHERC_EPTPC_SYRFL1R_PDRQ1_Msk          (0x200000UL)   /*!< PDRQ1 (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_SYRFL1R_PDRQ0_Pos          (20UL)         /*!< PDRQ0 (Bit 20)                                        */
#define R_ETHERC_EPTPC_SYRFL1R_PDRQ0_Msk          (0x100000UL)   /*!< PDRQ0 (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_SYRFL1R_DRP2_Pos           (18UL)         /*!< DRP2 (Bit 18)                                         */
#define R_ETHERC_EPTPC_SYRFL1R_DRP2_Msk           (0x40000UL)    /*!< DRP2 (Bitfield-Mask: 0x01)                            */
#define R_ETHERC_EPTPC_SYRFL1R_DRP1_Pos           (17UL)         /*!< DRP1 (Bit 17)                                         */
#define R_ETHERC_EPTPC_SYRFL1R_DRP1_Msk           (0x20000UL)    /*!< DRP1 (Bitfield-Mask: 0x01)                            */
#define R_ETHERC_EPTPC_SYRFL1R_DRP0_Pos           (16UL)         /*!< DRP0 (Bit 16)                                         */
#define R_ETHERC_EPTPC_SYRFL1R_DRP0_Msk           (0x10000UL)    /*!< DRP0 (Bitfield-Mask: 0x01)                            */
#define R_ETHERC_EPTPC_SYRFL1R_DRQ2_Pos           (14UL)         /*!< DRQ2 (Bit 14)                                         */
#define R_ETHERC_EPTPC_SYRFL1R_DRQ2_Msk           (0x4000UL)     /*!< DRQ2 (Bitfield-Mask: 0x01)                            */
#define R_ETHERC_EPTPC_SYRFL1R_DRQ1_Pos           (13UL)         /*!< DRQ1 (Bit 13)                                         */
#define R_ETHERC_EPTPC_SYRFL1R_DRQ1_Msk           (0x2000UL)     /*!< DRQ1 (Bitfield-Mask: 0x01)                            */
#define R_ETHERC_EPTPC_SYRFL1R_DRQ0_Pos           (12UL)         /*!< DRQ0 (Bit 12)                                         */
#define R_ETHERC_EPTPC_SYRFL1R_DRQ0_Msk           (0x1000UL)     /*!< DRQ0 (Bitfield-Mask: 0x01)                            */
#define R_ETHERC_EPTPC_SYRFL1R_FUP2_Pos           (10UL)         /*!< FUP2 (Bit 10)                                         */
#define R_ETHERC_EPTPC_SYRFL1R_FUP2_Msk           (0x400UL)      /*!< FUP2 (Bitfield-Mask: 0x01)                            */
#define R_ETHERC_EPTPC_SYRFL1R_FUP1_Pos           (9UL)          /*!< FUP1 (Bit 9)                                          */
#define R_ETHERC_EPTPC_SYRFL1R_FUP1_Msk           (0x200UL)      /*!< FUP1 (Bitfield-Mask: 0x01)                            */
#define R_ETHERC_EPTPC_SYRFL1R_FUP0_Pos           (8UL)          /*!< FUP0 (Bit 8)                                          */
#define R_ETHERC_EPTPC_SYRFL1R_FUP0_Msk           (0x100UL)      /*!< FUP0 (Bitfield-Mask: 0x01)                            */
#define R_ETHERC_EPTPC_SYRFL1R_SYNC2_Pos          (6UL)          /*!< SYNC2 (Bit 6)                                         */
#define R_ETHERC_EPTPC_SYRFL1R_SYNC2_Msk          (0x40UL)       /*!< SYNC2 (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_SYRFL1R_SYNC1_Pos          (5UL)          /*!< SYNC1 (Bit 5)                                         */
#define R_ETHERC_EPTPC_SYRFL1R_SYNC1_Msk          (0x20UL)       /*!< SYNC1 (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_SYRFL1R_SYNC0_Pos          (4UL)          /*!< SYNC0 (Bit 4)                                         */
#define R_ETHERC_EPTPC_SYRFL1R_SYNC0_Msk          (0x10UL)       /*!< SYNC0 (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_SYRFL1R_ANCE1_Pos          (1UL)          /*!< ANCE1 (Bit 1)                                         */
#define R_ETHERC_EPTPC_SYRFL1R_ANCE1_Msk          (0x2UL)        /*!< ANCE1 (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_SYRFL1R_ANCE0_Pos          (0UL)          /*!< ANCE0 (Bit 0)                                         */
#define R_ETHERC_EPTPC_SYRFL1R_ANCE0_Msk          (0x1UL)        /*!< ANCE0 (Bitfield-Mask: 0x01)                           */
/* ========================================================  SYRFL2R  ======================================================== */
#define R_ETHERC_EPTPC_SYRFL2R_ILL1_Pos           (29UL)         /*!< ILL1 (Bit 29)                                         */
#define R_ETHERC_EPTPC_SYRFL2R_ILL1_Msk           (0x20000000UL) /*!< ILL1 (Bitfield-Mask: 0x01)                            */
#define R_ETHERC_EPTPC_SYRFL2R_ILL0_Pos           (28UL)         /*!< ILL0 (Bit 28)                                         */
#define R_ETHERC_EPTPC_SYRFL2R_ILL0_Msk           (0x10000000UL) /*!< ILL0 (Bitfield-Mask: 0x01)                            */
#define R_ETHERC_EPTPC_SYRFL2R_SIG1_Pos           (5UL)          /*!< SIG1 (Bit 5)                                          */
#define R_ETHERC_EPTPC_SYRFL2R_SIG1_Msk           (0x20UL)       /*!< SIG1 (Bitfield-Mask: 0x01)                            */
#define R_ETHERC_EPTPC_SYRFL2R_SIG0_Pos           (4UL)          /*!< SIG0 (Bit 4)                                          */
#define R_ETHERC_EPTPC_SYRFL2R_SIG0_Msk           (0x10UL)       /*!< SIG0 (Bitfield-Mask: 0x01)                            */
#define R_ETHERC_EPTPC_SYRFL2R_MAN1_Pos           (1UL)          /*!< MAN1 (Bit 1)                                          */
#define R_ETHERC_EPTPC_SYRFL2R_MAN1_Msk           (0x2UL)        /*!< MAN1 (Bitfield-Mask: 0x01)                            */
#define R_ETHERC_EPTPC_SYRFL2R_MAN0_Pos           (0UL)          /*!< MAN0 (Bit 0)                                          */
#define R_ETHERC_EPTPC_SYRFL2R_MAN0_Msk           (0x1UL)        /*!< MAN0 (Bitfield-Mask: 0x01)                            */
/* ========================================================  SYTRENR  ======================================================== */
#define R_ETHERC_EPTPC_SYTRENR_PDRQ_Pos           (12UL)         /*!< PDRQ (Bit 12)                                         */
#define R_ETHERC_EPTPC_SYTRENR_PDRQ_Msk           (0x1000UL)     /*!< PDRQ (Bitfield-Mask: 0x01)                            */
#define R_ETHERC_EPTPC_SYTRENR_DRQ_Pos            (8UL)          /*!< DRQ (Bit 8)                                           */
#define R_ETHERC_EPTPC_SYTRENR_DRQ_Msk            (0x100UL)      /*!< DRQ (Bitfield-Mask: 0x01)                             */
#define R_ETHERC_EPTPC_SYTRENR_SYNC_Pos           (4UL)          /*!< SYNC (Bit 4)                                          */
#define R_ETHERC_EPTPC_SYTRENR_SYNC_Msk           (0x10UL)       /*!< SYNC (Bitfield-Mask: 0x01)                            */
#define R_ETHERC_EPTPC_SYTRENR_ANCE_Pos           (0UL)          /*!< ANCE (Bit 0)                                          */
#define R_ETHERC_EPTPC_SYTRENR_ANCE_Msk           (0x1UL)        /*!< ANCE (Bitfield-Mask: 0x01)                            */
/* ========================================================  MTCIDU  ========================================================= */
#define R_ETHERC_EPTPC_MTCIDU_MTCIDU_Pos          (0UL)          /*!< MTCIDU (Bit 0)                                        */
#define R_ETHERC_EPTPC_MTCIDU_MTCIDU_Msk          (0xffffffffUL) /*!< MTCIDU (Bitfield-Mask: 0xffffffff)                    */
/* ========================================================  MTCIDL  ========================================================= */
#define R_ETHERC_EPTPC_MTCIDL_MTCIDL_Pos          (0UL)          /*!< MTCIDL (Bit 0)                                        */
#define R_ETHERC_EPTPC_MTCIDL_MTCIDL_Msk          (0xffffffffUL) /*!< MTCIDL (Bitfield-Mask: 0xffffffff)                    */
/* =========================================================  MTPID  ========================================================= */
#define R_ETHERC_EPTPC_MTPID_PNUM_Pos             (0UL)          /*!< PNUM (Bit 0)                                          */
#define R_ETHERC_EPTPC_MTPID_PNUM_Msk             (0xffffUL)     /*!< PNUM (Bitfield-Mask: 0xffff)                          */
/* ========================================================  SYTLIR  ========================================================= */
#define R_ETHERC_EPTPC_SYTLIR_DREQ_Pos            (16UL)         /*!< DREQ (Bit 16)                                         */
#define R_ETHERC_EPTPC_SYTLIR_DREQ_Msk            (0xff0000UL)   /*!< DREQ (Bitfield-Mask: 0xff)                            */
#define R_ETHERC_EPTPC_SYTLIR_SYNC_Pos            (8UL)          /*!< SYNC (Bit 8)                                          */
#define R_ETHERC_EPTPC_SYTLIR_SYNC_Msk            (0xff00UL)     /*!< SYNC (Bitfield-Mask: 0xff)                            */
#define R_ETHERC_EPTPC_SYTLIR_ANCE_Pos            (0UL)          /*!< ANCE (Bit 0)                                          */
#define R_ETHERC_EPTPC_SYTLIR_ANCE_Msk            (0xffUL)       /*!< ANCE (Bitfield-Mask: 0xff)                            */
/* ========================================================  SYRLIR  ========================================================= */
#define R_ETHERC_EPTPC_SYRLIR_DRESP_Pos           (16UL)         /*!< DRESP (Bit 16)                                        */
#define R_ETHERC_EPTPC_SYRLIR_DRESP_Msk           (0xff0000UL)   /*!< DRESP (Bitfield-Mask: 0xff)                           */
#define R_ETHERC_EPTPC_SYRLIR_SYNC_Pos            (8UL)          /*!< SYNC (Bit 8)                                          */
#define R_ETHERC_EPTPC_SYRLIR_SYNC_Msk            (0xff00UL)     /*!< SYNC (Bitfield-Mask: 0xff)                            */
#define R_ETHERC_EPTPC_SYRLIR_ANCE_Pos            (0UL)          /*!< ANCE (Bit 0)                                          */
#define R_ETHERC_EPTPC_SYRLIR_ANCE_Msk            (0xffUL)       /*!< ANCE (Bitfield-Mask: 0xff)                            */
/* =========================================================  OFMRU  ========================================================= */
#define R_ETHERC_EPTPC_OFMRU_OFMRU_Pos            (0UL)          /*!< OFMRU (Bit 0)                                         */
#define R_ETHERC_EPTPC_OFMRU_OFMRU_Msk            (0xffffffffUL) /*!< OFMRU (Bitfield-Mask: 0xffffffff)                     */
/* =========================================================  OFMRL  ========================================================= */
#define R_ETHERC_EPTPC_OFMRL_OFMRL_Pos            (0UL)          /*!< OFMRL (Bit 0)                                         */
#define R_ETHERC_EPTPC_OFMRL_OFMRL_Msk            (0xffffffffUL) /*!< OFMRL (Bitfield-Mask: 0xffffffff)                     */
/* =========================================================  MPDRU  ========================================================= */
#define R_ETHERC_EPTPC_MPDRU_MPDRU_Pos            (0UL)          /*!< MPDRU (Bit 0)                                         */
#define R_ETHERC_EPTPC_MPDRU_MPDRU_Msk            (0xffffffffUL) /*!< MPDRU (Bitfield-Mask: 0xffffffff)                     */
/* =========================================================  MPDRL  ========================================================= */
#define R_ETHERC_EPTPC_MPDRL_MPDRL_Pos            (0UL)          /*!< MPDRL (Bit 0)                                         */
#define R_ETHERC_EPTPC_MPDRL_MPDRL_Msk            (0xffffffffUL) /*!< MPDRL (Bitfield-Mask: 0xffffffff)                     */
/* =========================================================  GMPR  ========================================================== */
#define R_ETHERC_EPTPC_GMPR_GMPR1_Pos             (16UL)         /*!< GMPR1 (Bit 16)                                        */
#define R_ETHERC_EPTPC_GMPR_GMPR1_Msk             (0xff0000UL)   /*!< GMPR1 (Bitfield-Mask: 0xff)                           */
#define R_ETHERC_EPTPC_GMPR_GMPR2_Pos             (0UL)          /*!< GMPR2 (Bit 0)                                         */
#define R_ETHERC_EPTPC_GMPR_GMPR2_Msk             (0xffUL)       /*!< GMPR2 (Bitfield-Mask: 0xff)                           */
/* =========================================================  GMCQR  ========================================================= */
#define R_ETHERC_EPTPC_GMCQR_GMCQR_Pos            (0UL)          /*!< GMCQR (Bit 0)                                         */
#define R_ETHERC_EPTPC_GMCQR_GMCQR_Msk            (0xffffffffUL) /*!< GMCQR (Bitfield-Mask: 0xffffffff)                     */
/* ========================================================  GMIDRU  ========================================================= */
#define R_ETHERC_EPTPC_GMIDRU_GMIDRU_Pos          (0UL)          /*!< GMIDRU (Bit 0)                                        */
#define R_ETHERC_EPTPC_GMIDRU_GMIDRU_Msk          (0xffffffffUL) /*!< GMIDRU (Bitfield-Mask: 0xffffffff)                    */
/* ========================================================  GMIDRL  ========================================================= */
#define R_ETHERC_EPTPC_GMIDRL_GMIDRL_Pos          (0UL)          /*!< GMIDRL (Bit 0)                                        */
#define R_ETHERC_EPTPC_GMIDRL_GMIDRL_Msk          (0xffffffffUL) /*!< GMIDRL (Bitfield-Mask: 0xffffffff)                    */
/* ========================================================  CUOTSR  ========================================================= */
#define R_ETHERC_EPTPC_CUOTSR_CUTO_Pos            (16UL)         /*!< CUTO (Bit 16)                                         */
#define R_ETHERC_EPTPC_CUOTSR_CUTO_Msk            (0xffff0000UL) /*!< CUTO (Bitfield-Mask: 0xffff)                          */
#define R_ETHERC_EPTPC_CUOTSR_TSRC_Pos            (0UL)          /*!< TSRC (Bit 0)                                          */
#define R_ETHERC_EPTPC_CUOTSR_TSRC_Msk            (0xffUL)       /*!< TSRC (Bitfield-Mask: 0xff)                            */
/* ==========================================================  SRR  ========================================================== */
#define R_ETHERC_EPTPC_SRR_SRMV_Pos               (0UL)          /*!< SRMV (Bit 0)                                          */
#define R_ETHERC_EPTPC_SRR_SRMV_Msk               (0xffffUL)     /*!< SRMV (Bitfield-Mask: 0xffff)                          */
/* ========================================================  PPMACRU  ======================================================== */
#define R_ETHERC_EPTPC_PPMACRU_PPMACRU_Pos        (0UL)          /*!< PPMACRU (Bit 0)                                       */
#define R_ETHERC_EPTPC_PPMACRU_PPMACRU_Msk        (0xffffffUL)   /*!< PPMACRU (Bitfield-Mask: 0xffffff)                     */
/* ========================================================  PPMACRL  ======================================================== */
#define R_ETHERC_EPTPC_PPMACRL_PPMACRL_Pos        (0UL)          /*!< PPMACRL (Bit 0)                                       */
#define R_ETHERC_EPTPC_PPMACRL_PPMACRL_Msk        (0xffffffUL)   /*!< PPMACRL (Bitfield-Mask: 0xffffff)                     */
/* ========================================================  PDMACRU  ======================================================== */
#define R_ETHERC_EPTPC_PDMACRU_PDMACRU_Pos        (0UL)          /*!< PDMACRU (Bit 0)                                       */
#define R_ETHERC_EPTPC_PDMACRU_PDMACRU_Msk        (0xffffffUL)   /*!< PDMACRU (Bitfield-Mask: 0xffffff)                     */
/* ========================================================  PDMACRL  ======================================================== */
#define R_ETHERC_EPTPC_PDMACRL_PDMACRL_Pos        (0UL)          /*!< PDMACRL (Bit 0)                                       */
#define R_ETHERC_EPTPC_PDMACRL_PDMACRL_Msk        (0xffffffUL)   /*!< PDMACRL (Bitfield-Mask: 0xffffff)                     */
/* ========================================================  PETYPER  ======================================================== */
#define R_ETHERC_EPTPC_PETYPER_TYPE_Pos           (0UL)          /*!< TYPE (Bit 0)                                          */
#define R_ETHERC_EPTPC_PETYPER_TYPE_Msk           (0xffffUL)     /*!< TYPE (Bitfield-Mask: 0xffff)                          */
/* =========================================================  PPIPR  ========================================================= */
#define R_ETHERC_EPTPC_PPIPR_PPIPR_Pos            (0UL)          /*!< PPIPR (Bit 0)                                         */
#define R_ETHERC_EPTPC_PPIPR_PPIPR_Msk            (0xffffffffUL) /*!< PPIPR (Bitfield-Mask: 0xffffffff)                     */
/* =========================================================  PDIPR  ========================================================= */
#define R_ETHERC_EPTPC_PDIPR_PDIPR_Pos            (0UL)          /*!< PDIPR (Bit 0)                                         */
#define R_ETHERC_EPTPC_PDIPR_PDIPR_Msk            (0xffffffffUL) /*!< PDIPR (Bitfield-Mask: 0xffffffff)                     */
/* ========================================================  PETOSR  ========================================================= */
#define R_ETHERC_EPTPC_PETOSR_EVTO_Pos            (0UL)          /*!< EVTO (Bit 0)                                          */
#define R_ETHERC_EPTPC_PETOSR_EVTO_Msk            (0xffUL)       /*!< EVTO (Bitfield-Mask: 0xff)                            */
/* ========================================================  PGTOSR  ========================================================= */
#define R_ETHERC_EPTPC_PGTOSR_GETO_Pos            (0UL)          /*!< GETO (Bit 0)                                          */
#define R_ETHERC_EPTPC_PGTOSR_GETO_Msk            (0xffUL)       /*!< GETO (Bitfield-Mask: 0xff)                            */
/* ========================================================  PPTTLR  ========================================================= */
#define R_ETHERC_EPTPC_PPTTLR_PRTL_Pos            (0UL)          /*!< PRTL (Bit 0)                                          */
#define R_ETHERC_EPTPC_PPTTLR_PRTL_Msk            (0xffUL)       /*!< PRTL (Bitfield-Mask: 0xff)                            */
/* ========================================================  PDTTLR  ========================================================= */
#define R_ETHERC_EPTPC_PDTTLR_PDTL_Pos            (0UL)          /*!< PDTL (Bit 0)                                          */
#define R_ETHERC_EPTPC_PDTTLR_PDTL_Msk            (0xffUL)       /*!< PDTL (Bitfield-Mask: 0xff)                            */
/* ========================================================  PEUDPR  ========================================================= */
#define R_ETHERC_EPTPC_PEUDPR_EVUPT_Pos           (0UL)          /*!< EVUPT (Bit 0)                                         */
#define R_ETHERC_EPTPC_PEUDPR_EVUPT_Msk           (0xffffUL)     /*!< EVUPT (Bitfield-Mask: 0xffff)                         */
/* ========================================================  PGUDPR  ========================================================= */
#define R_ETHERC_EPTPC_PGUDPR_GEUPT_Pos           (0UL)          /*!< GEUPT (Bit 0)                                         */
#define R_ETHERC_EPTPC_PGUDPR_GEUPT_Msk           (0xffffUL)     /*!< GEUPT (Bitfield-Mask: 0xffff)                         */
/* =========================================================  FFLTR  ========================================================= */
#define R_ETHERC_EPTPC_FFLTR_EXTPRM_Pos           (16UL)         /*!< EXTPRM (Bit 16)                                       */
#define R_ETHERC_EPTPC_FFLTR_EXTPRM_Msk           (0x10000UL)    /*!< EXTPRM (Bitfield-Mask: 0x01)                          */
#define R_ETHERC_EPTPC_FFLTR_ENB_Pos              (2UL)          /*!< ENB (Bit 2)                                           */
#define R_ETHERC_EPTPC_FFLTR_ENB_Msk              (0x4UL)        /*!< ENB (Bitfield-Mask: 0x01)                             */
#define R_ETHERC_EPTPC_FFLTR_PRT_Pos              (1UL)          /*!< PRT (Bit 1)                                           */
#define R_ETHERC_EPTPC_FFLTR_PRT_Msk              (0x2UL)        /*!< PRT (Bitfield-Mask: 0x01)                             */
#define R_ETHERC_EPTPC_FFLTR_SEL_Pos              (0UL)          /*!< SEL (Bit 0)                                           */
#define R_ETHERC_EPTPC_FFLTR_SEL_Msk              (0x1UL)        /*!< SEL (Bitfield-Mask: 0x01)                             */
/* ========================================================  FMAC0RU  ======================================================== */
#define R_ETHERC_EPTPC_FMAC0RU_FMAC0RU_Pos        (0UL)          /*!< FMAC0RU (Bit 0)                                       */
#define R_ETHERC_EPTPC_FMAC0RU_FMAC0RU_Msk        (0xffffffUL)   /*!< FMAC0RU (Bitfield-Mask: 0xffffff)                     */
/* ========================================================  FMAC0RL  ======================================================== */
#define R_ETHERC_EPTPC_FMAC0RL_FMAC0RL_Pos        (0UL)          /*!< FMAC0RL (Bit 0)                                       */
#define R_ETHERC_EPTPC_FMAC0RL_FMAC0RL_Msk        (0xffffffUL)   /*!< FMAC0RL (Bitfield-Mask: 0xffffff)                     */
/* ========================================================  FMAC1RU  ======================================================== */
#define R_ETHERC_EPTPC_FMAC1RU_FMAC1RU_Pos        (0UL)          /*!< FMAC1RU (Bit 0)                                       */
#define R_ETHERC_EPTPC_FMAC1RU_FMAC1RU_Msk        (0xffffffUL)   /*!< FMAC1RU (Bitfield-Mask: 0xffffff)                     */
/* ========================================================  FMAC1RL  ======================================================== */
#define R_ETHERC_EPTPC_FMAC1RL_FMAC1RL_Pos        (0UL)          /*!< FMAC1RL (Bit 0)                                       */
#define R_ETHERC_EPTPC_FMAC1RL_FMAC1RL_Msk        (0xffffffUL)   /*!< FMAC1RL (Bitfield-Mask: 0xffffff)                     */
/* ========================================================  DASYMRU  ======================================================== */
#define R_ETHERC_EPTPC_DASYMRU_DASYMRU_Pos        (0UL)          /*!< DASYMRU (Bit 0)                                       */
#define R_ETHERC_EPTPC_DASYMRU_DASYMRU_Msk        (0xffffUL)     /*!< DASYMRU (Bitfield-Mask: 0xffff)                       */
/* ========================================================  DASYMRL  ======================================================== */
#define R_ETHERC_EPTPC_DASYMRL_DASYMRL_Pos        (0UL)          /*!< DASYMRL (Bit 0)                                       */
#define R_ETHERC_EPTPC_DASYMRL_DASYMRL_Msk        (0xffffffffUL) /*!< DASYMRL (Bitfield-Mask: 0xffffffff)                   */
/* ========================================================  TSLATR  ========================================================= */
#define R_ETHERC_EPTPC_TSLATR_INGP_Pos            (16UL)         /*!< INGP (Bit 16)                                         */
#define R_ETHERC_EPTPC_TSLATR_INGP_Msk            (0xffff0000UL) /*!< INGP (Bitfield-Mask: 0xffff)                          */
#define R_ETHERC_EPTPC_TSLATR_EGP_Pos             (0UL)          /*!< EGP (Bit 0)                                           */
#define R_ETHERC_EPTPC_TSLATR_EGP_Msk             (0xffffUL)     /*!< EGP (Bitfield-Mask: 0xffff)                           */
/* ========================================================  SYCONFR  ======================================================== */
#define R_ETHERC_EPTPC_SYCONFR_TCMOD_Pos          (20UL)         /*!< TCMOD (Bit 20)                                        */
#define R_ETHERC_EPTPC_SYCONFR_TCMOD_Msk          (0x100000UL)   /*!< TCMOD (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_SYCONFR_FILDIS_Pos         (16UL)         /*!< FILDIS (Bit 16)                                       */
#define R_ETHERC_EPTPC_SYCONFR_FILDIS_Msk         (0x10000UL)    /*!< FILDIS (Bitfield-Mask: 0x01)                          */
#define R_ETHERC_EPTPC_SYCONFR_SBDIS_Pos          (12UL)         /*!< SBDIS (Bit 12)                                        */
#define R_ETHERC_EPTPC_SYCONFR_SBDIS_Msk          (0x1000UL)     /*!< SBDIS (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_SYCONFR_TCYC_Pos           (0UL)          /*!< TCYC (Bit 0)                                          */
#define R_ETHERC_EPTPC_SYCONFR_TCYC_Msk           (0xffUL)       /*!< TCYC (Bitfield-Mask: 0xff)                            */
/* ========================================================  SYFORMR  ======================================================== */
#define R_ETHERC_EPTPC_SYFORMR_FORM1_Pos          (1UL)          /*!< FORM1 (Bit 1)                                         */
#define R_ETHERC_EPTPC_SYFORMR_FORM1_Msk          (0x2UL)        /*!< FORM1 (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_SYFORMR_FORM0_Pos          (0UL)          /*!< FORM0 (Bit 0)                                         */
#define R_ETHERC_EPTPC_SYFORMR_FORM0_Msk          (0x1UL)        /*!< FORM0 (Bitfield-Mask: 0x01)                           */
/* ========================================================  RSTOUTR  ======================================================== */
#define R_ETHERC_EPTPC_RSTOUTR_RSTOUTR_Pos        (0UL)          /*!< RSTOUTR (Bit 0)                                       */
#define R_ETHERC_EPTPC_RSTOUTR_RSTOUTR_Msk        (0xffffffffUL) /*!< RSTOUTR (Bitfield-Mask: 0xffffffff)                   */

/* =========================================================================================================================== */
/* ================                                    R_ETHERC_EPTPC_CFG                                     ================ */
/* =========================================================================================================================== */

/* ========================================================  PTRSTR  ========================================================= */
#define R_ETHERC_EPTPC_CFG_PTRSTR_RESET_Pos       (0UL)       /*!< RESET (Bit 0)                                         */
#define R_ETHERC_EPTPC_CFG_PTRSTR_RESET_Msk       (0x1UL)     /*!< RESET (Bitfield-Mask: 0x01)                           */
/* ========================================================  STCSELR  ======================================================== */
#define R_ETHERC_EPTPC_CFG_STCSELR_SCLKSEL_Pos    (8UL)       /*!< SCLKSEL (Bit 8)                                       */
#define R_ETHERC_EPTPC_CFG_STCSELR_SCLKSEL_Msk    (0x700UL)   /*!< SCLKSEL (Bitfield-Mask: 0x07)                         */
#define R_ETHERC_EPTPC_CFG_STCSELR_SCLKDIV_Pos    (0UL)       /*!< SCLKDIV (Bit 0)                                       */
#define R_ETHERC_EPTPC_CFG_STCSELR_SCLKDIV_Msk    (0x7UL)     /*!< SCLKDIV (Bitfield-Mask: 0x07)                         */
/* ========================================================  BYPASS  ========================================================= */
#define R_ETHERC_EPTPC_CFG_BYPASS_BYPASS1_Pos     (16UL)      /*!< BYPASS1 (Bit 16)                                      */
#define R_ETHERC_EPTPC_CFG_BYPASS_BYPASS1_Msk     (0x10000UL) /*!< BYPASS1 (Bitfield-Mask: 0x01)                         */
#define R_ETHERC_EPTPC_CFG_BYPASS_BYPASS0_Pos     (0UL)       /*!< BYPASS0 (Bit 0)                                       */
#define R_ETHERC_EPTPC_CFG_BYPASS_BYPASS0_Msk     (0x1UL)     /*!< BYPASS0 (Bitfield-Mask: 0x01)                         */

/* =========================================================================================================================== */
/* ================                                   R_ETHERC_EPTPC_COMMON                                   ================ */
/* =========================================================================================================================== */

/* =========================================================  MIESR  ========================================================= */
#define R_ETHERC_EPTPC_COMMON_MIESR_CYC5_Pos           (21UL)         /*!< CYC5 (Bit 21)                                         */
#define R_ETHERC_EPTPC_COMMON_MIESR_CYC5_Msk           (0x200000UL)   /*!< CYC5 (Bitfield-Mask: 0x01)                            */
#define R_ETHERC_EPTPC_COMMON_MIESR_CYC4_Pos           (20UL)         /*!< CYC4 (Bit 20)                                         */
#define R_ETHERC_EPTPC_COMMON_MIESR_CYC4_Msk           (0x100000UL)   /*!< CYC4 (Bitfield-Mask: 0x01)                            */
#define R_ETHERC_EPTPC_COMMON_MIESR_CYC3_Pos           (19UL)         /*!< CYC3 (Bit 19)                                         */
#define R_ETHERC_EPTPC_COMMON_MIESR_CYC3_Msk           (0x80000UL)    /*!< CYC3 (Bitfield-Mask: 0x01)                            */
#define R_ETHERC_EPTPC_COMMON_MIESR_CYC2_Pos           (18UL)         /*!< CYC2 (Bit 18)                                         */
#define R_ETHERC_EPTPC_COMMON_MIESR_CYC2_Msk           (0x40000UL)    /*!< CYC2 (Bitfield-Mask: 0x01)                            */
#define R_ETHERC_EPTPC_COMMON_MIESR_CYC1_Pos           (17UL)         /*!< CYC1 (Bit 17)                                         */
#define R_ETHERC_EPTPC_COMMON_MIESR_CYC1_Msk           (0x20000UL)    /*!< CYC1 (Bitfield-Mask: 0x01)                            */
#define R_ETHERC_EPTPC_COMMON_MIESR_CYC0_Pos           (16UL)         /*!< CYC0 (Bit 16)                                         */
#define R_ETHERC_EPTPC_COMMON_MIESR_CYC0_Msk           (0x10000UL)    /*!< CYC0 (Bitfield-Mask: 0x01)                            */
#define R_ETHERC_EPTPC_COMMON_MIESR_PRC_Pos            (3UL)          /*!< PRC (Bit 3)                                           */
#define R_ETHERC_EPTPC_COMMON_MIESR_PRC_Msk            (0x8UL)        /*!< PRC (Bitfield-Mask: 0x01)                             */
#define R_ETHERC_EPTPC_COMMON_MIESR_SY1_Pos            (2UL)          /*!< SY1 (Bit 2)                                           */
#define R_ETHERC_EPTPC_COMMON_MIESR_SY1_Msk            (0x4UL)        /*!< SY1 (Bitfield-Mask: 0x01)                             */
#define R_ETHERC_EPTPC_COMMON_MIESR_SY0_Pos            (1UL)          /*!< SY0 (Bit 1)                                           */
#define R_ETHERC_EPTPC_COMMON_MIESR_SY0_Msk            (0x2UL)        /*!< SY0 (Bitfield-Mask: 0x01)                             */
#define R_ETHERC_EPTPC_COMMON_MIESR_ST_Pos             (0UL)          /*!< ST (Bit 0)                                            */
#define R_ETHERC_EPTPC_COMMON_MIESR_ST_Msk             (0x1UL)        /*!< ST (Bitfield-Mask: 0x01)                              */
/* ========================================================  MIEIPR  ========================================================= */
#define R_ETHERC_EPTPC_COMMON_MIEIPR_CYC5_Pos          (21UL)         /*!< CYC5 (Bit 21)                                         */
#define R_ETHERC_EPTPC_COMMON_MIEIPR_CYC5_Msk          (0x200000UL)   /*!< CYC5 (Bitfield-Mask: 0x01)                            */
#define R_ETHERC_EPTPC_COMMON_MIEIPR_CYC4_Pos          (20UL)         /*!< CYC4 (Bit 20)                                         */
#define R_ETHERC_EPTPC_COMMON_MIEIPR_CYC4_Msk          (0x100000UL)   /*!< CYC4 (Bitfield-Mask: 0x01)                            */
#define R_ETHERC_EPTPC_COMMON_MIEIPR_CYC3_Pos          (19UL)         /*!< CYC3 (Bit 19)                                         */
#define R_ETHERC_EPTPC_COMMON_MIEIPR_CYC3_Msk          (0x80000UL)    /*!< CYC3 (Bitfield-Mask: 0x01)                            */
#define R_ETHERC_EPTPC_COMMON_MIEIPR_CYC2_Pos          (18UL)         /*!< CYC2 (Bit 18)                                         */
#define R_ETHERC_EPTPC_COMMON_MIEIPR_CYC2_Msk          (0x40000UL)    /*!< CYC2 (Bitfield-Mask: 0x01)                            */
#define R_ETHERC_EPTPC_COMMON_MIEIPR_CYC1_Pos          (17UL)         /*!< CYC1 (Bit 17)                                         */
#define R_ETHERC_EPTPC_COMMON_MIEIPR_CYC1_Msk          (0x20000UL)    /*!< CYC1 (Bitfield-Mask: 0x01)                            */
#define R_ETHERC_EPTPC_COMMON_MIEIPR_CYC0_Pos          (16UL)         /*!< CYC0 (Bit 16)                                         */
#define R_ETHERC_EPTPC_COMMON_MIEIPR_CYC0_Msk          (0x10000UL)    /*!< CYC0 (Bitfield-Mask: 0x01)                            */
#define R_ETHERC_EPTPC_COMMON_MIEIPR_PRC_Pos           (3UL)          /*!< PRC (Bit 3)                                           */
#define R_ETHERC_EPTPC_COMMON_MIEIPR_PRC_Msk           (0x8UL)        /*!< PRC (Bitfield-Mask: 0x01)                             */
#define R_ETHERC_EPTPC_COMMON_MIEIPR_SY1_Pos           (2UL)          /*!< SY1 (Bit 2)                                           */
#define R_ETHERC_EPTPC_COMMON_MIEIPR_SY1_Msk           (0x4UL)        /*!< SY1 (Bitfield-Mask: 0x01)                             */
#define R_ETHERC_EPTPC_COMMON_MIEIPR_SY0_Pos           (1UL)          /*!< SY0 (Bit 1)                                           */
#define R_ETHERC_EPTPC_COMMON_MIEIPR_SY0_Msk           (0x2UL)        /*!< SY0 (Bitfield-Mask: 0x01)                             */
#define R_ETHERC_EPTPC_COMMON_MIEIPR_ST_Pos            (0UL)          /*!< ST (Bit 0)                                            */
#define R_ETHERC_EPTPC_COMMON_MIEIPR_ST_Msk            (0x1UL)        /*!< ST (Bitfield-Mask: 0x01)                              */
/* ========================================================  ELIPPR  ========================================================= */
#define R_ETHERC_EPTPC_COMMON_ELIPPR_PLSN_Pos          (24UL)         /*!< PLSN (Bit 24)                                         */
#define R_ETHERC_EPTPC_COMMON_ELIPPR_PLSN_Msk          (0x1000000UL)  /*!< PLSN (Bitfield-Mask: 0x01)                            */
#define R_ETHERC_EPTPC_COMMON_ELIPPR_PLSP_Pos          (16UL)         /*!< PLSP (Bit 16)                                         */
#define R_ETHERC_EPTPC_COMMON_ELIPPR_PLSP_Msk          (0x10000UL)    /*!< PLSP (Bitfield-Mask: 0x01)                            */
#define R_ETHERC_EPTPC_COMMON_ELIPPR_CYCN5_Pos         (13UL)         /*!< CYCN5 (Bit 13)                                        */
#define R_ETHERC_EPTPC_COMMON_ELIPPR_CYCN5_Msk         (0x2000UL)     /*!< CYCN5 (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_COMMON_ELIPPR_CYCN4_Pos         (12UL)         /*!< CYCN4 (Bit 12)                                        */
#define R_ETHERC_EPTPC_COMMON_ELIPPR_CYCN4_Msk         (0x1000UL)     /*!< CYCN4 (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_COMMON_ELIPPR_CYCN3_Pos         (11UL)         /*!< CYCN3 (Bit 11)                                        */
#define R_ETHERC_EPTPC_COMMON_ELIPPR_CYCN3_Msk         (0x800UL)      /*!< CYCN3 (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_COMMON_ELIPPR_CYCN2_Pos         (10UL)         /*!< CYCN2 (Bit 10)                                        */
#define R_ETHERC_EPTPC_COMMON_ELIPPR_CYCN2_Msk         (0x400UL)      /*!< CYCN2 (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_COMMON_ELIPPR_CYCN1_Pos         (9UL)          /*!< CYCN1 (Bit 9)                                         */
#define R_ETHERC_EPTPC_COMMON_ELIPPR_CYCN1_Msk         (0x200UL)      /*!< CYCN1 (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_COMMON_ELIPPR_CYCN0_Pos         (8UL)          /*!< CYCN0 (Bit 8)                                         */
#define R_ETHERC_EPTPC_COMMON_ELIPPR_CYCN0_Msk         (0x100UL)      /*!< CYCN0 (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_COMMON_ELIPPR_CYCP5_Pos         (5UL)          /*!< CYCP5 (Bit 5)                                         */
#define R_ETHERC_EPTPC_COMMON_ELIPPR_CYCP5_Msk         (0x20UL)       /*!< CYCP5 (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_COMMON_ELIPPR_CYCP4_Pos         (4UL)          /*!< CYCP4 (Bit 4)                                         */
#define R_ETHERC_EPTPC_COMMON_ELIPPR_CYCP4_Msk         (0x10UL)       /*!< CYCP4 (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_COMMON_ELIPPR_CYCP3_Pos         (3UL)          /*!< CYCP3 (Bit 3)                                         */
#define R_ETHERC_EPTPC_COMMON_ELIPPR_CYCP3_Msk         (0x8UL)        /*!< CYCP3 (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_COMMON_ELIPPR_CYCP2_Pos         (2UL)          /*!< CYCP2 (Bit 2)                                         */
#define R_ETHERC_EPTPC_COMMON_ELIPPR_CYCP2_Msk         (0x4UL)        /*!< CYCP2 (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_COMMON_ELIPPR_CYCP1_Pos         (1UL)          /*!< CYCP1 (Bit 1)                                         */
#define R_ETHERC_EPTPC_COMMON_ELIPPR_CYCP1_Msk         (0x2UL)        /*!< CYCP1 (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_COMMON_ELIPPR_CYCP0_Pos         (0UL)          /*!< CYCP0 (Bit 0)                                         */
#define R_ETHERC_EPTPC_COMMON_ELIPPR_CYCP0_Msk         (0x1UL)        /*!< CYCP0 (Bitfield-Mask: 0x01)                           */
/* ========================================================  ELIPACR  ======================================================== */
#define R_ETHERC_EPTPC_COMMON_ELIPACR_PLSN_Pos         (24UL)         /*!< PLSN (Bit 24)                                         */
#define R_ETHERC_EPTPC_COMMON_ELIPACR_PLSN_Msk         (0x1000000UL)  /*!< PLSN (Bitfield-Mask: 0x01)                            */
#define R_ETHERC_EPTPC_COMMON_ELIPACR_PLSP_Pos         (16UL)         /*!< PLSP (Bit 16)                                         */
#define R_ETHERC_EPTPC_COMMON_ELIPACR_PLSP_Msk         (0x10000UL)    /*!< PLSP (Bitfield-Mask: 0x01)                            */
#define R_ETHERC_EPTPC_COMMON_ELIPACR_CYCN5_Pos        (13UL)         /*!< CYCN5 (Bit 13)                                        */
#define R_ETHERC_EPTPC_COMMON_ELIPACR_CYCN5_Msk        (0x2000UL)     /*!< CYCN5 (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_COMMON_ELIPACR_CYCN4_Pos        (12UL)         /*!< CYCN4 (Bit 12)                                        */
#define R_ETHERC_EPTPC_COMMON_ELIPACR_CYCN4_Msk        (0x1000UL)     /*!< CYCN4 (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_COMMON_ELIPACR_CYCN3_Pos        (11UL)         /*!< CYCN3 (Bit 11)                                        */
#define R_ETHERC_EPTPC_COMMON_ELIPACR_CYCN3_Msk        (0x800UL)      /*!< CYCN3 (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_COMMON_ELIPACR_CYCN2_Pos        (10UL)         /*!< CYCN2 (Bit 10)                                        */
#define R_ETHERC_EPTPC_COMMON_ELIPACR_CYCN2_Msk        (0x400UL)      /*!< CYCN2 (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_COMMON_ELIPACR_CYCN1_Pos        (9UL)          /*!< CYCN1 (Bit 9)                                         */
#define R_ETHERC_EPTPC_COMMON_ELIPACR_CYCN1_Msk        (0x200UL)      /*!< CYCN1 (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_COMMON_ELIPACR_CYCN0_Pos        (8UL)          /*!< CYCN0 (Bit 8)                                         */
#define R_ETHERC_EPTPC_COMMON_ELIPACR_CYCN0_Msk        (0x100UL)      /*!< CYCN0 (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_COMMON_ELIPACR_CYCP5_Pos        (5UL)          /*!< CYCP5 (Bit 5)                                         */
#define R_ETHERC_EPTPC_COMMON_ELIPACR_CYCP5_Msk        (0x20UL)       /*!< CYCP5 (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_COMMON_ELIPACR_CYCP4_Pos        (4UL)          /*!< CYCP4 (Bit 4)                                         */
#define R_ETHERC_EPTPC_COMMON_ELIPACR_CYCP4_Msk        (0x10UL)       /*!< CYCP4 (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_COMMON_ELIPACR_CYCP3_Pos        (3UL)          /*!< CYCP3 (Bit 3)                                         */
#define R_ETHERC_EPTPC_COMMON_ELIPACR_CYCP3_Msk        (0x8UL)        /*!< CYCP3 (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_COMMON_ELIPACR_CYCP2_Pos        (2UL)          /*!< CYCP2 (Bit 2)                                         */
#define R_ETHERC_EPTPC_COMMON_ELIPACR_CYCP2_Msk        (0x4UL)        /*!< CYCP2 (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_COMMON_ELIPACR_CYCP1_Pos        (1UL)          /*!< CYCP1 (Bit 1)                                         */
#define R_ETHERC_EPTPC_COMMON_ELIPACR_CYCP1_Msk        (0x2UL)        /*!< CYCP1 (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_COMMON_ELIPACR_CYCP0_Pos        (0UL)          /*!< CYCP0 (Bit 0)                                         */
#define R_ETHERC_EPTPC_COMMON_ELIPACR_CYCP0_Msk        (0x1UL)        /*!< CYCP0 (Bitfield-Mask: 0x01)                           */
/* =========================================================  STSR  ========================================================== */
#define R_ETHERC_EPTPC_COMMON_STSR_W10D_Pos            (4UL)          /*!< W10D (Bit 4)                                          */
#define R_ETHERC_EPTPC_COMMON_STSR_W10D_Msk            (0x10UL)       /*!< W10D (Bitfield-Mask: 0x01)                            */
#define R_ETHERC_EPTPC_COMMON_STSR_SYNTOUT_Pos         (3UL)          /*!< SYNTOUT (Bit 3)                                       */
#define R_ETHERC_EPTPC_COMMON_STSR_SYNTOUT_Msk         (0x8UL)        /*!< SYNTOUT (Bitfield-Mask: 0x01)                         */
#define R_ETHERC_EPTPC_COMMON_STSR_SYNCOUT_Pos         (1UL)          /*!< SYNCOUT (Bit 1)                                       */
#define R_ETHERC_EPTPC_COMMON_STSR_SYNCOUT_Msk         (0x2UL)        /*!< SYNCOUT (Bitfield-Mask: 0x01)                         */
#define R_ETHERC_EPTPC_COMMON_STSR_SYNC_Pos            (0UL)          /*!< SYNC (Bit 0)                                          */
#define R_ETHERC_EPTPC_COMMON_STSR_SYNC_Msk            (0x1UL)        /*!< SYNC (Bitfield-Mask: 0x01)                            */
/* =========================================================  STIPR  ========================================================= */
#define R_ETHERC_EPTPC_COMMON_STIPR_W10D_Pos           (4UL)          /*!< W10D (Bit 4)                                          */
#define R_ETHERC_EPTPC_COMMON_STIPR_W10D_Msk           (0x10UL)       /*!< W10D (Bitfield-Mask: 0x01)                            */
#define R_ETHERC_EPTPC_COMMON_STIPR_SYNTOUT_Pos        (3UL)          /*!< SYNTOUT (Bit 3)                                       */
#define R_ETHERC_EPTPC_COMMON_STIPR_SYNTOUT_Msk        (0x8UL)        /*!< SYNTOUT (Bitfield-Mask: 0x01)                         */
#define R_ETHERC_EPTPC_COMMON_STIPR_SYNCOUT_Pos        (1UL)          /*!< SYNCOUT (Bit 1)                                       */
#define R_ETHERC_EPTPC_COMMON_STIPR_SYNCOUT_Msk        (0x2UL)        /*!< SYNCOUT (Bitfield-Mask: 0x01)                         */
#define R_ETHERC_EPTPC_COMMON_STIPR_SYNC_Pos           (0UL)          /*!< SYNC (Bit 0)                                          */
#define R_ETHERC_EPTPC_COMMON_STIPR_SYNC_Msk           (0x1UL)        /*!< SYNC (Bitfield-Mask: 0x01)                            */
/* =========================================================  STCFR  ========================================================= */
#define R_ETHERC_EPTPC_COMMON_STCFR_STCF_Pos           (0UL)          /*!< STCF (Bit 0)                                          */
#define R_ETHERC_EPTPC_COMMON_STCFR_STCF_Msk           (0x3UL)        /*!< STCF (Bitfield-Mask: 0x03)                            */
/* =========================================================  STMR  ========================================================== */
#define R_ETHERC_EPTPC_COMMON_STMR_ALEN1_Pos           (29UL)         /*!< ALEN1 (Bit 29)                                        */
#define R_ETHERC_EPTPC_COMMON_STMR_ALEN1_Msk           (0x20000000UL) /*!< ALEN1 (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_COMMON_STMR_ALEN0_Pos           (28UL)         /*!< ALEN0 (Bit 28)                                        */
#define R_ETHERC_EPTPC_COMMON_STMR_ALEN0_Msk           (0x10000000UL) /*!< ALEN0 (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_COMMON_STMR_DVTH_Pos            (20UL)         /*!< DVTH (Bit 20)                                         */
#define R_ETHERC_EPTPC_COMMON_STMR_DVTH_Msk            (0xf00000UL)   /*!< DVTH (Bitfield-Mask: 0x0f)                            */
#define R_ETHERC_EPTPC_COMMON_STMR_SYTH_Pos            (16UL)         /*!< SYTH (Bit 16)                                         */
#define R_ETHERC_EPTPC_COMMON_STMR_SYTH_Msk            (0xf0000UL)    /*!< SYTH (Bitfield-Mask: 0x0f)                            */
#define R_ETHERC_EPTPC_COMMON_STMR_W10S_Pos            (15UL)         /*!< W10S (Bit 15)                                         */
#define R_ETHERC_EPTPC_COMMON_STMR_W10S_Msk            (0x8000UL)     /*!< W10S (Bitfield-Mask: 0x01)                            */
#define R_ETHERC_EPTPC_COMMON_STMR_CMOD_Pos            (13UL)         /*!< CMOD (Bit 13)                                         */
#define R_ETHERC_EPTPC_COMMON_STMR_CMOD_Msk            (0x2000UL)     /*!< CMOD (Bitfield-Mask: 0x01)                            */
#define R_ETHERC_EPTPC_COMMON_STMR_WINT_Pos            (0UL)          /*!< WINT (Bit 0)                                          */
#define R_ETHERC_EPTPC_COMMON_STMR_WINT_Msk            (0xffUL)       /*!< WINT (Bitfield-Mask: 0xff)                            */
/* ========================================================  SYNTOR  ========================================================= */
#define R_ETHERC_EPTPC_COMMON_SYNTOR_SYNTOR_Pos        (0UL)          /*!< SYNTOR (Bit 0)                                        */
#define R_ETHERC_EPTPC_COMMON_SYNTOR_SYNTOR_Msk        (0xffffffffUL) /*!< SYNTOR (Bitfield-Mask: 0xffffffff)                    */
/* ========================================================  IPTSELR  ======================================================== */
#define R_ETHERC_EPTPC_COMMON_IPTSELR_IPTSEL5_Pos      (5UL)          /*!< IPTSEL5 (Bit 5)                                       */
#define R_ETHERC_EPTPC_COMMON_IPTSELR_IPTSEL5_Msk      (0x20UL)       /*!< IPTSEL5 (Bitfield-Mask: 0x01)                         */
#define R_ETHERC_EPTPC_COMMON_IPTSELR_IPTSEL4_Pos      (4UL)          /*!< IPTSEL4 (Bit 4)                                       */
#define R_ETHERC_EPTPC_COMMON_IPTSELR_IPTSEL4_Msk      (0x10UL)       /*!< IPTSEL4 (Bitfield-Mask: 0x01)                         */
#define R_ETHERC_EPTPC_COMMON_IPTSELR_IPTSEL3_Pos      (3UL)          /*!< IPTSEL3 (Bit 3)                                       */
#define R_ETHERC_EPTPC_COMMON_IPTSELR_IPTSEL3_Msk      (0x8UL)        /*!< IPTSEL3 (Bitfield-Mask: 0x01)                         */
#define R_ETHERC_EPTPC_COMMON_IPTSELR_IPTSEL2_Pos      (2UL)          /*!< IPTSEL2 (Bit 2)                                       */
#define R_ETHERC_EPTPC_COMMON_IPTSELR_IPTSEL2_Msk      (0x4UL)        /*!< IPTSEL2 (Bitfield-Mask: 0x01)                         */
#define R_ETHERC_EPTPC_COMMON_IPTSELR_IPTSEL1_Pos      (1UL)          /*!< IPTSEL1 (Bit 1)                                       */
#define R_ETHERC_EPTPC_COMMON_IPTSELR_IPTSEL1_Msk      (0x2UL)        /*!< IPTSEL1 (Bitfield-Mask: 0x01)                         */
#define R_ETHERC_EPTPC_COMMON_IPTSELR_IPTSEL0_Pos      (0UL)          /*!< IPTSEL0 (Bit 0)                                       */
#define R_ETHERC_EPTPC_COMMON_IPTSELR_IPTSEL0_Msk      (0x1UL)        /*!< IPTSEL0 (Bitfield-Mask: 0x01)                         */
/* ========================================================  MITSELR  ======================================================== */
#define R_ETHERC_EPTPC_COMMON_MITSELR_MINTEN5_Pos      (5UL)          /*!< MINTEN5 (Bit 5)                                       */
#define R_ETHERC_EPTPC_COMMON_MITSELR_MINTEN5_Msk      (0x20UL)       /*!< MINTEN5 (Bitfield-Mask: 0x01)                         */
#define R_ETHERC_EPTPC_COMMON_MITSELR_MINTEN4_Pos      (4UL)          /*!< MINTEN4 (Bit 4)                                       */
#define R_ETHERC_EPTPC_COMMON_MITSELR_MINTEN4_Msk      (0x10UL)       /*!< MINTEN4 (Bitfield-Mask: 0x01)                         */
#define R_ETHERC_EPTPC_COMMON_MITSELR_MINTEN3_Pos      (3UL)          /*!< MINTEN3 (Bit 3)                                       */
#define R_ETHERC_EPTPC_COMMON_MITSELR_MINTEN3_Msk      (0x8UL)        /*!< MINTEN3 (Bitfield-Mask: 0x01)                         */
#define R_ETHERC_EPTPC_COMMON_MITSELR_MINTEN2_Pos      (2UL)          /*!< MINTEN2 (Bit 2)                                       */
#define R_ETHERC_EPTPC_COMMON_MITSELR_MINTEN2_Msk      (0x4UL)        /*!< MINTEN2 (Bitfield-Mask: 0x01)                         */
#define R_ETHERC_EPTPC_COMMON_MITSELR_MINTEN1_Pos      (1UL)          /*!< MINTEN1 (Bit 1)                                       */
#define R_ETHERC_EPTPC_COMMON_MITSELR_MINTEN1_Msk      (0x2UL)        /*!< MINTEN1 (Bitfield-Mask: 0x01)                         */
#define R_ETHERC_EPTPC_COMMON_MITSELR_MINTEN0_Pos      (0UL)          /*!< MINTEN0 (Bit 0)                                       */
#define R_ETHERC_EPTPC_COMMON_MITSELR_MINTEN0_Msk      (0x1UL)        /*!< MINTEN0 (Bitfield-Mask: 0x01)                         */
/* ========================================================  ELTSELR  ======================================================== */
#define R_ETHERC_EPTPC_COMMON_ELTSELR_ELTDIS5_Pos      (5UL)          /*!< ELTDIS5 (Bit 5)                                       */
#define R_ETHERC_EPTPC_COMMON_ELTSELR_ELTDIS5_Msk      (0x20UL)       /*!< ELTDIS5 (Bitfield-Mask: 0x01)                         */
#define R_ETHERC_EPTPC_COMMON_ELTSELR_ELTDIS4_Pos      (4UL)          /*!< ELTDIS4 (Bit 4)                                       */
#define R_ETHERC_EPTPC_COMMON_ELTSELR_ELTDIS4_Msk      (0x10UL)       /*!< ELTDIS4 (Bitfield-Mask: 0x01)                         */
#define R_ETHERC_EPTPC_COMMON_ELTSELR_ELTDIS3_Pos      (3UL)          /*!< ELTDIS3 (Bit 3)                                       */
#define R_ETHERC_EPTPC_COMMON_ELTSELR_ELTDIS3_Msk      (0x8UL)        /*!< ELTDIS3 (Bitfield-Mask: 0x01)                         */
#define R_ETHERC_EPTPC_COMMON_ELTSELR_ELTDIS2_Pos      (2UL)          /*!< ELTDIS2 (Bit 2)                                       */
#define R_ETHERC_EPTPC_COMMON_ELTSELR_ELTDIS2_Msk      (0x4UL)        /*!< ELTDIS2 (Bitfield-Mask: 0x01)                         */
#define R_ETHERC_EPTPC_COMMON_ELTSELR_ELTDIS1_Pos      (1UL)          /*!< ELTDIS1 (Bit 1)                                       */
#define R_ETHERC_EPTPC_COMMON_ELTSELR_ELTDIS1_Msk      (0x2UL)        /*!< ELTDIS1 (Bitfield-Mask: 0x01)                         */
#define R_ETHERC_EPTPC_COMMON_ELTSELR_ELTDIS0_Pos      (0UL)          /*!< ELTDIS0 (Bit 0)                                       */
#define R_ETHERC_EPTPC_COMMON_ELTSELR_ELTDIS0_Msk      (0x1UL)        /*!< ELTDIS0 (Bitfield-Mask: 0x01)                         */
/* =======================================================  STCHSELR  ======================================================== */
#define R_ETHERC_EPTPC_COMMON_STCHSELR_SYSEL_Pos       (0UL)          /*!< SYSEL (Bit 0)                                         */
#define R_ETHERC_EPTPC_COMMON_STCHSELR_SYSEL_Msk       (0x1UL)        /*!< SYSEL (Bitfield-Mask: 0x01)                           */
/* =======================================================  SYNSTARTR  ======================================================= */
#define R_ETHERC_EPTPC_COMMON_SYNSTARTR_STR_Pos        (0UL)          /*!< STR (Bit 0)                                           */
#define R_ETHERC_EPTPC_COMMON_SYNSTARTR_STR_Msk        (0x1UL)        /*!< STR (Bitfield-Mask: 0x01)                             */
/* ========================================================  LCIVLDR  ======================================================== */
#define R_ETHERC_EPTPC_COMMON_LCIVLDR_LOAD_Pos         (0UL)          /*!< LOAD (Bit 0)                                          */
#define R_ETHERC_EPTPC_COMMON_LCIVLDR_LOAD_Msk         (0x1UL)        /*!< LOAD (Bitfield-Mask: 0x01)                            */
/* =======================================================  SYNTDARU  ======================================================== */
#define R_ETHERC_EPTPC_COMMON_SYNTDARU_SYNTDARU_Pos    (0UL)          /*!< SYNTDARU (Bit 0)                                      */
#define R_ETHERC_EPTPC_COMMON_SYNTDARU_SYNTDARU_Msk    (0xffffffffUL) /*!< SYNTDARU (Bitfield-Mask: 0xffffffff)                  */
/* =======================================================  SYNTDARL  ======================================================== */
#define R_ETHERC_EPTPC_COMMON_SYNTDARL_SYNTDARL_Pos    (0UL)          /*!< SYNTDARL (Bit 0)                                      */
#define R_ETHERC_EPTPC_COMMON_SYNTDARL_SYNTDARL_Msk    (0xffffffffUL) /*!< SYNTDARL (Bitfield-Mask: 0xffffffff)                  */
/* =======================================================  SYNTDBRU  ======================================================== */
#define R_ETHERC_EPTPC_COMMON_SYNTDBRU_SYNTDBRU_Pos    (0UL)          /*!< SYNTDBRU (Bit 0)                                      */
#define R_ETHERC_EPTPC_COMMON_SYNTDBRU_SYNTDBRU_Msk    (0xffffffffUL) /*!< SYNTDBRU (Bitfield-Mask: 0xffffffff)                  */
/* =======================================================  SYNTDBRL  ======================================================== */
#define R_ETHERC_EPTPC_COMMON_SYNTDBRL_SYNTDBRL_Pos    (0UL)          /*!< SYNTDBRL (Bit 0)                                      */
#define R_ETHERC_EPTPC_COMMON_SYNTDBRL_SYNTDBRL_Msk    (0xffffffffUL) /*!< SYNTDBRL (Bitfield-Mask: 0xffffffff)                  */
/* ========================================================  LCIVRU  ========================================================= */
#define R_ETHERC_EPTPC_COMMON_LCIVRU_LCIVRU_Pos        (0UL)          /*!< LCIVRU (Bit 0)                                        */
#define R_ETHERC_EPTPC_COMMON_LCIVRU_LCIVRU_Msk        (0xffffUL)     /*!< LCIVRU (Bitfield-Mask: 0xffff)                        */
/* ========================================================  LCIVRM  ========================================================= */
#define R_ETHERC_EPTPC_COMMON_LCIVRM_LCIVRM_Pos        (0UL)          /*!< LCIVRM (Bit 0)                                        */
#define R_ETHERC_EPTPC_COMMON_LCIVRM_LCIVRM_Msk        (0xffffffffUL) /*!< LCIVRM (Bitfield-Mask: 0xffffffff)                    */
/* ========================================================  LCIVRL  ========================================================= */
#define R_ETHERC_EPTPC_COMMON_LCIVRL_LCIVRL_Pos        (0UL)          /*!< LCIVRL (Bit 0)                                        */
#define R_ETHERC_EPTPC_COMMON_LCIVRL_LCIVRL_Msk        (0xffffffffUL) /*!< LCIVRL (Bitfield-Mask: 0xffffffff)                    */
/* ========================================================  GETW10R  ======================================================== */
#define R_ETHERC_EPTPC_COMMON_GETW10R_GW10_Pos         (0UL)          /*!< GW10 (Bit 0)                                          */
#define R_ETHERC_EPTPC_COMMON_GETW10R_GW10_Msk         (0x1UL)        /*!< GW10 (Bitfield-Mask: 0x01)                            */
/* =======================================================  PLIMITRU  ======================================================== */
#define R_ETHERC_EPTPC_COMMON_PLIMITRU_PLIMITRU_Pos    (0UL)          /*!< PLIMITRU (Bit 0)                                      */
#define R_ETHERC_EPTPC_COMMON_PLIMITRU_PLIMITRU_Msk    (0x7fffffffUL) /*!< PLIMITRU (Bitfield-Mask: 0x7fffffff)                  */
/* =======================================================  PLIMITRM  ======================================================== */
#define R_ETHERC_EPTPC_COMMON_PLIMITRM_PLIMITRM_Pos    (0UL)          /*!< PLIMITRM (Bit 0)                                      */
#define R_ETHERC_EPTPC_COMMON_PLIMITRM_PLIMITRM_Msk    (0xffffffffUL) /*!< PLIMITRM (Bitfield-Mask: 0xffffffff)                  */
/* =======================================================  PLIMITRL  ======================================================== */
#define R_ETHERC_EPTPC_COMMON_PLIMITRL_PLIMITRL_Pos    (0UL)          /*!< PLIMITRL (Bit 0)                                      */
#define R_ETHERC_EPTPC_COMMON_PLIMITRL_PLIMITRL_Msk    (0xffffffffUL) /*!< PLIMITRL (Bitfield-Mask: 0xffffffff)                  */
/* =======================================================  MLIMITRU  ======================================================== */
#define R_ETHERC_EPTPC_COMMON_MLIMITRU_MLIMITRU_Pos    (0UL)          /*!< MLIMITRU (Bit 0)                                      */
#define R_ETHERC_EPTPC_COMMON_MLIMITRU_MLIMITRU_Msk    (0x7fffffffUL) /*!< MLIMITRU (Bitfield-Mask: 0x7fffffff)                  */
/* =======================================================  MLIMITRM  ======================================================== */
#define R_ETHERC_EPTPC_COMMON_MLIMITRM_MLIMITRM_Pos    (0UL)          /*!< MLIMITRM (Bit 0)                                      */
#define R_ETHERC_EPTPC_COMMON_MLIMITRM_MLIMITRM_Msk    (0xffffffffUL) /*!< MLIMITRM (Bitfield-Mask: 0xffffffff)                  */
/* =======================================================  MLIMITRL  ======================================================== */
#define R_ETHERC_EPTPC_COMMON_MLIMITRL_MLIMITRL_Pos    (0UL)          /*!< MLIMITRL (Bit 0)                                      */
#define R_ETHERC_EPTPC_COMMON_MLIMITRL_MLIMITRL_Msk    (0xffffffffUL) /*!< MLIMITRL (Bitfield-Mask: 0xffffffff)                  */
/* =======================================================  GETINFOR  ======================================================== */
#define R_ETHERC_EPTPC_COMMON_GETINFOR_INFO_Pos        (0UL)          /*!< INFO (Bit 0)                                          */
#define R_ETHERC_EPTPC_COMMON_GETINFOR_INFO_Msk        (0x1UL)        /*!< INFO (Bitfield-Mask: 0x01)                            */
/* ========================================================  LCCVRU  ========================================================= */
#define R_ETHERC_EPTPC_COMMON_LCCVRU_LCCVRU_Pos        (0UL)          /*!< LCCVRU (Bit 0)                                        */
#define R_ETHERC_EPTPC_COMMON_LCCVRU_LCCVRU_Msk        (0xffffUL)     /*!< LCCVRU (Bitfield-Mask: 0xffff)                        */
/* ========================================================  LCCVRM  ========================================================= */
#define R_ETHERC_EPTPC_COMMON_LCCVRM_LCCVRM_Pos        (0UL)          /*!< LCCVRM (Bit 0)                                        */
#define R_ETHERC_EPTPC_COMMON_LCCVRM_LCCVRM_Msk        (0xffffffffUL) /*!< LCCVRM (Bitfield-Mask: 0xffffffff)                    */
/* ========================================================  LCCVRL  ========================================================= */
#define R_ETHERC_EPTPC_COMMON_LCCVRL_LCCVRL_Pos        (0UL)          /*!< LCCVRL (Bit 0)                                        */
#define R_ETHERC_EPTPC_COMMON_LCCVRL_LCCVRL_Msk        (0xffffffffUL) /*!< LCCVRL (Bitfield-Mask: 0xffffffff)                    */
/* ========================================================  PW10VRU  ======================================================== */
#define R_ETHERC_EPTPC_COMMON_PW10VRU_PW10VRU_Pos      (0UL)          /*!< PW10VRU (Bit 0)                                       */
#define R_ETHERC_EPTPC_COMMON_PW10VRU_PW10VRU_Msk      (0xffffffffUL) /*!< PW10VRU (Bitfield-Mask: 0xffffffff)                   */
/* ========================================================  PW10VRM  ======================================================== */
#define R_ETHERC_EPTPC_COMMON_PW10VRM_PW10VRM_Pos      (0UL)          /*!< PW10VRM (Bit 0)                                       */
#define R_ETHERC_EPTPC_COMMON_PW10VRM_PW10VRM_Msk      (0xffffffffUL) /*!< PW10VRM (Bitfield-Mask: 0xffffffff)                   */
/* ========================================================  PW10VRL  ======================================================== */
#define R_ETHERC_EPTPC_COMMON_PW10VRL_PW10VRL_Pos      (0UL)          /*!< PW10VRL (Bit 0)                                       */
#define R_ETHERC_EPTPC_COMMON_PW10VRL_PW10VRL_Msk      (0xffffffffUL) /*!< PW10VRL (Bitfield-Mask: 0xffffffff)                   */
/* ========================================================  MW10RU  ========================================================= */
#define R_ETHERC_EPTPC_COMMON_MW10RU_MW10RU_Pos        (0UL)          /*!< MW10RU (Bit 0)                                        */
#define R_ETHERC_EPTPC_COMMON_MW10RU_MW10RU_Msk        (0xffffffffUL) /*!< MW10RU (Bitfield-Mask: 0xffffffff)                    */
/* ========================================================  MW10RM  ========================================================= */
#define R_ETHERC_EPTPC_COMMON_MW10RM_MW10RM_Pos        (0UL)          /*!< MW10RM (Bit 0)                                        */
#define R_ETHERC_EPTPC_COMMON_MW10RM_MW10RM_Msk        (0xffffffffUL) /*!< MW10RM (Bitfield-Mask: 0xffffffff)                    */
/* ========================================================  MW10RL  ========================================================= */
#define R_ETHERC_EPTPC_COMMON_MW10RL_MW10RL_Pos        (0UL)          /*!< MW10RL (Bit 0)                                        */
#define R_ETHERC_EPTPC_COMMON_MW10RL_MW10RL_Msk        (0xffffffffUL) /*!< MW10RL (Bitfield-Mask: 0xffffffff)                    */
/* =======================================================  TMSTARTR  ======================================================== */
#define R_ETHERC_EPTPC_COMMON_TMSTARTR_EN5_Pos         (5UL)          /*!< EN5 (Bit 5)                                           */
#define R_ETHERC_EPTPC_COMMON_TMSTARTR_EN5_Msk         (0x20UL)       /*!< EN5 (Bitfield-Mask: 0x01)                             */
#define R_ETHERC_EPTPC_COMMON_TMSTARTR_EN4_Pos         (4UL)          /*!< EN4 (Bit 4)                                           */
#define R_ETHERC_EPTPC_COMMON_TMSTARTR_EN4_Msk         (0x10UL)       /*!< EN4 (Bitfield-Mask: 0x01)                             */
#define R_ETHERC_EPTPC_COMMON_TMSTARTR_EN3_Pos         (3UL)          /*!< EN3 (Bit 3)                                           */
#define R_ETHERC_EPTPC_COMMON_TMSTARTR_EN3_Msk         (0x8UL)        /*!< EN3 (Bitfield-Mask: 0x01)                             */
#define R_ETHERC_EPTPC_COMMON_TMSTARTR_EN2_Pos         (2UL)          /*!< EN2 (Bit 2)                                           */
#define R_ETHERC_EPTPC_COMMON_TMSTARTR_EN2_Msk         (0x4UL)        /*!< EN2 (Bitfield-Mask: 0x01)                             */
#define R_ETHERC_EPTPC_COMMON_TMSTARTR_EN1_Pos         (1UL)          /*!< EN1 (Bit 1)                                           */
#define R_ETHERC_EPTPC_COMMON_TMSTARTR_EN1_Msk         (0x2UL)        /*!< EN1 (Bitfield-Mask: 0x01)                             */
#define R_ETHERC_EPTPC_COMMON_TMSTARTR_EN0_Pos         (0UL)          /*!< EN0 (Bit 0)                                           */
#define R_ETHERC_EPTPC_COMMON_TMSTARTR_EN0_Msk         (0x1UL)        /*!< EN0 (Bitfield-Mask: 0x01)                             */
/* =========================================================  PRSR  ========================================================== */
#define R_ETHERC_EPTPC_COMMON_PRSR_URE1_Pos            (29UL)         /*!< URE1 (Bit 29)                                         */
#define R_ETHERC_EPTPC_COMMON_PRSR_URE1_Msk            (0x20000000UL) /*!< URE1 (Bitfield-Mask: 0x01)                            */
#define R_ETHERC_EPTPC_COMMON_PRSR_URE0_Pos            (28UL)         /*!< URE0 (Bit 28)                                         */
#define R_ETHERC_EPTPC_COMMON_PRSR_URE0_Msk            (0x10000000UL) /*!< URE0 (Bitfield-Mask: 0x01)                            */
#define R_ETHERC_EPTPC_COMMON_PRSR_MACE_Pos            (8UL)          /*!< MACE (Bit 8)                                          */
#define R_ETHERC_EPTPC_COMMON_PRSR_MACE_Msk            (0x100UL)      /*!< MACE (Bitfield-Mask: 0x01)                            */
#define R_ETHERC_EPTPC_COMMON_PRSR_OVRE3_Pos           (3UL)          /*!< OVRE3 (Bit 3)                                         */
#define R_ETHERC_EPTPC_COMMON_PRSR_OVRE3_Msk           (0x8UL)        /*!< OVRE3 (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_COMMON_PRSR_OVRE2_Pos           (2UL)          /*!< OVRE2 (Bit 2)                                         */
#define R_ETHERC_EPTPC_COMMON_PRSR_OVRE2_Msk           (0x4UL)        /*!< OVRE2 (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_COMMON_PRSR_OVRE1_Pos           (1UL)          /*!< OVRE1 (Bit 1)                                         */
#define R_ETHERC_EPTPC_COMMON_PRSR_OVRE1_Msk           (0x2UL)        /*!< OVRE1 (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_COMMON_PRSR_OVRE0_Pos           (0UL)          /*!< OVRE0 (Bit 0)                                         */
#define R_ETHERC_EPTPC_COMMON_PRSR_OVRE0_Msk           (0x1UL)        /*!< OVRE0 (Bitfield-Mask: 0x01)                           */
/* =========================================================  PRIPR  ========================================================= */
#define R_ETHERC_EPTPC_COMMON_PRIPR_URE1_Pos           (29UL)         /*!< URE1 (Bit 29)                                         */
#define R_ETHERC_EPTPC_COMMON_PRIPR_URE1_Msk           (0x20000000UL) /*!< URE1 (Bitfield-Mask: 0x01)                            */
#define R_ETHERC_EPTPC_COMMON_PRIPR_URE0_Pos           (28UL)         /*!< URE0 (Bit 28)                                         */
#define R_ETHERC_EPTPC_COMMON_PRIPR_URE0_Msk           (0x10000000UL) /*!< URE0 (Bitfield-Mask: 0x01)                            */
#define R_ETHERC_EPTPC_COMMON_PRIPR_MACE_Pos           (8UL)          /*!< MACE (Bit 8)                                          */
#define R_ETHERC_EPTPC_COMMON_PRIPR_MACE_Msk           (0x100UL)      /*!< MACE (Bitfield-Mask: 0x01)                            */
#define R_ETHERC_EPTPC_COMMON_PRIPR_OVRE3_Pos          (3UL)          /*!< OVRE3 (Bit 3)                                         */
#define R_ETHERC_EPTPC_COMMON_PRIPR_OVRE3_Msk          (0x8UL)        /*!< OVRE3 (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_COMMON_PRIPR_OVRE2_Pos          (2UL)          /*!< OVRE2 (Bit 2)                                         */
#define R_ETHERC_EPTPC_COMMON_PRIPR_OVRE2_Msk          (0x4UL)        /*!< OVRE2 (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_COMMON_PRIPR_OVRE1_Pos          (1UL)          /*!< OVRE1 (Bit 1)                                         */
#define R_ETHERC_EPTPC_COMMON_PRIPR_OVRE1_Msk          (0x2UL)        /*!< OVRE1 (Bitfield-Mask: 0x01)                           */
#define R_ETHERC_EPTPC_COMMON_PRIPR_OVRE0_Pos          (0UL)          /*!< OVRE0 (Bit 0)                                         */
#define R_ETHERC_EPTPC_COMMON_PRIPR_OVRE0_Msk          (0x1UL)        /*!< OVRE0 (Bitfield-Mask: 0x01)                           */
/* ========================================================  TRNDISR  ======================================================== */
#define R_ETHERC_EPTPC_COMMON_TRNDISR_TDIS_Pos         (0UL)          /*!< TDIS (Bit 0)                                          */
#define R_ETHERC_EPTPC_COMMON_TRNDISR_TDIS_Msk         (0x3UL)        /*!< TDIS (Bitfield-Mask: 0x03)                            */
/* =========================================================  TRNMR  ========================================================= */
#define R_ETHERC_EPTPC_COMMON_TRNMR_FWD1_Pos           (9UL)          /*!< FWD1 (Bit 9)                                          */
#define R_ETHERC_EPTPC_COMMON_TRNMR_FWD1_Msk           (0x200UL)      /*!< FWD1 (Bitfield-Mask: 0x01)                            */
#define R_ETHERC_EPTPC_COMMON_TRNMR_FWD0_Pos           (8UL)          /*!< FWD0 (Bit 8)                                          */
#define R_ETHERC_EPTPC_COMMON_TRNMR_FWD0_Msk           (0x100UL)      /*!< FWD0 (Bitfield-Mask: 0x01)                            */
#define R_ETHERC_EPTPC_COMMON_TRNMR_MOD_Pos            (0UL)          /*!< MOD (Bit 0)                                           */
#define R_ETHERC_EPTPC_COMMON_TRNMR_MOD_Msk            (0x1UL)        /*!< MOD (Bitfield-Mask: 0x01)                             */
/* =======================================================  TRNCTTDR  ======================================================== */
#define R_ETHERC_EPTPC_COMMON_TRNCTTDR_THVAL_Pos       (0UL)          /*!< THVAL (Bit 0)                                         */
#define R_ETHERC_EPTPC_COMMON_TRNCTTDR_THVAL_Msk       (0x7ffUL)      /*!< THVAL (Bitfield-Mask: 0x7ff)                          */

#endif /* R_ETHER_POSMSK_H */
