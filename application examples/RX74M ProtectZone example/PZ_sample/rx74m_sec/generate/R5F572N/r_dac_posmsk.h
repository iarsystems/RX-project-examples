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

#ifndef R_DAC_POSMSK_H
#define R_DAC_POSMSK_H

/* =========================================================================================================================== */
/* ================                                           R_DAC                                           ================ */
/* =========================================================================================================================== */

/* =========================================================  DADR  ========================================================== */
#define R_DAC_DADR_DADR_Pos           (0UL)      /*!< DADR (Bit 0)                                          */
#define R_DAC_DADR_DADR_Msk           (0xffffUL) /*!< DADR (Bitfield-Mask: 0xffff)                          */
/* =========================================================  DACR  ========================================================== */
#define R_DAC_DACR_DAOE1_Pos          (7UL)      /*!< DAOE1 (Bit 7)                                         */
#define R_DAC_DACR_DAOE1_Msk          (0x80UL)   /*!< DAOE1 (Bitfield-Mask: 0x01)                           */
#define R_DAC_DACR_DAOE0_Pos          (6UL)      /*!< DAOE0 (Bit 6)                                         */
#define R_DAC_DACR_DAOE0_Msk          (0x40UL)   /*!< DAOE0 (Bitfield-Mask: 0x01)                           */
#define R_DAC_DACR_DAE_Pos            (5UL)      /*!< DAE (Bit 5)                                           */
#define R_DAC_DACR_DAE_Msk            (0x20UL)   /*!< DAE (Bitfield-Mask: 0x01)                             */
/* =========================================================  DADPR  ========================================================= */
#define R_DAC_DADPR_DPSEL_Pos         (7UL)      /*!< DPSEL (Bit 7)                                         */
#define R_DAC_DADPR_DPSEL_Msk         (0x80UL)   /*!< DPSEL (Bitfield-Mask: 0x01)                           */
/* ========================================================  DAADSCR  ======================================================== */
#define R_DAC_DAADSCR_DAADST_Pos      (7UL)      /*!< DAADST (Bit 7)                                        */
#define R_DAC_DAADSCR_DAADST_Msk      (0x80UL)   /*!< DAADST (Bitfield-Mask: 0x01)                          */
/* ========================================================  DAAMPCR  ======================================================== */
#define R_DAC_DAAMPCR_DAAMP0_Pos      (7UL)      /*!< DAAMP0 (Bit 7)                                        */
#define R_DAC_DAAMPCR_DAAMP0_Msk      (0x80UL)   /*!< DAAMP0 (Bitfield-Mask: 0x01)                          */
#define R_DAC_DAAMPCR_DAAMP1_Pos      (6UL)      /*!< DAAMP1 (Bit 6)                                        */
#define R_DAC_DAAMPCR_DAAMP1_Msk      (0x40UL)   /*!< DAAMP1 (Bitfield-Mask: 0x01)                          */
/* ========================================================  DAASWCR  ======================================================== */
#define R_DAC_DAASWCR_DAASW1_Pos      (7UL)      /*!< DAASW0 (Bit 7)                                        */
#define R_DAC_DAASWCR_DAASW1_Msk      (0x80UL)   /*!< DAASW0 (Bitfield-Mask: 0x01)                          */
#define R_DAC_DAASWCR_DAASW0_Pos      (6UL)      /*!< DAASW1 (Bit 6)                                        */
#define R_DAC_DAASWCR_DAASW0_Msk      (0x40UL)   /*!< DAASW1 (Bitfield-Mask: 0x01)                          */
/* ========================================================  DAADUSR  ======================================================== */
#define R_DAC_DAADUSR_AMADSEL1_Pos    (1UL)      /*!< AMADSEL1 (Bit 1)                                      */
#define R_DAC_DAADUSR_AMADSEL1_Msk    (0x2UL)    /*!< AMADSEL1 (Bitfield-Mask: 0x01)                        */

#endif /* R_DAC_POSMSK_H */
