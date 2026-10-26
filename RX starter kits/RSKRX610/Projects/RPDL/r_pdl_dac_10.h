/* Adapted for use with IAR Embedded Workbench */
/*""FILE COMMENT""*******************************************************
* System Name  : DAC API
* File Name    : r_pdl_dac_10.h
* Version    : 0.10
* Contents    : CRC function prototypes
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
* History    : 2009.12.02
*        : Ver 0.10
*        : 2010.01.22
*        : Ver 0.11
*        : Added protection against multiple inclusion.
*        : 2010.01.26
*        : Ver 0.12
*        : Removed const keywords from the function prototypes.
*""FILE COMMENT END""**************************************************/

#ifndef R_PDL_DAC_H
#define R_PDL_DAC_H

#include "r_pdl_common_defs_RX610xV.h"

/* Function prototypes */
bool R_DAC_10_Create(
  uint8_t,
  uint16_t,
  uint16_t
);
bool R_DAC_10_Destroy(
  uint8_t
);
bool R_DAC_10_Write(
  uint8_t,
  uint16_t,
  uint16_t
);

/* Channel selection */
#define PDL_DAC_10_CHANNEL_0  0x01u
#define PDL_DAC_10_CHANNEL_1  0x02u

/* Data alignment */
#define PDL_DAC_10_ALIGN_LEFT  0x04u
#define PDL_DAC_10_ALIGN_RIGHT  0x08u

#endif
/* End of file */
