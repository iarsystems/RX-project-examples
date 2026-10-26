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

#ifndef R_SYSTEM_POSMSK_H
#define R_SYSTEM_POSMSK_H

/* =========================================================================================================================== */
/* ================                                          R_SYSTEM                                           ================ */
/* =========================================================================================================================== */

/* =======================================================  SCKDIVCR  ======================================================== */
#define R_SYSTEM_SCKDIVCR_MRPCK_Pos             (28UL)         /*!< MRPCK (Bit 28)                                        */
#define R_SYSTEM_SCKDIVCR_MRPCK_Msk             (0xf0000000UL) /*!< MRPCK (Bitfield-Mask: 0x0f)                           */
#define R_SYSTEM_SCKDIVCR_ICK_Pos               (24UL)         /*!< ICK (Bit 24)                                          */
#define R_SYSTEM_SCKDIVCR_ICK_Msk               (0xf000000UL)  /*!< ICK (Bitfield-Mask: 0x0f)                             */
#define R_SYSTEM_SCKDIVCR_PCKE_Pos              (20UL)         /*!< PCKE (Bit 20)                                         */
#define R_SYSTEM_SCKDIVCR_PCKE_Msk              (0xf00000UL)   /*!< PCKE (Bitfield-Mask: 0x0f)                            */
#define R_SYSTEM_SCKDIVCR_BCK_Pos               (16UL)         /*!< BCK (Bit 16)                                          */
#define R_SYSTEM_SCKDIVCR_BCK_Msk               (0xf0000UL)    /*!< BCK (Bitfield-Mask: 0x0f)                             */
#define R_SYSTEM_SCKDIVCR_PCKA_Pos              (12UL)         /*!< PCKA (Bit 12)                                         */
#define R_SYSTEM_SCKDIVCR_PCKA_Msk              (0xf000UL)     /*!< PCKA (Bitfield-Mask: 0x0f)                            */
#define R_SYSTEM_SCKDIVCR_PCKB_Pos              (8UL)          /*!< PCKB (Bit 8)                                          */
#define R_SYSTEM_SCKDIVCR_PCKB_Msk              (0xf00UL)      /*!< PCKB (Bitfield-Mask: 0x0f)                            */
#define R_SYSTEM_SCKDIVCR_PCKC_Pos              (4UL)          /*!< PCKC (Bit 4)                                          */
#define R_SYSTEM_SCKDIVCR_PCKC_Msk              (0xf0UL)       /*!< PCKC (Bitfield-Mask: 0x0f)                            */
#define R_SYSTEM_SCKDIVCR_PCKD_Pos              (0UL)          /*!< PCKD (Bit 0)                                          */
#define R_SYSTEM_SCKDIVCR_PCKD_Msk              (0xfUL)        /*!< PCKD (Bitfield-Mask: 0x0f)                            */
/* =======================================================  SCKDIVCR2  ======================================================= */
#define R_SYSTEM_SCKDIVCR2_MRICK_Pos            (12UL)         /*!< MRICK (Bit 12)                                        */
#define R_SYSTEM_SCKDIVCR2_MRICK_Msk            (0xf000UL)     /*!< MRICK (Bitfield-Mask: 0x0f)                           */
#define R_SYSTEM_SCKDIVCR2_CPUCK0_Pos           (0UL)          /*!< CPUCK0 (Bit 0)                                        */
#define R_SYSTEM_SCKDIVCR2_CPUCK0_Msk           (0xfUL)        /*!< CPUCK0 (Bitfield-Mask: 0x0f)                          */
/* ========================================================  SCKSCR  ========================================================= */
#define R_SYSTEM_SCKSCR_CKSEL_Pos               (0UL)          /*!< CKSEL (Bit 0)                                         */
#define R_SYSTEM_SCKSCR_CKSEL_Msk               (0x7UL)        /*!< CKSEL (Bitfield-Mask: 0x07)                           */
/* =========================================================  PLLCR  ========================================================= */
#define R_SYSTEM_PLLCR_PLLSTP_Pos               (0UL)          /*!< PLLSTP (Bit 0)                                        */
#define R_SYSTEM_PLLCR_PLLSTP_Msk               (0x1UL)        /*!< PLLSTP (Bitfield-Mask: 0x01)                          */
/* =========================================================  BCKCR  ========================================================= */
#define R_SYSTEM_BCKCR_EBCKASEL_Pos             (7UL)          /*!< EBCKASEL (Bit 7)                                      */
#define R_SYSTEM_BCKCR_EBCKASEL_Msk             (0x80UL)       /*!< EBCKASEL (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_BCKCR_BCLKDIV_Pos              (0UL)          /*!< BCLKDIV (Bit 0)                                       */
#define R_SYSTEM_BCKCR_BCLKDIV_Msk              (0x1UL)        /*!< BCLKDIV (Bitfield-Mask: 0x01)                         */
/* ========================================================  MOSCCR  ========================================================= */
#define R_SYSTEM_MOSCCR_MOSTP_Pos               (0UL)          /*!< MOSTP (Bit 0)                                         */
#define R_SYSTEM_MOSCCR_MOSTP_Msk               (0x1UL)        /*!< MOSTP (Bitfield-Mask: 0x01)                           */
/* ========================================================  HOCOCR  ========================================================= */
#define R_SYSTEM_HOCOCR_HCSTP_Pos               (0UL)          /*!< HCSTP (Bit 0)                                         */
#define R_SYSTEM_HOCOCR_HCSTP_Msk               (0x1UL)        /*!< HCSTP (Bitfield-Mask: 0x01)                           */
/* ========================================================  MOCOCR  ========================================================= */
#define R_SYSTEM_MOCOCR_MCSTP_Pos               (0UL)          /*!< MCSTP (Bit 0)                                         */
#define R_SYSTEM_MOCOCR_MCSTP_Msk               (0x1UL)        /*!< MCSTP (Bitfield-Mask: 0x01)                           */
/* ========================================================  FLLCR1  ========================================================= */
#define R_SYSTEM_FLLCR1_FLLEN_Pos               (0UL)          /*!< FLLEN (Bit 0)                                         */
#define R_SYSTEM_FLLCR1_FLLEN_Msk               (0x1UL)        /*!< FLLEN (Bitfield-Mask: 0x01)                           */
/* ========================================================  FLLCR2  ========================================================= */
#define R_SYSTEM_FLLCR2_FLLCNTL_Pos             (0UL)          /*!< FLLCNTL (Bit 0)                                       */
#define R_SYSTEM_FLLCR2_FLLCNTL_Msk             (0x7ffUL)      /*!< FLLCNTL (Bitfield-Mask: 0x7ff)                        */
/* =========================================================  OSCSF  ========================================================= */
#define R_SYSTEM_OSCSF_PLL2SF_Pos               (6UL)          /*!< PLL2SF (Bit 6)                                        */
#define R_SYSTEM_OSCSF_PLL2SF_Msk               (0x40UL)       /*!< PLL2SF (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_OSCSF_PLLSF_Pos                (5UL)          /*!< PLLSF (Bit 5)                                         */
#define R_SYSTEM_OSCSF_PLLSF_Msk                (0x20UL)       /*!< PLLSF (Bitfield-Mask: 0x01)                           */
#define R_SYSTEM_OSCSF_MOSCSF_Pos               (3UL)          /*!< MOSCSF (Bit 3)                                        */
#define R_SYSTEM_OSCSF_MOSCSF_Msk               (0x8UL)        /*!< MOSCSF (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_OSCSF_HOCOSF_Pos               (0UL)          /*!< HOCOSF (Bit 0)                                        */
#define R_SYSTEM_OSCSF_HOCOSF_Msk               (0x1UL)        /*!< HOCOSF (Bitfield-Mask: 0x01)                          */
/* =========================================================  CKOCR  ========================================================= */
#define R_SYSTEM_CKOCR_CKOEN_Pos                (7UL)          /*!< CKOEN (Bit 7)                                         */
#define R_SYSTEM_CKOCR_CKOEN_Msk                (0x80UL)       /*!< CKOEN (Bitfield-Mask: 0x01)                           */
#define R_SYSTEM_CKOCR_CKODIV_Pos               (4UL)          /*!< CKODIV (Bit 4)                                        */
#define R_SYSTEM_CKOCR_CKODIV_Msk               (0x70UL)       /*!< CKODIV (Bitfield-Mask: 0x07)                          */
#define R_SYSTEM_CKOCR_CKOSEL_Pos               (0UL)          /*!< CKOSEL (Bit 0)                                        */
#define R_SYSTEM_CKOCR_CKOSEL_Msk               (0x7UL)        /*!< CKOSEL (Bitfield-Mask: 0x07)                          */
/* ========================================================  OSTDCR  ========================================================= */
#define R_SYSTEM_OSTDCR_OSTDE_Pos               (7UL)          /*!< OSTDE (Bit 7)                                         */
#define R_SYSTEM_OSTDCR_OSTDE_Msk               (0x80UL)       /*!< OSTDE (Bitfield-Mask: 0x01)                           */
#define R_SYSTEM_OSTDCR_OSTDIE_Pos              (0UL)          /*!< OSTDIE (Bit 0)                                        */
#define R_SYSTEM_OSTDCR_OSTDIE_Msk              (0x1UL)        /*!< OSTDIE (Bitfield-Mask: 0x01)                          */
/* ========================================================  OSTDSR  ========================================================= */
#define R_SYSTEM_OSTDSR_OSTDF_Pos               (0UL)          /*!< OSTDF (Bit 0)                                         */
#define R_SYSTEM_OSTDSR_OSTDF_Msk               (0x1UL)        /*!< OSTDF (Bitfield-Mask: 0x01)                           */
/* ========================================================  OSCMONR  ======================================================== */
#define R_SYSTEM_OSCMONR_LOCOMON_Pos            (2UL)          /*!< LOCOMON (Bit 2)                                       */
#define R_SYSTEM_OSCMONR_LOCOMON_Msk            (0x4UL)        /*!< LOCOMON (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_OSCMONR_MOCOMON_Pos            (1UL)          /*!< MOCOMON (Bit 1)                                       */
#define R_SYSTEM_OSCMONR_MOCOMON_Msk            (0x2UL)        /*!< MOCOMON (Bitfield-Mask: 0x01)                         */
/* ========================================================  PLL2CR  ========================================================= */
#define R_SYSTEM_PLL2CR_PLL2STP_Pos             (0UL)          /*!< PLL2STP (Bit 0)                                       */
#define R_SYSTEM_PLL2CR_PLL2STP_Msk             (0x1UL)        /*!< PLL2STP (Bitfield-Mask: 0x01)                         */
/* ========================================================  PLLCCR2  ======================================================== */
#define R_SYSTEM_PLLCCR2_PLODIVR_Pos            (8UL)          /*!< PLODIVR (Bit 8)                                       */
#define R_SYSTEM_PLLCCR2_PLODIVR_Msk            (0xf00UL)      /*!< PLODIVR (Bitfield-Mask: 0x0f)                         */
#define R_SYSTEM_PLLCCR2_PLODIVQ_Pos            (4UL)          /*!< PLODIVQ (Bit 4)                                       */
#define R_SYSTEM_PLLCCR2_PLODIVQ_Msk            (0xf0UL)       /*!< PLODIVQ (Bitfield-Mask: 0x0f)                         */
#define R_SYSTEM_PLLCCR2_PLODIVP_Pos            (0UL)          /*!< PLODIVP (Bit 0)                                       */
#define R_SYSTEM_PLLCCR2_PLODIVP_Msk            (0xfUL)        /*!< PLODIVP (Bitfield-Mask: 0x0f)                         */
/* =======================================================  PLL2CCR2  ======================================================== */
#define R_SYSTEM_PLL2CCR2_PL2ODIVR_Pos          (8UL)          /*!< PL2ODIVR (Bit 8)                                      */
#define R_SYSTEM_PLL2CCR2_PL2ODIVR_Msk          (0xf00UL)      /*!< PL2ODIVR (Bitfield-Mask: 0x0f)                        */
#define R_SYSTEM_PLL2CCR2_PL2ODIVQ_Pos          (4UL)          /*!< PL2ODIVQ (Bit 4)                                      */
#define R_SYSTEM_PLL2CCR2_PL2ODIVQ_Msk          (0xf0UL)       /*!< PL2ODIVQ (Bitfield-Mask: 0x0f)                        */
#define R_SYSTEM_PLL2CCR2_PL2ODIVP_Pos          (0UL)          /*!< PL2ODIVP (Bit 0)                                      */
#define R_SYSTEM_PLL2CCR2_PL2ODIVP_Msk          (0xfUL)        /*!< PL2ODIVP (Bitfield-Mask: 0x0f)                        */
/* ========================================================  EBCKOCR  ======================================================== */
#define R_SYSTEM_EBCKOCR_EBCKOEN_Pos            (0UL)          /*!< EBCKOEN (Bit 0)                                       */
#define R_SYSTEM_EBCKOCR_EBCKOEN_Msk            (0x1UL)        /*!< EBCKOEN (Bitfield-Mask: 0x01)                         */
/* ========================================================  SDCKOCR  ======================================================== */
#define R_SYSTEM_SDCKOCR_SDCKOEN_Pos            (0UL)          /*!< SDCKOEN (Bit 0)                                       */
#define R_SYSTEM_SDCKOCR_SDCKOEN_Msk            (0x1UL)        /*!< SDCKOEN (Bitfield-Mask: 0x01)                         */
/* ======================================================  SCICKDIVCR  ======================================================= */
#define R_SYSTEM_SCICKDIVCR_SCICKDIV_Pos        (0UL)          /*!< SCICKDIV (Bit 0)                                      */
#define R_SYSTEM_SCICKDIVCR_SCICKDIV_Msk        (0xfUL)        /*!< SCICKDIV (Bitfield-Mask: 0x0f)                        */
/* ========================================================  SCICKCR  ======================================================== */
#define R_SYSTEM_SCICKCR_SCICKSRDY_Pos          (7UL)          /*!< SCICKSRDY (Bit 7)                                     */
#define R_SYSTEM_SCICKCR_SCICKSRDY_Msk          (0x80UL)       /*!< SCICKSRDY (Bitfield-Mask: 0x01)                       */
#define R_SYSTEM_SCICKCR_SCICKSREQ_Pos          (6UL)          /*!< SCICKSREQ (Bit 6)                                     */
#define R_SYSTEM_SCICKCR_SCICKSREQ_Msk          (0x40UL)       /*!< SCICKSREQ (Bitfield-Mask: 0x01)                       */
#define R_SYSTEM_SCICKCR_SCICKSEL_Pos           (0UL)          /*!< SCICKSEL (Bit 0)                                      */
#define R_SYSTEM_SCICKCR_SCICKSEL_Msk           (0xfUL)        /*!< SCICKSEL (Bitfield-Mask: 0x0f)                        */
/* ======================================================  SPICKDIVCR  ======================================================= */
#define R_SYSTEM_SPICKDIVCR_SPICKDIV_Pos        (0UL)          /*!< SPICKDIV (Bit 0)                                      */
#define R_SYSTEM_SPICKDIVCR_SPICKDIV_Msk        (0xfUL)        /*!< SPICKDIV (Bitfield-Mask: 0x0f)                        */
/* ========================================================  SPICKCR  ======================================================== */
#define R_SYSTEM_SPICKCR_SPICKSRDY_Pos          (7UL)          /*!< SPICKSRDY (Bit 7)                                     */
#define R_SYSTEM_SPICKCR_SPICKSRDY_Msk          (0x80UL)       /*!< SPICKSRDY (Bitfield-Mask: 0x01)                       */
#define R_SYSTEM_SPICKCR_SPICKSREQ_Pos          (6UL)          /*!< SPICKSREQ (Bit 6)                                     */
#define R_SYSTEM_SPICKCR_SPICKSREQ_Msk          (0x40UL)       /*!< SPICKSREQ (Bitfield-Mask: 0x01)                       */
#define R_SYSTEM_SPICKCR_SPICKSEL_Pos           (0UL)          /*!< SPICKSEL (Bit 0)                                      */
#define R_SYSTEM_SPICKCR_SPICKSEL_Msk           (0xfUL)        /*!< SPICKSEL (Bitfield-Mask: 0x0f)                        */
/* ======================================================  ADCCKDIVCR  ======================================================= */
#define R_SYSTEM_ADCCKDIVCR_ADCCKDIV_Pos        (0UL)          /*!< ADCCKDIV (Bit 0)                                      */
#define R_SYSTEM_ADCCKDIVCR_ADCCKDIV_Msk        (0xfUL)        /*!< ADCCKDIV (Bitfield-Mask: 0x0f)                        */
/* ========================================================  ADCCKCR  ======================================================== */
#define R_SYSTEM_ADCCKCR_ADCCKSRDY_Pos          (7UL)          /*!< ADCCKSRDY (Bit 7)                                     */
#define R_SYSTEM_ADCCKCR_ADCCKSRDY_Msk          (0x80UL)       /*!< ADCCKSRDY (Bitfield-Mask: 0x01)                       */
#define R_SYSTEM_ADCCKCR_ADCCKSREQ_Pos          (6UL)          /*!< ADCCKSREQ (Bit 6)                                     */
#define R_SYSTEM_ADCCKCR_ADCCKSREQ_Msk          (0x40UL)       /*!< ADCCKSREQ (Bitfield-Mask: 0x01)                       */
#define R_SYSTEM_ADCCKCR_ADCCKSEL_Pos           (0UL)          /*!< ADCCKSEL (Bit 0)                                      */
#define R_SYSTEM_ADCCKCR_ADCCKSEL_Msk           (0xfUL)        /*!< ADCCKSEL (Bitfield-Mask: 0x0f)                        */
/* ======================================================  GPTCKDIVCR  ======================================================= */
#define R_SYSTEM_GPTCKDIVCR_GPTCKDIV_Pos        (0UL)          /*!< GPTCKDIV (Bit 0)                                      */
#define R_SYSTEM_GPTCKDIVCR_GPTCKDIV_Msk        (0xfUL)        /*!< GPTCKDIV (Bitfield-Mask: 0x0f)                        */
/* ========================================================  GPTCKCR  ======================================================== */
#define R_SYSTEM_GPTCKCR_GPTCKSRDY_Pos          (7UL)          /*!< GPTCKSRDY (Bit 7)                                     */
#define R_SYSTEM_GPTCKCR_GPTCKSRDY_Msk          (0x80UL)       /*!< GPTCKSRDY (Bitfield-Mask: 0x01)                       */
#define R_SYSTEM_GPTCKCR_GPTCKSREQ_Pos          (6UL)          /*!< GPTCKSREQ (Bit 6)                                     */
#define R_SYSTEM_GPTCKCR_GPTCKSREQ_Msk          (0x40UL)       /*!< GPTCKSREQ (Bitfield-Mask: 0x01)                       */
#define R_SYSTEM_GPTCKCR_GPTCKSEL_Pos           (0UL)          /*!< GPTCKSEL (Bit 0)                                      */
#define R_SYSTEM_GPTCKCR_GPTCKSEL_Msk           (0xfUL)        /*!< GPTCKSEL (Bitfield-Mask: 0x0f)                        */
/* =======================================================  MOCOUTCR  ======================================================== */
#define R_SYSTEM_MOCOUTCR_MOCOUTRM_Pos          (0UL)          /*!< MOCOUTRM (Bit 0)                                      */
#define R_SYSTEM_MOCOUTCR_MOCOUTRM_Msk          (0xffUL)       /*!< MOCOUTRM (Bitfield-Mask: 0xff)                        */
/* =======================================================  HOCOUTCR  ======================================================== */
#define R_SYSTEM_HOCOUTCR_HOCOUTRM_Pos          (0UL)          /*!< HOCOUTRM (Bit 0)                                      */
#define R_SYSTEM_HOCOUTCR_HOCOUTRM_Msk          (0xffUL)       /*!< HOCOUTRM (Bitfield-Mask: 0xff)                        */
/* ======================================================  USBCKDIVCR  ======================================================= */
#define R_SYSTEM_USBCKDIVCR_USBCKDIV_Pos        (0UL)          /*!< USBCKDIV (Bit 0)                                      */
#define R_SYSTEM_USBCKDIVCR_USBCKDIV_Msk        (0xfUL)        /*!< USBCKDIV (Bitfield-Mask: 0x0f)                        */
/* ======================================================  OCTACKDIVCR  ====================================================== */
#define R_SYSTEM_OCTACKDIVCR_OCTACKDIV_Pos      (0UL)          /*!< OCTACKDIV (Bit 0)                                     */
#define R_SYSTEM_OCTACKDIVCR_OCTACKDIV_Msk      (0xfUL)        /*!< OCTACKDIV (Bitfield-Mask: 0x0f)                       */
/* =====================================================  CANFDCKDIVCR  ====================================================== */
#define R_SYSTEM_CANFDCKDIVCR_CANFDCKDIV_Pos    (0UL)          /*!< CANFDCKDIV (Bit 0)                                    */
#define R_SYSTEM_CANFDCKDIVCR_CANFDCKDIV_Msk    (0xfUL)        /*!< CANFDCKDIV (Bitfield-Mask: 0x0f)                      */
/* =====================================================  USB60CKDIVCR  ====================================================== */
#define R_SYSTEM_USB60CKDIVCR_USB60CKDIV_Pos    (0UL)          /*!< USB60CKDIV (Bit 0)                                    */
#define R_SYSTEM_USB60CKDIVCR_USB60CKDIV_Msk    (0xfUL)        /*!< USB60CKDIV (Bitfield-Mask: 0x0f)                      */
/* ======================================================  I3CCKDIVCR  ======================================================= */
#define R_SYSTEM_I3CCKDIVCR_I3CCKDIV_Pos        (0UL)          /*!< I3CCKDIV (Bit 0)                                      */
#define R_SYSTEM_I3CCKDIVCR_I3CCKDIV_Msk        (0xfUL)        /*!< I3CCKDIV (Bitfield-Mask: 0x0f)                        */
/* ========================================================  USBCKCR  ======================================================== */
#define R_SYSTEM_USBCKCR_USBCKSRDY_Pos          (7UL)          /*!< USBCKSRDY (Bit 7)                                     */
#define R_SYSTEM_USBCKCR_USBCKSRDY_Msk          (0x80UL)       /*!< USBCKSRDY (Bitfield-Mask: 0x01)                       */
#define R_SYSTEM_USBCKCR_USBCKSREQ_Pos          (6UL)          /*!< USBCKSREQ (Bit 6)                                     */
#define R_SYSTEM_USBCKCR_USBCKSREQ_Msk          (0x40UL)       /*!< USBCKSREQ (Bitfield-Mask: 0x01)                       */
#define R_SYSTEM_USBCKCR_USBCKSEL_Pos           (0UL)          /*!< USBCKSEL (Bit 0)                                      */
#define R_SYSTEM_USBCKCR_USBCKSEL_Msk           (0xfUL)        /*!< USBCKSEL (Bitfield-Mask: 0x0f)                        */
/* =======================================================  OCTACKCR  ======================================================== */
#define R_SYSTEM_OCTACKCR_OCTACKSRDY_Pos        (7UL)          /*!< OCTACKSRDY (Bit 7)                                    */
#define R_SYSTEM_OCTACKCR_OCTACKSRDY_Msk        (0x80UL)       /*!< OCTACKSRDY (Bitfield-Mask: 0x01)                      */
#define R_SYSTEM_OCTACKCR_OCTACKSREQ_Pos        (6UL)          /*!< OCTACKSREQ (Bit 6)                                    */
#define R_SYSTEM_OCTACKCR_OCTACKSREQ_Msk        (0x40UL)       /*!< OCTACKSREQ (Bitfield-Mask: 0x01)                      */
#define R_SYSTEM_OCTACKCR_OCTACKSEL_Pos         (0UL)          /*!< OCTACKSEL (Bit 0)                                     */
#define R_SYSTEM_OCTACKCR_OCTACKSEL_Msk         (0xfUL)        /*!< OCTACKSEL (Bitfield-Mask: 0x0f)                       */
/* =======================================================  CANFDCKCR  ======================================================= */
#define R_SYSTEM_CANFDCKCR_CANFDCKSRDY_Pos      (7UL)          /*!< CANFDCKSRDY (Bit 7)                                   */
#define R_SYSTEM_CANFDCKCR_CANFDCKSRDY_Msk      (0x80UL)       /*!< CANFDCKSRDY (Bitfield-Mask: 0x01)                     */
#define R_SYSTEM_CANFDCKCR_CANFDCKSREQ_Pos      (6UL)          /*!< CANFDCKSREQ (Bit 6)                                   */
#define R_SYSTEM_CANFDCKCR_CANFDCKSREQ_Msk      (0x40UL)       /*!< CANFDCKSREQ (Bitfield-Mask: 0x01)                     */
#define R_SYSTEM_CANFDCKCR_CANFDCKSEL_Pos       (0UL)          /*!< CANFDCKSEL (Bit 0)                                    */
#define R_SYSTEM_CANFDCKCR_CANFDCKSEL_Msk       (0xfUL)        /*!< CANFDCKSEL (Bitfield-Mask: 0x0f)                      */
/* =======================================================  USB60CKCR  ======================================================= */
#define R_SYSTEM_USB60CKCR_USB60CKSRDY_Pos      (7UL)          /*!< USB60CKSRDY (Bit 7)                                   */
#define R_SYSTEM_USB60CKCR_USB60CKSRDY_Msk      (0x80UL)       /*!< USB60CKSRDY (Bitfield-Mask: 0x01)                     */
#define R_SYSTEM_USB60CKCR_USB60CKSREQ_Pos      (6UL)          /*!< USB60CKSREQ (Bit 6)                                   */
#define R_SYSTEM_USB60CKCR_USB60CKSREQ_Msk      (0x40UL)       /*!< USB60CKSREQ (Bitfield-Mask: 0x01)                     */
#define R_SYSTEM_USB60CKCR_USB60CKSEL_Pos       (0UL)          /*!< USB60CKSEL (Bit 0)                                    */
#define R_SYSTEM_USB60CKCR_USB60CKSEL_Msk       (0xfUL)        /*!< USB60CKSEL (Bitfield-Mask: 0x0f)                      */
/* ========================================================  I3CCKCR  ======================================================== */
#define R_SYSTEM_I3CCKCR_I3CCKSRDY_Pos          (7UL)          /*!< I3CCKSRDY (Bit 7)                                     */
#define R_SYSTEM_I3CCKCR_I3CCKSRDY_Msk          (0x80UL)       /*!< I3CCKSRDY (Bitfield-Mask: 0x01)                       */
#define R_SYSTEM_I3CCKCR_I3CCKREQ_Pos           (6UL)          /*!< I3CCKREQ (Bit 6)                                      */
#define R_SYSTEM_I3CCKCR_I3CCKREQ_Msk           (0x40UL)       /*!< I3CCKREQ (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_I3CCKCR_I3CCKSEL_Pos           (0UL)          /*!< I3CCKSEL (Bit 0)                                      */
#define R_SYSTEM_I3CCKCR_I3CCKSEL_Msk           (0xfUL)        /*!< I3CCKSEL (Bitfield-Mask: 0x0f)                        */
/* ========================================================  MOSCSCR  ======================================================== */
#define R_SYSTEM_MOSCSCR_MOSCSOKP_Pos           (0UL)          /*!< MOSCSOKP (Bit 0)                                      */
#define R_SYSTEM_MOSCSCR_MOSCSOKP_Msk           (0x1UL)        /*!< MOSCSOKP (Bitfield-Mask: 0x01)                        */
/* ========================================================  HOCOSCR  ======================================================== */
#define R_SYSTEM_HOCOSCR_HOCOSOKP_Pos           (0UL)          /*!< HOCOSOKP (Bit 0)                                      */
#define R_SYSTEM_HOCOSCR_HOCOSOKP_Msk           (0x1UL)        /*!< HOCOSOKP (Bitfield-Mask: 0x01)                        */
/* ========================================================  MOCOSCR  ======================================================== */
#define R_SYSTEM_MOCOSCR_MOCOSOKP_Pos           (0UL)          /*!< MOCOSOKP (Bit 0)                                      */
#define R_SYSTEM_MOCOSCR_MOCOSOKP_Msk           (0x1UL)        /*!< MOCOSOKP (Bitfield-Mask: 0x01)                        */
/* =======================================================  MOSCWTCR  ======================================================== */
#define R_SYSTEM_MOSCWTCR_MSTS_Pos              (0UL)          /*!< MSTS (Bit 0)                                          */
#define R_SYSTEM_MOSCWTCR_MSTS_Msk              (0xfUL)        /*!< MSTS (Bitfield-Mask: 0x0f)                            */
/* ========================================================  PLLCCR  ========================================================= */
#define R_SYSTEM_PLLCCR_PLLMUL_Pos              (8UL)          /*!< PLLMUL (Bit 8)                                        */
#define R_SYSTEM_PLLCCR_PLLMUL_Msk              (0x1ff00UL)    /*!< PLLMUL (Bitfield-Mask: 0x1ff)                         */
#define R_SYSTEM_PLLCCR_PLLMULNF_Pos            (6UL)          /*!< PLLMULNF (Bit 6)                                      */
#define R_SYSTEM_PLLCCR_PLLMULNF_Msk            (0xc0UL)       /*!< PLLMULNF (Bitfield-Mask: 0x03)                        */
#define R_SYSTEM_PLLCCR_PLSRCSEL_Pos            (4UL)          /*!< PLSRCSEL (Bit 4)                                      */
#define R_SYSTEM_PLLCCR_PLSRCSEL_Msk            (0x10UL)       /*!< PLSRCSEL (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_PLLCCR_PLIDIV_Pos              (0UL)          /*!< PLIDIV (Bit 0)                                        */
#define R_SYSTEM_PLLCCR_PLIDIV_Msk              (0x3UL)        /*!< PLIDIV (Bitfield-Mask: 0x03)                          */
/* ========================================================  PLL2CCR  ======================================================== */
#define R_SYSTEM_PLL2CCR_PLL2MUL_Pos            (8UL)          /*!< PLL2MUL (Bit 8)                                       */
#define R_SYSTEM_PLL2CCR_PLL2MUL_Msk            (0x1ff00UL)    /*!< PLL2MUL (Bitfield-Mask: 0x1ff)                        */
#define R_SYSTEM_PLL2CCR_PLL2MULNF_Pos          (6UL)          /*!< PLL2MULNF (Bit 6)                                     */
#define R_SYSTEM_PLL2CCR_PLL2MULNF_Msk          (0xc0UL)       /*!< PLL2MULNF (Bitfield-Mask: 0x03)                       */
#define R_SYSTEM_PLL2CCR_PL2SRCSEL_Pos          (4UL)          /*!< PL2SRCSEL (Bit 4)                                     */
#define R_SYSTEM_PLL2CCR_PL2SRCSEL_Msk          (0x10UL)       /*!< PL2SRCSEL (Bitfield-Mask: 0x01)                       */
#define R_SYSTEM_PLL2CCR_PL2IDIV_Pos            (0UL)          /*!< PL2IDIV (Bit 0)                                       */
#define R_SYSTEM_PLL2CCR_PL2IDIV_Msk            (0x3UL)        /*!< PL2IDIV (Bitfield-Mask: 0x03)                         */
/* ========================================================  SYRACCR  ======================================================== */
#define R_SYSTEM_SYRACCR_BUSY_Pos               (0UL)          /*!< BUSY (Bit 0)                                          */
#define R_SYSTEM_SYRACCR_BUSY_Msk               (0x1UL)        /*!< BUSY (Bitfield-Mask: 0x01)                            */
/* =======================================================  BCKADIVCR  ======================================================= */
#define R_SYSTEM_BCKADIVCR_BCKACKDIV_Pos        (0UL)          /*!< BCKACKDIV (Bit 0)                                     */
#define R_SYSTEM_BCKADIVCR_BCKACKDIV_Msk        (0xfUL)        /*!< BCKACKDIV (Bitfield-Mask: 0x0f)                       */
/* ======================================================  ESWCKDIVCR  ======================================================= */
#define R_SYSTEM_ESWCKDIVCR_ESWCKDIV_Pos        (0UL)          /*!< ESWCKDIV (Bit 0)                                      */
#define R_SYSTEM_ESWCKDIVCR_ESWCKDIV_Msk        (0xfUL)        /*!< ESWCKDIV (Bitfield-Mask: 0x0f)                        */
/* ======================================================  ESWPCKDIVCR  ====================================================== */
#define R_SYSTEM_ESWPCKDIVCR_ESWPCKDIV_Pos      (0UL)          /*!< ESWPCKDIV (Bit 0)                                     */
#define R_SYSTEM_ESWPCKDIVCR_ESWPCKDIV_Msk      (0xfUL)        /*!< ESWPCKDIV (Bitfield-Mask: 0x0f)                       */
/* ======================================================  ESCCKDIVCR  ======================================================= */
#define R_SYSTEM_ESCCKDIVCR_ESCCKDIV_Pos        (0UL)          /*!< ESCCKDIV (Bit 0)                                      */
#define R_SYSTEM_ESCCKDIVCR_ESCCKDIV_Msk        (0xfUL)        /*!< ESCCKDIV (Bitfield-Mask: 0x0f)                        */
/* ======================================================  ETHPCKDIVCR  ====================================================== */
#define R_SYSTEM_ETHPCKDIVCR_ETHPCKDIV_Pos      (0UL)          /*!< ETHPCKDIV (Bit 0)                                     */
#define R_SYSTEM_ETHPCKDIVCR_ETHPCKDIV_Msk      (0xfUL)        /*!< ETHPCKDIV (Bitfield-Mask: 0x0f)                       */
/* =====================================================  DSMIFCKDIVCR  ====================================================== */
#define R_SYSTEM_DSMIFCKDIVCR_DSMIFCKDIV_Pos    (0UL)          /*!< DSMIFCKDIV (Bit 0)                                    */
#define R_SYSTEM_DSMIFCKDIVCR_DSMIFCKDIV_Msk    (0xfUL)        /*!< DSMIFCKDIV (Bitfield-Mask: 0x0f)                      */
/* ========================================================  BCKACR  ========================================================= */
#define R_SYSTEM_BCKACR_BCKACKSRDY_Pos          (7UL)          /*!< BCKACKSRDY (Bit 7)                                    */
#define R_SYSTEM_BCKACR_BCKACKSRDY_Msk          (0x80UL)       /*!< BCKACKSRDY (Bitfield-Mask: 0x01)                      */
#define R_SYSTEM_BCKACR_BCKACKSREQ_Pos          (6UL)          /*!< BCKACKSREQ (Bit 6)                                    */
#define R_SYSTEM_BCKACR_BCKACKSREQ_Msk          (0x40UL)       /*!< BCKACKSREQ (Bitfield-Mask: 0x01)                      */
#define R_SYSTEM_BCKACR_BCKACKSEL_Pos           (0UL)          /*!< BCKACKSEL (Bit 0)                                     */
#define R_SYSTEM_BCKACR_BCKACKSEL_Msk           (0xfUL)        /*!< BCKACKSEL (Bitfield-Mask: 0x0f)                       */
/* ========================================================  ESWCKCR  ======================================================== */
#define R_SYSTEM_ESWCKCR_ESWCKSRDY_Pos          (7UL)          /*!< ESWCKSRDY (Bit 7)                                     */
#define R_SYSTEM_ESWCKCR_ESWCKSRDY_Msk          (0x80UL)       /*!< ESWCKSRDY (Bitfield-Mask: 0x01)                       */
#define R_SYSTEM_ESWCKCR_ESWCKSREQ_Pos          (6UL)          /*!< ESWCKSREQ (Bit 6)                                     */
#define R_SYSTEM_ESWCKCR_ESWCKSREQ_Msk          (0x40UL)       /*!< ESWCKSREQ (Bitfield-Mask: 0x01)                       */
#define R_SYSTEM_ESWCKCR_ESWCKSEL_Pos           (0UL)          /*!< ESWCKSEL (Bit 0)                                      */
#define R_SYSTEM_ESWCKCR_ESWCKSEL_Msk           (0xfUL)        /*!< ESWCKSEL (Bitfield-Mask: 0x0f)                        */
/* =======================================================  ESWPCKCR  ======================================================== */
#define R_SYSTEM_ESWPCKCR_ESWPCKSRDY_Pos        (7UL)          /*!< ESWPCKSRDY (Bit 7)                                    */
#define R_SYSTEM_ESWPCKCR_ESWPCKSRDY_Msk        (0x80UL)       /*!< ESWPCKSRDY (Bitfield-Mask: 0x01)                      */
#define R_SYSTEM_ESWPCKCR_ESWPCKSREQ_Pos        (6UL)          /*!< ESWPCKSREQ (Bit 6)                                    */
#define R_SYSTEM_ESWPCKCR_ESWPCKSREQ_Msk        (0x40UL)       /*!< ESWPCKSREQ (Bitfield-Mask: 0x01)                      */
#define R_SYSTEM_ESWPCKCR_ESWPCKSEL_Pos         (0UL)          /*!< ESWPCKSEL (Bit 0)                                     */
#define R_SYSTEM_ESWPCKCR_ESWPCKSEL_Msk         (0xfUL)        /*!< ESWPCKSEL (Bitfield-Mask: 0x0f)                       */
/* ========================================================  ESCCKCR  ======================================================== */
#define R_SYSTEM_ESCCKCR_ESCCKSRDY_Pos          (7UL)          /*!< ESCCKSRDY (Bit 7)                                     */
#define R_SYSTEM_ESCCKCR_ESCCKSRDY_Msk          (0x80UL)       /*!< ESCCKSRDY (Bitfield-Mask: 0x01)                       */
#define R_SYSTEM_ESCCKCR_ESCCKSREQ_Pos          (6UL)          /*!< ESCCKSREQ (Bit 6)                                     */
#define R_SYSTEM_ESCCKCR_ESCCKSREQ_Msk          (0x40UL)       /*!< ESCCKSREQ (Bitfield-Mask: 0x01)                       */
#define R_SYSTEM_ESCCKCR_ESCCKSEL_Pos           (0UL)          /*!< ESCCKSEL (Bit 0)                                      */
#define R_SYSTEM_ESCCKCR_ESCCKSEL_Msk           (0xfUL)        /*!< ESCCKSEL (Bitfield-Mask: 0x0f)                        */
/* =======================================================  ETHPCKCR  ======================================================== */
#define R_SYSTEM_ETHPCKCR_ETHPCKSRDY_Pos        (7UL)          /*!< ETHPCKSRDY (Bit 7)                                    */
#define R_SYSTEM_ETHPCKCR_ETHPCKSRDY_Msk        (0x80UL)       /*!< ETHPCKSRDY (Bitfield-Mask: 0x01)                      */
#define R_SYSTEM_ETHPCKCR_ETHPCKSREQ_Pos        (6UL)          /*!< ETHPCKSREQ (Bit 6)                                    */
#define R_SYSTEM_ETHPCKCR_ETHPCKSREQ_Msk        (0x40UL)       /*!< ETHPCKSREQ (Bitfield-Mask: 0x01)                      */
#define R_SYSTEM_ETHPCKCR_ETHPCKSEL_Pos         (0UL)          /*!< ETHPCKSEL (Bit 0)                                     */
#define R_SYSTEM_ETHPCKCR_ETHPCKSEL_Msk         (0xfUL)        /*!< ETHPCKSEL (Bitfield-Mask: 0x0f)                       */
/* =======================================================  DSMIFCKCR  ======================================================= */
#define R_SYSTEM_DSMIFCKCR_DSMIFCKSRDY_Pos      (7UL)          /*!< DSMIFCKSRDY (Bit 7)                                   */
#define R_SYSTEM_DSMIFCKCR_DSMIFCKSRDY_Msk      (0x80UL)       /*!< DSMIFCKSRDY (Bitfield-Mask: 0x01)                     */
#define R_SYSTEM_DSMIFCKCR_DSMIFCKSREQ_Pos      (6UL)          /*!< DSMIFCKSREQ (Bit 6)                                   */
#define R_SYSTEM_DSMIFCKCR_DSMIFCKSREQ_Msk      (0x40UL)       /*!< DSMIFCKSREQ (Bitfield-Mask: 0x01)                     */
#define R_SYSTEM_DSMIFCKCR_DSMIFCKSEL_Pos       (0UL)          /*!< DSMIFCKSEL (Bit 0)                                    */
#define R_SYSTEM_DSMIFCKCR_DSMIFCKSEL_Msk       (0xfUL)        /*!< DSMIFCKSEL (Bitfield-Mask: 0x0f)                      */
/* ========================================================  CGFSAR  ========================================================= */
#define R_SYSTEM_CGFSAR_NONSEC29_Pos            (29UL)         /*!< NONSEC29 (Bit 29)                                     */
#define R_SYSTEM_CGFSAR_NONSEC29_Msk            (0x20000000UL) /*!< NONSEC29 (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_CGFSAR_NONSEC28_Pos            (28UL)         /*!< NONSEC28 (Bit 28)                                     */
#define R_SYSTEM_CGFSAR_NONSEC28_Msk            (0x10000000UL) /*!< NONSEC28 (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_CGFSAR_NONSEC27_Pos            (27UL)         /*!< NONSEC27 (Bit 27)                                     */
#define R_SYSTEM_CGFSAR_NONSEC27_Msk            (0x8000000UL)  /*!< NONSEC27 (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_CGFSAR_NONSEC25_Pos            (25UL)         /*!< NONSEC25 (Bit 25)                                     */
#define R_SYSTEM_CGFSAR_NONSEC25_Msk            (0x2000000UL)  /*!< NONSEC25 (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_CGFSAR_NONSEC24_Pos            (24UL)         /*!< NONSEC24 (Bit 24)                                     */
#define R_SYSTEM_CGFSAR_NONSEC24_Msk            (0x1000000UL)  /*!< NONSEC24 (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_CGFSAR_NONSEC22_Pos            (22UL)         /*!< NONSEC22 (Bit 22)                                     */
#define R_SYSTEM_CGFSAR_NONSEC22_Msk            (0x400000UL)   /*!< NONSEC22 (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_CGFSAR_NONSEC21_Pos            (21UL)         /*!< NONSEC21 (Bit 21)                                     */
#define R_SYSTEM_CGFSAR_NONSEC21_Msk            (0x200000UL)   /*!< NONSEC21 (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_CGFSAR_NONSEC20_Pos            (20UL)         /*!< NONSEC20 (Bit 20)                                     */
#define R_SYSTEM_CGFSAR_NONSEC20_Msk            (0x100000UL)   /*!< NONSEC20 (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_CGFSAR_NONSEC19_Pos            (19UL)         /*!< NONSEC19 (Bit 19)                                     */
#define R_SYSTEM_CGFSAR_NONSEC19_Msk            (0x80000UL)    /*!< NONSEC19 (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_CGFSAR_NONSEC18_Pos            (18UL)         /*!< NONSEC18 (Bit 18)                                     */
#define R_SYSTEM_CGFSAR_NONSEC18_Msk            (0x40000UL)    /*!< NONSEC18 (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_CGFSAR_NONSEC17_Pos            (17UL)         /*!< NONSEC17 (Bit 17)                                     */
#define R_SYSTEM_CGFSAR_NONSEC17_Msk            (0x20000UL)    /*!< NONSEC17 (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_CGFSAR_NONSEC16_Pos            (16UL)         /*!< NONSEC16 (Bit 16)                                     */
#define R_SYSTEM_CGFSAR_NONSEC16_Msk            (0x10000UL)    /*!< NONSEC16 (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_CGFSAR_NONSEC14_Pos            (14UL)         /*!< NONSEC14 (Bit 14)                                     */
#define R_SYSTEM_CGFSAR_NONSEC14_Msk            (0x4000UL)     /*!< NONSEC14 (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_CGFSAR_NONSEC13_Pos            (13UL)         /*!< NONSEC13 (Bit 13)                                     */
#define R_SYSTEM_CGFSAR_NONSEC13_Msk            (0x2000UL)     /*!< NONSEC13 (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_CGFSAR_NONSEC12_Pos            (12UL)         /*!< NONSEC12 (Bit 12)                                     */
#define R_SYSTEM_CGFSAR_NONSEC12_Msk            (0x1000UL)     /*!< NONSEC12 (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_CGFSAR_NONSEC11_Pos            (11UL)         /*!< NONSEC11 (Bit 11)                                     */
#define R_SYSTEM_CGFSAR_NONSEC11_Msk            (0x800UL)      /*!< NONSEC11 (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_CGFSAR_NONSEC10_Pos            (10UL)         /*!< NONSEC10 (Bit 10)                                     */
#define R_SYSTEM_CGFSAR_NONSEC10_Msk            (0x400UL)      /*!< NONSEC10 (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_CGFSAR_NONSEC09_Pos            (9UL)          /*!< NONSEC09 (Bit 9)                                      */
#define R_SYSTEM_CGFSAR_NONSEC09_Msk            (0x200UL)      /*!< NONSEC09 (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_CGFSAR_NONSEC08_Pos            (8UL)          /*!< NONSEC08 (Bit 8)                                      */
#define R_SYSTEM_CGFSAR_NONSEC08_Msk            (0x100UL)      /*!< NONSEC08 (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_CGFSAR_NONSEC07_Pos            (7UL)          /*!< NONSEC07 (Bit 7)                                      */
#define R_SYSTEM_CGFSAR_NONSEC07_Msk            (0x80UL)       /*!< NONSEC07 (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_CGFSAR_NONSEC06_Pos            (6UL)          /*!< NONSEC06 (Bit 6)                                      */
#define R_SYSTEM_CGFSAR_NONSEC06_Msk            (0x40UL)       /*!< NONSEC06 (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_CGFSAR_NONSEC05_Pos            (5UL)          /*!< NONSEC05 (Bit 5)                                      */
#define R_SYSTEM_CGFSAR_NONSEC05_Msk            (0x20UL)       /*!< NONSEC05 (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_CGFSAR_NONSEC04_Pos            (4UL)          /*!< NONSEC04 (Bit 4)                                      */
#define R_SYSTEM_CGFSAR_NONSEC04_Msk            (0x10UL)       /*!< NONSEC04 (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_CGFSAR_NONSEC03_Pos            (3UL)          /*!< NONSEC03 (Bit 3)                                      */
#define R_SYSTEM_CGFSAR_NONSEC03_Msk            (0x8UL)        /*!< NONSEC03 (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_CGFSAR_NONSEC02_Pos            (2UL)          /*!< NONSEC02 (Bit 2)                                      */
#define R_SYSTEM_CGFSAR_NONSEC02_Msk            (0x4UL)        /*!< NONSEC02 (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_CGFSAR_NONSEC00_Pos            (0UL)          /*!< NONSEC00 (Bit 0)                                      */
#define R_SYSTEM_CGFSAR_NONSEC00_Msk            (0x1UL)        /*!< NONSEC00 (Bitfield-Mask: 0x01)                        */
/* ========================================================  LOCOCR  ========================================================= */
#define R_SYSTEM_LOCOCR_LCSTP_Pos               (0UL)          /*!< LCSTP (Bit 0)                                         */
#define R_SYSTEM_LOCOCR_LCSTP_Msk               (0x1UL)        /*!< LCSTP (Bitfield-Mask: 0x01)                           */
/* =======================================================  LOCOUTCR  ======================================================== */
#define R_SYSTEM_LOCOUTCR_LOCOUTRM_Pos          (0UL)          /*!< LOCOUTRM (Bit 0)                                      */
#define R_SYSTEM_LOCOUTCR_LOCOUTRM_Msk          (0xffUL)       /*!< LOCOUTRM (Bitfield-Mask: 0xff)                        */
/* =========================================================  MOMCR  ========================================================= */
#define R_SYSTEM_MOMCR_MOSEL_Pos                (6UL)          /*!< MOSEL (Bit 6)                                         */
#define R_SYSTEM_MOMCR_MOSEL_Msk                (0x40UL)       /*!< MOSEL (Bitfield-Mask: 0x01)                           */
#define R_SYSTEM_MOMCR_MODRV0_Pos               (1UL)          /*!< MODRV0 (Bit 1)                                        */
#define R_SYSTEM_MOMCR_MODRV0_Msk               (0xeUL)        /*!< MODRV0 (Bitfield-Mask: 0x07)                          */
/* ========================================================  SOSCCR  ========================================================= */
#define R_SYSTEM_SOSCCR_SOSTP_Pos               (0UL)          /*!< SOSTP (Bit 0)                                         */
#define R_SYSTEM_SOSCCR_SOSTP_Msk               (0x1UL)        /*!< SOSTP (Bitfield-Mask: 0x01)                           */
/* =========================================================  SOMCR  ========================================================= */
#define R_SYSTEM_SOMCR_SOSEL_Pos                (6UL)          /*!< SOSEL (Bit 6)                                         */
#define R_SYSTEM_SOMCR_SOSEL_Msk                (0x40UL)       /*!< SOSEL (Bitfield-Mask: 0x01)                           */
#define R_SYSTEM_SOMCR_SODRV_Pos                (0UL)          /*!< SODRV (Bit 0)                                         */
#define R_SYSTEM_SOMCR_SODRV_Msk                (0x3UL)        /*!< SODRV (Bitfield-Mask: 0x03)                           */
/* ========================================================  SOSTDCR  ======================================================== */
#define R_SYSTEM_SOSTDCR_SOSTDE_Pos             (7UL)          /*!< SOSTDE (Bit 7)                                        */
#define R_SYSTEM_SOSTDCR_SOSTDE_Msk             (0x80UL)       /*!< SOSTDE (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_SOSTDCR_SOSTDIE_Pos            (0UL)          /*!< SOSTDIE (Bit 0)                                       */
#define R_SYSTEM_SOSTDCR_SOSTDIE_Msk            (0x1UL)        /*!< SOSTDIE (Bitfield-Mask: 0x01)                         */
/* ========================================================  SOSTDSR  ======================================================== */
#define R_SYSTEM_SOSTDSR_SOSTDF_Pos             (0UL)          /*!< SOSTDF (Bit 0)                                        */
#define R_SYSTEM_SOSTDSR_SOSTDF_Msk             (0x1UL)        /*!< SOSTDF (Bitfield-Mask: 0x01)                          */
/* ========================================================  LPMSAR  ========================================================= */
#define R_SYSTEM_LPMSAR_NONSEC21_Pos            (21UL)         /*!< NONSEC21 (Bit 21)                                     */
#define R_SYSTEM_LPMSAR_NONSEC21_Msk            (0x200000UL)   /*!< NONSEC21 (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_LPMSAR_NONSEC19_Pos            (19UL)         /*!< NONSEC19 (Bit 19)                                     */
#define R_SYSTEM_LPMSAR_NONSEC19_Msk            (0x80000UL)    /*!< NONSEC19 (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_LPMSAR_NONSEC18_Pos            (18UL)         /*!< NONSEC18 (Bit 18)                                     */
#define R_SYSTEM_LPMSAR_NONSEC18_Msk            (0x40000UL)    /*!< NONSEC18 (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_LPMSAR_NONSEC17_Pos            (17UL)         /*!< NONSEC17 (Bit 17)                                     */
#define R_SYSTEM_LPMSAR_NONSEC17_Msk            (0x20000UL)    /*!< NONSEC17 (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_LPMSAR_NONSEC8_Pos             (8UL)          /*!< NONSEC8 (Bit 8)                                       */
#define R_SYSTEM_LPMSAR_NONSEC8_Msk             (0x100UL)      /*!< NONSEC8 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_LPMSAR_NONSEC4_Pos             (4UL)          /*!< NONSEC4 (Bit 4)                                       */
#define R_SYSTEM_LPMSAR_NONSEC4_Msk             (0x10UL)       /*!< NONSEC4 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_LPMSAR_NONSEC2_Pos             (2UL)          /*!< NONSEC2 (Bit 2)                                       */
#define R_SYSTEM_LPMSAR_NONSEC2_Msk             (0x4UL)        /*!< NONSEC2 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_LPMSAR_NONSEC1_Pos             (1UL)          /*!< NONSEC1 (Bit 1)                                       */
#define R_SYSTEM_LPMSAR_NONSEC1_Msk             (0x2UL)        /*!< NONSEC1 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_LPMSAR_NONSEC0_Pos             (0UL)          /*!< NONSEC0 (Bit 0)                                       */
#define R_SYSTEM_LPMSAR_NONSEC0_Msk             (0x1UL)        /*!< NONSEC0 (Bitfield-Mask: 0x01)                         */
/* ========================================================  PGCSAR  ========================================================= */
#define R_SYSTEM_PGCSAR_NONSEC7_Pos             (7UL)          /*!< NONSEC7 (Bit 7)                                       */
#define R_SYSTEM_PGCSAR_NONSEC7_Msk             (0x80UL)       /*!< NONSEC7 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_PGCSAR_NONSEC6_Pos             (6UL)          /*!< NONSEC6 (Bit 6)                                       */
#define R_SYSTEM_PGCSAR_NONSEC6_Msk             (0x40UL)       /*!< NONSEC6 (Bitfield-Mask: 0x01)                         */
/* ========================================================  DPFSAR  ========================================================= */
#define R_SYSTEM_DPFSAR_DFPSA31_Pos             (31UL)         /*!< DFPSA31 (Bit 31)                                      */
#define R_SYSTEM_DPFSAR_DFPSA31_Msk             (0x80000000UL) /*!< DFPSA31 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPFSAR_DPFSA30_Pos             (30UL)         /*!< DPFSA30 (Bit 30)                                      */
#define R_SYSTEM_DPFSAR_DPFSA30_Msk             (0x40000000UL) /*!< DPFSA30 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPFSAR_DPFSA29_Pos             (29UL)         /*!< DPFSA29 (Bit 29)                                      */
#define R_SYSTEM_DPFSAR_DPFSA29_Msk             (0x20000000UL) /*!< DPFSA29 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPFSAR_DPFSA25_Pos             (25UL)         /*!< DPFSA25 (Bit 25)                                      */
#define R_SYSTEM_DPFSAR_DPFSA25_Msk             (0x2000000UL)  /*!< DPFSA25 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPFSAR_DPFSA24_Pos             (24UL)         /*!< DPFSA24 (Bit 24)                                      */
#define R_SYSTEM_DPFSAR_DPFSA24_Msk             (0x1000000UL)  /*!< DPFSA24 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPFSAR_DPFSA20_Pos             (20UL)         /*!< DPFSA20 (Bit 20)                                      */
#define R_SYSTEM_DPFSAR_DPFSA20_Msk             (0x100000UL)   /*!< DPFSA20 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPFSAR_DPFSA19_Pos             (19UL)         /*!< DPFSA19 (Bit 19)                                      */
#define R_SYSTEM_DPFSAR_DPFSA19_Msk             (0x80000UL)    /*!< DPFSA19 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPFSAR_DPFSA18_Pos             (18UL)         /*!< DPFSA18 (Bit 18)                                      */
#define R_SYSTEM_DPFSAR_DPFSA18_Msk             (0x40000UL)    /*!< DPFSA18 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPFSAR_DPFSA17_Pos             (17UL)         /*!< DPFSA17 (Bit 17)                                      */
#define R_SYSTEM_DPFSAR_DPFSA17_Msk             (0x20000UL)    /*!< DPFSA17 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPFSAR_DPFSA16_Pos             (16UL)         /*!< DPFSA16 (Bit 16)                                      */
#define R_SYSTEM_DPFSAR_DPFSA16_Msk             (0x10000UL)    /*!< DPFSA16 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPFSAR_DPFSA15_Pos             (15UL)         /*!< DPFSA15 (Bit 15)                                      */
#define R_SYSTEM_DPFSAR_DPFSA15_Msk             (0x8000UL)     /*!< DPFSA15 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPFSAR_DPFSA14_Pos             (14UL)         /*!< DPFSA14 (Bit 14)                                      */
#define R_SYSTEM_DPFSAR_DPFSA14_Msk             (0x4000UL)     /*!< DPFSA14 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPFSAR_DPFSA13_Pos             (13UL)         /*!< DPFSA13 (Bit 13)                                      */
#define R_SYSTEM_DPFSAR_DPFSA13_Msk             (0x2000UL)     /*!< DPFSA13 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPFSAR_DPFSA12_Pos             (12UL)         /*!< DPFSA12 (Bit 12)                                      */
#define R_SYSTEM_DPFSAR_DPFSA12_Msk             (0x1000UL)     /*!< DPFSA12 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPFSAR_DPFSA11_Pos             (11UL)         /*!< DPFSA11 (Bit 11)                                      */
#define R_SYSTEM_DPFSAR_DPFSA11_Msk             (0x800UL)      /*!< DPFSA11 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPFSAR_DPFSA10_Pos             (10UL)         /*!< DPFSA10 (Bit 10)                                      */
#define R_SYSTEM_DPFSAR_DPFSA10_Msk             (0x400UL)      /*!< DPFSA10 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPFSAR_DPFSA9_Pos              (9UL)          /*!< DPFSA9 (Bit 9)                                        */
#define R_SYSTEM_DPFSAR_DPFSA9_Msk              (0x200UL)      /*!< DPFSA9 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_DPFSAR_DPFSA8_Pos              (8UL)          /*!< DPFSA8 (Bit 8)                                        */
#define R_SYSTEM_DPFSAR_DPFSA8_Msk              (0x100UL)      /*!< DPFSA8 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_DPFSAR_DPFSA7_Pos              (7UL)          /*!< DPFSA7 (Bit 7)                                        */
#define R_SYSTEM_DPFSAR_DPFSA7_Msk              (0x80UL)       /*!< DPFSA7 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_DPFSAR_DPFSA6_Pos              (6UL)          /*!< DPFSA6 (Bit 6)                                        */
#define R_SYSTEM_DPFSAR_DPFSA6_Msk              (0x40UL)       /*!< DPFSA6 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_DPFSAR_DPFSA5_Pos              (5UL)          /*!< DPFSA5 (Bit 5)                                        */
#define R_SYSTEM_DPFSAR_DPFSA5_Msk              (0x20UL)       /*!< DPFSA5 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_DPFSAR_DPFSA4_Pos              (4UL)          /*!< DPFSA4 (Bit 4)                                        */
#define R_SYSTEM_DPFSAR_DPFSA4_Msk              (0x10UL)       /*!< DPFSA4 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_DPFSAR_DPFSA3_Pos              (3UL)          /*!< DPFSA3 (Bit 3)                                        */
#define R_SYSTEM_DPFSAR_DPFSA3_Msk              (0x8UL)        /*!< DPFSA3 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_DPFSAR_DPFSA2_Pos              (2UL)          /*!< DPFSA2 (Bit 2)                                        */
#define R_SYSTEM_DPFSAR_DPFSA2_Msk              (0x4UL)        /*!< DPFSA2 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_DPFSAR_DPFSA1_Pos              (1UL)          /*!< DPFSA1 (Bit 1)                                        */
#define R_SYSTEM_DPFSAR_DPFSA1_Msk              (0x2UL)        /*!< DPFSA1 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_DPFSAR_DPFSA0_Pos              (0UL)          /*!< DPFSA0 (Bit 0)                                        */
#define R_SYSTEM_DPFSAR_DPFSA0_Msk              (0x1UL)        /*!< DPFSA0 (Bitfield-Mask: 0x01)                          */
/* ========================================================  DPFSAR1  ======================================================== */
#define R_SYSTEM_DPFSAR1_DPFSA15_Pos            (15UL)         /*!< DPFSA15 (Bit 15)                                      */
#define R_SYSTEM_DPFSAR1_DPFSA15_Msk            (0x8000UL)     /*!< DPFSA15 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPFSAR1_DPFSA14_Pos            (14UL)         /*!< DPFSA14 (Bit 14)                                      */
#define R_SYSTEM_DPFSAR1_DPFSA14_Msk            (0x4000UL)     /*!< DPFSA14 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPFSAR1_DPFSA13_Pos            (13UL)         /*!< DPFSA13 (Bit 13)                                      */
#define R_SYSTEM_DPFSAR1_DPFSA13_Msk            (0x2000UL)     /*!< DPFSA13 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPFSAR1_DPFSA12_Pos            (12UL)         /*!< DPFSA12 (Bit 12)                                      */
#define R_SYSTEM_DPFSAR1_DPFSA12_Msk            (0x1000UL)     /*!< DPFSA12 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPFSAR1_DPFSA11_Pos            (11UL)         /*!< DPFSA11 (Bit 11)                                      */
#define R_SYSTEM_DPFSAR1_DPFSA11_Msk            (0x800UL)      /*!< DPFSA11 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPFSAR1_DPFSA10_Pos            (10UL)         /*!< DPFSA10 (Bit 10)                                      */
#define R_SYSTEM_DPFSAR1_DPFSA10_Msk            (0x400UL)      /*!< DPFSA10 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPFSAR1_DPFSA9_Pos             (9UL)          /*!< DPFSA9 (Bit 9)                                        */
#define R_SYSTEM_DPFSAR1_DPFSA9_Msk             (0x200UL)      /*!< DPFSA9 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_DPFSAR1_DPFSA8_Pos             (8UL)          /*!< DPFSA8 (Bit 8)                                        */
#define R_SYSTEM_DPFSAR1_DPFSA8_Msk             (0x100UL)      /*!< DPFSA8 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_DPFSAR1_DPFSA7_Pos             (7UL)          /*!< DPFSA7 (Bit 7)                                        */
#define R_SYSTEM_DPFSAR1_DPFSA7_Msk             (0x80UL)       /*!< DPFSA7 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_DPFSAR1_DPFSA6_Pos             (6UL)          /*!< DPFSA6 (Bit 6)                                        */
#define R_SYSTEM_DPFSAR1_DPFSA6_Msk             (0x40UL)       /*!< DPFSA6 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_DPFSAR1_DPFSA5_Pos             (5UL)          /*!< DPFSA5 (Bit 5)                                        */
#define R_SYSTEM_DPFSAR1_DPFSA5_Msk             (0x20UL)       /*!< DPFSA5 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_DPFSAR1_DPFSA4_Pos             (4UL)          /*!< DPFSA4 (Bit 4)                                        */
#define R_SYSTEM_DPFSAR1_DPFSA4_Msk             (0x10UL)       /*!< DPFSA4 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_DPFSAR1_DPFSA3_Pos             (3UL)          /*!< DPFSA3 (Bit 3)                                        */
#define R_SYSTEM_DPFSAR1_DPFSA3_Msk             (0x8UL)        /*!< DPFSA3 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_DPFSAR1_DPFSA2_Pos             (2UL)          /*!< DPFSA2 (Bit 2)                                        */
#define R_SYSTEM_DPFSAR1_DPFSA2_Msk             (0x4UL)        /*!< DPFSA2 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_DPFSAR1_DPFSA1_Pos             (1UL)          /*!< DPFSA1 (Bit 1)                                        */
#define R_SYSTEM_DPFSAR1_DPFSA1_Msk             (0x2UL)        /*!< DPFSA1 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_DPFSAR1_DPFSA0_Pos             (0UL)          /*!< DPFSA0 (Bit 0)                                        */
#define R_SYSTEM_DPFSAR1_DPFSA0_Msk             (0x1UL)        /*!< DPFSA0 (Bitfield-Mask: 0x01)                          */
/* ========================================================  RSCSAR  ========================================================= */
#define R_SYSTEM_RSCSAR_RSCSA25_Pos             (25UL)         /*!< RSCSA25 (Bit 25)                                      */
#define R_SYSTEM_RSCSAR_RSCSA25_Msk             (0x2000000UL)  /*!< RSCSA25 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_RSCSAR_RSCSA24_Pos             (24UL)         /*!< RSCSA24 (Bit 24)                                      */
#define R_SYSTEM_RSCSAR_RSCSA24_Msk             (0x1000000UL)  /*!< RSCSA24 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_RSCSAR_RSCSA14_Pos             (14UL)         /*!< RSCSA14 (Bit 14)                                      */
#define R_SYSTEM_RSCSAR_RSCSA14_Msk             (0x4000UL)     /*!< RSCSA14 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_RSCSAR_RSCSA13_Pos             (13UL)         /*!< RSCSA13 (Bit 13)                                      */
#define R_SYSTEM_RSCSAR_RSCSA13_Msk             (0x2000UL)     /*!< RSCSA13 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_RSCSAR_RSCSA12_Pos             (12UL)         /*!< RSCSA12 (Bit 12)                                      */
#define R_SYSTEM_RSCSAR_RSCSA12_Msk             (0x1000UL)     /*!< RSCSA12 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_RSCSAR_RSCSA11_Pos             (11UL)         /*!< RSCSA11 (Bit 11)                                      */
#define R_SYSTEM_RSCSAR_RSCSA11_Msk             (0x800UL)      /*!< RSCSA11 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_RSCSAR_RSCSA10_Pos             (10UL)         /*!< RSCSA10 (Bit 10)                                      */
#define R_SYSTEM_RSCSAR_RSCSA10_Msk             (0x400UL)      /*!< RSCSA10 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_RSCSAR_RSCSA9_Pos              (9UL)          /*!< RSCSA9 (Bit 9)                                        */
#define R_SYSTEM_RSCSAR_RSCSA9_Msk              (0x200UL)      /*!< RSCSA9 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_RSCSAR_RSCSA8_Pos              (8UL)          /*!< RSCSA8 (Bit 8)                                        */
#define R_SYSTEM_RSCSAR_RSCSA8_Msk              (0x100UL)      /*!< RSCSA8 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_RSCSAR_RSCSA7_Pos              (7UL)          /*!< RSCSA7 (Bit 7)                                        */
#define R_SYSTEM_RSCSAR_RSCSA7_Msk              (0x80UL)       /*!< RSCSA7 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_RSCSAR_RSCSA6_Pos              (6UL)          /*!< RSCSA6 (Bit 6)                                        */
#define R_SYSTEM_RSCSAR_RSCSA6_Msk              (0x40UL)       /*!< RSCSA6 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_RSCSAR_RSCSA5_Pos              (5UL)          /*!< RSCSA5 (Bit 5)                                        */
#define R_SYSTEM_RSCSAR_RSCSA5_Msk              (0x20UL)       /*!< RSCSA5 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_RSCSAR_RSCSA4_Pos              (4UL)          /*!< RSCSA4 (Bit 4)                                        */
#define R_SYSTEM_RSCSAR_RSCSA4_Msk              (0x10UL)       /*!< RSCSA4 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_RSCSAR_RSCSA3_Pos              (3UL)          /*!< RSCSA3 (Bit 3)                                        */
#define R_SYSTEM_RSCSAR_RSCSA3_Msk              (0x8UL)        /*!< RSCSA3 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_RSCSAR_RSCSA2_Pos              (2UL)          /*!< RSCSA2 (Bit 2)                                        */
#define R_SYSTEM_RSCSAR_RSCSA2_Msk              (0x4UL)        /*!< RSCSA2 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_RSCSAR_RSCSA1_Pos              (1UL)          /*!< RSCSA1 (Bit 1)                                        */
#define R_SYSTEM_RSCSAR_RSCSA1_Msk              (0x2UL)        /*!< RSCSA1 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_RSCSAR_RSCSA0_Pos              (0UL)          /*!< RSCSA0 (Bit 0)                                        */
#define R_SYSTEM_RSCSAR_RSCSA0_Msk              (0x1UL)        /*!< RSCSA0 (Bitfield-Mask: 0x01)                          */
/* =========================================================  OPCCR  ========================================================= */
#define R_SYSTEM_OPCCR_OPCMTSF_Pos              (4UL)          /*!< OPCMTSF (Bit 4)                                       */
#define R_SYSTEM_OPCCR_OPCMTSF_Msk              (0x10UL)       /*!< OPCMTSF (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_OPCCR_OPCM_Pos                 (0UL)          /*!< OPCM (Bit 0)                                          */
#define R_SYSTEM_OPCCR_OPCM_Msk                 (0x3UL)        /*!< OPCM (Bitfield-Mask: 0x03)                            */
/* =======================================================  PDCTRESWM  ======================================================= */
#define R_SYSTEM_PDCTRESWM_PDPGSF_Pos           (7UL)          /*!< PDPGSF (Bit 7)                                        */
#define R_SYSTEM_PDCTRESWM_PDPGSF_Msk           (0x80UL)       /*!< PDPGSF (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_PDCTRESWM_PDCSF_Pos            (6UL)          /*!< PDCSF (Bit 6)                                         */
#define R_SYSTEM_PDCTRESWM_PDCSF_Msk            (0x40UL)       /*!< PDCSF (Bitfield-Mask: 0x01)                           */
#define R_SYSTEM_PDCTRESWM_PDDE_Pos             (0UL)          /*!< PDDE (Bit 0)                                          */
#define R_SYSTEM_PDCTRESWM_PDDE_Msk             (0x1UL)        /*!< PDDE (Bitfield-Mask: 0x01)                            */
/* =======================================================  PDRAMSCR0  ======================================================= */
#define R_SYSTEM_PDRAMSCR0_RKEEP11_Pos          (11UL)         /*!< RKEEP11 (Bit 11)                                      */
#define R_SYSTEM_PDRAMSCR0_RKEEP11_Msk          (0x800UL)      /*!< RKEEP11 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_PDRAMSCR0_RKEEP10_Pos          (10UL)         /*!< RKEEP10 (Bit 10)                                      */
#define R_SYSTEM_PDRAMSCR0_RKEEP10_Msk          (0x400UL)      /*!< RKEEP10 (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_PDRAMSCR0_RKEEP9_Pos           (9UL)          /*!< RKEEP9 (Bit 9)                                        */
#define R_SYSTEM_PDRAMSCR0_RKEEP9_Msk           (0x200UL)      /*!< RKEEP9 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_PDRAMSCR0_RKEEP8_Pos           (8UL)          /*!< RKEEP8 (Bit 8)                                        */
#define R_SYSTEM_PDRAMSCR0_RKEEP8_Msk           (0x100UL)      /*!< RKEEP8 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_PDRAMSCR0_RKEEP7_Pos           (7UL)          /*!< RKEEP7 (Bit 7)                                        */
#define R_SYSTEM_PDRAMSCR0_RKEEP7_Msk           (0x80UL)       /*!< RKEEP7 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_PDRAMSCR0_RKEEP6_Pos           (6UL)          /*!< RKEEP6 (Bit 6)                                        */
#define R_SYSTEM_PDRAMSCR0_RKEEP6_Msk           (0x40UL)       /*!< RKEEP6 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_PDRAMSCR0_RKEEP5_Pos           (5UL)          /*!< RKEEP5 (Bit 5)                                        */
#define R_SYSTEM_PDRAMSCR0_RKEEP5_Msk           (0x20UL)       /*!< RKEEP5 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_PDRAMSCR0_RKEEP4_Pos           (4UL)          /*!< RKEEP4 (Bit 4)                                        */
#define R_SYSTEM_PDRAMSCR0_RKEEP4_Msk           (0x10UL)       /*!< RKEEP4 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_PDRAMSCR0_RKEEP3_Pos           (3UL)          /*!< RKEEP3 (Bit 3)                                        */
#define R_SYSTEM_PDRAMSCR0_RKEEP3_Msk           (0x8UL)        /*!< RKEEP3 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_PDRAMSCR0_RKEEP2_Pos           (2UL)          /*!< RKEEP2 (Bit 2)                                        */
#define R_SYSTEM_PDRAMSCR0_RKEEP2_Msk           (0x4UL)        /*!< RKEEP2 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_PDRAMSCR0_RKEEP1_Pos           (1UL)          /*!< RKEEP1 (Bit 1)                                        */
#define R_SYSTEM_PDRAMSCR0_RKEEP1_Msk           (0x2UL)        /*!< RKEEP1 (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_PDRAMSCR0_RKEEP0_Pos           (0UL)          /*!< RKEEP0 (Bit 0)                                        */
#define R_SYSTEM_PDRAMSCR0_RKEEP0_Msk           (0x1UL)        /*!< RKEEP0 (Bitfield-Mask: 0x01)                          */
/* =======================================================  PDRAMSCR2  ======================================================= */
#define R_SYSTEM_PDRAMSCR2_RKEEP0_Pos           (0UL)          /*!< RKEEP0 (Bit 0)                                        */
#define R_SYSTEM_PDRAMSCR2_RKEEP0_Msk           (0x1UL)        /*!< RKEEP0 (Bitfield-Mask: 0x01)                          */
/* =========================================================  SBYCR  ========================================================= */
#define R_SYSTEM_SBYCR_OPE_Pos                  (6UL)          /*!< OPE (Bit 6)                                           */
#define R_SYSTEM_SBYCR_OPE_Msk                  (0x40UL)       /*!< OPE (Bitfield-Mask: 0x01)                             */
/* =========================================================  SSCR1  ========================================================= */
#define R_SYSTEM_SSCR1_SS2LP_Pos                (2UL)          /*!< SS2LP (Bit 2)                                         */
#define R_SYSTEM_SSCR1_SS2LP_Msk                (0xcUL)        /*!< SS2LP (Bitfield-Mask: 0x03)                           */
#define R_SYSTEM_SSCR1_SS2FR_Pos                (0UL)          /*!< SS2FR (Bit 0)                                         */
#define R_SYSTEM_SSCR1_SS2FR_Msk                (0x1UL)        /*!< SS2FR (Bitfield-Mask: 0x01)                           */
/* =========================================================  SSCR2  ========================================================= */
#define R_SYSTEM_SSCR2_SS2RSF_Pos               (0UL)          /*!< SS2RSF (Bit 0)                                        */
#define R_SYSTEM_SSCR2_SS2RSF_Msk               (0x1UL)        /*!< SS2RSF (Bitfield-Mask: 0x01)                          */
/* =========================================================  LPSCR  ========================================================= */
#define R_SYSTEM_LPSCR_LPMD_Pos                 (0UL)          /*!< LPMD (Bit 0)                                          */
#define R_SYSTEM_LPSCR_LPMD_Msk                 (0xfUL)        /*!< LPMD (Bitfield-Mask: 0x0f)                            */
/* ========================================================  DPSBYCR  ======================================================== */
#define R_SYSTEM_DPSBYCR_IOKEEP_Pos             (6UL)          /*!< IOKEEP (Bit 6)                                        */
#define R_SYSTEM_DPSBYCR_IOKEEP_Msk             (0x40UL)       /*!< IOKEEP (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_DPSBYCR_DCSSMODE_Pos           (2UL)          /*!< DCSSMODE (Bit 2)                                      */
#define R_SYSTEM_DPSBYCR_DCSSMODE_Msk           (0xcUL)        /*!< DCSSMODE (Bitfield-Mask: 0x03)                        */
#define R_SYSTEM_DPSBYCR_LDOSSMODE_Pos          (0UL)          /*!< LDOSSMODE (Bit 0)                                     */
#define R_SYSTEM_DPSBYCR_LDOSSMODE_Msk          (0x3UL)        /*!< LDOSSMODE (Bitfield-Mask: 0x03)                       */
/* ========================================================  DPSIER0  ======================================================== */
#define R_SYSTEM_DPSIER0_DIRQ7E_Pos             (7UL)          /*!< DIRQ7E (Bit 7)                                        */
#define R_SYSTEM_DPSIER0_DIRQ7E_Msk             (0x80UL)       /*!< DIRQ7E (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_DPSIER0_DIRQ6E_Pos             (6UL)          /*!< DIRQ6E (Bit 6)                                        */
#define R_SYSTEM_DPSIER0_DIRQ6E_Msk             (0x40UL)       /*!< DIRQ6E (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_DPSIER0_DIRQ5E_Pos             (5UL)          /*!< DIRQ5E (Bit 5)                                        */
#define R_SYSTEM_DPSIER0_DIRQ5E_Msk             (0x20UL)       /*!< DIRQ5E (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_DPSIER0_DIRQ4E_Pos             (4UL)          /*!< DIRQ4E (Bit 4)                                        */
#define R_SYSTEM_DPSIER0_DIRQ4E_Msk             (0x10UL)       /*!< DIRQ4E (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_DPSIER0_DIRQ3E_Pos             (3UL)          /*!< DIRQ3E (Bit 3)                                        */
#define R_SYSTEM_DPSIER0_DIRQ3E_Msk             (0x8UL)        /*!< DIRQ3E (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_DPSIER0_DIRQ2E_Pos             (2UL)          /*!< DIRQ2E (Bit 2)                                        */
#define R_SYSTEM_DPSIER0_DIRQ2E_Msk             (0x4UL)        /*!< DIRQ2E (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_DPSIER0_DIRQ1E_Pos             (1UL)          /*!< DIRQ1E (Bit 1)                                        */
#define R_SYSTEM_DPSIER0_DIRQ1E_Msk             (0x2UL)        /*!< DIRQ1E (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_DPSIER0_DIRQ0E_Pos             (0UL)          /*!< DIRQ0E (Bit 0)                                        */
#define R_SYSTEM_DPSIER0_DIRQ0E_Msk             (0x1UL)        /*!< DIRQ0E (Bitfield-Mask: 0x01)                          */
/* ========================================================  DPSIER1  ======================================================== */
#define R_SYSTEM_DPSIER1_DIRQ15E_Pos            (7UL)          /*!< DIRQ15E (Bit 7)                                       */
#define R_SYSTEM_DPSIER1_DIRQ15E_Msk            (0x80UL)       /*!< DIRQ15E (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIER1_DIRQ14E_Pos            (6UL)          /*!< DIRQ14E (Bit 6)                                       */
#define R_SYSTEM_DPSIER1_DIRQ14E_Msk            (0x40UL)       /*!< DIRQ14E (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIER1_DIRQ13E_Pos            (5UL)          /*!< DIRQ13E (Bit 5)                                       */
#define R_SYSTEM_DPSIER1_DIRQ13E_Msk            (0x20UL)       /*!< DIRQ13E (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIER1_DIRQ12E_Pos            (4UL)          /*!< DIRQ12E (Bit 4)                                       */
#define R_SYSTEM_DPSIER1_DIRQ12E_Msk            (0x10UL)       /*!< DIRQ12E (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIER1_DIRQ11E_Pos            (3UL)          /*!< DIRQ11E (Bit 3)                                       */
#define R_SYSTEM_DPSIER1_DIRQ11E_Msk            (0x8UL)        /*!< DIRQ11E (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIER1_DIRQ10E_Pos            (2UL)          /*!< DIRQ10E (Bit 2)                                       */
#define R_SYSTEM_DPSIER1_DIRQ10E_Msk            (0x4UL)        /*!< DIRQ10E (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIER1_DIRQ9E_Pos             (1UL)          /*!< DIRQ9E (Bit 1)                                        */
#define R_SYSTEM_DPSIER1_DIRQ9E_Msk             (0x2UL)        /*!< DIRQ9E (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_DPSIER1_DIRQ8E_Pos             (0UL)          /*!< DIRQ8E (Bit 0)                                        */
#define R_SYSTEM_DPSIER1_DIRQ8E_Msk             (0x1UL)        /*!< DIRQ8E (Bitfield-Mask: 0x01)                          */
/* ========================================================  DPSIER2  ======================================================== */
#define R_SYSTEM_DPSIER2_DNMIE_Pos              (4UL)          /*!< DNMIE (Bit 4)                                         */
#define R_SYSTEM_DPSIER2_DNMIE_Msk              (0x10UL)       /*!< DNMIE (Bitfield-Mask: 0x01)                           */
#define R_SYSTEM_DPSIER2_DRTCAIE_Pos            (3UL)          /*!< DRTCAIE (Bit 3)                                       */
#define R_SYSTEM_DPSIER2_DRTCAIE_Msk            (0x8UL)        /*!< DRTCAIE (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIER2_DRTCIIE_Pos            (2UL)          /*!< DRTCIIE (Bit 2)                                       */
#define R_SYSTEM_DPSIER2_DRTCIIE_Msk            (0x4UL)        /*!< DRTCIIE (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIER2_DPVD2IE_Pos            (1UL)          /*!< DPVD2IE (Bit 1)                                       */
#define R_SYSTEM_DPSIER2_DPVD2IE_Msk            (0x2UL)        /*!< DPVD2IE (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIER2_DPVD1IE_Pos            (0UL)          /*!< DPVD1IE (Bit 0)                                       */
#define R_SYSTEM_DPSIER2_DPVD1IE_Msk            (0x1UL)        /*!< DPVD1IE (Bitfield-Mask: 0x01)                         */
/* ========================================================  DPSIER3  ======================================================== */
#define R_SYSTEM_DPSIER3_DVBATTADIE_Pos         (7UL)          /*!< DVBATTADIE (Bit 7)                                    */
#define R_SYSTEM_DPSIER3_DVBATTADIE_Msk         (0x80UL)       /*!< DVBATTADIE (Bitfield-Mask: 0x01)                      */
#define R_SYSTEM_DPSIER3_DSOSTDIE_Pos           (6UL)          /*!< DSOSTDIE (Bit 6)                                      */
#define R_SYSTEM_DPSIER3_DSOSTDIE_Msk           (0x40UL)       /*!< DSOSTDIE (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_DPSIER3_DIWDTIE_Pos            (5UL)          /*!< DIWDTIE (Bit 5)                                       */
#define R_SYSTEM_DPSIER3_DIWDTIE_Msk            (0x20UL)       /*!< DIWDTIE (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIER3_DUSBHSIE_Pos           (1UL)          /*!< DUSBHSIE (Bit 1)                                      */
#define R_SYSTEM_DPSIER3_DUSBHSIE_Msk           (0x2UL)        /*!< DUSBHSIE (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_DPSIER3_DUSBFSIE_Pos           (0UL)          /*!< DUSBFSIE (Bit 0)                                      */
#define R_SYSTEM_DPSIER3_DUSBFSIE_Msk           (0x1UL)        /*!< DUSBFSIE (Bitfield-Mask: 0x01)                        */
/* ========================================================  DPSIER4  ======================================================== */
#define R_SYSTEM_DPSIER4_DIRQ23E_Pos            (7UL)          /*!< DIRQ23E (Bit 7)                                       */
#define R_SYSTEM_DPSIER4_DIRQ23E_Msk            (0x80UL)       /*!< DIRQ23E (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIER4_DIRQ22E_Pos            (6UL)          /*!< DIRQ22E (Bit 6)                                       */
#define R_SYSTEM_DPSIER4_DIRQ22E_Msk            (0x40UL)       /*!< DIRQ22E (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIER4_DIRQ21E_Pos            (5UL)          /*!< DIRQ21E (Bit 5)                                       */
#define R_SYSTEM_DPSIER4_DIRQ21E_Msk            (0x20UL)       /*!< DIRQ21E (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIER4_DIRQ20E_Pos            (4UL)          /*!< DIRQ20E (Bit 4)                                       */
#define R_SYSTEM_DPSIER4_DIRQ20E_Msk            (0x10UL)       /*!< DIRQ20E (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIER4_DIRQ19E_Pos            (3UL)          /*!< DIRQ19E (Bit 3)                                       */
#define R_SYSTEM_DPSIER4_DIRQ19E_Msk            (0x8UL)        /*!< DIRQ19E (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIER4_DIRQ18E_Pos            (2UL)          /*!< DIRQ18E (Bit 2)                                       */
#define R_SYSTEM_DPSIER4_DIRQ18E_Msk            (0x4UL)        /*!< DIRQ18E (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIER4_DIRQ17E_Pos            (1UL)          /*!< DIRQ17E (Bit 1)                                       */
#define R_SYSTEM_DPSIER4_DIRQ17E_Msk            (0x2UL)        /*!< DIRQ17E (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIER4_DIRQ16E_Pos            (0UL)          /*!< DIRQ16E (Bit 0)                                       */
#define R_SYSTEM_DPSIER4_DIRQ16E_Msk            (0x1UL)        /*!< DIRQ16E (Bitfield-Mask: 0x01)                         */
/* ========================================================  DPSIER5  ======================================================== */
#define R_SYSTEM_DPSIER5_DIRQ31E_Pos            (7UL)          /*!< DIRQ31E (Bit 7)                                       */
#define R_SYSTEM_DPSIER5_DIRQ31E_Msk            (0x80UL)       /*!< DIRQ31E (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIER5_DIRQ30E_Pos            (6UL)          /*!< DIRQ30E (Bit 6)                                       */
#define R_SYSTEM_DPSIER5_DIRQ30E_Msk            (0x40UL)       /*!< DIRQ30E (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIER5_DIRQ29E_Pos            (5UL)          /*!< DIRQ29E (Bit 5)                                       */
#define R_SYSTEM_DPSIER5_DIRQ29E_Msk            (0x20UL)       /*!< DIRQ29E (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIER5_DIRQ28E_Pos            (4UL)          /*!< DIRQ28E (Bit 4)                                       */
#define R_SYSTEM_DPSIER5_DIRQ28E_Msk            (0x10UL)       /*!< DIRQ28E (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIER5_DIRQ27E_Pos            (3UL)          /*!< DIRQ27E (Bit 3)                                       */
#define R_SYSTEM_DPSIER5_DIRQ27E_Msk            (0x8UL)        /*!< DIRQ27E (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIER5_DIRQ26E_Pos            (2UL)          /*!< DIRQ26E (Bit 2)                                       */
#define R_SYSTEM_DPSIER5_DIRQ26E_Msk            (0x4UL)        /*!< DIRQ26E (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIER5_DIRQ25E_Pos            (1UL)          /*!< DIRQ25E (Bit 1)                                       */
#define R_SYSTEM_DPSIER5_DIRQ25E_Msk            (0x2UL)        /*!< DIRQ25E (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIER5_DIRQ24E_Pos            (0UL)          /*!< DIRQ24E (Bit 0)                                       */
#define R_SYSTEM_DPSIER5_DIRQ24E_Msk            (0x1UL)        /*!< DIRQ24E (Bitfield-Mask: 0x01)                         */
/* ========================================================  DPSIFR0  ======================================================== */
#define R_SYSTEM_DPSIFR0_DIRQ7F_Pos             (7UL)          /*!< DIRQ7F (Bit 7)                                        */
#define R_SYSTEM_DPSIFR0_DIRQ7F_Msk             (0x80UL)       /*!< DIRQ7F (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_DPSIFR0_DIRQ6F_Pos             (6UL)          /*!< DIRQ6F (Bit 6)                                        */
#define R_SYSTEM_DPSIFR0_DIRQ6F_Msk             (0x40UL)       /*!< DIRQ6F (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_DPSIFR0_DIRQ5F_Pos             (5UL)          /*!< DIRQ5F (Bit 5)                                        */
#define R_SYSTEM_DPSIFR0_DIRQ5F_Msk             (0x20UL)       /*!< DIRQ5F (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_DPSIFR0_DIRQ4F_Pos             (4UL)          /*!< DIRQ4F (Bit 4)                                        */
#define R_SYSTEM_DPSIFR0_DIRQ4F_Msk             (0x10UL)       /*!< DIRQ4F (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_DPSIFR0_DIRQ3F_Pos             (3UL)          /*!< DIRQ3F (Bit 3)                                        */
#define R_SYSTEM_DPSIFR0_DIRQ3F_Msk             (0x8UL)        /*!< DIRQ3F (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_DPSIFR0_DIRQ2F_Pos             (2UL)          /*!< DIRQ2F (Bit 2)                                        */
#define R_SYSTEM_DPSIFR0_DIRQ2F_Msk             (0x4UL)        /*!< DIRQ2F (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_DPSIFR0_DIRQ1F_Pos             (1UL)          /*!< DIRQ1F (Bit 1)                                        */
#define R_SYSTEM_DPSIFR0_DIRQ1F_Msk             (0x2UL)        /*!< DIRQ1F (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_DPSIFR0_DIRQ0F_Pos             (0UL)          /*!< DIRQ0F (Bit 0)                                        */
#define R_SYSTEM_DPSIFR0_DIRQ0F_Msk             (0x1UL)        /*!< DIRQ0F (Bitfield-Mask: 0x01)                          */
/* ========================================================  DPSIFR1  ======================================================== */
#define R_SYSTEM_DPSIFR1_DIRQ15F_Pos            (7UL)          /*!< DIRQ15F (Bit 7)                                       */
#define R_SYSTEM_DPSIFR1_DIRQ15F_Msk            (0x80UL)       /*!< DIRQ15F (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIFR1_DIRQ14F_Pos            (6UL)          /*!< DIRQ14F (Bit 6)                                       */
#define R_SYSTEM_DPSIFR1_DIRQ14F_Msk            (0x40UL)       /*!< DIRQ14F (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIFR1_DIRQ13F_Pos            (5UL)          /*!< DIRQ13F (Bit 5)                                       */
#define R_SYSTEM_DPSIFR1_DIRQ13F_Msk            (0x20UL)       /*!< DIRQ13F (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIFR1_DIRQ12F_Pos            (4UL)          /*!< DIRQ12F (Bit 4)                                       */
#define R_SYSTEM_DPSIFR1_DIRQ12F_Msk            (0x10UL)       /*!< DIRQ12F (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIFR1_DIRQ11F_Pos            (3UL)          /*!< DIRQ11F (Bit 3)                                       */
#define R_SYSTEM_DPSIFR1_DIRQ11F_Msk            (0x8UL)        /*!< DIRQ11F (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIFR1_DIRQ10F_Pos            (2UL)          /*!< DIRQ10F (Bit 2)                                       */
#define R_SYSTEM_DPSIFR1_DIRQ10F_Msk            (0x4UL)        /*!< DIRQ10F (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIFR1_DIRQ9F_Pos             (1UL)          /*!< DIRQ9F (Bit 1)                                        */
#define R_SYSTEM_DPSIFR1_DIRQ9F_Msk             (0x2UL)        /*!< DIRQ9F (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_DPSIFR1_DIRQ8F_Pos             (0UL)          /*!< DIRQ8F (Bit 0)                                        */
#define R_SYSTEM_DPSIFR1_DIRQ8F_Msk             (0x1UL)        /*!< DIRQ8F (Bitfield-Mask: 0x01)                          */
/* ========================================================  DPSIFR2  ======================================================== */
#define R_SYSTEM_DPSIFR2_DNMIF_Pos              (4UL)          /*!< DNMIF (Bit 4)                                         */
#define R_SYSTEM_DPSIFR2_DNMIF_Msk              (0x10UL)       /*!< DNMIF (Bitfield-Mask: 0x01)                           */
#define R_SYSTEM_DPSIFR2_DRTCAIF_Pos            (3UL)          /*!< DRTCAIF (Bit 3)                                       */
#define R_SYSTEM_DPSIFR2_DRTCAIF_Msk            (0x8UL)        /*!< DRTCAIF (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIFR2_DRTCIIF_Pos            (2UL)          /*!< DRTCIIF (Bit 2)                                       */
#define R_SYSTEM_DPSIFR2_DRTCIIF_Msk            (0x4UL)        /*!< DRTCIIF (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIFR2_DPVD2IF_Pos            (1UL)          /*!< DPVD2IF (Bit 1)                                       */
#define R_SYSTEM_DPSIFR2_DPVD2IF_Msk            (0x2UL)        /*!< DPVD2IF (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIFR2_DPVD1IF_Pos            (0UL)          /*!< DPVD1IF (Bit 0)                                       */
#define R_SYSTEM_DPSIFR2_DPVD1IF_Msk            (0x1UL)        /*!< DPVD1IF (Bitfield-Mask: 0x01)                         */
/* ========================================================  DPSIFR3  ======================================================== */
#define R_SYSTEM_DPSIFR3_DVBATTADIF_Pos         (7UL)          /*!< DVBATTADIF (Bit 7)                                    */
#define R_SYSTEM_DPSIFR3_DVBATTADIF_Msk         (0x80UL)       /*!< DVBATTADIF (Bitfield-Mask: 0x01)                      */
#define R_SYSTEM_DPSIFR3_DSOSTDIF_Pos           (6UL)          /*!< DSOSTDIF (Bit 6)                                      */
#define R_SYSTEM_DPSIFR3_DSOSTDIF_Msk           (0x40UL)       /*!< DSOSTDIF (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_DPSIFR3_DIWDTIF_Pos            (5UL)          /*!< DIWDTIF (Bit 5)                                       */
#define R_SYSTEM_DPSIFR3_DIWDTIF_Msk            (0x20UL)       /*!< DIWDTIF (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIFR3_DUSBHSIF_Pos           (1UL)          /*!< DUSBHSIF (Bit 1)                                      */
#define R_SYSTEM_DPSIFR3_DUSBHSIF_Msk           (0x2UL)        /*!< DUSBHSIF (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_DPSIFR3_DUSBFSIF_Pos           (0UL)          /*!< DUSBFSIF (Bit 0)                                      */
#define R_SYSTEM_DPSIFR3_DUSBFSIF_Msk           (0x1UL)        /*!< DUSBFSIF (Bitfield-Mask: 0x01)                        */
/* ========================================================  DPSIFR4  ======================================================== */
#define R_SYSTEM_DPSIFR4_DIRQ23F_Pos            (7UL)          /*!< DIRQ23F (Bit 7)                                       */
#define R_SYSTEM_DPSIFR4_DIRQ23F_Msk            (0x80UL)       /*!< DIRQ23F (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIFR4_DIRQ22F_Pos            (6UL)          /*!< DIRQ22F (Bit 6)                                       */
#define R_SYSTEM_DPSIFR4_DIRQ22F_Msk            (0x40UL)       /*!< DIRQ22F (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIFR4_DIRQ21F_Pos            (5UL)          /*!< DIRQ21F (Bit 5)                                       */
#define R_SYSTEM_DPSIFR4_DIRQ21F_Msk            (0x20UL)       /*!< DIRQ21F (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIFR4_DIRQ20F_Pos            (4UL)          /*!< DIRQ20F (Bit 4)                                       */
#define R_SYSTEM_DPSIFR4_DIRQ20F_Msk            (0x10UL)       /*!< DIRQ20F (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIFR4_DIRQ19F_Pos            (3UL)          /*!< DIRQ19F (Bit 3)                                       */
#define R_SYSTEM_DPSIFR4_DIRQ19F_Msk            (0x8UL)        /*!< DIRQ19F (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIFR4_DIRQ18F_Pos            (2UL)          /*!< DIRQ18F (Bit 2)                                       */
#define R_SYSTEM_DPSIFR4_DIRQ18F_Msk            (0x4UL)        /*!< DIRQ18F (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIFR4_DIRQ17F_Pos            (1UL)          /*!< DIRQ17F (Bit 1)                                       */
#define R_SYSTEM_DPSIFR4_DIRQ17F_Msk            (0x2UL)        /*!< DIRQ17F (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIFR4_DIRQ16F_Pos            (0UL)          /*!< DIRQ16F (Bit 0)                                       */
#define R_SYSTEM_DPSIFR4_DIRQ16F_Msk            (0x1UL)        /*!< DIRQ16F (Bitfield-Mask: 0x01)                         */
/* ========================================================  DPSIFR5  ======================================================== */
#define R_SYSTEM_DPSIFR5_DIRQ31F_Pos            (7UL)          /*!< DIRQ31F (Bit 7)                                       */
#define R_SYSTEM_DPSIFR5_DIRQ31F_Msk            (0x80UL)       /*!< DIRQ31F (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIFR5_DIRQ30F_Pos            (6UL)          /*!< DIRQ30F (Bit 6)                                       */
#define R_SYSTEM_DPSIFR5_DIRQ30F_Msk            (0x40UL)       /*!< DIRQ30F (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIFR5_DIRQ29F_Pos            (5UL)          /*!< DIRQ29F (Bit 5)                                       */
#define R_SYSTEM_DPSIFR5_DIRQ29F_Msk            (0x20UL)       /*!< DIRQ29F (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIFR5_DIRQ28F_Pos            (4UL)          /*!< DIRQ28F (Bit 4)                                       */
#define R_SYSTEM_DPSIFR5_DIRQ28F_Msk            (0x10UL)       /*!< DIRQ28F (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIFR5_DIRQ27F_Pos            (3UL)          /*!< DIRQ27F (Bit 3)                                       */
#define R_SYSTEM_DPSIFR5_DIRQ27F_Msk            (0x8UL)        /*!< DIRQ27F (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIFR5_DIRQ26F_Pos            (2UL)          /*!< DIRQ26F (Bit 2)                                       */
#define R_SYSTEM_DPSIFR5_DIRQ26F_Msk            (0x4UL)        /*!< DIRQ26F (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIFR5_DIRQ25F_Pos            (1UL)          /*!< DIRQ25F (Bit 1)                                       */
#define R_SYSTEM_DPSIFR5_DIRQ25F_Msk            (0x2UL)        /*!< DIRQ25F (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIFR5_DIRQ24F_Pos            (0UL)          /*!< DIRQ24F (Bit 0)                                       */
#define R_SYSTEM_DPSIFR5_DIRQ24F_Msk            (0x1UL)        /*!< DIRQ24F (Bitfield-Mask: 0x01)                         */
/* =======================================================  DPSIEGR0  ======================================================== */
#define R_SYSTEM_DPSIEGR0_DIRQ7EG_Pos           (7UL)          /*!< DIRQ7EG (Bit 7)                                       */
#define R_SYSTEM_DPSIEGR0_DIRQ7EG_Msk           (0x80UL)       /*!< DIRQ7EG (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIEGR0_DIRQ6EG_Pos           (6UL)          /*!< DIRQ6EG (Bit 6)                                       */
#define R_SYSTEM_DPSIEGR0_DIRQ6EG_Msk           (0x40UL)       /*!< DIRQ6EG (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIEGR0_DIRQ5EG_Pos           (5UL)          /*!< DIRQ5EG (Bit 5)                                       */
#define R_SYSTEM_DPSIEGR0_DIRQ5EG_Msk           (0x20UL)       /*!< DIRQ5EG (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIEGR0_DIRQ4EG_Pos           (4UL)          /*!< DIRQ4EG (Bit 4)                                       */
#define R_SYSTEM_DPSIEGR0_DIRQ4EG_Msk           (0x10UL)       /*!< DIRQ4EG (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIEGR0_DIRQ3EG_Pos           (3UL)          /*!< DIRQ3EG (Bit 3)                                       */
#define R_SYSTEM_DPSIEGR0_DIRQ3EG_Msk           (0x8UL)        /*!< DIRQ3EG (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIEGR0_DIRQ2EG_Pos           (2UL)          /*!< DIRQ2EG (Bit 2)                                       */
#define R_SYSTEM_DPSIEGR0_DIRQ2EG_Msk           (0x4UL)        /*!< DIRQ2EG (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIEGR0_DIRQ1EG_Pos           (1UL)          /*!< DIRQ1EG (Bit 1)                                       */
#define R_SYSTEM_DPSIEGR0_DIRQ1EG_Msk           (0x2UL)        /*!< DIRQ1EG (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIEGR0_DIRQ0EG_Pos           (0UL)          /*!< DIRQ0EG (Bit 0)                                       */
#define R_SYSTEM_DPSIEGR0_DIRQ0EG_Msk           (0x1UL)        /*!< DIRQ0EG (Bitfield-Mask: 0x01)                         */
/* =======================================================  DPSIEGR1  ======================================================== */
#define R_SYSTEM_DPSIEGR1_DIRQ15EG_Pos          (7UL)          /*!< DIRQ15EG (Bit 7)                                      */
#define R_SYSTEM_DPSIEGR1_DIRQ15EG_Msk          (0x80UL)       /*!< DIRQ15EG (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_DPSIEGR1_DIRQ14EG_Pos          (6UL)          /*!< DIRQ14EG (Bit 6)                                      */
#define R_SYSTEM_DPSIEGR1_DIRQ14EG_Msk          (0x40UL)       /*!< DIRQ14EG (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_DPSIEGR1_DIRQ13EG_Pos          (5UL)          /*!< DIRQ13EG (Bit 5)                                      */
#define R_SYSTEM_DPSIEGR1_DIRQ13EG_Msk          (0x20UL)       /*!< DIRQ13EG (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_DPSIEGR1_DIRQ12EG_Pos          (4UL)          /*!< DIRQ12EG (Bit 4)                                      */
#define R_SYSTEM_DPSIEGR1_DIRQ12EG_Msk          (0x10UL)       /*!< DIRQ12EG (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_DPSIEGR1_DIRQ11EG_Pos          (3UL)          /*!< DIRQ11EG (Bit 3)                                      */
#define R_SYSTEM_DPSIEGR1_DIRQ11EG_Msk          (0x8UL)        /*!< DIRQ11EG (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_DPSIEGR1_DIRQ10EG_Pos          (2UL)          /*!< DIRQ10EG (Bit 2)                                      */
#define R_SYSTEM_DPSIEGR1_DIRQ10EG_Msk          (0x4UL)        /*!< DIRQ10EG (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_DPSIEGR1_DIRQ9EG_Pos           (1UL)          /*!< DIRQ9EG (Bit 1)                                       */
#define R_SYSTEM_DPSIEGR1_DIRQ9EG_Msk           (0x2UL)        /*!< DIRQ9EG (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIEGR1_DIRQ8EG_Pos           (0UL)          /*!< DIRQ8EG (Bit 0)                                       */
#define R_SYSTEM_DPSIEGR1_DIRQ8EG_Msk           (0x1UL)        /*!< DIRQ8EG (Bitfield-Mask: 0x01)                         */
/* =======================================================  DPSIEGR2  ======================================================== */
#define R_SYSTEM_DPSIEGR2_DNMIEG_Pos            (4UL)          /*!< DNMIEG (Bit 4)                                        */
#define R_SYSTEM_DPSIEGR2_DNMIEG_Msk            (0x10UL)       /*!< DNMIEG (Bitfield-Mask: 0x01)                          */
#define R_SYSTEM_DPSIEGR2_DPVD2EG_Pos           (1UL)          /*!< DPVD2EG (Bit 1)                                       */
#define R_SYSTEM_DPSIEGR2_DPVD2EG_Msk           (0x2UL)        /*!< DPVD2EG (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_DPSIEGR2_DPVD1EG_Pos           (0UL)          /*!< DPVD1EG (Bit 0)                                       */
#define R_SYSTEM_DPSIEGR2_DPVD1EG_Msk           (0x1UL)        /*!< DPVD1EG (Bitfield-Mask: 0x01)                         */
/* =======================================================  DPSIEGR3  ======================================================== */
#define R_SYSTEM_DPSIEGR3_DIRQ23EG_Pos          (7UL)          /*!< DIRQ23EG (Bit 7)                                      */
#define R_SYSTEM_DPSIEGR3_DIRQ23EG_Msk          (0x80UL)       /*!< DIRQ23EG (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_DPSIEGR3_DIRQ22EG_Pos          (6UL)          /*!< DIRQ22EG (Bit 6)                                      */
#define R_SYSTEM_DPSIEGR3_DIRQ22EG_Msk          (0x40UL)       /*!< DIRQ22EG (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_DPSIEGR3_DIRQ21EG_Pos          (5UL)          /*!< DIRQ21EG (Bit 5)                                      */
#define R_SYSTEM_DPSIEGR3_DIRQ21EG_Msk          (0x20UL)       /*!< DIRQ21EG (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_DPSIEGR3_DIRQ20EG_Pos          (4UL)          /*!< DIRQ20EG (Bit 4)                                      */
#define R_SYSTEM_DPSIEGR3_DIRQ20EG_Msk          (0x10UL)       /*!< DIRQ20EG (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_DPSIEGR3_DIRQ19EG_Pos          (3UL)          /*!< DIRQ19EG (Bit 3)                                      */
#define R_SYSTEM_DPSIEGR3_DIRQ19EG_Msk          (0x8UL)        /*!< DIRQ19EG (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_DPSIEGR3_DIRQ18EG_Pos          (2UL)          /*!< DIRQ18EG (Bit 2)                                      */
#define R_SYSTEM_DPSIEGR3_DIRQ18EG_Msk          (0x4UL)        /*!< DIRQ18EG (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_DPSIEGR3_DIRQ17EG_Pos          (1UL)          /*!< DIRQ17EG (Bit 1)                                      */
#define R_SYSTEM_DPSIEGR3_DIRQ17EG_Msk          (0x2UL)        /*!< DIRQ17EG (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_DPSIEGR3_DIRQ16EG_Pos          (0UL)          /*!< DIRQ16EG (Bit 0)                                      */
#define R_SYSTEM_DPSIEGR3_DIRQ16EG_Msk          (0x1UL)        /*!< DIRQ16EG (Bitfield-Mask: 0x01)                        */
/* =======================================================  DPSIEGR4  ======================================================== */
#define R_SYSTEM_DPSIEGR4_DIRQ31EG_Pos          (7UL)          /*!< DIRQ31EG (Bit 7)                                      */
#define R_SYSTEM_DPSIEGR4_DIRQ31EG_Msk          (0x80UL)       /*!< DIRQ31EG (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_DPSIEGR4_DIRQ30EG_Pos          (6UL)          /*!< DIRQ30EG (Bit 6)                                      */
#define R_SYSTEM_DPSIEGR4_DIRQ30EG_Msk          (0x40UL)       /*!< DIRQ30EG (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_DPSIEGR4_DIRQ29EG_Pos          (5UL)          /*!< DIRQ29EG (Bit 5)                                      */
#define R_SYSTEM_DPSIEGR4_DIRQ29EG_Msk          (0x20UL)       /*!< DIRQ29EG (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_DPSIEGR4_DIRQ28EG_Pos          (4UL)          /*!< DIRQ28EG (Bit 4)                                      */
#define R_SYSTEM_DPSIEGR4_DIRQ28EG_Msk          (0x10UL)       /*!< DIRQ28EG (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_DPSIEGR4_DIRQ27EG_Pos          (3UL)          /*!< DIRQ27EG (Bit 3)                                      */
#define R_SYSTEM_DPSIEGR4_DIRQ27EG_Msk          (0x8UL)        /*!< DIRQ27EG (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_DPSIEGR4_DIRQ26EG_Pos          (2UL)          /*!< DIRQ26EG (Bit 2)                                      */
#define R_SYSTEM_DPSIEGR4_DIRQ26EG_Msk          (0x4UL)        /*!< DIRQ26EG (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_DPSIEGR4_DIRQ25EG_Pos          (1UL)          /*!< DIRQ25EG (Bit 1)                                      */
#define R_SYSTEM_DPSIEGR4_DIRQ25EG_Msk          (0x2UL)        /*!< DIRQ25EG (Bitfield-Mask: 0x01)                        */
#define R_SYSTEM_DPSIEGR4_DIRQ24EG_Pos          (0UL)          /*!< DIRQ24EG (Bit 0)                                      */
#define R_SYSTEM_DPSIEGR4_DIRQ24EG_Msk          (0x1UL)        /*!< DIRQ24EG (Bitfield-Mask: 0x01)                        */
/* =======================================================  PLL1LDOCR  ======================================================= */
#define R_SYSTEM_PLL1LDOCR_SKEEP_Pos            (1UL)          /*!< SKEEP (Bit 1)                                         */
#define R_SYSTEM_PLL1LDOCR_SKEEP_Msk            (0x2UL)        /*!< SKEEP (Bitfield-Mask: 0x01)                           */
#define R_SYSTEM_PLL1LDOCR_LDOSTP_Pos           (0UL)          /*!< LDOSTP (Bit 0)                                        */
#define R_SYSTEM_PLL1LDOCR_LDOSTP_Msk           (0x1UL)        /*!< LDOSTP (Bitfield-Mask: 0x01)                          */
/* =======================================================  PLL2LDOCR  ======================================================= */
#define R_SYSTEM_PLL2LDOCR_SKEEP_Pos            (1UL)          /*!< SKEEP (Bit 1)                                         */
#define R_SYSTEM_PLL2LDOCR_SKEEP_Msk            (0x2UL)        /*!< SKEEP (Bitfield-Mask: 0x01)                           */
#define R_SYSTEM_PLL2LDOCR_LDOSTP_Pos           (0UL)          /*!< LDOSTP (Bit 0)                                        */
#define R_SYSTEM_PLL2LDOCR_LDOSTP_Msk           (0x1UL)        /*!< LDOSTP (Bitfield-Mask: 0x01)                          */
/* =======================================================  HOCOLDOCR  ======================================================= */
#define R_SYSTEM_HOCOLDOCR_SKEEP_Pos            (1UL)          /*!< SKEEP (Bit 1)                                         */
#define R_SYSTEM_HOCOLDOCR_SKEEP_Msk            (0x2UL)        /*!< SKEEP (Bitfield-Mask: 0x01)                           */
#define R_SYSTEM_HOCOLDOCR_LDOSTP_Pos           (0UL)          /*!< LDOSTP (Bit 0)                                        */
#define R_SYSTEM_HOCOLDOCR_LDOSTP_Msk           (0x1UL)        /*!< LDOSTP (Bitfield-Mask: 0x01)                          */
/* =========================================================  LVOCR  ========================================================= */
#define R_SYSTEM_LVOCR_LVO0E_Pos                (0UL)          /*!< LVO0E (Bit 0)                                         */
#define R_SYSTEM_LVOCR_LVO0E_Msk                (0x1UL)        /*!< LVO0E (Bitfield-Mask: 0x01)                           */
/* =========================================================  VSCR  ========================================================== */
#define R_SYSTEM_VSCR_VSCMTSF_Pos               (4UL)          /*!< VSCMTSF (Bit 4)                                       */
#define R_SYSTEM_VSCR_VSCMTSF_Msk               (0x10UL)       /*!< VSCMTSF (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_VSCR_VSCM_Pos                  (0UL)          /*!< VSCM (Bit 0)                                          */
#define R_SYSTEM_VSCR_VSCM_Msk                  (0x7UL)        /*!< VSCM (Bitfield-Mask: 0x07)                            */
/* =========================================================  SVSCR  ========================================================= */
#define R_SYSTEM_SVSCR_SVSCM_Pos                (0UL)          /*!< SVSCM (Bit 0)                                         */
#define R_SYSTEM_SVSCR_SVSCM_Msk                (0x7UL)        /*!< SVSCM (Bitfield-Mask: 0x07)                           */
/* ========================================================  CPUDSCR  ======================================================== */
#define R_SYSTEM_CPUDSCR_PGD0_Pos               (0UL)          /*!< PGD0 (Bit 0)                                          */
#define R_SYSTEM_CPUDSCR_PGD0_Msk               (0x1UL)        /*!< PGD0 (Bitfield-Mask: 0x01)                            */
/* =========================================================  MWMCR  ========================================================= */
#define R_SYSTEM_MWMCR_MWM_Pos                  (0UL)          /*!< MWM (Bit 0)                                           */
#define R_SYSTEM_MWMCR_MWM_Msk                  (0x3UL)        /*!< MWM (Bitfield-Mask: 0x03)                             */
/* ========================================================  LDOECR  ========================================================= */
#define R_SYSTEM_LDOECR_EFUPTSF_Pos             (7UL)          /*!< EFUPTSF (Bit 7)                                       */
#define R_SYSTEM_LDOECR_EFUPTSF_Msk             (0x80UL)       /*!< EFUPTSF (Bitfield-Mask: 0x01)                         */
#define R_SYSTEM_LDOECR_EFUPE_Pos               (0UL)          /*!< EFUPE (Bit 0)                                         */
#define R_SYSTEM_LDOECR_EFUPE_Msk               (0x1UL)        /*!< EFUPE (Bitfield-Mask: 0x01)                           */
/* =========================================================  DCCR  ========================================================== */
#define R_SYSTEM_DCCR_WCODE_Pos                 (1UL)          /*!< WCODE (Bit 1)                                         */
#define R_SYSTEM_DCCR_WCODE_Msk                 (0xfeUL)       /*!< WCODE (Bitfield-Mask: 0x7f)                           */
#define R_SYSTEM_DCCR_DSTBYCON_Pos              (0UL)          /*!< DSTBYCON (Bit 0)                                      */
#define R_SYSTEM_DCCR_DSTBYCON_Msk              (0x1UL)        /*!< DSTBYCON (Bitfield-Mask: 0x01)                        */
/* ========================================================  PRCR  ========================================================= */
#define R_SYSTEM_PRCR_PRKEY_Pos                 (8UL)          /*!< PRKEY (Bit 8)                                         */
#define R_SYSTEM_PRCR_PRKEY_Msk                 (0xff00UL)     /*!< PRKEY (Bitfield-Mask: 0xff)                           */
#define R_SYSTEM_PRCR_PRC5_Pos                  (5UL)          /*!< PRC5 (Bit 5)                                          */
#define R_SYSTEM_PRCR_PRC5_Msk                  (0x20UL)       /*!< PRC5 (Bitfield-Mask: 0x01)                            */
#define R_SYSTEM_PRCR_PRC4_Pos                  (4UL)          /*!< PRC4 (Bit 4)                                          */
#define R_SYSTEM_PRCR_PRC4_Msk                  (0x10UL)       /*!< PRC4 (Bitfield-Mask: 0x01)                            */
#define R_SYSTEM_PRCR_PRC3_Pos                  (3UL)          /*!< PRC3 (Bit 3)                                          */
#define R_SYSTEM_PRCR_PRC3_Msk                  (0x8UL)        /*!< PRC3 (Bitfield-Mask: 0x01)                            */
#define R_SYSTEM_PRCR_PRC1_Pos                  (1UL)          /*!< PRC1 (Bit 1)                                          */
#define R_SYSTEM_PRCR_PRC1_Msk                  (0x2UL)        /*!< PRC1 (Bitfield-Mask: 0x01)                            */
#define R_SYSTEM_PRCR_PRC0_Pos                  (0UL)          /*!< PRC0 (Bit 0)                                          */
#define R_SYSTEM_PRCR_PRC0_Msk                  (0x1UL)        /*!< PRC0 (Bitfield-Mask: 0x01)                            */

/* =========================================================================================================================== */
/* ================                                         R_MSTP                                         ================ */
/* =========================================================================================================================== */

/* ========================================================  MSTPCRA  ======================================================== */
#define R_MSTP_MSTPCRA_MSTPA29_Pos    (29UL)         /*!< MSTPA29 (Bit 29)                                      */
#define R_MSTP_MSTPCRA_MSTPA29_Msk    (0x20000000UL) /*!< MSTPA29 (Bitfield-Mask: 0x01)                         */
#define R_MSTP_MSTPCRA_MSTPA22_Pos    (22UL)         /*!< MSTPA22 (Bit 22)                                      */
#define R_MSTP_MSTPCRA_MSTPA22_Msk    (0x400000UL)   /*!< MSTPA22 (Bitfield-Mask: 0x01)                         */
#define R_MSTP_MSTPCRA_MSTPA1_Pos     (1UL)          /*!< MSTPA1 (Bit 1)                                        */
#define R_MSTP_MSTPCRA_MSTPA1_Msk     (0x2UL)        /*!< MSTPA1 (Bitfield-Mask: 0x01)                          */
#define R_MSTP_MSTPCRA_MSTPA0_Pos     (0UL)          /*!< MSTPA0 (Bit 0)                                        */
#define R_MSTP_MSTPCRA_MSTPA0_Msk     (0x1UL)        /*!< MSTPA0 (Bitfield-Mask: 0x01)                          */
/* ========================================================  MSTPCRB  ======================================================== */
#define R_MSTP_MSTPCRB_MSTPB31_Pos    (31UL)         /*!< MSTPB31 (Bit 31)                                      */
#define R_MSTP_MSTPCRB_MSTPB31_Msk    (0x80000000UL) /*!< MSTPB31 (Bitfield-Mask: 0x01)                         */
#define R_MSTP_MSTPCRB_MSTPB30_Pos    (30UL)         /*!< MSTPB30 (Bit 30)                                      */
#define R_MSTP_MSTPCRB_MSTPB30_Msk    (0x40000000UL) /*!< MSTPB30 (Bitfield-Mask: 0x01)                         */
#define R_MSTP_MSTPCRB_MSTPB29_Pos    (29UL)         /*!< MSTPB29 (Bit 29)                                      */
#define R_MSTP_MSTPCRB_MSTPB29_Msk    (0x20000000UL) /*!< MSTPB29 (Bitfield-Mask: 0x01)                         */
#define R_MSTP_MSTPCRB_MSTPB28_Pos    (28UL)         /*!< MSTPB28 (Bit 28)                                      */
#define R_MSTP_MSTPCRB_MSTPB28_Msk    (0x10000000UL) /*!< MSTPB28 (Bitfield-Mask: 0x01)                         */
#define R_MSTP_MSTPCRB_MSTPB27_Pos    (27UL)         /*!< MSTPB27 (Bit 27)                                      */
#define R_MSTP_MSTPCRB_MSTPB27_Msk    (0x8000000UL)  /*!< MSTPB27 (Bitfield-Mask: 0x01)                         */
#define R_MSTP_MSTPCRB_MSTPB26_Pos    (26UL)         /*!< MSTPB26 (Bit 26)                                      */
#define R_MSTP_MSTPCRB_MSTPB26_Msk    (0x4000000UL)  /*!< MSTPB26 (Bitfield-Mask: 0x01)                         */
#define R_MSTP_MSTPCRB_MSTPB25_Pos    (25UL)         /*!< MSTPB25 (Bit 25)                                      */
#define R_MSTP_MSTPCRB_MSTPB25_Msk    (0x2000000UL)  /*!< MSTPB25 (Bitfield-Mask: 0x01)                         */
#define R_MSTP_MSTPCRB_MSTPB24_Pos    (24UL)         /*!< MSTPB24 (Bit 24)                                      */
#define R_MSTP_MSTPCRB_MSTPB24_Msk    (0x1000000UL)  /*!< MSTPB24 (Bitfield-Mask: 0x01)                         */
#define R_MSTP_MSTPCRB_MSTPB23_Pos    (23UL)         /*!< MSTPB23 (Bit 23)                                      */
#define R_MSTP_MSTPCRB_MSTPB23_Msk    (0x800000UL)   /*!< MSTPB23 (Bitfield-Mask: 0x01)                         */
#define R_MSTP_MSTPCRB_MSTPB22_Pos    (22UL)         /*!< MSTPB22 (Bit 22)                                      */
#define R_MSTP_MSTPCRB_MSTPB22_Msk    (0x400000UL)   /*!< MSTPB22 (Bitfield-Mask: 0x01)                         */
#define R_MSTP_MSTPCRB_MSTPB21_Pos    (21UL)         /*!< MSTPB21 (Bit 21)                                      */
#define R_MSTP_MSTPCRB_MSTPB21_Msk    (0x200000UL)   /*!< MSTPB21 (Bitfield-Mask: 0x01)                         */
#define R_MSTP_MSTPCRB_MSTPB20_Pos    (20UL)         /*!< MSTPB20 (Bit 20)                                      */
#define R_MSTP_MSTPCRB_MSTPB20_Msk    (0x100000UL)   /*!< MSTPB20 (Bitfield-Mask: 0x01)                         */
#define R_MSTP_MSTPCRB_MSTPB19_Pos    (19UL)         /*!< MSTPB19 (Bit 19)                                      */
#define R_MSTP_MSTPCRB_MSTPB19_Msk    (0x80000UL)    /*!< MSTPB19 (Bitfield-Mask: 0x01)                         */
#define R_MSTP_MSTPCRB_MSTPB18_Pos    (18UL)         /*!< MSTPB18 (Bit 18)                                      */
#define R_MSTP_MSTPCRB_MSTPB18_Msk    (0x40000UL)    /*!< MSTPB18 (Bitfield-Mask: 0x01)                         */
#define R_MSTP_MSTPCRB_MSTPB17_Pos    (17UL)         /*!< MSTPB17 (Bit 17)                                      */
#define R_MSTP_MSTPCRB_MSTPB17_Msk    (0x20000UL)    /*!< MSTPB17 (Bitfield-Mask: 0x01)                         */
#define R_MSTP_MSTPCRB_MSTPB16_Pos    (16UL)         /*!< MSTPB16 (Bit 16)                                      */
#define R_MSTP_MSTPCRB_MSTPB16_Msk    (0x10000UL)    /*!< MSTPB16 (Bitfield-Mask: 0x01)                         */
#define R_MSTP_MSTPCRB_MSTPB12_Pos    (12UL)         /*!< MSTPB12 (Bit 12)                                      */
#define R_MSTP_MSTPCRB_MSTPB12_Msk    (0x1000UL)     /*!< MSTPB12 (Bitfield-Mask: 0x01)                         */
#define R_MSTP_MSTPCRB_MSTPB11_Pos    (11UL)         /*!< MSTPB11 (Bit 11)                                      */
#define R_MSTP_MSTPCRB_MSTPB11_Msk    (0x800UL)      /*!< MSTPB11 (Bitfield-Mask: 0x01)                         */
#define R_MSTP_MSTPCRB_MSTPB8_Pos     (8UL)          /*!< MSTPB8 (Bit 8)                                        */
#define R_MSTP_MSTPCRB_MSTPB8_Msk     (0x100UL)      /*!< MSTPB8 (Bitfield-Mask: 0x01)                          */
#define R_MSTP_MSTPCRB_MSTPB7_Pos     (7UL)          /*!< MSTPB7 (Bit 7)                                        */
#define R_MSTP_MSTPCRB_MSTPB7_Msk     (0x80UL)       /*!< MSTPB7 (Bitfield-Mask: 0x01)                          */
#define R_MSTP_MSTPCRB_MSTPB4_Pos     (4UL)          /*!< MSTPB4 (Bit 4)                                        */
#define R_MSTP_MSTPCRB_MSTPB4_Msk     (0x10UL)       /*!< MSTPB4 (Bitfield-Mask: 0x01)                          */
#define R_MSTP_MSTPCRB_MSTPB0_Pos     (0UL)          /*!< MSTPB0 (Bit 0)                                        */
#define R_MSTP_MSTPCRB_MSTPB0_Msk     (0x1UL)        /*!< MSTPB0 (Bitfield-Mask: 0x01)                          */
/* ========================================================  MSTPCRC  ======================================================== */
#define R_MSTP_MSTPCRC_MSTPC31_Pos    (31UL)         /*!< MSTPC31 (Bit 31)                                      */
#define R_MSTP_MSTPCRC_MSTPC31_Msk    (0x80000000UL) /*!< MSTPC31 (Bitfield-Mask: 0x01)                         */
#define R_MSTP_MSTPCRC_MSTPC30_Pos    (30UL)         /*!< MSTPC30 (Bit 30)                                      */
#define R_MSTP_MSTPCRC_MSTPC30_Msk    (0x40000000UL) /*!< MSTPC30 (Bitfield-Mask: 0x01)                         */
#define R_MSTP_MSTPCRC_MSTPC29_Pos    (29UL)         /*!< MSTPC29 (Bit 29)                                      */
#define R_MSTP_MSTPCRC_MSTPC29_Msk    (0x20000000UL) /*!< MSTPC29 (Bitfield-Mask: 0x01)                         */
#define R_MSTP_MSTPCRC_MSTPC28_Pos    (28UL)         /*!< MSTPC28 (Bit 28)                                      */
#define R_MSTP_MSTPCRC_MSTPC28_Msk    (0x10000000UL) /*!< MSTPC28 (Bitfield-Mask: 0x01)                         */
#define R_MSTP_MSTPCRC_MSTPC27_Pos    (27UL)         /*!< MSTPC27 (Bit 27)                                      */
#define R_MSTP_MSTPCRC_MSTPC27_Msk    (0x8000000UL)  /*!< MSTPC27 (Bitfield-Mask: 0x01)                         */
#define R_MSTP_MSTPCRC_MSTPC25_Pos    (25UL)         /*!< MSTPC25 (Bit 25)                                      */
#define R_MSTP_MSTPCRC_MSTPC25_Msk    (0x2000000UL)  /*!< MSTPC25 (Bitfield-Mask: 0x01)                         */
#define R_MSTP_MSTPCRC_MSTPC24_Pos    (24UL)         /*!< MSTPC24 (Bit 24)                                      */
#define R_MSTP_MSTPCRC_MSTPC24_Msk    (0x1000000UL)  /*!< MSTPC24 (Bitfield-Mask: 0x01)                         */
#define R_MSTP_MSTPCRC_MSTPC23_Pos    (23UL)         /*!< MSTPC23 (Bit 23)                                      */
#define R_MSTP_MSTPCRC_MSTPC23_Msk    (0x800000UL)   /*!< MSTPC23 (Bitfield-Mask: 0x01)                         */
#define R_MSTP_MSTPCRC_MSTPC14_Pos    (14UL)         /*!< MSTPC14 (Bit 14)                                      */
#define R_MSTP_MSTPCRC_MSTPC14_Msk    (0x4000UL)     /*!< MSTPC14 (Bitfield-Mask: 0x01)                         */
#define R_MSTP_MSTPCRC_MSTPC13_Pos    (13UL)         /*!< MSTPC13 (Bit 13)                                      */
#define R_MSTP_MSTPCRC_MSTPC13_Msk    (0x2000UL)     /*!< MSTPC13 (Bitfield-Mask: 0x01)                         */
#define R_MSTP_MSTPCRC_MSTPC12_Pos    (12UL)         /*!< MSTPC12 (Bit 12)                                      */
#define R_MSTP_MSTPCRC_MSTPC12_Msk    (0x1000UL)     /*!< MSTPC12 (Bitfield-Mask: 0x01)                         */
#define R_MSTP_MSTPCRC_MSTPC8_Pos     (8UL)          /*!< MSTPC8 (Bit 8)                                        */
#define R_MSTP_MSTPCRC_MSTPC8_Msk     (0x100UL)      /*!< MSTPC8 (Bitfield-Mask: 0x01)                          */
#define R_MSTP_MSTPCRC_MSTPC7_Pos     (7UL)          /*!< MSTPC7 (Bit 7)                                        */
#define R_MSTP_MSTPCRC_MSTPC7_Msk     (0x80UL)       /*!< MSTPC7 (Bitfield-Mask: 0x01)                          */
#define R_MSTP_MSTPCRC_MSTPC2_Pos     (2UL)          /*!< MSTPC2 (Bit 2)                                        */
#define R_MSTP_MSTPCRC_MSTPC2_Msk     (0x4UL)        /*!< MSTPC2 (Bitfield-Mask: 0x01)                          */
#define R_MSTP_MSTPCRC_MSTPC1_Pos     (1UL)          /*!< MSTPC1 (Bit 1)                                        */
#define R_MSTP_MSTPCRC_MSTPC1_Msk     (0x2UL)        /*!< MSTPC1 (Bitfield-Mask: 0x01)                          */
#define R_MSTP_MSTPCRC_MSTPC0_Pos     (0UL)          /*!< MSTPC0 (Bit 0)                                        */
#define R_MSTP_MSTPCRC_MSTPC0_Msk     (0x1UL)        /*!< MSTPC0 (Bitfield-Mask: 0x01)                          */
/* ========================================================  MSTPCRD  ======================================================== */
#define R_MSTP_MSTPCRD_MSTPD22_Pos    (22UL)         /*!< MSTPD22 (Bit 22)                                      */
#define R_MSTP_MSTPCRD_MSTPD22_Msk    (0x400000UL)   /*!< MSTPD22 (Bitfield-Mask: 0x01)                         */
#define R_MSTP_MSTPCRD_MSTPD21_Pos    (21UL)         /*!< MSTPD21 (Bit 21)                                      */
#define R_MSTP_MSTPCRD_MSTPD21_Msk    (0x200000UL)   /*!< MSTPD21 (Bitfield-Mask: 0x01)                         */
#define R_MSTP_MSTPCRD_MSTPD20_Pos    (20UL)         /*!< MSTPD20 (Bit 20)                                      */
#define R_MSTP_MSTPCRD_MSTPD20_Msk    (0x100000UL)   /*!< MSTPD20 (Bitfield-Mask: 0x01)                         */
#define R_MSTP_MSTPCRD_MSTPD19_Pos    (19UL)         /*!< MSTPD19 (Bit 19)                                      */
#define R_MSTP_MSTPCRD_MSTPD19_Msk    (0x80000UL)    /*!< MSTPD19 (Bitfield-Mask: 0x01)                         */
#define R_MSTP_MSTPCRD_MSTPD14_Pos    (14UL)         /*!< MSTPD14 (Bit 14)                                      */
#define R_MSTP_MSTPCRD_MSTPD14_Msk    (0x4000UL)     /*!< MSTPD14 (Bitfield-Mask: 0x01)                         */
#define R_MSTP_MSTPCRD_MSTPD13_Pos    (13UL)         /*!< MSTPD13 (Bit 13)                                      */
#define R_MSTP_MSTPCRD_MSTPD13_Msk    (0x2000UL)     /*!< MSTPD13 (Bitfield-Mask: 0x01)                         */
#define R_MSTP_MSTPCRD_MSTPD12_Pos    (12UL)         /*!< MSTPD12 (Bit 12)                                      */
#define R_MSTP_MSTPCRD_MSTPD12_Msk    (0x1000UL)     /*!< MSTPD12 (Bitfield-Mask: 0x01)                         */
#define R_MSTP_MSTPCRD_MSTPD11_Pos    (11UL)         /*!< MSTPD11 (Bit 11)                                      */
#define R_MSTP_MSTPCRD_MSTPD11_Msk    (0x800UL)      /*!< MSTPD11 (Bitfield-Mask: 0x01)                         */
#define R_MSTP_MSTPCRD_MSTPD9_Pos     (9UL)          /*!< MSTPD9 (Bit 9)                                        */
#define R_MSTP_MSTPCRD_MSTPD9_Msk     (0x200UL)      /*!< MSTPD9 (Bitfield-Mask: 0x01)                          */
#define R_MSTP_MSTPCRD_MSTPD8_Pos     (8UL)          /*!< MSTPD8 (Bit 8)                                        */
#define R_MSTP_MSTPCRD_MSTPD8_Msk     (0x100UL)      /*!< MSTPD8 (Bitfield-Mask: 0x01)                          */
#define R_MSTP_MSTPCRD_MSTPD6_Pos     (6UL)          /*!< MSTPD6 (Bit 6)                                        */
#define R_MSTP_MSTPCRD_MSTPD6_Msk     (0x40UL)       /*!< MSTPD6 (Bitfield-Mask: 0x01)                          */
/* ========================================================  MSTPCRE  ======================================================== */
#define R_MSTP_MSTPCRE_MSTPE31_Pos    (31UL)         /*!< MSTPE31 (Bit 31)                                      */
#define R_MSTP_MSTPCRE_MSTPE31_Msk    (0x80000000UL) /*!< MSTPE31 (Bitfield-Mask: 0x01)                         */
/* ========================================================  MSTPCRF  ======================================================== */
#define R_MSTP_MSTPCRF_MSTPF31_Pos    (31UL)         /*!< MSTPF31 (Bit 31)                                      */
#define R_MSTP_MSTPCRF_MSTPF31_Msk    (0x80000000UL) /*!< MSTPF31 (Bitfield-Mask: 0x01)                         */
#define R_MSTP_MSTPCRF_MSTPF30_Pos    (30UL)         /*!< MSTPF30 (Bit 30)                                      */
#define R_MSTP_MSTPCRF_MSTPF30_Msk    (0x40000000UL) /*!< MSTPF30 (Bitfield-Mask: 0x01)                         */
#define R_MSTP_MSTPCRF_MSTPF16_Pos    (16UL)         /*!< MSTPF16 (Bit 16)                                      */
#define R_MSTP_MSTPCRF_MSTPF16_Msk    (0x10000UL)    /*!< MSTPF16 (Bitfield-Mask: 0x01)                         */
#define R_MSTP_MSTPCRF_MSTPF15_Pos    (15UL)         /*!< MSTPF15 (Bit 15)                                      */
#define R_MSTP_MSTPCRF_MSTPF15_Msk    (0x8000UL)     /*!< MSTPF15 (Bitfield-Mask: 0x01)                         */
#define R_MSTP_MSTPCRF_MSTPF14_Pos    (14UL)         /*!< MSTPF14 (Bit 14)                                      */
#define R_MSTP_MSTPCRF_MSTPF14_Msk    (0x4000UL)     /*!< MSTPF14 (Bitfield-Mask: 0x01)                         */
#define R_MSTP_MSTPCRF_MSTPF13_Pos    (13UL)         /*!< MSTPF13 (Bit 13)                                      */
#define R_MSTP_MSTPCRF_MSTPF13_Msk    (0x2000UL)     /*!< MSTPF13 (Bitfield-Mask: 0x01)                         */
#define R_MSTP_MSTPCRF_MSTPF12_Pos    (12UL)         /*!< MSTPF12 (Bit 12)                                      */
#define R_MSTP_MSTPCRF_MSTPF12_Msk    (0x1000UL)     /*!< MSTPF12 (Bitfield-Mask: 0x01)                         */
#define R_MSTP_MSTPCRF_MSTPF8_Pos     (8UL)          /*!< MSTPF8 (Bit 8)                                        */
#define R_MSTP_MSTPCRF_MSTPF8_Msk     (0x100UL)      /*!< MSTPF8 (Bitfield-Mask: 0x01)                          */
#define R_MSTP_MSTPCRF_MSTPF7_Pos     (7UL)          /*!< MSTPF7 (Bit 7)                                        */
#define R_MSTP_MSTPCRF_MSTPF7_Msk     (0x80UL)       /*!< MSTPF7 (Bitfield-Mask: 0x01)                          */
#define R_MSTP_MSTPCRF_MSTPF6_Pos     (6UL)          /*!< MSTPF6 (Bit 6)                                        */
#define R_MSTP_MSTPCRF_MSTPF6_Msk     (0x40UL)       /*!< MSTPF6 (Bitfield-Mask: 0x01)                          */

#endif /* R_SYSTEM_POSMSK_H */
