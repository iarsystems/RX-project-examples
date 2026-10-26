/*********************************************************************
*
* Device     : RX/RX200
*
* File Name  : resetprg.c
*
* Abstract   : Reset Program.
*
* History    : 1.00  (2010-12-17)
*            : 1.10  (2011-02-21)
*            : 1.11  (2011-06-20)
*
* NOTE       : THIS IS A TYPICAL EXAMPLE.
*
* Copyright (C) 2010 (2011) Renesas Electronics Corporation.
* and Renesas Solutions Corp.
*
*********************************************************************/

#include	<machine.h>

extern void        usb_cpu_mcu_initialize(void);

#define PSW_init  0x00010000	// PSW bit pattern
#define FPSW_init 0x00000000	// FPSW bit base pattern

int __low_level_init()
{
	/* MCU initialized */
	usb_cpu_mcu_initialize();

    __no_operation();

	set_psw(PSW_init);				// Set Ubit & Ibit for PSW

    return 1;
}

