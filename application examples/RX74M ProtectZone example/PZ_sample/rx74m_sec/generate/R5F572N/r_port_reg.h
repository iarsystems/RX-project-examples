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

#ifndef R_PORT_REG_H
#define R_PORT_REG_H

/* ================                              Device Specific Cluster Section                              ================ */

/** @addtogroup Device_Peripheral_clusters
 * @{
 */

/**
 * @brief R_PORT_ODR_ST [ODR_ST] (Open-Drain Control Registers)
 */
typedef struct
{
    union
    {
        __IOM uint8_t ODR_0;           /*!< (@ 0x00000000) Port Open-Drain Control Register 0                         */

        struct
        {
            __IOM uint8_t B0 : 1;      /*!< [0..0] Pm0 Output Type Select                                             */
            uint8_t          : 1;
            __IOM uint8_t B2 : 1;      /*!< [2..2] Pm1 Output Type Select                                             */
            __IOM uint8_t B3 : 1;      /*!< [3..3] PE1 Output Type Select                                             */
            __IOM uint8_t B4 : 1;      /*!< [4..4] Pm2 Output Type Select                                             */
            uint8_t          : 1;
            __IOM uint8_t B6 : 1;      /*!< [6..6] Pm3 Output Type Select                                             */
            uint8_t          : 1;
        } ODR_0_b;
    };

    union
    {
        __IOM uint8_t ODR_1;           /*!< (@ 0x00000001) Port Open-Drain Control Register 1                         */

        struct
        {
            __IOM uint8_t B0 : 1;      /*!< [0..0] Pm4 Output Type Select                                             */
            uint8_t          : 1;
            __IOM uint8_t B2 : 1;      /*!< [2..2] Pm5 Output Type Select                                             */
            uint8_t          : 1;
            __IOM uint8_t B4 : 1;      /*!< [4..4] Pm6 Output Type Select                                             */
            uint8_t          : 1;
            __IOM uint8_t B6 : 1;      /*!< [6..6] Pm7 Output Type Select                                             */
            uint8_t          : 1;
        } ODR_1_b;
    };
} R_PORT_ODR_ST_Type;                  /*!< Size = 2 (0x2)                                                            */

/* ================                                          R_PORT                                           ================ */

/**
 * @brief I/O Ports (R_PORT)
 */

typedef struct                         /*!< (@ 0x0008C000) R_PORT Structure                                           */
{
    union
    {
        __IOM uint8_t PDR[24];         /*!< (@ 0x00000000) Port [0..23] Register                                      */

        struct
        {
            __IOM uint8_t B0 : 1;      /*!< [0..0] Pm0 I/O Select                                                     */
            __IOM uint8_t B1 : 1;      /*!< [1..1] Pm1 I/O Select                                                     */
            __IOM uint8_t B2 : 1;      /*!< [2..2] Pm2 I/O Select                                                     */
            __IOM uint8_t B3 : 1;      /*!< [3..3] Pm3 I/O Select                                                     */
            __IOM uint8_t B4 : 1;      /*!< [4..4] Pm4 I/O Select                                                     */
            __IOM uint8_t B5 : 1;      /*!< [5..5] Pm5 I/O Select                                                     */
            __IOM uint8_t B6 : 1;      /*!< [6..6] Pm6 I/O Select                                                     */
            __IOM uint8_t B7 : 1;      /*!< [7..7] Pm7 I/O Select                                                     */
        } PDR_b[24];
    };
    __IM uint8_t RESERVED[8];

    union
    {
        __IOM uint8_t PODR[24];        /*!< (@ 0x00000020) Port [0..23] Output Data Register                          */

        struct
        {
            __IOM uint8_t B0 : 1;      /*!< [0..0] Pm0 Output Data Store                                              */
            __IOM uint8_t B1 : 1;      /*!< [1..1] Pm1 Output Data Store                                              */
            __IOM uint8_t B2 : 1;      /*!< [2..2] Pm2 Output Data Store                                              */
            __IOM uint8_t B3 : 1;      /*!< [3..3] Pm3 Output Data Store                                              */
            __IOM uint8_t B4 : 1;      /*!< [4..4] Pm4 Output Data Store                                              */
            __IOM uint8_t B5 : 1;      /*!< [5..5] Pm5 Output Data Store                                              */
            __IOM uint8_t B6 : 1;      /*!< [6..6] Pm6 Output Data Store                                              */
            __IOM uint8_t B7 : 1;      /*!< [7..7] Pm7 Output Data Store                                              */
        } PODR_b[24];
    };
    __IM uint8_t RESERVED1[8];

    union
    {
        __IOM uint8_t PIDR[24];        /*!< (@ 0x00000040) Port [0..23] Input Register                                */

        struct
        {
            __IOM uint8_t B0 : 1;      /*!< [0..0] Pm0                                                                */
            __IOM uint8_t B1 : 1;      /*!< [1..1] Pm1                                                                */
            __IOM uint8_t B2 : 1;      /*!< [2..2] Pm2                                                                */
            __IOM uint8_t B3 : 1;      /*!< [3..3] Pm3                                                                */
            __IOM uint8_t B4 : 1;      /*!< [4..4] Pm4                                                                */
            __IOM uint8_t B5 : 1;      /*!< [5..5] Pm5                                                                */
            __IOM uint8_t B6 : 1;      /*!< [6..6] Pm6                                                                */
            __IOM uint8_t B7 : 1;      /*!< [7..7] Pm7                                                                */
        } PIDR_b[24];
    };
    __IM uint8_t RESERVED2[8];

    union
    {
        __IOM uint8_t PMR[24];           /*!< (@ 0x00000060) Port [0..23] Mode Register                                 */

        struct
        {
            __IOM uint8_t B0 : 1;        /*!< [0..0] Pm0 Pin Mode Control                                               */
            __IOM uint8_t B1 : 1;        /*!< [1..1] Pm1 Pin Mode Control                                               */
            __IOM uint8_t B2 : 1;        /*!< [2..2] Pm2 Pin Mode Control                                               */
            __IOM uint8_t B3 : 1;        /*!< [3..3] Pm3 Pin Mode Control                                               */
            __IOM uint8_t B4 : 1;        /*!< [4..4] Pm4 Pin Mode Control                                               */
            __IOM uint8_t B5 : 1;        /*!< [5..5] Pm5 Pin Mode Control                                               */
            __IOM uint8_t B6 : 1;        /*!< [6..6] Pm6 Pin Mode Control                                               */
            __IOM uint8_t B7 : 1;        /*!< [7..7] Pm7 Pin Mode Control                                               */
        } PMR_b[24];
    };
    __IM uint8_t             RESERVED3[8];
    __IOM R_PORT_ODR_ST_Type ODR_ST[24]; /*!< (@ 0x00000080) Open-Drain Control Registers                               */
    __IM uint8_t             RESERVED4[16];

    union
    {
        __IOM uint8_t PCR[24];         /*!< (@ 0x000000C0) Port [0..23] Pull-Up Resistor Control Register             */

        struct
        {
            __IOM uint8_t B0 : 1;      /*!< [0..0] Pm0 Pull-Up Resistor Control Register                              */
            __IOM uint8_t B1 : 1;      /*!< [1..1] Pm1 Pull-Up Resistor Control Register                              */
            __IOM uint8_t B2 : 1;      /*!< [2..2] Pm2 Pull-Up Resistor Control Register                              */
            __IOM uint8_t B3 : 1;      /*!< [3..3] Pm3 Pull-Up Resistor Control Register                              */
            __IOM uint8_t B4 : 1;      /*!< [4..4] Pm4 Pull-Up Resistor Control Register                              */
            __IOM uint8_t B5 : 1;      /*!< [5..5] Pm5 Pull-Up Resistor Control Register                              */
            __IOM uint8_t B6 : 1;      /*!< [6..6] Pm6 Pull-Up Resistor Control Register                              */
            __IOM uint8_t B7 : 1;      /*!< [7..7] Pm7 Pull-Up Resistor Control Register                              */
        } PCR_b[24];
    };
    __IM uint8_t RESERVED5[8];

    union
    {
        __IOM uint8_t DSCR[24];        /*!< (@ 0x000000E0) Port [0..23] Drive Capacity Control Register               */

        struct
        {
            __IOM uint8_t B0 : 1;      /*!< [0..0] Pm0 Drive Capacity Control                                         */
            __IOM uint8_t B1 : 1;      /*!< [1..1] Pm1 Drive Capacity Control                                         */
            __IOM uint8_t B2 : 1;      /*!< [2..2] Pm2 Drive Capacity Control                                         */
            __IOM uint8_t B3 : 1;      /*!< [3..3] Pm3 Drive Capacity Control                                         */
            __IOM uint8_t B4 : 1;      /*!< [4..4] Pm4 Drive Capacity Control                                         */
            __IOM uint8_t B5 : 1;      /*!< [5..5] Pm5 Drive Capacity Control                                         */
            __IOM uint8_t B6 : 1;      /*!< [6..6] Pm6 Drive Capacity Control                                         */
            __IOM uint8_t B7 : 1;      /*!< [7..7] Pm7 Drive Capacity Control                                         */
        } DSCR_b[24];
    };
    __IM uint8_t RESERVED6[48];

    union
    {
        __IOM uint8_t DSCR_2[24];      /*!< (@ 0x00000128) Port [0..23] Drive Capacity Control Register
                                        *                  2                                                          */

        struct
        {
            __IOM uint8_t B0 : 1;      /*!< [0..0] Pm0 Drive Capacity Control 2                                       */
            __IOM uint8_t B1 : 1;      /*!< [1..1] Pm1 Drive Capacity Control 2                                       */
            __IOM uint8_t B2 : 1;      /*!< [2..2] Pm2 Drive Capacity Control 2                                       */
            __IOM uint8_t B3 : 1;      /*!< [3..3] Pm3 Drive Capacity Control 2                                       */
            __IOM uint8_t B4 : 1;      /*!< [4..4] Pm4 Drive Capacity Control 2                                       */
            __IOM uint8_t B5 : 1;      /*!< [5..5] Pm5 Drive Capacity Control 2                                       */
            __IOM uint8_t B6 : 1;      /*!< [6..6] Pm6 Drive Capacity Control 2                                       */
            __IOM uint8_t B7 : 1;      /*!< [7..7] Pm7 Drive Capacity Control 2                                       */
        } DSCR_2_b[24];
    };
} R_PORT_Type;                         /*!< Size = 320 (0x140)                                                        */

#endif /* R_PORT_REG_H */
