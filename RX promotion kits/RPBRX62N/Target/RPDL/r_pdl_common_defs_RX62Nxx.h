/*""FILE COMMENT""*******************************************************
* System Name	: API for RX62Nxx
* File Name		: r_pdl_common_defs_RX62Nxx.h
* Version		: 0.10
* Contents		: API common definitions
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
* History		: 2010.05.10
*				: Ver 0.10
*				: First draft
*				: 2010.07.15
*				: Ver 0.11
*				: Changed to the RPDL-released iodefine.h file
*""FILE COMMENT END""**************************************************/

#ifndef R_PDL_COMMON_DEFS_RX62Nxx_H
#define R_PDL_COMMON_DEFS_RX62Nxx_H

#include <stdint.h>
#include <stdbool.h>
#include <machine.h>
#include <stddef.h>
#include "iodefine_RPDL.h"

/* Callback function type */
typedef void (* VoidCallBackFunc)(void);

/* The supported MCU group */
#define PDL_MCU_GROUP RX62N

/* The highest interrupt priority level */
#define IPL_MAX	15

/* Use the I/O register area to indicate that a callback function pointer is not to be used */
#define PDL_NO_FUNC	(VoidCallBackFunc)0x00080000ul
/* Use the ROM area to indicate that a data pointer is not to be used */
#define PDL_NO_PTR (void *)0xFFFFFFFFul
/* When no parameters options are required */
#define PDL_NO_DATA 0

/* Shared global variables */
extern volatile uint32_t rpdl_CGC_f_pclk;
extern volatile uint32_t rpdl_CGC_f_iclk;
extern volatile uint32_t rpdl_CGC_f_bclk;
extern volatile	 uint8_t rpdl_INTC_brk_command;
extern volatile	 uint8_t rpdl_INTC_brk_data8;
extern volatile uint32_t rpdl_INTC_saved_isp;

/* Shared function */
uint8_t rpdl_DMAC_get_channel(const uint8_t);

/* BRK handler command options */
typedef enum {
	BRK_NO_COMMAND,
	BRK_START_ADC_10,
	BRK_START_ADC_10_AND_SLEEP,
	BRK_SLEEP,
	BRK_ALL_MODULE_CLOCK_STOP,
	BRK_STANDBY,
	BRK_DEEP_STANDBY,
	BRK_LOAD_FINTV_REGISTER,
	BRK_WRITE_IPL
} brk_commands;

/* Bit definitions */
#define  BIT_0 0x00000001ul
#define  BIT_1 0x00000002ul
#define  BIT_2 0x00000004ul
#define  BIT_3 0x00000008ul
#define  BIT_4 0x00000010ul
#define  BIT_5 0x00000020ul
#define  BIT_6 0x00000040ul
#define  BIT_7 0x00000080ul
#define  BIT_8 0x00000100ul
#define  BIT_9 0x00000200ul
#define BIT_10 0x00000400ul
#define BIT_11 0x00000800ul
#define BIT_12 0x00001000ul
#define BIT_13 0x00002000ul
#define BIT_14 0x00004000ul
#define BIT_15 0x00008000ul
#define BIT_16 0x00010000ul
#define BIT_17 0x00020000ul
#define BIT_18 0x00040000ul
#define BIT_19 0x00080000ul
#define BIT_20 0x00100000ul
#define BIT_21 0x00200000ul
#define BIT_22 0x00400000ul
#define BIT_23 0x00800000ul
#define BIT_24 0x01000000ul
#define BIT_25 0x02000000ul
#define BIT_26 0x04000000ul
#define BIT_27 0x08000000ul
#define BIT_28 0x10000000ul
#define BIT_29 0x20000000ul
#define BIT_30 0x40000000ul
#define BIT_31 0x80000000ul

#define INV_BIT_0  0xFFFFFFFEul
#define INV_BIT_1  0xFFFFFFFDul
#define INV_BIT_2  0xFFFFFFFBul
#define INV_BIT_3  0xFFFFFFF7ul
#define INV_BIT_4  0xFFFFFFEFul
#define INV_BIT_5  0xFFFFFFDFul
#define INV_BIT_6  0xFFFFFFBFul
#define INV_BIT_7  0xFFFFFF7Ful
#define INV_BIT_8  0xFFFFFEFFul
#define INV_BIT_9  0xFFFFFDFFul
#define INV_BIT_10 0xFFFFFBFFul
#define INV_BIT_11 0xFFFFF7FFul
#define INV_BIT_12 0xFFFFEFFFul
#define INV_BIT_13 0xFFFFDFFFul
#define INV_BIT_14 0xFFFFBFFFul
#define INV_BIT_15 0xFFFF7FFFul
#define INV_BIT_16 0xFFFEFFFFul
#define INV_BIT_17 0xFFFDFFFFul
#define INV_BIT_18 0xFFFBFFFFul
#define INV_BIT_19 0xFFF7FFFFul
#define INV_BIT_20 0xFFEFFFFFul
#define INV_BIT_21 0xFFDFFFFFul
#define INV_BIT_22 0xFFBFFFFFul
#define INV_BIT_23 0xFF7FFFFFul
#define INV_BIT_24 0xFEFFFFFFul
#define INV_BIT_25 0xFDFFFFFFul
#define INV_BIT_26 0xFBFFFFFFul
#define INV_BIT_27 0xF7FFFFFFul
#define INV_BIT_28 0xEFFFFFFFul
#define INV_BIT_29 0xDFFFFFFFul
#define INV_BIT_30 0xBFFFFFFFul
#define INV_BIT_31 0x7FFFFFFFul

#endif

/* End of file */
