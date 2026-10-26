/* Adapted for use with IAR Embedded Workbench */
/*""FILE COMMENT""*******************************************************
* System Name  : PPG API for RX610xV
* File Name    : r_pdl_ppg_RX610xV.h
* Version    : 0.10
* Contents    : PPG API header
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
* History    : 2010.01.14
*        : Ver 0.10
*        : 2010.01.22
*        : Ver 0.11
*        : Added protection against multiple inclusion.
*""FILE COMMENT END""**************************************************/

#ifndef R_PDL_PPG_RX610xV_H
#define R_PDL_PPG_RX610xV_H

#define PPG_UNITS      2
#define PPG_GROUPS_PER_UNIT  4
#define PPG_GROUPS  (uint8_t)(PPG_UNITS * PPG_GROUPS_PER_UNIT)

/* Library prototypes */
bool R_PPG_CreateAll(
  const uint8_t,
  const uint8_t,
  const uint8_t,
  const uint8_t,
  const uint8_t,
  const uint16_t,
  const uint8_t
);
bool R_PPG_DestroyAll(
  const uint8_t,
  const uint8_t,
  const uint8_t
);
bool R_PPG_ControlAll(
  const uint8_t,
  const uint8_t,
  const uint8_t
);
bool ReturnFalse(void);

/* Macro definitions */

#define R_PPG_Create(a, b, c) \
( \
( (a) != 0x00000000u ) ? \
R_PPG_CreateAll( (uint8_t)((a) >> 24), (uint8_t)(((a) & 0xF0u) >> 20), (uint8_t)(((a) & 0x0Fu) >> 16), (uint8_t)((a) >> 8), (uint8_t)(a), (b), (c) ) : \
ReturnFalse() \
)

#define R_PPG_Destroy(a) \
( \
( (a) != 0x00000000u ) ? \
R_PPG_DestroyAll( (uint8_t)((a) >> 24), (uint8_t)((a) >> 8), (uint8_t)(a) ) : \
ReturnFalse() \
)

#define R_PPG_Control(a, b ) \
( \
( (a) != 0x00u ) ? \
R_PPG_ControlAll( (uint8_t)((a) >> 24), (uint8_t)((a) >> 8), (b) ) : \
ReturnFalse() \
)

#endif
/* End of file */
