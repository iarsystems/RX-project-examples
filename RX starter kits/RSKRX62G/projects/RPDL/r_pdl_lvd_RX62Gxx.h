/* Adapted for use with IAR Embedded Workbench */
/*""FILE COMMENT""*************************************************************
* System Name  : Voltage detection circuit API for RX62G and RX62T
* File Name    : r_pdl_lvd_RX62Gxx.h
* Version    : 1.00
* Contents    : LVD API header
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

#ifndef R_PDL_LVD_RX62Gxx_H
#define R_PDL_LVD_RX62Gxx_H

/* Library prototypes */
bool R_LVD_ControlAll(
  const uint8_t
);

/* Macro definitions */

#define R_LVD_Control(a) \
( \
R_LVD_ControlAll( (a) ) \
)

#endif
/* End of file */
