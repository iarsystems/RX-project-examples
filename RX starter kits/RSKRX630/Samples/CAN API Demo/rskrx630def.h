/* Adapted for use with IAR Embedded Workbench */
/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. This 
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*******************************************************************************/
/*******************************************************************************
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.    */ 
/*******************************************************************************
* File Name    : rskrx630def.h
* Version     : 1.00
* Device     : 
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX630
* Description   : Defines for RSKRX630 
*******************************************************************************/
/*******************************************************************************
* History     : June 07, 2011 Ver. 1.00 First Release
*******************************************************************************/

#ifndef RSKRX630DEF_H_INCLUDED
#define RSKRX630DEF_H_INCLUDED


/*******************************************************************************
* System Includes
*******************************************************************************/
#include <machine.h>
//#include <builtin.h>

/*******************************************************************************
Defines
*******************************************************************************/
/* Peripheral Clock Speed */
#define  f1_CLK_SPEED  (48000000)
#define XIN_FREQ     (12000000)

/* Switches */
#define  SW1       PORT3.PIDR.BIT.B2
#define SW2       PORT4.PIDR.BIT.B4
#define SW3       PORT0.PIDR.BIT.B7

#define SW1_DDR      PORT3.PDR.BIT.B2
#define SW2_DDR      PORT4.PDR.BIT.B4
#define SW3_DDR      PORT0.PDR.BIT.B7

/* LEDs */
#define  LED0      PORTC.PODR.BIT.B5
#define  LED1      PORT2.PODR.BIT.B4
#define  LED2      PORTC.PODR.BIT.B2
#define  LED3      PORT1.PODR.BIT.B7

#define  LED0_DDR    PORTC.PDR.BIT.B5
#define  LED1_DDR    PORT2.PDR.BIT.B4
#define  LED2_DDR    PORTC.PDR.BIT.B2
#define  LED3_DDR    PORT1.PDR.BIT.B7

#define LED_ON        0
#define LED_OFF       1

#define ALL_LEDS_ON    {LED0 = LED1 = LED2 = LED3 = LED_ON;}
#define ALL_LEDS_OFF  {LED0 = LED1 = LED2 = LED3 = LED_OFF;}

/* Common Defines */
/*
#ifndef TRUE
#define TRUE        1
#endif
#ifndef FALSE
#define FALSE       0
#endif
*/

/* #define ENABLE_IRQ     { SETPSW I } */
/* #define ENABLE_IRQ     { uint32_t psw = _builtin_get_psw(); \
             _builtin_set_psw(psw | 0x00010000); \
            } 
            
 Use ", enable", as in 
#pragma interrupt CAN1_TXM0_ISR(vect=VECT_CAN1_TXM0, enable) */
#define DISABLE_IRQ    { CLRPSW I }

#define ENABLE_LEDS    {LED0_DDR = LED1_DDR = LED2_DDR = LED3_DDR = 1;}

#define CPU_NAME "RX630 "

/* RSKRX630DEF_H_INCLUDED */
#endif     
