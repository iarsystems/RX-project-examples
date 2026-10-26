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

#ifndef R_SYSTEM_REG_H
#define R_SYSTEM_REG_H

/* ================                              Device Specific Cluster Section                              ================ */

/** @addtogroup Device_Peripheral_clusters
 * @{
 */

/**
 * @brief R_SYSTEM_DPSBKR [DPSBKR] (Deep Standby Backup Register)
 */
typedef struct
{
    union
    {
        __IOM uint8_t DPSBKR;          /*!< (@ 0x00000000) Deep Standby Backup Register                               */

        struct
        {
            __IOM uint8_t DPSBKR : 8;  /*!< [7..0] DPSBKR                                                             */
        } DPSBKR_b;
    };
} R_SYSTEM_DPSBKR_Type;                /*!< Size = 1 (0x1)                                                            */

/* ================                                         R_SYSTEM                                          ================ */

/**
 * @brief System register (R_SYSTEM)
 */

typedef struct                         /*!< (@ 0x00080000) R_SYSTEM Structure                                         */
{
    union
    {
        __IOM uint16_t MDMONR;         /*!< (@ 0x00000000) Mode Monitor Register                                      */

        struct
        {
            __IOM uint16_t MD : 1;     /*!< [0..0] MD Pin Status Flag                                                 */
            uint16_t          : 15;
        } MDMONR_b;
    };
    __IM uint16_t RESERVED[2];

    union
    {
        __IOM uint16_t SYSCR0;         /*!< (@ 0x00000006) System Control Register 0                                  */

        struct
        {
            __IOM uint16_t ROME : 1;   /*!< [0..0] On-Chip ROM Enable                                                 */
            __IOM uint16_t EXBE : 1;   /*!< [1..1] External Bus Enable                                                */
            uint16_t            : 6;
            __IOM uint16_t KEY  : 8;   /*!< [15..8] SYSCR0 Key Code                                                   */
        } SYSCR0_b;
    };

    union
    {
        __IOM uint16_t SYSCR1;          /*!< (@ 0x00000008) System Control Register 1                                  */

        struct
        {
            __IOM uint16_t RAME    : 1; /*!< [0..0] RAM Enable                                                         */
            uint16_t               : 5;
            __IOM uint16_t ECCRAME : 1; /*!< [6..6] ECCRAM Enable                                                      */
            __IOM uint16_t SBYRAME : 1; /*!< [7..7] Standby RAM Enable                                                 */
            uint16_t               : 8;
        } SYSCR1_b;
    };
    __IM uint16_t RESERVED1;

    union
    {
        __IOM uint16_t SBYCR;          /*!< (@ 0x0000000C) Standby Control Register                                   */

        struct
        {
            uint16_t            : 14;
            __IOM uint16_t OPE  : 1;   /*!< [14..14] Output Port Enable                                               */
            __IOM uint16_t SSBY : 1;   /*!< [15..15] Software Standby                                                 */
        } SBYCR_b;
    };
    __IM uint16_t RESERVED2;

    union
    {
        __IOM uint32_t MSTPCRA;         /*!< (@ 0x00000010) Module Stop Control Register A                             */

        struct
        {
            __IOM uint32_t MSTPA0 : 1;  /*!< [0..0] Compare Match Timer W (Unit 1) Module Stop                         */
            __IOM uint32_t MSTPA1 : 1;  /*!< [1..1] Compare Match Timer W (Unit 0) Module Stop                         */
            uint32_t              : 2;
            __IOM uint32_t MSTPA4 : 1;  /*!< [4..4] 8-Bit Timer 3/2 (Unit 1) Module Stop                               */
            __IOM uint32_t MSTPA5 : 1;  /*!< [5..5] 8-Bit Timer 1/0 (Unit 0) Module Stop                               */
            uint32_t              : 1;
            __IOM uint32_t MSTPA7 : 1;  /*!< [7..7] General PWM Timer/GPTW Dedicated Port Output Enable Module
                                         *   Stop                                                                      */
            uint32_t               : 1;
            __IOM uint32_t MSTPA9  : 1; /*!< [9..9] Multifunction Timer Pulse Unit Module Stop                         */
            __IOM uint32_t MSTPA10 : 1; /*!< [10..10] Programmable Pulse Generator (Unit 1) Module Stop                */
            __IOM uint32_t MSTPA11 : 1; /*!< [11..11] Programmable Pulse Generator (Unit 0) Module Stop                */
            uint32_t               : 1;
            __IOM uint32_t MSTPA13 : 1; /*!< [13..13] 16-Bit Timer Pulse Unit 0 (Unit 0) Module Stop                   */
            __IOM uint32_t MSTPA14 : 1; /*!< [14..14] Compare Match Timer (Unit 1) ModuleStop                          */
            __IOM uint32_t MSTPA15 : 1; /*!< [15..15] Compare Match Timer (Unit 0) Module Stop                         */
            __IOM uint32_t MSTPA16 : 1; /*!< [16..16] 12-bit A/D Converter (Unit 1) Module Stop                        */
            __IOM uint32_t MSTPA17 : 1; /*!< [17..17] 12-bit A/D Converter (Unit 0) Module Stop                        */
            uint32_t               : 1;
            __IOM uint32_t MSTPA19 : 1; /*!< [19..19] 12-bit D/A Converter Module Stop                                 */
            uint32_t               : 4;
            __IOM uint32_t MSTPA24 : 1; /*!< [24..24] Module Stop A24                                                  */
            uint32_t               : 2;
            __IOM uint32_t MSTPA27 : 1; /*!< [27..27] Module Stop A27                                                  */
            __IOM uint32_t MSTPA28 : 1; /*!< [28..28] DMA Controller/Data Transfer Controller Module Stop              */
            __IOM uint32_t MSTPA29 : 1; /*!< [29..29] EXDMA Controller Module Stop                                     */
            uint32_t               : 1;
            __IOM uint32_t ACSE    : 1; /*!< [31..31] All-Module Clock Stop Mode Enable                                */
        } MSTPCRA_b;
    };

    union
    {
        __IOM uint32_t MSTPCRB;         /*!< (@ 0x00000014) Module Stop Control Register B                             */

        struct
        {
            __IOM uint32_t MSTPB0  : 1; /*!< [0..0] CAN Module 0 Module Stop                                           */
            __IOM uint32_t MSTPB1  : 1; /*!< [1..1] CAN Module 1 Module Stop                                           */
            __IOM uint32_t MSTPB2  : 1; /*!< [2..2] CAN Module 2 Module Stop                                           */
            uint32_t               : 1;
            __IOM uint32_t MSTPB4  : 1; /*!< [4..4] Serial Communication Interface SCIhModule Stop                     */
            uint32_t               : 1;
            __IOM uint32_t MSTPB6  : 1; /*!< [6..6] Data Operation Circuit Module Stop                                 */
            uint32_t               : 1;
            __IOM uint32_t MSTPB8  : 1; /*!< [8..8] Temperature Sensor Module Stop                                     */
            __IOM uint32_t MSTPB9  : 1; /*!< [9..9] Event Link Controller Module Stop                                  */
            uint32_t               : 3;
            __IOM uint32_t MSTPB13 : 1; /*!< [13..13] Ethernet-Controller PTP Controller and Ethernet-Controller
                                         *   DMA Controller Module Stop Setting                                        */
            __IOM uint32_t MSTPB14 : 1; /*!< [14..14] Ethernet Controller, Ethernet Controller DMA Controller,
                                         *   and PHY Management Interface (Channel 1) Modules Stop                     */
            __IOM uint32_t MSTPB15 : 1; /*!< [15..15] Ethernet Controller, Ethernet Controller DMA Controller,
                                         *   and PHY Management Interface (Channel 0) Modules Stop                     */
            __IOM uint32_t MSTPB16 : 1; /*!< [16..16] Serial Peripheral Interface 1 Module Stop                        */
            __IOM uint32_t MSTPB17 : 1; /*!< [17..17] Serial Peripheral Interface 0 Module Stop                        */
            uint32_t               : 1;
            __IOM uint32_t MSTPB19 : 1; /*!< [19..19] Universal Serial Bus 2.0 FS Interface Module Stop                */
            __IOM uint32_t MSTPB20 : 1; /*!< [20..20] I2C Bus Interface 1 Module Stop                                  */
            __IOM uint32_t MSTPB21 : 1; /*!< [21..21] I2C Bus Interface 0 Module Stop                                  */
            __IOM uint32_t MSTPB22 : 1; /*!< [22..22] Parallel Data Capture Unit Module Stop                           */
            __IOM uint32_t MSTPB23 : 1; /*!< [23..23] CRC Calculator Module Stop                                       */
            __IOM uint32_t MSTPB24 : 1; /*!< [24..24] Serial Communication Interface 7 Module Stop                     */
            __IOM uint32_t MSTPB25 : 1; /*!< [25..25] Serial Communication Interface 6 Module Stop                     */
            __IOM uint32_t MSTPB26 : 1; /*!< [26..26] Serial Communication Interface 5 Module Stop                     */
            __IOM uint32_t MSTPB27 : 1; /*!< [27..27] Serial Communication Interface 4 Module Stop                     */
            __IOM uint32_t MSTPB28 : 1; /*!< [28..28] Serial Communication Interface 3 Module Stop                     */
            __IOM uint32_t MSTPB29 : 1; /*!< [29..29] Serial Communication Interface 2 Module Stop                     */
            __IOM uint32_t MSTPB30 : 1; /*!< [30..30] Serial Communication Interface 1 Module Stop                     */
            __IOM uint32_t MSTPB31 : 1; /*!< [31..31] Serial Communication Interface 0 Module Stop                     */
        } MSTPCRB_b;
    };

    union
    {
        __IOM uint32_t MSTPCRC;         /*!< (@ 0x00000018) Module Stop Control Register C                             */

        struct
        {
            __IOM uint32_t MSTPC0  : 1; /*!< [0..0] RAM Module Stop                                                    */
            uint32_t               : 1;
            __IOM uint32_t MSTPC2  : 1; /*!< [2..2] Expansion RAM Module Stop                                          */
            uint32_t               : 3;
            __IOM uint32_t MSTPC6  : 1; /*!< [6..6] ECCRAM Module Stop                                                 */
            __IOM uint32_t MSTPC7  : 1; /*!< [7..7] Standby RAM Module Stop                                            */
            uint32_t               : 9;
            __IOM uint32_t MSTPC17 : 1; /*!< [17..17] I2C Bus Interface 2 Module Stop                                  */
            uint32_t               : 1;
            __IOM uint32_t MSTPC19 : 1; /*!< [19..19] CAC Module Stop                                                  */
            uint32_t               : 2;
            __IOM uint32_t MSTPC22 : 1; /*!< [22..22] Serial Peripheral Interface 2 Module Stop                        */
            __IOM uint32_t MSTPC23 : 1; /*!< [23..23] Quad Serial Peripheral Interface Module Stop                     */
            __IOM uint32_t MSTPC24 : 1; /*!< [24..24] Serial Communications Interface 11 Module Stop                   */
            __IOM uint32_t MSTPC25 : 1; /*!< [25..25] Serial Communications Interface 10 Module Stop                   */
            __IOM uint32_t MSTPC26 : 1; /*!< [26..26] Serial Communications Interface 9 Module Stop                    */
            __IOM uint32_t MSTPC27 : 1; /*!< [27..27] Serial Communications Interface 8 Module Stop                    */
            __IOM uint32_t MSTPC28 : 1; /*!< [28..28] 2D drawing engine Module Stop                                    */
            __IOM uint32_t MSTPC29 : 1; /*!< [29..29] Graphic-LCD controller Module Stop                               */
            uint32_t               : 2;
        } MSTPCRC_b;
    };

    union
    {
        __IOM uint32_t MSTPCRD;         /*!< (@ 0x0000001C) Module Stop Control Register D                             */

        struct
        {
            __IOM uint32_t MSTPD0  : 1; /*!< [0..0] Module Stop D0                                                     */
            __IOM uint32_t MSTPD1  : 1; /*!< [1..1] Module Stop D1                                                     */
            __IOM uint32_t MSTPD2  : 1; /*!< [2..2] Module Stop D2                                                     */
            __IOM uint32_t MSTPD3  : 1; /*!< [3..3] Module Stop D3                                                     */
            __IOM uint32_t MSTPD4  : 1; /*!< [4..4] Module Stop D4                                                     */
            __IOM uint32_t MSTPD5  : 1; /*!< [5..5] Module Stop D5                                                     */
            __IOM uint32_t MSTPD6  : 1; /*!< [6..6] Module Stop D6                                                     */
            __IOM uint32_t MSTPD7  : 1; /*!< [7..7] Module Stop D7                                                     */
            uint32_t               : 6;
            __IOM uint32_t MSTPD14 : 1; /*!< [14..14] Extended Serial Sound Interface 1 Module Stop                    */
            __IOM uint32_t MSTPD15 : 1; /*!< [15..15] Extended Serial Sound Interface 0 Module Stop                    */
            uint32_t               : 3;
            __IOM uint32_t MSTPD19 : 1; /*!< [19..19] SD Host Interface Module Stop                                    */
            uint32_t               : 1;
            __IOM uint32_t MSTPD21 : 1; /*!< [21..21] MMC Host Interface Module Stop                                   */
            uint32_t               : 5;
            __IOM uint32_t MSTPD27 : 1; /*!< [27..27] Trusted Secure IP Module Stop                                    */
            uint32_t               : 4;
        } MSTPCRD_b;
    };

    union
    {
        __IOM uint32_t SCKCR;          /*!< (@ 0x00000020) System Clock Control Register                              */

        struct
        {
            __IOM uint32_t PCKD   : 4; /*!< [3..0] Peripheral Module Clock D (PCLKD) Select                           */
            __IOM uint32_t PCKC   : 4; /*!< [7..4] Peripheral Module Clock C (PCLKC) Select                           */
            __IOM uint32_t PCKB   : 4; /*!< [11..8] Peripheral Module Clock B (PCLKB) Select                          */
            __IOM uint32_t PCKA   : 4; /*!< [15..12] Peripheral Module Clock A (PCLKA) Select                         */
            __IOM uint32_t BCK    : 4; /*!< [19..16] External Bus Clock (BCLK) Select                                 */
            uint32_t              : 2;
            __IOM uint32_t PSTOP0 : 1; /*!< [22..22] SDCLK Pin Output Control                                         */
            __IOM uint32_t PSTOP1 : 1; /*!< [23..23] BCLK Pin Output Control                                          */
            __IOM uint32_t ICK    : 4; /*!< [27..24] System Clock (ICLK) Select                                       */
            __IOM uint32_t FCK    : 4; /*!< [31..28] Flash-IF Clock (FCLK) Select                                     */
        } SCKCR_b;
    };

    union
    {
        __IOM uint16_t SCKCR2;         /*!< (@ 0x00000024) System Clock Control Register 2                            */

        struct
        {
            uint16_t           : 4;
            __IOM uint16_t UCK : 4;    /*!< [7..4] USB Clock (UCLK) Select                                            */
            uint16_t           : 8;
        } SCKCR2_b;
    };

    union
    {
        __IOM uint16_t SCKCR3;         /*!< (@ 0x00000026) System Clock Control Register 3                            */

        struct
        {
            uint16_t             : 8;
            __IOM uint16_t CKSEL : 3;  /*!< [10..8] Clock Source Select                                               */
            uint16_t             : 5;
        } SCKCR3_b;
    };

    union
    {
        __IOM uint16_t PLLCR;             /*!< (@ 0x00000028) PLL Control Register                                       */

        struct
        {
            __IOM uint16_t PLIDIV    : 2; /*!< [1..0] PLL Input Frequency Division Ratio Select                          */
            uint16_t                 : 2;
            __IOM uint16_t PLLSRCSEL : 1; /*!< [4..4] PLL Clock Source Select                                            */
            uint16_t                 : 3;
            __IOM uint16_t STC       : 6; /*!< [13..8] Frequency Multiplication Factor Select                            */
            uint16_t                 : 2;
        } PLLCR_b;
    };

    union
    {
        __IOM uint8_t PLLCR2;          /*!< (@ 0x0000002A) PLL Control Register 2                                     */

        struct
        {
            __IOM uint8_t PLLEN : 1;   /*!< [0..0] PLL Stop Control                                                   */
            uint8_t             : 7;
        } PLLCR2_b;
    };
    __IM uint8_t  RESERVED3;
    __IM uint32_t RESERVED4;

    union
    {
        __IOM uint8_t BCKCR;           /*!< (@ 0x00000030) External Bus Clock Control Register                        */

        struct
        {
            __IOM uint8_t BCLKDIV : 1; /*!< [0..0] BCLK Pin Output Select                                             */
            uint8_t               : 7;
        } BCKCR_b;
    };
    __IM uint8_t RESERVED5;

    union
    {
        __IOM uint8_t MOSCCR;          /*!< (@ 0x00000032) Main Clock Oscillator Control Register                     */

        struct
        {
            __IOM uint8_t MOSTP : 1;   /*!< [0..0] Main Clock Oscillator Stop                                         */
            uint8_t             : 7;
        } MOSCCR_b;
    };

    union
    {
        __IOM uint8_t SOSCCR;          /*!< (@ 0x00000033) Sub-Clock Oscillator Control Register                      */

        struct
        {
            __IOM uint8_t SOSTP : 1;   /*!< [0..0] Sub-Clock Oscillator Stop                                          */
            uint8_t             : 7;
        } SOSCCR_b;
    };

    union
    {
        __IOM uint8_t LOCOCR;          /*!< (@ 0x00000034) Low-Speed On-Chip Oscillator Control Register              */

        struct
        {
            __IOM uint8_t LCSTP : 1;   /*!< [0..0] LOCO Stop                                                          */
            uint8_t             : 7;
        } LOCOCR_b;
    };

    union
    {
        __IOM uint8_t ILOCOCR;         /*!< (@ 0x00000035) IWDT-Dedicated On-Chip Oscillator Control Register         */

        struct
        {
            __IOM uint8_t ILCSTP : 1;  /*!< [0..0] IWDT-Dedicated On-Chip Oscillator Stop                             */
            uint8_t              : 7;
        } ILOCOCR_b;
    };

    union
    {
        __IOM uint8_t HOCOCR;          /*!< (@ 0x00000036) High-Speed On-Chip Oscillator Control Register             */

        struct
        {
            __IOM uint8_t HCSTP : 1;   /*!< [0..0] HOCO Stop                                                          */
            uint8_t             : 7;
        } HOCOCR_b;
    };

    union
    {
        __IOM uint8_t HOCOCR2;         /*!< (@ 0x00000037) High-Speed On-Chip Oscillator Control Register
                                        *                  2                                                          */

        struct
        {
            __IOM uint8_t HCFRQ : 2;   /*!< [1..0] HOCO Frequency Setting                                             */
            uint8_t             : 6;
        } HOCOCR2_b;
    };
    __IM uint32_t RESERVED6;

    union
    {
        __IOM uint8_t OSCOVFSR;        /*!< (@ 0x0000003C) Oscillation Stabilization Flag Register                    */

        struct
        {
            __IM uint8_t MOOVF  : 1;   /*!< [0..0] Main Clock Oscillation Stabilization Flag                          */
            __IM uint8_t SOOVF  : 1;   /*!< [1..1] Sub-Clock Oscillation Stabilization Flag                           */
            __IM uint8_t PLOVF  : 1;   /*!< [2..2] PLL Clock Oscillation Stabilization Flag                           */
            __IM uint8_t HCOVF  : 1;   /*!< [3..3] HOCO Clock Oscillation Stabilization Flag                          */
            __IM uint8_t ILCOVF : 1;   /*!< [4..4] IWDT-Dedicated Clock Oscillation Stabilization Flag                */
            __IM uint8_t PPLOVF : 1;   /*!< [5..5] PPLL Clock Oscillation Stabilization Flag                          */
            uint8_t             : 2;
        } OSCOVFSR_b;
    };
    __IM uint8_t RESERVED7;

    union
    {
        __IOM uint16_t CKOCR;          /*!< (@ 0x0000003E) CLKOUT Output Control Register                             */

        struct
        {
            uint16_t              : 8;
            __IOM uint16_t CKOSEL : 3; /*!< [10..8] CLKOUT Output Source Select                                       */
            uint16_t              : 1;
            __IOM uint16_t CKODIV : 3; /*!< [14..12] CLKOUT Output Divisor Select                                     */
            __IOM uint16_t CKOSTP : 1; /*!< [15..15] CLKOUT Output Stop Control                                       */
        } CKOCR_b;
    };

    union
    {
        __IOM uint8_t OSTDCR;          /*!< (@ 0x00000040) Oscillation Stop Detection Control Register                */

        struct
        {
            __IOM uint8_t OSTDIE : 1;  /*!< [0..0] Oscillation Stop Detection Interrupt Enable                        */
            uint8_t              : 6;
            __IOM uint8_t OSTDE  : 1;  /*!< [7..7] Oscillation Stop Detection Function Enable                         */
        } OSTDCR_b;
    };

    union
    {
        __IOM uint8_t OSTDSR;          /*!< (@ 0x00000041) Oscillation Stop Detection Status Register                 */

        struct
        {
            __IOM uint8_t OSTDF : 1;   /*!< [0..0] Oscillation Stop Detection Flag                                    */
            uint8_t             : 7;
        } OSTDSR_b;
    };
    __IM uint16_t RESERVED8;

    union
    {
        __IOM uint16_t PACKCR;         /*!< (@ 0x00000044) Specific-Use Clock Control Register                        */

        struct
        {
            uint16_t                : 4;
            __IOM uint16_t OUTCKSEL : 1; /*!< [4..4] Ethernet-PHY External Clock (CLKOUT25M) Source Select              */
            uint16_t                : 7;
            __IOM uint16_t UPLLSEL  : 1; /*!< [12..12] USB Clock (UCLK) Source Select                                   */
            uint16_t                : 3;
        } PACKCR_b;
    };
    __IM uint16_t RESERVED9;

    union
    {
        __IOM uint16_t PPLLCR;          /*!< (@ 0x00000048) PPLL Control Register                                      */

        struct
        {
            __IOM uint16_t PPLIDIV : 2; /*!< [1..0] PPLL Input Pulse Frequency Division Ratio Select                   */
            uint16_t               : 6;
            __IOM uint16_t PPLSTC  : 6; /*!< [13..8] PPLL Frequency Multiplier Setting                                 */
            uint16_t               : 2;
        } PPLLCR_b;
    };

    union
    {
        __IOM uint8_t PPLLCR2;         /*!< (@ 0x0000004A) PPLL Control Register 2                                    */

        struct
        {
            __IOM uint8_t PPLLEN : 1;  /*!< [0..0] PPLL Stop Control                                                  */
            uint8_t              : 7;
        } PPLLCR2_b;
    };

    union
    {
        __IOM uint8_t PPLLCR3;         /*!< (@ 0x0000004B) PPLL Control Register 3                                    */

        struct
        {
            __IOM uint8_t PPLCK : 4;   /*!< [3..0] PPLL Frequency-Divided Clock (PPLLDIVCLK) Select                   */
            uint8_t             : 4;
        } PPLLCR3_b;
    };
    __IM uint32_t RESERVED10[21];

    union
    {
        __IOM uint8_t OPCCR;           /*!< (@ 0x000000A0) Operating Power Control Register                           */

        struct
        {
            __IOM uint8_t OPCM    : 3; /*!< [2..0] Operating Power Control Mode Select                                */
            uint8_t               : 1;
            __IOM uint8_t OPCMTSF : 1; /*!< [4..4] Operating Power Control Mode Transition Status Flag                */
            uint8_t               : 3;
        } OPCCR_b;
    };

    union
    {
        __IOM uint8_t RSTCKCR;          /*!< (@ 0x000000A1) Sleep Mode Return Clock Source Switching Register          */

        struct
        {
            __IOM uint8_t RSTCKSEL : 3; /*!< [2..0] Sleep Mode Return Clock Source Select                              */
            uint8_t                : 4;
            __IOM uint8_t RSTCKEN  : 1; /*!< [7..7] Sleep Mode Return Clock Source Switching Enable                    */
        } RSTCKCR_b;
    };

    union
    {
        __IOM uint8_t MOSCWTCR;        /*!< (@ 0x000000A2) Main Clock Oscillator Wait Control Register                */

        struct
        {
            __IOM uint8_t MSTS : 8;    /*!< [7..0] Control the waiting time until output of the signal from
                                        *   the main clock oscillator to the internal circuits starts                 */
        } MOSCWTCR_b;
    };

    union
    {
        __IOM uint8_t SOSCWTCR;        /*!< (@ 0x000000A3) Sub-Clock Oscillator Wait Control Register                 */

        struct
        {
            __IOM uint8_t SSTS : 8;    /*!< [7..0] control the waiting time until output of the signal from
                                        *   the sub-clock oscillator to the internal circuits starts                  */
        } SOSCWTCR_b;
    };
    __IM uint32_t RESERVED11[7];

    union
    {
        __IOM uint8_t RSTSR2;          /*!< (@ 0x000000C0) Reset Status Register 2                                    */

        struct
        {
            __IOM uint8_t IWDTRF : 1;  /*!< [0..0] Independent Watchdog Timer Reset Detect Flag                       */
            __IOM uint8_t WDTRF  : 1;  /*!< [1..1] Watchdog Timer Reset Detect Flag                                   */
            __IOM uint8_t SWRF   : 1;  /*!< [2..2] Software Reset Detect Flag                                         */
            uint8_t              : 5;
        } RSTSR2_b;
    };
    __IM uint8_t RESERVED12;

    union
    {
        __IOM uint16_t SWRR;           /*!< (@ 0x000000C2) Software Reset Register                                    */

        struct
        {
            __IOM uint16_t SWRR : 16;  /*!< [15..0] Software Reset                                                    */
        } SWRR_b;
    };
    __IM uint32_t RESERVED13[7];

    union
    {
        __IOM uint8_t LVD1CR1;            /*!< (@ 0x000000E0) Voltage Monitoring 1 Circuit Control Register
                                           *                  1                                                          */

        struct
        {
            __IOM uint8_t LVD1IDTSEL : 2; /*!< [1..0] Voltage Monitoring 1 Interrupt Generation Condition Select         */
            __IOM uint8_t LVD1IRQSEL : 1; /*!< [2..2] Voltage Monitoring 1 Interrupt Type Select                         */
            uint8_t                  : 5;
        } LVD1CR1_b;
    };

    union
    {
        __IOM uint8_t LVD1SR;          /*!< (@ 0x000000E1) Voltage Monitoring 1 Circuit Status Register               */

        struct
        {
            __IOM uint8_t LVD1DET : 1; /*!< [0..0] Voltage Monitoring 1 Voltage Change Detection Flag                 */
            __IM uint8_t  LVD1MON : 1; /*!< [1..1] Voltage Monitoring 1 Signal Monitor Flag                           */
            uint8_t               : 6;
        } LVD1SR_b;
    };

    union
    {
        __IOM uint8_t LVD2CR1;            /*!< (@ 0x000000E2) Voltage Monitoring 2 Circuit Control Register
                                           *                  1                                                          */

        struct
        {
            __IOM uint8_t LVD2IDTSEL : 2; /*!< [1..0] Voltage Monitoring 2 Interrupt Generation Condition Select         */
            __IOM uint8_t LVD2IRQSEL : 1; /*!< [2..2] Voltage Monitoring 2 Interrupt Type Select                         */
            uint8_t                  : 5;
        } LVD2CR1_b;
    };

    union
    {
        __IOM uint8_t LVD2SR;          /*!< (@ 0x000000E3) Voltage Monitoring 2 Circuit Status Register               */

        struct
        {
            __IOM uint8_t LVD2DET : 1; /*!< [0..0] Voltage Monitoring 2 Voltage Change Detection Flag                 */
            __IM uint8_t  LVD2MON : 1; /*!< [1..1] Voltage Monitoring 2 Signal Monitor Flag                           */
            uint8_t               : 6;
        } LVD2SR_b;
    };
    __IM uint32_t RESERVED14[198];
    __IM uint16_t RESERVED15;

    union
    {
        __IOM uint16_t PRCR;           /*!< (@ 0x000003FE) Protect Register                                           */

        struct
        {
            __IOM uint16_t PRC0  : 1;  /*!< [0..0] Protect Bit 0                                                      */
            __IOM uint16_t PRC1  : 1;  /*!< [1..1] Protect Bit 1                                                      */
            uint16_t             : 1;
            __IOM uint16_t PRC3  : 1;  /*!< [3..3] Protect Bit 3                                                      */
            uint16_t             : 4;
            __IOM uint16_t PRKEY : 8;  /*!< [15..8] PRC Key Code                                                      */
        } PRCR_b;
    };
    __IM uint32_t RESERVED16[775];

    union
    {
        __IOM uint8_t MEMWAIT;         /*!< (@ 0x0000101C) Memory Wait Cycle Setting Register                         */

        struct
        {
            __IOM uint8_t MEMWAIT : 1; /*!< [0..0] Memory Wait Cycle Setting                                          */
            uint8_t               : 7;
        } MEMWAIT_b;
    };
    __IM uint8_t  RESERVED17;
    __IM uint16_t RESERVED18;
    __IM uint32_t RESERVED19[11416];

    union
    {
        __IOM uint8_t DPSBYCR;         /*!< (@ 0x0000C280) Deep Standby Control Register                              */

        struct
        {
            __IOM uint8_t DEEPCUT : 2; /*!< [1..0] Deep Cut                                                           */
            uint8_t               : 4;
            __IOM uint8_t IOKEEP  : 1; /*!< [6..6] I/O Port Retention                                                 */
            __IOM uint8_t DPSBY   : 1; /*!< [7..7] Deep Software Standby                                              */
        } DPSBYCR_b;
    };
    __IM uint8_t RESERVED20;

    union
    {
        __IOM uint8_t DPSIER0;         /*!< (@ 0x0000C282) Deep Standby Interrupt Enable Register 0                   */

        struct
        {
            __IOM uint8_t DIRQ0E : 1;  /*!< [0..0] IRQ0-DS Pin Enable                                                 */
            __IOM uint8_t DIRQ1E : 1;  /*!< [1..1] IRQ1-DS Pin Enable                                                 */
            __IOM uint8_t DIRQ2E : 1;  /*!< [2..2] IRQ2-DS Pin Enable                                                 */
            __IOM uint8_t DIRQ3E : 1;  /*!< [3..3] IRQ3-DS Pin Enable                                                 */
            __IOM uint8_t DIRQ4E : 1;  /*!< [4..4] IRQ4-DS Pin Enable                                                 */
            __IOM uint8_t DIRQ5E : 1;  /*!< [5..5] IRQ5-DS Pin Enable                                                 */
            __IOM uint8_t DIRQ6E : 1;  /*!< [6..6] IRQ6-DS Pin Enable                                                 */
            __IOM uint8_t DIRQ7E : 1;  /*!< [7..7] IRQ7-DS Pin Enable                                                 */
        } DPSIER0_b;
    };

    union
    {
        __IOM uint8_t DPSIER1;         /*!< (@ 0x0000C283) Deep Standby Interrupt Enable Register 1                   */

        struct
        {
            __IOM uint8_t DIRQ8E  : 1; /*!< [0..0] IRQ8-DS Pin Enable                                                 */
            __IOM uint8_t DIRQ9E  : 1; /*!< [1..1] IRQ9-DS Pin Enable                                                 */
            __IOM uint8_t DIRQ10E : 1; /*!< [2..2] IRQ10-DS Pin Enable                                                */
            __IOM uint8_t DIRQ11E : 1; /*!< [3..3] IRQ11-DS Pin Enable                                                */
            __IOM uint8_t DIRQ12E : 1; /*!< [4..4] IRQ12-DS Pin Enable                                                */
            __IOM uint8_t DIRQ13E : 1; /*!< [5..5] IRQ13-DS Pin Enable                                                */
            __IOM uint8_t DIRQ14E : 1; /*!< [6..6] IRQ14-DS Pin Enable                                                */
            __IOM uint8_t DIRQ15E : 1; /*!< [7..7] IRQ15-DS Pin Enable                                                */
        } DPSIER1_b;
    };

    union
    {
        __IOM uint8_t DPSIER2;          /*!< (@ 0x0000C284) Deep Standby Interrupt Enable Register 2                   */

        struct
        {
            __IOM uint8_t DLVD1IE  : 1; /*!< [0..0] LVD1 Deep Standby Release Signal Enable                            */
            __IOM uint8_t DLVD2IE  : 1; /*!< [1..1] LVD2 Deep Standby Release Signal Enable                            */
            __IOM uint8_t DRTCIIE  : 1; /*!< [2..2] RTC Periodic Interrupt Deep Standby Release Signal Enable          */
            __IOM uint8_t DRTCAIE  : 1; /*!< [3..3] RTC Alarm Interrupt Deep Standby Release Signal Enable             */
            __IOM uint8_t DNMIE    : 1; /*!< [4..4] NMI Pin Enable                                                     */
            __IOM uint8_t DRIICDIE : 1; /*!< [5..5] SDA2-DS Deep Standby Release Signal Enable                         */
            __IOM uint8_t DRIICCIE : 1; /*!< [6..6] SCL2-DS Deep Standby Release Signal Enable                         */
            __IOM uint8_t DUSBIE   : 1; /*!< [7..7] USB Suspend/Resume Deep Standby Release Signal Enable              */
        } DPSIER2_b;
    };

    union
    {
        __IOM uint8_t DPSIER3;         /*!< (@ 0x0000C285) Deep Standby Interrupt Enable Register 3                   */

        struct
        {
            __IOM uint8_t DCANIE : 1;  /*!< [0..0] CRX1-DS Deep Standby Release Signal Enable                         */
            uint8_t              : 7;
        } DPSIER3_b;
    };

    union
    {
        __IOM uint8_t DPSIFR0;         /*!< (@ 0x0000C286) Deep Standby Interrupt Flag Register 0                     */

        struct
        {
            __IOM uint8_t DIRQ0F : 1;  /*!< [0..0] IRQ0-DS Pin Deep Standby Release Flag                              */
            __IOM uint8_t DIRQ1F : 1;  /*!< [1..1] IRQ1-DS Pin Deep Standby Release Flag                              */
            __IOM uint8_t DIRQ2F : 1;  /*!< [2..2] IRQ2-DS Pin Deep Standby Release Flag                              */
            __IOM uint8_t DIRQ3F : 1;  /*!< [3..3] IRQ3-DS Pin Deep Standby Release Flag                              */
            __IOM uint8_t DIRQ4F : 1;  /*!< [4..4] IRQ4-DS Pin Deep Standby Release Flag                              */
            __IOM uint8_t DIRQ5F : 1;  /*!< [5..5] IRQ5-DS Pin Deep Standby Release Flag                              */
            __IOM uint8_t DIRQ6F : 1;  /*!< [6..6] IRQ6-DS Pin Deep Standby Release Flag                              */
            __IOM uint8_t DIRQ7F : 1;  /*!< [7..7] IRQ7-DS Pin Deep Standby Release Flag                              */
        } DPSIFR0_b;
    };

    union
    {
        __IOM uint8_t DPSIFR1;         /*!< (@ 0x0000C287) Deep Standby Interrupt Flag Register 1                     */

        struct
        {
            __IOM uint8_t DIRQ8F  : 1; /*!< [0..0] IRQ8-DS Pin Deep Standby Release Flag                              */
            __IOM uint8_t DIRQ9F  : 1; /*!< [1..1] IRQ9-DS Pin Deep Standby Release Flag                              */
            __IOM uint8_t DIRQ10F : 1; /*!< [2..2] IRQ10-DS Pin Deep Standby Release Flag                             */
            __IOM uint8_t DIRQ11F : 1; /*!< [3..3] IRQ11-DS Pin Deep Standby Release Flag                             */
            __IOM uint8_t DIRQ12F : 1; /*!< [4..4] IRQ12-DS Pin Deep Standby Release Flag                             */
            __IOM uint8_t DIRQ13F : 1; /*!< [5..5] IRQ13-DS Pin Deep Standby Release Flag                             */
            __IOM uint8_t DIRQ14F : 1; /*!< [6..6] IRQ14-DS Pin Deep Standby Release Flag                             */
            __IOM uint8_t DIRQ15F : 1; /*!< [7..7] IRQ15-DS Pin Deep Standby Release Flag                             */
        } DPSIFR1_b;
    };

    union
    {
        __IOM uint8_t DPSIFR2;          /*!< (@ 0x0000C288) Deep Standby Interrupt Flag Register 2                     */

        struct
        {
            __IOM uint8_t DLVD1IF  : 1; /*!< [0..0] LVD1 Deep Standby Release Flag                                     */
            __IOM uint8_t DLVD2IF  : 1; /*!< [1..1] LVD2 Deep Standby Release Flag                                     */
            __IOM uint8_t DRTCIIF  : 1; /*!< [2..2] RTC Periodic Interrupt Deep Standby Release Flag                   */
            __IOM uint8_t DRTCAIF  : 1; /*!< [3..3] RTC Alarm Interrupt Deep Standby Release Flag                      */
            __IOM uint8_t DNMIF    : 1; /*!< [4..4] NMI Deep Standby Release Flag                                      */
            __IOM uint8_t DRIICDIF : 1; /*!< [5..5] SDA2-DS Deep Standby Release Flag                                  */
            __IOM uint8_t DRIICCIF : 1; /*!< [6..6] SCL2-DS Deep Standby Release Flag                                  */
            __IOM uint8_t DUSBIF   : 1; /*!< [7..7] USB Suspend/Resume Deep Standby Release Flag                       */
        } DPSIFR2_b;
    };

    union
    {
        __IOM uint8_t DPSIFR3;         /*!< (@ 0x0000C289) Deep Standby Interrupt Flag Register 3                     */

        struct
        {
            __IOM uint8_t DCANIF : 1;  /*!< [0..0] CRX1-DS Deep Standby Release Flag                                  */
            uint8_t              : 7;
        } DPSIFR3_b;
    };

    union
    {
        __IOM uint8_t DPSIEGR0;        /*!< (@ 0x0000C28A) Deep Standby Interrupt Edge Register 0                     */

        struct
        {
            __IOM uint8_t DIRQ0EG : 1; /*!< [0..0] IRQ0-DS Pin Edge Select                                            */
            __IOM uint8_t DIRQ1EG : 1; /*!< [1..1] IRQ1-DS Pin Edge Select                                            */
            __IOM uint8_t DIRQ2EG : 1; /*!< [2..2] IRQ2-DS Pin Edge Select                                            */
            __IOM uint8_t DIRQ3EG : 1; /*!< [3..3] IRQ3-DS Pin Edge Select                                            */
            __IOM uint8_t DIRQ4EG : 1; /*!< [4..4] IRQ4-DS Pin Edge Select                                            */
            __IOM uint8_t DIRQ5EG : 1; /*!< [5..5] IRQ5-DS Pin Edge Select                                            */
            __IOM uint8_t DIRQ6EG : 1; /*!< [6..6] IRQ6-DS Pin Edge Select                                            */
            __IOM uint8_t DIRQ7EG : 1; /*!< [7..7] IRQ7-DS Pin Edge Select                                            */
        } DPSIEGR0_b;
    };

    union
    {
        __IOM uint8_t DPSIEGR1;         /*!< (@ 0x0000C28B) Deep Standby Interrupt Edge Register 1                     */

        struct
        {
            __IOM uint8_t DIRQ8EG  : 1; /*!< [0..0] IRQ8-DS Pin Edge Select                                            */
            __IOM uint8_t DIRQ9EG  : 1; /*!< [1..1] IRQ9-DS Pin Edge Select                                            */
            __IOM uint8_t DIRQ10EG : 1; /*!< [2..2] IRQ10-DS Pin Edge Select                                           */
            __IOM uint8_t DIRQ11EG : 1; /*!< [3..3] IRQ11-DS Pin Edge Select                                           */
            __IOM uint8_t DIRQ12EG : 1; /*!< [4..4] IRQ12-DS Pin Edge Select                                           */
            __IOM uint8_t DIRQ13EG : 1; /*!< [5..5] IRQ13-DS Pin Edge Select                                           */
            __IOM uint8_t DIRQ14EG : 1; /*!< [6..6] IRQ14-DS Pin Edge Select                                           */
            __IOM uint8_t DIRQ15EG : 1; /*!< [7..7] IRQ15-DS Pin Edge Select                                           */
        } DPSIEGR1_b;
    };

    union
    {
        __IOM uint8_t DPSIEGR2;         /*!< (@ 0x0000C28C) Deep Standby Interrupt Edge Register 2                     */

        struct
        {
            __IOM uint8_t DLVD1EG  : 1; /*!< [0..0] LVD1 Edge Select                                                   */
            __IOM uint8_t DLVD2EG  : 1; /*!< [1..1] LVD2 Edge Select                                                   */
            uint8_t                : 2;
            __IOM uint8_t DNMIEG   : 1; /*!< [4..4] NMI Edge Select                                                    */
            __IOM uint8_t DRIICDEG : 1; /*!< [5..5] SDA2-DS Edge Select                                                */
            __IOM uint8_t DRIICCEG : 1; /*!< [6..6] SCL2-DS Edge Select                                                */
            uint8_t                : 1;
        } DPSIEGR2_b;
    };

    union
    {
        __IOM uint8_t DPSIEGR3;        /*!< (@ 0x0000C28D) Deep Standby Interrupt Edge Register 3                     */

        struct
        {
            __IOM uint8_t DCANIEG : 1; /*!< [0..0] CRX1-DS Edge Select                                                */
            uint8_t               : 7;
        } DPSIEGR3_b;
    };
    __IM uint16_t RESERVED21;

    union
    {
        __IOM uint8_t RSTSR0;          /*!< (@ 0x0000C290) Reset Status Register 0                                    */

        struct
        {
            __IOM uint8_t PORF    : 1; /*!< [0..0] Power-On Reset Detect Flag                                         */
            __IOM uint8_t LVD0RF  : 1; /*!< [1..1] Voltage-Monitoring 0 Reset Detect Flag                             */
            __IOM uint8_t LVD1RF  : 1; /*!< [2..2] Voltage-Monitoring 1 Reset Detect Flag                             */
            __IOM uint8_t LVD2RF  : 1; /*!< [3..3] Voltage-Monitoring 2 Reset Detect Flag                             */
            uint8_t               : 3;
            __IOM uint8_t DPSRSTF : 1; /*!< [7..7] Deep Software Standby Reset Flag                                   */
        } RSTSR0_b;
    };

    union
    {
        __IOM uint8_t RSTSR1;          /*!< (@ 0x0000C291) Reset Status Register 1                                    */

        struct
        {
            __IOM uint8_t CWSF : 1;    /*!< [0..0] Cold/Warm Start Determination Flag                                 */
            uint8_t            : 7;
        } RSTSR1_b;
    };
    __IM uint8_t RESERVED22;

    union
    {
        __IOM uint8_t MOFCR;           /*!< (@ 0x0000C293) Main Clock Oscillator Forced Oscillation Control
                                        *                  Register                                                   */

        struct
        {
            __IOM uint8_t MOFXIN : 1;  /*!< [0..0] Main Clock Oscillator Forced Oscillation                           */
            uint8_t              : 3;
            __IOM uint8_t MODRV2 : 2;  /*!< [5..4] Main Clock Oscillator Driving Ability 2 Switching                  */
            __IOM uint8_t MOSEL  : 1;  /*!< [6..6] Main Clock Oscillator Switching                                    */
            uint8_t              : 1;
        } MOFCR_b;
    };

    union
    {
        __IOM uint8_t HOCOPCR;          /*!< (@ 0x0000C294) High-Speed On-Chip Oscillator Power Supply Control
                                         *                  Register                                                   */

        struct
        {
            __IOM uint8_t HOCOPCNT : 1; /*!< [0..0] High-Speed On-Chip Oscillator Power Supply Control                 */
            uint8_t                : 7;
        } HOCOPCR_b;
    };
    __IM uint8_t RESERVED23;

    union
    {
        __IOM uint8_t FWEPROR;         /*!< (@ 0x0000C296) Flash P/E Protect Register                                    */

        struct
        {
            __IOM uint8_t FLWE : 2;    /*!< [1..0] Flash Programming and Erasure Enabling                                */
            uint8_t            : 6;
        } FWEPROR_b;
    };

    union
    {
        __IOM uint8_t LVCMPCR;         /*!< (@ 0x0000C297) Voltage Monitoring Circuit Control Register                */

        struct
        {
            uint8_t             : 5;
            __IOM uint8_t LVD1E : 1;   /*!< [5..5] Voltage Detection 1 Enable                                         */
            __IOM uint8_t LVD2E : 1;   /*!< [6..6] Voltage Detection 2 Enable                                         */
            uint8_t             : 1;
        } LVCMPCR_b;
    };

    union
    {
        __IOM uint8_t LVDLVLR;         /*!< (@ 0x0000C298) Voltage Detection Level Select Register                    */

        struct
        {
            __IOM uint8_t LVD1LVL : 4; /*!< [3..0] Voltage Detection 1 Level Select (Standard voltage during
                                        *   drop in voltage)                                                          */
            __IOM uint8_t LVD2LVL : 4; /*!< [7..4] Voltage Detection 2 Level Select (Standard voltage during
                                        *   drop in voltage)                                                          */
        } LVDLVLR_b;
    };
    __IM uint8_t RESERVED24;

    union
    {
        __IOM uint8_t LVD1CR0;           /*!< (@ 0x0000C29A) Voltage Monitoring 1 Circuit Control Register
                                          *                  0                                                          */

        struct
        {
            __IOM uint8_t LVD1RIE   : 1; /*!< [0..0] Voltage Monitoring 1 Interrupt/Reset Enable                        */
            __IOM uint8_t LVD1DFDIS : 1; /*!< [1..1] Voltage Monitoring 1 Digital Filter Disable Mode Select            */
            __IOM uint8_t LVD1CMPE  : 1; /*!< [2..2] Voltage Monitoring 1 Circuit Comparison Result Output
                                          *   Enable                                                                    */
            uint8_t                 : 1;
            __IOM uint8_t LVD1FSAMP : 2; /*!< [5..4] Sampling Clock Select                                              */
            __IOM uint8_t LVD1RI    : 1; /*!< [6..6] Voltage Monitoring 1 Circuit Mode Select                           */
            __IOM uint8_t LVD1RN    : 1; /*!< [7..7] Voltage Monitoring 1 Reset Negate Select                           */
        } LVD1CR0_b;
    };

    union
    {
        __IOM uint8_t LVD2CR0;             /*!< (@ 0x0000C29B) Voltage Monitoring 2 Circuit Control Register
                                            *                  0                                                          */

        struct
        {
            __IOM uint8_t LVD2RIE   : 1;   /*!< [0..0] Voltage Monitoring 2 Interrupt/Reset Enable                        */
            __IOM uint8_t LVD2DFDIS : 1;   /*!< [1..1] Voltage Monitoring 2 Digital Filter Disable Mode Select            */
            __IOM uint8_t LVD2CMPE  : 1;   /*!< [2..2] Voltage Monitoring 2 Circuit Comparison Result Output
                                            *   Enable                                                                    */
            uint8_t                 : 1;
            __IOM uint8_t LVD2FSAMP : 2;   /*!< [5..4] Sampling Clock Select                                              */
            __IOM uint8_t LVD2RI    : 1;   /*!< [6..6] Voltage Monitoring 2 Circuit Mode Select                           */
            __IOM uint8_t LVD2RN    : 1;   /*!< [7..7] Voltage Monitoring 2 Reset Negate Select                           */
        } LVD2CR0_b;
    };
    __IM uint32_t              RESERVED25;
    __IOM R_SYSTEM_DPSBKR_Type DPSBKR[32]; /*!< (@ 0x0000C2A0) Deep Standby Backup Register                               */
} R_SYSTEM_Type;                           /*!< Size = 49856 (0xc2c0)                                                  */

#endif /* R_SYSTEM_REG_H */
