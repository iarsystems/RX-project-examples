/******************************************************************************
* DISCLAIMER
* Please refer to http://www.renesas.com/disclaimer
******************************************************************************/
/***********************************************************************/
/*                                                                     */
/*  FILE        :Global.h                                              */
/*  DATE        :Jan 12, 2008                                          */
/*  DESCRIPTION :Globally useful definitions for LCD Direct Drive      */
/*  CPU TYPE    :H8S/2378R                                             */
/*                                                                     */
/***********************************************************************/
/************************************************************************
Revision History
DD.MM.YYYY OSO-UID Description
01.22.2008 RTA-JMB Driver Rewrite to Parameterization
************************************************************************/
#ifndef GLOBAL_H
#define GLOBAL_H

/* Typedef fundemental data types that may change between compilers/families */
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>

/* Include the MCU sfr definitions */
#include <iorx62n.h>
/* Include platform level hardware definitions */
#include "hwsetup.h"

#ifdef MEM_DECL
#define EXTERN
#else
#define EXTERN extern
#endif

/* convert 'a' to a string */
#define TO_STR1(a) #a
#define TO_STR(a) TO_STR1(a)

/* Define Pin Macros to ease implementation (secondary macros _1, _2 used for expansion ONLY */
#define CAT(a,b) a ## b
#define MERGE2(a,b) CAT(a,b)

/* Set signal DDR to output */
#if defined __RX600 || defined(__ICCRX__)
#define DDRo2(port,pin) MERGE2(PORT,port) ## MERGE2(.DDR.BIT.B,pin) = 1;
#define DDRi2(port,pin) MERGE2(PORT,port) ## MERGE2(.DDR.BIT.B,pin) = 0;
#define PINo2(port,pin) MERGE2(PORT,port) ## MERGE2(.DR.BIT.B,pin)
#define PINi2(port,pin) MERGE2(PORT,port) ## MERGE2(.PORT.BIT.B,pin)
#define ICRi2(port,pin) MERGE2(PORT,port) ## MERGE2(.ICR.BIT.B,pin)
#else
/* memory for DDR shadowing...used by DDR_xxxx Macros */
EXTERN unsigned char P1DDRS, P2DDRS, P3DDRS, P4DDRS, P5DDRS, P6DDRS, P8DDRS, PADDRS, PBDDRS, PCDDRS,PDDDRS, PEDDRS, PFDDRS,  PGDDRS, PHDDRS, PIDDRS, PJDDRS, PKDDRS, PLDDRS, PMDDRS;

#define DDRo2(port,pin) ( ## MERGE2(P,port) ## .DDR = (unsigned char)( ## MERGE2(P,MERGE2(port,DDRS)) |= (1 << pin)))
#define DDRi2(port,pin) ( ## MERGE2(P,port) ## .DDR = (unsigned char)( ## MERGE2(P,MERGE2(port,DDRS)) &= ~(1 << pin)))
#define PINo2(port,pin) MERGE2(P,port) ## MERGE2(.DR.BIT.B,pin)
#define PINi2(port,pin) MERGE2(P,port) ## MERGE2(.PORT.BIT.B,pin)
#define ICRi2(port,pin) MERGE2(P,port) ## MERGE2(.ICR.BIT.B,pin)
#endif

#define DDRo(signal) DDRo2(signal##_PORT, signal##_PIN)

/* Set signal DDR to input */
#define DDRi(signal) DDRi2(signal##_PORT, signal##_PIN)

/* Write signal level (for output) */
#define PINo(signal) PINo2(signal##_PORT, signal##_PIN)

/* Read signal level (for input) */
#define PINi(signal) PINi2(signal##_PORT, signal##_PIN)

/* Signals buffer control (for inputs) */
#define ICRi(signal) ICRi2(signal##_PORT, signal##_PIN)

#define size_ra(ra) (sizeof(ra)/sizeof(ra[0]))

/* macros to allow easy access to new style ICU bits and IODEFINE macros */
#define _ICU2_(func,unit_channel) _##func(unit_channel)
#define _ICU_(func,irq) _ICU2_(func, MERGE2(irq ## _UNIT, MERGE2(_,irq ## _CHANNEL)))

#endif
