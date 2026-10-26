/* Adapted for use with IAR Embedded Workbench */
/******************************************************************************
* DISCLAIMER:
* The software supplied by Renesas Technology Europe Ltd is
* intended and supplied for use on Renesas Technology products.
* This software is owned by Renesas Technology Europe, Ltd. Or
* Renesas Technology Corporation and is protected under applicable
* copyright laws. All rights are reserved.
*
* THIS SOFTWARE IS PROVIDED "AS IS". NO WARRANTIES, WHETHER EXPRESS,
* IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* APPLY TO THIS SOFTWARE. RENESAS TECHNOLOGY AMERICA, INC. AND
* AND RENESAS TECHNOLOGY CORPORATION RESERVE THE RIGHT, WITHOUT
* NOTICE, TO MAKE CHANGES TO THIS SOFTWARE. NEITHER RENESAS
* TECHNOLOGY AMERICA, INC. NOR RENESAS TECHNOLOGY CORPORATION SHALL,
* IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
* CONSEQUENTIAL DAMAGES FOR ANY REASON WHATSOEVER ARISING OUT OF THE
* USE OR APPLICATION OF THIS SOFTWARE.
******************************************************************************/
/* Copyright (C) 2010. Renesas Technology Europe, All Rights Reserved        */
/******************************************************************************
* File Name    : hwsetup.h
* Version     : 1.00
* Device     : R5F562N8
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSK+RX62N
* Description   : Hardware setup header file    
*******************************************************************************/
/*******************************************************************************
* History     : 24.06.2010 Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* Macro Defintions
*******************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef HWSETUP_H
#define HWSETUP_H

/******************************************************************************
Global Function Prototypes
******************************************************************************/
/* MCU clock configuration function declaration */
void ConfigureOperatingFrequency(void);
/* MCU I/O port configuration function delcaration */
void ConfigureOutputPorts(void);
/* Interrupt configuration function delcaration */
void ConfigureInterrupts(void);
/* MCU peripheral module configuration function declaration */
void EnablePeripheralModules(void);
/* Hardware setup funtion declaration */
void HardwareSetup(void);

/* End of multiple inclusion prevention macro */
#endif
