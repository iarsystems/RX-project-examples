/* Adapted for use with IAR Embedded Workbench */
/*""FILE COMMENT""*******************************************************
* System Name  : MCU API for RX610xV
* File Name    : r_pdl_mcu_RX610xV.h
* Version    : 0.10
* Contents    : MCU API header
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
* History    : 2010.02.09
*        : Ver 0.10
*""FILE COMMENT END""**************************************************/

#ifndef R_PDL_MCU_RX610xV_H
#define R_PDL_MCU_RX610xV_H

/* Library prototypes */
bool R_MCU_ControlAll(
  const uint8_t
);
bool R_MCU_GetStatusAll(
  uint16_t * const
);

/* Macro definitions */
#define R_MCU_Control(a) \
( \
R_MCU_ControlAll( (a) ) \
)

#define R_MCU_GetStatus(a) \
( \
R_MCU_GetStatusAll( (a) ) \
)

#endif
/* End of file */
