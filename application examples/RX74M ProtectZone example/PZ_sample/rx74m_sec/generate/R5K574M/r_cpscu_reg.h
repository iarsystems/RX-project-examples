/* ${DISCLAIMER_PLACEHOLDER} */

#ifndef R_CPSCU_REG_H
#define R_CPSCU_REG_H

/*======================================================================================================== */
/* ================                  Device Specific Cluster Section                      ================ */
/*======================================================================================================== */

/** @addtogroup Device_Peripheral_clusters
 * @{
 */

/**
 * @brief CPU System Security Control Unit (CPSCU)
 */

typedef struct                         /*!< CPSCU Structure                                                           */
{
    __IM uint32_t RESERVED[4];

    union
    {
        __IOM uint32_t SRAMSAR;          /*!< SRAM Security Attribution Register                                        */

        struct
        {
            __IOM uint32_t SRAMSA0  : 1; /*!< Security attributes of registers for SRAM0                                */
            __IOM uint32_t SRAMSA1  : 1; /*!< Security attributes of registers for SRAM1                                */
            uint32_t                : 6;
            __IOM uint32_t SRAMWTSA : 1; /*!< Security attributes of registers for SRAMWTSC                             */
            uint32_t                : 23;
        } SRAMSAR_b;
    };
    __IM uint32_t RESERVED1[7];

    union
    {
        __IOM uint32_t DTCSAR;           /*!< DTC Security Attribution Register                                         */

        struct
        {
            __IOM uint32_t DTCSTSA0 : 1; /*!< DTC0 Security Attribution                                                 */
            uint32_t                : 31;
        } DTCSAR_b;
    };

    union
    {
        __IOM uint32_t DMACSAR;          /*!< DMAC Security Attribution Register                                        */

        struct
        {
            __IOM uint32_t DMASTSA0 : 1; /*!< DMAC0.DMAST Security attributes                                           */
            uint32_t                : 31;
        } DMACSAR_b;
    };

    union
    {
        __IOM uint32_t DMACXSAR;         /*!< DMACX Security Attribution Register                                       */

        struct
        {
            __IOM uint32_t DMASTSA0 : 1; /*!< DMACX.DMAST Security attributes                                           */
            uint32_t                : 31;
        } DMACXSAR_b;
    };
    __IM uint32_t RESERVED2;

    union
    {
        __IOM uint32_t ICUSARA;           /*!< ICU Security Attribution Register A                                       */

        struct
        {
            __IOM uint32_t SAIRQCR0  : 1; /*!< Security attributes of registers for the IRQCR0 register                  */
            __IOM uint32_t SAIRQCR1  : 1; /*!< Security attributes of registers for the IRQCR1 register                  */
            __IOM uint32_t SAIRQCR2  : 1; /*!< Security attributes of registers for the IRQCR2 register                  */
            __IOM uint32_t SAIRQCR3  : 1; /*!< Security attributes of registers for the IRQCR3 register                  */
            __IOM uint32_t SAIRQCR4  : 1; /*!< Security attributes of registers for the IRQCR4 register                  */
            __IOM uint32_t SAIRQCR5  : 1; /*!< Security attributes of registers for the IRQCR5 register                  */
            __IOM uint32_t SAIRQCR6  : 1; /*!< Security attributes of registers for the IRQCR6 register                  */
            __IOM uint32_t SAIRQCR7  : 1; /*!< Security attributes of registers for the IRQCR7 register                  */
            __IOM uint32_t SAIRQCR8  : 1; /*!< Security attributes of registers for the IRQCR8 register                  */
            __IOM uint32_t SAIRQCR9  : 1; /*!< Security attributes of registers for the IRQCR9 register                  */
            __IOM uint32_t SAIRQCR10 : 1; /*!< Security attributes of registers for the IRQCR10 register                 */
            __IOM uint32_t SAIRQCR11 : 1; /*!< Security attributes of registers for the IRQCR11 register                 */
            __IOM uint32_t SAIRQCR12 : 1; /*!< Security attributes of registers for the IRQCR12 register                 */
            __IOM uint32_t SAIRQCR13 : 1; /*!< Security attributes of registers for the IRQCR13 register                 */
            __IOM uint32_t SAIRQCR14 : 1; /*!< Security attributes of registers for the IRQCR14 register                 */
            __IOM uint32_t SAIRQCR15 : 1; /*!< Security attributes of registers for the IRQCR15 register                 */
            __IOM uint32_t SAIRQCR16 : 1; /*!< Security attributes of registers for the IRQCR16 register                 */
            __IOM uint32_t SAIRQCR17 : 1; /*!< Security attributes of registers for the IRQCR17 register                 */
            __IOM uint32_t SAIRQCR18 : 1; /*!< Security attributes of registers for the IRQCR18 register                 */
            __IOM uint32_t SAIRQCR19 : 1; /*!< Security attributes of registers for the IRQCR19 register                 */
            __IOM uint32_t SAIRQCR20 : 1; /*!< Security attributes of registers for the IRQCR20 register                 */
            __IOM uint32_t SAIRQCR21 : 1; /*!< Security attributes of registers for the IRQCR21 register                 */
            __IOM uint32_t SAIRQCR22 : 1; /*!< Security attributes of registers for the IRQCR22 register                 */
            __IOM uint32_t SAIRQCR23 : 1; /*!< Security attributes of registers for the IRQCR23 register                 */
            __IOM uint32_t SAIRQCR24 : 1; /*!< Security attributes of registers for the IRQCR24 register                 */
            __IOM uint32_t SAIRQCR25 : 1; /*!< Security attributes of registers for the IRQCR25 register                 */
            __IOM uint32_t SAIRQCR26 : 1; /*!< Security attributes of registers for the IRQCR26 register                 */
            __IOM uint32_t SAIRQCR27 : 1; /*!< Security attributes of registers for the IRQCR27 register                 */
            __IOM uint32_t SAIRQCR28 : 1; /*!< Security attributes of registers for the IRQCR28 register                 */
            __IOM uint32_t SAIRQCR29 : 1; /*!< Security attributes of registers for the IRQCR29 register                 */
            __IOM uint32_t SAIRQCR30 : 1; /*!< Security attributes of registers for the IRQCR30 register                 */
            __IOM uint32_t SAIRQCR31 : 1; /*!< Security attributes of registers for the IRQCR31 register                 */
        } ICUSARA_b;
    };

    union
    {
        __IOM uint32_t ICUSARB;        /*!< ICU Security Attribution Register B                                       */

        struct
        {
            __IOM uint32_t SANMI  : 1; /*!< Security attributes of the NMICR register.                                */
            __IOM uint32_t SANMI0 : 1; /*!< Security attributes of the ICU0.NMISR, ICU0.NMIER, ICU0.NMICLR            */
            uint32_t              : 30;
        } ICUSARB_b;
    };

    union
    {
        __IOM uint32_t ICUSARC;          /*!< ICU Security Attribution Register C                                       */

        struct
        {
            __IOM uint32_t SAFIR0   : 1; /*!< Security attributes of the ICU.FIR register                               */
            __IOM uint32_t SACLPCR0 : 1; /*!< Security attributes of the ICU CLPCR registers                            */
            uint32_t                : 30;
        } ICUSARC_b;
    };

    union
    {
        __IOM uint32_t ICUSARD;          /*!< ICU Security Attribution Register D                                       */

        struct
        {
            __IOM uint32_t SAFPU    : 1; /*!< Security attributes of the ICU.FIR register                               */
            __IOM uint32_t SALM     : 1; /*!< Security attributes of the ICU CLPCR registers                            */
            __IOM uint32_t SACM     : 1; /*!< Security attributes of common memory error interrupt                      */
            __IOM uint32_t SAHLDTO  : 1; /*!< Security attributes of exterbus bus hold timeout error interrupt          */
            uint32_t                : 3;
            __IOM uint32_t SARXFINT : 1; /*!< Security attribute of debug interrupt by emulator                         */
            __IOM uint32_t SAMCEE   : 1; /*!< Security attributes of MRC ECC error interrupt                            */
            __IOM uint32_t SAMEEE   : 1; /*!< Security attributes of MRE ECC error interrupt                            */
            __IOM uint32_t SAMCWE   : 1; /*!< Security attributes of MRC write error interrupt                          */
            __IOM uint32_t SAMDWE   : 1; /*!< Security attributes of MRD write error interrupt                          */
            __IOM uint32_t SAMEWE   : 1; /*!< Security attributes of MRE write error interrupt                          */
            uint32_t                : 19;
        } ICUSARD_b;
    };
    __IM uint32_t RESERVED3[20];

    union
    {
        __IOM uint32_t ICU0SARA;       /*!< ICU0 Security Attribution Register for A                                  */

        struct
        {
            __IOM uint32_t SAIR0  : 1; /*!< Security attributes of registers for ICU0.IRR0                            */
            __IOM uint32_t SAIR1  : 1; /*!< Security attributes of registers for ICU0.IRR1                            */
            __IOM uint32_t SAIR2  : 1; /*!< Security attributes of registers for ICU0.IRR2                            */
            __IOM uint32_t SAIR3  : 1; /*!< Security attributes of registers for ICU0.IRR3                            */
            __IOM uint32_t SAIR4  : 1; /*!< Security attributes of registers for ICU0.IRR4                            */
            __IOM uint32_t SAIR5  : 1; /*!< Security attributes of registers for ICU0.IRR5                            */
            __IOM uint32_t SAIR6  : 1; /*!< Security attributes of registers for ICU0.IRR6                            */
            __IOM uint32_t SAIR7  : 1; /*!< Security attributes of registers for ICU0.IRR7                            */
            __IOM uint32_t SAIR8  : 1; /*!< Security attributes of registers for ICU0.IRR8                            */
            __IOM uint32_t SAIR9  : 1; /*!< Security attributes of registers for ICU0.IRR9                            */
            __IOM uint32_t SAIR10 : 1; /*!< Security attributes of registers for ICU0.IR10                            */
            __IOM uint32_t SAIR11 : 1; /*!< Security attributes of registers for ICU0.IR11                            */
            __IOM uint32_t SAIR12 : 1; /*!< Security attributes of registers for ICU0.IR12                            */
            __IOM uint32_t SAIR13 : 1; /*!< Security attributes of registers for ICU0.IR13                            */
            __IOM uint32_t SAIR14 : 1; /*!< Security attributes of registers for ICU0.IR14                            */
            __IOM uint32_t SAIR15 : 1; /*!< Security attributes of registers for ICU0.IR15                            */
            __IOM uint32_t SAIR16 : 1; /*!< Security attributes of registers for ICU0.IR16                            */
            __IOM uint32_t SAIR17 : 1; /*!< Security attributes of registers for ICU0.IR17                            */
            __IOM uint32_t SAIR18 : 1; /*!< Security attributes of registers for ICU0.IR18                            */
            __IOM uint32_t SAIR19 : 1; /*!< Security attributes of registers for ICU0.IR19                            */
            __IOM uint32_t SAIR20 : 1; /*!< Security attributes of registers for ICU0.IR20                            */
            __IOM uint32_t SAIR21 : 1; /*!< Security attributes of registers for ICU0.IR21                            */
            __IOM uint32_t SAIR22 : 1; /*!< Security attributes of registers for ICU0.IR22                            */
            __IOM uint32_t SAIR23 : 1; /*!< Security attributes of registers for ICU0.IR23                            */
            __IOM uint32_t SAIR24 : 1; /*!< Security attributes of registers for ICU0.IR24                            */
            __IOM uint32_t SAIR25 : 1; /*!< Security attributes of registers for ICU0.IR25                            */
            __IOM uint32_t SAIR26 : 1; /*!< Security attributes of registers for ICU0.IR26                            */
            __IOM uint32_t SAIR27 : 1; /*!< Security attributes of registers for ICU0.IR27                            */
            __IOM uint32_t SAIR28 : 1; /*!< Security attributes of registers for ICU0.IR28                            */
            __IOM uint32_t SAIR29 : 1; /*!< Security attributes of registers for ICU0.IR29                            */
            __IOM uint32_t SAIR30 : 1; /*!< Security attributes of registers for ICU0.IR30                            */
            __IOM uint32_t SAIR31 : 1; /*!< Security attributes of registers for ICU0.IR31                            */
        } ICU0SARA_b;
    };

    union
    {
        __IOM uint32_t ICU0SARB;       /*!< ICU0 Security Attribution Register for B                                  */

        struct
        {
            __IOM uint32_t SAIR0  : 1; /*!< Security attributes of registers for ICU0.IRR0                            */
            __IOM uint32_t SAIR1  : 1; /*!< Security attributes of registers for ICU0.IRR1                            */
            __IOM uint32_t SAIR2  : 1; /*!< Security attributes of registers for ICU0.IRR2                            */
            __IOM uint32_t SAIR3  : 1; /*!< Security attributes of registers for ICU0.IRR3                            */
            __IOM uint32_t SAIR4  : 1; /*!< Security attributes of registers for ICU0.IRR4                            */
            __IOM uint32_t SAIR5  : 1; /*!< Security attributes of registers for ICU0.IRR5                            */
            __IOM uint32_t SAIR6  : 1; /*!< Security attributes of registers for ICU0.IRR6                            */
            __IOM uint32_t SAIR7  : 1; /*!< Security attributes of registers for ICU0.IRR7                            */
            __IOM uint32_t SAIR8  : 1; /*!< Security attributes of registers for ICU0.IRR8                            */
            __IOM uint32_t SAIR9  : 1; /*!< Security attributes of registers for ICU0.IRR9                            */
            __IOM uint32_t SAIR10 : 1; /*!< Security attributes of registers for ICU0.IR10                            */
            __IOM uint32_t SAIR11 : 1; /*!< Security attributes of registers for ICU0.IR11                            */
            __IOM uint32_t SAIR12 : 1; /*!< Security attributes of registers for ICU0.IR12                            */
            __IOM uint32_t SAIR13 : 1; /*!< Security attributes of registers for ICU0.IR13                            */
            __IOM uint32_t SAIR14 : 1; /*!< Security attributes of registers for ICU0.IR14                            */
            __IOM uint32_t SAIR15 : 1; /*!< Security attributes of registers for ICU0.IR15                            */
            __IOM uint32_t SAIR16 : 1; /*!< Security attributes of registers for ICU0.IR16                            */
            __IOM uint32_t SAIR17 : 1; /*!< Security attributes of registers for ICU0.IR17                            */
            __IOM uint32_t SAIR18 : 1; /*!< Security attributes of registers for ICU0.IR18                            */
            __IOM uint32_t SAIR19 : 1; /*!< Security attributes of registers for ICU0.IR19                            */
            __IOM uint32_t SAIR20 : 1; /*!< Security attributes of registers for ICU0.IR20                            */
            __IOM uint32_t SAIR21 : 1; /*!< Security attributes of registers for ICU0.IR21                            */
            __IOM uint32_t SAIR22 : 1; /*!< Security attributes of registers for ICU0.IR22                            */
            __IOM uint32_t SAIR23 : 1; /*!< Security attributes of registers for ICU0.IR23                            */
            __IOM uint32_t SAIR24 : 1; /*!< Security attributes of registers for ICU0.IR24                            */
            __IOM uint32_t SAIR25 : 1; /*!< Security attributes of registers for ICU0.IR25                            */
            __IOM uint32_t SAIR26 : 1; /*!< Security attributes of registers for ICU0.IR26                            */
            __IOM uint32_t SAIR27 : 1; /*!< Security attributes of registers for ICU0.IR27                            */
            __IOM uint32_t SAIR28 : 1; /*!< Security attributes of registers for ICU0.IR28                            */
            __IOM uint32_t SAIR29 : 1; /*!< Security attributes of registers for ICU0.IR29                            */
            __IOM uint32_t SAIR30 : 1; /*!< Security attributes of registers for ICU0.IR30                            */
            __IOM uint32_t SAIR31 : 1; /*!< Security attributes of registers for ICU0.IR31                            */
        } ICU0SARB_b;
    };

    union
    {
        __IOM uint32_t ICU0SARC;       /*!< ICU0 Security Attribution Register for C                                  */

        struct
        {
            __IOM uint32_t SAIR0  : 1; /*!< Security attributes of registers for ICU0.IRR0                            */
            __IOM uint32_t SAIR1  : 1; /*!< Security attributes of registers for ICU0.IRR1                            */
            __IOM uint32_t SAIR2  : 1; /*!< Security attributes of registers for ICU0.IRR2                            */
            __IOM uint32_t SAIR3  : 1; /*!< Security attributes of registers for ICU0.IRR3                            */
            __IOM uint32_t SAIR4  : 1; /*!< Security attributes of registers for ICU0.IRR4                            */
            __IOM uint32_t SAIR5  : 1; /*!< Security attributes of registers for ICU0.IRR5                            */
            __IOM uint32_t SAIR6  : 1; /*!< Security attributes of registers for ICU0.IRR6                            */
            __IOM uint32_t SAIR7  : 1; /*!< Security attributes of registers for ICU0.IRR7                            */
            __IOM uint32_t SAIR8  : 1; /*!< Security attributes of registers for ICU0.IRR8                            */
            __IOM uint32_t SAIR9  : 1; /*!< Security attributes of registers for ICU0.IRR9                            */
            __IOM uint32_t SAIR10 : 1; /*!< Security attributes of registers for ICU0.IR10                            */
            __IOM uint32_t SAIR11 : 1; /*!< Security attributes of registers for ICU0.IR11                            */
            __IOM uint32_t SAIR12 : 1; /*!< Security attributes of registers for ICU0.IR12                            */
            __IOM uint32_t SAIR13 : 1; /*!< Security attributes of registers for ICU0.IR13                            */
            __IOM uint32_t SAIR14 : 1; /*!< Security attributes of registers for ICU0.IR14                            */
            __IOM uint32_t SAIR15 : 1; /*!< Security attributes of registers for ICU0.IR15                            */
            __IOM uint32_t SAIR16 : 1; /*!< Security attributes of registers for ICU0.IR16                            */
            __IOM uint32_t SAIR17 : 1; /*!< Security attributes of registers for ICU0.IR17                            */
            __IOM uint32_t SAIR18 : 1; /*!< Security attributes of registers for ICU0.IR18                            */
            __IOM uint32_t SAIR19 : 1; /*!< Security attributes of registers for ICU0.IR19                            */
            __IOM uint32_t SAIR20 : 1; /*!< Security attributes of registers for ICU0.IR20                            */
            __IOM uint32_t SAIR21 : 1; /*!< Security attributes of registers for ICU0.IR21                            */
            __IOM uint32_t SAIR22 : 1; /*!< Security attributes of registers for ICU0.IR22                            */
            __IOM uint32_t SAIR23 : 1; /*!< Security attributes of registers for ICU0.IR23                            */
            __IOM uint32_t SAIR24 : 1; /*!< Security attributes of registers for ICU0.IR24                            */
            __IOM uint32_t SAIR25 : 1; /*!< Security attributes of registers for ICU0.IR25                            */
            __IOM uint32_t SAIR26 : 1; /*!< Security attributes of registers for ICU0.IR26                            */
            __IOM uint32_t SAIR27 : 1; /*!< Security attributes of registers for ICU0.IR27                            */
            __IOM uint32_t SAIR28 : 1; /*!< Security attributes of registers for ICU0.IR28                            */
            __IOM uint32_t SAIR29 : 1; /*!< Security attributes of registers for ICU0.IR29                            */
            __IOM uint32_t SAIR30 : 1; /*!< Security attributes of registers for ICU0.IR30                            */
            __IOM uint32_t SAIR31 : 1; /*!< Security attributes of registers for ICU0.IR31                            */
        } ICU0SARC_b;
    };

    union
    {
        __IOM uint32_t ICU0SARD;       /*!< ICU0 Security Attribution Register for D                                  */

        struct
        {
            __IOM uint32_t SAIR0  : 1; /*!< Security attributes of registers for ICU0.IRR0                            */
            __IOM uint32_t SAIR1  : 1; /*!< Security attributes of registers for ICU0.IRR1                            */
            __IOM uint32_t SAIR2  : 1; /*!< Security attributes of registers for ICU0.IRR2                            */
            __IOM uint32_t SAIR3  : 1; /*!< Security attributes of registers for ICU0.IRR3                            */
            __IOM uint32_t SAIR4  : 1; /*!< Security attributes of registers for ICU0.IRR4                            */
            __IOM uint32_t SAIR5  : 1; /*!< Security attributes of registers for ICU0.IRR5                            */
            __IOM uint32_t SAIR6  : 1; /*!< Security attributes of registers for ICU0.IRR6                            */
            __IOM uint32_t SAIR7  : 1; /*!< Security attributes of registers for ICU0.IRR7                            */
            __IOM uint32_t SAIR8  : 1; /*!< Security attributes of registers for ICU0.IRR8                            */
            __IOM uint32_t SAIR9  : 1; /*!< Security attributes of registers for ICU0.IRR9                            */
            __IOM uint32_t SAIR10 : 1; /*!< Security attributes of registers for ICU0.IR10                            */
            __IOM uint32_t SAIR11 : 1; /*!< Security attributes of registers for ICU0.IR11                            */
            __IOM uint32_t SAIR12 : 1; /*!< Security attributes of registers for ICU0.IR12                            */
            __IOM uint32_t SAIR13 : 1; /*!< Security attributes of registers for ICU0.IR13                            */
            __IOM uint32_t SAIR14 : 1; /*!< Security attributes of registers for ICU0.IR14                            */
            __IOM uint32_t SAIR15 : 1; /*!< Security attributes of registers for ICU0.IR15                            */
            __IOM uint32_t SAIR16 : 1; /*!< Security attributes of registers for ICU0.IR16                            */
            __IOM uint32_t SAIR17 : 1; /*!< Security attributes of registers for ICU0.IR17                            */
            __IOM uint32_t SAIR18 : 1; /*!< Security attributes of registers for ICU0.IR18                            */
            __IOM uint32_t SAIR19 : 1; /*!< Security attributes of registers for ICU0.IR19                            */
            __IOM uint32_t SAIR20 : 1; /*!< Security attributes of registers for ICU0.IR20                            */
            __IOM uint32_t SAIR21 : 1; /*!< Security attributes of registers for ICU0.IR21                            */
            __IOM uint32_t SAIR22 : 1; /*!< Security attributes of registers for ICU0.IR22                            */
            __IOM uint32_t SAIR23 : 1; /*!< Security attributes of registers for ICU0.IR23                            */
            __IOM uint32_t SAIR24 : 1; /*!< Security attributes of registers for ICU0.IR24                            */
            __IOM uint32_t SAIR25 : 1; /*!< Security attributes of registers for ICU0.IR25                            */
            __IOM uint32_t SAIR26 : 1; /*!< Security attributes of registers for ICU0.IR26                            */
            __IOM uint32_t SAIR27 : 1; /*!< Security attributes of registers for ICU0.IR27                            */
            __IOM uint32_t SAIR28 : 1; /*!< Security attributes of registers for ICU0.IR28                            */
            __IOM uint32_t SAIR29 : 1; /*!< Security attributes of registers for ICU0.IR29                            */
            __IOM uint32_t SAIR30 : 1; /*!< Security attributes of registers for ICU0.IR30                            */
            __IOM uint32_t SAIR31 : 1; /*!< Security attributes of registers for ICU0.IR31                            */
        } ICU0SARD_b;
    };

    union
    {
        __IOM uint32_t ICU0SARE;       /*!< ICU0 Security Attribution Register for E                                  */

        struct
        {
            __IOM uint32_t SAIR0  : 1; /*!< Security attributes of registers for ICU0.IRR0                            */
            __IOM uint32_t SAIR1  : 1; /*!< Security attributes of registers for ICU0.IRR1                            */
            __IOM uint32_t SAIR2  : 1; /*!< Security attributes of registers for ICU0.IRR2                            */
            __IOM uint32_t SAIR3  : 1; /*!< Security attributes of registers for ICU0.IRR3                            */
            __IOM uint32_t SAIR4  : 1; /*!< Security attributes of registers for ICU0.IRR4                            */
            __IOM uint32_t SAIR5  : 1; /*!< Security attributes of registers for ICU0.IRR5                            */
            __IOM uint32_t SAIR6  : 1; /*!< Security attributes of registers for ICU0.IRR6                            */
            __IOM uint32_t SAIR7  : 1; /*!< Security attributes of registers for ICU0.IRR7                            */
            __IOM uint32_t SAIR8  : 1; /*!< Security attributes of registers for ICU0.IRR8                            */
            __IOM uint32_t SAIR9  : 1; /*!< Security attributes of registers for ICU0.IRR9                            */
            __IOM uint32_t SAIR10 : 1; /*!< Security attributes of registers for ICU0.IR10                            */
            __IOM uint32_t SAIR11 : 1; /*!< Security attributes of registers for ICU0.IR11                            */
            __IOM uint32_t SAIR12 : 1; /*!< Security attributes of registers for ICU0.IR12                            */
            __IOM uint32_t SAIR13 : 1; /*!< Security attributes of registers for ICU0.IR13                            */
            __IOM uint32_t SAIR14 : 1; /*!< Security attributes of registers for ICU0.IR14                            */
            __IOM uint32_t SAIR15 : 1; /*!< Security attributes of registers for ICU0.IR15                            */
            __IOM uint32_t SAIR16 : 1; /*!< Security attributes of registers for ICU0.IR16                            */
            __IOM uint32_t SAIR17 : 1; /*!< Security attributes of registers for ICU0.IR17                            */
            __IOM uint32_t SAIR18 : 1; /*!< Security attributes of registers for ICU0.IR18                            */
            __IOM uint32_t SAIR19 : 1; /*!< Security attributes of registers for ICU0.IR19                            */
            __IOM uint32_t SAIR20 : 1; /*!< Security attributes of registers for ICU0.IR20                            */
            __IOM uint32_t SAIR21 : 1; /*!< Security attributes of registers for ICU0.IR21                            */
            __IOM uint32_t SAIR22 : 1; /*!< Security attributes of registers for ICU0.IR22                            */
            __IOM uint32_t SAIR23 : 1; /*!< Security attributes of registers for ICU0.IR23                            */
            __IOM uint32_t SAIR24 : 1; /*!< Security attributes of registers for ICU0.IR24                            */
            __IOM uint32_t SAIR25 : 1; /*!< Security attributes of registers for ICU0.IR25                            */
            __IOM uint32_t SAIR26 : 1; /*!< Security attributes of registers for ICU0.IR26                            */
            __IOM uint32_t SAIR27 : 1; /*!< Security attributes of registers for ICU0.IR27                            */
            __IOM uint32_t SAIR28 : 1; /*!< Security attributes of registers for ICU0.IR28                            */
            __IOM uint32_t SAIR29 : 1; /*!< Security attributes of registers for ICU0.IR29                            */
            __IOM uint32_t SAIR30 : 1; /*!< Security attributes of registers for ICU0.IR30                            */
            __IOM uint32_t SAIR31 : 1; /*!< Security attributes of registers for ICU0.IR31                            */
        } ICU0SARE_b;
    };

    union
    {
        __IOM uint32_t ICU0SARF;       /*!< ICU0 Security Attribution Register for F                                  */

        struct
        {
            __IOM uint32_t SAIR0  : 1; /*!< Security attributes of registers for ICU0.IRR0                            */
            __IOM uint32_t SAIR1  : 1; /*!< Security attributes of registers for ICU0.IRR1                            */
            __IOM uint32_t SAIR2  : 1; /*!< Security attributes of registers for ICU0.IRR2                            */
            __IOM uint32_t SAIR3  : 1; /*!< Security attributes of registers for ICU0.IRR3                            */
            __IOM uint32_t SAIR4  : 1; /*!< Security attributes of registers for ICU0.IRR4                            */
            __IOM uint32_t SAIR5  : 1; /*!< Security attributes of registers for ICU0.IRR5                            */
            __IOM uint32_t SAIR6  : 1; /*!< Security attributes of registers for ICU0.IRR6                            */
            __IOM uint32_t SAIR7  : 1; /*!< Security attributes of registers for ICU0.IRR7                            */
            __IOM uint32_t SAIR8  : 1; /*!< Security attributes of registers for ICU0.IRR8                            */
            __IOM uint32_t SAIR9  : 1; /*!< Security attributes of registers for ICU0.IRR9                            */
            __IOM uint32_t SAIR10 : 1; /*!< Security attributes of registers for ICU0.IR10                            */
            __IOM uint32_t SAIR11 : 1; /*!< Security attributes of registers for ICU0.IR11                            */
            __IOM uint32_t SAIR12 : 1; /*!< Security attributes of registers for ICU0.IR12                            */
            __IOM uint32_t SAIR13 : 1; /*!< Security attributes of registers for ICU0.IR13                            */
            __IOM uint32_t SAIR14 : 1; /*!< Security attributes of registers for ICU0.IR14                            */
            __IOM uint32_t SAIR15 : 1; /*!< Security attributes of registers for ICU0.IR15                            */
            __IOM uint32_t SAIR16 : 1; /*!< Security attributes of registers for ICU0.IR16                            */
            __IOM uint32_t SAIR17 : 1; /*!< Security attributes of registers for ICU0.IR17                            */
            __IOM uint32_t SAIR18 : 1; /*!< Security attributes of registers for ICU0.IR18                            */
            __IOM uint32_t SAIR19 : 1; /*!< Security attributes of registers for ICU0.IR19                            */
            __IOM uint32_t SAIR20 : 1; /*!< Security attributes of registers for ICU0.IR20                            */
            __IOM uint32_t SAIR21 : 1; /*!< Security attributes of registers for ICU0.IR21                            */
            __IOM uint32_t SAIR22 : 1; /*!< Security attributes of registers for ICU0.IR22                            */
            __IOM uint32_t SAIR23 : 1; /*!< Security attributes of registers for ICU0.IR23                            */
            __IOM uint32_t SAIR24 : 1; /*!< Security attributes of registers for ICU0.IR24                            */
            __IOM uint32_t SAIR25 : 1; /*!< Security attributes of registers for ICU0.IR25                            */
            __IOM uint32_t SAIR26 : 1; /*!< Security attributes of registers for ICU0.IR26                            */
            __IOM uint32_t SAIR27 : 1; /*!< Security attributes of registers for ICU0.IR27                            */
            __IOM uint32_t SAIR28 : 1; /*!< Security attributes of registers for ICU0.IR28                            */
            __IOM uint32_t SAIR29 : 1; /*!< Security attributes of registers for ICU0.IR29                            */
            __IOM uint32_t SAIR30 : 1; /*!< Security attributes of registers for ICU0.IR30                            */
            __IOM uint32_t SAIR31 : 1; /*!< Security attributes of registers for ICU0.IR31                            */
        } ICU0SARF_b;
    };

    union
    {
        __IOM uint32_t ICU0SARG;       /*!< ICU0 Security Attribution Register for G                                  */

        struct
        {
            __IOM uint32_t SAIR0  : 1; /*!< Security attributes of registers for ICU0.IRR0                            */
            __IOM uint32_t SAIR1  : 1; /*!< Security attributes of registers for ICU0.IRR1                            */
            __IOM uint32_t SAIR2  : 1; /*!< Security attributes of registers for ICU0.IRR2                            */
            __IOM uint32_t SAIR3  : 1; /*!< Security attributes of registers for ICU0.IRR3                            */
            __IOM uint32_t SAIR4  : 1; /*!< Security attributes of registers for ICU0.IRR4                            */
            __IOM uint32_t SAIR5  : 1; /*!< Security attributes of registers for ICU0.IRR5                            */
            __IOM uint32_t SAIR6  : 1; /*!< Security attributes of registers for ICU0.IRR6                            */
            __IOM uint32_t SAIR7  : 1; /*!< Security attributes of registers for ICU0.IRR7                            */
            __IOM uint32_t SAIR8  : 1; /*!< Security attributes of registers for ICU0.IRR8                            */
            __IOM uint32_t SAIR9  : 1; /*!< Security attributes of registers for ICU0.IRR9                            */
            __IOM uint32_t SAIR10 : 1; /*!< Security attributes of registers for ICU0.IR10                            */
            __IOM uint32_t SAIR11 : 1; /*!< Security attributes of registers for ICU0.IR11                            */
            __IOM uint32_t SAIR12 : 1; /*!< Security attributes of registers for ICU0.IR12                            */
            __IOM uint32_t SAIR13 : 1; /*!< Security attributes of registers for ICU0.IR13                            */
            __IOM uint32_t SAIR14 : 1; /*!< Security attributes of registers for ICU0.IR14                            */
            __IOM uint32_t SAIR15 : 1; /*!< Security attributes of registers for ICU0.IR15                            */
            __IOM uint32_t SAIR16 : 1; /*!< Security attributes of registers for ICU0.IR16                            */
            __IOM uint32_t SAIR17 : 1; /*!< Security attributes of registers for ICU0.IR17                            */
            __IOM uint32_t SAIR18 : 1; /*!< Security attributes of registers for ICU0.IR18                            */
            __IOM uint32_t SAIR19 : 1; /*!< Security attributes of registers for ICU0.IR19                            */
            __IOM uint32_t SAIR20 : 1; /*!< Security attributes of registers for ICU0.IR20                            */
            __IOM uint32_t SAIR21 : 1; /*!< Security attributes of registers for ICU0.IR21                            */
            __IOM uint32_t SAIR22 : 1; /*!< Security attributes of registers for ICU0.IR22                            */
            __IOM uint32_t SAIR23 : 1; /*!< Security attributes of registers for ICU0.IR23                            */
            __IOM uint32_t SAIR24 : 1; /*!< Security attributes of registers for ICU0.IR24                            */
            __IOM uint32_t SAIR25 : 1; /*!< Security attributes of registers for ICU0.IR25                            */
            __IOM uint32_t SAIR26 : 1; /*!< Security attributes of registers for ICU0.IR26                            */
            __IOM uint32_t SAIR27 : 1; /*!< Security attributes of registers for ICU0.IR27                            */
            __IOM uint32_t SAIR28 : 1; /*!< Security attributes of registers for ICU0.IR28                            */
            __IOM uint32_t SAIR29 : 1; /*!< Security attributes of registers for ICU0.IR29                            */
            __IOM uint32_t SAIR30 : 1; /*!< Security attributes of registers for ICU0.IR30                            */
            __IOM uint32_t SAIR31 : 1; /*!< Security attributes of registers for ICU0.IR31                            */
        } ICU0SARG_b;
    };

    union
    {
        __IOM uint32_t ICU0SARH;       /*!< ICU0 Security Attribution Register for H                                  */

        struct
        {
            __IOM uint32_t SAIR0  : 1; /*!< Security attributes of registers for ICU0.IRR0                            */
            __IOM uint32_t SAIR1  : 1; /*!< Security attributes of registers for ICU0.IRR1                            */
            __IOM uint32_t SAIR2  : 1; /*!< Security attributes of registers for ICU0.IRR2                            */
            __IOM uint32_t SAIR3  : 1; /*!< Security attributes of registers for ICU0.IRR3                            */
            __IOM uint32_t SAIR4  : 1; /*!< Security attributes of registers for ICU0.IRR4                            */
            __IOM uint32_t SAIR5  : 1; /*!< Security attributes of registers for ICU0.IRR5                            */
            __IOM uint32_t SAIR6  : 1; /*!< Security attributes of registers for ICU0.IRR6                            */
            __IOM uint32_t SAIR7  : 1; /*!< Security attributes of registers for ICU0.IRR7                            */
            __IOM uint32_t SAIR8  : 1; /*!< Security attributes of registers for ICU0.IRR8                            */
            __IOM uint32_t SAIR9  : 1; /*!< Security attributes of registers for ICU0.IRR9                            */
            __IOM uint32_t SAIR10 : 1; /*!< Security attributes of registers for ICU0.IR10                            */
            __IOM uint32_t SAIR11 : 1; /*!< Security attributes of registers for ICU0.IR11                            */
            __IOM uint32_t SAIR12 : 1; /*!< Security attributes of registers for ICU0.IR12                            */
            __IOM uint32_t SAIR13 : 1; /*!< Security attributes of registers for ICU0.IR13                            */
            __IOM uint32_t SAIR14 : 1; /*!< Security attributes of registers for ICU0.IR14                            */
            __IOM uint32_t SAIR15 : 1; /*!< Security attributes of registers for ICU0.IR15                            */
            __IOM uint32_t SAIR16 : 1; /*!< Security attributes of registers for ICU0.IR16                            */
            __IOM uint32_t SAIR17 : 1; /*!< Security attributes of registers for ICU0.IR17                            */
            __IOM uint32_t SAIR18 : 1; /*!< Security attributes of registers for ICU0.IR18                            */
            __IOM uint32_t SAIR19 : 1; /*!< Security attributes of registers for ICU0.IR19                            */
            __IOM uint32_t SAIR20 : 1; /*!< Security attributes of registers for ICU0.IR20                            */
            __IOM uint32_t SAIR21 : 1; /*!< Security attributes of registers for ICU0.IR21                            */
            __IOM uint32_t SAIR22 : 1; /*!< Security attributes of registers for ICU0.IR22                            */
            __IOM uint32_t SAIR23 : 1; /*!< Security attributes of registers for ICU0.IR23                            */
            __IOM uint32_t SAIR24 : 1; /*!< Security attributes of registers for ICU0.IR24                            */
            __IOM uint32_t SAIR25 : 1; /*!< Security attributes of registers for ICU0.IR25                            */
            __IOM uint32_t SAIR26 : 1; /*!< Security attributes of registers for ICU0.IR26                            */
            __IOM uint32_t SAIR27 : 1; /*!< Security attributes of registers for ICU0.IR27                            */
            __IOM uint32_t SAIR28 : 1; /*!< Security attributes of registers for ICU0.IR28                            */
            __IOM uint32_t SAIR29 : 1; /*!< Security attributes of registers for ICU0.IR29                            */
            __IOM uint32_t SAIR30 : 1; /*!< Security attributes of registers for ICU0.IR30                            */
            __IOM uint32_t SAIR31 : 1; /*!< Security attributes of registers for ICU0.IR31                            */
        } ICU0SARH_b;
    };
    __IM uint32_t RESERVED4[16];

    union
    {
        __IOM uint32_t BUSSARA;        /*!< BUS Security Attribution Register A                                       */

        struct
        {
            __IOM uint32_t BUSSA0 : 1; /*!< BUS Security attributes A0.                                               */
            uint32_t              : 31;
        } BUSSARA_b;
    };

    union
    {
        __IOM uint32_t BUSSARB;        /*!< BUS Security Attribution Register B                                       */

        struct
        {
            __IOM uint32_t BUSSB0 : 1; /*!< BUS Security attributes B0.                                               */
            uint32_t              : 31;
        } BUSSARB_b;
    };
    __IM uint32_t RESERVED5[2];

    union
    {
        __IOM uint32_t BUSSARC;        /*!< BUS Security Attribution Register C (External BUS)                        */

        struct
        {
            __IOM uint32_t BUSSC0 : 1; /*!< BUS Security Attribution C0.                                              */
            uint32_t              : 31;
        } BUSSARC_b;
    };
    __IM uint32_t RESERVED6[8];

    union
    {
        __IOM uint32_t MMPUSARB;       /*!< Master MPU Security Attribution Register B                                */

        struct
        {
            uint32_t                : 8;
            __IOM uint32_t MMPUBSA8 : 1; /*!< MMPUB8 Security attributes.                                               */
            uint32_t                : 23;
        } MMPUSARB_b;
    };
    __IM uint32_t RESERVED7[2];

    union
    {
        __IOM uint32_t TFUSAR;         /*!< TFU Security Attribution Register                                         */

        struct
        {
            __IOM uint32_t TFUSA : 1;  /*!< TFU Security attributes.                                                  */
            uint32_t             : 31;
        } TFUSAR_b;
    };

    union
    {
        __IOM uint32_t IFUSAR;         /*!< IFU Security Attribution Register                                         */

        struct
        {
            __IOM uint32_t IFUSA : 1;  /*!< This bit set the security attribution of the registers of CPU
                                        *   Branch operation Accelerator (CPUBAEN/CPUBAINV).                          */
            uint32_t : 31;
        } IFUSAR_b;
    };
    __IM uint32_t RESERVED8[14];

    union
    {
        __IOM uint32_t DEBUGSAR;       /*!< Debug Security Attribution Register                                       */

        struct
        {
            __IOM uint32_t DBGSA0 : 1; /*!< Security attributes of registers for the DEBUG register                   */
            uint32_t              : 31;
        } DEBUGSAR_b;
    };
    __IM uint32_t RESERVED9[6];

    union
    {
        __IOM uint32_t DMACXCHSAR;        /*!< DMACX channel Security Attribution Register                               */

        struct
        {
            __IOM uint32_t SADMACX00 : 1; /*!< Security attributes of registers for DMACX channel 0                      */
            __IOM uint32_t SADMACX01 : 1; /*!< Security attributes of registers for DMACX channel 1                      */
            uint32_t                 : 30;
        } DMACXCHSAR_b;
    };

    union
    {
        __IOM uint32_t DMACCHSAR;         /*!< DMAC channel Security Attribution Register                                */

        struct
        {
            __IOM uint32_t SADMAC000 : 1; /*!< Security attributes of registers for DMAC0 channel 0                      */
            __IOM uint32_t SADMAC001 : 1; /*!< Security attributes of registers for DMAC0 channel 1                      */
            __IOM uint32_t SADMAC002 : 1; /*!< Security attributes of registers for DMAC0 channel 2                      */
            __IOM uint32_t SADMAC003 : 1; /*!< Security attributes of registers for DMAC0 channel 3                      */
            __IOM uint32_t SADMAC004 : 1; /*!< Security attributes of registers for DMAC0 channel 4                      */
            __IOM uint32_t SADMAC005 : 1; /*!< Security attributes of registers for DMAC0 channel 5                      */
            __IOM uint32_t SADMAC006 : 1; /*!< Security attributes of registers for DMAC0 channel 6                      */
            __IOM uint32_t SADMAC007 : 1; /*!< Security attributes of registers for DMAC0 channel 7                      */
            uint32_t                 : 24;
        } DMACCHSAR_b;
    };
    __IM uint32_t RESERVED10[151];

    union
    {
        __IOM uint32_t SRAMSABAR0;     /*!< SRAM Security Attribute Boundary Address Register 0                       */

        struct
        {
            uint32_t          : 13;
            __IOM uint32_t BA : 8;     /*!< SRAM secure area Indicate the area of secure region for SRAM              */
            uint32_t          : 11;
        } SRAMSABAR0_b;
    };

    union
    {
        __IOM uint32_t SRAMSABAR1;     /*!< SRAM Security Attribute Boundary Address Register 1                       */

        struct
        {
            uint32_t          : 13;
            __IOM uint32_t BA : 8;     /*!< SRAM secure area Indicate the area of secure region for SRAM              */
            uint32_t          : 11;
        } SRAMSABAR1_b;
    };
    __IM uint32_t RESERVED11[62];

    union
    {
        __IOM uint32_t CACHESAR;         /*!< Cache Security Attribution Register                                       */

        struct
        {
            __IOM uint32_t CACHESA  : 1; /*!< Security attributes of registers for CACHE Control                        */
            uint32_t                : 1;
            __IOM uint32_t CACHEESA : 1; /*!< Security attributes of registers for CACHE Error                          */
            uint32_t                : 29;
        } CACHESAR_b;
    };

    union
    {
        __IOM uint32_t LRMSAR;         /*!< LRAM Security Attribution Register                                        */

        struct
        {
            __IOM uint32_t LRMSA : 1;  /*!< Security Attributies of registers for LRAMTCM Control                     */
            uint32_t             : 31;
        } LRMSAR_b;
    };

    union
    {
        __IOM uint32_t LRMSABAR0;      /*!< LRAM (C-TCM) Security Attribute Boundary Address Register                 */

        struct
        {
            uint32_t                 : 13;
            __IOM uint32_t LRTCMSABA : 6; /*!< Security Attribute Boundary Address.                                      */
            uint32_t                 : 13;
        } LRMSABAR0_b;
    };

    union
    {
        __IOM uint32_t LRMSABAR1;      /*!< LRAM (S-TCM) Security Attribute Boundary Address Register                 */

        struct
        {
            uint32_t                 : 13;
            __IOM uint32_t LRTCMSABA : 6; /*!< Security Attribute Boundary Address.                                      */
            uint32_t                 : 13;
        } LRMSABAR1_b;
    };
    __IM uint32_t RESERVED12[60];

    union
    {
        __IOM uint32_t TEVTRCR;           /*!< Trusted EVenT Route Control Register                                      */

        struct
        {
            __IOM uint32_t TEVTE     : 1; /*!< Trusted Event Route Control for ELC                                       */
            __IOM uint32_t TEVTEICU0 : 1; /*!< Trusted Event Route Control for ICU0                                      */
            uint32_t                 : 30;
        } TEVTRCR_b;
    };

    union
    {
        __IOM uint32_t ICUNSACR;       /*!< ICU Non-Secure Access Control Register                                    */

        struct
        {
            __IOM uint32_t NSACE : 1;  /*!< Non-Secure Access Enable to PIXRn                                         */
            uint32_t             : 31;
        } ICUNSACR_b;
    };
    __IM uint32_t RESERVED13[6];

    union
    {
        __IOM uint32_t PRTZCR;         /*!< Protect Zone Control Register                                             */

        struct
        {
            __IOM uint32_t PRTZC : 1;  /*!< Protect Zone Control                                                      */
            uint32_t             : 31;
        } PRTZCR_b;
    };
} R_CPSCU_Type;                        /*!< Size = 1572 (0x624)                                                       */

#endif /* R_CPSCU_REG_H */
