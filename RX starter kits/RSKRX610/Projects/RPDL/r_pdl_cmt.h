/* Adapted for use with IAR Embedded Workbench */
/*""FILE COMMENT""*******************************************************
* System Name  : CMT API for RX610xV
* File Name    : r_pdl_cmt.h
* Version    : 0.10
* Contents    : CMT API header
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
* History    : 2009.11.11
*        : Ver 0.10
*        : 2010.01.22
*        : Ver 0.11
*        : Added protection against multiple inclusion.
*        : 2010.01.26
*        : Ver 0.12
*        : Removed const keywords from the function prototypes.
*""FILE COMMENT END""**************************************************/

#ifndef R_PDL_CMT_H
#define R_PDL_CMT_H

#include "r_pdl_common_defs_RX610xV.h"

/* Function prototypes */
bool R_CMT_Create(
  uint8_t,
  uint16_t,
  float,
  void *,
  uint8_t
);
bool R_CMT_CreateOneShot(
  uint8_t,
  uint16_t,
  float,
  void *,
  uint8_t
);
bool R_CMT_Destroy(
  uint8_t
); 
bool R_CMT_Control(
  uint8_t,
  uint16_t,
  float
);
bool R_CMT_Read(
  uint8_t,
  uint8_t *,
  uint16_t *
);

/* Timer counter control */
#define PDL_CMT_STOP            0x0001u
#define PDL_CMT_START            0x0002u

/* Calculation selection */
#define PDL_CMT_PERIOD            0x0004u
#define PDL_CMT_FREQUENCY          0x0008u

/* CPU control */
#define PDL_CMT_CPU_ON            0x0010u
#define PDL_CMT_CPU_OFF            0x0020u

/* DMAC / DTC trigger control */
#define PDL_CMT_DMAC_DTC_TRIGGER_DISABLE  0x0040u
#define PDL_CMT_DMAC_TRIGGER_ENABLE      0x0080u
#define PDL_CMT_DTC_TRIGGER_ENABLE      0x0100u

/* Direct register control */
#define PDL_CMT_CONSTANT          0x0200u
#define PDL_CMT_PCLK_DIV_8          0x0400u
#define PDL_CMT_PCLK_DIV_32          0x0800u
#define PDL_CMT_PCLK_DIV_128        0x1000u
#define PDL_CMT_PCLK_DIV_512        0x2000u

#endif
/* End of file */
