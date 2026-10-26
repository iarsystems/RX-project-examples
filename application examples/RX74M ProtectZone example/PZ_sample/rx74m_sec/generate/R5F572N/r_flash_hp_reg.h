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

#ifndef R_FLASH_HP_REG_H
#define R_FLASH_HP_REG_H

/* =========================================================================================================================== */
/* ================                                       R_FACI_HP_CMD                                       ================ */
/* =========================================================================================================================== */

/**
 * @brief Flash Application Command Interface Command-Issuing Area (R_FACI_HP_CMD)
 */

typedef struct                         /*!< (@ 0x407E0000) R_FACI_HP_CMD Structure                                    */
{
    union
    {
        __IOM uint16_t FACI_CMD16;     /*!< (@ 0x00000000) FACI Command Issuing Area (halfword access)                */
        __IOM uint8_t  FACI_CMD8;      /*!< (@ 0x00000000) FACI Command Issuing Area (halfword access)                */
    };
} R_FACI_HP_CMD_Type;                  /*!< Size = 2 (0x2)                                                            */

/* =========================================================================================================================== */
/* ================                                         R_FACI_HP                                         ================ */
/* =========================================================================================================================== */

/**
 * @brief Flash Application Command Interface (R_FACI_HP)
 */

typedef struct                         /*!< (@ 0x407FE000) R_FACI_HP Structure                                        */
{
    __IM uint32_t RESERVED[4];

    union
    {
        __IOM uint8_t FASTAT;          /*!< (@ 0x00000010) Flash Access Status                                        */

        struct
        {
            __IM uint8_t ECRCT  : 1;   /*!< [0..0] ECRCT                                                              */
            uint8_t             : 2;
            __IOM uint8_t DFAE  : 1;   /*!< [3..3] Data Flash Access Error                                            */
            __IM uint8_t  CMDLK : 1;   /*!< [4..4] Command Lock                                                       */
            uint8_t             : 2;
            __IOM uint8_t CFAE  : 1;   /*!< [7..7] Code Flash Access Error                                            */
        } FASTAT_b;
    };
    __IM uint8_t  RESERVED1;
    __IM uint16_t RESERVED2;

    union
    {
        __IOM uint8_t FAEINT;          /*!< (@ 0x00000014) Flash Access Error Interrupt Enable                        */

        struct
        {
            __IOM uint8_t ECRCTIE : 1; /*!< [0..0] Error Correct Interrupt Enable                                     */
            uint8_t               : 2;
            __IOM uint8_t DFAEIE  : 1; /*!< [3..3] Data Flash Access Error Interrupt Enable                           */
            __IOM uint8_t CMDLKIE : 1; /*!< [4..4] Command Lock Interrupt Enable                                      */
            uint8_t               : 2;
            __IOM uint8_t CFAEIE  : 1; /*!< [7..7] Code Flash Access Error Interrupt Enable                           */
        } FAEINT_b;
    };
    __IM uint8_t  RESERVED3;
    __IM uint16_t RESERVED4;

    union
    {
        __IOM uint8_t FRDYIE;          /*!< (@ 0x00000018) Flash Ready Interrupt Enable                               */

        struct
        {
            __IOM uint8_t FRDYIE : 1;  /*!< [0..0] FRDY Interrupt Enable                                              */
            uint8_t              : 7;
        } FRDYIE_b;
    };
    __IM uint8_t  RESERVED5;
    __IM uint16_t RESERVED6;
    __IM uint32_t RESERVED7[5];

    union
    {
        __IOM uint32_t FSADDR;         /*!< (@ 0x00000030) Flash Start Address                                        */

        struct
        {
            __IOM uint32_t FSA : 32;   /*!< [31..0] Start Address of Flash Sequencer Command Target Area
                                        *   These bits can be written when FRDY bit of FSTATR register
                                        *   is "1". Writing to these bits in FRDY = "0" is ignored.                   */
        } FSADDR_b;
    };

    union
    {
        __IOM uint32_t FEADDR;         /*!< (@ 0x00000034) Flash End Address                                          */

        struct
        {
            __IOM uint32_t FEA : 32;   /*!< [31..0] End Address of Flash Sequencer Command Target Area Specifies
                                        *   end address of target area in "Blank Check" command. These
                                        *   bits can be written when FRDY bit of FSTATR register is
                                        *   "1". Writing to these bits in FRDY = "0" is ignored.                      */
        } FEADDR_b;
    };
    __IM uint32_t RESERVED8[3];

    union
    {
        __IOM uint16_t FMEPROT;        /*!< (@ 0x00000044) Flash P/E Mode Entry Protection Register                   */

        struct
        {
            __IOM uint16_t CEPROT : 1; /*!< [0..0] Code Flash P/E Mode Entry ProtectionWriting to this bit
                                        *   is only possible when the FRDY bit in the FSTATR register
                                        *   is 1. Writing to this bit while the FRDY bit = 0 isignored.Writing
                                        *   to this bit is only possible when 16 bits are written and
                                        *   the value written to the KEY bits is D9h.Written values
                                        *   are not retained by these bits (always read as 0x00).Only
                                        *   secure access can write to this register. Both secure access
                                        *   and non-secure read access are allowed. Non-secure writeaccess
                                        *   is denied, but Trust                                                      */
            uint16_t          : 7;
            __OM uint16_t KEY : 8;     /*!< [15..8] KEY Code                                                          */
        } FMEPROT_b;
    };
    __IM uint16_t RESERVED9;
    __IM uint32_t RESERVED10[12];

    union
    {
        __IOM uint16_t FBPROT0;        /*!< (@ 0x00000078) Flash Block Protection Register                            */

        struct
        {
            __IOM uint16_t BPCN0 : 1;  /*!< [0..0] Block Protection for Non-secure CancelThis bit can be
                                        *   written when the FRDY bit in the FSTATR register is 1.
                                        *   Writing to this bit is ignored when the FRDY bit is 0.Writing
                                        *   to this bit is only possible when 16 bits are written and
                                        *   the value written to the KEY[7:0] bits is 0x78.Written
                                        *   values are not retained by these bits (always read as 0x00).              */
            uint16_t          : 7;
            __OM uint16_t KEY : 8;     /*!< [15..8] KEY Code                                                          */
        } FBPROT0_b;
    };
    __IM uint16_t RESERVED11;

    union
    {
        __IOM uint16_t FBPROT1;        /*!< (@ 0x0000007C) Flash Block Protection for Secure Register                 */

        struct
        {
            __IOM uint16_t BPCN1 : 1;  /*!< [0..0] Block Protection for Secure CancelWriting to this bit
                                        *   is only possible when the FRDY bit in the FSTATR register
                                        *   is 1. Writing to this bit while FRDY bit = 0 is ignored.Writing
                                        *   to this bit is only possible when 16 bits are written and
                                        *   the value written to the KEY[7:0] bits is 0xB1.Written
                                        *   values are not retained by these bits (always read as 0x00).              */
            uint16_t          : 7;
            __OM uint16_t KEY : 8;     /*!< [15..8] KEY Code                                                          */
        } FBPROT1_b;
    };
    __IM uint16_t RESERVED12;

    union
    {
        __IM uint32_t FSTATR;          /*!< (@ 0x00000080) Flash Status                                               */

        struct
        {
            uint32_t                 : 6;
            __IM uint32_t FLWEERR    : 1; /*!< [6..6] Flash Write/Erase Protect Error Flag                               */
            uint32_t                 : 1;
            __IM uint32_t PRGSPD     : 1; /*!< [8..8] Programming-Suspended Status                                       */
            __IM uint32_t ERSSPD     : 1; /*!< [9..9] Erasure-Suspended Status                                           */
            __IM uint32_t DBFULL     : 1; /*!< [10..10] Data Buffer Full                                                 */
            __IM uint32_t SUSRDY     : 1; /*!< [11..11] Suspend Ready                                                    */
            __IM uint32_t PRGERR     : 1; /*!< [12..12] Programming Error                                                */
            __IM uint32_t ERSERR     : 1; /*!< [13..13] Erasure Error                                                    */
            __IM uint32_t ILGLERR    : 1; /*!< [14..14] Illegal Command Error                                            */
            __IM uint32_t FRDY       : 1; /*!< [15..15] Flash Ready                                                      */
            uint32_t                 : 4;
            __IM uint32_t  OTERR     : 1; /*!< [20..20] Other Error                                                      */
            __IOM uint32_t SECERR    : 1; /*!< [21..21] Security Error                                                   */
            __IM uint32_t  FESETERR  : 1; /*!< [22..22] FENTRY Setting Error                                             */
            __IM uint32_t  ILGCOMERR : 1; /*!< [23..23] Illegal Command Error                                            */
            uint32_t                 : 8;
        } FSTATR_b;
    };

    union
    {
        __IOM uint16_t FENTRYR;         /*!< (@ 0x00000084) Program/Erase Mode Entry                                   */

        struct
        {
            __IOM uint16_t FENTRYC : 1; /*!< [0..0] Code Flash P/E Mode Entry These bits can be written when
                                         *   FRDY bit in FSTATR register is "1". Writing to this bit
                                         *   in FRDY = "0" is ignored. Writing to these bits is enabled
                                         *   only when this register is accessed in 16-bit size and
                                         *   H'AA is written to KEY bits                                               */
            uint16_t               : 6;
            __IOM uint16_t FENTRYD : 1; /*!< [7..7] Data Flash P/E Mode Entry These bits can be written when
                                         *   FRDY bit in FSTATR register is "1". Writing to this bit
                                         *   in FRDY = "0" is ignored. Writing to these bits is enabled
                                         *   only when this register is accessed in 16-bit size and
                                         *   H'AA is written to KEY bits.                                              */
            __OM uint16_t KEY : 8;      /*!< [15..8] KEY Code                                                          */
        } FENTRYR_b;
    };
    __IM uint16_t RESERVED13;
    __IM uint32_t RESERVED14;

    union
    {
        __IOM uint16_t FSUINITR;       /*!< (@ 0x0000008C) Flash Sequencer Set-up Initialize                          */

        struct
        {
            __IOM uint16_t SUINIT : 1; /*!< [0..0] Set-up Initialization This bit can be written when FRDY
                                        *   bit of FSTATR register is "1". Writing to this bit in FRDY
                                        *   = "0" is ignored. Writing to these bits is enabled only
                                        *   when this register is accessed in 16-bit size and H'2D
                                        *   is written to KEY bits.                                                   */
            uint16_t          : 7;
            __OM uint16_t KEY : 8;     /*!< [15..8] KEY Code                                                          */
        } FSUINITR_b;
    };
    __IM uint16_t RESERVED15;
    __IM uint32_t RESERVED16[4];

    union
    {
        __IM uint16_t FCMDR;           /*!< (@ 0x000000A0) Flash Sequencer Command                                    */

        struct
        {
            __IM uint16_t PCMDR : 8;   /*!< [7..0] Previous Command Register                                          */
            __IM uint16_t CMDR  : 8;   /*!< [15..8] Command Register                                                  */
        } FCMDR_b;
    };
    __IM uint16_t RESERVED17;
    __IM uint32_t RESERVED18[7];

    union
    {
        __IM uint16_t FPESTAT;         /*!< (@ 0x000000C0) Program/Erase Error Status                                 */

        struct
        {
            __IM uint16_t PEERRST : 8; /*!< [7..0] P/E Error Status                                                   */
            uint16_t              : 8;
        } FPESTAT_b;
    };
    __IM uint16_t RESERVED19;
    __IM uint32_t RESERVED20[3];

    union
    {
        __IOM uint8_t FBCCNT;          /*!< (@ 0x000000D0) Blank Check Control                                        */

        struct
        {
            __IOM uint8_t BCDIR : 1;   /*!< [0..0] Blank Check Direction                                              */
            uint8_t             : 7;
        } FBCCNT_b;
    };
    __IM uint8_t  RESERVED21;
    __IM uint16_t RESERVED22;

    union
    {
        __IM uint8_t FBCSTAT;          /*!< (@ 0x000000D4) Blank Check Status                                         */

        struct
        {
            __IM uint8_t BCST : 1;     /*!< [0..0] Blank Check Status Bit                                             */
            uint8_t           : 7;
        } FBCSTAT_b;
    };
    __IM uint8_t  RESERVED23;
    __IM uint16_t RESERVED24;

    union
    {
        __IM uint32_t FPSADDR;         /*!< (@ 0x000000D8) Programmed Area Start Address                              */

        struct
        {
            __IM uint32_t PSADR : 19;  /*!< [18..0] Programmed Area Start Address NOTE: Indicates address
                                        *   of the first programmed data which is found in "Blank Check"
                                        *   command execution.                                                        */
            uint32_t : 13;
        } FPSADDR_b;
    };

    union
    {
        __IM uint32_t FAWMON;          /*!< (@ 0x000000DC) Flash Access Window Monitor                                */

        struct
        {
            __IM uint32_t FAWS : 11;   /*!< [10..0] Start Sector Address for Access Window NOTE: These bits
                                        *   indicate the start sector address for setting the access
                                        *   window that is located in the configuration area.                         */
            uint32_t           : 4;
            __IM uint32_t FSPR : 1;    /*!< [15..15] Protection Flag of programming the Access Window, Boot
                                        *   Flag and Temporary Boot Swap Control and "Config Clear"
                                        *   command execution                                                         */
            __IM uint32_t FAWE : 11;   /*!< [26..16] End Sector Address for Access Window NOTE: These bits
                                        *   indicate the end sector address for setting the access
                                        *   window that is located in the configuration area.                         */
            uint32_t            : 4;
            __IM uint32_t BTFLG : 1;   /*!< [31..31] Flag of Start-Up area select for Boot Swap                       */
        } FAWMON_b;
    };

    union
    {
        __IOM uint16_t FCPSR;           /*!< (@ 0x000000E0) FCU Process Switch                                         */

        struct
        {
            __IOM uint16_t ESUSPMD : 1; /*!< [0..0] Erasure-Suspended Mode                                             */
            uint16_t               : 15;
        } FCPSR_b;
    };
    __IM uint16_t RESERVED25;

    union
    {
        __IOM uint16_t FPCKAR;         /*!< (@ 0x000000E4) Flash Sequencer Processing Clock Frequency Notification    */

        struct
        {
            __IOM uint16_t PCKA : 8;   /*!< [7..0] Flash Sequencer Processing Clock Frequency These bits
                                        *   can be written when FRDY bit in FSTATR register is "1".
                                        *   Writing to this bit in FRDY = "0" is ignored. Writing to
                                        *   these bits is enabled only when this register is accessed
                                        *   in 16-bit size and H'1E is written to KEY bits.                           */
            __OM uint16_t KEY : 8;     /*!< [15..8] KEY Code                                                          */
        } FPCKAR_b;
    };
    __IM uint16_t RESERVED26;

    union
    {
        __IOM uint16_t FSUACR;         /*!< (@ 0x000000E8) Flash Start-Up Area Control Register                       */

        struct
        {
            __IOM uint16_t SAS : 2;    /*!< [1..0] Start Up Area Select These bits can be written when FRDY
                                        *   bit in FSTATR register is "1". Writing to this bit in FRDY
                                        *   = "0" is ignored. Writing to these bits is enabled only
                                        *   when this register is accessed in 16-bit size and H'66
                                        *   is written to KEY bits.                                                   */
            uint16_t          : 6;
            __OM uint16_t KEY : 8;     /*!< [15..8] KEY Code                                                          */
        } FSUACR_b;
    };
    __IM uint16_t RESERVED27;
} R_FACI_HP_Type;                      /*!< Size = 236 (0xec)                                                         */

#endif /* FLASH_HP_REG_H */
