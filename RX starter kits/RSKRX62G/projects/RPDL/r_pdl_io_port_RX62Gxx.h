/* Adapted for use with IAR Embedded Workbench */
/*""FILE COMMENT""*************************************************************
* System Name  : I/O Port API for RX62G and RX62T
* File Name    : r_pdl_io_port_RX62Gxx.h
* Version    : 1.00
* Contents    : I/O Port API header
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

#ifndef R_PDL_IO_PORT_RX62G_H
#define R_PDL_IO_PORT_RX62G_H

/* Library prototypes */
bool R_IO_PORT_SetAll(
  const uint8_t,
  const uint8_t,
  const uint8_t,
  const uint8_t
);
bool R_IO_PORT_ReadControlAll(
  const uint8_t,
  const uint8_t,
  const uint8_t,
  volatile uint8_t * const
);
bool R_IO_PORT_ReadControlRegisterAll(
  const uint8_t,
  const uint8_t,
  volatile uint8_t * const
);
bool R_IO_PORT_ModifyControlAll(
  const uint8_t,
  const uint8_t,
  const uint8_t,
  const uint8_t,
  const uint8_t
);
bool R_IO_PORT_ModifyControlRegisterAll(
  const uint8_t,
  const uint8_t,
  const uint8_t
);
bool R_IO_PORT_ReadDataAll(
  const uint8_t,
  const uint8_t,
  volatile uint8_t * const
);
bool R_IO_PORT_ReadDataRegisterAll(
  const uint8_t,
  volatile uint8_t * const
);
bool R_IO_PORT_WriteAll(
  const uint8_t,
  const uint8_t,
  const uint8_t,
  const uint8_t
);
bool R_IO_PORT_WriteRegisterAll(
  const uint8_t,
  const uint8_t
);
bool R_IO_PORT_CompareAll(
  const uint8_t,
  const uint8_t,
  const uint8_t,
  VoidCallBackFunc const
);
bool R_IO_PORT_CompareRegisterAll(
  const uint8_t,
  const uint8_t,
  VoidCallBackFunc const
);
bool R_IO_PORT_ModifyDataAll(
  const uint8_t,
  const uint8_t,
  const uint8_t,
  const uint8_t,
  const uint8_t
);
bool R_IO_PORT_ModifyDataRegisterAll(
  const uint8_t,
  const uint8_t,
  const uint8_t
);
bool R_IO_PORT_WaitAll(
  const uint8_t,
  const uint8_t,
  const uint8_t
);
bool R_IO_PORT_WaitRegisterAll(
  const uint8_t,
  const uint8_t
);
bool ReturnFalse(void);

/* I/O Ports that are not available on smaller packages */
#ifdef DEVICE_PACKAGE_100_PIN
 #undef PDL_IO_PORT_G
#endif

#ifdef DEVICE_PACKAGE_80_PIN
 #undef PDL_IO_PORT_5
 #undef PDL_IO_PORT_8
 #undef PDL_IO_PORT_G
#endif

#ifdef DEVICE_PACKAGE_64_PIN
 #undef PDL_IO_PORT_5
 #undef PDL_IO_PORT_6
 #undef PDL_IO_PORT_8
 #undef PDL_IO_PORT_G
#endif

/* I/O port pins that are not available on smaller packages */

#ifdef DEVICE_PACKAGE_100_PIN
 #undef PDL_IO_PORT_G_0
 #undef PDL_IO_PORT_G_1
 #undef PDL_IO_PORT_G_2
 #undef PDL_IO_PORT_G_3
 #undef PDL_IO_PORT_G_4
 #undef PDL_IO_PORT_G_5
#endif

#ifdef DEVICE_PACKAGE_80_PIN
 #undef PDL_IO_PORT_5_0
 #undef PDL_IO_PORT_5_1
 #undef PDL_IO_PORT_5_2
 #undef PDL_IO_PORT_5_3
 #undef PDL_IO_PORT_5_4
 #undef PDL_IO_PORT_5_5
 #undef PDL_IO_PORT_6_4
 #undef PDL_IO_PORT_6_5
 #undef PDL_IO_PORT_8_0
 #undef PDL_IO_PORT_8_1
 #undef PDL_IO_PORT_8_2
 #undef PDL_IO_PORT_9_0
 #undef PDL_IO_PORT_A_0
 #undef PDL_IO_PORT_A_1
 #undef PDL_IO_PORT_D_0
 #undef PDL_IO_PORT_D_1
 #undef PDL_IO_PORT_D_2
 #undef PDL_IO_PORT_E_1
 #undef PDL_IO_PORT_E_5
 #undef PDL_IO_PORT_G_0
 #undef PDL_IO_PORT_G_1
 #undef PDL_IO_PORT_G_2
 #undef PDL_IO_PORT_G_3
 #undef PDL_IO_PORT_G_4
 #undef PDL_IO_PORT_G_5
#endif

#ifdef DEVICE_PACKAGE_64_PIN
 #undef PDL_IO_PORT_2_0
 #undef PDL_IO_PORT_2_1
 #undef PDL_IO_PORT_5_0
 #undef PDL_IO_PORT_5_1
 #undef PDL_IO_PORT_5_2
 #undef PDL_IO_PORT_5_3
 #undef PDL_IO_PORT_5_4
 #undef PDL_IO_PORT_5_5
 #undef PDL_IO_PORT_6_0
 #undef PDL_IO_PORT_6_1
 #undef PDL_IO_PORT_6_2
 #undef PDL_IO_PORT_6_3
 #undef PDL_IO_PORT_6_4
 #undef PDL_IO_PORT_6_5
 #undef PDL_IO_PORT_8_0
 #undef PDL_IO_PORT_8_1
 #undef PDL_IO_PORT_8_2
 #undef PDL_IO_PORT_9_0
 #undef PDL_IO_PORT_9_5
 #undef PDL_IO_PORT_9_6
 #undef PDL_IO_PORT_A_0
 #undef PDL_IO_PORT_A_1
 #undef PDL_IO_PORT_D_0
 #undef PDL_IO_PORT_D_1
 #undef PDL_IO_PORT_D_2
 #undef PDL_IO_PORT_E_0
 #undef PDL_IO_PORT_E_1
 #undef PDL_IO_PORT_E_3
 #undef PDL_IO_PORT_E_4
 #undef PDL_IO_PORT_E_5
 #undef PDL_IO_PORT_G_0
 #undef PDL_IO_PORT_G_1
 #undef PDL_IO_PORT_G_2
 #undef PDL_IO_PORT_G_3
 #undef PDL_IO_PORT_G_4
 #undef PDL_IO_PORT_G_5
#endif

/* Limits used by the macro definitions */
#ifdef DEVICE_PACKAGE_112_PIN
 #define LOWER_PORT_LIMIT  PDL_IO_PORT_1
 #define LOWER_PIN_LIMIT  PDL_IO_PORT_1_0
 #define UPPER_PORT_LIMIT  PDL_IO_PORT_G
 #define UPPER_PIN_LIMIT   (PDL_IO_PORT_G_0 | PDL_IO_PORT_G_1 | PDL_IO_PORT_G_2 | PDL_IO_PORT_G_3 | PDL_IO_PORT_G_4 | PDL_IO_PORT_G_5)
#endif

#ifdef DEVICE_PACKAGE_100_PIN
 #define LOWER_PORT_LIMIT  PDL_IO_PORT_1
 #define LOWER_PIN_LIMIT  PDL_IO_PORT_1_0
 #define UPPER_PORT_LIMIT  PDL_IO_PORT_E
 #define UPPER_PIN_LIMIT   (PDL_IO_PORT_E_0 | PDL_IO_PORT_E_1 | PDL_IO_PORT_E_2 | PDL_IO_PORT_E_3 | PDL_IO_PORT_E_4 | PDL_IO_PORT_E_5)
#endif

#ifdef DEVICE_PACKAGE_80_PIN
 #define LOWER_PORT_LIMIT  PDL_IO_PORT_1
 #define LOWER_PIN_LIMIT  PDL_IO_PORT_1_0
 #define UPPER_PORT_LIMIT  PDL_IO_PORT_E
 #define UPPER_PIN_LIMIT   (PDL_IO_PORT_E_0 | PDL_IO_PORT_E_2 | PDL_IO_PORT_E_3 | PDL_IO_PORT_E_4)
#endif

#ifdef DEVICE_PACKAGE_64_PIN
 #define LOWER_PORT_LIMIT  PDL_IO_PORT_1
 #define LOWER_PIN_LIMIT  PDL_IO_PORT_1_0
 #define UPPER_PORT_LIMIT  PDL_IO_PORT_E
 #define UPPER_PIN_LIMIT   PDL_IO_PORT_E_2
#endif

/* Macro definitions */

/* R_IO_PORT_Set */
/* Call the function if the port number is valid. */
#define R_IO_PORT_Set(a, b) \
( \
( ((a) >= LOWER_PIN_LIMIT) && ((a) <= UPPER_PIN_LIMIT) ) ? \
R_IO_PORT_SetAll( (uint8_t)((a) >> 8), (uint8_t)(a), (uint8_t)~(a), (b) ) : \
ReturnFalse() \
)

/* R_IO_PORT_ReadControl */
/* Call the function if the port number is valid. */
#define R_IO_PORT_ReadControl(a, b, c) \
( \
( (((a) >= LOWER_PIN_LIMIT) && ((a) <= UPPER_PIN_LIMIT)) ) ? \
R_IO_PORT_ReadControlAll( (uint8_t)((a) >> 8), (uint8_t)(a), (b), (c) ) : \
( ((a) >= LOWER_PORT_LIMIT) && ((a) <= UPPER_PORT_LIMIT) ) ? \
R_IO_PORT_ReadControlRegisterAll( (uint8_t)(a), (b), (c) ) : \
ReturnFalse() \
)

/* R_IO_PORT_ModifyControl */
/* Call the function if the port number is valid. */
#define R_IO_PORT_ModifyControl(a, b, c) \
( \
( (((a) >= LOWER_PIN_LIMIT) && ((a) <= UPPER_PIN_LIMIT)) ) ? \
R_IO_PORT_ModifyControlAll( (uint8_t)((a) >> 8), (uint8_t)(a), (uint8_t)~(a), (b), (c) ) : \
( ((a) >= LOWER_PORT_LIMIT) && ((a) <= UPPER_PORT_LIMIT) ) ? \
R_IO_PORT_ModifyControlRegisterAll( (uint8_t)(a), (b), (c) ) : \
ReturnFalse() \
)

/* R_IO_PORT_Read */
/* Call the function if the port number is valid. */
#define R_IO_PORT_Read(a, b) \
( \
( (((a) >= LOWER_PIN_LIMIT) && ((a) <= UPPER_PIN_LIMIT)) ) ? \
R_IO_PORT_ReadDataAll( (uint8_t)((a) >> 8), (uint8_t)(a), (b) ) : \
( ((a) >= LOWER_PORT_LIMIT) && ((a) <= UPPER_PORT_LIMIT) ) ? \
R_IO_PORT_ReadDataRegisterAll( (uint8_t)(a), (b) ) : \
ReturnFalse() \
)

/* R_IO_PORT_Write */
/* Call the function if the port number is valid. */
#define R_IO_PORT_Write(a, b) \
( \
( (((a) >= LOWER_PIN_LIMIT) && ((a) <= UPPER_PIN_LIMIT)) ) ? \
R_IO_PORT_WriteAll( (uint8_t)((a) >> 8), (uint8_t)(a), (uint8_t)~(a), (b) ) : \
( ((a) >= LOWER_PORT_LIMIT) && ((a) <= UPPER_PORT_LIMIT) ) ? \
R_IO_PORT_WriteRegisterAll( (uint8_t)(a), (b) ): \
ReturnFalse() \
)

/* R_IO_PORT_Compare */
/* Call the function if the port number is valid. */
#define R_IO_PORT_Compare(a, b, c) \
( \
( (((a) >= LOWER_PIN_LIMIT) && ((a) <= UPPER_PIN_LIMIT)) ) ? \
R_IO_PORT_CompareAll( (uint8_t)((a) >> 8), (uint8_t)(a), (b), (c) ) : \
( ((a) >= LOWER_PORT_LIMIT) && ((a) <= UPPER_PORT_LIMIT) ) ? \
R_IO_PORT_CompareRegisterAll( (uint8_t)(a), (b), (c) ): \
ReturnFalse() \
)

/* R_IO_PORT_Modify */
/* Call the function if the port number is valid. */
#define R_IO_PORT_Modify(a, b, c) \
( \
( (((a) >= LOWER_PIN_LIMIT) && ((a) <= UPPER_PIN_LIMIT)) ) ? \
R_IO_PORT_ModifyDataAll( (uint8_t)((a) >> 8), (uint8_t)(a), (uint8_t)~(a), (b), (c) ) : \
( ((a) >= LOWER_PORT_LIMIT) && ((a) <= UPPER_PORT_LIMIT) ) ? \
R_IO_PORT_ModifyDataRegisterAll( (uint8_t)(a), (b), (c) ): \
ReturnFalse() \
)

/* R_IO_PORT_Wait */
/* Call the function if the port number is valid. */
#define R_IO_PORT_Wait(a, b) \
( \
( (((a) >= LOWER_PIN_LIMIT) && ((a) <= UPPER_PIN_LIMIT)) && (b == 1u) ) ? \
R_IO_PORT_WaitAll( (uint8_t)((a) >> 8), (uint8_t)(a), (uint8_t)(a) ) : \
( (((a) >= LOWER_PIN_LIMIT) && ((a) <= UPPER_PIN_LIMIT)) && (b == 0u) ) ? \
R_IO_PORT_WaitAll( (uint8_t)((a) >> 8), (uint8_t)(a), 0u ) : \
( ((a) >= LOWER_PORT_LIMIT) && ((a) <= UPPER_PORT_LIMIT) ) ? \
R_IO_PORT_WaitRegisterAll( (uint8_t)(a), (b) ) : \
ReturnFalse() \
)

#endif
/* End of file */
