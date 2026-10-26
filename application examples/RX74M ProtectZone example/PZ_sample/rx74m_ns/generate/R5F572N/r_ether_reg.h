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

#ifndef R_ETHER_REG_H
#define R_ETHER_REG_H

/**
 * @brief Ethernet MAC Controller (R_ETHERC0)
 */

typedef struct                         /*!< (@ 0x40064100) R_ETHERC0 Structure                                        */
{
    union
    {
        __IOM uint32_t ECMR;           /*!< (@ 0x00000000) ETHERC Mode Register                                       */

        struct
        {
            __IOM uint32_t PRM   : 1;  /*!< [0..0] Promiscuous Mode                                                   */
            __IOM uint32_t DM    : 1;  /*!< [1..1] Duplex Mode                                                        */
            __IOM uint32_t RTM   : 1;  /*!< [2..2] Bit Rate                                                           */
            __IOM uint32_t ILB   : 1;  /*!< [3..3] Internal Loopback Mode                                             */
            uint32_t             : 1;
            __IOM uint32_t TE    : 1;  /*!< [5..5] Transmission Enable                                                */
            __IOM uint32_t RE    : 1;  /*!< [6..6] Reception Enable                                                   */
            uint32_t             : 2;
            __IOM uint32_t MPDE  : 1;  /*!< [9..9] Magic Packet Detection Enable                                      */
            uint32_t             : 2;
            __IOM uint32_t PRCEF : 1;  /*!< [12..12] CRC Error Frame Receive Mode                                     */
            uint32_t             : 3;
            __IOM uint32_t TXF   : 1;  /*!< [16..16] Transmit Flow Control Operating Mode                             */
            __IOM uint32_t RXF   : 1;  /*!< [17..17] Receive Flow Control Operating Mode                              */
            __IOM uint32_t PFR   : 1;  /*!< [18..18] PAUSE Frame Receive Mode                                         */
            __IOM uint32_t ZPF   : 1;  /*!< [19..19] 0 Time PAUSE Frame Enable                                        */
            __IOM uint32_t TPC   : 1;  /*!< [20..20] PAUSE Frame Transmit                                             */
            uint32_t             : 11;
        } ECMR_b;
    };
    __IM uint32_t RESERVED;

    union
    {
        __IOM uint32_t RFLR;           /*!< (@ 0x00000008) Receive Frame Maximum Length Register                      */

        struct
        {
            __IOM uint32_t RFL : 12;   /*!< [11..0] Receive Frame Maximum LengthThe set value becomes the
                                        *   maximum frame length. The minimum value that can be set
                                        *   is 1,518 bytes, and the maximum value that can be set is
                                        *   2,048 bytes. Values that are less than 1,518 bytes are
                                        *   regarded as 1,518 bytes, and values larger than 2,048 bytes
                                        *   are regarded as 2,048 bytes.                                              */
            uint32_t : 20;
        } RFLR_b;
    };
    __IM uint32_t RESERVED1;

    union
    {
        __IOM uint32_t ECSR;           /*!< (@ 0x00000010) ETHERC Status Register                                     */

        struct
        {
            __IOM uint32_t ICD   : 1;  /*!< [0..0] False Carrier Detect Flag                                          */
            __IOM uint32_t MPD   : 1;  /*!< [1..1] Magic Packet Detect Flag                                           */
            __IOM uint32_t LCHNG : 1;  /*!< [2..2] LCHNG Link Signal Change Flag                                      */
            uint32_t             : 1;
            __IOM uint32_t PSRTO : 1;  /*!< [4..4] PAUSE Frame Retransmit Over Flag                                   */
            __IOM uint32_t BFR   : 1;  /*!< [5..5] Continuous Broadcast Frame Reception Flag                          */
            uint32_t             : 26;
        } ECSR_b;
    };
    __IM uint32_t RESERVED2;

    union
    {
        __IOM uint32_t ECSIPR;          /*!< (@ 0x00000018) ETHERC Interrupt Enable Register                           */

        struct
        {
            __IOM uint32_t ICDIP   : 1; /*!< [0..0] False Carrier Detect Interrupt Enable                              */
            __IOM uint32_t MPDIP   : 1; /*!< [1..1] Magic Packet Detect Interrupt Enable                               */
            __IOM uint32_t LCHNGIP : 1; /*!< [2..2] LINK Signal Change Interrupt Enable                                */
            uint32_t               : 1;
            __IOM uint32_t PSRTOIP : 1; /*!< [4..4] PAUSE Frame Retransmit Over Interrupt Enable                       */
            __IOM uint32_t BFSIPR  : 1; /*!< [5..5] Continuous Broadcast Frame Reception Interrupt Enable              */
            uint32_t               : 26;
        } ECSIPR_b;
    };
    __IM uint32_t RESERVED3;

    union
    {
        __IOM uint32_t PIR;            /*!< (@ 0x00000020) PHY Interface Register                                     */

        struct
        {
            __IOM uint32_t MDC : 1;    /*!< [0..0] MII/RMII Management Data ClockThe MDC bit value is output
                                        *   from the ETn_MDC pin to supply the management data clock
                                        *   to the MII or RMII.                                                       */
            __IOM uint32_t MMD : 1;    /*!< [1..1] MII/RMII Management Mode                                           */
            __IOM uint32_t MDO : 1;    /*!< [2..2] MII/RMII Management Data-OutThe MDO bit value is output
                                        *   from the ETn_MDIO pin when the MMD bit is 1 (write). The
                                        *   value is not output when the MMD bit is 0 (read).                         */
            __IM uint32_t MDI : 1;     /*!< [3..3] MII/RMII Management Data-InThis bit indicates the level
                                        *   of the ETn_MDIO pin. The write value should be 0.                         */
            uint32_t : 28;
        } PIR_b;
    };
    __IM uint32_t RESERVED4;

    union
    {
        __IM uint32_t PSR;             /*!< (@ 0x00000028) PHY Status Register                                        */

        struct
        {
            __IM uint32_t LMON : 1;    /*!< [0..0] ETn_LINKSTA Pin Status FlagThe link status can be read
                                        *   by connecting the link signal output from the PHY-LSI to
                                        *   the ETn_LINKSTA pin. For details on the polarity, refer
                                        *   to the specifications of the connected PHY-LSI.                           */
            uint32_t : 31;
        } PSR_b;
    };
    __IM uint32_t RESERVED5[5];

    union
    {
        __IOM uint32_t RDMLR;          /*!< (@ 0x00000040) Random Number Generation Counter Upper Limit
                                        *                  Setting Register                                           */

        struct
        {
            __IOM uint32_t RMD : 20;   /*!< [19..0] Random Number Generation Counter                                  */
            uint32_t           : 12;
        } RDMLR_b;
    };
    __IM uint32_t RESERVED6[3];

    union
    {
        __IOM uint32_t IPGR;           /*!< (@ 0x00000050) IPG Register                                               */

        struct
        {
            __IOM uint32_t IPG : 5;    /*!< [4..0] Interpacket Gap Range:'16bit time(0x00)'-'140bit time(0x1F)'       */
            uint32_t           : 27;
        } IPGR_b;
    };

    union
    {
        __IOM uint32_t APR;            /*!< (@ 0x00000054) Automatic PAUSE Frame Register                             */

        struct
        {
            __IOM uint32_t AP : 16;    /*!< [15..0] Automatic PAUSE Time SettingThese bits set the value
                                        *   of the pause_time parameter for a PAUSE frame that is automatically
                                        *   transmitted. Transmission is not performed until the set
                                        *   value multiplied by 512 bit time has elapsed.                             */
            uint32_t : 16;
        } APR_b;
    };

    union
    {
        __OM uint32_t MPR;             /*!< (@ 0x00000058) Manual PAUSE Frame Register                                */

        struct
        {
            __OM uint32_t MP : 16;     /*!< [15..0] Manual PAUSE Time SettingThese bits set the value of
                                        *   the pause_time parameter for a PAUSE frame that is manually
                                        *   transmitted. Transmission is not performed until the set
                                        *   value multiplied by 512 bit time has elapsed. The read
                                        *   value is undefined.                                                       */
            uint32_t : 16;
        } MPR_b;
    };
    __IM uint32_t RESERVED7;

    union
    {
        __IM uint32_t RFCF;            /*!< (@ 0x00000060) Received PAUSE Frame Counter                               */

        struct
        {
            __IM uint32_t RPAUSE : 8;  /*!< [7..0] Received PAUSE Frame CountNumber of received PAUSE frames          */
            uint32_t             : 24;
        } RFCF_b;
    };

    union
    {
        __IOM uint32_t TPAUSER;         /*!< (@ 0x00000064) PAUSE Frame Retransmit Count Setting Register              */

        struct
        {
            __IOM uint32_t TPAUSE : 16; /*!< [15..0] Automatic PAUSE Frame Retransmit Setting                          */
            uint32_t              : 16;
        } TPAUSER_b;
    };
    __IM uint32_t TPAUSECR;             /*!< (@ 0x00000068) PAUSE Frame Retransmit Counter                             */

    union
    {
        __IOM uint32_t BCFRR;           /*!< (@ 0x0000006C) Broadcast Frame Receive Count Setting Register             */

        struct
        {
            __IOM uint32_t BCF : 16;    /*!< [15..0] Broadcast Frame Continuous Receive Count Setting                  */
            uint32_t           : 16;
        } BCFRR_b;
    };
    __IM uint32_t RESERVED8[20];

    union
    {
        __IOM uint32_t MAHR;           /*!< (@ 0x000000C0) MAC Address Upper Bit Register                             */

        struct
        {
            __IOM uint32_t MAHR : 32;  /*!< [31..0] MAC Address Upper Bit RegisterThe MAHR register sets
                                        *   the upper 32 bits (b47 to b16) of the 48-bit MAC address.                 */
        } MAHR_b;
    };
    __IM uint32_t RESERVED9;

    union
    {
        __IOM uint32_t MALR;           /*!< (@ 0x000000C8) MAC Address Lower Bit Register                             */

        struct
        {
            __IOM uint32_t MALR : 16;  /*!< [15..0] MAC Address Lower Bit RegisterThe MALR register sets
                                        *   the lower 16 bits of the 48-bit MAC address.                              */
            uint32_t : 16;
        } MALR_b;
    };
    __IM uint32_t RESERVED10;

    union
    {
        __IOM uint32_t TROCR;          /*!< (@ 0x000000D0) Transmit Retry Over Counter Register                       */

        struct
        {
            __IOM uint32_t TROCR : 32; /*!< [31..0] Transmit Retry Over Counter RegisterThe TROCR register
                                        *   is a counter indicating the number of frames that fail
                                        *   to be retransmitted.                                                      */
        } TROCR_b;
    };
    __IOM uint32_t CDCR;               /*!< (@ 0x000000D4) Late Collision Detect Counter Register                     */

    union
    {
        __IOM uint32_t LCCR;           /*!< (@ 0x000000D8) Lost Carrier Counter Register                              */

        struct
        {
            __IOM uint32_t LCCR : 32;  /*!< [31..0] Lost Carrier Counter RegisterThe LCCR register is a
                                        *   counter indicating the number of times a loss of carrier
                                        *   is detected during frame transmission.                                    */
        } LCCR_b;
    };

    union
    {
        __IOM uint32_t CNDCR;          /*!< (@ 0x000000DC) Carrier Not Detect Counter Register                        */

        struct
        {
            __IOM uint32_t CNDCR : 32; /*!< [31..0] Carrier Not Detect Counter RegisterThe CNDCR register
                                        *   is a counter indicating the number of times a carrier is
                                        *   not detected during preamble transmission.                                */
        } CNDCR_b;
    };
    __IM uint32_t RESERVED11;

    union
    {
        __IOM uint32_t CEFCR;          /*!< (@ 0x000000E4) CRC Error Frame Receive Counter Register                   */

        struct
        {
            __IOM uint32_t CEFCR : 32; /*!< [31..0] CRC Error Frame Receive Counter RegisterThe CEFCR register
                                        *   is a counter indicating the number of received frames where
                                        *   a CRC error has been detected.                                            */
        } CEFCR_b;
    };

    union
    {
        __IOM uint32_t FRECR;          /*!< (@ 0x000000E8) Frame Receive Error Counter Register                       */

        struct
        {
            __IOM uint32_t FRECR : 32; /*!< [31..0] Frame Receive Error Counter RegisterThe FRECR register
                                        *   is a counter indicating the number of times a frame receive
                                        *   error has occurred.                                                       */
        } FRECR_b;
    };

    union
    {
        __IOM uint32_t TSFRCR;          /*!< (@ 0x000000EC) Too-Short Frame Receive Counter Register                   */

        struct
        {
            __IOM uint32_t TSFRCR : 32; /*!< [31..0] Too-Short Frame Receive Counter RegisterThe TSFRCR register
                                         *   is a counter indicating the number of times a short frame
                                         *   that is shorter than 64 bytes has been received.                          */
        } TSFRCR_b;
    };

    union
    {
        __IOM uint32_t TLFRCR;          /*!< (@ 0x000000F0) Too-Long Frame Receive Counter Register                    */

        struct
        {
            __IOM uint32_t TLFRCR : 32; /*!< [31..0] Too-Long Frame Receive Counter RegisterThe TLFRCR register
                                         *   is a counter indicating the number of times a long frame
                                         *   that is longer than the RFLR register value has been received.            */
        } TLFRCR_b;
    };

    union
    {
        __IOM uint32_t RFCR;           /*!< (@ 0x000000F4) Received Alignment Error Frame Counter Register            */

        struct
        {
            __IOM uint32_t RFCR : 32;  /*!< [31..0] Received Alignment Error Frame Counter RegisterThe RFCR
                                        *   register is a counter indicating the number of times a
                                        *   frame has been received with the alignment error (frame
                                        *   is not an integral number of octets).                                     */
        } RFCR_b;
    };

    union
    {
        __IOM uint32_t MAFCR;          /*!< (@ 0x000000F8) Multicast Address Frame Receive Counter Register           */

        struct
        {
            __IOM uint32_t MAFCR : 32; /*!< [31..0] Multicast Address Frame Receive Counter RegisterThe
                                        *   MAFCR register is a counter indicating the number of times
                                        *   a frame where the multicast address is set has been received.             */
        } MAFCR_b;
    };
} R_ETHERC0_Type;                      /*!< Size = 252 (0xfc)                                                         */

/* =========================================================================================================================== */
/* ================                                      R_ETHERC_EDMAC                                       ================ */
/* =========================================================================================================================== */

/**
 * @brief Ethernet DMA Controller (R_ETHERC_EDMAC)
 */

typedef struct                         /*!< (@ 0x40064000) R_ETHERC_EDMAC Structure                                   */
{
    union
    {
        __IOM uint32_t EDMR;           /*!< (@ 0x00000000) EDMAC Mode Register                                        */

        struct
        {
            __OM uint32_t SWR : 1;     /*!< [0..0] Software Reset                                                     */
            uint32_t          : 3;
            __IOM uint32_t DL : 2;     /*!< [5..4] Transmit/Receive DescriptorLength                                  */
            __IOM uint32_t DE : 1;     /*!< [6..6] Big Endian Mode/Little Endian ModeNOTE: This setting
                                        *   applies to data for the transmit/receive buffer. It does
                                        *   not apply to transmit/receive descriptors and registers.                  */
            uint32_t : 25;
        } EDMR_b;
    };
    __IM uint32_t RESERVED;

    union
    {
        __IOM uint32_t EDTRR;          /*!< (@ 0x00000008) EDMAC Transmit Request Register                            */

        struct
        {
            __OM uint32_t TR : 1;      /*!< [0..0] Transmit Request                                                   */
            uint32_t         : 31;
        } EDTRR_b;
    };
    __IM uint32_t RESERVED1;

    union
    {
        __IOM uint32_t EDRRR;          /*!< (@ 0x00000010) EDMAC Receive Request Register                             */

        struct
        {
            __IOM uint32_t RR : 1;     /*!< [0..0] Receive Request                                                    */
            uint32_t          : 31;
        } EDRRR_b;
    };
    __IM uint32_t RESERVED2;

    union
    {
        __IOM uint32_t TDLAR;          /*!< (@ 0x00000018) Transmit Descriptor List Start Address Register            */

        struct
        {
            __IOM uint32_t TDLAR : 32; /*!< [31..0] The start address of the transmit descriptor list is
                                        *   set. Set the start address according to the descriptor
                                        *   length selected by the EDMR.DL[1:0] bits.16-byte boundary:
                                        *   Lower 4 bits = 0000b32-byte boundary: Lower 5 bits = 00000b64-byte
                                        *   boundary: Lower 6 bits = 000000b                                          */
        } TDLAR_b;
    };
    __IM uint32_t RESERVED3;

    union
    {
        __IOM uint32_t RDLAR;          /*!< (@ 0x00000020) Receive Descriptor List Start Address Register             */

        struct
        {
            __IOM uint32_t RDLAR : 32; /*!< [31..0] The start address of the receive descriptor list is
                                        *   set. Set the start address according to the descriptor
                                        *   length selected by the EDMR.DL[1:0] bits.16-byte boundary:
                                        *   Lower 4 bits = 0000b32-byte boundary: Lower 5 bits = 00000b64-byte
                                        *   boundary: Lower 6 bits = 000000b                                          */
        } RDLAR_b;
    };
    __IM uint32_t RESERVED4;

    union
    {
        __IOM uint32_t EESR;           /*!< (@ 0x00000028) ETHERC/EDMAC Status Register                               */

        struct
        {
            __IOM uint32_t CERF : 1;   /*!< [0..0] CRC Error Flag                                                     */
            __IOM uint32_t PRE  : 1;   /*!< [1..1] PHY-LSI Receive Error Flag                                         */
            __IOM uint32_t RTSF : 1;   /*!< [2..2] Frame-Too-Short Error Flag                                         */
            __IOM uint32_t RTLF : 1;   /*!< [3..3] Frame-Too-Long Error Flag                                          */
            __IOM uint32_t RRF  : 1;   /*!< [4..4] Alignment Error Flag                                               */
            uint32_t            : 2;
            __IOM uint32_t RMAF : 1;   /*!< [7..7] Multicast Address Frame Receive Flag                               */
            __IOM uint32_t TRO  : 1;   /*!< [8..8] Transmit Retry Over Flag                                           */
            __IOM uint32_t CD   : 1;   /*!< [9..9] Late Collision Detect Flag                                         */
            __IOM uint32_t DLC  : 1;   /*!< [10..10] Loss of Carrier Detect Flag                                      */
            __IOM uint32_t CND  : 1;   /*!< [11..11] Carrier Not Detect Flag                                          */
            uint32_t            : 4;
            __IOM uint32_t RFOF : 1;   /*!< [16..16] Receive FIFO Overflow Flag                                       */
            __IOM uint32_t RDE  : 1;   /*!< [17..17] Receive Descriptor Empty Flag                                    */
            __IOM uint32_t FR   : 1;   /*!< [18..18] Frame Receive Flag                                               */
            __IOM uint32_t TFUF : 1;   /*!< [19..19] Transmit FIFO Underflow Flag                                     */
            __IOM uint32_t TDE  : 1;   /*!< [20..20] Transmit Descriptor Empty Flag                                   */
            __IOM uint32_t TC   : 1;   /*!< [21..21] Frame Transfer Complete Flag                                     */
            __IM uint32_t  ECI  : 1;   /*!< [22..22] ETHERC Status Register Source FlagNOTE: When the source
                                        *   in the ETHERCn.ECSR register is cleared, the ECI flag is
                                        *   also cleared.                                                             */
            __IOM uint32_t ADE   : 1;  /*!< [23..23] Address Error Flag                                               */
            __IOM uint32_t RFCOF : 1;  /*!< [24..24] Receive Frame Counter Overflow Flag                              */
            __IOM uint32_t RABT  : 1;  /*!< [25..25] Receive Abort Detect Flag                                        */
            __IOM uint32_t TABT  : 1;  /*!< [26..26] Transmit Abort Detect Flag                                       */
            uint32_t             : 3;
            __IOM uint32_t TWB   : 1;  /*!< [30..30] Write-Back Complete Flag                                         */
            uint32_t             : 1;
        } EESR_b;
    };
    __IM uint32_t RESERVED5;

    union
    {
        __IOM uint32_t EESIPR;          /*!< (@ 0x00000030) ETHERC/EDMAC Status Interrupt Enable Register              */

        struct
        {
            __IOM uint32_t CERFIP  : 1; /*!< [0..0] CRC Error Interrupt Request Enable                                 */
            __IOM uint32_t PREIP   : 1; /*!< [1..1] PHY-LSI Receive Error Interrupt Request Enable                     */
            __IOM uint32_t RTSFIP  : 1; /*!< [2..2] Frame-Too-Short Error Interrupt Request Enable                     */
            __IOM uint32_t RTLFIP  : 1; /*!< [3..3] Frame-Too-Long Error Interrupt Request Enable                      */
            __IOM uint32_t RRFIP   : 1; /*!< [4..4] Alignment Error Interrupt Request Enable                           */
            uint32_t               : 2;
            __IOM uint32_t RMAFIP  : 1; /*!< [7..7] Multicast Address Frame Receive Interrupt Request Enable           */
            __IOM uint32_t TROIP   : 1; /*!< [8..8] Transmit Retry Over Interrupt Request Enable                       */
            __IOM uint32_t CDIP    : 1; /*!< [9..9] Late Collision Detect Interrupt Request Enable                     */
            __IOM uint32_t DLCIP   : 1; /*!< [10..10] Loss of Carrier Detect Interrupt Request Enable                  */
            __IOM uint32_t CNDIP   : 1; /*!< [11..11] Carrier Not Detect Interrupt Request Enable                      */
            uint32_t               : 4;
            __IOM uint32_t RFOFIP  : 1; /*!< [16..16] Receive FIFO Overflow Interrupt Request Enable                   */
            __IOM uint32_t RDEIP   : 1; /*!< [17..17] Receive Descriptor Empty Interrupt Request Enable                */
            __IOM uint32_t FRIP    : 1; /*!< [18..18] Frame Receive Interrupt Request Enable                           */
            __IOM uint32_t TFUFIP  : 1; /*!< [19..19] Transmit FIFO Underflow Interrupt Request Enable                 */
            __IOM uint32_t TDEIP   : 1; /*!< [20..20] Transmit Descriptor Empty Interrupt Request Enable               */
            __IOM uint32_t TCIP    : 1; /*!< [21..21] Frame Transfer Complete Interrupt Request Enable                 */
            __IOM uint32_t ECIIP   : 1; /*!< [22..22] ETHERC Status Register Source Interrupt Request Enable           */
            __IOM uint32_t ADEIP   : 1; /*!< [23..23] Address Error Interrupt Request Enable                           */
            __IOM uint32_t RFCOFIP : 1; /*!< [24..24] Receive Frame Counter Overflow Interrupt Request Enable          */
            __IOM uint32_t RABTIP  : 1; /*!< [25..25] Receive Abort Detect Interrupt Request Enable                    */
            __IOM uint32_t TABTIP  : 1; /*!< [26..26] Transmit Abort Detect Interrupt Request Enable                   */
            uint32_t               : 3;
            __IOM uint32_t TWBIP   : 1; /*!< [30..30] Write-Back Complete Interrupt Request Enable                     */
            uint32_t               : 1;
        } EESIPR_b;
    };
    __IM uint32_t RESERVED6;

    union
    {
        __IOM uint32_t TRSCER;         /*!< (@ 0x00000038) ETHERC/EDMAC Transmit/Receive Status Copy Enable
                                        *                  Register                                                   */

        struct
        {
            uint32_t              : 4;
            __IOM uint32_t RRFCE  : 1; /*!< [4..4] RRF Flag Copy Enable                                               */
            uint32_t              : 2;
            __IOM uint32_t RMAFCE : 1; /*!< [7..7] RMAF Flag Copy Enable                                              */
            uint32_t              : 24;
        } TRSCER_b;
    };
    __IM uint32_t RESERVED7;

    union
    {
        __IOM uint32_t RMFCR;          /*!< (@ 0x00000040) Missed-Frame Counter Register                              */

        struct
        {
            __IOM uint32_t MFC : 16;   /*!< [15..0] Missed-Frame CounterThese bits indicate the number of
                                        *   frames that are discarded and not transferred to the receive
                                        *   buffer during reception.                                                  */
            uint32_t : 16;
        } RMFCR_b;
    };
    __IM uint32_t RESERVED8;

    union
    {
        __IOM uint32_t TFTR;           /*!< (@ 0x00000048) Transmit FIFO Threshold Register                           */

        struct
        {
            __IOM uint32_t TFT : 11;   /*!< [10..0] Transmit FIFO Threshold00Dh to 200h: The threshold is
                                        *   the set value multiplied by 4. Example: 00Dh: 52 bytes
                                        *   040h: 256 bytes 100h: 1024 bytes 200h: 2048 bytes                         */
            uint32_t : 21;
        } TFTR_b;
    };
    __IM uint32_t RESERVED9;

    union
    {
        __IOM uint32_t FDR;            /*!< (@ 0x00000050) Transmit FIFO Threshold Register                           */

        struct
        {
            __IOM uint32_t RFD : 5;    /*!< [4..0] Transmit FIFO Depth                                                */
            uint32_t           : 3;
            __IOM uint32_t TFD : 5;    /*!< [12..8] Receive FIFO Depth                                                */
            uint32_t           : 19;
        } FDR_b;
    };
    __IM uint32_t RESERVED10;

    union
    {
        __IOM uint32_t RMCR;           /*!< (@ 0x00000058) Receive Method Control Register                            */

        struct
        {
            __IOM uint32_t RNR : 1;    /*!< [0..0] Receive Request Reset                                              */
            uint32_t           : 31;
        } RMCR_b;
    };
    __IM uint32_t RESERVED11[2];

    union
    {
        __IOM uint32_t TFUCR;          /*!< (@ 0x00000064) Transmit FIFO Underflow Counter                            */

        struct
        {
            __IOM uint32_t UNDER : 16; /*!< [15..0] Transmit FIFO Underflow CountThese bits indicate how
                                        *   many times the transmit FIFO has underflowed. The counter
                                        *   stops when the counter value reaches FFFFh.                               */
            uint32_t : 16;
        } TFUCR_b;
    };

    union
    {
        __IOM uint32_t RFOCR;          /*!< (@ 0x00000068) Receive FIFO Overflow Counter                              */

        struct
        {
            __IOM uint32_t OVER : 16;  /*!< [15..0] Receive FIFO Overflow CountThese bits indicate how many
                                        *   times the receive FIFO has overflowed. The counter stops
                                        *   when the counter value reaches FFFFh.                                     */
            uint32_t : 16;
        } RFOCR_b;
    };

    union
    {
        __IOM uint32_t IOSR;           /*!< (@ 0x0000006C) Independent Output Signal Setting Register                 */

        struct
        {
            __IOM uint32_t ELB : 1;    /*!< [0..0] External Loopback Mode                                             */
            uint32_t           : 31;
        } IOSR_b;
    };

    union
    {
        __IOM uint32_t FCFTR;          /*!< (@ 0x00000070) Flow Control Start FIFO Threshold Setting Register         */

        struct
        {
            __IOM uint32_t RFDO : 3;   /*!< [2..0] Receive FIFO Data PAUSE Output Threshold(When (RFDO+1)x256-32
                                        *   bytes of data is stored in the receive FIFO.)                             */
            uint32_t            : 13;
            __IOM uint32_t RFFO : 3;   /*!< [18..16] Receive FIFO Frame PAUSE Output Threshold(When ((RFFO+1)x2)
                                        *   receive frames have been stored in the receive FIFO.)                     */
            uint32_t : 13;
        } FCFTR_b;
    };
    __IM uint32_t RESERVED12;

    union
    {
        __IOM uint32_t RPADIR;         /*!< (@ 0x00000078) Receive Data Padding Insert Register                       */

        struct
        {
            __IOM uint32_t PADR : 6;   /*!< [5..0] Padding Slot                                                       */
            uint32_t            : 10;
            __IOM uint32_t PADS : 2;   /*!< [17..16] Padding Size                                                     */
            uint32_t            : 14;
        } RPADIR_b;
    };

    union
    {
        __IOM uint32_t TRIMD;          /*!< (@ 0x0000007C) Transmit Interrupt Setting Register                        */

        struct
        {
            __IOM uint32_t TIS : 1;    /*!< [0..0] Transmit Interrupt EnableSet the EESR.TWB flag to 1 in
                                        *   the mode selected by the TIM bit to notify an interrupt.                  */
            uint32_t           : 3;
            __IOM uint32_t TIM : 1;    /*!< [4..4] Transmit Interrupt Mode                                            */
            uint32_t           : 27;
        } TRIMD_b;
    };
    __IM uint32_t RESERVED13[18];

    union
    {
        __IOM uint32_t RBWAR;          /*!< (@ 0x000000C8) Receive Buffer Write Address Register                      */

        struct
        {
            __IM uint32_t RBWAR : 32;  /*!< [31..0] Receive Buffer Write Address RegisterThe RBWAR register
                                        *   indicates the last address that the EDMAC has written data
                                        *   to when writing to the receive buffer.Refer to the address
                                        *   indicated by the RBWAR register to recognize which address
                                        *   in the receive buffer the EDMAC is writing data to. Note
                                        *   that the address that the EDMAC is outputting to the receive
                                        *   buffer may not match the read value of the RBWAR register
                                        *   during data reception.                                                    */
        } RBWAR_b;
    };

    union
    {
        __IOM uint32_t RDFAR;          /*!< (@ 0x000000CC) Receive Descriptor Fetch Address Register                  */

        struct
        {
            __IM uint32_t RDFAR : 32;  /*!< [31..0] Receive Descriptor Fetch Address RegisterThe RDFAR register
                                        *   indicates the start address of the last fetched receive
                                        *   descriptor when the EDMAC fetches descriptor information
                                        *   from the receive descriptor.Refer to the address indicated
                                        *   by the RDFAR register to recognize which receive descriptor
                                        *   information the EDMAC is using for the current processing.
                                        *   Note that the address of the receive descriptor that the
                                        *   EDMAC fetches may not match the read value of the RDFAR
                                        *   register during data reception.                                           */
        } RDFAR_b;
    };
    __IM uint32_t RESERVED14;

    union
    {
        __IOM uint32_t TBRAR;          /*!< (@ 0x000000D4) Transmit Buffer Read Address Register                      */

        struct
        {
            __IM uint32_t TBRAR : 32;  /*!< [31..0] Transmit Buffer Read Address RegisterThe TBRAR register
                                        *   indicates the last address that the EDMAC has read data
                                        *   from when reading data from the transmit buffer.Refer to
                                        *   the address indicated by the TBRAR register to recognize
                                        *   which address in the transmit buffer the EDMAC is reading
                                        *   from. Note that the address that the EDMAC is outputting
                                        *   to the transmit buffer may not match the read value of
                                        *   the TBRAR register.                                                       */
        } TBRAR_b;
    };

    union
    {
        __IM uint32_t TDFAR;           /*!< (@ 0x000000D8) Transmit Descriptor Fetch Address Register                 */

        struct
        {
            __IM uint32_t TDFAR : 32;  /*!< [31..0] Transmit Descriptor Fetch Address RegisterThe TDFAR
                                        *   register indicates the start address of the last fetched
                                        *   transmit descriptor when the EDMAC fetches descriptor information
                                        *   from the transmit descriptor.Refer to the address indicated
                                        *   by the TDFAR register to recognize which transmit descriptor
                                        *   information the EDMAC is using for the current processing.
                                        *   Note that the address of the transmit descriptor that the
                                        *   EDMAC fetches may not match the read value of the TDFAR
                                        *   register.                                                                 */
        } TDFAR_b;
    };
} R_ETHERC_EDMAC_Type;                 /*!< Size = 220 (0xdc)                                                         */

/* =========================================================================================================================== */
/* ================                                      R_ETHERC_EPTPC                                       ================ */
/* =========================================================================================================================== */

/**
 * @brief Ethernet PTP Controller (R_ETHERC_EPTPC)
 */

typedef struct                         /*!< (@ 0x40065800) R_ETHERC_EPTPC Structure                                   */
{
    union
    {
        __IOM uint32_t SYSR;           /*!< (@ 0x00000000) SYNFP Status Register                                      */

        struct
        {
            __IOM uint32_t OFMUD  : 1; /*!< [0..0] offsetFromMaster Value Update Flag                                 */
            __IOM uint32_t INTCHG : 1; /*!< [1..1] Receive logMessageInterval Value Change Detection Flag             */
            __IOM uint32_t MPDUD  : 1; /*!< [2..2] meanPathDelay Value Update Flag                                    */
            uint32_t              : 1;
            __IOM uint32_t DRPTO  : 1; /*!< [4..4] Delay_Resp/Pdelay_Resp Reception Timeout Detection Flag            */
            __IOM uint32_t INTDEV : 1; /*!< [5..5] Receive logMessageInterval Value Out-of-Range Flag                 */
            __IOM uint32_t DRQOVR : 1; /*!< [6..6] Delay_Req Reception FIFO Overflow Detection Flag                   */
            uint32_t              : 5;
            __IOM uint32_t RECLP  : 1; /*!< [12..12] Loop Reception Detection Flag                                    */
            uint32_t              : 1;
            __IOM uint32_t INFABT : 1; /*!< [14..14] Control Information Abnormality Detection Flag                   */
            uint32_t              : 1;
            __IOM uint32_t RESDN  : 1; /*!< [16..16] Response Stop Completion Detection Flag                          */
            __IOM uint32_t GENDN  : 1; /*!< [17..17] Generation Stop Completion Detection Flag                        */
            uint32_t              : 14;
        } SYSR_b;
    };

    union
    {
        __IOM uint32_t SYIPR;          /*!< (@ 0x00000004) SYNFP Status Notification Permission Register              */

        struct
        {
            __IOM uint32_t OFMUD  : 1; /*!< [0..0] SYSR.OFMUD Status Notification Permission                          */
            __IOM uint32_t INTCHG : 1; /*!< [1..1] SYSR.INTCHG Status Notification Permission                         */
            __IOM uint32_t MPDUD  : 1; /*!< [2..2] SYSR.MPDUD Status Notification Permission                          */
            uint32_t              : 1;
            __IOM uint32_t DRPTO  : 1; /*!< [4..4] SYSR.DRPTO Status Notification Permission                          */
            __IOM uint32_t INTDEV : 1; /*!< [5..5] SYSR.INTDEV Status Notification Permission                         */
            __IOM uint32_t DRQOVR : 1; /*!< [6..6] SYSR.DRQOVR Status Notification Permission                         */
            uint32_t              : 5;
            __IOM uint32_t RECLP  : 1; /*!< [12..12] SYSR.RECLP Status Notification Permission                        */
            uint32_t              : 1;
            __IOM uint32_t INFABT : 1; /*!< [14..14] SYSR.INFABT Status Notification Permission                       */
            uint32_t              : 1;
            __IOM uint32_t RESDN  : 1; /*!< [16..16] SYSR.RESDN Status Notification Permission                        */
            __IOM uint32_t GENDN  : 1; /*!< [17..17] SYSR.GENDN Status Notification Permission                        */
            uint32_t              : 14;
        } SYIPR_b;
    };
    __IM uint32_t RESERVED[2];

    union
    {
        __IOM uint32_t SYMACRU;          /*!< (@ 0x00000010) SYNFP MAC Address Registers                                */

        struct
        {
            __IOM uint32_t SYMACRU : 24; /*!< [23..0] These bits hold the setting for the higher-order 24
                                          *   bits of the local MAC address.                                            */
            uint32_t : 8;
        } SYMACRU_b;
    };

    union
    {
        __IOM uint32_t SYMACRL;          /*!< (@ 0x00000014) SYNFP MAC Address Registers                                */

        struct
        {
            __IOM uint32_t SYMACRL : 24; /*!< [23..0] These bits hold the setting for the lower-order 24 bits
                                          *   of the local MAC address.                                                 */
            uint32_t : 8;
        } SYMACRL_b;
    };

    union
    {
        __IOM uint32_t SYLLCCTLR;      /*!< (@ 0x00000018) SYNFP LLC-CTL Value Register                               */

        struct
        {
            __IOM uint32_t CTL : 8;    /*!< [7..0] LLC-CTL FieldThese bits specify the value used for the
                                        *   control field in the LLC sublayer when generating IEEE802.3
                                        *   frames.                                                                   */
            uint32_t : 24;
        } SYLLCCTLR_b;
    };

    union
    {
        __IOM uint32_t SYIPADDRR;          /*!< (@ 0x0000001C) SYNFP Local IP Address Register                            */

        struct
        {
            __IOM uint32_t SYIPADDRR : 32; /*!< [31..0] These bits hold the setting for the local IP address.             */
        } SYIPADDRR_b;
    };
    __IM uint32_t RESERVED1[8];

    union
    {
        __IOM uint32_t SYSPVRR;        /*!< (@ 0x00000040) SYNFP Specification Version Setting Register               */

        struct
        {
            __IOM uint32_t VER : 4;    /*!< [3..0] versionPTP Field ValueThese bits are used to set the
                                        *   versionPTP field value of the PTP v2 header.When a message
                                        *   is received, this value is compared with the versionPTP
                                        *   field of the received frame.In generating messages, the
                                        *   value is used for the versionPTP field of the frame for
                                        *   transmission.Set these bits to 0010b (PTP v2).                            */
            __IOM uint32_t TRSP : 4;   /*!< [7..4] transportSpecific Field ValueThese bits are used to set
                                        *   the transportSpecific field value of the PTP v2 header.When
                                        *   a message is received, this value is compared with the
                                        *   transportSpecific field of the received frame.In generating
                                        *   messages, the value is used for the transportSpecific field
                                        *   of the frame for transmission.Set these bits to 0000b (IEEE
                                        *   1588).                                                                    */
            uint32_t : 24;
        } SYSPVRR_b;
    };

    union
    {
        __IOM uint32_t SYDOMR;         /*!< (@ 0x00000044) SYNFP Domain Number Setting Register                       */

        struct
        {
            __IOM uint32_t DNUM : 8;   /*!< [7..0] domainNumber Field Value SettingThese bits are used to
                                        *   set the domainNumber field value of the PTP v2 header.When
                                        *   a message is received, this value is compared with the
                                        *   domainNumber field of the received frame as a condition
                                        *   for PTP reception processing.In generating messages, the
                                        *   value is used for the domainNumber field of the frame for
                                        *   transmission.                                                             */
            uint32_t : 24;
        } SYDOMR_b;
    };
    __IM uint32_t RESERVED2[2];

    union
    {
        __IOM uint32_t ANFR;           /*!< (@ 0x00000050) Announce Message Flag Field Setting Register               */

        struct
        {
            __IOM uint32_t FLAG0 : 1;  /*!< [0..0] leap61This bit is used to set the logical value of the
                                        *   leap61 member of timePropertiesDS.                                        */
            __IOM uint32_t FLAG1 : 1;  /*!< [1..1] leap59This bit is used to set the logical value of the
                                        *   leap59 member of timePropertiesDS.                                        */
            __IOM uint32_t FLAG2 : 1;  /*!< [2..2] currentUtcOffsetValidThis bit is used to set the logical
                                        *   value of the currentUtcOffsetValid member of timePropertiesDS.            */
            __IOM uint32_t FLAG3 : 1;  /*!< [3..3] ptpTimescaleThis bit is used to set the logical value
                                        *   of the ptpTimescale member of timePropertiesDS.                           */
            __IOM uint32_t FLAG4 : 1;  /*!< [4..4] timeTraceableThis bit is used to set the logical value
                                        *   of the timeTraceable member of timePropertiesDS.                          */
            __IOM uint32_t FLAG5 : 1;  /*!< [5..5] frequencyTraceableThis bit is used to set the logical
                                        *   value of the frequencyTraceable member of timePropertiesDS.               */
            uint32_t              : 2;
            __IOM uint32_t FLAG8  : 1; /*!< [8..8] alternateMasterFlag                                                */
            uint32_t              : 1;
            __IOM uint32_t FLAG10 : 1; /*!< [10..10] unicastFlag                                                      */
            uint32_t              : 2;
            __IOM uint32_t FLAG13 : 1; /*!< [13..13] PTP profile Specific 1                                           */
            __IOM uint32_t FLAG14 : 1; /*!< [14..14] PTP profile Specific 2                                           */
            uint32_t              : 17;
        } ANFR_b;
    };

    union
    {
        __IOM uint32_t SYNFR;          /*!< (@ 0x00000054) Sync Message Flag Field Setting Register                   */

        struct
        {
            uint32_t              : 8;
            __IOM uint32_t FLAG8  : 1; /*!< [8..8] alternateMasterFlag                                                */
            __IOM uint32_t FLAG9  : 1; /*!< [9..9] twoStepFlag                                                        */
            __IOM uint32_t FLAG10 : 1; /*!< [10..10] unicastFlag                                                      */
            uint32_t              : 2;
            __IOM uint32_t FLAG13 : 1; /*!< [13..13] PTP profile Specific 1                                           */
            __IOM uint32_t FLAG14 : 1; /*!< [14..14] PTP profile Specific 2                                           */
            uint32_t              : 17;
        } SYNFR_b;
    };

    union
    {
        __IOM uint32_t DYRQFR;         /*!< (@ 0x00000058) Delay_Req Message Flag Field Setting Register              */

        struct
        {
            uint32_t              : 10;
            __IOM uint32_t FLAG10 : 1; /*!< [10..10] unicastFlag                                                      */
            uint32_t              : 2;
            __IOM uint32_t FLAG13 : 1; /*!< [13..13] PTP profile Specific 1                                           */
            __IOM uint32_t FLAG14 : 1; /*!< [14..14] PTP profile Specific 2                                           */
            uint32_t              : 17;
        } DYRQFR_b;
    };

    union
    {
        __IOM uint32_t DYRPFR;         /*!< (@ 0x0000005C) Delay_Resp Message Flag Field Setting Register             */

        struct
        {
            uint32_t              : 8;
            __IOM uint32_t FLAG8  : 1; /*!< [8..8] alternateMasterFlag                                                */
            __IOM uint32_t FLAG9  : 1; /*!< [9..9] woStepFlag                                                         */
            __IOM uint32_t FLAG10 : 1; /*!< [10..10] unicastFlag                                                      */
            uint32_t              : 2;
            __IOM uint32_t FLAG13 : 1; /*!< [13..13] PTP profile Specific 1                                           */
            __IOM uint32_t FLAG14 : 1; /*!< [14..14] PTP profile Specific 2                                           */
            uint32_t              : 17;
        } DYRPFR_b;
    };

    union
    {
        __IOM uint32_t SYCIDRU;          /*!< (@ 0x00000060) SYNFP Local Clock ID Registers                             */

        struct
        {
            __IOM uint32_t SYCIDRU : 32; /*!< [31..0] These bits hold the setting for the higher-order 32
                                          *   bits of the clock-ID of your port.                                        */
        } SYCIDRU_b;
    };

    union
    {
        __IOM uint32_t SYCIDRL;          /*!< (@ 0x00000064) SYNFP Local Clock ID Registers                             */

        struct
        {
            __IOM uint32_t SYCIDRL : 32; /*!< [31..0] These bits hold the setting for the lower-order 32 bits
                                          *   of the clock-ID of your port.                                             */
        } SYCIDRL_b;
    };

    union
    {
        __IOM uint32_t SYPNUMR;        /*!< (@ 0x00000068) SYNFP Local Port Number Register                           */

        struct
        {
            __IOM uint32_t PNUM : 16;  /*!< [15..0] Local Port Number SettingThese bits hold the setting
                                        *   for the port number of the local port.                                    */
            uint32_t : 16;
        } SYPNUMR_b;
    };
    __IM uint32_t RESERVED3[5];

    union
    {
        __OM uint32_t SYRVLDR;         /*!< (@ 0x00000080) SYNFP Register Value Load Directive Register               */

        struct
        {
            __OM uint32_t BMUP : 1;    /*!< [0..0] BMC Update                                                         */
            __OM uint32_t STUP : 1;    /*!< [1..1] State Update                                                       */
            __OM uint32_t ANUP : 1;    /*!< [2..2] Announce Message Generation Information Update                     */
            uint32_t           : 29;
        } SYRVLDR_b;
    };
    __IM uint32_t RESERVED4[3];

    union
    {
        __IOM uint32_t SYRFL1R;        /*!< (@ 0x00000090) SYNFP Reception Filter Register 1                          */

        struct
        {
            __IOM uint32_t ANCE0  : 1; /*!< [0..0] Announce Message Processing                                        */
            __IOM uint32_t ANCE1  : 1; /*!< [1..1] Announce Message Processing                                        */
            uint32_t              : 2;
            __IOM uint32_t SYNC0  : 1; /*!< [4..4] Sync Message Processing                                            */
            __IOM uint32_t SYNC1  : 1; /*!< [5..5] Sync Message Processing                                            */
            __IOM uint32_t SYNC2  : 1; /*!< [6..6] Sync Message Processing                                            */
            uint32_t              : 1;
            __IOM uint32_t FUP0   : 1; /*!< [8..8] Follow_Up Message Processing                                       */
            __IOM uint32_t FUP1   : 1; /*!< [9..9] Follow_Up Message Processing                                       */
            __IOM uint32_t FUP2   : 1; /*!< [10..10] Follow_Up Message Processing                                     */
            uint32_t              : 1;
            __IOM uint32_t DRQ0   : 1; /*!< [12..12] Delay_Req Message Processing                                     */
            __IOM uint32_t DRQ1   : 1; /*!< [13..13] Delay_Req Message Processing                                     */
            __IOM uint32_t DRQ2   : 1; /*!< [14..14] Delay_Req Message Processing                                     */
            uint32_t              : 1;
            __IOM uint32_t DRP0   : 1; /*!< [16..16] Delay_Resp Message Processing                                    */
            __IOM uint32_t DRP1   : 1; /*!< [17..17] Delay_Resp Message Processing                                    */
            __IOM uint32_t DRP2   : 1; /*!< [18..18] Delay_Resp Message Processing                                    */
            uint32_t              : 1;
            __IOM uint32_t PDRQ0  : 1; /*!< [20..20] Pdelay_Req Message Processing                                    */
            __IOM uint32_t PDRQ1  : 1; /*!< [21..21] Pdelay_Req Message Processing                                    */
            __IOM uint32_t PDRQ2  : 1; /*!< [22..22] Pdelay_Req Message Processing                                    */
            uint32_t              : 1;
            __IOM uint32_t PDRP0  : 1; /*!< [24..24] Pdelay_Resp Message Processing                                   */
            __IOM uint32_t PDRP1  : 1; /*!< [25..25] Pdelay_Resp Message Processing                                   */
            __IOM uint32_t PDRP2  : 1; /*!< [26..26] Pdelay_Resp Message Processing                                   */
            uint32_t              : 1;
            __IOM uint32_t PDFUP0 : 1; /*!< [28..28] Pdelay_Resp_Follow_Up Message Processing                         */
            __IOM uint32_t PDFUP1 : 1; /*!< [29..29] Pdelay_Resp_Follow_Up Message Processing                         */
            __IOM uint32_t PDFUP2 : 1; /*!< [30..30] Pdelay_Resp_Follow_Up Message Processing                         */
            uint32_t              : 1;
        } SYRFL1R_b;
    };

    union
    {
        __IOM uint32_t SYRFL2R;        /*!< (@ 0x00000094) SYNFP Reception Filter Register 2                          */

        struct
        {
            __IOM uint32_t MAN0 : 1;   /*!< [0..0] Management Message Processing Setting                              */
            __IOM uint32_t MAN1 : 1;   /*!< [1..1] Management Message Processing Setting                              */
            uint32_t            : 2;
            __IOM uint32_t SIG0 : 1;   /*!< [4..4] Signaling Message Processing Setting                               */
            __IOM uint32_t SIG1 : 1;   /*!< [5..5] Signaling Message Processing Setting                               */
            uint32_t            : 22;
            __IOM uint32_t ILL0 : 1;   /*!< [28..28] Illegal Message Processing Setting                               */
            __IOM uint32_t ILL1 : 1;   /*!< [29..29] Illegal Message Processing Setting                               */
            uint32_t            : 2;
        } SYRFL2R_b;
    };

    union
    {
        __IOM uint32_t SYTRENR;        /*!< (@ 0x00000098) SYNFP Transmission Enable Register                         */

        struct
        {
            __IOM uint32_t ANCE : 1;   /*!< [0..0] Announce Message Transmission Enable                               */
            uint32_t            : 3;
            __IOM uint32_t SYNC : 1;   /*!< [4..4] Sync Message Transmission Enable                                   */
            uint32_t            : 3;
            __IOM uint32_t DRQ  : 1;   /*!< [8..8] Delay_Req Message Transmission Enable                              */
            uint32_t            : 3;
            __IOM uint32_t PDRQ : 1;   /*!< [12..12] Pdelay_Req Message Transmission Enable                           */
            uint32_t            : 19;
        } SYTRENR_b;
    };
    __IM uint32_t RESERVED5;

    union
    {
        __IOM uint32_t MTCIDU;          /*!< (@ 0x000000A0) Master Clock ID Registers                                  */

        struct
        {
            __IOM uint32_t MTCIDU : 32; /*!< [31..0] These bits hold the setting for the higher-order 32
                                         *   bits of the clock-ID of the master clock.                                 */
        } MTCIDU_b;
    };

    union
    {
        __IOM uint32_t MTCIDL;          /*!< (@ 0x000000A4) Master Clock ID Registers                                  */

        struct
        {
            __IOM uint32_t MTCIDL : 32; /*!< [31..0] These bits hold the setting for the lower-order 32 bits
                                         *   of the clock-ID of the master clock.                                      */
        } MTCIDL_b;
    };

    union
    {
        __IOM uint32_t MTPID;          /*!< (@ 0x000000A8) Master clock port number register                          */

        struct
        {
            __IOM uint32_t PNUM : 16;  /*!< [15..0] Master Clock Port Number SettingThese bits hold the
                                        *   setting for the port number of the master clock.                          */
            uint32_t : 16;
        } MTPID_b;
    };
    __IM uint32_t RESERVED6[5];

    union
    {
        __IOM uint32_t SYTLIR;         /*!< (@ 0x000000C0) SYNFP Transmission Interval Setting Register               */

        struct
        {
            __IOM uint32_t ANCE : 8;   /*!< [7..0] Announce Message Transmission Interval SettingThese bits
                                        *   set the interval for the transmission of Announce messages.               */
            __IOM uint32_t SYNC : 8;   /*!< [15..8] Sync Message Transmission Interval SettingThese bits
                                        *   set the interval for the transmission of Sync messages.
                                        *   The setting is also placed in the logMessageInterval field
                                        *   of transmitted Sync messages.                                             */
            __IOM uint32_t DREQ : 8;   /*!< [23..16] Delay_Req Transmission Interval Average Value/ Pdelay_Req
                                        *   Transmission Interval SettingThe bits set the average interval
                                        *   for the transmission of Delay_Req messages and the interval
                                        *   for the transmission of Pdelay_Req messages.The setting
                                        *   is also placed in the logMessageInterval field of Delay_Resp
                                        *   messages.                                                                 */
            uint32_t : 8;
        } SYTLIR_b;
    };

    union
    {
        __IM uint32_t SYRLIR;          /*!< (@ 0x000000C4) SYNFP Received logMessageInterval Value Indication
                                        *                  Register                                                   */

        struct
        {
            __IM uint32_t ANCE : 8;    /*!< [7..0] Announce Message logMessageInterval Field IndicationThese
                                        *   bits indicate the logMessageInterval field value of a received
                                        *   Announce message.                                                         */
            __IM uint32_t SYNC : 8;    /*!< [15..8] Sync Message logMessageInterval Field IndicationThese
                                        *   bits indicate the logMessageInterval field value of a received
                                        *   Sync message.                                                             */
            __IM uint32_t DRESP : 8;   /*!< [23..16] Delay_Resp Message logMessageInterval Field IndicationThese
                                        *   bits indicate the logMessageInterval field value of a received
                                        *   Delay_Resp message.                                                       */
            uint32_t : 8;
        } SYRLIR_b;
    };

    union
    {
        __IM uint32_t OFMRU;           /*!< (@ 0x000000C8) offsetFromMaster Value Registers                           */

        struct
        {
            __IM uint32_t OFMRU : 32;  /*!< [31..0] These bits indicate the higher-order 32 bits of the
                                        *   calculated offsetFromMaster value.                                        */
        } OFMRU_b;
    };

    union
    {
        __IM uint32_t OFMRL;           /*!< (@ 0x000000CC) offsetFromMaster Value Registers                           */

        struct
        {
            __IM uint32_t OFMRL : 32;  /*!< [31..0] These bits indicate the lower-order 32 bits of the calculated
                                        *   offsetFromMaster value.                                                   */
        } OFMRL_b;
    };

    union
    {
        __IM uint32_t MPDRU;           /*!< (@ 0x000000D0) meanPathDelay Value Registers                              */

        struct
        {
            __IM uint32_t MPDRU : 32;  /*!< [31..0] These bits indicate the higher-order 32 bits of the
                                        *   calculated meanPathDelay value.                                           */
        } MPDRU_b;
    };

    union
    {
        __IM uint32_t MPDRL;           /*!< (@ 0x000000D4) meanPathDelay Value Registers                              */

        struct
        {
            __IM uint32_t MPDRL : 32;  /*!< [31..0] These bits indicate the lower-order 32 bits of the calculated
                                        *   meanPathDelay value.                                                      */
        } MPDRL_b;
    };
    __IM uint32_t RESERVED7[2];

    union
    {
        __IOM uint32_t GMPR;           /*!< (@ 0x000000E0) grandmasterPriority Field Setting Register                 */

        struct
        {
            __IOM uint32_t GMPR2 : 8;  /*!< [7..0] grandmasterPriority2 Field Value SettingThese bits are
                                        *   used to set the value of the grandmasterPriority2 fields
                                        *   of Announce messages.                                                     */
            uint32_t             : 8;
            __IOM uint32_t GMPR1 : 8;  /*!< [23..16] grandmasterPriority1 Field Value SettingThese bits
                                        *   are used to set the value of the grandmasterPriority1 fields
                                        *   of Announce messages.                                                     */
            uint32_t : 8;
        } GMPR_b;
    };

    union
    {
        __IOM uint32_t GMCQR;          /*!< (@ 0x000000E4) grandmasterClockQuality Field Setting Register             */

        struct
        {
            __IOM uint32_t GMCQR : 32; /*!< [31..0] These bits are used to set the value of the grandmasterClockQuality
                                        *   fields of Announce messages. The correspondence between
                                        *   bits and the grandmasterClockQuality fields is as listed
                                        *   below.b31 to b24: clockClassb23 to b16: clockAccuracyb15
                                        *   to b0: offsetScaledLogVariance                                            */
        } GMCQR_b;
    };

    union
    {
        __IOM uint32_t GMIDRU;          /*!< (@ 0x000000E8) grandmasterIdentity Field Setting Registers                */

        struct
        {
            __IOM uint32_t GMIDRU : 32; /*!< [31..0] These bits hold the setting for the higher-order 32
                                         *   bits of the value of the grandmasterIdentity fields of
                                         *   Announce messages.                                                        */
        } GMIDRU_b;
    };

    union
    {
        __IOM uint32_t GMIDRL;          /*!< (@ 0x000000EC) grandmasterIdentity Field Setting Registers                */

        struct
        {
            __IOM uint32_t GMIDRL : 32; /*!< [31..0] These bits hold the setting for the lower-order 32 bits
                                         *   of the value of the grandmasterIdentity fields of Announce
                                         *   messages.                                                                 */
        } GMIDRL_b;
    };

    union
    {
        __IOM uint32_t CUOTSR;         /*!< (@ 0x000000F0) currentUtcOffset/timeSource Field Setting Register         */

        struct
        {
            __IOM uint32_t TSRC : 8;   /*!< [7..0] timeSource Field SettingThese bits set the value of the
                                        *   timeSource fields of Announce messages.                                   */
            uint32_t            : 8;
            __IOM uint32_t CUTO : 16;  /*!< [31..16] currentUtcOffset Field SettingThese bits set the value
                                        *   of the currentUtcOffset fields of Announce messages.                      */
        } CUOTSR_b;
    };

    union
    {
        __IOM uint32_t SRR;            /*!< (@ 0x000000F4) stepsRemoved Field Setting Register                        */

        struct
        {
            __IOM uint32_t SRMV : 16;  /*!< [15..0] stepsRemoved Field Value SettingThese bits set the value
                                        *   of the stepsRemoved fields of Announce messages.                          */
            uint32_t : 16;
        } SRR_b;
    };
    __IM uint32_t RESERVED8[2];

    union
    {
        __IOM uint32_t PPMACRU;          /*!< (@ 0x00000100) PTP-primary Message Destination MAC Address Setting
                                          *                  Registers                                                  */

        struct
        {
            __IOM uint32_t PPMACRU : 24; /*!< [23..0] These bits hold the setting for the higher-order 24
                                          *   bits of the destination MAC address for PTP-primary messages.             */
            uint32_t : 8;
        } PPMACRU_b;
    };

    union
    {
        __IOM uint32_t PPMACRL;          /*!< (@ 0x00000104) PTP-primary Message Destination MAC Address Setting
                                          *                  Registers                                                  */

        struct
        {
            __IOM uint32_t PPMACRL : 24; /*!< [23..0] These bits hold the setting for the lower-order 24 bits
                                          *   of the destination MAC address for PTP-primary messages.                  */
            uint32_t : 8;
        } PPMACRL_b;
    };

    union
    {
        __IOM uint32_t PDMACRU;          /*!< (@ 0x00000108) PTP-pdelay Message MAC Address Setting Registers           */

        struct
        {
            __IOM uint32_t PDMACRU : 24; /*!< [23..0] These bits hold the setting for the higher-order 24
                                          *   bits of the destination MAC address for PTP-pdelay messages.              */
            uint32_t : 8;
        } PDMACRU_b;
    };

    union
    {
        __IOM uint32_t PDMACRL;          /*!< (@ 0x0000010C) PTP-pdelay Message MAC Address Setting Registers           */

        struct
        {
            __IOM uint32_t PDMACRL : 24; /*!< [23..0] These bits hold the setting for the lower-order 24 bits
                                          *   of the destination MAC address for PTP-pdelay messages.                   */
            uint32_t : 8;
        } PDMACRL_b;
    };

    union
    {
        __IOM uint32_t PETYPER;        /*!< (@ 0x00000110) PTP Message EtherType Setting Register                     */

        struct
        {
            __IOM uint32_t TYPE : 16;  /*!< [15..0] PTP Message EtherType Value SettingThese bits hold the
                                        *   setting for the EtherType field value for frames in the
                                        *   Ethernet II format.                                                       */
            uint32_t : 16;
        } PETYPER_b;
    };
    __IM uint32_t RESERVED9[3];

    union
    {
        __IOM uint32_t PPIPR;          /*!< (@ 0x00000120) PTP-primary Message Destination IP Address Setting
                                        *                  Register                                                   */

        struct
        {
            __IOM uint32_t PPIPR : 32; /*!< [31..0] These bits hold the setting for the destination IP address
                                        *   for PTPprimary messages.                                                  */
        } PPIPR_b;
    };

    union
    {
        __IOM uint32_t PDIPR;          /*!< (@ 0x00000124) PTP-pdelay Message Destination IP Address Setting
                                        *                  Register                                                   */

        struct
        {
            __IOM uint32_t PDIPR : 32; /*!< [31..0] These bits hold the setting for the destination IP address
                                        *   for PTPpdelay messages.                                                   */
        } PDIPR_b;
    };

    union
    {
        __IOM uint32_t PETOSR;         /*!< (@ 0x00000128) PTP Event Message TOS Setting Register                     */

        struct
        {
            __IOM uint32_t EVTO : 8;   /*!< [7..0] PTP Event Message TOS Field Value SettingThese bits hold
                                        *   the setting for the value of the TOS field within the IPv4
                                        *   headers of PTP event messages.                                            */
            uint32_t : 24;
        } PETOSR_b;
    };

    union
    {
        __IOM uint32_t PGTOSR;         /*!< (@ 0x0000012C) PTP general Message TOS Setting Register                   */

        struct
        {
            __IOM uint32_t GETO : 8;   /*!< [7..0] PTP general Message TOS Field Value SettingThese bits
                                        *   hold the setting for the value of the TOS field within
                                        *   the IPv4 headers of PTP general messages.                                 */
            uint32_t : 24;
        } PGTOSR_b;
    };

    union
    {
        __IOM uint32_t PPTTLR;         /*!< (@ 0x00000130) PTP-primary Message TTL Setting Register                   */

        struct
        {
            __IOM uint32_t PRTL : 8;   /*!< [7..0] PTP-primary Message TTL Field Value SettingThese bits
                                        *   hold the setting for the value of the TTL field within
                                        *   the IPv4 headers of PTP-primary messages.                                 */
            uint32_t : 24;
        } PPTTLR_b;
    };

    union
    {
        __IOM uint32_t PDTTLR;         /*!< (@ 0x00000134) PTP-pdelay Message TTL Setting Register                    */

        struct
        {
            __IOM uint32_t PDTL : 8;   /*!< [7..0] PTP-pdelay Message TTL Field ValueThese bits hold the
                                        *   setting for the value of the TTL field within the IPv4
                                        *   headers of PTP-pdelay messages.                                           */
            uint32_t : 24;
        } PDTTLR_b;
    };

    union
    {
        __IOM uint32_t PEUDPR;         /*!< (@ 0x00000138) PTP Event Message UDP Destination Port Number
                                        *                  Setting Register                                           */

        struct
        {
            __IOM uint32_t EVUPT : 16; /*!< [15..0] PTP Event Message Destination Port Number SettingThese
                                        *   bits hold the setting for the value of the destination
                                        *   port number field within the UDP headers of PTP event messages.           */
            uint32_t : 16;
        } PEUDPR_b;
    };

    union
    {
        __IOM uint32_t PGUDPR;         /*!< (@ 0x0000013C) PTP general Message UDP Destination Port Number
                                        *                  Setting Register                                           */

        struct
        {
            __IOM uint32_t GEUPT : 16; /*!< [15..0] PTP general Message Destination Port NumberThese bits
                                        *   hold the setting for the value of the destination port
                                        *   number field within the UDP headers of PTP general messages.              */
            uint32_t : 16;
        } PGUDPR_b;
    };

    union
    {
        __IOM uint32_t FFLTR;          /*!< (@ 0x00000140) Frame Reception Filter Setting Register                    */

        struct
        {
            __IOM uint32_t SEL : 1;    /*!< [0..0] Receive MAC Address SelectNOTE: The setting of these
                                        *   bits is only effective when EXTPRM=0, ENB=1and RPT=1.                     */
            __IOM uint32_t PRT : 1;    /*!< [1..1] Frame Reception EnableNOTE: The setting of these bits
                                        *   is only effective when EXTPRM=0 and ENB=1.                                */
            __IOM uint32_t ENB : 1;    /*!< [2..2] Reception Filter EnableNOTE: The setting of these bits
                                        *   is only effective when EXTPRM=0.                                          */
            uint32_t              : 13;
            __IOM uint32_t EXTPRM : 1; /*!< [16..16] Extended Promiscuous ModeSetting                                 */
            uint32_t              : 15;
        } FFLTR_b;
    };
    __IM uint32_t RESERVED10[7];

    union
    {
        __IOM uint32_t FMAC0RU;          /*!< (@ 0x00000160) Frame Reception Filter MAC Address 0 Setting
                                          *                  Register Upper                                             */

        struct
        {
            __IOM uint32_t FMAC0RU : 24; /*!< [23..0] These bits specify the upper-order 24 bits of the destination
                                          *   MAC address for received multicast frames.                                */
            uint32_t : 8;
        } FMAC0RU_b;
    };

    union
    {
        __IOM uint32_t FMAC0RL;          /*!< (@ 0x00000164) Frame Reception Filter MAC Address 0 Setting
                                          *                  Register Lower                                             */

        struct
        {
            __IOM uint32_t FMAC0RL : 24; /*!< [23..0] These bits specify the lower-order 24 bits of the destination
                                          *   MAC address for received multicast frames.                                */
            uint32_t : 8;
        } FMAC0RL_b;
    };

    union
    {
        __IOM uint32_t FMAC1RU;          /*!< (@ 0x00000168) Frame Reception Filter MAC Address 1 Setting
                                          *                  Register Upper                                             */

        struct
        {
            __IOM uint32_t FMAC1RU : 24; /*!< [23..0] These bits specify the upper-order 24 bits of the destination
                                          *   MAC address for received multicast frames.                                */
            uint32_t : 8;
        } FMAC1RU_b;
    };

    union
    {
        __IOM uint32_t FMAC1RL;          /*!< (@ 0x0000016C) Frame Reception Filter MAC Address 1 Setting
                                          *                  Register Lower                                             */

        struct
        {
            __IOM uint32_t FMAC1RL : 24; /*!< [23..0] These bits specify the lower-order 24 bits of the destination
                                          *   MAC address for received multicast frames.                                */
            uint32_t : 8;
        } FMAC1RL_b;
    };
    __IM uint32_t RESERVED11[20];

    union
    {
        __IOM uint32_t DASYMRU;          /*!< (@ 0x000001C0) Asymmetric Delay Setting Registers                         */

        struct
        {
            __IOM uint32_t DASYMRU : 16; /*!< [15..0] These bits hold the setting for the higher-order 16
                                          *   bits of the asymmetric delay value.                                       */
            uint32_t : 16;
        } DASYMRU_b;
    };

    union
    {
        __IOM uint32_t DASYMRL;          /*!< (@ 0x000001C4) Asymmetric Delay Setting Registers                         */

        struct
        {
            __IOM uint32_t DASYMRL : 32; /*!< [31..0] These bits hold the setting for the lower-order 32 bits
                                          *   of the asymmetric delay value.                                            */
        } DASYMRL_b;
    };

    union
    {
        __IOM uint32_t TSLATR;         /*!< (@ 0x000001C8) Timestamp Latency Setting Register                         */

        struct
        {
            __IOM uint32_t EGP : 16;   /*!< [15..0] Input Port Timestamp Latency SettingThese bits hold
                                        *   the setting for the time stamp latency (ns) for the input
                                        *   ports.                                                                    */
            __IOM uint32_t INGP : 16;  /*!< [31..16] Output Port Timestamp Latency SettingThese bits hold
                                        *   the setting for the time stamp latency (ns) for the output
                                        *   ports.                                                                    */
        } TSLATR_b;
    };

    union
    {
        __IOM uint32_t SYCONFR;        /*!< (@ 0x000001CC) SYNFP Operation Setting Register                           */

        struct
        {
            __IOM uint32_t TCYC : 8;   /*!< [7..0] PTP Message Transmission Interval SettingThese bits are
                                        *   used to set the time from the completion of one transmission
                                        *   to the start of the next in cycles of the transmission
                                        *   clock. A value n in these bits means that a transmission
                                        *   interval of n cycles will be secured.No interval is secured
                                        *   if the setting is 00h.We recommend the setting 28h (40
                                        *   cycles).                                                                  */
            uint32_t              : 4;
            __IOM uint32_t SBDIS  : 1; /*!< [12..12] Sync Message Transmission Bandwidth Securing Disable             */
            uint32_t              : 3;
            __IOM uint32_t FILDIS : 1; /*!< [16..16] Receive Message domainNumber Filter Disable                      */
            uint32_t              : 3;
            __IOM uint32_t TCMOD  : 1; /*!< [20..20] TC Mode Setting                                                  */
            uint32_t              : 11;
        } SYCONFR_b;
    };

    union
    {
        __IOM uint32_t SYFORMR;        /*!< (@ 0x000001D0) SYNFP Frame Format Setting Register                        */

        struct
        {
            __IOM uint32_t FORM0 : 1;  /*!< [0..0] Ethernet/UDP Encapsulation                                         */
            __IOM uint32_t FORM1 : 1;  /*!< [1..1] Ethernet Frame Format Setting                                      */
            uint32_t             : 30;
        } SYFORMR_b;
    };

    union
    {
        __IOM uint32_t RSTOUTR;          /*!< (@ 0x000001D4) Response Message Reception Timeout Register                */

        struct
        {
            __IOM uint32_t RSTOUTR : 32; /*!< [31..0] Response Message Reception Timeout Time SettingA response
                                          *   message not being received within n x 1024 (ns), where
                                          *   n is the setting, is judged to represent a timeout.                       */
        } RSTOUTR_b;
    };
} R_ETHERC_EPTPC_Type;                   /*!< Size = 472 (0x1d8)                                                        */

/* =========================================================================================================================== */
/* ================                                    R_ETHERC_EPTPC_CFG                                     ================ */
/* =========================================================================================================================== */

/**
 * @brief Ethernet PTP Configuration (R_ETHERC_EPTPC_CFG)
 */

typedef struct                         /*!< (@ 0x40064500) R_ETHERC_EPTPC_CFG Structure                               */
{
    union
    {
        __IOM uint32_t PTRSTR;         /*!< (@ 0x00000000) EPTPC Reset Register                                       */

        struct
        {
            __IOM uint32_t RESET : 1;  /*!< [0..0] EPTPC Software Reset                                               */
            uint32_t             : 31;
        } PTRSTR_b;
    };

    union
    {
        __IOM uint32_t STCSELR;         /*!< (@ 0x00000004) STCA Clock Select Register                                 */

        struct
        {
            __IOM uint32_t SCLKDIV : 3; /*!< [2..0] PCLKA Clock Frequency Division                                     */
            uint32_t               : 5;
            __IOM uint32_t SCLKSEL : 3; /*!< [10..8] STCA Clock Select                                                 */
            uint32_t               : 21;
        } STCSELR_b;
    };

    union
    {
        __IOM uint32_t BYPASS;          /*!< (@ 0x00000008) Bypass 1588 module Register                                */

        struct
        {
            __IOM uint32_t BYPASS0 : 1; /*!< [0..0] Bypass 1588 module for Ether 0ch                                   */
            uint32_t               : 15;
            __IOM uint32_t BYPASS1 : 1; /*!< [16..16] Bypass 1588 module for Ether 1ch                                 */
            uint32_t               : 15;
        } BYPASS_b;
    };
} R_ETHERC_EPTPC_CFG_Type;              /*!< Size = 12 (0xc)                                                           */

/**
 * @brief R_ETHERC_EPTPC_COMMON_TM [TM] (Timer Setting Registers)
 */
typedef struct
{
    union
    {
        __IOM uint32_t STTRU;            /*!< (@ 0x00000000) Timer Start Time Setting Register                          */

        struct
        {
            __IOM uint32_t TMSTTRU : 32; /*!< [31..0] These bits hold the setting for the higher-order 32
                                          *   bits of the start time of the pulse output timer in nanoseconds.          */
        } STTRU_b;
    };

    union
    {
        __IOM uint32_t STTRL;            /*!< (@ 0x00000004) Timer Start Time Setting Register                          */

        struct
        {
            __IOM uint32_t TMSTTRL : 32; /*!< [31..0] These bits hold the setting for the lower-order 32 bits
                                          *   of the start time of the pulse output timer in nanoseconds.               */
        } STTRL_b;
    };

    union
    {
        __IOM uint32_t CYCR;            /*!< (@ 0x00000008) Timer Cycle Setting Registers                              */

        struct
        {
            __IOM uint32_t TMCYCR : 30; /*!< [29..0] These bits set the cycle of the pulse output timer in
                                         *   nanoseconds. Set a value that is equivalent to at least
                                         *   four cycles of the STCA clock.                                            */
            uint32_t : 2;
        } CYCR_b;
    };

    union
    {
        __IOM uint32_t PLSR;            /*!< (@ 0x0000000C) Timer Pulse Width Setting Register                         */

        struct
        {
            __IOM uint32_t TMPLSR : 29; /*!< [28..0] These bits set the width at high level of the pulse
                                         *   signal from the timer in nanoseconds. Set a value that
                                         *   is equivalent to at least two cycles of the STCA clock.                   */
            uint32_t : 3;
        } PLSR_b;
    };
} R_ETHERC_EPTPC_COMMON_TM_Type;        /*!< Size = 16 (0x10)                                                          */

/**
 * @brief R_ETHERC_EPTPC_COMMON_PR [PR] (Local MAC Address Registers)
 */
typedef struct
{
    union
    {
        __IOM uint32_t MACRU;            /*!< (@ 0x00000000) Channel Local MAC Address Register                         */

        struct
        {
            __IOM uint32_t PRMACRU : 24; /*!< [23..0] These bits hold the setting for the higher-order 24
                                          *   bits of the local MAC address for Ethernet port 0.                        */
            uint32_t : 8;
        } MACRU_b;
    };

    union
    {
        __IOM uint32_t MACRL;            /*!< (@ 0x00000004) Channel Local MAC Address Register                         */

        struct
        {
            __IOM uint32_t PRMACRL : 24; /*!< [23..0] These bits hold the setting for the higher-order 24
                                          *   bits of the local MAC address for Ethernet port 0.                        */
            uint32_t : 8;
        } MACRL_b;
    };
} R_ETHERC_EPTPC_COMMON_PR_Type;         /*!< Size = 8 (0x8)                                                            */

/* =========================================================================================================================== */
/* ================                                   R_ETHERC_EPTPC_COMMON                                   ================ */
/* =========================================================================================================================== */

/**
 * @brief Ethernet PTP Controller Common (R_ETHERC_EPTPC_COMMON)
 */

typedef struct                         /*!< (@ 0x40065000) R_ETHERC_EPTPC_COMMON Structure                            */
{
    union
    {
        __IOM uint32_t MIESR;          /*!< (@ 0x00000000) MINT Interrupt Source Status Register                      */

        struct
        {
            __IM uint32_t ST    : 1;   /*!< [0..0] STCA Status Flag                                                   */
            __IM uint32_t SY0   : 1;   /*!< [1..1] SYNFP0 Status Flag                                                 */
            __IM uint32_t SY1   : 1;   /*!< [2..2] SYNFP1 Status Flag                                                 */
            __IM uint32_t PRC   : 1;   /*!< [3..3] PRC-TC Status Flag                                                 */
            uint32_t            : 12;
            __IOM uint32_t CYC0 : 1;   /*!< [16..16] Pulse Output Timer 0 Rising Edge Detection Flag                  */
            __IOM uint32_t CYC1 : 1;   /*!< [17..17] Pulse Output Timer 1 Rising Edge Detection Flag                  */
            __IOM uint32_t CYC2 : 1;   /*!< [18..18] Pulse Output Timer 2 Rising Edge Detection Flag                  */
            __IOM uint32_t CYC3 : 1;   /*!< [19..19] Pulse Output Timer 3 Rising Edge Detection Flag                  */
            __IOM uint32_t CYC4 : 1;   /*!< [20..20] Pulse Output Timer 4 Rising Edge Detection Flag                  */
            __IOM uint32_t CYC5 : 1;   /*!< [21..21] Pulse Output Timer 5 Rising Edge Detection Flag                  */
            uint32_t            : 10;
        } MIESR_b;
    };

    union
    {
        __IOM uint32_t MIEIPR;         /*!< (@ 0x00000004) MINT Interrupt Request Permission Register                 */

        struct
        {
            __IOM uint32_t ST   : 1;   /*!< [0..0] STCA Status Interrupt Request Permission                           */
            __IOM uint32_t SY0  : 1;   /*!< [1..1] SYNFP0 Status Interrupt Request Permission                         */
            __IOM uint32_t SY1  : 1;   /*!< [2..2] SYNFP1 Status Interrupt Request Permission                         */
            __IOM uint32_t PRC  : 1;   /*!< [3..3] PRC-TC Status Interrupt Request Permission                         */
            uint32_t            : 12;
            __IOM uint32_t CYC0 : 1;   /*!< [16..16] Pulse Output Timer 0 Rising Edge Detection Interrupt
                                        *   Request Permission                                                        */
            __IOM uint32_t CYC1 : 1;   /*!< [17..17] Pulse Output Timer 1 Rising Edge Detection Interrupt
                                        *   Request Permission                                                        */
            __IOM uint32_t CYC2 : 1;   /*!< [18..18] Pulse Output Timer 2 Rising Edge Detection Interrupt
                                        *   Request Permission                                                        */
            __IOM uint32_t CYC3 : 1;   /*!< [19..19] Pulse Output Timer 3 Rising Edge Detection Interrupt
                                        *   Request Permission                                                        */
            __IOM uint32_t CYC4 : 1;   /*!< [20..20] Pulse Output Timer 4 Rising Edge Detection Interrupt
                                        *   Request Permission                                                        */
            __IOM uint32_t CYC5 : 1;   /*!< [21..21] Pulse Output Timer 5 Rising Edge Detection Interrupt
                                        *   Request Permission                                                        */
            uint32_t : 10;
        } MIEIPR_b;
    };
    __IM uint32_t RESERVED[2];

    union
    {
        __IOM uint32_t ELIPPR;         /*!< (@ 0x00000010) ELC Output/ETHER_IPLS Interrupt Request Permission
                                        *                  Register                                                   */

        struct
        {
            __IOM uint32_t CYCP0 : 1;  /*!< [0..0] Pulse Output Timer 0 Rising Edge Detection Event Output
                                        *   Enable                                                                    */
            __IOM uint32_t CYCP1 : 1;  /*!< [1..1] Pulse Output Timer 1 Rising Edge Detection Event Output
                                        *   Enable                                                                    */
            __IOM uint32_t CYCP2 : 1;  /*!< [2..2] Pulse Output Timer 2 Rising Edge Detection Event Output
                                        *   Enable                                                                    */
            __IOM uint32_t CYCP3 : 1;  /*!< [3..3] Pulse Output Timer 3 Rising Edge Detection Event Output
                                        *   Enable                                                                    */
            __IOM uint32_t CYCP4 : 1;  /*!< [4..4] Pulse Output Timer 4 Rising Edge Detection Event Output
                                        *   Enable                                                                    */
            __IOM uint32_t CYCP5 : 1;  /*!< [5..5] Pulse Output Timer 5 Rising Edge Detection Event Output
                                        *   Enable                                                                    */
            uint32_t             : 2;
            __IOM uint32_t CYCN0 : 1;  /*!< [8..8] Pulse Output Timer 0 Falling Edge Detection Event Output
                                        *   Enable                                                                    */
            __IOM uint32_t CYCN1 : 1;  /*!< [9..9] Pulse Output Timer 1 Falling Edge Detection Event Output
                                        *   Enable                                                                    */
            __IOM uint32_t CYCN2 : 1;  /*!< [10..10] Pulse Output Timer 2 Falling Edge Detection Event Output
                                        *   Enable                                                                    */
            __IOM uint32_t CYCN3 : 1;  /*!< [11..11] Pulse Output Timer 3 Falling Edge Detection Event Output
                                        *   Enable                                                                    */
            __IOM uint32_t CYCN4 : 1;  /*!< [12..12] Pulse Output Timer 4 Falling Edge Detection Event Output
                                        *   Enable                                                                    */
            __IOM uint32_t CYCN5 : 1;  /*!< [13..13] Pulse Output Timer 5 Falling Edge Detection Event Output
                                        *   Enable                                                                    */
            uint32_t            : 2;
            __IOM uint32_t PLSP : 1;   /*!< [16..16] Pulse Output Timer Rising Edge Detection IPLS Interrupt
                                        *   Request Permission                                                        */
            uint32_t            : 7;
            __IOM uint32_t PLSN : 1;   /*!< [24..24] Pulse Output Timer Falling Edge Detection IPLS Interrupt
                                        *   Request Permission                                                        */
            uint32_t : 7;
        } ELIPPR_b;
    };

    union
    {
        __IOM uint32_t ELIPACR;        /*!< (@ 0x00000014) ELC Output/IPLS Interrupt Permission Automatic
                                        *                  Clearing Register                                          */

        struct
        {
            __IOM uint32_t CYCP0 : 1;  /*!< [0..0] ELIPPR.CYCP0 Bit Automatic Clearing                                */
            __IOM uint32_t CYCP1 : 1;  /*!< [1..1] ELIPPR.CYCP1 Bit Automatic Clearing                                */
            __IOM uint32_t CYCP2 : 1;  /*!< [2..2] ELIPPR.CYCP2 Bit Automatic Clearing                                */
            __IOM uint32_t CYCP3 : 1;  /*!< [3..3] ELIPPR.CYCP3 Bit Automatic Clearing                                */
            __IOM uint32_t CYCP4 : 1;  /*!< [4..4] ELIPPR.CYCP4 Bit Automatic Clearing                                */
            __IOM uint32_t CYCP5 : 1;  /*!< [5..5] ELIPPR.CYCP5 Bit Automatic Clearing                                */
            uint32_t             : 2;
            __IOM uint32_t CYCN0 : 1;  /*!< [8..8] ELIPPR.CYCN0 Bit Automatic Clearing                                */
            __IOM uint32_t CYCN1 : 1;  /*!< [9..9] ELIPPR.CYCN1 Bit Automatic Clearing                                */
            __IOM uint32_t CYCN2 : 1;  /*!< [10..10] ELIPPR.CYCN2 Bit Automatic Clearing                              */
            __IOM uint32_t CYCN3 : 1;  /*!< [11..11] ELIPPR.CYCN3 Bit Automatic Clearing                              */
            __IOM uint32_t CYCN4 : 1;  /*!< [12..12] ELIPPR.CYCN4 Bit Automatic Clearing                              */
            __IOM uint32_t CYCN5 : 1;  /*!< [13..13] ELIPPR.CYCN5 Bit Automatic Clearing                              */
            uint32_t             : 2;
            __IOM uint32_t PLSP  : 1;  /*!< [16..16] ELIPPR.PLSP Bit Automatic Clearing                               */
            uint32_t             : 7;
            __IOM uint32_t PLSN  : 1;  /*!< [24..24] ELIPPR.PLSN Bit Automatic Clearing                               */
            uint32_t             : 7;
        } ELIPACR_b;
    };
    __IM uint32_t RESERVED1[10];

    union
    {
        __IOM uint32_t STSR;            /*!< (@ 0x00000040) STCA Status Register                                       */

        struct
        {
            __IOM uint32_t SYNC    : 1; /*!< [0..0] Synchronized State Detection Flag                                  */
            __IOM uint32_t SYNCOUT : 1; /*!< [1..1] Synchronization Loss Detection Flag                                */
            uint32_t               : 1;
            __IOM uint32_t SYNTOUT : 1; /*!< [3..3] Sync Message Reception Timeout Detection Flag                      */
            __IOM uint32_t W10D    : 1; /*!< [4..4] Worst 10 Acquisition Completion Flag                               */
            uint32_t               : 27;
        } STSR_b;
    };

    union
    {
        __IOM uint32_t STIPR;           /*!< (@ 0x00000044) STCA Status Notification Permission Register               */

        struct
        {
            __IOM uint32_t SYNC    : 1; /*!< [0..0] SYNC Status Notification Enable                                    */
            __IOM uint32_t SYNCOUT : 1; /*!< [1..1] SYNCOUT Status Notification Enable                                 */
            uint32_t               : 1;
            __IOM uint32_t SYNTOUT : 1; /*!< [3..3] SYNTOUT Status Notification Enable                                 */
            __IOM uint32_t W10D    : 1; /*!< [4..4] W10D Status Notification Enable                                    */
            uint32_t               : 27;
        } STIPR_b;
    };
    __IM uint32_t RESERVED2[2];

    union
    {
        __IOM uint32_t STCFR;          /*!< (@ 0x00000050) STCA Clock Frequency Setting Register                      */

        struct
        {
            __IOM uint32_t STCF : 2;   /*!< [1..0] STCA Clock Frequency                                               */
            uint32_t            : 30;
        } STCFR_b;
    };

    union
    {
        __IOM uint32_t STMR;           /*!< (@ 0x00000054) STCA Operating Mode Register                               */

        struct
        {
            __IOM uint32_t WINT  : 8;  /*!< [7..0] Worst 10 Acquisition Time                                          */
            uint32_t             : 5;
            __IOM uint32_t CMOD  : 1;  /*!< [13..13] Time Synchronization Correction Mode                             */
            uint32_t             : 1;
            __IOM uint32_t W10S  : 1;  /*!< [15..15] Worst 10 Acquisition Control Select                              */
            __IOM uint32_t SYTH  : 4;  /*!< [19..16] Synchronized State Detection Threshold Setting                   */
            __IOM uint32_t DVTH  : 4;  /*!< [23..20] Synchronization Loss Detection Threshold Setting                 */
            uint32_t             : 4;
            __IOM uint32_t ALEN0 : 1;  /*!< [28..28] Alarm Detection Enable 0                                         */
            __IOM uint32_t ALEN1 : 1;  /*!< [29..29] Alarm Detection Enable 1                                         */
            uint32_t             : 2;
        } STMR_b;
    };

    union
    {
        __IOM uint32_t SYNTOR;          /*!< (@ 0x00000058) Sync Message Reception Timeout Register                    */

        struct
        {
            __IOM uint32_t SYNTOR : 32; /*!< [31..0] A Sync message not being received within 1024 x n (ns),
                                         *   where n is the setting, leads to a timeout for reception
                                         *   of Sync messages, leading to the STSR.SYNTOUT flag being
                                         *   set to 1.                                                                 */
        } SYNTOR_b;
    };
    __IM uint32_t RESERVED3;

    union
    {
        __IOM uint32_t IPTSELR;         /*!< (@ 0x00000060) IPLS Interrupt Request Timer Select Register               */

        struct
        {
            __IOM uint32_t IPTSEL0 : 1; /*!< [0..0] Pulse Output Timer 0 Select                                        */
            __IOM uint32_t IPTSEL1 : 1; /*!< [1..1] Pulse Output Timer 1 Select                                        */
            __IOM uint32_t IPTSEL2 : 1; /*!< [2..2] Pulse Output Timer 2 Select                                        */
            __IOM uint32_t IPTSEL3 : 1; /*!< [3..3] Pulse Output Timer 3 Select                                        */
            __IOM uint32_t IPTSEL4 : 1; /*!< [4..4] Pulse Output Timer 4 Select                                        */
            __IOM uint32_t IPTSEL5 : 1; /*!< [5..5] Pulse Output Timer 5 Select                                        */
            uint32_t               : 26;
        } IPTSELR_b;
    };

    union
    {
        __IOM uint32_t MITSELR;         /*!< (@ 0x00000064) MINT Interrupt Request Timer Select Register               */

        struct
        {
            __IOM uint32_t MINTEN0 : 1; /*!< [0..0] Pulse Output Timer 0 MINT Interrupt Output Enable                  */
            __IOM uint32_t MINTEN1 : 1; /*!< [1..1] Pulse Output Timer 1 MINT Interrupt Output Enable                  */
            __IOM uint32_t MINTEN2 : 1; /*!< [2..2] Pulse Output Timer 2 MINT Interrupt Output Enable                  */
            __IOM uint32_t MINTEN3 : 1; /*!< [3..3] Pulse Output Timer 3 MINT Interrupt Output Enable                  */
            __IOM uint32_t MINTEN4 : 1; /*!< [4..4] Pulse Output Timer 4 MINT Interrupt Output Enable                  */
            __IOM uint32_t MINTEN5 : 1; /*!< [5..5] Pulse Output Timer 5 MINT Interrupt Output Enable                  */
            uint32_t               : 26;
        } MITSELR_b;
    };

    union
    {
        __IOM uint32_t ELTSELR;         /*!< (@ 0x00000068) ELC Output Timer Select Register                           */

        struct
        {
            __IOM uint32_t ELTDIS0 : 1; /*!< [0..0] Pulse Output Timer 0 Event Generation Disable                      */
            __IOM uint32_t ELTDIS1 : 1; /*!< [1..1] Pulse Output Timer 1 Event Generation Disable                      */
            __IOM uint32_t ELTDIS2 : 1; /*!< [2..2] Pulse Output Timer 2 Event Generation Disable                      */
            __IOM uint32_t ELTDIS3 : 1; /*!< [3..3] Pulse Output Timer 3 Event Generation Disable                      */
            __IOM uint32_t ELTDIS4 : 1; /*!< [4..4] Pulse Output Timer 4 Event Generation Disable                      */
            __IOM uint32_t ELTDIS5 : 1; /*!< [5..5] Pulse Output Timer 5 Event Generation Disable                      */
            uint32_t               : 26;
        } ELTSELR_b;
    };

    union
    {
        __IOM uint32_t STCHSELR;       /*!< (@ 0x0000006C) Time Synchronization Channel Select Register               */

        struct
        {
            __IOM uint32_t SYSEL : 1;  /*!< [0..0] Timer Information Input SelectNOTE: Do not change the
                                        *   value of this bit while the SYNSTARTR.STR bit is 1.                       */
            uint32_t : 31;
        } STCHSELR_b;
    };
    __IM uint32_t RESERVED4[4];

    union
    {
        __IOM uint32_t SYNSTARTR;      /*!< (@ 0x00000080) Slave Time Synchronization Start Register                  */

        struct
        {
            __IOM uint32_t STR : 1;    /*!< [0..0] Slave Time Synchronization Control                                 */
            uint32_t           : 31;
        } SYNSTARTR_b;
    };

    union
    {
        __OM uint32_t LCIVLDR;         /*!< (@ 0x00000084) Local Time Counter Initial Value Load Directive
                                        *                  Register                                                   */

        struct
        {
            __OM uint32_t LOAD : 1;    /*!< [0..0] Local Time Counter Initial Value Load Directive                    */
            uint32_t           : 31;
        } LCIVLDR_b;
    };
    __IM uint32_t RESERVED5[2];

    union
    {
        __IOM uint32_t SYNTDARU;          /*!< (@ 0x00000090) Synchronization Loss Detection Threshold Registers         */

        struct
        {
            __IOM uint32_t SYNTDARU : 32; /*!< [31..0] These bits hold the setting for the higher-order 32
                                           *   bits of the threshold for detection of loss of synchronization.           */
        } SYNTDARU_b;
    };

    union
    {
        __IOM uint32_t SYNTDARL;          /*!< (@ 0x00000094) Synchronization Loss Detection Threshold Registers         */

        struct
        {
            __IOM uint32_t SYNTDARL : 32; /*!< [31..0] These bits hold the setting for the lower-order 32 bits
                                           *   of the threshold for detection of loss of synchronization.                */
        } SYNTDARL_b;
    };

    union
    {
        __IOM uint32_t SYNTDBRU;          /*!< (@ 0x00000098) Synchronization Detection Threshold Registers              */

        struct
        {
            __IOM uint32_t SYNTDBRU : 32; /*!< [31..0] These bits hold the setting for the higher-order 32
                                           *   bits of the threshold for detection of synchronization.                   */
        } SYNTDBRU_b;
    };

    union
    {
        __IOM uint32_t SYNTDBRL;          /*!< (@ 0x0000009C) Synchronization Detection Threshold Registers              */

        struct
        {
            __IOM uint32_t SYNTDBRL : 32; /*!< [31..0] These bits hold the setting for the lower-order 32 bits
                                           *   of the threshold for detection of synchronization.                        */
        } SYNTDBRL_b;
    };
    __IM uint32_t RESERVED6[4];

    union
    {
        __IOM uint32_t LCIVRU;          /*!< (@ 0x000000B0) Local Time Counter Initial Value Registers                 */

        struct
        {
            __IOM uint32_t LCIVRU : 16; /*!< [15..0] These bits hold the setting for the higher-order 16
                                         *   bits of the integer portion of the initial value for the
                                         *   local timer counter.                                                      */
            uint32_t : 16;
        } LCIVRU_b;
    };

    union
    {
        __IOM uint32_t LCIVRM;          /*!< (@ 0x000000B4) Local Time Counter Initial Value Registers                 */

        struct
        {
            __IOM uint32_t LCIVRM : 32; /*!< [31..0] These bits hold the setting for the lower-order 32 bits
                                         *   of the integer portion of the initial value for the local
                                         *   timer counter.                                                            */
        } LCIVRM_b;
    };

    union
    {
        __IOM uint32_t LCIVRL;          /*!< (@ 0x000000B8) Local Time Counter Initial Value Registers                 */

        struct
        {
            __IOM uint32_t LCIVRL : 32; /*!< [31..0] These bits hold the setting for the fractional portion
                                         *   of the initial value of the local timer counter in nanoseconds.           */
        } LCIVRL_b;
    };
    __IM uint32_t RESERVED7[26];

    union
    {
        __IOM uint32_t GETW10R;        /*!< (@ 0x00000124) Worst 10 Acquisition Directive Register                    */

        struct
        {
            __IOM uint32_t GW10 : 1;   /*!< [0..0] Worst 10 Acquisition Directive                                     */
            uint32_t            : 31;
        } GETW10R_b;
    };

    union
    {
        __IOM uint32_t PLIMITRU;          /*!< (@ 0x00000128) Positive Gradient Limit Registers                          */

        struct
        {
            __IOM uint32_t PLIMITRU : 31; /*!< [30..0] These bits hold the setting for the higher-order 31
                                           *   bits of the limit for the positive gradient.                              */
            uint32_t : 1;
        } PLIMITRU_b;
    };

    union
    {
        __IOM uint32_t PLIMITRM;          /*!< (@ 0x0000012C) Positive Gradient Limit Registers                          */

        struct
        {
            __IOM uint32_t PLIMITRM : 32; /*!< [31..0] These bits hold the setting for the middle-order 32
                                           *   bits of the limit for the positive gradient.                              */
        } PLIMITRM_b;
    };

    union
    {
        __IOM uint32_t PLIMITRL;          /*!< (@ 0x00000130) Positive Gradient Limit Registers                          */

        struct
        {
            __IOM uint32_t PLIMITRL : 32; /*!< [31..0] These bits hold the setting for the lower-order 32 bits
                                           *   of the limit for the positive gradient.                                   */
        } PLIMITRL_b;
    };

    union
    {
        __IOM uint32_t MLIMITRU;          /*!< (@ 0x00000134) Negative Gradient Limit Registers                          */

        struct
        {
            __IOM uint32_t MLIMITRU : 31; /*!< [30..0] These bits hold the setting for the higher-order 31
                                           *   bits of the limit for the negative gradient.                              */
            uint32_t : 1;
        } MLIMITRU_b;
    };

    union
    {
        __IOM uint32_t MLIMITRM;          /*!< (@ 0x00000138) Negative Gradient Limit Registers                          */

        struct
        {
            __IOM uint32_t MLIMITRM : 32; /*!< [31..0] These bits hold the setting for the middle-order 32
                                           *   bits of the limit for the negative gradient.                              */
        } MLIMITRM_b;
    };

    union
    {
        __IOM uint32_t MLIMITRL;          /*!< (@ 0x0000013C) Negative Gradient Limit Registers                          */

        struct
        {
            __IOM uint32_t MLIMITRL : 32; /*!< [31..0] These bits hold the setting for the lower-order 32 bits
                                           *   of the limit for the negative gradient.                                   */
        } MLIMITRL_b;
    };

    union
    {
        __IOM uint32_t GETINFOR;       /*!< (@ 0x00000140) Statistical Information Retention Control Register         */

        struct
        {
            __IOM uint32_t INFO : 1;   /*!< [0..0] Information Retention ControlNOTE: Once information fetching
                                        *   is directed, values of various statistical information
                                        *   read before completion of information fetching are not
                                        *   guaranteed.                                                               */
            uint32_t : 31;
        } GETINFOR_b;
    };
    __IM uint32_t RESERVED8[11];

    union
    {
        __IM uint32_t LCCVRU;          /*!< (@ 0x00000170) Local Time Counters                                        */

        struct
        {
            __IM uint32_t LCCVRU : 16; /*!< [15..0] These bits are for reading the higher-order 16 bits
                                        *   of the integer portion of the local timer counter's value.                */
            uint32_t : 16;
        } LCCVRU_b;
    };

    union
    {
        __IM uint32_t LCCVRM;          /*!< (@ 0x00000174) Local Time Counters                                        */

        struct
        {
            __IM uint32_t LCCVRM : 32; /*!< [31..0] These bits are for reading the lower-order 32 bits of
                                        *   the integer portion of the local timer counter's value.                   */
        } LCCVRM_b;
    };

    union
    {
        __IM uint32_t LCCVRL;          /*!< (@ 0x00000178) Local Time Counters                                        */

        struct
        {
            __IM uint32_t LCCVRL : 32; /*!< [31..0] These bits are for reading the fractional portion of
                                        *   the local timer counter's value (in nanoseconds).                         */
        } LCCVRL_b;
    };
    __IM uint32_t RESERVED9[37];

    union
    {
        __IM uint32_t PW10VRU;          /*!< (@ 0x00000210) Positive Gradient Worst 10 Value Registers                 */

        struct
        {
            __IM uint32_t PW10VRU : 32; /*!< [31..0] These bits are for reading the higher-order 32 bits
                                         *   of the positive gradient value.                                           */
        } PW10VRU_b;
    };

    union
    {
        __IM uint32_t PW10VRM;          /*!< (@ 0x00000214) Positive Gradient Worst 10 Value Registers                 */

        struct
        {
            __IM uint32_t PW10VRM : 32; /*!< [31..0] These bits are for reading the middle-order 32 bits
                                         *   of the positive gradient value.                                           */
        } PW10VRM_b;
    };

    union
    {
        __IM uint32_t PW10VRL;          /*!< (@ 0x00000218) Positive Gradient Worst 10 Value Registers                 */

        struct
        {
            __IM uint32_t PW10VRL : 32; /*!< [31..0] These bits are for reading the lower-order 32 bits of
                                         *   the positive gradient value.                                              */
        } PW10VRL_b;
    };
    __IM uint32_t RESERVED10[45];

    union
    {
        __IM uint32_t MW10RU;          /*!< (@ 0x000002D0) Negative Gradient Worst 10 Value Registers                 */

        struct
        {
            __IM uint32_t MW10RU : 32; /*!< [31..0] These bits are for reading the higher-order 32 bits
                                        *   of the negative gradient value.                                           */
        } MW10RU_b;
    };

    union
    {
        __IM uint32_t MW10RM;          /*!< (@ 0x000002D4) Negative Gradient Worst 10 Value Registers                 */

        struct
        {
            __IM uint32_t MW10RM : 32; /*!< [31..0] These bits are for reading the middle-order 32 bits
                                        *   of the negative gradient value.                                           */
        } MW10RM_b;
    };

    union
    {
        __IM uint32_t MW10RL;                  /*!< (@ 0x000002D8) Negative Gradient Worst 10 Value Registers                 */

        struct
        {
            __IM uint32_t MW10RL : 32;         /*!< [31..0] These bits are for reading the lower-order 32 bits of
                                                *   the negative gradient value.                                              */
        } MW10RL_b;
    };
    __IM uint32_t RESERVED11[9];
    __IOM R_ETHERC_EPTPC_COMMON_TM_Type TM[6]; /*!< (@ 0x00000300) Timer Setting Registers                                    */
    __IM uint32_t RESERVED12[7];

    union
    {
        __IOM uint32_t TMSTARTR;       /*!< (@ 0x0000037C) Timer Start Register                                       */

        struct
        {
            __IOM uint32_t EN0 : 1;    /*!< [0..0] Pulse Output Timer 0 Start                                         */
            __IOM uint32_t EN1 : 1;    /*!< [1..1] Pulse Output Timer 1 Start                                         */
            __IOM uint32_t EN2 : 1;    /*!< [2..2] Pulse Output Timer 2 Start                                         */
            __IOM uint32_t EN3 : 1;    /*!< [3..3] Pulse Output Timer 3 Start                                         */
            __IOM uint32_t EN4 : 1;    /*!< [4..4] Pulse Output Timer 4 Start                                         */
            __IOM uint32_t EN5 : 1;    /*!< [5..5] Pulse Output Timer 5 Start                                         */
            uint32_t           : 26;
        } TMSTARTR_b;
    };
    __IM uint32_t RESERVED13[32];

    union
    {
        __IOM uint32_t PRSR;           /*!< (@ 0x00000400) PRC-TC Status Register                                     */

        struct
        {
            __IOM uint32_t OVRE0 : 1;  /*!< [0..0] Relay Packet Overflow Detection Flag 0                             */
            __IOM uint32_t OVRE1 : 1;  /*!< [1..1] Relay Packet Overflow Detection Flag 1                             */
            __IOM uint32_t OVRE2 : 1;  /*!< [2..2] Relay Packet Overflow Detection Flag 2                             */
            __IOM uint32_t OVRE3 : 1;  /*!< [3..3] Relay Packet Overflow Detection Flag 3                             */
            uint32_t             : 4;
            __IOM uint32_t MACE  : 1;  /*!< [8..8] Originating MAC Address Mismatch Detection Flag                    */
            uint32_t             : 19;
            __IOM uint32_t URE0  : 1;  /*!< [28..28] Relay Packet Underflow Detection Flag 0                          */
            __IOM uint32_t URE1  : 1;  /*!< [29..29] Relay Packet Underflow Detection Flag 1                          */
            uint32_t             : 2;
        } PRSR_b;
    };

    union
    {
        __IOM uint32_t PRIPR;          /*!< (@ 0x00000404) PRC-TC Status Notification Permission Register             */

        struct
        {
            __IOM uint32_t OVRE0 : 1;  /*!< [0..0] PRSR.OVRE0 Status Notification Permission                          */
            __IOM uint32_t OVRE1 : 1;  /*!< [1..1] PRSR.OVRE1 Status Notification Permission                          */
            __IOM uint32_t OVRE2 : 1;  /*!< [2..2] PRSR.OVRE2 Status Notification Permission                          */
            __IOM uint32_t OVRE3 : 1;  /*!< [3..3] PRSR.OVRE3 Status Notification Permission                          */
            uint32_t             : 4;
            __IOM uint32_t MACE  : 1;  /*!< [8..8] PRSR.MACE Status Notification Permission                           */
            uint32_t             : 19;
            __IOM uint32_t URE0  : 1;  /*!< [28..28] PRSR.URE0 Status Notification Permission                         */
            __IOM uint32_t URE1  : 1;  /*!< [29..29] PRSR.URE1 Status Notification Permission                         */
            uint32_t             : 2;
        } PRIPR_b;
    };
    __IM uint32_t RESERVED14[2];
    __IOM R_ETHERC_EPTPC_COMMON_PR_Type PR[2]; /*!< (@ 0x00000410) Local MAC Address Registers                                */

    union
    {
        __IOM uint32_t TRNDISR;                /*!< (@ 0x00000420) Packet Transmission Control Register                       */

        struct
        {
            __IOM uint32_t TDIS : 2;           /*!< [1..0] Packet Transmission Control                                        */
            uint32_t            : 30;
        } TRNDISR_b;
    };
    __IM uint32_t RESERVED15[3];

    union
    {
        __IOM uint32_t TRNMR;          /*!< (@ 0x00000430) Relay Mode Register                                        */

        struct
        {
            __IOM uint32_t MOD  : 1;   /*!< [0..0] Cut-Through Mode                                                   */
            uint32_t            : 7;
            __IOM uint32_t FWD0 : 1;   /*!< [8..8] Channel 0 Relay Enable                                             */
            __IOM uint32_t FWD1 : 1;   /*!< [9..9] Channel 1 Relay Enable                                             */
            uint32_t            : 22;
        } TRNMR_b;
    };

    union
    {
        __IOM uint32_t TRNCTTDR;       /*!< (@ 0x00000434) Cut-Through Transfer Start Threshold Register              */

        struct
        {
            __IOM uint32_t THVAL : 11; /*!< [10..0] FIFO Read Start ThresholdThreshold for starting to read
                                        *   data from the relay FIFO in cut-through mode (specified
                                        *   as the number of bytes)NOTE1: A value cannot be set in
                                        *   the lower-order 2 bits. These bits are fixed to 0.NOTE2:
                                        *   A value of less than 96 bytes cannot be set.                              */
            uint32_t : 21;
        } TRNCTTDR_b;
    };
} R_ETHERC_EPTPC_COMMON_Type;          /*!< Size = 1080 (0x438)                                                       */

#endif /* R_ETHER_REG_H */
