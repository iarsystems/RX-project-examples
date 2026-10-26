/* Adapted for use with IAR Embedded Workbench */
/*""FILE COMMENT""*************************************************************
* System Name  : BSC API for RX62G and RX62T
* File Name    : r_pdl_bsc.h
* Version    : 1.00
* Contents    : BSC API header
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

#ifndef R_PDL_BSC_H
#define R_PDL_BSC_H

#include "r_pdl_common_defs_RX62Gxx.h"

/* Function prototypes */
bool R_BSC_Create(
  uint8_t,
  void *,
  uint8_t
);
bool R_BSC_Control(
  uint8_t
);
bool R_BSC_GetStatus(
  uint8_t *,
  uint16_t *
);

/* R_BSC_Create parameter options */
#define PDL_BSC_ERROR_ILLEGAL_ADDRESS_ENABLE  0x01u
#define PDL_BSC_ERROR_ILLEGAL_ADDRESS_DISABLE  0x02u

/* R_BSC_Control parameter options */
#define PDL_BSC_ERROR_CLEAR            0x01u
#define PDL_BSC_DISABLE_BUSERR_IRQ        0x02u

#endif
/* End of file */
