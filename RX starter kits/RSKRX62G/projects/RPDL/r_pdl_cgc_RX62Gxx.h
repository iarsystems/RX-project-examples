/* Adapted for use with IAR Embedded Workbench */
/*""FILE COMMENT""*************************************************************
* System Name  : CGC API for RX62G and RX62T
* File Name    : r_pdl_cgc_RX62Gxx.h
* Version    : 1.00
* Contents    : CGC header
* Customer    :
* Model      :
* Order      :
* CPU      : RX
* Compiler    : RXC
* OS      : Nothing
* Programmer  :
* Note      :
*******************************************************************************
* Copyright, 2012. Renesas Electronics Corporation and Renesas Solutions Corp.
*******************************************************************************
* History    : 2012.09.11
*        : Ver 1.00
*        : CS release.
*""FILE COMMENT END""*********************************************************/

#ifndef R_PDL_CGC_RX62Gxx_H
#define R_PDL_CGC_RX62Gxx_H

/* Library prototypes */
bool R_CGC_SetAll(
  const uint32_t,
  const uint32_t,
  const uint32_t,
  const uint8_t
);

bool R_CGC_GetStatusAll(
  volatile uint8_t * const
);

/* Macro definitions */

#define R_CGC_Set(a, b, c, d) \
R_CGC_SetAll( (a), (b), (c), (d) )

#define R_CGC_GetStatus(a) \
R_CGC_GetStatusAll( (a) )

#endif
/* End of file */
