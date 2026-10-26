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

#ifndef R_DMA_REG_H
#define R_DMA_REG_H

/* =========================================================================================================================== */
/* ================                                           R_DMA                                           ================ */
/* =========================================================================================================================== */

/**
 * @brief DMA Controller Common (R_DMA)
 */

typedef struct                         /*!< (@ 0x00082018) R_DMA Structure                                            */
{
    union
    {
        __IOM uint32_t DMOFR;          /*!< (@ 0x00000000) DMA Offset Register                                        */

        struct
        {
            __IOM uint32_t DMOFR : 32; /*!< [31..0] Specifies the offset when offset addition is selected
                                        *   as the address update mode for transfer source or destination.            */
        } DMOFR_b;
    };
    __IM uint32_t RESERVED[121];

    union
    {
        __IOM uint8_t DMAST;           /*!< (@ 0x000001E8) DMAC Module Start Register                                 */

        struct
        {
            __IOM uint8_t DMST : 1;    /*!< [0..0] DMAC Module Start                                                  */
            uint8_t            : 7;
        } DMAST_b;
    };
    __IM uint8_t  RESERVED1;
    __IM uint16_t RESERVED2;

    union
    {
        __IOM uint8_t DMIST;           /*!< (@ 0x000001EC) DMAC74 Interrupt Status Monitor Register                   */

        struct
        {
            uint8_t            : 4;
            __IM uint8_t DMIS4 : 1;    /*!< [4..4] DMAC4 Interrupt Status Flag                                        */
            __IM uint8_t DMIS5 : 1;    /*!< [5..5] DMAC5 Interrupt Status Flag                                        */
            __IM uint8_t DMIS6 : 1;    /*!< [6..6] DMAC6 Interrupt Status Flag                                        */
            __IM uint8_t DMIS7 : 1;    /*!< [7..7] DMAC7 Interrupt Status Flag                                        */
        } DMIST_b;
    };
    __IM uint8_t  RESERVED3;
    __IM uint16_t RESERVED4;
} R_DMA_Type;                          /*!< Size = 496 (0x1f0)                                                        */

#endif /* R_DMA_REG_H */
