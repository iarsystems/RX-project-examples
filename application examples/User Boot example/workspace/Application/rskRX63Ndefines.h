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


/*
 * rskRX63Ndefines.h
 *
 *  Created on: 01 Oct 2011
 *      Author: RJW
 *              Reneses Electronics Europe Ltd
 */
 
#ifndef _RSK_DEFINES_H
#define _RSK_DEFINES_H


#define     OUTPUT      1
#define     INPUT       0

#define     ON          0
#define     OFF         1

#define     NOT_PRESSED 1
#define     PRESSED     0


// LED Port Direction Registers
#define    LED0_PDR  PORT0.PDR.BIT.B3
#define    LED1_PDR  PORT0.PDR.BIT.B5
#define    LED2_PDR  PORT1.PDR.BIT.B0
#define    LED3_PDR  PORT1.PDR.BIT.B1


// LED Port Output Data Registers
#define    LED0    PORT0.PODR.BIT.B3
#define    LED1    PORT0.PODR.BIT.B5
#define    LED2    PORT1.PODR.BIT.B0
#define    LED3    PORT1.PODR.BIT.B1


// Switch Port Input Data Registers
#define     SW1         PORT3.PIDR.BIT.B2
#define     SW2         PORT0.PIDR.BIT.B0
#define     SW3         PORT0.PIDR.BIT.B7


#endif
