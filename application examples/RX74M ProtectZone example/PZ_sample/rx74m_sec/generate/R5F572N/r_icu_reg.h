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

#ifndef R_ICU_REG_H
#define R_ICU_REG_H

/* ================                              Device Specific Cluster Section                              ================ */

/** @addtogroup Device_Peripheral_clusters
 * @{
 */

/**
 * @brief R_ICU_IR [IR] (Interrupt Request Register)
 */
typedef struct
{
    union
    {
        __IOM uint8_t IR;              /*!< (@ 0x00000000) Interrupt Request Register                                 */

        struct
        {
            __IOM uint8_t IR : 1;      /*!< [0..0] Interrupt Status Flag                                              */
            uint8_t          : 7;
        } IR_b;
    };
} R_ICU_IR_Type;                       /*!< Size = 1 (0x1)                                                            */

/**
 * @brief R_ICU_DTCER [DTCER] (DTC Transfer Request Enable Register)
 */
typedef struct
{
    union
    {
        __IOM uint8_t DTCER;           /*!< (@ 0x00000000) DTC Transfer Request Enable Register                       */

        struct
        {
            __IOM uint8_t DTCE : 1;    /*!< [0..0] DTC Transfer Request Enable                                        */
            uint8_t            : 7;
        } DTCER_b;
    };
} R_ICU_DTCER_Type;                    /*!< Size = 1 (0x1)                                                            */

/**
 * @brief R_ICU_IER [IER] (Interrupt Request Enable Register)
 */
typedef struct
{
    union
    {
        __IOM uint8_t IER;             /*!< (@ 0x00000000) Interrupt Request Enable Register                          */

        struct
        {
            __IOM uint8_t IEN0 : 1;    /*!< [0..0] Interrupt Request Enable 0                                         */
            __IOM uint8_t IEN1 : 1;    /*!< [1..1] Interrupt Request Enable 1                                         */
            __IOM uint8_t IEN2 : 1;    /*!< [2..2] Interrupt Request Enable 2                                         */
            __IOM uint8_t IEN3 : 1;    /*!< [3..3] Interrupt Request Enable 3                                         */
            __IOM uint8_t IEN4 : 1;    /*!< [4..4] Interrupt Request Enable 4                                         */
            __IOM uint8_t IEN5 : 1;    /*!< [5..5] Interrupt Request Enable 5                                         */
            __IOM uint8_t IEN6 : 1;    /*!< [6..6] Interrupt Request Enable 6                                         */
            __IOM uint8_t IEN7 : 1;    /*!< [7..7] Interrupt Request Enable 7                                         */
        } IER_b;
    };
} R_ICU_IER_Type;                      /*!< Size = 1 (0x1)                                                            */

/**
 * @brief R_ICU_IPR [IPR] (Interrupt Source Priority Register)
 */
typedef struct
{
    union
    {
        __IOM uint8_t IPR;             /*!< (@ 0x00000000) Interrupt Source Priority Register                         */

        struct
        {
            __IOM uint8_t IPR : 4;     /*!< [3..0] Interrupt Priority Level Select                                    */
            uint8_t           : 4;
        } IPR_b;
    };
} R_ICU_IPR_Type;                      /*!< Size = 1 (0x1)                                                            */

/**
 * @brief R_ICU_DMRSR [DMRSR] (DMAC Trigger Select Register)
 */
typedef struct
{
    union
    {
        __IOM uint8_t DMRSR;           /*!< (@ 0x00000000) DMAC Trigger Select Register                               */

        struct
        {
            __IOM uint8_t DMRSR : 8;   /*!< [7..0] These bits set the interrupt vector number of the interrupt
                                        *   source as the DMAC trigger.                                               */
        } DMRSR_b;
    };
    __IM uint8_t RESERVED[3];
} R_ICU_DMRSR_Type;                    /*!< Size = 4 (0x4)                                                            */

/**
 * @brief R_ICU_IRQCR [IRQCR] (IRQ Control Register)
 */
typedef struct
{
    union
    {
        __IOM uint8_t IRQCR;           /*!< (@ 0x00000000) IRQ Control Register                                       */

        struct
        {
            uint8_t             : 2;
            __IOM uint8_t IRQMD : 2;   /*!< [3..2] IRQ Detection Select                                               */
            uint8_t             : 4;
        } IRQCR_b;
    };
} R_ICU_IRQCR_Type;                    /*!< Size = 1 (0x1)                                                            */

/**
 * @brief R_ICU_IRQFLTE [IRQFLTE] (IRQ Pin Digital Filter Enable Register)
 */
typedef struct
{
    union
    {
        __IOM uint8_t IRQFLTE;         /*!< (@ 0x00000000) IRQ Pin Digital Filter Enable Register                     */

        struct
        {
            __IOM uint8_t FLTEN0 : 1;  /*!< [0..0] IRQ0 Digital Filter Enable                                         */
            __IOM uint8_t FLTEN1 : 1;  /*!< [1..1] IRQ1 Digital Filter Enable                                         */
            __IOM uint8_t FLTEN2 : 1;  /*!< [2..2] IRQ2 Digital Filter Enable                                         */
            __IOM uint8_t FLTEN3 : 1;  /*!< [3..3] IRQ3 Digital Filter Enable                                         */
            __IOM uint8_t FLTEN4 : 1;  /*!< [4..4] IRQ4 Digital Filter Enable                                         */
            __IOM uint8_t FLTEN5 : 1;  /*!< [5..5] IRQ5 Digital Filter Enable                                         */
            __IOM uint8_t FLTEN6 : 1;  /*!< [6..6] IRQ6 Digital Filter Enable                                         */
            __IOM uint8_t FLTEN7 : 1;  /*!< [7..7] IRQ7 Digital Filter Enable                                         */
        } IRQFLTE_b;
    };
} R_ICU_IRQFLTE_Type;                  /*!< Size = 1 (0x1)                                                            */

/**
 * @brief R_ICU_IRQFLTC [IRQFLTC] (IRQ Pin Digital Filter Setting Register)
 */
typedef struct
{
    union
    {
        __IOM uint16_t IRQFLTC;          /*!< (@ 0x00000000) IRQ Pin Digital Filter Setting Register                    */

        struct
        {
            __IOM uint16_t FCLKSEL0 : 2; /*!< [1..0] IRQ0 Digital Filter Sampling Clock                                 */
            __IOM uint16_t FCLKSEL1 : 2; /*!< [3..2] IRQ1 Digital Filter Sampling Clock                                 */
            __IOM uint16_t FCLKSEL2 : 2; /*!< [5..4] IRQ2 Digital Filter Sampling Clock                                 */
            __IOM uint16_t FCLKSEL3 : 2; /*!< [7..6] IRQ3 Digital Filter Sampling Clock                                 */
            __IOM uint16_t FCLKSEL4 : 2; /*!< [9..8] IRQ4 Digital Filter Sampling Clock                                 */
            __IOM uint16_t FCLKSEL5 : 2; /*!< [11..10] IRQ5 Digital Filter Sampling Clock                               */
            __IOM uint16_t FCLKSEL6 : 2; /*!< [13..12] IRQ6 Digital Filter Sampling Clock                               */
            __IOM uint16_t FCLKSEL7 : 2; /*!< [15..14] IRQ7 Digital Filter Sampling Clock                               */
        } IRQFLTC_b;
    };
} R_ICU_IRQFLTC_Type;                    /*!< Size = 2 (0x2)                                                            */

/**
 * @brief R_ICU_PIBR [PIBR] (Software Configurable Interrupt B Request Register)
 */
typedef struct
{
    union
    {
        __IOM uint8_t PIBR;            /*!< (@ 0x00000000) Software Configurable Interrupt B Request Register         */

        struct
        {
            __IOM uint8_t PIR0 : 1;    /*!< [0..0] Software Configurable Interrupt B Status Flag 0                    */
            __IOM uint8_t PIR1 : 1;    /*!< [1..1] Software Configurable Interrupt B Status Flag 1                    */
            __IOM uint8_t PIR2 : 1;    /*!< [2..2] Software Configurable Interrupt B Status Flag 2                    */
            __IOM uint8_t PIR3 : 1;    /*!< [3..3] Software Configurable Interrupt B Status Flag 3                    */
            __IOM uint8_t PIR4 : 1;    /*!< [4..4] Software Configurable Interrupt B Status Flag 4                    */
            __IOM uint8_t PIR5 : 1;    /*!< [5..5] Software Configurable Interrupt B Status Flag 5                    */
            __IOM uint8_t PIR6 : 1;    /*!< [6..6] Software Configurable Interrupt B Status Flag 6                    */
            __IOM uint8_t PIR7 : 1;    /*!< [7..7] Software Configurable Interrupt B Status Flag 7                    */
        } PIBR_b;
    };
} R_ICU_PIBR_Type;                     /*!< Size = 1 (0x1)                                                            */

/**
 * @brief R_ICU_SLIBXR [SLIBXR] (Software Configurable Interrupt B Source Select Register X)
 */
typedef struct
{
    union
    {
        __IOM uint8_t SLIBXR;          /*!< (@ 0x00000000) Software Configurable Interrupt B Source Select
                                        *                  Register X                                                 */

        struct
        {
            __IOM uint8_t SLIBXR : 8;  /*!< [7..0] 00h: No interrupt source selected.01h: Interrupt source
                                        *   number 1 :FEh: Interrupt source number 254FFh: Interrupt
                                        *   source number 255                                                         */
        } SLIBXR_b;
    };
} R_ICU_SLIBXR_Type;                   /*!< Size = 1 (0x1)                                                            */

/**
 * @brief R_ICU_SLIBR [SLIBR] (Software Configurable Interrupt B Source Select Register)
 */
typedef struct
{
    union
    {
        __IOM uint8_t SLIBR;           /*!< (@ 0x00000000) Software Configurable Interrupt B Source Select
                                        *                  Register                                                   */

        struct
        {
            __IOM uint8_t SLIBR : 8;   /*!< [7..0] 00h: No interrupt source selected.01h: Interrupt source
                                        *   number 1 :FEh: Interrupt source number 254FFh: No interrupt
                                        *   source selected.                                                          */
        } SLIBR_b;
    };
} R_ICU_SLIBR_Type;                    /*!< Size = 1 (0x1)                                                            */

/**
 * @brief R_ICU_PIAR [PIAR] (Software Configurable Interrupt A Request Register)
 */
typedef struct
{
    union
    {
        __IOM uint8_t PIAR;            /*!< (@ 0x00000000) Software Configurable Interrupt A Request Register         */

        struct
        {
            __IOM uint8_t PIR0 : 1;    /*!< [0..0] Software Configurable Interrupt A Status Flag 0                    */
            __IOM uint8_t PIR1 : 1;    /*!< [1..1] Software Configurable Interrupt A Status Flag 1                    */
            __IOM uint8_t PIR2 : 1;    /*!< [2..2] Software Configurable Interrupt A Status Flag 2                    */
            __IOM uint8_t PIR3 : 1;    /*!< [3..3] Software Configurable Interrupt A Status Flag 3                    */
            __IOM uint8_t PIR4 : 1;    /*!< [4..4] Software Configurable Interrupt A Status Flag 4                    */
            __IOM uint8_t PIR5 : 1;    /*!< [5..5] Software Configurable Interrupt A Status Flag 5                    */
            __IOM uint8_t PIR6 : 1;    /*!< [6..6] Software Configurable Interrupt A Status Flag 6                    */
            __IOM uint8_t PIR7 : 1;    /*!< [7..7] Software Configurable Interrupt A Status Flag 7                    */
        } PIAR_b;
    };
} R_ICU_PIAR_Type;                     /*!< Size = 1 (0x1)                                                            */

/**
 * @brief R_ICU_SLIAR [SLIAR] (Software Configurable Interrupt A Source Select Register)
 */
typedef struct
{
    union
    {
        __IOM uint8_t SLIAR;           /*!< (@ 0x00000000) Software Configurable Interrupt A Source Select
                                        *                  Register                                                   */

        struct
        {
            __IOM uint8_t SLIAR : 8;   /*!< [7..0] 00h: No interrupt source selected.01h: Interrupt source
                                        *   number 1 :FEh: Interrupt source number 254FFh: No interrupt
                                        *   source selected.                                                          */
        } SLIAR_b;
    };
} R_ICU_SLIAR_Type;                    /*!< Size = 1 (0x1)                                                            */

/* ================                                           R_ICU                                           ================ */

/**
 * @brief Interrupt Controller (R_ICU)
 */

typedef struct                         /*!< (@ 0x00087000) R_ICU Structure                                            */
{
    __IOM R_ICU_IR_Type    IR[256];    /*!< (@ 0x00000000) Interrupt Request Register                                 */
    __IOM R_ICU_DTCER_Type DTCER[256]; /*!< (@ 0x00000100) DTC Transfer Request Enable Register                       */
    __IOM R_ICU_IER_Type   IER[32];    /*!< (@ 0x00000200) Interrupt Request Enable Register                          */
    __IM uint32_t          RESERVED[48];

    union
    {
        __IOM uint8_t SWINTR;          /*!< (@ 0x000002E0) Software Interrupt Generation Register                     */

        struct
        {
            __IOM uint8_t SWINT : 1;   /*!< [0..0] Software Interrupt Generation                                      */
            uint8_t             : 7;
        } SWINTR_b;
    };

    union
    {
        __IOM uint8_t SWINT2R;         /*!< (@ 0x000002E1) Software Interrupt 2 Generation Register                   */

        struct
        {
            __IOM uint8_t SWINT2 : 1;  /*!< [0..0] Software Interrupt 2 Generation                                    */
            uint8_t              : 7;
        } SWINT2R_b;
    };
    __IM uint16_t RESERVED1;
    __IM uint32_t RESERVED2[3];

    union
    {
        __IOM uint16_t FIR;            /*!< (@ 0x000002F0) Fast Interrupt Set Register                                */

        struct
        {
            __IOM uint16_t FVCT : 8;   /*!< [7..0] Fast Interrupt Vector Number                                       */
            uint16_t            : 7;
            __IOM uint16_t FIEN : 1;   /*!< [15..15] Fast Interrupt Enable                                            */
        } FIR_b;
    };
    __IM uint16_t            RESERVED3;
    __IM uint32_t            RESERVED4[3];
    __IOM R_ICU_IPR_Type     IPR[256];   /*!< (@ 0x00000300) Interrupt Source Priority Register                         */
    __IOM R_ICU_DMRSR_Type   DMRSR[8];   /*!< (@ 0x00000400) DMAC Trigger Select Register                               */
    __IM uint32_t            RESERVED5[56];
    __IOM R_ICU_IRQCR_Type   IRQCR[16];  /*!< (@ 0x00000500) IRQ Control Register                                       */
    __IM uint32_t            RESERVED6[4];
    __IOM R_ICU_IRQFLTE_Type IRQFLTE[2]; /*!< (@ 0x00000520) IRQ Pin Digital Filter Enable Register                     */
    __IM uint16_t            RESERVED7;
    __IM uint32_t            RESERVED8;
    __IOM R_ICU_IRQFLTC_Type IRQFLTC[2]; /*!< (@ 0x00000528) IRQ Pin Digital Filter Setting Register                    */
    __IM uint32_t            RESERVED9[21];

    union
    {
        __IM uint8_t NMISR;            /*!< (@ 0x00000580) Non-Maskable Interrupt Status Register                     */

        struct
        {
            __IM uint8_t NMIST   : 1;  /*!< [0..0] NMI Status Flag                                                    */
            __IM uint8_t OSTST   : 1;  /*!< [1..1] Oscillation Stop Detection Interrupt Status Flag                   */
            __IM uint8_t WDTST   : 1;  /*!< [2..2] WDT Underflow/Refresh Error Status Flag                            */
            __IM uint8_t IWDTST  : 1;  /*!< [3..3] IWDT Underflow/Refresh Error Status Flag                           */
            __IM uint8_t LVD1ST  : 1;  /*!< [4..4] Voltage Monitoring 1 Interrupt Status Flag                         */
            __IM uint8_t LVD2ST  : 1;  /*!< [5..5] Voltage Monitoring 2 Interrupt Status Flag                         */
            __IM uint8_t EXNMIST : 1;  /*!< [6..6] Expanded Non-Maskable Interrupt Status Flag                        */
            uint8_t              : 1;
        } NMISR_b;
    };

    union
    {
        __IOM uint8_t NMIER;           /*!< (@ 0x00000581) Non-Maskable Interrupt Enable Register                     */

        struct
        {
            __IOM uint8_t NMIEN   : 1; /*!< [0..0] NMI Pin Interrupt Enable                                           */
            __IOM uint8_t OSTEN   : 1; /*!< [1..1] Oscillation Stop Detection Interrupt Enable                        */
            __IOM uint8_t WDTEN   : 1; /*!< [2..2] WDT Underflow/Refresh Error Enable                                 */
            __IOM uint8_t IWDTEN  : 1; /*!< [3..3] IWDT Underflow/Refresh Error Enable                                */
            __IOM uint8_t LVD1EN  : 1; /*!< [4..4] Voltage Monitoring 1 Interrupt Enable                              */
            __IOM uint8_t LVD2EN  : 1; /*!< [5..5] Voltage Monitoring 2 Interrupt Enable                              */
            __IOM uint8_t EXNMIEN : 1; /*!< [6..6] Expanded Non-Maskable Interrupt Enable                             */
            uint8_t               : 1;
        } NMIER_b;
    };

    union
    {
        __IOM uint8_t NMICLR;          /*!< (@ 0x00000582) Non-Maskable Interrupt Status Clear Register               */

        struct
        {
            __IOM uint8_t NMICLR  : 1; /*!< [0..0] NMI Clear                                                          */
            __IOM uint8_t OSTCLR  : 1; /*!< [1..1] OST Clear                                                          */
            __IOM uint8_t WDTCLR  : 1; /*!< [2..2] WDT Clear                                                          */
            __IOM uint8_t IWDTCLR : 1; /*!< [3..3] IWDT Clear                                                         */
            __IOM uint8_t LVD1CLR : 1; /*!< [4..4] LVD1 Clear                                                         */
            __IOM uint8_t LVD2CLR : 1; /*!< [5..5] LVD2 Clear                                                         */
            uint8_t               : 2;
        } NMICLR_b;
    };

    union
    {
        __IOM uint8_t NMICR;           /*!< (@ 0x00000583) NMI Pin Interrupt Control Register                         */

        struct
        {
            uint8_t             : 3;
            __IOM uint8_t NMIMD : 1;   /*!< [3..3] NMI Detection Set                                                  */
            uint8_t             : 4;
        } NMICR_b;
    };

    union
    {
        __IM uint8_t EXNMISR;          /*!< (@ 0x00000584) Expanded Non-Maskable Interrupt Status Register            */

        struct
        {
            __IM uint8_t RAMST   : 1;  /*!< [0..0] RAM Error Interrupt Status Flag                                    */
            __IM uint8_t DPFPUST : 1;  /*!< [1..1] Double-Precision Floating-Point Exception Status Flag              */
            uint8_t              : 6;
        } EXNMISR_b;
    };

    union
    {
        __IOM uint8_t EXNMIER;         /*!< (@ 0x00000585) Expanded Non-Maskable Interrupt Enable Register            */

        struct
        {
            __IOM uint8_t RAMEN   : 1; /*!< [0..0] RAM Error Interrupt Enable                                         */
            __IOM uint8_t DPFPUEN : 1; /*!< [1..1] Double-Precision Floating-Point Exception Enable                   */
            uint8_t               : 6;
        } EXNMIER_b;
    };

    union
    {
        __IOM uint8_t EXNMICLR;        /*!< (@ 0x00000586) Expanded Non-Maskable Interrupt Status Clear
                                        *                  Register                                                   */

        struct
        {
            uint8_t                : 1;
            __IOM uint8_t DPFPUCLR : 1; /*!< [1..1] Double-Precision Floating-Point Exception Clear                    */
            uint8_t                : 6;
        } EXNMICLR_b;
    };
    __IM uint8_t  RESERVED10;
    __IM uint32_t RESERVED11[2];

    union
    {
        __IOM uint8_t NMIFLTE;         /*!< (@ 0x00000590) NMI Pin Digital Filter Enable Register                     */

        struct
        {
            __IOM uint8_t NFLTEN : 1;  /*!< [0..0] NMI Digital Filter Enable                                          */
            uint8_t              : 7;
        } NMIFLTE_b;
    };
    __IM uint8_t  RESERVED12;
    __IM uint16_t RESERVED13;

    union
    {
        __IOM uint8_t NMIFLTC;          /*!< (@ 0x00000594) NMI Pin Digital Filter Setting Register                    */

        struct
        {
            __IOM uint8_t NFCLKSEL : 2; /*!< [1..0] NMI Digital Filter Sampling Clock                                  */
            uint8_t                : 6;
        } NMIFLTC_b;
    };
    __IM uint8_t  RESERVED14;
    __IM uint16_t RESERVED15;
    __IM uint32_t RESERVED16[6];

    union
    {
        __IM uint32_t GRPIE0;          /*!< (@ 0x000005B0) Group IE0 Interrupt Request Register                       */

        struct
        {
            __IM uint32_t IS0  : 1;    /*!< [0..0] Interrupt Status Flag 0                                            */
            __IM uint32_t IS1  : 1;    /*!< [1..1] Interrupt Status Flag 1                                            */
            __IM uint32_t IS2  : 1;    /*!< [2..2] Interrupt Status Flag 2                                            */
            __IM uint32_t IS3  : 1;    /*!< [3..3] Interrupt Status Flag 3                                            */
            __IM uint32_t IS4  : 1;    /*!< [4..4] Interrupt Status Flag 4                                            */
            __IM uint32_t IS5  : 1;    /*!< [5..5] Interrupt Status Flag 5                                            */
            __IM uint32_t IS6  : 1;    /*!< [6..6] Interrupt Status Flag 6                                            */
            __IM uint32_t IS7  : 1;    /*!< [7..7] Interrupt Status Flag 7                                            */
            __IM uint32_t IS8  : 1;    /*!< [8..8] Interrupt Status Flag 8                                            */
            __IM uint32_t IS9  : 1;    /*!< [9..9] Interrupt Status Flag 9                                            */
            __IM uint32_t IS10 : 1;    /*!< [10..10] Interrupt Status Flag 10                                         */
            __IM uint32_t IS11 : 1;    /*!< [11..11] Interrupt Status Flag 11                                         */
            __IM uint32_t IS12 : 1;    /*!< [12..12] Interrupt Status Flag 12                                         */
            __IM uint32_t IS13 : 1;    /*!< [13..13] Interrupt Status Flag 13                                         */
            __IM uint32_t IS14 : 1;    /*!< [14..14] Interrupt Status Flag 14                                         */
            __IM uint32_t IS15 : 1;    /*!< [15..15] Interrupt Status Flag 15                                         */
            __IM uint32_t IS16 : 1;    /*!< [16..16] Interrupt Status Flag 16                                         */
            __IM uint32_t IS17 : 1;    /*!< [17..17] Interrupt Status Flag 17                                         */
            __IM uint32_t IS18 : 1;    /*!< [18..18] Interrupt Status Flag 18                                         */
            __IM uint32_t IS19 : 1;    /*!< [19..19] Interrupt Status Flag 19                                         */
            __IM uint32_t IS20 : 1;    /*!< [20..20] Interrupt Status Flag 20                                         */
            __IM uint32_t IS21 : 1;    /*!< [21..21] Interrupt Status Flag 21                                         */
            __IM uint32_t IS22 : 1;    /*!< [22..22] Interrupt Status Flag 22                                         */
            __IM uint32_t IS23 : 1;    /*!< [23..23] Interrupt Status Flag 23                                         */
            __IM uint32_t IS24 : 1;    /*!< [24..24] Interrupt Status Flag 24                                         */
            __IM uint32_t IS25 : 1;    /*!< [25..25] Interrupt Status Flag 25                                         */
            __IM uint32_t IS26 : 1;    /*!< [26..26] Interrupt Status Flag 26                                         */
            __IM uint32_t IS27 : 1;    /*!< [27..27] Interrupt Status Flag 27                                         */
            __IM uint32_t IS28 : 1;    /*!< [28..28] Interrupt Status Flag 28                                         */
            __IM uint32_t IS29 : 1;    /*!< [29..29] Interrupt Status Flag 29                                         */
            __IM uint32_t IS30 : 1;    /*!< [30..30] Interrupt Status Flag 30                                         */
            __IM uint32_t IS31 : 1;    /*!< [31..31] Interrupt Status Flag 31                                         */
        } GRPIE0_b;
    };

    union
    {
        __IOM uint32_t GENIE0;         /*!< (@ 0x000005B4) Group IE0 Interrupt Request Enable Register                */

        struct
        {
            __IM uint32_t EN0  : 1;    /*!< [0..0] Interrupt Request Enable 0                                         */
            __IM uint32_t EN1  : 1;    /*!< [1..1] Interrupt Request Enable 1                                         */
            __IM uint32_t EN2  : 1;    /*!< [2..2] Interrupt Request Enable 2                                         */
            __IM uint32_t EN3  : 1;    /*!< [3..3] Interrupt Request Enable 3                                         */
            __IM uint32_t EN4  : 1;    /*!< [4..4] Interrupt Request Enable 4                                         */
            __IM uint32_t EN5  : 1;    /*!< [5..5] Interrupt Request Enable 5                                         */
            __IM uint32_t EN6  : 1;    /*!< [6..6] Interrupt Request Enable 6                                         */
            __IM uint32_t EN7  : 1;    /*!< [7..7] Interrupt Request Enable 7                                         */
            __IM uint32_t EN8  : 1;    /*!< [8..8] Interrupt Request Enable 8                                         */
            __IM uint32_t EN9  : 1;    /*!< [9..9] Interrupt Request Enable 9                                         */
            __IM uint32_t EN10 : 1;    /*!< [10..10] Interrupt Request Enable 10                                      */
            __IM uint32_t EN11 : 1;    /*!< [11..11] Interrupt Request Enable 11                                      */
            __IM uint32_t EN12 : 1;    /*!< [12..12] Interrupt Request Enable 12                                      */
            __IM uint32_t EN13 : 1;    /*!< [13..13] Interrupt Request Enable 13                                      */
            __IM uint32_t EN14 : 1;    /*!< [14..14] Interrupt Request Enable 14                                      */
            __IM uint32_t EN15 : 1;    /*!< [15..15] Interrupt Request Enable 15                                      */
            __IM uint32_t EN16 : 1;    /*!< [16..16] Interrupt Request Enable 16                                      */
            __IM uint32_t EN17 : 1;    /*!< [17..17] Interrupt Request Enable 17                                      */
            __IM uint32_t EN18 : 1;    /*!< [18..18] Interrupt Request Enable 18                                      */
            __IM uint32_t EN19 : 1;    /*!< [19..19] Interrupt Request Enable 19                                      */
            __IM uint32_t EN20 : 1;    /*!< [20..20] Interrupt Request Enable 20                                      */
            __IM uint32_t EN21 : 1;    /*!< [21..21] Interrupt Request Enable 21                                      */
            __IM uint32_t EN22 : 1;    /*!< [22..22] Interrupt Request Enable 22                                      */
            __IM uint32_t EN23 : 1;    /*!< [23..23] Interrupt Request Enable 23                                      */
            __IM uint32_t EN24 : 1;    /*!< [24..24] Interrupt Request Enable 24                                      */
            __IM uint32_t EN25 : 1;    /*!< [25..25] Interrupt Request Enable 25                                      */
            __IM uint32_t EN26 : 1;    /*!< [26..26] Interrupt Request Enable 26                                      */
            __IM uint32_t EN27 : 1;    /*!< [27..27] Interrupt Request Enable 27                                      */
            __IM uint32_t EN28 : 1;    /*!< [28..28] Interrupt Request Enable 28                                      */
            __IM uint32_t EN29 : 1;    /*!< [29..29] Interrupt Request Enable 29                                      */
            __IM uint32_t EN30 : 1;    /*!< [30..30] Interrupt Request Enable 30                                      */
            __IM uint32_t EN31 : 1;    /*!< [31..31] Interrupt Request Enable 31                                      */
        } GENIE0_b;
    };

    union
    {
        __IOM uint32_t GCRIE0;         /*!< (@ 0x000005B8) Group IE0 Interrupt Clear Register                         */

        struct
        {
            __IM uint32_t CLR0  : 1;   /*!< [0..0] Interrupt Source Clear 0                                           */
            __IM uint32_t CLR1  : 1;   /*!< [1..1] Interrupt Source Clear 1                                           */
            __IM uint32_t CLR2  : 1;   /*!< [2..2] Interrupt Source Clear 2                                           */
            __IM uint32_t CLR3  : 1;   /*!< [3..3] Interrupt Source Clear 3                                           */
            __IM uint32_t CLR4  : 1;   /*!< [4..4] Interrupt Source Clear 4                                           */
            __IM uint32_t CLR5  : 1;   /*!< [5..5] Interrupt Source Clear 5                                           */
            __IM uint32_t CLR6  : 1;   /*!< [6..6] Interrupt Source Clear 6                                           */
            __IM uint32_t CLR7  : 1;   /*!< [7..7] Interrupt Source Clear 7                                           */
            __IM uint32_t CLR8  : 1;   /*!< [8..8] Interrupt Source Clear 8                                           */
            __IM uint32_t CLR9  : 1;   /*!< [9..9] Interrupt Source Clear 9                                           */
            __IM uint32_t CLR10 : 1;   /*!< [10..10] Interrupt Source Clear 10                                        */
            __IM uint32_t CLR11 : 1;   /*!< [11..11] Interrupt Source Clear 11                                        */
            __IM uint32_t CLR12 : 1;   /*!< [12..12] Interrupt Source Clear 12                                        */
            __IM uint32_t CLR13 : 1;   /*!< [13..13] Interrupt Source Clear 13                                        */
            __IM uint32_t CLR14 : 1;   /*!< [14..14] Interrupt Source Clear 14                                        */
            __IM uint32_t CLR15 : 1;   /*!< [15..15] Interrupt Source Clear 15                                        */
            __IM uint32_t CLR16 : 1;   /*!< [16..16] Interrupt Source Clear 16                                        */
            __IM uint32_t CLR17 : 1;   /*!< [17..17] Interrupt Source Clear 17                                        */
            __IM uint32_t CLR18 : 1;   /*!< [18..18] Interrupt Source Clear 18                                        */
            __IM uint32_t CLR19 : 1;   /*!< [19..19] Interrupt Source Clear 19                                        */
            __IM uint32_t CLR20 : 1;   /*!< [20..20] Interrupt Source Clear 20                                        */
            __IM uint32_t CLR21 : 1;   /*!< [21..21] Interrupt Source Clear 21                                        */
            __IM uint32_t CLR22 : 1;   /*!< [22..22] Interrupt Source Clear 22                                        */
            __IM uint32_t CLR23 : 1;   /*!< [23..23] Interrupt Source Clear 23                                        */
            __IM uint32_t CLR24 : 1;   /*!< [24..24] Interrupt Source Clear 24                                        */
            __IM uint32_t CLR25 : 1;   /*!< [25..25] Interrupt Source Clear 25                                        */
            __IM uint32_t CLR26 : 1;   /*!< [26..26] Interrupt Source Clear 26                                        */
            __IM uint32_t CLR27 : 1;   /*!< [27..27] Interrupt Source Clear 27                                        */
            __IM uint32_t CLR28 : 1;   /*!< [28..28] Interrupt Source Clear 28                                        */
            __IM uint32_t CLR29 : 1;   /*!< [29..29] Interrupt Source Clear 29                                        */
            __IM uint32_t CLR30 : 1;   /*!< [30..30] Interrupt Source Clear 30                                        */
            __IM uint32_t CLR31 : 1;   /*!< [31..31] Interrupt Source Clear 31                                        */
        } GCRIE0_b;
    };
    __IM uint32_t RESERVED17[17];

    union
    {
        __IM uint32_t GRPBE0;          /*!< (@ 0x00000600) Group BE0 Interrupt Request Register                       */

        struct
        {
            __IM uint32_t IS0  : 1;    /*!< [0..0] Interrupt Status Flag 0                                            */
            __IM uint32_t IS1  : 1;    /*!< [1..1] Interrupt Status Flag 1                                            */
            __IM uint32_t IS2  : 1;    /*!< [2..2] Interrupt Status Flag 2                                            */
            __IM uint32_t IS3  : 1;    /*!< [3..3] Interrupt Status Flag 3                                            */
            __IM uint32_t IS4  : 1;    /*!< [4..4] Interrupt Status Flag 4                                            */
            __IM uint32_t IS5  : 1;    /*!< [5..5] Interrupt Status Flag 5                                            */
            __IM uint32_t IS6  : 1;    /*!< [6..6] Interrupt Status Flag 6                                            */
            __IM uint32_t IS7  : 1;    /*!< [7..7] Interrupt Status Flag 7                                            */
            __IM uint32_t IS8  : 1;    /*!< [8..8] Interrupt Status Flag 8                                            */
            __IM uint32_t IS9  : 1;    /*!< [9..9] Interrupt Status Flag 9                                            */
            __IM uint32_t IS10 : 1;    /*!< [10..10] Interrupt Status Flag 10                                         */
            __IM uint32_t IS11 : 1;    /*!< [11..11] Interrupt Status Flag 11                                         */
            __IM uint32_t IS12 : 1;    /*!< [12..12] Interrupt Status Flag 12                                         */
            __IM uint32_t IS13 : 1;    /*!< [13..13] Interrupt Status Flag 13                                         */
            __IM uint32_t IS14 : 1;    /*!< [14..14] Interrupt Status Flag 14                                         */
            __IM uint32_t IS15 : 1;    /*!< [15..15] Interrupt Status Flag 15                                         */
            __IM uint32_t IS16 : 1;    /*!< [16..16] Interrupt Status Flag 16                                         */
            __IM uint32_t IS17 : 1;    /*!< [17..17] Interrupt Status Flag 17                                         */
            __IM uint32_t IS18 : 1;    /*!< [18..18] Interrupt Status Flag 18                                         */
            __IM uint32_t IS19 : 1;    /*!< [19..19] Interrupt Status Flag 19                                         */
            __IM uint32_t IS20 : 1;    /*!< [20..20] Interrupt Status Flag 20                                         */
            __IM uint32_t IS21 : 1;    /*!< [21..21] Interrupt Status Flag 21                                         */
            __IM uint32_t IS22 : 1;    /*!< [22..22] Interrupt Status Flag 22                                         */
            __IM uint32_t IS23 : 1;    /*!< [23..23] Interrupt Status Flag 23                                         */
            __IM uint32_t IS24 : 1;    /*!< [24..24] Interrupt Status Flag 24                                         */
            __IM uint32_t IS25 : 1;    /*!< [25..25] Interrupt Status Flag 25                                         */
            __IM uint32_t IS26 : 1;    /*!< [26..26] Interrupt Status Flag 26                                         */
            __IM uint32_t IS27 : 1;    /*!< [27..27] Interrupt Status Flag 27                                         */
            __IM uint32_t IS28 : 1;    /*!< [28..28] Interrupt Status Flag 28                                         */
            __IM uint32_t IS29 : 1;    /*!< [29..29] Interrupt Status Flag 29                                         */
            __IM uint32_t IS30 : 1;    /*!< [30..30] Interrupt Status Flag 30                                         */
            __IM uint32_t IS31 : 1;    /*!< [31..31] Interrupt Status Flag 31                                         */
        } GRPBE0_b;
    };
    __IM uint32_t RESERVED18[11];

    union
    {
        __IM uint32_t GRPBL0;          /*!< (@ 0x00000630) Group BL0 Interrupt Request Register                       */

        struct
        {
            __IM uint32_t IS0  : 1;    /*!< [0..0] Interrupt Status Flag 0                                            */
            __IM uint32_t IS1  : 1;    /*!< [1..1] Interrupt Status Flag 1                                            */
            __IM uint32_t IS2  : 1;    /*!< [2..2] Interrupt Status Flag 2                                            */
            __IM uint32_t IS3  : 1;    /*!< [3..3] Interrupt Status Flag 3                                            */
            __IM uint32_t IS4  : 1;    /*!< [4..4] Interrupt Status Flag 4                                            */
            __IM uint32_t IS5  : 1;    /*!< [5..5] Interrupt Status Flag 5                                            */
            __IM uint32_t IS6  : 1;    /*!< [6..6] Interrupt Status Flag 6                                            */
            __IM uint32_t IS7  : 1;    /*!< [7..7] Interrupt Status Flag 7                                            */
            __IM uint32_t IS8  : 1;    /*!< [8..8] Interrupt Status Flag 8                                            */
            __IM uint32_t IS9  : 1;    /*!< [9..9] Interrupt Status Flag 9                                            */
            __IM uint32_t IS10 : 1;    /*!< [10..10] Interrupt Status Flag 10                                         */
            __IM uint32_t IS11 : 1;    /*!< [11..11] Interrupt Status Flag 11                                         */
            __IM uint32_t IS12 : 1;    /*!< [12..12] Interrupt Status Flag 12                                         */
            __IM uint32_t IS13 : 1;    /*!< [13..13] Interrupt Status Flag 13                                         */
            __IM uint32_t IS14 : 1;    /*!< [14..14] Interrupt Status Flag 14                                         */
            __IM uint32_t IS15 : 1;    /*!< [15..15] Interrupt Status Flag 15                                         */
            __IM uint32_t IS16 : 1;    /*!< [16..16] Interrupt Status Flag 16                                         */
            __IM uint32_t IS17 : 1;    /*!< [17..17] Interrupt Status Flag 17                                         */
            __IM uint32_t IS18 : 1;    /*!< [18..18] Interrupt Status Flag 18                                         */
            __IM uint32_t IS19 : 1;    /*!< [19..19] Interrupt Status Flag 19                                         */
            __IM uint32_t IS20 : 1;    /*!< [20..20] Interrupt Status Flag 20                                         */
            __IM uint32_t IS21 : 1;    /*!< [21..21] Interrupt Status Flag 21                                         */
            __IM uint32_t IS22 : 1;    /*!< [22..22] Interrupt Status Flag 22                                         */
            __IM uint32_t IS23 : 1;    /*!< [23..23] Interrupt Status Flag 23                                         */
            __IM uint32_t IS24 : 1;    /*!< [24..24] Interrupt Status Flag 24                                         */
            __IM uint32_t IS25 : 1;    /*!< [25..25] Interrupt Status Flag 25                                         */
            __IM uint32_t IS26 : 1;    /*!< [26..26] Interrupt Status Flag 26                                         */
            __IM uint32_t IS27 : 1;    /*!< [27..27] Interrupt Status Flag 27                                         */
            __IM uint32_t IS28 : 1;    /*!< [28..28] Interrupt Status Flag 28                                         */
            __IM uint32_t IS29 : 1;    /*!< [29..29] Interrupt Status Flag 29                                         */
            __IM uint32_t IS30 : 1;    /*!< [30..30] Interrupt Status Flag 30                                         */
            __IM uint32_t IS31 : 1;    /*!< [31..31] Interrupt Status Flag 31                                         */
        } GRPBL0_b;
    };

    union
    {
        __IM uint32_t GRPBL1;          /*!< (@ 0x00000634) Group BL1 Interrupt Request Register                       */

        struct
        {
            __IM uint32_t IS0  : 1;    /*!< [0..0] Interrupt Status Flag 0                                            */
            __IM uint32_t IS1  : 1;    /*!< [1..1] Interrupt Status Flag 1                                            */
            __IM uint32_t IS2  : 1;    /*!< [2..2] Interrupt Status Flag 2                                            */
            __IM uint32_t IS3  : 1;    /*!< [3..3] Interrupt Status Flag 3                                            */
            __IM uint32_t IS4  : 1;    /*!< [4..4] Interrupt Status Flag 4                                            */
            __IM uint32_t IS5  : 1;    /*!< [5..5] Interrupt Status Flag 5                                            */
            __IM uint32_t IS6  : 1;    /*!< [6..6] Interrupt Status Flag 6                                            */
            __IM uint32_t IS7  : 1;    /*!< [7..7] Interrupt Status Flag 7                                            */
            __IM uint32_t IS8  : 1;    /*!< [8..8] Interrupt Status Flag 8                                            */
            __IM uint32_t IS9  : 1;    /*!< [9..9] Interrupt Status Flag 9                                            */
            __IM uint32_t IS10 : 1;    /*!< [10..10] Interrupt Status Flag 10                                         */
            __IM uint32_t IS11 : 1;    /*!< [11..11] Interrupt Status Flag 11                                         */
            __IM uint32_t IS12 : 1;    /*!< [12..12] Interrupt Status Flag 12                                         */
            __IM uint32_t IS13 : 1;    /*!< [13..13] Interrupt Status Flag 13                                         */
            __IM uint32_t IS14 : 1;    /*!< [14..14] Interrupt Status Flag 14                                         */
            __IM uint32_t IS15 : 1;    /*!< [15..15] Interrupt Status Flag 15                                         */
            __IM uint32_t IS16 : 1;    /*!< [16..16] Interrupt Status Flag 16                                         */
            __IM uint32_t IS17 : 1;    /*!< [17..17] Interrupt Status Flag 17                                         */
            __IM uint32_t IS18 : 1;    /*!< [18..18] Interrupt Status Flag 18                                         */
            __IM uint32_t IS19 : 1;    /*!< [19..19] Interrupt Status Flag 19                                         */
            __IM uint32_t IS20 : 1;    /*!< [20..20] Interrupt Status Flag 20                                         */
            __IM uint32_t IS21 : 1;    /*!< [21..21] Interrupt Status Flag 21                                         */
            __IM uint32_t IS22 : 1;    /*!< [22..22] Interrupt Status Flag 22                                         */
            __IM uint32_t IS23 : 1;    /*!< [23..23] Interrupt Status Flag 23                                         */
            __IM uint32_t IS24 : 1;    /*!< [24..24] Interrupt Status Flag 24                                         */
            __IM uint32_t IS25 : 1;    /*!< [25..25] Interrupt Status Flag 25                                         */
            __IM uint32_t IS26 : 1;    /*!< [26..26] Interrupt Status Flag 26                                         */
            __IM uint32_t IS27 : 1;    /*!< [27..27] Interrupt Status Flag 27                                         */
            __IM uint32_t IS28 : 1;    /*!< [28..28] Interrupt Status Flag 28                                         */
            __IM uint32_t IS29 : 1;    /*!< [29..29] Interrupt Status Flag 29                                         */
            __IM uint32_t IS30 : 1;    /*!< [30..30] Interrupt Status Flag 30                                         */
            __IM uint32_t IS31 : 1;    /*!< [31..31] Interrupt Status Flag 31                                         */
        } GRPBL1_b;
    };

    union
    {
        __IM uint32_t GRPBL2;          /*!< (@ 0x00000638) Group BL2 Interrupt Request Register                       */

        struct
        {
            __IM uint32_t IS0  : 1;    /*!< [0..0] Interrupt Status Flag 0                                            */
            __IM uint32_t IS1  : 1;    /*!< [1..1] Interrupt Status Flag 1                                            */
            __IM uint32_t IS2  : 1;    /*!< [2..2] Interrupt Status Flag 2                                            */
            __IM uint32_t IS3  : 1;    /*!< [3..3] Interrupt Status Flag 3                                            */
            __IM uint32_t IS4  : 1;    /*!< [4..4] Interrupt Status Flag 4                                            */
            __IM uint32_t IS5  : 1;    /*!< [5..5] Interrupt Status Flag 5                                            */
            __IM uint32_t IS6  : 1;    /*!< [6..6] Interrupt Status Flag 6                                            */
            __IM uint32_t IS7  : 1;    /*!< [7..7] Interrupt Status Flag 7                                            */
            __IM uint32_t IS8  : 1;    /*!< [8..8] Interrupt Status Flag 8                                            */
            __IM uint32_t IS9  : 1;    /*!< [9..9] Interrupt Status Flag 9                                            */
            __IM uint32_t IS10 : 1;    /*!< [10..10] Interrupt Status Flag 10                                         */
            __IM uint32_t IS11 : 1;    /*!< [11..11] Interrupt Status Flag 11                                         */
            __IM uint32_t IS12 : 1;    /*!< [12..12] Interrupt Status Flag 12                                         */
            __IM uint32_t IS13 : 1;    /*!< [13..13] Interrupt Status Flag 13                                         */
            __IM uint32_t IS14 : 1;    /*!< [14..14] Interrupt Status Flag 14                                         */
            __IM uint32_t IS15 : 1;    /*!< [15..15] Interrupt Status Flag 15                                         */
            __IM uint32_t IS16 : 1;    /*!< [16..16] Interrupt Status Flag 16                                         */
            __IM uint32_t IS17 : 1;    /*!< [17..17] Interrupt Status Flag 17                                         */
            __IM uint32_t IS18 : 1;    /*!< [18..18] Interrupt Status Flag 18                                         */
            __IM uint32_t IS19 : 1;    /*!< [19..19] Interrupt Status Flag 19                                         */
            __IM uint32_t IS20 : 1;    /*!< [20..20] Interrupt Status Flag 20                                         */
            __IM uint32_t IS21 : 1;    /*!< [21..21] Interrupt Status Flag 21                                         */
            __IM uint32_t IS22 : 1;    /*!< [22..22] Interrupt Status Flag 22                                         */
            __IM uint32_t IS23 : 1;    /*!< [23..23] Interrupt Status Flag 23                                         */
            __IM uint32_t IS24 : 1;    /*!< [24..24] Interrupt Status Flag 24                                         */
            __IM uint32_t IS25 : 1;    /*!< [25..25] Interrupt Status Flag 25                                         */
            __IM uint32_t IS26 : 1;    /*!< [26..26] Interrupt Status Flag 26                                         */
            __IM uint32_t IS27 : 1;    /*!< [27..27] Interrupt Status Flag 27                                         */
            __IM uint32_t IS28 : 1;    /*!< [28..28] Interrupt Status Flag 28                                         */
            __IM uint32_t IS29 : 1;    /*!< [29..29] Interrupt Status Flag 29                                         */
            __IM uint32_t IS30 : 1;    /*!< [30..30] Interrupt Status Flag 30                                         */
            __IM uint32_t IS31 : 1;    /*!< [31..31] Interrupt Status Flag 31                                         */
        } GRPBL2_b;
    };
    __IM uint32_t RESERVED19;

    union
    {
        __IOM uint32_t GENBE0;         /*!< (@ 0x00000640) Group BE0 Interrupt Request Enable Register                */

        struct
        {
            __IM uint32_t EN0  : 1;    /*!< [0..0] Interrupt Request Enable 0                                         */
            __IM uint32_t EN1  : 1;    /*!< [1..1] Interrupt Request Enable 1                                         */
            __IM uint32_t EN2  : 1;    /*!< [2..2] Interrupt Request Enable 2                                         */
            __IM uint32_t EN3  : 1;    /*!< [3..3] Interrupt Request Enable 3                                         */
            __IM uint32_t EN4  : 1;    /*!< [4..4] Interrupt Request Enable 4                                         */
            __IM uint32_t EN5  : 1;    /*!< [5..5] Interrupt Request Enable 5                                         */
            __IM uint32_t EN6  : 1;    /*!< [6..6] Interrupt Request Enable 6                                         */
            __IM uint32_t EN7  : 1;    /*!< [7..7] Interrupt Request Enable 7                                         */
            __IM uint32_t EN8  : 1;    /*!< [8..8] Interrupt Request Enable 8                                         */
            __IM uint32_t EN9  : 1;    /*!< [9..9] Interrupt Request Enable 9                                         */
            __IM uint32_t EN10 : 1;    /*!< [10..10] Interrupt Request Enable 10                                      */
            __IM uint32_t EN11 : 1;    /*!< [11..11] Interrupt Request Enable 11                                      */
            __IM uint32_t EN12 : 1;    /*!< [12..12] Interrupt Request Enable 12                                      */
            __IM uint32_t EN13 : 1;    /*!< [13..13] Interrupt Request Enable 13                                      */
            __IM uint32_t EN14 : 1;    /*!< [14..14] Interrupt Request Enable 14                                      */
            __IM uint32_t EN15 : 1;    /*!< [15..15] Interrupt Request Enable 15                                      */
            __IM uint32_t EN16 : 1;    /*!< [16..16] Interrupt Request Enable 16                                      */
            __IM uint32_t EN17 : 1;    /*!< [17..17] Interrupt Request Enable 17                                      */
            __IM uint32_t EN18 : 1;    /*!< [18..18] Interrupt Request Enable 18                                      */
            __IM uint32_t EN19 : 1;    /*!< [19..19] Interrupt Request Enable 19                                      */
            __IM uint32_t EN20 : 1;    /*!< [20..20] Interrupt Request Enable 20                                      */
            __IM uint32_t EN21 : 1;    /*!< [21..21] Interrupt Request Enable 21                                      */
            __IM uint32_t EN22 : 1;    /*!< [22..22] Interrupt Request Enable 22                                      */
            __IM uint32_t EN23 : 1;    /*!< [23..23] Interrupt Request Enable 23                                      */
            __IM uint32_t EN24 : 1;    /*!< [24..24] Interrupt Request Enable 24                                      */
            __IM uint32_t EN25 : 1;    /*!< [25..25] Interrupt Request Enable 25                                      */
            __IM uint32_t EN26 : 1;    /*!< [26..26] Interrupt Request Enable 26                                      */
            __IM uint32_t EN27 : 1;    /*!< [27..27] Interrupt Request Enable 27                                      */
            __IM uint32_t EN28 : 1;    /*!< [28..28] Interrupt Request Enable 28                                      */
            __IM uint32_t EN29 : 1;    /*!< [29..29] Interrupt Request Enable 29                                      */
            __IM uint32_t EN30 : 1;    /*!< [30..30] Interrupt Request Enable 30                                      */
            __IM uint32_t EN31 : 1;    /*!< [31..31] Interrupt Request Enable 31                                      */
        } GENBE0_b;
    };
    __IM uint32_t RESERVED20[11];

    union
    {
        __IOM uint32_t GENBL0;         /*!< (@ 0x00000670) Group BL0 Interrupt Request Enable Register                */

        struct
        {
            __IM uint32_t EN0  : 1;    /*!< [0..0] Interrupt Request Enable 0                                         */
            __IM uint32_t EN1  : 1;    /*!< [1..1] Interrupt Request Enable 1                                         */
            __IM uint32_t EN2  : 1;    /*!< [2..2] Interrupt Request Enable 2                                         */
            __IM uint32_t EN3  : 1;    /*!< [3..3] Interrupt Request Enable 3                                         */
            __IM uint32_t EN4  : 1;    /*!< [4..4] Interrupt Request Enable 4                                         */
            __IM uint32_t EN5  : 1;    /*!< [5..5] Interrupt Request Enable 5                                         */
            __IM uint32_t EN6  : 1;    /*!< [6..6] Interrupt Request Enable 6                                         */
            __IM uint32_t EN7  : 1;    /*!< [7..7] Interrupt Request Enable 7                                         */
            __IM uint32_t EN8  : 1;    /*!< [8..8] Interrupt Request Enable 8                                         */
            __IM uint32_t EN9  : 1;    /*!< [9..9] Interrupt Request Enable 9                                         */
            __IM uint32_t EN10 : 1;    /*!< [10..10] Interrupt Request Enable 10                                      */
            __IM uint32_t EN11 : 1;    /*!< [11..11] Interrupt Request Enable 11                                      */
            __IM uint32_t EN12 : 1;    /*!< [12..12] Interrupt Request Enable 12                                      */
            __IM uint32_t EN13 : 1;    /*!< [13..13] Interrupt Request Enable 13                                      */
            __IM uint32_t EN14 : 1;    /*!< [14..14] Interrupt Request Enable 14                                      */
            __IM uint32_t EN15 : 1;    /*!< [15..15] Interrupt Request Enable 15                                      */
            __IM uint32_t EN16 : 1;    /*!< [16..16] Interrupt Request Enable 16                                      */
            __IM uint32_t EN17 : 1;    /*!< [17..17] Interrupt Request Enable 17                                      */
            __IM uint32_t EN18 : 1;    /*!< [18..18] Interrupt Request Enable 18                                      */
            __IM uint32_t EN19 : 1;    /*!< [19..19] Interrupt Request Enable 19                                      */
            __IM uint32_t EN20 : 1;    /*!< [20..20] Interrupt Request Enable 20                                      */
            __IM uint32_t EN21 : 1;    /*!< [21..21] Interrupt Request Enable 21                                      */
            __IM uint32_t EN22 : 1;    /*!< [22..22] Interrupt Request Enable 22                                      */
            __IM uint32_t EN23 : 1;    /*!< [23..23] Interrupt Request Enable 23                                      */
            __IM uint32_t EN24 : 1;    /*!< [24..24] Interrupt Request Enable 24                                      */
            __IM uint32_t EN25 : 1;    /*!< [25..25] Interrupt Request Enable 25                                      */
            __IM uint32_t EN26 : 1;    /*!< [26..26] Interrupt Request Enable 26                                      */
            __IM uint32_t EN27 : 1;    /*!< [27..27] Interrupt Request Enable 27                                      */
            __IM uint32_t EN28 : 1;    /*!< [28..28] Interrupt Request Enable 28                                      */
            __IM uint32_t EN29 : 1;    /*!< [29..29] Interrupt Request Enable 29                                      */
            __IM uint32_t EN30 : 1;    /*!< [30..30] Interrupt Request Enable 30                                      */
            __IM uint32_t EN31 : 1;    /*!< [31..31] Interrupt Request Enable 31                                      */
        } GENBL0_b;
    };

    union
    {
        __IOM uint32_t GENBL1;         /*!< (@ 0x00000674) Group BL1 Interrupt Request Enable Register                */

        struct
        {
            __IM uint32_t EN0  : 1;    /*!< [0..0] Interrupt Request Enable 0                                         */
            __IM uint32_t EN1  : 1;    /*!< [1..1] Interrupt Request Enable 1                                         */
            __IM uint32_t EN2  : 1;    /*!< [2..2] Interrupt Request Enable 2                                         */
            __IM uint32_t EN3  : 1;    /*!< [3..3] Interrupt Request Enable 3                                         */
            __IM uint32_t EN4  : 1;    /*!< [4..4] Interrupt Request Enable 4                                         */
            __IM uint32_t EN5  : 1;    /*!< [5..5] Interrupt Request Enable 5                                         */
            __IM uint32_t EN6  : 1;    /*!< [6..6] Interrupt Request Enable 6                                         */
            __IM uint32_t EN7  : 1;    /*!< [7..7] Interrupt Request Enable 7                                         */
            __IM uint32_t EN8  : 1;    /*!< [8..8] Interrupt Request Enable 8                                         */
            __IM uint32_t EN9  : 1;    /*!< [9..9] Interrupt Request Enable 9                                         */
            __IM uint32_t EN10 : 1;    /*!< [10..10] Interrupt Request Enable 10                                      */
            __IM uint32_t EN11 : 1;    /*!< [11..11] Interrupt Request Enable 11                                      */
            __IM uint32_t EN12 : 1;    /*!< [12..12] Interrupt Request Enable 12                                      */
            __IM uint32_t EN13 : 1;    /*!< [13..13] Interrupt Request Enable 13                                      */
            __IM uint32_t EN14 : 1;    /*!< [14..14] Interrupt Request Enable 14                                      */
            __IM uint32_t EN15 : 1;    /*!< [15..15] Interrupt Request Enable 15                                      */
            __IM uint32_t EN16 : 1;    /*!< [16..16] Interrupt Request Enable 16                                      */
            __IM uint32_t EN17 : 1;    /*!< [17..17] Interrupt Request Enable 17                                      */
            __IM uint32_t EN18 : 1;    /*!< [18..18] Interrupt Request Enable 18                                      */
            __IM uint32_t EN19 : 1;    /*!< [19..19] Interrupt Request Enable 19                                      */
            __IM uint32_t EN20 : 1;    /*!< [20..20] Interrupt Request Enable 20                                      */
            __IM uint32_t EN21 : 1;    /*!< [21..21] Interrupt Request Enable 21                                      */
            __IM uint32_t EN22 : 1;    /*!< [22..22] Interrupt Request Enable 22                                      */
            __IM uint32_t EN23 : 1;    /*!< [23..23] Interrupt Request Enable 23                                      */
            __IM uint32_t EN24 : 1;    /*!< [24..24] Interrupt Request Enable 24                                      */
            __IM uint32_t EN25 : 1;    /*!< [25..25] Interrupt Request Enable 25                                      */
            __IM uint32_t EN26 : 1;    /*!< [26..26] Interrupt Request Enable 26                                      */
            __IM uint32_t EN27 : 1;    /*!< [27..27] Interrupt Request Enable 27                                      */
            __IM uint32_t EN28 : 1;    /*!< [28..28] Interrupt Request Enable 28                                      */
            __IM uint32_t EN29 : 1;    /*!< [29..29] Interrupt Request Enable 29                                      */
            __IM uint32_t EN30 : 1;    /*!< [30..30] Interrupt Request Enable 30                                      */
            __IM uint32_t EN31 : 1;    /*!< [31..31] Interrupt Request Enable 31                                      */
        } GENBL1_b;
    };

    union
    {
        __IOM uint32_t GENBL2;         /*!< (@ 0x00000678) Group BL2 Interrupt Request Enable Register                */

        struct
        {
            __IM uint32_t EN0  : 1;    /*!< [0..0] Interrupt Request Enable 0                                         */
            __IM uint32_t EN1  : 1;    /*!< [1..1] Interrupt Request Enable 1                                         */
            __IM uint32_t EN2  : 1;    /*!< [2..2] Interrupt Request Enable 2                                         */
            __IM uint32_t EN3  : 1;    /*!< [3..3] Interrupt Request Enable 3                                         */
            __IM uint32_t EN4  : 1;    /*!< [4..4] Interrupt Request Enable 4                                         */
            __IM uint32_t EN5  : 1;    /*!< [5..5] Interrupt Request Enable 5                                         */
            __IM uint32_t EN6  : 1;    /*!< [6..6] Interrupt Request Enable 6                                         */
            __IM uint32_t EN7  : 1;    /*!< [7..7] Interrupt Request Enable 7                                         */
            __IM uint32_t EN8  : 1;    /*!< [8..8] Interrupt Request Enable 8                                         */
            __IM uint32_t EN9  : 1;    /*!< [9..9] Interrupt Request Enable 9                                         */
            __IM uint32_t EN10 : 1;    /*!< [10..10] Interrupt Request Enable 10                                      */
            __IM uint32_t EN11 : 1;    /*!< [11..11] Interrupt Request Enable 11                                      */
            __IM uint32_t EN12 : 1;    /*!< [12..12] Interrupt Request Enable 12                                      */
            __IM uint32_t EN13 : 1;    /*!< [13..13] Interrupt Request Enable 13                                      */
            __IM uint32_t EN14 : 1;    /*!< [14..14] Interrupt Request Enable 14                                      */
            __IM uint32_t EN15 : 1;    /*!< [15..15] Interrupt Request Enable 15                                      */
            __IM uint32_t EN16 : 1;    /*!< [16..16] Interrupt Request Enable 16                                      */
            __IM uint32_t EN17 : 1;    /*!< [17..17] Interrupt Request Enable 17                                      */
            __IM uint32_t EN18 : 1;    /*!< [18..18] Interrupt Request Enable 18                                      */
            __IM uint32_t EN19 : 1;    /*!< [19..19] Interrupt Request Enable 19                                      */
            __IM uint32_t EN20 : 1;    /*!< [20..20] Interrupt Request Enable 20                                      */
            __IM uint32_t EN21 : 1;    /*!< [21..21] Interrupt Request Enable 21                                      */
            __IM uint32_t EN22 : 1;    /*!< [22..22] Interrupt Request Enable 22                                      */
            __IM uint32_t EN23 : 1;    /*!< [23..23] Interrupt Request Enable 23                                      */
            __IM uint32_t EN24 : 1;    /*!< [24..24] Interrupt Request Enable 24                                      */
            __IM uint32_t EN25 : 1;    /*!< [25..25] Interrupt Request Enable 25                                      */
            __IM uint32_t EN26 : 1;    /*!< [26..26] Interrupt Request Enable 26                                      */
            __IM uint32_t EN27 : 1;    /*!< [27..27] Interrupt Request Enable 27                                      */
            __IM uint32_t EN28 : 1;    /*!< [28..28] Interrupt Request Enable 28                                      */
            __IM uint32_t EN29 : 1;    /*!< [29..29] Interrupt Request Enable 29                                      */
            __IM uint32_t EN30 : 1;    /*!< [30..30] Interrupt Request Enable 30                                      */
            __IM uint32_t EN31 : 1;    /*!< [31..31] Interrupt Request Enable 31                                      */
        } GENBL2_b;
    };
    __IM uint32_t RESERVED21;

    union
    {
        __IOM uint32_t GCRBE0;          /*!< (@ 0x00000680) Group BE0 Interrupt Clear Register                         */

        struct
        {
            __IM uint32_t CLR0  : 1;    /*!< [0..0] Interrupt Source Clear 0                                           */
            __IM uint32_t CLR1  : 1;    /*!< [1..1] Interrupt Source Clear 1                                           */
            __IM uint32_t CLR2  : 1;    /*!< [2..2] Interrupt Source Clear 2                                           */
            __IM uint32_t CLR3  : 1;    /*!< [3..3] Interrupt Source Clear 3                                           */
            __IM uint32_t CLR4  : 1;    /*!< [4..4] Interrupt Source Clear 4                                           */
            __IM uint32_t CLR5  : 1;    /*!< [5..5] Interrupt Source Clear 5                                           */
            __IM uint32_t CLR6  : 1;    /*!< [6..6] Interrupt Source Clear 6                                           */
            __IM uint32_t CLR7  : 1;    /*!< [7..7] Interrupt Source Clear 7                                           */
            __IM uint32_t CLR8  : 1;    /*!< [8..8] Interrupt Source Clear 8                                           */
            __IM uint32_t CLR9  : 1;    /*!< [9..9] Interrupt Source Clear 9                                           */
            __IM uint32_t CLR10 : 1;    /*!< [10..10] Interrupt Source Clear 10                                        */
            __IM uint32_t CLR11 : 1;    /*!< [11..11] Interrupt Source Clear 11                                        */
            __IM uint32_t CLR12 : 1;    /*!< [12..12] Interrupt Source Clear 12                                        */
            __IM uint32_t CLR13 : 1;    /*!< [13..13] Interrupt Source Clear 13                                        */
            __IM uint32_t CLR14 : 1;    /*!< [14..14] Interrupt Source Clear 14                                        */
            __IM uint32_t CLR15 : 1;    /*!< [15..15] Interrupt Source Clear 15                                        */
            __IM uint32_t CLR16 : 1;    /*!< [16..16] Interrupt Source Clear 16                                        */
            __IM uint32_t CLR17 : 1;    /*!< [17..17] Interrupt Source Clear 17                                        */
            __IM uint32_t CLR18 : 1;    /*!< [18..18] Interrupt Source Clear 18                                        */
            __IM uint32_t CLR19 : 1;    /*!< [19..19] Interrupt Source Clear 19                                        */
            __IM uint32_t CLR20 : 1;    /*!< [20..20] Interrupt Source Clear 20                                        */
            __IM uint32_t CLR21 : 1;    /*!< [21..21] Interrupt Source Clear 21                                        */
            __IM uint32_t CLR22 : 1;    /*!< [22..22] Interrupt Source Clear 22                                        */
            __IM uint32_t CLR23 : 1;    /*!< [23..23] Interrupt Source Clear 23                                        */
            __IM uint32_t CLR24 : 1;    /*!< [24..24] Interrupt Source Clear 24                                        */
            __IM uint32_t CLR25 : 1;    /*!< [25..25] Interrupt Source Clear 25                                        */
            __IM uint32_t CLR26 : 1;    /*!< [26..26] Interrupt Source Clear 26                                        */
            __IM uint32_t CLR27 : 1;    /*!< [27..27] Interrupt Source Clear 27                                        */
            __IM uint32_t CLR28 : 1;    /*!< [28..28] Interrupt Source Clear 28                                        */
            __IM uint32_t CLR29 : 1;    /*!< [29..29] Interrupt Source Clear 29                                        */
            __IM uint32_t CLR30 : 1;    /*!< [30..30] Interrupt Source Clear 30                                        */
            __IM uint32_t CLR31 : 1;    /*!< [31..31] Interrupt Source Clear 31                                        */
        } GCRBE0_b;
    };
    __IM uint32_t           RESERVED22[31];
    __IOM R_ICU_PIBR_Type   PIBR[12];   /*!< (@ 0x00000700) Software Configurable Interrupt B Request Register         */
    __IM uint32_t           RESERVED23[29];
    __IOM R_ICU_SLIBXR_Type SLIBXR[16]; /*!< (@ 0x00000780) Software Configurable Interrupt B Source Select
                                         *                  Register X                                                 */
    __IOM R_ICU_SLIBR_Type SLIBR[64];   /*!< (@ 0x00000790) Software Configurable Interrupt B Source Select
                                         *                  Register                                                   */
    __IM uint32_t RESERVED24[24];

    union
    {
        __IM uint32_t GRPAL0;          /*!< (@ 0x00000830) Group AL0 Interrupt Request Register                       */

        struct
        {
            __IM uint32_t IS0  : 1;    /*!< [0..0] Interrupt Status Flag 0                                            */
            __IM uint32_t IS1  : 1;    /*!< [1..1] Interrupt Status Flag 1                                            */
            __IM uint32_t IS2  : 1;    /*!< [2..2] Interrupt Status Flag 2                                            */
            __IM uint32_t IS3  : 1;    /*!< [3..3] Interrupt Status Flag 3                                            */
            __IM uint32_t IS4  : 1;    /*!< [4..4] Interrupt Status Flag 4                                            */
            __IM uint32_t IS5  : 1;    /*!< [5..5] Interrupt Status Flag 5                                            */
            __IM uint32_t IS6  : 1;    /*!< [6..6] Interrupt Status Flag 6                                            */
            __IM uint32_t IS7  : 1;    /*!< [7..7] Interrupt Status Flag 7                                            */
            __IM uint32_t IS8  : 1;    /*!< [8..8] Interrupt Status Flag 8                                            */
            __IM uint32_t IS9  : 1;    /*!< [9..9] Interrupt Status Flag 9                                            */
            __IM uint32_t IS10 : 1;    /*!< [10..10] Interrupt Status Flag 10                                         */
            __IM uint32_t IS11 : 1;    /*!< [11..11] Interrupt Status Flag 11                                         */
            __IM uint32_t IS12 : 1;    /*!< [12..12] Interrupt Status Flag 12                                         */
            __IM uint32_t IS13 : 1;    /*!< [13..13] Interrupt Status Flag 13                                         */
            __IM uint32_t IS14 : 1;    /*!< [14..14] Interrupt Status Flag 14                                         */
            __IM uint32_t IS15 : 1;    /*!< [15..15] Interrupt Status Flag 15                                         */
            __IM uint32_t IS16 : 1;    /*!< [16..16] Interrupt Status Flag 16                                         */
            __IM uint32_t IS17 : 1;    /*!< [17..17] Interrupt Status Flag 17                                         */
            __IM uint32_t IS18 : 1;    /*!< [18..18] Interrupt Status Flag 18                                         */
            __IM uint32_t IS19 : 1;    /*!< [19..19] Interrupt Status Flag 19                                         */
            __IM uint32_t IS20 : 1;    /*!< [20..20] Interrupt Status Flag 20                                         */
            __IM uint32_t IS21 : 1;    /*!< [21..21] Interrupt Status Flag 21                                         */
            __IM uint32_t IS22 : 1;    /*!< [22..22] Interrupt Status Flag 22                                         */
            __IM uint32_t IS23 : 1;    /*!< [23..23] Interrupt Status Flag 23                                         */
            __IM uint32_t IS24 : 1;    /*!< [24..24] Interrupt Status Flag 24                                         */
            __IM uint32_t IS25 : 1;    /*!< [25..25] Interrupt Status Flag 25                                         */
            __IM uint32_t IS26 : 1;    /*!< [26..26] Interrupt Status Flag 26                                         */
            __IM uint32_t IS27 : 1;    /*!< [27..27] Interrupt Status Flag 27                                         */
            __IM uint32_t IS28 : 1;    /*!< [28..28] Interrupt Status Flag 28                                         */
            __IM uint32_t IS29 : 1;    /*!< [29..29] Interrupt Status Flag 29                                         */
            __IM uint32_t IS30 : 1;    /*!< [30..30] Interrupt Status Flag 30                                         */
            __IM uint32_t IS31 : 1;    /*!< [31..31] Interrupt Status Flag 31                                         */
        } GRPAL0_b;
    };

    union
    {
        __IM uint32_t GRPAL1;          /*!< (@ 0x00000834) Group AL1 Interrupt Request Register                       */

        struct
        {
            __IM uint32_t IS0  : 1;    /*!< [0..0] Interrupt Status Flag 0                                            */
            __IM uint32_t IS1  : 1;    /*!< [1..1] Interrupt Status Flag 1                                            */
            __IM uint32_t IS2  : 1;    /*!< [2..2] Interrupt Status Flag 2                                            */
            __IM uint32_t IS3  : 1;    /*!< [3..3] Interrupt Status Flag 3                                            */
            __IM uint32_t IS4  : 1;    /*!< [4..4] Interrupt Status Flag 4                                            */
            __IM uint32_t IS5  : 1;    /*!< [5..5] Interrupt Status Flag 5                                            */
            __IM uint32_t IS6  : 1;    /*!< [6..6] Interrupt Status Flag 6                                            */
            __IM uint32_t IS7  : 1;    /*!< [7..7] Interrupt Status Flag 7                                            */
            __IM uint32_t IS8  : 1;    /*!< [8..8] Interrupt Status Flag 8                                            */
            __IM uint32_t IS9  : 1;    /*!< [9..9] Interrupt Status Flag 9                                            */
            __IM uint32_t IS10 : 1;    /*!< [10..10] Interrupt Status Flag 10                                         */
            __IM uint32_t IS11 : 1;    /*!< [11..11] Interrupt Status Flag 11                                         */
            __IM uint32_t IS12 : 1;    /*!< [12..12] Interrupt Status Flag 12                                         */
            __IM uint32_t IS13 : 1;    /*!< [13..13] Interrupt Status Flag 13                                         */
            __IM uint32_t IS14 : 1;    /*!< [14..14] Interrupt Status Flag 14                                         */
            __IM uint32_t IS15 : 1;    /*!< [15..15] Interrupt Status Flag 15                                         */
            __IM uint32_t IS16 : 1;    /*!< [16..16] Interrupt Status Flag 16                                         */
            __IM uint32_t IS17 : 1;    /*!< [17..17] Interrupt Status Flag 17                                         */
            __IM uint32_t IS18 : 1;    /*!< [18..18] Interrupt Status Flag 18                                         */
            __IM uint32_t IS19 : 1;    /*!< [19..19] Interrupt Status Flag 19                                         */
            __IM uint32_t IS20 : 1;    /*!< [20..20] Interrupt Status Flag 20                                         */
            __IM uint32_t IS21 : 1;    /*!< [21..21] Interrupt Status Flag 21                                         */
            __IM uint32_t IS22 : 1;    /*!< [22..22] Interrupt Status Flag 22                                         */
            __IM uint32_t IS23 : 1;    /*!< [23..23] Interrupt Status Flag 23                                         */
            __IM uint32_t IS24 : 1;    /*!< [24..24] Interrupt Status Flag 24                                         */
            __IM uint32_t IS25 : 1;    /*!< [25..25] Interrupt Status Flag 25                                         */
            __IM uint32_t IS26 : 1;    /*!< [26..26] Interrupt Status Flag 26                                         */
            __IM uint32_t IS27 : 1;    /*!< [27..27] Interrupt Status Flag 27                                         */
            __IM uint32_t IS28 : 1;    /*!< [28..28] Interrupt Status Flag 28                                         */
            __IM uint32_t IS29 : 1;    /*!< [29..29] Interrupt Status Flag 29                                         */
            __IM uint32_t IS30 : 1;    /*!< [30..30] Interrupt Status Flag 30                                         */
            __IM uint32_t IS31 : 1;    /*!< [31..31] Interrupt Status Flag 31                                         */
        } GRPAL1_b;
    };
    __IM uint32_t RESERVED25[14];

    union
    {
        __IOM uint32_t GENAL0;         /*!< (@ 0x00000870) Group AL0 Interrupt Request Enable Register                */

        struct
        {
            __IM uint32_t EN0  : 1;    /*!< [0..0] Interrupt Request Enable 0                                         */
            __IM uint32_t EN1  : 1;    /*!< [1..1] Interrupt Request Enable 1                                         */
            __IM uint32_t EN2  : 1;    /*!< [2..2] Interrupt Request Enable 2                                         */
            __IM uint32_t EN3  : 1;    /*!< [3..3] Interrupt Request Enable 3                                         */
            __IM uint32_t EN4  : 1;    /*!< [4..4] Interrupt Request Enable 4                                         */
            __IM uint32_t EN5  : 1;    /*!< [5..5] Interrupt Request Enable 5                                         */
            __IM uint32_t EN6  : 1;    /*!< [6..6] Interrupt Request Enable 6                                         */
            __IM uint32_t EN7  : 1;    /*!< [7..7] Interrupt Request Enable 7                                         */
            __IM uint32_t EN8  : 1;    /*!< [8..8] Interrupt Request Enable 8                                         */
            __IM uint32_t EN9  : 1;    /*!< [9..9] Interrupt Request Enable 9                                         */
            __IM uint32_t EN10 : 1;    /*!< [10..10] Interrupt Request Enable 10                                      */
            __IM uint32_t EN11 : 1;    /*!< [11..11] Interrupt Request Enable 11                                      */
            __IM uint32_t EN12 : 1;    /*!< [12..12] Interrupt Request Enable 12                                      */
            __IM uint32_t EN13 : 1;    /*!< [13..13] Interrupt Request Enable 13                                      */
            __IM uint32_t EN14 : 1;    /*!< [14..14] Interrupt Request Enable 14                                      */
            __IM uint32_t EN15 : 1;    /*!< [15..15] Interrupt Request Enable 15                                      */
            __IM uint32_t EN16 : 1;    /*!< [16..16] Interrupt Request Enable 16                                      */
            __IM uint32_t EN17 : 1;    /*!< [17..17] Interrupt Request Enable 17                                      */
            __IM uint32_t EN18 : 1;    /*!< [18..18] Interrupt Request Enable 18                                      */
            __IM uint32_t EN19 : 1;    /*!< [19..19] Interrupt Request Enable 19                                      */
            __IM uint32_t EN20 : 1;    /*!< [20..20] Interrupt Request Enable 20                                      */
            __IM uint32_t EN21 : 1;    /*!< [21..21] Interrupt Request Enable 21                                      */
            __IM uint32_t EN22 : 1;    /*!< [22..22] Interrupt Request Enable 22                                      */
            __IM uint32_t EN23 : 1;    /*!< [23..23] Interrupt Request Enable 23                                      */
            __IM uint32_t EN24 : 1;    /*!< [24..24] Interrupt Request Enable 24                                      */
            __IM uint32_t EN25 : 1;    /*!< [25..25] Interrupt Request Enable 25                                      */
            __IM uint32_t EN26 : 1;    /*!< [26..26] Interrupt Request Enable 26                                      */
            __IM uint32_t EN27 : 1;    /*!< [27..27] Interrupt Request Enable 27                                      */
            __IM uint32_t EN28 : 1;    /*!< [28..28] Interrupt Request Enable 28                                      */
            __IM uint32_t EN29 : 1;    /*!< [29..29] Interrupt Request Enable 29                                      */
            __IM uint32_t EN30 : 1;    /*!< [30..30] Interrupt Request Enable 30                                      */
            __IM uint32_t EN31 : 1;    /*!< [31..31] Interrupt Request Enable 31                                      */
        } GENAL0_b;
    };

    union
    {
        __IOM uint32_t GENAL1;         /*!< (@ 0x00000874) Group AL1 Interrupt Request Enable Register                */

        struct
        {
            __IM uint32_t EN0  : 1;    /*!< [0..0] Interrupt Request Enable 0                                         */
            __IM uint32_t EN1  : 1;    /*!< [1..1] Interrupt Request Enable 1                                         */
            __IM uint32_t EN2  : 1;    /*!< [2..2] Interrupt Request Enable 2                                         */
            __IM uint32_t EN3  : 1;    /*!< [3..3] Interrupt Request Enable 3                                         */
            __IM uint32_t EN4  : 1;    /*!< [4..4] Interrupt Request Enable 4                                         */
            __IM uint32_t EN5  : 1;    /*!< [5..5] Interrupt Request Enable 5                                         */
            __IM uint32_t EN6  : 1;    /*!< [6..6] Interrupt Request Enable 6                                         */
            __IM uint32_t EN7  : 1;    /*!< [7..7] Interrupt Request Enable 7                                         */
            __IM uint32_t EN8  : 1;    /*!< [8..8] Interrupt Request Enable 8                                         */
            __IM uint32_t EN9  : 1;    /*!< [9..9] Interrupt Request Enable 9                                         */
            __IM uint32_t EN10 : 1;    /*!< [10..10] Interrupt Request Enable 10                                      */
            __IM uint32_t EN11 : 1;    /*!< [11..11] Interrupt Request Enable 11                                      */
            __IM uint32_t EN12 : 1;    /*!< [12..12] Interrupt Request Enable 12                                      */
            __IM uint32_t EN13 : 1;    /*!< [13..13] Interrupt Request Enable 13                                      */
            __IM uint32_t EN14 : 1;    /*!< [14..14] Interrupt Request Enable 14                                      */
            __IM uint32_t EN15 : 1;    /*!< [15..15] Interrupt Request Enable 15                                      */
            __IM uint32_t EN16 : 1;    /*!< [16..16] Interrupt Request Enable 16                                      */
            __IM uint32_t EN17 : 1;    /*!< [17..17] Interrupt Request Enable 17                                      */
            __IM uint32_t EN18 : 1;    /*!< [18..18] Interrupt Request Enable 18                                      */
            __IM uint32_t EN19 : 1;    /*!< [19..19] Interrupt Request Enable 19                                      */
            __IM uint32_t EN20 : 1;    /*!< [20..20] Interrupt Request Enable 20                                      */
            __IM uint32_t EN21 : 1;    /*!< [21..21] Interrupt Request Enable 21                                      */
            __IM uint32_t EN22 : 1;    /*!< [22..22] Interrupt Request Enable 22                                      */
            __IM uint32_t EN23 : 1;    /*!< [23..23] Interrupt Request Enable 23                                      */
            __IM uint32_t EN24 : 1;    /*!< [24..24] Interrupt Request Enable 24                                      */
            __IM uint32_t EN25 : 1;    /*!< [25..25] Interrupt Request Enable 25                                      */
            __IM uint32_t EN26 : 1;    /*!< [26..26] Interrupt Request Enable 26                                      */
            __IM uint32_t EN27 : 1;    /*!< [27..27] Interrupt Request Enable 27                                      */
            __IM uint32_t EN28 : 1;    /*!< [28..28] Interrupt Request Enable 28                                      */
            __IM uint32_t EN29 : 1;    /*!< [29..29] Interrupt Request Enable 29                                      */
            __IM uint32_t EN30 : 1;    /*!< [30..30] Interrupt Request Enable 30                                      */
            __IM uint32_t EN31 : 1;    /*!< [31..31] Interrupt Request Enable 31                                      */
        } GENAL1_b;
    };
    __IM uint32_t         RESERVED26[34];
    __IOM R_ICU_PIAR_Type PIAR[11];    /*!< (@ 0x00000900) Software Configurable Interrupt A Request Register         */
    __IM uint8_t          RESERVED27;

    union
    {
        __IOM uint8_t PIARC;           /*!< (@ 0x0000090C) Software Configurable Interrupt A Request Register
                                        *                  C                                                          */

        struct
        {
            __IOM uint8_t PIR0 : 1;    /*!< [0..0] Software Configurable Interrupt A Status Flag 0                    */
            __IOM uint8_t PIR1 : 1;    /*!< [1..1] Software Configurable Interrupt A Status Flag 1                    */
            __IOM uint8_t PIR2 : 1;    /*!< [2..2] Software Configurable Interrupt A Status Flag 2                    */
            __IOM uint8_t PIR3 : 1;    /*!< [3..3] Software Configurable Interrupt A Status Flag 3                    */
            __IOM uint8_t PIR4 : 1;    /*!< [4..4] Software Configurable Interrupt A Status Flag 4                    */
            __IOM uint8_t PIR5 : 1;    /*!< [5..5] Software Configurable Interrupt A Status Flag 5                    */
            __IOM uint8_t PIR6 : 1;    /*!< [6..6] Software Configurable Interrupt A Status Flag 6                    */
            __IOM uint8_t PIR7 : 1;    /*!< [7..7] Software Configurable Interrupt A Status Flag 7                    */
        } PIARC_b;
    };
    __IM uint8_t           RESERVED28;
    __IM uint16_t          RESERVED29;
    __IM uint32_t          RESERVED30[48];
    __IOM R_ICU_SLIAR_Type SLIAR[48];  /*!< (@ 0x000009D0) Software Configurable Interrupt A Source Select
                                        *                  Register                                                   */

    union
    {
        __IOM uint8_t SLIPRCR;         /*!< (@ 0x00000A00) Software Configurable Interrupt Source Select
                                        *                  Register Write Protect Register                            */

        struct
        {
            __IOM uint8_t WPRC : 1;    /*!< [0..0] Software Configurable Interrupt Source Select Register
                                        *   Write Protect                                                             */
            uint8_t : 7;
        } SLIPRCR_b;
    };

    union
    {
        __IOM uint8_t SELEXDR;         /*!< (@ 0x00000A01) EXDMAC Trigger Select Register                             */

        struct
        {
            __IOM uint8_t SELEXD0 : 1; /*!< [0..0] EXDMAC0 Trigger Select                                             */
            __IOM uint8_t SELEXD1 : 1; /*!< [1..1] EXDMAC1 Trigger Select                                             */
            uint8_t               : 6;
        } SELEXDR_b;
    };
    __IM uint16_t RESERVED31;
} R_ICU_Type;                          /*!< Size = 2564 (0xa04)                                                       */

#endif /* R_ICU_REG_H */
