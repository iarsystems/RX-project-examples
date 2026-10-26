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
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.    */ 
/*******************************************************************************
* File Name    : iic_eeprom_slave.h
* Version      : 1.00
* Device       : R5F562GAADFP
* Tool Chain   : RX Family C Compiler 1.2.1.0
* H/W Platform : RSKRX62G
* Description  : Provides delarations for functions defined in the iic_eeprom_
*                slave.c file. The functions defined operate ton the following 
*                Renesas IIC EEPROM devices:
*                R1EX24xxx series, 16Kbit EEPROM, 400KHz
*******************************************************************************/
/*******************************************************************************
* History       : 06.09.2012 Ver. 1.00 First Release
*******************************************************************************/

/******************************************************************************
* Project Includes
******************************************************************************/
/* Defines standard variable types used in this file */
#include <stdint.h>

/*******************************************************************************
* Macro Definitions
*******************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef IIC_EEPROM_H
#define IIC_EEPROM_H

/* Defines the IIC channel used for reading and writing to the EEPROM device */
#define IIC_SLAVE_CHANNEL               0
/* Defines the data rate (in bits per second) used to communicate to EEPROM
   Valid between 25E3 and 750E3 */
#define IIC_SLAVE_TRANSFER_DATA_RATE    100E3
/* Defines the CMT channel used for oneshot timer delays */
#define CMT_CHANNEL                     2u
/* Defines the address the EEPROM device responds to on the IIC bus */
#define SIM_EEPROM_DEVICE_ADDRESS       0xA0
/* Defines the maximum time required for an EEPROM write, in seconds */
#define SIM_EEPROM_WRITE_WAIT           5E-3
/* Defines the maximum number of writes the EEPROM supports per page write */
#define SIM_EEPROM_MAX_WRITE            16u     
/* Defines the maximum address range of the EEPROM device memory */
#define SIM_EEPROM_MAX_ADDRESS          0x800
/* Defines the maximium EEPROM rise time of the SCL line, in nanoseconds */
#define SIM_EEPROM_SCL_RISE_TIME        300
/* Defines the maximum EEPROM fall time of the SCL line, in nanoseconds */
#define SIM_EEPROM_SCL_FALL_TIME        200
/* Defines the location the simulated memory will be stored on the MCU */
#define SIM_EEPROM_LOCATION             0x00003000
/* Define slave idle status flag value */
#define IIC_SLAVE_IDLE                  0x00
/* Define slave read operation status flag value */
#define IIC_SLAVE_EEPROM_READ           0x31
/* Define slave write operation status flag value */
#define IIC_SLAVE_EEPROM_WRITE          0x32
/* Define slave read fail status flag value */
#define IIC_SLAVE_FAILURE               0xF1
/* Define slave write memory overflow fail status flag value */
#define IIC_SLAVE_EEPROM_WRITE_OVERFLOW 0xF2


/******************************************************************************
* Global Variables
******************************************************************************/
/* IIC slave status flag variable */
extern uint8_t gIIC_Slave_Status;

/******************************************************************************
* Global Function Prototypes
******************************************************************************/
/* IIC slave mode intialisation function prototype declaration */
void Init_EEPROM_Slave(void);
/* IIC slave bus monitor function prototype declaration */
void BusMonitor_EEPROM_Slave(void);
/* Write request handler function prototype declaration */
void Write_EEPROM_Slave(void);
/* Read request handler function prototype declaration */
void Read_EEPROM_Slave(void);


/* End of multiple inclusion prevention macro */
#endif
