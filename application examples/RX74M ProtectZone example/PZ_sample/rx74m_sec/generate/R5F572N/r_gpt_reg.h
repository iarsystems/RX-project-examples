/* ${DISCLAIMER_PLACEHOLDER} */

#ifndef R_GPT_REG_H
#define R_GPT_REG_H

/* =========================================================================================================================== */
/* ================                                           GPTW                                            ================ */
/* =========================================================================================================================== */

/**
 * @brief General PWM Timer (GPTW)
 */

typedef struct                         /*!< (@ 0x000C2000) GPTW Structure                                             */
{
    union
    {
        __IOM uint32_t GTWP;           /*!< (@ 0x00000000) General PWM Timer Write-Protection Register                */

        struct
        {
            __IOM uint32_t WP    : 1;  /*!< [0..0] Register Write Disabled                                            */
            __IOM uint32_t STRWP : 1;  /*!< [1..1] GTSTR.CSTRT Bit Write Disabled                                     */
            __IOM uint32_t STPWP : 1;  /*!< [2..2] GTSTP.CSTOP Bit Write Disabled                                     */
            __IOM uint32_t CLRWP : 1;  /*!< [3..3] GTCLR.CCLR Bit Write Disabled                                      */
            __IOM uint32_t CMNWP : 1;  /*!< [4..4] Common Register Write Disabled                                     */
            uint32_t             : 3;
            __IOM uint32_t PRKEY : 8;  /*!< [15..8] GTWP Key Code                                                     */
            uint32_t             : 16;
        } GTWP_b;
    };

    union
    {
        __IOM uint32_t GTSTR;          /*!< (@ 0x00000004) General PWM Timer Software Start Register                  */

        struct
        {
            __IOM uint32_t CSTRT0 : 1; /*!< [0..0] Channel 0 Count Start                                              */
            __IOM uint32_t CSTRT1 : 1; /*!< [1..1] Channel 1 Count Start                                              */
            __IOM uint32_t CSTRT2 : 1; /*!< [2..2] Channel 2 Count Start                                              */
            __IOM uint32_t CSTRT3 : 1; /*!< [3..3] Channel 3 Count Start                                              */
            uint32_t              : 28;
        } GTSTR_b;
    };

    union
    {
        __IOM uint32_t GTSTP;          /*!< (@ 0x00000008) General PWM Timer Software Stop Register                   */

        struct
        {
            __IOM uint32_t CSTOP0 : 1; /*!< [0..0] Channel 0 Count Stop                                               */
            __IOM uint32_t CSTOP1 : 1; /*!< [1..1] Channel 1 Count Stop                                               */
            __IOM uint32_t CSTOP2 : 1; /*!< [2..2] Channel 2 Count Stop                                               */
            __IOM uint32_t CSTOP3 : 1; /*!< [3..3] Channel 3 Count Stop                                               */
            uint32_t              : 28;
        } GTSTP_b;
    };

    union
    {
        __IOM uint32_t GTCLR;          /*!< (@ 0x0000000C) General PWM Timer Software Clear Register                  */

        struct
        {
            __IOM uint32_t CCLR0 : 1;  /*!< [0..0] Channel 0 Counter Clear                                            */
            __IOM uint32_t CCLR1 : 1;  /*!< [1..1] Channel 1 Counter Clear                                            */
            __IOM uint32_t CCLR2 : 1;  /*!< [2..2] Channel 2 Counter Clear                                            */
            __IOM uint32_t CCLR3 : 1;  /*!< [3..3] Channel 3 Counter Clear                                            */
            uint32_t             : 28;
        } GTCLR_b;
    };

    union
    {
        __IOM uint32_t GTSSR;            /*!< (@ 0x00000010) General PWM Timer Start Source Select Register             */

        struct
        {
            __IOM uint32_t SSGTRGAR : 1; /*!< [0..0] SSGTRGAR                                                           */
            __IOM uint32_t SSGTRGAF : 1; /*!< [1..1] GTETRGA Signal Edge Select                                         */
            __IOM uint32_t SSGTRGBR : 1; /*!< [2..2] SSGTRGBR                                                           */
            __IOM uint32_t SSGTRGBF : 1; /*!< [3..3] GTETRGB Signal Edge Select                                         */
            __IOM uint32_t SSGTRGCR : 1; /*!< [4..4] SSGTRGCR                                                           */
            __IOM uint32_t SSGTRGCF : 1; /*!< [5..5] GTETRGC Signal Edge Select                                         */
            __IOM uint32_t SSGTRGDR : 1; /*!< [6..6] SSGTRGDR                                                           */
            __IOM uint32_t SSGTRGDF : 1; /*!< [7..7] GTETRGD Signal Edge Select                                         */
            __IOM uint32_t SSCARBL  : 1; /*!< [8..8] SSCARBL                                                            */
            __IOM uint32_t SSCARBH  : 1; /*!< [9..9] GTIOCnA Signal Rising EdgeApplying Condition Select                */
            __IOM uint32_t SSCAFBL  : 1; /*!< [10..10] SSCAFBL                                                          */
            __IOM uint32_t SSCAFBH  : 1; /*!< [11..11] GTIOCnA Signal Falling EdgeApplying Condition Select             */
            __IOM uint32_t SSCBRAL  : 1; /*!< [12..12] SSCBRAL                                                          */
            __IOM uint32_t SSCBRAH  : 1; /*!< [13..13] GTIOCnB Signal Rising EdgeApplying Condition Select              */
            __IOM uint32_t SSCBFAL  : 1; /*!< [14..14] SSCBFAL                                                          */
            __IOM uint32_t SSCBFAH  : 1; /*!< [15..15] GTIOCnB Signal Falling EdgeApplying Condition Select             */
            __IOM uint32_t SSELCA   : 1; /*!< [16..16] ELCA Event Source Count StartEnable                              */
            __IOM uint32_t SSELCB   : 1; /*!< [17..17] ELCB Event Source Count StartEnable                              */
            __IOM uint32_t SSELCC   : 1; /*!< [18..18] ELCC Event Source Count Start Enable                             */
            __IOM uint32_t SSELCD   : 1; /*!< [19..19] ELCD Event Source Count Start Enable                             */
            __IOM uint32_t SSELCE   : 1; /*!< [20..20] ELCE Event Source Count Start Enable                             */
            __IOM uint32_t SSELCF   : 1; /*!< [21..21] ELCF Event Source Count Start Enable                             */
            __IOM uint32_t SSELCG   : 1; /*!< [22..22] ELCG Event Source Count Start Enable                             */
            __IOM uint32_t SSELCH   : 1; /*!< [23..23] ELCG Event Source Count Start Enable                             */
            uint32_t                : 7;
            __IOM uint32_t CSTRT    : 1; /*!< [31..31] Software Source Count Start Enable                               */
        } GTSSR_b;
    };

    union
    {
        __IOM uint32_t GTPSR;            /*!< (@ 0x00000014) General PWM Timer Stop Source Select Register              */

        struct
        {
            __IOM uint32_t PSGTRGAR : 1; /*!< [0..0] PSGTRGAR                                                           */
            __IOM uint32_t PSGTRGAF : 1; /*!< [1..1] GTETRGA Signal Edge Select                                         */
            __IOM uint32_t PSGTRGBR : 1; /*!< [2..2] PSGTRGBR                                                           */
            __IOM uint32_t PSGTRGBF : 1; /*!< [3..3] GTETRGB Signal Edge Select                                         */
            __IOM uint32_t PSGTRGCR : 1; /*!< [4..4] PSGTRGCR                                                           */
            __IOM uint32_t PSGTRGCF : 1; /*!< [5..5] GTETRGC Signal Edge Select                                         */
            __IOM uint32_t PSGTRGDR : 1; /*!< [6..6] PSGTRGDR                                                           */
            __IOM uint32_t PSGTRGDF : 1; /*!< [7..7] GTETRGD Signal Edge Select                                         */
            __IOM uint32_t PSCARBL  : 1; /*!< [8..8] PSCARBL                                                            */
            __IOM uint32_t PSCARBH  : 1; /*!< [9..9] GTIOCnA Signal Rising EdgeApplying Condition Select                */
            __IOM uint32_t PSCAFBL  : 1; /*!< [10..10] PSCAFBL                                                          */
            __IOM uint32_t PSCAFBH  : 1; /*!< [11..11] GTIOCnA Signal Falling EdgeApplying Condition Select             */
            __IOM uint32_t PSCBRAL  : 1; /*!< [12..12] PSCBRAL                                                          */
            __IOM uint32_t PSCBRAH  : 1; /*!< [13..13] GTIOCnB Signal Rising EdgeApplying Condition Select              */
            __IOM uint32_t PSCBFAL  : 1; /*!< [14..14] PSCBFAL                                                          */
            __IOM uint32_t PSCBFAH  : 1; /*!< [15..15] GTIOCnB Signal Falling EdgeApplying Condition Select             */
            __IOM uint32_t PSELCA   : 1; /*!< [16..16] ELCA Event Source Count Stop Enable                              */
            __IOM uint32_t PSELCB   : 1; /*!< [17..17] ELCB Event Source Count Stop Enable                              */
            __IOM uint32_t PSELCC   : 1; /*!< [18..18] ELCC Event Source Count Stop Enable                              */
            __IOM uint32_t PSELCD   : 1; /*!< [19..19] ELCD Event Source Count Stop Enable                              */
            __IOM uint32_t PSELCE   : 1; /*!< [20..20] ELCE Event Source Count Stop Enable                              */
            __IOM uint32_t PSELCF   : 1; /*!< [21..21] ELCF Event Source Count Stop Enable                              */
            __IOM uint32_t PSELCG   : 1; /*!< [22..22] ELCG Event Source Count Stop Enable                              */
            __IOM uint32_t PSELCH   : 1; /*!< [23..23] ELCH Event Source Count Stop Enable                              */
            uint32_t                : 7;
            __IOM uint32_t CSTOP    : 1; /*!< [31..31] Software Source Count Stop Enable                                */
        } GTPSR_b;
    };

    union
    {
        __IOM uint32_t GTCSR;            /*!< (@ 0x00000018) General PWM Timer Clear Source Select Register             */

        struct
        {
            __IOM uint32_t CSGTRGAR : 1; /*!< [0..0] CSGTRGAR                                                           */
            __IOM uint32_t CSGTRGAF : 1; /*!< [1..1] GTETRGA Signal Edge Select                                         */
            __IOM uint32_t CSGTRGBR : 1; /*!< [2..2] CSGTRGBR                                                           */
            __IOM uint32_t CSGTRGBF : 1; /*!< [3..3] GTETRGB Signal Edge Select                                         */
            __IOM uint32_t CSGTRGCR : 1; /*!< [4..4] CSGTRGCR                                                           */
            __IOM uint32_t CSGTRGCF : 1; /*!< [5..5] GTETRGC Signal Edge Select                                         */
            __IOM uint32_t CSGTRGDR : 1; /*!< [6..6] CSGTRGDR                                                           */
            __IOM uint32_t CSGTRGDF : 1; /*!< [7..7] GTETRGD Signal Edge Select                                         */
            __IOM uint32_t CSCARBL  : 1; /*!< [8..8] CSCARBL                                                            */
            __IOM uint32_t CSCARBH  : 1; /*!< [9..9] GTIOCnA Signal Rising EdgeApplying Condition Select                */
            __IOM uint32_t CSCAFBL  : 1; /*!< [10..10] CSCAFBL                                                          */
            __IOM uint32_t CSCAFBH  : 1; /*!< [11..11] GTIOCnA Signal Falling EdgeApplying Condition Select             */
            __IOM uint32_t CSCBRAL  : 1; /*!< [12..12] CSCBRAL                                                          */
            __IOM uint32_t CSCBRAH  : 1; /*!< [13..13] GTIOCnB Signal Rising EdgeApplying Condition Select              */
            __IOM uint32_t CSCBFAL  : 1; /*!< [14..14] CSCBFAL                                                          */
            __IOM uint32_t CSCBFAH  : 1; /*!< [15..15] GTIOCnB Signal Falling EdgeApplying Condition Select             */
            __IOM uint32_t CSELCA   : 1; /*!< [16..16] ELCA Event Source Count Stop Enable                              */
            __IOM uint32_t CSELCB   : 1; /*!< [17..17] ELCB Event Source Count Stop Enable                              */
            __IOM uint32_t CSELCC   : 1; /*!< [18..18] ELCC Event Source Count Stop Enable                              */
            __IOM uint32_t CSELCD   : 1; /*!< [19..19] ELCD Event Source Count Stop Enable                              */
            __IOM uint32_t CSELCE   : 1; /*!< [20..20] ELCE Event Source Count Stop Enable                              */
            __IOM uint32_t CSELCF   : 1; /*!< [21..21] ELCF Event Source Count Stop Enable                              */
            __IOM uint32_t CSELCG   : 1; /*!< [22..22] ELCG Event Source Count Stop Enable                              */
            __IOM uint32_t CSELCH   : 1; /*!< [23..23] ELCH Event Source Count Stop Enable                              */
            uint32_t                : 7;
            __IOM uint32_t CCLR     : 1; /*!< [31..31] Software Source Counter Clear Enable                             */
        } GTCSR_b;
    };

    union
    {
        __IOM uint32_t GTUPSR;           /*!< (@ 0x0000001C) General PWM Timer Count-Up Source Select Register          */

        struct
        {
            __IOM uint32_t USGTRGAR : 1; /*!< [0..0] USGTRGAR                                                           */
            __IOM uint32_t USGTRGAF : 1; /*!< [1..1] GTETRGA Signal Edge Select                                         */
            __IOM uint32_t USGTRGBR : 1; /*!< [2..2] USGTRGBR                                                           */
            __IOM uint32_t USGTRGBF : 1; /*!< [3..3] GTETRGB Signal Edge Select                                         */
            __IOM uint32_t USGTRGCR : 1; /*!< [4..4] USGTRGCR                                                           */
            __IOM uint32_t USGTRGCF : 1; /*!< [5..5] GTETRGC Signal Edge Select                                         */
            __IOM uint32_t USGTRGDR : 1; /*!< [6..6] USGTRGDR                                                           */
            __IOM uint32_t USGTRGDF : 1; /*!< [7..7] GTETRGD Signal Edge Select                                         */
            __IOM uint32_t USCARBL  : 1; /*!< [8..8] USCARBL                                                            */
            __IOM uint32_t USCARBH  : 1; /*!< [9..9] GTIOCnA Signal Rising EdgeApplying Condition Select                */
            __IOM uint32_t USCAFBL  : 1; /*!< [10..10] USCAFBL                                                          */
            __IOM uint32_t USCAFBH  : 1; /*!< [11..11] GTIOCnA Signal Falling EdgeApplying Condition Select             */
            __IOM uint32_t USCBRAL  : 1; /*!< [12..12] USCBRAL                                                          */
            __IOM uint32_t USCBRAH  : 1; /*!< [13..13] GTIOCnB Signal Rising EdgeApplying Condition Select              */
            __IOM uint32_t USCBFAL  : 1; /*!< [14..14] USCBFAL                                                          */
            __IOM uint32_t USCBFAH  : 1; /*!< [15..15] GTIOCnB Signal Falling EdgeApplying Condition Select             */
            __IOM uint32_t USELCA   : 1; /*!< [16..16] ELCA Event Source Count-Up Enable                                */
            __IOM uint32_t USELCB   : 1; /*!< [17..17] ELCB Event Source Count-Up Enable                                */
            __IOM uint32_t USELCC   : 1; /*!< [18..18] ELCC Event Source Count-Up Enable                                */
            __IOM uint32_t USELCD   : 1; /*!< [19..19] ELCD Event Source Count-Up Enable                                */
            __IOM uint32_t USELCE   : 1; /*!< [20..20] ELCE Event Source Count-Up Enable                                */
            __IOM uint32_t USELCF   : 1; /*!< [21..21] ELCF Event Source Count-Up Enable                                */
            __IOM uint32_t USELCG   : 1; /*!< [22..22] ELCG Event Source Count-Up Enable                                */
            __IOM uint32_t USELCH   : 1; /*!< [23..23] ELCH Event Source Count-Up Enable                                */
            uint32_t                : 8;
        } GTUPSR_b;
    };

    union
    {
        __IOM uint32_t GTDNSR;           /*!< (@ 0x00000020) General PWM Timer Count-Down Source Select Register        */

        struct
        {
            __IOM uint32_t DSGTRGAR : 1; /*!< [0..0] DSGTRGAR                                                           */
            __IOM uint32_t DSGTRGAF : 1; /*!< [1..1] GTETRGA Signal Edge Select                                         */
            __IOM uint32_t DSGTRGBR : 1; /*!< [2..2] DSGTRGBR                                                           */
            __IOM uint32_t DSGTRGBF : 1; /*!< [3..3] GTETRGB Signal Edge Select                                         */
            __IOM uint32_t DSGTRGCR : 1; /*!< [4..4] DSGTRGCR                                                           */
            __IOM uint32_t DSGTRGCF : 1; /*!< [5..5] GTETRGC Signal Edge Select                                         */
            __IOM uint32_t DSGTRGDR : 1; /*!< [6..6] DSGTRGDR                                                           */
            __IOM uint32_t DSGTRGDF : 1; /*!< [7..7] GTETRGD Signal Edge Select                                         */
            __IOM uint32_t DSCARBL  : 1; /*!< [8..8] DSCARBL                                                            */
            __IOM uint32_t DSCARBH  : 1; /*!< [9..9] GTIOCnA Signal Rising EdgeApplying Condition Select                */
            __IOM uint32_t DSCAFBL  : 1; /*!< [10..10] DSCAFBL                                                          */
            __IOM uint32_t DSCAFBH  : 1; /*!< [11..11] GTIOCnA Signal Falling EdgeApplying Condition Select             */
            __IOM uint32_t DSCBRAL  : 1; /*!< [12..12] DSCBRAL                                                          */
            __IOM uint32_t DSCBRAH  : 1; /*!< [13..13] GTIOCnB Signal Rising EdgeApplying Condition Select              */
            __IOM uint32_t DSCBFAL  : 1; /*!< [14..14] DSCBFAL                                                          */
            __IOM uint32_t DSCBFAH  : 1; /*!< [15..15] GTIOCnB Signal Falling EdgeApplying Condition Select             */
            __IOM uint32_t DSELCA   : 1; /*!< [16..16] ELCA Event Source Count-Down Enable                              */
            __IOM uint32_t DSELCB   : 1; /*!< [17..17] ELCB Event Source Count-Down Enable                              */
            __IOM uint32_t DSELCC   : 1; /*!< [18..18] ELCC Event Source Count-Down Enable                              */
            __IOM uint32_t DSELCD   : 1; /*!< [19..19] ELCD Event Source Count-Down Enable                              */
            __IOM uint32_t DSELCE   : 1; /*!< [20..20] ELCE Event Source Count-Down Enable                              */
            __IOM uint32_t DSELCF   : 1; /*!< [21..21] ELCF Event Source Count-Down Enable                              */
            __IOM uint32_t DSELCG   : 1; /*!< [22..22] ELCG Event Source Count-Down Enable                              */
            __IOM uint32_t DSELCH   : 1; /*!< [23..23] ELCH Event Source Count-Down Enable                              */
            uint32_t                : 8;
        } GTDNSR_b;
    };

    union
    {
        __IOM uint32_t GTICASR;          /*!< (@ 0x00000024) General PWM Timer Input Capture Source Select
                                          *                  Register A                                                 */

        struct
        {
            __IOM uint32_t ASGTRGAR : 1; /*!< [0..0] ASGTRGAR                                                           */
            __IOM uint32_t ASGTRGAF : 1; /*!< [1..1] GTETRGA Signal Edge Select                                         */
            __IOM uint32_t ASGTRGBR : 1; /*!< [2..2] ASGTRGBR                                                           */
            __IOM uint32_t ASGTRGBF : 1; /*!< [3..3] GTETRGB Signal Edge Select                                         */
            __IOM uint32_t ASGTRGCR : 1; /*!< [4..4] ASGTRGCR                                                           */
            __IOM uint32_t ASGTRGCF : 1; /*!< [5..5] GTETRGC Signal Edge Select                                         */
            __IOM uint32_t ASGTRGDR : 1; /*!< [6..6] ASGTRGDR                                                           */
            __IOM uint32_t ASGTRGDF : 1; /*!< [7..7] GTETRGD Signal Edge Select                                         */
            __IOM uint32_t ASCARBL  : 1; /*!< [8..8] ASCARBL                                                            */
            __IOM uint32_t ASCARBH  : 1; /*!< [9..9] GTIOCnA Signal Rising EdgeApplying Condition Select                */
            __IOM uint32_t ASCAFBL  : 1; /*!< [10..10] ASCAFBL                                                          */
            __IOM uint32_t ASCAFBH  : 1; /*!< [11..11] GTIOCnA Signal Falling EdgeApplying Condition Select             */
            __IOM uint32_t ASCBRAL  : 1; /*!< [12..12] ASCBRAL                                                          */
            __IOM uint32_t ASCBRAH  : 1; /*!< [13..13] GTIOCnB Signal Rising EdgeApplying Condition Select              */
            __IOM uint32_t ASCBFAL  : 1; /*!< [14..14] ASCBFAL                                                          */
            __IOM uint32_t ASCBFAH  : 1; /*!< [15..15] GTIOCnB Signal Falling EdgeApplying Condition Select             */
            __IOM uint32_t ASELCA   : 1; /*!< [16..16] ELCA Event Source GTCCRA Input Capture Enable                    */
            __IOM uint32_t ASELCB   : 1; /*!< [17..17] ELCB Event Source GTCCRA Input Capture Enable                    */
            __IOM uint32_t ASELCC   : 1; /*!< [18..18] ELCC Event Source GTCCRA Input Capture Enable                    */
            __IOM uint32_t ASELCD   : 1; /*!< [19..19] ELCD Event Source GTCCRA Input Capture Enable                    */
            __IOM uint32_t ASELCE   : 1; /*!< [20..20] ELCE Event Source GTCCRA Input Capture Enable                    */
            __IOM uint32_t ASELCF   : 1; /*!< [21..21] ELCF Event Source GTCCRA Input Capture Enable                    */
            __IOM uint32_t ASELCG   : 1; /*!< [22..22] ELCG Event Source GTCCRA Input Capture Enable                    */
            __IOM uint32_t ASELCH   : 1; /*!< [23..23] ELCH Event Source GTCCRA Input Capture Enable                    */
            uint32_t                : 8;
        } GTICASR_b;
    };

    union
    {
        __IOM uint32_t GTICBSR;          /*!< (@ 0x00000028) General PWM Timer Input Capture Source Select
                                          *                  Register B                                                 */

        struct
        {
            __IOM uint32_t BSGTRGAR : 1; /*!< [0..0] BSGTRGAR                                                           */
            __IOM uint32_t BSGTRGAF : 1; /*!< [1..1] GTETRGA Signal Edge Select                                         */
            __IOM uint32_t BSGTRGBR : 1; /*!< [2..2] BSGTRGBR                                                           */
            __IOM uint32_t BSGTRGBF : 1; /*!< [3..3] GTETRGB Signal Edge Select                                         */
            __IOM uint32_t BSGTRGCR : 1; /*!< [4..4] BSGTRGCR                                                           */
            __IOM uint32_t BSGTRGCF : 1; /*!< [5..5] GTETRGC Signal Edge Select                                         */
            __IOM uint32_t BSGTRGDR : 1; /*!< [6..6] BSGTRGDR                                                           */
            __IOM uint32_t BSGTRGDF : 1; /*!< [7..7] GTETRGD Signal Edge Select                                         */
            __IOM uint32_t BSCARBL  : 1; /*!< [8..8] BSCARBL                                                            */
            __IOM uint32_t BSCARBH  : 1; /*!< [9..9] GTIOCnA Signal Rising EdgeApplying Condition Select                */
            __IOM uint32_t BSCAFBL  : 1; /*!< [10..10] BSCAFBL                                                          */
            __IOM uint32_t BSCAFBH  : 1; /*!< [11..11] GTIOCnA Signal Falling EdgeApplying Condition Select             */
            __IOM uint32_t BSCBRAL  : 1; /*!< [12..12] BSCBRAL                                                          */
            __IOM uint32_t BSCBRAH  : 1; /*!< [13..13] GTIOCnB Signal Rising EdgeApplying Condition Select              */
            __IOM uint32_t BSCBFAL  : 1; /*!< [14..14] BSCBFAL                                                          */
            __IOM uint32_t BSCBFAH  : 1; /*!< [15..15] GTIOCnB Signal Falling EdgeApplying Condition Select             */
            __IOM uint32_t BSELCA   : 1; /*!< [16..16] ELCA Event Source GTCCRA Input Capture Enable                    */
            __IOM uint32_t BSELCB   : 1; /*!< [17..17] ELCB Event Source GTCCRA Input Capture Enable                    */
            __IOM uint32_t BSELCC   : 1; /*!< [18..18] ELCC Event Source GTCCRA Input Capture Enable                    */
            __IOM uint32_t BSELCD   : 1; /*!< [19..19] ELCD Event Source GTCCRA Input Capture Enable                    */
            __IOM uint32_t BSELCE   : 1; /*!< [20..20] ELCE Event Source GTCCRA Input Capture Enable                    */
            __IOM uint32_t BSELCF   : 1; /*!< [21..21] ELCF Event Source GTCCRA Input Capture Enable                    */
            __IOM uint32_t BSELCG   : 1; /*!< [22..22] ELCG Event Source GTCCRA Input Capture Enable                    */
            __IOM uint32_t BSELCH   : 1; /*!< [23..23] ELCH Event Source GTCCRA Input Capture Enable                    */
            uint32_t                : 8;
        } GTICBSR_b;
    };

    union
    {
        __IOM uint32_t GTCR;           /*!< (@ 0x0000002C) General PWM Timer Control Register                         */

        struct
        {
            __IOM uint32_t CST  : 1;   /*!< [0..0] Count Start                                                        */
            uint32_t            : 7;
            __IOM uint32_t ICDS : 1;   /*!< [8..8] Input Capture Operation Select at Count Stop                       */
            uint32_t            : 7;
            __IOM uint32_t MD   : 3;   /*!< [18..16] Mode Select                                                      */
            uint32_t            : 4;
            __IOM uint32_t TPCS : 4;   /*!< [26..23] Timer Prescaler Select                                           */
            uint32_t            : 5;
        } GTCR_b;
    };

    union
    {
        __IOM uint32_t GTUDDTYC;       /*!< (@ 0x00000030) General PWM Timer Count Direction and Duty Setting
                                        *                  Register                                                   */

        struct
        {
            __IOM uint32_t UD     : 1; /*!< [0..0] Count Direction Setting                                            */
            __IOM uint32_t UDF    : 1; /*!< [1..1] Forcible Count DirectionSetting                                    */
            uint32_t              : 14;
            __IOM uint32_t OADTY  : 2; /*!< [17..16] GTIOCnA Pin Output Duty Setting                                  */
            __IOM uint32_t OADTYF : 1; /*!< [18..18] GTIOCnA Pin Output Duty Forced Setting                           */
            __IOM uint32_t OADTYR : 1; /*!< [19..19] Output after Release of GTIOCnA Pin Output 0%/ 100%
                                        *   Duty Cycle Settings                                                       */
            uint32_t              : 4;
            __IOM uint32_t OBDTY  : 2; /*!< [25..24] GTIOCnB Pin Output Duty Setting                                  */
            __IOM uint32_t OBDTYF : 1; /*!< [26..26] Timer Prescaler Select                                           */
            __IOM uint32_t OBDTYR : 1; /*!< [27..27] Output after Release of GTIOCnB Pin Output 0%/ 100%
                                        *   Duty Cycle Settings                                                       */
            uint32_t : 4;
        } GTUDDTYC_b;
    };

    union
    {
        __IOM uint32_t GTIOR;          /*!< (@ 0x00000034) General PWM Timer I/O Control Register                     */

        struct
        {
            __IOM uint32_t GTIOA  : 5; /*!< [4..0] GTIOCnA Pin Function Select                                        */
            uint32_t              : 1;
            __IOM uint32_t OADFLT : 1; /*!< [6..6] GTIOCnA Pin Output Value Setting at the Count Stop                 */
            __IOM uint32_t OAHLD  : 1; /*!< [7..7] GTIOCnA Pin Output Retention at the Start/Stop Count               */
            __IOM uint32_t OAE    : 1; /*!< [8..8] GTIOCnA Pin Output Enable                                          */
            __IOM uint32_t OADF   : 2; /*!< [10..9] GTIOCnA Pin Negate Value Setting                                  */
            uint32_t              : 2;
            __IOM uint32_t NFAEN  : 1; /*!< [13..13] GTIOCnA Pin Input Noise Filter Enable                            */
            __IOM uint32_t NFCSA  : 2; /*!< [15..14] GTIOCnA Pin Input Noise Filter Sampling Clock Select             */
            __IOM uint32_t GTIOB  : 5; /*!< [20..16] GTIOCnB Pin Function Select                                      */
            uint32_t              : 1;
            __IOM uint32_t OBDFLT : 1; /*!< [22..22] GTIOCnB Pin Output Value Setting at the Count Stop               */
            __IOM uint32_t OBHLD  : 1; /*!< [23..23] GTIOCnB Pin Output Retention at the Start/Stop Count             */
            __IOM uint32_t OBE    : 1; /*!< [24..24] GTIOCnB Pin Output Enable                                        */
            __IOM uint32_t OBDF   : 2; /*!< [26..25] GTIOCnB Pin Negate Value Setting                                 */
            uint32_t              : 2;
            __IOM uint32_t NFBEN  : 1; /*!< [29..29] GTIOCnB Pin Input Noise Filter Enable                            */
            __IOM uint32_t NFCSB  : 2; /*!< [31..30] GTIOCnB Pin Input Noise Filter Sampling Clock Select             */
        } GTIOR_b;
    };

    union
    {
        __IOM uint32_t GTINTAD;          /*!< (@ 0x00000038) General PWM Timer Interrupt Output Setting Register        */

        struct
        {
            __IOM uint32_t GTINTA : 1;   /*!< [0..0] GTCCRA Register Compare Match/Input Capture Interrupt
                                          *   Enable                                                                    */
            __IOM uint32_t GTINTB : 1;   /*!< [1..1] GTCCRB Register Compare Match/Input Capture Interrupt
                                          *   Enable                                                                    */
            __IOM uint32_t GTINTC : 1;   /*!< [2..2] GTCCRC Register Compare Match/Input Capture Interrupt
                                          *   Enable                                                                    */
            __IOM uint32_t GTINTD : 1;   /*!< [3..3] GTCCRD Register Compare Match/Input Capture Interrupt
                                          *   Enable                                                                    */
            __IOM uint32_t GTINTE : 1;   /*!< [4..4] GTCCRE Register Compare Match/Input Capture Interrupt
                                          *   Enable                                                                    */
            __IOM uint32_t GTINTF : 1;   /*!< [5..5] GTCCRF Register Compare Match/Input Capture Interrupt
                                          *   Enable                                                                    */
            __IOM uint32_t GTINTPR  : 2; /*!< [7..6] GTPR Register Compare Match Interrupt Enable                       */
            uint32_t                : 8;
            __IOM uint32_t ADTRAUEN : 1; /*!< [16..16] GTADTRA Register Compare Match(Up-Counting) A/D Converter
                                          *   Start Request Enable                                                      */
            __IOM uint32_t ADTRADEN : 1; /*!< [17..17] GTADTRA Register Compare Match(Down-Counting) A/D Converter
                                          *   Start Request Enable                                                      */
            __IOM uint32_t ADTRBUEN : 1; /*!< [18..18] GTADTRB Register Compare Match(Up-Counting) A/D Converter
                                          *   Start Request Enable                                                      */
            __IOM uint32_t ADTRBDEN : 1; /*!< [19..19] GTADTRB Register Compare Match(Down-Counting) A/D Converter
                                          *   Start Request Enable                                                      */
            uint32_t              : 4;
            __IOM uint32_t GRP    : 2;   /*!< [25..24] Output Stop Group Select                                         */
            uint32_t              : 2;
            __IOM uint32_t GRPDTE : 1;   /*!< [28..28] Dead Time Error Output Stop Detection Enable                     */
            __IOM uint32_t GRPABH : 1;   /*!< [29..29] Simultaneous High Output Stop Detection Enable                   */
            __IOM uint32_t GRPABL : 1;   /*!< [30..30] Simultaneous Low Output Stop Detection Enable                    */
            uint32_t              : 1;
        } GTINTAD_b;
    };

    union
    {
        __IOM uint32_t GTST;           /*!< (@ 0x0000003C) General PWM Timer Status Register                          */

        struct
        {
            uint32_t               : 8;
            __IM uint32_t ITCNT    : 3; /*!< [10..8] GTCIV/GTCIU Interrupt Skipping Count Counter                      */
            uint32_t               : 4;
            __IM uint32_t  TUCF    : 1; /*!< [15..15] Count Direction Flag                                             */
            __IOM uint32_t ADTRAUF : 1; /*!< [16..16] GTADTRA Register Compare Match (Up-Counting) A/D Converter
                                         *   Start Request Flag                                                        */
            __IOM uint32_t ADTRADF : 1; /*!< [17..17] GTADTRA Register Compare Match (Down-Counting) A/D
                                         *   Converter Start Request Flag                                              */
            __IOM uint32_t ADTRBUF : 1; /*!< [18..18] GTADTRB Register Compare Match (Up-Counting) A/D Converter
                                         *   Start Request Flag                                                        */
            __IOM uint32_t ADTRBDF : 1; /*!< [19..19] GTADTRB Register Compare Match (Down-Counting) A/D
                                         *   Converter Start Request Flag                                              */
            uint32_t            : 4;
            __IM uint32_t ODF   : 1;    /*!< [24..24] Output Stop Request Flag                                         */
            uint32_t            : 3;
            __IM uint32_t DTEF  : 1;    /*!< [28..28] Dead Time Error Flag                                             */
            __IM uint32_t OABHF : 1;    /*!< [29..29] Simultaneous High Output Flag                                    */
            __IM uint32_t OABLF : 1;    /*!< [30..30] Simultaneous Low Output Flag                                     */
            uint32_t            : 1;
        } GTST_b;
    };

    union
    {
        __IOM uint32_t GTBER;           /*!< (@ 0x00000040) General PWM Timer Buffer Enable Register                   */

        struct
        {
            __IOM uint32_t BD0     : 1; /*!< [0..0] GTCCRA/GTCCRB Registers Buffer Operation Disable                   */
            __IOM uint32_t BD1     : 1; /*!< [1..1] GTPR Register Buffer Operation Disable                             */
            __IOM uint32_t BD2     : 1; /*!< [2..2] GTADTRA/GTADTRB Registers Buffer Operation Disable                 */
            __IOM uint32_t BD3     : 1; /*!< [3..3] GTDVU/GTDVD Registers Buffer Operation Disable                     */
            uint32_t               : 4;
            __IOM uint32_t DBRTECA : 1; /*!< [8..8] GTCCRA Register Double Buffer Repeat Operation Enable              */
            uint32_t               : 1;
            __IOM uint32_t DBRTECB : 1; /*!< [10..10] GTCCRB Register Double Buffer Repeat Operation Enable            */
            uint32_t               : 5;
            __IOM uint32_t CCRA    : 2; /*!< [17..16] GTCCRA Register Buffer Operation                                 */
            __IOM uint32_t CCRB    : 2; /*!< [19..18] GTCCRB Register Buffer Operation                                 */
            __IOM uint32_t PR      : 2; /*!< [21..20] GTPR Register Buffer Operation                                   */
            __IOM uint32_t CCRSWT  : 1; /*!< [22..22] GTCCRA and GTCCRB Registers Forcible Buffer Operation            */
            uint32_t               : 1;
            __IOM uint32_t ADTTA   : 2; /*!< [25..24] GTADTRA Register Buffer Transfer Timing Select                   */
            __IOM uint32_t ADTDA   : 1; /*!< [26..26] GTADTRA Register Double Buffer Operation                         */
            uint32_t               : 1;
            __IOM uint32_t ADTTB   : 2; /*!< [29..28] GTADTRB Register Buffer Transfer Timing Select                   */
            __IOM uint32_t ADTDB   : 1; /*!< [30..30] GTADTRB Register Double Buffer Operation                         */
            uint32_t               : 1;
        } GTBER_b;
    };

    union
    {
        __IOM uint32_t GTITC;          /*!< (@ 0x00000044) General PWM Timer Interrupt and A/D Converter
                                        *                  Start Request Skipping Setting Register                    */

        struct
        {
            __IOM uint32_t ITLA : 1;   /*!< [0..0] GTCCRA Register Compare Match/Input Capture Interrupt
                                        *   Link                                                                      */
            __IOM uint32_t ITLB : 1;   /*!< [1..1] GTCCRB Register Compare Match/Input Capture Interrupt
                                        *   Link                                                                      */
            __IOM uint32_t ITLC : 1;   /*!< [2..2] GTCCRC Register Compare Match/Input Capture Interrupt
                                        *   Link                                                                      */
            __IOM uint32_t ITLD : 1;   /*!< [3..3] GTCCRD Register Compare Match/Input Capture Interrupt
                                        *   Link                                                                      */
            __IOM uint32_t ITLE : 1;   /*!< [4..4] GTCCRE Register Compare Match/Input Capture Interrupt
                                        *   Link                                                                      */
            __IOM uint32_t ITLF : 1;   /*!< [5..5] GTCCRF Register Compare Match/Input Capture Interrupt
                                        *   Link                                                                      */
            __IOM uint32_t IVTC  : 2;  /*!< [7..6] GTCIV/GTCIU Interrupt Skipping Function Select                     */
            __IOM uint32_t IVTT  : 3;  /*!< [10..8] GTCIV/GTCIU Interrupt Skipping Count Select                       */
            uint32_t             : 1;
            __IOM uint32_t ADTAL : 1;  /*!< [12..12] GTADTRA Register A/D Converter Start Request Link                */
            uint32_t             : 1;
            __IOM uint32_t ADTBL : 1;  /*!< [14..14] GTADTRB Register A/D Converter Start Request Link                */
            uint32_t             : 17;
        } GTITC_b;
    };

    union
    {
        __IOM uint32_t GTCNT;          /*!< (@ 0x00000048) General PWM Timer Counter                                  */

        struct
        {
            __IOM uint32_t GTCNT : 32; /*!< [31..0] GTCNT                                                             */
        } GTCNT_b;
    };

    union
    {
        __IOM uint32_t GTCCR[6];       /*!< (@ 0x0000004C) General PWM Timer Compare Capture Register [0..5]          */

        struct
        {
            __IOM uint32_t GTCCR : 32; /*!< [31..0] GTCCR                                                             */
        } GTCCR_b[6];
    };

    union
    {
        __IOM uint32_t GTPR;           /*!< (@ 0x00000064) General PWM Timer Period Setting Register                  */

        struct
        {
            __IOM uint32_t GTPR : 32;  /*!< [31..0] GTPR                                                              */
        } GTPR_b;
    };

    union
    {
        __IOM uint32_t GTPBR;          /*!< (@ 0x00000068) General PWM Timer Period Setting Buffer Register           */

        struct
        {
            __IOM uint32_t GTPBR : 32; /*!< [31..0] GTPBR                                                             */
        } GTPBR_b;
    };

    union
    {
        __IOM uint32_t GTPDBR;          /*!< (@ 0x0000006C) General PWM Timer Period Setting Double-Buffer
                                         *                  Register                                                   */

        struct
        {
            __IOM uint32_t GTPDBR : 32; /*!< [31..0] GTPDBR                                                            */
        } GTPDBR_b;
    };

    union
    {
        __IOM uint32_t GTADTRA;          /*!< (@ 0x00000070) A/D Converter Start Request Timing Register                */

        struct
        {
            __IOM uint32_t GTADTRA : 32; /*!< [31..0] GTADTRA                                                           */
        } GTADTRA_b;
    };

    union
    {
        __IOM uint32_t GTADTBRA;          /*!< (@ 0x00000074) A/D Converter Start Request Timing Buffer Register         */

        struct
        {
            __IOM uint32_t GTADTBRA : 32; /*!< [31..0] GTADTBRA                                                          */
        } GTADTBRA_b;
    };

    union
    {
        __IOM uint32_t GTADTDBRA;          /*!< (@ 0x00000078) A/D Converter Start Request Timing Double-Buffer
                                            *                  Register                                                   */

        struct
        {
            __IOM uint32_t GTADTDBRA : 32; /*!< [31..0] GTADTDBRA                                                         */
        } GTADTDBRA_b;
    };

    union
    {
        __IOM uint32_t GTADTRB;          /*!< (@ 0x0000007C) A/D Converter Start Request Timing Register                */

        struct
        {
            __IOM uint32_t GTADTRB : 32; /*!< [31..0] GTADTRB                                                           */
        } GTADTRB_b;
    };

    union
    {
        __IOM uint32_t GTADTBRB;          /*!< (@ 0x00000080) A/D Converter Start Request Timing Buffer Register         */

        struct
        {
            __IOM uint32_t GTADTBRB : 32; /*!< [31..0] GTADTBRB                                                          */
        } GTADTBRB_b;
    };

    union
    {
        __IOM uint32_t GTADTDBRB;          /*!< (@ 0x00000084) A/D Converter Start Request Timing Double-Buffer
                                            *                  Register                                                   */

        struct
        {
            __IOM uint32_t GTADTDBRB : 32; /*!< [31..0] GTADTDBRB                                                         */
        } GTADTDBRB_b;
    };

    union
    {
        __IOM uint32_t GTDTCR;         /*!< (@ 0x00000088) General PWM Timer Dead Time Control Register               */

        struct
        {
            uint32_t             : 4;
            __IOM uint32_t TDBUE : 1;  /*!< [4..4] GTDVU Register Buffer Operation Enable                             */
            __IOM uint32_t TDBDE : 1;  /*!< [5..5] GTDVD Register Buffer Operation Enable                             */
            uint32_t             : 2;
            __IOM uint32_t TDFER : 1;  /*!< [8..8] GTDVD Register Setting                                             */
            uint32_t             : 23;
        } GTDTCR_b;
    };

    union
    {
        __IOM uint32_t GTDVU;          /*!< (@ 0x0000008C) General PWM Timer Dead Time Value Register                 */

        struct
        {
            __IOM uint32_t GTDVU : 32; /*!< [31..0] GTDVU                                                             */
        } GTDVU_b;
    };

    union
    {
        __IOM uint32_t GTDVD;          /*!< (@ 0x00000090) General PWM Timer Dead Time Value Register                 */

        struct
        {
            __IOM uint32_t GTDVD : 32; /*!< [31..0] GTDVD                                                             */
        } GTDVD_b;
    };

    union
    {
        __IOM uint32_t GTDBU;          /*!< (@ 0x00000094) General PWM Timer Dead Time Value Buffer Register          */

        struct
        {
            __IOM uint32_t GTDBU : 32; /*!< [31..0] GTDBU                                                             */
        } GTDBU_b;
    };

    union
    {
        __IOM uint32_t GTDBD;          /*!< (@ 0x00000098) General PWM Timer Dead Time Value Register                 */

        struct
        {
            __IOM uint32_t GTDBD : 32; /*!< [31..0] GTDBD                                                             */
        } GTDBD_b;
    };

    union
    {
        __IOM uint32_t GTSOS;          /*!< (@ 0x0000009C) General PWM Timer Output Protection Function
                                        *                  Status Register                                            */

        struct
        {
            __IM uint32_t SOS : 2;     /*!< [1..0] Output Protection FunctionStatus                                   */
            uint32_t          : 30;
        } GTSOS_b;
    };

    union
    {
        __IOM uint32_t GTSOTR;         /*!< (@ 0x000000A0) General PWM Timer Output Protection Function
                                        *                  Temporary Release Register                                 */

        struct
        {
            __IOM uint32_t SOTR : 1;   /*!< [0..0] Output Protection Function Temporary Release                       */
            uint32_t            : 31;
        } GTSOTR_b;
    };

    union
    {
        __IOM uint32_t GTADSMR;         /*!< (@ 0x000000A4) General PWM Timer A/D Converter Start Request
                                         *                  Signal Monitoring Register                                 */

        struct
        {
            __IOM uint32_t ADSMS0  : 2; /*!< [1..0] A/D Conversion Start Request Signal Monitor 0 Selection            */
            uint32_t               : 6;
            __IOM uint32_t ADSMEN0 : 1; /*!< [8..8] A/D Conversion Start Request Signal Monitor 0 Output
                                         *   Enabling                                                                  */
            uint32_t               : 7;
            __IOM uint32_t ADSMS1  : 2; /*!< [17..16] A/D Conversion Start Request Signal Monitor 0 Selection          */
            uint32_t               : 6;
            __IOM uint32_t ADSMEN1 : 1; /*!< [24..24] A/D Conversion Start Request Signal Monitor 1 Output
                                         *   Enabling                                                                  */
            uint32_t : 7;
        } GTADSMR_b;
    };

    union
    {
        __IOM uint32_t GTEITC;            /*!< (@ 0x000000A8) General PWM Timer Extended Interrupt Skipping
                                           *                  Counter Control Register                                   */

        struct
        {
            __IOM uint32_t EIVTC1  : 2;   /*!< [1..0] Extended Interrupt Skipping Counter 1 Count Source Select          */
            uint32_t               : 2;
            __IOM uint32_t EIVTT1  : 4;   /*!< [7..4] Extended Interrupt Skipping 1 Skipping Count Setting               */
            uint32_t               : 4;
            __IM uint32_t  EITCNT1 : 4;   /*!< [15..12] Extended Interrupt Skipping Counter 1                            */
            __IOM uint32_t EIVTC2  : 2;   /*!< [17..16] Extended Interrupt Skipping Counter 2 Count Source
                                           *   Select                                                                    */
            uint32_t                 : 2;
            __IOM uint32_t EIVTT2    : 4; /*!< [23..20] Extended Interrupt Skipping 2 Skipping Count Setting             */
            __IOM uint32_t EITCNT2IV : 4; /*!< [27..24] Extended Interrupt Skipping Counter 2 Initial Value              */
            __IM uint32_t  EITCNT2   : 4; /*!< [31..28] Extended Interrupt Skipping Counter 2                            */
        } GTEITC_b;
    };

    union
    {
        __IOM uint32_t GTEITLI1;       /*!< (@ 0x000000AC) General PWM Timer Extended Interrupt Skipping
                                        *                  Setting Register 1                                         */

        struct
        {
            uint32_t              : 24;
            __IOM uint32_t EIVTC1 : 3; /*!< [26..24] Overflow Interrupt Extended Skipping Function Select             */
            uint32_t              : 1;
            __IOM uint32_t EIVTT1 : 3; /*!< [30..28] Underflow Interrupt Extended Skipping Function Select            */
            uint32_t              : 1;
        } GTEITLI1_b;
    };

    union
    {
        __IOM uint32_t GTEITLI2;       /*!< (@ 0x000000B0) General PWM Timer Extended Interrupt Skipping
                                        *                  Setting Register 2                                         */

        struct
        {
            __IOM uint32_t EADTAL : 3; /*!< [2..0] GTADTRA A/D Converter Start Request Extended Skipping
                                        *   Function Select                                                           */
            uint32_t              : 1;
            __IOM uint32_t EADTBL : 3; /*!< [6..4] GTADTRB A/D Converter Start Request Extended Skipping
                                        *   Function Select                                                           */
            uint32_t : 25;
        } GTEITLI2_b;
    };

    union
    {
        __IOM uint32_t GTEITLB;         /*!< (@ 0x000000B4) General PWM Timer Extended Buffer Transfer Skipping
                                         *                  Setting Register                                           */

        struct
        {
            __IOM uint32_t EBTLCA : 3;  /*!< [2..0] GTCCRA Register Compare Match/Input Capture Interrupt
                                         *   Extended Skipping Function Select                                         */
            uint32_t              : 1;
            __IOM uint32_t EBTLCB : 3;  /*!< [6..4] GTCCRB Register Compare Match/Input Capture Interrupt
                                         *   Extended Skipping Function Select                                         */
            uint32_t              : 1;
            __IOM uint32_t EBTLPR : 3;  /*!< [10..8] GTCCRC Register Compare Match Interrupt Extended Skipping
                                         *   Function Select                                                           */
            uint32_t               : 5;
            __IOM uint32_t EBTLADA : 3; /*!< [18..16] GTCCRE Register Compare Match Interrupt Extended Skipping
                                         *   Function Select                                                           */
            uint32_t               : 1;
            __IOM uint32_t EBTLADB : 3; /*!< [22..20] GTCCRF Register Compare Match Interrupt Extended Skipping
                                         *   Function Select                                                           */
            uint32_t               : 1;
            __IOM uint32_t EBTLDVU : 3; /*!< [26..24] Overflow Interrupt Extended Skipping Function Select             */
            uint32_t               : 1;
            __IOM uint32_t EBTLDVD : 3; /*!< [30..28] Underflow Interrupt Extended Skipping Function Select            */
            uint32_t               : 1;
        } GTEITLB_b;
    };
    __IM uint32_t RESERVED[6];

    union
    {
        __IOM uint32_t GTSECSR;         /*!< (@ 0x000000D0) General PWM Timer Operation Enable Bit Simultaneous
                                         *                  Control Channel Select Register                            */

        struct
        {
            __IOM uint32_t SECSEL0 : 1; /*!< [0..0] Channel 0 Operation Enable Bit Simultaneous Control Channel
                                         *   Select                                                                    */
            __IOM uint32_t SECSEL1 : 1; /*!< [1..1] Channel 1 Operation Enable Bit Simultaneous Control Channel
                                         *   Select                                                                    */
            __IOM uint32_t SECSEL2 : 1; /*!< [2..2] Channel 2 Operation Enable Bit Simultaneous Control Channel
                                         *   Select                                                                    */
            __IOM uint32_t SECSEL3 : 1; /*!< [3..3] Channel 3 Operation Enable Bit Simultaneous Control Channel
                                         *   Select                                                                    */
            uint32_t : 28;
        } GTSECSR_b;
    };

    union
    {
        __IOM uint32_t GTSECR;         /*!< (@ 0x000000D4) General PWM Timer Operation Enable Bit Simultaneous
                                        *                  Control Register                                           */

        struct
        {
            __IOM uint32_t SBDCE : 1;  /*!< [0..0] Channel 0 Operation Enable Bit Simultaneous Control Channel
                                        *   Select                                                                    */
            uint32_t             : 1;
            __IOM uint32_t SBDAE : 1;  /*!< [2..2] Channel 2 Operation Enable Bit Simultaneous Control Channel
                                        *   Select                                                                    */
            __IOM uint32_t SBDDE : 1;  /*!< [3..3] Channel 3 Operation Enable Bit Simultaneous Control Channel
                                        *   Select                                                                    */
            uint32_t             : 4;
            __IOM uint32_t SBDCD : 1;  /*!< [8..8] Channel 0 Operation Enable Bit Simultaneous Control Channel
                                        *   Select                                                                    */
            __IOM uint32_t SBDPD : 1;  /*!< [9..9] Channel 1 Operation Enable Bit Simultaneous Control Channel
                                        *   Select                                                                    */
            __IOM uint32_t SBDAD : 1;  /*!< [10..10] Channel 2 Operation Enable Bit Simultaneous Control
                                        *   Channel Select                                                            */
            __IOM uint32_t SBDDD : 1;  /*!< [11..11] Channel 3 Operation Enable Bit Simultaneous Control
                                        *   Channel Select                                                            */
            uint32_t : 20;
        } GTSECR_b;
    };
} R_GPT0_Type;                         /*!< Size = 216 (0xd8)                                                         */

#endif                                 /* R_GPT_REG_H */
