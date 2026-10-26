/* ${DISCLAIMER_PLACEHOLDER} */

#ifndef R_PSCU_REG_H
#define R_PSCU_REG_H

/*======================================================================================================== */
/* ================                  Device Specific Cluster Section                      ================ */
/*======================================================================================================== */

/** @addtogroup Device_Peripheral_clusters
 * @{
 */

/**
 * @brief Peripheral Security Control Unit (PSCU)
 */

typedef struct                          /*!< (@ 0x87804000) PSCU Structure                                             */
{
    __IOM uint32_t PSARA;               /*!< (@ 0x00000000) Peripheral Security Attribution Register A                 */

    union
    {
        __IOM uint32_t PSARB;           /*!< (@ 0x00000004) Peripheral Security Attribution Register B                 */

        struct
        {
            __IOM uint32_t PSARB0  : 1; /*!< [0..0] Serial Communication Interface 12 Security Attribution             */
            uint32_t               : 3;
            __IOM uint32_t PSARB4  : 1; /*!< [4..4] I3C Bus Interface 2 Security Attribution                           */
            uint32_t               : 2;
            __IOM uint32_t PSARB7  : 1; /*!< [7..7] I2C Bus Interface 2 Security Attribution                           */
            __IOM uint32_t PSARB8  : 1; /*!< [8..8] I2C Bus Interface 1 Security Attribution                           */
            uint32_t               : 2;
            __IOM uint32_t PSARB11 : 1; /*!< [11..11] Universal Serial Bus 2.0 FS Interface 0 Security Attribution     */
            __IOM uint32_t PSARB12 : 1; /*!< [12..12] Universal Serial Bus 2.0 HS Interface 0 Security Attribution     */
            uint32_t               : 3;
            __IOM uint32_t PSARB16 : 1; /*!< [16..16] Octa Memory Controller 1 Security Attribution                    */
            __IOM uint32_t PSARB17 : 1; /*!< [17..17] Octa Memory Controller 0 Security Attribution                    */
            __IOM uint32_t PSARB18 : 1; /*!< [18..18] Serial Peripheral Interface 1 Security Attribution               */
            __IOM uint32_t PSARB19 : 1; /*!< [19..19] Serial Peripheral Interface 0 Security Attribution               */
            __IOM uint32_t PSARB20 : 1; /*!< [20..20] Serial Communication Interface 11 Security Attribution           */
            __IOM uint32_t PSARB21 : 1; /*!< [21..21] Serial Communication Interface 10 Security Attribution           */
            __IOM uint32_t PSARB22 : 1; /*!< [22..22] Serial Communication Interface 9 Security Attribution            */
            __IOM uint32_t PSARB23 : 1; /*!< [23..23] Serial Communication Interface 8 Security Attribution            */
            __IOM uint32_t PSARB24 : 1; /*!< [24..24] Serial Communication Interface 7 Security Attribution            */
            __IOM uint32_t PSARB25 : 1; /*!< [25..25] Serial Communication Interface 6 Security Attribution            */
            __IOM uint32_t PSARB26 : 1; /*!< [26..26] Serial Communication Interface 5 Security Attribution            */
            __IOM uint32_t PSARB27 : 1; /*!< [27..27] Serial Communication Interface 4 Security Attribution            */
            __IOM uint32_t PSARB28 : 1; /*!< [28..28] Serial Communication Interface 3 Security Attribution            */
            __IOM uint32_t PSARB29 : 1; /*!< [29..29] Serial Communication Interface 2 Security Attribution            */
            __IOM uint32_t PSARB30 : 1; /*!< [30..30] Serial Communication Interface 1 Security Attribution            */
            __IOM uint32_t PSARB31 : 1; /*!< [31..31] Serial Communication Interface 0 Security Attribution            */
        } PSARB_b;
    };

    union
    {
        __IOM uint32_t PSARC;           /*!< (@ 0x00000008) Peripheral Security Attribution Register C                 */

        struct
        {
            __IOM uint32_t PSARC0 : 1;  /*!< [0..0] Clock Frequency Accuracy Measurement Circuit Security
                                         *   Attribution                                                               */
            __IOM uint32_t PSARC1 : 1;  /*!< [1..1] Cyclic Redundancy Check Calculator Security Attribution            */
            __IOM uint32_t PSARC2 : 1;  /*!< [2..2] Parallel Data Capture Unit Security Attribution                    */
            uint32_t              : 4;
            __IOM uint32_t PSARC7 : 1;  /*!< [7..7] Serial Sound Interface Enhanced (channel 1) Security
                                         *   Attribution                                                               */
            __IOM uint32_t PSARC8 : 1;  /*!< [8..8] Serial Sound Interface Enhanced (channel 0) Security
                                         *   Attribution                                                               */
            uint32_t               : 3;
            __IOM uint32_t PSARC12 : 1; /*!< [12..12] Secure Digital Host IF 0 Security Attribution                    */
            __IOM uint32_t PSARC13 : 1; /*!< [13..13] Data Operation Circuit Security Attribution                      */
            uint32_t               : 9;
            __IOM uint32_t PSARC23 : 1; /*!< [23..23] Serial Peripheral Interface 2 Security Attribution               */
            __IOM uint32_t PSARC24 : 1; /*!< [24..24] PDMIF Security Attribution                                       */
            __IOM uint32_t PSARC25 : 1; /*!< [25..25] Controller Area Network with Flexible Data-Rate Lite
                                         *   Security Attribution                                                      */
            uint32_t               : 1;
            __IOM uint32_t PSARC27 : 1; /*!< [27..27] Controller Area Network with Flexible Data-Rate 0 Security
                                         *   Attribution                                                               */
            __IOM uint32_t PSARC28 : 1; /*!< [28..28] ETHPHY Security Attribution                                      */
            __IOM uint32_t PSARC29 : 1; /*!< [29..29] EtherCAT Security Attribution                                    */
            __IOM uint32_t PSARC30 : 1; /*!< [30..30] R-Switch2(RSW2) Security Attribution                             */
            __IOM uint32_t PSARC31 : 1; /*!< [31..31] SHIP Security Attribution                                        */
        } PSARC_b;
    };

    union
    {
        __IOM uint32_t PSARD;          /*!< (@ 0x0000000C) Peripheral Security Attribution Register D                 */

        struct
        {
            uint32_t               : 6;
            __IOM uint32_t PSARD6  : 1; /*!< [6..6] Pulse Width Modulation PWM Security Attribution                    */
            __IOM uint32_t PSARD7  : 1; /*!< [7..7] Port Output Enable 3 Security Attribution                          */
            __IOM uint32_t PSARD8  : 1; /*!< [8..8] DSMIF1 Security Attribution                                        */
            __IOM uint32_t PSARD9  : 1; /*!< [9..9] DSMIF0 Security Attribution                                        */
            uint32_t               : 1;
            __IOM uint32_t PSARD11 : 1; /*!< [11..11] Port Output Enable for GPT Group 3 Security Attribution          */
            __IOM uint32_t PSARD12 : 1; /*!< [12..12] Port Output Enable for GPT Group 2 Security Attribution          */
            __IOM uint32_t PSARD13 : 1; /*!< [13..13] Port Output Enable for GPT Group 1 Security Attribution          */
            __IOM uint32_t PSARD14 : 1; /*!< [14..14] Port Output Enable for GPT Group 0 Security Attribution          */
            uint32_t               : 4;
            __IOM uint32_t PSARD19 : 1; /*!< [19..19] 12-Bit D/A Converter 1 Security Attribution                      */
            __IOM uint32_t PSARD20 : 1; /*!< [20..20] 12-Bit D/A Converter 0 Security Attribution                      */
            __IOM uint32_t PSARD21 : 1; /*!< [21..21] 16-Bit A/D Converter Security Attribution                        */
            __IOM uint32_t PSARD22 : 1; /*!< [22..22] Temperature Sensor Security Attribution                          */
            uint32_t               : 9;
        } PSARD_b;
    };

    union
    {
        __IOM uint32_t PSARE;          /*!< (@ 0x00000010) Peripheral Security Attribution Register E                 */

        struct
        {
            uint32_t               : 1;
            __IOM uint32_t PSARE1  : 1; /*!< [1..1] WDT0 Security Attribution                                          */
            __IOM uint32_t PSARE2  : 1; /*!< [2..2] Independent Watchdog Timer Security Attribution                    */
            __IOM uint32_t PSARE3  : 1; /*!< [3..3] Real Time Clock Security Attribution                               */
            uint32_t               : 11;
            __IOM uint32_t PSARE15 : 1; /*!< [15..15] GPT common Security Attribution                                  */
            uint32_t               : 15;
            __IOM uint32_t PSARE31 : 1; /*!< [31..31] General PWM Timer channel0 Security Attribution                  */
        } PSARE_b;
    };

    union
    {
        __IOM uint32_t MSSAR;           /*!< (@ 0x00000014) Module Stop Security Attribution Register                  */

        struct
        {
            __IOM uint32_t MSSAR0  : 1; /*!< [0..0] SRAM0 Clock Stop Security Attribution                              */
            __IOM uint32_t MSSAR1  : 1; /*!< [1..1] SRAM1 Clock Stop Security Attribution                              */
            uint32_t               : 20;
            __IOM uint32_t MSSAR22 : 1; /*!< [22..22] DMAC0/DTC0 Clock Stop Security Attribution                       */
            uint32_t               : 6;
            __IOM uint32_t MSSAR29 : 1; /*!< [29..29] EXDMAC Clock Stop Security Attribution                           */
            uint32_t               : 1;
            __IOM uint32_t MSSAR31 : 1; /*!< [31..31] ELC clock stop Security Attribution                              */
        } MSSAR_b;
    };
    __IM uint32_t RESERVED[6];

    union
    {
        __IM uint32_t CMSAMON;         /*!< (@ 0x00000030) Code MRAMSS Security Attribution Monitor Register          */

        struct
        {
            uint32_t          : 15;
            __IM uint32_t CMS : 9;     /*!< [23..15] Code MRAM Secure area                                            */
            uint32_t          : 8;
        } CMSAMON_b;
    };

    union
    {
        __IM uint32_t DMSAMON;         /*!< (@ 0x00000034) Data MRAMSS Security Attribution Monitor Register          */

        struct
        {
            uint32_t          : 11;
            __IM uint32_t DMS : 5;     /*!< [15..11] Data MRAM Secure area                                            */
            uint32_t          : 16;
        } DMSAMON_b;
    };

    union
    {
        __IM uint32_t DLMMON;          /*!< (@ 0x00000038) Device Lifecycle Management State Monitor Register         */

        struct
        {
            __IM uint32_t DLMMON : 4;  /*!< [3..0] Device Lifecycle Management State Monitor                          */
            uint32_t             : 28;
        } DLMMON_b;
    };
    __IM uint32_t RESERVED1;

    union
    {
        __IOM uint32_t PSARF;          /*!< (@ 0x00000040) Peripheral Security Attribution Register F                 */

        struct
        {
            uint32_t               : 6;
            __IOM uint32_t PSARF6  : 1; /*!< [6..6] 8-bit Timer 2, 3 Security Attribution                              */
            __IOM uint32_t PSARF7  : 1; /*!< [7..7] 8-bit Timer 0, 1 Security Attribution                              */
            __IOM uint32_t PSARF8  : 1; /*!< [8..8] Multi-function Timer Unit Security Attribution                     */
            uint32_t               : 3;
            __IOM uint32_t PSARF12 : 1; /*!< [12..12] Compare Match Timer W 1 Security Attribution                     */
            __IOM uint32_t PSARF13 : 1; /*!< [13..13] Compare Match Timer W 0 Security Attribution                     */
            __IOM uint32_t PSARF14 : 1; /*!< [14..14] Compare Match Timer 2, 3 Security Attribution                    */
            __IOM uint32_t PSARF15 : 1; /*!< [15..15] Compare Match Timer 0, 1 Security Attribution                    */
            __IOM uint32_t PSARF16 : 1; /*!< [16..16] Universal Digital I/O function 0 Security Attribution            */
            uint32_t               : 13;
            __IOM uint32_t PSARF30 : 1; /*!< [30..30] Phase Counting Interface 1 Security Attribution                  */
            __IOM uint32_t PSARF31 : 1; /*!< [31..31] Phase Counting Interface 0 Security Attribution                  */
        } PSARF_b;
    };
} R_PSCU_Type;                          /*!< Size = 68 (0x44)                                                          */

#endif /* R_PSCU_REG_H */
