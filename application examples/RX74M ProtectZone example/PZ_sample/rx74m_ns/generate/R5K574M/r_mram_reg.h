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

#ifndef R_MRAM_REG_H
#define R_MRAM_REG_H

/* =========================================================================================================================== */
/* ================                                          R_MRMS                                           ================ */
/* =========================================================================================================================== */

/**
 * @brief MRMS (R_MRMS)
 */

typedef struct                         /*!< (@ 0x8709C000) R_MRMS Structure                                           */
{
    union
    {
        __IOM uint8_t MRCPFB;          /*!< (@ 0x00000000) Code MRAM Pre-Fetch Buffer Enable Register                 */

        struct
        {
            __IOM uint8_t MPFBEN : 1;  /*!< [0..0] MRAM Pre-Fetch Buffer enable.                                      */
            uint8_t              : 7;
        } MRCPFB_b;
    };
    __IM uint8_t  RESERVED;
    __IM uint16_t RESERVED1;

    union
    {
        __IOM uint32_t MRCFREQ;         /*!< (@ 0x00000004) Code MRAM Frequency Notifications Register                 */

        struct
        {
            __IOM uint32_t MRCMHZ : 10; /*!< [9..0] Setting the operating frequency in the order of MHz                */
            uint32_t              : 14;
            __IOM uint32_t KEY    : 8;  /*!< [31..24] Key Code                                                         */
        } MRCFREQ_b;
    };

    union
    {
        __IOM uint32_t MREFREQ;        /*!< (@ 0x00000008) Extra MRAM Frequency Notifications Register                */

        struct
        {
            __IOM uint32_t MREMHZ : 8; /*!< [7..0] Setting the operating frequency in the order of MHz                */
            uint32_t              : 16;
            __IOM uint32_t KEY    : 8; /*!< [31..24] Key Code                                                         */
        } MREFREQ_b;
    };
    __IM uint32_t RESERVED2;

    union
    {
        __IOM uint16_t MRCDECC;         /*!< (@ 0x00000010) Code MRAM ECC Decoder Control Register                     */

        struct
        {
            __IOM uint16_t DECDISC : 1; /*!< [0..0] MRC ECC Decoder disable                                            */
            __IOM uint16_t ECCSELC : 1; /*!< [1..1] MRC ECC Data select                                                */
            uint16_t               : 6;
            __IOM uint16_t KEY     : 8; /*!< [15..8] Key Code                                                          */
        } MRCDECC_b;
    };
    __IM uint16_t RESERVED3;

    union
    {
        __IOM uint8_t MRCRAEINT;        /*!< (@ 0x00000014) Code MRAM Read Access Error Interrupt Enable
                                         *                  Register                                                   */

        struct
        {
            __IOM uint8_t INTENBDC : 1; /*!< [0..0] MRC DEC error interrupt enable.                                    */
            __IOM uint8_t INTENBTC : 1; /*!< [1..1] MRC TED error interrupt enable.                                    */
            uint8_t                : 6;
        } MRCRAEINT_b;
    };
    __IM uint8_t  RESERVED4;
    __IM uint16_t RESERVED5;

    union
    {
        __IOM uint8_t MRCRAES;         /*!< (@ 0x00000018) Code MRAM Read Access Error Status Register                */

        struct
        {
            __IOM uint8_t DECERRC : 1; /*!< [0..0] MRC DEC error detected.                                            */
            __IOM uint8_t TEDERRC : 1; /*!< [1..1] MRC TED error detected.                                            */
            uint8_t               : 6;
        } MRCRAES_b;
    };
    __IM uint8_t  RESERVED6;
    __IM uint16_t RESERVED7;

    union
    {
        __IOM uint32_t MRCRTEA;        /*!< (@ 0x0000001C) Code MRAM TED Error Address Register                       */

        struct
        {
            uint32_t              : 5;
            __IM uint32_t MRCRTEA : 27; /*!< [31..5] MRC read access TED error Address.                                */
        } MRCRTEA_b;
    };

    union
    {
        __IOM uint32_t MRCRDEA;        /*!< (@ 0x00000020) Code MRAM DEC Error Address Register                       */

        struct
        {
            uint32_t              : 5;
            __IM uint32_t MRCRDEA : 27; /*!< [31..5] MRC read access DEC error Address.                                */
        } MRCRDEA_b;
    };
    __IM uint32_t RESERVED8[3];

    union
    {
        __IOM uint16_t MREDECC;         /*!< (@ 0x00000030) MRE ECC Decoder Control Register                           */

        struct
        {
            __IOM uint16_t DECDISE : 1; /*!< [0..0] MRE ECC Decoder disable                                            */
            __IOM uint16_t ECCSELE : 1; /*!< [1..1] MRE ECC Data select                                                */
            uint16_t               : 6;
            __IOM uint16_t KEY     : 8; /*!< [15..8] Key Code                                                          */
        } MREDECC_b;
    };
    __IM uint16_t RESERVED9;

    union
    {
        __IOM uint8_t MRERAEINT;        /*!< (@ 0x00000034) Extra MRAM Read Access Error Interrupt Enable
                                         *                  Register                                                   */

        struct
        {
            __IOM uint8_t INTENBDE : 1; /*!< [0..0] MRE DEC error interrupt enable.                                    */
            __IOM uint8_t INTENBTE : 1; /*!< [1..1] MRE TED error interrupt enable.                                    */
            uint8_t                : 6;
        } MRERAEINT_b;
    };
    __IM uint8_t  RESERVED10;
    __IM uint16_t RESERVED11;

    union
    {
        __IOM uint8_t MRERAES;         /*!< (@ 0x00000038) Extra MRAM Read Access Error Status Register               */

        struct
        {
            __IOM uint8_t DECERRE : 1; /*!< [0..0] MRE DEC error detected.                                            */
            __IOM uint8_t TEDERRE : 1; /*!< [1..1] MRE TED error detected.                                            */
            uint8_t               : 6;
        } MRERAES_b;
    };
    __IM uint8_t  RESERVED12;
    __IM uint16_t RESERVED13;

    union
    {
        __IOM uint32_t MRERTEA;        /*!< (@ 0x0000003C) Extra MRAM TED Error Address Register                      */

        struct
        {
            uint32_t              : 4;
            __IM uint32_t MRERTEA : 28; /*!< [31..4] MRE read access TED error Address.                                */
        } MRERTEA_b;
    };

    union
    {
        __IOM uint32_t MRERDEA;        /*!< (@ 0x00000040) Extra MRAM DEC Error Address Register                      */

        struct
        {
            uint32_t              : 4;
            __IM uint32_t MRERDEA : 28; /*!< [31..4] MRE read access DEC error Address.                                */
        } MRERDEA_b;
    };
    __IM uint32_t RESERVED14[47];

    union
    {
        __IOM uint16_t MSAR;              /*!< (@ 0x00000100) MRAM Security Attribution Register                         */

        struct
        {
            __IOM uint16_t MREECCSA  : 1; /*!< [0..0] Extra MRAM ECC Register Security Attribution                       */
            __IOM uint16_t MREFREQSA : 1; /*!< [1..1] MREFREQ register Security Attribution                              */
            __IOM uint16_t MRCECCSA  : 1; /*!< [2..2] Code MRAM ECC Register Security Attribution                        */
            __IOM uint16_t MRCFREQSA : 1; /*!< [3..3] MRCFREQ register Security Attribution                              */
            __IOM uint16_t MPFBENSA  : 1; /*!< [4..4] MRCPFB register Security Attribution                               */
            uint16_t                 : 4;
            __IOM uint16_t MACICMISA : 1; /*!< [9..9] MACI Command Issuing Security Attribution                          */
            __IOM uint16_t MACICMRSA : 1; /*!< [10..10] MACI Command Registers Security Attribution                      */
            __IOM uint16_t MACITRSA  : 1; /*!< [11..11] MACI Transfer Security Attribution                               */
            __IOM uint16_t MRDPSA    : 1; /*!< [12..12] MRDP Registers Security Attribution                              */
            __IOM uint16_t MRCPSA    : 1; /*!< [13..13] Code MRAM Program Register Security Attribution                  */
            __IOM uint16_t MREPSEQSA : 1; /*!< [14..14] EPSEQ Area Register Security Attribution                         */
            __IOM uint16_t MRCPSEQSA : 1; /*!< [15..15] CPSEQ Area Register Security Attribution                         */
        } MSAR_b;
    };
    __IM uint16_t RESERVED15;
    __IM uint32_t RESERVED16[191];

    union
    {
        __IOM uint8_t MREZS;           /*!< (@ 0x00000400) Extra MRAM Zeroization Status Register                     */

        struct
        {
            __IM uint8_t WHUKZF  : 1;  /*!< [0..0] W-HUK Zero Flag Status.                                            */
            __IM uint8_t WHUKEXE : 1;  /*!< [1..1] W-HUK Zeroization Executing Status                                 */
            uint8_t              : 6;
        } MREZS_b;
    };
    __IM uint8_t  RESERVED17;
    __IM uint16_t RESERVED18;

    union
    {
        __IOM uint16_t MREZC;          /*!< (@ 0x00000404) Extra MRAM Zeroization Control Register                    */

        struct
        {
            __IOM uint16_t WHUKZE : 3; /*!< [2..0] W-KUK zeroization execute.                                         */
            uint16_t              : 5;
            __IOM uint16_t KEY    : 8; /*!< [15..8] Key Code                                                          */
        } MREZC_b;
    };
    __IM uint16_t RESERVED19;
    __IM uint32_t RESERVED20[1794];

    union
    {
        __IOM uint8_t MREASTAT;        /*!< (@ 0x00002010) MRE Access Status Register                                 */

        struct
        {
            uint8_t             : 3;
            __IOM uint8_t MREAE : 1;   /*!< [3..3] Extra MRAM Access Error                                            */
            __IM uint8_t  CMDLK : 1;   /*!< [4..4] Command Lock                                                       */
            uint8_t             : 3;
        } MREASTAT_b;
    };
    __IM uint8_t  RESERVED21;
    __IM uint16_t RESERVED22;

    union
    {
        __IOM uint8_t MREPAEINT;       /*!< (@ 0x00002014) MRE Write Access Error Interrupt Enable Register           */

        struct
        {
            uint8_t               : 3;
            __IOM uint8_t MREAEIE : 1; /*!< [3..3] MRE Access Error Interrupt Enable                                  */
            __IOM uint8_t CMDLKIE : 1; /*!< [4..4] Command Lock Interrupt Enable                                      */
            uint8_t               : 3;
        } MREPAEINT_b;
    };
    __IM uint8_t  RESERVED23;
    __IM uint16_t RESERVED24;

    union
    {
        __IOM uint8_t MRDYIE;          /*!< (@ 0x00002018) MRDY Interrupt Enable Register                             */

        struct
        {
            __IOM uint8_t MRDYIE : 1;  /*!< [0..0] MRDY Interrupt Enable                                              */
            uint8_t              : 7;
        } MRDYIE_b;
    };
    __IM uint8_t  RESERVED25;
    __IM uint16_t RESERVED26;
    __IM uint32_t RESERVED27[5];

    union
    {
        __IOM uint32_t MRESADDR;       /*!< (@ 0x00002030) MRE Start Address Register                                 */

        struct
        {
            uint32_t           : 4;
            __IOM uint32_t MSA : 28;   /*!< [31..4] Start Address of MRAM Sequencer Command Target Area               */
        } MRESADDR_b;
    };
    __IM uint32_t RESERVED28[5];

    union
    {
        __IOM uint8_t MCNTSELR;        /*!< (@ 0x00002048) MRAM Counter Select Register                               */

        struct
        {
            __IOM uint8_t CNTSEL : 3;  /*!< [2..0] Counter Select                                                     */
            uint8_t              : 5;
        } MCNTSELR_b;
    };
    __IM uint8_t  RESERVED29;
    __IM uint16_t RESERVED30;

    union
    {
        __IM uint32_t MCNTDATAR[2];     /*!< (@ 0x0000204C) MRAM Counter Data Register [0..1]                          */

        struct
        {
            __IM uint32_t CNTRDAT : 32; /*!< [31..0] Counter Read Data                                                 */
        } MCNTDATAR_b[2];
    };
    __IM uint32_t RESERVED31[3];

    union
    {
        __IOM uint16_t MCTRCNTR;       /*!< (@ 0x00002060) MRAM Configuration Update Transfer Control Register        */

        struct
        {
            __IOM uint16_t TRTRG : 1;  /*!< [0..0] Transfer Start Trigger                                             */
            __IOM uint16_t TRSEL : 1;  /*!< [1..1] Transfer Information Select                                        */
            uint16_t             : 6;
            __IOM uint16_t KEY   : 8;  /*!< [15..8] Key Code                                                          */
        } MCTRCNTR_b;
    };
    __IM uint16_t RESERVED32;

    union
    {
        __IOM uint8_t MCTRLSR;         /*!< (@ 0x00002064) MRAM Configuration Update Transfer List Select
                                        *                  Register                                                   */

        struct
        {
            __IOM uint8_t TRLIST : 3;  /*!< [2..0] Configuration Update Transfer List                                 */
            uint8_t              : 5;
        } MCTRLSR_b;
    };
    __IM uint8_t  RESERVED33;
    __IM uint16_t RESERVED34;

    union
    {
        __IOM uint32_t MCTRADDR;       /*!< (@ 0x00002068) MRAM Configuration Update Transfer Address Register        */

        struct
        {
            __IOM uint32_t CTRA : 32;  /*!< [31..0] Configuration Update Transfer Source Address                      */
        } MCTRADDR_b;
    };

    union
    {
        __IOM uint8_t MCTRSTATR;       /*!< (@ 0x0000206C) MRAM Configuration Update Transfer Status Register         */

        struct
        {
            __IM uint8_t TRBUSY : 1;   /*!< [0..0] Transfer Busy Status                                               */
            __IM uint8_t TRAD   : 1;   /*!< [1..1] Transfer Address Setting Status                                    */
            __IM uint8_t TRMD   : 1;   /*!< [2..2] Transfer Mode Setting Status                                       */
            uint8_t             : 5;
        } MCTRSTATR_b;
    };
    __IM uint8_t  RESERVED35;
    __IM uint16_t RESERVED36;
    __IM uint32_t RESERVED37[4];

    union
    {
        __IM uint32_t MSTATR;          /*!< (@ 0x00002080) MRE Status Register                                        */

        struct
        {
            uint32_t                : 4;
            __IM uint32_t PROTERR   : 1; /*!< [4..4] Update error in protected area                                     */
            __IM uint32_t CFGPRGERR : 1; /*!< [5..5] Config Program Error                                               */
            uint32_t                : 6;
            __IM uint32_t PRGERR    : 1; /*!< [12..12] Programming Error                                                */
            uint32_t                : 1;
            __IM uint32_t ILGLERR   : 1; /*!< [14..14] Illegal Error                                                    */
            __IM uint32_t MRDY      : 1; /*!< [15..15] MRE Ready                                                        */
            uint32_t                : 3;
            __IM uint32_t TZFERR    : 1; /*!< [19..19] TrustZone Filter Error                                           */
            __IM uint32_t OTERR     : 1; /*!< [20..20] Other Error                                                      */
            __IM uint32_t SECERR    : 1; /*!< [21..21] Security Error                                                   */
            uint32_t                : 1;
            __IM uint32_t ILGCOMERR : 1; /*!< [23..23] Illegal Command Error                                            */
            uint32_t                : 8;
        } MSTATR_b;
    };

    union
    {
        __IOM uint16_t MENTRYR;        /*!< (@ 0x00002084) Program Mode Entry Register                                */

        struct
        {
            uint16_t              : 7;
            __IOM uint16_t MENTRY : 1; /*!< [7..7] Extra MRAM Mode Entry                                              */
            __IOM uint16_t KEY    : 8; /*!< [15..8] Key Code                                                          */
        } MENTRYR_b;
    };
    __IM uint16_t RESERVED38;
    __IM uint32_t RESERVED39;

    union
    {
        __IOM uint16_t MSUINITR;       /*!< (@ 0x0000208C) MRE Sequencer Set-up Initialize Register                   */

        struct
        {
            __IOM uint16_t SUINIT : 1; /*!< [0..0] Set-up Initialization                                              */
            uint16_t              : 7;
            __IOM uint16_t KEY    : 8; /*!< [15..8] Key Code                                                          */
        } MSUINITR_b;
    };
    __IM uint16_t RESERVED40;
    __IM uint32_t RESERVED41[4];

    union
    {
        __IM uint16_t MCMDR;           /*!< (@ 0x000020A0) MRE Sequencer Command Register                             */

        struct
        {
            __IM uint16_t PCMDR : 8;   /*!< [7..0] Previous Command Register                                          */
            __IM uint16_t CMDR  : 8;   /*!< [15..8] Command Register                                                  */
        } MCMDR_b;
    };
    __IM uint16_t RESERVED42;
    __IM uint32_t RESERVED43[14];

    union
    {
        __IM uint32_t MSUASMON;        /*!< (@ 0x000020DC) MRAM Start-Up Area Select Monitor Register                 */

        struct
        {
            uint32_t            : 14;
            __IM uint32_t FSPRS : 1;   /*!< [14..14] Protection Flag of programing to set the Start-Up Size
                                        *   Select setting                                                            */
            __IM uint32_t FSPR : 1;    /*!< [15..15] Protection Flag of programing to set the Start-Up Area
                                        *   Select setting                                                            */
            uint32_t             : 13;
            __IM uint32_t BTSIZE : 2;  /*!< [30..29] Size of Start-Up area select for Boot Swap                       */
            __IM uint32_t BTFLG  : 1;  /*!< [31..31] Flag of Start-Up area select for Boot Swap                       */
        } MSUASMON_b;
    };
    __IM uint32_t RESERVED44[2];

    union
    {
        __IOM uint16_t MSUACR;         /*!< (@ 0x000020E8) MRAM Start-Up Area Control Register                        */

        struct
        {
            __IOM uint16_t SAS : 2;    /*!< [1..0] Start Up Area Select                                               */
            uint16_t           : 6;
            __IOM uint16_t KEY : 8;    /*!< [15..8] Key Code                                                          */
        } MSUACR_b;
    };
    __IM uint16_t RESERVED45;
    __IM uint32_t RESERVED46;

    union
    {
        __IOM uint8_t MRSTTRST;        /*!< (@ 0x000020F0) MRAM Reset Transfer Status Register                        */

        struct
        {
            __IM uint8_t RSTTRER : 1;  /*!< [0..0] Status of the MACI reset transfer.                                 */
            uint8_t              : 7;
        } MRSTTRST_b;
    };
    __IM uint8_t  RESERVED47;
    __IM uint16_t RESERVED48;
    __IM uint32_t RESERVED49[451];

    union
    {
        __IOM uint8_t MRPC;            /*!< (@ 0x00002800) MRAM Program Control                                       */

        struct
        {
            __IOM uint8_t MHSPEN : 1;  /*!< [0..0] MRAM high speed program mode enable.                               */
            uint8_t              : 7;
        } MRPC_b;
    };
    __IM uint8_t  RESERVED50;
    __IM uint16_t RESERVED51;

    union
    {
        __IOM uint16_t MREEECC;         /*!< (@ 0x00002804) MRE ECC Encoder Control                                    */

        struct
        {
            __IOM uint16_t ECCBYPE : 1; /*!< [0..0] MRE ECC encoder outputs bypass enable.                             */
            uint16_t               : 7;
            __IOM uint16_t KEY     : 8; /*!< [15..8] KEY code                                                          */
        } MREEECC_b;
    };
    __IM uint16_t RESERVED52;
    __IM uint32_t RESERVED53[510];

    union
    {
        __IOM uint16_t MRCPC0;          /*!< (@ 0x00003000) MRC Program Control Register                               */

        struct
        {
            __IOM uint16_t MRCPNEN : 1; /*!< [0..0] MRC write enable for Non-Secure                                    */
            uint16_t               : 7;
            __IOM uint16_t KEY     : 8; /*!< [15..8] Key Code                                                          */
        } MRCPC0_b;
    };
    __IM uint16_t RESERVED54;

    union
    {
        __IOM uint16_t MRCPC1;          /*!< (@ 0x00003004) MRC Program Control Register for Secure                    */

        struct
        {
            __IOM uint16_t MRCPSEN : 1; /*!< [0..0] MRC write enable for Secure                                        */
            uint16_t               : 7;
            __IOM uint16_t KEY     : 8; /*!< [15..8] Key Code                                                          */
        } MRCPC1_b;
    };
    __IM uint16_t RESERVED55;

    union
    {
        __IOM uint16_t MBPROT0;        /*!< (@ 0x00003008) MRAM Block Protection Register                             */

        struct
        {
            __IOM uint16_t BPCN0 : 1;  /*!< [0..0] Block Protection cancel for non-secure                             */
            uint16_t             : 7;
            __IOM uint16_t KEY   : 8;  /*!< [15..8] Key Code                                                          */
        } MBPROT0_b;
    };
    __IM uint16_t RESERVED56;

    union
    {
        __IOM uint16_t MBPROT1;        /*!< (@ 0x0000300C) MRAM Block Protection Register for Secure                  */

        struct
        {
            __IOM uint16_t BPCN1 : 1;  /*!< [0..0] Block Protection cancel for secure                                 */
            uint16_t             : 7;
            __IOM uint16_t KEY   : 8;  /*!< [15..8] Key Code                                                          */
        } MBPROT1_b;
    };
    __IM uint16_t RESERVED57;

    union
    {
        __IOM uint8_t MRCPS;             /*!< (@ 0x00003010) MRC Program Status Register                                */

        struct
        {
            __IOM uint8_t PRGERRC   : 1; /*!< [0..0] Programming Error                                                  */
            __IOM uint8_t ECCERRC   : 1; /*!< [1..1] ECC Error                                                          */
            __IM uint8_t  ABUFEMPB  : 1; /*!< [2..2] Address Buffer Empty                                               */
            __IM uint8_t  ABUFFULLB : 1; /*!< [3..3] Address Buffer Full                                                */
            __IM uint8_t  PRGBSYCB  : 1; /*!< [4..4] MRCB Program Busy                                                  */
            __IM uint8_t  ABUFEMP   : 1; /*!< [5..5] Address Buffer Empty                                               */
            __IM uint8_t  ABUFFULL  : 1; /*!< [6..6] Address Buffer Full                                                */
            __IM uint8_t  PRGBSYC   : 1; /*!< [7..7] MRC program busy                                                   */
        } MRCPS_b;
    };
    __IM uint8_t  RESERVED58;
    __IM uint16_t RESERVED59;

    union
    {
        __IOM uint8_t MRCPAEINT;       /*!< (@ 0x00003014) MRC Write Access Error Interrupt Enable Register           */

        struct
        {
            uint8_t               : 7;
            __IOM uint8_t MRCAEIE : 1; /*!< [7..7] MRC program Access Error Interrupt Enable                          */
        } MRCPAEINT_b;
    };
    __IM uint8_t  RESERVED60;
    __IM uint16_t RESERVED61;

    union
    {
        __IOM uint32_t MRCPEA;         /*!< (@ 0x00003018) MRC Program Error Address Register                         */

        struct
        {
            uint32_t            : 5;
            __IM uint32_t MCPEA : 27;  /*!< [31..5] MRC Program Error Address.                                        */
        } MRCPEA_b;
    };
    __IM uint32_t RESERVED62[5];

    union
    {
        __IOM uint16_t MRCFLR;         /*!< (@ 0x00003030) MRAM Block Protection Register for Secure                  */

        struct
        {
            __IOM uint16_t MRCFL : 1;  /*!< [0..0] MRC Flush execute.                                                 */
            uint16_t             : 7;
            __IOM uint16_t KEY   : 8;  /*!< [15..8] Key Code                                                          */
        } MRCFLR_b;
    };
    __IM uint16_t RESERVED63;
    __IM uint32_t RESERVED64[3];

    union
    {
        __IOM uint8_t MRCPRICR;          /*!< (@ 0x00003040) MRC Program Ready Interrupt Control Register               */

        struct
        {
            __IOM uint8_t MRCINTSEL : 1; /*!< [0..0] MRC Program Ready Interrupt Select                                 */
            uint8_t                 : 6;
            __IOM uint8_t MRCPRRIE  : 1; /*!< [7..7] MRC Program Ready Interrupt Enable                                 */
        } MRCPRICR_b;
    };
    __IM uint8_t  RESERVED65;
    __IM uint16_t RESERVED66;
    __IM uint32_t RESERVED67[243];

    union
    {
        __IOM uint8_t MRDPS;            /*!< (@ 0x00003410) MRAM Program Control                                       */

        struct
        {
            __IOM uint8_t PRGERRD  : 1; /*!< [0..0] MRD Programming Error                                              */
            __IOM uint8_t ECCERRD  : 1; /*!< [1..1] MRD ECC Error                                                      */
            uint8_t                : 3;
            __IM uint8_t ABUFEMPD  : 1; /*!< [5..5] Address Buffer Empty                                               */
            __IM uint8_t ABUFFULLD : 1; /*!< [6..6] Address Buffer Full                                                */
            __IM uint8_t PRGBSYD   : 1; /*!< [7..7] MRD Program Busy                                                   */
        } MRDPS_b;
    };
    __IM uint8_t  RESERVED68;
    __IM uint16_t RESERVED69;

    union
    {
        __IOM uint8_t MRDPAEINT;       /*!< (@ 0x00003414) MRD Program Access Error Interrupt Enable Register         */

        struct
        {
            uint8_t               : 7;
            __IOM uint8_t MRDAEIE : 1; /*!< [7..7] MRD Program Access Error Interrupt Enable                          */
        } MRDPAEINT_b;
    };
    __IM uint8_t  RESERVED70;
    __IM uint16_t RESERVED71;

    union
    {
        __IOM uint32_t MRDPEA;         /*!< (@ 0x00003418) MRD Program Error Address Register                         */

        struct
        {
            uint32_t            : 4;
            __IM uint32_t MDPEA : 28;  /*!< [31..4] MRD Program Error Address.                                        */
        } MRDPEA_b;
    };
    __IM uint32_t RESERVED72[5];

    union
    {
        __IOM uint16_t MRDFLR;         /*!< (@ 0x00003430) MRD Flush Register                                         */

        struct
        {
            __IOM uint16_t MRDFL : 1;  /*!< [0..0] Flush FIFO and MDATAR for MRD                                      */
            uint16_t             : 7;
            __IOM uint16_t KEY   : 8;  /*!< [15..8] Key Code                                                          */
        } MRDFLR_b;
    };
    __IM uint16_t RESERVED73;
    __IM uint32_t RESERVED74[3];

    union
    {
        __IOM uint8_t MRDPRICR;        /*!< (@ 0x00003440) MRD Program Ready Interrupt Control Register               */

        struct
        {
            uint8_t                : 7;
            __IOM uint8_t MRDPRRIE : 1; /*!< [7..7] MRD Program Ready Interrupt Enable                                 */
        } MRDPRICR_b;
    };
    __IM uint8_t  RESERVED75;
    __IM uint16_t RESERVED76;
    __IM uint32_t RESERVED77[240];

    union
    {
        __IOM uint16_t MRCEECC;         /*!< (@ 0x00003804) MRC ECC Encoder Control                                    */

        struct
        {
            __IOM uint16_t ECCBYPC : 1; /*!< [0..0] MRC ECC encoder outputs bypass enable                              */
            uint16_t               : 7;
            __IOM uint16_t KEY     : 8; /*!< [15..8] Key Code                                                          */
        } MRCEECC_b;
    };
    __IM uint16_t RESERVED78;
} R_MRMS_Type;                          /*!< Size = 14344 (0x3808)                                                     */

#endif /* MRAM_REG_H */
