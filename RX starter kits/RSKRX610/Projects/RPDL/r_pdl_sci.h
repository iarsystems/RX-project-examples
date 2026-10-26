/* Adapted for use with IAR Embedded Workbench */
/*""FILE COMMENT""*******************************************************
* System Name  : Serial Interface API for RX610xV
* File Name    : r_pdl_sci.h
* Version    : 0.10
* Contents    : Serial Interface API header
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
* History    : 2009.11.12
*        : Ver 0.10
*        : 2009.11.26
*        : Ver 0.11
*        : Removed the Error callback function
*        : 2009.12.03
*        : Ver 0.12
*        : Changed the receive count and threshold variables to 16 bits
*        : 2010.01.22
*        : Ver 0.13
*        : Added protection against multiple inclusion.
*        : 2010.01.26
*        : Ver 0.14
*        : Removed const keywords from the function prototypes.
*        : 2010.02.15
*        : Ver 0.15
*        : Renamed R_SCI_Stop to R_SCI_Control
*""FILE COMMENT END""**************************************************/

#ifndef R_PDL_SCI_H
#define R_PDL_SCI_H

#include "r_pdl_common_defs_RX610xV.h"

/* Function prototypes */
bool R_SCI_Create(
  uint8_t,
  uint32_t,
  uint32_t,
  uint8_t
);
bool R_SCI_Destroy(
  uint8_t
);
bool R_SCI_Send(
  uint8_t,
  uint8_t,
  uint8_t *,
  uint16_t,
  void *
);
bool R_SCI_Receive(
  uint8_t,
  uint8_t,
  uint8_t *,
  uint16_t,
  void *,
  void *
);
bool R_SCI_Control(
  uint8_t,
  uint8_t
);
bool R_SCI_GetStatus(
  uint8_t,
  uint8_t *,
  uint8_t *,
  uint16_t *,
  uint16_t *
);

/* Operation mode */
#define PDL_SCI_ASYNC      0x00000001ul
#define PDL_SCI_SYNC      0x00000002ul
#define PDL_SCI_SMART      0x00000004ul

/* Common selections */

/* Data format */
#define PDL_SCI_LSB_FIRST    0x00000008ul
#define PDL_SCI_MSB_FIRST    0x00000010ul

#define PDL_SCI_INVERSION_OFF  0x00000020ul
#define PDL_SCI_INVERSION_ON  0x00000040ul

#define PDL_SCI_TX_CONNECTED  0x00000080ul
#define PDL_SCI_TX_DISCONNECTED  0x00000100ul
#define PDL_SCI_RX_CONNECTED  0x00000200ul
#define PDL_SCI_RX_DISCONNECTED  0x00000400ul

/* Asynchronous selections */

/* Data clock source */
#define PDL_SCI_CLK_INT_IO    0x00010000ul
#define PDL_SCI_CLK_INT_OUT    0x00020000ul
#define PDL_SCI_CLK_EXT_DIV_8  0x00040000ul
#define PDL_SCI_CLK_EXT_DIV_16  0x00080000ul
#define PDL_SCI_CLK_TMR      0x00100000ul

/* Data length */
#define PDL_SCI_8_BIT_LENGTH  0x00200000ul
#define PDL_SCI_7_BIT_LENGTH  0x00400000ul

/* Parity mode */
#define PDL_SCI_PARITY_NONE    0x00800000ul
#define PDL_SCI_PARITY_EVEN    0x01000000ul
#define PDL_SCI_PARITY_ODD    0x02000000ul

/* Stop bit length */
#define PDL_SCI_STOP_1      0x04000000ul
#define PDL_SCI_STOP_2      0x08000000ul

#define PDL_SCI_8N1  (PDL_SCI_8_BIT_LENGTH | PDL_SCI_PARITY_NONE | PDL_SCI_STOP_1)

/* Synchronous selections */

/* Data clock source */
#define PDL_SCI_CLK_EXT      0x00010000ul

/* Smart card selections */

/* Base clock pulse cycle count */
#define PDL_SCI_BCP_32      0x00010000ul
#define PDL_SCI_BCP_64      0x00020000ul
#define PDL_SCI_BCP_93      0x00040000ul
#define PDL_SCI_BCP_128      0x00080000ul
#define PDL_SCI_BCP_186      0x00100000ul
#define PDL_SCI_BCP_256      0x00200000ul
#define PDL_SCI_BCP_372      0x00400000ul
#define PDL_SCI_BCP_512      0x00800000ul

/* Block transfer mode */
#define PDL_SCI_BLOCK_MODE_OFF  0x01000000ul
#define PDL_SCI_BLOCK_MODE_ON  0x02000000ul

/* GSM mode */
#define PDL_SCI_GSM_MODE_OFF  0x04000000ul
#define PDL_SCI_GSM_MODE_ON    0x08000000ul

/* Clock output control */
#define PDL_SCI_SCK_OUTPUT_OFF  0x10000000ul
#define PDL_SCI_SCK_OUTPUT_LOW  0x20000000ul
#define PDL_SCI_SCK_OUTPUT_ON  0x40000000ul
#define PDL_SCI_SCK_OUTPUT_HIGH  0x80000000ul

/* CKS selection (optional) */
#define PDL_SCI_PCLK_DIV_1    0x01000000ul
#define PDL_SCI_PCLK_DIV_4    0x02000000ul
#define PDL_SCI_PCLK_DIV_16    0x03000000ul
#define PDL_SCI_PCLK_DIV_64    0x04000000ul

/* ABCS selection (optional) */
#define PDL_SCI_CYCLE_BIT_16  0x10000000ul
#define PDL_SCI_CYCLE_BIT_8    0x20000000ul

/* DMAC / DTC trigger control */
#define PDL_SCI_DMAC_DTC_TRIGGER_DISABLE  0x01u
#define PDL_SCI_DMAC_TRIGGER_ENABLE      0x02u
#define PDL_SCI_DTC_TRIGGER_ENABLE      0x04u

/* Stop control */
#define PDL_SCI_STOP_TX            0x01u
#define PDL_SCI_STOP_RX            0x02u
#define PDL_SCI_STOP_TX_AND_RX  (PDL_SCI_STOP_TX | PDL_SCI_STOP_RX)

/* Break output control */
#define PDL_SCI_OUTPUT_MARK          0x04u
#define PDL_SCI_OUTPUT_SPACE        0x08u

/* Error flag control */
#define PDL_SCI_CLEAR_RECEIVE_ERROR_FLAGS  0x10u

#endif
/* End of file */
