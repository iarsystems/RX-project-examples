/* Adapted for use with IAR Embedded Workbench */
/*""FILE COMMENT""*******************************************************
* System Name  : CRC calculator API for RX610xV
* File Name    : r_pdl_crc_RX610xV.h
* Version    : 0.10
* Contents    : CRC header
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
* History    : 2009.12.01
*        : Ver 0.10
*        : 2010.01.22
*        : Ver 0.11
*        : Added protection against multiple inclusion.
*""FILE COMMENT END""**************************************************/

#ifndef R_PDL_CRC_RX610xV_H
#define R_PDL_CRC_RX610xV_H

/* Library prototypes */
bool R_CRC_CreateAll(
  const uint8_t
);
bool R_CRC_DestroyAll(
  void
);
bool R_CRC_WriteAll(
  const uint8_t
);
bool R_CRC_ReadAll(
  uint16_t * const,
  const uint8_t
);

/* Macro definitions */

#define R_CRC_Create(a) \
( \
R_CRC_CreateAll( (a) ) \
)

#define R_CRC_Destroy() \
( \
R_CRC_DestroyAll() \
)

#define R_CRC_Write(a) \
( \
R_CRC_WriteAll( (a) ) \
)

#define R_CRC_Read(a, b) \
( \
R_CRC_ReadAll( (a), (b) ) \
)

#endif
/* End of file */
