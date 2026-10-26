/* Adapted for use with IAR Embedded Workbench */
/*""FILE COMMENT""*******************************************************
* System Name  : DTC API for RX610xV
* File Name    : r_pdl_dtc_RX610xV.h
* Version    : 0.10
* Contents    : DTC API header
* Customer    : 
* Model      : 
* Order      : 
* CPU      : RX600
* Compiler    : RXC
* OS      : Nothing
* Programmer  : 
* Note      : 
************************************************************************
* Copyright,2010 RENESAS TECHNOLOGY CORPORATION
* AND RENESAS SOLUTIONS CORPORATION
************************************************************************
* History    : 2010.01.28
*        : Ver 0.10
*""FILE COMMENT END""**************************************************/

#ifndef R_PDL_DTC_RX610xV_H
#define R_PDL_DTC_RX610xV_H

/* Library prototypes */
bool R_DTC_SetAll(
  const uint8_t,
  uint32_t * const
);
bool R_DTC_CreateAll(
  const uint32_t,
  const uint8_t,
  uint32_t * const,
  const void * const,
  const void * const,
  const uint16_t,
  const uint8_t
);
bool R_DTC_DestroyAll(
  void
);
bool R_DTC_ControlAll(
  const uint32_t
);
bool R_DTC_GetStatusAll(
  const uint32_t * const,
  uint8_t * const,
  uint32_t * const,
  uint32_t * const,
   uint16_t * const
);
bool ReturnFalse(void);

/* Macro definitions */

#define R_DTC_Set(a, b) \
( \
R_DTC_SetAll( (a), (uint32_t *)(b) ) \
)

#define R_DTC_Create(a, b, c, d, e, f) \
( \
R_DTC_CreateAll( (a), (uint8_t)((a) >> 24), (b), (c), (d), (e), (f) ) \
)

#define R_DTC_Destroy() \
( \
R_DTC_DestroyAll() \
)

#define R_DTC_Control(a) \
( \
R_DTC_ControlAll( (a) ) \
)

#define R_DTC_GetStatus(a, b, c, d, e) \
( \
R_DTC_GetStatusAll( (a), (b), (c), (d), (e) ) \
)

#endif
/* End of file */
