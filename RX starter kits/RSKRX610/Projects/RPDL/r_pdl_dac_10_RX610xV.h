/* Adapted for use with IAR Embedded Workbench */
/*""FILE COMMENT""*******************************************************
* System Name  : DAC API for RX610xV
* File Name    : r_pdl_dac_10_RX610xV.h
* Version    : 0.10
* Contents    : DAC header
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
*""FILE COMMENT END""**************************************************/

#ifndef R_PDL_DAC_RX610xV_H
#define R_PDL_DAC_RX610xV_H

/* Library prototypes */
bool R_DAC_10_CreateAll(
  const uint8_t,
  const uint16_t,
  const uint16_t
);
bool R_DAC_10_DestroyAll(
  const uint8_t
);
bool R_DAC_10_WriteAll(
  const uint8_t,
  const uint16_t,
  const uint16_t
);

/* Macro definitions */

#define R_DAC_10_Create(a, b, c) \
( \
R_DAC_10_CreateAll( (a), (b), (c) ) \
)

#define R_DAC_10_Destroy(a) \
( \
R_DAC_10_DestroyAll( (a) ) \
)

#define R_DAC_10_Write(a, b, c) \
( \
R_DAC_10_WriteAll( (a), (b), (c) ) \
)

#endif
/* End of file */
