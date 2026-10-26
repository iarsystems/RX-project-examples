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

#ifndef R_PMGI_REG_H
#define R_PMGI_REG_H

/**
 * @brief PHY Management Interface0 (R_PMGI0)
 */

typedef struct                         /*!< (@ 0x000C5880) R_PMGI0 Structure                                          */
{
    union
    {
        __IOM uint32_t PMGCR;          /*!< (@ 0x00000000) PMGI Configuration Register                                */

        struct
        {
            uint32_t             : 8;
            __IOM uint32_t PSMCS : 6;  /*!< [13..8] PHY Station Management ClockSelect                                */
            uint32_t             : 1;
            __IOM uint32_t PSMDP : 1;  /*!< [15..15] PHY Station Management Preamble Control                          */
            __IOM uint32_t PSMHT : 3;  /*!< [18..16] PHY Station Management Hold Time Adjustment                      */
            uint32_t             : 1;
            __IOM uint32_t PSMCT : 3;  /*!< [22..20] PHY Station Management Capture Timing Adjustment                 */
            uint32_t             : 9;
        } PMGCR_b;
    };

    union
    {
        __IOM uint32_t PSMR;           /*!< (@ 0x00000004) PHY Station Management Register                            */

        struct
        {
            __IOM uint32_t PSME  : 1;  /*!< [0..0] PHY Station Management Start                                       */
            __IOM uint32_t PSMAD : 1;  /*!< [1..1] PHY Station Management Access Direction                            */
            uint32_t             : 1;
            __IOM uint32_t PDA   : 5;  /*!< [7..3] PHY Address                                                        */
            __IOM uint32_t PRA   : 5;  /*!< [12..8] PHY Register Address                                              */
            uint32_t             : 3;
            __IOM uint32_t PRD   : 16; /*!< [31..16] PHY Register Data                                                */
        } PSMR_b;
    };
} R_PMGI0_Type;                        /*!< Size = 8 (0x8)                                                            */

#endif /* R_PMGI_REG_H */
