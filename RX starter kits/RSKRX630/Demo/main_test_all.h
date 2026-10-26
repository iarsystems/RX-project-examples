/******************************************************************************
 * DISCLAIMER:
 * The software supplied by Renesas Technology Europe Ltd is
 * intended and supplied for use on Renesas Technology products.
 * This software is owned by Renesas Technology Europe, Ltd. Or
 * Renesas Technology Corporation and is protected under applicable
 * copyright laws. All rights are reserved.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS". NO WARRANTIES, WHETHER EXPRESS,
 * IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * APPLY TO THIS SOFTWARE. RENESAS TECHNOLOGY AMERICA, INC. AND
 * AND RENESAS TECHNOLOGY CORPORATION RESERVE THE RIGHT, WITHOUT
 * NOTICE, TO MAKE CHANGES TO THIS SOFTWARE. NEITHER RENESAS
 * TECHNOLOGY AMERICA, INC. NOR RENESAS TECHNOLOGY CORPORATION SHALL,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES FOR ANY REASON WHATSOEVER ARISING OUT OF THE
 * USE OR APPLICATION OF THIS SOFTWARE.
 ******************************************************************************/

/* Copyright (C) 2010. Renesas Technology Corporation, All Rights Reserved */
/* Copyright (C) 2010. Renesas Solutions Corporation, All Rights Reserved */

/*""FILE COMMENT""******************************* Technical reference data ****
 * File Name		: sample_led.h
 * Version 		: 1.00
 * Device 		: RX610
 * Device 		: RX610
 * Tool Chain 	: RXC
 * H/W Platform	: RSKRX610 WS version
 * Description   : main function header file
 ******************************************************************************/

/******************************************************************************
 * History 		: 28.01.2010 Ver. 1.00 First Release
 *""FILE COMMENT END""*********************************************************/

#ifndef _SAMPLE_H
#define _SAMPLE_H

/******************************************************************************
User Includes (Project level includes)
******************************************************************************/
/* for RX62n SFR definition file */
//#include "RX62Niodefine.h"				

/******************************************************************************
Definition Functions
******************************************************************************/
void main(void);
#ifdef __cplusplus
extern "C" {
  void abort(void);
}
#endif

void hwsetup(void);
void AD_test_func(void);
void AD_init(void);
void LED_test_func(void);
unsigned char led_fnc(void);
void SW_test_func(void);
int Check_SW_Input(void);
int SW_Input(void);
void notify_LED_time(void);

/***********************************************************************/
/*  Definition of constant value                                       */
/***********************************************************************/
/* TIMER define */
/* Time of LED lighting50msec = 50000 / (45MHz / 64) */
#define TGRA0_VALUE		39062			

/* LED define */
/* LED lights.  */
#define LED_ON		0					
/* LED is turned off. */
#define LED_OFF		1					

/* Port data of LED0 (PC5) */
#define led_0_port	PORTC.PODR.BIT.B5		
/* Port data of LED1 (P24) */
#define led_1_port	PORT2.PODR.BIT.B4		
/* Port data of LED2 (PC2) */
#define led_2_port	PORTC.PODR.BIT.B2		
/* Port data of LED3 (P17) */
#define led_3_port	PORT1.PODR.BIT.B7		

/* Port direction of LED0(PC5) */
#define led_0_port_dir	PORTC.PDR.BIT.B5	
/* Port direction of LED1(P24) */
#define led_1_port_dir	PORT2.PDR.BIT.B4	
/* Port direction of LED2(PC2) */
#define led_2_port_dir	PORTC.PDR.BIT.B2	
/* Port direction of LED3(P17) */
#define led_3_port_dir	PORT1.PDR.BIT.B7	
	

#endif
