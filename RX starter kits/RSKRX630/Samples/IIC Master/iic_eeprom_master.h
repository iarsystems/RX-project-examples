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
/* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.   */
/*******************************************************************************
* File Name      : iic_eeprom_master.h
* Version     : 1.00
* Device     : R5F5630E
* Tool-Chain   : Renesas RX Toolchain 1.2.0.0
* H/W Platform  : RSKRX630
* Description    : Defines IIC functions used in this sample. The functions
*          defined are to operate the IIC EEPROM with 8 bit memory 
*          address format.
*******************************************************************************/
/*******************************************************************************
* History     : 23 Jan. 2012  Ver. 1.00 First Release
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
/* Defines RX630 LEDs and switches */
#include "rskrx630def.h"
/* Defines RX630 port registers */
#include "iorx630.h"
/* Switch handler function definitions */
#include "switch.h"


/*******************************************************************************
* Macro Definitions
*******************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef IIC_EEPROM_MASTER_H
#define IIC_EEPROM_MASTER_H

/* Defines the address the EEPROM device responds to on the IIC bus */
#define EEPROM_DEVICE_ADDRESS    0xA0


/******************************************************************************
* Global Function Prototypes
******************************************************************************/
/* IIC master mode initialisation function prototype declaration */
void Init_EEPROM_Master(void);
/* EEPROM write function prototype declaration */
bool Write_EEPROM_Master(uint8_t, uint8_t *, uint8_t);
/* EEPROM read function prototype declaration */
bool Read_EEPROM_Master(uint8_t, uint8_t);

/* End of multiple inclusion prevention macro */
#endif
