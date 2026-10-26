/* Adapted for use with IAR Embedded Workbench */
/*""FILE COMMENT""*******************************************************
* System Name  : CGC API
* File Name    : r_pdl_cgc.h
* Version    : 0.10
* Contents    : CGC header
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
* History    : 2009.11.11
*        : Ver 0.10
*        : 2010.01.22
*        : Ver 0.11
*        : Added protection against multiple inclusion.
*        : 2010.01.26
*        : Ver 0.12
*        : Removed const keywords from the function prototypes.
*""FILE COMMENT END""**************************************************/

#ifndef R_PDL_CGC_H
#define R_PDL_CGC_H

#include "r_pdl_common_defs_RX610xV.h"

/* Function prototypes */
bool R_CGC_Set(
  uint32_t,
  uint32_t,
  uint32_t,
  uint32_t,
  uint8_t
);

/* BLCK pin putput control */
#define PDL_CGC_BCLK_DISABLE  0x01u
#define PDL_CGC_BCLK_ENABLE    0x02u
#define PDL_CGC_BCLK_HIGH    0x04u

#endif
/* End of file */
