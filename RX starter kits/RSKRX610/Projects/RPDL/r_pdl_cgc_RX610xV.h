/* Adapted for use with IAR Embedded Workbench */
/*""FILE COMMENT""*******************************************************
* System Name  : CGC API for RX610xV
* File Name    : r_pdl_cgc_RX610xV.h
* Version    : 0.10
* Contents    : CGC header
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
* History    : 2009.11.13
*        : Ver 0.10
*        : 2010.01.22
*        : Ver 0.11
*        : Added protection against multiple inclusion.
*""FILE COMMENT END""**************************************************/

#ifndef R_PDL_CGC_RX610xV_H
#define R_PDL_CGC_RX610xV_H

/* Library prototypes */
bool R_CGC_SetAll(
  const uint32_t,
  const uint32_t,
  const uint32_t,
  const uint32_t,
  const uint8_t
);

/* Macro definitions */

#define R_CGC_Set(a, b, c, d, e) \
R_CGC_SetAll( (a), (b), (c), (d), (e) )

#endif
/* End of file */
