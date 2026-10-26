/******************************************************************************
* DISCLAIMER
* Please refer to http://www.renesas.com/disclaimer
******************************************************************************/
/***********************************************************************/
/*                                                                     */
/*  FILE        :Global.h                                              */
/*  DATE        :Sep 12, 2015                                          */
/*  DESCRIPTION :Globally useful definitions for LCD Direct Drive      */
/*  CPU TYPE    :RX64M/RX71M                                           */
/*                                                                     */
/***********************************************************************/
/************************************************************************
Revision History
DD.MM.YYYY OSO-UID Description
12.09.2015 REE - Ported from RX63N GAPI Demo
************************************************************************/
#ifndef GLOBAL_H
#define GLOBAL_H

/* Typedef fundemental data types that may change between compilers/families */
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>

/* Include the MCU sfr definitions */
#include "platform.h"

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

/* backend macros */
#ifdef __RX62NIODEFINE_HEADER__
/* RX62x */
#define DDRo2(port,pin) MERGE2(PORT,port) ## MERGE2(.DDR.BIT.B,pin) = 1;
#define DDRi2(port,pin) MERGE2(PORT,port) ## MERGE2(.DDR.BIT.B,pin) = 0;
#define PINo2(port,pin) MERGE2(PORT,port) ## MERGE2(.DR.BIT.B,pin)
#define PINi2(port,pin) MERGE2(PORT,port) ## MERGE2(.PORT.BIT.B,pin)
#elif defined(__RX63NIODEFINE_HEADER__)
/* RX63x */
#define DDRo2(port,pin) port MERGE2(.PDR.BIT.B,pin) = 1;
#define DDRi2(port,pin) port MERGE2(.PDR.BIT.B,pin) = 0;
#define PINo2(port,pin) port MERGE2(.PODR.BIT.B,pin)
#define PINi2(port,pin) port MERGE2(.PIDR.BIT.B,pin)
#elif defined(__RX64MIODEFINE_HEADER__) || defined(__RX71MIODEFINE_HEADER__)
#define DDRo2(port,pin) port MERGE2(.PDR.BIT.B,pin) = 1;
#define DDRi2(port,pin) port MERGE2(.PDR.BIT.B,pin) = 0;
#define PINo2(port,pin) port MERGE2(.PODR.BIT.B,pin)
#define PINi2(port,pin) port MERGE2(.PIDR.BIT.B,pin)
#endif


/* Set signal DDR to output */
#define DDRo(signal) DDRo2(signal##_PORT, signal##_PIN)

/* Set signal DDR to input */
#define DDRi(signal) DDRi2(signal##_PORT, signal##_PIN)

/* Write signal level (for output) */
#define PINo(signal) PINo2(signal##_PORT, signal##_PIN)

/* Read signal level (for input) */
#define PINi(signal) PINi2(signal##_PORT, signal##_PIN)

/* Signals buffer control (for inputs) */
#define ICRi2(port,pin) port MERGE2(.ICR.BIT.B,pin)
#define ICRi(signal) ICRi2(DD_##signal##_PORT, DD_##signal##_PIN)
/* Signals port function control */
#define PFCRs2(mpc, port,pin,value) MERGE2(MERGE2(MPC., mpc),MERGE2(pin,PFS.BIT.PSEL)=value;) port MERGE2(.PMR.BIT.B,pin) = 1;
#define PFCRs(signal,value) PFCRs2(DD_##signal##_MPC, DD_##signal##_PORT, DD_##signal##_PIN, value)

#define size_ra(ra) (sizeof(ra)/sizeof(ra[0]))
/* macros to allow easy access to new style ICU bits and IODEFINE macros */
#define _ICU2_(func,unit_channel) _##func(unit_channel)
#define _ICU_(func,irq) _ICU2_(func, MERGE2(irq ## _UNIT, MERGE2(_,irq ## _CHANNEL)))

/* Include platform level hardware definitions */
#include "platform.h"

#endif
