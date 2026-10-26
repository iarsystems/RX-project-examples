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
/*******************************************************************************
* Copyright (C) 2010 Renesas Electronics Corporation. All rights reserved.    */ 
/*******************************************************************************
* File Name    : async.h
* Version     : 1.00
* Device         : R5F562N8
* Tool Chain   : IAR Embedded Workbench
* H/W Platform   : RSK+RX62N
* Description   : Defines UART function prototypes    
*******************************************************************************/
/*******************************************************************************
* History     : 22.01.2010 Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* Macro Defintions
*******************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef ASYNC_H
#define ASYNC_H

/*******************************************************************************
Function Prototypes
*******************************************************************************/
/* UART initialisation function declaration */
void UART_Init(void);
/* Text write function declaration */
void Text_Write(void);
/* rx-callback function declaration */
void rx_callback(void);

/* End of multiple inclusion prevention macro */
#endif
