/*""FILE COMMENT""*******************************************************
* System Name	: BSC API for RX610xx
* File Name		: r_pdl_bsc_RX610xx.h
* Version		: 0.50
* Contents		: BSC API header
* Customer		: 
* Model			: 
* Order			: 
* CPU			: RX600
* Compiler		: RXC
* OS			: Nothing
* Programmer	: 
* Note			: 
************************************************************************
* Copyright,2010 RENESAS ELECTRONICS CORPORATION
* AND RENESAS SOLUTIONS CORPORATION
************************************************************************
* History		: 2009.11.13
*				: Ver 0.10
*				: 2010.01.22
*				: Ver 0.11
*				: Added protection against multiple inclusion.
*				: 2010.03.22
*				: Ver 0.12
*				: Re-named to support multiple device packages.
*				: 2010.04.06
*				: Ver 0.50
*				: ES-level release
*""FILE COMMENT END""**************************************************/

#ifndef R_PDL_BSC_RX610xx_H
#define R_PDL_BSC_RX610xx_H

#define BSC_AREAS	8

/* Callback function storage */
extern VoidCallBackFunc rpdl_BSC_callback_func;

/* Library prototypes */
bool R_BSC_CreateAll(
	const uint32_t,
	const uint32_t,
	const uint8_t,
	VoidCallBackFunc const,
	const uint8_t
);
bool R_BSC_CreateAreaAll(
	const uint8_t,
	const uint16_t,
	const uint8_t,
	const uint8_t,
	const uint8_t,
	const uint8_t,
	const uint8_t,
	const uint8_t,
	const uint8_t,
	const uint8_t,
	const uint8_t,
	const uint8_t,
	const uint8_t,
	const uint8_t,
	const uint8_t
);
bool R_BSC_DestroyAll(
	const uint8_t
);
bool R_BSC_ControlAll(
	const uint8_t
);
bool R_BSC_GetStatusAll(
	volatile uint8_t * const
);
bool ReturnFalse(void);

/* Macro definitions */

#define R_BSC_Create(a, b, c, d, e) \
( \
( (e) <= IPL_MAX ) ? \
R_BSC_CreateAll( (a), (b), (c), (d), (e) ) : \
ReturnFalse() \
)

#define R_BSC_CreateArea(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o) \
( \
( (a) < BSC_AREAS ) ? \
R_BSC_CreateAreaAll( (a), (b), (c), (d), (e), (f), (g), (h), (i), (j), (k), (l), (m), (n), (o) ) : \
ReturnFalse() \
)

#define R_BSC_Destroy(a) \
( \
( (a) < BSC_AREAS ) ? \
R_BSC_DestroyAll( (a) ) : \
ReturnFalse() \
)

#define R_BSC_Control(a) \
( \
R_BSC_ControlAll( (a) ) \
)

#define R_BSC_GetStatus(a) \
( \
R_BSC_GetStatusAll( (a) ) \
)

#endif
/* End of file */