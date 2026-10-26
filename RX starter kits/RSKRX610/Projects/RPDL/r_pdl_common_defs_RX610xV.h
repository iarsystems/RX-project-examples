/* Adapted for use with IAR Embedded Workbench */
/*""FILE COMMENT""*******************************************************
* System Name  : API for RX610xV
* File Name    : r_pdl_common_defs_RX610xV.h
* Version    : 0.10
* Contents    : API common definitions
* Customer    :
* Model      :
* Order      :
* CPU      : RX600
* Compiler    : RXC
* OS      : Nothing
* Programmer  :
* Note      :
************************************************************************
* Copyright,2010 RENESAS TECHNOLOGY CORPORATION
* AND RENESAS SOLUTIONS CORPORATION
************************************************************************
* History    : 2009.12.18
*        : Ver 0.10
*        : 2010.01.06
*        : Ver 0.11
*        : Updated to reflect changes in Hardware Manual Rev0.40.
*        : Corrected TPU overflow and underflow definition names.
*        : 2010.02.08
*        : Ver 0.12
*        : Moved the DMAC trigger definitions to the DMAC header.
*""FILE COMMENT END""**************************************************/

#ifndef R_PDL_COMMON_DEFS_RX610xV_H
#define R_PDL_COMMON_DEFS_RX610xV_H

#include <stdint.h>
#include <stdbool.h>
#include <machine.h>
#include <stddef.h>
#include "iorx610.h"
/* Callback function type */
typedef void (* VoidCallBackFunc)(void);

/* The highest interrupt priority level */
#define IPL_MAX  7

/* Use the I/O register area to indicate that a callback function pointer is not to be used */
#define PDL_NO_FUNC  (VoidCallBackFunc)0x00080000ul
/* Use the ROM area to indicate that a data pointer is not to be used */
#define PDL_NO_PTR (void *)0xFFFFFFFFul
/* When no parameters options are required */
#define PDL_NO_DATA 0

/* Shared global variables */
extern volatile uint32_t rpdl_CGC_f_pclk;
extern volatile uint32_t rpdl_CGC_f_iclk;
extern volatile uint32_t rpdl_CGC_f_bclk;
extern volatile   uint8_t rpdl_INTC_brk_command;
extern volatile   uint8_t rpdl_INTC_brk_data8;
extern volatile uint32_t rpdl_INTC_saved_isp;

/* BRK handler command options */
typedef enum {
  BRK_NO_COMMAND,
  BRK_START_ADC_10,
  BRK_START_ADC_10_AND_SLEEP,
  BRK_SLEEP,
  BRK_ALL_MODULE_CLOCK_STOP,
  BRK_STANDBY,
  BRK_DEEP_STANDBY,
  BRK_LOAD_FINTV_REGISTER,
  BRK_WRITE_IPL
} brk_commands;

/* Bit definitions */
#define  BIT_0 0x00000001ul
#define  BIT_1 0x00000002ul
#define  BIT_2 0x00000004ul
#define  BIT_3 0x00000008ul
#define  BIT_4 0x00000010ul
#define  BIT_5 0x00000020ul
#define  BIT_6 0x00000040ul
#define  BIT_7 0x00000080ul
#define  BIT_8 0x00000100ul
#define  BIT_9 0x00000200ul
#define BIT_10 0x00000400ul
#define BIT_11 0x00000800ul
#define BIT_12 0x00001000ul
#define BIT_13 0x00002000ul
#define BIT_14 0x00004000ul
#define BIT_15 0x00008000ul
#define BIT_16 0x00010000ul
#define BIT_17 0x00020000ul
#define BIT_18 0x00040000ul
#define BIT_19 0x00080000ul
#define BIT_20 0x00100000ul
#define BIT_21 0x00200000ul
#define BIT_22 0x00400000ul
#define BIT_23 0x00800000ul
#define BIT_24 0x01000000ul
#define BIT_25 0x02000000ul
#define BIT_26 0x04000000ul
#define BIT_27 0x08000000ul
#define BIT_28 0x10000000ul
#define BIT_29 0x20000000ul
#define BIT_30 0x40000000ul
#define BIT_31 0x80000000ul

#define INV_BIT_0  0xFFFFFFFEul
#define INV_BIT_1  0xFFFFFFFDul
#define INV_BIT_2  0xFFFFFFFBul
#define INV_BIT_3  0xFFFFFFF7ul
#define INV_BIT_4  0xFFFFFFEFul
#define INV_BIT_5  0xFFFFFFDFul
#define INV_BIT_6  0xFFFFFFBFul
#define INV_BIT_7  0xFFFFFF7Ful
#define INV_BIT_8  0xFFFFFEFFul
#define INV_BIT_9  0xFFFFFDFFul
#define INV_BIT_10 0xFFFFFBFFul
#define INV_BIT_11 0xFFFFF7FFul
#define INV_BIT_12 0xFFFFEFFFul
#define INV_BIT_13 0xFFFFDFFFul
#define INV_BIT_14 0xFFFFBFFFul
#define INV_BIT_15 0xFFFF7FFFul
#define INV_BIT_16 0xFFFEFFFFul
#define INV_BIT_17 0xFFFDFFFFul
#define INV_BIT_18 0xFFFBFFFFul
#define INV_BIT_19 0xFFF7FFFFul
#define INV_BIT_20 0xFFEFFFFFul
#define INV_BIT_21 0xFFDFFFFFul
#define INV_BIT_22 0xFFBFFFFFul
#define INV_BIT_23 0xFF7FFFFFul
#define INV_BIT_24 0xFEFFFFFFul
#define INV_BIT_25 0xFDFFFFFFul
#define INV_BIT_26 0xFBFFFFFFul
#define INV_BIT_27 0xF7FFFFFFul
#define INV_BIT_28 0xEFFFFFFFul
#define INV_BIT_29 0xDFFFFFFFul
#define INV_BIT_30 0xBFFFFFFFul
#define INV_BIT_31 0x7FFFFFFFul

/* Interrupt vector numbers */
#define PDL_INTC_VECTOR_BUSERR   16
#define PDL_INTC_VECTOR_FIFERR   21
#define PDL_INTC_VECTOR_FRDYI    23
#define PDL_INTC_VECTOR_CMT0     28
#define PDL_INTC_VECTOR_CMT1     29
#define PDL_INTC_VECTOR_CMT2     30
#define PDL_INTC_VECTOR_CMT3     31
#define PDL_INTC_VECTOR_IRQ0     64
#define PDL_INTC_VECTOR_IRQ1     65
#define PDL_INTC_VECTOR_IRQ2     66
#define PDL_INTC_VECTOR_IRQ3     67
#define PDL_INTC_VECTOR_IRQ4     68
#define PDL_INTC_VECTOR_IRQ5     69
#define PDL_INTC_VECTOR_IRQ6     70
#define PDL_INTC_VECTOR_IRQ7     71
#define PDL_INTC_VECTOR_IRQ8     72
#define PDL_INTC_VECTOR_IRQ9     73
#define PDL_INTC_VECTOR_IRQ10    74
#define PDL_INTC_VECTOR_IRQ11    75
#define PDL_INTC_VECTOR_IRQ12    76
#define PDL_INTC_VECTOR_IRQ13    77
#define PDL_INTC_VECTOR_IRQ14    78
#define PDL_INTC_VECTOR_IRQ15    79
#define PDL_INTC_VECTOR_WOVI     96
#define PDL_INTC_VECTOR_ADI0     98
#define PDL_INTC_VECTOR_ADI1     99
#define PDL_INTC_VECTOR_ADI2    100
#define PDL_INTC_VECTOR_ADI3    101
#define PDL_INTC_VECTOR_TGI0A   104
#define PDL_INTC_VECTOR_TGI0B   105
#define PDL_INTC_VECTOR_TGI0C   106
#define PDL_INTC_VECTOR_TGI0D   107
#define PDL_INTC_VECTOR_TCI0V   108
#define PDL_INTC_VECTOR_TGI1A   111
#define PDL_INTC_VECTOR_TGI1B   112
#define PDL_INTC_VECTOR_TCI1V   115
#define PDL_INTC_VECTOR_TCI1U   116
#define PDL_INTC_VECTOR_TGI2A   117
#define PDL_INTC_VECTOR_TGI2B   118
#define PDL_INTC_VECTOR_TCI2V   120
#define PDL_INTC_VECTOR_TCI2U   121
#define PDL_INTC_VECTOR_TGI3A   122
#define PDL_INTC_VECTOR_TGI3B   123
#define PDL_INTC_VECTOR_TGI3C   124
#define PDL_INTC_VECTOR_TGI3D   125
#define PDL_INTC_VECTOR_TCI3V   126
#define PDL_INTC_VECTOR_TGI4A   127
#define PDL_INTC_VECTOR_TGI4B   128
#define PDL_INTC_VECTOR_TCI4V   131
#define PDL_INTC_VECTOR_TCI4U   132
#define PDL_INTC_VECTOR_TGI5A   133
#define PDL_INTC_VECTOR_TGI5B   134
#define PDL_INTC_VECTOR_TCI5V   136
#define PDL_INTC_VECTOR_TCI5U   137
#define PDL_INTC_VECTOR_TGI6A   138
#define PDL_INTC_VECTOR_TGI6B   139
#define PDL_INTC_VECTOR_TGI6C   140
#define PDL_INTC_VECTOR_TGI6D   141
#define PDL_INTC_VECTOR_TCI6V   142
#define PDL_INTC_VECTOR_TGI7A   145
#define PDL_INTC_VECTOR_TGI7B   146
#define PDL_INTC_VECTOR_TCI7V   149
#define PDL_INTC_VECTOR_TCI7U   150
#define PDL_INTC_VECTOR_TGI8A   151
#define PDL_INTC_VECTOR_TGI8B   152
#define PDL_INTC_VECTOR_TCI8V   154
#define PDL_INTC_VECTOR_TCI8U   155
#define PDL_INTC_VECTOR_TGI9A   156
#define PDL_INTC_VECTOR_TGI9B   157
#define PDL_INTC_VECTOR_TGI9C   158
#define PDL_INTC_VECTOR_TGI9D   159
#define PDL_INTC_VECTOR_TCI9V   160
#define PDL_INTC_VECTOR_TGI10A  161
#define PDL_INTC_VECTOR_TGI10B  162
#define PDL_INTC_VECTOR_TCI10V  165
#define PDL_INTC_VECTOR_TCI10U  166
#define PDL_INTC_VECTOR_TGI11A  167
#define PDL_INTC_VECTOR_TGI11B  168
#define PDL_INTC_VECTOR_TCI11V  170
#define PDL_INTC_VECTOR_TCI11U  171
#define PDL_INTC_VECTOR_CMIA0   174
#define PDL_INTC_VECTOR_CMIB0   175
#define PDL_INTC_VECTOR_OVI0    176
#define PDL_INTC_VECTOR_CMIA1   177
#define PDL_INTC_VECTOR_CMIB1   178
#define PDL_INTC_VECTOR_OVI1    179
#define PDL_INTC_VECTOR_CMIA2   180
#define PDL_INTC_VECTOR_CMIB2   181
#define PDL_INTC_VECTOR_OVI2    182
#define PDL_INTC_VECTOR_CMIA3   183
#define PDL_INTC_VECTOR_CMIB3   184
#define PDL_INTC_VECTOR_OVI3    185
#define PDL_INTC_VECTOR_DMTEND0 198
#define PDL_INTC_VECTOR_DMTEND1 199
#define PDL_INTC_VECTOR_DMTEND2 200
#define PDL_INTC_VECTOR_DMTEND3 201
#define PDL_INTC_VECTOR_ERI0    214
#define PDL_INTC_VECTOR_RXI0    215
#define PDL_INTC_VECTOR_TXI0    216
#define PDL_INTC_VECTOR_TEI0    217
#define PDL_INTC_VECTOR_ERI1    218
#define PDL_INTC_VECTOR_RXI1    219
#define PDL_INTC_VECTOR_TXI1    220
#define PDL_INTC_VECTOR_TEI1    221
#define PDL_INTC_VECTOR_ERI2    222
#define PDL_INTC_VECTOR_RXI2    223
#define PDL_INTC_VECTOR_TXI2    224
#define PDL_INTC_VECTOR_TEI2    225
#define PDL_INTC_VECTOR_ERI3    226
#define PDL_INTC_VECTOR_RXI3    227
#define PDL_INTC_VECTOR_TXI3    228
#define PDL_INTC_VECTOR_TEI3    229
#define PDL_INTC_VECTOR_ERI4    230
#define PDL_INTC_VECTOR_RXI4    231
#define PDL_INTC_VECTOR_TXI4    232
#define PDL_INTC_VECTOR_TEI4    233
#define PDL_INTC_VECTOR_ERI5    234
#define PDL_INTC_VECTOR_RXI5    235
#define PDL_INTC_VECTOR_TXI5    236
#define PDL_INTC_VECTOR_TEI5    237
#define PDL_INTC_VECTOR_ERI6    238
#define PDL_INTC_VECTOR_RXI6    239
#define PDL_INTC_VECTOR_TXI6    240
#define PDL_INTC_VECTOR_TEI6    241
#define PDL_INTC_VECTOR_ICEEI0  246
#define PDL_INTC_VECTOR_ICRXI0  247
#define PDL_INTC_VECTOR_ICTXI0  248
#define PDL_INTC_VECTOR_ICTEI0  249
#define PDL_INTC_VECTOR_ICEEI1  250
#define PDL_INTC_VECTOR_ICRXI1  251
#define PDL_INTC_VECTOR_ICTXI1  252
#define PDL_INTC_VECTOR_ICTEI1  253

#endif

/* End of file */
