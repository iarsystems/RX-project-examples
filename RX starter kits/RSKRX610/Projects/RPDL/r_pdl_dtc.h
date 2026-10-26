/* Adapted for use with IAR Embedded Workbench */
/*""FILE COMMENT""*******************************************************
* System Name  : DTC API for RX610xV
* File Name    : r_pdl_dtc.h
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
* History    : 2010.02.05
*        : Ver 0.10
*""FILE COMMENT END""**************************************************/

#ifndef R_PDL_DTC_H
#define R_PDL_DTC_H

#include "r_pdl_common_defs_RX610xV.h"

/* Function prototypes */
bool R_DTC_Set(
  uint8_t,
  uint32_t *
);
bool R_DTC_Create(
  uint32_t,
  uint32_t *,
  void *,
  void *,
  uint16_t,
  uint8_t
);
bool R_DTC_Destroy(
  void
);
bool R_DTC_Control(
  uint32_t
);
bool R_DTC_GetStatus(
  uint32_t *,
  uint8_t *,
  uint32_t *,
  uint32_t *,
   uint16_t *
);

/* Chain transfer control */
#define PDL_DTC_CHAIN_AFTER_REPEAT_DISABLE  0x01u
#define PDL_DTC_CHAIN_AFTER_REPEAT_ENABLE  0x02u

/* Read skip control */
#define PDL_DTC_READ_SKIP_DISABLE      0x04u
#define PDL_DTC_READ_SKIP_ENABLE      0x08u

/* Address size control */
#define PDL_DTC_ADDRESS_FULL        0x10u
#define PDL_DTC_ADDRESS_SHORT        0x20u

/* Transfer mode selection */
#define PDL_DTC_NORMAL            0x00000001ul
#define PDL_DTC_REPEAT            0x00000002ul
#define PDL_DTC_BLOCK            0x00000004ul
#define PDL_DTC_SOURCE            0x00000008ul
#define PDL_DTC_DESTINATION          0x00000010ul

/* Address direction selection */
#define PDL_DTC_SOURCE_ADDRESS_FIXED    0x00000020ul
#define PDL_DTC_SOURCE_ADDRESS_PLUS      0x00000040ul
#define PDL_DTC_SOURCE_ADDRESS_MINUS    0x00000080ul
#define PDL_DTC_DESTINATION_ADDRESS_FIXED  0x00000100ul
#define PDL_DTC_DESTINATION_ADDRESS_PLUS  0x00000200ul
#define PDL_DTC_DESTINATION_ADDRESS_MINUS  0x00000400ul

/* Transfer data size */
#define PDL_DTC_SIZE_8            0x00000800ul
#define PDL_DTC_SIZE_16            0x00001000ul
#define PDL_DTC_SIZE_32            0x00002000ul

/* Chain transfer control */
#define PDL_DTC_CHAIN_DISABLE        0x00004000ul
#define PDL_DTC_CHAIN_CONTINUOUS      0x00008000ul
#define PDL_DTC_CHAIN_0            0x00010000ul

/* Interrupt generation */
#define PDL_DTC_IRQ_COMPLETE        0x00020000ul
#define PDL_DTC_IRQ_TRANSFER        0x00040000ul

#define PDL_DTC_TRIGGER_CMT0  (uint32_t)(PDL_INTC_VECTOR_CMT0 << 24)
#define PDL_DTC_TRIGGER_CMT1  (uint32_t)(PDL_INTC_VECTOR_CMT1 << 24)
#define PDL_DTC_TRIGGER_CMT2  (uint32_t)(PDL_INTC_VECTOR_CMT2 << 24)
#define PDL_DTC_TRIGGER_CMT3  (uint32_t)(PDL_INTC_VECTOR_CMT3 << 24)
#define PDL_DTC_TRIGGER_IRQ0  (uint32_t)(PDL_INTC_VECTOR_IRQ0 << 24)
#define PDL_DTC_TRIGGER_IRQ1  (uint32_t)(PDL_INTC_VECTOR_IRQ1 << 24)
#define PDL_DTC_TRIGGER_IRQ2  (uint32_t)(PDL_INTC_VECTOR_IRQ2 << 24)
#define PDL_DTC_TRIGGER_IRQ3  (uint32_t)(PDL_INTC_VECTOR_IRQ3 << 24)
#define PDL_DTC_TRIGGER_IRQ4  (uint32_t)(PDL_INTC_VECTOR_IRQ4 << 24)
#define PDL_DTC_TRIGGER_IRQ5  (uint32_t)(PDL_INTC_VECTOR_IRQ5 << 24)
#define PDL_DTC_TRIGGER_IRQ6  (uint32_t)(PDL_INTC_VECTOR_IRQ6 << 24)
#define PDL_DTC_TRIGGER_IRQ7  (uint32_t)(PDL_INTC_VECTOR_IRQ7 << 24)
#define PDL_DTC_TRIGGER_IRQ8  (uint32_t)(PDL_INTC_VECTOR_IRQ8 << 24)
#define PDL_DTC_TRIGGER_IRQ9  (uint32_t)(PDL_INTC_VECTOR_IRQ9 << 24)
#define PDL_DTC_TRIGGER_IRQ10  (uint32_t)(PDL_INTC_VECTOR_IRQ10 << 24)
#define PDL_DTC_TRIGGER_IRQ11  (uint32_t)(PDL_INTC_VECTOR_IRQ11 << 24)
#define PDL_DTC_TRIGGER_IRQ12  (uint32_t)(PDL_INTC_VECTOR_IRQ12 << 24)
#define PDL_DTC_TRIGGER_IRQ13  (uint32_t)(PDL_INTC_VECTOR_IRQ13 << 24)
#define PDL_DTC_TRIGGER_IRQ14  (uint32_t)(PDL_INTC_VECTOR_IRQ14 << 24)
#define PDL_DTC_TRIGGER_IRQ15  (uint32_t)(PDL_INTC_VECTOR_IRQ15 << 24)
#define PDL_DTC_TRIGGER_ADI0  (uint32_t)(PDL_INTC_VECTOR_ADI0 << 24)
#define PDL_DTC_TRIGGER_ADI1  (uint32_t)(PDL_INTC_VECTOR_ADI1 << 24)
#define PDL_DTC_TRIGGER_ADI2  (uint32_t)(PDL_INTC_VECTOR_ADI2 << 24)
#define PDL_DTC_TRIGGER_ADI3  (uint32_t)(PDL_INTC_VECTOR_ADI3 << 24)
#define PDL_DTC_TRIGGER_TGI0A  (uint32_t)(PDL_INTC_VECTOR_TGI0A << 24)
#define PDL_DTC_TRIGGER_TGI0B  (uint32_t)(PDL_INTC_VECTOR_TGI0B << 24)
#define PDL_DTC_TRIGGER_TGI0C  (uint32_t)(PDL_INTC_VECTOR_TGI0C << 24)
#define PDL_DTC_TRIGGER_TGI0D  (uint32_t)(PDL_INTC_VECTOR_TGI0D << 24)
#define PDL_DTC_TRIGGER_TGI1A  (uint32_t)(PDL_INTC_VECTOR_TGI1A << 24)
#define PDL_DTC_TRIGGER_TGI1B  (uint32_t)(PDL_INTC_VECTOR_TGI1B << 24)
#define PDL_DTC_TRIGGER_TGI2A  (uint32_t)(PDL_INTC_VECTOR_TGI2A << 24)
#define PDL_DTC_TRIGGER_TGI2B  (uint32_t)(PDL_INTC_VECTOR_TGI2B << 24)
#define PDL_DTC_TRIGGER_TGI3A  (uint32_t)(PDL_INTC_VECTOR_TGI3A << 24)
#define PDL_DTC_TRIGGER_TGI3B  (uint32_t)(PDL_INTC_VECTOR_TGI3B << 24)
#define PDL_DTC_TRIGGER_TGI3C  (uint32_t)(PDL_INTC_VECTOR_TGI3C << 24)
#define PDL_DTC_TRIGGER_TGI3D  (uint32_t)(PDL_INTC_VECTOR_TGI3D << 24)
#define PDL_DTC_TRIGGER_TGI4A  (uint32_t)(PDL_INTC_VECTOR_TGI4A << 24)
#define PDL_DTC_TRIGGER_TGI4B  (uint32_t)(PDL_INTC_VECTOR_TGI4B << 24)
#define PDL_DTC_TRIGGER_TGI5A  (uint32_t)(PDL_INTC_VECTOR_TGI5A << 24)
#define PDL_DTC_TRIGGER_TGI5B  (uint32_t)(PDL_INTC_VECTOR_TGI5B << 24)
#define PDL_DTC_TRIGGER_TGI6A  (uint32_t)(PDL_INTC_VECTOR_TGI6A << 24)
#define PDL_DTC_TRIGGER_TGI6B  (uint32_t)(PDL_INTC_VECTOR_TGI6B << 24)
#define PDL_DTC_TRIGGER_TGI6C  (uint32_t)(PDL_INTC_VECTOR_TGI6C << 24)
#define PDL_DTC_TRIGGER_TGI6D  (uint32_t)(PDL_INTC_VECTOR_TGI6D << 24)
#define PDL_DTC_TRIGGER_TGI7A  (uint32_t)(PDL_INTC_VECTOR_TGI7A << 24)
#define PDL_DTC_TRIGGER_TGI7B  (uint32_t)(PDL_INTC_VECTOR_TGI7B << 24)
#define PDL_DTC_TRIGGER_TGI8A  (uint32_t)(PDL_INTC_VECTOR_TGI8A << 24)
#define PDL_DTC_TRIGGER_TGI8B  (uint32_t)(PDL_INTC_VECTOR_TGI8B << 24)
#define PDL_DTC_TRIGGER_TGI9A  (uint32_t)(PDL_INTC_VECTOR_TGI9A << 24)
#define PDL_DTC_TRIGGER_TGI9B  (uint32_t)(PDL_INTC_VECTOR_TGI9B << 24)
#define PDL_DTC_TRIGGER_TGI9C  (uint32_t)(PDL_INTC_VECTOR_TGI9C << 24)
#define PDL_DTC_TRIGGER_TGI9D  (uint32_t)(PDL_INTC_VECTOR_TGI9D << 24)
#define PDL_DTC_TRIGGER_TGI10A  (uint32_t)(PDL_INTC_VECTOR_TGI10A << 24)
#define PDL_DTC_TRIGGER_TGI10B  (uint32_t)(PDL_INTC_VECTOR_TGI10B << 24)
#define PDL_DTC_TRIGGER_TGI11A  (uint32_t)(PDL_INTC_VECTOR_TGI11A << 24)
#define PDL_DTC_TRIGGER_TGI11B  (uint32_t)(PDL_INTC_VECTOR_TGI11B << 24)
#define PDL_DTC_TRIGGER_CMIA0  (uint32_t)(PDL_INTC_VECTOR_CMIA0 << 24)
#define PDL_DTC_TRIGGER_CMIB0  (uint32_t)(PDL_INTC_VECTOR_CMIB0 << 24)
#define PDL_DTC_TRIGGER_CMIA1  (uint32_t)(PDL_INTC_VECTOR_CMIA1 << 24)
#define PDL_DTC_TRIGGER_CMIB1  (uint32_t)(PDL_INTC_VECTOR_CMIB1 << 24)
#define PDL_DTC_TRIGGER_CMIA2  (uint32_t)(PDL_INTC_VECTOR_CMIA2 << 24)
#define PDL_DTC_TRIGGER_CMIB2  (uint32_t)(PDL_INTC_VECTOR_CMIB2 << 24)
#define PDL_DTC_TRIGGER_CMIA3  (uint32_t)(PDL_INTC_VECTOR_CMIA3 << 24)
#define PDL_DTC_TRIGGER_CMIB3  (uint32_t)(PDL_INTC_VECTOR_CMIB3 << 24)
#define PDL_DTC_TRIGGER_DMTEND0  (uint32_t)(PDL_INTC_VECTOR_DMTEND0 << 24)
#define PDL_DTC_TRIGGER_DMTEND1  (uint32_t)(PDL_INTC_VECTOR_DMTEND1 << 24)
#define PDL_DTC_TRIGGER_DMTEND2  (uint32_t)(PDL_INTC_VECTOR_DMTEND2 << 24)
#define PDL_DTC_TRIGGER_DMTEND3  (uint32_t)(PDL_INTC_VECTOR_DMTEND3 << 24)
#define PDL_DTC_TRIGGER_RXI0  (uint32_t)(PDL_INTC_VECTOR_RXI0 << 24)
#define PDL_DTC_TRIGGER_TXI0  (uint32_t)(PDL_INTC_VECTOR_TXI0 << 24)
#define PDL_DTC_TRIGGER_RXI1  (uint32_t)(PDL_INTC_VECTOR_RXI1 << 24)
#define PDL_DTC_TRIGGER_TXI1  (uint32_t)(PDL_INTC_VECTOR_TXI1 << 24)
#define PDL_DTC_TRIGGER_RXI2  (uint32_t)(PDL_INTC_VECTOR_RXI2 << 24)
#define PDL_DTC_TRIGGER_TXI2  (uint32_t)(PDL_INTC_VECTOR_TXI2 << 24)
#define PDL_DTC_TRIGGER_RXI3  (uint32_t)(PDL_INTC_VECTOR_RXI3 << 24)
#define PDL_DTC_TRIGGER_TXI3  (uint32_t)(PDL_INTC_VECTOR_TXI3 << 24)
#define PDL_DTC_TRIGGER_RXI4  (uint32_t)(PDL_INTC_VECTOR_RXI4 << 24)
#define PDL_DTC_TRIGGER_TXI4  (uint32_t)(PDL_INTC_VECTOR_TXI4 << 24)
#define PDL_DTC_TRIGGER_RXI5  (uint32_t)(PDL_INTC_VECTOR_RXI5 << 24)
#define PDL_DTC_TRIGGER_TXI5  (uint32_t)(PDL_INTC_VECTOR_TXI5 << 24)
#define PDL_DTC_TRIGGER_RXI6  (uint32_t)(PDL_INTC_VECTOR_RXI6 << 24)
#define PDL_DTC_TRIGGER_TXI6  (uint32_t)(PDL_INTC_VECTOR_TXI6 << 24)
#define PDL_DTC_TRIGGER_ICRXI0  (uint32_t)(PDL_INTC_VECTOR_ICRXI0 << 24)
#define PDL_DTC_TRIGGER_ICTXI0  (uint32_t)(PDL_INTC_VECTOR_ICTXI0 << 24)
#define PDL_DTC_TRIGGER_ICRXI1  (uint32_t)(PDL_INTC_VECTOR_ICRXI1 << 24)
#define PDL_DTC_TRIGGER_ICTXI1  (uint32_t)(PDL_INTC_VECTOR_ICTXI1 << 24)

/* Stop / Start control */
#define PDL_DTC_STOP  0x00000001ul
#define PDL_DTC_START  0x00000002ul

#endif
/* End of file */
