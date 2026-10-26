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
/* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.   */
/*******************************************************************************
* File Name     : iic_eeprom_master.h
* Version       : 1.00
* Device        : R5F52206
* Tool-Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX220
* Description   : Defines IIC functions used in this sample. The functions
*                 defined are to operate the IIC EEPROM with 8 bit memory 
*                 address format.
*******************************************************************************/
/*******************************************************************************
* History       : 14.12.2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
System Includes
*******************************************************************************/

/* Following header file provides standard integer type definitions. */
#include <stdint.h>

/* Following header file provides string type definitions. */
#include <string.h>

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/

/* Switch handler function definitions */
#include "switch.h"


/*******************************************************************************
* Macro Definitions
*******************************************************************************/

/* Multiple inclusion prevention macro */
#ifndef IIC_EEPROM_MASTER_H
#define IIC_EEPROM_MASTER_H

/* Defines the address the EEPROM device responds to on the IIC bus */
#define EEPROM_DEVICE_ADDRESS 0xA0u

/* Defines the IIC SCL high and low level period for 100kbps transfer with 50%
   duty cycle. Calculated from HW Manual section 28.2.14 Transfer Rate equation,
   using tr = 1.16us, tf = 130ns */
#define IICBUSRATE            0x56u

/******************************************************************************
* Global Function Prototypes
******************************************************************************/

/* IIC master mode initialisation function prototype declaration */
void init_eeprom_master (void);

/* EEPROM write function prototype declaration */
bool write_eeprom_master (const uint8_t address, uint8_t * write_data, 
                                                          const uint8_t bytes);

/* EEPROM read function prototype declaration */
bool read_eeprom_master (const uint8_t address, const uint8_t num_bytes);

/* End of multiple inclusion prevention macro */
#endif

