/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name	   : cmd_parser.h
* Device(s)    : RX
* H/W Platform : RSKRX111
* Description  : Provides variable and function declarations for cmd_parser.c file
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 02.12.2013 0.01     Beta Release
***********************************************************************************************************************/
#ifndef CMD_PARSER_H_
#define CMD_PARSER_H_

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/******************************************************************************
Data structure for host command handlers
******************************************************************************/
typedef struct
{
   char *command ;
   bool (*commandHandler) (void) ;
   char *format;
} HOST_COMMAND ;


/*
    System Clock Control Register 3 (SCKCR3)
*/
#define CGC_CLOCKSOURCE_LOCO          (0x0000U) /* LOCO */
#define CGC_CLOCKSOURCE_HOCO          (0x0100U) /* HOCO */
#define CGC_CLOCKSOURCE_MAINCLK       (0x0200U) /* Main clock oscillator */
#define CGC_CLOCKSOURCE_SUBCLK        (0x0300U) /* Sub-clock oscillator */
#define CGC_CLOCKSOURCE_PLL           (0x0400U) /* PLL circuit */

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/


/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/
bool processCommand(char *rxbuffer);
bool setPowermode(char mode);
uint32_t getTimeinSecs();
uint8_t ClockDivToBits(uint8_t clockDiv);



/* End of multiple inclusion prevention macro */
#endif /* CMD_PARSER_H_ */
