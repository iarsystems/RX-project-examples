/*""FILE COMMENT""*******************************************************************
* File Name		: r_board.h
* Version      	: 1.01
* Device(s)    	: RX210
* Tool-Chain 	: RX Standard Toolchain (V.1.1.0.0)
* H/W Platform 	: RX210 Renesas Starter Kit
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

#define LED0			PORT1.PODR.BIT.B4
#define LED1			PORT1.PODR.BIT.B5
#define LED2			PORT1.PODR.BIT.B6
#define LED3			PORT1.PODR.BIT.B7
#define DLED0			PORT1.PDR.BIT.B4
#define DLED1			PORT1.PDR.BIT.B5
#define DLED2			PORT1.PDR.BIT.B6
#define DLED3			PORT1.PDR.BIT.B7


#define MCU_CLOCK_SET() do{	\
	uint32_t volatile work;																	\
	SYSTEM.PRCR.WORD = 0xA500;																\
	SYSTEM.PRCR.WORD = 0xA503;																\
	SYSTEM.HOCOCR.BIT.HCSTP = 0;															\
	work = 350;																				\
	while(work--);		/* More than 3072 cycles (HOCOWTCR2 = 0000b(3072cycle))    */		\
	work = 0x10821110;					/* [HOCO  = 50MHz]                         */		\
										/* PCLKD  = 50/1 = 50MHz,                  */		\
										/* PCLKB  = 50/2 = 25MHz,                  */		\
										/* BCLK   = 50/4 = 12.5MHz(Output Disable) */		\
										/* ICLK   = 50/1 = 50MHz                   */		\
										/* FCLK   = 50/2 = 25MHz                   */		\
	SYSTEM.SCKCR.LONG = work;																\
	while(work != SYSTEM.SCKCR.LONG){}															\
	SYSTEM.SCKCR3.WORD = 0x0100;		/* ICLK = HOCO                             */		\
	SYSTEM.HOCOCR2.BYTE = 0x03;			/* HOCO = 50MHz */									\
	SYSTEM.PRCR.WORD = 0xA500;																\
	SYSTEM.PRCR.WORD = 0x0000;																\
}while(0)

#define LED_INIT()	do{\
	LED0 	 = SET_BIT_HIGH;\
	DLED0	 = SET_BIT_HIGH;\
	LED1 	 = SET_BIT_HIGH;\
	DLED1	 = SET_BIT_HIGH;\
	LED2 	 = SET_BIT_HIGH;\
	DLED2	 = SET_BIT_HIGH;\
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
