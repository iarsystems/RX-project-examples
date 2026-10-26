/* Adapted for use with IAR Embedded Workbench */
/*""FILE COMMENT""*******************************************************
* System Name  : Timer TPU API for RX610xV
* File Name    : r_pdl_tpu.h
* Version    : 0.10
* Contents    : Timer TPU API header
* Customer    : 
* Model      : 
* Order      : 
* CPU      : RX600
* Compiler    : RXC
* OS      : Nothing
* Programmer  : 
* Note      : 
************************************************************************
* Copyright,2009 RENESAS TECHNOLOGY CORPORATION
* AND RENESAS SOLUTIONS CORPORATION
************************************************************************
* History    : 2009.11.12
*        : Ver 0.10
*        : 2010.01.22
*        : Ver 0.11
*        : Added protection against multiple inclusion.
*        : 2010.01.26
*        : Ver 0.12
*        : Removed const keywords from the function prototypes.
*""FILE COMMENT END""**************************************************/

#ifndef R_PDL_TPU_H
#define R_PDL_TPU_H

#include "r_pdl_common_defs_RX610xV.h"

/* Function prototypes */
bool R_TPU_Create(
  uint8_t,
  uint16_t,
  uint32_t,
  uint32_t,
  uint32_t,
  uint16_t,
  uint16_t,
  uint16_t,
  uint16_t,
  uint16_t,
  void *,
  void *,
  void *,
  void *,
  uint8_t,
  void *,
  void *,
  uint8_t
);
bool R_TPU_Destroy(
  uint8_t
);
bool R_TPU_Control(
  uint8_t,
  uint8_t,
  uint16_t,
  uint16_t,
  uint16_t,
  uint16_t,
  uint16_t
);
bool R_TPU_Read(
  uint8_t,
  uint8_t *,
  uint16_t *,
  uint16_t *,
  uint16_t *,
  uint16_t *,
  uint16_t *
);
  
/* Operation mode */
#define PDL_TPU_MODE_NORMAL          0x0001u
#define PDL_TPU_MODE_PWM1          0x0002u
#define PDL_TPU_MODE_PWM2          0x0004u
#define PDL_TPU_MODE_PHASE1          0x0008u
#define PDL_TPU_MODE_PHASE2          0x0010u
#define PDL_TPU_MODE_PHASE3          0x0020u
#define PDL_TPU_MODE_PHASE4          0x0040u

/* Synchronous mode control */
#define PDL_TPU_SYNC_DISABLE        0x0080u
#define PDL_TPU_SYNC_ENABLE          0x0100u

/* DMAC trigger control */
#define PDL_TPU_DMAC_DTC_TRIGGER_DISABLE  0x0200u
#define PDL_TPU_DMAC_TRIGGER_ENABLE      0x0400u
#define PDL_TPU_TGRA_DTC_TRIGGER_ENABLE    0x0800u
#define PDL_TPU_TGRB_DTC_TRIGGER_ENABLE    0x1000u
#define PDL_TPU_TGRC_DTC_TRIGGER_ENABLE    0x2000u
#define PDL_TPU_TGRD_DTC_TRIGGER_ENABLE    0x4000u

/* Counter clock sources */
#define PDL_TPU_CLK_PCLK_DIV_1    0x00000001ul
#define PDL_TPU_CLK_PCLK_DIV_4    0x00000002ul
#define PDL_TPU_CLK_PCLK_DIV_16    0x00000004ul
#define PDL_TPU_CLK_PCLK_DIV_64    0x00000008ul
#define PDL_TPU_CLK_PCLK_DIV_256  0x00000010ul
#define PDL_TPU_CLK_PCLK_DIV_1024  0x00000020ul
#define PDL_TPU_CLK_PCLK_DIV_4096  0x00000040ul
#define PDL_TPU_CLK_TCLKA      0x00000080ul
#define PDL_TPU_CLK_TCLKB      0x00000100ul
#define PDL_TPU_CLK_TCLKC      0x00000200ul
#define PDL_TPU_CLK_TCLKD      0x00000400ul
#define PDL_TPU_CLK_TCLKE      0x00000800ul
#define PDL_TPU_CLK_TCLKF      0x00001000ul
#define PDL_TPU_CLK_TCLKG      0x00002000ul
#define PDL_TPU_CLK_TCLKH      0x00004000ul
#define PDL_TPU_CLK_TPU        0x00008000ul

/* Counter edge selection */
#define PDL_TPU_CLK_FALLING      0x00010000ul
#define PDL_TPU_CLK_RISING      0x00020000ul
#define PDL_TPU_CLK_BOTH      0x00040000ul

/* Counter clearing options */
#define PDL_TPU_CLEAR_DISABLE    0x00080000ul
#define PDL_TPU_CLEAR_CM_A      0x00100000ul
#define PDL_TPU_CLEAR_CM_B      0x00200000ul
#define PDL_TPU_CLEAR_CM_C      0x00400000ul
#define PDL_TPU_CLEAR_CM_D      0x00800000ul
#define PDL_TPU_CLEAR_CM_SYNC    0x01000000ul

/* Buffer operation */
#define PDL_TPU_BUFFER_AC_DISABLE  0x02000000ul
#define PDL_TPU_BUFFER_AC_ENABLE  0x04000000ul
#define PDL_TPU_BUFFER_BD_DISABLE  0x08000000ul
#define PDL_TPU_BUFFER_BD_ENABLE  0x10000000ul

/* ADC trigger control */
#define PDL_TPU_ADC_TRIG_DISABLE  0x20000000ul
#define PDL_TPU_ADC_TRIG_ENABLE    0x40000000ul

/* TGRA options */
#define PDL_TPU_A_OC_DISABLED    0x00000001ul
#define PDL_TPU_A_OC_LOW      0x00000002ul
#define PDL_TPU_A_OC_LOW_CM_HIGH  0x00000004ul
#define PDL_TPU_A_OC_LOW_CM_INV    0x00000008ul
#define PDL_TPU_A_OC_HIGH_CM_LOW  0x00000010ul
#define PDL_TPU_A_OC_HIGH      0x00000020ul
#define PDL_TPU_A_OC_HIGH_CM_INV  0x00000040ul
#define PDL_TPU_A_IC_RISING_EDGE  0x00000080ul
#define PDL_TPU_A_IC_FALLING_EDGE  0x00000100ul
#define PDL_TPU_A_IC_BOTH_EDGES    0x00000200ul
#define PDL_TPU_A_IC_TPU_COUNT_CLK  0x00000400ul
#define PDL_TPU_A_IC_TPU_CM_IC    0x00000800ul

/* Multiplexed or separate pin selection for TIOCA */
#define PDL_TPU_A_IC_SHARED      0x00001000ul
#define PDL_TPU_A_IC_SEPARATE    0x00002000ul

/* TGRB options */
#define PDL_TPU_B_OC_DISABLED    0x00004000ul
#define PDL_TPU_B_OC_LOW      0x00008000ul
#define PDL_TPU_B_OC_LOW_CM_HIGH  0x00010000ul
#define PDL_TPU_B_OC_LOW_CM_INV    0x00020000ul
#define PDL_TPU_B_OC_HIGH_CM_LOW  0x00040000ul
#define PDL_TPU_B_OC_HIGH      0x00080000ul
#define PDL_TPU_B_OC_HIGH_CM_INV  0x00100000ul
#define PDL_TPU_B_IC_RISING_EDGE  0x00200000ul
#define PDL_TPU_B_IC_FALLING_EDGE  0x00400000ul
#define PDL_TPU_B_IC_BOTH_EDGES    0x00800000ul
#define PDL_TPU_B_IC_TPU_COUNT_CLK  0x01000000ul
#define PDL_TPU_B_IC_TPU_CM_IC    0x02000000ul

/* TGRB input capture input selection */
#define PDL_TPU_B_IC_TIOCB      0x04000000ul
#define PDL_TPU_B_IC_TIOCA      0x08000000ul

/* TGRC options */
#define PDL_TPU_C_OC_DISABLED    0x00000001ul
#define PDL_TPU_C_OC_LOW      0x00000002ul
#define PDL_TPU_C_OC_LOW_CM_HIGH  0x00000004ul
#define PDL_TPU_C_OC_LOW_CM_INV    0x00000008ul
#define PDL_TPU_C_OC_HIGH_CM_LOW  0x00000010ul
#define PDL_TPU_C_OC_HIGH      0x00000020ul
#define PDL_TPU_C_OC_HIGH_CM_INV  0x00000040ul
#define PDL_TPU_C_IC_RISING_EDGE  0x00000080ul
#define PDL_TPU_C_IC_FALLING_EDGE  0x00000100ul
#define PDL_TPU_C_IC_BOTH_EDGES    0x00000200ul
#define PDL_TPU_C_IC_TPU_COUNT_CLK  0x00000400ul

/* Multiplexed or separate pin selection for TIOCC */
#define PDL_TPU_C_IC_SHARED      0x00001000ul
#define PDL_TPU_C_IC_SEPARATE    0x00002000ul

/* TGRD options */
#define PDL_TPU_D_OC_DISABLED    0x00004000ul
#define PDL_TPU_D_OC_LOW      0x00008000ul
#define PDL_TPU_D_OC_LOW_CM_HIGH  0x00010000ul
#define PDL_TPU_D_OC_LOW_CM_INV    0x00020000ul
#define PDL_TPU_D_OC_HIGH_CM_LOW  0x00040000ul
#define PDL_TPU_D_OC_HIGH      0x00080000ul
#define PDL_TPU_D_OC_HIGH_CM_INV  0x00100000ul
#define PDL_TPU_D_IC_RISING_EDGE  0x00200000ul
#define PDL_TPU_D_IC_FALLING_EDGE  0x00400000ul
#define PDL_TPU_D_IC_BOTH_EDGES    0x00800000ul
#define PDL_TPU_D_IC_TPU_COUNT_CLK  0x01000000ul

/* TGRD input capture input selection */
#define PDL_TPU_D_IC_TIOCD      0x02000000ul
#define PDL_TPU_D_IC_TIOCC      0x04000000ul

/* Counter stop / re-start */
#define PDL_TPU_STOP  0x01u
#define PDL_TPU_START  0x02u

/* The registers to be modified */
#define PDL_TPU_COUNTER  0x04u
#define PDL_TPU_TGRA  0x08u
#define PDL_TPU_TGRB  0x10u
#define PDL_TPU_TGRC  0x20u
#define PDL_TPU_TGRD  0x40u

#endif
/* End of file */
