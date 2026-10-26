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

#ifndef R_GLCDC_REG_H
#define R_GLCDC_REG_H

/* =========================================================================================================================== */
/* ================                                           GLCDC                                            ================ */
/* =========================================================================================================================== */

/**
 * @brief R_GLCDC_BG [BG] (Background Registers)
 */
typedef struct
{
    union
    {
        __IOM uint32_t EN;             /*!< (@ 0x00000000) Background Plane Setting Operation Control Register        */

        struct
        {
            __IOM uint32_t EN  : 1;    /*!< [0..0] Background plane generation module operation enable                */
            uint32_t           : 7;
            __IOM uint32_t VEN : 1;    /*!< [8..8] Control of LCDC internal register value reflection to
                                        *   internal operations                                                       */
            uint32_t             : 7;
            __IOM uint32_t SWRST : 1;  /*!< [16..16] Entire module SW reset control                                   */
            uint32_t             : 15;
        } EN_b;
    };

    union
    {
        __IOM uint32_t PERI;           /*!< (@ 0x00000004) Background Plane Setting Free-Running Period
                                        *                  Register                                                   */

        struct
        {
            __IOM uint32_t FH : 11;    /*!< [10..0] Background plane horizontal synchronization signal period
                                        *   on the basis of pixel clock (PXCLK).                                      */
            uint32_t          : 5;
            __IOM uint32_t FV : 11;    /*!< [26..16] Background plane vertical synchronization signal period
                                        *   on the basis of line.                                                     */
            uint32_t : 5;
        } PERI_b;
    };

    union
    {
        __IOM uint32_t SYNC;           /*!< (@ 0x00000008) Background Plane Setting Synchronization Position
                                        *                  Register                                                   */

        struct
        {
            __IOM uint32_t HP : 4;     /*!< [3..0] Background plane horizontal synchronization signal assertion
                                        *   position on the basis of pixel clock (PXCLK).                             */
            uint32_t          : 12;
            __IOM uint32_t VP : 4;     /*!< [19..16] Background plane vertical synchronization signal assertion
                                        *   position on the basis of line.                                            */
            uint32_t : 12;
        } SYNC_b;
    };

    union
    {
        __IOM uint32_t VSIZE;          /*!< (@ 0x0000000C) Background Plane Setting Full Image Vertical
                                        *                  Size Register                                              */

        struct
        {
            __IOM uint32_t VW : 11;    /*!< [10..0] Background plane vertical valid pixel width on the basis
                                        *   of line                                                                   */
            uint32_t          : 5;
            __IOM uint32_t VP : 11;    /*!< [26..16] Background plane vertical valid pixel start position
                                        *   on the basis of line                                                      */
            uint32_t : 5;
        } VSIZE_b;
    };

    union
    {
        __IOM uint32_t HSIZE;          /*!< (@ 0x00000010) Background Plane Setting Full Image Horizontal
                                        *                  Size Register                                              */

        struct
        {
            __IOM uint32_t HW : 11;    /*!< [10..0] Background plane horizontall valid pixel width on the
                                        *   basis of pixel clock (PXCLK) Note: When serial RGB is selected
                                        *   as the output format for the output control block, add
                                        *   two to the horizontal enable signal width and set the resulting
                                        *   value to this field.                                                      */
            uint32_t          : 5;
            __IOM uint32_t HP : 11;    /*!< [26..16] Background plane horizontal valid pixel start position
                                        *   on the basis of pixel clock (PXCLK).                                      */
            uint32_t : 5;
        } HSIZE_b;
    };

    union
    {
        __IOM uint32_t BGC;            /*!< (@ 0x00000014) Background Plane Setting Background Color Register         */

        struct
        {
            __IOM uint32_t B : 8;      /*!< [7..0] B value for background plane valid pixel area Unsigned;
                                        *   8-bit integer                                                             */
            __IOM uint32_t G : 8;      /*!< [15..8] G value for background plane valid pixel area Unsigned;
                                        *   8-bit integer                                                             */
            __IOM uint32_t R : 8;      /*!< [23..16] R value for background plane valid pixel area. Unsigned;
                                        *   8-bit integer.                                                            */
            uint32_t : 8;
        } BGC_b;
    };

    union
    {
        __IM uint32_t MON;             /*!< (@ 0x00000018) Background Plane Setting Status Monitor Register           */

        struct
        {
            __IM uint32_t EN  : 1;     /*!< [0..0] Background plane generation module operation state monitor.        */
            uint32_t          : 7;
            __IM uint32_t VEN : 1;     /*!< [8..8] Entire module internal operation reflection control signal
                                        *   monitor. The signal state for controlling reflection of
                                        *   the register values to the internal operations upon assertion
                                        *   of the vertical synchronization signal.                                   */
            uint32_t            : 7;
            __IM uint32_t SWRST : 1;   /*!< [16..16] Entire module SW reset state monitor.                            */
            uint32_t            : 15;
        } MON_b;
    };
} R_GLCDC_BG_Type;                     /*!< Size = 28 (0x1c)                                                          */

/**
 * @brief R_GLCDC_GR [GR] (Layer Registers)
 */
typedef struct
{
    union
    {
        __IOM uint32_t VEN;            /*!< (@ 0x00000000) Graphics Register Update Control Register                  */

        struct
        {
            __IOM uint32_t PVEN : 1;   /*!< [0..0] Control of graphics n module register value reflection
                                        *   to internal operations. Reflection of the register values
                                        *   to the internal operation at the assertion of the vertical
                                        *   synchronization signal (VS).                                              */
            uint32_t : 31;
        } VEN_b;
    };

    union
    {
        __IOM uint32_t FLMRD;          /*!< (@ 0x00000004) Graphics Frame Buffer Read Control Register                */

        struct
        {
            __IOM uint32_t RENB : 1;   /*!< [0..0] Graphics data (frame buffer data) read enable.                     */
            uint32_t            : 31;
        } FLMRD_b;
    };

    union
    {
        __IM uint32_t FLM1;            /*!< (@ 0x00000008) Graphics Frame Buffer Control Register 1                   */

        struct
        {
            __IM uint32_t BSTMD : 2;   /*!< [1..0] Burst transfer control for graphics data (frame buffer
                                        *   data) access                                                              */
            uint32_t : 30;
        } FLM1_b;
    };

    union
    {
        __IOM uint32_t FLM2;           /*!< (@ 0x0000000C) Graphics Frame Buffer Control Register 2                   */

        struct
        {
            __IOM uint32_t BASE : 32;  /*!< [31..0] Base address for accessing graphics data (frame buffer
                                        *   data) Set the head address in the frame buffer where graphics
                                        *   data is to be stored. GRn_FLM2.BASE[5:0] should be fixed
                                        *   to 0 during 64-byte burst transfer.                                       */
        } FLM2_b;
    };

    union
    {
        __IOM uint32_t FLM3;           /*!< (@ 0x00000010) Graphics Frame Buffer Control Register 3                   */

        struct
        {
            uint32_t             : 16;
            __IOM uint32_t LNOFF : 16; /*!< [31..16] Macro line offset address for accessing graphics data
                                        *   (frame buffer data) Signed; 16-bit integer                                */
        } FLM3_b;
    };
    __IM uint32_t RESERVED;

    union
    {
        __IOM uint32_t FLM5;             /*!< (@ 0x00000018) Graphics Frame Buffer Control Register 5                   */

        struct
        {
            __IOM uint32_t DATANUM : 16; /*!< [15..0] Number of data transfer times per line for accessing
                                          *   graphics data (frame buffer data), where one transfer is
                                          *   defined as 16-beat burst access (64-byte boundary)                        */
            __IOM uint32_t LNNUM : 11;   /*!< [26..16] Number of lines per frame for accessing graphics data
                                          *   (frame buffer data).                                                      */
            uint32_t : 5;
        } FLM5_b;
    };

    union
    {
        __IOM uint32_t FLM6;           /*!< (@ 0x0000001C) Graphics Frame Buffer Control Register 6                   */

        struct
        {
            uint32_t              : 28;
            __IOM uint32_t FORMAT : 3; /*!< [30..28] Data format for accessing graphics data (frame buffer
                                        *   data).                                                                    */
            uint32_t : 1;
        } FLM6_b;
    };

    union
    {
        __IOM uint32_t AB1;               /*!< (@ 0x00000020) Graphics Alpha Blending Control Register 1                 */

        struct
        {
            __IOM uint32_t DISPSEL   : 2; /*!< [1..0] Graphics display plane control.                                    */
            uint32_t                 : 2;
            __IOM uint32_t GRCDISPON : 1; /*!< [4..4] Graphics image area border display control.                        */
            uint32_t                 : 3;
            __IOM uint32_t ARCDISPON : 1; /*!< [8..8] Image area border display control for rectangular area
                                           *   alpha blending.                                                           */
            uint32_t             : 3;
            __IOM uint32_t ARCON : 1;     /*!< [12..12] Rectangular area alpha blending control.                         */
            uint32_t             : 19;
        } AB1_b;
    };

    union
    {
        __IOM uint32_t AB2;            /*!< (@ 0x00000024) Graphics Alpha Blending Control Register 2                 */

        struct
        {
            __IOM uint32_t GRCVW : 11; /*!< [10..0] Vertical width of graphics image area.                            */
            uint32_t             : 5;
            __IOM uint32_t GRCVS : 11; /*!< [26..16] Vertical start position of graphics image area.                  */
            uint32_t             : 5;
        } AB2_b;
    };

    union
    {
        __IOM uint32_t AB3;            /*!< (@ 0x00000028) Graphics Alpha Blending Control Register 3                 */

        struct
        {
            __IOM uint32_t GRCHW : 11; /*!< [10..0] Horizontal width of graphics image area.                          */
            uint32_t             : 5;
            __IOM uint32_t GRCHS : 11; /*!< [26..16] Horizontal start position of graphics image area.                */
            uint32_t             : 5;
        } AB3_b;
    };

    union
    {
        __IOM uint32_t AB4;            /*!< (@ 0x0000002C) Graphics Alpha Blending Control Register 4                 */

        struct
        {
            __IOM uint32_t ARCVW : 11; /*!< [10..0] Vertical width of rectangular area alpha blending image
                                        *   area.                                                                     */
            uint32_t             : 5;
            __IOM uint32_t ARCVS : 11; /*!< [26..16] Vertical start position of rectangular area alpha blending
                                        *   image area                                                                */
            uint32_t : 5;
        } AB4_b;
    };

    union
    {
        __IOM uint32_t AB5;            /*!< (@ 0x00000030) Graphics Alpha Blending Control Register 5                 */

        struct
        {
            __IOM uint32_t ARCHW : 11; /*!< [10..0] Horizontal width of rectangular area alpha blending
                                        *   image area.                                                               */
            uint32_t             : 5;
            __IOM uint32_t ARCHS : 11; /*!< [26..16] Horizontal start position of rectangular area alpha
                                        *   blending image area.                                                      */
            uint32_t : 5;
        } AB5_b;
    };

    union
    {
        __IOM uint32_t AB6;             /*!< (@ 0x00000034) Graphics Alpha Blending Control Register 6                 */

        struct
        {
            __IOM uint32_t ARCRATE : 8; /*!< [7..0] Frame rate for alpha blending in rectangular area.                 */
            uint32_t               : 8;
            __IOM uint32_t ARCCOEF : 9; /*!< [24..16] Alpha coefficient for alpha blending in rectangular
                                         *   area (-255 to 255). [8]: Sign (0: addition, 1: subtraction)
                                         *   [7:0]: Variation (absolute value)                                         */
            uint32_t : 7;
        } AB6_b;
    };

    union
    {
        __IOM uint32_t AB7;            /*!< (@ 0x00000038) Graphics Alpha Blending Control Register 7                 */

        struct
        {
            __IOM uint32_t CKON   : 1; /*!< [0..0] RGB-index chroma-key processing control.                           */
            uint32_t              : 15;
            __IOM uint32_t ARCDEF : 8; /*!< [23..16] Initial alpha value for alpha blending in rectangular
                                        *   area.                                                                     */
            uint32_t : 8;
        } AB7_b;
    };

    union
    {
        __IOM uint32_t AB8;            /*!< (@ 0x0000003C) Graphics Alpha Blending Control Register 8                 */

        struct
        {
            __IOM uint32_t CKKR : 8;   /*!< [7..0] R signal for RGB-index chroma-key processing Unsigned;
                                        *   8 bits.                                                                   */
            __IOM uint32_t CKKB : 8;   /*!< [15..8] B signal for RGB-index chroma-key processing Unsigned;
                                        *   8 bits.                                                                   */
            __IOM uint32_t CKKG : 8;   /*!< [23..16] G signal for RGB-index chroma-key processing Unsigned;
                                        *   8 bits.                                                                   */
            uint32_t : 8;
        } AB8_b;
    };

    union
    {
        __IOM uint32_t AB9;            /*!< (@ 0x00000040) Graphics Alpha Blending Control Register 9                 */

        struct
        {
            __IOM uint32_t CKR : 8;    /*!< [7..0] R value after RGB-index chroma-key processing replacement
                                        *   Unsigned; 8 bits.                                                         */
            __IOM uint32_t CKB : 8;    /*!< [15..8] B value after RGB-index chroma-key processing replacement
                                        *   Unsigned; 8 bits.                                                         */
            __IOM uint32_t CKG : 8;    /*!< [23..16] G value after RGB-index chroma-key processing replacement
                                        *   Unsigned; 8 bits.                                                         */
            __IOM uint32_t CKA : 8;    /*!< [31..24] A value after RGB-index chroma-key processing replacement.       */
        } AB9_b;
    };
    __IM uint32_t RESERVED1[2];

    union
    {
        __IOM uint32_t BASE;           /*!< (@ 0x0000004C) Graphics Background Color Control Register                 */

        struct
        {
            __IOM uint32_t R : 8;      /*!< [7..0] Background color R value Unsigned; 8 bits                          */
            __IOM uint32_t B : 8;      /*!< [15..8] Background color B value Unsigned; 8 bits                         */
            __IOM uint32_t G : 8;      /*!< [23..16] Background color G value Unsigned; 8 bits                        */
            uint32_t         : 8;
        } BASE_b;
    };

    union
    {
        __IOM uint32_t CLUTINT;        /*!< (@ 0x00000050) Graphics CLUT Table Interrupt Control Register             */

        struct
        {
            __IOM uint32_t LINE : 11;  /*!< [10..0] Number of detection lines                                         */
            uint32_t            : 5;
            __IOM uint32_t SEL  : 1;   /*!< [16..16] CLUT table control                                               */
            uint32_t            : 15;
        } CLUTINT_b;
    };

    union
    {
        __IM uint32_t MON;             /*!< (@ 0x00000054) Graphics Status Monitor Register                           */

        struct
        {
            __IM uint32_t ARCST   : 1; /*!< [0..0] Status monitor for alpha blending in rectangular area              */
            uint32_t              : 15;
            __IM uint32_t UNDFLST : 1; /*!< [16..16] Status monitor for underflow                                     */
            uint32_t              : 15;
        } MON_b;
    };
    __IM uint32_t RESERVED2[42];
} R_GLCDC_GR_Type;                     /*!< Size = 256 (0x100)                                                        */

/**
 * @brief R_GLCDC_GAM [GAM] (Gamma Settings)
 */
typedef struct
{
    union
    {
        __IOM uint32_t LATCH;          /*!< (@ 0x00000000) Gamma Register Update Control Register                     */

        struct
        {
            __IOM uint32_t VEN : 1;    /*!< [0..0] Control of gamma correction x module register value reflection
                                        *   to internal operations. The register values to be reflected
                                        *   to the internal operations at the assertion of the vertical
                                        *   synchronization signal (VS).                                              */
            uint32_t : 31;
        } LATCH_b;
    };

    union
    {
        __IOM uint32_t GAM_SW;         /*!< (@ 0x00000004) Gamma Correction Block Function Switch Register            */

        struct
        {
            __IOM uint32_t GAMON : 1;  /*!< [0..0] Gamma correction on/off control                                    */
            uint32_t             : 31;
        } GAM_SW_b;
    };

    union
    {
        __IOM uint32_t LUT[8];         /*!< (@ 0x00000008) Gamma Correction Block Table Setting Register              */

        struct
        {
            __IOM uint32_t _HIGH : 11; /*!< [10..0] Gain value of area 0. Unsigned 11-bit fixed point.                */
            uint32_t             : 5;
            __IOM uint32_t _LOW  : 11; /*!< [26..16] Gain value of area 0. Unsigned 11-bit fixed point.               */
            uint32_t             : 5;
        } LUT_b[8];
    };

    union
    {
        __IOM uint32_t AREA[5];        /*!< (@ 0x00000028) Gamma Correction Block Area Setting Register               */

        struct
        {
            __IOM uint32_t _HIGH : 10; /*!< [9..0] Start threshold of area 1 Unsigned 10-bit integer                  */
            __IOM uint32_t _MID  : 10; /*!< [19..10] Start threshold of area 1 Unsigned 10-bit integer                */
            __IOM uint32_t _LOW  : 10; /*!< [29..20] Start threshold of area 1 Unsigned 10-bit integer                */
            uint32_t             : 2;
        } AREA_b[5];
    };
    __IM uint32_t RESERVED;
} R_GLCDC_GAM_Type;                    /*!< Size = 64 (0x40)                                                          */

/**
 * @brief R_GLCDC_OUT [OUT] (Output Control Registers)
 */
typedef struct
{
    union
    {
        __IOM uint32_t VLATCH;         /*!< (@ 0x00000000) Output Control Block Register Update Control
                                        *                  Register                                                   */

        struct
        {
            __IOM uint32_t VEN : 1;    /*!< [0..0] Control of output control module register value reflection
                                        *   to internal operations. The register values to be reflected
                                        *   to the internal operations at the assertion of the vertical
                                        *   synchronization signal (VS).                                              */
            uint32_t : 31;
        } VLATCH_b;
    };

    union
    {
        __IOM uint32_t SET;              /*!< (@ 0x00000004) Output Control Block Output Interface Register             */

        struct
        {
            __IOM uint32_t PHASE    : 2; /*!< [1..0] Data delay in serial RGB format (based on OUTCLK)                  */
            uint32_t                : 2;
            __IOM uint32_t DIRSEL   : 1; /*!< [4..4] Invalid data position control in serial RGB format                 */
            uint32_t                : 3;
            __IOM uint32_t FRQSEL   : 2; /*!< [9..8] Clock frequency division control                                   */
            uint32_t                : 2;
            __IOM uint32_t FORMAT   : 2; /*!< [13..12] Output format select                                             */
            uint32_t                : 10;
            __IOM uint32_t SWAPON   : 1; /*!< [24..24] Pixel order control                                              */
            uint32_t                : 3;
            __IOM uint32_t ENDIANON : 1; /*!< [28..28] Bit endian change control                                        */
            uint32_t                : 3;
        } SET_b;
    };

    union
    {
        __IOM uint32_t BRIGHT1;        /*!< (@ 0x00000008) Output Control Block Brightness Correction Register
                                        *                  1                                                          */

        struct
        {
            __IOM uint32_t BRTG : 10;  /*!< [9..0] Brightness (DC) adjustment of G signal Unsigned; 10 bits;
                                        +512 with offset; integer                                                 */
            uint32_t : 22;
        } BRIGHT1_b;
    };

    union
    {
        __IOM uint32_t BRIGHT2;        /*!< (@ 0x0000000C) Output Control Block Brightness Correction Register
                                        *                  2                                                          */

        struct
        {
            __IOM uint32_t BRTR : 10;  /*!< [9..0] Brightness (DC) adjustment of R signal Unsigned; 10 bits;
                                        +512 with offset; integer                                                 */
            uint32_t            : 6;
            __IOM uint32_t BRTB : 10;  /*!< [25..16] Brightness (DC) adjustment of B signal Unsigned; 10
                                        *   bits; +512 with offset; integer                                           */
            uint32_t : 6;
        } BRIGHT2_b;
    };

    union
    {
        __IOM uint32_t CONTRAST;       /*!< (@ 0x00000010) Output Control Block Contrast Correction Register          */

        struct
        {
            __IOM uint32_t CONTR : 8;  /*!< [7..0] Contrast (GAIN) adjustment of R signal Unsigned; 8 bits
                                        *   fixed point                                                               */
            __IOM uint32_t CONTB : 8;  /*!< [15..8] Contrast (GAIN) adjustment of B signal Unsigned; 8 bits
                                        *   fixed point                                                               */
            __IOM uint32_t CONTG : 8;  /*!< [23..16] Contrast (GAIN) adjustment of G signal Unsigned; 8
                                        *   bits fixed point.                                                         */
            uint32_t : 8;
        } CONTRAST_b;
    };

    union
    {
        __IOM uint32_t PDTHA;          /*!< (@ 0x00000014) Output Control Block Panel Dither Correction
                                        *                  Register                                                   */

        struct
        {
            __IOM uint32_t PD : 2;     /*!< [1..0] Pattern value (D) of 2 x 2 pattern dither Unsigned 2-bit
                                        *   integer                                                                   */
            uint32_t          : 2;
            __IOM uint32_t PC : 2;     /*!< [5..4] Pattern value (C) of 2 x 2 pattern dither Unsigned 2-bit
                                        *   integer                                                                   */
            uint32_t          : 2;
            __IOM uint32_t PB : 2;     /*!< [9..8] Pattern value (B) of 2 x 2 pattern dither Unsigned 2-bit
                                        *   integer                                                                   */
            uint32_t          : 2;
            __IOM uint32_t PA : 2;     /*!< [13..12] Pattern value (A) of 2 x 2 pattern dither Unsigned
                                        *   2-bit integer                                                             */
            uint32_t            : 2;
            __IOM uint32_t FORM : 2;   /*!< [17..16] Output format select                                             */
            uint32_t            : 2;
            __IOM uint32_t SEL  : 2;   /*!< [21..20] Operation mode                                                   */
            uint32_t            : 10;
        } PDTHA_b;
    };
    __IM uint32_t RESERVED[3];

    union
    {
        __IOM uint32_t CLKPHASE;       /*!< (@ 0x00000024) Output Control Block Output Phase Control Register         */

        struct
        {
            uint32_t                 : 3;
            __IOM uint32_t TCON3EDGE : 1; /*!< [3..3] LCD_TCON3 Output Phase Control                                     */
            __IOM uint32_t TCON2EDGE : 1; /*!< [4..4] LCD_TCON2 Output Phase Control                                     */
            __IOM uint32_t TCON1EDGE : 1; /*!< [5..5] LCD_TCON1 Output Phase Control                                     */
            __IOM uint32_t TCON0EDGE : 1; /*!< [6..6] LCD_TCON0 Output Phase Control                                     */
            uint32_t                 : 1;
            __IOM uint32_t LCDEDGE   : 1; /*!< [8..8] LCD_DATA Output Phase Control                                      */
            uint32_t                 : 3;
            __IOM uint32_t FRONTGAM  : 1; /*!< [12..12] Correction control                                               */
            uint32_t                 : 19;
        } CLKPHASE_b;
    };
} R_GLCDC_OUT_Type;                       /*!< Size = 40 (0x28)                                                          */

/**
 * @brief R_GLCDC_TCON [TCON] (Timing Control Registers)
 */
typedef struct
{
    __IM uint32_t RESERVED;

    union
    {
        __IOM uint32_t TIM;             /*!< (@ 0x00000004) TCON Reference Timing Setting Register                     */

        struct
        {
            __IOM uint32_t OFFSET : 11; /*!< [10..0] Horizontal synchronization signal generation reference
                                         *   timing Sets the offset from the assertion of the internal
                                         *   horizontal synchronization signal in terms of pixels.                     */
            uint32_t            : 5;
            __IOM uint32_t HALF : 11;   /*!< [26..16] Vertical synchronization signal generation change timing
                                         *   Sets the delay from the assertion of the internal horizontal
                                         *   synchronization signal in terms of pixels.                                */
            uint32_t : 5;
        } TIM_b;
    };

    union
    {
        __IOM uint32_t STVA1;          /*!< (@ 0x00000008) TCON Vertical Timing Setting Register A1                   */

        struct
        {
            __IOM uint32_t VW : 11;    /*!< [10..0] STVx1 second change timing Sets the signal assertion
                                        *   width.                                                                    */
            uint32_t          : 5;
            __IOM uint32_t VS : 11;    /*!< [26..16] STVx1 first change timing                                        */
            uint32_t          : 5;
        } STVA1_b;
    };

    union
    {
        __IOM uint32_t STVA2;          /*!< (@ 0x0000000C) TCON Vertical Timing Setting Register A2                   */

        struct
        {
            __IOM uint32_t SEL : 3;    /*!< [2..0] Output signal select control for VSOUT (controlled by
                                        *   TCON_STVA2 register)/VEOUT (controlled by the TCON_STVB2
                                        *   register) pin                                                             */
            uint32_t           : 1;
            __IOM uint32_t INV : 1;    /*!< [4..4] STVx signal polarity inversion control                             */
            uint32_t           : 27;
        } STVA2_b;
    };

    union
    {
        __IOM uint32_t STVB1;          /*!< (@ 0x00000010) TCON Vertical Timing Setting Register B1                   */

        struct
        {
            __IOM uint32_t VW : 11;    /*!< [10..0] STVx1 second change timing Sets the signal assertion
                                        *   width.                                                                    */
            uint32_t          : 5;
            __IOM uint32_t VS : 11;    /*!< [26..16] STVx1 first change timing                                        */
            uint32_t          : 5;
        } STVB1_b;
    };

    union
    {
        __IOM uint32_t STVB2;          /*!< (@ 0x00000014) TCON Vertical Timing Setting Register B2                   */

        struct
        {
            __IOM uint32_t SEL : 3;    /*!< [2..0] Output signal select control for VSOUT (controlled by
                                        *   TCON_STVA2 register)/VEOUT (controlled by the TCON_STVB2
                                        *   register) pin                                                             */
            uint32_t           : 1;
            __IOM uint32_t INV : 1;    /*!< [4..4] STVx signal polarity inversion control                             */
            uint32_t           : 27;
        } STVB2_b;
    };

    union
    {
        __IOM uint32_t STHA1;          /*!< (@ 0x00000018) TCON Horizontal Timing Setting Register STHA1              */

        struct
        {
            __IOM uint32_t HW : 11;    /*!< [10..0] STHx1 second change timing. Sets the signal assertion
                                        *   width.                                                                    */
            uint32_t          : 5;
            __IOM uint32_t HS : 11;    /*!< [26..16] STHx1 first change timing                                        */
            uint32_t          : 5;
        } STHA1_b;
    };

    union
    {
        __IOM uint32_t STHA2;          /*!< (@ 0x0000001C) TCON Horizontal Timing Setting Register STHA2              */

        struct
        {
            __IOM uint32_t SEL : 3;    /*!< [2..0] Output signal select control for LCD_TCON2 (controlled
                                        *   by TCON_STHA2 register)/LCD_TCON3 (controlled by the TCON_STHB2
                                        *   register) pin.                                                            */
            uint32_t             : 1;
            __IOM uint32_t INV   : 1;  /*!< [4..4] STVx signal polarity inversion control.                            */
            uint32_t             : 3;
            __IOM uint32_t HSSEL : 1;  /*!< [8..8] STHx signal generation reference timing control.                   */
            uint32_t             : 23;
        } STHA2_b;
    };

    union
    {
        __IOM uint32_t STHB1;          /*!< (@ 0x00000020) TCON Horizontal Timing Setting Register STHB1              */

        struct
        {
            __IOM uint32_t HW : 11;    /*!< [10..0] STHx1 second change timing. Sets the signal assertion
                                        *   width.                                                                    */
            uint32_t          : 5;
            __IOM uint32_t HS : 11;    /*!< [26..16] STHx1 first change timing                                        */
            uint32_t          : 5;
        } STHB1_b;
    };

    union
    {
        __IOM uint32_t STHB2;          /*!< (@ 0x00000024) TCON Horizontal Timing Setting Register STHB2              */

        struct
        {
            __IOM uint32_t SEL : 3;    /*!< [2..0] Output signal select control for LCD_TCON2 (controlled
                                        *   by TCON_STHA2 register)/LCD_TCON3 (controlled by the TCON_STHB2
                                        *   register) pin.                                                            */
            uint32_t             : 1;
            __IOM uint32_t INV   : 1;  /*!< [4..4] STVx signal polarity inversion control.                            */
            uint32_t             : 3;
            __IOM uint32_t HSSEL : 1;  /*!< [8..8] STHx signal generation reference timing control.                   */
            uint32_t             : 23;
        } STHB2_b;
    };

    union
    {
        __IOM uint32_t DE;             /*!< (@ 0x00000028) TCON Data Enable Polarity Setting Register                 */

        struct
        {
            __IOM uint32_t INV : 1;    /*!< [0..0] DE signal polarity inversion control.                              */
            uint32_t           : 31;
        } DE_b;
    };
} R_GLCDC_TCON_Type;                   /*!< Size = 44 (0x2c)                                                          */

/**
 * @brief R_GLCDC_SYSCNT [SYSCNT] (GLCDC System Control Registers)
 */
typedef struct
{
    union
    {
        __IOM uint32_t DTCTEN;            /*!< (@ 0x00000000) System control block State Detection Control
                                           *                  Register                                                   */

        struct
        {
            __IOM uint32_t VPOSDTC   : 1; /*!< [0..0] Specified line detection control                                   */
            __IOM uint32_t L1UNDFDTC : 1; /*!< [1..1] Graphics 1 underflow detection control                             */
            __IOM uint32_t L2UNDFDTC : 1; /*!< [2..2] Graphics 2 underflow detection control                             */
            uint32_t                 : 29;
        } DTCTEN_b;
    };

    union
    {
        __IOM uint32_t INTEN;               /*!< (@ 0x00000004) System control block Interrupt Request Enable
                                             *                  Control Register                                           */

        struct
        {
            __IOM uint32_t VPOSINTEN   : 1; /*!< [0..0] Interrupt request signal GLCDC_VPOS enable control.                */
            __IOM uint32_t L1UNDFINTEN : 1; /*!< [1..1] Interrupt request signal GLCDC_L1UNDF enable control.              */
            __IOM uint32_t L2UNDFINTEN : 1; /*!< [2..2] Interrupt request signal GLCDC_L2UNDF enable control.              */
            uint32_t                   : 29;
        } INTEN_b;
    };

    union
    {
        __IOM uint32_t STCLR;             /*!< (@ 0x00000008) System control block Status Clear Register                 */

        struct
        {
            __IOM uint32_t VPOSCLR   : 1; /*!< [0..0] Graphics 2 specified line detection flag clear field               */
            __IOM uint32_t L1UNDFCLR : 1; /*!< [1..1] Graphics 1 underflow detection flag clear field                    */
            __IOM uint32_t L2UNDFCLR : 1; /*!< [2..2] Graphics 2 underflow detection flag clear field                    */
            uint32_t                 : 29;
        } STCLR_b;
    };

    union
    {
        __IM uint32_t STMON;           /*!< (@ 0x0000000C) System control block Status Monitor Register               */

        struct
        {
            __IM uint32_t VPOS   : 1;  /*!< [0..0] Graphics 2 specified line detection flag                           */
            __IM uint32_t L1UNDF : 1;  /*!< [1..1] Graphics 1 underflow detection flag                                */
            __IM uint32_t L2UNDF : 1;  /*!< [2..2] Graphics 2 underflow detection flag                                */
            uint32_t             : 29;
        } STMON_b;
    };

    union
    {
        __IOM uint32_t PANEL_CLK;      /*!< (@ 0x00000010) System control block Version and Panel Clock
                                        *                  Control Register                                           */

        struct
        {
            __IOM uint32_t DCDR : 6;   /*!< [5..0] Clock division ratio setting control Refer toTable 2.7.1
                                        *   for details about setting value. Note: Settings that are
                                        *   not listed in table 2.7.1 are prohibited.                                 */
            __IOM uint32_t CLKEN : 1;  /*!< [6..6] Panel clock output enable control Note: Before changing
                                        *   the PIXSEL,CLKSEL or DCDR bit, this bit must be set to
                                        *   0.                                                                        */
            uint32_t              : 1;
            __IOM uint32_t CLKSEL : 1; /*!< [8..8] Panel clock supply source select                                   */
            uint32_t              : 3;
            __IOM uint32_t PIXSEL : 1; /*!< [12..12] Pixel clock select control. Must be set to the same
                                        *   value as OUT_SET.FRQSEL[1].                                               */
            uint32_t          : 3;
            __IM uint32_t VER : 16;    /*!< [31..16] Version information Version information of the GLCDC             */
        } PANEL_CLK_b;
    };
} R_GLCDC_SYSCNT_Type;                 /*!< Size = 20 (0x14)                                                          */

/**
 * @brief R_GPT_ODC_GTDLYR [GTDLYR] (PWM DELAY RISING)
 */
typedef struct
{
    union
    {
        __IOM uint16_t A;              /*!< (@ 0x00000000) GTIOCA Output Delay Register                               */

        struct
        {
            __IOM uint16_t DLY : 5;    /*!< [4..0] GTIOCnA Output Rising Edge Delay Setting                           */
            uint16_t           : 11;
        } A_b;
    };

    union
    {
        __IOM uint16_t B;              /*!< (@ 0x00000002) GTIOCB Output Delay Register                               */

        struct
        {
            __IOM uint16_t DLY : 5;    /*!< [4..0] GTIOCnA Output Rising Edge Delay Setting                           */
            uint16_t           : 11;
        } B_b;
    };
} R_GPT_ODC_GTDLYR_Type;               /*!< Size = 4 (0x4)                                                            */

/**
 * @brief Graphics LCD Controller (R_GLCDC)
 */

typedef struct                         /*!< (@ 0x000E0000) R_GLCDC Structure                                          */
{
    union
    {
        __IOM uint32_t GR1_CLUT0[256]; /*!< (@ 0x00000000) Color Palette 0 Plane for Graphics 1 Plane                 */

        struct
        {
            __IOM uint32_t B : 8;      /*!< [7..0] B Value of Color Palette n Plane for Graphics m Plane              */
            __IOM uint32_t G : 8;      /*!< [15..8] G Value of Color Palette n Plane for Graphics m Plane             */
            __IOM uint32_t R : 8;      /*!< [23..16] R Value of Color Palette n Plane for Graphics m Plane            */
            __IOM uint32_t A : 8;      /*!< [31..24] Alpha Blending Value of Color Palette n Plane for Graphics
                                        *   m Plane                                                                   */
        } GR1_CLUT0_b[256];
    };

    union
    {
        __IOM uint32_t GR1_CLUT1[256]; /*!< (@ 0x00000400) Color Palette 1 Plane for Graphics 1 Plane                 */

        struct
        {
            __IOM uint32_t B : 8;      /*!< [7..0] B Value of Color Palette n Plane for Graphics m Plane              */
            __IOM uint32_t G : 8;      /*!< [15..8] G Value of Color Palette n Plane for Graphics m Plane             */
            __IOM uint32_t R : 8;      /*!< [23..16] R Value of Color Palette n Plane for Graphics m Plane            */
            __IOM uint32_t A : 8;      /*!< [31..24] Alpha Blending Value of Color Palette n Plane for Graphics
                                        *   m Plane                                                                   */
        } GR1_CLUT1_b[256];
    };

    union
    {
        __IOM uint32_t GR2_CLUT0[256]; /*!< (@ 0x00000800) Color Palette 0 Plane for Graphics 2 Plane                 */

        struct
        {
            __IOM uint32_t B : 8;      /*!< [7..0] B Value of Color Palette n Plane for Graphics m Plane              */
            __IOM uint32_t G : 8;      /*!< [15..8] G Value of Color Palette n Plane for Graphics m Plane             */
            __IOM uint32_t R : 8;      /*!< [23..16] R Value of Color Palette n Plane for Graphics m Plane            */
            __IOM uint32_t A : 8;      /*!< [31..24] Alpha Blending Value of Color Palette n Plane for Graphics
                                        *   m Plane                                                                   */
        } GR2_CLUT0_b[256];
    };

    union
    {
        __IOM uint32_t GR2_CLUT1[256]; /*!< (@ 0x00000C00) Color Palette 1 Plane for Graphics 2 Plane                 */

        struct
        {
            __IOM uint32_t B : 8;      /*!< [7..0] B Value of Color Palette n Plane for Graphics m Plane              */
            __IOM uint32_t G : 8;      /*!< [15..8] G Value of Color Palette n Plane for Graphics m Plane             */
            __IOM uint32_t R : 8;      /*!< [23..16] R Value of Color Palette n Plane for Graphics m Plane            */
            __IOM uint32_t A : 8;      /*!< [31..24] Alpha Blending Value of Color Palette n Plane for Graphics
                                        *   m Plane                                                                   */
        } GR2_CLUT1_b[256];
    };
    __IOM R_GLCDC_BG_Type     BG;      /*!< (@ 0x00001000) Background Registers                                       */
    __IM uint32_t             RESERVED[57];
    __IOM R_GLCDC_GR_Type     GR[2];   /*!< (@ 0x00001100) Layer Registers                                            */
    __IOM R_GLCDC_GAM_Type    GAM[3];  /*!< (@ 0x00001300) Gamma Settings                                             */
    __IOM R_GLCDC_OUT_Type    OUT;     /*!< (@ 0x000013C0) Output Control Registers                                   */
    __IM uint32_t             RESERVED1[6];
    __IOM R_GLCDC_TCON_Type   TCON;    /*!< (@ 0x00001400) Timing Control Registers                                   */
    __IM uint32_t             RESERVED2[5];
    __IOM R_GLCDC_SYSCNT_Type SYSCNT;  /*!< (@ 0x00001440) GLCDC System Control Registers                             */
} R_GLCDC_Type;                        /*!< Size = 5204 (0x1454)                                                      */

#endif /* R_GLCDC_REG_H */
