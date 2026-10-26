/*
* Copyright (c) 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/************************************************************************/
/*    File Version: V0.50A                                              */
/*    History: 0.50  (2025-06-04)  [Hardware Manual Revision : 0.50]    */
/*           : 0.50A (2025-06-05)  [Hardware Manual Revision : 0.50]    */
/************************************************************************/

#include "interrupt_handlers.h"

extern void stack (void);


#ifdef __ICCRX__
typedef __interrupt void (*fp) (void);
#define EXVECT_SECT    __attribute__ ((section (".exvectors")))
extern char _iar_program_start;
__root
#else
typedef void (*fp) (void);
#define EXVECT_SECT    __attribute__ ((section (".exvectors")))
extern void PowerON_Reset (void);
#endif

const fp ExceptVectors[] EXVECT_SECT  = {
		//;0xffffff80  Reserved
	    INT_Dummy,
	//;0xffffff84  Reserved
	    INT_Dummy,
	//;0xffffff88  Exception(Secure Instruction)
		INT_Excep_Secure,
	//;0xffffff8c  Reserved
	    INT_Dummy,
	//;0xffffff90  Reserved
	    INT_Dummy,
	//;0xffffff94  Reserved
	    INT_Dummy,
	//;0xffffff98  WTNMI
		INT_WorldTransitionNonMaskableInterrupt,
	//;0xffffff9c  Reserved
	    INT_Dummy,
	//;0xffffffa0  Reserved
	    INT_Dummy,
	//;0xffffffa4  Reserved
	    INT_Dummy,
	//;0xffffffa8  Reserved
	    INT_Dummy,
	//;0xffffffac  Reserved
	    INT_Dummy,
	//;0xffffffb0  Reserved
	    INT_Dummy,
	//;0xffffffb4  Reserved
	    INT_Dummy,
	//;0xffffffb8  Reserved
	    INT_Dummy,
	//;0xffffffbc  Reserved
	    INT_Dummy,
	//;0xffffffc0  Reserved
	    INT_Dummy,
	//;0xffffffc4  Reserved
	    INT_Dummy,
	//;0xffffffc8  Reserved
	    INT_Dummy,
	//;0xffffffcc  Reserved
	    INT_Dummy,
	//;0xffffffd0  Exception(Supervisor Instruction)
	    INT_Excep_SuperVisorInst,
	//;0xffffffd4  Exception(Access Instruction)
		INT_Excep_AccessInst,
	//;0xffffffd8  Reserved
	    INT_Dummy,
	//;0xffffffdc  Exception(Undefined Instruction)
		INT_Excep_UndefinedInst,
	//;0xffffffe0  Exception(Address Instruction)
		INT_Excep_AddressInst,
	//;0xffffffe4  Exception(Floating Point)
		INT_Excep_FloatingPoint,
	//;0xffffffe8  Reserved
	    INT_Dummy,
	//;0xffffffec  Reserved
	    INT_Dummy,
	//;0xfffffff0  Reserved
	    INT_Dummy,
	//;0xfffffff4  Reserved
	    INT_Dummy,
	//;0xfffffff8  NMI
		INT_NonMaskableInterrupt,
};

#ifdef __ICCRX__
#define FVECT_SECT    __attribute__ ((section (".fvectors")))
__root void const* const HardwareVectors FVECT_SECT = 
//;0xfffffffc  RESET
//;Power On Reset PC
    /*(void*)*/ &_iar_program_start;
#else
#define FVECT_SECT    __attribute__ ((section (".fvectors")))

const fp HardwareVectors[] FVECT_SECT  = {
//;0xfffffffc  RESET
//;<<VECTOR DATA START (POWER ON RESET)>>
//;Power On Reset PC
    /*(void*)*/ PowerON_Reset
//;<<VECTOR DATA END (POWER ON RESET)>>
};
#endif

#define RVECT_SECT          __attribute__ ((section (".rvectors")))
#ifdef __ICCRX__
__root
#endif
const fp RelocatableVectors[] RVECT_SECT  = {
//;0x0000  BRK
    (fp)INT_Excep_BRK,
//;0x0004  Reserved
    (fp)INT_Dummy,
//;0x0008  Reserved
    (fp)INT_Dummy,
//;0x000C  Reserved
    (fp)INT_Dummy,
//;0x0010  Reserved
    (fp)INT_Dummy,
//;0x0014  Reserved
    (fp)INT_Dummy,
//;0x0018  Reserved
    (fp)INT_Dummy,
//;0x001C  Reserved
    (fp)INT_Dummy,
//;0x0020  Reserved
    (fp)INT_Dummy,
//;0x0024  Reserved
    (fp)INT_Dummy,
//;0x0028  Reserved
    (fp)INT_Dummy,
//;0x002C  Reserved
    (fp)INT_Dummy,
//;0x0030  Reserved
    (fp)INT_Dummy,
//;0x0034  Reserved
    (fp)INT_Dummy,
//;0x0038  Reserved
    (fp)INT_Dummy,
//;0x003C  Reserved
    (fp)INT_Dummy,
//;0x0040  BUSERR
    (fp)INT_Excep_BSC_BUSERR,
//;0x0044  GROUPIL0_S
    (fp)INT_Excep_ICU_GROUPIL0_S,
//;0x0048  GROUPIL0_NS
    (fp)INT_Excep_ICU_GROUPIL0_NS,
//;0x004C  Reserved
    (fp)INT_Dummy,
//;0x0050  Reserved
    (fp)INT_Dummy,
//;0x0054  Reserved
    (fp)INT_Dummy,
//;0x0058  Reserved
    (fp)INT_Dummy,
//;0x005C  Reserved
    (fp)INT_Dummy,
//;0x0060  Reserved
    (fp)INT_Dummy,
//;0x0064  Reserved
    (fp)INT_Dummy,
//;0x0068  SWINT2
    (fp)INT_Excep_ICU_SWINT2,
//;0x006C  SWINT
    (fp)INT_Excep_ICU_SWINT,
//;0x0070  CMI0
    (fp)INT_Excep_CMT0_CMI0,
//;0x0074  CMI1
    (fp)INT_Excep_CMT1_CMI1,
//;0x0078  CMWI0
    (fp)INT_Excep_CMTW0_CMWI0,
//;0x007C  CMWI1
    (fp)INT_Excep_CMTW1_CMWI1,
//;0x0080  MRDYI
    (fp)INT_Excep_MRAM_MRDYI,
//;0x0084  MRPRRI
    (fp)INT_Excep_MRAM_MRPRRI,
//;0x0088  Reserved
    (fp)INT_Dummy,
//;0x008C  Reserved
    (fp)INT_Dummy,
//;0x0090  Reserved
    (fp)INT_Dummy,
//;0x0094  Reserved
    (fp)INT_Dummy,
//;0x0098  Reserved
    (fp)INT_Dummy,
//;0x009C  Reserved
    (fp)INT_Dummy,
//;0x00A0  Reserved
    (fp)INT_Dummy,
//;0x00A4  Reserved
    (fp)INT_Dummy,
//;0x00A8  Reserved
    (fp)INT_Dummy,
//;0x00AC  Reserved
    (fp)INT_Dummy,
//;0x00B0  Reserved
    (fp)INT_Dummy,
//;0x00B4  Reserved
    (fp)INT_Dummy,
//;0x00B8  Reserved
    (fp)INT_Dummy,
//;0x00BC  Reserved
    (fp)INT_Dummy,
//;0x00C0  Reserved
    (fp)INT_Dummy,
//;0x00C4  Reserved
    (fp)INT_Dummy,
//;0x00C8  Reserved
    (fp)INT_Dummy,
//;0x00CC  Reserved
    (fp)INT_Dummy,
//;0x00D0  Reserved
    (fp)INT_Dummy,
//;0x00D4  Reserved
    (fp)INT_Dummy,
//;0x00D8  Reserved
    (fp)INT_Dummy,
//;0x00DC  Reserved
    (fp)INT_Dummy,
//;0x00E0  Reserved
    (fp)INT_Dummy,
//;0x00E4  Reserved
    (fp)INT_Dummy,
//;0x00E8  Reserved
    (fp)INT_Dummy,
//;0x00EC  Reserved
    (fp)INT_Dummy,
//;0x00F0  Reserved
    (fp)INT_Dummy,
//;0x00F4  Reserved
    (fp)INT_Dummy,
//;0x00F8  Reserved
    (fp)INT_Dummy,
//;0x00FC  Reserved
    (fp)INT_Dummy,
//;0x0100  IRQ0
    (fp)INT_Excep_ICU_IRQ0,
//;0x0104  IRQ1
    (fp)INT_Excep_ICU_IRQ1,
//;0x0108  IRQ2
    (fp)INT_Excep_ICU_IRQ2,
//;0x010C  IRQ3
    (fp)INT_Excep_ICU_IRQ3,
//;0x0110  IRQ4
    (fp)INT_Excep_ICU_IRQ4,
//;0x0114  IRQ5
    (fp)INT_Excep_ICU_IRQ5,
//;0x0118  IRQ6
    (fp)INT_Excep_ICU_IRQ6,
//;0x011C  IRQ7
    (fp)INT_Excep_ICU_IRQ7,
//;0x0120  IRQ8
    (fp)INT_Excep_ICU_IRQ8,
//;0x0124  IRQ9
    (fp)INT_Excep_ICU_IRQ9,
//;0x0128  IRQ10
    (fp)INT_Excep_ICU_IRQ10,
//;0x012C  IRQ11
    (fp)INT_Excep_ICU_IRQ11,
//;0x0130  IRQ12
    (fp)INT_Excep_ICU_IRQ12,
//;0x0134  IRQ13
    (fp)INT_Excep_ICU_IRQ13,
//;0x0138  IRQ14
    (fp)INT_Excep_ICU_IRQ14,
//;0x013C  IRQ15
    (fp)INT_Excep_ICU_IRQ15,
//;0x0140  RI3C0_WUI
    (fp)INT_Excep_RI3C0_WUI,
//;0x0144  RIICHS1_WUI
    (fp)INT_Excep_RIICHS1_WUI,
//;0x0148  RIICHS2_WUI
    (fp)INT_Excep_RIICHS2_WUI,
//;0x014C  VBATT_TAMPDI
    (fp)INT_Excep_VBATT_TAMPDI,
//;0x0150  PDM_SDET
    (fp)INT_Excep_PDM_SDET,
//;0x0154  SOSTDI
    (fp)INT_Excep_SOST_SOSTDI,
//;0x0158  Reserved
    (fp)INT_Dummy,
//;0x015C  Reserved
    (fp)INT_Dummy,
//;0x0160  PVD1
    (fp)INT_Excep_PVD1_PVD1,
//;0x0164  PVD2
    (fp)INT_Excep_PVD2_PVD2,
//;0x0168  USBR
    (fp)INT_Excep_USB0_USBR,
//;0x016C  Reserved
    (fp)INT_Dummy,
//;0x0170  ALM
    (fp)INT_Excep_RTC_ALM,
//;0x0174  PRD
    (fp)INT_Excep_RTC_PRD,
//;0x0178  USBHSR
    (fp)INT_Excep_USBHS_USBHSR,
//;0x017C  IWUNI
    (fp)INT_Excep_IWDT_IWUNI,
//;0x0180  WUNI
    (fp)INT_Excep_WDT_WUNI,
//;0x0184  Reserved
    (fp)INT_Dummy,
//;0x0188  Reserved
    (fp)INT_Dummy,
//;0x018C  Reserved
    (fp)INT_Dummy,
//;0x0190  Reserved
    (fp)INT_Dummy,
//;0x0194  Reserved
    (fp)INT_Dummy,
//;0x0198  GROUPBL0_S
    (fp)INT_Excep_ICU_GROUPBL0_S,
//;0x019C  GROUPBL1_S
    (fp)INT_Excep_ICU_GROUPBL1_S,
//;0x01A0  GROUPAL0_S
    (fp)INT_Excep_ICU_GROUPAL0_S,
//;0x01A4  GROUPAL1_S
    (fp)INT_Excep_ICU_GROUPAL1_S,
//;0x01A8  GROUPAL2_S
    (fp)INT_Excep_ICU_GROUPAL2_S,
//;0x01AC  GROUPAL3_S
    (fp)INT_Excep_ICU_GROUPAL3_S,
//;0x01B0  GROUPAL4_S
    (fp)INT_Excep_ICU_GROUPAL4_S,
//;0x01B4  GROUPAL5_S
    (fp)INT_Excep_ICU_GROUPAL5_S,
//;0x01B8  GROUPAL6_S
    (fp)INT_Excep_ICU_GROUPAL6_S,
//;0x01BC  GROUPBL0_NS
    (fp)INT_Excep_ICU_GROUPBL0_NS,
//;0x01C0  GROUPBL1_NS
    (fp)INT_Excep_ICU_GROUPBL1_NS,
//;0x01C4  GROUPAL0_NS
    (fp)INT_Excep_ICU_GROUPAL0_NS,
//;0x01C8  GROUPAL1_NS
    (fp)INT_Excep_ICU_GROUPAL1_NS,
//;0x01CC  GROUPAL2_NS
    (fp)INT_Excep_ICU_GROUPAL2_NS,
//;0x01D0  GROUPAL3_NS
    (fp)INT_Excep_ICU_GROUPAL3_NS,
//;0x01D4  GROUPAL4_NS
    (fp)INT_Excep_ICU_GROUPAL4_NS,
//;0x01D8  GROUPAL5_NS
    (fp)INT_Excep_ICU_GROUPAL5_NS,
//;0x01DC  GROUPAL6_NS
    (fp)INT_Excep_ICU_GROUPAL6_NS,
//;0x01E0  DMAC0I
    (fp)INT_Excep_DMAC_DMAC0I,
//;0x01E4  DMAC1I
    (fp)INT_Excep_DMAC_DMAC1I,
//;0x01E8  DMAC2I
    (fp)INT_Excep_DMAC_DMAC2I,
//;0x01EC  DMAC3I
    (fp)INT_Excep_DMAC_DMAC3I,
//;0x01F0  DMAC74I
    (fp)INT_Excep_DMAC_DMAC74I,
//;0x01F4  OSTDI
    (fp)INT_Excep_OST_OSTDI,
//;0x01F8  EXDMAC0I
    (fp)INT_Excep_EXDMAC_EXDMAC0I,
//;0x01FC  EXDMAC1I
    (fp)INT_Excep_EXDMAC_EXDMAC1I,
//;0x0200  SLIX128
    (fp)INT_Excep_PERIX_SLIX128,
//;0x0204  SLIX129
    (fp)INT_Excep_PERIX_SLIX129,
//;0x0208  SLIX130
    (fp)INT_Excep_PERIX_SLIX130,
//;0x020C  SLIX131
    (fp)INT_Excep_PERIX_SLIX131,
//;0x0210  SLIX132
    (fp)INT_Excep_PERIX_SLIX132,
//;0x0214  SLIX133
    (fp)INT_Excep_PERIX_SLIX133,
//;0x0218  SLIX134
    (fp)INT_Excep_PERIX_SLIX134,
//;0x021C  SLIX135
    (fp)INT_Excep_PERIX_SLIX135,
//;0x0220  SLIX136
    (fp)INT_Excep_PERIX_SLIX136,
//;0x0224  SLIX137
    (fp)INT_Excep_PERIX_SLIX137,
//;0x0228  SLIX138
    (fp)INT_Excep_PERIX_SLIX138,
//;0x022C  SLIX139
    (fp)INT_Excep_PERIX_SLIX139,
//;0x0230  SLIX140
    (fp)INT_Excep_PERIX_SLIX140,
//;0x0234  SLIX141
    (fp)INT_Excep_PERIX_SLIX141,
//;0x0238  SLIX142
    (fp)INT_Excep_PERIX_SLIX142,
//;0x023C  SLIX143
    (fp)INT_Excep_PERIX_SLIX143,
//;0x0240  SLI144
    (fp)INT_Excep_PERI_SLI144,
//;0x0244  SLI145
    (fp)INT_Excep_PERI_SLI145,
//;0x0248  SLI146
    (fp)INT_Excep_PERI_SLI146,
//;0x024C  SLI147
    (fp)INT_Excep_PERI_SLI147,
//;0x0250  SLI148
    (fp)INT_Excep_PERI_SLI148,
//;0x02540 SLI149
    (fp)INT_Excep_PERI_SLI149,
//;0x0258  SLI150
    (fp)INT_Excep_PERI_SLI150,
//;0x025C  SLI151
    (fp)INT_Excep_PERI_SLI151,
//;0x0260  SLI152
    (fp)INT_Excep_PERI_SLI152,
//;0x0264  SLI153
    (fp)INT_Excep_PERI_SLI153,
//;0x0268  SLI154
    (fp)INT_Excep_PERI_SLI154,
//;0x026C  SLI155
    (fp)INT_Excep_PERI_SLI155,
//;0x0270  SLI156
    (fp)INT_Excep_PERI_SLI156,
//;0x0274  SLI157
    (fp)INT_Excep_PERI_SLI157,
//;0x0278  SLI158
    (fp)INT_Excep_PERI_SLI158,
//;0x027C  SLI159
    (fp)INT_Excep_PERI_SLI159,
//;0x0280  SLI160
    (fp)INT_Excep_PERI_SLI160,
//;0x0284  SLI161
    (fp)INT_Excep_PERI_SLI161,
//;0x0288  SLI162
    (fp)INT_Excep_PERI_SLI162,
//;0x028C  SLI163
    (fp)INT_Excep_PERI_SLI163,
//;0x0290  SLI164
    (fp)INT_Excep_PERI_SLI164,
//;0x0294  SLI165
    (fp)INT_Excep_PERI_SLI165,
//;0x0298  SLI166
    (fp)INT_Excep_PERI_SLI166,
//;0x029C  SLI167
    (fp)INT_Excep_PERI_SLI167,
//;0x02A0  SLI168
    (fp)INT_Excep_PERI_SLI168,
//;0x02A4  SLI169
    (fp)INT_Excep_PERI_SLI169,
//;0x02A8  SLI170
    (fp)INT_Excep_PERI_SLI170,
//;0x02AC  SLI171
    (fp)INT_Excep_PERI_SLI171,
//;0x02B0  SLI172
    (fp)INT_Excep_PERI_SLI172,
//;0x02B4  SLI173
    (fp)INT_Excep_PERI_SLI173,
//;0x02B8  SLI174
    (fp)INT_Excep_PERI_SLI174,
//;0x02BC  SLI175
    (fp)INT_Excep_PERI_SLI175,
//;0x02C0  SLI176
    (fp)INT_Excep_PERI_SLI176,
//;0x02C4  SLI177
    (fp)INT_Excep_PERI_SLI177,
//;0x02C8  SLI178
    (fp)INT_Excep_PERI_SLI178,
//;0x02CC  SLI179
    (fp)INT_Excep_PERI_SLI179,
//;0x02D0  SLI180
    (fp)INT_Excep_PERI_SLI180,
//;0x02D4  SLI181
    (fp)INT_Excep_PERI_SLI181,
//;0x02D8  SLI182
    (fp)INT_Excep_PERI_SLI182,
//;0x02DC  SLI183
    (fp)INT_Excep_PERI_SLI183,
//;0x02E0  SLI184
    (fp)INT_Excep_PERI_SLI184,
//;0x02E4  SLI185
    (fp)INT_Excep_PERI_SLI185,
//;0x02E8  SLI186
    (fp)INT_Excep_PERI_SLI186,
//;0x02EC  SLI187
    (fp)INT_Excep_PERI_SLI187,
//;0x02F0  SLI188
    (fp)INT_Excep_PERI_SLI188,
//;0x02F4  SLI189
    (fp)INT_Excep_PERI_SLI189,
//;0x02F8  SLI190
    (fp)INT_Excep_PERI_SLI190,
//;0x02FC  SLI191
    (fp)INT_Excep_PERI_SLI191,
//;0x0300  SLI192
    (fp)INT_Excep_PERI_SLI192,
//;0x0304  SLI193
    (fp)INT_Excep_PERI_SLI193,
//;0x0308  SLI194
    (fp)INT_Excep_PERI_SLI194,
//;0x030C  SLI195
    (fp)INT_Excep_PERI_SLI195,
//;0x0310  SLI196
    (fp)INT_Excep_PERI_SLI196,
//;0x0314  SLI197
    (fp)INT_Excep_PERI_SLI197,
//;0x0318  SLI198
    (fp)INT_Excep_PERI_SLI198,
//;0x031C  SLI199
    (fp)INT_Excep_PERI_SLI199,
//;0x0320  SLI200
    (fp)INT_Excep_PERI_SLI200,
//;0x0324  SLI201
    (fp)INT_Excep_PERI_SLI201,
//;0x0328  SLI202
    (fp)INT_Excep_PERI_SLI202,
//;0x032C  SLI203
    (fp)INT_Excep_PERI_SLI203,
//;0x0320  SLI204
    (fp)INT_Excep_PERI_SLI204,
//;0x0334  SLI205
    (fp)INT_Excep_PERI_SLI205,
//;0x0338  SLI206
    (fp)INT_Excep_PERI_SLI206,
//;0x033C  SLI207
    (fp)INT_Excep_PERI_SLI207,
//;0x0340  SLI208
    (fp)INT_Excep_PERI_SLI208,
//;0x0344  SLI209
    (fp)INT_Excep_PERI_SLI209,
//;0x0348  SLI210
    (fp)INT_Excep_PERI_SLI210,
//;0x034C  SLI211
    (fp)INT_Excep_PERI_SLI211,
//;0x0350  SLI212
    (fp)INT_Excep_PERI_SLI212,
//;0x0354  SLI213
    (fp)INT_Excep_PERI_SLI213,
//;0x0358  SLI214
    (fp)INT_Excep_PERI_SLI214,
//;0x035C  SLI215
    (fp)INT_Excep_PERI_SLI215,
//;0x0360  SLI216
    (fp)INT_Excep_PERI_SLI216,
//;0x0364  SLI217
    (fp)INT_Excep_PERI_SLI217,
//;0x0368  SLI218
    (fp)INT_Excep_PERI_SLI218,
//;0x036C  SLI219
    (fp)INT_Excep_PERI_SLI219,
//;0x0370  SLI220
    (fp)INT_Excep_PERI_SLI220,
//;0x0374  SLI221
    (fp)INT_Excep_PERI_SLI221,
//;0x0378  SLI222
    (fp)INT_Excep_PERI_SLI222,
//;0x037C  SLI223
    (fp)INT_Excep_PERI_SLI223,
//;0x0380  SLI224
    (fp)INT_Excep_PERI_SLI224,
//;0x0384  SLI225
    (fp)INT_Excep_PERI_SLI225,
//;0x0388  SLI226
    (fp)INT_Excep_PERI_SLI226,
//;0x038C  SLI227
    (fp)INT_Excep_PERI_SLI227,
//;0x0390  SLI228
    (fp)INT_Excep_PERI_SLI228,
//;0x0394  SLI229
    (fp)INT_Excep_PERI_SLI229,
//;0x0398  SLI230
    (fp)INT_Excep_PERI_SLI230,
//;0x039C  SLI231
    (fp)INT_Excep_PERI_SLI231,
//;0x03A0  SLI232
    (fp)INT_Excep_PERI_SLI232,
//;0x03A4  SLI233
    (fp)INT_Excep_PERI_SLI233,
//;0x03A8  SLI234
    (fp)INT_Excep_PERI_SLI234,
//;0x03AC  SLI235
    (fp)INT_Excep_PERI_SLI235,
//;0x03B0  SLI236
    (fp)INT_Excep_PERI_SLI236,
//;0x04B4  SLI237
    (fp)INT_Excep_PERI_SLI237,
//;0x03B8  SLI238
    (fp)INT_Excep_PERI_SLI238,
//;0x03BC  SLI239
    (fp)INT_Excep_PERI_SLI239,
//;0x03C0  SLI240
    (fp)INT_Excep_PERI_SLI240,
//;0x03C4  SLI241
    (fp)INT_Excep_PERI_SLI241,
//;0x03C8  SLI242
    (fp)INT_Excep_PERI_SLI242,
//;0x03CC  SLI243
    (fp)INT_Excep_PERI_SLI243,
//;0x03D0  SLI244
    (fp)INT_Excep_PERI_SLI244,
//;0x03D4  SLI245
    (fp)INT_Excep_PERI_SLI245,
//;0x03D8  SLI246
    (fp)INT_Excep_PERI_SLI246,
//;0x03DC  SLI247
    (fp)INT_Excep_PERI_SLI247,
//;0x03E0  SLI248
    (fp)INT_Excep_PERI_SLI248,
//;0x03E4  SLI249
    (fp)INT_Excep_PERI_SLI249,
//;0x03E8  SLI250
    (fp)INT_Excep_PERI_SLI250,
//;0x03EC  SLI251
    (fp)INT_Excep_PERI_SLI251,
//;0x03F0  SLI252
    (fp)INT_Excep_PERI_SLI252,
//;0x03F4  SLI253
    (fp)INT_Excep_PERI_SLI253,
//;0x03F8  SLI254
    (fp)INT_Excep_PERI_SLI254,
//;0x03FC  SLI255
    (fp)INT_Excep_PERI_SLI255,
};
