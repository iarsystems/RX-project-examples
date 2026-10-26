/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No 
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all 
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM 
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES 
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS 
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of 
* this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer 
*
* Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.    
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : rskrx24udef.h
* Device(s)    : R5F524UEAxFB
* Tool-Chain   : CCRX
* H/W Platform : RSKRX24U
* Description  : Defines macros relating to the RSKRX24U user LEDs and switches
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 30.11.2016 1.00     First Release
***********************************************************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef RSKRX24U_H
#define RSKRX24U_H

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
#define SW1             (PORT1.PIDR.BIT.B0)
#define SW2             (PORT6.PIDR.BIT.B0)
#define SW3             (PORT2.PIDR.BIT.B0)

/* LED port settings */
#define LED0            (PORT2.PODR.BIT.B1)
#define LED1            (PORT2.PODR.BIT.B2)
#define LED2            (PORTC.PODR.BIT.B3)
#define LED3            (PORTC.PODR.BIT.B4)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/

#endif

