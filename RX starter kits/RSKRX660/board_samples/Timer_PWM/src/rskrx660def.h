/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2022 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : rskrx660def.h
* Version      : 1.0
* Device(s)    : R5F56609HDFB
* Tool-Chain   : CCRX
* H/W Platform : RSKRX660
* Description  : Defines macros relating to the RSKRX660 user LEDs and switches
* Creation Date: 2022-01-18
***********************************************************************************************************************/
#ifndef RSKRX660DEF_H
#define RSKRX660DEF_H

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/* General Values */
#define LED_ON          (0)
#define LED_OFF         (1)
#define SET_BIT_HIGH    (1)
#define SET_BIT_LOW     (0)
#define SET_BYTE_HIGH   (0xFF)
#define SET_BYTE_LOW    (0x00)

/* Switches */
#define SW1             (PORT9.PIDR.BIT.B1)
#define SW2             (PORT9.PIDR.BIT.B2)
#define SW3             (PORT0.PIDR.BIT.B7)

/* LED port settings */
#define LED0            (PORT1.PODR.BIT.B7)
#define LED1            (PORTF.PODR.BIT.B5)
#define LED2            (PORT0.PODR.BIT.B4)
#define LED3            (PORT0.PODR.BIT.B6)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/

#endif /* RSKRX660DEF_H */

