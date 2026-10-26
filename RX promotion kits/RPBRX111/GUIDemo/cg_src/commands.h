/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software.  By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : commands.h
* Version      : 1.0
* Device(s)    : R5F51115AxFM
* Tool-Chain   : CCRX
* Description  : Header file for command processing modules.
* Creation Date: 25/07/2013
***********************************************************************************************************************/
#ifndef COMMANDS_H
#define COMMANDS_H

#define CMD_DISCONNECT      0x10
#define CMD_GET_TIME        0x21
#define CMD_SET_TIME        0x22
#define CMD_GET_VERSION     0x30
#define CMD_SET_OP_MODE     0x54
#define CMD_GET_USB_STATE   0x80

#define CR                    13
#define LF                    10

/* Operating mode parameters */
//#define OPMODE_HIGH_SPEED   0x31
//#define OPMODE_MED_SPEED    0x32
//#define OPMODE_LOW_SPEED    0x33
#define OPMODE_HIGH_SPEED   1
#define OPMODE_MED_SPEED    2
#define OPMODE_LOW_SPEED    3


void cmd_process(void);

#endif
