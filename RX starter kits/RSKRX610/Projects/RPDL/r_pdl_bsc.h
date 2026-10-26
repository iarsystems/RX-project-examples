/* Adapted for use with IAR Embedded Workbench */
/*""FILE COMMENT""*******************************************************
* System Name  : BSC API for RX610xV
* File Name    : r_pdl_bsc.h
* Version    : 0.10
* Contents    : BSC API header
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
*        : 2010.01.22
*        : Ver 0.11
*        : Added protection against multiple inclusion.
*        : 2010.01.26
*        : Ver 0.12
*        : Removed const keywords from the function prototypes.
*""FILE COMMENT END""**************************************************/

#ifndef R_PDL_BSC_H
#define R_PDL_BSC_H

#include "r_pdl_common_defs_RX610xV.h"

/* Function prototypes */
bool R_BSC_Create(
  uint32_t,
  uint32_t,
  uint8_t,
  void *,
  uint8_t
);
bool R_BSC_CreateArea(
  uint8_t,
  uint16_t,
  uint8_t,
  uint8_t,
  uint8_t,
  uint8_t,
  uint8_t,
  uint8_t,
  uint8_t,
  uint8_t,
  uint8_t,
  uint8_t,
  uint8_t,
  uint8_t,
  uint8_t
);
bool R_BSC_Destroy(
  uint8_t
);
bool R_BSC_Control(
  uint8_t
);
bool R_BSC_GetStatus(
  uint8_t *
);

/* Chip select pin selection */
#define PDL_BSC_CS2_A      0x00000001ul
#define PDL_BSC_CS2_B      0x00000002ul
#define PDL_BSC_CS3_A      0x00000004ul
#define PDL_BSC_CS3_B      0x00000008ul
#define PDL_BSC_CS4_A      0x00000010ul
#define PDL_BSC_CS4_B      0x00000020ul
#define PDL_BSC_CS4_C      0x00000040ul
#define PDL_BSC_CS4_D      0x00000080ul
#define PDL_BSC_CS5_A      0x00000100ul
#define PDL_BSC_CS5_B      0x00000200ul
#define PDL_BSC_CS5_C      0x00000400ul
#define PDL_BSC_CS5_D      0x00000800ul
#define PDL_BSC_CS6_A      0x00001000ul
#define PDL_BSC_CS6_B      0x00002000ul
#define PDL_BSC_CS6_C      0x00004000ul
#define PDL_BSC_CS6_D      0x00008000ul
#define PDL_BSC_CS7_A      0x00010000ul
#define PDL_BSC_CS7_B      0x00020000ul
#define PDL_BSC_CS7_C      0x00040000ul
#define PDL_BSC_CS7_D      0x00080000ul

/* Strobe pin control */
#define PDL_BSC_WR1BC1_ENABLE  0x00100000ul
#define PDL_BSC_WR1BC1_DISABLE  0x00200000ul

/* Address output control */
#define PDL_BSC_A0BC0_DISABLE  0x00000001ul
#define PDL_BSC_A1_DISABLE    0x00000002ul
#define PDL_BSC_A2_DISABLE    0x00000004ul
#define PDL_BSC_A3_DISABLE    0x00000008ul
#define PDL_BSC_A4_DISABLE    0x00000010ul
#define PDL_BSC_A5_DISABLE    0x00000020ul
#define PDL_BSC_A6_DISABLE    0x00000040ul
#define PDL_BSC_A7_DISABLE    0x00000080ul
#define PDL_BSC_A8_DISABLE    0x00000100ul
#define PDL_BSC_A9_DISABLE    0x00000200ul
#define PDL_BSC_A10_DISABLE    0x00000400ul
#define PDL_BSC_A11_DISABLE    0x00000800ul
#define PDL_BSC_A12_DISABLE    0x00001000ul
#define PDL_BSC_A13_DISABLE    0x00002000ul
#define PDL_BSC_A14_DISABLE    0x00004000ul
#define PDL_BSC_A15_DISABLE    0x00008000ul
#define PDL_BSC_A16_DISABLE    0x00010000ul
#define PDL_BSC_A17_DISABLE    0x00020000ul
#define PDL_BSC_A18_DISABLE    0x00040000ul
#define PDL_BSC_A19_DISABLE    0x00080000ul
#define PDL_BSC_A20_DISABLE    0x00100000ul
#define PDL_BSC_A21_DISABLE    0x00200000ul
#define PDL_BSC_A22_DISABLE    0x00400000ul
#define PDL_BSC_A23_DISABLE    0x00800000ul

/* Error monitoring */
#define PDL_BSC_ERROR_ILLEGAL_ADDRESS_ENABLE  0x01u
#define PDL_BSC_ERROR_ILLEGAL_ADDRESS_DISABLE  0x02u
#define PDL_BSC_ERROR_TIME_OUT_ENABLE      0x04u
#define PDL_BSC_ERROR_TIME_OUT_DISABLE      0x08u

/* Bus width */
#define PDL_BSC_WIDTH_16        0x0001u
#define PDL_BSC_WIDTH_8          0x0002u

/* Endian mode */
#define PDL_BSC_ENDIAN_SAME        0x0004u
#define PDL_BSC_ENDIAN_OPPOSITE      0x0008u

/* Write access mode */
#define PDL_BSC_WRITE_BYTE        0x0010u
#define PDL_BSC_WRITE_SINGLE      0x0020u

/* Write access mode */
#define PDL_BSC_WAIT_DISABLE      0x0040u
#define PDL_BSC_WAIT_ENABLE        0x0080u

/* Page access control */
#define PDL_BSC_PAGE_READ_DISABLE    0x0100u
#define PDL_BSC_PAGE_READ_NORMAL    0x0200u
#define PDL_BSC_PAGE_READ_CONTINUOUS  0x0400u
#define PDL_BSC_PAGE_WRITE_DISABLE    0x0800u
#define PDL_BSC_PAGE_WRITE_ENABLE    0x1000u

/* Error clearing */
#define PDL_BSC_ERROR_CLEAR   0x01u

#endif
/* End of file */
