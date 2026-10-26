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
* File Name     : iic_eeprom_master.h
* Version       : 1.00
* Device        : R5F5210B
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX210B
* Description   : Defines IIC functions used in this sample. The functions
*                 defined are to operate the IIC EEPROM with 16 bit memory 
*                 address format.
*******************************************************************************/
/*******************************************************************************
* History       : 14.05.2013  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/
/* Defines standard typedefs used in this file */
#include <stdint.h>

/*******************************************************************************
* Macro Definitions
*******************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef IIC_EEPROM_MASTER_H
#define IIC_EEPROM_MASTER_H

/* Defines the IIC channel used for reading and writing to the EEPROM device */
#define IIC_CHANNEL                 0
/* Defines the data rate (in bits per second) used to communicate to EEPROM 
   Note: it has been kept to 100K to ensure compatibility with other similar IIC
   EEPROM devices */
#define IIC_TRANSFER_DATA_RATE      100E3
/* Defines the CMT channel used for one-shot timer delays */
#define CMT_CHANNEL                 2u
/* Defines the address the EEPROM device responds to on the IIC bus */
#define EEPROM_DEVICE_ADDRESS       0xA0
/* Defines the maximum time required for an EEPROM write, in seconds */
#define EEPROM_WRITE_WAIT           5E-3
/* Defines the maximum number of writes the EEPROM supports per page write
   Note: it has been kept to 16 to ensure compatibility with other similar IIC
   EEPROM devices */
#define EEPROM_MAX_WRITE            16u     
/* Defines the maximum address range of the EEPROM device memory
   Note: it has been kept to 0x800 to ensure compatibility with other similar 
   IIC EEPROM devices */
#define EEPROM_MAX_ADDRESS          0x800
/* Defines the maximum EEPROM rise time of the SCL line, in nanoseconds */
#define EEPROM_SCL_RISE_TIME        300
/* Defines the maximum EEPROM fall time of the SCL line, in nanoseconds */
#define EEPROM_SCL_FALL_TIME        200

/******************************************************************************
* Global Function Prototypes
******************************************************************************/
/* IIC master mode initialisation function prototype declaration */
void Init_EEPROM_Master(void);
/* EEPROM write function prototype declaration */
void Write_EEPROM_Master(uint16_t, uint8_t *, uint8_t);
/* EEPROM read function prototype declaration */
void Read_EEPROM_Master(uint16_t, uint8_t *, uint16_t);
/* IIC transaction check status function prototype declaration */
bool CheckStatus_EEPROM_Master(void);

/* End of multiple inclusion prevention macro */
#endif
