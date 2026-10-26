/*""FILE COMMENT""*******************************************************************
* File Name		: r_board.h
* Version      	: 1.01
* Device(s)    	: RX62N
* Tool-Chain 	: RX Standard Toolchain (V.1.1.0.0)
* H/W Platform 	: RX62N Renesas Starter Kit+
* Description	: MCU setting and Board peripheral I/O Definition header file
*************************************************************************************
* History	: DD.MM.YYYY Version Description
*			: 22.08.2011 Ver1.01
*""FILE COMMENT END""***************************************************************/
#ifndef _R_BOARD_H_
#define _R_BOARD_H_

#include <stdint.h>
#include "iodefine.h"

#define SET_BIT_HIGH	(1)
#define SET_BIT_LOW		(0)
#define SET_BYTE_HIGH	(0xFF)
#define SET_BYTE_LOW	(0x00)
#define ON				(1)
#define OFF				(0)

#define LED0			PORT0.DR.BIT.B2
#define LED1			PORT0.DR.BIT.B3
#define LED2			PORT0.DR.BIT.B5
#define LED3			PORT3.DR.BIT.B4
#define DLED0			PORT0.DDR.BIT.B2
#define DLED1			PORT0.DDR.BIT.B3
#define DLED2			PORT0.DDR.BIT.B5
#define DLED3			PORT3.DDR.BIT.B4
#define ICRLED0			PORT0.ICR.BIT.B2
#define ICRLED1			PORT0.ICR.BIT.B3
#define ICRLED2			PORT0.ICR.BIT.B5
#define ICRLED3			PORT3.ICR.BIT.B4


#define MCU_CLOCK_SET() do{	\
	SYSTEM.SCKCR.LONG = 0x00C30100;	/* [XIN   = 12.0MHz]                       */\
									/* PCLK   = 12.0*4 = 48MHz,                */\
									/* BCLK   = 12.0*1 = 12MHz(Output Disable) */\
									/* SYSCLK = 12.0*8 = 96MHz                 */\
}while(0)

#define LED_INIT()	do{\
	ICRLED0  = SET_BIT_HIGH;\
	LED0 	 = SET_BIT_HIGH;\
	DLED0	 = SET_BIT_HIGH;\
	ICRLED1  = SET_BIT_HIGH;\
	LED1 	 = SET_BIT_HIGH;\
	DLED1	 = SET_BIT_HIGH;\
	ICRLED2  = SET_BIT_HIGH;\
	LED2 	 = SET_BIT_HIGH;\
	DLED2	 = SET_BIT_HIGH;\
	ICRLED3  = SET_BIT_HIGH;\
	LED3 	 = SET_BIT_HIGH;\
	DLED3	 = SET_BIT_HIGH;\
}while(0)

#define	LED0_ON()	(LED0  = SET_BIT_LOW)
#define	LED0_OFF()	(LED0  = SET_BIT_HIGH)
#define	LED1_ON()	(LED1  = SET_BIT_LOW)
#define	LED1_OFF()	(LED1  = SET_BIT_HIGH)
#define	LED2_ON()	(LED2  = SET_BIT_LOW)
#define	LED2_OFF()	(LED2  = SET_BIT_HIGH)
#define	LED3_ON()	(LED3  = SET_BIT_LOW)
#define	LED3_OFF()	(LED3  = SET_BIT_HIGH)

#endif	/*	#ifndef _R_BOARD_H_	*/
