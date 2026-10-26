#ifndef R_DAC_REG_H
#define R_DAC_REG_H

/* =========================================================================================================================== */
/* ================                                           R_DAC                                           ================ */
/* =========================================================================================================================== */

/**
 * @brief 12-Bit D/A Converter (R_DAC)
 */

typedef struct                         /*!< (@ 0x00088040) R_DAC Structure                                            */
{
    union
    {
        __IOM uint16_t DADR[2];        /*!< (@ 0x00000000) D/A Data Register                                          */

        struct
        {
            __IOM uint16_t DADR : 16;  /*!< [15..0] When DADPR.DPSEL = 0, the high-order 4 bits are fixed
                                        * to 0: right justified format.When DADPR.DPSEL = 1, the
                                        * high-order 4 bits are fixed to 0: left justified format.                  */
        } DADR_b[2];
    };

    union
    {
        __IOM uint8_t DACR;            /*!< (@ 0x00000004) D/A Control Register                                       */

        struct
        {
            uint8_t             : 5;
            __IOM uint8_t DAE   : 1;   /*!< [5..5] D/A Enable                                                         */
            __IOM uint8_t DAOE0 : 1;   /*!< [6..6] D/A Output Enable 0                                                */
            __IOM uint8_t DAOE1 : 1;   /*!< [7..7] D/A Output Enable 1                                                */
        } DACR_b;
    };

    union
    {
        __IOM uint8_t DADPR;           /*!< (@ 0x00000005) Data Register Format Select Register                       */

        struct
        {
            uint8_t             : 7;
            __IOM uint8_t DPSEL : 1;   /*!< [7..7] Format Select                                                      */
        } DADPR_b;
    };

    union
    {
        __IOM uint8_t DAADSCR;         /*!< (@ 0x00000006) D/A A/D Synchronous Start Control Register                 */

        struct
        {
            uint8_t              : 7;
            __IOM uint8_t DAADST : 1;  /*!< [7..7] D/A A/D Synchronous Conversion                                     */
        } DAADSCR_b;
    };
    __IM uint8_t RESERVED;

    union
    {
        __IOM uint8_t DAAMPCR;         /*!< (@ 0x00000008) D/A Output Amplifier Control Register                      */

        struct
        {
            uint8_t              : 6;
            __IOM uint8_t DAAMP1 : 1;  /*!< [6..6] Amplifier Control 1                                                */
            __IOM uint8_t DAAMP0 : 1;  /*!< [7..7] Amplifier Control 0                                                */
        } DAAMPCR_b;
    };
    __IM uint8_t  RESERVED1;
    __IM uint16_t RESERVED2[9];

    union
    {
        __IOM uint8_t DAASWCR;         /*!< (@ 0x0000001C) D/A Output Amplifier Stabilization Wait Control
                                        *                  Register                                                   */

        struct
        {
            uint8_t              : 6;
            __IOM uint8_t DAASW0 : 1;  /*!< [6..6] Channel 0 Output Amplifier Stabilization Waiting                   */
            __IOM uint8_t DAASW1 : 1;  /*!< [7..7] Channel 1 Output Amplifier Stabilization Waiting                   */
        } DAASWCR_b;
    };
    __IM uint8_t  RESERVED3;
    __IM uint16_t RESERVED4[8881];

    union
    {
        __IOM uint8_t DAADUSR;         /*!< (@ 0x00004580) D/A A/D Synchronous Unit Select Register                   */

        struct
        {
            uint8_t                : 1;
            __IOM uint8_t AMADSEL1 : 1; /*!< [1..1] A/D Unit 1 Select                                                  */
            uint8_t                : 6;
        } DAADUSR_b;
    };
} R_DAC_Type;                           /*!< Size = 17796 (0x4584)                                                     */

#endif /* R_DAC_REG_H */
