/* Adapted for use with IAR Embedded Workbench */
/*""FILE COMMENT""*******************************************************
* System Name  : Serial Interface API for RX610xV
* File Name    : r_pdl_sci_RX610xV.h
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
* History    : 2009.11.25
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
*        : 2010.02.15
*        : Ver 0.14
*        : Renamed R_SCI_StopAll to R_SCI_ControlAll
*""FILE COMMENT END""**************************************************/

#ifndef R_PDL_SCI_RX610xV_H
#define R_PDL_SCI_RX610xV_H

#define SCI_CHANNELS 7

/* Globals */
extern volatile const uint8_t * rpdl_SCI_tx_string_pointer[SCI_CHANNELS];
extern volatile uint8_t * rpdl_SCI_rx_string_pointer[SCI_CHANNELS];
extern volatile uint8_t * rpdl_SCI_rx_string_start[SCI_CHANNELS];
extern volatile uint16_t rpdl_SCI_rx_counter[SCI_CHANNELS];
extern volatile uint16_t rpdl_SCI_tx_counter[SCI_CHANNELS];
extern volatile uint16_t rpdl_SCI_rx_threshold[SCI_CHANNELS];
extern volatile uint16_t rpdl_SCI_tx_threshold[SCI_CHANNELS];
extern volatile uint8_t rpdl_SCI_tx_using_irq[SCI_CHANNELS];
extern volatile uint8_t rpdl_SCI_rx_using_irq[SCI_CHANNELS];
extern volatile uint8_t rpdl_SCI_sync_simultaneous[SCI_CHANNELS];

/* Callback function prototypes */
extern VoidCallBackFunc rpdl_SCI_TX_End_callback_func[SCI_CHANNELS];
extern VoidCallBackFunc rpdl_SCI_RX_End_callback_func[SCI_CHANNELS];
extern VoidCallBackFunc  rpdl_SCI_RX_Error_callback_func[SCI_CHANNELS];

/* Library prototypes */
bool R_SCI_CreateAsyncAll(
  const uint8_t,
  const uint32_t,
  const uint32_t,
  const uint8_t
);
bool R_SCI_CreateSyncAll(
  const uint8_t,
  const uint32_t,
  const uint32_t,
  const uint8_t
);
bool R_SCI_CreateSmartAll(
  const uint8_t,
  const uint32_t,
  const uint32_t,
  const uint8_t
);
bool R_SCI_DestroyAll(
  const uint8_t
);
bool R_SCI_SendAll(
  const uint8_t,
  const uint8_t,
  const uint8_t * const,
  const uint16_t,
  VoidCallBackFunc const
);
bool R_SCI_ReceiveAll(
  const uint8_t,
  const uint8_t,
  uint8_t * const,
  const uint16_t,
  VoidCallBackFunc const,
  VoidCallBackFunc const
);
bool R_SCI_ControlAll(
  const uint8_t,
  const uint8_t
);
bool R_SCI_GetStatusAll(
  const uint8_t,
  uint8_t * const,
  uint8_t * const,
  uint16_t * const,
  uint16_t * const
);
bool ReturnFalse(void);

/* Macro definitions */

#define R_SCI_Create(a, b, c, d) \
( \
( ((a) >= SCI_CHANNELS) || ((c) == 0) || ((d) > IPL_MAX) )? \
ReturnFalse(): \
( (((b) & PDL_SCI_SYNC) == PDL_SCI_SYNC) )? \
R_SCI_CreateSyncAll( (a), (b), (c), (d) ): \
( (((b) & PDL_SCI_SMART) == PDL_SCI_SMART) )? \
R_SCI_CreateSmartAll( (a), (b), (c), (d) ): \
R_SCI_CreateAsyncAll( (a), (b), (c), (d) ) \
)

#define R_SCI_Destroy(a) \
( \
( (a) < SCI_CHANNELS ) ? \
R_SCI_DestroyAll( (a) ): \
ReturnFalse() \
)

#define R_SCI_Send(a, b, c, d, e) \
( \
( (a) < SCI_CHANNELS ) ? \
R_SCI_SendAll( (a), (b), (c), (d), (e) ): \
ReturnFalse() \
)

#define R_SCI_Receive(a, b, c, d, e, f) \
( \
( (a) < SCI_CHANNELS ) ? \
R_SCI_ReceiveAll( (a), (b), (c), (d), (e), (f) ): \
ReturnFalse() \
)

#define R_SCI_Control(a, b) \
( \
( (a) < SCI_CHANNELS ) ? \
R_SCI_ControlAll( (a), (b) ): \
ReturnFalse() \
)

#define R_SCI_GetStatus(a, b, c, d, e) \
( \
( (a) < SCI_CHANNELS ) ? \
R_SCI_GetStatusAll( (a), (b), (c), (d), (e) ): \
ReturnFalse() \
)

#endif
/* End of file */
