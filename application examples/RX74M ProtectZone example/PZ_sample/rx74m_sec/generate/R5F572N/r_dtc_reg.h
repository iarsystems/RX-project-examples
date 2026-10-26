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

#ifndef R_DTC_REG_H
#define R_DTC_REG_H

/* =========================================================================================================================== */
/* ================                            Device Specific Peripheral Section                             ================ */
/* =========================================================================================================================== */

/** @addtogroup Device_Peripheral_peripherals
 * @{
 */

/* =========================================================================================================================== */
/* ================                                           R_DTC                                           ================ */
/* =========================================================================================================================== */

/**
 * @brief Data Transfer Controller (R_DTC)
 */

typedef struct                         /*!< (@ 0x00082400) R_DTC Structure                                            */
{
    union
    {
        __IOM uint8_t DTCCR;           /*!< (@ 0x00000000) DTC Control Register                                       */

        struct
        {
            uint8_t           : 4;
            __IOM uint8_t RRS : 1;     /*!< [4..4] DTC Transfer Information Read Skip Enable                          */
            uint8_t           : 3;
        } DTCCR_b;
    };
    __IM uint8_t  RESERVED;
    __IM uint16_t RESERVED1;

    union
    {
        __IOM uint32_t DTCVBR;          /*!< (@ 0x00000004) DTC Vector Base Register                                   */

        struct
        {
            __IOM uint32_t DTCVBR : 28; /*!< [27..0] The DTCVBR register is used to set the base address
                                         *   for calculating the address to which the DTC vector is
                                         *   allocated.                                                                */
            uint32_t : 4;
        } DTCVBR_b;
    };

    union
    {
        __IOM uint8_t DTCADMOD;        /*!< (@ 0x00000008) DTC Address Mode Register                                  */

        struct
        {
            __IOM uint8_t SHORT : 1;   /*!< [0..0] Short-Address Mode Set                                             */
            uint8_t             : 7;
        } DTCADMOD_b;
    };
    __IM uint8_t  RESERVED2;
    __IM uint16_t RESERVED3;

    union
    {
        __IOM uint8_t DTCST;           /*!< (@ 0x0000000C) DTC Module Start Register                                  */

        struct
        {
            __IOM uint8_t DTCST : 1;   /*!< [0..0] DTC Module Start                                                   */
            uint8_t             : 7;
        } DTCST_b;
    };
    __IM uint8_t RESERVED4;

    union
    {
        __IM uint16_t DTCSTS;          /*!< (@ 0x0000000E) DTC Status Register                                        */

        struct
        {
            __IM uint16_t VECN : 8;    /*!< [7..0] DTC Active Vector Number Monitoring Flag                           */
            uint16_t           : 7;
            __IM uint16_t ACT  : 1;    /*!< [15..15] DTC Active Flag                                                  */
        } DTCSTS_b;
    };

    union
    {
        __IOM uint32_t DTCIBR;          /*!< (@ 0x00000010) DTC Index Table Base Register                              */

        struct
        {
            __IOM uint32_t DTCIBR : 28; /*!< [27..0] The DTCIBR register is used to set the base address
                                         *   for calculating the address to which the DTC index is allocated.          */
            uint32_t : 4;
        } DTCIBR_b;
    };

    union
    {
        __IOM uint8_t DTCOR;           /*!< (@ 0x00000014) DTC Address Mode Register                                  */

        struct
        {
            __IOM uint8_t SQTFRL : 1;  /*!< [0..0] Sequence Transfer Terminate                                        */
            uint8_t              : 7;
        } DTCOR_b;
    };
    __IM uint8_t RESERVED5;

    union
    {
        __IOM uint16_t DTCSQE;         /*!< (@ 0x00000016) DTC Operation Register                                     */

        struct
        {
            __IOM uint16_t VECN   : 8; /*!< [7..0] Sequence Transfer Vector Number Setting                            */
            uint16_t              : 7;
            __IOM uint16_t ESPSEL : 1; /*!< [15..15] Sequence Transfer Enable                                         */
        } DTCSQE_b;
    };

    union
    {
        __IOM uint32_t DTCDISP;          /*!< (@ 0x00000018) DTC Address Displacement Register                          */

        struct
        {
            __IOM uint32_t DTCDISP : 32; /*!< [31..0] The DTCDISP register is used to specify the displacement
                                          *   value to add to the DTC transfer source address.                          */
        } DTCDISP_b;
    };
} R_DTC_Type;                            /*!< Size = 28 (0x1c)                                                          */

/** @} */ /* End of group Device_Peripheral_peripherals */

#endif /* R_DTC_REG_H */
