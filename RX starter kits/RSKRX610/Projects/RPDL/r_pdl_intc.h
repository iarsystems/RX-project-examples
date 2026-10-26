/* Adapted for use with IAR Embedded Workbench */
/*""FILE COMMENT""*******************************************************
* System Name  : External interrupt API
* File Name    : r_pdl_intc.h
* Version    : 0.10
* Contents    : External interrupt API header
* Customer    :
* Model      :
* Order      :
* CPU      :
* Compiler    :
* OS      :
* Programmer  :
* Note      :
************************************************************************
* Copyright,2009 RENESAS TECHNOLOGY CORPORATION
* AND RENESAS SOLUTIONS CORPORATION
************************************************************************
* History    : 2009.11.11
*        : Ver 0.10
*        : 2010.01.22
*        : Ver 0.11
*        : Added protection against multiple inclusion.
*        : 2010.01.26
*        : Ver 0.12
*        : Removed const keywords from the function prototypes.
*        : 2010.02.24
*        : Ver 0.13
*        : Removed a FastInterrupt parameter
*""FILE COMMENT END""**************************************************/

#ifndef R_PDL_INTC_H
#define R_PDL_INTC_H

#include "r_pdl_common_defs_RX610xV.h"

/* Function prototypes */
bool R_INTC_CreateExtInterrupt(
  uint8_t,
  uint16_t,
  void *,
  uint8_t
);
bool R_INTC_CreateFastInterrupt(
  uint8_t
);
bool R_INTC_CreateExceptionHandlers(
  void *,
  void *,
  void *
);
bool R_INTC_ControlExtInterrupt(
  uint8_t,
  uint16_t
);
bool R_INTC_GetExtInterruptStatus(
  uint8_t,
  uint8_t *
);
bool R_INTC_Read(
  uint8_t,
  uint8_t,
  uint8_t *
);
bool R_INTC_Write(
  uint8_t,
  uint8_t,
  uint8_t
);
bool R_INTC_Modify(
  uint8_t,
  uint8_t,
  uint8_t,
  uint8_t
);

/* Interrupt pins */
#define PDL_INTC_IRQ0      0
#define PDL_INTC_IRQ1      1
#define PDL_INTC_IRQ2      2
#define PDL_INTC_IRQ3      3
#define PDL_INTC_IRQ4      4
#define PDL_INTC_IRQ5      5
#define PDL_INTC_IRQ6      6
#define PDL_INTC_IRQ7      7
#define PDL_INTC_IRQ8      8
#define PDL_INTC_IRQ9      9
#define PDL_INTC_IRQ10      10
#define PDL_INTC_IRQ11      11
#define PDL_INTC_IRQ12      12
#define PDL_INTC_IRQ13      13
#define PDL_INTC_IRQ14      14
#define PDL_INTC_IRQ15      15
#define PDL_INTC_PRIVILEGED    16
#define PDL_INTC_UNDEFINED    17
#define PDL_INTC_FLOATING_POINT 18
#define PDL_INTC_NMI      19

/* Detection sense selection */
#define PDL_INTC_LOW            0x0001u
#define PDL_INTC_FALLING          0x0002u
#define PDL_INTC_RISING            0x0004u
#define PDL_INTC_BOTH            0x0008u

/* IRQ alternate pin selection */
#define PDL_INTC_A              0x0010u
#define PDL_INTC_B              0x0020u

/* DMAC trigger control */
#define PDL_INTC_DMAC_DTC_TRIGGER_DISABLE  0x0040u
#define PDL_INTC_DMAC_TRIGGER_ENABLE    0x0080u
#define PDL_INTC_DTC_TRIGGER_ENABLE      0x0100u

/* Interrupt control */
#define PDL_INTC_ENABLE            0x0200u
#define PDL_INTC_DISABLE          0x0400u

/* Flag control */
#define PDL_INTC_CLEAR_IR_FLAG        0x0800u

/* Fast interrupt control */
#define PDL_INTC_FAST_INT_ENABLE  0x01u
#define PDL_INTC_FAST_INT_DISABLE  0x02u

/* Interrupt registers */
#define PDL_INTC_REG_IPL  0x01u
#define PDL_INTC_REG_IR    0x02u
#define PDL_INTC_REG_ISELR  0x04u
#define PDL_INTC_REG_IER  0x08u
#define PDL_INTC_REG_IPR  0x10u

#define PDL_INTC_AND  0x01u
#define PDL_INTC_OR    0x02u
#define PDL_INTC_XOR  0x04u

#endif
/* End of file */
