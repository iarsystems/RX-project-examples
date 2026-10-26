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
/* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.   */
/*******************************************************************************
* File Name		: main.h
* Version 		: 1.00
* Device 		: R5F52108DFP
* Tool-Chain 	: RX Family C Compiler
* H/W Platform	: RPBRX210
* Description   : Main header file
*******************************************************************************/
/*******************************************************************************
* History 		: 13.08.2011  Ver. 1.00 First Release
*******************************************************************************/
#ifndef _MAIN_H
#define _MAIN_H

/* The main function declaration */
void main(void);
/* The hardware initialisation function declaration */
void Initialise( void );
/* The main welcome menu function declaration */
uint8_t WelcomeMenu( void );
/* The CPU load menu function declaration */
uint8_t CPULoadMenu( void );
/* The RTC menu function declaration */
void RTCMenu( void );
/* The peripheral configuration function declaration */
void ConfigurePeripherals( uint8_t cpu_calc);
/* The power tests configuration function declaration */
void ConfigurePowerTests(uint8_t clock_mode);
/* The LED Flashing function declaration */
void FlashLEDValue( uint8_t value, uint8_t repeat );
/* The Settle delay function declaration */
void Settle( uint16_t loops );

/* End of multiple inclusion prevention macro */
#endif

/* End of file */