/* Adapted for use with IAR Embedded Workbench */
/*""FILE COMMENT""*******************************************************
* System Name  : WDT API for RX610xV
* File Name    : r_pdl_wdt_RX610xV.h
* Version    : 0.10
* Contents    : WDT API header
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
* History    : 2009.11.27
*        : Ver 0.10
*        : 2010.01.22
*        : Ver 0.11
*        : Added protection against multiple inclusion.
*""FILE COMMENT END""**************************************************/

#ifndef R_PDL_WDT_RX610xV_H
#define R_PDL_WDT_RX610xV_H

/* Callback function storage */
extern VoidCallBackFunc rpdl_WDT_callback_func;

/* Library prototypes */
bool R_WDT_CreateAll(
  const uint16_t,
  VoidCallBackFunc const,
  const uint8_t
);
bool R_WDT_ControlAll(
  const uint8_t
);
bool R_WDT_ReadAll(
  uint8_t * const
);
bool ReturnFalse(void);

/* Macro definitions */

#define R_WDT_Create(a, b, c) \
( \
( (c) <= IPL_MAX) ? \
R_WDT_CreateAll( (a), (b), (c) ) : \
ReturnFalse() \
)

#define R_WDT_Control(a) \
( \
R_WDT_ControlAll( (a) ) \
)

#define R_WDT_Read(a) \
( \
R_WDT_ReadAll( (a) ) \
)

#endif
/* End of file */
