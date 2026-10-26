/* Adapted for use with IAR Embedded Workbench */
/*""FILE COMMENT""*************************************************************
* System Name  : WDT API for RX62G and RX62T
* File Name    : r_pdl_wdt_RX62Gxx.h
* Version    : 1.00
* Contents    : WDT API header
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

#ifndef R_PDL_WDT_RX62Gxx_H
#define R_PDL_WDT_RX62Gxx_H

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
  volatile uint8_t * const
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
