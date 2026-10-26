/*""FILE COMMENT""*******************************************************
* System Name	: ADC converter API for RX62N
* File Name		: r_pdl_adc_10.h
* Version		: 0.50
* Contents		: ADC function prototypes
* Customer		:
* Model			:
* Order			:
* CPU			:
* Compiler		:
* OS			:
* Programmer	:
* Note			:
************************************************************************
* Copyright,2010 RENESAS ELECTRONICS CORPORATION
* AND RENESAS SOLUTIONS CORPORATION
************************************************************************
* History		: 2009.11.12
*				: Ver 0.10
*				: 2010.01.12
*				: Ver 0.11
*				: Added protection against multiple inclusion.
*				: 2010.01.26
*				: Ver 0.12
*				: Removed const keywords from the function prototypes.
*				: 2010.04.06
*				: Ver 0.50
*				: ES-level release
*""FILE COMMENT END""**************************************************/

#ifndef R_PDL_ADC_10_H
#define R_PDL_ADC_10_H

#include "r_pdl_common_defs_RX62Nxx.h"

/* Function prototypes */
bool R_ADC_10_Create(
	uint8_t,
	uint32_t,
	uint32_t,
	float,
	void *,
	uint8_t
);
bool R_ADC_10_Destroy(
	uint8_t
);
bool R_ADC_10_Control(
	uint16_t
);
bool R_ADC_10_Read(
	uint8_t,
	uint16_t *
);

/* Scan mode */
#define PDL_ADC_10_MODE_SINGLE				0x00000001u
#define PDL_ADC_10_MODE_CONTINUOUS_SCAN		0x00000002u
#define PDL_ADC_10_MODE_ONE_CYCLE_SCAN		0x00000004u

// Channel selection
#define PDL_ADC_10_CHANNELS_OPTION_1		0x00000008u
#define PDL_ADC_10_CHANNELS_OPTION_2		0x00000010u
#define PDL_ADC_10_CHANNELS_OPTION_3		0x00000020u
#define PDL_ADC_10_CHANNELS_OPTION_4		0x00000040u

/* Trigger selection */
#define PDL_ADC_10_TRIGGER_SOFTWARE			0x00000080u
#define PDL_ADC_10_TRIGGER_TPU_UNIT0		0x00000100u
#define PDL_ADC_10_TRIGGER_TPU_UNIT1		0x00000200u
#define PDL_ADC_10_TRIGGER_TMR				0x00000400u
#define PDL_ADC_10_TRIGGER_ADTRG0			0x00000800u
#define PDL_ADC_10_TRIGGER_ADTRG1			0x00001000u
#define PDL_ADC_10_TRIGGER_TPU0_CM			0x00002000u

/* Data alignment */
#define PDL_ADC_10_DATA_ALIGNMENT_LEFT		0x00004000u
#define PDL_ADC_10_DATA_ALIGNMENT_RIGHT		0x00008000u

/* DMAC / DTC trigger control */
#define PDL_ADC_10_DMAC_DTC_TRIGGER_DISABLE	0x00010000u
#define PDL_ADC_10_DMAC_TRIGGER_ENABLE		0x00020000u
#define PDL_ADC_10_DTC_TRIGGER_ENABLE		0x00040000u

/* Sampling time calcuation control */
#define PDL_ADC_10_ADSSTR_CALCULATE			0x00080000u
#define PDL_ADC_10_ADSSTR_SPECIFY			0x00100000u

/* On / off control */
#define PDL_ADC_10_0_ON		0x0001u
#define PDL_ADC_10_0_OFF	0x0002u
#define PDL_ADC_10_1_ON		0x0004u
#define PDL_ADC_10_1_OFF	0x0008u

/* CPU control */
#define PDL_ADC_10_CPU_ON	0x0100u
#define PDL_ADC_10_CPU_OFF	0x0200u

#endif
/* End of file */
