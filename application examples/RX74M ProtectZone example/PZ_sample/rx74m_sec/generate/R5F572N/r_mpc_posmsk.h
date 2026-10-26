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

#ifndef R_MPC_POSMSK_H
#define R_MPC_POSMSK_H

/* ================                                            PIN                                            ================ */

/* ========================================================  PmnPFS  ========================================================= */
#define R_MPC_PORT_PIN_PmnPFS_PSEL_Pos    (0UL)    /*!< PSEL (Bit 0)                                          */
#define R_MPC_PORT_PIN_PmnPFS_PSEL_Msk    (0x3fUL) /*!< PSEL (Bitfield-Mask: 0x3f)                            */
#define R_MPC_PORT_PIN_PmnPFS_ISEL_Pos    (6UL)    /*!< ISEL (Bit 6)                                          */
#define R_MPC_PORT_PIN_PmnPFS_ISEL_Msk    (0x40UL) /*!< ISEL (Bitfield-Mask: 0x01)                            */
#define R_MPC_PORT_PIN_PmnPFS_ASEL_Pos    (7UL)    /*!< ASEL (Bit 7)                                          */
#define R_MPC_PORT_PIN_PmnPFS_ASEL_Msk    (0x80UL) /*!< ASEL (Bitfield-Mask: 0x01)                            */

/* ================                                           R_MPC                                           ================ */

/* =========================================================  PWPR  ========================================================== */
#define R_MPC_PWPR_B0WI_Pos               (7UL)    /*!< B0WI (Bit 7)                                          */
#define R_MPC_PWPR_B0WI_Msk               (0x80UL) /*!< B0WI (Bitfield-Mask: 0x01)                            */
#define R_MPC_PWPR_PFSWE_Pos              (6UL)    /*!< PFSWE (Bit 6)                                         */
#define R_MPC_PWPR_PFSWE_Msk              (0x40UL) /*!< PFSWE (Bitfield-Mask: 0x01)                           */
/* =========================================================  PFCSE  ========================================================= */
#define R_MPC_PFCSE_CS7E_Pos              (7UL)    /*!< CS7E (Bit 7)                                          */
#define R_MPC_PFCSE_CS7E_Msk              (0x80UL) /*!< CS7E (Bitfield-Mask: 0x01)                            */
#define R_MPC_PFCSE_CS6E_Pos              (6UL)    /*!< CS6E (Bit 6)                                          */
#define R_MPC_PFCSE_CS6E_Msk              (0x40UL) /*!< CS6E (Bitfield-Mask: 0x01)                            */
#define R_MPC_PFCSE_CS5E_Pos              (5UL)    /*!< CS5E (Bit 5)                                          */
#define R_MPC_PFCSE_CS5E_Msk              (0x20UL) /*!< CS5E (Bitfield-Mask: 0x01)                            */
#define R_MPC_PFCSE_CS4E_Pos              (4UL)    /*!< CS4E (Bit 4)                                          */
#define R_MPC_PFCSE_CS4E_Msk              (0x10UL) /*!< CS4E (Bitfield-Mask: 0x01)                            */
#define R_MPC_PFCSE_CS3E_Pos              (3UL)    /*!< CS3E (Bit 3)                                          */
#define R_MPC_PFCSE_CS3E_Msk              (0x8UL)  /*!< CS3E (Bitfield-Mask: 0x01)                            */
#define R_MPC_PFCSE_CS2E_Pos              (2UL)    /*!< CS2E (Bit 2)                                          */
#define R_MPC_PFCSE_CS2E_Msk              (0x4UL)  /*!< CS2E (Bitfield-Mask: 0x01)                            */
#define R_MPC_PFCSE_CS1E_Pos              (1UL)    /*!< CS1E (Bit 1)                                          */
#define R_MPC_PFCSE_CS1E_Msk              (0x2UL)  /*!< CS1E (Bitfield-Mask: 0x01)                            */
#define R_MPC_PFCSE_CS0E_Pos              (0UL)    /*!< CS0E (Bit 0)                                          */
#define R_MPC_PFCSE_CS0E_Msk              (0x1UL)  /*!< CS0E (Bitfield-Mask: 0x01)                            */
/* ========================================================  PFCSS0  ========================================================= */
#define R_MPC_PFCSS0_CS3S_Pos             (6UL)    /*!< CS3S (Bit 6)                                          */
#define R_MPC_PFCSS0_CS3S_Msk             (0xc0UL) /*!< CS3S (Bitfield-Mask: 0x03)                            */
#define R_MPC_PFCSS0_CS2S_Pos             (4UL)    /*!< CS2S (Bit 4)                                          */
#define R_MPC_PFCSS0_CS2S_Msk             (0x30UL) /*!< CS2S (Bitfield-Mask: 0x03)                            */
#define R_MPC_PFCSS0_CS1S_Pos             (2UL)    /*!< CS1S (Bit 2)                                          */
#define R_MPC_PFCSS0_CS1S_Msk             (0xcUL)  /*!< CS1S (Bitfield-Mask: 0x03)                            */
#define R_MPC_PFCSS0_CS0S_Pos             (0UL)    /*!< CS0S (Bit 0)                                          */
#define R_MPC_PFCSS0_CS0S_Msk             (0x1UL)  /*!< CS0S (Bitfield-Mask: 0x01)                            */
/* ========================================================  PFCSS1  ========================================================= */
#define R_MPC_PFCSS1_CS7S_Pos             (6UL)    /*!< CS7S (Bit 6)                                          */
#define R_MPC_PFCSS1_CS7S_Msk             (0xc0UL) /*!< CS7S (Bitfield-Mask: 0x03)                            */
#define R_MPC_PFCSS1_CS6S_Pos             (4UL)    /*!< CS6S (Bit 4)                                          */
#define R_MPC_PFCSS1_CS6S_Msk             (0x30UL) /*!< CS6S (Bitfield-Mask: 0x03)                            */
#define R_MPC_PFCSS1_CS5S_Pos             (2UL)    /*!< CS5S (Bit 2)                                          */
#define R_MPC_PFCSS1_CS5S_Msk             (0xcUL)  /*!< CS5S (Bitfield-Mask: 0x03)                            */
#define R_MPC_PFCSS1_CS4S_Pos             (0UL)    /*!< CS4S (Bit 0)                                          */
#define R_MPC_PFCSS1_CS4S_Msk             (0x3UL)  /*!< CS4S (Bitfield-Mask: 0x03)                            */
/* ========================================================  PFAOE0  ========================================================= */
#define R_MPC_PFAOE0_A15E_Pos             (7UL)    /*!< A15E (Bit 7)                                          */
#define R_MPC_PFAOE0_A15E_Msk             (0x80UL) /*!< A15E (Bitfield-Mask: 0x01)                            */
#define R_MPC_PFAOE0_A14E_Pos             (6UL)    /*!< A14E (Bit 6)                                          */
#define R_MPC_PFAOE0_A14E_Msk             (0x40UL) /*!< A14E (Bitfield-Mask: 0x01)                            */
#define R_MPC_PFAOE0_A13E_Pos             (5UL)    /*!< A13E (Bit 5)                                          */
#define R_MPC_PFAOE0_A13E_Msk             (0x20UL) /*!< A13E (Bitfield-Mask: 0x01)                            */
#define R_MPC_PFAOE0_A12E_Pos             (4UL)    /*!< A12E (Bit 4)                                          */
#define R_MPC_PFAOE0_A12E_Msk             (0x10UL) /*!< A12E (Bitfield-Mask: 0x01)                            */
#define R_MPC_PFAOE0_A11E_Pos             (3UL)    /*!< A11E (Bit 3)                                          */
#define R_MPC_PFAOE0_A11E_Msk             (0x8UL)  /*!< A11E (Bitfield-Mask: 0x01)                            */
#define R_MPC_PFAOE0_A10E_Pos             (2UL)    /*!< A10E (Bit 2)                                          */
#define R_MPC_PFAOE0_A10E_Msk             (0x4UL)  /*!< A10E (Bitfield-Mask: 0x01)                            */
#define R_MPC_PFAOE0_A9E_Pos              (1UL)    /*!< A9E (Bit 1)                                           */
#define R_MPC_PFAOE0_A9E_Msk              (0x2UL)  /*!< A9E (Bitfield-Mask: 0x01)                             */
#define R_MPC_PFAOE0_A8E_Pos              (0UL)    /*!< A8E (Bit 0)                                           */
#define R_MPC_PFAOE0_A8E_Msk              (0x1UL)  /*!< A8E (Bitfield-Mask: 0x01)                             */
/* ========================================================  PFAOE1  ========================================================= */
#define R_MPC_PFAOE1_A23E_Pos             (7UL)    /*!< A23E (Bit 7)                                          */
#define R_MPC_PFAOE1_A23E_Msk             (0x80UL) /*!< A23E (Bitfield-Mask: 0x01)                            */
#define R_MPC_PFAOE1_A22E_Pos             (6UL)    /*!< A22E (Bit 6)                                          */
#define R_MPC_PFAOE1_A22E_Msk             (0x40UL) /*!< A22E (Bitfield-Mask: 0x01)                            */
#define R_MPC_PFAOE1_A21E_Pos             (5UL)    /*!< A21E (Bit 5)                                          */
#define R_MPC_PFAOE1_A21E_Msk             (0x20UL) /*!< A21E (Bitfield-Mask: 0x01)                            */
#define R_MPC_PFAOE1_A20E_Pos             (4UL)    /*!< A20E (Bit 4)                                          */
#define R_MPC_PFAOE1_A20E_Msk             (0x10UL) /*!< A20E (Bitfield-Mask: 0x01)                            */
#define R_MPC_PFAOE1_A19E_Pos             (3UL)    /*!< A19E (Bit 3)                                          */
#define R_MPC_PFAOE1_A19E_Msk             (0x8UL)  /*!< A19E (Bitfield-Mask: 0x01)                            */
#define R_MPC_PFAOE1_A18E_Pos             (2UL)    /*!< A18E (Bit 2)                                          */
#define R_MPC_PFAOE1_A18E_Msk             (0x4UL)  /*!< A18E (Bitfield-Mask: 0x01)                            */
#define R_MPC_PFAOE1_A17E_Pos             (1UL)    /*!< A17E (Bit 1)                                          */
#define R_MPC_PFAOE1_A17E_Msk             (0x2UL)  /*!< A17E (Bitfield-Mask: 0x01)                            */
#define R_MPC_PFAOE1_A16E_Pos             (0UL)    /*!< A16E (Bit 0)                                          */
#define R_MPC_PFAOE1_A16E_Msk             (0x1UL)  /*!< A16E (Bitfield-Mask: 0x01)                            */
/* ========================================================  PFBCR0  ========================================================= */
#define R_MPC_PFBCR0_WR32BC32E_Pos        (7UL)    /*!< WR32BC32E (Bit 7)                                     */
#define R_MPC_PFBCR0_WR32BC32E_Msk        (0x80UL) /*!< WR32BC32E (Bitfield-Mask: 0x01)                       */
#define R_MPC_PFBCR0_WR1BC1E_Pos          (6UL)    /*!< WR1BC1E (Bit 6)                                       */
#define R_MPC_PFBCR0_WR1BC1E_Msk          (0x40UL) /*!< WR1BC1E (Bitfield-Mask: 0x01)                         */
#define R_MPC_PFBCR0_DH32E_Pos            (5UL)    /*!< DH32E (Bit 5)                                         */
#define R_MPC_PFBCR0_DH32E_Msk            (0x20UL) /*!< DH32E (Bitfield-Mask: 0x01)                           */
#define R_MPC_PFBCR0_DHE_Pos              (4UL)    /*!< DHE (Bit 4)                                           */
#define R_MPC_PFBCR0_DHE_Msk              (0x10UL) /*!< DHE (Bitfield-Mask: 0x01)                             */
#define R_MPC_PFBCR0_BCLKO_Pos            (3UL)    /*!< BCLKO (Bit 3)                                         */
#define R_MPC_PFBCR0_BCLKO_Msk            (0x8UL)  /*!< BCLKO (Bitfield-Mask: 0x01)                           */
#define R_MPC_PFBCR0_ADRHMS2_Pos          (2UL)    /*!< ADRHMS2 (Bit 2)                                       */
#define R_MPC_PFBCR0_ADRHMS2_Msk          (0x4UL)  /*!< ADRHMS2 (Bitfield-Mask: 0x01)                         */
#define R_MPC_PFBCR0_ADRHMS_Pos           (1UL)    /*!< ADRHMS (Bit 1)                                        */
#define R_MPC_PFBCR0_ADRHMS_Msk           (0x2UL)  /*!< ADRHMS (Bitfield-Mask: 0x01)                          */
#define R_MPC_PFBCR0_ADRLE_Pos            (0UL)    /*!< ADRLE (Bit 0)                                         */
#define R_MPC_PFBCR0_ADRLE_Msk            (0x1UL)  /*!< ADRLE (Bitfield-Mask: 0x01)                           */
/* ========================================================  PFBCR1  ========================================================= */
#define R_MPC_PFBCR1_SDCLKE_Pos           (7UL)    /*!< SDCLKE (Bit 7)                                        */
#define R_MPC_PFBCR1_SDCLKE_Msk           (0x80UL) /*!< SDCLKE (Bitfield-Mask: 0x01)                          */
#define R_MPC_PFBCR1_DQM1E_Pos            (6UL)    /*!< DQM1E (Bit 6)                                         */
#define R_MPC_PFBCR1_DQM1E_Msk            (0x40UL) /*!< DQM1E (Bitfield-Mask: 0x01)                           */
#define R_MPC_PFBCR1_MDSDE_Pos            (4UL)    /*!< MDSDE (Bit 4)                                         */
#define R_MPC_PFBCR1_MDSDE_Msk            (0x10UL) /*!< MDSDE (Bitfield-Mask: 0x01)                           */
#define R_MPC_PFBCR1_ALES_Pos             (3UL)    /*!< ALES (Bit 3)                                          */
#define R_MPC_PFBCR1_ALES_Msk             (0x8UL)  /*!< ALES (Bitfield-Mask: 0x01)                            */
#define R_MPC_PFBCR1_ALEOE_Pos            (2UL)    /*!< ALEOE (Bit 2)                                         */
#define R_MPC_PFBCR1_ALEOE_Msk            (0x4UL)  /*!< ALEOE (Bitfield-Mask: 0x01)                           */
#define R_MPC_PFBCR1_WAITS_Pos            (0UL)    /*!< WAITS (Bit 0)                                         */
#define R_MPC_PFBCR1_WAITS_Msk            (0x3UL)  /*!< WAITS (Bitfield-Mask: 0x03)                           */
/* ========================================================  PFBCR2  ========================================================= */
#define R_MPC_PFBCR2_D3S_Pos              (6UL)    /*!< D3S (Bit 6)                                           */
#define R_MPC_PFBCR2_D3S_Msk              (0xc0UL) /*!< D3S (Bitfield-Mask: 0x03)                             */
#define R_MPC_PFBCR2_D2S_Pos              (4UL)    /*!< D2S (Bit 4)                                           */
#define R_MPC_PFBCR2_D2S_Msk              (0x30UL) /*!< D2S (Bitfield-Mask: 0x03)                             */
#define R_MPC_PFBCR2_D1S_Pos              (2UL)    /*!< D1S (Bit 2)                                           */
#define R_MPC_PFBCR2_D1S_Msk              (0xcUL)  /*!< D1S (Bitfield-Mask: 0x03)                             */
#define R_MPC_PFBCR2_D0S_Pos              (0UL)    /*!< D0S (Bit 0)                                           */
#define R_MPC_PFBCR2_D0S_Msk              (0x3UL)  /*!< D0S (Bitfield-Mask: 0x03)                             */
/* ========================================================  PFBCR3  ========================================================= */
#define R_MPC_PFBCR3_WAITS2_Pos           (7UL)    /*!< WAITS2 (Bit 7)                                        */
#define R_MPC_PFBCR3_WAITS2_Msk           (0x80UL) /*!< WAITS2 (Bitfield-Mask: 0x01)                          */
#define R_MPC_PFBCR3_SDCLKDRV_Pos         (6UL)    /*!< SDCLKDRV (Bit 6)                                      */
#define R_MPC_PFBCR3_SDCLKDRV_Msk         (0x40UL) /*!< SDCLKDRV (Bitfield-Mask: 0x01)                        */
#define R_MPC_PFBCR3_DLHS_Pos             (0UL)    /*!< DLHS (Bit 0)                                          */
#define R_MPC_PFBCR3_DLHS_Msk             (0x1UL)  /*!< DLHS (Bitfield-Mask: 0x01)                            */
/* ========================================================  PFENET  ========================================================= */
#define R_MPC_PFENET_PHYMODE1_Pos         (5UL)    /*!< PHYMODE1 (Bit 5)                                      */
#define R_MPC_PFENET_PHYMODE1_Msk         (0x20UL) /*!< PHYMODE1 (Bitfield-Mask: 0x01)                        */
#define R_MPC_PFENET_PHYMODE0_Pos         (4UL)    /*!< PHYMODE0 (Bit 4)                                      */
#define R_MPC_PFENET_PHYMODE0_Msk         (0x10UL) /*!< PHYMODE0 (Bitfield-Mask: 0x01)                        */

#endif /* R_MPC_POSMSK_H */
