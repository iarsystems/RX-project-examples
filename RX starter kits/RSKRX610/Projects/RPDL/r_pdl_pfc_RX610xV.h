/* Adapted for use with IAR Embedded Workbench */
/*""FILE COMMENT""*******************************************************
* System Name  : PFC API for RX610xV
* File Name    : r_pdl_pfc_RX610xV.h
* Version    : 0.10
* Contents    : PFC API header
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

#ifndef R_PDL_PFC_RX610xV_H
#define R_PDL_PFC_RX610xV_H

/* Library prototypes */
bool R_PFC_ReadRegisterAll(
  const uint8_t,
  uint8_t * const
);
bool R_PFC_WriteRegisterAll(
  const uint8_t,
  const uint8_t
);
bool R_PFC_ModifyRegisterAll(
  const uint8_t,
  const uint8_t,
  const uint8_t
);
bool ReturnFalse(void);

/* Macro definitions */

/* R_PFC_Read */
/* Call the function if the PFC number is valid (0 to 9). */
#define R_PFC_Read(a, b) \
( \
((a) <= 9) ? \
R_PFC_ReadRegisterAll( (a), (b) ) : \
ReturnFalse() \
)

/* R_PFC_Write */
/* Call the function if the PFC number is valid (0 to 9). */
#define R_PFC_Write(a, b) \
( \
((a) <= 9) ? \
R_PFC_WriteRegisterAll( (a), (b) ) : \
ReturnFalse() \
)

/* R_PFC_Modify */
/* Call the function if the PFC number is valid (0 to 9). */
#define R_PFC_Modify(a, b, c) \
( \
((a) <= 9) ? \
R_PFC_ModifyRegisterAll( (a), (b), (c) ) : \
ReturnFalse() \
)

#endif
/* End of file */
