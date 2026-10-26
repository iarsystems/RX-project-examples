/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer *
* Copyright (C) 2009 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/

/*---------------------------------------------------------------------------*/
/* fixmath.h: Fixed-point Mathematics Library (Ver. 1.01)                    */
/*---------------------------------------------------------------------------*/

/* Type definitions.                                                         */

/* Type FIXnn: Sign: 1 bit                                                   */
/*             Integral part: 31-nn bits                                     */
/*             Fractional part: nn bits                                      */

/* Following types support function library.                                 */
typedef long FIX16, FIX24, FIX29;

/* Following types support only multiplication.                              */
typedef long FIX1, FIX2, FIX3, FIX4, FIX5, FIX6, FIX7, FIX8, FIX9, FIX10,
             FIX11, FIX12, FIX13, FIX14, FIX15, FIX17, FIX18, FIX19, FIX20,
             FIX21, FIX22, FIX23, FIX25, FIX26, FIX27, FIX28, FIX30, FIX31;
typedef long FIX;

/* Conversion macros between floating point.                                 */

#define FIX1_tofloat(x) ((float)(x)/2.0f)
#define FIX2_tofloat(x) ((float)(x)/4.0f)
#define FIX3_tofloat(x) ((float)(x)/8.0f)
#define FIX4_tofloat(x) ((float)(x)/16.0f)
#define FIX5_tofloat(x) ((float)(x)/32.0f)
#define FIX6_tofloat(x) ((float)(x)/64.0f)
#define FIX7_tofloat(x) ((float)(x)/128.0f)
#define FIX8_tofloat(x) ((float)(x)/256.0f)
#define FIX9_tofloat(x) ((float)(x)/512.0f)
#define FIX10_tofloat(x) ((float)(x)/1024.0f)
#define FIX11_tofloat(x) ((float)(x)/2048.0f)
#define FIX12_tofloat(x) ((float)(x)/4096.0f)
#define FIX13_tofloat(x) ((float)(x)/8192.0f)
#define FIX14_tofloat(x) ((float)(x)/16384.0f)
#define FIX15_tofloat(x) ((float)(x)/32768.0f)
#define FIX16_tofloat(x) ((float)(x)/65536.0f)
#define FIX17_tofloat(x) ((float)(x)/131072.0f)
#define FIX18_tofloat(x) ((float)(x)/262144.0f)
#define FIX19_tofloat(x) ((float)(x)/524288.0f)
#define FIX20_tofloat(x) ((float)(x)/1048576.0f)
#define FIX21_tofloat(x) ((float)(x)/2097152.0f)
#define FIX22_tofloat(x) ((float)(x)/4194304.0f)
#define FIX23_tofloat(x) ((float)(x)/8388608.0f)
#define FIX24_tofloat(x) ((float)(x)/16777216.0f)
#define FIX25_tofloat(x) ((float)(x)/33554432.0f)
#define FIX26_tofloat(x) ((float)(x)/67108864.0f)
#define FIX27_tofloat(x) ((float)(x)/134217728.0f)
#define FIX28_tofloat(x) ((float)(x)/268435456.0f)
#define FIX29_tofloat(x) ((float)(x)/536870912.0f)
#define FIX30_tofloat(x) ((float)(x)/1073741824.0f)
#define FIX31_tofloat(x) ((float)(x)/2147483648.0f)

#define FIX1_fromfloat(x) ((FIX1)((x)*2.0f))
#define FIX2_fromfloat(x) ((FIX2)((x)*4.0f))
#define FIX3_fromfloat(x) ((FIX3)((x)*8.0f))
#define FIX4_fromfloat(x) ((FIX4)((x)*16.0f))
#define FIX5_fromfloat(x) ((FIX5)((x)*32.0f))
#define FIX6_fromfloat(x) ((FIX6)((x)*64.0f))
#define FIX7_fromfloat(x) ((FIX7)((x)*128.0f))
#define FIX8_fromfloat(x) ((FIX8)((x)*256.0f))
#define FIX9_fromfloat(x) ((FIX9)((x)*512.0f))
#define FIX10_fromfloat(x) ((FIX10)((x)*1024.0f))
#define FIX11_fromfloat(x) ((FIX11)((x)*2048.0f))
#define FIX12_fromfloat(x) ((FIX12)((x)*4096.0f))
#define FIX13_fromfloat(x) ((FIX13)((x)*8192.0f))
#define FIX14_fromfloat(x) ((FIX14)((x)*16384.0f))
#define FIX15_fromfloat(x) ((FIX15)((x)*32768.0f))
#define FIX16_fromfloat(x) ((FIX16)((x)*65536.0f))
#define FIX17_fromfloat(x) ((FIX17)((x)*131072.0f))
#define FIX18_fromfloat(x) ((FIX18)((x)*262144.0f))
#define FIX19_fromfloat(x) ((FIX19)((x)*524288.0f))
#define FIX20_fromfloat(x) ((FIX20)((x)*1048576.0f))
#define FIX21_fromfloat(x) ((FIX21)((x)*2097152.0f))
#define FIX22_fromfloat(x) ((FIX22)((x)*4194304.0f))
#define FIX23_fromfloat(x) ((FIX23)((x)*8388608.0f))
#define FIX24_fromfloat(x) ((FIX24)((x)*16777216.0f))
#define FIX25_fromfloat(x) ((FIX25)((x)*33554432.0f))
#define FIX26_fromfloat(x) ((FIX26)((x)*67108864.0f))
#define FIX27_fromfloat(x) ((FIX27)((x)*134217728.0f))
#define FIX28_fromfloat(x) ((FIX28)((x)*268435456.0f))
#define FIX29_fromfloat(x) ((FIX29)((x)*536870912.0f))
#define FIX30_fromfloat(x) ((FIX30)((x)*1073741824.0f))
#define FIX31_fromfloat(x) ((FIX31)((x)*2147483648.0f))


/* Conversion macros between floating point.  to double and from double	     */

#define FIX1_todouble(x) ((double)(x)/2.0)
#define FIX2_todouble(x) ((double)(x)/4.0)
#define FIX3_todouble(x) ((double)(x)/8.0)
#define FIX4_todouble(x) ((double)(x)/16.0)
#define FIX5_todouble(x) ((double)(x)/32.0)
#define FIX6_todouble(x) ((double)(x)/64.0)
#define FIX7_todouble(x) ((double)(x)/128.0)
#define FIX8_todouble(x) ((double)(x)/256.0)
#define FIX9_todouble(x) ((double)(x)/512.0)
#define FIX10_todouble(x) ((double)(x)/1024.0)
#define FIX11_todouble(x) ((double)(x)/2048.0)
#define FIX12_todouble(x) ((double)(x)/4096.0)
#define FIX13_todouble(x) ((double)(x)/8192.0)
#define FIX14_todouble(x) ((double)(x)/16384.0)
#define FIX15_todouble(x) ((double)(x)/32768.0)
#define FIX16_todouble(x) ((double)(x)/65536.0)
#define FIX17_todouble(x) ((double)(x)/131072.0)
#define FIX18_todouble(x) ((double)(x)/262144.0)
#define FIX19_todouble(x) ((double)(x)/524288.0)
#define FIX20_todouble(x) ((double)(x)/1048576.0)
#define FIX21_todouble(x) ((double)(x)/2097152.0)
#define FIX22_todouble(x) ((double)(x)/4194304.0)
#define FIX23_todouble(x) ((double)(x)/8388608.0)
#define FIX24_todouble(x) ((double)(x)/16777216.0)
#define FIX25_todouble(x) ((double)(x)/33554432.0)
#define FIX26_todouble(x) ((double)(x)/67108864.0)
#define FIX27_todouble(x) ((double)(x)/134217728.0)
#define FIX28_todouble(x) ((double)(x)/268435456.0)
#define FIX29_todouble(x) ((double)(x)/536870912.0)
#define FIX30_todouble(x) ((double)(x)/1073741824.0)
#define FIX31_todouble(x) ((double)(x)/2147483648.0)

#define FIX1_fromdouble(x) ((FIX1)((x)*2.0))
#define FIX2_fromdouble(x) ((FIX2)((x)*4.0))
#define FIX3_fromdouble(x) ((FIX3)((x)*8.0))
#define FIX4_fromdouble(x) ((FIX4)((x)*16.0))
#define FIX5_fromdouble(x) ((FIX5)((x)*32.0))
#define FIX6_fromdouble(x) ((FIX6)((x)*64.0))
#define FIX7_fromdouble(x) ((FIX7)((x)*128.0))
#define FIX8_fromdouble(x) ((FIX8)((x)*256.0))
#define FIX9_fromdouble(x) ((FIX9)((x)*512.0))
#define FIX10_fromdouble(x) ((FIX10)((x)*1024.0))
#define FIX11_fromdouble(x) ((FIX11)((x)*2048.0))
#define FIX12_fromdouble(x) ((FIX12)((x)*4096.0))
#define FIX13_fromdouble(x) ((FIX13)((x)*8192.0))
#define FIX14_fromdouble(x) ((FIX14)((x)*16384.0))
#define FIX15_fromdouble(x) ((FIX15)((x)*32768.0))
#define FIX16_fromdouble(x) ((FIX16)((x)*65536.0))
#define FIX17_fromdouble(x) ((FIX17)((x)*131072.0))
#define FIX18_fromdouble(x) ((FIX18)((x)*262144.0))
#define FIX19_fromdouble(x) ((FIX19)((x)*524288.0))
#define FIX20_fromdouble(x) ((FIX20)((x)*1048576.0))
#define FIX21_fromdouble(x) ((FIX21)((x)*2097152.0))
#define FIX22_fromdouble(x) ((FIX22)((x)*4194304.0))
#define FIX23_fromdouble(x) ((FIX23)((x)*8388608.0))
#define FIX24_fromdouble(x) ((FIX24)((x)*16777216.0))
#define FIX25_fromdouble(x) ((FIX25)((x)*33554432.0))
#define FIX26_fromdouble(x) ((FIX26)((x)*67108864.0))
#define FIX27_fromdouble(x) ((FIX27)((x)*134217728.0))
#define FIX28_fromdouble(x) ((FIX28)((x)*268435456.0))
#define FIX29_fromdouble(x) ((FIX29)((x)*536870912.0))
#define FIX30_fromdouble(x) ((FIX30)((x)*1073741824.0))
#define FIX31_fromdouble(x) ((FIX31)((x)*2147483648.0))


/* Short multiplication macros.                                              */
/* Multiplication with 32-bit intermediate result.                           */

#define FIX1_mul_short(x, y) ((FIX1)((x)*(y)>>1))
#define FIX2_mul_short(x, y) ((FIX2)((x)*(y)>>2))
#define FIX3_mul_short(x, y) ((FIX3)((x)*(y)>>3))
#define FIX4_mul_short(x, y) ((FIX4)((x)*(y)>>4))
#define FIX5_mul_short(x, y) ((FIX5)((x)*(y)>>5))
#define FIX6_mul_short(x, y) ((FIX6)((x)*(y)>>6))
#define FIX7_mul_short(x, y) ((FIX7)((x)*(y)>>7))
#define FIX8_mul_short(x, y) ((FIX8)((x)*(y)>>8))
#define FIX9_mul_short(x, y) ((FIX9)((x)*(y)>>9))
#define FIX10_mul_short(x, y) ((FIX10)((x)*(y)>>10))
#define FIX11_mul_short(x, y) ((FIX11)((x)*(y)>>11))
#define FIX12_mul_short(x, y) ((FIX12)((x)*(y)>>12))
#define FIX13_mul_short(x, y) ((FIX13)((x)*(y)>>13))
#define FIX14_mul_short(x, y) ((FIX14)((x)*(y)>>14))
#define FIX15_mul_short(x, y) ((FIX15)((x)*(y)>>15))
#define FIX16_mul_short(x, y) ((FIX16)((x)*(y)>>16))
#define FIX17_mul_short(x, y) ((FIX17)((x)*(y)>>17))
#define FIX18_mul_short(x, y) ((FIX18)((x)*(y)>>18))
#define FIX19_mul_short(x, y) ((FIX19)((x)*(y)>>19))
#define FIX20_mul_short(x, y) ((FIX20)((x)*(y)>>20))
#define FIX21_mul_short(x, y) ((FIX21)((x)*(y)>>21))
#define FIX22_mul_short(x, y) ((FIX22)((x)*(y)>>22))
#define FIX23_mul_short(x, y) ((FIX23)((x)*(y)>>23))
#define FIX24_mul_short(x, y) ((FIX24)((x)*(y)>>24))
#define FIX25_mul_short(x, y) ((FIX25)((x)*(y)>>25))
#define FIX26_mul_short(x, y) ((FIX26)((x)*(y)>>26))
#define FIX27_mul_short(x, y) ((FIX27)((x)*(y)>>27))
#define FIX28_mul_short(x, y) ((FIX28)((x)*(y)>>28))
#define FIX29_mul_short(x, y) ((FIX29)((x)*(y)>>29))
#define FIX30_mul_short(x, y) ((FIX30)((x)*(y)>>30))
#define FIX31_mul_short(x, y) ((FIX31)((x)*(y)>>31))

/* Division macros.                                                          */

#define FIX1_div(x, y) ((FIX1)(((long long)(x)<<1)/(y)))
#define FIX2_div(x, y) ((FIX2)(((long long)(x)<<2)/(y)))
#define FIX3_div(x, y) ((FIX3)(((long long)(x)<<3)/(y)))
#define FIX4_div(x, y) ((FIX4)(((long long)(x)<<4)/(y)))
#define FIX5_div(x, y) ((FIX5)(((long long)(x)<<5)/(y)))
#define FIX6_div(x, y) ((FIX6)(((long long)(x)<<6)/(y)))
#define FIX7_div(x, y) ((FIX7)(((long long)(x)<<7)/(y)))
#define FIX8_div(x, y) ((FIX8)(((long long)(x)<<8)/(y)))
#define FIX9_div(x, y) ((FIX9)(((long long)(x)<<9)/(y)))
#define FIX10_div(x, y) ((FIX10)(((long long)(x)<<10)/(y)))
#define FIX11_div(x, y) ((FIX11)(((long long)(x)<<11)/(y)))
#define FIX12_div(x, y) ((FIX12)(((long long)(x)<<12)/(y)))
#define FIX13_div(x, y) ((FIX13)(((long long)(x)<<13)/(y)))
#define FIX14_div(x, y) ((FIX14)(((long long)(x)<<14)/(y)))
#define FIX15_div(x, y) ((FIX15)(((long long)(x)<<15)/(y)))
#define FIX16_div(x, y) ((FIX16)(((long long)(x)<<16)/(y)))
#define FIX17_div(x, y) ((FIX17)(((long long)(x)<<17)/(y)))
#define FIX18_div(x, y) ((FIX18)(((long long)(x)<<18)/(y)))
#define FIX19_div(x, y) ((FIX19)(((long long)(x)<<19)/(y)))
#define FIX20_div(x, y) ((FIX20)(((long long)(x)<<20)/(y)))
#define FIX21_div(x, y) ((FIX21)(((long long)(x)<<21)/(y)))
#define FIX22_div(x, y) ((FIX22)(((long long)(x)<<22)/(y)))
#define FIX23_div(x, y) ((FIX23)(((long long)(x)<<23)/(y)))
#define FIX24_div(x, y) ((FIX24)(((long long)(x)<<24)/(y)))
#define FIX25_div(x, y) ((FIX25)(((long long)(x)<<25)/(y)))
#define FIX26_div(x, y) ((FIX26)(((long long)(x)<<26)/(y)))
#define FIX27_div(x, y) ((FIX27)(((long long)(x)<<27)/(y)))
#define FIX28_div(x, y) ((FIX28)(((long long)(x)<<28)/(y)))
#define FIX29_div(x, y) ((FIX29)(((long long)(x)<<29)/(y)))
#define FIX30_div(x, y) ((FIX30)(((long long)(x)<<30)/(y)))
#define FIX31_div(x, y) ((FIX31)(((long long)(x)<<31)/(y)))

/* Multiplication routines                                                   */
/* Multiplication with 64-bit intermediate result.                           */

#if defined(RX) || defined(MACOS) || defined(__ICCRX__)
#define FIX1_mul(x, y)	(FIX1)FIX_mul((long)x, (long)y, 1)
#define FIX2_mul(x, y)	(FIX2)FIX_mul((long)x, (long)y, 2)
#define FIX3_mul(x, y)	(FIX3)FIX_mul((long)x, (long)y, 3)
#define FIX4_mul(x, y)	(FIX4)FIX_mul((long)x, (long)y, 4)
#define FIX5_mul(x, y)	(FIX5)FIX_mul((long)x, (long)y, 5)
#define FIX6_mul(x, y)	(FIX6)FIX_mul((long)x, (long)y, 6)
#define FIX7_mul(x, y)	(FIX7)FIX_mul((long)x, (long)y, 7)
#define FIX8_mul(x, y)	(FIX8)FIX_mul((long)x, (long)y, 8)
#define FIX9_mul(x, y)	(FIX9)FIX_mul((long)x, (long)y, 9)
#define FIX10_mul(x, y)	(FIX10)FIX_mul((long)x, (long)y, 10)
#define FIX11_mul(x, y)	(FIX11)FIX_mul((long)x, (long)y, 11)
#define FIX12_mul(x, y)	(FIX12)FIX_mul((long)x, (long)y, 12)
#define FIX13_mul(x, y)	(FIX13)FIX_mul((long)x, (long)y, 13)
#define FIX14_mul(x, y)	(FIX14)FIX_mul((long)x, (long)y, 14)
#define FIX15_mul(x, y)	(FIX15)FIX_mul((long)x, (long)y, 15)
#define FIX16_mul(x, y)	(FIX16)FIX_mul((long)x, (long)y, 16)
#define FIX17_mul(x, y)	(FIX17)FIX_mul((long)x, (long)y, 17)
#define FIX18_mul(x, y)	(FIX18)FIX_mul((long)x, (long)y, 18)
#define FIX19_mul(x, y)	(FIX19)FIX_mul((long)x, (long)y, 19)
#define FIX20_mul(x, y)	(FIX20)FIX_mul((long)x, (long)y, 20)
#define FIX21_mul(x, y)	(FIX21)FIX_mul((long)x, (long)y, 21)
#define FIX22_mul(x, y)	(FIX22)FIX_mul((long)x, (long)y, 22)
#define FIX23_mul(x, y)	(FIX23)FIX_mul((long)x, (long)y, 23)
#define FIX24_mul(x, y)	(FIX24)FIX_mul((long)x, (long)y, 24)
#define FIX25_mul(x, y)	(FIX25)FIX_mul((long)x, (long)y, 25)
#define FIX26_mul(x, y)	(FIX26)FIX_mul((long)x, (long)y, 26)
#define FIX27_mul(x, y)	(FIX27)FIX_mul((long)x, (long)y, 27)
#define FIX28_mul(x, y)	(FIX28)FIX_mul((long)x, (long)y, 28)
#define FIX29_mul(x, y)	(FIX29)FIX_mul((long)x, (long)y, 29)
#define FIX30_mul(x, y)	(FIX30)FIX_mul((long)x, (long)y, 30)
#define FIX31_mul(x, y)	(FIX31)FIX_mul((long)x, (long)y, 31)

#if defined(RX) || defined(__ICCRX__)
static FIX FIX_mul(FIX, FIX, int);

static
FIX FIX_mul(FIX x , FIX y , int n)
{
  asm("emul	r2, r1");
  asm("shlr	r3, r1");
  asm("neg	r3");
  asm("shll	r3, r2");
  asm("or	r2, r1");
  return x;      
}
#endif

#else
extern FIX1 FIX1_mul(FIX1 x, FIX1 y);
extern FIX2 FIX2_mul(FIX2 x, FIX2 y);
extern FIX3 FIX3_mul(FIX3 x, FIX3 y);
extern FIX4 FIX4_mul(FIX4 x, FIX4 y);
extern FIX5 FIX5_mul(FIX5 x, FIX5 y);
extern FIX6 FIX6_mul(FIX6 x, FIX6 y);
extern FIX7 FIX7_mul(FIX7 x, FIX7 y);
extern FIX8 FIX8_mul(FIX8 x, FIX8 y);
extern FIX9 FIX9_mul(FIX9 x, FIX9 y);
extern FIX10 FIX10_mul(FIX10 x, FIX10 y);
extern FIX11 FIX11_mul(FIX11 x, FIX11 y);
extern FIX12 FIX12_mul(FIX12 x, FIX12 y);
extern FIX13 FIX13_mul(FIX13 x, FIX13 y);
extern FIX14 FIX14_mul(FIX14 x, FIX14 y);
extern FIX15 FIX15_mul(FIX15 x, FIX15 y);
extern FIX16 FIX16_mul(FIX16 x, FIX16 y);
extern FIX17 FIX17_mul(FIX17 x, FIX17 y);
extern FIX18 FIX18_mul(FIX18 x, FIX18 y);
extern FIX19 FIX19_mul(FIX19 x, FIX19 y);
extern FIX20 FIX20_mul(FIX20 x, FIX20 y);
extern FIX21 FIX21_mul(FIX21 x, FIX21 y);
extern FIX22 FIX22_mul(FIX22 x, FIX22 y);
extern FIX23 FIX23_mul(FIX23 x, FIX23 y);
extern FIX24 FIX24_mul(FIX24 x, FIX24 y);
extern FIX25 FIX25_mul(FIX25 x, FIX25 y);
extern FIX26 FIX26_mul(FIX26 x, FIX26 y);
extern FIX27 FIX27_mul(FIX27 x, FIX27 y);
extern FIX28 FIX28_mul(FIX28 x, FIX28 y);
extern FIX29 FIX29_mul(FIX29 x, FIX29 y);
extern FIX30 FIX30_mul(FIX30 x, FIX30 y);
extern FIX31 FIX31_mul(FIX31 x, FIX31 y);
#endif

/* Multiplication with fractional part result.                               */

#define FIX1_mul_frac(x, y) (FIX1_mul_short((x), (y))&0x1)
#define FIX2_mul_frac(x, y) (FIX2_mul_short((x), (y))&0x3)
#define FIX3_mul_frac(x, y) (FIX3_mul_short((x), (y))&0x7)
#define FIX4_mul_frac(x, y) (FIX4_mul_short((x), (y))&0xf)
#define FIX5_mul_frac(x, y) (FIX5_mul_short((x), (y))&0x1f)
#define FIX6_mul_frac(x, y) (FIX6_mul_short((x), (y))&0x3f)
#define FIX7_mul_frac(x, y) (FIX7_mul_short((x), (y))&0x7f)
#define FIX8_mul_frac(x, y) (FIX8_mul_short((x), (y))&0xff)
#define FIX9_mul_frac(x, y) (FIX9_mul_short((x), (y))&0x1ff)
#define FIX10_mul_frac(x, y) (FIX10_mul_short((x), (y))&0x3ff)
#define FIX11_mul_frac(x, y) (FIX11_mul_short((x), (y))&0x7ff)
#define FIX12_mul_frac(x, y) (FIX12_mul_short((x), (y))&0xfff)
#define FIX13_mul_frac(x, y) (FIX13_mul_short((x), (y))&0x1fff)
#define FIX14_mul_frac(x, y) (FIX14_mul_short((x), (y))&0x3fff)
#define FIX15_mul_frac(x, y) (FIX15_mul_short((x), (y))&0x7fff)
#define FIX16_mul_frac(x, y) (FIX16_mul_short((x), (y))&0xffff)
#define FIX17_mul_frac(x, y) (FIX17_mul((x), (y))&0x1ffff)
#define FIX18_mul_frac(x, y) (FIX18_mul((x), (y))&0x3ffff)
#define FIX19_mul_frac(x, y) (FIX19_mul((x), (y))&0x7ffff)
#define FIX20_mul_frac(x, y) (FIX20_mul((x), (y))&0xfffff)
#define FIX21_mul_frac(x, y) (FIX21_mul((x), (y))&0x1fffff)
#define FIX22_mul_frac(x, y) (FIX22_mul((x), (y))&0x3fffff)
#define FIX23_mul_frac(x, y) (FIX23_mul((x), (y))&0x7fffff)
#define FIX24_mul_frac(x, y) (FIX24_mul((x), (y))&0xffffff)
#define FIX25_mul_frac(x, y) (FIX25_mul((x), (y))&0x1ffffff)
#define FIX26_mul_frac(x, y) (FIX26_mul((x), (y))&0x3ffffff)
#define FIX27_mul_frac(x, y) (FIX27_mul((x), (y))&0x7ffffff)
#define FIX28_mul_frac(x, y) (FIX28_mul((x), (y))&0xfffffff)
#define FIX29_mul_frac(x, y) (FIX29_mul((x), (y))&0x1fffffff)
#define FIX30_mul_frac(x, y) (FIX30_mul((x), (y))&0x3fffffff)
#define FIX31_mul_frac(x, y) (FIX31_mul((x), (y))&0x7fffffff)

/* Multiplication with 64-bit intermediate result, with saturated result.    */

#define FIX1_mul_sat(x, y) (FIX_mul_sat_scale((x), (y), 1))
#define FIX2_mul_sat(x, y) (FIX_mul_sat_scale((x), (y), 2))
#define FIX3_mul_sat(x, y) (FIX_mul_sat_scale((x), (y), 3))
#define FIX4_mul_sat(x, y) (FIX_mul_sat_scale((x), (y), 4))
#define FIX5_mul_sat(x, y) (FIX_mul_sat_scale((x), (y), 5))
#define FIX6_mul_sat(x, y) (FIX_mul_sat_scale((x), (y), 6))
#define FIX7_mul_sat(x, y) (FIX_mul_sat_scale((x), (y), 7))
#define FIX8_mul_sat(x, y) (FIX_mul_sat_scale((x), (y), 8))
#define FIX9_mul_sat(x, y) (FIX_mul_sat_scale((x), (y), 9))
#define FIX10_mul_sat(x, y) (FIX_mul_sat_scale((x), (y), 10))
#define FIX11_mul_sat(x, y) (FIX_mul_sat_scale((x), (y), 11))
#define FIX12_mul_sat(x, y) (FIX_mul_sat_scale((x), (y), 12))
#define FIX13_mul_sat(x, y) (FIX_mul_sat_scale((x), (y), 13))
#define FIX14_mul_sat(x, y) (FIX_mul_sat_scale((x), (y), 14))
#define FIX15_mul_sat(x, y) (FIX_mul_sat_scale((x), (y), 15))
#define FIX16_mul_sat(x, y) (FIX_mul_sat_scale((x), (y), 16))
#define FIX17_mul_sat(x, y) (FIX_mul_sat_scale((x), (y), 17))
#define FIX18_mul_sat(x, y) (FIX_mul_sat_scale((x), (y), 18))
#define FIX19_mul_sat(x, y) (FIX_mul_sat_scale((x), (y), 19))
#define FIX20_mul_sat(x, y) (FIX_mul_sat_scale((x), (y), 20))
#define FIX21_mul_sat(x, y) (FIX_mul_sat_scale((x), (y), 21))
#define FIX22_mul_sat(x, y) (FIX_mul_sat_scale((x), (y), 22))
#define FIX23_mul_sat(x, y) (FIX_mul_sat_scale((x), (y), 23))
#define FIX24_mul_sat(x, y) (FIX_mul_sat_scale((x), (y), 24))
#define FIX25_mul_sat(x, y) (FIX_mul_sat_scale((x), (y), 25))
#define FIX26_mul_sat(x, y) (FIX_mul_sat_scale((x), (y), 26))
#define FIX27_mul_sat(x, y) (FIX_mul_sat_scale((x), (y), 27))
#define FIX28_mul_sat(x, y) (FIX_mul_sat_scale((x), (y), 28))
#define FIX29_mul_sat(x, y) (FIX_mul_sat_scale((x), (y), 29))
#define FIX30_mul_sat(x, y) (FIX_mul_sat_scale((x), (y), 30))
#define FIX31_mul_sat(x, y) (FIX_mul_sat_scale((x), (y), 31))

/* Generic fixed-point routines.                                             */

#define FIX_mul_scale1(x, y) (FIX1_mul((x), (y)))
#define FIX_mul_scale2(x, y) (FIX2_mul((x), (y)))
#define FIX_mul_scale3(x, y) (FIX3_mul((x), (y)))
#define FIX_mul_scale4(x, y) (FIX4_mul((x), (y)))
#define FIX_mul_scale5(x, y) (FIX5_mul((x), (y)))
#define FIX_mul_scale6(x, y) (FIX6_mul((x), (y)))
#define FIX_mul_scale7(x, y) (FIX7_mul((x), (y)))
#define FIX_mul_scale8(x, y) (FIX8_mul((x), (y)))
#define FIX_mul_scale9(x, y) (FIX9_mul((x), (y)))
#define FIX_mul_scale10(x, y) (FIX10_mul((x), (y)))
#define FIX_mul_scale11(x, y) (FIX11_mul((x), (y)))
#define FIX_mul_scale12(x, y) (FIX12_mul((x), (y)))
#define FIX_mul_scale13(x, y) (FIX13_mul((x), (y)))
#define FIX_mul_scale14(x, y) (FIX14_mul((x), (y)))
#define FIX_mul_scale15(x, y) (FIX15_mul((x), (y)))
#define FIX_mul_scale16(x, y) (FIX16_mul((x), (y)))
#define FIX_mul_scale17(x, y) (FIX17_mul((x), (y)))
#define FIX_mul_scale18(x, y) (FIX18_mul((x), (y)))
#define FIX_mul_scale19(x, y) (FIX19_mul((x), (y)))
#define FIX_mul_scale20(x, y) (FIX20_mul((x), (y)))
#define FIX_mul_scale21(x, y) (FIX21_mul((x), (y)))
#define FIX_mul_scale22(x, y) (FIX22_mul((x), (y)))
#define FIX_mul_scale23(x, y) (FIX23_mul((x), (y)))
#define FIX_mul_scale24(x, y) (FIX24_mul((x), (y)))
#define FIX_mul_scale25(x, y) (FIX25_mul((x), (y)))
#define FIX_mul_scale26(x, y) (FIX26_mul((x), (y)))
#define FIX_mul_scale27(x, y) (FIX27_mul((x), (y)))
#define FIX_mul_scale28(x, y) (FIX28_mul((x), (y)))
#define FIX_mul_scale29(x, y) (FIX29_mul((x), (y)))
#define FIX_mul_scale30(x, y) (FIX30_mul((x), (y)))
#define FIX_mul_scale31(x, y) (FIX31_mul((x), (y)))

extern FIX FIX_mul_scale(FIX x, FIX y, int n);

#define FIX_mul_frac_scale1(x, y) (FIX1_mul_frac((x), (y)))
#define FIX_mul_frac_scale2(x, y) (FIX2_mul_frac((x), (y)))
#define FIX_mul_frac_scale3(x, y) (FIX3_mul_frac((x), (y)))
#define FIX_mul_frac_scale4(x, y) (FIX4_mul_frac((x), (y)))
#define FIX_mul_frac_scale5(x, y) (FIX5_mul_frac((x), (y)))
#define FIX_mul_frac_scale6(x, y) (FIX6_mul_frac((x), (y)))
#define FIX_mul_frac_scale7(x, y) (FIX7_mul_frac((x), (y)))
#define FIX_mul_frac_scale8(x, y) (FIX8_mul_frac((x), (y)))
#define FIX_mul_frac_scale9(x, y) (FIX9_mul_frac((x), (y)))
#define FIX_mul_frac_scale10(x, y) (FIX10_mul_frac((x), (y)))
#define FIX_mul_frac_scale11(x, y) (FIX11_mul_frac((x), (y)))
#define FIX_mul_frac_scale12(x, y) (FIX12_mul_frac((x), (y)))
#define FIX_mul_frac_scale13(x, y) (FIX13_mul_frac((x), (y)))
#define FIX_mul_frac_scale14(x, y) (FIX14_mul_frac((x), (y)))
#define FIX_mul_frac_scale15(x, y) (FIX15_mul_frac((x), (y)))
#define FIX_mul_frac_scale16(x, y) (FIX16_mul_frac((x), (y)))
#define FIX_mul_frac_scale17(x, y) (FIX17_mul_frac((x), (y)))
#define FIX_mul_frac_scale18(x, y) (FIX18_mul_frac((x), (y)))
#define FIX_mul_frac_scale19(x, y) (FIX19_mul_frac((x), (y)))
#define FIX_mul_frac_scale20(x, y) (FIX20_mul_frac((x), (y)))
#define FIX_mul_frac_scale21(x, y) (FIX21_mul_frac((x), (y)))
#define FIX_mul_frac_scale22(x, y) (FIX22_mul_frac((x), (y)))
#define FIX_mul_frac_scale23(x, y) (FIX23_mul_frac((x), (y)))
#define FIX_mul_frac_scale24(x, y) (FIX24_mul_frac((x), (y)))
#define FIX_mul_frac_scale25(x, y) (FIX25_mul_frac((x), (y)))
#define FIX_mul_frac_scale26(x, y) (FIX26_mul_frac((x), (y)))
#define FIX_mul_frac_scale27(x, y) (FIX27_mul_frac((x), (y)))
#define FIX_mul_frac_scale28(x, y) (FIX28_mul_frac((x), (y)))
#define FIX_mul_frac_scale29(x, y) (FIX29_mul_frac((x), (y)))
#define FIX_mul_frac_scale30(x, y) (FIX30_mul_frac((x), (y)))
#define FIX_mul_frac_scale31(x, y) (FIX31_mul_frac((x), (y)))

#define FIX_mul_frac_scale(x, y, n) (FIX_mul_scale((x), (y), (n))&((1<<n)-1))

#define FIX_mul_sat_scale1(x, y) (FIX_mul_sat_scale((x), (y), 1))
#define FIX_mul_sat_scale2(x, y) (FIX_mul_sat_scale((x), (y), 2))
#define FIX_mul_sat_scale3(x, y) (FIX_mul_sat_scale((x), (y), 3))
#define FIX_mul_sat_scale4(x, y) (FIX_mul_sat_scale((x), (y), 4))
#define FIX_mul_sat_scale5(x, y) (FIX_mul_sat_scale((x), (y), 5))
#define FIX_mul_sat_scale6(x, y) (FIX_mul_sat_scale((x), (y), 6))
#define FIX_mul_sat_scale7(x, y) (FIX_mul_sat_scale((x), (y), 7))
#define FIX_mul_sat_scale8(x, y) (FIX_mul_sat_scale((x), (y), 8))
#define FIX_mul_sat_scale9(x, y) (FIX_mul_sat_scale((x), (y), 9))
#define FIX_mul_sat_scale10(x, y) (FIX_mul_sat_scale((x), (y), 10))
#define FIX_mul_sat_scale11(x, y) (FIX_mul_sat_scale((x), (y), 11))
#define FIX_mul_sat_scale12(x, y) (FIX_mul_sat_scale((x), (y), 12))
#define FIX_mul_sat_scale13(x, y) (FIX_mul_sat_scale((x), (y), 13))
#define FIX_mul_sat_scale14(x, y) (FIX_mul_sat_scale((x), (y), 14))
#define FIX_mul_sat_scale15(x, y) (FIX_mul_sat_scale((x), (y), 15))
#define FIX_mul_sat_scale16(x, y) (FIX_mul_sat_scale((x), (y), 16))
#define FIX_mul_sat_scale17(x, y) (FIX_mul_sat_scale((x), (y), 17))
#define FIX_mul_sat_scale18(x, y) (FIX_mul_sat_scale((x), (y), 18))
#define FIX_mul_sat_scale19(x, y) (FIX_mul_sat_scale((x), (y), 19))
#define FIX_mul_sat_scale20(x, y) (FIX_mul_sat_scale((x), (y), 20))
#define FIX_mul_sat_scale21(x, y) (FIX_mul_sat_scale((x), (y), 21))
#define FIX_mul_sat_scale22(x, y) (FIX_mul_sat_scale((x), (y), 22))
#define FIX_mul_sat_scale23(x, y) (FIX_mul_sat_scale((x), (y), 23))
#define FIX_mul_sat_scale24(x, y) (FIX_mul_sat_scale((x), (y), 24))
#define FIX_mul_sat_scale25(x, y) (FIX_mul_sat_scale((x), (y), 25))
#define FIX_mul_sat_scale26(x, y) (FIX_mul_sat_scale((x), (y), 26))
#define FIX_mul_sat_scale27(x, y) (FIX_mul_sat_scale((x), (y), 27))
#define FIX_mul_sat_scale28(x, y) (FIX_mul_sat_scale((x), (y), 28))
#define FIX_mul_sat_scale29(x, y) (FIX_mul_sat_scale((x), (y), 29))
#define FIX_mul_sat_scale30(x, y) (FIX_mul_sat_scale((x), (y), 30))
#define FIX_mul_sat_scale31(x, y) (FIX_mul_sat_scale((x), (y), 31))

extern FIX FIX_mul_sat_scale(FIX x, FIX y, int n);

/* Mathematical functions.                                                   */

extern FIX16 FIX16_cos(FIX16 x);
extern FIX16 FIX16_sin(FIX16 x);
extern FIX16 FIX16_atan(FIX16 x);
extern FIX16 FIX16_sqrt(FIX16 x);

extern FIX24 FIX24_cos(FIX24 x);
extern FIX24 FIX24_sin(FIX24 x);
extern FIX24 FIX24_atan(FIX24 x);
extern FIX24 FIX24_sqrt(FIX24 x);

extern FIX29 FIX29_cos(FIX29 x);
extern FIX29 FIX29_sin(FIX29 x);
extern FIX29 FIX29_atan(FIX29 x);
extern FIX29 FIX29_sqrt(FIX29 x);
