/* Adapted for use with IAR Embedded Workbench */
#ifndef RX_DHRY_H
#define RX_DHRY_H

#include <stdio.h>

#define  PORT8_DR  (*(volatile unsigned char *)0x0008C028)   /* Port 8 - Data Register */


/* Defines used to signal when the benchmark starts and when it ends */
/* Turn LED0 on RX610 RSK off (1) */
#define SCOPE_HI()  PORT8_DR |= 0x08;
/* Turn LED0 on RX610 RSK on (0) */
#define SCOPE_LO()  PORT8_DR &= 0xF7;

/* This undef is included because using stdio has it's own DEFINE for
   putc that will mess up the function declaration of putc that is used
   in rx_dhry.c.  If you want to see what this means then comment out 
   the line below, switch to see preprocessed output, and look at the
   putc() function */
#undef  putc

void InitSerial(void);

//RX_DHRY_H
#endif
