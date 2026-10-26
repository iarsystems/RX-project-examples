/* Adapted for use with IAR Embedded Workbench */
/*""FILE COMMENT""*******************************************************
* System Name  : PFC API
* File Name    : r_pdl_pfc.h
* Version    : 0.10
* Contents    : PFC API header
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

#ifndef R_PDL_PFC_H
#define R_PDL_PFC_H

#include "r_pdl_common_defs_RX610xV.h"

/* Function prototypes */
bool R_PFC_Read(
  uint8_t,
  uint8_t *
);
bool R_PFC_Write(
  uint8_t,
  uint8_t
);
bool R_PFC_Modify(
  uint8_t,
  uint8_t,
  uint8_t
);

#define PDL_PFC_AND  0x01u
#define PDL_PFC_OR  0x02u
#define PDL_PFC_XOR  0x04u

#endif
/* End of file */
