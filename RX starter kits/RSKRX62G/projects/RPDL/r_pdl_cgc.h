/* Adapted for use with IAR Embedded Workbench */
/*""FILE COMMENT""*************************************************************
* System Name  : CGC API for RX62G and RX62T
* File Name    : r_pdl_cgc.h
* Version    : 1.00
* Contents    : CGC header
* Customer    :
* Model      :
* Order      :
* CPU      : RX
* Compiler    : RXC
* OS      :
* Programmer  :
* Note      :
*******************************************************************************
* Copyright, 2012. Renesas Electronics Corporation and Renesas Solutions Corp.
*******************************************************************************
* History    : 2012.09.11
*        : Ver 1.00
*        : CS release.
*""FILE COMMENT END""*********************************************************/

#ifndef R_PDL_CGC_H
#define R_PDL_CGC_H

#include "r_pdl_common_defs_RX62Gxx.h"

/* Function prototypes */
bool R_CGC_Set(
  uint32_t,
  uint32_t,
  uint32_t,
  uint8_t
);

bool R_CGC_GetStatus(
  uint8_t *
);

/* Oscillation Stop Detection control */
#define PDL_CGC_OSC_STOP_ENABLE    0x01u
#define PDL_CGC_OSC_STOP_DISABLE  0x02u

#endif
/* End of file */
