/* Adapted for use with IAR Embedded Workbench */
/*""FILE COMMENT""*******************************************************
* System Name  : DMAC API for RX610xV
* File Name    : r_pdl_dmac_RX610xV.h
* Version    : 0.10
* Contents    : DMAC API header
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
* History    : 2009.11.24
*        : Ver 0.10
*        : 2010.01.22
*        : Ver 0.11
*        : Added protection against multiple inclusion.
*        : 2010.02.08
*        : Ver 0.12
*        : Added a parameter to R_DMAC_Control
*        : 2010.02.11
*        : Ver 0.13
*        : Added channel range check to R_DMAC_Control
*""FILE COMMENT END""**************************************************/

#ifndef R_PDL_DMAC_RX610xV_H
#define R_PDL_DMAC_RX610xV_H

#define DMAC_CHANNELS  4

/* Callback function storage */
extern VoidCallBackFunc rpdl_DMAC_callback_func[];

/* Library prototypes */
bool R_DMAC_CreateAll(
  const uint8_t,
  const uint32_t,
  const uint8_t,
  const void * const,
  const void * const,
  const uint32_t,
  const void * const,
  const void * const,
  const uint32_t,
  VoidCallBackFunc const,
  const uint8_t
);
bool R_DMAC_DestroyAll(
  void
);
bool R_DMAC_ControlAll(
  const uint8_t,
  const uint8_t,
  const uint8_t,
  const uint32_t,
  const uint32_t,
  const uint32_t
);
bool R_DMAC_GetStatusAll(
  const uint8_t,
  uint16_t * const,
  uint32_t * const,
  uint32_t * const,
  uint32_t * const
);
bool ReturnFalse(void);

/* Macro definitions */

#define R_DMAC_Create(a, b, c, d, e, f, g, h, i, j, k) \
( \
( ( ((a) < DMAC_CHANNELS ) && ((k) <= IPL_MAX) ) ) ? \
R_DMAC_CreateAll( (a), (b), (c), (d), (e), (f), (g), (h), (i), (j), (k) ) : \
ReturnFalse() \
)

#define R_DMAC_Control(a, b, c, d, e, f) \
( \
( ((a) >= PDL_DMAC_0) ) ? \
R_DMAC_ControlAll( (a), (b), (c), (d), (e), (f) ) : \
ReturnFalse() \
)

#define R_DMAC_GetStatus(a, b, c, d, e) \
( \
( ((a) < DMAC_CHANNELS) ) ? \
R_DMAC_GetStatusAll( (a), (b), (c), (d), (e) ) : \
ReturnFalse() \
)

#define R_DMAC_Destroy() \
( R_DMAC_DestroyAll() )

#endif
/* End of file */
