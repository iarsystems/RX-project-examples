/* Adapted to IAR Embedded Workbench */
/* Copyright,2006 RENESAS SOLUTIONS CORPORATION ALL RIGHT RESERVED */


/* $Date$ */

#ifndef __r_cryptogram_h__
#define __r_cryptogram_h__

#if defined(MACOS) || defined(RX)
#include <stdint.h>
#else
#include "r_stdint.h"
#endif

#if !defined(M16C) && !defined(M32C)
#define near
#endif

void R_Sha1_Hash(uint8_t __data16*, uint8_t near *, uint32_t near); 

void R_Des_Keysch(uint8_t __data16*,  uint32_t near *);
void R_Des_Ecbenc(uint8_t __data16*, uint8_t near *, uint32_t near *, uint32_t near);
void R_Des_Ecbdec(uint8_t __data16*, uint8_t near *, uint32_t near *, uint32_t near);


#if defined(M16C) || defined(M32C)
#pragma PARAMETER R_Sha1_Hash(r1, r2, r3)
#endif

#endif