#ifndef R_SDHI_REG_H
#define R_SDHI_REG_H

/* =========================================================================================================================== */
/* ================                                          R_SDHI                                           ================ */
/* =========================================================================================================================== */

/**
 * @brief SD Host interface (R_SDHI)
 */

typedef struct                          /*!< (@ 0x0008AC00) R_SDHI Structure                                           */
{
    union
    {
        __IOM uint32_t SD_CMD;          /*!< (@ 0x00000000) Command Register                                          */

        struct
        {
            __IOM uint32_t CMDIDX  : 6; /*!< [5..0] Command Index Field Value Select                                   */
            __IOM uint32_t ACMD    : 2; /*!< [7..6] Command Type Select                                                */
            __IOM uint32_t RSPTP   : 3; /*!< [10..8] Response Type Select                                              */
            __IOM uint32_t CDMTP   : 1; /*!< [11..11] Data Transfer Select                                             */
            __IOM uint32_t CMDRW   : 1; /*!< [12..12] Data Transfer Direction Select                                   */
            __IOM uint32_t TRSTP   : 1; /*!< [13..13] Block Transfer Select                                            */
            __IOM uint32_t CMD12AT : 2; /*!< [15..14] CMD12 Automatic Issue Select                                     */
            uint32_t               : 16;
        } SD_CMD_b;
    };
    __IM uint32_t RESERVED;

    union
    {
        __IOM uint32_t SD_ARG;          /*!< (@ 0x00000008) Argument Register                                          */

        struct
        {
            __IOM uint32_t SD_ARG : 32; /*!< [31..0] Argument Register is used for setting the argument field
                                         *   value                                                                     */
        } SD_ARG_b;
    };
    __IM uint32_t RESERVED1;

    union
    {
        __IOM uint32_t SD_STOP;        /*!< (@ 0x00000010) Data Stop Register                                         */

        struct
        {
            __IOM uint32_t STP : 1;    /*!< [0..0] Transfer Stop                                                      */
            uint32_t           : 7;
            __IOM uint32_t SEC : 1;    /*!< [8..8] Block Transfer Select                                              */
            uint32_t           : 23;
        } SD_STOP_b;
    };

    union
    {
        __IOM uint32_t SD_SECCNT;          /*!< (@ 0x00000014) Block Count Register                                       */

        struct
        {
            __IOM uint32_t SD_SECCNT : 32; /*!< [31..0] Block Count Register is used to set the number of blocks
                                            *   to be transferred                                                         */
        } SD_SECCNT_b;
    };

    union
    {
        __IM uint32_t SD_RSP10;          /*!< (@ 0x00000018) Response Register 10                                       */

        struct
        {
            __IM uint32_t SD_RSP10 : 32; /*!< [31..0] Store the response from the SD card                               */
        } SD_RSP10_b;
    };
    __IM uint32_t RESERVED2;

    union
    {
        __IM uint32_t SD_RSP32;          /*!< (@ 0x00000020) Response Register 32                                       */

        struct
        {
            __IM uint32_t SD_RSP32 : 32; /*!< [31..0] Store the response from the SD card                               */
        } SD_RSP32_b;
    };
    __IM uint32_t RESERVED3;

    union
    {
        __IM uint32_t SD_RSP54;          /*!< (@ 0x00000028) Response Register 54                                       */

        struct
        {
            __IM uint32_t SD_RSP54 : 32; /*!< [31..0] Store the response from the SD card                               */
        } SD_RSP54_b;
    };
    __IM uint32_t RESERVED4;

    union
    {
        __IM uint32_t SD_RSP76;          /*!< (@ 0x00000030) Response Register 76                                       */

        struct
        {
            __IM uint32_t SD_RSP76 : 24; /*!< [23..0] Store the response from the SD card                               */
            uint32_t               : 8;
        } SD_RSP76_b;
    };
    __IM uint32_t RESERVED5;

    union
    {
        __IOM uint32_t SD_INFO1;        /*!< (@ 0x00000038) SD Status Register 1                                       */

        struct
        {
            __IOM uint32_t RSPEND  : 1; /*!< [0..0] Response End Detection Flag                                        */
            uint32_t               : 1;
            __IOM uint32_t ACEND   : 1; /*!< [2..2] Access End Detection Flag                                          */
            __IOM uint32_t SDCDRM  : 1; /*!< [3..3] SDHI_CD Removal Flag                                               */
            __IOM uint32_t SDCDIN  : 1; /*!< [4..4] SDHI_CD Insertion Flag                                             */
            __IOM uint32_t SDCDMON : 1; /*!< [5..5] SDHI_CD Pin Monitor Flag                                           */
            uint32_t               : 1;
            __IM uint32_t  SDWPMON : 1; /*!< [7..7] SDHI_WP Pin Monitor Flag                                           */
            __IOM uint32_t SDD3RM  : 1; /*!< [8..8] SDHI_D3 Removal Flag                                               */
            __IOM uint32_t SDD3IN  : 1; /*!< [9..9] SDHI_D3 Insertion Flag                                             */
            __IM uint32_t  SDD3MON : 1; /*!< [10..10] SDHI_D3 Pin Monitor Flag                                         */
            uint32_t               : 21;
        } SD_INFO1_b;
    };

    union
    {
        __IOM uint32_t SD_INFO2;              /*!< (@ 0x0000003C) SD Status Register 2                                       */

        struct
        {
            __IOM uint32_t CMDE          : 1; /*!< [0..0] Command Error Detection Flag                                       */
            __IOM uint32_t CRCE          : 1; /*!< [1..1] CRC Error Detection Flag                                           */
            __IOM uint32_t ENDE          : 1; /*!< [2..2] End Bit Error Detection Flag                                       */
            __IOM uint32_t DTO           : 1; /*!< [3..3] Data Timeout Detection Flag                                        */
            __IOM uint32_t ILW           : 1; /*!< [4..4] SDBUFR Illegal Write Access Detection Flag                         */
            __IOM uint32_t ILR           : 1; /*!< [5..5] SDBUFR Illegal Read Access Detection Flag                          */
            __IOM uint32_t RSPTO         : 1; /*!< [6..6] Response Timeout Detection Flag                                    */
            __IM uint32_t  SDD0MON       : 1; /*!< [7..7] SDHI_D0 Pin Status Flag                                            */
            __IOM uint32_t BRE           : 1; /*!< [8..8] SDBUFR Read Enable Flag                                            */
            __IOM uint32_t BWE           : 1; /*!< [9..9] SDBUFR Write Enable Flag                                           */
            uint32_t                     : 3;
            __IM uint32_t  SD_CLK_CTRLEN : 1; /*!< [13..13] SDCLKCR Write Enable Flag                                        */
            __IM uint32_t  CBSY          : 1; /*!< [14..14] Command Sequence Status Flag                                     */
            __IOM uint32_t ILA           : 1; /*!< [15..15] Illegal Access Error Detection Flag                              */
            uint32_t                     : 16;
        } SD_INFO2_b;
    };

    union
    {
        __IOM uint32_t SD_INFO1_MASK;   /*!< (@ 0x00000040) SD Interrupt Mask Register 1                               */

        struct
        {
            __IOM uint32_t RSPENDM : 1; /*!< [0..0] Response End Interrupt Request Mask                                */
            uint32_t               : 1;
            __IOM uint32_t ACENDM  : 1; /*!< [2..2] Access End Interrupt Request Mask                                  */
            __IOM uint32_t SDCDRMM : 1; /*!< [3..3] SDHI_CD Removal Interrupt Request Mask                             */
            __IOM uint32_t SDCDINM : 1; /*!< [4..4] SDHI_CD Insertion Interrupt Request Mask                           */
            uint32_t               : 3;
            __IOM uint32_t SDD3RMM : 1; /*!< [8..8] SDHI_D3 Removal Interrupt Request Mask                             */
            __IOM uint32_t SDD3INM : 1; /*!< [9..9] SDHI_D3 Insertion Interrupt Request Mask                           */
            uint32_t               : 22;
        } SD_INFO1_MASK_b;
    };

    union
    {
        __IOM uint32_t SD_INFO2_MASK;  /*!< (@ 0x00000044) SD Interrupt Mask Register 2                               */

        struct
        {
            __IOM uint32_t CMDEM  : 1; /*!< [0..0] Command Error Interrupt Request Mask                               */
            __IOM uint32_t CRCEM  : 1; /*!< [1..1] CRC Error Interrupt Request Mask                                   */
            __IOM uint32_t ENDEM  : 1; /*!< [2..2] End Bit Error Interrupt Request Mask                               */
            __IOM uint32_t DTOM   : 1; /*!< [3..3] Data Timeout Interrupt Request Mask                                 */
            __IOM uint32_t ILWM   : 1; /*!< [4..4] SD_BUF Register Illegal Write Interrupt Request Mask               */
            __IOM uint32_t ILRM   : 1; /*!< [5..5] SD_BUF Register Illegal Read Interrupt Request Mask                */
            __IOM uint32_t RSPTOM : 1; /*!< [6..6] Response Timeout Interrupt Request Mask                            */
            uint32_t              : 1;
            __IOM uint32_t BREM   : 1; /*!< [8..8] BRE Interrupt Request Mask                                         */
            __IOM uint32_t BWEM   : 1; /*!< [9..9] BWE Interrupt Request Mask                                         */
            uint32_t              : 5;
            __IOM uint32_t ILAM   : 1; /*!< [15..15] Illegal Access Error Interrupt Request Mask                      */
            uint32_t              : 16;
        } SD_INFO2_MASK_b;
    };

    union
    {
        __IOM uint32_t SD_CLK_CTRL;       /*!< (@ 0x00000048) SDHI Clock Control Register                                */

        struct
        {
            __IOM uint32_t CLKSEL    : 8; /*!< [7..0] SDHI Clock Frequency Select                                        */
            __IOM uint32_t CLKEN     : 1; /*!< [8..8] SDHI Clock Output Control                                          */
            __IOM uint32_t CLKCTRLEN : 1; /*!< [9..9] SDHI Clock Output Automatic Control Select                         */
            uint32_t                 : 22;
        } SD_CLK_CTRL_b;
    };

    union
    {
        __IOM uint32_t SD_SIZE;        /*!< (@ 0x0000004C) Transfer Data Size Register                                */

        struct
        {
            __IOM uint32_t LEN : 10;   /*!< [9..0] Set the transfer data size                                         */
            uint32_t           : 22;
        } SD_SIZE_b;
    };

    union
    {
        __IOM uint32_t SD_OPTION;      /*!< (@ 0x00000050) Card Access Option Register                                */

        struct
        {
            __IOM uint32_t CTOP  : 4;  /*!< [3..0] Card Detection Time Counter                                        */
            __IOM uint32_t TOP   : 4;  /*!< [7..4] Timeout Counter                                                    */
            uint32_t             : 7;
            __IOM uint32_t WIDTH : 1;  /*!< [15..15] SD Bus Width Select                                              */
            uint32_t             : 16;
        } SD_OPTION_b;
    };
    __IM uint32_t RESERVED6;

    union
    {
        __IM uint32_t SD_ERR_STS1;      /*!< (@ 0x00000058) SD Error Status Register 1                                 */

        struct
        {
            __IM uint32_t CMDE0    : 1; /*!< [0..0] Command Error Flag 0                                               */
            __IM uint32_t CMDE1    : 1; /*!< [1..1] Command Error Flag 1                                               */
            __IM uint32_t RSPLENE0 : 1; /*!< [2..2] Response Length Error Flag 0                                       */
            __IM uint32_t RSPLENE1 : 1; /*!< [3..3] Response Length Error Flag 1                                       */
            __IM uint32_t RDLENE   : 1; /*!< [4..4] Read Data Length Error Flag                                        */
            __IM uint32_t CRCLENE  : 1; /*!< [5..5] CRC Status Token Length Error Flag                                 */
            uint32_t               : 2;
            __IM uint32_t RSPCRCE0 : 1; /*!< [8..8] Response CRC Error Flag 0                                          */
            __IM uint32_t RSPCRCE1 : 1; /*!< [9..9] Response CRC Error Flag 1                                          */
            __IM uint32_t RDCRCE   : 1; /*!< [10..10] Read Data CRC Error Flag                                         */
            __IM uint32_t CRCTKE   : 1; /*!< [11..11] CRC Status Token Error Flag                                      */
            __IM uint32_t CRCTK    : 3; /*!< [14..12] CRC Status Token                                                 */
            uint32_t               : 17;
        } SD_ERR_STS1_b;
    };

    union
    {
        __IM uint32_t SD_ERR_STS2;      /*!< (@ 0x0000005C) SD Error Status Register 2                                 */

        struct
        {
            __IM uint32_t RSPTO0   : 1; /*!< [0..0] Response Timeout Flag 0                                            */
            __IM uint32_t RSPTO1   : 1; /*!< [1..1] Response Timeout Flag 1                                            */
            __IM uint32_t BSYTO0   : 1; /*!< [2..2] Busy Timeout Flag 0                                                */
            __IM uint32_t BSYTO1   : 1; /*!< [3..3] Busy Timeout Flag 1                                                */
            __IM uint32_t RDTO     : 1; /*!< [4..4] Read Data Timeout Flag                                             */
            __IM uint32_t CRCTO    : 1; /*!< [5..5] CRC Status Token Timeout Flag                                      */
            __IM uint32_t CRCBSYTO : 1; /*!< [6..6] CRC Status Token Busy Timeout Flag                                 */
            uint32_t               : 25;
        } SD_ERR_STS2_b;
    };
    __IOM uint32_t SD_BUF0;             /*!< (@ 0x00000060) SD Buffer Register                                         */
    __IM uint32_t  RESERVED7;

    union
    {
        __IOM uint32_t SDIO_MODE;      /*!< (@ 0x00000068) SDIO Mode Control Register                                 */

        struct
        {
            __IOM uint32_t INTEN  : 1; /*!< [0..0] SDIO Interrupt Acceptance Enable                                   */
            uint32_t              : 1;
            __IOM uint32_t RWREQ  : 1; /*!< [2..2] Read Wait Request                                                  */
            uint32_t              : 5;
            __IOM uint32_t IOABT  : 1; /*!< [8..8] SDIO Abort                                                         */
            __IOM uint32_t C52PUB : 1; /*!< [9..9] SDIO None Abort                                                    */
            uint32_t              : 22;
        } SDIO_MODE_b;
    };

    union
    {
        __IOM uint32_t SDIO_INFO1;      /*!< (@ 0x0000006C) SDIO Status Register                                       */

        struct
        {
            __IOM uint32_t IOIRQ   : 1; /*!< [0..0] SDIO Interrupt Status Flag                                         */
            uint32_t               : 13;
            __IOM uint32_t EXPUB52 : 1; /*!< [14..14] EXPUB52 Status Flag                                              */
            __IOM uint32_t EXWT    : 1; /*!< [15..15] EXWT Status Flag                                                 */
            uint32_t               : 16;
        } SDIO_INFO1_b;
    };

    union
    {
        __IOM uint32_t SDIO_INFO1_MASK;  /*!< (@ 0x00000070) SDIO Interrupt Mask Register                               */

        struct
        {
            __IOM uint32_t IOIRQM   : 1; /*!< [0..0] IOIRQ Interrupt Mask Control                                       */
            uint32_t                : 13;
            __IOM uint32_t EXPUB52M : 1; /*!< [14..14] EXPUB52 Interrupt Request Mask Control                           */
            __IOM uint32_t EXWTM    : 1; /*!< [15..15] EXWT Interrupt Request Mask Control                              */
            uint32_t                : 16;
        } SDIO_INFO1_MASK_b;
    };
    __IM uint32_t RESERVED8[79];

    union
    {
        __IOM uint32_t SD_DMAEN;       /*!< (@ 0x000001B0) DMA Transfer Enable Register                               */

        struct
        {
            uint32_t             : 1;
            __IOM uint32_t DMAEN : 1;  /*!< [1..1] DMA Transfer Enable                                                */
            uint32_t             : 30;
        } SDDMAEN_b;
    };
    __IM uint32_t RESERVED9[3];

    union
    {
        __IOM uint32_t SOFT_RST;       /*!< (@ 0x000001C0) SDHI Software Reset Register                               */

        struct
        {
            __IOM uint32_t SDRST : 1;  /*!< [0..0] SDHI Software Reset Control                                        */
            uint32_t             : 31;
        } SOFT_RST_b;
    };
    __IM uint32_t RESERVED10[7];

    union
    {
        __IOM uint32_t EXT_SWAP;       /*!< (@ 0x000001E0) Swap Control Register                                      */

        struct
        {
            uint32_t             : 6;
            __IOM uint32_t BWSWP : 1;  /*!< [6..6] SDBUFR Swap Write                                                  */
            __IOM uint32_t BRSWP : 1;  /*!< [7..7] SDBUFR Swap Read                                                   */
            uint32_t             : 24;
        } EXT_SWAP_b;
    };
} R_SDHI0_Type;                        /*!< Size = 484 (0x1e4)                                                        */

#endif /* R_SDHI_REG_H */
