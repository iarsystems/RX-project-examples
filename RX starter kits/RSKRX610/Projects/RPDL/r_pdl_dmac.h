/* Adapted for use with IAR Embedded Workbench */
/*""FILE COMMENT""*******************************************************
* System Name  : DMAC API for RX610xV
* File Name    : r_pdl_dmac.h
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
* Copyright,2010 RENESAS TECHNOLOGY CORPORATION
* AND RENESAS SOLUTIONS CORPORATION
************************************************************************
* History    : 2009.11.24
*        : Ver 0.10
*        : 2010.01.22
*        : Ver 0.11
*        : Added protection against multiple inclusion.
*        : 2010.01.26
*        : Ver 0.12
*        : Removed const keywords from the function prototypes.
*        : 2010.02.08
*        : Ver 0.13
*        : Moved the DMAC trigger definitions from the common header.
*        : Added a parameter to R_DMAC_Control
*        : 2010.02.11
*        : Ver 0.14
*        : Moved the channel definitions above 0, 1, 2 and 3.
*        : 2010.02.12
*        : Ver 0.15
*        : Renamed the Byte Count register selection definition.
*""FILE COMMENT END""**************************************************/

#ifndef R_PDL_DMAC_H
#define R_PDL_DMAC_H

#include "r_pdl_common_defs_RX610xV.h"

/* Function prototypes */
bool R_DMAC_Create(
  uint8_t,
  uint32_t,
  uint8_t,
  void *,
  void *,
  uint32_t,
  void *,
  void *,
  uint32_t,
  void *,
  uint8_t
);
bool R_DMAC_Destroy(
  void
);
bool R_DMAC_Control(
  uint8_t,
  uint8_t,
  uint8_t,
  uint32_t,
  uint32_t,
  uint32_t
);
bool R_DMAC_GetStatus(
  uint8_t,
  uint16_t *,
  uint32_t *,
  uint32_t *,
  uint32_t *
);
  
/* Transfer system selection */
#define PDL_DMAC_SINGLE                0x00000001ul
#define PDL_DMAC_CONSECUTIVE            0x00000002ul
#define PDL_DMAC_NON_STOP              0x00000004ul

/* Address addition direction selection */
#define PDL_DMAC_SOURCE_ADDRESS_FIXED        0x00000008ul
#define PDL_DMAC_SOURCE_ADDRESS_PLUS        0x00000010ul
#define PDL_DMAC_SOURCE_ADDRESS_MINUS        0x00000020ul
#define PDL_DMAC_SOURCE_ADDRESS_ROTATE        0x00000040ul
#define PDL_DMAC_DESTINATION_ADDRESS_FIXED      0x00000080ul
#define PDL_DMAC_DESTINATION_ADDRESS_PLUS      0x00000100ul
#define PDL_DMAC_DESTINATION_ADDRESS_MINUS      0x00000200ul
#define PDL_DMAC_DESTINATION_ADDRESS_ROTATE      0x00000400ul

/* Transfer data size */
#define PDL_DMAC_SIZE_8                0x00000800ul
#define PDL_DMAC_SIZE_16              0x00001000ul
#define PDL_DMAC_SIZE_32              0x00002000ul

/* Operand transfer data count */
#define PDL_DMAC_COUNT_1              0x00004000ul
#define PDL_DMAC_COUNT_2              0x00008000ul
#define PDL_DMAC_COUNT_4              0x00010000ul
#define PDL_DMAC_COUNT_8              0x00020000ul
#define PDL_DMAC_COUNT_16              0x00040000ul
#define PDL_DMAC_COUNT_32              0x00080000ul
#define PDL_DMAC_COUNT_64              0x00100000ul
#define PDL_DMAC_COUNT_128              0x00200000ul

/* End-of-transfer reload control */
#define PDL_DMAC_SOURCE_ADDRESS_RELOAD_DISABLE    0x00400000ul
#define PDL_DMAC_SOURCE_ADDRESS_RELOAD_ENABLE    0x00800000ul
#define PDL_DMAC_DESTINATION_ADDRESS_RELOAD_DISABLE  0x01000000ul
#define PDL_DMAC_DESTINATION_ADDRESS_RELOAD_ENABLE  0x02000000ul
#define PDL_DMAC_COUNT_RELOAD_DISABLE        0x04000000ul
#define PDL_DMAC_COUNT_RELOAD_ENABLE        0x08000000ul

/* DTC trigger control */
#define PDL_DMAC_DTC_TRIGGER_OFF          0x10000000ul
#define PDL_DMAC_DTC_TRIGGER_ON            0x20000000ul

/* DMAC trigger request source */
#define PDL_DMAC_REQUEST_SW      0x00u
#define PDL_DMAC_REQUEST_CMT0    0x01u
#define PDL_DMAC_REQUEST_CMT1    0x02u
#define PDL_DMAC_REQUEST_CMT2    0x03u
#define PDL_DMAC_REQUEST_CMT3    0x04u
#define PDL_DMAC_REQUEST_IRQ0    0x05u
#define PDL_DMAC_REQUEST_IRQ1    0x06u
#define PDL_DMAC_REQUEST_IRQ2    0x07u
#define PDL_DMAC_REQUEST_IRQ3    0x08u
#define PDL_DMAC_REQUEST_ADC0    0x09u
#define PDL_DMAC_REQUEST_ADC1    0x0Au
#define PDL_DMAC_REQUEST_ADC2    0x0Bu
#define PDL_DMAC_REQUEST_ADC3    0x0Cu
#define PDL_DMAC_REQUEST_TPU0    0x0Du
#define PDL_DMAC_REQUEST_TPU1    0x0Eu
#define PDL_DMAC_REQUEST_TPU2    0x0Fu
#define PDL_DMAC_REQUEST_TPU3    0x10u
#define PDL_DMAC_REQUEST_TPU4    0x11u
#define PDL_DMAC_REQUEST_TPU5    0x12u
#define PDL_DMAC_REQUEST_TPU6    0x13u
#define PDL_DMAC_REQUEST_TPU7    0x14u
#define PDL_DMAC_REQUEST_TPU8    0x15u
#define PDL_DMAC_REQUEST_TPU9    0x16u
#define PDL_DMAC_REQUEST_TPU10    0x17u
#define PDL_DMAC_REQUEST_TPU11    0x18u
#define PDL_DMAC_REQUEST_SCI0_RX  0x19u
#define PDL_DMAC_REQUEST_SCI0_TX  0x1Au
#define PDL_DMAC_REQUEST_SCI1_RX  0x1Bu
#define PDL_DMAC_REQUEST_SCI1_TX  0x1Cu
#define PDL_DMAC_REQUEST_SCI2_RX  0x1Du
#define PDL_DMAC_REQUEST_SCI2_TX  0x1Eu
#define PDL_DMAC_REQUEST_SCI3_RX  0x1Fu
#define PDL_DMAC_REQUEST_SCI3_TX  0x20u
#define PDL_DMAC_REQUEST_SCI4_RX  0x21u
#define PDL_DMAC_REQUEST_SCI4_TX  0x22u
#define PDL_DMAC_REQUEST_SCI5_RX  0x23u
#define PDL_DMAC_REQUEST_SCI5_TX  0x24u
#define PDL_DMAC_REQUEST_SCI6_RX  0x25u
#define PDL_DMAC_REQUEST_SCI6_TX  0x26u
#define PDL_DMAC_REQUEST_IIC0_RX  0x27u
#define PDL_DMAC_REQUEST_IIC0_TX  0x28u
#define PDL_DMAC_REQUEST_IIC1_RX  0x29u
#define PDL_DMAC_REQUEST_IIC1_TX  0x2Au

/* Channel selection */
#define PDL_DMAC_0    0x10u
#define PDL_DMAC_1    0x20u
#define PDL_DMAC_2    0x40u
#define PDL_DMAC_3    0x80u
#define PDL_DMAC_ALL  (PDL_DMAC_0 | PDL_DMAC_1 | PDL_DMAC_2 | PDL_DMAC_3)

/* Enable / suspend control */
#define PDL_DMAC_ENABLE        0x01u
#define PDL_DMAC_SUSPEND      0x02u

/* Software trigger control */
#define PDL_DMAC_START        0x04u

/* Transfer end detection flag control */
#define PDL_DMAC_CLEAR_TEDF      0x08u

/* Trigger control */
#define PDL_DMAC_TRIGGER_RESET    0x10u

/* Reload registers selection */
#define PDL_DMAC_UPDATE_SOURCE    0x20u
#define PDL_DMAC_UPDATE_DESTINATION  0x40u
#define PDL_DMAC_UPDATE_COUNT    0x80u

#endif
/* End of file */
