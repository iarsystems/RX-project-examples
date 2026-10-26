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

#ifndef R_SCI_REG_H
#define R_SCI_REG_H

/* =========================================================================================================================== */
/* ================                                           SCI0                                            ================ */
/* =========================================================================================================================== */

/**
 * @brief Serial Communications Interface (SCI0)
 */

typedef struct                         /*!< (@ 0x0008A000) R_SCI0 Structure                                           */
{
    union
    {
        union
        {
            __IOM uint8_t SMR;          /*!< (@ 0x00000000) Serial Mode Register (Non-Smart Card Interface
                                         *                  Mode (SCMR.SMIF = 0))                                      */

            struct
            {
                __IOM uint8_t CKS  : 2; /*!< [1..0] Clock Select                                                       */
                __IOM uint8_t MP   : 1; /*!< [2..2] Multi-Processor Mode                                               */
                __IOM uint8_t STOP : 1; /*!< [3..3] Stop Bit Length                                                    */
                __IOM uint8_t PM   : 1; /*!< [4..4] Parity Mode                                                        */
                __IOM uint8_t PE   : 1; /*!< [5..5] Parity Enable                                                      */
                __IOM uint8_t CHR  : 1; /*!< [6..6] Character Length                                                   */
                __IOM uint8_t CM   : 1; /*!< [7..7] Communications Mode                                                */
            } SMR_b;
        };

        union
        {
            __IOM uint8_t SMR_SMCI;    /*!< (@ 0x00000000) Serial Mode Register (Smart Card Interface Mode
                                        *                  (SCMR.SMIF = 1))                                           */

            struct
            {
                __IOM uint8_t CKS : 2; /*!< [1..0] Clock Select                                                       */
                __IOM uint8_t BCP : 2; /*!< [3..2] Base Clock Pulse                                                   */
                __IOM uint8_t PM  : 1; /*!< [4..4] Parity Mode                                                        */
                __IOM uint8_t PE  : 1; /*!< [5..5] Parity Enable                                                      */
                __IOM uint8_t BLK : 1; /*!< [6..6] Block Transfer Mode                                                */
                __IOM uint8_t GM  : 1; /*!< [7..7] GSM Mode                                                           */
            } SMR_SMCI_b;
        };
    };

    union
    {
        __IOM uint8_t BRR;             /*!< (@ 0x00000001) Bit Rate Register                                          */

        struct
        {
            __IOM uint8_t BRR : 8;     /*!< [7..0] The BRR register is an 8-bit register that adjusts the
                                        *   bit rate.                                                                 */
        } BRR_b;
    };

    union
    {
        union
        {
            __IOM uint8_t SCR;          /*!< (@ 0x00000002) Serial Control Register                                    */

            struct
            {
                __IOM uint8_t CKE  : 2; /*!< [1..0] Clock Enable                                                       */
                __IOM uint8_t TEIE : 1; /*!< [2..2] Transmit End Interrupt Enable                                      */
                __IOM uint8_t MPIE : 1; /*!< [3..3] Multi-Processor Interrupt Enable                                   */
                __IOM uint8_t RE   : 1; /*!< [4..4] Receive Enable                                                     */
                __IOM uint8_t TE   : 1; /*!< [5..5] Transmit Enable                                                    */
                __IOM uint8_t RIE  : 1; /*!< [6..6] Receive Interrupt Enable                                           */
                __IOM uint8_t TIE  : 1; /*!< [7..7] Transmit Interrupt Enable                                          */
            } SCR_b;
        };

        union
        {
            __IOM uint8_t SCR_SMCI;     /*!< (@ 0x00000002) Serial Control Register (SCMR.SMIF =1)                     */

            struct
            {
                __IOM uint8_t CKE  : 2; /*!< [1..0] Clock Enable                                                       */
                __IOM uint8_t TEIE : 1; /*!< [2..2] Transmit End Interrupt Enable                                      */
                __IOM uint8_t MPIE : 1; /*!< [3..3] Multi-Processor Interrupt Enable                                   */
                __IOM uint8_t RE   : 1; /*!< [4..4] Receive Enable                                                     */
                __IOM uint8_t TE   : 1; /*!< [5..5] Transmit Enable                                                    */
                __IOM uint8_t RIE  : 1; /*!< [6..6] Receive Interrupt Enable                                           */
                __IOM uint8_t TIE  : 1; /*!< [7..7] Transmit Interrupt Enable                                          */
            } SCR_SMCI_b;
        };
    };

    union
    {
        __IOM uint8_t TDR;             /*!< (@ 0x00000003) Transmit Data Register                                     */

        struct
        {
            __IOM uint8_t TDR : 8;     /*!< [7..0] TDR is an 8-bit register that stores transmit data.                */
        } TDR_b;
    };

    union
    {
        union
        {
            __IOM uint8_t SSR;          /*!< (@ 0x00000004) Serial Status Register                                     */

            struct
            {
                __IOM uint8_t MPBT : 1; /*!< [0..0] Multi-Processor Bit Transfer                                       */
                __IM uint8_t  MPB  : 1; /*!< [1..1] Multi-Processor                                                    */
                __IM uint8_t  TEND : 1; /*!< [2..2] Transmit End Flag                                                  */
                __IOM uint8_t PER  : 1; /*!< [3..3] Parity Error Flag                                                  */
                __IOM uint8_t FER  : 1; /*!< [4..4] Framing Error Flag                                                 */
                __IOM uint8_t ORER : 1; /*!< [5..5] Overrun Error Flag                                                 */
                __IOM uint8_t RDRF : 1; /*!< [6..6] Receive Data Full Flag                                             */
                __IOM uint8_t TDRE : 1; /*!< [7..7] Transmit Data Empty Flag                                           */
            } SSR_b;
        };

        union
        {
            __IOM uint8_t SSR_FIFO;     /*!< (@ 0x00000004) Serial Status Register (Non-Smart Card Interface
                                         *                  Mode and FIFO Mode (SCMR.SMIF = 0 and FCR.FM
                                         *                  = 1))                                                      */

            struct
            {
                __IOM uint8_t DR   : 1; /*!< [0..0] Receive Data Ready Flag                                            */
                uint8_t            : 1;
                __IOM uint8_t TEND : 1; /*!< [2..2] Transmit End Flag                                                  */
                __IOM uint8_t PER  : 1; /*!< [3..3] Parity Error Flag                                                  */
                __IOM uint8_t FER  : 1; /*!< [4..4] Framing Error Flag                                                 */
                __IOM uint8_t ORER : 1; /*!< [5..5] Overrun Error Flag                                                 */
                __IOM uint8_t RDF  : 1; /*!< [6..6] Receive FIFO Full Flag                                             */
                __IOM uint8_t TDFE : 1; /*!< [7..7] Transmit FIFO Empty Flag                                           */
            } SSR_FIFO_b;
        };

        union
        {
            __IOM uint8_t SSR_SMCI;     /*!< (@ 0x00000004) Serial Status Register ( Smart Card Interface
                                         *                  Mode (SCMR.SMIF = 1))                                      */

            struct
            {
                __IOM uint8_t MPBT : 1; /*!< [0..0] Multi-Processor Bit Transfer                                       */
                __IM uint8_t  MPB  : 1; /*!< [1..1] Multi-Processor                                                    */
                __IM uint8_t  TEND : 1; /*!< [2..2] Transmit End Flag                                                  */
                __IOM uint8_t PER  : 1; /*!< [3..3] Parity Error Flag                                                  */
                __IOM uint8_t ERS  : 1; /*!< [4..4] Error Signal Status Flag                                           */
                __IOM uint8_t ORER : 1; /*!< [5..5] Overrun Error Flag                                                 */
                __IOM uint8_t RDRF : 1; /*!< [6..6] Receive Data Full Flag                                             */
                __IOM uint8_t TDRE : 1; /*!< [7..7] Transmit Data Empty Flag                                           */
            } SSR_SMCI_b;
        };
    };

    union
    {
        __IM uint8_t RDR;              /*!< (@ 0x00000005) Receive Data Register                                      */

        struct
        {
            __IM uint8_t RDR : 8;      /*!< [7..0] RDR is an 8-bit register that stores receive data.                 */
        } RDR_b;
    };

    union
    {
        __IOM uint8_t SCMR;            /*!< (@ 0x00000006) Smart Card Mode Register                                   */

        struct
        {
            __IOM uint8_t SMIF : 1;    /*!< [0..0] Smart Card Interface Mode Select                                   */
            uint8_t            : 1;
            __IOM uint8_t SINV : 1;    /*!< [2..2] Transmitted/Received Data Invert                                   */
            __IOM uint8_t SDIR : 1;    /*!< [3..3] Transmitted/Received Data Transfer Direction                       */
            __IOM uint8_t CHR1 : 1;    /*!< [4..4] Character Length 1                                                 */
            uint8_t            : 2;
            __IOM uint8_t BCP2 : 1;    /*!< [7..7] Base Clock Pulse 2                                                 */
        } SCMR_b;
    };

    union
    {
        __IOM uint8_t SEMR;            /*!< (@ 0x00000007) Serial Extended Mode Register                              */

        struct
        {
            __IOM uint8_t ACS0    : 1; /*!< [0..0] Asynchronous Mode Clock Source Select                              */
            uint8_t               : 1;
            __IOM uint8_t BRME    : 1; /*!< [2..2] Bit Rate Modulation Enable                                         */
            __IOM uint8_t ABCSE   : 1; /*!< [3..3] Asynchronous Mode Base Clock Select Extended                       */
            __IOM uint8_t ABCS    : 1; /*!< [4..4] Asynchronous Mode Base Clock Select                                */
            __IOM uint8_t NFEN    : 1; /*!< [5..5] Digital Noise Filter Function Enable                               */
            __IOM uint8_t BGDM    : 1; /*!< [6..6] Baud Rate Generator Double-Speed Mode Select                       */
            __IOM uint8_t RXDESEL : 1; /*!< [7..7] Asynchronous Start Bit Edge Detection Select                       */
        } SEMR_b;
    };

    union
    {
        __IOM uint8_t SNFR;            /*!< (@ 0x00000008) Noise Filter Setting Register                              */

        struct
        {
            __IOM uint8_t NFCS : 3;    /*!< [2..0] Noise Filter Clock Select                                          */
            uint8_t            : 5;
        } SNFR_b;
    };

    union
    {
        __IOM uint8_t SIMR1;           /*!< (@ 0x00000009) I2C Mode Register 1                                        */

        struct
        {
            __IOM uint8_t IICM  : 1;   /*!< [0..0] Simple I2C Mode Select                                             */
            uint8_t             : 2;
            __IOM uint8_t IICDL : 5;   /*!< [7..3] SSDA Output Delay Select                                           */
        } SIMR1_b;
    };

    union
    {
        __IOM uint8_t SIMR2;           /*!< (@ 0x0000000A) I2C Mode Register 2                                        */

        struct
        {
            __IOM uint8_t IICINTM : 1; /*!< [0..0] I2C Interrupt Mode Select                                          */
            __IOM uint8_t IICCSC  : 1; /*!< [1..1] Clock Synchronization                                              */
            uint8_t               : 3;
            __IOM uint8_t IICACKT : 1; /*!< [5..5] ACK Transmission Data                                              */
            uint8_t               : 2;
        } SIMR2_b;
    };

    union
    {
        __IOM uint8_t SIMR3;              /*!< (@ 0x0000000B) I2C Mode Register 3                                        */

        struct
        {
            __IOM uint8_t IICSTAREQ  : 1; /*!< [0..0] Start Condition Generation                                         */
            __IOM uint8_t IICRSTAREQ : 1; /*!< [1..1] Restart Condition Generation                                       */
            __IOM uint8_t IICSTPREQ  : 1; /*!< [2..2] Stop Condition Generation                                          */
            __IOM uint8_t IICSTIF    : 1; /*!< [3..3] Issuing of Start, Restart, or Stop Condition Completed
                                           *   Flag                                                                      */
            __IOM uint8_t IICSDAS : 2;    /*!< [5..4] SSDA Output Select                                                 */
            __IOM uint8_t IICSCLS : 2;    /*!< [7..6] SSCL Output Select                                                 */
        } SIMR3_b;
    };

    union
    {
        __IOM uint8_t SISR;            /*!< (@ 0x0000000C) I2C Status Register                                        */

        struct
        {
            __IOM uint8_t IICACKR : 1; /*!< [0..0] ACK Reception Data Flag                                            */
            uint8_t               : 7;
        } SISR_b;
    };

    union
    {
        __IOM uint8_t SPMR;            /*!< (@ 0x0000000D) SPI Mode Register                                          */

        struct
        {
            __IOM uint8_t SSE   : 1;   /*!< [0..0] SSn# Pin Function Enable                                           */
            __IOM uint8_t CTSE  : 1;   /*!< [1..1] CTS Enable                                                         */
            __IOM uint8_t MSS   : 1;   /*!< [2..2] Master Slave Select                                                */
            uint8_t             : 1;
            __IOM uint8_t MFF   : 1;   /*!< [4..4] Mode Fault Flag                                                    */
            uint8_t             : 1;
            __IOM uint8_t CKPOL : 1;   /*!< [6..6] Clock Polarity Select                                              */
            __IOM uint8_t CKPH  : 1;   /*!< [7..7] Clock Phase Select                                                 */
        } SPMR_b;
    };

    union
    {
        union
        {
            __IOM uint16_t TDRHL;         /*!< (@ 0x0000000E) Transmit Data Register HL                                  */

            struct
            {
                __OM uint16_t TDRHL : 16; /*!< [15..0] TDRHL is a 16-bit register that stores transmit data.             */
            } TDRHL_b;
        };

        union
        {
            __OM uint16_t FTDRHL;       /*!< (@ 0x0000000E) Transmit FIFO Data Register                                */

            struct
            {
                __OM uint16_t TDAT : 9; /*!< [8..0] Transmit Data                                                      */
                __OM uint16_t MPBT : 1; /*!< [9..9] Transmit Multi-Processor                                           */
                uint16_t           : 6;
            } FTDRHL_b;
        };

        struct
        {
            union
            {
                __OM uint8_t FTDRH;         /*!< (@ 0x0000000E) Transmit FIFO Data Register H                              */

                struct
                {
                    __OM uint8_t TDATH : 1; /*!< [0..0] Transmit Data                                                      */
                    __OM uint8_t MPBT  : 1; /*!< [1..1] Transmit Multi-Processor                                           */
                    uint8_t            : 6;
                } FTDRH_b;
            };

            union
            {
                __OM uint8_t FTDRL;         /*!< (@ 0x0000000F) Transmit FIFO Data Register L                              */

                struct
                {
                    __OM uint8_t TDATL : 8; /*!< [7..0] Transmit Data                                                      */
                } FTDRL_b;
            };
        };
    };

    union
    {
        union
        {
            __IM uint16_t RDRHL;          /*!< (@ 0x00000010) Receive Data Register HL                                   */

            struct
            {
                __IM uint16_t RDRHL : 16; /*!< [15..0] RDRHL is an 16-bit register that stores receive data.             */
            } RDRHL_b;
        };

        union
        {
            __IM uint16_t FRDRHL;       /*!< (@ 0x00000010) Receive FIFO Data Register                                 */

            struct
            {
                __IM uint16_t RDAT : 9; /*!< [8..0] Receive Data                                                       */
                __IM uint16_t MPB  : 1; /*!< [9..9] Multi-Processor Bit Monitor Flag                                   */
                __IM uint16_t DR   : 1; /*!< [10..10] Receive Data Ready Flag                                          */
                __IM uint16_t PER  : 1; /*!< [11..11] Parity Error Flag                                                */
                __IM uint16_t FER  : 1; /*!< [12..12] Framing Error Flag                                               */
                __IM uint16_t ORER : 1; /*!< [13..13] Overrun Error Flag                                               */
                __IM uint16_t RDF  : 1; /*!< [14..14] Receive FIFO Full Flag                                           */
                uint16_t           : 1;
            } FRDRHL_b;
        };

        struct
        {
            union
            {
                __IM uint8_t FRDRH;         /*!< (@ 0x00000010) Receive FIFO Data Register H                               */

                struct
                {
                    __IM uint8_t RDATH : 1; /*!< [0..0] Receive Data                                                       */
                    __IM uint8_t MPB   : 1; /*!< [1..1] Multi-Processor Bit Monitor Flag                                   */
                    __IM uint8_t DR    : 1; /*!< [2..2] Receive Data Ready Flag                                            */
                    __IM uint8_t PER   : 1; /*!< [3..3] Parity Error Flag                                                  */
                    __IM uint8_t FER   : 1; /*!< [4..4] Framing Error Flag                                                 */
                    __IM uint8_t ORER  : 1; /*!< [5..5] Overrun Error Flag                                                 */
                    __IM uint8_t RDF   : 1; /*!< [6..6] Receive FIFO Full Flag                                             */
                    uint8_t            : 1;
                } FRDRH_b;
            };

            union
            {
                __IM uint8_t FRDRL;         /*!< (@ 0x00000011) Receive FIFO Data Register L                               */

                struct
                {
                    __IM uint8_t RDATL : 8; /*!< [7..0] Receive Data                                                       */
                } FRDRL_b;
            };
        };
    };

    union
    {
        __IOM uint8_t MDDR;            /*!< (@ 0x00000012) Modulation Duty Register                                   */

        struct
        {
            __IOM uint8_t MDDR : 8;    /*!< [7..0] The MDDR register corrects the bit rate adjusted by the
                                        *   BRR register                                                              */
        } MDDR_b;
    };

    union
    {
        __IOM uint8_t DCCR;            /*!< (@ 0x00000013) Data Comparison Control Register                           */

        struct
        {
            __IOM uint8_t DCMF  : 1;   /*!< [0..0] Data Match Flag                                                    */
            uint8_t             : 2;
            __IOM uint8_t DPER  : 1;   /*!< [3..3] Match Data Parity Error Flag                                       */
            __IOM uint8_t DFER  : 1;   /*!< [4..4] Match Data Framing Error Flag                                      */
            uint8_t             : 1;
            __IOM uint8_t IDSEL : 1;   /*!< [6..6] ID Frame Select                                                    */
            __IOM uint8_t DCME  : 1;   /*!< [7..7] Data Match Detection Enable                                        */
        } DCCR_b;
    };

    union
    {
        __IOM uint16_t FCR;            /*!< (@ 0x00000014) FIFO Control Register                                      */

        struct
        {
            __IOM uint16_t FM    : 1;  /*!< [0..0] FIFO Mode Select                                                   */
            __IOM uint16_t RFRST : 1;  /*!< [1..1] Receive FIFO Reset                                                 */
            __IOM uint16_t TFRST : 1;  /*!< [2..2] Transmit FIFO Reset                                                */
            __IOM uint16_t DRES  : 1;  /*!< [3..3] Receive Data Ready Interrupt Select                                */
            __IOM uint16_t TTRG  : 4;  /*!< [7..4] Transmit FIFO Threshold Setting                                    */
            __IOM uint16_t RTRG  : 4;  /*!< [11..8] Receive FIFO Threshold Setting                                    */
            __IOM uint16_t RSTRG : 4;  /*!< [15..12] RTS# Output Threshold Setting                                    */
        } FCR_b;
    };

    union
    {
        __IM uint16_t FDR;             /*!< (@ 0x00000016) FIFO Data Count Register                                   */

        struct
        {
            __IM uint16_t R : 5;       /*!< [4..0] Receive FIFO Data Count                                            */
            uint16_t        : 3;
            __IM uint16_t T : 5;       /*!< [12..8] Transmit FIFO Data Count                                          */
            uint16_t        : 3;
        } FDR_b;
    };

    union
    {
        __IM uint16_t LSR;             /*!< (@ 0x00000018) Line Status Register                                       */

        struct
        {
            __IM uint16_t ORER : 1;    /*!< [0..0] Overrun Error Flag                                                 */
            uint16_t           : 1;
            __IM uint16_t FNUM : 5;    /*!< [6..2] Framing Error Count                                                */
            uint16_t           : 1;
            __IM uint16_t PNUM : 5;    /*!< [12..8] Parity Error Count                                                */
            uint16_t           : 3;
        } LSR_b;
    };

    union
    {
        __IOM uint16_t CDR;            /*!< (@ 0x0000001A) Comparison Data Register                                   */

        struct
        {
            __IOM uint16_t CMPD : 9;   /*!< [8..0] Comparison Data                                                    */
            uint16_t            : 7;
        } CDR_b;
    };

    union
    {
        __IOM uint8_t SPTR;            /*!< (@ 0x0000001C) Serial Port Register                                       */

        struct
        {
            __IM uint8_t  RXDMON : 1;  /*!< [0..0] RXD Line Monitoring Flag                                           */
            __IOM uint8_t SPB2DT : 1;  /*!< [1..1] Serial Port Break Data                                             */
            __IOM uint8_t SPB2IO : 1;  /*!< [2..2] Serial Port Break I/O                                              */
            uint8_t              : 5;
        } SPTR_b;
    };
    __IM uint8_t  RESERVED;
    __IM uint16_t RESERVED1;
} R_SCI0_Type;                         /*!< Size = 32 (0x20)                                                          */

#endif /* R_SCI_REG_H */
