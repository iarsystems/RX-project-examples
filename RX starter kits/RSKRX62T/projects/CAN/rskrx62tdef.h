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
* File Name    : rskRX62Tdef.h
* Version     : 2.00
* Device     : R5F562TAADFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX62T
* Description   : Defines for RSKRX62T 
*******************************************************************************/
/*******************************************************************************
* History     : 15.10.2010 Ver. 1.00 First Release
              : 21.05.2011 Ver. 2.00 Second Release (Removed RPDL)
*******************************************************************************/

#ifndef RSKRX600DEF_H_INCLUDED
#define RSKRX600DEF_H_INCLUDED


/*******************************************************************************
* System Includes
*******************************************************************************/
#include <machine.h>
//#include <builtin.h>

/*******************************************************************************
Defines
*******************************************************************************/
/* Peripheral Clock Speed */
#define  f1_CLK_SPEED  (50000000)
#define XIN_FREQ     (12500000)

/* Switches */
#define  SW1       PORTE.PORT.BIT.B5
#define SW2       PORTE.PORT.BIT.B4
#define SW3       PORTB.PORT.BIT.B4

#define SW1_DDR      PORTE.DDR.BIT.B5
#define SW2_DDR      PORTE.DDR.BIT.B4
#define SW3_DDR      PORTB.DDR.BIT.B4

/* LEDs */
#define  GRN_LED0      PORT7.DR.BIT.B1
#define  ORG_LED1      PORT7.DR.BIT.B2
#define  RED_LED2      PORT7.DR.BIT.B3
#define  RED_LED3      PORT3.DR.BIT.B3

#define  GRN_LED0_DDR    PORT7.DDR.BIT.B1
#define  ORG_LED1_DDR    PORT7.DDR.BIT.B2
#define  RED_LED2_DDR    PORT7.DDR.BIT.B3
#define  RED_LED3_DDR    PORT3.DDR.BIT.B3

#define LED_ON        0
#define LED_OFF       1

#define ALL_LEDS_ON    {GRN_LED0 = ORG_LED1 = RED_LED2 = RED_LED3 = LED_ON;}
#define ALL_LEDS_OFF  {GRN_LED0 = ORG_LED1 = RED_LED2 = RED_LED3 = LED_OFF;}

/* Common Defines */
#ifndef TRUE
#define TRUE        1
#endif
#ifndef FALSE
#define FALSE       0
#endif

/* #define ENABLE_IRQ     { SETPSW I } */
/* #define ENABLE_IRQ     { uint32_t psw = _builtin_get_psw(); \
             _builtin_set_psw(psw | 0x00010000); \
            } 
            
 Use ", enable", as in 
#pragma interrupt CAN0_TXM0_ISR(vect=VECT_CAN0_TXM0, enable) */
#define DISABLE_IRQ    { CLRPSW I }

#define ENABLE_LEDS    {GRN_LED0_DDR = ORG_LED1_DDR = RED_LED2_DDR = RED_LED3_DDR = 1;}

/* RSKRX600DEF_H_INCLUDED */
#endif     
