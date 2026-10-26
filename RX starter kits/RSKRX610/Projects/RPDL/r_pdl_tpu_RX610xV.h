/* Adapted for use with IAR Embedded Workbench */
/*""FILE COMMENT""*******************************************************
* System Name  : Timer TPU API for RX610xV
* File Name    : r_pdl_tpu_RX610xV.h
* Version    : 0.10
* Contents    : Timer TPU API header
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

#ifndef R_PDL_TPU_RX610xV_H
#define R_PDL_TPU_RX610xV_H

#define TPU_UNITS         2
#define TPU_CHANNELS_PER_UNIT  6
#define TPU_CHANNELS (uint8_t)(TPU_UNITS * TPU_CHANNELS_PER_UNIT)

/* Callback function storage */
extern VoidCallBackFunc rpdl_TPU_TGRA_callback_func[TPU_CHANNELS];
extern VoidCallBackFunc rpdl_TPU_TGRB_callback_func[TPU_CHANNELS];
extern VoidCallBackFunc rpdl_TPU_TGRC_callback_func[TPU_CHANNELS];
extern VoidCallBackFunc rpdl_TPU_TGRD_callback_func[TPU_CHANNELS];
extern VoidCallBackFunc rpdl_TPU_Overflow_callback_func[TPU_CHANNELS];
extern VoidCallBackFunc rpdl_TPU_Underflow_callback_func[TPU_CHANNELS];

/* Library prototypes */
bool R_TPU_CreateAll(
  const uint8_t,
  const uint16_t,
  const uint32_t,
  const uint32_t,
  const uint32_t,
  const uint16_t,
  const uint16_t,
  const uint16_t,
  const uint16_t,
  const uint16_t,
  VoidCallBackFunc const,
  VoidCallBackFunc const,
  VoidCallBackFunc const,
  VoidCallBackFunc const,
  const uint8_t,
  VoidCallBackFunc const,
  VoidCallBackFunc const,
  const uint8_t
);
bool R_TPU_DestroyAll(
  const uint8_t
);
bool R_TPU_ControlAll(
  const uint8_t,
  const uint8_t,
  const uint16_t,
  const uint16_t,
  const uint16_t,
  const uint16_t,
  const uint16_t
);
bool R_TPU_ReadAll(
  const uint8_t,
  uint8_t * const,
  uint16_t * const,
  uint16_t * const,
  uint16_t * const,
  uint16_t * const,
  uint16_t * const
);
bool ReturnFalse(void);

/* Macro definitions */

#define R_TPU_Create(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r) \
( \
( ((a) < TPU_CHANNELS) && ((o) <= IPL_MAX) && ((r) <= IPL_MAX) ) ? \
R_TPU_CreateAll( (a), (b), (c), (d), (e), (f), (g), (h), (i), (j), (k), (l), (m), (n), (o), (p), (q), (r) ) : \
ReturnFalse() \
)

#define R_TPU_Control(a, b, c, d, e, f, g) \
( \
( (a) < TPU_CHANNELS ) ? \
R_TPU_ControlAll( (a), (b), (c), (d), (e), (f), (g) ) : \
ReturnFalse() \
)

#define R_TPU_Read(a, b, c, d, e, f, g) \
( \
( (a) < TPU_CHANNELS ) ? \
R_TPU_ReadAll( (a), (b), (c), (d), (e), (f), (g) ) : \
ReturnFalse() \
)

#define R_TPU_Destroy(a) \
( \
( (a) < TPU_UNITS ) ? \
R_TPU_DestroyAll( (a) ) : \
ReturnFalse() \
)

#endif
/* End of file */
