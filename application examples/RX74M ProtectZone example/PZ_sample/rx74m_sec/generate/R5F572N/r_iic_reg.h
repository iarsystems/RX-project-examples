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

#ifndef R_IIC_REG_H
#define R_IIC_REG_H

/* ================                              Device Specific Cluster Section                              ================ */

/** @addtogroup Device_Peripheral_clusters
 * @{
 */

/**
 * @brief R_IIC0_SAR [SAR] (Slave Address Registers)
 */
typedef struct
{
    union
    {
        __IOM uint8_t L;               /*!< (@ 0x00000000) Slave Address Register L                                   */

        struct
        {
            __IOM uint8_t SVA0 : 1;    /*!< [0..0] 10-Bit Address LSB                                                 */
            __IOM uint8_t SVA  : 7;    /*!< [7..1] 7-Bit Address/10-Bit Address Lower Bits                            */
        } L_b;
    };

    union
    {
        __IOM uint8_t U;               /*!< (@ 0x00000001) Slave Address Register U                                   */

        struct
        {
            __IOM uint8_t FS  : 1;     /*!< [0..0] 7-Bit/10-Bit Address Format Selection                              */
            __IOM uint8_t SVA : 2;     /*!< [2..1] 10-Bit Address Upper Bits                                          */
            uint8_t           : 5;
        } U_b;
    };
} R_IIC0_SAR_Type;                     /*!< Size = 2 (0x2)                                                            */

/** @} */ /* End of group Device_Peripheral_clusters */

/* =========================================================================================================================== */
/* ================                            Device Specific Peripheral Section                             ================ */
/* =========================================================================================================================== */

/** @addtogroup Device_Peripheral_peripherals
 * @{
 */

/* =========================================================================================================================== */
/* ================                                          R_IIC0                                           ================ */
/* =========================================================================================================================== */

/**
 * @brief I2C-bus Interface channel 0 (R_IIC0)
 */

typedef struct                         /*!< (@ 0x00088300) R_IIC0 Structure                                           */
{
    union
    {
        __IOM uint8_t ICCR1;           /*!< (@ 0x00000000) I2C Bus Control Register 1                                 */

        struct
        {
            __IM uint8_t  SDAI   : 1;  /*!< [0..0] SDA Line Monitor                                                   */
            __IM uint8_t  SCLI   : 1;  /*!< [1..1] SCL Line Monitor                                                   */
            __IOM uint8_t SDAO   : 1;  /*!< [2..2] SDA Output Control/Monitor                                         */
            __IOM uint8_t SCLO   : 1;  /*!< [3..3] SCL Output Control/Monitor                                         */
            __IOM uint8_t SOWP   : 1;  /*!< [4..4] SCLO/SDAO Write Protect                                            */
            __IOM uint8_t CLO    : 1;  /*!< [5..5] Extra SCL Clock Cycle Output                                       */
            __IOM uint8_t IICRST : 1;  /*!< [6..6] I2C Bus Interface Internal ResetNote:If an internal reset
                                        *   is initiated using the IICRST bit for a bus hang-up occurred
                                        *   during communication with the master device in slave mode,
                                        *   the states may become different between the slave device
                                        *   and the master device (due to the difference in the bit
                                        *   counter information).                                                     */
            __IOM uint8_t ICE : 1;     /*!< [7..7] I2C Bus Interface Enable                                           */
        } ICCR1_b;
    };

    union
    {
        __IOM uint8_t ICCR2;           /*!< (@ 0x00000001) I2C Bus Control Register 2                                 */

        struct
        {
            uint8_t          : 1;
            __IOM uint8_t ST : 1;      /*!< [1..1] Start Condition Issuance RequestSet the ST bit to 1 (start
                                        *   condition issuance request) when the BBSY flag is set to
                                        *   0 (bus free state).                                                       */
            __IOM uint8_t RS : 1;      /*!< [2..2] Restart Condition Issuance RequestNote: Do not set the
                                        *   RS bit to 1 while issuing a stop condition.                               */
            __IOM uint8_t SP : 1;      /*!< [3..3] Stop Condition Issuance RequestNote: Writing to the SP
                                        *   bit is not possible while the setting of the BBSY flag
                                        *   is 0 (bus free state).Note: Do not set the SP bit to 1
                                        *   while a restart condition is being issued.                                */
            uint8_t            : 1;
            __IOM uint8_t TRS  : 1;    /*!< [5..5] Transmit/Receive Mode                                              */
            __IOM uint8_t MST  : 1;    /*!< [6..6] Master/Slave Mode                                                  */
            __IM uint8_t  BBSY : 1;    /*!< [7..7] Bus Busy Detection Flag                                            */
        } ICCR2_b;
    };

    union
    {
        __IOM uint8_t ICMR1;           /*!< (@ 0x00000002) I2C Bus Mode Register 1                                    */

        struct
        {
            __IOM uint8_t BC   : 3;    /*!< [2..0] Bit Counter                                                        */
            __OM uint8_t  BCWP : 1;    /*!< [3..3] BC Write Protect(This bit is read as 1.)                           */
            __IOM uint8_t CKS  : 3;    /*!< [6..4] Internal Reference Clock (fIIC) Selection ( fIIC = PCLKB
                                        *   / 2^CKS )                                                                 */
            __IOM uint8_t MTWP : 1;    /*!< [7..7] MST/TRS Write Protect                                              */
        } ICMR1_b;
    };

    union
    {
        __IOM uint8_t ICMR2;           /*!< (@ 0x00000003) I2C Bus Mode Register 2                                    */

        struct
        {
            __IOM uint8_t TMOS : 1;    /*!< [0..0] Timeout Detection Time Select                                      */
            __IOM uint8_t TMOL : 1;    /*!< [1..1] Timeout L Count Control                                            */
            __IOM uint8_t TMOH : 1;    /*!< [2..2] Timeout H Count Control                                            */
            uint8_t            : 1;
            __IOM uint8_t SDDL : 3;    /*!< [6..4] SDA Output Delay Counter                                           */
            __IOM uint8_t DLCS : 1;    /*!< [7..7] SDA Output Delay Clock Source Select                               */
        } ICMR2_b;
    };

    union
    {
        __IOM uint8_t ICMR3;           /*!< (@ 0x00000004) I2C Bus Mode Register 3                                    */

        struct
        {
            __IOM uint8_t NF    : 2;   /*!< [1..0] Noise Filter Stage Selection                                       */
            __IM uint8_t  ACKBR : 1;   /*!< [2..2] Receive Acknowledge                                                */
            __IOM uint8_t ACKBT : 1;   /*!< [3..3] Transmit Acknowledge                                               */
            __IOM uint8_t ACKWP : 1;   /*!< [4..4] ACKBT Write Protect                                                */
            __IOM uint8_t RDRFS : 1;   /*!< [5..5] RDRF Flag Set Timing Selection                                     */
            __IOM uint8_t WAIT  : 1;   /*!< [6..6] WAITNote: When the value of the WAIT bit is to be read,
                                        *   be sure to read the ICDRR beforehand.                                     */
            __IOM uint8_t SMBS : 1;    /*!< [7..7] SMBus/I2C Bus Selection                                            */
        } ICMR3_b;
    };

    union
    {
        __IOM uint8_t ICFER;           /*!< (@ 0x00000005) I2C Bus Function Enable Register                           */

        struct
        {
            __IOM uint8_t TMOE  : 1;   /*!< [0..0] Timeout Function Enable                                            */
            __IOM uint8_t MALE  : 1;   /*!< [1..1] Master Arbitration-Lost Detection Enable                           */
            __IOM uint8_t NALE  : 1;   /*!< [2..2] NACK Transmission Arbitration-Lost Detection Enable                */
            __IOM uint8_t SALE  : 1;   /*!< [3..3] Slave Arbitration-Lost Detection Enable                            */
            __IOM uint8_t NACKE : 1;   /*!< [4..4] NACK Reception Transfer Suspension Enable                          */
            __IOM uint8_t NFE   : 1;   /*!< [5..5] Digital Noise Filter Circuit Enable                                */
            __IOM uint8_t SCLE  : 1;   /*!< [6..6] SCL Synchronous Circuit Enable                                     */
            __IOM uint8_t FMPE  : 1;   /*!< [7..7] Fast-mode Plus Enable                                              */
        } ICFER_b;
    };

    union
    {
        __IOM uint8_t ICSER;           /*!< (@ 0x00000006) I2C Bus Status Enable Register                             */

        struct
        {
            __IOM uint8_t SAR0E : 1;   /*!< [0..0] Slave Address Register 0 Enable                                    */
            __IOM uint8_t SAR1E : 1;   /*!< [1..1] Slave Address Register 1 Enable                                    */
            __IOM uint8_t SAR2E : 1;   /*!< [2..2] Slave Address Register 2 Enable                                    */
            __IOM uint8_t GCAE  : 1;   /*!< [3..3] General Call Address Enable                                        */
            uint8_t             : 1;
            __IOM uint8_t DIDE  : 1;   /*!< [5..5] Device-ID Address Detection Enable                                 */
            uint8_t             : 1;
            __IOM uint8_t HOAE  : 1;   /*!< [7..7] Host Address Enable                                                */
        } ICSER_b;
    };

    union
    {
        __IOM uint8_t ICIER;           /*!< (@ 0x00000007) I2C Bus Interrupt Enable Register                          */

        struct
        {
            __IOM uint8_t TMOIE : 1;   /*!< [0..0] Timeout Interrupt Request Enable                                   */
            __IOM uint8_t ALIE  : 1;   /*!< [1..1] Arbitration-Lost Interrupt Request Enable                          */
            __IOM uint8_t STIE  : 1;   /*!< [2..2] Start Condition Detection Interrupt Request Enable                 */
            __IOM uint8_t SPIE  : 1;   /*!< [3..3] Stop Condition Detection Interrupt Request Enable                  */
            __IOM uint8_t NAKIE : 1;   /*!< [4..4] NACK Reception Interrupt Request Enable                            */
            __IOM uint8_t RIE   : 1;   /*!< [5..5] Receive Data Full Interrupt Request Enable                         */
            __IOM uint8_t TEIE  : 1;   /*!< [6..6] Transmit End Interrupt Request Enable                              */
            __IOM uint8_t TIE   : 1;   /*!< [7..7] Transmit Data Empty Interrupt Request Enable                       */
        } ICIER_b;
    };

    union
    {
        __IOM uint8_t ICSR1;           /*!< (@ 0x00000008) I2C Bus Status Register 1                                  */

        struct
        {
            __IOM uint8_t AAS0 : 1;    /*!< [0..0] Slave Address 0 Detection Flag                                     */
            __IOM uint8_t AAS1 : 1;    /*!< [1..1] Slave Address 1 Detection Flag                                     */
            __IOM uint8_t AAS2 : 1;    /*!< [2..2] Slave Address 2 Detection Flag                                     */
            __IOM uint8_t GCA  : 1;    /*!< [3..3] General Call Address Detection Flag                                */
            uint8_t            : 1;
            __IOM uint8_t DID  : 1;    /*!< [5..5] Device-ID Address Detection Flag                                   */
            uint8_t            : 1;
            __IOM uint8_t HOA  : 1;    /*!< [7..7] Host Address Detection Flag                                        */
        } ICSR1_b;
    };

    union
    {
        __IOM uint8_t ICSR2;           /*!< (@ 0x00000009) I2C Bus Status Register 2                                  */

        struct
        {
            __IOM uint8_t TMOF  : 1;   /*!< [0..0] Timeout Detection Flag                                             */
            __IOM uint8_t AL    : 1;   /*!< [1..1] Arbitration-Lost Flag                                              */
            __IOM uint8_t START : 1;   /*!< [2..2] Start Condition Detection Flag                                     */
            __IOM uint8_t STOP  : 1;   /*!< [3..3] Stop Condition Detection Flag                                      */
            __IOM uint8_t NACKF : 1;   /*!< [4..4] NACK Detection Flag                                                */
            __IOM uint8_t RDRF  : 1;   /*!< [5..5] Receive Data Full Flag                                             */
            __IOM uint8_t TEND  : 1;   /*!< [6..6] Transmit End Flag                                                  */
            __IM uint8_t  TDRE  : 1;   /*!< [7..7] Transmit Data Empty Flag                                           */
        } ICSR2_b;
    };
    __IOM R_IIC0_SAR_Type SAR[3];      /*!< (@ 0x0000000A) Slave Address Registers                                    */

    union
    {
        __IOM uint8_t ICBRL;           /*!< (@ 0x00000010) I2C Bus Bit Rate Low-Level Register                        */

        struct
        {
            __IOM uint8_t BRL : 5;     /*!< [4..0] Bit Rate Low-Level Period(Low-level period of SCL clock)           */
            uint8_t           : 3;
        } ICBRL_b;
    };

    union
    {
        __IOM uint8_t ICBRH;           /*!< (@ 0x00000011) I2C Bus Bit Rate High-Level Register                       */

        struct
        {
            __IOM uint8_t BRH : 5;     /*!< [4..0] Bit Rate High-Level Period(High-level period of SCL clock)         */
            uint8_t           : 3;
        } ICBRH_b;
    };

    union
    {
        __IOM uint8_t ICDRT;           /*!< (@ 0x00000012) I2C Bus Transmit Data Register                             */

        struct
        {
            __IOM uint8_t ICDRT : 8;   /*!< [7..0] 8-bit read-write register that stores transmit data.               */
        } ICDRT_b;
    };

    union
    {
        __IM uint8_t ICDRR;            /*!< (@ 0x00000013) I2C Bus Receive Data Register                              */

        struct
        {
            __IM uint8_t ICDRR : 8;    /*!< [7..0] 8-bit register that stores the received data                       */
        } ICDRR_b;
    };
} R_IIC0_Type;                         /*!< Size = 20 (0x14)                                                          */

/** @} */ /* End of group Device_Peripheral_peripherals */

#endif /* R_IIC_REG_H */
