/* Adapted for use with IAR Embedded Workbench */
/*""FILE COMMENT""*******************************************************
* System Name  : I²C API for RX610xV
* File Name    : r_pdl_iic_RX610xV.h
* Version    : 0.10
* Contents    : I²C API header
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
* History    : 2010.03.02
*        : Ver 0.10
*""FILE COMMENT END""**************************************************/

#ifndef R_PDL_IIC_RX610xV_H
#define R_PDL_IIC_RX610xV_H

#define IIC_CHANNELS 2

/* Globals */
extern volatile uint8_t rpdl_IIC_stop[IIC_CHANNELS];
extern volatile uint8_t rpdl_IIC_current_state[IIC_CHANNELS];
extern volatile uint8_t rpdl_IIC_next_state[IIC_CHANNELS];
extern volatile uint8_t rpdl_IIC_slave_address_lower[IIC_CHANNELS];
extern volatile uint8_t rpdl_IIC_slave_address_upper[IIC_CHANNELS];
extern volatile const uint8_t * rpdl_IIC_tx_data_pointer[IIC_CHANNELS];
extern volatile uint8_t * rpdl_IIC_rx_data_pointer[IIC_CHANNELS];
extern volatile uint8_t * rpdl_IIC_rx_data_start[IIC_CHANNELS];
extern volatile uint16_t rpdl_IIC_rx_counter[IIC_CHANNELS];
extern volatile uint16_t rpdl_IIC_tx_counter[IIC_CHANNELS];
extern volatile uint16_t rpdl_IIC_rx_threshold[IIC_CHANNELS];
extern volatile uint16_t rpdl_IIC_tx_threshold[IIC_CHANNELS];

/* IIC state machine states */
typedef enum {
  SEND_START,
  MASTER_SEND_SLAVE_ADDRESS_7,
  MASTER_SEND_SLAVE_ADDRESS_10a,
  MASTER_SEND_SLAVE_ADDRESS_10b,
  MASTER_SEND_DATA,
  MASTER_WAIT_FOR_TX_COMPLETE,
  MASTER_WAIT_FOR_STOP,
  EXIT_LOOP,
  MASTER_START_READ,
  MASTER_READ_DATA,
  SEND_STOP,
  SLAVE_MONITOR,
  SLAVE_SEND_DATA,
  SLAVE_READ_SLAVE_ADDRESS,
  SLAVE_READ_DATA
} iic_states;

/* Callback function prototype */
extern VoidCallBackFunc  rpdl_IIC_callback_func[IIC_CHANNELS];

/* Library prototypes */
bool R_IIC_CreateAll(
  const uint8_t,
  const uint32_t,
  const uint32_t,
  const uint16_t,
  const uint16_t,
  const uint16_t,
  const uint32_t,
  const uint32_t
);
bool R_IIC_DestroyAll(
  const uint8_t
);
bool R_IIC_MasterSendAll(
  const uint8_t,
  const uint8_t,
  const uint16_t,
  const uint8_t * const,
  const uint16_t,
  VoidCallBackFunc const,
  const uint8_t
);
bool R_IIC_MasterReceiveAll(
  const uint8_t,
  const uint8_t,
  const uint16_t,
  uint8_t * const,
  const uint16_t,
  VoidCallBackFunc const,
  const uint8_t
);
bool R_IIC_SlaveMonitorAll(
  const uint8_t,
  VoidCallBackFunc const,
  const uint8_t
);
bool R_IIC_SlaveSendAll(
  const uint8_t,
  const uint8_t,
  const uint8_t * const,
  const uint16_t
);
bool R_IIC_SlaveReceiveAll(
  const uint8_t,
  const uint8_t,
  volatile uint8_t * const,
  const uint16_t
);
bool R_IIC_ControlAll(
  const uint8_t,
  const uint8_t
);
bool R_IIC_GetStatusAll(
  const uint8_t,
  uint16_t * const,
  uint16_t * const,
  uint16_t * const
);
bool ReturnFalse(void);

/* Macro definitions */

#define R_IIC_Create(a, b, c, d, e, f, g, h) \
( \
( (a) < IIC_CHANNELS ) ? \
R_IIC_CreateAll( (a), (b), (c), (d), (e), (f), (g), (h) ) : \
ReturnFalse() \
)

#define R_IIC_Destroy(a) \
( \
( (a) < IIC_CHANNELS ) ? \
R_IIC_DestroyAll( (a) ) : \
ReturnFalse() \
)

#define R_IIC_MasterSend(a, b, c, d, e, f, g) \
( \
( (a) < IIC_CHANNELS ) ? \
R_IIC_MasterSendAll( (a), (b), (c), (d), (e), (f), (g) ) : \
ReturnFalse() \
)

#define R_IIC_MasterReceive(a, b, c, d, e, f, g) \
( \
( (a) < IIC_CHANNELS ) ? \
R_IIC_MasterReceiveAll( (a), (b), (c), (d), (e), (f), (g) ) : \
ReturnFalse() \
)

#define R_IIC_SlaveMonitor(a, b, c) \
( \
( (a) < IIC_CHANNELS ) ? \
R_IIC_SlaveMonitorAll( (a), (b), (c) ) : \
ReturnFalse() \
)

#define R_IIC_SlaveSend(a, b, c, d) \
( \
( (a) < IIC_CHANNELS ) ? \
R_IIC_SlaveSendAll( (a), (b), (c), (d) ) : \
ReturnFalse() \
)

#define R_IIC_SlaveReceive(a, b, c, d) \
( \
( (a) < IIC_CHANNELS ) ? \
R_IIC_SlaveReceiveAll( (a), (b), (c), (d) ) : \
ReturnFalse() \
)

#define R_IIC_Control(a, b) \
( \
( (a) < IIC_CHANNELS ) ? \
R_IIC_ControlAll( (a), (b) ) : \
ReturnFalse() \
)

#define R_IIC_GetStatus(a, b, c, d) \
( \
( (a) < IIC_CHANNELS ) ? \
R_IIC_GetStatusAll( (a), (b), (c), (d) ) : \
ReturnFalse() \
)

#endif
/* End of file */
