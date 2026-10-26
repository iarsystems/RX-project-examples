/* Adapted for use with IAR Embedded Workbench */
/*""FILE COMMENT""*******************************************************
* System Name  : CRC calculator API
* File Name    : r_pdl_crc.h
* Version    : 0.10
* Contents    : CRC function prototypes
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
* History    : 2010.01.21
*        : Ver 0.10
*        : 2010.01.22
*        : Ver 0.11
*        : Added protection against multiple inclusion.
*        : 2010.01.26
*        : Ver 0.12
*        : Removed const keywords from the function prototypes.
*""FILE COMMENT END""**************************************************/

#ifndef R_PDL_CRC_H
#define R_PDL_CRC_H

#include "r_pdl_common_defs_RX610xV.h"

/* Function prototypes */
bool R_CRC_Create(
  uint8_t
);
bool R_CRC_Destroy(
  void
);
bool R_CRC_Write(
  uint8_t
);
bool R_CRC_Read(
  uint16_t *,
  uint8_t
);

/* Polynomial selection */
#define PDL_CRC_POLY_CRC_8    0x01u
#define PDL_CRC_POLY_CRC_16    0x02u
#define PDL_CRC_POLY_CRC_CCITT  0x04u

/* Bit order */
#define PDL_CRC_LSB_FIRST    0x08u
#define PDL_CRC_MSB_FIRST    0x10u

/* Result register clearing */
#define PDL_CRC_CLEAR_RESULT  0x01u
#define PDL_CRC_RETAIN_RESULT  0x02u

#endif
/* End of file */
