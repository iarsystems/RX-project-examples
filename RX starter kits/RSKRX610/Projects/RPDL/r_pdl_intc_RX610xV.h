/* Adapted for use with IAR Embedded Workbench */
/*""FILE COMMENT""*******************************************************
* System Name  : External interrupt API for RX610xV
* File Name    : r_pdl_intc_RX610xV.h
* Version    : 0.10
* Contents    : INTC API header
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
*        : 2010.02.24
*        : Ver 0.12
*        : Removed a FastInterrupt parameter
*""FILE COMMENT END""**************************************************/

#ifndef R_PDL_INTC_RX610xV_H
#define R_PDL_INTC_RX610xV_H

/* Callback function storage */
extern VoidCallBackFunc rpdl_INTC_callback_func[];

/* Library prototypes */
bool R_INTC_CreateExtInterruptAll(
  const uint8_t,
  const uint16_t,
  VoidCallBackFunc const,
  const uint8_t
);
bool R_INTC_CreateFastInterruptAll(
  const uint8_t
);
bool R_INTC_CreateExceptionHandlersAll(
  VoidCallBackFunc const,
  VoidCallBackFunc const,
  VoidCallBackFunc const
);
bool R_INTC_ControlExtInterruptAll(
  const uint8_t,
  const uint16_t
);
bool R_INTC_GetExtInterruptStatusAll(
  const uint8_t,
  uint8_t * const
);
bool R_INTC_ReadAll(
  const uint8_t,
  const uint8_t,
  uint8_t * const
);
bool R_INTC_WriteAll(
  const uint8_t,
  const uint8_t,
  const uint8_t
);
bool R_INTC_ModifyAll(
  const uint8_t,
  const uint8_t,
  const uint8_t,
  const uint8_t
);
bool ReturnFalse(void);

/* Macro definitions */

#define R_INTC_CreateExtInterrupt(a, b, c, d) \
( \
( ( (a) <= PDL_INTC_NMI ) & ((d) <= IPL_MAX) ) ? \
R_INTC_CreateExtInterruptAll( (a), (b), (c), (d) ) : \
ReturnFalse() \
)

#define R_INTC_ControlExtInterrupt(a, b) \
( \
( (a) <= PDL_INTC_NMI ) ? \
R_INTC_ControlExtInterruptAll( (a), (b) ) : \
ReturnFalse() \
)

#define R_INTC_GetExtInterruptStatus(a, b) \
( \
( (a) <= PDL_INTC_NMI ) ? \
R_INTC_GetExtInterruptStatusAll( (a), (b) ) : \
ReturnFalse() \
)

#define R_INTC_CreateFastInterrupt(a) \
( R_INTC_CreateFastInterruptAll( (a) ) )

#define R_INTC_CreateExceptionHandlers(a, b, c) \
( R_INTC_CreateExceptionHandlersAll( (a), (b), (c) ) )

#define R_INTC_Read(a, b, c) \
( R_INTC_ReadAll( (a), (b), (c) ) )

#define R_INTC_Write(a, b, c) \
( R_INTC_WriteAll( (a), (b), (c) ) )

#define R_INTC_Modify(a, b, c, d) \
( R_INTC_ModifyAll( (a), (b), (c), (d) ) )

#endif
/* End of file */
