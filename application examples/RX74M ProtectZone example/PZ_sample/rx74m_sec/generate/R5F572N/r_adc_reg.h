#ifndef R_ADC_REG_H
#define R_ADC_REG_H

/* =========================================================================================================================== */
/* ================                                           R_ADC                                           ================ */
/* =========================================================================================================================== */

/**
 * @brief 12-Bit A/D Converter (R_ADC)
 */

typedef struct                         /*!< (@ 0x00089000) R_ADC Structure                                            */
{
    union
    {
        __IOM uint16_t ADCSR;          /*!< (@ 0x00000000) A/D Control Register                                       */

        struct
        {
            __IOM uint16_t DBLANS : 5; /*!< [4..0] Double Trigger Channel Select                                      */
            uint16_t              : 1;
            __IOM uint16_t GBADIE : 1; /*!< [6..6] Group B Scan End Interrupt Enable                                  */
            __IOM uint16_t DBLE   : 1; /*!< [7..7] Double Trigger Mode Select                                         */
            __IOM uint16_t EXTRG  : 1; /*!< [8..8] Trigger Select                                                     */
            __IOM uint16_t TRGE   : 1; /*!< [9..9] Trigger Start Enable                                               */
            uint16_t              : 2;
            __IOM uint16_t ADIE   : 1; /*!< [12..12] Scan End Interrupt Enable                                        */
            __IOM uint16_t ADCS   : 2; /*!< [14..13] Scan Mode Select                                                 */
            __IOM uint16_t ADST   : 1; /*!< [15..15] Scan End Interrupt Enable                                        */
        } ADCSR_b;
    };
    __IM uint16_t RESERVED;

    union
    {
        __IOM uint16_t ADANSA[2];       /*!< (@ 0x00000004) A/D Channel Select Register A0                             */

        struct
        {
            __IOM uint16_t ANSA000 : 1; /*!< [0..0] AN Input Select                                                    */
            __IOM uint16_t ANSA001 : 1; /*!< [1..1] AN Input Select                                                    */
            __IOM uint16_t ANSA002 : 1; /*!< [2..2] AN Input Select                                                    */
            __IOM uint16_t ANSA003 : 1; /*!< [3..3] AN Input Select                                                    */
            __IOM uint16_t ANSA004 : 1; /*!< [4..4] AN Input Select                                                    */
            __IOM uint16_t ANSA005 : 1; /*!< [5..5] AN Input Select                                                    */
            __IOM uint16_t ANSA006 : 1; /*!< [6..6] AN Input Select                                                    */
            __IOM uint16_t ANSA007 : 1; /*!< [7..7] AN Input Select                                                    */
            __IOM uint16_t ANSA008 : 1; /*!< [8..8] AN Input Select                                                    */
            __IOM uint16_t ANSA009 : 1; /*!< [9..9] AN Input Select                                                    */
            __IOM uint16_t ANSA010 : 1; /*!< [10..10] AN Input Select                                                  */
            __IOM uint16_t ANSA011 : 1; /*!< [11..11] AN Input Select                                                  */
            __IOM uint16_t ANSA012 : 1; /*!< [12..12] AN Input Select                                                  */
            __IOM uint16_t ANSA013 : 1; /*!< [13..13] AN Input Select                                                  */
            __IOM uint16_t ANSA014 : 1; /*!< [14..14] AN Input Select                                                  */
            __IOM uint16_t ANSA015 : 1; /*!< [15..15] AN Input Select                                                  */
        } ADANSA_b[2];
    };

    union
    {
        __IOM uint16_t ADADS[2];       /*!< (@ 0x00000008) A/D-Converted Value Addition/Average Function
                                        *                  Channel Select Register 0                                  */

        struct
        {
            __IOM uint16_t ADS000 : 1; /*!< [0..0] A/D-Converted Value Addition/Average Channel Select                */
            __IOM uint16_t ADS001 : 1; /*!< [1..1] A/D-Converted Value Addition/Average Channel Select                */
            __IOM uint16_t ADS002 : 1; /*!< [2..2] A/D-Converted Value Addition/Average Channel Select                */
            __IOM uint16_t ADS003 : 1; /*!< [3..3] A/D-Converted Value Addition/Average Channel Select                */
            __IOM uint16_t ADS004 : 1; /*!< [4..4] A/D-Converted Value Addition/Average Channel Select                */
            __IOM uint16_t ADS005 : 1; /*!< [5..5] A/D-Converted Value Addition/Average Channel Select                */
            __IOM uint16_t ADS006 : 1; /*!< [6..6] A/D-Converted Value Addition/Average Channel Select                */
            __IOM uint16_t ADS007 : 1; /*!< [7..7] A/D-Converted Value Addition/Average Channel Select                */
            __IOM uint16_t ADS008 : 1; /*!< [8..8] A/D-Converted Value Addition/Average Channel Select                */
            __IOM uint16_t ADS009 : 1; /*!< [9..9] A/D-Converted Value Addition/Average Channel Select                */
            __IOM uint16_t ADS010 : 1; /*!< [10..10] A/D-Converted Value Addition/Average Channel Select              */
            __IOM uint16_t ADS011 : 1; /*!< [11..11] A/D-Converted Value Addition/Average Channel Select              */
            __IOM uint16_t ADS012 : 1; /*!< [12..12] A/D-Converted Value Addition/Average Channel Select              */
            __IOM uint16_t ADS013 : 1; /*!< [13..13] A/D-Converted Value Addition/Average Channel Select              */
            __IOM uint16_t ADS014 : 1; /*!< [14..14] A/D-Converted Value Addition/Average Channel Select              */
            __IOM uint16_t ADS015 : 1; /*!< [15..15] A/D-Converted Value Addition/Average Channel Select              */
        } ADADS_b[2];
    };

    union
    {
        __IOM uint8_t ADADC;           /*!< (@ 0x0000000C) A/D-Converted Value Addition/Average Count Select
                                        *                  Register                                                   */

        struct
        {
            __IOM uint8_t ADC  : 3;    /*!< [2..0] Addition Count Select                                              */
            uint8_t            : 4;
            __IOM uint8_t AVEE : 1;    /*!< [7..7] Average Mode Enable                                                */
        } ADADC_b;
    };
    __IM uint8_t RESERVED1;

    union
    {
        __IOM uint16_t ADCER;          /*!< (@ 0x0000000E) A/D Control Extended Register                              */

        struct
        {
            uint16_t               : 1;
            __IOM uint16_t ADPRC   : 2; /*!< [2..1] Double Trigger Channel Select                                      */
            uint16_t               : 2;
            __IOM uint16_t ACE     : 1; /*!< [5..5] A/D Data Register Automatic Clearing Enable                        */
            uint16_t               : 2;
            __IOM uint16_t DIAGVAL : 2; /*!< [9..8] Self-Diagnosis Conversion Voltage Select                           */
            __IOM uint16_t DIAGLD  : 1; /*!< [10..10] Self-Diagnosis Mode Select                                       */
            __IOM uint16_t DIAGM   : 1; /*!< [11..11] Self-Diagnosis Enable                                            */
            uint16_t               : 3;
            __IOM uint16_t ADRFMT  : 1; /*!< [15..15] A/D Data Register Format Select                                  */
        } ADCER_b;
    };

    union
    {
        __IOM uint16_t ADSTRGR;        /*!< (@ 0x00000010) A/D Conversion Start Trigger Select Register               */

        struct
        {
            __IOM uint16_t TRSB : 6;   /*!< [5..0] A/D Conversion Start Trigger Select for Group B                    */
            uint16_t            : 2;
            __IOM uint16_t TRSA : 6;   /*!< [13..8] A/D Conversion Start Trigger Select                               */
            uint16_t            : 2;
        } ADSTRGR_b;
    };

    union
    {
        __IOM uint16_t ADEXICR;        /*!< (@ 0x00000012) A/D Conversion Extended Input Control Register             */

        struct
        {
            __IOM uint16_t TSSAD : 1;  /*!< [0..0] Temperature Sensor Output A/D converted Value Addition/Average
                                        *   Mode Select                                                               */
            __IOM uint16_t OCSAD : 1;  /*!< [1..1] Internal Reference Voltage A/D converted Value Addition/Average
                                        *   Mode Select                                                               */
            uint16_t             : 6;
            __IOM uint16_t TSSA  : 1;  /*!< [8..8] Temperature Sensor Output A/D Conversion Select                    */
            __IOM uint16_t OCSA  : 1;  /*!< [9..9] Internal Reference Voltage A/D Conversion Select                   */
            __IOM uint16_t TSSB  : 1;  /*!< [10..10] Temperature Sensor Output A/D Conversion Select                  */
            __IOM uint16_t OCSB  : 1;  /*!< [11..11] Internal Reference Voltage A/D Conversion Select                 */
            uint16_t             : 1;
            __IOM uint16_t EXSEL : 2;  /*!< [14..13] Extended Analog Input Select                                     */
            __IOM uint16_t EXOEN : 1;  /*!< [15..15] Extended Analog Output Control                                   */
        } ADEXICR_b;
    };

    union
    {
        __IOM uint16_t ADANSB[2];       /*!< (@ 0x00000014) A/D Channel Select Register B0                             */

        struct
        {
            __IOM uint16_t ANSB000 : 1; /*!< [0..0] AN Input Select                                                    */
            __IOM uint16_t ANSB001 : 1; /*!< [1..1] AN Input Select                                                    */
            __IOM uint16_t ANSB002 : 1; /*!< [2..2] AN Input Select                                                    */
            __IOM uint16_t ANSB003 : 1; /*!< [3..3] AN Input Select                                                    */
            __IOM uint16_t ANSB004 : 1; /*!< [4..4] AN Input Select                                                    */
            __IOM uint16_t ANSB005 : 1; /*!< [5..5] AN Input Select                                                    */
            __IOM uint16_t ANSB006 : 1; /*!< [6..6] AN Input Select                                                    */
            __IOM uint16_t ANSB007 : 1; /*!< [7..7] AN Input Select                                                    */
            __IOM uint16_t ANSB008 : 1; /*!< [8..8] AN Input Select                                                    */
            __IOM uint16_t ANSB009 : 1; /*!< [9..9] AN Input Select                                                    */
            __IOM uint16_t ANSB010 : 1; /*!< [10..10] AN Input Select                                                  */
            __IOM uint16_t ANSB011 : 1; /*!< [11..11] AN Input Select                                                  */
            __IOM uint16_t ANSB012 : 1; /*!< [12..12] AN Input Select                                                  */
            __IOM uint16_t ANSB013 : 1; /*!< [13..13] AN Input Select                                                  */
            __IOM uint16_t ANSB014 : 1; /*!< [14..14] AN Input Select                                                  */
            __IOM uint16_t ANSB015 : 1; /*!< [15..15] AN Input Select                                                  */
        } ADANSB_b[2];
    };

    union
    {
        __IM uint16_t ADDBLDR;          /*!< (@ 0x00000018) A/D Data Duplication Register                              */

        struct
        {
            __IM uint16_t ADDBLDR : 16; /*!< [15..0] This is a 16-bit read-only register for storing the
                                         *   result of A/D conversion in response to the second trigger
                                         *   in double trigger mode.                                                   */
        } ADDBLDR_b;
    };

    union
    {
        __IM uint16_t ADTSDR;          /*!< (@ 0x0000001A) A/D Temperature Sensor Data Register                       */

        struct
        {
            __IM uint16_t ADTSDR : 16; /*!< [15..0] This is a 16-bit read-only register for storing the
                                        *   A/D conversion result of temperature sensor output.                       */
        } ADTSDR_b;
    };

    union
    {
        __IM uint16_t ADOCDR;          /*!< (@ 0x0000001C) A/D Internal Reference Voltage Data Register               */

        struct
        {
            __IM uint16_t ADOCDR : 16; /*!< [15..0] This is a 16-bit read-only register for storing the
                                        *   A/D result of internal reference voltage.                                 */
        } ADOCDR_b;
    };

    union
    {
        union
        {
            __IM uint16_t ADRD_RIGHT;     /*!< (@ 0x0000001E) A/D Self-Diagnosis Data Register Right Justified           */

            struct
            {
                __IM uint16_t AD : 14;    /*!< [13..0] A/D-converted value (right-justified)The format for
                                           *   data determine ADCER.ADRFMT and ADCER.ADPRC.                              */
                __IM uint16_t DIAGST : 2; /*!< [15..14] Self-Diagnosis Status                                            */
            } ADRD_RIGHT_b;
        };

        union
        {
            __IM uint16_t ADRD_LEFT;       /*!< (@ 0x0000001E) A/D Self-Diagnosis Data Register Left Justified            */

            struct
            {
                __IM uint16_t DIAGST : 2;  /*!< [1..0] Self-Diagnosis Status                                              */
                __IM uint16_t AD     : 14; /*!< [15..2] A/D-converted value (left-justified)The format for data
                                            *   determine ADCER.ADRFMT and ADCER.ADPRC.                                   */
            } ADRD_LEFT_b;
        };
    };

    union
    {
        __IM uint16_t ADDR[21];        /*!< (@ 0x00000020) A/D Data Register                                          */

        struct
        {
            __IM uint16_t ADDR : 16;   /*!< [15..0] The ADDR register is a 16-bit read-only registers for
                                        *   storing the result of A/D conversion.                                     */
        } ADDR_b[21];
    };
    __IM uint16_t RESERVED2[12];
    __IM uint8_t  RESERVED3;

    union
    {
        __IOM uint8_t ADSAMPR;         /*!< (@ 0x00000063) A/D Conversion Time Setting Protection Release
                                        *                  Register                                                   */

        struct
        {
            __IOM uint8_t PRO : 2;     /*!< [1..0] A/D Conversion Time Setting Register Protection                    */
            uint8_t           : 6;
        } ADSAMPR_b;
    };
    __IM uint16_t RESERVED4;

    union
    {
        __IOM uint16_t ADSHCR;         /*!< (@ 0x00000066) A/D Sample-and-Hold Circuit Control Register               */

        struct
        {
            __IOM uint16_t SSTSH : 8;  /*!< [7..0] Channel-Dedicated Sample-and-Hold Circuit Sampling Time
                                        *   Setting                                                                   */
            __IOM uint16_t SHANS : 3;  /*!< [10..8] Channel-Dedicated Sample-and-Hold Circuit Bypass Select           */
            uint16_t             : 5;
        } ADSHCR_b;
    };
    __IM uint16_t RESERVED5[3];

    union
    {
        __IOM uint16_t ADSAM;          /*!< (@ 0x0000006E) A/D Conversion Time Setting Register                       */

        struct
        {
            uint16_t           : 5;
            __IOM uint16_t SAM : 1;    /*!< [5..5] Conversion Time Setting                                            */
            uint16_t           : 10;
        } ADSAM_b;
    };
    __IM uint16_t RESERVED6[5];

    union
    {
        __IOM uint8_t ADDISCR;         /*!< (@ 0x0000007A) A/D Disconnection Detection Control Register               */

        struct
        {
            __IOM uint8_t ADNDIS : 5;  /*!< [4..0] A/D Disconnection Detection Assist Setting                         */
            uint8_t              : 3;
        } ADDISCR_b;
    };
    __IM uint8_t RESERVED7;

    union
    {
        __IOM uint8_t ADSHMSR;         /*!< (@ 0x0000007C) A/D Sample-and-Hold Operating Mode Select Register         */

        struct
        {
            __IOM uint8_t SHMD : 1;    /*!< [0..0] Channel-Dedicated Sample-and-Hold Circuit Operating Mode
                                        *   Setting                                                                   */
            uint8_t : 7;
        } ADSHMSR_b;
    };
    __IM uint8_t  RESERVED8;
    __IM uint16_t RESERVED9;

    union
    {
        __IOM uint16_t ADGSPCR;        /*!< (@ 0x00000080) A/D Group Scan Priority Control Register                   */

        struct
        {
            __IOM uint16_t PGS    : 1; /*!< [0..0] Group A priority control setting bit                               */
            __IOM uint16_t GBRSCN : 1; /*!< [1..1] Low-Priority Group Restart Setting                                 */
            uint16_t              : 12;
            __IOM uint16_t LGRRS  : 1; /*!< [14..14] Restart Channel Select                                           */
            __IOM uint16_t GBRP   : 1; /*!< [15..15] Single Scan Continuous Start                                     */
        } ADGSPCR_b;
    };
    __IM uint16_t RESERVED10;

    union
    {
        __IM uint16_t ADDBLDRA;          /*!< (@ 0x00000084) A/D Data Duplexing Register A                              */

        struct
        {
            __IM uint16_t ADDBLDRA : 16; /*!< [15..0] This register is a 16-bit read-only registers for storing
                                          *   the result of A/D conversion in response to the respective
                                          *   triggers during extended operation in double trigger mode.                */
        } ADDBLDRA_b;
    };

    union
    {
        __IM uint16_t ADDBLDRB;          /*!< (@ 0x00000086) A/D Data Duplexing Register B                              */

        struct
        {
            __IM uint16_t ADDBLDRB : 16; /*!< [15..0] This register is a 16-bit read-only registers for storing
                                          *   the result of A/D conversion in response to the respective
                                          *   triggers during extended operation in double trigger mode.                */
        } ADDBLDRB_b;
    };
    __IM uint16_t RESERVED11[2];

    union
    {
        __IM uint8_t ADWINMON;         /*!< (@ 0x0000008C) A/D Comparison Function Window A/B Status Monitoring
                                        *                  Register                                                   */

        struct
        {
            __IM uint8_t MONCOMB : 1;  /*!< [0..0] Combination Result Monitoring                                      */
            uint8_t              : 3;
            __IM uint8_t MONCMPA : 1;  /*!< [4..4] Comparison Result Monitor A                                        */
            __IM uint8_t MONCMPB : 1;  /*!< [5..5] Comparison Result Monitor B                                        */
            uint8_t              : 2;
        } ADWINMON_b;
    };
    __IM uint8_t  RESERVED12;
    __IM uint16_t RESERVED13;

    union
    {
        __IOM uint16_t ADCMPCR;        /*!< (@ 0x00000090) A/D Comparison Function Control Register                   */

        struct
        {
            __IOM uint16_t CMPAB  : 2; /*!< [1..0] Window A/B Complex Conditions Setting                              */
            uint16_t              : 7;
            __IOM uint16_t CMPBE  : 1; /*!< [9..9] Comparison Window B Enable                                         */
            uint16_t              : 1;
            __IOM uint16_t CMPAE  : 1; /*!< [11..11] Comparison Window A Enable                                       */
            uint16_t              : 1;
            __IOM uint16_t CMPBIE : 1; /*!< [13..13] Comparison Window B Interrupt Enable                             */
            __IOM uint16_t WCMPE  : 1; /*!< [14..14] Window Function Setting                                          */
            __IOM uint16_t CMPAIE : 1; /*!< [15..15] Comparison Window A Interrupt Enable                             */
        } ADCMPCR_b;
    };

    union
    {
        __IOM uint8_t ADCMPANSER;      /*!< (@ 0x00000092) A/D Comparison Function Window A Extended Input
                                        *                  Select Register                                            */

        struct
        {
            __IOM uint8_t CMPSTS : 1;  /*!< [0..0] Temperature Sensor Output Comparison Select                        */
            __IOM uint8_t CMPSOC : 1;  /*!< [1..1] Internal Reference Voltage Compare Select                          */
            uint8_t              : 6;
        } ADCMPANSER_b;
    };

    union
    {
        __IOM uint8_t ADCMPLER;        /*!< (@ 0x00000093) A/D Comparison Function Window A Extended Input
                                        *                  Comparison Condition Setting Register                      */

        struct
        {
            __IOM uint8_t CMPLTS : 1;  /*!< [0..0] Comparison Window A Temperature Sensor Output Comparison
                                        *   Condition Select                                                          */
            __IOM uint8_t CMPLOC : 1;  /*!< [1..1] Comparison Window A Internal Reference Voltage Comparison
                                        *   Condition Select                                                          */
            uint8_t : 6;
        } ADCMPLER_b;
    };

    union
    {
        __IOM uint16_t ADCMPANSR[2];     /*!< (@ 0x00000094) A/D Compare Function Window A Channel Select
                                          *                  Register                                                   */

        struct
        {
            __IOM uint16_t CMPCHA0  : 1; /*!< [0..0] AN Input Select                                                    */
            __IOM uint16_t CMPCHA1  : 1; /*!< [1..1] AN Input Select                                                    */
            __IOM uint16_t CMPCHA2  : 1; /*!< [2..2] AN Input Select                                                    */
            __IOM uint16_t CMPCHA3  : 1; /*!< [3..3] AN Input Select                                                    */
            __IOM uint16_t CMPCHA4  : 1; /*!< [4..4] AN Input Select                                                    */
            __IOM uint16_t CMPCHA5  : 1; /*!< [5..5] AN Input Select                                                    */
            __IOM uint16_t CMPCHA6  : 1; /*!< [6..6] AN Input Select                                                    */
            __IOM uint16_t CMPCHA7  : 1; /*!< [7..7] AN Input Select                                                    */
            __IOM uint16_t CMPCHA8  : 1; /*!< [8..8] AN Input Select                                                    */
            __IOM uint16_t CMPCHA9  : 1; /*!< [9..9] AN Input Select                                                    */
            __IOM uint16_t CMPCHA10 : 1; /*!< [10..10] AN Input Select                                                  */
            __IOM uint16_t CMPCHA11 : 1; /*!< [11..11] AN Input Select                                                  */
            __IOM uint16_t CMPCHA12 : 1; /*!< [12..12] AN Input Select                                                  */
            __IOM uint16_t CMPCHA13 : 1; /*!< [13..13] AN Input Select                                                  */
            __IOM uint16_t CMPCHA14 : 1; /*!< [14..14] AN Input Select                                                  */
            __IOM uint16_t CMPCHA15 : 1; /*!< [15..15] AN Input Select                                                  */
        } ADCMPANSR_b[2];
    };

    union
    {
        __IOM uint16_t ADCMPLR[2];        /*!< (@ 0x00000098) A/D Compare Function Window A Comparison Condition
                                           *                  Setting Register                                           */

        struct
        {
            __IOM uint16_t CMPLCHA0  : 1; /*!< [0..0] Comparison condition of input                                      */
            __IOM uint16_t CMPLCHA1  : 1; /*!< [1..1] Comparison condition of input                                      */
            __IOM uint16_t CMPLCHA2  : 1; /*!< [2..2] Comparison condition of input                                      */
            __IOM uint16_t CMPLCHA3  : 1; /*!< [3..3] Comparison condition of input                                      */
            __IOM uint16_t CMPLCHA4  : 1; /*!< [4..4] Comparison condition of input                                      */
            __IOM uint16_t CMPLCHA5  : 1; /*!< [5..5] Comparison condition of input                                      */
            __IOM uint16_t CMPLCHA6  : 1; /*!< [6..6] Comparison condition of input                                      */
            __IOM uint16_t CMPLCHA7  : 1; /*!< [7..7] Comparison condition of input                                      */
            __IOM uint16_t CMPLCHA8  : 1; /*!< [8..8] Comparison condition of input                                      */
            __IOM uint16_t CMPLCHA9  : 1; /*!< [9..9] Comparison condition of input                                      */
            __IOM uint16_t CMPLCHA10 : 1; /*!< [10..10] Comparison condition of input                                    */
            __IOM uint16_t CMPLCHA11 : 1; /*!< [11..11] Comparison condition of input                                    */
            __IOM uint16_t CMPLCHA12 : 1; /*!< [12..12] Comparison condition of input                                    */
            __IOM uint16_t CMPLCHA13 : 1; /*!< [13..13] Comparison condition of input                                    */
            __IOM uint16_t CMPLCHA14 : 1; /*!< [14..14] Comparison condition of input                                    */
            __IOM uint16_t CMPLCHA15 : 1; /*!< [15..15] Comparison condition of input                                    */
        } ADCMPLR_b[2];
    };

    union
    {
        __IOM uint16_t ADCMPDR0;          /*!< (@ 0x0000009C) A/D Comparison Function Window A Lower Level
                                           *                  Setting Register                                           */

        struct
        {
            __IOM uint16_t ADCMPDR0 : 16; /*!< [15..0] The ADCMPDR0 register sets the reference data when the
                                           *   compare window A function is used. ADCMPDR0 sets the lower-side
                                           *   level of window A.                                                        */
        } ADCMPDR0_b;
    };

    union
    {
        __IOM uint16_t ADCMPDR1;          /*!< (@ 0x0000009E) A/D Compare Function Window A Upper-Side Level
                                           *                  Setting Register                                           */

        struct
        {
            __IOM uint16_t ADCMPDR1 : 16; /*!< [15..0] The ADCMPDR0 register sets the reference data when the
                                           *   compare window A function is used. ADCMPDR0 sets the lower-side
                                           *   level of window A.                                                        */
        } ADCMPDR1_b;
    };

    union
    {
        __IOM uint16_t ADCMPSR[2];         /*!< (@ 0x000000A0) A/D Compare Function Window A Channel Status
                                            *                  Register                                                   */

        struct
        {
            __IOM uint16_t CMPSTCHA0  : 1; /*!< [0..0] Compare window A flag of input                                     */
            __IOM uint16_t CMPSTCHA1  : 1; /*!< [1..1] Compare window A flag of input                                     */
            __IOM uint16_t CMPSTCHA2  : 1; /*!< [2..2] Compare window A flag of input                                     */
            __IOM uint16_t CMPSTCHA3  : 1; /*!< [3..3] Compare window A flag of input                                     */
            __IOM uint16_t CMPSTCHA4  : 1; /*!< [4..4] Compare window A flag of input                                     */
            __IOM uint16_t CMPSTCHA5  : 1; /*!< [5..5] Compare window A flag of input                                     */
            __IOM uint16_t CMPSTCHA6  : 1; /*!< [6..6] Compare window A flag of input                                     */
            __IOM uint16_t CMPSTCHA7  : 1; /*!< [7..7] Compare window A flag of input                                     */
            __IOM uint16_t CMPSTCHA8  : 1; /*!< [8..8] Compare window A flag of input                                     */
            __IOM uint16_t CMPSTCHA9  : 1; /*!< [9..9] Compare window A flag of input                                     */
            __IOM uint16_t CMPSTCHA10 : 1; /*!< [10..10] Compare window A flag of input                                   */
            __IOM uint16_t CMPSTCHA11 : 1; /*!< [11..11] Compare window A flag of input                                   */
            __IOM uint16_t CMPSTCHA12 : 1; /*!< [12..12] Compare window A flag of input                                   */
            __IOM uint16_t CMPSTCHA13 : 1; /*!< [13..13] Compare window A flag of input                                   */
            __IOM uint16_t CMPSTCHA14 : 1; /*!< [14..14] Compare window A flag of input                                   */
            __IOM uint16_t CMPSTCHA15 : 1; /*!< [15..15] Compare window A flag of input                                   */
        } ADCMPSR_b[2];
    };

    union
    {
        __IOM uint8_t ADCMPSER;        /*!< (@ 0x000000A4) A/D Comparison Function Window A Extended Input
                                        *                  Channel Status Register                                    */

        struct
        {
            __IOM uint8_t CMPFTS : 1;  /*!< [0..0] Comparison Window A Temperature Sensor Output Comparison
                                        *   Flag                                                                      */
            __IOM uint8_t CMPFOC : 1;  /*!< [1..1] Comparison Window A Internal Reference Voltage Comparison
                                        *   Flag                                                                      */
            uint8_t : 6;
        } ADCMPSER_b;
    };
    __IM uint8_t RESERVED14;

    union
    {
        __IOM uint8_t ADCMPBNSR;       /*!< (@ 0x000000A6) A/D Comparison Function Window B Channel Select
                                        *                  Register                                                   */

        struct
        {
            __IOM uint8_t CMPCHB : 6;  /*!< [5..0] Comparison Window B Channel Select                                 */
            uint8_t              : 1;
            __IOM uint8_t CMPLB  : 1;  /*!< [7..7] Comparison Window B Comparison Condition Setting                   */
        } ADCMPBNSR_b;
    };
    __IM uint8_t RESERVED15;

    union
    {
        __IOM uint16_t ADWINLLB;          /*!< (@ 0x000000A8) A/D Comparison Function Window B Lower Level
                                           *                  Setting Register                                           */

        struct
        {
            __IOM uint16_t ADWINLLB : 16; /*!< [15..0] This register is used to compare A window function is
                                           *   used to set the lower level of the window B.                              */
        } ADWINLLB_b;
    };

    union
    {
        __IOM uint16_t ADWINULB;          /*!< (@ 0x000000AA) A/D Comparison Function Window B Upper Level
                                           *                  Setting Register                                           */

        struct
        {
            __IOM uint16_t ADWINULB : 16; /*!< [15..0] This register is used to compare A window function is
                                           *   used to set the higher level of the window B.                             */
        } ADWINULB_b;
    };

    union
    {
        __IOM uint8_t ADCMPBSR;        /*!< (@ 0x000000AC) A/D Comparison Function Window B Channel Status
                                        *                  Register                                                   */

        struct
        {
            __IOM uint8_t CMPSTB : 1;  /*!< [0..0] Comparison Window B Flag                                           */
            uint8_t              : 7;
        } ADCMPBSR_b;
    };
    __IM uint8_t  RESERVED16;
    __IM uint16_t RESERVED17[19];

    union
    {
        __IOM uint16_t ADANSC[2];       /*!< (@ 0x000000D4) A/D Channel Select Register C0                             */

        struct
        {
            __IOM uint16_t ANSC000 : 1; /*!< [0..0] AN Input Select                                                    */
            __IOM uint16_t ANSC001 : 1; /*!< [1..1] AN Input Select                                                    */
            __IOM uint16_t ANSC002 : 1; /*!< [2..2] AN Input Select                                                    */
            __IOM uint16_t ANSC003 : 1; /*!< [3..3] AN Input Select                                                    */
            __IOM uint16_t ANSC004 : 1; /*!< [4..4] AN Input Select                                                    */
            __IOM uint16_t ANSC005 : 1; /*!< [5..5] AN Input Select                                                    */
            __IOM uint16_t ANSC006 : 1; /*!< [6..6] AN Input Select                                                    */
            __IOM uint16_t ANSC007 : 1; /*!< [7..7] AN Input Select                                                    */
            __IOM uint16_t ANSC008 : 1; /*!< [8..8] AN Input Select                                                    */
            __IOM uint16_t ANSC009 : 1; /*!< [9..9] AN Input Select                                                    */
            __IOM uint16_t ANSC010 : 1; /*!< [10..10] AN Input Select                                                  */
            __IOM uint16_t ANSC011 : 1; /*!< [11..11] AN Input Select                                                  */
            __IOM uint16_t ANSC012 : 1; /*!< [12..12] AN Input Select                                                  */
            __IOM uint16_t ANSC013 : 1; /*!< [13..13] AN Input Select                                                  */
            __IOM uint16_t ANSC014 : 1; /*!< [14..14] AN Input Select                                                  */
            __IOM uint16_t ANSC015 : 1; /*!< [15..15] AN Input Select                                                  */
        } ADANSC_b[2];
    };

    union
    {
        __IOM uint8_t ADGCEXCR;        /*!< (@ 0x000000D8) A/D Group C Extended Input Control Register                */

        struct
        {
            __IOM uint8_t TSSC : 1;    /*!< [0..0] Group C Temperature Sensor Output A/D Conversion Select            */
            __IOM uint8_t OCSC : 1;    /*!< [1..1] Group C Internal Reference Voltage A/D Conversion Select           */
            uint8_t            : 6;
        } ADGCEXCR_b;
    };

    union
    {
        __IOM uint8_t ADGCTRGR;        /*!< (@ 0x000000D9) A/D Group C Trigger Select Register                        */

        struct
        {
            __IOM uint8_t TRSC   : 6;  /*!< [5..0] Group C A/D Conversion Start Trigger Select                        */
            __IOM uint8_t GCADIE : 1;  /*!< [6..6] Group C Scan End Interrupt Enable                                  */
            __IOM uint8_t GRCE   : 1;  /*!< [7..7] Group C A/D Conversion Operation Enable                            */
        } ADGCTRGR_b;
    };
    __IM uint16_t RESERVED18;
    __IM uint8_t  RESERVED19;

    union
    {
        __IOM uint8_t ADSSTRL;         /*!< (@ 0x000000DD) A/D Sampling State Register L                              */

        struct
        {
            __IOM uint8_t SST : 8;     /*!< [7..0] Sampling Time Setting (AN016-AN027)                                */
        } ADSSTRL_b;
    };

    union
    {
        __IOM uint8_t ADSSTRT;         /*!< (@ 0x000000DE) A/D Sampling State Register T                              */

        struct
        {
            __IOM uint8_t SST : 8;     /*!< [7..0] Sampling Time Setting (temperature sensor output)                  */
        } ADSSTRT_b;
    };

    union
    {
        __IOM uint8_t ADSSTRO;         /*!< (@ 0x000000DF) A/D Sampling State Register O                              */

        struct
        {
            __IOM uint8_t SST : 8;     /*!< [7..0] Sampling Time Setting (Internal reference voltage)                 */
        } ADSSTRO_b;
    };

    union
    {
        __IOM uint8_t ADSSTR[16];      /*!< (@ 0x000000E0) A/D Sampling State Registers                               */

        struct
        {
            __IOM uint8_t SST : 8;     /*!< [7..0] Sampling time setting                                              */
        } ADSSTR_b[16];
    };
} R_ADC0_Type;                         /*!< Size = 240 (0xf0)                                                         */

#endif /* R_ADC_REG_H */
