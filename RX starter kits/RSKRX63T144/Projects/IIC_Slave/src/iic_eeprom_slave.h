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
/* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.   */
/*******************************************************************************
* File Name     : iic_eeprom_slave.h
* Version       : 1.00
* Device        : R5F563TE
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX63T144
* Description   : Provides declarations for functions defined in the iic_eeprom_
*                 slave.c file. The functions defined operate ton the following 
*                 Renesas IIC EEPROM devices:
*                 R1EX24xxx series, 16Kbit EEPROM, 400KHz
*******************************************************************************/
/*******************************************************************************
* History       : 05.12.2013  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* Macro Definitions
*******************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef IIC_EEPROM_H
#define IIC_EEPROM_H

/* Defines the address the EEPROM device responds to on the IIC bus */
#define SIM_EEPROM_DEVICE_ADDRESS   0xA0

/* Defines the location the simulated memory will be stored on the MCU */
#define SIM_EEPROM_LOCATION         0x00001f00

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/
/* Defines standard variable types used in this file */
#include <stdint.h>

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Declare the simulated EEPROM data table */
extern uint8_t g_sim_eeprom_memory[256];

/******************************************************************************
* Global Function Prototypes
******************************************************************************/
/* IIC slave mode initialisation function prototype declaration */
void Init_EEPROM_Slave(void);

/* Slave sequence function prototype declaration */
void SlaveSequence_EEPROM(void);


/* End of multiple inclusion prevention macro */
#endif


