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

#ifndef R_MPC_REG_H
#define R_MPC_REG_H

/* ================                              Device Specific Cluster Section                              ================ */

/** @addtogroup Device_Peripheral_clusters
 * @{
 */

/**
 * @brief R_PFS_PORT [PIN] (Pmn Pin Function Control Register)
 */
typedef struct
{
    union
    {
        __IOM uint8_t PmnPFS;          /*!< (@ 0x00000000) Pmn Pin Function Control Register                          */

        struct
        {
            __IOM uint8_t PSEL : 6;    /*!< [5..0] Pin Function Select                                                */
            __IOM uint8_t ISEL : 1;    /*!< [6..6] Interrupt Input Function Select                                    */
            __IOM uint8_t ASEL : 1;    /*!< [7..7] Analog Function Select                                             */
        } PmnPFS_b;
    };
} R_PFS_PORT_Type;                     /*!< Size = 1 (0x1)                                                            */

/**
 * @brief R_PFS [PORT] (Pmn Port Function Control Register)
 */
typedef struct
{
    __IOM R_PFS_PORT_Type PIN[8];      /*!< (@ 0x00000000) Pmn Pin Function Control Register                          */
} R_PFS_Type;                          /*!< Size = 8 (0x8)                                                            */

/** @} */ /* End of group Device_Peripheral_clusters */

/* ================                                           R_MPC                                           ================ */

/**
 * @brief Multi - Function Pin Controller (R_MPC)
 */

typedef struct                         /*!< (@ 0x0008C100) R_MPC Structure                                            */
{
    union
    {
        __IOM uint8_t PFCSE;           /*!< (@ 0x00000000) CS Output Enable Register                                  */

        struct
        {
            __IOM uint8_t CS0E : 1;    /*!< [0..0] CS0 Enable                                                         */
            __IOM uint8_t CS1E : 1;    /*!< [1..1] CS1 Enable                                                         */
            __IOM uint8_t CS2E : 1;    /*!< [2..2] CS2 Enable                                                         */
            __IOM uint8_t CS3E : 1;    /*!< [3..3] CS3 Enable                                                         */
            __IOM uint8_t CS4E : 1;    /*!< [4..4] CS4 Enable                                                         */
            __IOM uint8_t CS5E : 1;    /*!< [5..5] CS5 Enable                                                         */
            __IOM uint8_t CS6E : 1;    /*!< [6..6] CS6 Enable                                                         */
            __IOM uint8_t CS7E : 1;    /*!< [7..7] CS7 Enable                                                         */
        } PFCSE_b;
    };
    __IM uint8_t RESERVED;

    union
    {
        __IOM uint8_t PFCSS0;          /*!< (@ 0x00000002) CS Output Pin Select Register 0                            */

        struct
        {
            __IOM uint8_t CS0S : 1;    /*!< [0..0] CS0# Output Pin Select                                             */
            uint8_t            : 1;
            __IOM uint8_t CS1S : 2;    /*!< [3..2] CS1# Output Pin Select                                             */
            __IOM uint8_t CS2S : 2;    /*!< [5..4] CS2# Output Pin Select                                             */
            __IOM uint8_t CS3S : 2;    /*!< [7..6] CS3# Output Pin Select                                             */
        } PFCSS0_b;
    };

    union
    {
        __IOM uint8_t PFCSS1;          /*!< (@ 0x00000003) CS Output Pin Select Register 1                            */

        struct
        {
            __IOM uint8_t CS4S : 2;    /*!< [1..0] CS4# Output Pin Select                                             */
            __IOM uint8_t CS5S : 2;    /*!< [3..2] CS5# Output Pin Select                                             */
            __IOM uint8_t CS6S : 2;    /*!< [5..4] CS6# Output Pin Select                                             */
            __IOM uint8_t CS7S : 2;    /*!< [7..6] CS7# Output Pin Select                                             */
        } PFCSS1_b;
    };

    union
    {
        __IOM uint8_t PFAOE0;          /*!< (@ 0x00000004) Address Output Enable Register 0                           */

        struct
        {
            __IOM uint8_t A8E  : 1;    /*!< [0..0] Address A8 Output Enable                                           */
            __IOM uint8_t A9E  : 1;    /*!< [1..1] Address A9 Output Enable                                           */
            __IOM uint8_t A10E : 1;    /*!< [2..2] Address A10 Output Enable                                          */
            __IOM uint8_t A11E : 1;    /*!< [3..3] Address A11 Output Enable                                          */
            __IOM uint8_t A12E : 1;    /*!< [4..4] Address A12 Output Enable                                          */
            __IOM uint8_t A13E : 1;    /*!< [5..5] Address A13 Output Enable                                          */
            __IOM uint8_t A14E : 1;    /*!< [6..6] Address A14 Output Enable                                          */
            __IOM uint8_t A15E : 1;    /*!< [7..7] Address A15 Output Enable                                          */
        } PFAOE0_b;
    };

    union
    {
        __IOM uint8_t PFAOE1;          /*!< (@ 0x00000005) Address Output Enable Register 1                           */

        struct
        {
            __IOM uint8_t A16E : 1;    /*!< [0..0] Address A16 Output Enable                                          */
            __IOM uint8_t A17E : 1;    /*!< [1..1] Address A17 Output Enable                                          */
            __IOM uint8_t A18E : 1;    /*!< [2..2] Address A18 Output Enable                                          */
            __IOM uint8_t A19E : 1;    /*!< [3..3] Address A19 Output Enable                                          */
            __IOM uint8_t A20E : 1;    /*!< [4..4] Address A20 Output Enable                                          */
            __IOM uint8_t A21E : 1;    /*!< [5..5] Address A21 Output Enable                                          */
            __IOM uint8_t A22E : 1;    /*!< [6..6] Address A22 Output Enable                                          */
            __IOM uint8_t A23E : 1;    /*!< [7..7] Address A23 Output Enable                                          */
        } PFAOE1_b;
    };

    union
    {
        __IOM uint8_t PFBCR0;            /*!< (@ 0x00000006) External Bus Control Register 0                            */

        struct
        {
            __IOM uint8_t ADRLE     : 1; /*!< [0..0] A0 to A7 Output Enable                                             */
            __IOM uint8_t ADRHMS    : 1; /*!< [1..1] A16 to A23 Output Enable                                           */
            __IOM uint8_t ADRHMS2   : 1; /*!< [2..2] A16 to A23 Output Enable 2                                         */
            __IOM uint8_t BCLKO     : 1; /*!< [3..3] BCLK Forced Output                                                 */
            __IOM uint8_t DHE       : 1; /*!< [4..4] D8 to D15 Output Enable                                            */
            __IOM uint8_t DH32E     : 1; /*!< [5..5] D16 to D31 Output Enable                                           */
            __IOM uint8_t WR1BC1E   : 1; /*!< [6..6] WR1#/BC1# Output Enable                                            */
            __IOM uint8_t WR32BC32E : 1; /*!< [7..7] WR3#/BC3# and WR2#/BC2# Output Enable                              */
        } PFBCR0_b;
    };

    union
    {
        __IOM uint8_t PFBCR1;          /*!< (@ 0x00000007) External Bus Control Register 1                            */

        struct
        {
            __IOM uint8_t WAITS  : 2;  /*!< [1..0] WAIT Select                                                        */
            __IOM uint8_t ALEOE  : 1;  /*!< [2..2] ALE Output Enable                                                  */
            __IOM uint8_t ALES   : 1;  /*!< [3..3] ALE Select                                                         */
            __IOM uint8_t MDSDE  : 1;  /*!< [4..4] SDRAM Pin Enable                                                   */
            uint8_t              : 1;
            __IOM uint8_t DQM1E  : 1;  /*!< [6..6] DQM1 Enable                                                        */
            __IOM uint8_t SDCLKE : 1;  /*!< [7..7] SDCLK Enable                                                       */
        } PFBCR1_b;
    };

    union
    {
        __IOM uint8_t PFBCR2;          /*!< (@ 0x00000008) External Bus Control Register 2                            */

        struct
        {
            __IOM uint8_t D0S : 2;     /*!< [1..0] D0 Selection                                                       */
            __IOM uint8_t D1S : 2;     /*!< [3..2] D1 Selection                                                       */
            __IOM uint8_t D2S : 2;     /*!< [5..4] D2 Selection                                                       */
            __IOM uint8_t D3S : 2;     /*!< [7..6] D3 Selection                                                       */
        } PFBCR2_b;
    };

    union
    {
        __IOM uint8_t PFBCR3;           /*!< (@ 0x00000009) External Bus Control Register 3                            */

        struct
        {
            __IOM uint8_t DLHS     : 1; /*!< [0..0] D4 to D7 Selection                                                 */
            uint8_t                : 5;
            __IOM uint8_t SDCLKDRV : 1; /*!< [6..6] SDCLK Pin Driving Ability Select                                   */
            __IOM uint8_t WAITS2   : 1; /*!< [7..7] WAIT Select 2                                                      */
        } PFBCR3_b;
    };
    __IM uint8_t RESERVED1[4];

    union
    {
        __IOM uint8_t PFENET;          /*!< (@ 0x0000000E) PFENET                                                     */

        struct
        {
            uint8_t                : 4;
            __IOM uint8_t PHYMODE0 : 1; /*!< [4..4] Ethernet Channel 1 Mode Set                                        */
            __IOM uint8_t PHYMODE1 : 1; /*!< [5..5] Ethernet Channel 0 Mode Set                                        */
            uint8_t                : 2;
        } PFENET_b;
    };
    __IM uint8_t RESERVED2[16];

    union
    {
        __IOM uint8_t PWPR;            /*!< (@ 0x0000001F) Write - protect Register                                   */

        struct
        {
            uint8_t             : 6;
            __IOM uint8_t PFSWE : 1;   /*!< [6..6] PFS Register Write Enable                                          */
            __IOM uint8_t B0WI  : 1;   /*!< [7..7] PFSWE Bit Write Disable                                            */
        } PWPR_b;
    };
    __IM uint8_t     RESERVED3[32];
    __IOM R_PFS_Type PORT[24];         /*!< (@ 0x00000040) Pmn Port Function Control Register                         */
} R_MPC_Type;                          /*!< Size = 256 (0x100)                                                        */

#endif /* R_MPC_REG_H */
