/* Adapted for use with IAR Embedded Workbench */
/*""FILE COMMENT""*******************************************************
* System Name  : A/D converter API for RX610xV
* File Name    : r_pdl_adc_10_RX610xV.h
* Version    : 0.10
* Contents    : ADC header
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
* History    : 2009.11.11
*        : Ver 0.10
*         : 2010.01.08
*        : Ver 0.11
*        : Added a mask to the Control error detection.
*        : 2010.01.22
*        : Ver 0.12
*        : Added protection against multiple inclusion.
*""FILE COMMENT END""**************************************************/

#ifndef R_PDL_ADC_10_RX610xV_H
#define R_PDL_ADC_10_RX610xV_H

#define ADC_10_UNITS 4

extern VoidCallBackFunc rpdl_ADC_10_callback_func[];

/* Library prototypes */
bool R_ADC_10_CreateAll(
  const uint8_t,
  const uint32_t,
  const uint32_t,
  const float,
  VoidCallBackFunc const,
  const uint8_t
);
bool R_ADC_10_DestroyAll(
  const uint8_t
);
bool R_ADC_10_ControlAll(
  const uint16_t
);
bool R_ADC_10_ReadAll(
  const uint8_t,
  uint16_t * const
);
bool ReturnFalse(void);

/* Macro definitions */

#define R_ADC_10_Create(a, b, c, d, e, f) \
( \
( ( ((a) < ADC_10_UNITS) && ((f) <= IPL_MAX) ) ) ? \
R_ADC_10_CreateAll( (a), (b), (c), (d), (e), (f)): \
ReturnFalse() \
)

#define R_ADC_10_Destroy(a) \
( \
( (a) < ADC_10_UNITS ) ? \
R_ADC_10_DestroyAll( (a) ): \
ReturnFalse() \
)

#define R_ADC_10_Control(a) \
( \
( ((a) & 0xFFu) != 0x00u ) ? \
R_ADC_10_ControlAll( (a) ): \
ReturnFalse() \
)

#define R_ADC_10_Read(a, b) \
( \
( (a) < ADC_10_UNITS ) ? \
R_ADC_10_ReadAll( (a), (b) ): \
ReturnFalse() \
)

#endif
/* End of file */
