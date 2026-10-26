/* Adapted for use with IAR Embedded Workbench */
/******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Electronics Corp. and is
* only intended for use with Renesas products.  

* No other uses are authorized.

* This software is owned by Renesas Electronics Corp. and is 
* protected under the applicable laws, including copyright laws.

* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY,
* INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH
* WARRANTIES ARE EXPRESSLY DISCLAIMED.

* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER 
* RENESAS ELECTRONICS CORP. NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR AND DIRECT, INDIRECT, SPECIAL, INCIDENTAL
* OR COSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE,
* EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE
* POSSIBILITIES OF SUCH DAMAGES.

* Renesas reserves the right, without notice, to make changes to this
* software and to discontinue availability of this software.
* By using this software, you agree to the additional terms and 
* conditions found by accessing the following link:
* http://www.renesas.com/disclaimer
*******************************************************************************/
/* Copyright (C) 2010. Renesas Electronics Corp., All Rights Reserved         */
/******************************************************************************
* File Name     : i2cprom.h
* Version       : 1.0
* Device(s)     : RPBRX62N
* Tool-Chain    : RX Family C Compiler
* OS            : None
* H/W Platform  : RX62N
* Description   : IIC EEPROM read/write
* Operation     :
* Limitations   : None
*******************************************************************************
* History : DD.MM.YYYY  Version Description
*         : 06.10.2010  1.00    First release
*******************************************************************************/

#ifndef __I2CPROM_H__
#define __I2CPROM_H__

#define IIC_ARBITRATION_LOST    10
#define IIC_NO_ACKNOWLEDGE      11
#define IIC_RESET_FAILED      12

int i2c_eeprom_write(unsigned char address, unsigned char *data, int n);
int i2c_eeprom_read (unsigned char address, unsigned char *data, int n);
int i2c_eeprom_reset(void);

#endif /* __I2CPROM_H__ */
